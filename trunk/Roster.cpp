// Roster.cpp: implementation of the Roster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DVT.h"
#include "Roster.h"
#include "StudentDlg.h"
#include "PictureWizardDlg.h"
#include "CameraConnect.h"
#include <fstream.h>
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Roster::Roster()
{
}

Roster::Roster(CString curpath)
{
	cur_path = curpath;
	LoadAssociations(true);
	label = "Unnamed Roster";
}

Roster::Roster(CString curpath, CString path)
{
	cur_path = curpath;

	
	// Create a new filestream for input.
	ifstream loadstream(path, ios::in); 
	char tempstr[32768]; //The max characters in a database line.
	CString line;

	//Input roster properties
	loadstream.getline(tempstr,32768);
	line = tempstr;
	line.Remove('\t');
	label = line;

	//Input student properties
	loadstream.getline(tempstr,32768);
	line = tempstr;
	int delim = 0;
	while(delim!=-1)
	{
		StudentProperty newprop;
		delim = line.Find('\t');
		if (delim!=-1)
		{
			newprop.name = line.Left(delim);
			property.push_back(newprop);
			line = line.Right(line.GetLength()-(delim+1));
		}
		else
		{
			newprop.name = line;
			property.push_back(newprop);
		}
		
	}

	//Input student data
	int index = 0;
	while(loadstream.getline(tempstr,32768))
	{
		Student *newstudent = CreateStudent();
		line = tempstr;
		delim = 0;
		index = 0;
		while(delim!=-1)
		{
			delim = line.Find('\t');
			if (delim!=-1)
			{
				if (property[index].name == "Photo Timestamp")
					newstudent->SetPropertyValue(index,DecryptString(line.Left(delim)));
				else
					newstudent->SetPropertyValue(index,line.Left(delim));

				line = line.Right(line.GetLength()-(delim+1));
			}
			else
			{
				if (property[index].name == "Photo Timestamp")
					newstudent->SetPropertyValue(index,DecryptString(line));
				else
					newstudent->SetPropertyValue(index,line);
			}
			index++;
		}
	}

	//Close the filestream.
	loadstream.close();

  
	LoadAssociations(false);
}

Roster::~Roster()
{

}

void Roster::LoadAssociations(bool wprops)
{
	//Load the student properties.ini file for property lists and associations
	ifstream instream(cur_path + "\\Student Properties.ini", ios::in);
	char tempstr[32768]; //The max characters in a database line.
	CString line;
	CString cur_property;
	CString def;
 
	//Input roster properties
	while(instream.getline(tempstr,32768))
	{
		cur_property = tempstr;
		instream.getline(tempstr,32768);
		def = tempstr;
				
		if (wprops) AddProperty(cur_property, def);
		else if (GetPropertyIndex(cur_property)!=-1) 
			SetPropertyDefault(cur_property, def);

		instream.getline(tempstr,32768);
		line = tempstr;

		//Load associations
		while (line!="")
		{
			if (wprops) AddPropertyAssociation(property.size()-1, line);
			else if (GetPropertyIndex(cur_property)!=-1) 
				AddPropertyAssociation(cur_property, line);

			instream.getline(tempstr,32768);
			line = tempstr;
		}
	}

	//Close the filestream
	instream.close();

}

CString Roster::GetLabel()
{
	return label;
}

void Roster::SetLabel(CString newlabel)
{
	label = newlabel;
}

void Roster::DisplayStudentEditor(int index)
{
	StudentDlg dlgStudent(NULL,student[index],NULL, &property);
	dlgStudent.DoModal();
}


Student* Roster::CreateStudent()
{
	//Create a new student and populate the properties
	//Student temp;
	Student *temp = new Student;

	for (int a=0;a<property.size();a++)
		temp->AddProperty(property[a]);

	student.push_back(temp);

	return temp;
}

void Roster::EditStudent(Student *newstudent, bool *addanother)
{

	//Create the dialog box and run it
	StudentDlg dlgStudent(NULL, newstudent, addanother, &property);
	
	dlgStudent.DoModal();
	
	//Check for name duplication and fix it if necessary
	bool repeat = false;
	bool skippedself = false;
	bool callmsg = false;
	for (int a=0; a<student.size(); a++)
		if (newstudent->GetPropertyValue("Name") == student[a]->GetPropertyValue("Name"))
			if (skippedself == true)
			{
				repeat = true;
				callmsg = true;
			}
			else skippedself = true;

	while (repeat == true)
	{
		repeat = false;
		skippedself = false;

		CString n1 = newstudent->GetPropertyValue("Name");
		CString n2 = student[0]->GetPropertyValue("Name");
		CString n3 = student[1]->GetPropertyValue("Name");
			
		newstudent->SetPropertyValue("Name", newstudent->GetPropertyValue("Name") + "$");

		n1 = newstudent->GetPropertyValue("Name");
		n2 = student[0]->GetPropertyValue("Name");
		n3 = student[1]->GetPropertyValue("Name");

		for (int a=0; a<student.size(); a++)
		{
			if (newstudent->GetPropertyValue("Name") == student[a]->GetPropertyValue("Name"))
			{
				if (skippedself == true) repeat = true;
				else skippedself = true;
			}
		}
	}
	
	//Alert the user of the changed name
	if (callmsg == true) dlgStudent.MessageBox("A student has been added with an identical name to a previous student.  The new student's name has been changed to: " + newstudent->GetPropertyValue("Name"));


}

