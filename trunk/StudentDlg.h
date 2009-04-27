#if !defined(AFX_STUDENTDLG_H__EBE0B08E_F1F7_464D_9108_C79E47E8DBD5__INCLUDED_)
#define AFX_STUDENTDLG_H__EBE0B08E_F1F7_464D_9108_C79E47E8DBD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StudentDlg.h : header file
//

#include "Student.h"
#include "Roster.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// StudentDlg dialog

class StudentDlg : public CDialog
{
// Construction
public:
	StudentDlg(CWnd* pParent, Student *tstudent, bool *taddanother, std::vector<StudentProperty> *tproperty);   // standard constructor

// Dialog Data
	//{{AFX_DATA(StudentDlg)
	enum { IDD = IDD_StudentDlg };
	CComboBox	m_CPropertyValue;
	CButton	m_BOK;
	CListBox	m_proplist;
	CString	m_CPropertyValueText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StudentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	private:
		void StudentDlg::RefreshList();
		void StudentDlg::RefreshComboList();

		Student *student;
		int curselection;

		CString prevtext;
		bool *addanother;
		std::vector<StudentProperty> *property;
		std::vector<StudentProperty> OriginalList;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(StudentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePropList();
	virtual void OnOK();
	afx_msg void OnNext();
	afx_msg void OnEditchangeCPropertyValue();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STUDENTDLG_H__EBE0B08E_F1F7_464D_9108_C79E47E8DBD5__INCLUDED_)
