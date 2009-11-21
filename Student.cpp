// Students.cpp: implementation of the Students class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DVT.h"
#include "Student.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Student::Student()
{
	numTimesAnalyzed = 0;
}

Student::~Student()
{

}

void Student::AddProperty(StudentProperty prop)
{
	my_property.push_back(prop);
}

void Student::RemoveProperty(int index)
{
	my_property.erase(my_property.begin()+index);
}

void Student::RemoveProperty(CString pname)
{
	for (size_t a=0;a<my_property.size();a++)
		if (my_property[a].name==pname) RemoveProperty(a);
}

int Student::NumProperties()
{
	return my_property.size();
}

CString Student::GetPropertyName(int index)
{
	return my_property[index].name;
}

CString Student::GetPropertyValue(int index)
{
	return my_property[index].value;
}

CString Student::GetPropertyValue(CString pname)
{
	for (size_t a=0;a<my_property.size();a++)
		if (my_property[a].name == pname) return my_property[a].value;
	
	return "Error: Student property not found.";
}

void Student::SetPropertyName(int index, CString pname)
{
	my_property[index].name = pname;
}

void Student::SetPropertyDefault(int index, CString tvalue)
{
	my_property[index].defaultvalue = tvalue;
}

void Student::SetPropertyValue(int index, CString tvalue)
{
	my_property[index].value = tvalue;
}

void Student::SetPropertyValue(CString pname, CString tvalue)
{
	for (size_t a=0;a<my_property.size();a++)
		if (my_property[a].name == pname) my_property[a].value = tvalue;
}

std::vector<StudentProperty> Student::ClonePropList()
{
	std::vector<StudentProperty> list;
	list.assign(my_property.begin(), my_property.end());
	return list;
}

void Student::OverridePropList(std::vector<StudentProperty> list)
{
	my_property.assign(list.begin(), list.end());
}

void Student::AddVertPicture(CString path, CString timestamp)
{
	Pics newpicture(path, timestamp);
	my_vertpicture.push_back(newpicture);
}

void Student::AddHorizPicture(CString path, CString timestamp)
{
	/*#include <CTime>
	CTime now = CTime::GetCurrentTime();
	CString nowstring = now.Format( "%m_%d_%Y_%H_%M_%S" );*/
	Pics newpicture(path, timestamp);
	my_horizpicture.push_back(newpicture);
}

int Student::GetNumberOfTimesAnalyzed(){
	return numTimesAnalyzed;
}
void Student::IncrementNumberOfTimesAnalyzed(){
	numTimesAnalyzed++;
}



