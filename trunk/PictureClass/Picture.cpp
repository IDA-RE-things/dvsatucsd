////////////////////////////////////////////////////////////////
// MSDN Magazine -- October 2001
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0 for Windows 98 and probably Windows 2000 too.
// Set tabsize = 3 in your editor.
//
#include "StdAfx.h"
#include "Picture.h"
#include "AfxPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////
// CPicture implementation
//

CPicture::CPicture()
{
}

CPicture::~CPicture()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// Function: CPicture::Load(UINT nIDRes)
// Parameters:  UINT nIDRes - FILL IN
// Returns: TRUE - if Pictures loads correctly
//          FALSE - otherwise
// Description: Loads from resource, looks for "IMAGE" type
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPicture::Load(UINT nIDRes)
{
	// find resource in resource file
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst,
		MAKEINTRESOURCE(nIDRes),
		"IMAGE"); // type
	if (!hRsrc)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;

	// create memory file and load it
	CMemFile file(lpRsrc, len);
	BOOL bRet = Load(file);
	FreeResource(hRsrc);

	return bRet;
}

//////////////////
// Load from path name.
//
///////////////////////////////////////////////////////////////////////////////
//
// Function:  CPicture::Load(LPCTSTR pszPathName)
// Parameters:  LPCTSTR pszPathName - path to Picture
// Returns: TRUE - if load succeeds
//          FALSE - otherwise
// Description: Loads a picture from a path name
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPicture::Load(LPCTSTR pszPathName)
{
	CFile file;
	if (!file.Open(pszPathName, CFile::modeRead|CFile::shareDenyWrite))
		return FALSE;
	BOOL bRet = Load(file);
	file.Close();
	return bRet;
}


///////////////////////////////////////////////////////////////////////////////
//
// Function: CPicture::Load(CFile& file)
// Parameters:  CFile & file - CFile object to load
// Returns: FILL IN
// Description: Loads picture from CFILE object
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPicture::Load(CFile& file)
{
	CArchive ar(&file, CArchive::load | CArchive::bNoFlushOnDelete);
	return Load(ar);
}

///////////////////////////////////////////////////////////////////////////////
//
// Function:  CPicture::Load(CArchive& ar)
// Parameters:  CArchive & ar - CArchive object to load
// Returns: FILL IN
// Description: Loads picture from CArchive object.  Creates a stream and 
//              loads from stream
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPicture::Load(CArchive& ar)
{
	CArchiveStream arcstream(&ar);
	return Load((IStream*)&arcstream);
}

//////////////////
// Load from stream (IStream). This is the one that really does it: call
// OleLoadPicture to do the work.
//
///////////////////////////////////////////////////////////////////////////////
//
// Function: CPicture::Load(IStream* pstm)
// Parameters:  IStream * pstm - Istream to load picture from
// Returns: TRUE in all cases
// Description: Load from stream (IStream). This is the one that really does it: 
//              call OleLoadPicture to do the work.
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPicture::Load(IStream* pstm)
{
	Free();
	HRESULT hr = OleLoadPicture(pstm, 0, FALSE,
		IID_IPicture, (void**)&m_spIPicture);
	ASSERT(SUCCEEDED(hr) && m_spIPicture);	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// Function: CPicture::Render(CDC* pDC, CRect rc, LPCRECT prcMFBounds)
// Parameters:  CDC* pDC - FILL IN
//			    CRect rc - FILL IN
//			    LPCRECT prcMFBounds - FILL IN
// Returns: TRUE in all cases
// Description: Renders a picture to the screen I THINK.  Converts HIMETRIC
//              for IPICTURE
//
///////////////////////////////////////////////////////////////////////////////
BOOL CPicture::Render(CDC* pDC, CRect rc, LPCRECT prcMFBounds) const
{
	ASSERT(pDC);

	if (rc.IsRectNull()) {
		CSize sz = GetImageSize(pDC);
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	long hmWidth,hmHeight; // HIMETRIC units
	GetHIMETRICSize(hmWidth, hmHeight);
	m_spIPicture->Render(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
		0, hmHeight, hmWidth, -hmHeight, prcMFBounds);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
// Function: CPicture::GetImageSize(CDC* pDC) const
// Parameters:  CDC* pDC - FILL IN
// Returns:  CSize - size of image in pixels
// Description: Get image size in pixels. Converts from 
//              HIMETRIC to device coords.
//
///////////////////////////////////////////////////////////////////////////////
CSize CPicture::GetImageSize(CDC* pDC) const
{
	if (!m_spIPicture)
		return CSize(0,0);
	
	LONG hmWidth, hmHeight; // HIMETRIC units
	m_spIPicture->get_Width(&hmWidth);
	m_spIPicture->get_Height(&hmHeight);
	CSize sz(hmWidth,hmHeight);
	if (pDC==NULL) {
		CWindowDC dc(NULL);
		dc.HIMETRICtoDP(&sz); // convert to pixels
	} else {
		pDC->HIMETRICtoDP(&sz);
	}
	return sz;
}

