# Microsoft Developer Studio Project File - Name="FVCLIDE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FVCLIDE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FVCLIDE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FVCLIDE.mak" CFG="FVCLIDE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FVCLIDE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FVCLIDE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FVCLIDE - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP BASE Intermediate_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP Intermediate_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Version.lib FreeVcl.lib FreeVcl_ToolsEDI.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FVCLIDE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP BASE Intermediate_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP Intermediate_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Version.lib FreeVcl.lib FreeVcl_ToolsEDI.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FVCLIDE - Win32 Release"
# Name "FVCLIDE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\TBaseTree.cpp
# End Source File
# Begin Source File

SOURCE=.\TConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\TControlSizer.cpp
# End Source File
# Begin Source File

SOURCE=.\TElemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TElemVCL.cpp
# End Source File
# Begin Source File

SOURCE=.\TFileASCII.cpp
# End Source File
# Begin Source File

SOURCE=.\TFileForm.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Forms\TForm_APropos.cpp
# End Source File
# Begin Source File

SOURCE=.\TForm_Chercher.cpp
# End Source File
# Begin Source File

SOURCE=.\TForm_Principale.cpp
# End Source File
# Begin Source File

SOURCE=.\TForm_Proprietes.cpp
# End Source File
# Begin Source File

SOURCE=.\TForm_SaisieRacc.cpp
# End Source File
# Begin Source File

SOURCE=.\TForm_Statistics.cpp
# End Source File
# Begin Source File

SOURCE=.\TFrame_EditFichierText.cpp
# End Source File
# Begin Source File

SOURCE=.\TFrame_SaisieDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TGlobalConstr.cpp
# End Source File
# Begin Source File

SOURCE=.\TSynchroTree.cpp
# End Source File
# Begin Source File

SOURCE=.\TVCLTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\TXMLTree.cpp
# End Source File
# Begin Source File

SOURCE=.\FVCLIDE.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\TBaseTree.h
# End Source File
# Begin Source File

SOURCE=.\TConfig.h
# End Source File
# Begin Source File

SOURCE=.\TControlSizer.h
# End Source File
# Begin Source File

SOURCE=.\TElemBase.h
# End Source File
# Begin Source File

SOURCE=.\TElemVCL.h
# End Source File
# Begin Source File

SOURCE=.\TFileASCII.h
# End Source File
# Begin Source File

SOURCE=.\TFileForm.h
# End Source File
# Begin Source File

SOURCE=..\Common\Forms\TForm_APropos.h
# End Source File
# Begin Source File

SOURCE=.\TForm_Chercher.h
# End Source File
# Begin Source File

SOURCE=.\TForm_Principale.h
# End Source File
# Begin Source File

SOURCE=.\TForm_Proprietes.h
# End Source File
# Begin Source File

SOURCE=.\TForm_SaisieRacc.h
# End Source File
# Begin Source File

SOURCE=.\TForm_Statistics.h
# End Source File
# Begin Source File

SOURCE=.\TFrame_EditFichierText.h
# End Source File
# Begin Source File

SOURCE=.\TFrame_SaisieDialog.h
# End Source File
# Begin Source File

SOURCE=.\TGlobalConstr.h
# End Source File
# Begin Source File

SOURCE=.\TSynchroTree.h
# End Source File
# Begin Source File

SOURCE=.\TVCLTreeView.h
# End Source File
# Begin Source File

SOURCE=.\TXMLTree.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\SIZE2.CUR
# End Source File
# Begin Source File

SOURCE=..\Common\Forms\TForm_APropos.dfm
# End Source File
# Begin Source File

SOURCE=.\TForm_Chercher.dfm
# End Source File
# Begin Source File

SOURCE=.\TForm_Principale.dfm
# End Source File
# Begin Source File

SOURCE=.\TForm_Proprietes.dfm
# End Source File
# Begin Source File

SOURCE=.\TForm_SaisieRacc.dfm
# End Source File
# Begin Source File

SOURCE=.\TForm_Statistics.dfm
# End Source File
# Begin Source File

SOURCE=.\TFrame_EditFichierText.dfm
# End Source File
# Begin Source File

SOURCE=.\TFrame_SaisieDialog.dfm
# End Source File
# Begin Source File

SOURCE=.\FVCLIDE.ICO
# End Source File
# Begin Source File

SOURCE=.\FVCLIDE.rc
# End Source File
# End Group
# End Target
# End Project
