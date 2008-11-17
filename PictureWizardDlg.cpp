// PictureWizardDlg.cpp : implementation file
//
#include <winuser.h>
#include <windows.h>
#include <stdio.h>

#include "resource.h"
#include "stdafx.h"
#include "DVT.h"
#include "PictureWizardDlg.h"
#include "CameraConnect.h"
#include "PictureEx.h"

#define EYEWIND "EyeDx Automated Digital Vision Screener"
#define PICTURE_WINDOW_WIDTH 145
#define PICTURE_WINDOW_HEIGHT 130

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif



byte * genKeystrokes(CString name, int & length);


/////////////////////////////////////////////////////////////////////////////
// CPictureWizardDlg dialog


CPictureWizardDlg::CPictureWizardDlg(CWnd* pParent /*=NULL*/, Roster *roster, Student *student, void* theCamera, CString curpath)
	: CDialog(CPictureWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPictureWizardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	cur_path = curpath;
	my_roster = roster;
	my_student = student;
	my_theCamera = theCamera;
	cur_path = curpath;
	horizontal = true;

}


void CPictureWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPictureWizardDlg)
	DDX_Control(pDX, IDC_PICTURE_RIGHT, m_jpgRight);
	DDX_Control(pDX, IDC_PICTURE_LEFT, m_jpgLeft);
	DDX_Control(pDX, IDC_ViewFinder, m_ViewFinder);
	DDX_Control(pDX, IDC_StudentName, m_StudentLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPictureWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CPictureWizardDlg)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON5, OnEyeDx)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureWizardDlg message handlers


// not sure if this is used.  We were originally hardcoding in 
// the directory where to store the image, but now it is stored to
// the project directory
void CPictureWizardDlg::GetSavePath(char *cpBuffer, int iBufferSize)
{
	//char	*cpPos;  Commented due to unreferenced variable
	

	//strcpy( cpBuffer, cur_path );
	strcpy( cpBuffer, "c:\\\0" );


#if 0
	/* The directory to save is acquired. */
	m_CSavePath.GetWindowText( cpBuffer, iBufferSize );
	if( strlen(cpBuffer) == 0 )
	{
		GetModuleFileName( NULL, cpBuffer, iBufferSize );
		cpPos = strrchr( cpBuffer, '\\' );
		if( cpPos )
		{
			*(cpPos+1) = '\0';
		}
	}
	else
	{
		if( cpBuffer[strlen(cpBuffer)-1] != '\\' )
		{
			strcat( cpBuffer, "\\" );
		}
	}
#endif	
}



BOOL CPictureWizardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString SPath_name;

	
	
	CFont *m_NewFont = new CFont;
	m_NewFont->CreatePointFont(180, "Times New Roman");
	m_StudentLabel.SetFont(m_NewFont);
	m_StudentLabel.SetWindowText(my_student->GetPropertyValue("Name"));
		
	/*
	 * No longer needed because checked for in StudentDlg
	 if (my_student->GetPropertyValue("Name").Find("\\")>=0) {
		MessageBox("Invalid student name.");
	}*/ 
	
	/*hcount++;
	vcount++;

	str_hcount.Format("%d", hcount);
	str_vcount.Format("%d", vcount);*/

	// create two files names for the left and right image
	//SFile_nameleft = cur_path+"\\"+my_student->GetPropertyValue("Name")+"H"+ str_hcount + ".jpg";
	//SFile_nameright = cur_path+"\\"+my_student->GetPropertyValue("Name")+"V"+str_vcount + ".jpg";

	CreateDirectory(my_roster->GetLabel(),NULL);

	SPath_name = cur_path+"\\"+ my_roster->GetLabel()+"\\"+my_student->GetPropertyValue("Name");


	// pass image names to instance of CCameraConnect
	((CCameraConnect*) my_theCamera)->SetPathName((SPath_name));
		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}


LRESULT CPictureWizardDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	char szSavePath[100];
	GetSavePath(szSavePath, 0);
	((CCameraConnect*) my_theCamera)->WindowProc(message, wParam, lParam, szSavePath);
	
	//Picture is ready here
	if (((CCameraConnect*) my_theCamera)->PictureDoneWait()) 
	{
		((CCameraConnect*) my_theCamera)->PictureDoneSignal(FALSE);
		
		// At this point the images should be displayed to the screen and later 
		// analyzed
		((CCameraConnect*) my_theCamera)->StartViewfinder();
		//MessageBox("Picture is ready.Please wait while the image is analyzed");
	}


	return CDialog::WindowProc(message, wParam, lParam);
}


