// Result.cpp: implementation of the Result class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DVT.h"
#include "Result.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Result::Result()
{

}

Result::~Result()
{

}

CString Result::GetResult()
{
	return my_result;
}
