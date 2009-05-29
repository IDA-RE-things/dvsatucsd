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



/**
 * Function: AddPropertyDlg(CWnd* pParent, CString *tnewpropname)
 * Parameters: pParent - NULL
 *             tnewpropname - name of new property   
 * Returns: none
 * Description: Functions for IDD_AddProperty which is used to add
 *              properties to a roster
 */
AddPropertyDlg::AddPropertyDlg(CWnd* pParent /*=NULL*/, CString *tpropname)
	: CDialog(AddPropertyDlg::IDD, pParent)
{
	propname = tpropname;

	//{{AFX_DATA_INIT(AddPropertyDlg)
	m_TEPropNameText = _T("");
	m_TEPropAssociationsText = _T("");
	//}}AFX_DATA_INIT
}

/**
 * Function: AddPropertyDlg::DoDataExchange(CDataExchange* pDX)
 * Parameters: pDX - FILL IN
 * Returns: none
 * Description: FILL IN
 */
void AddPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddPropertyDlg)
	DDX_Control(pDX, IDC_TEPropName, m_TEPropName);
	DDX_Text(pDX, IDC_TEPropName, m_TEPropNameText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AddPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(AddPropertyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddPropertyDlg message handlers

/**
 * Function: AddPropertyDlg::OnInitDialog() 
 * Parameters: none
 * Returns: none
 * Description: Sets value of text box to name of property being edited
 */
BOOL AddPropertyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_TEPropName.SetWindowText(*propname);

	return TRUE;
}

/**
 * Function: AddPropertyDlg::OnOK() 
 * Parameters: none
 * Returns: none
 * Description: Updates properties of a roster to include new property added
 */
void AddPropertyDlg::OnOK() 
{
	UpdateData();
	*propname = m_TEPropNameText;
	UpdateData(FALSE);

	CDialog::OnOK();
}
