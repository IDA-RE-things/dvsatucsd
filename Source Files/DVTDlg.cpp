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

#include "WiaWrap.h"
#include "BitmapWnd.h"

//CameraConnect Includes
#include "CameraConnect.h"

#include <direct.h> 
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
//
// Function:
// Parameters:
// Returns:
// Description:
//
//
//
///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
//
// Function:
// Parameters:
// Returns:
// Description:
//
//
//
///////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

///////////////////////////////////////////////////////////////////////////////
//
// Function:
// Parameters:
// Returns:
// Description:
//
//
//
///////////////////////////////////////////////////////////////////////////////
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

void CDVTDlg::OnHelpAbout() 
{
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnCameraConnect()
// Parameters: none
// Returns: none
// Description: Determines whether a WIA-compatible camera is connected. If so,
//				loads default camera control windows.
//
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnCameraConnect() 
{
    HRESULT hr;

    m_bDisplayWaitCursor = TRUE;

    // Launch the get image dialog

    WiaWrap::CComPtrArray<IStream> ppStream;

    hr = WiaWrap::WiaGetImage(
        m_hMDIClient,
        StiDeviceTypeDefault,
        0,
        WIA_INTENT_NONE,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &ppStream.Count(),
        &ppStream
    );

    m_bDisplayWaitCursor = FALSE;

    // If there was an error, display an error message box

	if (FAILED(hr)) 
    {
		TCHAR szError[DEFAULT_STRING_SIZE] = _T("");

		LoadString(g_hInstance, IDS_ERROR_GET_IMAGE_DLG, szError, COUNTOF(szError));

        MessageBox(szError);
	}

    // Open a new window for each successfully transferred image

    for (int i = 0; i < ppStream.Count(); ++i)
    {
        CComPtr<CBitmapWnd> pBitmapWnd = new CBitmapWnd(ppStream[i]);
        if (pBitmapWnd != NULL)
        {
            m_nNumImages += 1;

            TCHAR szFormat[DEFAULT_STRING_SIZE] = _T("%d");

		    LoadString(g_hInstance, IDS_BITMAP_WINDOW_TITLE, szFormat, COUNTOF(szFormat));

		    TCHAR szTitle[DEFAULT_STRING_SIZE];

    	    _sntprintf_s(szTitle, COUNTOF(szTitle), COUNTOF(szTitle) - 1, szFormat, m_nNumImages);

            szTitle[COUNTOF(szTitle) - 1] = _T('\0');

            CreateWindowEx(
                WS_EX_MDICHILD,
                _T("BitmapWindow"),
                szTitle,
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                m_hMDIClient, 
                NULL, 
                g_hInstance, 
                pBitmapWnd
            );
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg(CWnd* pParent) : CDialog(CDVTDlg::IDD, pParent)
// Parameters: CWnd* pParent - NULL
//			   CDialog(CDVTDlg::IDD,pParent) - FILL IN
// Returns: None
// Description:  Defines what a CDVTDlg is and what parameters it takes.
//               Creates a Roster and stores it, and initializes the Camera to
//               NULL
//
///////////////////////////////////////////////////////////////////////////////
CDVTDlg::CDVTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDVTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDVTDlg)
	m_SortResultChecked = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	char tempstr[32768];
	_getcwd(tempstr, 32768);
	cur_path = tempstr;

	Roster newroster(cur_path);
	roster = newroster;
	theCamera = NULL;

	m_cRef = 0;

    m_nNumImages = 0;

    m_bDisplayWaitCursor = FALSE;

    m_pEventCallback = new WiaWrap::CEventCallback;

    if (m_pEventCallback != NULL)
    {
        m_pEventCallback->Register();
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::DoDataExchange(CDataExchange* pDX)
// Parameters: CDataExchange * pDX - FILL IN
// Returns: none
// Description: FILL IN
//
//
//
///////////////////////////////////////////////////////////////////////////////
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
	ON_COMMAND(ID_FILE_SAVEROSTERAS, OnFileSaverosteras)
	ON_COMMAND(ID_FILE_OPENROSTER, OnFileOpenroster)
	ON_BN_CLICKED(IDC_BRemoveStudent, OnBRemoveStudent)
	ON_LBN_SELCHANGE(IDC_StudentList, OnSelchangeStudentList)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_NEWROSTER, OnFileNewroster)
	ON_BN_CLICKED(IDC_BTakePictures, OnBTakePictures)
	ON_BN_CLICKED(IDC_SortResult, OnSortResult)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CAMERA_CONNECT, OnCameraConnect)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnInitDialog()
// Parameters: None
// Returns:  FALSE - if no focus to control is set
//           TRUE - if focus to control is set
// Description:  Initialized the CDVTDlg.  Creates a CDialog. Creates 
//               StartScreenDlg and waits for roster name to be set.  Once
//               Roster name is set, Roster is created with that name.
//
///////////////////////////////////////////////////////////////////////////////
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
	
	while(roster.GetLabel() == "Unnamed Roster")
	{
		//Display the opening menu
		CString result;
		StartScreenDlg startscreen(NULL, &result);
		startscreen.DoModal();

		//Check for which menu option was chosen and perform the selected action.
		if (result=="New") CDVTDlg::OnUpdateFileRostersettings(NULL);
		if (result=="Open") CDVTDlg::OnInitOpenroster();
	}
	
	//display roster name on dlg box
	RosterName = new CStatic();
	RosterName->Create("Roster Name", WS_CHILD | WS_VISIBLE,
		CRect(10,10,400,60), this,90210);
	RosterName->SetWindowText(roster.GetLabel());

	//set focus to take pictures button
	CDialog::SetDefID(IDC_BTakePictures);

	//load camera connection windows
	CLIENTCREATESTRUCT ccs = { 0 };

	ccs.hWindowMenu  = GetSubMenu(GetMenu()->m_hMenu, WINDOW_MENU_POSITION);
	ccs.idFirstChild = FIRST_MDI_CHILD;

	m_hMDIClient = CreateWindowEx(
                0,
                _T("MDICLIENT"), 
                0, 
                WS_CLIPCHILDREN | WS_CHILD | WS_VISIBLE, 
                0,
                0,
                0, 
                0,
                m_hWnd,
                NULL, 
                g_hInstance, 
                &ccs
            );
		
	return FALSE;  // return TRUE  unless you set the button focus to a control
}


///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnSysCommand(UINT nID, LPARAM lParam)
// Parameters: UINT nID - FILL IN 
//             LPARAM lParam - FILL IN
// Returns: None
// Description: FILL IN
//
//
//
///////////////////////////////////////////////////////////////////////////////
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



///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnPaint()
// Parameters: None
// Returns: None
// Description: If you add a minimize button to your dialog, you will need
//              the code below to draw the icon.  For MFC applications using 
//              the document/view model, this is automatically done for you
//              by the framework.
//
///////////////////////////////////////////////////////////////////////////////

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


///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnQueryDragIcon()
// Parameters: None
// Returns: HCURSOR - FILL IN
// Description: The system calls this to obtain the cursor to display 
//              while the user drags the minimized window.
//
//
//
///////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDVTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnUpdateFileRostersettings(CCmdUI* pCmdUI) 
// Parameters: CCmDUI * pCmdUI - FILL IN
// Returns: None
// Description: Updates File Roster Settings.
//
//
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnUpdateFileRostersettings(CCmdUI* pCmdUI) 
{
	RosterDlg dlgRoster(NULL,&roster);
	dlgRoster.DoModal();

	RosterName->SetWindowText(roster.GetLabel());
	RefreshStudentList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnBAddStudent() 
// Parameters: None
// Returns: None
// Description: Creates a new student and launches the student editor
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnBAddStudent() 
{
	//Create a new student and launch the student editor
	bool addanother = false;
	Student* entry= roster.CreateStudent();
	

	if(entry==NULL){
		//error
		MessageBox(CREATEERROR);

	}else{
		roster.EditStudent(entry, &addanother, TRUE);
	}

	RefreshStudentList();

	if (addanother == true) CDVTDlg::OnBAddStudent();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnBEditStudent() 
// Parameters: None
// Returns: None
// Description: Checks if student name has been selected.  If student has been
//              selected, gets student name and calls roster.EditStudent to
//              allow user to edit Student.
//
///////////////////////////////////////////////////////////////////////////////
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
	roster.EditStudent(roster.GetStudent(stuname), NULL, FALSE);

	RefreshStudentList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnFileSaveroster() 
// Parameters: None
// Returns: None
// Description:  Saves the roster.
//
//
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnFileSaveroster() 
{
	if (roster.GetPath() == "") {
		CDVTDlg::OnFileSaverosteras();
	} else {
		//Save the roster to the chosen path
		roster.Save(roster.GetPath());
	}
}

///////////////////////////////////////////////////////////////////////////////
//
// Function:  CDVTDlg::OnFileSaverosteras()
// Parameters: None
// Returns: None
// Description: Opens Save Dialog to allow user to save dialog to XLS file.
//              Then saves XLS to that location
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnFileSaverosteras()
{
	//Set CFileDialog file filters and run it.
	char strFilter[] = {"XLS Files (*.xls)|*.xls|All Files (*.*)|*.*||" };
	CFileDialog savedlg(FALSE,".xls",roster.GetLabel(),0,strFilter);
	savedlg.DoModal();

	//Save the roster to the chosen path.
	roster.Save(savedlg.GetPathName());
	roster.SetPath(savedlg.GetPathName());
	// roster.SetLabel(savedlg.GetFileTitle()); // should label reflect filename or roster title?

	//MessageBox(savedlg.GetPathName(), NULL, MB_OK);
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnFileOpenroster() 
// Parameters: None
// Returns: None
// Description:  Allows user to open a roster.  
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnFileOpenroster() 
{
	//Checks to see if current roster is saved
	bool result;
	YESNODlg yesno(NULL, "Would you like to save '" + 
		roster.GetLabel() + "' before opening a different roster?", &result);
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

	RosterName->SetWindowText(roster.GetLabel());
	RefreshStudentList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnInitOpenroster() 
// Parameters: None
// Returns: None
// Description: Loads opened roster into program, and refreshes student list
//              to reflect data from opened roster.
//
///////////////////////////////////////////////////////////////////////////////
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

	//highlight first student entry
	m_studentlist.SetCurSel(0);
	RefreshPropertyList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnBRemoveStudent() 
// Parameters: None
// Returns: None
// Description: Removes student from Roster.
//
///////////////////////////////////////////////////////////////////////////////
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
	YESNODlg yesno(NULL, "Are you sure you wish to remove the student " 
		+ stuname + 
		" from the roster?" + 
		" This will erase all data associated with this student on this roster.", 
		&result);
	yesno.DoModal();

	if (result==FALSE) return;

	roster.RemoveStudent(stuname);

	RefreshStudentList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnSelchangeStudentList() 
// Parameters: None
// Returns: None
// Description: FILL IN
//
//
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnSelchangeStudentList() 
{
	RefreshPropertyList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::RefreshPropertyList()
// Parameters: None
// Returns: None
// Description: Refreshes the Roster property list.
//
///////////////////////////////////////////////////////////////////////////////
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
	int a;
	for (a=0; a<roster.NumProperties(); a++)
		if (roster.GetPropertyName(a).GetLength()>maxlength) 
			maxlength = roster.GetPropertyName(a).GetLength();
	
	for (a=0; a<roster.NumProperties(); a++)
	{
		//Use the longest string in the list to add tabs to the list for even columns
		CString tabs = "";
		for (int b=0; b<(maxlength/8-roster.GetPropertyName(a).GetLength()/8)+1; b++)
			tabs += "\t";

		m_studentview.AddString(roster.GetPropertyName(a) + ": "
			+ tabs + roster.GetStudentPropertyValue(stuname, a));
	}
	

	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::RefreshStudentList()
// Parameters: None
// Returns: None
// Description:  Refreshes the list of students in the DVTDlg.
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::RefreshStudentList()
{
	//Store the current selection index before clearing the list
	curselection = m_studentlist.GetCurSel();

	UpdateData();
	
	m_studentlist.ResetContent();
	
	//Find the longest string in the list
	int maxlength = 0;
	int a;
	for (a=0; a<roster.NumStudents(); a++)
		if (roster.GetStudentName(a).GetLength()>maxlength) 
			maxlength = roster.GetStudentName(a).GetLength();

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
	if (curselection >= 0 && curselection < m_studentlist.GetCount())
		m_studentlist.SetCurSel(curselection);

	RefreshPropertyList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnFileExit() 
// Parameters: None
// Returns: None
// Description: Prompts user to save changes to Roster currently being worked
//              on.  Saves if user says to then exits.
//
//
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnFileExit() 
{
	bool result;
	YESNODlg yesno(NULL, "Do you wish to save changes to '" 
		+ roster.GetLabel() + "'?", &result);
	yesno.DoModal();
	if (result==TRUE) 
	{
		OnFileSaveroster();
	}
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnFileNewroster() 
// Parameters:  None
// Returns:  None
// Description:   Prompts user to start a new roster.  If user agrees, new 
//                roster is created.
//
//
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnFileNewroster() 
{
	//Confirm the creation of a new roster
	bool result;
	YESNODlg yesno(NULL, "Are you sure you wish to start a new roster? This will erase any unsaved changes you have made.", &result);
	yesno.DoModal();

	if (result==FALSE) return;

	//Create a new blank roster and feed it to the roster editor
	Roster newroster(cur_path);
	roster = newroster;

	RosterDlg dlgRoster(NULL,&roster);
	dlgRoster.DoModal();

	RosterName->SetWindowText(roster.GetLabel());
	RefreshStudentList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnBTakePictures() 
// Parameters: None
// Returns: None
// Description: Checks if student has been selected, then starts Camera and
//              calls functions to take pictures of student
//
///////////////////////////////////////////////////////////////////////////////

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

	if( !roster.TakePictures(roster.GetStudent(stuname), theCamera) )
		MessageBox(CONNECTERROR); 
	
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnSortResult() 
// Parameters: None
// Returns: None
// Description: Refreshes Student list
//
//
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnSortResult() 
{
	RefreshStudentList();
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CDVTDlg::OnClose() 
// Parameters: None
// Returns: None
// Description: Closes the Camera and closes the Dialog
//
///////////////////////////////////////////////////////////////////////////////
void CDVTDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	BOOL result;

	OnFileExit();
	
	if (theCamera) // camera might have not been created yet
		result = ((CCameraConnect*) theCamera)->EndCamera();
	CDialog::OnClose();
}
