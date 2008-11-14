// Result.h: interface for the Result class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULT_H__CFC75076_AC72_46B8_94AC_E20F8DE6C229__INCLUDED_)
#define AFX_RESULT_H__CFC75076_AC72_46B8_94AC_E20F8DE6C229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pics.h"

class Result  
{
public:
	Result();
	virtual ~Result();

	CString GetResult();

private:
	Pics my_horizontal_picture;
	Pics my_vertical_picture;
	Pics my_horizontal_left_eye; 
	Pics my_horizontal_right_eye;
	Pics my_vertical_left_eye;
	Pics my_vertical_right_eye;
	CString my_result;
};

#endif // !defined(AFX_RESULT_H__CFC75076_AC72_46B8_94AC_E20F8DE6C229__INCLUDED_)
