// Picture.cpp: implementation of the Picture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DVT.h"
#include "Pics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Pics::Pics()
{

}

Pics::Pics(CString path, CString timestamp)
{
	my_path = path;
	my_timestamp = timestamp;
}

Pics::~Pics()
{

}

// Accessesors
CString Pics::GetPath()
{
	return my_path;
}
CString Pics::GetTimeStamp()
{
	return my_timestamp;
}

// Mutators
void Pics::SetPath( CString path )
{
	my_path = path;
}
void Pics::SetTimeStamp( CString timestamp )
{
	my_timestamp = timestamp;
}