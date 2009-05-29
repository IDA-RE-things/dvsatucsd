// RosterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "RosterDlg.h"
#include "AddPropertyDlg.h"
#include "YESNODlg.h"
#include "Roster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RosterDlg dialog


RosterDlg::RosterDlg(CWnd* pParent /*=NULL*/,Roster *iroster): CDialog(RosterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RosterDlg)
	m_rosterlabeltext = _T("");
	m_CDefaultValueText = _T("");
	//}}AFX_DATA_INIT

	roster = iroster;

	// Clone student property list before changes are made
	OriginalList = roster->ClonePropList();
}


void RosterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RosterDlg)
	DDX_Control(pDX, IDC_CDefaultValue, m_CDefaultValue);
	DDX_Control(pDX, IDC_TERosterLabel, m_rosterlabel);
	DDX_Control(pDX, IDC_PropList, m_proplist);
	DDX_Text(pDX, IDC_TERosterLabel, m_rosterlabeltext);
	DDX_CBString(pDX, IDC_CDefaultValue, m_CDefaultValueText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RosterDlg, CDialog)
	//{{AFX_MSG_MAP(RosterDlg)
	ON_BN_CLICKED(IDC_BAddProperty, OnBAddProperty)
	ON_BN_CLICKED(IDC_BRemoveProperty, OnBRemoveProperty)
	ON_LBN_SELCHANGE(IDC_PropList, OnSelchangePropList)
	ON_BN_CLICKED(IDC_Next, OnNext)
	ON_CBN_EDITCHANGE(IDC_CDefaultValue, OnEditchangeCDefaultValue)
	ON_BN_CLICKED(IDC_BEditProperty, OnBEditProperty)
	ON_BN_CLICKED(IDC_Apply, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RosterDlg message handlers

BOOL RosterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	curselection = -1;
	prevtext = "-This string will never be typed.";
	/// Clone student property list before changes are made
	// Populate roster properties
	m_rosterlabel.SetWindowText(roster->GetLabel());

	//Populate student property list
	RefreshPropList();

	//highlight on roster label input box so user can type immediately
	m_rosterlabel.SetFocus();
	m_rosterlabel.SetSel(0,-1,FALSE);
	CDialog::SetDefID(IDOK);

	// return TRUE unless you set the focus to a control
	// returning FALSE so that roster label highlighted
	return FALSE;  
}

void RosterDlg::OnBAddProperty() 
{
	//Create the add propery dialog box
	CString newpropname;
	AddPropertyDlg apdlg(NULL,&newpropname);
	apdlg.DoModal();	

	UpdateData();

	if (newpropname == "")
	{
		MessageBox("A property must have a name to be created.");
		return;
	}

	//Alert the user about protected property types
	if (newpropname == "Name")
	{
		MessageBox("The \"Name\" property is used as a unique student ID and cannot be duplicated.");
		return;
	}
	if (newpropname == "Photo Timestamp")
	{
		MessageBox("The \"Photo Timestamp\" property is used as a unique identifier and cannot be duplicated.");
		return;
	}

	//Add Property to roster
	roster->AddProperty(newpropname, "N\\A");

	//Update the roster listbox
	RefreshPropList();

	UpdateData(FALSE);

	//Refresh the property list
	RefreshPropList();
}

void RosterDlg::OnBEditProperty() 
{
	//Make sure a propery is selected
	int selindex = m_proplist.GetCurSel();
	if (selindex == -1)	
	{
		MessageBox("A property must be selected for editing.");
		return;
	}

	//Extract the property's name
	CString propname;
	m_proplist.GetText(m_proplist.GetCurSel(), propname);
	propname = propname.Left(propname.Find(':'));

	//Alert the user about protected property types
	if (propname == "Name")
	{
		MessageBox("The \"Name\" property is used as a unique student ID and cannot be edited.");
		return;
	}
	if (propname == "Photo Timestamp")
	{
		MessageBox("The \"Photo Timestamp\" property is used as a unique identifier and cannot be edited.");
		return;
	}

	AddPropertyDlg apdlg(NULL,&propname);
	apdlg.DoModal();

	UpdateData();

	if (propname == "")
	{
		MessageBox("A property must have a name to exist.");
		return;
	}

	//Edit Property of roster
	roster->EditProperty(selindex, propname);

	//Update the roster listbox
	RefreshPropList();

	UpdateData(FALSE);

	//Refresh the property list
	RefreshPropList();
}