BOOL CPictureWizardDlg::OnCamera()
{
	
	BOOL CamOk;

	//CCameraConnect *cameraconnect = new CCameraConnect(&m_ViewFinder);
	//my_theCamera = (void*) cameraconnect;
	// connect to the camera
	CamOk = ((CCameraConnect*) my_theCamera)->ConnectCamera(this, &m_ViewFinder);
	if (CamOk)
	{
		//cameraconnect->ConnectCamera(this);
		//cameraconnect->StartViewfinder();
		// start the view finder
		((CCameraConnect*) my_theCamera)->StartViewfinder();
		return TRUE;
		
	}
	else
		return FALSE;
}

void CPictureWizardDlg::OnButton2() 
{
	horizontal = true;
	MessageBox("Please Take the Horizontal Picture Now");
	
	//the following was done in camera connect
	//failed attempt to draw. have fun with this next quarter!
	/*PAINTSTRUCT paintstruct;

	CDC * hdc = BeginPaint((LPPAINTSTRUCT)&paintstruct);
	HDC t = hdc->m_hDC;
	

	HDC hImageDC = CreateCompatibleDC(t);
	HANDLE hImage;
	hImage = LoadImage(NULL, "test.jpg", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//	hImage = 0;
	BITMAP Bitmap;

	SIZE ImageSize;

	GetObject(hImage, sizeof(BITMAP), &Bitmap);
	ImageSize.cx = Bitmap.bmWidth;
	ImageSize.cy = Bitmap.bmHeight;

	SelectObject(hImageDC, hImage);
	hdc->BitBlt(t, 0, 0, ImageSize.cx, ImageSize.cy, hImageDC, 0,0, SRCCOPY);
	EndPaint((LPPAINTSTRUCT)&paintstruct);*/
}

void CPictureWizardDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	//((CCameraConnect*) my_theCamera)->StartRelease("C:\\Documents and Settings\\Administrator\\Desktop\\DVT\\testpics");
   /* ((CCameraConnect*) my_theCamera)->WindowProc(message, wParam, lParam, cpBuffer)*/
}
























#define MAX_LOADSTRING	100
#define HIMETRIC_INCH	2540
#define MAP_LOGHIM_TO_PIX(x,ppli)   ( ((ppli)*(x) + HIMETRIC_INCH/2) / HIMETRIC_INCH )

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text
LPPICTURE gpPicture;
HWND ghWnd;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void CPictureWizardDlg::LoadPictureFile(LPCTSTR szFile)
{
	OutputDebugString("\nsizefile's values is: ");
	OutputDebugString(szFile);
	
	// open file
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	_ASSERTE(INVALID_HANDLE_VALUE != hFile);


	// get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	_ASSERTE(-1 != dwFileSize);

	LPVOID pvData = NULL;
	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	DWORD dwBytesRead = 0;
	// read file and store in global memory
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
	_ASSERTE(FALSE != bRead);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	LPSTREAM pstm = NULL;
	// create IStream* from global memory
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	_ASSERTE(SUCCEEDED(hr) && pstm);

	// Create IPicture from image file
	if (gpPicture)
		gpPicture->Release();
	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture);
	_ASSERTE(SUCCEEDED(hr) && gpPicture);	
	pstm->Release();

	//InvalidateRect(ghWnd, NULL, TRUE);
	


}



















void CPictureWizardDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here

	// Calling Eyedx
SHELLEXECUTEINFO sei;
sei.cbSize = sizeof(SHELLEXECUTEINFO);
sei.fMask = NULL;
sei.hwnd = NULL;
sei.lpVerb = "open";
//sei.lpFile = "C:\\Program Files\\EyeDx1.5.2\\eyedx.exe";
//sei.lpFile = "C:\\WINDOWS\\system32\\calc.exe";
sei.lpFile = "C:\\Documents and Settings\\Administrator\\Desktop\\DVT\\Debug\\EyeDx1.5.2\\eyedx.EXE";
sei.lpParameters= NULL;
sei.nShow = SW_SHOWNORMAL;
sei.hInstApp = NULL;
sei.lpIDList = NULL;
sei.lpClass = NULL;
sei.hkeyClass = NULL;
sei.dwHotKey = NULL;
sei.hIcon = NULL;
sei.hProcess = NULL;
sei.lpDirectory = ".\\EyeDx1.5.2\\";
int ReturnCode = ::ShellExecuteEx(&sei);




//sei.hProcess->SetFocus();
HWND eyeWnd;
eyeWnd = FindWindowEx(NULL, NULL, NULL, NULL);//EYEWIND);
if(eyeWnd == NULL)
{
printf("Couldn't find solitare's window: %d\n", GetLastError());
MessageBox("EyeDx not found, bucko!");
//return -1;
}
//else
//MessageBox("EyeDx was found, good job!");


