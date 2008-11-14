// CameraConnect.cpp: implementation of the CCameraConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <vfw.h>
#include <afxmt.h>
#include <assert.h>

#include "CameraConnect.h"
#include "PictureWizardDlg.h"
#include "Progress.h"
#include "cdAPI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		CAMERAEVENTMESSAGESTRING_RELEASE_ON		"Camera Event ReleaseOn"
#define		CAMERAEVENTMESSAGESTRING_RELEASE_COMP	"Camera Event ReleaseComplete"
#define		CAMERAEVENTMESSAGESTRING_ABORT_PC_EVF	"Camera Event Abort PC EVF"
#define		CAMERAEVENTMESSAGESTRING_CHANGE_BY_UI	"Camera Event Change By UI"


#define		VIEWFINDER_WIDTH		320
#define		VIEWFINDER_HEIGHT		240



#define		GETERRORID( x )		(cdERROR_ERRORID_MASK&x)

// this should not be here
#define     IDC_DISCONNECT 420

CMutex			g_CVFMutex;					/* The synchronous class of image data */
CWinThread		*g_CpVFThread;				/* The class of a picture display thread */
UINT			g_ChangeByUI=0;				/* A message when the capture parameter of the camera is changed */ 
UINT			g_ReleaseOnMessage=0;		/* A message when the shutter release of a camera is pushed */
UINT			g_ReleaseCompleteMessage=0;	/* A message when photography is completed 											   
											   by operation of the camera UI. */
UINT			g_AbortPCEVF=0;


CString PathName;


//#include "progress.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name: CCameraConnect
//  Description:   Constructor for CCameraConnect class
//  Parameters:    (none)
//  Returns:       (none)
//  Summary:       Besides doing the obvious parameter initialization
//                 that is normally done in constructors, we also 
//                 start the Cannon SDK by setting some initial parameters
//                 and then calling the CDStartSDK
//  Author:        Adam Levy (adam.mlevy@gmail.com)
///////////////////////////////////////////////////////////////////////////
CCameraConnect::CCameraConnect()

{
	
	
	
	// TODO: Add extra initialization here
	BOOL		fRes;
	//char		szPath[MAX_PATH];
	//char		*cpPos;
	cdError		err;
	
	bPictDone = FALSE;
	iPictureCount = 0;
	m_hSource = NULL;
	m_hCallbackFunction = NULL;
	//m_CpViewFinder = &m_cViewImage;

	//m_CpViewFinder = m_cViewImage;
	
	memset( &m_BackSurface, 0, sizeof(DIBData) );
	m_fVFEnd = FALSE;
	m_fProgramRelease = FALSE;
	m_fCamTypeEOS = FALSE;
	m_fMCard = FALSE;
	
	/* A picture buffer is created. */
	fRes = CreateDIBBuffer();
	if( !fRes )
	{
	//	goto	apierr;
	}
	
	
	g_ReleaseOnMessage = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_RELEASE_ON );
	g_ReleaseCompleteMessage = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_RELEASE_COMP );
	g_AbortPCEVF = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_ABORT_PC_EVF );
	g_ChangeByUI = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_CHANGE_BY_UI );
	if( !g_ReleaseOnMessage || !g_ReleaseCompleteMessage || !g_AbortPCEVF || !g_ChangeByUI)
	{
		//goto	apierr;
	}
		
	
	/* CDSDK is changed the first stage. */
	
	
	cdVersionInfo	SVersion;
	memset( &SVersion, 0, sizeof(cdVersionInfo) );
	SVersion.Size = sizeof(cdVersionInfo);
	SVersion.MajorVersion = 7;
	SVersion.MinorVersion = 3;

	err = CDStartSDK( &SVersion, 0 );
	if( GETERRORID(err) != cdOK )
	{
		//goto	camerr;
		
	}
	//cdSize size;
	
	//return TRUE;  // return TRUE  unless you set the focus to a control
			

	
	
	
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name: ~CCameraConnect
//  Description:   Distructor for CCameraConnect class
//  Parameters:    (none)
//  Returns:       (none)
//  Summary:       Once the constructor is called, we end the Cannon SDK 
//  Author:        Adam Levy (adam.mlevy@gmail.com)               
//                     
///////////////////////////////////////////////////////////////////////////
CCameraConnect::~CCameraConnect()
{
	cdError		err;
//	char		szErrStr[256];
	
	/* End processing of CDSDK is performed. */
	err = CDFinishSDK();
	//if( GETERRORID(err) != cdOK )
	//{
	//	wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
	//	MessageBox( szErrStr );
	//}
}


