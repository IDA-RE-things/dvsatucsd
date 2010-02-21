// StartScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "StartScreenDlg.h"
#include "DVTDlg.h"


#include "PictureWizardDlg.h"
#include "CameraConnect.h"

#include "YESNODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CONNECTERROR "Camera Error!  Please verify that the camera is turned on or that it is plugged in."

/////////////////////////////////////////////////////////////////////////////
// StartScreenDlg dialog


StartScreenDlg::StartScreenDlg(CWnd* pParent /*=NULL*/, CString *tresult)
	: CDialog(StartScreenDlg::IDD, pParent)
{
	result = tresult;
	//{{AFX_DATA_INIT(StartScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void StartScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StartScreenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StartScreenDlg, CDialog)
	//{{AFX_MSG_MAP(StartScreenDlg)
	ON_BN_CLICKED(IDC_BNewRoster, OnBNewRoster)
	ON_BN_CLICKED(IDC_BOpenRoster, OnBOpenRoster)
	//ON_BN_CLICKED(IDOK3, OnQuick) NO longer used
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StartScreenDlg message handlers

void StartScreenDlg::OnBNewRoster() 
{
	*result = "New";
	CDialog::OnOK();
}

void StartScreenDlg::OnBOpenRoster() 
{
	*result = "Open";
	CDialog::OnOK();
}

BOOL StartScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void StartScreenDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

/*** NO longer used
void StartScreenDlg::OnQuick() 
{
	// TODO: Add your control notification handler code here
	//Make sure a student is selected

	//Extract the student's name	
	//CString stuname = "Unnamed";
	//m_studentlist.GetText(m_studentlist.GetCurSel(), stuname);
	//if (stuname.Find('\t')!=-1) stuname = stuname.Left(stuname.Find('\t'));
	
	//Use the student's name to get the student and pass it to the picture wizard
	CCameraConnect *cameraconnect = new CCameraConnect();
	theCamera = (void*)cameraconnect;
	
	Student* unNamed = new Student;
	(*unNamed).SetPropertyValue("Name", "Unnamed");
//@alan temp disable camera power on
	CPictureWizardDlg dlgPicture(NULL, unNamed, theCamera, ".");
	if(true||dlgPicture.OnCamera())
	{	
		dlgPicture.DoModal();
	}		
	else
	{
		MessageBox(CONNECTERROR);
	}
	//if( !roster.TakePictures(roster.GetStudent(stuname), theCamera) )
	//	MessageBox(CONNECTERROR);

	//CDialog::OnOK();	// This line will close the StartScreenDlg, but will
						// then automatically launch the RosterDlg, which we
						// don't want.
}
*/


//on pressing X, exits from program completely
void StartScreenDlg::OnClose() 
{
	CDialog::OnOK();
	CDialog::OnClose();
	exit(0);
}
