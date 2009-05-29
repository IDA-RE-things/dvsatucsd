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
#include <direct.h>

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
	xsl_path = "";
	LoadAssociations(true);
	label = "Unnamed Roster";
}

Roster::Roster(CString curpath, CString path)
{
	cur_path = curpath;
	xsl_path = path;

	// Create a new filestream for input.
	ifstream loadstream(path, ios::in); 
	char tempstr[32768]; //The max characters in a database line.
	CString line;

	//Input roster name
	loadstream.getline(tempstr,32768);
	line = tempstr;
	line.Remove('\t');
	label = line;

	//Input student property names
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
			line = line.Right(line.GetLength()-(delim+1));
		}
		else
		{
			newprop.name = line;
		}
		property.push_back(newprop);
		
	}

	LoadAssociations(false); // global defaults from .ini files, overwritten by saved defaults in first row of .xsl file

	//Input default values
	loadstream.getline(tempstr,32768);
	line = tempstr;
	delim = 0;
	int index = 0;
	while(delim!=-1)
	{
		delim = line.Find('\t');
		if (delim!=-1)
		{
			property[index].value = line.Left(delim);
			line = line.Right(line.GetLength()-(delim+1));
		}
		else
		{
			property[index].value = line;
		}
		if (property[index].value == "") property[index].value = property[index].defaultvalue;
		index++;
	}

	//Input student data
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
		student.push_back(newstudent);
	}

	//Close the filestream.
	loadstream.close();
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
	int index = 0;
 
	//Input roster properties
	while(instream.getline(tempstr,32768))
	{
		cur_property = tempstr;
		instream.getline(tempstr,32768);
		def = tempstr;
				
		if (wprops) AddProperty(cur_property, def);
		else {
			index = GetPropertyIndex(cur_property);
			if (index != -1) property[index].defaultvalue = def;
		}

		instream.getline(tempstr,32768);
		line = tempstr;

		//Load associations
		while (line!="")
		{
			if (wprops) AddPropertyAssociation(property.size()-1, line);
			else if (index != -1) AddPropertyAssociation(cur_property, line);

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

CString Roster::GetPath()
{
	return xsl_path;
}

void Roster::SetLabel(CString newlabel)
{
	label = newlabel;
}

void Roster::SetPath(CString newpath)
{
	xsl_path = newpath;
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

	return temp;
}

void Roster::EditStudent(Student *editstudent, bool *addanother, bool newstudent)
{
	INT_PTR ret = -1;
	
	//Create the dialog box and run it
	StudentDlg dlgStudent(NULL, editstudent, addanother, &property);

	ret = dlgStudent.DoModal();

	//Check for name duplication and fix it if necessary
	bool repeat = false;
	bool skippedself = false;
	bool callmsg = false;

	if (ret == IDOK) {
		for (int a=0; a<student.size(); a++)
			if (editstudent->GetPropertyValue("Name") == student[a]->GetPropertyValue("Name")) {
				if (newstudent == true || skippedself == true)
				{
					repeat = true;
					callmsg = true;
				}
				else skippedself = true;
			}

		// check for name$, name$$, etc. until free namespace found
		while (repeat == true)
		{
			repeat = false;
			skippedself = false;
			
			editstudent->SetPropertyValue("Name", editstudent->GetPropertyValue("Name") + "$");

			for (int a=0; a<student.size(); a++)
			{
				if (editstudent->GetPropertyValue("Name") == student[a]->GetPropertyValue("Name"))
				{
					if (newstudent == true || skippedself == true) repeat = true;
					else skippedself = true;
				}
			}
		}
		
		if (newstudent == true) student.push_back(editstudent);

		//Alert the user of the changed name
		if (callmsg == true) dlgStudent.MessageBox("A student has been added with an identical name to a previous student. The new student's name has been changed to: " + editstudent->GetPropertyValue("Name"));
	}
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

void Roster::EditProperty(int index, CString iname)
{
	//Adds a new property to the roster list.
	property[index].name = iname;

	//Adds the new property to all of the existing students.
	for (int a=0; a<student.size(); a++)
		student[a]->SetPropertyName(index, iname);
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
	return property[index].value;
}

void Roster::SetPropertyDefault(int index, CString newval)
{
	//Checks the roster for a property with the given index and sets the default value; note: roster value (default in ini) displayed in settings becomes default student value
	property[index].value = newval;

	for (int a=0;a<student.size();a++)
		student[a]->SetPropertyDefault(index, newval);
}

void Roster::SetPropertyDefault(CString pname, CString newval)
{
	SetPropertyDefault(GetPropertyIndex(pname), newval);
}

void Roster::SetPropertyOverride(int index, CString newval)
{
	// Override student values of property with roster property value
	for (int a=0;a<student.size();a++)
		student[a]->SetPropertyValue(index, newval);
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
}

void Roster::RemovePropertyFromStudents(int index)
{
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

	//Output roster name
	savestream << label << "\n";

	int numprops = property.size();

	//Output student properties
	for (int a=0;a<numprops;a++)
	{
		savestream << property[a].name;
		if (a!=numprops-1) savestream << "\t";
	}
	savestream << "\n";


	//Output default properties (roster settings)
	for (a=0;a<numprops;a++)
	{
		savestream << property[a].value; // override student name roster setting (not saved for the sake of spreadsheet readability)? (a == 0 ? "Default Student" : property[a].value)
		if (a!=numprops-1) savestream << "\t";
	}
	savestream << "\n";

	//Output student property values
	for (a=0;a<student.size();a++)
	{
		for (int b=0;b<numprops;b++)
		{
			if (property[b].name == "Photo Timestamp")
			{
				savestream << EncryptString(student[a]->GetPropertyValue(b));
			}
			else if (property[b].name == "Photo Result")
			{
				savestream << GetResult( student[a]->GetPropertyValue(0) );
			}
			else savestream << student[a]->GetPropertyValue(b);
			
			if (b!=numprops-1) 
				savestream << "\t";
		}

		if (a!=student.size()-1) 
			savestream << "\n";
	}

	savestream.close();
}

/*
 *  Gets results of EyeDx analysis on student's pictures.
 *  Also copies report files to roster folder and deletes
 *  them in the EyeDx folder.  
 *
 *  @param studentName Name of student analyzed.
 *  @return Results of EyeDx analysis: unclear, refer, or pass
 */
CString Roster::GetResult(CString studentName)
{
	//get path to DVT main folder
	char origpath[32768];
	getcwd(origpath, 32768);

	chdir("..");

	char curpath[32768];
	getcwd(curpath, 32768);

	//get path to eyedx folder
	CString cur_path = CString(curpath);
	CString eyedxDir = cur_path + "\\EyeDx1.5.2\\NonSession";
	
	//MessageBox(NULL,fileNameU,"hello",NULL);

	chdir(origpath);

	//determine EyeDx result
	CString result;
	if (FileExists ( eyedxDir+"\\reports\\R_"+ studentName + ".htm" )
		|| FileExists ( cur_path+"\\"+GetLabel()+"\\reports\\R_"+ studentName + ".htm" ))
		result = "R";
	else if (FileExists ( eyedxDir+"\\reports\\U_"+ studentName + ".htm" )
		|| FileExists ( cur_path+"\\"+GetLabel()+"\\reports\\U_"+ studentName + ".htm" ))
		result = "U";
	else if (FileExists ( eyedxDir+"\\reports\\N_"+ studentName + ".htm" )
		|| FileExists ( cur_path+"\\"+GetLabel()+"\\reports\\N_"+ studentName + ".htm" ))
		result = "N";
	else
		return "N\\A";

	//copy report and images from eyedx folders to roster folders
	CopyFile(eyedxDir+"\\reports\\"+result+"_"+ studentName + ".htm",
			cur_path+"\\"+GetLabel()+"\\reports\\" + result+ "_"+ studentName + ".htm",false);

	CopyFile(eyedxDir+"\\Simages\\"+ studentName + "siSsc.jpg",
			cur_path+"\\"+GetLabel()+"\\Simages\\" + studentName + "siSsc.jpg",false);
	CopyFile(eyedxDir+"\\Simages\\"+ studentName + "upUsc.jpg",
			cur_path+"\\"+GetLabel()+"\\Simages\\" + studentName + "upUsc.jpg",false);

	CopyFile(eyedxDir+"\\eyes\\"+ studentName + "upUle.jpg",
			cur_path+"\\"+GetLabel()+"\\eyes\\" + studentName + "upUle.jpg",false);
	CopyFile(eyedxDir+"\\eyes\\"+ studentName + "upUre.jpg",
			cur_path+"\\"+GetLabel()+"\\eyes\\" + studentName + "upUre.jpg",false);
	CopyFile(eyedxDir+"\\eyes\\"+ studentName + "siSle.jpg",
			cur_path+"\\"+GetLabel()+"\\eyes\\" + studentName + "siSle.jpg",false);
	CopyFile(eyedxDir+"\\eyes\\"+ studentName + "siSre.jpg",
			cur_path+"\\"+GetLabel()+"\\eyes\\" + studentName + "siSre.jpg",false);

	CopyFile(eyedxDir+"\\EyeDxLogo.jpg", 
			cur_path+"\\"+GetLabel()+"\\EyeDxLogo.jpg", false);

	//delete files in eyeDx folder
	DeleteFile(eyedxDir+"\\reports\\"+result+"_"+ studentName + ".htm");
	DeleteFile(eyedxDir+"\\Simages\\"+ studentName + "siSsc.jpg");
	DeleteFile(eyedxDir+"\\Simages\\"+ studentName + "upUsc.jpg");
	DeleteFile(eyedxDir+"\\eyes\\"+ studentName + "upUle.jpg");
	DeleteFile(eyedxDir+"\\eyes\\"+ studentName + "upUre.jpg");
	DeleteFile(eyedxDir+"\\eyes\\"+ studentName + "siSle.jpg");
	DeleteFile(eyedxDir+"\\eyes\\"+ studentName + "siSre.jpg");


	//return results
	if(result.Compare("R") == 0)
		return "refer";
	if(result.Compare("U") == 0)
		return "unclear";
	if(result.Compare("N") == 0)
		return "pass";

	return "N\\A";
}

/*
 *  Determines if a file exists at specified
 *  path.  Used by getResult().
 *
 *  @param fname Full path to file.
 *  @return True if file exists or false otherwise.
 */
bool Roster::FileExists(CString fname)
{
	CFile f;
	if (f.Open(fname,CFile::modeRead))
		return true;
	else
		return false;
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

std::vector<StudentProperty> Roster::ClonePropList()
{
	std::vector<StudentProperty> list;
	list.assign(property.begin(), property.end());
	return list;
}

void Roster::OverridePropList(std::vector<StudentProperty> list)
{
	property.assign(list.begin(), list.end());
}