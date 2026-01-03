# Microsoft Developer Studio Project File - Name="FreeVCL_XFiles" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FreeVCL_XFiles - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FreeVCL_XFiles.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FreeVCL_XFiles.mak" CFG="FreeVCL_XFiles - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FreeVCL_XFiles - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FreeVCL_XFiles - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FreeVCL_XFiles - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP BASE Intermediate_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP Intermediate_Dir "VisualStudio_6\MBCS_32bits\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VisualStudio_6\MBCS_32bits\Release\FreeVCL_XFiles.lib"

!ELSEIF  "$(CFG)" == "FreeVCL_XFiles - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP BASE Intermediate_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP Intermediate_Dir "VisualStudio_6\MBCS_32bits\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VisualStudio_6\MBCS_32bits\Debug\FreeVCL_XFiles.lib"

!ENDIF 

# Begin Target

# Name "FreeVCL_XFiles - Win32 Release"
# Name "FreeVCL_XFiles - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\TBrushPdf.cpp
# End Source File
# Begin Source File

SOURCE=.\TCanvasPdf.cpp
# End Source File
# Begin Source File

SOURCE=.\TColorPdf.cpp
# End Source File
# Begin Source File

SOURCE=.\TDBFFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TFactory_FVCL_XFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\TFontPdf.cpp
# End Source File
# Begin Source File

SOURCE=.\TMetafile.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdf.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfCatalog.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfContent.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfFont.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfOutlines.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfPages.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfProcSet.cpp
# End Source File
# Begin Source File

SOURCE=.\TObjetPdfUnknown.cpp
# End Source File
# Begin Source File

SOURCE=.\TPDFFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TPenPdf.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Include\TBrushPdf.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCanvasPdf.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TColorPdf.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TDBFFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFactory_FVCL_XFiles.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFontPdf.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdf.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfCatalog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfContent.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfFont.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfOutlines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfPage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfPages.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfProcSet.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObjetPdfUnknown.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPDFFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPenPdf.h
# End Source File
# End Group
# End Target
# End Project