if(eyeWnd)
{
	// Restore if minimized --->
	if(::IsIconic(eyeWnd))
		::OpenIcon(eyeWnd);
	// Restore if minimized <---
	::SetForegroundWindow(eyeWnd);
	::SetFocus(eyeWnd);
	Sleep(500);
}
//::SetActiveWindow(eyeWnd);
//eyeWnd->SetFocus();

// Sending input to Eyedx
INPUT input[2];
memset(input, 0, sizeof(input));
input[0].type = INPUT_KEYBOARD;
input[0].ki.wVk = 0x44;//(00,32); // ASCI value of ALT + 'D'
input[0].ki.dwFlags = 0;
input[0].ki.time = 0;
input[0].ki.dwExtraInfo = 0;

input[1].type = INPUT_KEYBOARD;
input[1].ki.wVk = 0x44;//(00,32); // ASCI value of ALT + 'D'
input[1].ki.dwFlags = KEYEVENTF_KEYUP;
input[1].ki.time = 0;
input[1].ki.dwExtraInfo = 0;

SendInput(2,input,sizeof(INPUT));

// define keystrokes
const int totalkeys = 30;
/*byte inputs[totalkeys] = {0x44,0x55,0x55,0x50,0xBE,
					0x4A,0x50,0x47,0x0D,0x59,
					0x53,0x53,0x49,0xBE,0x4A,
					0x50,0x47,0x0D,0x59,0x09,
					0x09,0x09,0x54,0x4D,0x50,
					0x30,0x09,0x4f,0x0d,0x0d};
byte inputs[totalkeys] = {'D','U','U','P','.',
					'J','P','G',0x0D,'Y',
					'S','S','I','.','J',
					'P','G',0x0D,'Y',0x09,
					0x09,0x09,'T','M','P',
					'0',0x09,'O',0x0d,0x0d};*/
/*byte inputs[totalkeys] = {0x44,0x55,0x55,0x50,0xBE,
					0x4A,0x50,0x47,0x0D,0x59,
					0x53,0x53,0x49,0xBE,0x4A,
					0x50,0x47,0x0D,0x59,0x09,
					0x09,0x09,'T','M','P',
					'0',0x09,0x4f,0x0d,0x0d};*/
int length;
byte * inputs = genKeystrokes(my_student->GetPropertyValue("Name"), length);
int i;

