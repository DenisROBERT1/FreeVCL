//-----------------------------------------------------------------------------
//! @file TForm_Principale.h
//!
//! @author Denis ROBERT (http://denis.beru.free.fr)
//!
//! Copyright 2008-2025
//! @n
//! This file is part of FreeVCL.
//! @n
//!     FreeVCL is free software: you can redistribute it and/or modify
//!     it under the terms of the GNU Lesser General Public License as
//!     published by the Free Software Foundation, either version 3 of
//!     the License, or (at your option) any later version.
//! @n
//!     FreeVCL is distributed in the hope that it will be useful,
//!     but WITHOUT ANY WARRANTY; without even the implied warranty of
//!     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//!     GNU Lesser General Public License for more details.
//! @n
//!     You should have received a copy of the GNU Lesser General Public
//!     License along with FreeVCL.
//!     If not, see <http://www.gnu.org/licenses/>.
//!
//-----------------------------------------------------------------------------

#ifndef TForm_PrincipaleH
#define TForm_PrincipaleH

//---------------------------------------------------------------------------
#include <FreeVcl.h>
#include <FreeVcl_ToolsEDI.h>
#include <FreeVcl_XFiles.h>
#include <FreeVcl_Web.h>

#include "TFVCLIDEAutomation.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TCustomWinSocket;
class TListTypeProjects;
class TProject;
class TFrame_FVCLIDEBase;
class TForm_Autocompletion;
class TForm_SearchAndReplace;


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

typedef struct {
  TTabSheet *TabSheet;
  TFrame_FVCLIDEBase *Content;
} TABSHEET;

typedef struct {
  int nVirtKey;
	TShiftState Shift;
} KEYACTION;

#define DECAL_TYPES_PROJECT 3


//---------------------------------------------------------------------------
class TForm_Principale: public TForm, public TFVCLIDEAutomation {
private:

  AnsiString asFileWorkSpace;
  std::vector<TABSHEET> TabSheets;
  int NbTabSheet;				// Pour éviter les doublons. TabSheets.size() ne marche pas si on supprime et qu'on crée des onglets.
	bool bTempWorkSpace;  // WorkSpace temporaire (fichiers en ligne de commande) => il ne faut pas l'enregistrer
	bool bRecMacro;				// Enregistrement d'une macro en cours
	bool bFromAnotherApp; // True si on vient d'une autre application
  std::vector<KEYACTION> KeyActions;

	TForm_SearchAndReplace *Form_SearchAndReplace;
  SEARCHPARAM SearchParam;
	int ReachBeginOrEnd;  // 0 = aucun, 1 = début de fichier, 2 = Fin.

  TForm_Autocompletion *Form_Autocompletion;

	TListTypeProjects *ListTypeProjects;
	bool bDebugging;      // True si debogage en cours
	bool bBuilding;      // True si build en cours
	TProject *CurrentProject;
	TProject *DebugProject;
  AnsiString asDebugFileName;
	int DebugNumLine1;
	int DebugNumCol1;
	int DebugNumLine2;
	int DebugNumCol2;

	bool bCodeOnly;
	bool bMemoCodeOnly;

	TClientSocket *ClientSocket;
	char * szBufSocket;
	bool bByteReady;


  bool FASTCALL CheckUpdatesStart(void);
  int FASTCALL CheckUpdates(bool AffichageErrors);

  bool FASTCALL QuestEnrModifs(void);
  bool FASTCALL QuestEnrModifsFiles(void);
  bool FASTCALL QuestEnrModifsFile(int Index);
  bool FASTCALL QuestEnrModifsProjects(void);

  bool FASTCALL QuestDebugging(void);

  void FASTCALL ReadConfig(void);
  void FASTCALL SaveConfig(void);

	void FASTCALL SearchAndReplace(int TabIndex);
	bool FASTCALL UpdateTreeView(TXMLFile *XMLNode);
	bool FASTCALL UpdateTreeNode(TTreeNode *TreeNode, TXMLFile *XMLNode);
	bool FASTCALL UpdateXMLTree(TXMLFile *XMLNode);
	bool FASTCALL UpdateXMLNode(TTreeNode *TreeNode, TXMLFile *XMLNode);
	bool FASTCALL UpdateTreeViewStates(void);
	bool FASTCALL UpdateTreeNodeStates(TTreeNode *TreeNode);
  bool FASTCALL SaveWorkSpaceIfNotEmpty(void);

	bool FASTCALL AddNewFile(TTreeNode *NodeParent);
	bool FASTCALL AddExistingFile(TTreeNode *NodeParent);
	bool FASTCALL AddNewProject(TTreeNode *NodeParent);
	bool FASTCALL AddExistingProject(TTreeNode *NodeParent);
	bool FASTCALL AddProject(TTreeNode *NodeParent, TProject *ProjectParent, AnsiString asProjectFileName, AnsiString asProjectName, AnsiString asProjectType);

	bool FASTCALL RemoveDependencies(TTreeNode *Node);
	bool FASTCALL InsertDependencies(TTreeNode *Node);
	bool FASTCALL SaveFilesOfNode(TTreeNode *Node);
	bool FASTCALL PreCompilationNode(TTreeNode *Node);
	bool FASTCALL BuildNode(TTreeNode *Node, bool bAll);
	bool FASTCALL MustBuildNode(TTreeNode *Node, bool *bMustBuild, AnsiString *asMessage);
	bool FASTCALL RunNode(TTreeNode *Node);
	bool FASTCALL NodeToDebug(TTreeNode *Node, TProject **ProjectToDebug);
  bool FASTCALL StartDebug(void);
  void FASTCALL StepByStep(TYPESTEPBYSTEP TypeStepByStep);

	bool FASTCALL LogLineDblClick(AnsiString asLine);
	bool FASTCALL DecodeLineBookmark(AnsiString asLine,
			AnsiString *asFileName,
			bool *bTextFile, int *NumCol, int *NumLine,
			bool *bHexaFile, __int64 *FilePos);

