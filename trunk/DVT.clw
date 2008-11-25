; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPictureDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DVT.h"

ClassCount=10
Class1=CDVTApp
Class2=CDVTDlg
Class3=CAboutDlg

ResourceCount=12
Resource1=IDD_PictureWizard
Resource2=IDR_MAINFRAME
Resource3=IDD_RosterSettings
Resource4=IDD_DISPLAYPIC
Resource5=IDD_YESNO
Class4=RosterDlg
Resource6=IDD_AddProperty
Class5=StudentDlg
Resource7=IDD_PROGRESS
Class6=AddPropertyDlg
Resource8=IDD_DVT_DIALOG
Class7=YESNODlg
Class8=StartScreenDlg
Resource9=IDD_StartScreen
Class9=CPictureDlg
Resource10=IDD_ABOUTBOX
Class10=CPictureWizardDlg
Resource11=IDD_StudentDlg
Resource12=MainMenu

[CLS:CDVTApp]
Type=0
HeaderFile=DVT.h
ImplementationFile=DVT.cpp
Filter=N

[CLS:CDVTDlg]
Type=0
HeaderFile=DVTDlg.h
ImplementationFile=DVTDlg.cpp
Filter=D
LastObject=CDVTDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=DVTDlg.h
ImplementationFile=DVTDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DVT_DIALOG]
Type=1
Class=CDVTDlg
ControlCount=9
Control1=IDC_BAddStudent,button,1342242816
Control2=IDC_StudentList,listbox,1352728963
Control3=IDC_BEditStudent,button,1342242816
Control4=IDC_BRemoveStudent,button,1342242816
Control5=IDC_StudentView,listbox,1352679809
Control6=IDC_SortRoom,button,1073807363
Control7=IDC_SortLabel,static,1073872897
Control8=IDC_SortResult,button,1073807363
Control9=IDC_BTakePictures,button,1342242816

[DLG:IDD_RosterSettings]
Type=1
Class=RosterDlg
ControlCount=11
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_PropList,listbox,1352728961
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BAddProperty,button,1342242816
Control6=IDC_BRemoveProperty,button,1342242816
Control7=IDC_TERosterLabel,edit,1350631552
Control8=IDC_RosterName,static,1342308352
Control9=IDC_DefaultValue,static,1342308352
Control10=IDC_Next,button,1342242817
Control11=IDC_CDefaultValue,combobox,1344339970

[CLS:RosterDlg]
Type=0
HeaderFile=RosterDlg.h
ImplementationFile=RosterDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_FILE_EXIT
VirtualFilter=dWC

[MNU:MainMenu]
Type=1
Class=CDVTDlg
Command1=ID_FILE_NEWROSTER
Command2=ID_FILE_OPENROSTER
Command3=ID_FILE_SAVEROSTER
Command4=ID_FILE_ROSTERSETTINGS
Command5=ID_FILE_EXIT
Command6=ID_HELP_ABOUTEYEDXWINDOW
Command7=ID_HELP_HELP
Command8=ID_HELP_ABOUT
CommandCount=8

[CLS:StudentDlg]
Type=0
HeaderFile=StudentDlg.h
ImplementationFile=StudentDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_FILE_EXIT

[DLG:IDD_AddProperty]
Type=1
Class=AddPropertyDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TENewPropName,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:AddPropertyDlg]
Type=0
HeaderFile=AddPropertyDlg.h
ImplementationFile=AddPropertyDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC

[DLG:IDD_YESNO]
Type=1
Class=YESNODlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_Message,static,1342308352
Control4=IDC_CautionPic,static,1073741838

[CLS:YESNODlg]
Type=0
HeaderFile=YESNODlg.h
ImplementationFile=YESNODlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_FILE_EXIT

[DLG:IDD_StartScreen]
Type=1
Class=StartScreenDlg
ControlCount=2
Control1=IDC_BNewRoster,button,1342242816
Control2=IDC_BOpenRoster,button,1342242816

[CLS:StartScreenDlg]
Type=0
HeaderFile=StartScreenDlg.h
ImplementationFile=StartScreenDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BOpenRoster
VirtualFilter=dWC

[CLS:CPictureDlg]
Type=0
HeaderFile=CPictureDlg.h
ImplementationFile=CPictureDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPictureDlg
VirtualFilter=dWC

[DLG:IDD_StudentDlg]
Type=1
Class=StudentDlg
ControlCount=7
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1073807360
Control3=IDC_PropList,listbox,1352728961
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PropertyValueLabel,static,1342308352
Control6=IDC_Next,button,1342242817
Control7=IDC_CPropertyValue,combobox,1344339970

[DLG:IDD_PROGRESS]
Type=1
Class=?
ControlCount=1
Control1=IDC_PROGRESS,msctls_progress32,1350565888

[CLS:CPictureWizardDlg]
Type=0
HeaderFile=PictureWizardDlg.h
ImplementationFile=PictureWizardDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPictureWizardDlg

[DLG:IDD_PictureWizard]
Type=1
Class=CPictureWizardDlg
ControlCount=14
Control1=IDCANCEL,button,1342242816
Control2=IDC_PICTURE_RIGHT,static,1342177287
Control3=IDC_PICTURE_LEFT,static,1342177287
Control4=IDC_LIST1,listbox,1352728833
Control5=IDC_LIST2,listbox,1352728833
Control6=IDC_BUTTON1,button,1342242816
Control7=IDC_BUTTON2,button,1342242816
Control8=IDC_BUTTON3,button,1342242816
Control9=IDC_BUTTON4,button,1342242816
Control10=IDC_StudentName,static,1342308876
Control11=IDC_ViewFinder,static,1342177287
Control12=IDC_BUTTON5,button,1342242816
Control13=IDC_BUTTON6,button,1342242816
Control14=IDC_BUTTON7,button,1342242816

[DLG:IDD_DISPLAYPIC]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

