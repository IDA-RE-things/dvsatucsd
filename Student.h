// Student.h: interface for the Student class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STUDENTS_H__46B2CD1B_273B_4127_BD81_97DCEDDCA0B4__INCLUDED_)
#define AFX_STUDENTS_H__46B2CD1B_273B_4127_BD81_97DCEDDCA0B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include <vector>
#include "Pics.h"
#include "StudentProperty.h"

class Student 
{
public:
	Student();
	virtual ~Student();

	//Property Relations
	void Student::AddProperty(StudentProperty prop);
	int Student::NumProperties();
	CString Student::GetPropertyName(int index);
	CString Student::GetPropertyValue(int index);
	CString Student::GetPropertyValue(CString pname);
	void Student::SetPropertyName(int index, CString pname);
	void Student::SetPropertyValue(int index, CString tvalue);
	void Student::SetPropertyValue(CString pname, CString tvalue);
	void Student::SetPropertyDefault(int index, CString tvalue);
	void Student::RemoveProperty(int index);
	void Student::RemoveProperty(CString pname);
	std::vector<StudentProperty> Student::ClonePropList();
	void Student::OverridePropList(std::vector<StudentProperty> list);

	void Student::AddVertPicture(CString path, CString timestamp);
	void Student::AddHorizPicture(CString path, CString timestamp);
	int Student::GetNumberOfTimesAnalyzed();
	void Student:: IncrementNumberOfTimesAnalyzed();

private:

	int numTimesAnalyzed;
	std::vector<StudentProperty> my_property;
	std::vector<Pics> my_vertpicture;
	std::vector<Pics> my_horizpicture;

};




#endif // !defined(AFX_STUDENTS_H__46B2CD1B_273B_4127_BD81_97DCEDDCA0B4__INCLUDED_)