// copy pictures
if (((CCameraConnect*) my_theCamera)->getHSelect()) CopyFile(fileNameRight,
		 cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\up.jpg",false);
else
{

	CString HorizontalFile(((CCameraConnect*) my_theCamera)->GetLastHorizontalFile());

	CopyFile(HorizontalFile,cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\up.jpg",false);
}

if (((CCameraConnect*) my_theCamera)->getVSelect()) CopyFile(fileNameLeft,
		 cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\up.jpg",false);
else
{

	CString VerticalFile(((CCameraConnect*) my_theCamera)->GetLastVerticalFile());

	CopyFile(VerticalFile,cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\si.jpg",false);
}


/*
CopyFile(cur_path+"\\"+my_student->GetPropertyValue("Name")+"H1left.jpg",
		 cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\up.jpg",false);

CopyFile(cur_path+"\\"+my_student->GetPropertyValue("Name")+"H1right.jpg",
		 cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\si.jpg",false);
*/

MessageBox(cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\up.jpg");
MessageBox(cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\si.jpg");

LoadPictureFile(cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\up.jpg");
LoadPictureFile(cur_path+"\\EyeDx1.5.2\\NonSession\\Pimages\\si.jpg");

/*
ghWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
*/
// send keystrokes here
fprintf(stderr, "%d length", &length);
for(i=0;i<length;i++) {
	input[0].ki.wVk = input[1].ki.wVk = inputs[i];
	SendInput(2,input,sizeof(INPUT));
}

//rename the report
/*CopyFile(cur_path+"\\EyeDx1.5.2\\NonSession\\reports\\U_tmp0.htm",
		 cur_path+"\\EyeDx1.5.2\\NonSession\\reports\\U_"+
		 my_student->GetPropertyValue("Name")+"_report.htm",false);

CopyFile(cur_path+"\\EyeDx1.5.2\\NonSession\\reports\\R_tmp0.htm",
		 cur_path+"\\EyeDx1.5.2\\NonSession\\reports\\R_"+
		 my_student->GetPropertyValue("Name")+"_report.htm",false);
*/
}

void CPictureWizardDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	BOOL result;
	
	if (my_theCamera) // camera might have not been created yet
		result = ((CCameraConnect*) my_theCamera)->EndCamera();

	CDialog::OnCancel();
}

byte * genKeystrokes(CString name, int & length)
{

	int caps = GetKeyState(VK_CAPITAL);
	length = 25 + name.GetLength();
	if(caps & 1)
	{
		length += 2;
	}
	byte * inputs = new byte[length];
		
	
		
	byte staticContent[26]	= {0x44,0x55,0x55,0x50,0xBE,
					0x4A,0x50,0x47,0x0D,0x59,
					0x53,0x53,0x49,0xBE,0x4A,
					0x50,0x47,0x0D,0x59,0x09,
					0x09,0x09,0x09,0x4f,0x0d,0x0d};

	byte * temp;
/*	if(caps & 1)
	{
		//fprintf(stdout, "here");
		temp = inputs++;
		inputs[0] = VK_CAPITAL;
	}
	else
	{
		fprintf(stdout, "THERE");
	*/	temp = inputs;
/*	}*/
	int i = 0;
	for(i = 0;i < 22; i++)
		temp[i] = staticContent[i];

	name.MakeUpper();
	for(i = 0; i < name.GetLength(); i++)
	{
		temp[22 + i] = name.GetAt(i);
	}

	for(i = 0; i < 3; i++)
	{
		temp[22 + name.GetLength() + i] = staticContent[22 + i];
	}

	if(caps & 0xF000)
	{
		inputs[length-1] = VK_CAPITAL;
	}

	return inputs;

}

void CPictureWizardDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CPictureWizardDlg::OnButton3() 
{
	horizontal = false;
	MessageBox("Please Take the Vertical Picture Now");
	
}

void CPictureWizardDlg::OnEyeDx() 
{
	// TODO: Add your control notification handler code here

		// Calling Eyedx
		SHELLEXECUTEINFO sei;
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.fMask = NULL;
		sei.hwnd = NULL;
		sei.lpVerb = "open";
		//sei.lpFile = "C:\\Program Files\\EyeDx1.5.2\\eyedx.exe";
		//sei.lpFile = "C:\\WINDOWS\\system32\\calc.exe";
		sei.lpFile = "C:\\Documents and Settings\\Administrator\\Desktop\\DVT\\Debug\\EyeDx1.5.2\\eyedx.EXE";
		sei.lpParameters= NULL;
		sei.nShow = SW_SHOWNORMAL;
		sei.hInstApp = NULL;
		sei.lpIDList = NULL;
		sei.lpClass = NULL;
		sei.hkeyClass = NULL;
		sei.dwHotKey = NULL;
		sei.hIcon = NULL;
		sei.hProcess = NULL;
		sei.lpDirectory = ".\\EyeDx1.5.2\\";
		int ReturnCode = ::ShellExecuteEx(&sei);

	
}



void CPictureWizardDlg::OnButton6() 
{
//    hselect = false; 
//	CString fileNameLeft; 
	((CCameraConnect*) my_theCamera)->setH(true);
	CFileDialog dlg(TRUE/*Open=TRUE Save=False*/,NULL/*Filename Extension*/,"Directory Selection"/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST/*Flags*/,"JPG(*.jpg)|*.jpg||"/*Filetype Filter*/,this/*parent Window*/);
	if (dlg.DoModal() == IDOK)
	{
     fileNameLeft = dlg.GetFileName();
	}

   	if (m_jpgLeft.Load(_T(fileNameLeft)))
	{	
		m_jpgLeft.Scale(PICTURE_WINDOW_WIDTH, PICTURE_WINDOW_HEIGHT);
		m_jpgLeft.Draw();
	}
	  
}	


void CPictureWizardDlg::OnButton7() 
{
		
//	vselect = true; 
//	CString fileNameRight; 
	((CCameraConnect*) my_theCamera)->setV(true); 
	CFileDialog dlg(TRUE/*Open=TRUE Save=False*/,NULL/*Filename Extension*/,"Directory Selection"/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST/*Flags*/,"JPG(*.jpg)|*.jpg||"/*Filetype Filter*/,this/*parent Window*/);
	if (dlg.DoModal() == IDOK)
	{
     fileNameRight = dlg.GetFileName();
	}


	if (m_jpgRight.Load(_T(fileNameRight)))
	{	
		m_jpgRight.Scale(PICTURE_WINDOW_WIDTH, PICTURE_WINDOW_HEIGHT); 
		m_jpgRight.Draw();
	}	
}	

