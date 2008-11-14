// CPictureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "CPictureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg dialog


CPictureDlg::CPictureDlg(CWnd* pParent /*=NULL*/, Student *student)
	: CDialog(CPictureDlg::IDD, pParent)
{
	my_student = student;
	//{{AFX_DATA_INIT(CPictureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPictureDlg)
	DDX_Control(pDX, IDC_StudentName, m_studentlabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPictureDlg, CDialog)
	//{{AFX_MSG_MAP(CPictureDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg message handlers

BOOL CPictureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CFont *m_NewFont = new CFont;
	m_NewFont->CreatePointFont(180, "Times New Roman");
	m_studentlabel.SetFont(m_NewFont);
	m_studentlabel.SetWindowText(my_student->GetPropertyValue("Name"));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
