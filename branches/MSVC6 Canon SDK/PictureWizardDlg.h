#if !defined(AFX_PICTUREWIZARDDLG_H__4C03A352_B59D_4E0F_94DE_8743402F093C__INCLUDED_)
#define AFX_PICTUREWIZARDDLG_H__4C03A352_B59D_4E0F_94DE_8743402F093C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PictureWizardDlg.h : header file
//

#include "Roster.h"
#include "Student.h"
#include "resource.h"
#include "PictureEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPictureWizardDlg dialog

class CPictureWizardDlg : public CDialog
{
// Construction
public:
	BOOL OnCamera();
	CPictureWizardDlg(CWnd* pParent, Roster *roster, Student *student, void* theCamera, CString curpath);   // standard constructor
	void GetSavePath(	char	*cpBuffer, int		iBufferSize );

	BOOL horizontal;
	
// Dialog Data
	//{{AFX_DATA(CPictureWizardDlg)
	enum { IDD = IDD_PictureWizard };
	CPictureEx	m_jpgLeft;
	CPictureEx	m_jpgRight;
	CStatic	m_ViewFinder;
	CStatic	m_StudentLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPictureWizardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHorizontal();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnAnalyze();
	virtual void OnCancel();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVertical();
	afx_msg void OnEyeDx();
	afx_msg void LoadPictureFile(LPCTSTR szFile);
	afx_msg void WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow);
	afx_msg void OnVSelect();
	afx_msg void OnHSelect();
	afx_msg byte * genKeystrokes (CString name, int & length, bool genExtraKeystroke);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Roster * my_roster;
	Student * my_student;
	void* my_theCamera;
	CString cur_path;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREWIZARDDLG_H__4C03A352_B59D_4E0F_94DE_8743402F093C__INCLUDED_)
