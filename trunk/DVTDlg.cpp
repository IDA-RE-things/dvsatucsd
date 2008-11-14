// DVTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "DVTDlg.h"
#include "cdType.h"
#include "cdAPI.h"
#include "Student.h"
#include "RosterDlg.h"
#include "StudentDlg.h"
#include "StartScreenDlg.h"
#include "YESNODlg.h"

//CameraConnect Includes
#include "CameraConnect.h"

#include <direct.h> 
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVTDlg dialog

CDVTDlg::CDVTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDVTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDVTDlg)
	m_SortResultChecked = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	char tempstr[32768];
	getcwd(tempstr, 32768);
	cur_path = tempstr;

	Roster newroster(cur_path);
	roster = newroster;
	theCamera = NULL;
}

void CDVTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDVTDlg)
	DDX_Control(pDX, IDC_StudentView, m_studentview);
	DDX_Control(pDX, IDC_StudentList, m_studentlist);
	DDX_Check(pDX, IDC_SortResult, m_SortResultChecked);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDVTDlg, CDialog)
	//{{AFX_MSG_MAP(CDVTDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_UPDATE_COMMAND_UI(ID_FILE_ROSTERSETTINGS, OnUpdateFileRostersettings)
	ON_BN_CLICKED(IDC_BAddStudent, OnBAddStudent)
	ON_BN_CLICKED(IDC_BEditStudent, OnBEditStudent)
	ON_COMMAND(ID_FILE_SAVEROSTER, OnFileSaveroster)
	ON_COMMAND(ID_FILE_OPENROSTER, OnFileOpenroster)
	ON_BN_CLICKED(IDC_BRemoveStudent, OnBRemoveStudent)
	ON_LBN_SELCHANGE(IDC_StudentList, OnSelchangeStudentList)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_NEWROSTER, OnFileNewroster)
	ON_BN_CLICKED(IDC_BTakePictures, OnBTakePictures)
	ON_BN_CLICKED(IDC_SortResult, OnSortResult)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDVTDlg message handlers

BOOL CDVTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	//Display the opening menu
	CString result;
	StartScreenDlg startscreen(NULL, &result);
	startscreen.DoModal();

	//Check for which menu option was chosen and perform the selected action.
	if (result=="New") CDVTDlg::OnUpdateFileRostersettings(NULL);
	if (result=="Open") CDVTDlg::OnInitOpenroster();
	
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDVTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDVTDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDVTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CDVTDlg::OnUpdateFileRostersettings(CCmdUI* pCmdUI) 
{
	RosterDlg dlgRoster(NULL,&roster);
	dlgRoster.DoModal();	
}

void CDVTDlg::OnBAddStudent() 
{
	//Create a new student and launch the student editor
	bool addanother = false;
	Student* entry= roster.CreateStudent();
	

	if(entry==NULL){
		//error
		MessageBox(CREATEERROR);

	}else{
		roster.EditStudent(entry, &addanother);
	}

	RefreshStudentList();

	if (addanother == true) CDVTDlg::OnBAddStudent();
}

void CDVTDlg::OnBEditStudent() 
{
	//Check to make sure a student is selected
	if (m_studentlist.GetCurSel()==-1)
	{
		MessageBox(SELECTEDITERROR);
		return;
	}

	//Extract the student's name	
	CString stuname;
	m_studentlist.GetText(m_studentlist.GetCurSel(), stuname);
	if (stuname.Find('\t')!=-1) stuname = stuname.Left(stuname.Find('\t'));

	//Use the student's name to get the student and pass it to the student editor
	roster.EditStudent(roster.GetStudent(stuname), NULL);

	RefreshStudentList();
}

void CDVTDlg::OnFileSaveroster() 
{
	//Set CFileDialog file filters and run it.
	char strFilter[] = {"XLS Files (*.xls)|*.xls|All Files (*.*)|*.*||" };
	CFileDialog savedlg(FALSE,".xls",roster.GetLabel(),0,strFilter);
	savedlg.DoModal();

	//Save the roster to the chosen path.
	roster.Save(savedlg.GetPathName());
	roster.SetLabel(savedlg.GetFileTitle());
}

void CDVTDlg::OnFileOpenroster() 
{
	//Checks to see if current roster is saved
	bool result;
	YESNODlg yesno(NULL, "Would you like to save '" + roster.GetLabel() + "' before opening a different roster?", &result);
	yesno.DoModal();

	if (result==TRUE) OnFileSaveroster();


	//Set CFileDialog file filters and run it.
	char strFilter[] = {"XLS Files (*.xls)|*.xls|All Files (*.*)|*.*||" };
	CFileDialog opendlg(TRUE,".xls",NULL,0,strFilter);
	opendlg.DoModal();


	CString path = opendlg.GetPathName();
	if (path == "") return;
	//Create a new roster from the file and set it as the project's roster
	Roster newroster(cur_path, path);
	roster = newroster;

	RefreshStudentList();
}

void CDVTDlg::OnInitOpenroster() 
{
	//Set CFileDialog file filters and run it.
	char strFilter[] = {"XLS Files (*.xls)|*.xls|All Files (*.*)|*.*||" };
	CFileDialog opendlg(TRUE,".xls",NULL,0,strFilter);
	opendlg.DoModal();


	CString path = opendlg.GetPathName();
	if (path == "") return;
	//Create a new roster from the file and set it as the project's roster
	Roster newroster(cur_path, path);
	roster = newroster;

	RefreshStudentList();
}

