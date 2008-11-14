// Progress1.cpp : implementation file
//

#include "stdafx.h"
#include "DVT.h"
#include "Progress.h"
#include "MyStream.h"
#include "CameraConnect.h"

#include "cdAPI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef		GETERRORID
#define		GETERRORID( x )		(cdERROR_ERRORID_MASK&x)
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgress dialog


CProgress::CProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CProgress::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(CProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_CProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgress, CDialog)
	//{{AFX_MSG_MAP(CProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgress message handlers

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  OnInitDialog 
//  Description:    Called when dialog is created 
//  Parameters:     none               
//  Returns:        BOOL
//  Summary:        Once the dialog is created, we also want to start
//				    the thread.  
//  Author:         CannonSDK
//					Modified by Adam Levy (adam.mlevy@gmail.com)
//                           
///////////////////////////////////////////////////////////////////////////
BOOL CProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// TODO: Add extra initialization here
	m_LastErr = cdOK;
	
	/* Progress control is changed the first stage. */
	m_CProgress.SetRange( 0, 100 );
	SetProgressPos( 0 );
	
	/* Negatives are Release using a thread. */
	AfxBeginThread(	(AFX_THREADPROC)ReleaseThreadProc,
					(LPVOID)this );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  ReleaseProgressFunc 
//  Description:    The definition of the callback function for progress  
//  Parameters:     cdUInt32 Progress
//					cdProgressStatus Status
//					cdContext Context               
//  Returns:        cdUInt32 - currently returns a cdOK but should 
//					implement error checking
//  Summary:        This function is called to update the progress bar.  
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
cdUInt32 cdSTDCALL CProgress::ReleaseProgressFunc(	cdUInt32			Progress,
													cdProgressStatus	Status,
													cdContext			Context )
{
	CProgress	*CpThis;

	CpThis = (CProgress*)Context;
	CpThis->SetProgressPos( Progress );
	return	cdOK;
}

/* The function performed by the thread */

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  ReleaseThreadProc 
//  Description:    Thread for storing the image  
//  Parameters:     LPVOID lpParameter               
//  Returns:        UINT - now returns 0, but should return something else
//  Summary:        This is the thread that is used for storing the image.
//                    
//  Author:         CannonSDK
//                  Modified by Adam Levy (adam.mlevy@gmail.com)         
///////////////////////////////////////////////////////////////////////////
UINT 	CProgress::ReleaseThreadProc( LPVOID	lpParameter )
{
	cdStgMedium					MyMedium;
	cdReleaseImageInfo			RelImgInfo;
	char						szFileName[MAX_PATH],szExt[MAX_PATH],szPrefix[MAX_PATH];
	static	int					i=0;
	CProgress					*CpThis;
	cdUInt32					NumData;
	cdStream					FilStream;
	
	CpThis = (CProgress*)lpParameter;
		
	for( NumData=CpThis->m_NumData; NumData>0; NumData--, i++ )
	{
		/* A preservation file name is decided. */
		if( CDGetReleasedData(	CpThis->m_hSource,
								NULL,
								0,
								0,
								&RelImgInfo,
								NULL ) != cdOK )
		{
			
			break;
		}
		
		switch( RelImgInfo.DataType )
		{
			case cdDATA_TYPE_THUMBNAIL:
				strcpy(szPrefix, "TH");
				strcpy(szExt, "JPG");
				break;
			case cdDATA_TYPE_PICTURE:
				strcpy(szPrefix, "FV");
				if(RelImgInfo.Format == 1)
				{
					strcpy(szExt, "JPG");
				}
				else
				{
					strcpy(szExt, "CRW");
				}
				break;
			case cdDATA_TYPE_PLUS_JPEG:
				strcpy(szPrefix, "FP");
				strcpy(szExt, "JPG");
				break;
		}
		
		wsprintf( szFileName, "%s%s%06d.%s",
				CpThis->m_szSavePath, szPrefix, i, szExt );
		strcpy(szFileName, CpThis->m_szSavePath);
		/* A stream is initialized. */
		CreateMyFilStream( &FilStream, szFileName );
			
		MyMedium.Type = cdMEMTYPE_STREAM;
		MyMedium.u.pStream = &FilStream;
		/* Data is acquired. */
		CpThis->m_LastErr = CDGetReleasedData(	CpThis->m_hSource,
												ReleaseProgressFunc,
												(cdContext)CpThis,
												cdPROG_REPORT_PERIODICALLY,
												&RelImgInfo,
												&MyMedium );
		/* A stream is released. */
		ReleaseMyFilStream( &FilStream );
		if( GETERRORID(CpThis->m_LastErr) != cdOK )
		{
			break;
		}
		//strcpy( CpThis->FilePath, szFileName);
		strcpy( CpThis->m_szSavePath, szFileName);
		
	}
	
	CpThis->PostMessage( CpThis->m_ThreadEndMessage );
	return	0;
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  SetProgressPos 
//  Description:    Sets the position of the progress bar  
//  Parameters:     int iPos - position to set the progress bar at
//					             
//  Returns:        void
//  Summary:        This function is called by ReleaseProgressFunc, 
//                  which is the callback used for updating the progress 
//					bar  
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
void CProgress::SetProgressPos( int		iPos )
{
	m_CProgress.SetPos( iPos );
}

//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  GetReleaseData 
//  Description:    Prepares for storing the image.    
//  Parameters:     cdHSource hSource
//					cdUInt32 NumData
//					char *szSavePath - either horizontal or vertical image
//					void *theCamera - instance of our camera object              
//  Returns:        BOOL - true if no errors, false otherwise
//  Summary:        This function is called by the WindowProc in CCameraConnect
//					when a picture is taken.    
//  Author:         CannonSDK
//                           
///////////////////////////////////////////////////////////////////////////
BOOL	CProgress::GetReleaseData(	cdHSource	hSource,
									cdUInt32	NumData,
									char		*szSavePath, 
									void *theCamera)
{
	/* Data is set. */
	my_theCamera = theCamera;
	m_hSource = hSource;
	m_NumData = NumData;
	strcpy( m_szSavePath, szSavePath );
	
	m_ThreadEndMessage = RegisterWindowMessage( "Information of a thread end" );
	if( m_ThreadEndMessage == 0 )
	{
		return	FALSE;
	}
	
	/* A progress dialog is displayed and developed. */
	if( DoModal() != IDOK )
	{
		return	FALSE;
	}
	
	//strcpy( szSavePath, FilePath);
	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////
// 
//  Function Name:  WindowProc 
//  Description:    Message handler for dialog box  
//  Parameters:     UINT message
//					WPARAM wParam
//					LPARAM lParam               
//  Returns:        LRESULT
//  Summary:        WindowProc is called many times during the life of the
//					dialog box.  Everytime we call the WindowProc, we are 
//					checking if the message is m_ThreadEndMessage.  This will 
//					only occur when the thread is complete.  Once this happens,
//					we end the dialog and call PictureDoneSignal to
//					indicate that we are done storing the image.  				  
//  Author:         Adam Levy (adam.mlevy@gmail.com)
//                           
///////////////////////////////////////////////////////////////////////////
LRESULT CProgress::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( m_ThreadEndMessage == message )
	{
		EndDialog( IDOK );
		((CCameraConnect*) my_theCamera)->PictureDoneSignal(TRUE);
		return	TRUE;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}
