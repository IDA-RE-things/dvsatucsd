#if !defined(AFX_CPICTUREDLG_H__8D4F73EF_DD93_4E0B_87BF_2C8CF1145958__INCLUDED_)
#define AFX_CPICTUREDLG_H__8D4F73EF_DD93_4E0B_87BF_2C8CF1145958__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPictureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg dialog

#include "Student.h"

class CPictureDlg : public CDialog
{
// Construction
public:
	CPictureDlg(CWnd* pParent = NULL, Student *student = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPictureDlg)
	enum { IDD = IDD_Picture };
	CStatic	m_studentlabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	Student * my_student;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPictureDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPICTUREDLG_H__8D4F73EF_DD93_4E0B_87BF_2C8CF1145958__INCLUDED_)