void RosterDlg::OnBRemoveProperty() 
{
	//Make sure a propery is selected
	int selindex = m_proplist.GetCurSel();
	if (selindex == -1)	
	{
		MessageBox("A property must be selected for removal.");
		return;
	}

	//Extract the property's name
	CString propname;
	m_proplist.GetText(m_proplist.GetCurSel(), propname);
	propname = propname.Left(propname.Find(':'));

	//Alert the user about protected property types
	if (propname == "Name")
	{
		MessageBox("The \"Name\" property is used as a unique student ID and cannot be removed.");
		return;
	}
	if (propname == "Photo Timestamp")
	{
		MessageBox("The \"Photo Timestamp\" property is used as a unique identifier and cannot be removed.");
		return;
	}

	//Confirm the removal of the property
	bool result;
	YESNODlg yesno(NULL, "Are you sure you wish to remove the student property " + propname + "?  If this property is removed from the roster, it will also be removed from all students on the roster.", &result);
	yesno.DoModal();

	if (result==FALSE) return;

	//Remove the property from the roster
	roster->RemoveProperty(selindex);

	RefreshPropList();

	//Display the next selection
	OnNext();
}

void RosterDlg::OnOK() 
{
	//Update the roster data before closing

	UpdateData();
	if (m_rosterlabeltext==("Unnamed Roster"))
	{
		MessageBox("Please enter a roster label.");

		//highlight on roster label input box so user can type immediately
		m_rosterlabel.SetFocus();
		m_rosterlabel.SetSel(0,-1,FALSE);

		return;
	}
	
	roster->SetLabel(m_rosterlabeltext);
	UpdateData(FALSE);

	UpdateData();
	if (curselection>=0 && curselection < roster->NumProperties()) {
		roster->SetPropertyDefault(curselection, m_CDefaultValueText);
	}
	UpdateData(FALSE);
	RefreshPropList();

	CDialog::OnOK();
}

void RosterDlg::OnSelchangePropList() 
{
	if (curselection>=0 && curselection < roster->NumProperties()) {
		//Change the student properties as you change selection.
		UpdateData();
		if (curselection!=-1) roster->SetPropertyDefault(curselection, m_CDefaultValueText);
		UpdateData(FALSE);
	}

	curselection = m_proplist.GetCurSel();

	//Display the current selection
	CString valuetext = roster->GetPropertyDefault(curselection);
	m_CDefaultValue.SetWindowText(valuetext);

	//Refresh default value list
	RefreshPropList();
	RefreshComboList();

	//Set the focus on the default text
	m_CDefaultValue.SetFocus();
}

void RosterDlg::RefreshPropList()
{
	//Store the current selection index before clearing the list
	curselection = m_proplist.GetCurSel();

	UpdateData();
	
	m_proplist.ResetContent();
	
	//Find the longest string in the list
	int maxlength = 0;
	for (int a=0; a<roster->NumProperties(); a++)
		if (roster->GetPropertyName(a).GetLength()>maxlength) maxlength = roster->GetPropertyName(a).GetLength();
	
	for (a=0; a<roster->NumProperties(); a++)
	{
		//Use the longest string in the list to add tabs to the list for even columns
		CString tabs = "";
		for (int b=0; b<(maxlength/8-roster->GetPropertyName(a).GetLength()/8)+1; b++)
			tabs += "\t";
		m_proplist.AddString(roster->GetPropertyName(a) + ": " + tabs + roster->GetPropertyDefault(a));
	}

	UpdateData(FALSE);

	//If it is still available, set the selection where it was
	if (curselection>=0 && curselection < roster->NumProperties()) m_proplist.SetCurSel(curselection);
}

