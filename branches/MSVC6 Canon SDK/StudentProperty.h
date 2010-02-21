// StudentProperty.h: interface for the StudentProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STUDENTPROPERTY_H__73D69B00_1847_4AE1_BADA_E003AD3E3FF3__INCLUDED_)
#define AFX_STUDENTPROPERTY_H__73D69B00_1847_4AE1_BADA_E003AD3E3FF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class StudentProperty
{
public:
	StudentProperty();
	virtual ~StudentProperty();

	CString name;
	CString value;
	CString defaultvalue;
	std::vector<CString> association;
};

#endif // !defined(AFX_STUDENTPROPERTY_H__73D69B00_1847_4AE1_BADA_E003AD3E3FF3__INCLUDED_)