//////////////////////////////////////////////////////////////////////////
// 
//  Function Name: CamCallBackFunc
//  Description:   Callback function for the camera
//  Parameters:    cdEventID EventID
//                 const cdVoid* pData
//                 cdUInt32 DataSize
//                 cdContext Context
//  Returns:       cdOK to indicate that we returned successfully
//  Summary:       
//  Author:        CannonSDK
//                                           
///////////////////////////////////////////////////////////////////////////
cdUInt32 cdSTDCALL	CCameraConnect::CamCallBackFunc(	cdEventID		EventID,
													const cdVoid*	pData,
													cdUInt32		DataSize,
													cdContext		Context )
{
	WPARAM			wParam;
	CCameraConnect		*CpThis;
		
	CpThis = (CCameraConnect*)Context;
	
	// A security level is checked. 
	switch( cdEVENT_SEVERITY(EventID) )
	{
		case cdEVENT_SEVERITY_SHUTDOWN:

			wParam = ((WPARAM)BN_CLICKED<<16)|(0xFFFF&IDC_DISCONNECT);
			CpThis->pCPictureWizardDlg->PostMessage( WM_COMMAND, wParam, NULL );
			break;
		case cdEVENT_SEVERITY_NONE:
		case cdEVENT_SEVERITY_WARNING:
			break;
	}
	
	return	cdOK;
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name: RelCallBackFunc
//  Description:   Used to set camera in remote release control
//  Parameters:    cdReleaseEventID EventID
//                 const void* pData
//                 cdUInt32 DataSize
//                 cdContext Context
//  Returns:       cdError code
//  Summary:       CDEnterReleaseControl is called with this callback function
//  Author:        Mostly CannonSDK.  Some small modifications by Adam Levy.
//                           
///////////////////////////////////////////////////////////////////////////
cdUInt32 cdSTDCALL	CCameraConnect::RelCallBackFunc(	cdReleaseEventID	EventID,
													const void *		pData,
													cdUInt32 			DataSize,
													cdContext			Context )
{
	cdError			err=cdOK;
	//CCameraConnect		*CpThis;
	CPictureWizardDlg		*CpThis;
	
	cdUInt32		*dwpCount;
	CProgress		CProg;
	
	//CpThis = (CCameraConnect*)Context;
	CpThis = (CPictureWizardDlg*)Context;
	switch( EventID )
	{
		case cdRELEASE_EVENT_RESET_HW_ERROR:
			break;
		case cdRELEASE_EVENT_CHANGED_BY_UI:
			//CpThis->pDVTDlg->PostMessage( g_ChangeByUI );
			break;
		case cdRELEASE_EVENT_RELEASE_START:
			break;
		case cdRESEASE_EVENT_RELEASE_COMPLETE:
			dwpCount = (cdUInt32*)pData;
			//CpThis->pDVTDlg->PostMessage( g_ReleaseCompleteMessage, (WPARAM)*dwpCount );
			CpThis->PostMessage( g_ReleaseCompleteMessage, (WPARAM)*dwpCount );
			break;
		case cdRELEASE_EVENT_CAM_RELEASE_ON:
			//CpThis->pDVTDlg->PostMessage( g_ReleaseOnMessage );
			CpThis->PostMessage( g_ReleaseOnMessage );
			
			break;
		case cdRELEASE_EVENT_ABORT_PC_EVF:
			//CpThis->pDVTDlg->PostMessage( g_AbortPCEVF );
			break;
	}
	
	return	err;
}


//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  ViewFinderCallBackFun 
//  Description:    The function which receives the picture from a camera 
//  Parameters:     cdVoid *pBuf
//                  cdUInt32 Size
//                  cdUInt32 Format
//                  cdContext Context
//  Returns:        cdError code
//  Summary:       
//  Author:         CannonSDK                           
///////////////////////////////////////////////////////////////////////////
cdUInt32 cdSTDCALL	CCameraConnect::ViewFinderCallBackFun(	cdVoid		*pBuf,
											 			cdUInt32	Size,
														cdUInt32	Format,
														cdContext	Context )
{
	cdError				err=cdOK;
	LPBYTE				bpPixel;
	PBITMAPFILEHEADER	pBmpFileheader;
	PBITMAPINFOHEADER	pBmpInfoheader;
	CCameraConnect			*CpThis;
	
	CpThis = (CCameraConnect*)Context;
	
	
	if( Format == FILEFORMAT_BMP )
	{
		pBmpFileheader = (PBITMAPFILEHEADER)pBuf;
		pBmpInfoheader = (PBITMAPINFOHEADER)((LPBYTE)pBuf + sizeof(BITMAPFILEHEADER));
		bpPixel = (LPBYTE)pBuf + pBmpFileheader->bfOffBits;
		
		// A picture is saved at a buffer. 
		g_CVFMutex.Lock();
		memcpy(CpThis->m_BackSurface.vpBits, bpPixel, pBmpInfoheader->biSizeImage );
		g_CVFMutex.Unlock();
		// A picture is displayed. 
		g_CpVFThread->ResumeThread();

	}
	
	return	err;

 }

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  ViewFinderProc 
//  Description:    The thread for displaying a picture 
//  Parameters:     LPVOID vpParam                
//  Returns:        some UINT value
//  Summary:       
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
UINT WINAPI	CCameraConnect::ViewFinderProc( LPVOID	vpParam )
{
	CCameraConnect		*CpThis;
	BOOL			*fpEnd;
	//BOOL fEnd;  //Commented due to unreferenced variable
	HDC				hdcDest,hdcSrc;
	//CStatic *viewfinder;  //Commented due to unreferenced variable
	//DIBData backsurface;  Commented due to unfeferenced variable

	CpThis = (CCameraConnect*)vpParam;
	fpEnd = (BOOL*)(&CpThis->m_fVFEnd);
	
	while( *fpEnd )
	{
		/* It waits until it can use data. */
		g_CVFMutex.Lock();
		
		/* A picture is displayed. */
		hdcDest = ::GetDC( CpThis->m_CpViewFinder->GetSafeHwnd() );
		hdcSrc = ::CreateCompatibleDC( hdcDest );
		::SelectObject( hdcSrc, CpThis->m_BackSurface.hBmp );
		::BitBlt(	hdcDest,
					0,0,VIEWFINDER_WIDTH,VIEWFINDER_HEIGHT,
					hdcSrc,
					0,0,
					SRCCOPY );
		::DeleteDC( hdcSrc );
		::ReleaseDC( CpThis->m_CpViewFinder->GetSafeHwnd(), hdcDest );
		
		/* The ownership of data is released. */
		g_CVFMutex.Unlock();
		/* A thread is suspended. */
		g_CpVFThread->SuspendThread();
	}
	
	return	0;
}


//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  ConnectCamera 
//  Description:    Function that connects to camera 
//  Parameters:     CPictureWizardDlg * pCPictureWizardDlg - the dialog that calls this
//                  functions
//                  CStatic *m_cViewImage - picture box where viewfinder will be displayed                
//  Returns:        BOOL - true if successful, otherwise false
//  Summary:        Initially, GetCameraSource is called to get the camera information so we
//                  can connect to the camera called various cdSDK functions. One of the 
//                  obvious things that happens in this function is the camera turning on  
//  Author:         Adam Levy (adam.mlevy@gmail.com)
//                           
///////////////////////////////////////////////////////////////////////////
BOOL CCameraConnect::ConnectCamera(CPictureWizardDlg* pCPictureWizardDlg, CStatic *m_cViewImage)
{
	cdError			err;
	cdSourceInfo	SSrcInfo;
	cdRelDataKind	Kind;
	
	m_CpViewFinder = m_cViewImage;
	this->pCPictureWizardDlg = pCPictureWizardDlg;
	
	err = GetCameraSource(&SSrcInfo);

	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}
    
	if(SSrcInfo.SurceType == cdSRC_TYPE_INVALID)
	{	
		return false;
	}
	
	/* A device is opened. */
	err = CDOpenSource( &SSrcInfo, &m_hSource );
	
	
	
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}

	/* UI is locked so that information may not be changed. */
	//err = CDLockUI( m_hSource );
	//if( GETERRORID(err) != cdOK )
	//{
	//	goto	camerr;
	//}

	/* The function which receives the event from a camera is set up. */
	//err = CDRegisterEventCallbackFunction( m_hSource, CamCallBackFunc, (cdContext)this, &m_hCallbackFunction );
	//if( GETERRORID(err) != cdOK )
	//{
	//	goto	camerr;
	//}

	/* The existence of memory card is checked. */
	cdHEnum			hEnumVol;
	cdHVolume		hVolume;
	cdVolumeInfo	SVolInfo;
	
	m_fMCard = FALSE;
	/*Volumes are enumerated.*/
	err = CDEnumVolumeReset( m_hSource, &hEnumVol );
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return FALSE;
	}
	
	/* It repeats until it enumerates all volumes. */
	while( (err = CDEnumVolumeNext( hEnumVol, &hVolume )) == cdOK )
	{
		/* The information on volume is acquired. */
		err = CDGetVolumeInfo( hVolume, &SVolInfo );
		if( GETERRORID(err) != cdOK )
		{
			CDEnumVolumeRelease( hEnumVol );
			CameraError(err);
			return false;
		}
		
		if( SVolInfo.TotalSpace )
		{
			m_fMCard = TRUE;
			break;
		}
	}
	
	//if( GETERRORID(err) != cdOK && GETERRORID(err) != cdENUM_NA)
	//{
	//	CDEnumVolumeRelease( hEnumVol );
	//	goto	camerr;
	//}
	
	/* Listing of volume is ended. */
	err = CDEnumVolumeRelease( hEnumVol );
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}

	cdHEnum		hEnum;
	cdUInt32	bufsize;
	cdChar		ModelName[32];
	err = CDEnumDevicePropertyReset(m_hSource, 0, &hEnum);
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}

	bufsize = sizeof(m_RelControlCap);
	//err = CDGetDevicePropertyData(m_hSource, cdDEVICE_PROP_RELEASE_CONTROL_CAP, &bufsize, &m_RelControlCap, 0);
	//if( GETERRORID(err) != cdOK )
	//{
	//	CDEnumDevicePropertyRelease(hEnum);
	//	goto	camerr;
	//}
	//if( m_RelControlCap&cdRELEASE_CONTROL_CAP_SUPPORT==0 ){
	//	err = cdNOT_SUPPORTED;
	//	goto camerr;
	//}

	bufsize = sizeof(ModelName);
	memset(ModelName, 0, bufsize);
	err = CDGetDevicePropertyData(m_hSource, cdDEVICE_PROP_MODEL_NAME, &bufsize, ModelName, 0);
	if( GETERRORID(err) != cdOK )
	{
		CDEnumDevicePropertyRelease(hEnum);
		CameraError(err);
		return false;
	}
	if( strstr(ModelName, "EOS") )
	{
		m_fCamTypeEOS = TRUE;
	}
	else
	{
		m_fCamTypeEOS = FALSE;
	}

	err = CDEnumDevicePropertyRelease(hEnum);
	hEnum = NULL;
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}

	/* A camera is set as remote release control mode. */
	//err = CDEnterReleaseControl( m_hSource, RelCallBackFunc, (cdContext)this );
	err = CDEnterReleaseControl( m_hSource, RelCallBackFunc, (cdContext)pCPictureWizardDlg );
	
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}
	
	// window commands
	//m_CConnect.EnableWindow( FALSE );
	//m_CDisconnect.EnableWindow( TRUE );
	//m_CRelChk1.EnableWindow( TRUE );
	//m_CRelChk2.EnableWindow( TRUE );

	if( m_RelControlCap&cdRELEASE_CONTROL_CAP_VIEWFINDER )
	{	
		// window commands
		//m_CViewFinder.EnableWindow( TRUE );
		//m_CAEAF.EnableWindow( TRUE );
	}

	//err = SetReleaseState();
	Kind =  cdREL_KIND_PICT_TO_PC;
	if( Kind )
		{
			// The Kind of Release is set up. 
			err = CDSelectReleaseDataKind( m_hSource, Kind );
			if( GETERRORID(err) != cdOK )
			{
				CameraError(err);
				return false;
			}
		}
	if( GETERRORID(err) != cdOK )
	{
		CDExitReleaseControl( m_hSource );
		CameraError(err);
		return false;
	}
	
	// change image size
	cdImageSizeSpec pSpec;
	cdUInt32 Option = 0;
	cdHEnum phEnum;
	cdUInt32 pCount;
	cdCompQuality pQuality;
	cdImageSize pSize;
	int index;
	
	// Prepare enumeration of image sizes
	err = CDEnumSupportedImageSizeReset(m_hSource, Option, &phEnum);
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}
	// Get count of image sizes 
	err = CDGetSupportedImageSizeCount(phEnum, &pCount);
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}
	
	for (index = 0; index<pCount; index++)
	{
		err = CDEnumSupportedImageSizeNext(phEnum, &pSpec);
		if( GETERRORID(err) != cdOK )
		{
			CameraError(err);
			return false;
		}
		// Found ideal image size
		if (pSpec.Size.Width == 640 && pSpec.Size.Height == 480)
		{
			break;
		}

	}
	// Release enumeration of image sizes
	CDEnumSupportedImageSizeRelease(phEnum);

	// Prepare enumeration of image quality
	CDEnumImageFormatAttributeReset(m_hSource, &phEnum);
	// Get count of image qualities 
	CDGetImageFormatAttributeCount(phEnum, &pCount);
	
	for(index = 0; index<pCount; index++)
	{
		err = CDEnumImageFormatAttributeNext(phEnum, &pQuality, &pSize); 
		if( GETERRORID(err) != cdOK )
		{
			CameraError(err);
			return false;
		}
		// We found the ideal size so exit
		if (pSize == pSpec.ImageSize)
			break;
	}

	CDSetImageFormatAttribute(m_hSource, pQuality, pSize);
	
	err = CDEnumImageFormatAttributeRelease(phEnum);
	if( GETERRORID(err) != cdOK )
	{
		CameraError(err);
		return false;
	}
	CDSelectViewFinderCameraOutput(m_hSource, cdREL_FINDER_TO_LCD);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  SetReleaseState 