void RosterDlg::OnNext() 
{
	if (curselection>=0 && curselection < roster->NumProperties()) {
		//Update the database
		UpdateData();
		if (roster->GetPropertyName(curselection)!="Photo Timestamp") roster->SetPropertyDefault(curselection, m_CDefaultValueText);
		UpdateData(FALSE);

		//Update the list
		RefreshPropList();
	}

	//Advance the selection
	if (curselection<m_proplist.GetCount()-1) 
	{
		curselection++;
		m_proplist.SetCurSel(curselection);
	}

	//Display the current selection
	CString valuetext = roster->GetPropertyDefault(curselection);
	m_CDefaultValue.SetWindowText(valuetext);

	RefreshComboList();

	//Set the focus on the property text
	m_CDefaultValue.SetFocus();
}

void RosterDlg::OnApply() 
{
	//Make sure a propery is selected
	int selindex = m_proplist.GetCurSel();
	if (selindex == -1)	
	{
		MessageBox("A property must be selected for overriding.");
		return;
	}

	UpdateData();

	//Extract the property's name
	CString propname;
	CString propvalue = m_CDefaultValueText;
	m_proplist.GetText(m_proplist.GetCurSel(), propname);
	propname = propname.Left(propname.Find(':'));

	//Alert the user about protected property types
	if (propname == "Name")
	{
		MessageBox("The \"Name\" property is used as a unique student ID and cannot be overriden.");
		return;
	}
	if (propname == "Photo Timestamp")
	{
		MessageBox("The \"Photo Timestamp\" property is used as a unique identifier and cannot be overriden.");
		return;
	}

	//Confirm the removal of the property
	bool result;
	YESNODlg yesno(NULL, "Are you sure you wish to override the " + propname + " of all students with the value " + propvalue + "?  You cannot undo this action.", &result);
	yesno.DoModal();

	if (result==FALSE) return;

	//Set default value to text box value
	roster->SetPropertyDefault(curselection, propvalue);
	UpdateData(FALSE);

	//Override the property immediately
	roster->SetPropertyOverride(selindex, propvalue);
	
	RefreshPropList();
}

void RosterDlg::RefreshComboList()
{
	//Clears and repopulates the combolistbox.

	UpdateData();
	m_CDefaultValue.ResetContent();

	int numass = roster->GetNumPropertyAssociations(curselection);
	
	if (numass > 0)
		for (int a=0;a<numass;a++)
			m_CDefaultValue.AddString(roster->GetPropertyAssociation(curselection,a));


	UpdateData(FALSE);
}

void RosterDlg::OnEditchangeCDefaultValue() 
{
	if (curselection == -1) return;

	int numass = roster->GetNumPropertyAssociations(curselection);
	
	UpdateData();
	CString curstr = m_CDefaultValueText;
	UpdateData(FALSE);

	int curlen = curstr.GetLength();

	if (curstr.CompareNoCase(prevtext)==0 && curlen>0) 
	{
		//MessageBox("1 prev:[" + prevtext+"] cur:["+curstr+"]");
		prevtext = curstr.Left(curlen-1);
		m_CDefaultValue.SetWindowText(curstr.Left(curlen-1));
	}
	else if (prevtext == "-This string will never be typed.") prevtext = curstr;
		else prevtext = curstr.Left(curlen);

	
	if (curlen>0)
	{
		for (int a=0;a<numass;a++)
			if (roster->GetPropertyAssociation(curselection,a).Find(m_CDefaultValueText) == 0 && roster->GetPropertyAssociation(curselection,a)!=m_CDefaultValueText)
			{
				m_CDefaultValueText = roster->GetPropertyAssociation(curselection,a);
				break;
			}
	}

	//MessageBox("2 prev:[" + prevtext+"] cur:["+curstr+"]");

	m_CDefaultValue.SetEditSel(curlen,-1);
}

void RosterDlg::OnCancel() 
{
	// Override default property list w/ original
	roster->OverridePropList(OriginalList);

	// RefreshList w/ original default property list

	
	CDialog::OnCancel();
}
