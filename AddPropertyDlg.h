#if !defined(AFX_ADDPROPERTYDLG_H__D0BC4F83_99FB_4F72_89B7_4D3CB0E3370D__INCLUDED_)
#define AFX_ADDPROPERTYDLG_H__D0BC4F83_99FB_4F72_89B7_4D3CB0E3370D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddPropertyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AddPropertyDlg dialog

class AddPropertyDlg : public CDialog
{
// Construction
public:
	AddPropertyDlg(CWnd* pParent = NULL, CString *tpropname = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AddPropertyDlg)
	enum { IDD = IDD_AddProperty };
	CEdit	m_TEPropName;
	CString	m_TEPropNameText;
	CString	m_TEPropAssociationsText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AddPropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AddPropertyDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString *propname;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDPROPERTYDLG_H__D0BC4F83_99FB_4F72_89B7_4D3CB0E3370D__INCLUDED_)
