#if !defined(AFX_PICTUREDLG_H__58D16792_1573_42FD_BC8C_3B565BA2AD27__INCLUDED_)
#define AFX_PICTUREDLG_H__58D16792_1573_42FD_BC8C_3B565BA2AD27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PictureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PictureDlg dialog

class PictureDlg : public CDialog
{
// Construction
public:
	PictureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPictureDlg)
	enum { IDD = IDD_Picture };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPictureDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREDLG_H__58D16792_1573_42FD_BC8C_3B565BA2AD27__INCLUDED_)