//  Description:    (I found no use for this function, but it may come in
//                   handy later)  
//  Parameters:                    
//  Returns:        a cdError value
//  Summary:       
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
cdError CCameraConnect::SetReleaseState()
{
	cdError			err=cdOK;
	/*
	int				i;
	long			lStyle;
	cdRelDataKind	Kind;
	CButton			*CpButtonList[4] = {	&m_CRelChk1,
											&m_CRelChk2,
											&m_CRelChk3,
											&m_CRelChk4 };
	cdRelDataKind	KindList[4] = {	cdREL_KIND_THUMB_TO_PC,
									cdREL_KIND_PICT_TO_PC,
									cdREL_KIND_THUMB_TO_CAM,
									cdREL_KIND_PICT_TO_CAM };
	

	if( m_fCamTypeEOS && !m_CRelChk1.GetCheck() && !m_CRelChk2.GetCheck() )
	{
		m_CRelChk3.EnableWindow( FALSE );
		m_CRelChk4.EnableWindow( FALSE );
		m_CRelChk3.SetCheck( 0 );
		m_CRelChk4.SetCheck( 0 );
	}
	else
	{
		m_CRelChk3.EnableWindow( m_fMCard );
		m_CRelChk4.EnableWindow( m_fMCard );
	}

	// The state of a release button is set up. 
	lStyle = GetWindowLong( m_CDisconnect.m_hWnd, GWL_STYLE );
	if( (m_CRelChk4.GetCheck() || m_CRelChk3.GetCheck()
			|| m_CRelChk2.GetCheck() || m_CRelChk1.GetCheck()) && !(lStyle&WS_DISABLED) )
	{
		m_CRelease.EnableWindow( TRUE );
	}
	else
	{
		m_CRelease.EnableWindow( FALSE );
	}
	
	if( m_hSource )
	{]
		 The Kind of Release is acquired. 
		Kind = 0;
		for( i=0 ; i<4 ; i++ )
		{
			if( CpButtonList[i]->GetCheck() )
			{
				Kind |= KindList[i];
			}
		}
		
		if( Kind )
		{
			// The Kind of Release is set up. 
			err = CDSelectReleaseDataKind( m_hSource, Kind );
			if( GETERRORID(err) != cdOK )
			{
				return	err;
			}
		}
	}
	*/
	return	err;
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  DisconnectCamera 
//  Description:    Disconnects the camera
//  Parameters:                    
//  Returns:        void
//  Summary:        Disconnecting the camera involves calling the 
//                  CDExitReleaseControl (which turns the camera off)
//                  and calling CDCloseSource (which closes the camera source)
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
void CCameraConnect::DisconnectCamera()
{
	cdError err;

	if( m_fVFEnd )
	{
		/* A view finder is ended. */
		err = CDTermViewfinder( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
		//	goto	camerr;
		}
		
		/* A thread is ended. */
		m_fVFEnd = FALSE;
		g_CpVFThread->ResumeThread();
		WaitForSingleObject( g_CpVFThread->m_hThread, INFINITE );
		
		// window related command
		//m_CViewFinder.SetWindowText( "Start" );
		//Invalidate();
		//UpdateWindow();	
	}
	if( m_hSource )
	{
		/* Remote release control mode is ended. */
		err = CDExitReleaseControl( m_hSource );
		// if( GETERRORID(err) != cdOK )
		//{
		//	goto	camerr;
		//}
		
		/* The function which receives the event from a camera is canceled. */
		//if( m_hCallbackFunction )
		//{
		//	err = CDUnregisterEventCallbackFunction( m_hSource, m_hCallbackFunction );
		//	if( GETERRORID(err) != cdOK )
		//	{
		//		goto	camerr;
		//	}
		//	m_hCallbackFunction = NULL;
		//}
		
		/* A device is closed. */
		err = CDCloseSource( m_hSource );
		//if( GETERRORID(err) != cdOK )
		//{
		//	goto	camerr;
		//}
		
		//m_hSource = NULL;
	}
	
	
	
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  StartViewFinder 
//  Description:    Turns the viewfinder on the camera and also ends it
//					when necessary
//  Parameters:                    
//  Returns:        void
//  Summary:        
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
void CCameraConnect::StartViewfinder()
{
	cdError			err;
	cdStgMedium		VFMedium;

	/* UI is locked so that information may not be changed. */
	err = CDLockUI( m_hSource );
	if( GETERRORID(err) != cdOK )
	{
	//	goto	camerr;
	}

	if( !m_fVFEnd )
	{
		/* The thread which displays view finder is created. */
		m_fVFEnd = TRUE;
		g_CpVFThread = AfxBeginThread(	(AFX_THREADPROC)ViewFinderProc,
										(LPVOID)this,
										0,
										0,
										CREATE_SUSPENDED,
										NULL );
		
		/* A view finder is started. */
		VFMedium.Type = cdMEMTYPE_STREAM;
		err = CDStartViewfinder(	m_hSource,
									FILEFORMAT_BMP,
									ViewFinderCallBackFun,
									(cdContext)this );
		if( GETERRORID(err) != cdOK )
		{
			//goto	camerr;
		}
		
		//m_CViewFinder.SetWindowText( "Stop" );
	}

	else
	{
		/* A view finder is ended. */
		err = CDTermViewfinder( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
			printf("");
			//goto	camerr;
		}
		
		/* A thread is ended. */
		m_fVFEnd = FALSE;
		g_CpVFThread->ResumeThread();
		WaitForSingleObject( g_CpVFThread->m_hThread, INFINITE );
		
		//m_CViewFinder.SetWindowText( "Start" );		
		//Invalidate();
		//UpdateWindow();	

		/* The lock of UI is canceled. */
		err = CDUnlockUI( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
			//	goto	camerr;
		}
	}
	
}


//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  CreateDIBBuffer 
//  Description:    Sets up the image specifications  
//  Parameters:                    
//  Returns:        BOOL - returns true if ok, false otherwise
//  Summary:       
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
BOOL CCameraConnect::CreateDIBBuffer()
{
	cdError		err=cdOK;
	BITMAPINFO	BmpInfo;
		
	memset( &BmpInfo, 0, sizeof(BITMAPINFO) );
	BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BmpInfo.bmiHeader.biWidth = VIEWFINDER_WIDTH;
	BmpInfo.bmiHeader.biHeight = VIEWFINDER_HEIGHT;
	BmpInfo.bmiHeader.biPlanes = 1;
	BmpInfo.bmiHeader.biBitCount = 24;
	BmpInfo.bmiHeader.biCompression = BI_RGB;
	
	m_BackSurface.hBmp = ::CreateDIBSection(	NULL,
												&BmpInfo,
												DIB_RGB_COLORS,
												&m_BackSurface.vpBits,
												NULL,
												0 );
	if( !m_BackSurface.hBmp )
	{
		return	FALSE;
	}
	
	return	TRUE;
}



//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  SetReleaseState 
//  Description:    Takes and stores picture
//  Parameters:                    
//  Returns:        void
//  Summary:        When this function is called, the picture is first taken
//                  and then stored by calling GetReleaseData.   
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
void CCameraConnect::StartRelease(char* szSavePath)
{
	cdError		err;
	BOOL		fRes;
	CProgress	CProg;
	//char		szSavePath[MAX_PATH];
	cdUInt32	NumData;
	
	/* UI is locked so that information may not be changed. */
	err = CDLockUI( m_hSource );
	if( GETERRORID(err) != cdOK )
	{
	//	goto	camerr;
	}
	
	/* It sets up so that a complete message may be disregarded. */
	m_fProgramRelease = TRUE;
	
	/* A special camera ends a view finder. */
	if( m_RelControlCap&cdRELEASE_CONTROL_CAP_ABORT_VIEWFINDER )
	{
		if( m_fVFEnd )
		{
			/* A view finder is ended. */
			err = CDTermViewfinder( m_hSource );
			if( GETERRORID(err) != cdOK )
			{
			//	goto	camerr;
			}
			
			/* A thread is ended. */
			m_fVFEnd = FALSE;
			g_CpVFThread->ResumeThread();
			WaitForSingleObject( g_CpVFThread->m_hThread, INFINITE );
			
			//m_CViewFinder.SetWindowText( "Start" );
//			Invalidate();
//			UpdateWindow();	
		}
	}
	
	/* A photograph is taken. */
	NumData = 0;
	err = CDRelease( m_hSource, FALSE, NULL, NULL, cdPROG_NO_REPORT, &NumData );
	if( GETERRORID(err) != cdOK )
	{
	//	goto	camerr;
	}
	
	/* The directory to save is acquired. */
	//GetSavePath( szSavePath, MAX_PATH );
	
	/* The photoed picture is saved. */
	fRes = CProg.GetReleaseData( m_hSource, NumData, szSavePath, this );
	if( !fRes )
	{
	//	goto	apierr;
	}
	else if( GETERRORID(CProg.m_LastErr) == cdOPERATION_CANCELLED )
	{
		m_fProgramRelease = FALSE;
		CDUnlockUI( m_hSource );
		return;
	}
	else if( GETERRORID(CProg.m_LastErr) != cdOK )
	{
		err = CProg.m_LastErr;
	//	goto	camerr;
	}
	
	/* The lock of UI is canceled. */
	err = CDUnlockUI( m_hSource );
	if( GETERRORID(err) != cdOK )
	{
	//	goto	camerr;
	}
	
	/* It sets up so that a complete message may be received. */
	m_fProgramRelease = FALSE;
	return;
	
	


}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  WindowProc 
//  Description:    Used for handling windows messages 
//  Parameters:     UINT message
//                  WPARAM wParam
//                  LPARAM lparam
//                  char *cpBuffer               
//  Returns:        BOOL
//  Summary:        This functions gets called in the CPictureWizardDlg's WindowProc
//                  Within the CPictureWizardDlg's WindowProc functions, we are 
//                  also calling this function since there is a possibility that 
//                  the release button was pressed.  The 'message' tells us what sort of 
//                  message was generated prior to calling WindowProc.  If a picture is 
//                  taken, the ReleaseCallBackFunction will be called setting the message
//                  to g_ReleaseOnMessage 
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
BOOL CCameraConnect::WindowProc(UINT message, WPARAM wParam, LPARAM lParam, char* cpBuffer)
{
	cdError		err;
	BOOL		fRes;
	CProgress	CProg;
	//char		szSavePath[MAX_PATH];
	
	cdUInt32	NumData;
	
	if( message == g_ReleaseOnMessage )
	{
		/* UI is locked so that information may not be changed. */
		err = CDLockUI( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
			//goto	camerr;
		}
		
		/* It sets up so that a complete message may be disregarded. */
		m_fProgramRelease = TRUE;
	
		/* A special camera ends a view finder. */
		if( m_RelControlCap&cdRELEASE_CONTROL_CAP_ABORT_VIEWFINDER )
		{
			if( m_fVFEnd )
			{
				/* A view finder is ended. */
				err = CDTermViewfinder( m_hSource );
				if( GETERRORID(err) != cdOK )
				{
					//goto	camerr;
				}
		
				/* A thread is ended. */
				m_fVFEnd = FALSE;
				g_CpVFThread->ResumeThread();
				WaitForSingleObject( g_CpVFThread->m_hThread, INFINITE );
				
				// this is something that will have to be done on the dlg
				//m_CViewFinder.SetWindowText( "Start" );				
				//Invalidate();
				//UpdateWindow();
			}
		}
		
		/* A photograph is taken. */
		NumData = 0;		
		err = CDRelease( m_hSource, FALSE, NULL, NULL, cdPROG_NO_REPORT, &NumData );
		if( GETERRORID(err) != cdOK )
		{
			//goto	camerr;
		}
		
		/* The directory to save is acquired. */
		// this is actually done in the dlg
		//GetSavePath( szSavePath, MAX_PATH );
		
		//szSavePath = cpBuffer;

		/* The photoed picture is saved. */
		//fRes = CProg.GetReleaseData( m_hSource, NumData, cpBuffer );
		
		// Determine if we should take the left picture or the right 
		if (pCPictureWizardDlg->horizontal) // left picture
		{
			pCPictureWizardDlg->horizontal = false;
			
			SetFileName(true);
			fRes = CProg.GetReleaseData( m_hSource, NumData, FileName, this );
		}
		else // right picture
		{
			SetFileName(false);
			fRes = CProg.GetReleaseData( m_hSource, NumData, FileName, this );
		 
		}

		if( !fRes )
		{
			//goto	apierr;
		}
		else if( GETERRORID(CProg.m_LastErr) == cdOPERATION_CANCELLED )
		{
			m_fProgramRelease = FALSE;
			CDUnlockUI( m_hSource );
			return	TRUE;
		}
		else if( GETERRORID(CProg.m_LastErr) != cdOK )
		{
			err = CProg.m_LastErr;
			//goto	camerr;
		}
		
		/* The lock of UI is canceled. */
		err = CDUnlockUI( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
			//goto	camerr;
		}
		
		/* It sets up so that a complete message may be received. */
		m_fProgramRelease = FALSE;
		
		//bPictDone = TRUE;
		//PictureDoneWait();
		return	TRUE;
	}
	return TRUE;	
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  GetCameraSource 
//  Description:    Gets the camera's source information  
//  Parameters:                    
//  Returns:        a cdError value
//  Summary:        ConnectCamera calls this function, passing in a pointer
//                  to the SourceInfo.  ConnectCamera will eventually
//                  use the cdSourceInfo to connect to the camera.
//  Author:         CannonSDK
//                  Modified by Adam Levy (adam.mlevy@gmail.com)
//                           
///////////////////////////////////////////////////////////////////////////
cdError CCameraConnect::GetCameraSource(cdSourceInfo *pSourceInfo)
{
	memset( &m_SelectedSrc, 0, sizeof(cdSourceInfo) );

	cdError		err;
	cdHEnum		hEnumDevice;
	cdUInt32 	count;
	//cdUInt32	index;

	cdSourceInfo* pSourceInfoTmp;
	    

    err = CDEnumDeviceReset(1, &hEnumDevice);
    
	if( GETERRORID(err) != cdOK )
	{
		return err;
	}
    err = CDGetDeviceCount(hEnumDevice, &count);
    if( GETERRORID(err) != cdOK )
	{
		return err;
	}
	else if(count == 0)
	{
		return 23;
		// since there are no devices we should be doing something
	}
	else
	{	
		
		pSourceInfoTmp= new cdSourceInfo;

		err = CDEnumDeviceNext(hEnumDevice, pSourceInfoTmp);
		if(!err)
		{
			//m_CameraList.InsertString(index, pSourceInfo->Name);
			//m_CameraList.SetItemDataPtr( index, pSourceInfo );
		}
	}

    
	
    err = CDEnumDeviceRelease(hEnumDevice);
	if( GETERRORID(err) != cdOK )
	{
		return err;
	}
	hEnumDevice = 0;

	if(pSourceInfoTmp && (int)pSourceInfoTmp != CB_ERR)
	{
		memcpy( &m_SelectedSrc, pSourceInfoTmp, sizeof(cdSourceInfo) );
	}
    //if(err) goto ErrHandler;
	
	memcpy( pSourceInfo, &m_SelectedSrc, sizeof(cdSourceInfo) );
	
	return err;
//ErrHandler:
//
 //   if(hEnumDevice)
 //       CDEnumDeviceRelease (hEnumDevice);
	
//	char	szErrStr[256];
//	wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
//	MessageBox( szErrStr );
	
//	EndDialog(0);

//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  CameraError 
//  Description:    Called when any cdSDK functions has reported an error 
//  Parameters:     cdError err - the reported error               
//  Returns:        void
//  Summary:        This functions was written for the purpose of having 
//                  a specific way of dealing with errors generated by the 
//                  cdSDK.  The error codes and their meaning are located 
//                  in the cdError.h file.  
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
void CCameraConnect::CameraError(cdError err)
{
	if(err != cdNOT_SUPPORTED ){
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		//MessageBox( szErrStr );
	}

	if( m_hSource )
	{
		//if( m_hCallbackFunction )
		//{
		//	CDUnregisterEventCallbackFunction( m_hSource, m_hCallbackFunction );
		//	m_hCallbackFunction = NULL;
		//}
		
		CDUnlockUI( m_hSource );
		CDCloseSource( m_hSource );
		m_hSource = NULL;
	}
	
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  EndCamera
//  Description:    Called when the main dialog is closed.    
//  Parameters:                    
//  Returns:        BOOL - true if end ok, false otherwise
//  Summary:       
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////

BOOL CCameraConnect::EndCamera()
{
	cdError			err;
	
	
	if( m_fVFEnd )
	{
		/* A view finder is ended. */
		err = CDTermViewfinder( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
			goto	camerr;
		}
		
		/* A thread is ended. */
		m_fVFEnd = FALSE;
		g_CpVFThread->ResumeThread();
		WaitForSingleObject( g_CpVFThread->m_hThread, INFINITE );
	}
	
	if( m_hSource )
	{
		/* Remote release control mode is ended. */
		err = CDExitReleaseControl( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
			goto	camerr;
		}
		
		if( m_hCallbackFunction )
		{
			/* The function which receives the event from a camera is canceled. */
			err = CDUnregisterEventCallbackFunction( m_hSource, m_hCallbackFunction );
			if( GETERRORID(err) != cdOK )
			{
				goto	camerr;
			}
			m_hCallbackFunction = NULL;
		}
		
		/* A device is closed. */
		err = CDCloseSource( m_hSource );
		if( GETERRORID(err) != cdOK )
		{
			goto	camerr;
		}
		
		m_hSource = NULL;
	}
	
	/* A picture buffer is deleted. */
	if( m_BackSurface.hBmp )
	{
		DeleteObject( m_BackSurface.hBmp );
		m_BackSurface.hBmp = NULL;
	}
	
	//EndDialog( 0 );	
	return TRUE;
	
camerr:
	char	szErrStr[256];
	
	wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
	//MessageBox( szErrStr );
	
	if( m_fVFEnd )
	{
		CDTermViewfinder( m_hSource );
		m_fVFEnd = FALSE;
		g_CpVFThread->ResumeThread();
		WaitForSingleObject( g_CpVFThread->m_hThread, INFINITE );
	}
	
	if( m_hSource )
	{
		CDExitReleaseControl( m_hSource );
		if( m_hCallbackFunction )
		{
			CDUnregisterEventCallbackFunction( m_hSource, m_hCallbackFunction );
			m_hCallbackFunction = NULL;
		}
		CDCloseSource( m_hSource );
		m_hSource = NULL;
	}
	
	if( m_BackSurface.hBmp )
	{
		DeleteObject( m_BackSurface.hBmp );
		m_BackSurface.hBmp = NULL;
	}
	
	//EndDialog( 0 );
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  PictureDoneWait
//  Description:    Called by PictureWizardDialog to see if picture is done being
//                  stored.      
//  Parameters:                    
//  Returns:        BOOL - true if picture is done being stored, false
//                  otherwise
//  Summary:        The PictureWizardDialog needs to know if the picture
//                  has been stored yet.  We use a semaphor paradigm (signal
//					and wait) to see if the picture has been stored.  
//  Author:         Adam Levy (adam.mlevy@gmail.com)
//                           
///////////////////////////////////////////////////////////////////////////
BOOL CCameraConnect::PictureDoneWait()
{
	return bPictDone;
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  PictureDoneSignal
//  Description:    Called by CProgress to to indicate that 
//                  the picture has been stored.      
//  Parameters:                    
//  Returns:        void
//  Summary:        The CProgress dialog needs to tell us when it is 
//                  finished storing the image.  Once it is complete, 
//                  this function is called with a true parameter. Upon
//					signaling, the PictureWizardDlg now has control
//                  over the image (like storing it to the dialog box).  
//  Author:         Adam Levy (adam.mlevy@gmail.com)
//                           
///////////////////////////////////////////////////////////////////////////
void CCameraConnect::PictureDoneSignal(BOOL done)
{

	bPictDone = done;
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  SetPathAndFileName
//  Description:    Sets the full path and filename (with extension) 
//                  of the two images to be stored    
//  Parameters:     char *PicNameLeft - the left image (horizontal)
//                  char *PicNameRight - the right image (vertical)               
//  Returns:        BOOL - might need some error checking in here.  
//  Summary:        The PictureWizardDlg will generate the file and path 
//                  names, and pass them to this function so we can later pass
//                  them to the CProgress dialog (which is where the image is 
//                  stored)
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
BOOL CCameraConnect::SetPathName(CString NewPathName )
{


	PathName = NewPathName;
	

	return TRUE;
}

BOOL CCameraConnect::SetFileName(BOOL horizontal){

	//add the count
	// create two files names for the left and right image

	if ( horizontal )
	{
		hcount++;
		str_hcount.Format("%d", hcount);
		strcpy(FileName, (char*)(LPCTSTR)(PathName + "H"+ str_hcount + ".jpg") );
		strcpy(FileNameHorizontal, FileName);
		//FileName = *(char*)(LPCTSTR)(PathName + "H"+ str_hcount + ".jpg");
	}
	else{
		vcount++;
		str_vcount.Format("%d", vcount);
		strcpy(FileName, (char*)(LPCTSTR)(PathName + "V"+ str_vcount + ".jpg") );
		strcpy(FileNameVertical, FileName);
		//FileName = *(char*)(LPCTSTR)(PathName + "V"+str_vcount + ".jpg");
		;

	}
	//PathName = cur_path+"\\"+my_student->GetPropertyValue("Name");//+"H"+ str_hcount + ".jpg";
	//PathNameRight = cur_path+"\\"+my_student->GetPropertyValue("Name");//+"V"+str_vcount + ".jpg";
	


	//strcpy(FilePathLeft, PicNameLeft);
	//strcpy(FilePathRight, PicNameRight);
	return TRUE;


}


char* CCameraConnect::GetLastVerticalFile ()
{
	return FileNameVertical;
}
char * CCameraConnect::GetLastHorizontalFile()
{
	return FileNameHorizontal;
}