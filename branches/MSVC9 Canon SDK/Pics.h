// Picture.h: interface for the Picture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__108C5CCA_A7D4_4E3A_930B_349B8CCD4337__INCLUDED_)
#define AFX_PICTURE_H__108C5CCA_A7D4_4E3A_930B_349B8CCD4337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Pics  
{
public:

	// Accessesors
	CString Pics::GetPath();
	CString Pics::GetTimeStamp();

	// Mutators
	void Pics::SetPath( CString path );
	void Pics::SetTimeStamp( CString timestamp );

	Pics();
	Pics(CString path, CString timestamp);
	virtual ~Pics();

private:
	CString my_path;
	CString my_timestamp;

};

#endif // !defined(AFX_PICTURE_H__108C5CCA_A7D4_4E3A_930B_349B8CCD4337__INCLUDED_)