	bool FASTCALL AddUserToolsInMenu(void);

	bool AddBitmapToImageList(TProject *Project);

	bool SetCurrentDebugLine(AnsiString asFileName, int NumLine1, int NumCol1, int NumLine2, int NumCol2);

	bool FASTCALL SpacesToTabs(bool bBeginOnly);

	bool FASTCALL PlayMacro(bool bStopBegin, bool bStopEnd);

	TTreeNode * FASTCALL TreeViewFindNode(const AnsiString asNodeName);
	bool FASTCALL TreeViewDeleteNode(TTreeNode *NodeChild);
	TTreeNode * FASTCALL TreeViewAddNodeProject(TTreeNode *TreeNodeParent, AnsiString asName);

	bool CopyReplaceFile(AnsiString asSrcFileName, AnsiString asDestFileName, AnsiString asName);

	bool FileLineInput(HANDLE hFile, CharString *csBuffer);

public:
  FVCL_BEGIN_COMPONENTS
  // TCoolBar *CoolBar1;
	TAction *Action_NewWorkSpace;
  TAction *Action_OpenWorkSpace;
  TAction *Action_SaveWorkSpace;
  TAction *Action_SaveAsWorkSpace;
  TAction *Action_CloseWorkSpace;
  TAction *Action_AddNewFileOnCurrent;
  TAction *Action_AddExistingFileOnCurrent;
  TAction *Action_AddNewProjectOnCurrent;
  TAction *Action_AddExistingProjectOnCurrent;
  TAction *Action_CloseProject;
  TAction *Action_ExpandNode;
  TAction *Action_CollapseNode;
  TAction *Action_AddNewFileOnSelected;
  TAction *Action_AddExistingFileOnSelected;
  TAction *Action_AddNewProjectOnSelected;
  TAction *Action_AddExistingProjectOnSelected;
  TAction *Action_RenameProject;
	TAction *Action_RemoveProject;
  TAction *Action_EditProject;
  TAction *Action_EditFile;
  TAction *Action_OpenProjectFile;
	TAction *Action_NewTextFile;
  TAction *Action_NewBinFile;
  TAction *Action_OpenTextFile;
  TAction *Action_OpenBinFile;
  TAction *Action_SaveFile;
  TAction *Action_SaveAllFiles;
  TAction *Action_SaveAsFile;
  TAction *Action_CopyPath;
  TAction *Action_OpenFolder;
  TAction *Action_OpenFichierAssocie1;
  TAction *Action_OpenFichierAssocie2;
  TAction *Action_NewDialog;
  TAction *Action_OpenDialog;
  TAction *Action_SaveDialog;
  TAction *Action_SaveAsDialog;
	TAction *Action_NewBmp;
  TAction *Action_NewIcon;
  TAction *Action_NewCursor;
  TAction *Action_OpenGraphic;
  TAction *Action_SaveGraphic;
  TAction *Action_SaveAsGraphic;
  TAction *Action_Close;
  TAction *Action_CloseAll;
  TAction *Action_CloseOthers;
  TAction *Action_Print;
  TAction *Action_Quit;
  TAction *Action_Undo;
  TAction *Action_Redo;
  TAction *Action_CutToClipboard;
  TAction *Action_CopyToClipboard;
  TAction *Action_PasteFromClipboard;
  TAction *Action_RefPrevious;
  TAction *Action_RefNext;
  TAction *Action_SelectAll;
  TAction *Action_Goto;
  TAction *Action_LowerCase;
  TAction *Action_UpperCase;
  TAction *Action_Comment;
  TAction *Action_Uncomment;
  TAction *Action_AddBookmark;
  TAction *Action_DelBookmark;
  TAction *Action_ClearBookmarks;
  TAction *Action_FileFormat;
  TAction *Action_TabsToSpaces;
  TAction *Action_SpacesToTabs;
  TAction *Action_SpacesToTabsBegin;
  TAction *Action_EndingSpaces;
	TAction *Action_Find;
  TAction *Action_FindInFiles;
  TAction *Action_Replace;
  TAction *Action_ReplaceInFiles;
  TAction *Action_FindPrevious;
  TAction *Action_FindNext;
  TAction *Action_FindSelPrevious;
  TAction *Action_FindSelNext;
  TAction *Action_RecMacro;
  TAction *Action_PlayMacro;
  TAction *Action_RepeatMacro;
  TAction *Action_Models;
  TAction *Action_PreCompilCurrent;
  TAction *Action_BuildCurent;
  TAction *Action_RebuildCurrent;
  TAction *Action_PreCompilSelected;
  TAction *Action_BuildSelected;
  TAction *Action_RebuildSelected;
  TAction *Action_BuildAll;
  TAction *Action_PreCompilAll;
  TAction *Action_RebuildAll;
  TAction *Action_SetCurrent;
  TAction *Action_ViewOutput;
  TAction *Action_Properties;
  TAction *Action_RunCurrentCurrent;
  TAction *Action_DebugCurrentCurrent;
  TAction *Action_StopDebug;
  TAction *Action_StepByStepNext;
  TAction *Action_StepByStepIn;
  TAction *Action_StepByStepOut;
  TAction *Action_AddBreakpoint;
  TAction *Action_DelBreakpoint;
  TAction *Action_ClearBreakpoints;
  TAction *Action_Statistics;
  TAction *Action_CodeFormat;
  TAction *Action_ConvertNumbers;
  TAction *Action_ConfigTools;
  TAction *Action_Language;
  TAction *Action_CodeOnly;
  TAction *Action_EdiOptionsGenerales;
  TAction *Action_ShortCuts;
  TAction *Action_Colors;
  TAction *Action_Font;
  TAction *Action_CodeFormatParams;
  TAction *Action_DefaultResponses;
  TAction *Action_FVCLIDEHelp;
  TAction *Action_FreeVCLHelp;
  TAction *Action_Autocompletion;
  TAction *Action_APropos;
  TAction *Action_ContactMe;
  TAction *Action_MiseAJour;
	TAction *Action_LogCopyLine;
	TAction *Action_LogCopyAll;
	TMenuItem *MenuItem_NewWorkSpace;
  TMenuItem *MenuItem_OpenWorkSpace;
  TMenuItem *MenuItem_SaveWorkSpace;
  TMenuItem *MenuItem_SaveAsWorkSpace;
  TMenuItem *MenuItem_CloseWorkSpace;
  TMenuItem *MenuItem_Separator1;
  TMenuItem *MenuItem_WorkSpaces;
	TMenuItem *MenuItem_NewProject;
  TMenuItem *MenuItem_OpenProject;
  TMenuItem *MenuItem_CloseProject;
  TMenuItem *MenuItem_Separator2;
  TMenuItem *MenuItem_Projects;
  TMenuItem *MenuItem_NewTextFile;
  TMenuItem *MenuItem_NewBinFile;
  TMenuItem *MenuItem_NewFile;
  TMenuItem *MenuItem_OpenTextFile;
  TMenuItem *MenuItem_OpenBinFile;
  TMenuItem *MenuItem_OpenFile;
  TMenuItem *MenuItem_SaveFile;
  TMenuItem *MenuItem_SaveAllFiles;
  TMenuItem *MenuItem_SaveAsFile;
  TMenuItem *MenuItem_CloseFile;
  TMenuItem *MenuItem_Separator3;
  TMenuItem *MenuItem_Files;
  TMenuItem *MenuItem_NewDialog;
  TMenuItem *MenuItem_OpenDialog;
  TMenuItem *MenuItem_SaveDialog;
  TMenuItem *MenuItem_SaveAsDialog;
  TMenuItem *MenuItem_CloseDialog;
  TMenuItem *MenuItem_Separator4;
  TMenuItem *MenuItem_Dialogs;
  TMenuItem *MenuItem_NewBmp;
  TMenuItem *MenuItem_NewIcon;
  TMenuItem *MenuItem_NewCursor;
  TMenuItem *MenuItem_NewGraphic;
  TMenuItem *MenuItem_OpenGraphic;
  TMenuItem *MenuItem_SaveGraphic;
  TMenuItem *MenuItem_SaveAsGraphic;
  TMenuItem *MenuItem_CloseGraphic;
  TMenuItem *MenuItem_Separator5;
  TMenuItem *MenuItem_Graphics;
  TMenuItem *MenuItem_Separator6;
  TMenuItem *MenuItem_Close;
  TMenuItem *MenuItem_CloseAll;
  TMenuItem *MenuItem_CloseOthers;
  TMenuItem *MenuItem_Separator7;
	TMenuItem *MenuItem_Print;
  TMenuItem *MenuItem_Separator8;
  TMenuItem *MenuItem_Quit;
  TMenuItem *MenuItem_FilesAndProjects;
  TMenuItem *MenuItem_Undo;
  TMenuItem *MenuItem_Redo;
  TMenuItem *MenuItem_Separator9;
  TMenuItem *MenuItem_CutToClipboard;
  TMenuItem *MenuItem_CopyToClipboard;
  TMenuItem *MenuItem_PasteFromClipboard;
  TMenuItem *MenuItem_Separator10;
  TMenuItem *MenuItem_RefPrevious;
  TMenuItem *MenuItem_RefNext;
  TMenuItem *MenuItem_Separator11;
  TMenuItem *MenuItem_SelectAll;
  TMenuItem *MenuItem_Goto;
  TMenuItem *MenuItem_Separator12;
  TMenuItem *MenuItem_LowerCase;
  TMenuItem *MenuItem_UpperCase;
  TMenuItem *MenuItem_Separator13;
  TMenuItem *MenuItem_Comment;
  TMenuItem *MenuItem_Uncomment;
  TMenuItem *MenuItem_Advanced;
  TMenuItem *MenuItem_AddBookmark;
  TMenuItem *MenuItem_DelBookmark;
  TMenuItem *MenuItem_ClearBookmarks;
  TMenuItem *MenuItem_Bookmarks;
  TMenuItem *MenuItem_FileFormat;
  TMenuItem *MenuItem_TabsToSpaces;
  TMenuItem *MenuItem_SpacesToTabs;
  TMenuItem *MenuItem_SpacesToTabsBegin;
  TMenuItem *MenuItem_EndingSpaces;
  TMenuItem *MenuItem_Document;
  TMenuItem *MenuItem_Edit;
  TMenuItem *MenuItem_Find;
  TMenuItem *MenuItem_FindInFile;
  TMenuItem *MenuItem_Separator14;
  TMenuItem *MenuItem_Replace;
  TMenuItem *MenuItem_ReplaceInFiles;
  TMenuItem *MenuItem_Separator15;
  TMenuItem *MenuItem_FindPrevious;
  TMenuItem *MenuItem_FindNext;
  TMenuItem *MenuItem_FindSelPrevious;
  TMenuItem *MenuItem_FindSelNext;
  TMenuItem *MenuItem_FindAndReplace;
  TMenuItem *MenuItem_RecMacro;
  TMenuItem *MenuItem_PlayMacro;
  TMenuItem *MenuItem_RepeatMacro;
  TMenuItem *MenuItem_Separator16;
  TMenuItem *MenuItem_Models;
  TMenuItem *MenuItem_Macros;
  TMenuItem *MenuItem_PreCompil;
  TMenuItem *MenuItem_Build;
  TMenuItem *MenuItem_Rebuild;
  TMenuItem *MenuItem_Separator17;
  TMenuItem *MenuItem_PreCompilAll;
  TMenuItem *MenuItem_BuildAll;
  TMenuItem *MenuItem_RebuildAll;
  TMenuItem *MenuItem_Separator18;
  TMenuItem *MenuItem_SetCurrent;
  TMenuItem *MenuItem_ViewOutput;
  TMenuItem *MenuItem_BuildAndView;
  TMenuItem *MenuItem_Run;
  TMenuItem *MenuItem_Debug;
  TMenuItem *MenuItem_StopDebug;
  TMenuItem *MenuItem_Separator19;
  TMenuItem *MenuItem_StepByStepNext;
  TMenuItem *MenuItem_StepByStepIn;
  TMenuItem *MenuItem_StepByStepOut;
  TMenuItem *MenuItem_Separator20;
  TMenuItem *MenuItem_AddBreakpoint;
  TMenuItem *MenuItem_DelBreakpoint;
  TMenuItem *MenuItem_ClearBreakpoints;
  TMenuItem *MenuItem_Breakpoints;
  TMenuItem *MenuItem_DebugAndRun;
  TMenuItem *MenuItem_Statistics;
  TMenuItem *MenuItem_CodeFormat;
  TMenuItem *MenuItem_ConvertNumbers;
  TMenuItem *MenuItem_Separator21;
  TMenuItem *MenuItem_ConfigTools;
  TMenuItem *MenuItem_Separator22;
  TMenuItem *MenuItem_UserTools;
  TMenuItem *MenuItem_Tools;
  TMenuItem *MenuItem_Language;
  TMenuItem *MenuItem_CodeOnly;
  TMenuItem *MenuItem_Separator23;
  TMenuItem *MenuItem_EdiOptionsGenerales;
  TMenuItem *MenuItem_ShortCuts;
  TMenuItem *MenuItem_Colors;
  TMenuItem *MenuItem_Font;
  TMenuItem *MenuItem_CodeFormatParams;
  TMenuItem *MenuItem_DefaultResponses;
  TMenuItem *MenuItem_Options;
  TMenuItem *MenuItem_FVCLIDEHelp;
  TMenuItem *MenuItem_FreeVCLHelp;
  TMenuItem *MenuItem_Autocompletion;
  TMenuItem *MenuItem_APropos;
  TMenuItem *MenuItem_ContactMe;
  TMenuItem *MenuItem_MiseAJour;
  TMenuItem *MenuItem_Help;
  TMainMenu *MainMenu;
  TMenuItem *MenuItemPopup_ExpandNode;
  TMenuItem *MenuItemPopup_CollapseNode;
  TMenuItem *MenuItem_Separator24;
  TMenuItem *MenuItemPopup_AddNewFileOnSelected;
  TMenuItem *MenuItemPopup_AddExistingFileOnSelected;
  TMenuItem *MenuItem_Separator25;
  TMenuItem *MenuItemPopup_AddNewProjectOnSelected;
  TMenuItem *MenuItemPopup_AddExistingProjectOnSelected;
  TMenuItem *MenuItemPopup_RenameProject;
  TMenuItem *MenuItemPopup_RemoveProject;
  TMenuItem *MenuItem_Separator26;
  TMenuItem *MenuItemPopup_EditProject;
  TMenuItem *MenuItemPopup_EditFile;
  TMenuItem *MenuItemPopup_OpenProjectFile;
  TMenuItem *MenuItem_Separator27;
  TMenuItem *MenuItemPopup_PreCompil;
  TMenuItem *MenuItemPopup_Build;
  TMenuItem *MenuItemPopup_Rebuild;
  TMenuItem *MenuItem_Separator28;
  TMenuItem *MenuItemPopup_SetCurrent;
  TMenuItem *MenuItemPopup_ViewOutput;
  TMenuItem *MenuItem_Separator29;
  TMenuItem *MenuItemPopup_Properties;
  TPopupMenu *PopupMenu_Projects;
  TMenuItem *MenuItemPopup_Close;
  TMenuItem *MenuItemPopup_CloseAll;
  TMenuItem *MenuItemPopup_CloseOthers;
  TMenuItem *MenuItem_Separator30;
  TMenuItem *MenuItemPopup_CopyPath;
  TMenuItem *MenuItemPopup_OpenFolder;
  TMenuItem *MenuItemPopup_OpenFichierAssocie1;
  TMenuItem *MenuItemPopup_OpenFichierAssocie2;
	TPopupMenu *PopupMenu_Files;
  TImageList *ImageList_Buttons;
  TToolButton *ToolButton_NewTextFile;
  TToolButton *ToolButton_OpenTextFile;
  TToolButton *ToolButton_SaveFile;
  TToolButton *ToolButton_SaveAllFiles;
  TToolButton *ToolButton_AsSaveFile;
  TToolButton *ToolButton_Separator1;
  TToolButton *ToolButton_BuildCurrent;
  TToolButton *ToolButton_RunCurrent;
  TToolButton *ToolButton_DebugCurrent;
  TToolButton *ToolButton_Separator2;
  TToolButton *ToolButton_CodeOnly;
  TToolBar *ToolBar;
  TStatusBar *StatusBar;
	TMenuItem *MenuItemPopup_LogCopyLine;
	TMenuItem *MenuItemPopup_LogCopyAll;
	TPopupMenu *PopupMenu_Log;
  TListBox *ListBox_Search;
  TTabSheet *TabSheet_Search;
  TListBox *ListBox_Bookmarks;
  TTabSheet *TabSheet_Bookmarks;
  TListBox *ListBox_Build;
  TTabSheet *TabSheet_Build;
  TListBox *ListBox_Process;
  TTabSheet *TabSheet_Process;
  TListBox *ListBox_Breakpoints;
  TTabSheet *TabSheet_Breakpoints;
  TListBox *ListBox_CallStack;
  TTabSheet *TabSheet_CallStack;
  TPageControl *PageControl_Logs;
  TSplitter *Splitter1;
	TImageList *ImageList_TreeStates;
	TImageList *ImageList_TypeProjects;
  TTreeView *TreeView_Projects;
  TSplitter *Splitter2;
  TPageControl *PageControl_Files;
  TSaveDialog *OpenDialog_WorkSpace;
  TOpenDialog *OpenDialog_Project;
  TOpenDialog *OpenDialog_File;
  TOpenDialog *OpenDialog_Dialog;
  TOpenDialog *OpenDialog_Graphic;
  TSaveDialog *SaveDialog_WorkSpace;
  TSaveDialog *SaveDialog_Project;
  TSaveDialog *SaveDialog_File;
  TSaveDialog *SaveDialog_Dialog;
  TSaveDialog *SaveDialog_Graphic;
  TFontDialog *FontDialog;
  TTimer *Timer_Activate;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL ApplicationActivate(TObject *Sender);
  void FASTCALL ApplicationMinimize(TObject *Sender);
  void FASTCALL ApplicationRestore(TObject *Sender);
  void FASTCALL FormActivate(TObject *Sender);
  void FASTCALL Action_NewWorkSpaceExecute(TObject *Sender);
  void FASTCALL Action_OpenWorkSpaceExecute(TObject *Sender);
  void FASTCALL Action_SaveWorkSpaceExecute(TObject *Sender);
  void FASTCALL Action_SaveAsWorkSpaceExecute(TObject *Sender);
  void FASTCALL Action_CloseWorkSpaceExecute(TObject *Sender);
  void FASTCALL MenuItem_LastWorkSpaceClick(TObject *Sender);
  void FASTCALL Action_AddNewFileOnCurrentExecute(TObject *Sender);
  void FASTCALL Action_AddExistingFileOnCurrentExecute(TObject *Sender);
  void FASTCALL Action_AddNewProjectOnCurrentExecute(TObject *Sender);
  void FASTCALL Action_AddExistingProjectOnCurrentExecute(TObject *Sender);
  void FASTCALL Action_CloseProjectExecute(TObject *Sender);
  void FASTCALL MenuItem_LastProjectClick(TObject *Sender);
  void FASTCALL Action_ExpandNodeExecute(TObject *Sender);
  void FASTCALL Action_CollapseNodeExecute(TObject *Sender);
  void FASTCALL Action_AddNewFileOnSelectedExecute(TObject *Sender);
  void FASTCALL Action_AddExistingFileOnSelectedExecute(TObject *Sender);
  void FASTCALL Action_AddNewProjectOnSelectedExecute(TObject *Sender);
  void FASTCALL Action_AddExistingProjectOnSelectedExecute(TObject *Sender);
  void FASTCALL Action_RenameProjectExecute(TObject *Sender);
  void FASTCALL Action_RemoveProjectExecute(TObject *Sender);
  void FASTCALL Action_EditProjectExecute(TObject *Sender);
  void FASTCALL Action_EditFileExecute(TObject *Sender);
  void FASTCALL Action_OpenProjectFileExecute(TObject *Sender);
	void FASTCALL Action_NewTextFileExecute(TObject *Sender);
  void FASTCALL Action_NewBinFileExecute(TObject *Sender);
  void FASTCALL Action_OpenTextFileExecute(TObject *Sender);
  void FASTCALL Action_OpenBinFileExecute(TObject *Sender);
  void FASTCALL Action_SaveFileExecute(TObject *Sender);
  void FASTCALL Action_SaveAllFilesExecute(TObject *Sender);
  void FASTCALL Action_SaveAsFileExecute(TObject *Sender);
  void FASTCALL MenuItem_LastFileClick(TObject *Sender);
  void FASTCALL Action_CopyPathExecute(TObject *Sender);
  void FASTCALL Action_OpenFolderExecute(TObject *Sender);
  void FASTCALL Action_OpenFichierAssocie1Execute(TObject *Sender);
  void FASTCALL Action_OpenFichierAssocie2Execute(TObject *Sender);
  void FASTCALL Action_NewDialogExecute(TObject *Sender);
  void FASTCALL Action_OpenDialogExecute(TObject *Sender);
  void FASTCALL Action_SaveDialogExecute(TObject *Sender);
  void FASTCALL Action_SaveAsDialogExecute(TObject *Sender);
  void FASTCALL MenuItem_LastDialogClick(TObject *Sender);
	void FASTCALL Action_NewBmpExecute(TObject *Sender);
  void FASTCALL Action_NewIconExecute(TObject *Sender);
  void FASTCALL Action_NewCursorExecute(TObject *Sender);
  void FASTCALL Action_OpenGraphicExecute(TObject *Sender);
  void FASTCALL Action_SaveGraphicExecute(TObject *Sender);
  void FASTCALL Action_SaveAsGraphicExecute(TObject *Sender);
  void FASTCALL MenuItem_LastGraphicClick(TObject *Sender);
  void FASTCALL Action_CloseExecute(TObject *Sender);
  void FASTCALL Action_CloseAllExecute(TObject *Sender);
  void FASTCALL Action_CloseOthersExecute(TObject *Sender);
	void FASTCALL Action_PrintExecute(TObject *Sender);
  void FASTCALL Action_QuitExecute(TObject *Sender);
  void FASTCALL Action_UndoExecute(TObject *Sender);
  void FASTCALL Action_RedoExecute(TObject *Sender);
  void FASTCALL Action_CutToClipboardExecute(TObject *Sender);
  void FASTCALL Action_CopyToClipboardExecute(TObject *Sender);
  void FASTCALL Action_PasteFromClipboardExecute(TObject *Sender);
  void FASTCALL Action_RefPreviousExecute(TObject *Sender);
  void FASTCALL Action_RefNextExecute(TObject *Sender);
  void FASTCALL Action_SelectAllExecute(TObject *Sender);
  void FASTCALL Action_GotoExecute(TObject *Sender);
  void FASTCALL Action_LowerCaseExecute(TObject *Sender);
  void FASTCALL Action_UpperCaseExecute(TObject *Sender);
  void FASTCALL Action_CommentExecute(TObject *Sender);
  void FASTCALL Action_UncommentExecute(TObject *Sender);
  void FASTCALL Action_AddBookmarkExecute(TObject *Sender);
  void FASTCALL Action_DelBookmarkExecute(TObject *Sender);
  void FASTCALL Action_ClearBookmarksExecute(TObject *Sender);
  void FASTCALL Action_FileFormatExecute(TObject *Sender);
  void FASTCALL Action_TabsToSpacesExecute(TObject *Sender);
  void FASTCALL Action_SpacesToTabsExecute(TObject *Sender);
  void FASTCALL Action_SpacesToTabsBeginExecute(TObject *Sender);
  void FASTCALL Action_EndingSpacesExecute(TObject *Sender);
  void FASTCALL Action_FindExecute(TObject *Sender);
  void FASTCALL Action_FindInFilesExecute(TObject *Sender);
  void FASTCALL Action_ReplaceExecute(TObject *Sender);
  void FASTCALL Action_ReplaceInFilesExecute(TObject *Sender);
  void FASTCALL Action_FindPreviousExecute(TObject *Sender);
  void FASTCALL Action_FindNextExecute(TObject *Sender);
  void FASTCALL Action_FindSelPreviousExecute(TObject *Sender);
  void FASTCALL Action_FindSelNextExecute(TObject *Sender);
  void FASTCALL Action_RecMacroExecute(TObject *Sender);
  void FASTCALL Action_PlayMacroExecute(TObject *Sender);
  void FASTCALL Action_RepeatMacroExecute(TObject *Sender);
  void FASTCALL Action_ModelsExecute(TObject *Sender);
  void FASTCALL Action_PreCompilCurrentExecute(TObject *Sender);
  void FASTCALL Action_BuildCurrentExecute(TObject *Sender);
  void FASTCALL Action_RebuildCurrentExecute(TObject *Sender);
  void FASTCALL Action_PreCompilSelectedExecute(TObject *Sender);
  void FASTCALL Action_BuildSelectedExecute(TObject *Sender);
  void FASTCALL Action_RebuildSelectedExecute(TObject *Sender);
  void FASTCALL Action_PreCompilAllExecute(TObject *Sender);
  void FASTCALL Action_BuildAllExecute(TObject *Sender);
  void FASTCALL Action_RebuildAllExecute(TObject *Sender);
  void FASTCALL Action_SetCurrentExecute(TObject *Sender);
  void FASTCALL Action_ViewOutputExecute(TObject *Sender);
  void FASTCALL Action_PropertiesExecute(TObject *Sender);
  void FASTCALL Action_RunCurrentExecute(TObject *Sender);
  void FASTCALL Action_DebugCurrentExecute(TObject *Sender);
  void FASTCALL Action_StopDebugExecute(TObject *Sender);
  void FASTCALL Action_StepByStepNextExecute(TObject *Sender);
  void FASTCALL Action_StepByStepInExecute(TObject *Sender);
  void FASTCALL Action_StepByStepOutExecute(TObject *Sender);
  void FASTCALL Action_AddBreakpointExecute(TObject *Sender);
  void FASTCALL Action_DelBreakpointExecute(TObject *Sender);
  void FASTCALL Action_ClearBreakpointsExecute(TObject *Sender);
  void FASTCALL Action_StatisticsExecute(TObject *Sender);
  void FASTCALL Action_CodeFormatExecute(TObject *Sender);
  void FASTCALL Action_ConvertNumbersExecute(TObject *Sender);
  void FASTCALL Action_LanguageExecute(TObject *Sender);
  void FASTCALL Action_CodeOnlyExecute(TObject *Sender);
  void FASTCALL Action_ConfigToolsExecute(TObject *Sender);
  void FASTCALL MenuItem_UserToolClick(TObject *Sender);
  void FASTCALL Action_FVCLIDEHelpExecute(TObject *Sender);
  void FASTCALL Action_FreeVCLHelpExecute(TObject *Sender);
  void FASTCALL Action_AutocompletionExecute(TObject *Sender);
  void FASTCALL Action_AProposExecute(TObject *Sender);
  void FASTCALL Action_ContactMeExecute(TObject *Sender);
  void FASTCALL Action_MiseAJourExecute(TObject *Sender);
  void FASTCALL Action_LogCopyLineExecute(TObject *Sender);
  void FASTCALL Action_LogCopyAllExecute(TObject *Sender);
  void FASTCALL Action_EdiOptionsGeneralesExecute(TObject *Sender);
  void FASTCALL Action_ShortCutsExecute(TObject *Sender);
  void FASTCALL Action_ColorsExecute(TObject *Sender);
  void FASTCALL Action_FontExecute(TObject *Sender);
  void FASTCALL Action_CodeFormatParamsExecute(TObject *Sender);
  void FASTCALL Action_DefaultResponsesExecute(TObject *Sender);
  void FASTCALL FormClose(TObject *Sender, TCloseAction &Action);
	void FASTCALL ObjectFlagChanged(TObject *Sender, bool bChanged);
	void FASTCALL ObjectPassBegin(TObject *Sender);
	void FASTCALL ObjectPassEnd(TObject *Sender);
	void FASTCALL ObjectAutocompletionOn(TObject *Sender, AnsiString asParent, int X, int Y, int NumCompletion);
	void FASTCALL ObjectAutocompletionOff(TObject *Sender);
	void FASTCALL TextEditorHint(TObject *Sender);
	void FASTCALL PageControl_FilesChange(TObject *Sender);
  void FASTCALL PageControl_FilesMouseMove(TObject *Sender,
        TShiftState Shift, int X, int Y);
  void FASTCALL PageControl_FilesMouseDown(TObject *Sender,
        TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL PageControl_FilesDropFile(TObject *Sender,
        AnsiString &asFileName, TShiftState Shift, int X, int Y);
	void FASTCALL ListBox_SearchDblClick(TObject *Sender);
	void FASTCALL ListBox_BookmarksDblClick(TObject *Sender);
	void FASTCALL ListBox_BuildDblClick(TObject *Sender);
	void FASTCALL ListBox_ProcessDblClick(TObject *Sender);
	void FASTCALL ListBox_BreakpointsDblClick(TObject *Sender);
	void FASTCALL ListBox_CallStackDblClick(TObject *Sender);
	void FASTCALL ListBox_LogContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled);
	void FASTCALL TreeView_ProjectsEdited(TObject *Sender, TTreeNode* Node, AnsiString NewText);
	void FASTCALL TreeView_ProjectsMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL TreeView_ProjectsDragOver(TObject *Sender,
          TObject *Source, int X, int Y, TDragState State, bool &Accept);
  void FASTCALL TreeView_ProjectsEndDrag(TObject *Sender,
          TObject *Target, int X, int Y);
	void FASTCALL Form_PrincipaleShortCut(TObject *Sender, int nVirtKey, TShiftState Shift, bool &Handled);
	void FASTCALL PopupMenu_ProjectsPopup(TObject *Sender);
	void FASTCALL PopupMenu_FilesPopup(TObject *Sender);
	void ClientSocketRead(TObject* Sender, TCustomWinSocket* Socket);
	void ClientSocketError(TObject* Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &Error);
	void FASTCALL Timer_ActivateTimer(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TForm_Principale, TOnActivate, OnActivate, ApplicationActivate, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnMinimize, OnMinimize, ApplicationMinimize, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnRestore, OnRestore, ApplicationRestore, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnActivate, OnActivate, FormActivate, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_NewWorkSpaceExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenWorkSpaceExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveWorkSpaceExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveAsWorkSpaceExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CloseWorkSpaceExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_LastWorkSpaceClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddNewFileOnCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddExistingFileOnCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddNewProjectOnCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddExistingProjectOnCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CloseProjectExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_LastProjectClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ExpandNodeExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CollapseNodeExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddNewFileOnSelectedExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddExistingFileOnSelectedExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddNewProjectOnSelectedExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddExistingProjectOnSelectedExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RenameProjectExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RemoveProjectExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_EditProjectExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_EditFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenProjectFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_NewTextFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_NewBinFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenTextFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenBinFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveAllFilesExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveAsFileExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_LastFileClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CopyPathExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenFolderExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenFichierAssocie1Execute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenFichierAssocie2Execute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_NewDialogExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenDialogExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveDialogExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveAsDialogExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_LastDialogClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_NewBmpExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_NewIconExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_NewCursorExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_OpenGraphicExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveGraphicExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SaveAsGraphicExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_LastGraphicClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CloseExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CloseAllExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CloseOthersExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_PrintExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_QuitExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_UndoExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RedoExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CutToClipboardExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CopyToClipboardExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_PasteFromClipboardExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RefPreviousExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RefNextExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SelectAllExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_GotoExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_LowerCaseExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_UpperCaseExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CommentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_UncommentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddBookmarkExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_DelBookmarkExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ClearBookmarksExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FileFormatExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_TabsToSpacesExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SpacesToTabsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SpacesToTabsBeginExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_EndingSpacesExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FindExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FindInFilesExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ReplaceExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ReplaceInFilesExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FindPreviousExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FindNextExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FindSelPreviousExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FindSelNextExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RecMacroExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_PlayMacroExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RepeatMacroExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ModelsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_PreCompilCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_BuildCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RebuildCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_PreCompilSelectedExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_BuildSelectedExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RebuildSelectedExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_PreCompilAllExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_BuildAllExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RebuildAllExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_SetCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ViewOutputExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_PropertiesExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_RunCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_DebugCurrentExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_StopDebugExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_StepByStepNextExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_StepByStepInExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_StepByStepOutExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AddBreakpointExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_DelBreakpointExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ClearBreakpointsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_StatisticsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CodeFormatExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ConvertNumbersExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_LanguageExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CodeOnlyExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ConfigToolsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_UserToolClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FVCLIDEHelpExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FreeVCLHelpExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AutocompletionExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_AProposExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ContactMeExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_MiseAJourExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_LogCopyLineExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_LogCopyAllExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_EdiOptionsGeneralesExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ShortCutsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_ColorsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_FontExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_CodeFormatParamsExecute, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnExecute, OnExecute, Action_DefaultResponsesExecute, TObject *);
		SET_EVENT_STR_2(TForm_Principale, TOnClose, OnClose, FormClose, TObject *, TCloseAction &);
		SET_EVENT_STR_2(TForm_Principale, TOnFlagChanged, OnFlagChanged, ObjectFlagChanged, TObject *, bool);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnPassBegin, ObjectPassBegin, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnPassEnd, ObjectPassEnd, TObject *);
		SET_EVENT_STR_5(TForm_Principale, TOnAutocompletionOn, OnAutocompletionOn, ObjectAutocompletionOn, TObject *, AnsiString, int, int, int);
		SET_EVENT_STR_1(TForm_Principale, TOnAutocompletionOff, OnAutocompletionOff, ObjectAutocompletionOff, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnHint, OnHint, TextEditorHint, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnChange, PageControl_FilesChange, TObject *);
		SET_EVENT_STR_4(TForm_Principale, TOnMouseMove, OnMouseMove, PageControl_FilesMouseMove, TObject *, TShiftState, int, int);
		SET_EVENT_STR_5(TForm_Principale, TOnMouseDown, OnMouseDown, PageControl_FilesMouseDown, TObject *, TMouseButton, TShiftState, int, int);
		SET_EVENT_STR_5(TForm_Principale, TOnDropFile, OnDropFile, PageControl_FilesDropFile, TObject *, AnsiString, TShiftState, int, int);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnDblClick, ListBox_SearchDblClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnDblClick, ListBox_BookmarksDblClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnDblClick, ListBox_BuildDblClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnDblClick, ListBox_ProcessDblClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnDblClick, ListBox_BreakpointsDblClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnDblClick, ListBox_CallStackDblClick, TObject *);
		SET_EVENT_STR_3(TForm_Principale, TOnContextPopup, OnContextPopup, ListBox_LogContextPopup, TObject *, const TPoint &, bool &);
		SET_EVENT_STR_3(TForm_Principale, TOnEdited, OnEdited, TreeView_ProjectsEdited, TObject *, TTreeNode*, AnsiString);
		SET_EVENT_STR_5(TForm_Principale, TOnMouseDown, OnMouseDown, TreeView_ProjectsMouseDown, TObject *, TMouseButton, TShiftState, int, int);
		SET_EVENT_STR_6(TForm_Principale, TOnDragOver, OnDragOver, TreeView_ProjectsDragOver, TObject *, TObject *, int, int, TDragState, bool &);
		SET_EVENT_STR_4(TForm_Principale, TOnEndDrag, OnEndDrag, TreeView_ProjectsEndDrag, TObject *, TObject *, int, int);
		SET_EVENT_STR_4(TForm_Principale, TOnShortCut, OnShortCut, Form_PrincipaleShortCut, TObject *, int, TShiftState, bool &);
		SET_EVENT_STR_1(TForm_Principale, TOnPopup, OnPopup, PopupMenu_ProjectsPopup, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TOnPopup, OnPopup, PopupMenu_FilesPopup, TObject *);
		SET_EVENT_STR_2(TForm_Principale, TOnRead, OnRead, ClientSocketRead, TObject*, TCustomWinSocket*);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnTimer, Timer_ActivateTimer, TObject*);
  }

  FVCL_END_EVENTS

  TForm_Principale(TComponent *Owner, LPCTSTR szName);
  virtual ~TForm_Principale(void);

	// Méthodes virtuelles héritées de TFVCLIDEAutomation

  virtual bool FASTCALL NewWorkSpace(void);
  virtual bool FASTCALL OpenWorkSpace(const AnsiString &asFileName);
  virtual bool FASTCALL SaveWorkSpace(void);
  virtual bool FASTCALL CloseWorkSpace(void);

	virtual bool FASTCALL AddExistingFile(const AnsiString &asNodeName, const AnsiString &asFileName);
	virtual bool FASTCALL AddExistingProject(const AnsiString &asNodeName, const AnsiString &asProjectFileName, const AnsiString & asProjectName, const AnsiString &asProjectType);

	virtual bool FASTCALL OpenFile(const AnsiString &asFileName);
	virtual bool FASTCALL OpenTextFile(const AnsiString &asFileName);
	virtual bool FASTCALL OpenBinFile(const AnsiString &asFileName);
  virtual bool FASTCALL OpenDialog(const AnsiString &asFileName);
  virtual bool FASTCALL OpenIcon(const AnsiString &asFileName);
  virtual bool FASTCALL OpenCursor(const AnsiString &asFileName);
  virtual bool FASTCALL SaveFile(int Index);

	virtual int FASTCALL GetNbFiles(void);
  virtual void FASTCALL GetListFiles(TStrings *ListFiles);
  virtual TFVCLIDEFileType FASTCALL GetFileType(int Index);
  virtual AnsiString FASTCALL GetFileName(int Index);
  virtual int FASTCALL GetFileIndex(const AnsiString asFileName, TFVCLIDEFileType FileType);
  virtual TTextEditor * FASTCALL GetTextFile(int Index);
  virtual THexaEditor * FASTCALL GetHexaFile(int Index);
  virtual TTextEditor * FASTCALL GetCurTextFile(void);
  virtual THexaEditor * FASTCALL GetCurHexaFile(void);
  virtual int FASTCALL GetCurFileIndex(void);
  virtual bool FASTCALL ActivateFile(int Index);
  virtual bool FASTCALL SetFocusOnEditor(void);

	virtual AnsiString FASTCALL GetSelText(void);
	virtual bool FASTCALL SetSelText(AnsiString asText);

	virtual bool FASTCALL AddBookmark(void);
	virtual bool FASTCALL DelBookmark(void);
	virtual bool FASTCALL AddBreakpoint(void);
	virtual bool FASTCALL DelBreakpoint(void);

	virtual void FASTCALL DisplayMessage(TTypeDisplay TypeDisplay, const AnsiString &asMessage);
	virtual int FASTCALL DisplayError(const AnsiString &asMessage, const AnsiString &asTitle, const AnsiString &asKey, int Flags);
	virtual int FASTCALL DisplaySystemError(const AnsiString &asMessage, int SystemError, const AnsiString &asTitle, const AnsiString &asKey, int Flags);

	virtual void FASTCALL ClearLogMessage(TTypeLog TypeLog);
	virtual void FASTCALL AddLogMessage(TTypeLog TypeLog, const AnsiString &asMessage);
	virtual void FASTCALL DeleteLogMessage(TTypeLog TypeLog, int i);
	virtual AnsiString FASTCALL GetLogMessage(TTypeLog TypeLog, int i);
	virtual int FASTCALL GetCountLogMessage(TTypeLog TypeLog);

	virtual bool FASTCALL AddVariableDefinition(AnsiString asVarName, int TypeDefinition, AnsiString asDescription, AnsiString asTypeContenant, AnsiString asTypeVariable, AnsiString asFileName, int NumLine, int NumCol);
	virtual bool FASTCALL FindDeclaration(AnsiString asVarName, AnsiString *asFileName, int *NumLine, int *NumCol);
	virtual bool FASTCALL FindImplementation(AnsiString asVarName, AnsiString *asFileName, int *NumLine, int *NumCol);
	virtual bool FASTCALL GetDescription(AnsiString asVarName, AnsiString *asDescription);
	virtual bool FASTCALL GetTypeVariable(AnsiString asVarName, AnsiString *asTypeVariable);
	virtual bool FASTCALL GetListAutocompletion(AnsiString asTypeContenant, TStrings *slList);

	virtual bool FASTCALL ReplaceDeclarationsFVCL(AnsiString asFileNameH, AnsiString asClassName, AnsiString asComponents, AnsiString asEvents);
	virtual bool FASTCALL SearchOrCreateCodeEvent(AnsiString asFileNameCpp, AnsiString asClassName, AnsiString asFunctionEvent, AnsiString asArgs);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Principale *Form_Principale;
//---------------------------------------------------------------------------
#endif
