// CameraConnect.h: interface for the CCameraConnect class.
//
//////////////////////////////////////////////////////////////////////
	 


#if !defined(AFX_CAMERACONNECT_H__5320DF90_7AE1_4DCA_A85F_BFEE6932E1E5__INCLUDED_)
#define AFX_CAMERACONNECT_H__5320DF90_7AE1_4DCA_A85F_BFEE6932E1E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* Information on a picture */
typedef	struct	tagDIBData
{
	HBITMAP		hBmp;
	LPVOID		vpBits;
}DIBData;


#include "cdAPI.h"

#define		FILEFORMAT_JPG			0
#define		FILEFORMAT_BMP			1
#include "PictureWizardDlg.h"

class CCameraConnect  
{
public:
	void PictureDoneSignal(BOOL done);
	BOOL SetPathName( CString NewPathName );
	BOOL SetFileName( BOOL horizontal );
	void setH(BOOL h);
	void setV(BOOL v);
	BOOL getHSelect();
	BOOL getVSelect();

	char * GetLastVerticalFile ();
	char * GetLastHorizontalFile();
	//BOOL SetPathAndFileName(char *PicNameLeft, char *PicNameRight);
	BOOL bPictDone;
	BOOL PictureDoneWait();
	BOOL EndCamera();
	void CameraError(cdError err);
	cdError GetCameraSource(cdSourceInfo* pSourceInfo);
	BOOL WindowProc(UINT message, WPARAM wParam, LPARAM lParam, char* cpBuffer);
	void StartRelease( char* );
	BOOL CreateDIBBuffer();
	void StartViewfinder();
	void DisconnectCamera();
	cdError SetReleaseState();
	BOOL ConnectCamera(CPictureWizardDlg* pCPictureWizardDlg, CStatic *m_cViewImage);
	//CCameraConnect(CStatic *m_cViewImage);
	CCameraConnect();
	virtual ~CCameraConnect();

private:
	/* call back function */
	static cdUInt32	cdSTDCALL CamCallBackFunc(	cdEventID		EventID,
												const cdVoid*	pData,
												cdUInt32		DataSize,
												cdContext		Context );
	static cdUInt32	cdSTDCALL RelCallBackFunc(	cdReleaseEventID	EventID,
												const void *		pData,
												cdUInt32 			DataSize,
												cdContext			Context );
	static cdUInt32	cdSTDCALL ViewFinderCallBackFun(	cdVoid		*pBuf,
										 				cdUInt32	Size,	
														cdUInt32	Format,
														cdContext	Context );
	static UINT WINAPI	ViewFinderProc( LPVOID	vpParam );

private:
	int iPictureCount;                     // indicates how many pictures we have stored  
	
	char FileName[MAX_PATH];
	char FileNameHorizontal[MAX_PATH];    // file and path name of horizontal image 
	char FileNameVertical[MAX_PATH];   // file and pathname of vertical image 

	int hcount;
	int vcount;

	BOOL hselect;
	BOOL vselect; 

	CString str_hcount;
	CString str_vcount;

	cdSourceInfo m_SelectedSrc;            // the source defice info of the camera
	cdReleaseControlCap m_RelControlCap;   // (not sure)
	cdHandle m_hCallbackFunction;          // handle for device					   
	cdHSource m_hSource;			       // The handle of a camera 
	
	BOOL m_fMCard;	                       // true if there is a memory card in the camera false if not	
	BOOL m_fCamTypeEOS;                    // (not sure)
	BOOL m_fProgramRelease;	               // The release flag by the program 
	BOOL m_fVFEnd;			               // The flag of a view finder of operation 
    
	DIBData	m_BackSurface;		           // Information on a picture buffer 

	CStatic	*m_CpViewFinder;	           // The pointer to the class which shows the view finder picture 
    CStatic	m_cViewImage;				   // (not used)

	CPictureWizardDlg* pCPictureWizardDlg; // Pointer to camera dialog class 
	
};

#endif // !defined(AFX_CAMERACONNECT_H__5320DF90_7AE1_4DCA_A85F_BFEE6932E1E5__INCLUDED_)
