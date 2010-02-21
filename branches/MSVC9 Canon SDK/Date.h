// Date.h: interface for the Date class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATE_H__DB1D7FC9_A47B_4CB6_8D0E_58E507BE55DD__INCLUDED_)
#define AFX_DATE_H__DB1D7FC9_A47B_4CB6_8D0E_58E507BE55DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Date  
{
public:
	Date();
	virtual ~Date();

	int year;
	int month;
	int day;
	int time;

};

#endif // !defined(AFX_DATE_H__DB1D7FC9_A47B_4CB6_8D0E_58E507BE55DD__INCLUDED_)
