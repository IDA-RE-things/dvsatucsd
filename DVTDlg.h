// DVTDlg.h : header file
//

#if !defined(AFX_DVTDLG_H__35186E6D_F1FF_4967_A93C_14549A71B7D5__INCLUDED_)
#define AFX_DVTDLG_H__35186E6D_F1FF_4967_A93C_14549A71B7D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DVT.h"
#include "DVTDlg.h"
#include "cdType.h"
#include "cdAPI.h"
#include "Roster.h"

#define CONNECTERROR "Camera Error!  Please verify that the camera is turned on or that it is plugged in."
#define SELECTERROR "You must have a student selected to take pictures."
#define CREATEERROR "Error Creating a Student."
#define SELECTEDITERROR "You must have a student selected to edit."
#define SELECTREMOVEERROR "You must have a student selected to remove."

/////////////////////////////////////////////////////////////////////////////
// CDVTDlg dialog

class CDVTDlg : public CDialog
{
// Construction
public:
	CDVTDlg(CWnd* pParent = NULL);	// standard constructor

	CStatic * RosterName;

// Dialog Data
	//{{AFX_DATA(CDVTDlg)
	enum { IDD = IDD_DVT_DIALOG };
	CListBox	m_studentview;
	CListBox	m_studentlist;
	BOOL		m_SortResultChecked;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	static cdUInt32 cdSTDCALL CamCallBackFunc(
		cdEventID EventID,
		const cdVoid * pData,
		cdUInt32 DataSize,
		cdContext Context
	);

	// Generated message map functions
	//{{AFX_MSG(CDVTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUpdateFileRostersettings(CCmdUI* pCmdUI);
	afx_msg void OnBAddStudent();
	afx_msg void OnBEditStudent();
	afx_msg void OnFileSaveroster();
	afx_msg void OnFileSaverosteras();
	afx_msg void OnInitOpenroster();
	afx_msg void OnFileOpenroster();
	afx_msg void OnBRemoveStudent();
	afx_msg void OnSelchangeStudentList();
	afx_msg void OnFileExit();
	afx_msg void OnFileNewroster();
	afx_msg void OnBTakePictures();
	afx_msg void OnSortResult();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	Roster roster;
	CString cur_path;
	void CDVTDlg::RefreshPropertyList();
	void CDVTDlg::RefreshStudentList();
	int curselection;
	void* theCamera;	// object that represents camera

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVTDLG_H__35186E6D_F1FF_4967_A93C_14549A71B7D5__INCLUDED_)
