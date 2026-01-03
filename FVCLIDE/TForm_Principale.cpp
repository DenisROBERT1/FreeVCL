//-----------------------------------------------------------------------------
//! @file TForm_Principale.cpp
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

#include <FreeVcl.h>
#include <FreeVcl_Web.h>
#include <FreeVcl_XFiles.h>
#include <Shlwapi.h>


#include "TFileASCII.h"
#include "TNumVersion.h"
#include "Utilit.h"

#include "TConfig.h"
#include "TTypeProject.h"
#include "TListTypeProjects.h"
#include "TProject.h"
#include "TForm_AddNewFile.h"
#include "TForm_AddNewProject.h"
#include "TForm_FileProperties.h"
#include "TForm_DisplayError.h"
#include "TForm_NewDialog.h"
#include "TForm_FileFormat.h"
#include "TForm_SearchAndReplace.h"
#include "TForm_RepeatMacro.h"
#include "TForm_Models.h"
#include "TForm_Statistics.h"
#include "TForm_EdiOptions.h"
#include "TForm_SaisieRacc.h"
#include "TForm_SaisieColors.h"
#include "TForm_CodeFormatParams.h"
#include "TForm_DefaultResponses.h"
#include "TForm_ConvertNumbers.h"
#include "TForm_Tools.h"
#include "TForm_Autocompletion.h"
#include "TForm_Principale.h"
#include "../../Common/Forms/TForm_APropos.h"

#include "TElemVcl.h"
#include "TFrame_EditFichierText.h"
#include "TFrame_EditFichierHexa.h"
#include "TFrame_SaisieDialog.h"
#include "TFrame_SaisieIcon.h"
#include "TFrame_SaisieCursor.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

TForm_Principale * Form_Principale;


//---------------------------------------------------------------------------
TForm_Principale::TForm_Principale(TComponent *Owner, LPCTSTR szName)
		: TForm(Owner, szName, FVCL_ARG_SUPP) {
	TIcon *Icon;
	bool bUpdatesOk;
	int i;


	// Provoquer le lien avec les libs
	Factory_FVCL_Web.Init();
	Factory_FVCL_ToolsEDI.Init();
	Factory_FVCL_XFiles.Init();

	// Configuration (doit rester au début)
	Config = new TConfig();

	Icon = new TIcon();
	Icon->Width = 16;
	Icon->Height = 16;
	Icon->LoadFromResourceName(HInstance, _T("AAAA_MAINICON"));
	Application->Icon = Icon;
	delete Icon;

	// Image type projet = fichier texte
	TBitmap *Bitmap = new TBitmap();
	Bitmap->Width = 20;
	Bitmap->Height = 20;
	Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILE_20"));
	ImageList_TypeProjects->Add(Bitmap, NULL);
	Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILES_20"));
	ImageList_TypeProjects->Add(Bitmap, NULL);
	Bitmap->LoadFromResource(HInstance, _T("BMP_DIRECTORY_20"));
	ImageList_TypeProjects->Add(Bitmap, NULL);
	delete Bitmap;

	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	SET_EVENT(Application, TOnActivate, OnActivate, TForm_Principale, this, ApplicationActivate);
	SET_EVENT(Application, TOnMinimize, OnMinimize, TForm_Principale, this, ApplicationMinimize);
	SET_EVENT(Application, TOnRestore, OnRestore, TForm_Principale, this, ApplicationRestore);
	bFromAnotherApp = false;
	bDebugging = false;
	bBuilding = false;
	CurrentProject = NULL;
	DebugProject = NULL;
	ListTypeProjects = new TListTypeProjects();
	bCodeOnly = false;
	bMemoCodeOnly = false;
	Form_Autocompletion = NULL;

	PageControl_Files->MultiLine = Config->MultiLineTabs;
	NbTabSheet = 0;
	Form_SearchAndReplace = NULL;
	memset(&SearchParam, 0, sizeof(SEARCHPARAM));
	SearchParam.Down = true;
	bTempWorkSpace = false;
	bRecMacro = false;
	ReadConfig();

	EnableWindow(FHandle, FALSE);  // La propriété Enabled ne marche pas car on est dans le constructeur
	bUpdatesOk = CheckUpdatesStart();
	EnableWindow(FHandle, TRUE);

	if (bUpdatesOk) {

		// Fichiers passés en ligne de commande
		AnsiString asFileName;
		for (i = 1; i < ParamCount(); i++) {
			asFileName = ParamStr(i);
			if (ExtractFileExt(asFileName).LowerCase() == _T(".fvcl")) {
				OpenWorkSpace(asFileName);
			}
			else {
				OpenFile(asFileName);
			}
		}
		Visible = true;
	}

}

//---------------------------------------------------------------------------
TForm_Principale::~TForm_Principale(void) {

	delete Form_SearchAndReplace;
  delete Form_Autocompletion;
	delete ListTypeProjects;
	delete Config;

}

//---------------------------------------------------------------------------
//
//													TRAITEMENT DES EVENEMENTS
//
//---------------------------------------------------------------------------

