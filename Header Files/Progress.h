#if !defined(AFX_PROGRESS1_H__675200AD_A2D2_4354_862E_43EEEA23C9A4__INCLUDED_)
#define AFX_PROGRESS1_H__675200AD_A2D2_4354_862E_43EEEA23C9A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Progress.h : header file
//
#include "cdAPI.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CProgress dialog

class CProgress : public CDialog
{
// Construction
public:
	CProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_CProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void* my_theCamera;
	static cdUInt32 cdSTDCALL ReleaseProgressFunc(	cdUInt32			Progress,
													cdProgressStatus	Status,
													cdContext			Context );
	static UINT 	ReleaseThreadProc( LPVOID	lpParameter );
	
public:
	UINT			m_ThreadEndMessage;
	cdHSource		m_hSource;
	char			m_szSavePath[MAX_PATH];
	char			*FilePath;
	cdError			m_LastErr;
	cdUInt32		m_NumData;
	
	void			SetProgressPos( int	iPos );
	BOOL			GetReleaseData( cdHSource	hSource,
									cdUInt32	NumData,
									char		*szSavePath,
									void        *theCamera);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESS1_H__675200AD_A2D2_4354_862E_43EEEA23C9A4__INCLUDED_)
