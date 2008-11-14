// AddPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "AddPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AddPropertyDlg dialog


AddPropertyDlg::AddPropertyDlg(CWnd* pParent /*=NULL*/, CString *tnewpropname)
	: CDialog(AddPropertyDlg::IDD, pParent)
{
	newpropname = tnewpropname;

	//{{AFX_DATA_INIT(AddPropertyDlg)
	m_TENewPropNameText = _T("");
	//}}AFX_DATA_INIT
}


void AddPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddPropertyDlg)
	DDX_Text(pDX, IDC_TENewPropName, m_TENewPropNameText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AddPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(AddPropertyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddPropertyDlg message handlers

void AddPropertyDlg::OnOK() 
{
	UpdateData();
	*newpropname = m_TENewPropNameText;
	UpdateData(FALSE);

	CDialog::OnOK();
}