BOOL Roster::TakePictures(Student *newstudent, void* theCamera)
{
	//Create the dialog box and run it
	CPictureWizardDlg dlgPicture(NULL, this, newstudent, theCamera, cur_path);
	if(dlgPicture.OnCamera())
	{	
		dlgPicture.DoModal();
		return true;
	}		
	else
	{
		return false;
	}
	
}

void Roster::AddProperty(CString iname, CString tdefaultvalue)
{
	//Adds a new property to the roster list.
	StudentProperty tprop;
	tprop.name = iname;
	tprop.defaultvalue = tdefaultvalue;
	tprop.value = tdefaultvalue;
	property.push_back(tprop);

	//Adds the new property to all of the existing students.
	for (int a=0; a<student.size(); a++)
		student[a]->AddProperty(tprop);
}

void Roster::AddPropertyAssociation(int pindex, CString tassociation)
{
	property[pindex].association.push_back(tassociation);
}

void Roster::AddPropertyAssociation(CString pname, CString tassociation)
{
	int index = GetPropertyIndex(pname);
	AddPropertyAssociation(GetPropertyIndex(pname), tassociation);
}

int Roster::GetNumPropertyAssociations(int pindex)
{
	return property[pindex].association.size();
}

CString Roster::GetPropertyAssociation(int pindex, int aindex)
{
	return property[pindex].association[aindex];
}

//Returns a list of the property names
std::vector<CString> Roster::GetPropertyNames()
{
	std::vector<CString> propnames;

	for (int a=0; a<property.size(); a++)
		propnames.push_back(property[a].name);

	return propnames;
}

CString Roster::GetPropertyName(int index)
{
	return property[index].name;
}

int Roster::GetPropertyIndex(CString pname)
{
	for (int a=0;a<property.size();a++)
		if (property[a].name == pname) return a;

	return -1;
}

CString Roster::GetPropertyDefault(int index)
{
	return property[index].defaultvalue;
}

void Roster::SetPropertyDefault(int index, CString newval)
{
	//Checks the roster for a property with the given index and sets the default value
	property[index].defaultvalue = newval;
	property[index].value = newval;

	for (int a=0;a<student.size();a++)
		student[a]->SetPropertyDefault(index, newval);
}

void Roster::SetPropertyDefault(CString pname, CString newval)
{
	SetPropertyDefault(GetPropertyIndex(pname), newval);
}

Student* Roster::GetStudent(CString sname)
{
	for (int a=0;a<student.size();a++)
		if (student[a]->GetPropertyValue("Name")==sname) return student[a];

	return NULL;
}

CString Roster::GetStudentName(int index)
{
	return student[index]->GetPropertyValue("Name");
}

int Roster::GetStudentPropertyIndex(CString pname)
{
	for (int a=0;a<property.size();a++)
		if (property[a].name == pname) return a;

	return -1;
}

CString Roster::GetStudentPropertyValue(CString sname, int pindex)
{
	for (int a=0;a<student.size();a++)
		if (student[a]->GetPropertyValue("Name")==sname) return student[a]->GetPropertyValue(pindex);

	return "Error: Student property value not found.";
}

CString Roster::GetStudentPropertyValue(int Sindex, CString pname)
{
	return student[Sindex]->GetPropertyValue(pname);

	return "Error: Student property value not found.";
}

int Roster::NumProperties()
{
	return property.size();
}

int Roster::NumStudents()
{
	return student.size();
}

void Roster::RemoveProperty(int index)
{
	//Remove property from roster
	property.erase(property.begin()+index);

	//Remove property from all of the students
	for (int a=0; a<student.size(); a++)
		student[a]->RemoveProperty(index);
}

void Roster::RemoveStudent(CString sname)
{
	for (int a=0;a<student.size();a++)
		if (student[a]->GetPropertyValue("Name")==sname) student.erase(student.begin()+a);
}

void Roster::Save(CString path)
{
	ofstream savestream(path, ios::out);

	//Output roster properties
	savestream << label << "\n";

	int numprops = property.size();
	//Output student properties
	for (int a=0;a<numprops;a++)
	{
		savestream << property[a].name;
		if (a!=numprops-1) savestream << "\t";
	}
	savestream << "\n";

	/*
	//Output default properties
	for (a=0;a<numprops;a++)
	{
		savestream << property[a].defaultvalue;
		if (a!=numprops-1) savestream << "\t";
	}
	savestream << "\n";
	*/

	//Output student propvalues
	for (a=0;a<student.size();a++)
	{
		for (int b=0;b<numprops;b++)
		{
			if (property[b].name == "Photo Timestamp")
			{
				savestream << EncryptString(student[a]->GetPropertyValue(b));
			}
			else savestream << student[a]->GetPropertyValue(b);
			
			if (b!=numprops-1) savestream << "\t";
		}

		if (a!=student.size()-1) savestream << "\n";
	}

	savestream.close();
}

CString Roster::EncryptString(CString str)
{
	CString encrypted_string;
	for (int a=0;a<str.GetLength();a++)
		encrypted_string += str[a]+a/2+1;

	return encrypted_string;
}

CString Roster::DecryptString(CString str)
{
	CString decrypted_string;
	for (int a=0;a<str.GetLength();a++)
		decrypted_string += str[a]-a/2-1;

	return decrypted_string;
}

void Roster::SortByProperty(CString pname)
{

}

void Roster::SetCurPath(CString curpath)
{
	cur_path = curpath;
}