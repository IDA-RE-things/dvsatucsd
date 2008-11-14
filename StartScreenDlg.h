#if !defined(AFX_STARTSCREENDLG_H__0BCB2798_1FB5_4A11_A27B_F4EE1286DC1D__INCLUDED_)
#define AFX_STARTSCREENDLG_H__0BCB2798_1FB5_4A11_A27B_F4EE1286DC1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartScreenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StartScreenDlg dialog

class StartScreenDlg : public CDialog
{
// Construction
public:
	StartScreenDlg(CWnd* pParent = NULL, CString *tresult = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(StartScreenDlg)
	enum { IDD = IDD_StartScreen };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StartScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(StartScreenDlg)
	afx_msg void OnBNewRoster();
	afx_msg void OnBOpenRoster();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//afx_msg void OnQuick();  No longer used
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString *result;
	void* theCamera;	// object that represents camera
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTSCREENDLG_H__0BCB2798_1FB5_4A11_A27B_F4EE1286DC1D__INCLUDED_)
