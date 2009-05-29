// StudentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "StudentDlg.h"
#include "Student.h"
#include "Roster.h"
#include "YESNODlg.h"
#include <vector>
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StudentDlg dialog


StudentDlg::StudentDlg(CWnd* pParent, Student *tstudent, bool *taddanother, std::vector<StudentProperty> *tproperty)
	: CDialog(StudentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(StudentDlg)
	m_CPropertyValueText = _T("");
	//}}AFX_DATA_INIT

	student = tstudent;
	addanother = taddanother;
	property = tproperty;

	// Clone student property list before changes are made
	OriginalList = student->ClonePropList();
}


void StudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StudentDlg)
	DDX_Control(pDX, IDC_CPropertyValue, m_CPropertyValue);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDC_PropList, m_proplist);
	DDX_CBString(pDX, IDC_CPropertyValue, m_CPropertyValueText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StudentDlg, CDialog)
	//{{AFX_MSG_MAP(StudentDlg)
	ON_LBN_SELCHANGE(IDC_PropList, OnSelchangePropList)
	ON_BN_CLICKED(IDC_Next, OnNext)
	ON_CBN_EDITCHANGE(IDC_CPropertyValue, OnEditchangeCPropertyValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StudentDlg message handlers

BOOL StudentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Populate student property list
	RefreshList();
	
	curselection = 0;
	m_proplist.SetCurSel(curselection);
	prevtext = "-This string will never be typed.";
	
	//Display the new property's value.
	CString valuetext = student->GetPropertyValue(curselection);
	
	m_CPropertyValue.SetWindowText(valuetext);

	//Set the focus on the property text; not focusing - why?
	m_CPropertyValue.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void StudentDlg::OnSelchangePropList() 
{
	//Change the student properties as you change selection.
	UpdateData();
	if (curselection!=-1) student->SetPropertyValue(curselection, m_CPropertyValueText);
	UpdateData(FALSE);

	//Display the new property's value.
	curselection = m_proplist.GetCurSel();
	CString valuetext = student->GetPropertyValue(curselection);
	m_CPropertyValue.SetWindowText(valuetext);

	//Update the list
	RefreshList();

	//Set the focus on the property text
	m_CPropertyValue.SetFocus();
}

void StudentDlg::OnOK() 
{
	//Update the currently changed data
	UpdateData();
	if (curselection!=-1) student->SetPropertyValue(curselection, m_CPropertyValueText);
	UpdateData(FALSE);

	if (student->GetPropertyValue("Name")=="" || 
		student->GetPropertyValue("Name").FindOneOf("\\") >= 0 )
	{
		MessageBox("The student must have a name to be created.");
		curselection = 0;
		m_proplist.SetCurSel(curselection);
		CString valuetext = student->GetPropertyValue(curselection);
		m_CPropertyValue.SetWindowText(valuetext);
		//Set the focus on the property text
		m_CPropertyValue.SetFocus();
		return;
	}

	//Ask if the user would like to add another student
	if (addanother != NULL) 
	{
		YESNODlg yesno(NULL, "Do you wish to add another student at this time?", addanother);
		yesno.DoModal();
	}
	CDialog::OnOK();
}

void StudentDlg::RefreshList()
{
	//Store the current selection before clearing the list
	curselection = m_proplist.GetCurSel();
	
	UpdateData();

	m_proplist.ResetContent();

	//Find the longest string in the list
	int maxlength = 0;
	for (int a=0; a<student->NumProperties(); a++)
		if (student->GetPropertyName(a).GetLength()>maxlength) maxlength = student->GetPropertyName(a).GetLength();

	for (a=0; a< student->NumProperties(); a++)
	{
		//Use the longest string in the list to add tabs to the list for even columns
		CString tabs = "";
		for (int b=0; b<(maxlength/8-student->GetPropertyName(a).GetLength()/8)+1; b++)
			tabs += "\t";
		m_proplist.AddString(student->GetPropertyName(a) + ": " + tabs + student->GetPropertyValue(a));
	}

	//If it is still available, set the selection where it was
	m_proplist.SetCurSel(curselection);

	UpdateData(FALSE);
	
	//Update the combobox
	RefreshComboList();
}

void StudentDlg::OnNext() 
{
	UpdateData();
	if (curselection!=-1 && student->GetPropertyName(curselection)!="Photo Timestamp")
		student->SetPropertyValue(curselection, m_CPropertyValueText);
	UpdateData(FALSE);

	//Update the list
	RefreshList();

	//Advance the selection
	if (curselection!=-1 && curselection<m_proplist.GetCount()-1) 
	{
		curselection++;
		m_proplist.SetCurSel(curselection);
	}

	RefreshComboList();

	//Display the new property's value.
	curselection = m_proplist.GetCurSel();

	CString valuetext = student->GetPropertyValue(curselection);
	
	m_CPropertyValue.SetWindowText(valuetext);

	//Set the focus on the property text
	m_CPropertyValue.SetFocus();
}

void StudentDlg::RefreshComboList()
{
	//Clears and repopulates the combolistbox.
	UpdateData();
	m_CPropertyValue.ResetContent();
	if (curselection > -1 )
	{
		int numa = (*property)[curselection].association.size();
		
		if (numa > 0)
			for (int a=0;a<numa;a++)
				m_CPropertyValue.AddString((*property)[curselection].association[a]);

	}

	
	/*char output[100];
		sprintf(output, "curselection %d, propertysize %d", curselection, property->size());
		MessageBox(output);*/

	UpdateData(FALSE);
}
	

void StudentDlg::OnEditchangeCPropertyValue() 
{
	if (curselection == -1) return;

	int numass = (*property)[curselection].association.size();
	
	UpdateData();
	CString curstr = m_CPropertyValueText;
	UpdateData(FALSE);

	int curlen = curstr.GetLength();

	if (curstr.CompareNoCase(prevtext)==0 && curlen>0) 
	{
		//MessageBox("1 prev:[" + prevtext+"] cur:["+curstr+"]");
		prevtext = curstr.Left(curlen-1);
		m_CPropertyValue.SetWindowText(curstr.Left(curlen-1));
	}
	else if (prevtext == "-This string will never be typed.") prevtext = curstr;
		else prevtext = curstr.Left(curlen);

	
	if (curlen>0)
	{
		for (int a=0;a<numass;a++)
			if ((*property)[curselection].association[a].Find(m_CPropertyValueText) == 0 && (*property)[curselection].association[a]!=m_CPropertyValueText)
			{
				m_CPropertyValueText = (*property)[curselection].association[a];
				break;
			}
	}

	//MessageBox("2 prev:[" + prevtext+"] cur:["+curstr+"]");

	m_CPropertyValue.SetEditSel(curlen,-1);
}

void StudentDlg::OnCancel() 
{
	/// Override student property list w/ original
	student->OverridePropList(OriginalList);

	CDialog::OnCancel();
}
