// Roster.h: interface for the Roster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROSTER_H__F88E7B56_5ADD_4213_ABC7_7830110DC69E__INCLUDED_)
#define AFX_ROSTER_H__F88E7B56_5ADD_4213_ABC7_7830110DC69E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include <vector>
#include "Student.h"

class Roster  
{

public:
	Roster();
	Roster(CString curpath);
	virtual ~Roster();

	//Loads a roster from a filepath.
	Roster::Roster(CString curpath, CString path);
	
	CString Roster::GetLabel();
	CString Roster::GetPath();
	void Roster::SetLabel(CString newlabel);
	void Roster::SetPath(CString newpath);

	void Roster::LoadAssociations(bool wprops);

	//Add a property to the roster and all contained students.
	void Roster::AddProperty(CString iname, CString tdefaultvalue);
	
	void Roster::AddPropertyAssociation(int pindex, CString tassociation);
	void Roster::AddPropertyAssociation(CString pname, CString tassociation);
	int Roster::GetNumPropertyAssociations(int pindex);
	CString Roster::GetPropertyAssociation(int pindex, int aindex);
	
	//Property Relations
	std::vector<CString> Roster::GetPropertyNames();
	CString Roster::GetPropertyName(int index);
	CString Roster::GetPropertyDefault(int index);
	int GetPropertyIndex(CString pname);
	void Roster::SetPropertyDefault(int index, CString newval);
	void Roster::SetPropertyDefault(CString pname, CString newval);
	void Roster::RemoveProperty(int index);
	int Roster::NumProperties();

	//Student Relations
	Student* Roster::GetStudent(CString sname);
	CString Roster::GetStudentName(int index);
	int Roster::GetStudentPropertyIndex(CString pname);
	CString Roster::GetStudentPropertyValue(CString sname, int pindex);
	CString Roster::GetStudentPropertyValue(int Sindex, CString pname);
	int Roster::NumStudents();
	void SortByProperty(CString pname);

	//Create a new student and return the reference to it.
	Student* Roster::CreateStudent();
	
	void Roster::EditStudent(Student *editstudent, bool *addanother, bool newstudent);
	void Roster::RemoveStudent(CString sname);
	
	//Calls the student editor
	void Roster::DisplayStudentEditor(int index);
	
	//Save the roster to an Excel
	void Roster::Save(CString path);
	CString Roster::GetResult(CString s);
	CString EncryptString(CString str);
	CString DecryptString(CString str);
	bool Roster::FileExists( CString filename);

	BOOL Roster::TakePictures(Student *newstudent, void* theCamera);

	void SetCurPath(CString curpath);



private:
	CString label;
	CString xsl_path; // excel file!
	CString cur_path; //Project path, not excel file!
	std::vector<StudentProperty> property;
	std::vector<Student*> student;	
};

#endif // !defined(AFX_ROSTER_H__F88E7B56_5ADD_4213_ABC7_7830110DC69E__INCLUDED_)
