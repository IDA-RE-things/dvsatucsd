#if !defined(AFX_ROSTERDLG_H__9AB6C91E_A5E4_4525_82C2_0F6282F44758__INCLUDED_)
#define AFX_ROSTERDLG_H__9AB6C91E_A5E4_4525_82C2_0F6282F44758__INCLUDED_

#include "Roster.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RosterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RosterDlg dialog

class RosterDlg : public CDialog
{
// Construction
public:
	RosterDlg(CWnd* pParent = NULL,Roster *roster = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RosterDlg)
	enum { IDD = IDD_RosterSettings };
	CComboBox	m_CDefaultValue;
	CEdit	m_rosterlabel;
	CListBox	m_proplist;
	CString	m_rosterlabeltext;
	CString	m_CDefaultValueText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RosterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	private:
		Roster *roster;
		int curselection;
		CString prevtext;
		void RosterDlg::RefreshPropList();
		void RosterDlg::RefreshComboList();
protected:

	// Generated message map functions
	//{{AFX_MSG(RosterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBAddProperty();
	afx_msg void OnBRemoveProperty();
	virtual void OnOK();
	afx_msg void OnSelchangePropList();
	afx_msg void OnNext();
	afx_msg void OnEditchangeCDefaultValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROSTERDLG_H__9AB6C91E_A5E4_4525_82C2_0F6282F44758__INCLUDED_)