void FASTCALL TForm_Principale::ApplicationActivate(TObject *Sender) {
	bFromAnotherApp = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ApplicationMinimize(TObject *Sender) {
	if (Form_SearchAndReplace != NULL) {
		Form_SearchAndReplace->WindowState = wsMinimized;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ApplicationRestore(TObject *Sender) {
	if (Form_SearchAndReplace != NULL) {
		Form_SearchAndReplace->WindowState = wsNormal;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::FormActivate(TObject *Sender) {

	// Le test de la date des fichiers se fait au moment du FormActivate,
	// et non sur l'évènement ApplicationActivate, car si l'application est
	// en icône, les MessageBox éventuels empêchent la restauration.

	Timer_Activate->Enabled = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Timer_ActivateTimer(TObject *Sender) {
	TFrame_FVCLIDEBase *Frame_FVCLIDEBase;
	int ExternalChange;
	int i;


	// Le test de la date des fichiers se fait avec un timer,
	// sinon ça perturbe le FormActivate

	Timer_Activate->Enabled = false;

	if (bFromAnotherApp) {
		bFromAnotherApp = false;
		for (i = (int) TabSheets.size() - 1; i >= 0; i--) {
			Frame_FVCLIDEBase = TabSheets[i].Content;
			ExternalChange = Frame_FVCLIDEBase->TestExternalChange();
			if (ExternalChange != 0) {
				int Rep;
				AnsiString asMsg;
				AnsiString asFileName;
				asFileName = Frame_FVCLIDEBase->FileName;

				if (ExternalChange == 1) {
					asMsg = _TT("Un programme externe a modifié ce fichier:\n");
					asMsg += asFileName + _T("\n");
					asMsg += _TT("Voulez-vous le recharger ?\n");
					Rep = DisplayError(asMsg,
														 _TT("Fichier modifié"),
														 _T("RELOAD_FILE"),
														 MB_YESNO | MB_ICONQUESTION);
					if (Rep == IDYES) {
						if (Frame_FVCLIDEBase->OpenFile(asFileName)) {
							Frame_FVCLIDEBase->MemoDateFile();
							Frame_FVCLIDEBase->Changed = false;
						}
						else {
							Frame_FVCLIDEBase->Changed = true;
						}
					}
					else {
						Frame_FVCLIDEBase->MemoDateFile();
						Frame_FVCLIDEBase->Changed = true;
					}
				}
				else if (ExternalChange == 2) {
					asMsg = _TT("Un programme externe a supprimé ce fichier:\n");
					asMsg += asFileName + _T("\n");
					asMsg += _TT("Voulez-vous le garder en mémoire ?\n");
					Rep = DisplayError(asMsg,
														 _TT("Fichier supprimé"),
														 _T("RELOAD_FILE"),
														 MB_YESNO | MB_ICONQUESTION);
					if (Rep == IDYES) {
						Frame_FVCLIDEBase->MemoDateFile();
						Frame_FVCLIDEBase->Changed = true;
					}
					else {
						delete (TTabSheet *) (PageControl_Files->Pages[i]);
						TabSheets.erase(TabSheets.begin() + i);
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_NewWorkSpaceExecute(TObject *Sender) {

	if (!QuestDebugging()) return;
	if (!QuestEnrModifs()) return;

	SaveWorkSpaceIfNotEmpty();
	CloseWorkSpace();
	NewWorkSpace();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenWorkSpaceExecute(TObject *Sender) {

	OpenDialog_Project->FileName = _T("");
	OpenDialog_Project->Title = _TT("Nom du workspace à ouvrir");
	if (OpenDialog_Project->Execute()) {
		if (!QuestDebugging()) return;
		if (!QuestEnrModifs()) return;
		SaveWorkSpaceIfNotEmpty();
		CloseWorkSpace();
		OpenWorkSpace(OpenDialog_Project->FileName);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveWorkSpaceExecute(TObject *Sender) {

	if (TreeView_Projects->Items->Count == 0) {
		DisplayError(_TT("Impossible d'enregistrer un espace de travail vide"),
								 _TT("Sauvegarde"),
								 _T("SAVE_EMPTY"),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	if (asFileWorkSpace.IsEmpty()) Action_SaveAsWorkSpaceExecute(Sender);
	else SaveWorkSpace();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveAsWorkSpaceExecute(TObject *Sender) {

	if (TreeView_Projects->Items->Count == 0) {
		DisplayError(_TT("Impossible d'enregistrer un espace de travail vide"),
								 _TT("Sauvegarde"),
								 _T("SAVE_EMPTY"),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	if (SaveDialog_WorkSpace->Execute()) {
		asFileWorkSpace = SaveDialog_WorkSpace->FileName;
		Config->PushLastWorkSpace(asFileWorkSpace);
		SaveWorkSpace();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CloseWorkSpaceExecute(TObject *Sender) {

	if (!QuestDebugging()) return;
	if (!QuestEnrModifs()) return;

	SaveWorkSpaceIfNotEmpty();
	CloseWorkSpace();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::MenuItem_LastWorkSpaceClick(TObject *Sender) {
	TMenuItem *MenuItem;


	MenuItem = dynamic_cast<TMenuItem *>(Sender);
	if (MenuItem) {
		if (!QuestDebugging()) return;
		if (!QuestEnrModifs()) return;
		SaveWorkSpaceIfNotEmpty();
		CloseWorkSpace();
		if (!OpenWorkSpace(MenuItem->Caption)) {
			AnsiString asMsg;
			asMsg = _TT("Impossible d'ouvrir le fichier:\n");
			asMsg += MenuItem->Caption;
			DisplayError(asMsg,
									 _TT("Fichier inexistant"),
									 _T(""),
									 MB_OK | MB_ICONWARNING);
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddNewFileOnCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Ajout d'un fichier"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	TreeNode = NULL;
	for (i = 0; i < TreeView_Projects->Items->Count; i++) {
		if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
			TreeNode = TreeView_Projects->Items->Item[i];
			break;
		}
	}

	if (TreeNode) AddNewFile(TreeNode);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddExistingFileOnCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Ajout d'un fichier"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	TreeNode = NULL;
	for (i = 0; i < TreeView_Projects->Items->Count; i++) {
		if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
			TreeNode = TreeView_Projects->Items->Item[i];
			break;
		}
	}

	if (TreeNode) AddExistingFile(TreeNode);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddNewProjectOnCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int i;


	// Si aucun workspace ni aucun projet : on crée un workspace vide
	if (CurrentProject == NULL && TreeView_Projects->Items->Count <= 1) {
		NewWorkSpace();
		CurrentProject = (TProject *) TreeView_Projects->Items->FirstNode->GetFirstChild()->Data;
	}

	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Ajout d'un projet"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	TreeNode = NULL;
	for (i = 0; i < TreeView_Projects->Items->Count; i++) {
		if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
			TreeNode = TreeView_Projects->Items->Item[i];
			break;
		}
	}

	if (TreeNode) AddNewProject(TreeNode);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddExistingProjectOnCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Ajout d'un projet"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	TreeNode = NULL;
	for (i = 0; i < TreeView_Projects->Items->Count; i++) {
		if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
			TreeNode = TreeView_Projects->Items->Item[i];
			break;
		}
	}

	if (TreeNode) AddExistingProject(TreeNode);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CloseProjectExecute(TObject *Sender) {
	TProject *ProjectParent;
	TProject *ProjectChild;
	TTreeNode *NodeParent;
	TTreeNode *NodeChild;
	TDateTime dtLastChange;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Fermeture d'un projet"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	NodeChild = NULL;
	for (i = 0; i < TreeView_Projects->Items->Count; i++) {
		if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
			NodeChild = TreeView_Projects->Items->Item[i];
			break;
		}
	}

	if (NodeChild) {
		ProjectChild = CurrentProject;
		NodeParent = NodeChild->Parent;
		ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);

		// Suppression du TreeView
		TreeViewDeleteNode(NodeChild);

		// Dernière modification du projet
		if (ProjectParent) {
			dtLastChange.CurrentDateTime(false);
			ProjectParent->LastChange = dtLastChange;
		}

		// Il n'y a plus de projet courant
		CurrentProject = NULL;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ExpandNodeExecute(TObject *Sender) {
	TTreeNode *TreeNode;


	TreeNode = TreeView_Projects->Selected;
	if (TreeNode) TreeNode->Expand(true);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CollapseNodeExecute(TObject *Sender) {
	TTreeNode *TreeNode;


	TreeNode = TreeView_Projects->Selected;
	if (TreeNode) TreeNode->Collapse(true);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddNewFileOnSelectedExecute(TObject *Sender) {
	TTreeNode *NodeParent;


	NodeParent = TreeView_Projects->Selected;
	if (NodeParent->ImageIndex < DECAL_TYPES_PROJECT) return;

	AddNewFile(NodeParent);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddExistingFileOnSelectedExecute(TObject *Sender) {
	TTreeNode *NodeParent;


	NodeParent = TreeView_Projects->Selected;
	if (NodeParent->ImageIndex < DECAL_TYPES_PROJECT) return;

	AddExistingFile(NodeParent);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddNewProjectOnSelectedExecute(TObject *Sender) {
	TTreeNode *NodeParent;


	NodeParent = TreeView_Projects->Selected;
	if (NodeParent->ImageIndex < DECAL_TYPES_PROJECT) return;

	AddNewProject(NodeParent);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddExistingProjectOnSelectedExecute(TObject *Sender) {
	TTreeNode *NodeParent;


	NodeParent = TreeView_Projects->Selected;
	if (NodeParent->ImageIndex < DECAL_TYPES_PROJECT) return;

	AddExistingProject(NodeParent);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RenameProjectExecute(TObject *Sender) {
	TreeView_Projects->Selected->EditText();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RemoveProjectExecute(TObject *Sender) {
	TProject *ProjectParent;
	TProject *ProjectChild;
	TTreeNode *NodeParent;
	TTreeNode *NodeChild;
	TDateTime dtLastChange;
	int IndexTypeProject;


	NodeChild = TreeView_Projects->Selected;
	ProjectChild = reinterpret_cast<TProject *>((void *) NodeChild->Data);
	NodeParent = NodeChild->Parent;
	ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);

	IndexTypeProject = NodeChild->ImageIndex - DECAL_TYPES_PROJECT;
	if (IndexTypeProject < 0) {
		if (ProjectChild && ProjectParent && ProjectParent->Can_RemoveFile()) {
			ProjectParent->RemoveFile(ProjectChild->FileName);
		}

	}

	// Suppression du TreeView
	TreeViewDeleteNode(NodeChild);

	// Dernière modification du projet
	if (ProjectParent) {
		dtLastChange.CurrentDateTime(false);
		ProjectParent->LastChange = dtLastChange;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_EditProjectExecute(TObject *Sender) {
	TTreeNode *TreeNodeSelected;
	TProject *ProjectSelected;
	int IndexTypeProject;


	TreeNodeSelected = TreeView_Projects->Selected;
	if (TreeNodeSelected) {
		ProjectSelected = reinterpret_cast<TProject *>((void *) TreeNodeSelected->Data);
		if (ProjectSelected) {
			IndexTypeProject = TreeNodeSelected->ImageIndex - DECAL_TYPES_PROJECT;
			if (IndexTypeProject >= 0) {
				if (ProjectSelected->Open()) {

					// Mise à jour des dépendances
					RemoveDependencies(TreeNodeSelected);
					InsertDependencies(TreeNodeSelected);

				}
			}
			else {
				AnsiString asProjectFileName = ProjectSelected->FileName;
				if (!asProjectFileName.IsEmpty()) OpenFile(asProjectFileName);
			}

			UpdateTreeViewStates();

		}
		else {
			AnsiString asNodeName, asMessage;
			asNodeName = TreeNodeSelected->Text;
			asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
			DisplayError(asMessage,
									 _TT("Opération impossible"),
									 _T(""),
									 MB_OK | MB_ICONWARNING);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_EditFileExecute(TObject *Sender) {
	TTreeNode *TreeNodeSelected;
	TProject *ProjectSelected;
	AnsiString asFileName;


	TreeNodeSelected = TreeView_Projects->Selected;
	if (TreeNodeSelected) {
		ProjectSelected = reinterpret_cast<TProject *>((void *) TreeNodeSelected->Data);
		if (ProjectSelected) {
			asFileName = ProjectSelected->FileName;
			if (!asFileName.IsEmpty()) {
				if (ExtractFileExt(asFileName).LowerCase() == _T(".dfm")) {
					// OpenTextFile et non OpenFile pour pouvoir ouvrir les DFM en tant que texte
					// et non avec le constructeur de fiche.
					OpenTextFile(asFileName);
				}
				else {
					// Sinon on ouvre de la façon la plus lisible (hexa ou texte)
					OpenFile(asFileName);
				}
			}
		}
		else {
			AnsiString asNodeName, asMessage;
			asNodeName = TreeNodeSelected->Text;
			asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
			DisplayError(asMessage,
									 _TT("Opération impossible"),
									 _T(""),
									 MB_OK | MB_ICONWARNING);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenProjectFileExecute(TObject *Sender) {
	TTreeNode *TreeNodeSelected;
	TProject *ProjectSelected;
	AnsiString asProjectFile;


	TreeNodeSelected = TreeView_Projects->Selected;
	if (TreeNodeSelected) {
		ProjectSelected = reinterpret_cast<TProject *>((void *) TreeNodeSelected->Data);
		if (ProjectSelected) {
			asProjectFile = ProjectSelected->FileName;
			if (!asProjectFile.IsEmpty()) {
				Screen->Cursor = crHourGlass;
				ShellExecute(FHandle, _T("open"), asProjectFile, _T(""),
										 ExtractFilePath(asProjectFile), SW_SHOWNORMAL);
				Screen->Cursor = crDefault;
			}
		}
		else {
			AnsiString asNodeName, asMessage;
			asNodeName = TreeNodeSelected->Text;
			asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
			DisplayError(asMessage,
									 _TT("Opération impossible"),
									 _T(""),
									 MB_OK | MB_ICONWARNING);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::MenuItem_LastProjectClick(TObject *Sender) {
	AnsiString asFileName;
	TMenuItem *MenuItem;


	MenuItem = dynamic_cast<TMenuItem *>(Sender);
	if (MenuItem) {
		asFileName = MenuItem->Caption;
		// $$$ OpenProject(asFileName);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_NewTextFileExecute(TObject *Sender) {
	TABSHEET NewTabSheet;
	TFrame_EditFichierText *Frame_EditFichierText;
	AnsiString asName;


	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->Caption = _TT("Nouveau fichier");
	NewTabSheet.TabSheet->PageControl = PageControl_Files;

	Frame_EditFichierText = new TFrame_EditFichierText(NewTabSheet.TabSheet);
	Frame_EditFichierText->Name = _T("Frame_EditFichierText");
	Frame_EditFichierText->Parent = NewTabSheet.TabSheet;
	Frame_EditFichierText->Align = alClient;
	SET_EVENT(Frame_EditFichierText, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);
	SET_EVENT(Frame_EditFichierText, TNotifyEvent, OnPassBegin, TForm_Principale, this, ObjectPassBegin);
	SET_EVENT(Frame_EditFichierText, TNotifyEvent, OnPassEnd, TForm_Principale, this, ObjectPassEnd);
	SET_EVENT(Frame_EditFichierText, TOnAutocompletionOn, OnAutocompletionOn, TForm_Principale, this, ObjectAutocompletionOn);
	SET_EVENT(Frame_EditFichierText, TOnAutocompletionOff, OnAutocompletionOff, TForm_Principale, this, ObjectAutocompletionOff);
	SET_EVENT(Frame_EditFichierText->TextEditor->Tooltip, TOnHint, OnHint, TForm_Principale, this, TextEditorHint);
	NewTabSheet.Content = Frame_EditFichierText;
	TabSheets.push_back(NewTabSheet);

	PageControl_Files->ActivePage = NewTabSheet.TabSheet;
	Frame_EditFichierText->SetFocusChild();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_NewBinFileExecute(TObject *Sender) {
	TABSHEET NewTabSheet;
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	AnsiString asName;


	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->Caption = _TT("Nouveau fichier");
	NewTabSheet.TabSheet->PageControl = PageControl_Files;

	Frame_EditFichierHexa = new TFrame_EditFichierHexa(NewTabSheet.TabSheet);
	Frame_EditFichierHexa->Name = _T("Frame_EditFichierHexa");
	Frame_EditFichierHexa->Parent = NewTabSheet.TabSheet;
	Frame_EditFichierHexa->Align = alClient;
	SET_EVENT(Frame_EditFichierHexa, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);
	SET_EVENT(Frame_EditFichierHexa, TNotifyEvent, OnPassBegin, TForm_Principale, this, ObjectPassBegin);
	SET_EVENT(Frame_EditFichierHexa, TNotifyEvent, OnPassEnd, TForm_Principale, this, ObjectPassEnd);
	NewTabSheet.Content = Frame_EditFichierHexa;
	TabSheets.push_back(NewTabSheet);

	PageControl_Files->ActivePage = NewTabSheet.TabSheet;
	Frame_EditFichierHexa->SetFocusChild();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenTextFileExecute(TObject *Sender) {
	int i;


	OpenDialog_File->FilterIndex = 1;
	if (OpenDialog_File->Execute()) {
		for (i = 0; i < OpenDialog_File->Files->Count; i++) {
			OpenTextFile(OpenDialog_File->Files->Strings[i]);
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenBinFileExecute(TObject *Sender) {
	int i;


	OpenDialog_File->FilterIndex = 3;
	if (OpenDialog_File->Execute()) {
		for (i = 0; i < OpenDialog_File->Files->Count; i++) {
			OpenBinFile(OpenDialog_File->Files->Strings[i]);
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveFileExecute(TObject *Sender) {
	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) SaveFile(Index);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveAllFilesExecute(TObject *Sender) {
	AnsiString asFileName;
	int Index;


	for (Index = 0; Index < PageControl_Files->PageCount; Index++) {
		SaveFile(Index);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveAsFileExecute(TObject *Sender) {

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_FVCLIDEBase *Frame_FVCLIDEBase = TabSheets[Index].Content;
		SaveDialog_File->FileName = Frame_FVCLIDEBase->FileName;
		if (SaveDialog_File->Execute()) {
			if (Frame_FVCLIDEBase->SaveToFile(SaveDialog_File->FileName)) {
				TabSheets[Index].TabSheet->Caption = Frame_FVCLIDEBase->DisplayedFileName;
				Frame_FVCLIDEBase->MemoDateFile();
				Frame_FVCLIDEBase->Changed = false;
			}
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CloseExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		if (QuestEnrModifsFile(Index)) {
			delete (TTabSheet *) (PageControl_Files->Pages[Index]);
			TabSheets.erase(TabSheets.begin() + Index);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CloseAllExecute(TObject *Sender) {
	int i;

	if (QuestEnrModifsFiles()) {
		for (i = (int) TabSheets.size() - 1; i >= 0; i--) {
			delete (TTabSheet *) (PageControl_Files->Pages[i]);
			TabSheets.erase(TabSheets.begin() + i);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CloseOthersExecute(TObject *Sender) {
	int i;
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		for (i = (int) TabSheets.size() - 1; i >= 0; i--) {
			if (i != Index) {
				if (QuestEnrModifsFile(i)) {
					delete (TTabSheet *) (PageControl_Files->Pages[i]);
					TabSheets.erase(TabSheets.begin() + i);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CopyPathExecute(TObject *Sender) {

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		Clipboard->Open();
		Clipboard->Clear();
		Clipboard->SetTextBuf(((AnsiString) TabSheets[Index].Content->FileName));
		Clipboard->Close();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenFolderExecute(TObject *Sender) {

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		AnsiString asPath = ExtractFilePath(TabSheets[Index].Content->FileName);
		ShellExecute(FHandle, _T("open"), asPath, NULL, NULL, SW_SHOWNORMAL);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenFichierAssocie1Execute(TObject *Sender) {

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			AnsiString asFileName = TabSheets[Index].Content->FileName;
			asFileName = ChangeFileExt(asFileName, Frame_EditFichierText->TextEditor->Associated1);
			OpenTextFile(asFileName);
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenFichierAssocie2Execute(TObject *Sender) {

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			AnsiString asFileName = TabSheets[Index].Content->FileName;
			asFileName = ChangeFileExt(asFileName, Frame_EditFichierText->TextEditor->Associated2);
			OpenTextFile(asFileName);
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::MenuItem_LastFileClick(TObject *Sender) {
	AnsiString asFileName;
	TMenuItem *MenuItem;


	MenuItem = dynamic_cast<TMenuItem *>(Sender);
	if (MenuItem) {
		asFileName = MenuItem->Caption;
		OpenFile(asFileName);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_NewDialogExecute(TObject *Sender) {
	TForm_NewDialog *Form_NewDialog;


	Form_NewDialog = new TForm_NewDialog(this, _T("TForm_NewDialog"));
	if (Form_NewDialog->ShowModal() == mrOk) {
		TABSHEET NewTabSheet;
		TFrame_SaisieDialog *Frame_SaisieDialog;
		AnsiString asName;

		NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
		asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
		NewTabSheet.TabSheet->Name = asName;
		NewTabSheet.TabSheet->Caption = _TT("Nouveau fichier");
		NewTabSheet.TabSheet->PageControl = PageControl_Files;

		Frame_SaisieDialog = new TFrame_SaisieDialog(NewTabSheet.TabSheet);
		Frame_SaisieDialog->Name = _T("Frame_SaisieDialog");
		Frame_SaisieDialog->Parent = NewTabSheet.TabSheet;
		Frame_SaisieDialog->Align = alClient;

		// Content et push_back tout de suite sinon crash lorsqu'on survole l'onglet
		NewTabSheet.Content = Frame_SaisieDialog;
		TabSheets.push_back(NewTabSheet);
		SET_EVENT(Frame_SaisieDialog, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);

		if (Frame_SaisieDialog->CreateNewDialog(Form_NewDialog->Description,
																						Form_NewDialog->HandleNewWindow)) {

			PageControl_Files->ActivePage = NewTabSheet.TabSheet;

			Frame_SaisieDialog->SetFocusChild();
		}
		else {
			TabSheets.pop_back();
			delete NewTabSheet.TabSheet;
		}

	}
	delete Form_NewDialog;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenDialogExecute(TObject *Sender) {

	if (OpenDialog_Dialog->Execute()) OpenDialog(OpenDialog_Dialog->FileName);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveDialogExecute(TObject *Sender) {
	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) SaveFile(Index);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveAsDialogExecute(TObject *Sender) {

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_FVCLIDEBase *Frame_FVCLIDEBase = TabSheets[Index].Content;
		SaveDialog_Dialog->FileName = Frame_FVCLIDEBase->FileName;
		if (SaveDialog_Dialog->Execute()) {
			if (Frame_FVCLIDEBase->SaveToFile(SaveDialog_Dialog->FileName)) {
				TabSheets[Index].TabSheet->Caption = Frame_FVCLIDEBase->DisplayedFileName;
				Frame_FVCLIDEBase->Changed = false;
			}
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::MenuItem_LastDialogClick(TObject *Sender) {
	AnsiString asFileName;
	TMenuItem *MenuItem;


	MenuItem = dynamic_cast<TMenuItem *>(Sender);
	if (MenuItem) {
		asFileName = MenuItem->Caption;
		OpenDialog(asFileName);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_NewBmpExecute(TObject *Sender) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_NewIconExecute(TObject *Sender) {
	TABSHEET NewTabSheet;
	TFrame_SaisieIcon *Frame_SaisieIcon;
	AnsiString asName;

	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->Caption = _TT("Nouveau fichier");
	NewTabSheet.TabSheet->PageControl = PageControl_Files;

	Frame_SaisieIcon = new TFrame_SaisieIcon(NewTabSheet.TabSheet);
	Frame_SaisieIcon->Name = _T("Frame_SaisieIcon");
	Frame_SaisieIcon->Parent = NewTabSheet.TabSheet;
	Frame_SaisieIcon->Align = alClient;

	if (Frame_SaisieIcon->CreateNewIcon()) {
		SET_EVENT(Frame_SaisieIcon, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);
		NewTabSheet.Content = Frame_SaisieIcon;
		TabSheets.push_back(NewTabSheet);

		PageControl_Files->ActivePage = NewTabSheet.TabSheet;

		Frame_SaisieIcon->SetFocusChild();
	}
	else {
		delete NewTabSheet.TabSheet;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_NewCursorExecute(TObject *Sender) {
	TABSHEET NewTabSheet;
	TFrame_SaisieIcon *Frame_SaisieIcon;
	AnsiString asName;

	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->Caption = _TT("Nouveau fichier");
	NewTabSheet.TabSheet->PageControl = PageControl_Files;

	Frame_SaisieIcon = new TFrame_SaisieIcon(NewTabSheet.TabSheet);
	Frame_SaisieIcon->Name = _T("Frame_SaisieIcon");
	Frame_SaisieIcon->Parent = NewTabSheet.TabSheet;
	Frame_SaisieIcon->Align = alClient;

	if (Frame_SaisieIcon->CreateNewIcon()) {
		SET_EVENT(Frame_SaisieIcon, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);
		NewTabSheet.Content = Frame_SaisieIcon;
		TabSheets.push_back(NewTabSheet);

		PageControl_Files->ActivePage = NewTabSheet.TabSheet;

		Frame_SaisieIcon->SetFocusChild();
	}
	else {
		delete NewTabSheet.TabSheet;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_OpenGraphicExecute(TObject *Sender) {

	if (OpenDialog_Graphic->Execute()) OpenFile(OpenDialog_Graphic->FileName);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveGraphicExecute(TObject *Sender) {
	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) SaveFile(Index);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SaveAsGraphicExecute(TObject *Sender) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::MenuItem_LastGraphicClick(TObject *Sender) {
	AnsiString asFileName;
	TMenuItem *MenuItem;


	MenuItem = dynamic_cast<TMenuItem *>(Sender);
	if (MenuItem) {
		asFileName = MenuItem->Caption;
		OpenFile(asFileName);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_PrintExecute(TObject *Sender) {
	TPrintDialog *PrintDialog;
	TPrinter *APrinter;
	int Count;
	HDC hdc;
	int i, y;


	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			PrintDialog = new TPrintDialog(this);
			if (PrintDialog->Execute()) {
				APrinter = Printer();
				APrinter->BeginDoc();
				hdc = APrinter->Handle;
				Count = Frame_EditFichierText->TextEditor->Lines->Count;
				y = 0;
				for (i = 0; i < Count; i++) {
					TextOut(hdc, 0, y,
									Frame_EditFichierText->TextEditor->Lines->Strings[i],
									Frame_EditFichierText->TextEditor->Lines->Strings[i].Length());
					y += 120;  // $$$ à finaliser (font, espacement, ...)
				}
				APrinter->EndDoc();
			}
			delete PrintDialog;
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_QuitExecute(TObject *Sender) {
	Close();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_UndoExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TabSheets[Index].Content->Undo();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RedoExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TabSheets[Index].Content->Redo();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CutToClipboardExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TabSheets[Index].Content->CutToClipboard();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CopyToClipboardExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TabSheets[Index].Content->CopyToClipboard();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_PasteFromClipboardExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TabSheets[Index].Content->PasteFromClipboard();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RefPreviousExecute(TObject *Sender) {
	TListBox *ListBox_Log = NULL;


	switch (PageControl_Logs->ActivePageIndex) {
		case tl_Search: ListBox_Log = ListBox_Search; break;
		case tl_Bookmarks: ListBox_Log = ListBox_Bookmarks; break;
		case tl_Build: ListBox_Log = ListBox_Build; break;
		case tl_Process: ListBox_Log = ListBox_Process; break;
		case tl_Breakpoints: ListBox_Log = ListBox_Breakpoints; break;
		case tl_CallStack: ListBox_Log = ListBox_CallStack; break;
	}

	int ItemIndex = ListBox_Log->ItemIndex;
	if (ItemIndex > 0) {
		ListBox_Log->ItemIndex = ItemIndex - 1;

		switch (PageControl_Logs->ActivePageIndex) {
			case tl_Search: ListBox_SearchDblClick(this); break;
			case tl_Bookmarks: ListBox_BookmarksDblClick(this); break;
			case tl_Build: ListBox_BuildDblClick(this); break;
			case tl_Process: ListBox_ProcessDblClick(this); break;
			case tl_Breakpoints: ListBox_BreakpointsDblClick(this); break;
			case tl_CallStack: ListBox_CallStackDblClick(this); break;
		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RefNextExecute(TObject *Sender) {
	TListBox *ListBox_Log = NULL;


	switch (PageControl_Logs->ActivePageIndex) {
		case tl_Search: ListBox_Log = ListBox_Search; break;
		case tl_Bookmarks: ListBox_Log = ListBox_Bookmarks; break;
		case tl_Build: ListBox_Log = ListBox_Build; break;
		case tl_Process: ListBox_Log = ListBox_Process; break;
		case tl_Breakpoints: ListBox_Log = ListBox_Breakpoints; break;
		case tl_CallStack: ListBox_Log = ListBox_CallStack; break;
	}

	if (ListBox_Log) {
		int ItemIndex = ListBox_Log->ItemIndex;
		if (ItemIndex < 0) ItemIndex = 0;
		ListBox_Log->ItemIndex = ItemIndex + 1;

		switch (PageControl_Logs->ActivePageIndex) {
			case tl_Search: ListBox_SearchDblClick(this); break;
			case tl_Bookmarks: ListBox_BookmarksDblClick(this); break;
			case tl_Build: ListBox_BuildDblClick(this); break;
			case tl_Process: ListBox_ProcessDblClick(this); break;
			case tl_Breakpoints: ListBox_BreakpointsDblClick(this); break;
			case tl_CallStack: ListBox_CallStackDblClick(this); break;
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SelectAllExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			Frame_EditFichierText->TextEditor->SelectAll();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_GotoExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			AnsiString asInput;
			int OldNumLine, NewNumLine;

			OldNumLine = Frame_EditFichierText->TextEditor->NumLine + 1;
			asInput = InputBox(_TT("Aller à la ligne"),
												 _TT("Entrer le numéro de ligne :"),
												 AnsiString(OldNumLine),
												 100);
			NewNumLine = asInput.ToIntDef(OldNumLine);
			if (NewNumLine != OldNumLine) {
				Frame_EditFichierText->TextEditor->NumLine = NewNumLine - 1;
				Frame_EditFichierText->TextEditor->NumCol = 0;
			}
		}

		TFrame_EditFichierHexa *Frame_EditFichierHexa;
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
		if (Frame_EditFichierHexa) {
			AnsiString asInput;
			__int64 OldFilePos, NewFilePos;
			TCHAR szBuf[2 * sizeof(OldFilePos) + 1];

			OldFilePos = Frame_EditFichierHexa->HexaEditor->FilePos;
			IntToHex(&OldFilePos, szBuf, sizeof(OldFilePos));
			asInput = InputBox(_T("Aller à la position"),
												 _T("Entrer la position (hexa) :"),
												 szBuf,
												 100);
			HexToInt(asInput, &NewFilePos, sizeof(NewFilePos));
			if (NewFilePos != OldFilePos) {
				Frame_EditFichierHexa->HexaEditor->FilePos = NewFilePos;
			}
		}

	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_LowerCaseExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			AnsiString asSelection = Frame_EditFichierText->TextEditor->GetSelText();
			Frame_EditFichierText->TextEditor->Replace(asSelection.LowerCase(), false);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_UpperCaseExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			AnsiString asSelection = Frame_EditFichierText->TextEditor->GetSelText();
			Frame_EditFichierText->TextEditor->Replace(asSelection.UpperCase(), false);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CommentExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			Frame_EditFichierText->TextEditor->CommentSelection();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_UncommentExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			Frame_EditFichierText->TextEditor->UncommentSelection();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddBookmarkExecute(TObject *Sender) {
	AddBookmark();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_DelBookmarkExecute(TObject *Sender) {
	DelBookmark();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ClearBookmarksExecute(TObject *Sender) {
	int i;


	for (i = 0; i < (int) TabSheets.size(); i++) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) {
			Frame_EditFichierText->TextEditor->ClearFlag(lfBookmark);
		}

		TFrame_EditFichierHexa *Frame_EditFichierHexa;
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
		if (Frame_EditFichierHexa) {
			Frame_EditFichierHexa->HexaEditor->ClearFlag(lfBookmark);
		}

	}

	ClearLogMessage(tl_Bookmarks);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FileFormatExecute(TObject *Sender) {
	TForm_FileFormat *Form_FileFormat;
	TTextEditor *TextEditor;
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			Form_FileFormat = new TForm_FileFormat(this, _T("TForm_FileFormat"));
			TextEditor = Frame_EditFichierText->TextEditor;
			if (TextEditor->ByteOrderMark) Form_FileFormat->CheckBox_ByteOrderMark->Checked = true;
			if (TextEditor->BigEndian) Form_FileFormat->CheckBox_BigEndian->Checked = true;
			if (TextEditor->Encodage == ceUNICODE) Form_FileFormat->RadioButton_UNICODE->Checked = true;
			else if (TextEditor->Encodage == ceUtf8) Form_FileFormat->RadioButton_UTF8->Checked = true;
			else Form_FileFormat->RadioButton_ASCII->Checked = true;
			if (TextEditor->LinuxEndLine) Form_FileFormat->RadioButton_LF->Checked = true;
			else Form_FileFormat->RadioButton_CRLF->Checked = true;
			if (Form_FileFormat->ShowModal() == mrOk) {
				TextEditor->ByteOrderMark = Form_FileFormat->CheckBox_ByteOrderMark->Checked;
				TextEditor->BigEndian = Form_FileFormat->CheckBox_BigEndian->Checked;
				if (Form_FileFormat->RadioButton_UNICODE->Checked) TextEditor->Encodage = ceUNICODE;
				else if (Form_FileFormat->RadioButton_UTF8->Checked) TextEditor->Encodage = ceUtf8;
				else TextEditor->Encodage = ceASCII;
				TextEditor->LinuxEndLine = Form_FileFormat->RadioButton_LF->Checked;
				Frame_EditFichierText->Changed = true;
			}
			delete Form_FileFormat;
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_TabsToSpacesExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			TTextEditor * TextEditor = Frame_EditFichierText->TextEditor;
			TextEditor->BeginOperation(_TT("Transformation des tabulations en espaces"));
			TextEditor->BeginUpdate();
			int Count = TextEditor->Lines->Count;
			int NbSpacesTab = Config->NbSpacesTab;
			AnsiString asLine;
			AnsiString asLine2;
			int Len;
			int i, j;
			TCHAR c;
			for (i = 0; i < Count; i++) {
				asLine = TextEditor->Lines->Strings[i];
				asLine2 = _T("");
				Len = asLine.Length();
				for (j = 0; j < Len; j++) {
					c = asLine[j + 1];
					if (c == _T('\t')) {
						do {
							asLine2 += _T(" ");
						} while (asLine2.Length() % NbSpacesTab != 0);
					}
					else {
						asLine2 += c;
					}
				}
				if (asLine != asLine2) {
					TextEditor->Lines_Change(i, asLine2);
				}
			}
			TextEditor->EndUpdate();
			TextEditor->EndOperation();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SpacesToTabsExecute(TObject *Sender) {
	SpacesToTabs(false);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SpacesToTabsBeginExecute(TObject *Sender) {
	SpacesToTabs(true);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SpacesToTabs(bool bBeginOnly) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			TTextEditor * TextEditor = Frame_EditFichierText->TextEditor;
			TextEditor->BeginOperation(bBeginOnly? _TT("Transformation des tabulations en espaces (débuts de ligne)"): _TT("Transformation des tabulations en espaces"));
			TextEditor->BeginUpdate();
			int Count = TextEditor->Lines->Count;
			int NbSpacesTab = Config->NbSpacesTab;
			AnsiString asLine;
			AnsiString asLine2;
			int Len;
			int NumCol;
			int NbSpaces;
			int i, j;
			TCHAR c, dc;
			bool bQuote;
			bool bBeginLine;
			for (i = 0; i < Count; i++) {
				asLine = TextEditor->Lines->Strings[i];
				asLine2 = _T("");
				Len = asLine.Length();
				NbSpaces = 0;
				NumCol = 0;
				bQuote = false;
				bBeginLine = true;
				dc = _T('\0');
				for (j = 0; j < Len; j++) {
					c = asLine[j + 1];
					if (c == _T(' ') && !bQuote) {
						NbSpaces++;
					}
					else if (c == _T('\t') && !bQuote) {
						NbSpaces += NbSpacesTab - NumCol % NbSpacesTab;
					}
					else {
						while (NbSpaces) {
							asLine2 += _T(" ");
							NumCol++;
							NbSpaces--;
						}
						asLine2 += c;
						NumCol++;
						bBeginLine = false;
					}

					if ((NumCol + NbSpaces) % NbSpacesTab == 0 && !bQuote && (bBeginLine || !bBeginOnly)) {
						if (NbSpaces > 1) {
							asLine2 += _T("\t");
							NumCol = NbSpacesTab * (NumCol / NbSpacesTab + 1);
						}
						else if (NbSpaces == 1) {
							asLine2 += _T(" ");
							NumCol++;
						}
						NbSpaces = 0;
					}

					if (c == _T('\"')) {
						if (dc != _T('\\')) bQuote = !bQuote;
					}

					dc = c;

				}
				if (asLine != asLine2) {
					TextEditor->Lines_Change(i, asLine2);
				}
			}
			TextEditor->EndUpdate();
			TextEditor->EndOperation();
		}
	}

	return true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_EndingSpacesExecute(TObject *Sender) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			TTextEditor * TextEditor = Frame_EditFichierText->TextEditor;
			TextEditor->BeginOperation(_TT("Suppression des espaces de fin"));
			TextEditor->BeginUpdate();
			int Count = TextEditor->Lines->Count;
			AnsiString asLine;
			int Len;
			int i;
			for (i = 0; i < Count; i++) {
				asLine = TextEditor->Lines->Strings[i];
				Len = asLine.Length();
				asLine = asLine.TrimRight();
				if (Len != asLine.Length()) {
					TextEditor->Lines_Change(i, asLine);
				}
			}
			TextEditor->EndUpdate();
			TextEditor->EndOperation();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FindExecute(TObject *Sender) {
	SearchAndReplace(0);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FindInFilesExecute(TObject *Sender) {
	SearchAndReplace(1);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ReplaceExecute(TObject *Sender) {
	SearchAndReplace(2);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ReplaceInFilesExecute(TObject *Sender) {
	SearchAndReplace(3);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FindPreviousExecute(TObject *Sender) {
	int Index;
	TCustomEditor * CustomEditor;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		CustomEditor = NULL;

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			CustomEditor = Frame_EditFichierText->TextEditor;
		}
		else {
			TFrame_EditFichierHexa *Frame_EditFichierHexa;
			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
			if (Frame_EditFichierHexa) {
				CustomEditor = Frame_EditFichierHexa->HexaEditor;
			}
		}

		if (CustomEditor) {
			AnsiString asSearch;
			asSearch = Config->SearchStrings->Strings[0];
			SearchParam.Down = false;
			SearchParam.SinceCursor = true;
			FVCLIDEAutomation->DisplayMessage(td_Message, _T(""));
			if (!CustomEditor->Search(asSearch, &SearchParam)) {
				AnsiString asMsg;
				asMsg.sprintf(_TT("Texte \"%s\" non trouvé"), (LPCTSTR) asSearch);
				FVCLIDEAutomation->DisplayMessage(td_Message, asMsg);
				DisplayError(asMsg,
										 _TT("Recherche"),
										 _T("STRING_NOT_FOUND"),
										 MB_OK | MB_ICONINFORMATION);
			}
			CustomEditor->SetFocusChild();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FindNextExecute(TObject *Sender) {
	int Index;
	TCustomEditor * CustomEditor;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		CustomEditor = NULL;

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			CustomEditor = Frame_EditFichierText->TextEditor;
		}
		else {
			TFrame_EditFichierHexa *Frame_EditFichierHexa;
			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
			if (Frame_EditFichierHexa) {
				CustomEditor = Frame_EditFichierHexa->HexaEditor;
			}
		}

		if (CustomEditor) {
			AnsiString asSearch;
			asSearch = Config->SearchStrings->Strings[0];
			SearchParam.Down = true;
			SearchParam.SinceCursor = true;
			ReachBeginOrEnd = 0;
			FVCLIDEAutomation->DisplayMessage(td_Message, _T(""));
			if (!CustomEditor->Search(asSearch, &SearchParam)) {
				AnsiString asMsg;
				asMsg.sprintf(_TT("Texte \"%s\" non trouvé"), (LPCTSTR) asSearch);
				FVCLIDEAutomation->DisplayMessage(td_Message, asMsg);
				DisplayError(asMsg,
										 _TT("Recherche"),
										 _T("STRING_NOT_FOUND"),
										 MB_OK | MB_ICONINFORMATION);
			}
			CustomEditor->SetFocusChild();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FindSelPreviousExecute(TObject *Sender) {
	int Index;
	TCustomEditor * CustomEditor;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		CustomEditor = NULL;

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			CustomEditor = Frame_EditFichierText->TextEditor;
		}
		else {
			TFrame_EditFichierHexa *Frame_EditFichierHexa;
			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
			if (Frame_EditFichierHexa) {
				CustomEditor = Frame_EditFichierHexa->HexaEditor;
			}
		}

		if (CustomEditor) {
			AnsiString asSearch;
			if (!asSearch.IsEmpty()) {
				Config->PushSearchString(asSearch);
				SearchParam.Down = false;
				SearchParam.SinceCursor = true;
				FVCLIDEAutomation->DisplayMessage(td_Message, _T(""));
				if (!CustomEditor->Search(asSearch, &SearchParam)) {
					AnsiString asMsg;
					asMsg.sprintf(_TT("Texte \"%s\" non trouvé"), (LPCTSTR) asSearch);
					FVCLIDEAutomation->DisplayMessage(td_Message, asMsg);
					DisplayError(asMsg,
											 _TT("Recherche"),
											 _T("STRING_NOT_FOUND"),
											 MB_OK | MB_ICONINFORMATION);
				}
			}
			else {
				DisplayError(_TT("Aucun texte sélectionné"),
										 _TT("Recherche"),
										 _T("NO_SELECTION"),
										 MB_OK | MB_ICONINFORMATION);
			}
			CustomEditor->SetFocusChild();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FindSelNextExecute(TObject *Sender) {
	int Index;
	TCustomEditor * CustomEditor;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		CustomEditor = NULL;

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			CustomEditor = Frame_EditFichierText->TextEditor;
		}
		else {
			TFrame_EditFichierHexa *Frame_EditFichierHexa;
			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
			if (Frame_EditFichierHexa) {
				CustomEditor = Frame_EditFichierHexa->HexaEditor;
			}
		}

		if (CustomEditor) {
			AnsiString asSearch;
			asSearch = FVCLIDEAutomation->GetSelText();
			if (!asSearch.IsEmpty()) {
				Config->PushSearchString(asSearch);
				SearchParam.Down = true;
				SearchParam.SinceCursor = true;
				ReachBeginOrEnd = 0;
				FVCLIDEAutomation->DisplayMessage(td_Message, _T(""));
				if (!CustomEditor->Search(asSearch, &SearchParam)) {
					AnsiString asMsg;
					asMsg.sprintf(_TT("Texte \"%s\" non trouvé"), (LPCTSTR) asSearch);
					FVCLIDEAutomation->DisplayMessage(td_Message, asMsg);
					DisplayError(asMsg,
											 _TT("Recherche"),
											 _T("STRING_NOT_FOUND"),
											 MB_OK | MB_ICONINFORMATION);
				}
			}
			else {
				DisplayError(_TT("Aucun texte sélectionné"),
										 _TT("Recherche"),
										 _T("NO_SELECTION"),
										 MB_OK | MB_ICONINFORMATION);
			}
			CustomEditor->SetFocusChild();
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RecMacroExecute(TObject *Sender) {
	bRecMacro = !bRecMacro;

	if (bRecMacro) {
		KeyActions.clear();
		MenuItem_RecMacro->Caption = _TT("Arrêter l'enregistrement");
		MenuItem_PlayMacro->Enabled = false;
		MenuItem_RepeatMacro->Enabled = false;
	}
	else {
		MenuItem_RecMacro->Caption = _TT("Enregistrer une macro");
		MenuItem_PlayMacro->Enabled = true;
		MenuItem_RepeatMacro->Enabled = true;
	}

}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::PlayMacro(bool bStopBegin, bool bStopEnd) {
  INPUT Input;
	int i;


	for (i = 0; i < (int) KeyActions.size(); i++) {

		ReachBeginOrEnd = 0;

		if (KeyActions[i].Shift.Contains(ssCtrl)) {
			Input.type = INPUT_KEYBOARD;
			Input.ki.wVk = VK_CONTROL;
			Input.ki.wScan = 0;
			Input.ki.time = GetTickCount();
			Input.ki.dwExtraInfo = GetMessageExtraInfo();
			Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
			SendInput(1, &Input, sizeof(Input));
		}

		if (KeyActions[i].Shift.Contains(ssShift)) {
			Input.type = INPUT_KEYBOARD;
			Input.ki.wVk = VK_SHIFT;
			Input.ki.wScan = 0;
			Input.ki.time = GetTickCount();
			Input.ki.dwExtraInfo = GetMessageExtraInfo();
			Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
			SendInput(1, &Input, sizeof(Input));
		}

		if (KeyActions[i].Shift.Contains(ssAlt)) {
			Input.type = INPUT_KEYBOARD;
			Input.ki.wVk = VK_MENU;
			Input.ki.wScan = 0;
			Input.ki.time = GetTickCount();
			Input.ki.dwExtraInfo = GetMessageExtraInfo();
			Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
			SendInput(1, &Input, sizeof(Input));
		}

		Input.type = INPUT_KEYBOARD;
		Input.ki.wVk = KeyActions[i].nVirtKey;
		Input.ki.wScan = 0;
		Input.ki.time = GetTickCount();
		Input.ki.dwExtraInfo = GetMessageExtraInfo();
		Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
		SendInput(1, &Input, sizeof(Input));

		Input.ki.time = GetTickCount();
		Input.ki.dwExtraInfo = GetMessageExtraInfo();
		Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
		SendInput(1, &Input, sizeof(Input));

		if (KeyActions[i].Shift.Contains(ssAlt)) {
			Input.type = INPUT_KEYBOARD;
			Input.ki.wVk = VK_MENU;
			Input.ki.wScan = 0;
			Input.ki.time = GetTickCount();
			Input.ki.dwExtraInfo = GetMessageExtraInfo();
			Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
			SendInput(1, &Input, sizeof(Input));
		}

		if (KeyActions[i].Shift.Contains(ssShift)) {
			Input.type = INPUT_KEYBOARD;
			Input.ki.wVk = VK_SHIFT;
			Input.ki.wScan = 0;
			Input.ki.time = GetTickCount();
			Input.ki.dwExtraInfo = GetMessageExtraInfo();
			Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
			SendInput(1, &Input, sizeof(Input));
		}

		if (KeyActions[i].Shift.Contains(ssCtrl)) {
			Input.type = INPUT_KEYBOARD;
			Input.ki.wVk = VK_CONTROL;
			Input.ki.wScan = 0;
			Input.ki.time = GetTickCount();
			Input.ki.dwExtraInfo = GetMessageExtraInfo();
			Input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
			SendInput(1, &Input, sizeof(Input));
		}

		Sleep(5);  // Le temps d'ouvrir une fenêtre, par exemple
		Application->ProcessMessages();

		if (bStopBegin && ReachBeginOrEnd == 1) return false;
		if (bStopEnd && ReachBeginOrEnd == 2) return false;

	}

	return true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_PlayMacroExecute(TObject *Sender) {

	if (!bRecMacro) {

		TTextEditor *TextEditor = FVCLIDEAutomation->GetCurTextFile();
		if (TextEditor) {
			if (KeyActions.size() > 0) {
				// Attend que l'utilisateur relâche les touches SHIFT et CTRL
				while ((GetKeyState(VK_CONTROL) & 0x8000) != 0 ||
								(GetKeyState(VK_SHIFT) & 0x8000) != 0 ||
								(GetKeyState(VK_MENU) & 0x8000) != 0) {
					Sleep(2);
					Application->ProcessMessages();
				}

				TextEditor->BeginOperation(_T("Exécution d'une macro"));
				PlayMacro(false, false);
				TextEditor->EndOperation();
			}
			else {
				DisplayError(_TT("Aucune macro enregistrée."),
											_TT("Exécuter une macro"),
											_T("EMPTY_MACRO"),
											MB_OK | MB_ICONWARNING);
			}
		}
		else {
			DisplayError(_TT("Impossible d'exécuter une macro dans ce type de fichier."),
										_TT("Exécuter une macro"),
										_T("BAD_FILE_TYPE"),
										MB_OK | MB_ICONWARNING);
		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RepeatMacroExecute(TObject *Sender) {
	TForm_RepeatMacro *Form_RepeatMacro;
	int i;
	int NbrRepet;
	bool bStopBegin, bStopEnd;


	TTextEditor *TextEditor = FVCLIDEAutomation->GetCurTextFile();
	if (TextEditor) {
		if (KeyActions.size() > 0) {
			Form_RepeatMacro = new TForm_RepeatMacro(this, _T("TForm_RepeatMacro"));
			if (Form_RepeatMacro->ShowModal() == mrOk) {
				// Attend que l'utilisateur relâche les touches SHIFT et CTRL
				while ((GetKeyState(VK_CONTROL) & 0x8000) != 0 ||
							 (GetKeyState(VK_SHIFT) & 0x8000) != 0 ||
							 (GetKeyState(VK_MENU) & 0x8000) != 0) {
					Sleep(2);
					Application->ProcessMessages();
				}

				NbrRepet = ((AnsiString) Form_RepeatMacro->TEdit_NbrRepet->Text).ToIntDef(0);
				bStopBegin = Form_RepeatMacro->TCheckBox_StopBegin->Checked;
				bStopEnd = Form_RepeatMacro->TCheckBox_StopEnd->Checked;

				TextEditor->BeginOperation(_T("Répétition d'une macro"));
				for (i = 0; i < NbrRepet; i++) {
					if (PlayMacro(bStopBegin, bStopEnd) == false) break;
				}
				TextEditor->EndOperation();
			}
			delete Form_RepeatMacro;
		}
		else {
			DisplayError(_TT("Aucune macro enregistrée."),
										_TT("Répéter une macro"),
										_T("EMPTY_MACRO"),
										MB_OK | MB_ICONWARNING);
		}
	}
	else {
		DisplayError(_TT("Impossible d'exécuter une macro dans ce type de fichier."),
								 _TT("Répéter une macro"),
								 _T("BAD_FILE_TYPE"),
								 MB_OK | MB_ICONWARNING);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ModelsExecute(TObject *Sender) {
	TForm_Models *Form_Models;


	TTextEditor *TextEditor = FVCLIDEAutomation->GetCurTextFile();
	if (TextEditor) {
		Form_Models = new TForm_Models(this, _T("TForm_Models"));
		Form_Models->ShowModal();
		delete Form_Models;
	}
	else {
		DisplayError(_TT("Impossible d'insérer du code dans ce type de fichier."),
								 _TT("Insertion d'un modèle de code"),
								 _T("BAD_FILE_TYPE"),
								 MB_OK | MB_ICONWARNING);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_PreCompilCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Projet à pré-compiler"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	TreeNode = NULL;
	for (i = 0; i < TreeView_Projects->Items->Count; i++) {
		if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
			TreeNode = TreeView_Projects->Items->Item[i];
			break;
		}
	}

	if (TreeNode) {
		if (bCodeOnly) Action_CodeOnlyExecute(this);
		ClearLogMessage(tl_Build);
		PreCompilationNode(TreeNode);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_BuildCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int IndexTypeProject;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Projet à construire"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	if (!QuestDebugging()) return;

	if (!bBuilding) {

		bBuilding = true;  // Pour éviter plusieurs builds en parallèle
		ToolButton_BuildCurrent->Enabled = false;

		TreeNode = NULL;
		for (i = 0; i < TreeView_Projects->Items->Count; i++) {
			if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
				TreeNode = TreeView_Projects->Items->Item[i];
				break;
			}
		}

		if (TreeNode) {
			IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
			if (IndexTypeProject >= 0) {
				if (SaveFilesOfNode(TreeNode)) {
					if (bCodeOnly) Action_CodeOnlyExecute(this);
					ClearLogMessage(tl_Build);
					BuildNode(TreeNode, false);
				}
			}

			UpdateTreeViewStates();

		}

		ToolButton_BuildCurrent->Enabled = true;
		bBuilding = false;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RebuildCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int IndexTypeProject;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Projet à construire"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	if (!QuestDebugging()) return;

	if (!bBuilding) {

		bBuilding = true;  // Pour éviter plusieurs builds en parallèle
		ToolButton_BuildCurrent->Enabled = false;

		TreeNode = NULL;
		for (i = 0; i < TreeView_Projects->Items->Count; i++) {
			if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
				TreeNode = TreeView_Projects->Items->Item[i];
				break;
			}
		}

		if (TreeNode) {
			IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
			if (IndexTypeProject >= 0) {
				if (SaveFilesOfNode(TreeNode)) {
					if (bCodeOnly) Action_CodeOnlyExecute(this);
					ClearLogMessage(tl_Build);
					BuildNode(TreeNode, true);
				}
			}

			UpdateTreeViewStates();

		}

		ToolButton_BuildCurrent->Enabled = true;
		bBuilding = false;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_PreCompilSelectedExecute(TObject *Sender) {
	TTreeNode *TreeNode;


	TreeNode = TreeView_Projects->Selected;
	if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {
		if (bCodeOnly) Action_CodeOnlyExecute(this);
		ClearLogMessage(tl_Build);
		PreCompilationNode(TreeNode);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_BuildSelectedExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int IndexTypeProject;


	if (!QuestDebugging()) return;

	TreeNode = TreeView_Projects->Selected;
	if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {
		IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject >= 0) {
			if (SaveFilesOfNode(TreeNode)) {
				if (bCodeOnly) Action_CodeOnlyExecute(this);
				ClearLogMessage(tl_Build);
				BuildNode(TreeNode, false);
			}
		}

		UpdateTreeViewStates();

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RebuildSelectedExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int IndexTypeProject;


	if (!QuestDebugging()) return;

	TreeNode = TreeView_Projects->Selected;
	if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {
		IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject >= 0) {
			if (SaveFilesOfNode(TreeNode)) {
				if (bCodeOnly) Action_CodeOnlyExecute(this);
				ClearLogMessage(tl_Build);
				BuildNode(TreeNode, true);
			}
		}

		UpdateTreeViewStates();

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_PreCompilAllExecute(TObject *Sender) {
	TTreeNode *TreeNode;


	TreeNode = TreeView_Projects->Selected;
	if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {
		if (bCodeOnly) Action_CodeOnlyExecute(this);
		ClearLogMessage(tl_Build);
		PreCompilationNode(TreeNode);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_BuildAllExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int IndexTypeProject;


	if (!QuestDebugging()) return;

	TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {
		IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject >= 0) {
			if (SaveFilesOfNode(TreeNode)) {
				if (bCodeOnly) Action_CodeOnlyExecute(this);
				ClearLogMessage(tl_Build);
				BuildNode(TreeNode, false);
			}
		}

		UpdateTreeViewStates();

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RebuildAllExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int IndexTypeProject;


	if (!QuestDebugging()) return;

	TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {
		IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject >= 0) {
			if (SaveFilesOfNode(TreeNode)) {
				if (bCodeOnly) Action_CodeOnlyExecute(this);
				ClearLogMessage(tl_Build);
				BuildNode(TreeNode, true);
			}
		}

		UpdateTreeViewStates();

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_SetCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNodeSelected;


	TreeNodeSelected = TreeView_Projects->Selected;
	if (TreeNodeSelected) {
		CurrentProject = reinterpret_cast<TProject *>((void *) TreeNodeSelected->Data);
		UpdateTreeViewStates();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ViewOutputExecute(TObject *Sender) {
	TTreeNode *TreeNodeSelected;
	AnsiString asOutputFileName;


	TreeNodeSelected = TreeView_Projects->Selected;
	if (TreeNodeSelected) {
		TProject *Project = reinterpret_cast<TProject *>((void *) TreeNodeSelected->Data);
		if (Project) {
			asOutputFileName = Project->Output;
			if (!asOutputFileName.IsEmpty()) {
				OpenFile(asOutputFileName);
			}
		}
		else {
			AnsiString asNodeName, asMessage;
			asNodeName = TreeNodeSelected->Text;
			asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
			DisplayError(asMessage,
									 _TT("Opération impossible"),
									 _T(""),
									 MB_OK | MB_ICONWARNING);
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_PropertiesExecute(TObject *Sender) {
	TForm_FileProperties *Form_FileProperties;
	TTreeNode *TreeNode;
	TProject *Project;
	int IndexTypeProject;
	AnsiString asFileName;
	WIN32_FILE_ATTRIBUTE_DATA FileAttributeData;
	TDateTime dtLastChange;
	TDateTime dtBuilt;
	TDateTime dtUninitialized;
	AnsiString asLastChange;
	AnsiString asBuilt;
	AnsiString asMessage;
	bool bMustBuild;


	TreeNode = TreeView_Projects->Selected;
	if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {

		Form_FileProperties = new TForm_FileProperties(this, _T("TForm_FileProperties"));

		Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
		IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;

		if (Project) {

			TBitmap *Bitmap = Project->GetBitmap48();
			Form_FileProperties->Image_Icon->Picture->Graphic = Bitmap;
			delete Bitmap;
			Form_FileProperties->Label_Nom->Caption = Project->Name;
			Form_FileProperties->Label_TypeProjet->Caption = Project->GetDescription();

			asFileName = Project->FileName;
			if (!asFileName.IsEmpty()) {
				Form_FileProperties->Label_File->Caption = asFileName;
			}
			else {
				Form_FileProperties->Label_File->Caption = _TT("(Projet inclus dans le workspace)");
			}

			if (IndexTypeProject >= 0) {
				dtLastChange = Project->LastChange;
				dtBuilt = Project->Built;
				if (dtLastChange != dtUninitialized) {
					asLastChange = dtLastChange.FormatString(_T("dd/mm/yyyy hh:nn:ss"));
				}
				if (dtBuilt != dtUninitialized) {
					asBuilt = dtBuilt.FormatString(_T("dd/mm/yyyy hh:nn:ss"));
				}
			}
			else {
				if (GetFileAttributesEx(asFileName, GetFileExInfoStandard, &FileAttributeData)) {
					dtLastChange = FileAttributeData.ftLastWriteTime;
					dtBuilt = dtLastChange;
					asLastChange = dtLastChange.FormatString(_T("dd/mm/yyyy hh:nn:ss"));
					asBuilt = dtBuilt.FormatString(_T("dd/mm/yyyy hh:nn:ss"));
				}
				else {
					TCHAR szBuf[128];
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
					asLastChange = szBuf;
					asBuilt = asLastChange;
				}
			}
			Form_FileProperties->Label_LastModif->Caption = asLastChange;
			Form_FileProperties->Label_Built->Caption = asBuilt;

			if (IndexTypeProject >= 0) {
				MustBuildNode(TreeNode, &bMustBuild, &asMessage);
				Bitmap = new TBitmap();
				Bitmap->Width = 20;
				Bitmap->Height = 20;
				if (bMustBuild) {
					Bitmap->LoadFromResource(HInstance, _T("BMP_UPDATENO"));
				}
				else {
					Bitmap->LoadFromResource(HInstance, _T("BMP_UPDATEYES"));
				}
				Form_FileProperties->Image_State->Picture->Graphic = Bitmap;
				delete Bitmap;
				Form_FileProperties->Label_State->Caption = asMessage;
			}

			Form_FileProperties->ShowModal();
			delete Form_FileProperties;

		}
		else {
			AnsiString asNodeName, asMessage;
			asNodeName = TreeNode->Text;
			asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
			DisplayError(asMessage,
									 _TT("Opération impossible"),
									 _T(""),
									 MB_OK | MB_ICONWARNING);
		}

	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_RunCurrentExecute(TObject *Sender) {
	TTreeNode *TreeNode;
	int IndexTypeProject;
	AnsiString asMessage;
	bool bMustBuild;
	int i;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Projet à lancer"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	if (!QuestDebugging()) return;

	TreeNode = NULL;
	for (i = 0; i < TreeView_Projects->Items->Count; i++) {
		if (CurrentProject == TreeView_Projects->Items->Item[i]->Data) {
			TreeNode = TreeView_Projects->Items->Item[i];
			break;
		}
	}

	if (TreeNode) {
		if (SaveFilesOfNode(TreeNode)) {
			IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
			if (IndexTypeProject >= 0) {
				MustBuildNode(TreeNode, &bMustBuild, &asMessage);
				if (bMustBuild) {
					int Rep;
					AnsiString asMsg;

					asMsg.sprintf(_TT("Le projet \"%s\" n'est pas à jour,\nvoulez-vous le construire ?"),
												(LPCTSTR) ((AnsiString) TreeNode->Text));
					Rep = DisplayError(asMsg,
														 _TT("Projet à construire"),
														 _T("BUILD_BEFORE_RUN"),
														 MB_YESNOCANCEL | MB_ICONQUESTION);
					if (Rep == IDCANCEL) return;
					else if (Rep == IDYES) {
						if (!BuildNode(TreeNode, false)) return;
					}

				}

				ClearLogMessage(tl_Process);
				if (RunNode(TreeNode)) {
					AddLogMessage(tl_Process, _TT("Exécution réussie"));
				}
				else {
					AddLogMessage(tl_Process, _TT("Erreur(s) lors de l'exécution"));
				}

			}
		}
	}

}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::StartDebug(void) {
	TTreeNode *TreeNode;
	TProject *Project;
	int IndexTypeProject;
	int Rep;
	AnsiString asMsg;
	AnsiString asProjectFileName;
	AnsiString asProjectContent;
	AnsiString asMessage;
	bool bMustBuild;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Débuggage"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return false;
	}

	TreeNode = TreeView_Projects->Selected;
	if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {
		if (SaveFilesOfNode(TreeNode)) {
			IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
			if (IndexTypeProject >= 0) {
				MustBuildNode(TreeNode, &bMustBuild, &asMessage);
				if (bMustBuild) {

					asMsg.sprintf(_TT("Le projet \"%s\" n'est pas à jour,\nvoulez-vous le construire ?"),
												(LPCTSTR) ((AnsiString) TreeNode->Text));
					Rep = DisplayError(asMsg,
														 _TT("Projet à construire"),
														 _T("BUILD_BEFORE_RUN"),
														 MB_YESNOCANCEL | MB_ICONQUESTION);
					if (Rep == IDCANCEL) return false;
					else if (Rep == IDYES) {
						if (!BuildNode(TreeNode, false)) return false;
					}

				}

				if (NodeToDebug(TreeNode, &Project)) {

					asProjectFileName = Project->FileName;
					asProjectContent = Project->Content;

					if (DebugProject->StartDebug()) {
						bDebugging = true;
						MenuItem_Debug->Caption = _TT("Continue");
						MenuItem_StopDebug->Enabled = true;
						if (bCodeOnly) {
							bMemoCodeOnly = true;
							Action_CodeOnlyExecute(this);
						}
						else {
							bMemoCodeOnly = false;
						}
						ClearLogMessage(tl_Process);
						AddLogMessage(tl_Process, _TT("Démarrage du debogueur"));
						return true;
					}
				}

				asMsg.sprintf(_TT("Le projet \"%s\" ne peut pas être débogué,\nvoulez-vous le lancer sans dégogage ?"),
											(LPCTSTR) ((AnsiString) TreeNode->Text));
				Rep = DisplayError(asMsg,
														_TT("Erreur debogage"),
														_T("RUN_IF_CANT_DEBUG"),
														MB_YESNOCANCEL | MB_ICONQUESTION);
				if (Rep == IDCANCEL) return false;
				else if (Rep == IDYES) {
					if (RunNode(TreeNode)) {
						AddLogMessage(tl_Process, _TT("Exécution réussie"));
					}
					else {
						AddLogMessage(tl_Process, _TT("Erreur(s) lors de l'exécution"));
					}

				}

			}
		}
	}

	return false;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_DebugCurrentExecute(TObject *Sender) {
	StepByStep(tsbs_Continue);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_StopDebugExecute(TObject *Sender) {

	if (bDebugging) {
		bDebugging = false;
		MenuItem_Debug->Caption = _TT("&Lancer le débogueur");
		MenuItem_StopDebug->Enabled = false;
		if (bMemoCodeOnly) Action_CodeOnlyExecute(this);
		SetCurrentDebugLine(_T(""), -1, -1, -1, -1);
		AddLogMessage(tl_Process, _TT("Arrêt manuel du debogueur"));
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_StepByStepNextExecute(TObject *Sender) {
	StepByStep(tsbs_Next);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_StepByStepInExecute(TObject *Sender) {
	StepByStep(tsbs_In);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_StepByStepOutExecute(TObject *Sender) {
	StepByStep(tsbs_Out);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::StepByStep(TYPESTEPBYSTEP TypeStepByStep) {
	int NumLine1, NumCol1, NumLine2, NumCol2;
	AnsiString asFileName;
	AnsiString asCallStack;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Pas-à-pas"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	if (!bDebugging) {
		if (!StartDebug()) return;
	}

	if (DebugProject->StepByStep(TypeStepByStep, &asFileName, &NumLine1, &NumCol1, &NumLine2, &NumCol2)) {

		if (NumLine1 != -1) {
			SetCurrentDebugLine(asFileName, NumLine1, NumCol1, NumLine2, NumCol2);
			if (DebugProject->GetCallStack(&asCallStack)) {
				ClearLogMessage(tl_CallStack);
				AddLogMessage(tl_CallStack, asCallStack);
			}
		}
		else {
			bDebugging = false;
			MenuItem_Debug->Caption = _TT("&Lancer le débogueur");
			MenuItem_StopDebug->Enabled = false;
			if (bMemoCodeOnly) Action_CodeOnlyExecute(this);
			SetCurrentDebugLine(_T(""), -1, -1, -1, -1);
			ClearLogMessage(tl_CallStack);
			AddLogMessage(tl_Process, _TT("Fin du debogage"));
		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AddBreakpointExecute(TObject *Sender) {
	AddBreakpoint();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_DelBreakpointExecute(TObject *Sender) {
	DelBreakpoint();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ClearBreakpointsExecute(TObject *Sender) {
	int i;
	int Rep;
	TTreeNode *TreeNode;
	TProject *Project;


	Rep = DisplayError(_TT("Voulez_vous réellement supprimer tous les points d'arrêt ?"),
										 _TT("Suppression des points d'arrêt"),
										 _T("CLEAR_BREAKPOINTS"),
										 MB_YESNOCANCEL | MB_ICONQUESTION);
	if (Rep != IDYES) return;

	TreeNode = TreeView_Projects->Selected;
	if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
	if (TreeNode) {

		if (NodeToDebug(TreeNode, &Project)) {
			// Ne pas tester le retour : le point d'arrêt doit pouvoir être détruit dans tous les cas
			Project->ClearBreakpoints();
		}
	}


	for (i = 0; i < (int) TabSheets.size(); i++) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) {
			Frame_EditFichierText->TextEditor->ClearFlag(lfBreakpoint);
		}

	}

	ClearLogMessage(tl_Breakpoints);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_StatisticsExecute(TObject *Sender) {
	TForm_Statistics *Form_Statistics;


	Form_Statistics = new TForm_Statistics(this, _T("TForm_Statistics"));
	Form_Statistics->ShowModal();
	delete Form_Statistics;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CodeFormatExecute(TObject *Sender) {
	int Index;
	TTextEditor *TextEditor;
	bool bEndLineAcc, bSpacesAfterPar;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			TextEditor = Frame_EditFichierText->TextEditor;
			TextEditor->BeginOperation(_TT("Formatage de code"));
			bEndLineAcc = Config->EndLineAcc;
			bSpacesAfterPar = Config->SpacesAfterPar;
			Frame_EditFichierText->CodeFormat(bEndLineAcc, bSpacesAfterPar);
			TextEditor->EndOperation();
		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ConvertNumbersExecute(TObject *Sender) {
	TForm_ConvertNumbers *Form_ConvertNumbers;


	Form_ConvertNumbers = new TForm_ConvertNumbers(this, _T("TForm_ConvertNumbers"));
	Form_ConvertNumbers->ShowModal();
	delete Form_ConvertNumbers;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ConfigToolsExecute(TObject *Sender) {
	TForm_Tools *Form_Tools;
	bool Ok;
	int i;


	Form_Tools = new TForm_Tools(this, _T("TForm_Tools"));
	Ok = (Form_Tools->ShowModal() == mrOk);
	delete Form_Tools;

	if (Ok) {
		for (i = MenuItem_UserTools->ComponentCount - 1; i >= 2; i--) {
			MenuItem_UserTools->Delete(i);
		}
		AddUserToolsInMenu();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::MenuItem_UserToolClick(TObject *Sender) {
	TMenuItem *MenuItem;
	const USERTOOL *UserTool;
	AnsiString asIdTool;
	int IdTool;


	MenuItem = dynamic_cast<TMenuItem *>(Sender);
	if (MenuItem) {
		asIdTool = ((AnsiString) MenuItem->Name).SubString(9, ((AnsiString) MenuItem->Name).Length() - 8);  // 8 = sizeof("UserTool") (nom du MenuItem)
		IdTool = asIdTool.ToIntDef(0);
		UserTool = Config->UserTools[IdTool];
		if (UserTool) {
			ShellExecute(FHandle, _T("open"),
									 UserTool->Prog,
									 UserTool->Args,
									 UserTool->InitDir,
									 SW_SHOWNORMAL);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_LanguageExecute(TObject *Sender) {
	Application->ChooseLanguage();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CodeOnlyExecute(TObject *Sender) {
	PageControl_Files->DisableAlign();
	if (!bCodeOnly) {
		Action_CodeOnly->Checked = true;
		PageControl_Logs->Visible = false;
		Splitter1->Visible = false;
		TreeView_Projects->Visible = false;
		Splitter2->Visible = false;
		bCodeOnly = true;
	}
	else {
		Action_CodeOnly->Checked = false;
		Splitter2->Visible = true;
		TreeView_Projects->Visible = true;
		Splitter1->Visible = true;
		PageControl_Logs->Visible = true;
		bCodeOnly = false;
	}
	PageControl_Files->EnableAlign();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FVCLIDEHelpExecute(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath + _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTENTS, 0);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FreeVCLHelpExecute(TObject *Sender) {
	int Index;
	AnsiString asWord;
	AnsiString asContext;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			int i;
			TCHAR c;
			asWord = Frame_EditFichierText->TextEditor->GetSelText();
			if (!asWord.IsEmpty()) {
				asContext = _T("class");
				for (i = 1; i <= asWord.Length(); i++) {
					c = asWord[i];
					if (_T('a') <= c && c <= _T('z')) {
						asContext += c;
					}
					else if (_T('0') <= c && c <= _T('9')) {
						asContext += c;
					}
					else if (_T('A') <= c && c <= _T('Z')) {
						asContext += _T('_');
						asContext += c;
					}
					else if (c == _T('_')) {
						asContext += _T("__");
					}
					else if (c == _T('.')) {
						asContext += _T("_8");
					}
					else {
						asContext = _T("");
						break;
					}
				}
			}
		}
	}

	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FreeVCL.chm");
	if (asContext.IsEmpty()) {
		Application->HelpCommand(HELP_CONTENTS, 0);
	}
	else {
		asContext = asContext.LowerCase();
		asContext += _T(".html");
		Application->HelpCommand(HELP_CONTEXT, (int) (size_t) (LPCTSTR) asContext);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AutocompletionExecute(TObject *Sender) {
	int Index;
	TTextEditor *TextEditor;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			TextEditor = Frame_EditFichierText->TextEditor;
			TextEditor->AutocompletionOn(-1);
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_AProposExecute(TObject *Sender) {
	TForm_APropos *Form_APropos;


	Form_APropos = new TForm_APropos(this, _T("TForm_APropos"));
	Form_APropos->ShowModal();
	delete Form_APropos;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ContactMeExecute(TObject *Sender) {
	MeContacter(FHandle);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_MiseAJourExecute(TObject *Sender) {
  HKEY hKey;
  DWORD dwSize, dwRead;
	BOOL bCheckUpdates;
  DWORD NbFailsUpdates;
  int rc;
	int Rep;


	bCheckUpdates = TRUE;

	rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Denis ROBERT\\FVCLIDE"),
                    0, KEY_ALL_ACCESS, &hKey);
	if (rc == ERROR_ACCESS_DENIED) {
		rc = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE"),
											0, KEY_ALL_ACCESS, &hKey);
	}
	if (rc == ERROR_SUCCESS) {

		// Mise à jour désactivée
		dwSize = sizeof(DWORD);
		RegQueryValueEx(hKey, _T("CheckUpdates"), NULL, &dwRead, (BYTE*) &bCheckUpdates, &dwSize);

		RegCloseKey(hKey);

	}

	if (bCheckUpdates == FALSE) {

		Rep = DisplayError(_T("La mise à jour automatique a été désactivée.\nVoulez-vous la réactiver ?"),
										_T("Mise à jour"),
										_T("QUESTENABLEUPDATE"),
										MB_YESNO | MB_ICONWARNING);
		if (Rep == IDYES) {

			bCheckUpdates = TRUE;
			NbFailsUpdates = 0;

			rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
													_T("Software\\Denis ROBERT\\FVCLIDE"),
													0,
													0,
													REG_OPTION_NON_VOLATILE,
													KEY_ALL_ACCESS,
													NULL,
													&hKey,
													&dwSize);
			if (rc == ERROR_ACCESS_DENIED) {
				rc = RegCreateKeyEx(HKEY_CURRENT_USER,
														_T("Software\\Denis ROBERT\\FVCLIDE"),
														0,
														0,
														REG_OPTION_NON_VOLATILE,
														KEY_ALL_ACCESS,
														NULL,
														&hKey,
														&dwSize);
			}
			if (rc == ERROR_SUCCESS) {

				// Mise à jour désactivée
				rc = RegSetValueEx(hKey, _T("CheckUpdates"), 0, REG_DWORD, (BYTE*) &bCheckUpdates, sizeof(DWORD));

				// Nombre d'échecs de la mise à jour
				rc = RegSetValueEx(hKey, _T("NbFailsUpdates"), 0, REG_DWORD, (BYTE*) &NbFailsUpdates, sizeof(DWORD));

				RegCloseKey(hKey);

			}

		}

	}

	switch (CheckUpdates(true)) {
		case 1:  // Le check a échoué  
			DisplayError(_T("Impossible de vérifier les mises à jour. Vérifiez que vous êtes bien connecté à internet et que le pare-feu est bien configuré."),
								_T("Mise à jour"),
								_T(""),
								MB_OK | MB_ICONWARNING);
			break;
		case 3:  // Programme à jour
			DisplayError(_T("Aucune nouvelle mise à jour trouvée."),
								_T("Mise à jour"),
								_T(""),
								MB_OK | MB_ICONINFORMATION);
			break;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ShortCutsExecute(TObject *Sender) {
	TForm_SaisieRacc *Form_SaisieRacc;

	Form_SaisieRacc = new TForm_SaisieRacc(this, _T("TForm_SaisieRacc"));
	Form_SaisieRacc->ShowModal();
	delete Form_SaisieRacc;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_LogCopyLineExecute(TObject *Sender) {
	AnsiString asLine;
	TListBox *ListBox_Log = NULL;
	int LineNumber;


	switch (PageControl_Logs->ActivePageIndex) {
		case tl_Search: ListBox_Log = ListBox_Search; break;
		case tl_Bookmarks: ListBox_Log = ListBox_Bookmarks; break;
		case tl_Build: ListBox_Log = ListBox_Build; break;
		case tl_Process: ListBox_Log = ListBox_Process; break;
		case tl_Breakpoints: ListBox_Log = ListBox_Breakpoints; break;
		case tl_CallStack: ListBox_Log = ListBox_CallStack; break;
	}

	if (ListBox_Log) {
		LineNumber = ListBox_Log->ItemIndex;
		if (LineNumber >= 0) {
			asLine = ListBox_Log->Items->Strings[LineNumber];
		}

		Clipboard->Open();
		Clipboard->Clear();
		Clipboard->SetTextBuf(asLine);
		Clipboard->Close();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_LogCopyAllExecute(TObject *Sender) {
	AnsiString asLines;
	TListBox *ListBox_Log = NULL;
	int i;


	switch (PageControl_Logs->ActivePageIndex) {
		case tl_Search: ListBox_Log = ListBox_Search; break;
		case tl_Bookmarks: ListBox_Log = ListBox_Bookmarks; break;
		case tl_Build: ListBox_Log = ListBox_Build; break;
		case tl_Process: ListBox_Log = ListBox_Process; break;
		case tl_Breakpoints: ListBox_Log = ListBox_Breakpoints; break;
		case tl_CallStack: ListBox_Log = ListBox_CallStack; break;
	}

	if (ListBox_Log) {
		for (i = 0; i < ListBox_Log->Count; i++) {
			asLines += ListBox_Log->Items->Strings[i];
			asLines += _T("\r\n");
		}

		Clipboard->Open();
		Clipboard->Clear();
		Clipboard->SetTextBuf(asLines);
		Clipboard->Close();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_ColorsExecute(TObject *Sender) {
	TForm_SaisieColors *Form_SaisieColors;
	int i, j;


	Form_SaisieColors = new TForm_SaisieColors(this, _T("TForm_SaisieColors"));
	for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
		Form_SaisieColors->ColorParams[i] = Config->ColorParams[i];
	}
	if (Form_SaisieColors->ShowModal() == mrOk) {
		for (j = 0; j < (int) TabSheets.size(); j++) {

			TFrame_EditFichierText *Frame_EditFichierText;
			Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[j].Content);
			if (Frame_EditFichierText) {
				for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
					Frame_EditFichierText->TextEditor->ColorParams[i] = Form_SaisieColors->ColorParams[i];
				}
			}

			TFrame_EditFichierHexa *Frame_EditFichierHexa;
			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[j].Content);
			if (Frame_EditFichierHexa) {
				for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
					Frame_EditFichierHexa->HexaEditor->ColorParams[i] = Form_SaisieColors->ColorParams[i];
				}
			}

		}
		for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
			Config->ColorParams[i] = Form_SaisieColors->ColorParams[i];
		}
	}
	delete Form_SaisieColors;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_FontExecute(TObject *Sender) {
	int i;


	FontDialog->Font = Config->Font;
	if (FontDialog->Execute()) {
		for (i = 0; i < (int) TabSheets.size(); i++) {

			TFrame_EditFichierText *Frame_EditFichierText;
			Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
			if (Frame_EditFichierText) {
				Frame_EditFichierText->TextEditor->Font = FontDialog->Font;
				Frame_EditFichierText->TextEditor->Invalidate();
			}

			TFrame_EditFichierHexa *Frame_EditFichierHexa;
			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
			if (Frame_EditFichierHexa) {
				Frame_EditFichierHexa->HexaEditor->Font = FontDialog->Font;
				Frame_EditFichierHexa->HexaEditor->Invalidate();
			}

		}
		Config->Font = FontDialog->Font;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_EdiOptionsGeneralesExecute(TObject *Sender) {
	TForm_EdiOptions *Form_EdiOptions;

	Form_EdiOptions = new TForm_EdiOptions(this, _T("TForm_EdiOptions"));
	if (Form_EdiOptions->ShowModal() == mrOk) {
		TFrame_EditFichierText *Frame_EditFichierText;
		TFrame_EditFichierHexa *Frame_EditFichierHexa;
		int i;

		PageControl_Files->MultiLine = Config->MultiLineTabs;

		for (i = 0; i < (int) TabSheets.size(); i++) {

			Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
			if (Frame_EditFichierText) {
				Frame_EditFichierText->TextEditor->NbSpacesTab = (int) Config->NbSpacesTab;
				Frame_EditFichierText->TextEditor->SpacesToTabs = (int) Config->SpacesToTabs;
				Frame_EditFichierText->TextEditor->TabsToSpaces = (int) Config->TabsToSpaces;
				Frame_EditFichierText->TextEditor->LinesNumbers = (int) Config->LinesNumbers;
				Frame_EditFichierText->TextEditor->IconsColumn = (int) Config->IconsColumn;
				Frame_EditFichierText->TextEditor->TypeAuto = (int) Config->TypeAuto;
				Frame_EditFichierText->TextEditor->IndentAuto = (int) Config->IndentAuto;
				Frame_EditFichierText->TextEditor->CurAfterEndLine = (int) Config->CurAfterEndLine;
				Frame_EditFichierText->TextEditor->UndoRedoMoveCur = (int) Config->UndoRedoMoveCur;
				Frame_EditFichierText->TextEditor->Autocompletion = (int) Config->Autocompletion;
			}

			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
			if (Frame_EditFichierHexa) {
				Frame_EditFichierHexa->HexaEditor->IconsColumn = (int) Config->IconsColumn;
				Frame_EditFichierHexa->HexaEditor->UndoRedoMoveCur = (int) Config->UndoRedoMoveCur;
			}

			// Transparence des fenêtres d'autocomplétion et de recherche
			bool bAlphaBlend = Config->AlphaBlend;
			if (Form_Autocompletion) {
				Form_Autocompletion->AlphaBlend = bAlphaBlend;
				Form_Autocompletion->Timer_AlphaBlend->Enabled = bAlphaBlend;
			}
			if (Form_SearchAndReplace) {
				Form_SearchAndReplace->AlphaBlend = bAlphaBlend;
				Form_SearchAndReplace->Timer_AlphaBlend->Enabled = bAlphaBlend;
			}

		}

	}
	delete Form_EdiOptions;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_CodeFormatParamsExecute(TObject *Sender) {
	TForm_CodeFormatParams *Form_CodeFormatParams;

	Form_CodeFormatParams = new TForm_CodeFormatParams(this, _T("TForm_CodeFormatParams"));
	if (Form_CodeFormatParams->ShowModal() == mrOk) {
		TFrame_EditFichierText *Frame_EditFichierText;
		TFrame_EditFichierHexa *Frame_EditFichierHexa;
		int i;

		PageControl_Files->MultiLine = Config->MultiLineTabs;

		for (i = 0; i < (int) TabSheets.size(); i++) {

			Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
			if (Frame_EditFichierText) {
				Frame_EditFichierText->TextEditor->NbSpacesTab = (int) Config->NbSpacesTab;
				Frame_EditFichierText->TextEditor->SpacesToTabs = (int) Config->SpacesToTabs;
				Frame_EditFichierText->TextEditor->TabsToSpaces = (int) Config->TabsToSpaces;
				Frame_EditFichierText->TextEditor->LinesNumbers = (int) Config->LinesNumbers;
				Frame_EditFichierText->TextEditor->IconsColumn = (int) Config->IconsColumn;
				Frame_EditFichierText->TextEditor->TypeAuto = (int) Config->TypeAuto;
				Frame_EditFichierText->TextEditor->IndentAuto = (int) Config->IndentAuto;
				Frame_EditFichierText->TextEditor->CurAfterEndLine = (int) Config->CurAfterEndLine;
				Frame_EditFichierText->TextEditor->UndoRedoMoveCur = (int) Config->UndoRedoMoveCur;
				Frame_EditFichierText->TextEditor->Autocompletion = (int) Config->Autocompletion;
			}

			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
			if (Frame_EditFichierHexa) {
				Frame_EditFichierHexa->HexaEditor->IconsColumn = (int) Config->IconsColumn;
			}

		}

	}
	delete Form_CodeFormatParams;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Action_DefaultResponsesExecute(TObject *Sender) {
	TForm_DefaultResponses *Form_DefaultResponses;

	Form_DefaultResponses = new TForm_DefaultResponses(this, _T("TForm_DefaultResponses"));
	Form_DefaultResponses->ShowModal();
	delete Form_DefaultResponses;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::PageControl_FilesChange(TObject *Sender) {
	FVCLIDEAutomation->DisplayMessage(td_Position, _T(""));
	FVCLIDEAutomation->DisplayMessage(td_Message, _T(""));
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::PageControl_FilesMouseMove(TObject *Sender,
			TShiftState Shift, int X, int Y) {
	int Index;
	Index = PageControl_Files->IndexOfTabAt(X, Y);
	if (Index >= 0) {
		PageControl_Files->Hint = TabSheets[Index].Content->FileName;
	}
	else {
		PageControl_Files->Hint = "";
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::PageControl_FilesMouseDown(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y) {
	if (Button == mbRight) {
		int ActivePageIndex;
		ActivePageIndex = PageControl_Files->IndexOfTabAt(X, Y);
		if (ActivePageIndex != -1) {
			PageControl_Files->ActivePageIndex = ActivePageIndex;
			TPoint Pt;
			Pt.x = X;
			Pt.y = Y;
			Pt = PageControl_Files->ClientToScreen(Pt);
			PopupMenu_Files->Popup(Pt.x, Pt.y);
		}
	}
	PageControl_Files->SetFocusChild();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::PageControl_FilesDropFile(TObject *Sender,
			AnsiString &asFileName, TShiftState Shift, int X, int Y) {
	OpenFile(asFileName);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ListBox_SearchDblClick(TObject *Sender) {
	AnsiString asLine;


	asLine = ListBox_Search->Items->Strings[ListBox_Search->ItemIndex];
	LogLineDblClick(asLine);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ListBox_BookmarksDblClick(TObject *Sender) {
	AnsiString asLine;


	asLine = ListBox_Bookmarks->Items->Strings[ListBox_Bookmarks->ItemIndex];
	LogLineDblClick(asLine);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ListBox_BuildDblClick(TObject *Sender) {
	AnsiString asLine;


	asLine = ListBox_Build->Items->Strings[ListBox_Build->ItemIndex];
	LogLineDblClick(asLine);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ListBox_ProcessDblClick(TObject *Sender) {
	AnsiString asLine;


	asLine = ListBox_Process->Items->Strings[ListBox_Process->ItemIndex];
	LogLineDblClick(asLine);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ListBox_BreakpointsDblClick(TObject *Sender) {
	AnsiString asLine;


	asLine = ListBox_Breakpoints->Items->Strings[ListBox_Breakpoints->ItemIndex];
	LogLineDblClick(asLine);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ListBox_CallStackDblClick(TObject *Sender) {
	AnsiString asLine;


	asLine = ListBox_CallStack->Items->Strings[ListBox_CallStack->ItemIndex];
	LogLineDblClick(asLine);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ListBox_LogContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled) {
	TListBox *ListBox_Log = NULL;


	switch (PageControl_Logs->ActivePageIndex) {
		case tl_Search: ListBox_Log = ListBox_Search; break;
		case tl_Bookmarks: ListBox_Log = ListBox_Bookmarks; break;
		case tl_Build: ListBox_Log = ListBox_Build; break;
		case tl_Process: ListBox_Log = ListBox_Process; break;
		case tl_Breakpoints: ListBox_Log = ListBox_Breakpoints; break;
		case tl_CallStack: ListBox_Log = ListBox_CallStack; break;
	}

	if (ListBox_Log) {
		int ItemIndex = ListBox_Log->ItemIndex;
		MenuItemPopup_LogCopyLine->Enabled = (ItemIndex >= 0);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::TreeView_ProjectsMouseDown(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y) {
	TTreeNode *TreeNode;

	if (Button == mbLeft) {
		TreeNode = TreeView_Projects->GetNodeAt(X, Y);
		if (TreeNode) {
			TreeView_Projects->Selected = TreeNode;
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::TreeView_ProjectsEdited(TObject *Sender, TTreeNode* TreeNode, AnsiString NewText) {
	TProject * Project;
	Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (Project) Project->Name = NewText;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::TreeView_ProjectsDragOver(TObject *Sender,
        TObject *Source, int X, int Y, TDragState State, bool &Accept) {
	TTreeNode *TreeNodeSrc,  *TreeNodeDest;


	if (Sender == TreeView_Projects) {
		TreeNodeSrc = TreeView_Projects->Selected;
		TreeNodeDest = TreeView_Projects->GetNodeAt(X, Y);
		if (TreeNodeDest && TreeNodeDest->ImageIndex >= DECAL_TYPES_PROJECT) {
			if (!TreeView_Projects->Items->IsChild(TreeNodeDest, TreeNodeSrc)) {
				Accept = true;
			}
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::TreeView_ProjectsEndDrag(TObject *Sender,
        TObject *Target, int X, int Y) {
	TTreeNode *TreeNodeSrc, *TreeNodeDest;


	TreeNodeSrc = TreeView_Projects->Selected;
	TreeNodeDest = TreeView_Projects->GetNodeAt(X, Y);
	if (TreeNodeDest && TreeNodeDest->ImageIndex >= DECAL_TYPES_PROJECT) {
		if (!TreeView_Projects->Items->IsChild(TreeNodeDest, TreeNodeSrc)) {
			TTreeNode *NewTreeNode = TreeViewAddNodeProject(TreeNodeDest, _T(""));
			if (TreeView_Projects->Items->CopyNode(TreeNodeSrc, NewTreeNode)) {
				TreeView_Projects->Items->Delete(TreeNodeSrc);
				UpdateTreeViewStates();  // Il n'y a pas que les noeuds source et destination qui peuvent changer de statut
			}
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::FormClose(TObject *Sender,
			TCloseAction &Action) {

	if (!QuestDebugging())  {
		Action = caNone;
		return;
	}
	if (!QuestEnrModifs()) {
		Action = caNone;
		return;
	}

	SaveWorkSpaceIfNotEmpty();
	SaveConfig();

	// Destruction des projets (avant la fermeture de la fenêtre et la destruction du TreeView)
	TProject *Project;
	TTreeNode *TreeNode;
	int i;

	for (i = 0; i < (int) TreeView_Projects->Items->Count; i++) {
		TreeNode = TreeView_Projects->Items->Item[i];
		Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
		delete Project;
	}

	Action = caFree;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ObjectFlagChanged(TObject *Sender, bool bChanged) {
	TFrame_FVCLIDEBase *Frame_FVCLIDEBase;
	int Index;


	for (Index = 0; Index < (int) TabSheets.size(); Index++) {
		if (TabSheets[Index].Content == Sender) break;
	}
	if (Index >= (int) TabSheets.size()) Index = PageControl_Files->ActivePageIndex;

	if (Index >= 0) {
		AnsiString asCaption;
		Frame_FVCLIDEBase = TabSheets[Index].Content;
		asCaption = Frame_FVCLIDEBase->DisplayedFileName;
		if (bChanged) asCaption += _T(" *");
		PageControl_Files->Pages[Index]->Caption = asCaption;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ObjectPassBegin(TObject *Sender) {
	FVCLIDEAutomation->DisplayMessage(td_Message, _TT("Début de fichier passé"));
	ReachBeginOrEnd = 1;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ObjectPassEnd(TObject *Sender) {
	FVCLIDEAutomation->DisplayMessage(td_Message, _TT("Fin de fichier passée"));
	ReachBeginOrEnd = 2;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ObjectAutocompletionOn(TObject *Sender, AnsiString asParent, int X, int Y, int NumCompletion) {
	AnsiString asTypeContenant;
	TStringList slListVariables;


	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Auto-complétion"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return;
	}

	delete Form_Autocompletion;
	Form_Autocompletion = NULL;

	if (!asParent.IsEmpty()) GetTypeVariable(asParent, &asTypeContenant);
	if (!asTypeContenant.IsEmpty() || NumCompletion == 0) {
		slListVariables.Clear();
		slListVariables.Sorted = true;
		GetListAutocompletion(asTypeContenant, &slListVariables);
		if (slListVariables.Count > 0) {

			int Index = PageControl_Files->ActivePageIndex;
			if (Index >= 0) {
				TFrame_EditFichierText *Frame_EditFichierText;
				Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
				TTextEditor *TextEditor = Frame_EditFichierText->TextEditor;

				Form_Autocompletion = new TForm_Autocompletion(NULL, _T("TForm_Autocompletion"));
				TPoint WindowPos = TextEditor->ClientToScreen(TPoint(X, Y));
				Form_Autocompletion->Left = WindowPos.x;
				Form_Autocompletion->Top = WindowPos.y;
				Form_Autocompletion->Items->Assign(&slListVariables);
				Form_Autocompletion->Show();

				TextEditor->SetFocus();
			}

		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ObjectAutocompletionOff(TObject *Sender) {

	delete Form_Autocompletion;
	Form_Autocompletion = NULL;

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		TTextEditor *TextEditor = Frame_EditFichierText->TextEditor;
		TextEditor->SetFocus();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::TextEditorHint(TObject *Sender) {

	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			AnsiString asVarName;
			TTextEditor *TextEditor = Frame_EditFichierText->TextEditor;
			int NumCol, NumLine;
			TPoint Pt = TextEditor->ScreenToClient(Mouse->CursorPos);
			TextEditor->GetPosAt(Pt.y, Pt.x, &NumLine, &NumCol);
			if (TextEditor->WordFromPos(NumLine, NumCol, &asVarName)) {
				bool bIsHexa = false;
				bool bIsDec;
				unsigned long Number;

				if (IsDecimalOrHexa(asVarName, &Number, &bIsDec, &bIsHexa)) {
					AnsiString asToolTipText;
					asToolTipText.sprintf(_T("%i = 0x%X"), Number, Number);
					TextEditor->Tooltip->Text = asToolTipText;
				}
				else if (bDebugging) {
					TextEditor->Tooltip->Text = asVarName + _T(" = ?");
				}
				else {
					AnsiString asDescription;
					GetDescription(asVarName, &asDescription);
					TextEditor->Tooltip->Text = asDescription;
				}
			}
			else {
				TextEditor->Tooltip->Text = _T("");
			}
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Form_PrincipaleShortCut(TObject *Sender, int nVirtKey, TShiftState Shift, bool &Handled) {
	if (bRecMacro) {
		KEYACTION NewKeyAction;

		NewKeyAction.nVirtKey = nVirtKey;
		NewKeyAction.Shift = Shift;
		KeyActions.push_back(NewKeyAction);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::PopupMenu_ProjectsPopup(TObject *Sender) {
	TTreeNode *TreeNodeSelected;
	TProject *ProjectSelected;
	AnsiString asProjectFile;
	AnsiString asOutputFileName;


	TreeNodeSelected = TreeView_Projects->Selected;
	if (TreeNodeSelected) {
		ProjectSelected = reinterpret_cast<TProject *>((void *) TreeNodeSelected->Data);
		if (ProjectSelected) {
			asProjectFile = ProjectSelected->FileName;
			asOutputFileName = ProjectSelected->Output;
			bool bIsProject = (TreeNodeSelected->ImageIndex >= DECAL_TYPES_PROJECT);
			// MenuItemPopup_ExpandNode est toujours Enabled car on peut vouloir développer
			// tous les sous-noeuds même si le noeud lui-même est déjà développé.
			MenuItemPopup_CollapseNode->Enabled = (TreeNodeSelected->Expanded);
			MenuItemPopup_AddNewFileOnSelected->Enabled = bIsProject;
			MenuItemPopup_AddExistingFileOnSelected->Enabled = bIsProject;
			MenuItemPopup_AddNewProjectOnSelected->Enabled = bIsProject;
			MenuItemPopup_AddExistingProjectOnSelected->Enabled = bIsProject;
			MenuItemPopup_EditFile->Enabled = !asProjectFile.IsEmpty();
			MenuItemPopup_OpenProjectFile->Enabled = !asProjectFile.IsEmpty();
			MenuItemPopup_PreCompil->Enabled = (bIsProject && ProjectSelected->Can_PreCompil());
			MenuItemPopup_Build->Enabled = bIsProject;
			MenuItemPopup_Rebuild->Enabled = bIsProject;
			MenuItemPopup_SetCurrent->Enabled = bIsProject;
			MenuItemPopup_ViewOutput->Enabled = (!asOutputFileName.IsEmpty());
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::PopupMenu_FilesPopup(TObject *Sender) {
	bool Ok1 = false;
	bool Ok2 = false;


	int Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			AnsiString asAssociated1 = Frame_EditFichierText->TextEditor->Associated1;
			AnsiString asAssociated2 = Frame_EditFichierText->TextEditor->Associated2;
			AnsiString asCaption;
			AnsiString asFileName = ExtractFileName(TabSheets[Index].Content->FileName);
			AnsiString asExt = ExtractFileExt(asFileName);
			if (!asAssociated1.IsEmpty()) {
				asFileName = ChangeFileExt(asFileName, asAssociated1);
				asCaption.sprintf(_TT("Ouvrir \"%s\""), (LPCTSTR) asFileName);
				MenuItemPopup_OpenFichierAssocie1->Caption = asCaption;
				MenuItemPopup_OpenFichierAssocie1->Enabled = true;
				Ok1 = true;
			}
			if (!asAssociated2.IsEmpty()) {
				asFileName = ChangeFileExt(asFileName, asAssociated2);
				asCaption.sprintf(_TT("Ouvrir \"%s\""), (LPCTSTR) asFileName);
				MenuItemPopup_OpenFichierAssocie2->Caption = asCaption;
				MenuItemPopup_OpenFichierAssocie2->Enabled = true;
				Ok2 = true;
			}
		}
	}
	if (!Ok1) {
		MenuItemPopup_OpenFichierAssocie1->Caption = _T("Ouvrir fichier associé 1");
		MenuItemPopup_OpenFichierAssocie1->Enabled = false;
	}
	if (!Ok2) {
		MenuItemPopup_OpenFichierAssocie2->Caption = _T("Ouvrir fichier associé 2");
		MenuItemPopup_OpenFichierAssocie2->Enabled = false;
	}
}

//---------------------------------------------------------------------------
//
//									IMPLEMENTATION DES METHODES VIRTUELLES
//
//---------------------------------------------------------------------------

bool FASTCALL TForm_Principale::NewWorkSpace(void) {
  TXMLFile *XMLFile;
	TXMLFile *XMLNodeWorkSpace;


	XMLFile = new TXMLFile(this);
	XMLFile->NoComment = true;
	XMLFile->SetAttribute(_T("version"), _T("1.0"));
	XMLFile->SetAttribute(_T("encoding"), _T("utf-8"));

	XMLNodeWorkSpace = XMLFile->AddNode(_T("project"));
	XMLNodeWorkSpace->SetAttribute(_T("type"), _T("WorkSpace"));
	XMLNodeWorkSpace->Text = _TT("Nouveau");

	asFileWorkSpace = _T("");
	UpdateTreeView(XMLFile);
	UpdateTreeViewStates();
	Caption = _TT("Nouvel espace de travail");

	delete XMLFile;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::OpenWorkSpace(const AnsiString &asFileName) {
	TFrame_EditFichierText *Frame_EditFichierText;
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
  TXMLFile *XMLFile;
	bool Ok = false;


	Screen->Cursor = crHourGlass;

	XMLFile = new TXMLFile(this);

	asFileWorkSpace = asFileName;
	if (XMLFile->LoadFromFile(asFileWorkSpace)) {
		TXMLFile *XMLNodeWorkSpace;
		TXMLFile *XMLNode;
		AnsiString asFileName;
		int FileType;
		int ActivePageIndex = 0;
		int i, j;


		if (XMLFile->Count > 0) {

			XMLNodeWorkSpace = XMLFile->Node[0];

			for (i = 0; i < XMLNodeWorkSpace->Count; i++) {
				XMLNode = XMLNodeWorkSpace->Node[i];
				if (XMLNode->Name == _T("openfile")) {
					FileType = XMLNode->GetAttribute(_T("type")).ToIntDef(0);
					asFileName = XMLNode->GetAttribute(_T("file"));
					switch (FileType) {
						case ftf_TextFile:
							if (OpenTextFile(asFileName)) {
								j = (int) TabSheets.size() - 1;
								Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[j].Content);
								if (Frame_EditFichierText) {
									int FirstLine, FirstCol;
									int NumLine, NumCol;

									FirstLine = XMLNode->GetAttribute(_T("firstline")).ToIntDef(0);
									Frame_EditFichierText->TextEditor->FirstLine = FirstLine;
									FirstCol = XMLNode->GetAttribute(_T("firstcol")).ToIntDef(0);
									Frame_EditFichierText->TextEditor->FirstCol = FirstCol;
									NumLine = XMLNode->GetAttribute(_T("numline")).ToIntDef(0);
									Frame_EditFichierText->TextEditor->NumLine = NumLine;
									NumCol = XMLNode->GetAttribute(_T("numcol")).ToIntDef(0);
									Frame_EditFichierText->TextEditor->NumCol = NumCol;
								}
							}
							break;
						case ftf_HexaFile:
							if (OpenBinFile(asFileName)) {
								j = (int) TabSheets.size() - 1;
								Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[j].Content);
								if (Frame_EditFichierHexa) {
									int FirstLine;
									int FilePos;

									FirstLine = XMLNode->GetAttribute(_T("firstline")).ToIntDef(0);
									Frame_EditFichierHexa->HexaEditor->FirstLine = FirstLine;
									FilePos = XMLNode->GetAttribute(_T("filepos")).ToIntDef(0);
									Frame_EditFichierHexa->HexaEditor->FilePos = FilePos;
								}
							}
							break;
						case ftf_Dialog:
							OpenDialog(asFileName);
							break;
						case ftf_Bmp:
							break;
						case ftf_Icon:
							OpenIcon(asFileName);
							break;
						case ftf_Cursor:
							OpenCursor(asFileName);
							break;
					}
				}
				else if (XMLNode->Name == _T("activepageindex")) {
					ActivePageIndex = ((AnsiString) XMLNode->Text).ToIntDef(0);
				}
			}
			if (ActivePageIndex >= PageControl_Files->PageCount) {  // Certains fichiers ont pu être supprimés donc l'onglet fermé
				ActivePageIndex = PageControl_Files->PageCount - 1;
			}
			PageControl_Files->ActivePageIndex = ActivePageIndex;

			XMLNodeWorkSpace->SetAttribute(_T("file"), asFileWorkSpace);  // Au cas où le fichier aurait été déplacé manuellement
			Config->PushLastWorkSpace(asFileWorkSpace);
			UpdateTreeView(XMLFile);
			UpdateTreeViewStates();
			Caption = asFileWorkSpace;
			Ok = true;
		}
	}

	if (Ok) bTempWorkSpace = false;

	delete XMLFile;

	Screen->Cursor = crDefault;

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SaveWorkSpace(void) {
	TFrame_EditFichierText *Frame_EditFichierText;
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	TFrame_FVCLIDEBase *Frame_FVCLIDEBase;
  TXMLFile *XMLFile;
	TXMLFile *XMLNodeWorkSpace;
	TXMLFile *NewXMLNode;
	int UnFichierNonVide;
	bool PageSelectionneeVide;
	int i;


	if (TreeView_Projects->Items->Count == 0) return false;

	Screen->Cursor = crHourGlass;

	XMLFile = new TXMLFile(this);
	UpdateXMLTree(XMLFile);

	XMLNodeWorkSpace = XMLFile->Node[0];
	XMLNodeWorkSpace->SetAttribute(_T("file"), asFileWorkSpace);

	UnFichierNonVide = -1;
	PageSelectionneeVide = false;

	for (i = 0; i < (int) TabSheets.size(); i++) {
		Frame_FVCLIDEBase = TabSheets[i].Content;
		if (!((AnsiString) Frame_FVCLIDEBase->FileName).IsEmpty()) {
			NewXMLNode = XMLNodeWorkSpace->AddNode(_T("openfile"));
			NewXMLNode->SetAttribute(_T("type"), AnsiString((int) Frame_FVCLIDEBase->FileType));
			Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
			if (Frame_EditFichierText) {
				NewXMLNode->SetAttribute(_T("firstline"), AnsiString((int) Frame_EditFichierText->TextEditor->FirstLine));
				NewXMLNode->SetAttribute(_T("firstcol"), AnsiString((int) Frame_EditFichierText->TextEditor->FirstCol));
				NewXMLNode->SetAttribute(_T("numline"), AnsiString((int) Frame_EditFichierText->TextEditor->NumLine));
				NewXMLNode->SetAttribute(_T("numcol"), AnsiString((int) Frame_EditFichierText->TextEditor->NumCol));
			}
			Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
			if (Frame_EditFichierHexa) {
				NewXMLNode->SetAttribute(_T("firstline"), AnsiString((int) Frame_EditFichierHexa->HexaEditor->FirstLine));
				NewXMLNode->SetAttribute(_T("filepos"), AnsiString((int) Frame_EditFichierHexa->HexaEditor->FilePos));
			}
			NewXMLNode->SetAttribute(_T("file"), Frame_FVCLIDEBase->FileName);
			NewXMLNode->Text = Frame_FVCLIDEBase->DisplayedFileName;
			UnFichierNonVide = i;
		}
		else {
			// Nouveau fichier non sauvegardé sans doute
			if (i == PageControl_Files->ActivePageIndex) {
				// Page sélectionnée => il va falloir changer la page courante
				PageSelectionneeVide = true;
			}
		}
	}
	NewXMLNode = XMLNodeWorkSpace->AddNode(_T("activepageindex"));
	NewXMLNode->Text = AnsiString(PageSelectionneeVide? UnFichierNonVide: (int) PageControl_Files->ActivePageIndex);

	XMLFile->SaveToFile(asFileWorkSpace);
	Caption = asFileWorkSpace;

	// Si le fichier projet est aussi ouvert en tant que texte => on met à jour le fichier
	for (i = (int) TabSheets.size() - 1; i >= 0; i--) {
		TFrame_FVCLIDEBase *Frame_FVCLIDEBase = TabSheets[i].Content;
		if (Frame_FVCLIDEBase->FileName == asFileWorkSpace &&
				Frame_FVCLIDEBase->Changed == false) {
			if (Frame_FVCLIDEBase->OpenFile(asFileWorkSpace)) {
				Frame_FVCLIDEBase->MemoDateFile();
			}
		}
	}

	bTempWorkSpace = false;

	delete XMLFile;

	Screen->Cursor = crDefault;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SaveWorkSpaceIfNotEmpty(void) {

	if (TreeView_Projects->Items->Count > 1) {
		if (asFileWorkSpace.IsEmpty()) Action_SaveAsWorkSpaceExecute(this);
		else SaveWorkSpace();
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::CloseWorkSpace(void) {
	TProject *Project;
	TTreeNode *TreeNode;
	int i;


	for (i = (int) TabSheets.size() - 1; i >= 0; i--) {
		delete (TTabSheet *) (PageControl_Files->Pages[i]);
		TabSheets.erase(TabSheets.begin() + i);
	}

	for (i = 0; i < (int) TreeView_Projects->Items->Count; i++) {
		TreeNode = TreeView_Projects->Items->Item[i];
		Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
		delete Project;
	}
	TreeView_Projects->Items->Clear();

	asFileWorkSpace = _T("");
	Caption = _TT("Environnement de Développement Intégré Free VCL");

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddNewFile(TTreeNode *NodeParent) {
	TForm_AddNewFile *Form_AddNewFile;
	TProject *ProjectParent;
	bool Ok = false;


	ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);
	if (ProjectParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

	if (ProjectParent->Can_AddFile()) {

		Form_AddNewFile = new TForm_AddNewFile(this, _T("TForm_AddNewFile"));
		if (Form_AddNewFile->ShowModal() == mrOk) {
			AnsiString asName;
			AnsiString asPath;
			AnsiString asSrcFileName;
			AnsiString asDestFileName;
			int FileType;

			asName = Form_AddNewFile->Edit_Name->Text;
			asPath = Form_AddNewFile->Edit_Path->Text;
			FileType = Form_AddNewFile->ListBox_Types->ItemIndex;

			switch (FileType) {
			case 0:
				// Fichier header (.h)")
				asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Header.h");
				asDestFileName = asPath + DIR_DELIMITER + asName + _T(".h");
				Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenTextFile(asDestFileName);
				break;
			case 1:
				// Fichier source C (.c)")
				asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Source.c");
				asDestFileName = asPath + DIR_DELIMITER + asName + _T(".c");
				Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenTextFile(asDestFileName);
				break;
			case 2:
				// Fichier source C++ (.cpp)")
				asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Source.cpp");
				asDestFileName = asPath + DIR_DELIMITER + asName + _T(".cpp");
				Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenTextFile(asDestFileName);
				break;
			case 3:
				// Définition de classe (.h et .cpp)")
				asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Class.h");
				asDestFileName = asPath + DIR_DELIMITER + asName + _T(".h");
				Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenTextFile(asDestFileName);
				if (Ok) {
					asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Class.cpp");
					asDestFileName = asPath + DIR_DELIMITER + asName + _T(".cpp");
					Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				}
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenTextFile(asDestFileName);
				break;
			case 4:
				// Nouvelle unité (.h, .cpp et .dfm)")
				asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Unit.h");
				asDestFileName = asPath + DIR_DELIMITER + asName + _T(".h");
				Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenTextFile(asDestFileName);
				if (Ok) {
					asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Unit.cpp");
					asDestFileName = asPath + DIR_DELIMITER + asName + _T(".cpp");
					Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				}
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenTextFile(asDestFileName);
				if (Ok) {
					asSrcFileName = (AnsiString) Config->InstallPath + _T("\\Models\\Files\\Unit.dfm");
					asDestFileName = asPath + DIR_DELIMITER + asName + _T(".dfm");
					Ok = CopyReplaceFile(asSrcFileName, asDestFileName, asName);
				}
				if (Ok) Ok = ProjectParent->AddFile(asDestFileName);
				if (Ok) OpenDialog(asDestFileName);
				break;
			}

		}
		delete Form_AddNewFile;

	}
	else if (!((AnsiString) ProjectParent->FileName).IsEmpty()) {
		AnsiString asProjectFile = ProjectParent->FileName;
		ShellExecute(FHandle, _T("open"), asProjectFile, _T(""),
									ExtractFilePath(asProjectFile), SW_SHOWNORMAL);
		Ok = true;
	}
	else {
		Ok = ProjectParent->Open();
	}

	if (Ok) {

		// Mise à jour des dépendances
		RemoveDependencies(NodeParent);
		InsertDependencies(NodeParent);

		UpdateTreeViewStates();

	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TForm_Principale::CopyReplaceFile(AnsiString asSrcFileName, AnsiString asDestFileName, AnsiString asName) {
	TFileASCII *FileASCII;
  HANDLE hFile;
	DWORD dw;
	AnsiString asLine;
	TReplaceFlags ReplaceFlags;
	bool Ok;


	if (FileExists(asDestFileName)) {
		AnsiString asMsg;
		asMsg = _TT("Ce fichier existe déjà, voulez-vous l'écraser ?\n") + asDestFileName;
		if (DisplayError(asMsg,
										 _TT("Enregistrement fichier"),
										 _T("OVERWRITE_FILE"),
										 MB_YESNOCANCEL | MB_ICONQUESTION) != IDYES) return false;
	}

  FileASCII = new TFileASCII();
  FileASCII->ReadOnly = true;
  Ok = FileASCII->OpenFile(asSrcFileName);
  if (Ok) {

		hFile = CreateFile(asDestFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {

			ReplaceFlags.Clear() << rfReplaceAll;
			while (FileASCII->ReadLine(&asLine)) {
				asLine = StringReplace(asLine, "{{{FILE_NAME}}}", asName.AsCharString(), ReplaceFlags);
				WriteFile(hFile, (LPCSTR) asLine.AsCharString(), (int) asLine.Length(), &dw, NULL);
				WriteFile(hFile, "\r\n", 2, &dw, NULL);
			}

			CloseHandle(hFile);

		}

		else {

			AnsiString asErrMsg;

			// Message d'erreur ouverture asDestFileName
			asErrMsg.sprintf(_TT("Impossible d'ouvrir le fichier \"%s\""), (LPCTSTR) asDestFileName);
			DisplaySystemError(asErrMsg, GetLastError(), _TT("Erreur"), _T(""), MB_OK | MB_ICONWARNING);

		}

  }
	else {
		AnsiString asErrMsg;
		DWORD dwLastError = GetLastError();

		// Message d'erreur ouverture asSrcFileName
		asErrMsg.sprintf(_TT("Impossible d'ouvrir le fichier \"%s\""), (LPCTSTR) asSrcFileName);
		DisplaySystemError(asErrMsg, dwLastError, _TT("Erreur"), _T(""), MB_OK | MB_ICONWARNING);

	}

  FileASCII->CloseFile();
  delete FileASCII;

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddExistingFile(const AnsiString &asNodeName, const AnsiString &asFileName) {
	TProject *ProjectParent;
	TTreeNode *NodeParent;
	bool Ok = false;


	NodeParent = TreeViewFindNode(asNodeName);
	if (NodeParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: Élément \"%s\" non trouvé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

	ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);
	if (ProjectParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

	if (ProjectParent->Can_AddFile()) {
		Ok = ProjectParent->AddFile(asFileName);
	}
	else {
		Ok = ProjectParent->Open();
	}

	if (Ok) {

		// Mise à jour des dépendances
		RemoveDependencies(NodeParent);
		InsertDependencies(NodeParent);

	}

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddExistingFile(TTreeNode *NodeParent) {
	TProject *ProjectParent;
	bool Ok = false;


	ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);
	if (ProjectParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

	if (ProjectParent->Can_AddFile()) {
		int i;

		OpenDialog_File->FilterIndex = 1;
		if (OpenDialog_File->Execute()) {
			for (i = 0; i < OpenDialog_File->Files->Count; i++) {
				ProjectParent->AddFile(OpenDialog_File->Files->Strings[i]);
			}
			Ok = true;
		}

	}
	else {

		Ok = ProjectParent->Open();

	}

	if (Ok) {

		// Mise à jour des dépendances
		RemoveDependencies(NodeParent);
		InsertDependencies(NodeParent);

	}

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddNewProject(TTreeNode *NodeParent) {
  TForm_AddNewProject *Form_AddNewProject;
	TProject *ProjectParent;


	ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);
	if (ProjectParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

  Form_AddNewProject = new TForm_AddNewProject(this, _T("TForm_AddNewProject"));
  if (Form_AddNewProject->ShowModal() == mrOk) {
		AnsiString asNameProject;
		AnsiString asProjectType;
		int IndexTypeProject;
		TProject *ProjectChild;
		TTreeNode *NodeChild;


		asNameProject = Form_AddNewProject->Edit_Name->Text;
		asProjectType = Form_AddNewProject->TypeProject;

		// Création de l'objet projet
		ProjectChild = new TProject(ListTypeProjects);
		ProjectChild->TypeProject = asProjectType;
		ProjectChild->Name = asNameProject;
		AddBitmapToImageList(ProjectChild);
		IndexTypeProject = ProjectChild->IndexTypeProject;

		// Insertion dans le TreeView
		NodeChild = TreeViewAddNodeProject(NodeParent, asNameProject);
		NodeChild->StateIndex = 0;
		NodeChild->ImageIndex = IndexTypeProject + DECAL_TYPES_PROJECT;
		NodeChild->SelectedIndex = IndexTypeProject + DECAL_TYPES_PROJECT;
		NodeChild->Data = ProjectChild;
		NodeParent->Expand(false);

		if (ProjectChild->Create()) {

			// Insertion des dépendances
			InsertDependencies(NodeChild);

		}

		UpdateTreeViewStates();

  }
  delete Form_AddNewProject;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddExistingProject(const AnsiString &asNodeName, const AnsiString &asProjectFileName, const AnsiString & asProjectName, const AnsiString &asProjectType) {
	TProject *ProjectParent;
	TTreeNode *NodeParent;


	NodeParent = TreeViewFindNode(asNodeName);
	if (NodeParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: Élément \"%s\" non trouvé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

	ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);
	if (ProjectParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

	return AddProject(NodeParent, ProjectParent, asProjectFileName, asProjectName, asProjectType);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddExistingProject(TTreeNode *NodeParent) {
  TForm_AddNewProject *Form_AddNewProject;
	TProject *ProjectParent;


	ProjectParent = reinterpret_cast<TProject *>((void *) NodeParent->Data);
	if (ProjectParent == NULL) {
		AnsiString asNodeName, asMessage;
		asNodeName = NodeParent->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
									_TT("Opération impossible"),
									_T(""),
									MB_OK | MB_ICONWARNING);
		return false;
	}

	OpenDialog_Project->FileName = _T("");
	OpenDialog_Project->Title = _TT("Nom du fichier projet à ouvrir");
	if (OpenDialog_Project->Execute()) {

		AnsiString asProjectFileName = OpenDialog_Project->FileName;

		Form_AddNewProject = new TForm_AddNewProject(this, _T("TForm_AddNewProject"));
		if (Form_AddNewProject->ShowModal() == mrOk) {
			AnsiString asProjectName;
			AnsiString asProjectType;

			asProjectName = Form_AddNewProject->Edit_Name->Text;
			asProjectType = Form_AddNewProject->TypeProject;

			AddProject(NodeParent, ProjectParent, asProjectFileName, asProjectName, asProjectType);

		}
		delete Form_AddNewProject;

		return true;

	}

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddProject(TTreeNode *NodeParent, TProject *ProjectParent, AnsiString asProjectFileName, AnsiString asProjectName, AnsiString asProjectType) {
	TProject *ProjectChild;
	TTreeNode *NodeChild;
	int IndexTypeProject;
	TDateTime dtLastChange;


	// Création de l'objet projet
	ProjectChild = new TProject(ListTypeProjects);
	ProjectChild->TypeProject = asProjectType;
	ProjectChild->FileName = asProjectFileName;
	ProjectChild->Name = asProjectName;
	AddBitmapToImageList(ProjectChild);
	IndexTypeProject = ProjectChild->IndexTypeProject;

	// Insertion dans le TreeView
	NodeChild = TreeViewAddNodeProject(NodeParent, asProjectName);
	NodeChild->StateIndex = 0;
	NodeChild->ImageIndex = IndexTypeProject + DECAL_TYPES_PROJECT;
	NodeChild->SelectedIndex = IndexTypeProject + DECAL_TYPES_PROJECT;
	NodeChild->Data = ProjectChild;
	NodeParent->Expand(false);

	// Insertion des dépendances
	InsertDependencies(NodeChild);

	// Dernière modification du projet
	dtLastChange.CurrentDateTime(false);
	ProjectParent->LastChange = dtLastChange;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::OpenFile(const AnsiString &asFileName) {
	AnsiString asExt = ExtractFileExt(asFileName).LowerCase();
	if (asExt == _T(".dfm")) {
		return OpenDialog(asFileName);
	}
	else if (asExt == _T(".ico")) {
		return OpenIcon(asFileName);
	}
	else if (asExt == _T(".cur")) {
		return OpenCursor(asFileName);
	}
	else if (IsAsciiFile(asFileName)) {
		return OpenTextFile(asFileName);
	}
	else {
		return OpenBinFile(asFileName);
	}
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::OpenTextFile(const AnsiString &asFileName) {
	TABSHEET NewTabSheet;
	TFrame_EditFichierText *Frame_EditFichierText;
	AnsiString asLongFileName;
	AnsiString asName;
	bool Ok = true;
	int i;


	// Nom long
	TCHAR * szLongFileName = new TCHAR[32767];
	if (GetLongPathName(asFileName, szLongFileName, 32767)) {
		asLongFileName = szLongFileName;
	}
	delete[] szLongFileName;
	if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

	// Recherche si le fichier n'est pas déjà ouvert
	i = GetFileIndex(asLongFileName, ftf_TextFile);
	if (i != -1) {
		PageControl_Files->ActivePageIndex = i;
		return true;
	}

	Screen->Cursor = crHourGlass;

	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->PageControl = PageControl_Files;
	NewTabSheet.TabSheet->ShowHint = true;
	NewTabSheet.TabSheet->Hint = asLongFileName;

	Frame_EditFichierText = new TFrame_EditFichierText(NewTabSheet.TabSheet);
	Frame_EditFichierText->Name = _T("Frame_EditFichierText");
	Frame_EditFichierText->Align = alClient;
	if (!Frame_EditFichierText->OpenFile(asLongFileName)) Ok = false;

	if (Ok) {

		Frame_EditFichierText->MemoDateFile();
		Frame_EditFichierText->Parent = NewTabSheet.TabSheet;
		SET_EVENT(Frame_EditFichierText, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);
		SET_EVENT(Frame_EditFichierText, TNotifyEvent, OnPassBegin, TForm_Principale, this, ObjectPassBegin);
		SET_EVENT(Frame_EditFichierText, TNotifyEvent, OnPassEnd, TForm_Principale, this, ObjectPassEnd);
		SET_EVENT(Frame_EditFichierText, TOnAutocompletionOn, OnAutocompletionOn, TForm_Principale, this, ObjectAutocompletionOn);
		SET_EVENT(Frame_EditFichierText, TOnAutocompletionOff, OnAutocompletionOff, TForm_Principale, this, ObjectAutocompletionOff);
		SET_EVENT(Frame_EditFichierText->TextEditor->Tooltip, TOnHint, OnHint, TForm_Principale, this, TextEditorHint);

		NewTabSheet.Content = Frame_EditFichierText;
		NewTabSheet.TabSheet->Caption = Frame_EditFichierText->DisplayedFileName;
		TabSheets.push_back(NewTabSheet);

		// Positionnement des marque-pages
		for (i = 0; i < (int) ListBox_Bookmarks->Count; i++) {
			AnsiString asFileNameBookmark;
			AnsiString asLine;

			bool bTextFile;
			int NumCol, NumLine;

			bool bHexaFile;
			__int64 FilePos;


			asLine = ListBox_Bookmarks->Items->Strings[i];
			if (DecodeLineBookmark(asLine, &asFileNameBookmark, &bTextFile,  &NumCol,  &NumLine, &bHexaFile,  &FilePos)) {
				if (bTextFile) {
					if (asFileNameBookmark == asFileName) {
						Frame_EditFichierText->TextEditor->SetFlag(NumLine, lfBookmark, true);
					}
				}
			}

		}

		PageControl_Files->ActivePage = NewTabSheet.TabSheet;
		Frame_EditFichierText->SetFocusChild();

		Config->PushLastFile(asLongFileName);

	}
	else {

		delete Frame_EditFichierText;
		delete NewTabSheet.TabSheet;

	}

	Screen->Cursor = crDefault;

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::OpenBinFile(const AnsiString &asFileName) {
	TABSHEET NewTabSheet;
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	AnsiString asLongFileName;
	AnsiString asName;
	bool Ok = true;
	int i;


	// Nom long
	TCHAR * szLongFileName = new TCHAR[32767];
	if (GetLongPathName(asFileName, szLongFileName, 32767)) {
		asLongFileName = szLongFileName;
	}
	delete[] szLongFileName;
	if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

	// Recherche si le fichier n'est pas déjà ouvert
	i = GetFileIndex(asLongFileName, ftf_HexaFile);
	if (i != -1) {
		PageControl_Files->ActivePageIndex = i;
		return true;
	}

	Screen->Cursor = crHourGlass;

	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->PageControl = PageControl_Files;
	NewTabSheet.TabSheet->ShowHint = true;
	NewTabSheet.TabSheet->Hint = asLongFileName;

	Frame_EditFichierHexa = new TFrame_EditFichierHexa(NewTabSheet.TabSheet);
	Frame_EditFichierHexa->Name = _T("Frame_EditFichierHexa");
	Frame_EditFichierHexa->Align = alClient;
	if (!Frame_EditFichierHexa->OpenFile(asLongFileName)) Ok = false;

	if (Ok) {

		Frame_EditFichierHexa->MemoDateFile();
		Frame_EditFichierHexa->Parent = NewTabSheet.TabSheet;
		SET_EVENT(Frame_EditFichierHexa, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);
		SET_EVENT(Frame_EditFichierHexa, TNotifyEvent, OnPassBegin, TForm_Principale, this, ObjectPassBegin);
		SET_EVENT(Frame_EditFichierHexa, TNotifyEvent, OnPassEnd, TForm_Principale, this, ObjectPassEnd);

		NewTabSheet.Content = Frame_EditFichierHexa;
		NewTabSheet.TabSheet->Caption = Frame_EditFichierHexa->DisplayedFileName;
		TabSheets.push_back(NewTabSheet);

		// Positionnement des marque-pages
		for (i = 0; i < (int) ListBox_Bookmarks->Count; i++) {
			AnsiString asFileNameBookmark;
			AnsiString asLine;

			bool bTextFile;
			int NumCol, NumLine;

			bool bHexaFile;
			__int64 FilePos;


			asLine = ListBox_Bookmarks->Items->Strings[i];
			if (DecodeLineBookmark(asLine, &asFileNameBookmark, &bTextFile,  &NumCol,  &NumLine, &bHexaFile,  &FilePos)) {
				if (bHexaFile) {
					if (asFileNameBookmark == asFileName) {
						Frame_EditFichierHexa->HexaEditor->SetFlag(FilePos, lfBookmark, true);
					}
				}
			}

		}

		PageControl_Files->ActivePage = NewTabSheet.TabSheet;
		Frame_EditFichierHexa->SetFocusChild();

		Config->PushLastFile(asLongFileName);

	}
	else {

		delete Frame_EditFichierHexa;
		delete NewTabSheet.TabSheet;

	}

	Screen->Cursor = crDefault;

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::OpenDialog(const AnsiString &asFileName) {
	TABSHEET NewTabSheet;
	TFrame_SaisieDialog *Frame_SaisieDialog;
	AnsiString asLongFileName;
	AnsiString asName;
	bool bTrouve = false;
	bool Ok = true;
	int i;


	// Nom long
	TCHAR * szLongFileName = new TCHAR[32767];
	if (GetLongPathName(asFileName, szLongFileName, 32767)) {
		asLongFileName = szLongFileName;
	}
	delete[] szLongFileName;
	if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

	// Recherche si le fichier n'est pas déjà ouvert
	i = GetFileIndex(asLongFileName, ftf_Dialog);
	if (i != -1) {
		PageControl_Files->ActivePageIndex = i;
		return true;
	}

	Screen->Cursor = crHourGlass;

	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->PageControl = PageControl_Files;
	NewTabSheet.TabSheet->ShowHint = true;
	NewTabSheet.TabSheet->Hint = asLongFileName;

	Frame_SaisieDialog = new TFrame_SaisieDialog(NewTabSheet.TabSheet);
	Frame_SaisieDialog->Name = _T("Frame_SaisieDialog");
	Frame_SaisieDialog->Align = alClient;
	if (!Frame_SaisieDialog->OpenFile(asLongFileName)) Ok = false;

	if (Ok) {

		Frame_SaisieDialog->MemoDateFile();
		Frame_SaisieDialog->Parent = NewTabSheet.TabSheet;
		SET_EVENT(Frame_SaisieDialog, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);

		NewTabSheet.Content = Frame_SaisieDialog;
		NewTabSheet.TabSheet->Caption = Frame_SaisieDialog->DisplayedFileName;
		TabSheets.push_back(NewTabSheet);

		PageControl_Files->ActivePage = NewTabSheet.TabSheet;
		Frame_SaisieDialog->SetFocusChild();

		Config->PushLastDialog(asLongFileName);

	}
	else {

		delete Frame_SaisieDialog;
		delete NewTabSheet.TabSheet;

	}

	Screen->Cursor = crDefault;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::OpenIcon(const AnsiString &asFileName) {
	TABSHEET NewTabSheet;
	TFrame_SaisieIcon *Frame_SaisieIcon;
	AnsiString asLongFileName;
	AnsiString asName;
	bool bTrouve = false;
	bool Ok = true;
	int i;


	// Nom long
	TCHAR * szLongFileName = new TCHAR[32767];
	if (GetLongPathName(asFileName, szLongFileName, 32767)) {
		asLongFileName = szLongFileName;
	}
	delete[] szLongFileName;
	if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

	// Recherche si le fichier n'est pas déjà ouvert
	i = GetFileIndex(asLongFileName, ftf_Icon);
	if (i != -1) {
		PageControl_Files->ActivePageIndex = i;
		return true;
	}

	Screen->Cursor = crHourGlass;

	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->PageControl = PageControl_Files;
	NewTabSheet.TabSheet->ShowHint = true;
	NewTabSheet.TabSheet->Hint = asLongFileName;

	Frame_SaisieIcon = new TFrame_SaisieIcon(NewTabSheet.TabSheet);
	Frame_SaisieIcon->Name = _T("Frame_SaisieIcon");
	Frame_SaisieIcon->Align = alClient;
	if (!Frame_SaisieIcon->OpenFile(asLongFileName)) Ok = false;

	if (Ok) {

		Frame_SaisieIcon->MemoDateFile();
		Frame_SaisieIcon->Parent = NewTabSheet.TabSheet;
		SET_EVENT(Frame_SaisieIcon, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);

		NewTabSheet.Content = Frame_SaisieIcon;
		NewTabSheet.TabSheet->Caption = Frame_SaisieIcon->DisplayedFileName;
		TabSheets.push_back(NewTabSheet);

		PageControl_Files->ActivePage = NewTabSheet.TabSheet;
		Frame_SaisieIcon->SetFocusChild();

		Config->PushLastGraphic(asLongFileName);

	}
	else {

		delete Frame_SaisieIcon;
		delete NewTabSheet.TabSheet;

	}

	Screen->Cursor = crDefault;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::OpenCursor(const AnsiString &asFileName) {
	TABSHEET NewTabSheet;
	TFrame_SaisieCursor *Frame_SaisieCursor;
	AnsiString asLongFileName;
	AnsiString asName;
	bool bTrouve = false;
	bool Ok = true;
	int i;


	// Nom long
	TCHAR * szLongFileName = new TCHAR[32767];
	if (GetLongPathName(asFileName, szLongFileName, 32767)) {
		asLongFileName = szLongFileName;
	}
	delete[] szLongFileName;
	if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

	// Recherche si le fichier n'est pas déjà ouvert
	i = GetFileIndex(asLongFileName, ftf_Cursor);
	if (i != -1) {
		PageControl_Files->ActivePageIndex = i;
		return true;
	}

	Screen->Cursor = crHourGlass;

	NewTabSheet.TabSheet = new TTabSheet(PageControl_Files);
	asName.sprintf(_T("TabSheet_%i"), ++NbTabSheet);
	NewTabSheet.TabSheet->Name = asName;
	NewTabSheet.TabSheet->PageControl = PageControl_Files;
	NewTabSheet.TabSheet->ShowHint = true;
	NewTabSheet.TabSheet->Hint = asLongFileName;

	Frame_SaisieCursor = new TFrame_SaisieCursor(NewTabSheet.TabSheet);
	Frame_SaisieCursor->Name = _T("Frame_SaisieCursor");
	Frame_SaisieCursor->Align = alClient;
	if (!Frame_SaisieCursor->OpenFile(asLongFileName)) Ok = false;

	if (Ok) {

		Frame_SaisieCursor->MemoDateFile();
		Frame_SaisieCursor->Parent = NewTabSheet.TabSheet;
		SET_EVENT(Frame_SaisieCursor, TOnFlagChanged, OnFlagChanged, TForm_Principale, this, ObjectFlagChanged);

		NewTabSheet.Content = Frame_SaisieCursor;
		NewTabSheet.TabSheet->Caption = Frame_SaisieCursor->DisplayedFileName;
		TabSheets.push_back(NewTabSheet);

		PageControl_Files->ActivePage = NewTabSheet.TabSheet;
		Frame_SaisieCursor->SetFocusChild();

		Config->PushLastGraphic(asLongFileName);

	}
	else {

		delete Frame_SaisieCursor;
		delete NewTabSheet.TabSheet;

	}

	Screen->Cursor = crDefault;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SaveFile(int Index) {
	TFrame_FVCLIDEBase *Frame_FVCLIDEBase;
	bool Ok = false;


	Frame_FVCLIDEBase = TabSheets[Index].Content;
	if (Frame_FVCLIDEBase->Changed) {
		if (((AnsiString) Frame_FVCLIDEBase->FileName).IsEmpty()) {
			SaveDialog_File->FileName = _T("");
			if (SaveDialog_File->Execute()) {
				if (Frame_FVCLIDEBase->SaveToFile(SaveDialog_File->FileName)) {
					Frame_FVCLIDEBase->MemoDateFile();
					Frame_FVCLIDEBase->Changed = false;
					Ok = true;
				}
			}
		}
		else {
			if (Frame_FVCLIDEBase->Save()) {
				Frame_FVCLIDEBase->MemoDateFile();
				Frame_FVCLIDEBase->Changed = false;
				Ok = true;
			}
		}
	}

	return Ok;
}

/*
//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetNbTextFiles(void) {
	TFrame_EditFichierText *Frame_EditFichierText;
	int NbTextFile = 0;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) NbTextFile++;
	}

	return NbTextFile;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::GetListTextFiles(TStrings *ListTextFiles) {
	TFrame_EditFichierText *Frame_EditFichierText;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) ListTextFiles->Add(Frame_EditFichierText->FileName);
	}
}

//---------------------------------------------------------------------------
TTextEditor * FASTCALL TForm_Principale::GetTextFile(int Index) {
	TFrame_EditFichierText *Frame_EditFichierText;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) {
			if (Index-- == 0) return Frame_EditFichierText->TextEditor;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL TForm_Principale::GetTextFileName(int Index) {
	TFrame_EditFichierText *Frame_EditFichierText;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) {
			if (Index-- == 0) return Frame_EditFichierText->FileName;
		}
	}

	return _T("");
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetCurTextFileIndex(void) {
	TFrame_EditFichierText *Frame_EditFichierText;
	int Index = 0;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) {
			if (i == PageControl_Files->ActivePageIndex) return Index;
			Index++;
		}
	}

	return -1;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::ActivateTextFile(int Index) {
	TFrame_EditFichierText *Frame_EditFichierText;
	int Index2 = 0;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
		if (Frame_EditFichierText) {
			if (Index2++ == Index) {
				PageControl_Files->ActivePageIndex = i;
				return true;
			}
		}
	}

	return false;
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetNbHexaFiles(void) {
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	int NbHexaFile = 0;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
		if (Frame_EditFichierHexa) NbHexaFile++;
	}

	return NbHexaFile;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::GetListHexaFiles(TStrings *ListHexaFiles) {
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
		if (Frame_EditFichierHexa) ListHexaFiles->Add(Frame_EditFichierHexa->FileName);
	}
}

//---------------------------------------------------------------------------
THexaEditor * FASTCALL TForm_Principale::GetHexaFile(int Index) {
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
		if (Frame_EditFichierHexa) {
			if (Index-- == 0) return Frame_EditFichierHexa->HexaEditor;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL TForm_Principale::GetHexaFileName(int Index) {
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
		if (Frame_EditFichierHexa) {
			if (Index-- == 0) return Frame_EditFichierHexa->FileName;
		}
	}

	return _T("");
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetCurHexaFileIndex(void) {
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	int Index = 0;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
		if (Frame_EditFichierHexa) {
			if (i == PageControl_Files->ActivePageIndex) return Index;
			Index++;
		}
	}

	return -1;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::ActivateHexaFile(int Index) {
	TFrame_EditFichierHexa *Frame_EditFichierHexa;
	int Index2 = 0;
	int i;


	for (i = 0; i < PageControl_Files->PageCount; i++) {
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[i].Content);
		if (Frame_EditFichierHexa) {
			if (Index2++ == Index) {
				PageControl_Files->ActivePageIndex = i;
				return true;
			}
		}
	}

	return false;
}
*/

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetNbFiles(void) {
	return (int) TabSheets.size();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::GetListFiles(TStrings *ListFiles) {
	int i;


	for (i = 0; i < (int) TabSheets.size(); i++) {
		ListFiles->Add(TabSheets[i].Content->FileName);
	}
}

//---------------------------------------------------------------------------
TFVCLIDEFileType FASTCALL TForm_Principale::GetFileType(int Index) {

	if (Index < 0 || (int) TabSheets.size() <= Index) return ftf_Other;

	return TabSheets[Index].Content->FileType;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL TForm_Principale::GetFileName(int Index) {

	if (Index < 0 || (int) TabSheets.size() <= Index) return _T("");

	return TabSheets[Index].Content->FileName;
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetFileIndex(const AnsiString asFileName, TFVCLIDEFileType FileType) {
	int Index;


	// Recherche de l'index du fichier
	for (Index = 0; Index < (int) TabSheets.size(); Index++) {
		if (FileType == TabSheets[Index].Content->FileType &&
				FileNameStandardFormat(asFileName) == FileNameStandardFormat((AnsiString) TabSheets[Index].Content->FileName)) {
			return Index;
		}
	}

	return -1;
}

//---------------------------------------------------------------------------
TTextEditor * FASTCALL TForm_Principale::GetTextFile(int Index) {
	TFrame_EditFichierText *Frame_EditFichierText;


	if (Index < 0 || (int) TabSheets.size() <= Index) return NULL;

	Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
	if (Frame_EditFichierText) return Frame_EditFichierText->TextEditor;

	return NULL;
}

//---------------------------------------------------------------------------
THexaEditor * FASTCALL TForm_Principale::GetHexaFile(int Index) {
	TFrame_EditFichierHexa *Frame_EditFichierHexa;


	if (Index < 0 || (int) TabSheets.size() <= Index) return NULL;

	Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
	if (Frame_EditFichierHexa) return Frame_EditFichierHexa->HexaEditor;

	return NULL;
}

//---------------------------------------------------------------------------
TTextEditor * FASTCALL TForm_Principale::GetCurTextFile(void) {
	return GetTextFile(PageControl_Files->ActivePageIndex);
}

//---------------------------------------------------------------------------
THexaEditor * FASTCALL TForm_Principale::GetCurHexaFile(void) {
	return GetHexaFile(PageControl_Files->ActivePageIndex);
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetCurFileIndex(void) {
	return PageControl_Files->ActivePageIndex;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::ActivateFile(int Index) {

	if (Index < 0 || (int) TabSheets.size() <= Index) return false;

	PageControl_Files->ActivePageIndex = Index;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SetFocusOnEditor(void) {
	PageControl_Files->SetFocusChild();

	return true;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL TForm_Principale::GetSelText(void) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) return Frame_EditFichierText->TextEditor->GetSelText();

		TFrame_EditFichierHexa *Frame_EditFichierHexa;
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
		if (Frame_EditFichierHexa) return Frame_EditFichierHexa->HexaEditor->GetSelText();

	}

	return _T("");
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SetSelText(AnsiString asText) {
	int Index;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			Frame_EditFichierText->TextEditor->SelText = asText;
			return true;
		}

	}

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddBookmark(void) {
	int Index;
	int NumLine, NumCol;
	__int64 FilePos;
	AnsiString asFileName;
	AnsiString asLine;
	AnsiString asBuf;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			asFileName = Frame_EditFichierText->FileName;
			NumLine = Frame_EditFichierText->TextEditor->NumLine;
			NumCol = Frame_EditFichierText->TextEditor->NumCol;
			if (Frame_EditFichierText->TextEditor->GetFlag(NumLine, lfBookmark) == false) {
				asBuf = Frame_EditFichierText->TextEditor->Lines_Strings(NumLine);
				asLine.sprintf(_T("  %s (%i, %i): %s"),
											 (LPCTSTR) asFileName, NumLine + 1, NumCol + 1, (LPCTSTR) asBuf);
				AddLogMessage(tl_Bookmarks, asLine);
				Frame_EditFichierText->TextEditor->SetFlag(NumLine, lfBookmark, true);

				return true;
			}
		}

		TFrame_EditFichierHexa *Frame_EditFichierHexa;
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
		if (Frame_EditFichierHexa) {
			asFileName = Frame_EditFichierHexa->FileName;
			FilePos = Frame_EditFichierHexa->HexaEditor->FilePos;
			if (Frame_EditFichierHexa->HexaEditor->GetFlag(FilePos, lfBookmark) == false) {
				asLine.sprintf(_T("  %s (%04X:%04X)"),
											 (LPCTSTR) asFileName, (int) ((FilePos >> 16) & 0xFFFF), (int) (FilePos & 0xFFFF));
				AddLogMessage(tl_Bookmarks, asLine);
				Frame_EditFichierHexa->HexaEditor->SetFlag(FilePos, lfBookmark, true);

				return true;
			}
		}

	}

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::DelBookmark(void) {
	int Index;
	int NumLine, NumCol;
	__int64 FilePos;
	AnsiString asFileName;
	AnsiString asLine;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {
			asFileName = Frame_EditFichierText->FileName;
			NumLine = Frame_EditFichierText->TextEditor->NumLine;
			NumCol = Frame_EditFichierText->TextEditor->NumCol;
			if (Frame_EditFichierText->TextEditor->GetFlag(NumLine, lfBookmark) == true) {
				asLine.sprintf(_T("  %s (%i,"), (LPCTSTR) asFileName, NumLine + 1);
				Frame_EditFichierText->TextEditor->SetFlag(NumLine, lfBookmark, false);
			}
		}

		TFrame_EditFichierHexa *Frame_EditFichierHexa;
		Frame_EditFichierHexa = dynamic_cast<TFrame_EditFichierHexa *>(TabSheets[Index].Content);
		if (Frame_EditFichierHexa) {
			asFileName = Frame_EditFichierHexa->FileName;
			FilePos = Frame_EditFichierHexa->HexaEditor->FilePos;
			if (Frame_EditFichierHexa->HexaEditor->GetFlag(FilePos, lfBookmark) == true) {
				asLine.sprintf(_T("  %s (%04X:%04X)"),
											 (LPCTSTR) asFileName, (int) ((FilePos >> 16) & 0xFFFF), (int) (FilePos & 0xFFFF));
				Frame_EditFichierHexa->HexaEditor->SetFlag(FilePos, lfBookmark, false);
			}
		}

		if (!asLine.IsEmpty()) {
			AnsiString asLine2;
			int i;
			int Len;
			int Count;

			Len = asLine.Length();
			Count = GetCountLogMessage(tl_Bookmarks);
			for (i = 0; i < Count; i++) {
				asLine2 = GetLogMessage(tl_Bookmarks, i);
				if (asLine2.Length() >= Len && asLine2.SubString(1, Len) == asLine) {
					DeleteLogMessage(tl_Bookmarks, i);
					break;
				}
			}

			return true;
		}

	}

	return false;
}


//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddBreakpoint(void) {
	int Index;
	int NumLine;
	AnsiString asFileName;
	AnsiString asLine;
	AnsiString asBuf;
	TTreeNode *TreeNode;
	TProject *Project;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {
		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);
		if (Frame_EditFichierText) {

			TreeNode = TreeView_Projects->Selected;
			if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
			if (TreeNode) {

				if (NodeToDebug(TreeNode, &Project)) {

					asFileName = Frame_EditFichierText->FileName;
					NumLine = Frame_EditFichierText->TextEditor->NumLine;
					if (Frame_EditFichierText->TextEditor->GetFlag(NumLine, lfBreakpoint) == false) {

						if (Project->AddBreakpoint(asFileName, NumLine, 0)) {
							asBuf = Frame_EditFichierText->TextEditor->Lines_Strings(NumLine);
							asLine.sprintf(_T("  %s (%i, %i): %s"),
															(LPCTSTR) asFileName, NumLine + 1, 1, asBuf);
							AddLogMessage(tl_Breakpoints, asLine);
							Frame_EditFichierText->TextEditor->SetFlag(NumLine, lfBreakpoint, true);
							return true;
						}

					}

				}
				else {
					AnsiString asMessage;
					AnsiString asProject;
					asProject = TreeNode->Text;
					asMessage.sprintf(_TT("Ni le projet \"%s\" ni aucun de ses sous-projets ne peut être débogué."), (LPCTSTR) asProject);
					DisplayError(asMessage,
											 _TT("Erreur point d'arrêt"),
											 _T("ERROR_BREAKPOINT"),
											 MB_OK | MB_ICONWARNING);
				}

			}

		}
		else {
			DisplayError(_TT("Impossible de positionner un point d'arrêt sur ce type de fichier"),
									 _TT("Erreur point d'arrêt"),
									 _T("ERROR_BREAKPOINT"),
									 MB_OK | MB_ICONWARNING);
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::DelBreakpoint(void) {
	int Index;
	int NumLine;
	AnsiString asFileName;
	AnsiString asLine;
	AnsiString asLine2;
	int i;
	int Len;
	int Count;
	TTreeNode *TreeNode;
	TProject *Project;


	Index = PageControl_Files->ActivePageIndex;
	if (Index >= 0) {

		TFrame_EditFichierText *Frame_EditFichierText;
		Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[Index].Content);

		if (Frame_EditFichierText) {

			asFileName = Frame_EditFichierText->FileName;
			NumLine = Frame_EditFichierText->TextEditor->NumLine;
			if (Frame_EditFichierText->TextEditor->GetFlag(NumLine, lfBreakpoint) == true) {

				TreeNode = TreeView_Projects->Selected;
				if (!TreeNode) TreeNode = TreeView_Projects->Items->Item[1];
				if (TreeNode) {

					if (NodeToDebug(TreeNode, &Project)) {

						// Ne pas tester le retour : le point d'arrêt doit pouvoir être détruit dans tous les cas
						Project->DelBreakpoint(asFileName, NumLine, 0);

						Len = asLine.Length();
						Count = GetCountLogMessage(tl_Breakpoints);
						for (i = 0; i < Count; i++) {
							asLine2 = GetLogMessage(tl_Breakpoints, i);
							if (asLine2.Length() >= Len && asLine2.SubString(1, Len) == asLine) {
								DeleteLogMessage(tl_Breakpoints, i);
								break;
							}
						}

						asLine.sprintf(_T("  %s (%i,"), (LPCTSTR) asFileName, NumLine + 1);
						Frame_EditFichierText->TextEditor->SetFlag(NumLine, lfBreakpoint, false);

						return true;

					}
				}

			}

		}
		else {
			DisplayError(_TT("Impossible de positionner un point d'arrêt sur ce type de fichier"),
									 _TT("Erreur point d'arrêt"),
									 _T("ERROR_BREAKPOINT"),
									 MB_OK | MB_ICONWARNING);
		}
	}

	return false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::DisplayMessage(TTypeDisplay TypeDisplay, const AnsiString &asMessage) {
	switch (TypeDisplay) {
		case td_Position:
			StatusBar->PanelText[0] = asMessage;
			break;
		case td_Message:
			StatusBar->PanelText[1] = asMessage;
			break;
	}
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::DisplayError(const AnsiString &asMessage, const AnsiString &asTitle, const AnsiString &asKey, int Flags) {
	return ::DisplayError(asMessage, asTitle, asKey, Flags);
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::DisplaySystemError(const AnsiString &asMessage, int SystemError, const AnsiString &asTitle, const AnsiString &asKey, int Flags) {
	return ::DisplaySystemError(asMessage, SystemError, asTitle, asKey, Flags);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ClearLogMessage(TTypeLog TypeLog) {

	PageControl_Logs->ActivePageIndex = (int) TypeLog;
	switch (TypeLog) {
		case tl_Search:
			ListBox_Search->Items->Clear();
			break;
		case tl_Bookmarks:
			ListBox_Bookmarks->Items->Clear();
			break;
		case tl_Build:
			ListBox_Build->Items->Clear();
			break;
		case tl_Process:
			ListBox_Process->Items->Clear();
			break;
		case tl_Breakpoints:
			ListBox_Breakpoints->Items->Clear();
			break;
		case tl_CallStack:
			ListBox_CallStack->Items->Clear();
			break;
	}

	Application->ProcessMessages();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::AddLogMessage(TTypeLog TypeLog, const AnsiString &asMessage) {
	int LineNumber;


	PageControl_Logs->ActivePageIndex = (int) TypeLog;
	switch (TypeLog) {
		case tl_Search:
			LineNumber = ListBox_Search->Items->Add(asMessage);
			ListBox_Search->TopIndex = LineNumber;
			break;
		case tl_Bookmarks:
			LineNumber = ListBox_Bookmarks->Items->Add(asMessage);
			ListBox_Bookmarks->TopIndex = LineNumber;
			break;
		case tl_Build:
			LineNumber = ListBox_Build->Items->Add(asMessage);
			ListBox_Build->TopIndex = LineNumber;
			break;
		case tl_Process:
			LineNumber = ListBox_Process->Items->Add(asMessage);
			ListBox_Process->TopIndex = LineNumber;
			break;
		case tl_Breakpoints:
			LineNumber = ListBox_Breakpoints->Items->Add(asMessage);
			ListBox_Breakpoints->TopIndex = LineNumber;
			break;
		case tl_CallStack:
			LineNumber = ListBox_CallStack->Items->Add(asMessage);
			ListBox_CallStack->TopIndex = LineNumber;
			break;
	}

	Application->ProcessMessages();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::DeleteLogMessage(TTypeLog TypeLog, int i) {

	switch (TypeLog) {
		case tl_Search:	return ListBox_Search->Items->Delete(i);
		case tl_Bookmarks: return ListBox_Bookmarks->Items->Delete(i);
		case tl_Build: return ListBox_Build->Items->Delete(i);
		case tl_Process: return ListBox_Process->Items->Delete(i);
		case tl_Breakpoints: return ListBox_Breakpoints->Items->Delete(i);
		case tl_CallStack: return ListBox_CallStack->Items->Delete(i);
	}

	Application->ProcessMessages();

}

//---------------------------------------------------------------------------
AnsiString FASTCALL TForm_Principale::GetLogMessage(TTypeLog TypeLog, int i) {

	switch (TypeLog) {
		case tl_Search:	return ListBox_Search->Items->Strings[i];
		case tl_Bookmarks: return ListBox_Bookmarks->Items->Strings[i];
		case tl_Build: return ListBox_Build->Items->Strings[i];
		case tl_Process: return ListBox_Process->Items->Strings[i];
		case tl_Breakpoints: return ListBox_Breakpoints->Items->Strings[i];
		case tl_CallStack: return ListBox_CallStack->Items->Strings[i];
	}

	return "";
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::GetCountLogMessage(TTypeLog TypeLog) {

	switch (TypeLog) {
		case tl_Search:	return ListBox_Search->Items->Count;
		case tl_Bookmarks: return ListBox_Bookmarks->Items->Count;
		case tl_Build: return ListBox_Build->Items->Count;
		case tl_Process: return ListBox_Process->Items->Count;
		case tl_Breakpoints: return ListBox_Breakpoints->Items->Count;
		case tl_CallStack: return ListBox_CallStack->Items->Count;
	}

	return 0;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddVariableDefinition(AnsiString asVarName, int TypeDefinition, AnsiString asDescription, AnsiString asTypeContenant, AnsiString asTypeVariable, AnsiString asFileName, int NumLine, int NumCol) {

	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Ajout d'une variable"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return false;
	}

	return CurrentProject->AddVariableDefinition(asVarName, TypeDefinition, asDescription, asTypeContenant, asTypeVariable, asFileName, NumLine, NumCol);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::FindDeclaration(AnsiString asVarName, AnsiString *asFileName, int *NumLine, int *NumCol) {

	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Déclaration d'une variable"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return false;
	}

	if (!CurrentProject->IsPreCompiled()) {
		Screen->Cursor = crHourGlass;
		CurrentProject->PreCompilation();
		Screen->Cursor = crDefault;
	}

	return CurrentProject->GetVariableDefinition(asVarName, 1, asFileName, NumLine, NumCol);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::FindImplementation(AnsiString asVarName, AnsiString *asFileName, int *NumLine, int *NumCol) {

	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Implémentation d'une variable"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return false;
	}

	if (!CurrentProject->IsPreCompiled()) {
		CurrentProject->PreCompilation();
	}

	return CurrentProject->GetVariableDefinition(asVarName, 2, asFileName, NumLine, NumCol);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::GetDescription(AnsiString asVarName, AnsiString *asDescription) {

	// Pas de message d'erreur, cette fonction est utilisée par l'info-bulle
	if (CurrentProject == NULL) return false;

	return CurrentProject->GetVariableDescription(asVarName, asDescription);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::GetTypeVariable(AnsiString asVarName, AnsiString *asTypeVariable) {

	// Pas de message d'erreur
	if (CurrentProject == NULL) return false;

	if (!CurrentProject->IsPreCompiled()) {
		CurrentProject->PreCompilation();
	}

	return CurrentProject->GetTypeVariable(asVarName, asTypeVariable);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::GetListAutocompletion(AnsiString asTypeContenant, TStrings *slList) {

	if (CurrentProject == NULL) {
		DisplayError(_TT("Aucun projet courant n'a été défini.\nVeuillez définir un projet courant dans la liste des projets."),
								 _TT("Implémentation d'une variable"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
		return false;
	}

	if (!CurrentProject->IsPreCompiled()) {
		CurrentProject->PreCompilation();
	}

	return CurrentProject->GetListAutocompletion(asTypeContenant, slList);
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::ReplaceDeclarationsFVCL(AnsiString asFileNameH, AnsiString asClassName, AnsiString asComponents, AnsiString asEvents) {
	TTextEditor *TextEditor;
	SEARCHPARAM SearchParam;
	int NumLineBeginComponents, NumLineEndComponents, NumLineBeginEvents, NumLineEndEvents;
  bool Ok;
  AnsiString asFileNameBak;
  CharString csLigne, csLigneSansEsp;
	CharString csClass;
  HANDLE hFile, hFile2;
  DWORD dw;
	bool bBeginClass;
	bool bBeginComponents;
	bool bBeginEvents;
	int OldActivePageIndex;
	int PosDeuxPoints;


	// Le ficher .h est supposé exister (création d'un fichier par défaut avant l'appel de cette fonction)

	// Fichier H
	// Si le fichier est déjà ouvert dans l'éditeur : on le modifie aussi dans l'éditeur
	// pour éviter de perdre les modifications éventuelles de l'utilisateur.
	// Sinon on modifie et on enregistre directement le fichier.
	if (GetFileIndex(asFileNameH, ftf_TextFile) != -1) {
		OldActivePageIndex = PageControl_Files->ActivePageIndex;
		if (OpenTextFile(asFileNameH)) {
			TextEditor = GetCurTextFile();
			// En principe, TextEditor ne devrait pas être NULL (sauf fichier H binaire ???)
			if (TextEditor) {

				SearchParam.Down = true;
				SearchParam.SinceCursor = false;
				SearchParam.WholeWord = true;
				SearchParam.CaseSensitive = true;
				SearchParam.BegLine = false;
				SearchParam.EndLine = false;
				SearchParam.OnlyInComments = false;
				SearchParam.NotInComments = true;
				SearchParam.OnlyInQuotes = false;
				SearchParam.NotInQuotes = true;
				SearchParam.RegExp = false;
				SearchParam.KeepCase = true;
				SearchParam.Loop = false;

				TextEditor->BeginOperation(_TT("Remplacement automatique des déclarations FVCL"));
				TextEditor->BeginUpdate();

				if (TextEditor->Search(_T("FVCL_BEGIN_COMPONENTS"), &SearchParam)) {
					NumLineBeginComponents = TextEditor->NumLine;
				}
				else {
					NumLineBeginComponents = -1;
				}
				if (TextEditor->Search(_T("FVCL_END_COMPONENTS"), &SearchParam)) {
					NumLineEndComponents = TextEditor->NumLine;
				}
				else {
					NumLineEndComponents = -1;
				}
				if (NumLineBeginComponents != -1 &&
						NumLineBeginComponents < NumLineEndComponents) {
					TextEditor->MoveCur(NumLineBeginComponents + 1, 0, false, false);
					TextEditor->MoveCur(NumLineEndComponents, 0, true, false);
					TextEditor->SetSelTextBuf(asComponents);
				}

				// Il faut faire le search de FVCL_XXXX_EVENTS après l'insertion des
				// composants (qui ont pu ajouter ou supprimer des lignes)
				if (TextEditor->Search(_T("FVCL_BEGIN_EVENTS"), &SearchParam)) {
					NumLineBeginEvents = TextEditor->NumLine;
				}
				else {
					NumLineBeginEvents = -1;
				}
				if (TextEditor->Search(_T("FVCL_END_EVENTS"), &SearchParam)) {
					NumLineEndEvents = TextEditor->NumLine;
				}
				else {
					NumLineEndEvents = -1;
				}
				if (NumLineBeginEvents != -1 &&
						NumLineBeginEvents < NumLineEndEvents) {
					TextEditor->MoveCur(NumLineBeginEvents + 1, 0, false, false);
					TextEditor->MoveCur(NumLineEndEvents, 0, true, false);
					TextEditor->SetSelTextBuf(asEvents);
				}

				TextEditor->EndUpdate();
				TextEditor->EndOperation();

			}

			PageControl_Files->ActivePageIndex = OldActivePageIndex;

		}
	}
	else {
    asFileNameBak = asFileNameH + _T(".bak");

		Ok = (CopyFile(asFileNameH.c_str(), asFileNameBak.c_str(), FALSE) == TRUE);
		if (Ok) {

			hFile = CreateFile(asFileNameBak, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			hFile2 = CreateFile(asFileNameH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile != INVALID_HANDLE_VALUE && hFile2 != INVALID_HANDLE_VALUE) {

				bBeginClass = false;
				bBeginComponents = false;
				bBeginEvents = false;
				while (FileLineInput(hFile, &csLigne)) {

					csLigneSansEsp = csLigne.Trim();

					if (bBeginComponents && csLigneSansEsp == "FVCL_END_COMPONENTS") {
						WriteFile(hFile2, asComponents.AsCharString().c_str(), asComponents.Length(), &dw, NULL);
						bBeginComponents = false;
					}
					else if (bBeginEvents && csLigneSansEsp == "FVCL_END_EVENTS") {
						WriteFile(hFile2, asEvents.AsCharString().c_str(), asEvents.Length(), &dw, NULL);
						bBeginEvents = false;
					}

					if (!bBeginComponents && !bBeginEvents) {
						WriteFile(hFile2, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
						WriteFile(hFile2, "\r\n", 2, &dw, NULL);
					}

					if (csLigneSansEsp.Length() >= 7 && (csLigneSansEsp.SubString(1, 6) == "class " || csLigneSansEsp.SubString(1, 6) == "class\t")) {
						PosDeuxPoints = csLigneSansEsp.Pos(":");
						if (PosDeuxPoints > 6) {
							csClass = csLigneSansEsp.SubString(7, PosDeuxPoints - 7).Trim();
							bBeginClass = (csClass == asClassName.AsCharString());
						}
					}
					else if (bBeginClass && csLigneSansEsp == "FVCL_BEGIN_COMPONENTS") {
						bBeginComponents = true;
					}
					else if (bBeginClass && csLigneSansEsp == "FVCL_BEGIN_EVENTS") {
						bBeginEvents = true;
					}

				}

			}
			else if (hFile == INVALID_HANDLE_VALUE) {
				DisplaySystemError(asFileNameH,
														GetLastError(),
														_TT("Impossible d'ouvrir ce fichier"),
														_T(""),
														MB_OK | MB_ICONSTOP);
			}
			else {
				DisplaySystemError(asFileNameBak,
														GetLastError(),
														_TT("Impossible d'ouvrir ce fichier"),
														_T(""),
														MB_OK | MB_ICONSTOP);
			}

			if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
			if (hFile2 != INVALID_HANDLE_VALUE) CloseHandle(hFile2);

		}
		else {
			DisplaySystemError(asFileNameH,
													GetLastError(),
													_TT("Impossible de sauvegarder le fichier"),
													_T(""),
													MB_OK | MB_ICONSTOP);
		}

	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SearchOrCreateCodeEvent(AnsiString asFileNameCpp, AnsiString asClassName, AnsiString asFunctionEvent, AnsiString asArgs) {
	AnsiString asFunctionPrototype;
	TTextEditor *TextEditor;
	SEARCHPARAM SearchParam;
	int NumLine;


	// Fichier CPP
	if (OpenTextFile(asFileNameCpp)) {
		TextEditor = GetCurTextFile();
		// En principe, TextEditor ne devrait pas être NULL (sauf CPP binaire ???)
		if (TextEditor) {
			SearchParam.Down = true;
			SearchParam.SinceCursor = false;
			SearchParam.WholeWord = true;
			SearchParam.CaseSensitive = true;
			SearchParam.BegLine = false;
			SearchParam.EndLine = false;
			SearchParam.OnlyInComments = false;
			SearchParam.NotInComments = true;
			SearchParam.OnlyInQuotes = false;
			SearchParam.NotInQuotes = true;
			SearchParam.RegExp = false;
			SearchParam.KeepCase = true;
			SearchParam.Loop = false;
			asFunctionPrototype = _T("void FASTCALL ");
			asFunctionPrototype += asClassName;
			asFunctionPrototype += _T("::");
			asFunctionPrototype += asFunctionEvent;
			asFunctionPrototype += asArgs;
			if (!TextEditor->Search(asFunctionPrototype, &SearchParam)) {
				AnsiString asContent;

				TextEditor->BeginOperation(_TT("Création d'une méthode de traitement d'évènement"));
				TextEditor->BeginUpdate();

				asContent = _T("//---------------------------------------------------------------------------\n");
				asContent += asFunctionPrototype;
				asContent += _T(" {\n\t\n}\n");
				TextEditor->MoveCur(INT_MAX, 0, false, false);
				NumLine = TextEditor->NumLine + 1;
				TextEditor->SetSelTextBuf(asContent);

				TextEditor->EndUpdate();
				TextEditor->EndOperation();
			}
			else {
				NumLine = TextEditor->NumLine;
			}
			TextEditor->MoveCur(NumLine + 1, 2, false, false);
			TextEditor->SetFocus();
		}
	}

	return true;
}

//---------------------------------------------------------------------------
//
//															METHODES INTERNES
//
//---------------------------------------------------------------------------

bool FASTCALL TForm_Principale::CheckUpdatesStart(void) {
  HKEY hKey;
  DWORD dwSize, dwRead;
	BOOL bCheckUpdates;
  DWORD NbFailsUpdates;
  int rc;
	bool bRet = true;


	NbFailsUpdates = 0;
	bCheckUpdates = TRUE;

	rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Denis ROBERT\\FVCLIDE"),
                    0, KEY_ALL_ACCESS, &hKey);
	if (rc == ERROR_ACCESS_DENIED) {
		rc = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE"),
											0, KEY_ALL_ACCESS, &hKey);
	}
	if (rc == ERROR_SUCCESS) {

		// Mise à jour désactivée
		dwSize = sizeof(DWORD);
		RegQueryValueEx(hKey, _T("CheckUpdates"), NULL, &dwRead, (BYTE*) &bCheckUpdates, &dwSize);

		// Nombre d'échecs de la mise à jour
		dwSize = sizeof(DWORD);
		RegQueryValueEx(hKey, _T("NbFailsUpdates"), NULL, &dwRead, (BYTE*) &NbFailsUpdates, &dwSize);

	  RegCloseKey(hKey);

	}

	if (bCheckUpdates == TRUE) {

		switch (CheckUpdates(false)) {
			case 0:  // Mise à jour demandée
				NbFailsUpdates = 0;
				bRet = false;
				break;
			case 1:  // Le check a échoué
				if (++NbFailsUpdates >= 3) {
					int Rep;
					Rep = DisplayError(_T("Impossible de vérifier les mises à jour disponibles.\nVoulez-vous désactiver la mise à jour automatique ?"),
														 _T("Mise à jour"),
														 _T("QUESTDISABLEUPDATE"),
														 MB_YESNO | MB_ICONWARNING);
					if (Rep == IDYES) {
						DisplayError(_T("Vous pourrez faire les mises à jour ultérieurement en passant par le menu \"Aide\" (F4)."),
												 _T("Mise à jour"),
												 _T("INFOMENUUPDATE"),
												 MB_OK | MB_ICONINFORMATION);
						bCheckUpdates = FALSE;
					}
				}
				break;
			case 2:  // Mise à jour disponible mais refusée
				NbFailsUpdates = 0;
				break;
			default:  // Programme à jour
				NbFailsUpdates = 0;
				break;
		}

	}

	rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
											_T("Software\\Denis ROBERT\\FVCLIDE"),
											0,
											0,
											REG_OPTION_NON_VOLATILE,
											KEY_ALL_ACCESS,
											NULL,
											&hKey,
											&dwSize);
	if (rc == ERROR_ACCESS_DENIED) {
		rc = RegCreateKeyEx(HKEY_CURRENT_USER,
												_T("Software\\Denis ROBERT\\FVCLIDE"),
												0,
												0,
												REG_OPTION_NON_VOLATILE,
												KEY_ALL_ACCESS,
												NULL,
												&hKey,
												&dwSize);
	}
	if (rc == ERROR_SUCCESS) {

		// Mise à jour désactivée
		rc = RegSetValueEx(hKey, _T("CheckUpdates"), 0, REG_DWORD, (BYTE*) &bCheckUpdates, sizeof(DWORD));

		// Nombre d'échecs de la mise à jour
		rc = RegSetValueEx(hKey, _T("NbFailsUpdates"), 0, REG_DWORD, (BYTE*) &NbFailsUpdates, sizeof(DWORD));

		RegCloseKey(hKey);

	}

	return bRet;
}

//---------------------------------------------------------------------------
int FASTCALL TForm_Principale::CheckUpdates(bool AffichageErrors) {
	char * szPos;
	CharString csBuf;
	AnsiString asCurVersion;
	AnsiString asNewVersion;
	AnsiString asMessage;
	int Rep;
	int Ret = 1;
	int i;


	Screen->Cursor = crHourGlass;

	ClientSocket = new TClientSocket(this);
	szBufSocket = new char[16000];
	memset(szBufSocket, 0, 16000);
	SET_EVENT(ClientSocket, TOnRead, OnRead, TForm_Principale, this, ClientSocketRead);
	if (!AffichageErrors) {
		SET_EVENT(ClientSocket, TOnError, OnError, TForm_Principale, this, ClientSocketError);
	}
	ClientSocket->Host = _T("denis.beru.free.fr");
	ClientSocket->Port = 80;
	if (ClientSocket->Open()) {
		for (i = 0; ClientSocket->Active == false && i < 100; i++) {
			Sleep(100);
			Application->ProcessMessages();
		}
		if (ClientSocket->Active) {
			csBuf = "GET /Download/FreeVCL.xml HTTP/1.1\r\nHost: denis.beru.free.fr\r\nConnection: keep-alive\r\n\r\n";
			ClientSocket->SendBuf(csBuf.c_str(), csBuf.Length());
			bByteReady = false;
			for (i = 0; !bByteReady && i < 1000; i++) {
				Sleep(10);
				Application->ProcessMessages();
			}
			if (bByteReady) {
				szPos = strstr(szBufSocket, "<Program_Version>");
				if (szPos) {
					szPos += 17; // sizeof("<Program_Version>")
					while (*szPos != '\0' && *szPos != '<') {
						if (*szPos != ' ' && *szPos != '\t' && *szPos != '\r' && *szPos != '\n') {
							asNewVersion += *szPos;
						}
						szPos++;
					}
				}
			}
		}
		ClientSocket->Close();
	}
	delete[] szBufSocket;
	delete ClientSocket;

	if (!asNewVersion.IsEmpty()) {
		VerLectureInfo(L"FileVersion", &asCurVersion);
		// asCurVersion = _T("1.1.1.2");  // $$$ test
		// asNewVersion = _T("1.1.2.1");  // $$$ test
		TNumVersion CurVersion(asCurVersion.c_str());
		TNumVersion NewVersion(asNewVersion.c_str());
		if (CurVersion < NewVersion) {
			asMessage.sprintf(_T("Version actuelle : %s.\nVoulez-vous télécharger et installer la nouvelle version %s ?"), asCurVersion.c_str(), asNewVersion.c_str());
			Rep = DisplayError(asMessage,
											_T("Mise à jour"),
											_T("QUESTAUTOUPDATE"),
											MB_YESNO | MB_ICONQUESTION);
			if (Rep == IDYES) {
				ShellExecute(Handle, _T("open"), _T("http://denis.beru.free.fr/Download/FreeVCL.exe"), NULL, NULL, SW_SHOW);
				Close();
				Ret = 0;
			}
			else {
				Ret = 2;
			}
	  }
		else {
			Ret = 3;
		}
	}

	Screen->Cursor = crDefault;

	return Ret;
}

//---------------------------------------------------------------------------
void TForm_Principale::ClientSocketRead(TObject* Sender, TCustomWinSocket* Socket) {
	ClientSocket->ReceiveBuf(szBufSocket, 16000);
	bByteReady = true;
}

//---------------------------------------------------------------------------
void TForm_Principale::ClientSocketError(TObject* Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &Error) {
	// On ignore les erreurs (démarrage sans réseau par exemple)
	Error = 0;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::QuestEnrModifs(void) {

	if (!QuestEnrModifsFiles()) return false;
	if (!QuestEnrModifsProjects()) return false;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::QuestEnrModifsFiles(void) {
	int i;


	for (i = 0; i < (int) TabSheets.size(); i++) {
		if (!QuestEnrModifsFile(i)) return false;
	}

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::QuestEnrModifsFile(int Index) {
	AnsiString asFileName;


	TFrame_FVCLIDEBase *Frame_FVCLIDEBase = TabSheets[Index].Content;

	if (Frame_FVCLIDEBase->Changed) {
		int Rep;
		asFileName = Frame_FVCLIDEBase->FileName;
		if (asFileName.IsEmpty()) asFileName = _TT("Nouveau fichier");
		PageControl_Files->ActivePageIndex = Index;
		AnsiString asMsg;
		asMsg = _TT("Voulez-vous enregistrer les modifications ?\n") + asFileName + _T("\n");
		asMsg += _T("(Modifications:\n");
		asMsg += Frame_FVCLIDEBase->GetModificationsList();
		asMsg += _T(")");
		Rep = DisplayError(asMsg,
											 _TT("Fichier modifié"),
											 _T("SAVE_CHANGES"),
											 MB_YESNOCANCEL | MB_ICONQUESTION);
		if (Rep == IDCANCEL) return false;
		else if (Rep == IDYES) {
			Action_SaveFileExecute(this);
			if (Frame_FVCLIDEBase->Changed) return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::QuestEnrModifsProjects(void) {
	/*
	AnsiString asNom;


	if (bProjectChanged) {
		int Rep;
		if (!asFileProject.IsEmpty()) asNom = asFileProject;
		else asNom = _TT("Nouveau projet");
		AnsiString asMsg;
		asMsg = _TT("Voulez-vous enregistrer les modifications ?\n") + asNom;
		Rep = DisplayError(asMsg,
											 _TT("Fichier modifié"),
											 _T("SAVE_CHANGES"),
											 MB_YESNOCANCEL | MB_ICONQUESTION);
		if (Rep == IDCANCEL) return false;
		else if (Rep == IDYES) {
			Action_SaveProjectExecute(this);
			if (bProjectChanged) return false;
		}
	} */

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::QuestDebugging(void) {

	if (bDebugging) {
		int Rep;

		Rep = DisplayError(_TT("Voulez-vous arrêter le débogage en cours ?"),
											 _TT("Débogage"),
											 _T("STOP_DEBUG"),
											 MB_YESNOCANCEL | MB_ICONQUESTION);
		if (Rep != IDYES) return false;

		Action_StopDebugExecute(this);

	}

	return true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::SearchAndReplace(int TabIndex) {
	AnsiString asSelText;


	if (!Form_SearchAndReplace) {
		Form_SearchAndReplace = new TForm_SearchAndReplace(this, _T("TForm_SearchAndReplace"));
	}

	Form_SearchAndReplace->TabControl->TabIndex = TabIndex;
	Form_SearchAndReplace->SearchParam = &SearchParam;
	asSelText = FVCLIDEAutomation->GetSelText();
	if (!asSelText.IsEmpty()) {
		if (asSelText.Pos(_T("\n")) == 0) {
			Form_SearchAndReplace->ComboBox_Chercher->Text = asSelText;
			Form_SearchAndReplace->Button_Chercher->Caption = _TT("Suivant");
			if (Form_SearchAndReplace->TabControl->TabIndex > 1) Form_SearchAndReplace->Button_Remplacer->Enabled = true;
		}
		else {
			Form_SearchAndReplace->RadioButton_TexteSelectionne->Checked = true;
		}
	}
	Form_SearchAndReplace->Show();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ReadConfig(void) {
	int i;
	TMenuItem *NewMenuItem;
	AnsiString asLastFileName;
	AnsiString asName;

	int EdiLeft;
	int EdiTop;
	int EdiWidth;
	int EdiHeight;
	bool EdiFullScreen;
	int BottomPaneHeight;
	int LeftPaneWidth;
	RECT RectScreen;


	EdiLeft = Config->EdiLeft;
	EdiTop = Config->EdiTop;
	EdiWidth = Config->EdiWidth;
	EdiHeight = Config->EdiHeight;
	EdiFullScreen = Config->EdiFullScreen;
	LeftPaneWidth = Config->LeftPaneWidth;
	BottomPaneHeight = Config->BottomPaneHeight;

	if (EdiLeft != -1 &&
			EdiTop != -1 &&
			EdiWidth != -1 &&
			EdiHeight != -1) {

		// Test pour que la fenêtre s'affiche dans l'écran (changement des paramètres d'affichage, par exemple)
		RectScreen.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		RectScreen.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
		RectScreen.right = RectScreen.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
		RectScreen.bottom = RectScreen.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
		if (EdiWidth > RectScreen.right - RectScreen.left) EdiWidth = RectScreen.right - RectScreen.left;
		if (EdiHeight > RectScreen.bottom - RectScreen.top) EdiHeight = RectScreen.bottom - RectScreen.top;
		if (EdiLeft < RectScreen.left) EdiLeft = RectScreen.left;
		if (EdiTop < RectScreen.top) EdiTop = RectScreen.top;
		if (EdiLeft > RectScreen.right - EdiWidth) EdiLeft = RectScreen.right - EdiWidth;
		if (EdiTop > RectScreen.bottom - EdiHeight) EdiTop = RectScreen.bottom - EdiHeight;

		Size(EdiLeft, EdiTop, EdiWidth, EdiHeight, false);
		Position = poDesigned;
	}
	if (EdiFullScreen) WindowState = wsMaximized;
	if (LeftPaneWidth != -1) {
		TreeView_Projects->Width = LeftPaneWidth;
	}
	if (BottomPaneHeight != -1) {
		PageControl_Logs->Height = BottomPaneHeight;
	}

	// Derniers espaces de travail
	for (i = 0; i < 4; i++) {
		asLastFileName = Config->LastWorkSpaces[i];
		if (asLastFileName.IsEmpty()) break;
		NewMenuItem = new TMenuItem(this);
		asName.sprintf(_T("LastWorkSpace%i"), i);
		NewMenuItem->Name = asName;
		NewMenuItem->Caption = asLastFileName;
		SET_EVENT(NewMenuItem, TNotifyEvent, OnClick, TForm_Principale, this, MenuItem_LastWorkSpaceClick);
		MenuItem_WorkSpaces->Add(NewMenuItem);
	}

	// Derniers projets
	for (i = 0; i < 4; i++) {
		asLastFileName = Config->LastProjects[i];
		if (asLastFileName.IsEmpty()) break;
		NewMenuItem = new TMenuItem(this);
		asName.sprintf(_T("LastProject%i"), i);
		NewMenuItem->Name = asName;
		NewMenuItem->Caption = asLastFileName;
		SET_EVENT(NewMenuItem, TNotifyEvent, OnClick, TForm_Principale, this, MenuItem_LastProjectClick);
		MenuItem_Projects->Add(NewMenuItem);
	}

	// Derniers fichiers
	for (i = 0; i < 4; i++) {
		asLastFileName = Config->LastFiles[i];
		if (asLastFileName.IsEmpty()) break;
		NewMenuItem = new TMenuItem(this);
		asName.sprintf(_T("LastFile%i"), i);
		NewMenuItem->Name = asName;
		NewMenuItem->Caption = asLastFileName;
		SET_EVENT(NewMenuItem, TNotifyEvent, OnClick, TForm_Principale, this, MenuItem_LastFileClick);
		MenuItem_Files->Add(NewMenuItem);
	}

	// Derniers dialogues
	for (i = 0; i < 4; i++) {
		asLastFileName = Config->LastDialogs[i];
		if (asLastFileName.IsEmpty()) break;
		NewMenuItem = new TMenuItem(this);
		asName.sprintf(_T("LastDialog%i"), i);
		NewMenuItem->Name = asName;
		NewMenuItem->Caption = asLastFileName;
		SET_EVENT(NewMenuItem, TNotifyEvent, OnClick, TForm_Principale, this, MenuItem_LastDialogClick);
		MenuItem_Dialogs->Add(NewMenuItem);
	}

	// Derniers fichiers graphiques
	for (i = 0; i < 4; i++) {
		asLastFileName = Config->LastGraphics[i];
		if (asLastFileName.IsEmpty()) break;
		NewMenuItem = new TMenuItem(this);
		asName.sprintf(_T("LastGraphic%i"), i);
		NewMenuItem->Name = asName;
		NewMenuItem->Caption = asLastFileName;
		SET_EVENT(NewMenuItem, TNotifyEvent, OnClick, TForm_Principale, this, MenuItem_LastGraphicClick);
		MenuItem_Graphics->Add(NewMenuItem);
	}

	if (ParamCount() <= 1) {

		// WorkSpace ouvert
		asFileWorkSpace = Config->OpenWorkSpace;
		if (!asFileWorkSpace.IsEmpty()) {
			if (!OpenWorkSpace(asFileWorkSpace)) {
				int Rep;
				AnsiString asMsg;
				asMsg = _TT("Impossible d'ouvrir le fichier:\n");
				asMsg += asFileWorkSpace + _T("\n");
				asMsg += _TT("Voulez-vous le chercher à un autre emplacement ?\n");
				Rep = DisplayError(asMsg,
													 _TT("Fichier détruit ou déplacé"),
													 _T(""),
													 MB_YESNO | MB_ICONQUESTION);
				asFileWorkSpace = _T("");
				if (Rep == IDYES) {
					Action_OpenWorkSpaceExecute(this);
				}
			}
		}

	}
	else {
		bTempWorkSpace = true;
	}

	// Chercher / remplacer
	SearchParam.WholeWord = Config->WholeWord;
	SearchParam.CaseSensitive = Config->CaseSensitive;
	SearchParam.BegLine = Config->BegLine;
	SearchParam.EndLine = Config->EndLine;
	SearchParam.OnlyInComments = Config->OnlyInComments;
	SearchParam.NotInComments = Config->NotInComments;
	SearchParam.OnlyInQuotes = Config->OnlyInQuotes;
	SearchParam.NotInQuotes = Config->NotInQuotes;
	SearchParam.RegExp = Config->RegExp;
	SearchParam.KeepCase = Config->KeepCase;
	SearchParam.Loop = Config->Loop;

	// Raccourcis
	TAction *Action;
	TShortCut ShortCut;
	for (i = 0; i < (int) FComponents.size(); i++) {
		Action = dynamic_cast<TAction *>(FComponents[i]);
		if (Action) {
			ShortCut = Config->GetShortCut(Action->Name);
			if (ShortCut) Action->ShortCut = ShortCut;
		}
	}

	// Outils
	AddUserToolsInMenu();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::SaveConfig(void) {

	// WorkSpace ouvert
	if (!bTempWorkSpace) Config->OpenWorkSpace = asFileWorkSpace;

	// Fenêtre principale
	Config->EdiLeft = Left;
	Config->EdiTop = Top;
	Config->EdiWidth = Width;
	Config->EdiHeight = Height;
	Config->EdiFullScreen = (WindowState == wsMaximized);
	Config->LeftPaneWidth = TreeView_Projects->Width;
	Config->BottomPaneHeight = PageControl_Logs->Height;

	// Chercher / remplacer
	Config->WholeWord = SearchParam.WholeWord;
	Config->CaseSensitive = SearchParam.CaseSensitive;
	Config->BegLine = SearchParam.BegLine;
	Config->EndLine = SearchParam.EndLine;
	Config->OnlyInComments = SearchParam.OnlyInComments;
	Config->NotInComments = SearchParam.NotInComments;
	Config->OnlyInQuotes = SearchParam.OnlyInQuotes;
	Config->NotInQuotes = SearchParam.NotInQuotes;
	Config->RegExp = SearchParam.RegExp;
	Config->KeepCase = SearchParam.KeepCase;
	Config->Loop = SearchParam.Loop;

}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::UpdateTreeView(TXMLFile *XMLNode) {
	TProject *Project;
	TTreeNode *TreeNode;
	int i;


	for (i = 0; i < (int) TreeView_Projects->Items->Count; i++) {
		TreeNode = TreeView_Projects->Items->Item[i];
		Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
		delete Project;
	}
	TreeView_Projects->Items->Clear();

	UpdateTreeNode(TreeView_Projects->Items->FirstNode, XMLNode);

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::UpdateTreeNode(TTreeNode *TreeNode, TXMLFile *XMLNode) {
	int i;
	AnsiString asProjectType;
	TTreeNode *NodeChild;
	TXMLFile *XMLNodeChild;
	TProject *ProjectChild;
	int IndexTypeProject;


	for (i = 0; i < XMLNode->Count; i++) {
		XMLNodeChild = XMLNode->Node[i];
		if (XMLNodeChild->Name == _T("project")) {

			asProjectType = XMLNode->Node[i]->GetAttribute(_T("type"));

			// Création du projet
			ProjectChild = new TProject(ListTypeProjects);
			ProjectChild->Name = XMLNodeChild->Text;
			ProjectChild->TypeProject = asProjectType;
			ProjectChild->FileName = XMLNodeChild->GetAttribute(_T("file"));
			ProjectChild->Output = XMLNodeChild->GetAttribute(_T("output"));
			ProjectChild->Content = XMLNodeChild->GetAttribute(_T("content"));
			ProjectChild->LastChange = XMLNodeChild->GetAttribute(_T("lastchange"));
			ProjectChild->Built = XMLNodeChild->GetAttribute(_T("built"));
			if (XMLNodeChild->GetAttribute(_T("current")) == _T("true")) CurrentProject = ProjectChild;
			AddBitmapToImageList(ProjectChild);
			IndexTypeProject = ProjectChild->IndexTypeProject;

			// Enregistrement dans le TreeView
			NodeChild = TreeView_Projects->Items->AddChild(TreeNode, XMLNodeChild->Text);
			NodeChild->StateIndex = 0;
			NodeChild->ImageIndex = IndexTypeProject + DECAL_TYPES_PROJECT;  // Image numéro 0 = Text file
			NodeChild->SelectedIndex = IndexTypeProject + DECAL_TYPES_PROJECT;
			NodeChild->Data = ProjectChild;

			UpdateTreeNode(NodeChild, XMLNodeChild);

			NodeChild->Expanded = (XMLNodeChild->GetAttribute(_T("expanded")) == _T("true"));
		}
	}

	for (i = 0; i < XMLNode->Count; i++) {
		XMLNodeChild = XMLNode->Node[i];
		if (XMLNodeChild->Name == _T("input")) {

			asProjectType = XMLNodeChild->GetAttribute(_T("type"));

			ProjectChild = new TProject(ListTypeProjects);
			ProjectChild->Name = XMLNodeChild->Text;
			ProjectChild->TypeProject = asProjectType;
			ProjectChild->FileName = XMLNodeChild->GetAttribute(_T("file"));
			IndexTypeProject = ProjectChild->IndexTypeProject + DECAL_TYPES_PROJECT;

			NodeChild = TreeView_Projects->Items->AddChild(TreeNode,  XMLNodeChild->Text);
			NodeChild->StateIndex = 0;
			NodeChild->ImageIndex = IndexTypeProject;
			NodeChild->SelectedIndex = IndexTypeProject;
			NodeChild->Data = ProjectChild;

		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::UpdateXMLTree(TXMLFile *XMLNode) {

	XMLNode->Clear();
	XMLNode->NoComment = true;
	XMLNode->SetAttribute(_T("version"), _T("1.0"));
	XMLNode->SetAttribute(_T("encoding"), _T("utf-8"));

	UpdateXMLNode(TreeView_Projects->Items->FirstNode, XMLNode);

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::UpdateXMLNode(TTreeNode *TreeNode, TXMLFile *XMLNode) {
	TTreeNode *TreeNodeChild;
	TXMLFile *XMLNodeChild;
	TProject *ProjectChild;
	TDateTime dtNonInitialise;


	TreeNodeChild = TreeNode->GetFirstChild();
	while (TreeNodeChild) {

		ProjectChild = reinterpret_cast<TProject *>((void *) TreeNodeChild->Data);
		if (ProjectChild) {
			if (ProjectChild->IndexTypeProject >= 0) {
				XMLNodeChild = XMLNode->AddNode(_T("project"));
			}
			else {
				XMLNodeChild = XMLNode->AddNode(_T("input"));
			}

			XMLNodeChild->SetAttribute(_T("type"), ProjectChild->TypeProject);
			XMLNodeChild->Text = ProjectChild->Name;
			if (!((AnsiString) ProjectChild->FileName).IsEmpty()) XMLNodeChild->SetAttribute(_T("file"), ProjectChild->FileName);
			if (ProjectChild->Built != dtNonInitialise) {
				XMLNodeChild->SetAttribute(_T("built"), ((TDateTime) ProjectChild->Built).FormatString(_T("dd/mm/yyyy hh:nn:ss")));
			}
			if (ProjectChild->LastChange != dtNonInitialise) {
				XMLNodeChild->SetAttribute(_T("lastchange"), ((TDateTime) ProjectChild->LastChange).FormatString(_T("dd/mm/yyyy hh:nn:ss")));
			}
			if (!((AnsiString) ProjectChild->Content).IsEmpty()) XMLNodeChild->SetAttribute(_T("content"), ProjectChild->Content);
			if (!((AnsiString) ProjectChild->Output).IsEmpty()) XMLNodeChild->SetAttribute(_T("output"), ProjectChild->Output);
			if (ProjectChild == CurrentProject) XMLNodeChild->SetAttribute(_T("current"), _T("true"));
			if (TreeNodeChild->Expanded) XMLNodeChild->SetAttribute(_T("expanded"), _T("true"));


			UpdateXMLNode(TreeNodeChild, XMLNodeChild);

			TreeNodeChild = TreeNode->GetNextChild(TreeNodeChild);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::UpdateTreeViewStates(void) {

	UpdateTreeNodeStates(TreeView_Projects->Items->FirstNode);

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::UpdateTreeNodeStates(TTreeNode *TreeNode) {
	TTreeNode *TreeNodeChild;
	TProject *Project;
	int StateIndex;
	AnsiString asMessage;
	bool bMustBuild;


	TreeNodeChild = TreeNode->GetFirstChild();
	while (TreeNodeChild) {

		UpdateTreeNodeStates(TreeNodeChild);

		TreeNodeChild = TreeNode->GetNextChild(TreeNodeChild);
	}

	Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (Project) {
		if (Project->IndexTypeProject >= 0) {
			MustBuildNode(TreeNode, &bMustBuild, &asMessage);
			if (bMustBuild) StateIndex = 1;
			else StateIndex = 0;
			if (Project == CurrentProject) StateIndex += 2;
		}
		else {
			StateIndex = -1;
		}
	}
	else {
		StateIndex = -1;
	}

	TreeNode->StateIndex = StateIndex;

	return true;
}

//---------------------------------------------------------------------------
// Suppression d'une dépendance
//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::RemoveDependencies(TTreeNode *TreeNode) {
	TProject *ProjectParent;
	TProject *ProjectChild;
	TTreeNode *NodeChild;
	TTreeNode *NodeChildTmp;


	ProjectParent = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (ProjectParent == NULL) return false;

	NodeChild = TreeNode->GetLastChild();
	while (NodeChild) {
		ProjectChild = reinterpret_cast<TProject *>((void *) NodeChild->Data);
		NodeChildTmp = TreeNode->GetPrevChild(NodeChild);  // A faire avant de le supprimer (sinon plantage)

		if (ProjectChild && ProjectChild->IndexTypeProject < 0) {

			// Suppression du TreeView
			TreeViewDeleteNode(NodeChild);

		}

		NodeChild = NodeChildTmp;
	}

	return true;
}

//---------------------------------------------------------------------------
// Insertion des dépendances
//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::InsertDependencies(TTreeNode *TreeNode) {

	int IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
	TProject *Project;
	AnsiString asProjectFileName;
	AnsiString asProjectContent;


	Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (Project == NULL) return false;

	asProjectFileName = Project->FileName;
	asProjectContent = Project->Content;

	AnsiString asFileName;
	if (Project->GetOutput(&asFileName)) {

		// Insertion dans le projet
		Project->Output = asFileName;

	}

	TStringList *slListInputs;
	slListInputs = new TStringList();
	if (Project->GetInputs(slListInputs)) {
		TProject *ProjectFile;
		TTreeNode *NodeFile;
		int IndexTypeProject;
		int Len;
		int i;

		AnsiString asFileName;
		AnsiString asShortFileName;
		for (i = 0; i < slListInputs->Count; i++) {
			asFileName = slListInputs->Strings[i];
			Len = asFileName.Length();
			if (Len > 0 && (asFileName[Len] == _T('/') || asFileName[Len] == _T('\\'))) {
				// Répertoire
				IndexTypeProject = -1;
				asFileName = asFileName.SubString(1, Len - 1);
				asShortFileName = ExtractFileName(asFileName);
			}
			else {
				asShortFileName = ExtractFileName(asFileName);
				if (asShortFileName.Pos(_T("*")) != 0 || asShortFileName.Pos(_T("?")) != 0) {
					// Ensemble de fichiers
					IndexTypeProject = -2;
				}
				else {
					// Simple fichier
					IndexTypeProject = -3;
				}
			}


			// Insertion dans l'arborescence des projets
			ProjectFile = new TProject(ListTypeProjects);
			if (IndexTypeProject == -1) ProjectFile->TypeProject = _T("Directory");
			else if (IndexTypeProject == -2) ProjectFile->TypeProject = _T("Files");
			else ProjectFile->TypeProject = _T("File");
			ProjectFile->FileName = asFileName;
			ProjectFile->Name = asShortFileName;

			// Insertion dans le TreeView
			NodeFile = TreeView_Projects->Items->AddChild(TreeNode, asShortFileName);
			NodeFile->StateIndex = -1;
			NodeFile->ImageIndex = IndexTypeProject + DECAL_TYPES_PROJECT;
			NodeFile->SelectedIndex = IndexTypeProject + DECAL_TYPES_PROJECT;
			NodeFile->Data = ProjectFile;
			TreeNode->Expand(false);

		}

	}
	delete slListInputs;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::SaveFilesOfNode(TTreeNode *TreeNode) {
	TFrame_FVCLIDEBase *Frame_FVCLIDEBase;
	TTreeNode *NodeChild;
	int IndexTypeProject;
	bool bSaveBeforeBuild;
	bool Ok = true;
	int i;


	// -----------------------------------
	// Sauvegarde des fichiers du projet
	// -----------------------------------

	bSaveBeforeBuild = Config->SaveBeforeBuild;

	NodeChild = TreeNode->GetFirstChild();
	while (NodeChild) {

		IndexTypeProject = NodeChild->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject < 0) {
			TProject *ProjectChild;

			ProjectChild = reinterpret_cast<TProject *>((void *) NodeChild->Data);
			if (ProjectChild) {
				AnsiString asInputFileName;

				// Dépendance = simple fichier => on teste si fichier modifié
				asInputFileName = ProjectChild->FileName;

				for (i = 0; i < (int) TabSheets.size(); i++) {


					Frame_FVCLIDEBase = TabSheets[i].Content;
					if (asInputFileName == (AnsiString) Frame_FVCLIDEBase->FileName &&
							Frame_FVCLIDEBase->Changed) {
						if (bSaveBeforeBuild) {
							PageControl_Files->ActivePageIndex = i;
							Action_SaveFileExecute(this);
							if (Frame_FVCLIDEBase->Changed) return false;
						}
						else {
							if (!QuestEnrModifsFile(i)) return false;
						}
						// Pas de break (un même fichier peut être ouvert plusieurs fois)
					}
				}

			}

		}
		else {
			if (!SaveFilesOfNode(NodeChild)) {
				Ok = false;
				break;
			}
		}

		NodeChild = TreeNode->GetNextChild(NodeChild);

	}

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::PreCompilationNode(TTreeNode *TreeNode) {
	TTreeNode *TreeNodeChild;
	int IndexTypeProject;
	TProject *Project;
	int i;


	IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
	if (IndexTypeProject >= 0) {
		for (i = 0; i < TreeNode->Count; i++) {
			TreeNodeChild = TreeNode->Item[i];
			PreCompilationNode(TreeNodeChild);
		}
		Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
		if (Project) Project->PreCompilation();
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::BuildNode(TTreeNode *TreeNode, bool bAll) {
	AnsiString asProject;
	AnsiString asProjectFileName;
	AnsiString asProjectContent;
	AnsiString asOutputFileName;
	AnsiString asBuilt;
	AnsiString asLastChange;
	AnsiString asMessage;
	TTreeNode *NodeChild;
	TProject *Project;
	int IndexTypeProject;
	bool bMustBuild;
	bool Ok = true;
	int i;


	asProject = TreeNode->Text;
	Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (Project == NULL) {
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asProject);
		AddLogMessage(tl_Build, asMessage);
		return false;
	}

	// -----------------------------------
	// Génération des dépendances
	// -----------------------------------

	NodeChild = TreeNode->GetFirstChild();
	while (NodeChild) {

		IndexTypeProject = NodeChild->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject >= 0) {
			if (!BuildNode(NodeChild, bAll)) {
				Ok = false;
				break;
			}
		}

		NodeChild = TreeNode->GetNextChild(NodeChild);

	}


	// -----------------------------------------------
	// Test s'il faut compiler le projet
	// -----------------------------------------------

	if (Ok) {
		if (!bAll) {
			Ok = MustBuildNode(TreeNode, &bMustBuild, &asMessage);
			if (!Ok || bMustBuild) {
				AddLogMessage(tl_Build, asMessage);
			}
		}
		else {
			bMustBuild = true;
		}
	}


	// ----------------------
	// Compilation du projet
	// ----------------------

	if (Ok) {

		if (bMustBuild) {

			FVCL_ASSERT(Project->IndexTypeProject >= 0 && "Impossible de compiler un simple fichier");
			asMessage.sprintf(_TT(">>>>>>>>>> Génération de \"%s\" >>>>>>>>>>"), (LPCTSTR) asProject);
			AddLogMessage(tl_Build, asMessage);

			// Appel du build via le plugin
			Ok = Project->Build();

			if (Ok) {

				TDateTime dtBuilt;
				dtBuilt.CurrentDateTime(false);
				dtBuilt.IncSecond();  // Ajout d'une seconde à cause de l'imprécision des millisecondes
				Project->Built = dtBuilt;
				asMessage.sprintf(_TT("<<<<<<<<<< La génération de \"%s\" a réussi <<<<<<<<<<"), (LPCTSTR) asProject);
				AddLogMessage(tl_Build, asMessage);
			}
			else {
				asMessage.sprintf(_TT("<<<<<<<<<< La génération de \"%s\" a échoué <<<<<<<<<<"), (LPCTSTR) asProject);
				AddLogMessage(tl_Build, asMessage);
			}

		}
		else {
			asMessage.sprintf(_TT("Le projet \"%s\" est à jour"), (LPCTSTR) asProject);
			AddLogMessage(tl_Build, asMessage);
		}

	}

	// -------------------------------
	// Rechargement du fichier sortie
	// -------------------------------

	if (Ok) {
		TFrame_FVCLIDEBase *Frame_FVCLIDEBase;

		for (i = 0; i < (int) TabSheets.size(); i++) {

			Frame_FVCLIDEBase = TabSheets[i].Content;
			if (asOutputFileName == (AnsiString) Frame_FVCLIDEBase->FileName) {
				if (Frame_FVCLIDEBase->OpenFile(asOutputFileName)) {
					Frame_FVCLIDEBase->MemoDateFile();
				}
				// Pas de break (un même fichier peut être ouvert plusieurs fois)
			}
		}

	}

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::MustBuildNode(TTreeNode *TreeNode, bool *bMustBuild, AnsiString *asMessage) {
	AnsiString asProject;
	AnsiString asProjectFileName;
	AnsiString asProjectContent;
	AnsiString asOutputFileName;
	AnsiString asBuilt;
	AnsiString asLastChange;
	TTreeNode *NodeChild;
	TProject *Project;
	int IndexTypeProject;


	asProject = TreeNode->Text;
	Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (Project == NULL) {
		asMessage->sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asProject);
		*bMustBuild = true;
		return false;
	}

	// -----------------------------------
	// Test des dépendances
	// -----------------------------------

	NodeChild = TreeNode->GetFirstChild();
	while (NodeChild) {

		IndexTypeProject = NodeChild->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject >= 0) {
			AnsiString asMessageChild;
			MustBuildNode(NodeChild, bMustBuild, &asMessageChild);
			if (*bMustBuild) {
				asMessage->sprintf(_TT("Construction du projet \"%s\" car le projet \"%s\" n'est pas à jour"),
													(LPCTSTR) asProject,
													(LPCTSTR) (AnsiString) NodeChild->Text);
				*bMustBuild = true;
				return true;
			}
		}

		NodeChild = TreeNode->GetNextChild(NodeChild);

	}


	// -----------------------------------------------
	// Nom du fichier projet et paramètres principaux
	// -----------------------------------------------

	asProjectFileName = Project->FileName;
	asProjectContent = Project->Content;
	asOutputFileName = Project->Output;
	asBuilt = ((TDateTime) Project->Built).FormatString(_T("dd/mm/yyyy hh:nn:ss"));
	asLastChange = ((TDateTime) Project->LastChange).FormatString(_T("dd/mm/yyyy hh:nn:ss"));
	if (asProjectFileName.IsEmpty() && asProjectContent.IsEmpty()) {
		asMessage->sprintf(_TT("Erreur: projet \"%s\" non défini"), (LPCTSTR) asProject);
		*bMustBuild = true;
		return false;
	}

	// ---------------------------------------------------------------------------
	// Test de la date des dépendances pour savoir s'il faut recompiler le projet
	// ---------------------------------------------------------------------------

	AnsiString asInputFileName;
	TProject *ProjectChild;
	WIN32_FILE_ATTRIBUTE_DATA FileAttributeData;
	TDateTime dtOutputTime;
	TDateTime dtInputTime;
	TDateTime dtProjectTime;

	*bMustBuild = false;

	if (!asOutputFileName.IsEmpty()) {
		// Date du fichier de sortie
		if (GetFileAttributesEx(asOutputFileName, GetFileExInfoStandard, &FileAttributeData)) {
			dtOutputTime = FileAttributeData.ftLastWriteTime;
		}
		else {
			asMessage->sprintf(_TT("Construction du projet \"%s\" car fichier \"%s\" inexistant"),
												(LPCTSTR) asProject,
												(LPCTSTR) asOutputFileName);
			*bMustBuild = true;
			return true;
		}
	}
	else {
		// Date de dernière compilation
		if (!asBuilt.IsEmpty()) {
			dtOutputTime = asBuilt;
		}
		else {
			asMessage->sprintf(_TT("Construction du projet \"%s\" car ce projet n'a encore jamais été construit"),
												(LPCTSTR) asProject);
			*bMustBuild = true;
			return true;
		}
	}

	// Comparaison avec la date du projet

	// La valeur asLastChange est prioritaire sur la date de modification du fichier projet
	// car sinon les fichiers FVCL sont systématiquement recompilés (ils sont systématiquement
	// enregistrés car ils contiennent la date de dernière compilation, et donc le fichier
	// projet est plus récent que la date de dernière compilation).
	// Mais UNIQUEMENT pour les projets FreeVCL (sinon les modifications externes ne sont pas détectées)
	if (TreeNode->ImageIndex == DECAL_TYPES_PROJECT && !asLastChange.IsEmpty()) {
		dtProjectTime = asLastChange;
	}
	else if (!asProjectFileName.IsEmpty()) {
		if (GetFileAttributesEx(asProjectFileName, GetFileExInfoStandard, &FileAttributeData)) {
			dtProjectTime = FileAttributeData.ftLastWriteTime;
		}
		else {
			asMessage->sprintf(_TT("Erreur: impossible d'ouvrir \"%s\""), (LPCTSTR) asProjectFileName);
			*bMustBuild = true;
			return false;
		}
	}
	else if (!asLastChange.IsEmpty()) {
		dtProjectTime = asLastChange;
	}
	else {
		asMessage->sprintf(_TT("Erreur: impossible de lire la date de modification du projet \"%s\""), (LPCTSTR) asProject);
		*bMustBuild = true;
		return false;
	}
	if (dtProjectTime > dtOutputTime) {
		if (!asProjectFileName.IsEmpty()) {
			if (!asOutputFileName.IsEmpty()) {
				asMessage->sprintf(_TT("Construction du projet \"%s\" car projet \"%s\" plus récent que \"%s\""),
													(LPCTSTR) asProject,
													(LPCTSTR) asProjectFileName,
													(LPCTSTR) asOutputFileName);
			}
			else {
				asMessage->sprintf(_TT("Construction du projet \"%s\" car projet \"%s\" plus récent que la dernière construction"),
													(LPCTSTR) asProject,
													(LPCTSTR) asProjectFileName);
			}
		}
		else {
			if (!asOutputFileName.IsEmpty()) {
				asMessage->sprintf(_TT("Construction du projet \"%s\" car dernière modification du projet plus récente que \"%s\""),
													(LPCTSTR) asProject,
													(LPCTSTR) asOutputFileName);
			}
			else {
				asMessage->sprintf(_TT("Construction du projet \"%s\" car dernière modification du projet plus récente que la dernière construction"),
													(LPCTSTR) asProject,
													(LPCTSTR) asProjectFileName);
			}
		}
		*bMustBuild = true;
		return true;
	}

	// Comparaison avec la date des fichiers d'entrée

	NodeChild = TreeNode->GetFirstChild();
	while (NodeChild) {

		ProjectChild = reinterpret_cast<TProject *>((void *) NodeChild->Data);
		if (ProjectChild) {
			IndexTypeProject = NodeChild->ImageIndex - DECAL_TYPES_PROJECT;
			if (IndexTypeProject < 0) {

				// Dépendance = simple fichier => on teste la date de ce fichier
				asInputFileName = ProjectChild->FileName;
				if (!asInputFileName.IsEmpty()) {
					if (GetFileAttributesEx(asInputFileName, GetFileExInfoStandard, &FileAttributeData)) {
						dtInputTime = FileAttributeData.ftLastWriteTime;
						if (dtInputTime > dtOutputTime) {
							if (!asOutputFileName.IsEmpty()) {
								asMessage->sprintf(_TT("Construction du projet \"%s\" car fichier \"%s\" plus récent que \"%s\""),
																	(LPCTSTR) asProject,
																	(LPCTSTR) asInputFileName,
																	(LPCTSTR) asOutputFileName);
							}
							else {
								asMessage->sprintf(_TT("Construction du projet \"%s\" car fichier \"%s\" plus récent que la dernière construction"),
																	(LPCTSTR) asProject,
																	(LPCTSTR) asInputFileName);
							}
							*bMustBuild = true;
							return true;
						}
					}
					else {
						asMessage->sprintf(_TT("Erreur: impossible d'ouvrir \"%s\""), (LPCTSTR) asInputFileName);
						*bMustBuild = true;
						return false;
					}
				}
				else {
					AnsiString asNodeName;

					asNodeName = TreeNode->Text;
					asMessage->sprintf(_TT("Erreur: le noeud \"%s\" n'a pas de fichier associé."), (LPCTSTR) asNodeName);
					*bMustBuild = true;
					return false;
				}

			}
			else {

				// Dépendance = projet => on teste la date du fichier output (qui sert donc ici d'input)
				asInputFileName = ProjectChild->Output;
				if (!asInputFileName.IsEmpty()) {
					if (GetFileAttributesEx(asInputFileName, GetFileExInfoStandard, &FileAttributeData)) {
						dtInputTime = FileAttributeData.ftLastWriteTime;
						if (dtInputTime > dtOutputTime) {
							if (!asOutputFileName.IsEmpty()) {
								asMessage->sprintf(_TT("Construction du projet \"%s\" car fichier \"%s\" plus récent que \"%s\""),
																	(LPCTSTR) asProject,
																	(LPCTSTR) asInputFileName,
																	(LPCTSTR) asOutputFileName);
							}
							else {
								asMessage->sprintf(_TT("Construction du projet \"%s\" car fichier \"%s\" plus récent que la dernière construction"),
																	(LPCTSTR) asProject,
																	(LPCTSTR) asInputFileName);
							}
							*bMustBuild = true;
							return true;
						}
					}
					else {
						asMessage->sprintf(_TT("Erreur: impossible d'ouvrir \"%s\""), (LPCTSTR) asInputFileName);
						*bMustBuild = true;
						return false;
					}
				}
				else {
					AnsiString asBuilt;
					AnsiString asProjectChildName;

					// Pas de fichier output (qui sert d'input) => on teste la date de dernière compilation du projet enfant
					asBuilt = ((TDateTime) ProjectChild->Built).FormatString(_T("dd/mm/yyyy hh:nn:ss"));
					asProjectChildName = ProjectChild->Name;
					if (!asBuilt.IsEmpty()) {
						dtInputTime = asBuilt;
						if (dtInputTime > dtOutputTime) {
							if (!asOutputFileName.IsEmpty()) {
								asMessage->sprintf(_TT("Construction du projet \"%s\" car construction \"%s\" plus récente que \"%s\""),
																	(LPCTSTR) asProject,
																	(LPCTSTR) asProjectChildName,
																	(LPCTSTR) asOutputFileName);
							}
							else {
								asMessage->sprintf(_TT("Construction du projet \"%s\" car construction \"%s\" plus récente que la dernière construction"),
																	(LPCTSTR) asProject,
																	(LPCTSTR) asProjectChildName);
							}
							*bMustBuild = true;
							return true;
						}
					}
					else {
						AnsiString asProjectChild;
						asProjectChild = ProjectChild->Name;
						asMessage->sprintf(_TT("Erreur: Construction du projet \"%s\" impossible car le projet \"%s\" n'a pas été construit"),
															(LPCTSTR) asProject,
															(LPCTSTR) asProjectChild);
						*bMustBuild = true;
						return false;
					}
				}

			}

		}
		else {
			AnsiString asNodeName;

			asNodeName = TreeNode->Text;
			asMessage->sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
			*bMustBuild = true;
			return false;
		}

		NodeChild = TreeNode->GetNextChild(NodeChild);

	}

	*asMessage = _T("Le projet est à jour");

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::RunNode(TTreeNode *TreeNode) {
	AnsiString asProjectFileName;
	AnsiString asProjectContent;
	AnsiString asProject;
	AnsiString asMessage;
	TTreeNode *NodeChild;
	TProject *Project;
	int IndexTypeProject;
	bool bFirstChild;
	bool Ok = false;


	asProject = TreeNode->Text;
	Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (Project) {
		IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
		if (Project->Can_Run()) {
			// Run via le plugin
			asProjectFileName = Project->FileName;
			asProjectContent = Project->Content;
			Ok = Project->Run();
		}
		else {
			// Si l'exécution n'est pas possible, on lance les sous-projets
			asMessage.sprintf(_TT("Le projet \"%s\" ne peut pas être lancé directement"), (LPCTSTR) asProject);
			AddLogMessage(tl_Process, asMessage);

			bFirstChild = true;
			NodeChild = TreeNode->GetFirstChild();
			while (NodeChild) {
				IndexTypeProject = NodeChild->ImageIndex - DECAL_TYPES_PROJECT;
				if (IndexTypeProject >= 0) {
					asProject = NodeChild->Text;

					if (bFirstChild) {
						AddLogMessage(tl_Process, _TT("Lancement des projets enfants :"));
						bFirstChild = false;
					}
					asMessage.sprintf(_TT("Exécution de \"%s\""), (LPCTSTR) asProject);
					AddLogMessage(tl_Process, asMessage);

					// Lancement du projet enfant
					Ok = RunNode(NodeChild);
					if (!Ok) break;

				}
				NodeChild = TreeNode->GetNextChild(NodeChild);
			}
		}
	}
	else {
		AnsiString asNodeName, asMessage;

		asNodeName = TreeNode->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		AddLogMessage(tl_Process, asMessage);
	}

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::NodeToDebug(TTreeNode *TreeNode, TProject **Project) {
	TTreeNode *NodeChild;
	int IndexTypeProject;
	AnsiString asMessage;


	*Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	if (*Project) {
		IndexTypeProject = TreeNode->ImageIndex - DECAL_TYPES_PROJECT;
		if (IndexTypeProject >= 0) {
			if ((*Project)->Can_Debug()) {
				return true;
			}
			else {
				// Si l'exécution n'est pas possible, on débugge les sous-projets
				NodeChild = TreeNode->GetFirstChild();
				while (NodeChild) {
					if (NodeToDebug(NodeChild, Project)) return true;
					NodeChild = TreeNode->GetNextChild(NodeChild);
				}
				// Aucun sous-projet ne peut débugger => return false
			}
		}
	}
	else {
		AnsiString asNodeName, asMessage;
		asNodeName = TreeNode->Text;
		asMessage.sprintf(_TT("Erreur: l'élément \"%s\" n'a pas de projet associé."), (LPCTSTR) asNodeName);
		DisplayError(asMessage,
								 _TT("Opération impossible"),
								 _T(""),
								 MB_OK | MB_ICONWARNING);
	}

	*Project = NULL;

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::LogLineDblClick(AnsiString asLine) {
	AnsiString asFileName;

	bool bTextFile;
	TTextEditor *TextEditor;
	int NumCol, NumLine;

	bool bHexaFile;
	THexaEditor *HexaEditor;
	__int64 FilePos;


	if (DecodeLineBookmark(asLine, &asFileName, &bTextFile,  &NumCol,  &NumLine, &bHexaFile,  &FilePos)) {
		if (bTextFile) {
			OpenTextFile(asFileName);
			TextEditor = GetCurTextFile();
			FVCL_ASSERT(TextEditor != NULL && "Impossible d'obtenir un fichier texte qu'on vient d'ouvrir");
			TextEditor->MoveCur(NumLine, NumCol, false, false);
			TextEditor->SetFocus();
		}
		else if (bHexaFile) {
			OpenBinFile(asFileName);
			HexaEditor = GetCurHexaFile();
			FVCL_ASSERT(HexaEditor != NULL && "Impossible d'obtenir un fichier hexa qu'on vient d'ouvrir");
			HexaEditor->MoveCur(FilePos, true, false);
			HexaEditor->SetFocus();
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::DecodeLineBookmark(AnsiString asLine,
		AnsiString *asFileName,
		bool *bTextFile, int *NumCol, int *NumLine,
		bool *bHexaFile, __int64 *FilePos) {
	int PosPar1;
	int PosPar2;
	int PosComma;
	int PosDoubleDot;
	int i;


	*bTextFile = false;
	*bHexaFile = false;

	PosPar2 = asLine.Pos(_T("): "));
	// Il peut y avoir aussi des parenthèses dans le nom ("Program Files (x86)" par exemple)
	// => il faut rechercher la dernière parenthèse ouvrante précédent la parenthèse fermante
	PosPar1 = 0;
	for (i = PosPar2; i >= 1; i--) {
		if (asLine[i] == _T('(')) {
			PosPar1 = i;
			break;
		}
	}
	if (PosPar1 > 4 && PosPar1 < PosPar2) {
		AnsiString asNumLine;

		*asFileName = asLine.SubString(1, PosPar1 - 1).Trim();
		asNumLine = asLine.SubString(PosPar1 + 1, PosPar2 - PosPar1 - 1);
		PosComma = asNumLine.Pos(_T(","));
		PosDoubleDot = asNumLine.Pos(_T(":"));
		if (PosComma > 1) {
			*NumLine = asNumLine.SubString(1, PosComma - 1).ToIntDef(1) - 1;
			*NumCol = asNumLine.SubString(PosComma + 1, 10).ToIntDef(1) - 1;
			*bTextFile = true;
		}
		else if (PosDoubleDot > 0) {
			int i;
			TCHAR c;
			AnsiString asFilePos;

			asFilePos = asNumLine.SubString(1, PosDoubleDot - 1) +
									asNumLine.SubString(PosDoubleDot + 1, 4);
			*FilePos = 0;
			for (i = 1; i <= asFilePos.Length(); i++) {
				c = asFilePos[i];
				(*FilePos) <<= 4;
				if ('0' <= c && c <= '9') (*FilePos) += c - '0';
				else (*FilePos) += c - 'A' + 10;
			}
			*bHexaFile = true;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::AddUserToolsInMenu(void) {
	int i;
	TMenuItem *NewMenuItem;
	const USERTOOL *UserTool;
	AnsiString asName;


	for (i = 0; i < Config->UserToolsCount; i++) {
		UserTool = Config->UserTools[i];
		if (UserTool->Name.IsEmpty()) break;
		NewMenuItem = new TMenuItem(this);
		asName.sprintf(_T("UserTool%i"), i);
		NewMenuItem->Name = asName;
		NewMenuItem->Caption = UserTool->Name;
		SET_EVENT(NewMenuItem, TNotifyEvent, OnClick, TForm_Principale, this, MenuItem_UserToolClick);
		MenuItem_UserTools->Add(NewMenuItem);
	}

	return true;
}

//---------------------------------------------------------------------------
bool TForm_Principale::AddBitmapToImageList(TProject *Project) {
	int IndexTypeProject;


	IndexTypeProject = Project->IndexTypeProject;
	if (IndexTypeProject + DECAL_TYPES_PROJECT >= ImageList_TypeProjects->Count) {
		TBitmap *Bitmap = Project->GetBitmap20();
		ImageList_TypeProjects->Add(Bitmap, NULL);
		delete Bitmap;
	}

	return true;
}

//---------------------------------------------------------------------------
bool TForm_Principale::SetCurrentDebugLine(AnsiString asFileName, int NumLine1, int NumCol1, int NumLine2, int NumCol2) {
	int i;
	TTextEditor *TextEditor;


	if (!asDebugFileName.IsEmpty()) {

		for (i = 0; i < (int) TabSheets.size(); i++) {
			if (TabSheets[i].Content->FileType == ftf_TextFile &&
					asDebugFileName == (AnsiString) TabSheets[i].Content->FileName) {
				TFrame_EditFichierText *Frame_EditFichierText;
				Frame_EditFichierText = dynamic_cast<TFrame_EditFichierText *>(TabSheets[i].Content);
				if (Frame_EditFichierText) {
					Frame_EditFichierText->TextEditor->SetFlag(DebugNumLine1, lfCurStatement, false);
				}
			}
		}

	}

	if (!asFileName.IsEmpty()) {

		if (OpenTextFile(asFileName)) {

			TextEditor = GetCurTextFile();
			if (TextEditor) {
				TextEditor->MoveCur(NumLine1, NumCol1, false, false);
				TextEditor->SetFlag(NumLine1, lfCurStatement, true);
				TextEditor->SetFocus();
			}

		}

	}


	asDebugFileName = asFileName;
	DebugNumLine1 = NumLine1;
	DebugNumCol1 = NumCol1;
	DebugNumLine2 = NumLine2;
	DebugNumCol2 = NumCol2;


	return true;
}


//---------------------------------------------------------------------------
TTreeNode * FASTCALL TForm_Principale::TreeViewFindNode(const AnsiString asNodeName) {
	TTreeNode *TreeNode;
	int i;


	for (i = 0; i < (int) TreeView_Projects->Items->Count; i++) {
		TreeNode = TreeView_Projects->Items->Item[i];
		if (TreeNode->Text == asNodeName) return TreeNode;
	}

	return NULL;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Principale::TreeViewDeleteNode(TTreeNode *TreeNode) {
	TTreeNode *TreeNodeChild;
	TProject *Project;


	TreeNodeChild = TreeNode->GetFirstChild();
	while (TreeNodeChild) {

		// Récursion
		TreeViewDeleteNode(TreeNodeChild);

		// Enfant suivant
		TreeNodeChild = TreeNode->GetFirstChild();
	}

	// Destruction du projet
	Project = reinterpret_cast<TProject *>((void *) TreeNode->Data);
	delete Project;

	// Suppression du TreeView
	TreeView_Projects->Items->Delete(TreeNode);

	return true;
}

//---------------------------------------------------------------------------
TTreeNode * FASTCALL TForm_Principale::TreeViewAddNodeProject(TTreeNode *TreeNodeParent, AnsiString asName) {
	// Endroit où insérer le nouveau noeud (après les projets et avant les dépendances)
	TTreeNode *TreeNodeChild, *TreeNodeRelative, *NewTreeNode;
	TreeNodeRelative = NULL;
	TreeNodeChild = TreeNodeParent->GetFirstChild();
	while (TreeNodeChild) {
		if (TreeNodeChild->ImageIndex < DECAL_TYPES_PROJECT) break;
		TreeNodeRelative = TreeNodeChild;
		TreeNodeChild = TreeNodeParent->GetNextChild(TreeNodeChild);
	}

	if (TreeNodeRelative) {
		NewTreeNode = TreeView_Projects->Items->AddNode(NULL, TreeNodeRelative, asName, NULL, naInsert);
	}
	else {
		NewTreeNode = TreeView_Projects->Items->AddNode(NULL, TreeNodeParent, asName, NULL, naAddChildFirst);
	}

	return NewTreeNode;
}

//---------------------------------------------------------------------------
//   bool FileLineInput(HFILE hFile, CharString *csBuffer)
// Description:
//   Lecture d'une donnée dans un fichier texte (séparateur = CR)
// Paramètres:
//   [in]  HFILE hFile = handle du fichier
//   [out] asBuffer = ligne lue
// Sortie:
//   FALSE si fin de fichier atteinte ou erreur de lecture
//---------------------------------------------------------------------------

bool TForm_Principale::FileLineInput(HANDLE hFile, CharString *csBuffer) {
  char c;
  DWORD dwBytesRead;


  *csBuffer = "";
  while (true) {
    ReadFile(hFile, &c, 1, &dwBytesRead, NULL);
    if (dwBytesRead != 1) {
      if (!csBuffer->empty()) break;
      else return false;
    }
    if (c == '\n') break;
    if (c != '\r') *csBuffer += c;
  }

  return true;
}

//---------------------------------------------------------------------------