void CDVTDlg::OnBRemoveStudent() 
{
	//Make sure a student is selected
	if (m_studentlist.GetCurSel()==-1)
	{
		MessageBox(SELECTREMOVEERROR);
		return;
	}

	//Extract the student's name	
	CString stuname;
	m_studentlist.GetText(m_studentlist.GetCurSel(), stuname);
	if (stuname.Find('\t')!=-1) stuname = stuname.Left(stuname.Find('\t'));

	//Confirm erasure
	bool result;
	YESNODlg yesno(NULL, "Are you sure you wish to remove the student " + stuname + " from the roster?  This will erase all data associated with this student on this roster.", &result);
	yesno.DoModal();

	if (result==FALSE) return;

	roster.RemoveStudent(stuname);

	RefreshStudentList();
}

void CDVTDlg::OnSelchangeStudentList() 
{
	RefreshPropertyList();
}

void CDVTDlg::RefreshPropertyList()
{
	//Store the current selection index before clearing the list
	curselection = m_studentlist.GetCurSel();
	
	//if the list is empty, just reset it and return
	if (curselection == -1)
	{
		m_studentview.ResetContent();
		return;
	}

	//Extract the student's name
	CString stuname;
	m_studentlist.GetText(curselection, stuname);
	if (stuname.Find('\t')!=-1) stuname = stuname.Left(stuname.Find('\t'));

	//Update the studentview with new student's data.
	UpdateData();
	m_studentview.ResetContent();
	
	//Find the longest string in the list
	int maxlength = 0;
	for (int a=0; a<roster.NumProperties(); a++)
		if (roster.GetPropertyName(a).GetLength()>maxlength) maxlength = roster.GetPropertyName(a).GetLength();
	
	for (a=0; a<roster.NumProperties(); a++)
	{
		//Use the longest string in the list to add tabs to the list for even columns
		CString tabs = "";
		for (int b=0; b<(maxlength/8-roster.GetPropertyName(a).GetLength()/8)+1; b++)
			tabs += "\t";

		m_studentview.AddString(roster.GetPropertyName(a) + ": " + tabs + roster.GetStudentPropertyValue(stuname, a));
	}
	

	UpdateData(FALSE);
}

void CDVTDlg::RefreshStudentList()
{
	//Store the current selection index before clearing the list
	curselection = m_studentlist.GetCurSel();

	
	UpdateData();
	
	m_studentlist.ResetContent();
	
	//Find the longest string in the list
	int maxlength = 0;
	for (int a=0; a<roster.NumStudents(); a++)
		if (roster.GetStudentName(a).GetLength()>maxlength) maxlength = roster.GetStudentName(a).GetLength();

	for (a=0;a<roster.NumStudents();a++)
	{
		//Use the longest string in the list to add tabs to the list for even columns
		CString tabs = "";
		for (int b=0; b<(maxlength/8-roster.GetStudentName(a).GetLength()/8)+1; b++)
			tabs += "\t";

		//Add the P/R marker in the second column
		CString result = roster.GetStudentPropertyValue(a,"Photo Result");
		if (result=="Pass")
			m_studentlist.AddString(roster.GetStudentName(a) + tabs + "P");
		else if (result=="Refer")
			m_studentlist.AddString(roster.GetStudentName(a) + tabs + "R");
		else if (result=="Rescreen")
			m_studentlist.AddString(roster.GetStudentName(a) + tabs + "RS");
		else if (result=="Unable")
			m_studentlist.AddString(roster.GetStudentName(a) + tabs + "U");
		else
			m_studentlist.AddString(roster.GetStudentName(a));
	}
	
	UpdateData(FALSE);

	//If it is still available, set the selection where it was
	if (curselection >= 0 && curselection < m_studentlist.GetCount()) m_studentlist.SetCurSel(curselection);

	RefreshPropertyList();
}

void CDVTDlg::OnFileExit() 
{
	bool result;
	YESNODlg yesno(NULL, "Do you wish to save changes to '" + roster.GetLabel() + "'?", &result);
	yesno.DoModal();
	if (result==TRUE) 
	{
		OnFileSaveroster();
	}
	CDialog::OnOK();
}

void CDVTDlg::OnFileNewroster() 
{
	//Confirm the creation of a new roster
	bool result;
	YESNODlg yesno(NULL, "Are you sure you wish to start a new roster?  This will erase any unsaved changes you have made.", &result);
	yesno.DoModal();

	if (result==FALSE) return;

	//Create a new blank roster and feed it to the roster editor
	Roster newroster(cur_path);
	roster = newroster;

	RosterDlg dlgRoster(NULL,&roster);
	dlgRoster.DoModal();

	RefreshStudentList();
}

void CDVTDlg::OnBTakePictures() 
{
	//Make sure a student is selected
	if (m_studentlist.GetCurSel()==-1)
	{
		MessageBox(SELECTERROR);
		return;
	}

	//Extract the student's name	
	CString stuname;
	m_studentlist.GetText(m_studentlist.GetCurSel(), stuname);
	if (stuname.Find('\t')!=-1) stuname = stuname.Left(stuname.Find('\t'));
	
	//Use the student's name to get the student and pass it to the picture wizard
	CCameraConnect *cameraconnect = new CCameraConnect();
	theCamera = (void*)cameraconnect; 
	
	//check if name has not been entered
	/*if (stuname.Compare("N/A") == 0)
		MessageBox("Invalid Student Name");*/

	if( !roster.TakePictures(roster.GetStudent(stuname), theCamera) )
		MessageBox(CONNECTERROR); 
	
}

void CDVTDlg::OnSortResult() 
{
	RefreshStudentList();
	//if (m_SortResultChecked)
}

void CDVTDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	BOOL result;

	OnFileExit();
	
	if (theCamera) // camera might have not been created yet
		result = ((CCameraConnect*) theCamera)->EndCamera();
	CDialog::OnClose();
}
