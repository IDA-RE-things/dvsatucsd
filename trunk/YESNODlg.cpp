// YESNODlg.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "YESNODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// YESNODlg dialog


YESNODlg::YESNODlg(CWnd* pParent /*=NULL*/, CString tmessage, bool *tresult)
	: CDialog(YESNODlg::IDD, pParent)
{
	message = tmessage;
	result = tresult;
	*result = FALSE;
	//{{AFX_DATA_INIT(YESNODlg)
	//}}AFX_DATA_INIT
}


void YESNODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(YESNODlg)
	DDX_Control(pDX, IDC_Message, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(YESNODlg, CDialog)
	//{{AFX_MSG_MAP(YESNODlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// YESNODlg message handlers

void YESNODlg::OnOK() 
{
	*result = TRUE;
	
	CDialog::OnOK();
}

void YESNODlg::OnCancel() 
{
	*result = FALSE;
	
	CDialog::OnCancel();
}

BOOL YESNODlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Message.SetWindowText(message);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
