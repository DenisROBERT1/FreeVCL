# Microsoft Developer Studio Project File - Name="FreeVCL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FreeVCL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FreeVCL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FreeVCL.mak" CFG="FreeVCL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FreeVCL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FreeVCL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FreeVCL - Win32 Release"

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
# ADD CPP /nologo /Zp4 /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VisualStudio_6\MBCS_32bits\Release\FreeVCL.lib"

!ELSEIF  "$(CFG)" == "FreeVCL - Win32 Debug"

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
# ADD CPP /nologo /Zp4 /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Lib\VisualStudio_6\MBCS_32bits\Debug\FreeVCL.lib"

!ENDIF 

# Begin Target

# Name "FreeVCL - Win32 Release"
# Name "FreeVCL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CharString.cpp
# End Source File
# Begin Source File

SOURCE=.\Exceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeVcl.cpp
# End Source File
# Begin Source File

SOURCE=.\TAction.cpp
# End Source File
# Begin Source File

SOURCE=.\TApplication.cpp
# End Source File
# Begin Source File

SOURCE=.\TBevel.cpp
# End Source File
# Begin Source File

SOURCE=.\TBitBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\TBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\TBMPFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\TButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\TCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TClipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\TColor.cpp
# End Source File
# Begin Source File

SOURCE=.\TColorBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TColorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TComboBoxItems.cpp
# End Source File
# Begin Source File

SOURCE=.\TComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\TControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TControlCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\TControlScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomFont.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\TCustomPen.cpp
# End Source File
# Begin Source File

SOURCE=.\TDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\TDateTimePicker.cpp
# End Source File
# Begin Source File

SOURCE=.\TDragControlObject.cpp
# End Source File
# Begin Source File

SOURCE=.\TDragControlObjectEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TDragImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\TDragObject.cpp
# End Source File
# Begin Source File

SOURCE=.\TEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\TFactory_FVCL.cpp
# End Source File
# Begin Source File

SOURCE=.\TFileStream.cpp
# End Source File
# Begin Source File

SOURCE=.\TFont.cpp
# End Source File
# Begin Source File

SOURCE=.\TFontDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TForm.cpp
# End Source File
# Begin Source File

SOURCE=.\TFormFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\TFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\TGraphic.cpp
# End Source File
# Begin Source File

SOURCE=.\TGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\THintWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\THotKey.cpp
# End Source File
# Begin Source File

SOURCE=.\TICOFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TImage.cpp
# End Source File
# Begin Source File

SOURCE=.\TImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\TLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\TLink.cpp
# End Source File
# Begin Source File

SOURCE=.\TListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TListBoxItems.cpp
# End Source File
# Begin Source File

SOURCE=.\TListColumn.cpp
# End Source File
# Begin Source File

SOURCE=.\TListColumns.cpp
# End Source File
# Begin Source File

SOURCE=.\TListItem.cpp
# End Source File
# Begin Source File

SOURCE=.\TListItems.cpp
# End Source File
# Begin Source File

SOURCE=.\TListView.cpp
# End Source File
# Begin Source File

SOURCE=.\TListViewSubItems.cpp
# End Source File
# Begin Source File

SOURCE=.\TMainMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\TMaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TMemo.cpp
# End Source File
# Begin Source File

SOURCE=.\TMemoLines.cpp
# End Source File
# Begin Source File

SOURCE=.\TMemoryStream.cpp
# End Source File
# Begin Source File

SOURCE=.\TMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\TMenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonthCalColors.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonthCalendar.cpp
# End Source File
# Begin Source File

SOURCE=.\TMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\TObject.cpp
# End Source File
# Begin Source File

SOURCE=.\TOpenDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TOpenSaveDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TPageControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TPaintBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TPaletteVCL.cpp
# End Source File
# Begin Source File

SOURCE=.\TPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\TPen.cpp
# End Source File
# Begin Source File

SOURCE=.\TPersistent.cpp
# End Source File
# Begin Source File

SOURCE=.\TPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\TPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\TPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\TPrintDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\TPrinterSetupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TRadioGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\TRect.cpp
# End Source File
# Begin Source File

SOURCE=.\TSaveDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\TScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TScrollBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TSizeConstraints.cpp
# End Source File
# Begin Source File

SOURCE=.\TSpeedButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\TStaticText.cpp
# End Source File
# Begin Source File

SOURCE=.\TStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TStream.cpp
# End Source File
# Begin Source File

SOURCE=.\TStringGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\TStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\TStrings.cpp
# End Source File
# Begin Source File

SOURCE=.\TStringTab.cpp
# End Source File
# Begin Source File

SOURCE=.\TTabControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TTabControlTabs.cpp
# End Source File
# Begin Source File

SOURCE=.\TTabSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\TTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\TToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TToolButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TTrackBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TTrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TTreeNode.cpp
# End Source File
# Begin Source File

SOURCE=.\TTreeNodes.cpp
# End Source File
# Begin Source File

SOURCE=.\TTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\TUpDown.cpp
# End Source File
# Begin Source File

SOURCE=.\TWinControl.cpp
# End Source File
# Begin Source File

SOURCE=.\WideString.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Include\CharString.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Collections.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Components.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Event.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Exceptions.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\FreeVcl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\FVCLBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\FVCLTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Persistents.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Property.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Set.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TAction.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TBaseDragControlObject.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TBevel.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TBitBtn.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TBitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TBMPFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TBrush.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCanvas.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCheckBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TClipboard.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TColor.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TColorBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TColorDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TComboBoxItems.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TComponent.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TControlBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TControlCanvas.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TControlScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomBrush.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomCanvas.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomFont.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomGrid.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomImageList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomPanel.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TCustomPen.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TDateTime.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TDateTimePicker.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TDragControlObject.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TDragControlObjectEx.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TDragImageList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TDragObject.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFactory_FVCL.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFileStream.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFont.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFontDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TForm.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFormFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TGraphic.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TGroupBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\THintWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\THotKey.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TICOFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TIcon.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TImage.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TImageList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TLabel.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TLink.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListBoxItems.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListColumn.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListColumns.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListItem.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListItems.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TListViewSubItems.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMainMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMaskEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMemo.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMemoLines.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMemoryStream.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMenuItem.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMonthCalColors.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMonthCalendar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TMouse.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TObject.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TOpenDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TOpenSaveDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPageControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPaintBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPaletteVCL.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPanel.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPen.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPersistent.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPicture.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPopupMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPrintDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPrinter.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TPrinterSetupDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TRadioButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TRadioGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Translate.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TRect.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TSaveDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TScreen.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TScrollBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TSizeConstraints.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TSpeedButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TSplitter.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TStaticText.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TStatusBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TStream.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TStringGrid.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TStringList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TStrings.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TStringTab.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTabControlTabs.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTabSheet.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTimer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TToolBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TToolButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTrackBar.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTrayIcon.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTreeNode.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTreeNodes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TTreeView.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TUpDown.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\TWinControl.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\WideString.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\WinControls.h
# End Source File
# End Group
# End Target
# End Project
