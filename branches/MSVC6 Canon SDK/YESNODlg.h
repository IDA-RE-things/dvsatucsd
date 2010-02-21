#if !defined(AFX_YESNODLG_H__B3D9A30A_CF00_4D53_A5FF_2DF46BF90517__INCLUDED_)
#define AFX_YESNODLG_H__B3D9A30A_CF00_4D53_A5FF_2DF46BF90517__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YESNODlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// YESNODlg dialog

class YESNODlg : public CDialog
{
// Construction
public:
	YESNODlg(CWnd* pParent = NULL, CString tmessage = "", bool *tresult = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(YESNODlg)
	enum { IDD = IDD_YESNO };
	CEdit	m_Message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(YESNODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(YESNODlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool *result;
	CString message;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YESNODLG_H__B3D9A30A_CF00_4D53_A5FF_2DF46BF90517__INCLUDED_)
