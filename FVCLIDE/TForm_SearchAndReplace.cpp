//-----------------------------------------------------------------------------
//! @file TForm_SearchAndReplace.cpp
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

#include <FreeVCL.h>

#include "Utilit.h"

#include "TTextEditor.h"
#include "THexaEditor.h"
#include "TConfig.h"
#include "TForm_DisplayError.h"
#include "TForm_SearchAndReplace.h"
#include "TFVCLIDEAutomation.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_SearchAndReplace::TForm_SearchAndReplace(TComponent *Owner, LPCTSTR szName)
		: TForm(Owner, szName, FVCL_ARG_SUPP) {
	FSearchParam = NULL;
	bCanReplace = false;
	bBusy = false;

	TStrings *SearchStrings;
	SearchStrings = Config->SearchStrings;
	ComboBox_Chercher->Items->Assign(SearchStrings);
	ComboBox_Chercher->ItemIndex = 0;
	ComboBox_Chercher->SetFocus();

	TStrings *ReplaceStrings;
	ReplaceStrings = Config->ReplaceStrings;
	ComboBox_Remplacer->Items->Assign(ReplaceStrings);
	ComboBox_Remplacer->ItemIndex = 0;

	int SearchIn = Config->SearchIn;
	if (SearchIn == 0) RadioButton_FichiersProjet->Checked = true;
	else if (SearchIn == 1) RadioButton_FichiersTousProjet->Checked = true;
	else if (SearchIn == 2) RadioButton_FichiersOuverts->Checked = true;
	else if (SearchIn == 3) {
		RadioButton_Repertoire->Checked = true;
		RadioButton_RepertoireClick(this);
	}
	CheckBox_SousRep->Checked = Config->Recurse;
	Edit_Repertoire->Text = Config->SearchDirectory;
	ComboBox_Extensions->Text = Config->SearchExtensions;

	if (Config->AlphaBlend == false) {
		AlphaBlend = false;
		Timer_AlphaBlend->Enabled = false;
	}

}

//---------------------------------------------------------------------------
TForm_SearchAndReplace::~TForm_SearchAndReplace(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SearchParam
//---------------------------------------------------------------------------

SEARCHPARAM * TForm_SearchAndReplace::Get_SearchParam(void) {
	return FSearchParam;
}

bool TForm_SearchAndReplace::Set_SearchParam(SEARCHPARAM * NewSearchParam) {

	if (FSearchParam != NewSearchParam) {
		FSearchParam = NewSearchParam;

		CheckBox_MotEntier->Checked = FSearchParam->WholeWord;
		CheckBox_MAJmin->Checked = FSearchParam->CaseSensitive;
		CheckBox_DebutLigne->Checked = FSearchParam->BegLine;
		CheckBox_FinLigne->Checked = FSearchParam->EndLine;
		CheckBox_DansCommentaires->Checked = FSearchParam->OnlyInComments;
		CheckBox_HorsCommentaires->Checked = FSearchParam->NotInComments;
		CheckBox_DansGuillemets->Checked = FSearchParam->OnlyInQuotes;
		CheckBox_HorsGuillemets->Checked = FSearchParam->NotInQuotes;
		CheckBox_ExpressionReg->Checked = FSearchParam->RegExp;
		CheckBox_ConserverCasse->Checked = FSearchParam->KeepCase;
		// RadioButton_VersLeBas: On recherche toujours vers le bas
		RadioButton_DepuisDebut->Checked = !FSearchParam->SinceCursor;
		RadioButton_DepuisCurseur->Checked = FSearchParam->SinceCursor;
		CheckBox_Boucler->Checked = FSearchParam->Loop;
		RadioButton_Globale->Checked = !FSearchParam->InSelection;
		RadioButton_TexteSelectionne->Checked = FSearchParam->InSelection;

		CheckBox_DansCommentaires->Enabled = !CheckBox_HorsCommentaires->Checked;
		CheckBox_HorsCommentaires->Enabled = !CheckBox_DansCommentaires->Checked;
		CheckBox_DansGuillemets->Enabled = !CheckBox_HorsGuillemets->Checked;
		CheckBox_HorsGuillemets->Enabled = !CheckBox_DansGuillemets->Checked;

	}
	return true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::TabControlChange(TObject *Sender) {
	switch (TabControl->TabIndex) {
		case 0:
			CheckBox_ConserverCasse->Enabled = false;
			Label_Remplacer->Visible = false;
			ComboBox_Remplacer->Visible = false;
			GroupBox_Direction->Visible = true;
			GroupBox_Origine->Visible = true;
			GroupBox_Portee->Visible = true;
			GroupBox_ChercherDans->Visible = false;
			Label_Repertoire->Visible = false;
			Edit_Repertoire->Visible = false;
			Button_Repertoire->Visible = false;
			Label_Extensions->Visible = false;
			ComboBox_Extensions->Visible = false;
			Button_Remplacer->Enabled = false;
			break;
		case 1:
			CheckBox_ConserverCasse->Enabled = false;
			Label_Remplacer->Visible = false;
			ComboBox_Remplacer->Visible = false;
			GroupBox_Direction->Visible = false;
			GroupBox_Origine->Visible = false;
			GroupBox_Portee->Visible = false;
			GroupBox_ChercherDans->Visible = true;
			Label_Repertoire->Visible = true;
			Edit_Repertoire->Visible = true;
			Button_Repertoire->Visible = true;
			Label_Extensions->Visible = true;
			ComboBox_Extensions->Visible = true;
			Button_Remplacer->Enabled = false;
			break;
		case 2:
			CheckBox_ConserverCasse->Enabled = true;
			Label_Remplacer->Visible = true;
			ComboBox_Remplacer->Visible = true;
			GroupBox_Direction->Visible = true;
			GroupBox_Origine->Visible = true;
			GroupBox_Portee->Visible = true;
			GroupBox_ChercherDans->Visible = false;
			Label_Repertoire->Visible = false;
			Edit_Repertoire->Visible = false;
			Button_Repertoire->Visible = false;
			Label_Extensions->Visible = false;
			ComboBox_Extensions->Visible = false;
			Button_Remplacer->Enabled = bCanReplace;
			break;
		case 3:
			CheckBox_ConserverCasse->Enabled = true;
			Label_Remplacer->Visible = true;
			ComboBox_Remplacer->Visible = true;
			GroupBox_Direction->Visible = false;
			GroupBox_Origine->Visible = false;
			GroupBox_Portee->Visible = false;
			GroupBox_ChercherDans->Visible = true;
			Label_Repertoire->Visible = true;
			Edit_Repertoire->Visible = true;
			Button_Repertoire->Visible = true;
			Label_Extensions->Visible = true;
			ComboBox_Extensions->Visible = true;
			Button_Remplacer->Enabled = bCanReplace;
			break;
	}
	ComboBox_Chercher->SetFocus();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::Timer_AlphaBlendTimer(TObject *Sender) {
	RECT Rect;
	TPoint ptCursorPos;
	int NewAlphaBlendValue;
	int Distance, MaxDistance = 0;


	GetWindowRect(Handle, &Rect);
	ptCursorPos = Mouse->CursorPos;
	if (ptCursorPos.x < Rect.left) {
		Distance = Rect.left - ptCursorPos.x;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	if (Rect.right < ptCursorPos.x) {
		Distance = ptCursorPos.x - Rect.right;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	if (ptCursorPos.y < Rect.top) {
		Distance = Rect.top - ptCursorPos.y;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	if (Rect.bottom < ptCursorPos.y) {
		Distance = ptCursorPos.y - Rect.bottom;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	NewAlphaBlendValue = 255 - MaxDistance * 2;

	if (NewAlphaBlendValue < 31) NewAlphaBlendValue = 31;
	AlphaBlendValue = NewAlphaBlendValue;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::CheckBox_DansCommentairesClick(TObject *Sender) {
	CheckBox_HorsCommentaires->Enabled = !CheckBox_DansCommentaires->Checked;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::CheckBox_HorsCommentairesClick(TObject *Sender) {
	CheckBox_DansCommentaires->Enabled = !CheckBox_HorsCommentaires->Checked;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::CheckBox_DansGuillemetsClick(TObject *Sender) {
	CheckBox_HorsGuillemets->Enabled = !CheckBox_DansGuillemets->Checked;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::CheckBox_HorsGuillemetsClick(TObject *Sender) {
	CheckBox_DansGuillemets->Enabled = !CheckBox_HorsGuillemets->Checked;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::RadioButton_DepuisDebutClick(TObject *Sender) {
	CheckBox_Boucler->Checked = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::RadioButton_DepuisCurseurClick(TObject *Sender) {
	CheckBox_Boucler->Checked = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::RadioButton_RepertoireClick(TObject *Sender) {
	Label_Repertoire->Enabled = true;
	Edit_Repertoire->Enabled = true;
	Button_Repertoire->Enabled = true;
	Label_Extensions->Enabled = true;
	ComboBox_Extensions->Enabled = true;
	CheckBox_SousRep->Enabled = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::RadioButton_FichiersProjetClick(TObject *Sender) {
	Label_Repertoire->Enabled = false;
	Edit_Repertoire->Enabled = false;
	Button_Repertoire->Enabled = false;
	Label_Extensions->Enabled = false;
	ComboBox_Extensions->Enabled = false;
	CheckBox_SousRep->Enabled = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::RadioButton_FichiersTousProjetClick(TObject *Sender) {
	Label_Repertoire->Enabled = false;
	Edit_Repertoire->Enabled = false;
	Button_Repertoire->Enabled = false;
	Label_Extensions->Enabled = false;
	ComboBox_Extensions->Enabled = false;
	CheckBox_SousRep->Enabled = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::RadioButton_FichiersOuvertsClick(TObject *Sender) {
	Label_Repertoire->Enabled = false;
	Edit_Repertoire->Enabled = false;
	Button_Repertoire->Enabled = false;
	Label_Extensions->Enabled = false;
	ComboBox_Extensions->Enabled = false;
	CheckBox_SousRep->Enabled = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::Button_RepertoireClick(TObject *Sender) {
	AnsiString asRep;

	if (SelectDirectory(_TT("Répertoire de recherche"), _T(""), asRep)) {
		Edit_Repertoire->Text = asRep;
		Edit_Repertoire->SetFocus();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::Button_ChercherClick(TObject *Sender) {
	AnsiString asSearch;
	bool bCurrentFile;
	bool bProjectFiles = false;
	bool bAllProjectsFiles = false;
	bool bOpenFiles = false;
	bool bDirectory = false;
	int NbFound = 0;
	int Index;
	int FirstFileSearch = 0;
	TFVCLIDEFileType FileType;
	int i, j;


	if (FSearchParam == NULL) return;

	Screen->Cursor = crHourGlass;
	StartSearch();

	asSearch = ComboBox_Chercher->Text;
	Config->PushSearchString(asSearch);

	TStrings *SearchStrings;
	SearchStrings = Config->SearchStrings;
	ComboBox_Chercher->Items->Assign(SearchStrings);
	ComboBox_Chercher->ItemIndex = 0;

	FSearchParam->WholeWord = CheckBox_MotEntier->Checked;
	FSearchParam->CaseSensitive = CheckBox_MAJmin->Checked;
	FSearchParam->BegLine = CheckBox_DebutLigne->Checked;
	FSearchParam->EndLine = CheckBox_FinLigne->Checked;
	FSearchParam->OnlyInComments = CheckBox_DansCommentaires->Checked;
	FSearchParam->NotInComments = CheckBox_HorsCommentaires->Checked;
	FSearchParam->OnlyInQuotes = CheckBox_DansGuillemets->Checked;
	FSearchParam->NotInQuotes = CheckBox_HorsGuillemets->Checked;
	FSearchParam->RegExp = CheckBox_ExpressionReg->Checked;
	FSearchParam->KeepCase = CheckBox_ConserverCasse->Checked;

	bCurrentFile = (TabControl->TabIndex == 0 || TabControl->TabIndex == 2);
	if (bCurrentFile) {
		FSearchParam->Down = RadioButton_VersLeBas->Checked;
		FSearchParam->SinceCursor = RadioButton_DepuisCurseur->Checked;
		FSearchParam->InSelection = RadioButton_TexteSelectionne->Checked;
		FSearchParam->Loop = CheckBox_Boucler->Checked;
	}
	else {
		FSearchParam->Down = true;
		FSearchParam->InSelection = false;
		FSearchParam->Loop = false;
		bProjectFiles = RadioButton_FichiersProjet->Checked;
		bAllProjectsFiles = RadioButton_FichiersTousProjet->Checked;
		bOpenFiles = RadioButton_FichiersOuverts->Checked;
		bDirectory = RadioButton_Repertoire->Checked;
		if (bCanReplace) {
			FirstFileSearch = CurrentFileSearch;
			FSearchParam->SinceCursor = true;
		}
		else {
			FSearchParam->SinceCursor = false;
		}
		if (bProjectFiles) Config->SearchIn = 0;
		else if (bAllProjectsFiles) Config->SearchIn = 1;
		else if (bOpenFiles) Config->SearchIn = 2;
		else if (bDirectory) {
			Config->SearchIn = 3;
			Config->Recurse = CheckBox_SousRep->Checked;
			Config->SearchDirectory = Edit_Repertoire->Text;
			Config->SearchExtensions = ComboBox_Extensions->Text;
		}
	}

	TStringList *ListFiles = new TStringList();
	TStringList *OpenFiles = new TStringList();
	FVCLIDEAutomation->GetListFiles(OpenFiles);

	if (bCurrentFile) {
		int Index = FVCLIDEAutomation->GetCurFileIndex();
		ListFiles->Add(FVCLIDEAutomation->GetFileName(Index));
	}
	else if (bOpenFiles) {
		FVCLIDEAutomation->GetListFiles(ListFiles);
	}
	else if (bProjectFiles) {
		// $$$
	}
	else if (bAllProjectsFiles) {
		// $$$
	}
	else if (bDirectory) {
		TFileList *FileList = new TFileList(this);
		FileList->Recursive = CheckBox_SousRep->Checked;
		FileList->Filter = ComboBox_Extensions->Text;
		FileList->GetFileList(Edit_Repertoire->Text, true, ListFiles);
		delete FileList;
	}

	TCustomEditor *CustomEditor;
	ProgressBar->Max = ListFiles->Count;
	for (i = FirstFileSearch; i < ListFiles->Count && !bAbort; i++) {

		ProgressBar->Position = i;
		Label_Progress->Caption = ListFiles->Strings[i];
		Application->ProcessMessages();

		// Ne pas utiliser IndexOf car le même fichier peut être ouvert deux fois (sous forme de dialogue et de texte)
		// Index = OpenFiles->IndexOf(ListFiles->Strings[i]);
		Index = -1;
		for (j = 0; j < (int) OpenFiles->Count; j++) {
			FileType = FVCLIDEAutomation->GetFileType(j);
			if (FileType == ftf_TextFile || FileType == ftf_HexaFile) {
				if (!lstrcmpi(OpenFiles->Strings[j], ListFiles->Strings[i])) {
					Index = j;
					break;
				}
			}
		}
		if (Index != -1) {
			switch (FVCLIDEAutomation->GetFileType(Index)) {
				case ftf_TextFile:
					CustomEditor = FVCLIDEAutomation->GetTextFile(Index);
					break;
				case ftf_HexaFile:
					CustomEditor = FVCLIDEAutomation->GetHexaFile(Index);
					break;
				default:
					Index = -1;
					break;
			}
		}
		if (Index == -1) {
			if (IsAsciiFile(ListFiles->Strings[i])) {
				CustomEditor = new TTextEditor(this);
			}
			else {
				CustomEditor = new THexaEditor(this);
			}
			CustomEditor->LoadFromFile(ListFiles->Strings[i]);
		}

		FVCLIDEAutomation->DisplayMessage(td_Message, _T(""));
		if (CustomEditor->Search(asSearch, FSearchParam)) {
			if (Index == -1) {
				// Réouverture du fichier dans l'éditeur (pour visualisation)
				if (FVCLIDEAutomation->OpenFile(ListFiles->Strings[i])) {
					OpenFiles->Clear();
					FVCLIDEAutomation->GetListFiles(OpenFiles);
					// Ne pas utiliser IndexOf car le même fichier peut aussi être ouvert sous forme de dialogue
					// Index = OpenFiles->IndexOf(ListFiles->Strings[i]);
					Index = -1;
					for (j = 0; j < (int) OpenFiles->Count; j++) {
						FileType = FVCLIDEAutomation->GetFileType(j);
						if (FileType == ftf_TextFile || FileType == ftf_HexaFile) {
							if (!lstrcmpi(OpenFiles->Strings[j], ListFiles->Strings[i])) {
								Index = j;
								break;
							}
						}
					}
					if (Index != -1) {
						delete CustomEditor;
						if (FVCLIDEAutomation->GetFileType(Index) == ftf_TextFile) {
							CustomEditor = FVCLIDEAutomation->GetTextFile(Index);
						}
						else {
							CustomEditor = FVCLIDEAutomation->GetHexaFile(Index);
						}
						FVCL_ASSERT(CustomEditor != NULL && "OpenFile a ouvert un fichier qui est ni texte ni hexa");
						CustomEditor->Search(asSearch, FSearchParam);
					}
				}
			}
			FVCLIDEAutomation->ActivateFile(Index);
			NbFound++;
			CurrentFileSearch = i;
			FSearchParam->SinceCursor = true;
			RadioButton_DepuisDebut->Checked = false;
			RadioButton_DepuisCurseur->Checked = true;
			break;
		}

		FSearchParam->SinceCursor = false;

		if (Index == -1) delete CustomEditor;

	}

	delete OpenFiles;
	delete ListFiles;

	EndSearch();
	Screen->Cursor = crDefault;


	if (NbFound > 0) {
		Button_Chercher->Caption = _TT("Suivant");
		bCanReplace = true;
		if (TabControl->TabIndex > 1) Button_Remplacer->Enabled = true;
	}
	else {
		AnsiString asMsg;
		asMsg.sprintf(_TT("Texte \"%s\" non trouvé"), (LPCTSTR) asSearch);
		FVCLIDEAutomation->DisplayMessage(td_Message, asMsg);
		DisplayError(asMsg,
								 _TT("Recherche"),
								 _T("STRING_NOT_FOUND"),
								 MB_OK | MB_ICONINFORMATION);
		Button_Chercher->Caption = _TT("Chercher");
		bCanReplace = false;
		Button_Remplacer->Enabled = false;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::Button_RemplacerClick(TObject *Sender) {
	AnsiString asReplace;


	asReplace = ComboBox_Remplacer->Text;
	Config->PushReplaceString(asReplace);

	TStrings *ReplaceStrings;
	ReplaceStrings = Config->ReplaceStrings;
	ComboBox_Remplacer->Items->Assign(ReplaceStrings);
	ComboBox_Remplacer->ItemIndex = 0;

	TTextEditor *TextEditor = FVCLIDEAutomation->GetCurTextFile();
	if (TextEditor == NULL) return;

	TextEditor->Replace(asReplace, FSearchParam->KeepCase);

	Button_ChercherClick(Sender);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::Button_ToutClick(TObject *Sender) {
	AnsiString asSearch;
	AnsiString asReplace;
	AnsiString asMessage;
	bool bReplace;
	bool bCurrentFile;
	bool bProjectFiles = false;
	bool bAllProjectsFiles = false;
	bool bOpenFiles = false;
	bool bDirectory = false;
	SEARCHPARAM MemoSearchParam;
	int NumLineInit;
	int NumColInit;
	__int64 FilePosInit;
	int NumLine0;
	int NumCol0;
	__int64 FilePos0;
	int NbFound = 0;
	int Index;
	int i;

	if (FSearchParam == NULL) return;

	Screen->Cursor = crHourGlass;
	StartSearch();

	asSearch = ComboBox_Chercher->Text;
	Config->PushSearchString(asSearch);

	TStrings *SearchStrings;
	SearchStrings = Config->SearchStrings;
	ComboBox_Chercher->Items->Assign(SearchStrings);
	ComboBox_Chercher->ItemIndex = 0;

	FVCLIDEAutomation->ClearLogMessage(tl_Search);

	bReplace = (TabControl->TabIndex >= 2);
	bCurrentFile = (TabControl->TabIndex == 0 || TabControl->TabIndex == 2);
	if (bReplace) {
		asReplace = ComboBox_Remplacer->Text;
		Config->PushReplaceString(asReplace);

		TStrings *ReplaceStrings;
		ReplaceStrings = Config->ReplaceStrings;
		ComboBox_Remplacer->Items->Assign(ReplaceStrings);
		ComboBox_Remplacer->ItemIndex = 0;

		asMessage.sprintf(
			_TT("Remplacement de la chaîne \"%s\" par \"%s\""),
			(LPCTSTR) asSearch,
			(LPCTSTR) asReplace);
	}
	else {
		asMessage.sprintf(
			_TT("Recherche chaîne \"%s\""),
			(LPCTSTR) asSearch);
	}
	FVCLIDEAutomation->AddLogMessage(tl_Search, asMessage);

	FSearchParam->WholeWord = CheckBox_MotEntier->Checked;
	FSearchParam->CaseSensitive = CheckBox_MAJmin->Checked;
	FSearchParam->BegLine = CheckBox_DebutLigne->Checked;
	FSearchParam->EndLine = CheckBox_FinLigne->Checked;
	FSearchParam->OnlyInComments = CheckBox_DansCommentaires->Checked;
	FSearchParam->NotInComments = CheckBox_HorsCommentaires->Checked;
	FSearchParam->OnlyInQuotes = CheckBox_DansGuillemets->Checked;
	FSearchParam->NotInQuotes = CheckBox_HorsGuillemets->Checked;
	FSearchParam->RegExp = CheckBox_ExpressionReg->Checked;
	FSearchParam->KeepCase = CheckBox_ConserverCasse->Checked;
	memset(&MemoSearchParam, 0, sizeof(MemoSearchParam));


	if (bCurrentFile) {
		FSearchParam->Down = RadioButton_VersLeBas->Checked;
		FSearchParam->SinceCursor = RadioButton_DepuisCurseur->Checked;
		FSearchParam->InSelection = RadioButton_TexteSelectionne->Checked;
		FSearchParam->Loop = CheckBox_Boucler->Checked;
	}
	else {
		MemoSearchParam = *FSearchParam;
		FSearchParam->Down = true;
		FSearchParam->SinceCursor = false;
		FSearchParam->InSelection = false;
		FSearchParam->Loop = false;
		bProjectFiles = RadioButton_FichiersProjet->Checked;
		bAllProjectsFiles = RadioButton_FichiersTousProjet->Checked;
		bOpenFiles = RadioButton_FichiersOuverts->Checked;
		bDirectory = RadioButton_Repertoire->Checked;
		if (bProjectFiles) Config->SearchIn = 0;
		else if (bAllProjectsFiles) Config->SearchIn = 1;
		else if (bOpenFiles) Config->SearchIn = 2;
		else if (bDirectory) {
			Config->SearchIn = 3;
			Config->Recurse = CheckBox_SousRep->Checked;
			Config->SearchDirectory = Edit_Repertoire->Text;
			Config->SearchExtensions = ComboBox_Extensions->Text;
		}
	}

	TStringList *ListFiles = new TStringList();
	TStringList *OpenFiles = new TStringList();
	FVCLIDEAutomation->GetListFiles(OpenFiles);

	if (bCurrentFile) {
		int Index = FVCLIDEAutomation->GetCurFileIndex();
		ListFiles->Add(FVCLIDEAutomation->GetFileName(Index));
	}
	else if (bOpenFiles) {
		FVCLIDEAutomation->GetListFiles(ListFiles);
	}
	else if (bProjectFiles) {
		// $$$
	}
	else if (bAllProjectsFiles) {
		// $$$
	}
	else if (bDirectory) {
		TFileList *FileList = new TFileList(this);
		FileList->Recursive = CheckBox_SousRep->Checked;
		FileList->Filter = ComboBox_Extensions->Text;
		FileList->GetFileList(Edit_Repertoire->Text, true, ListFiles);
		delete FileList;
	}

	TTextEditor *TextEditor;
	THexaEditor *HexaEditor;
	TCustomEditor *CustomEditor;
	ProgressBar->Max = ListFiles->Count;
	for (i = 0; i < ListFiles->Count && !bAbort; i++) {

		ProgressBar->Position = i;
		Label_Progress->Caption = ListFiles->Strings[i];
		Application->ProcessMessages();

		TextEditor = NULL;
		HexaEditor = NULL;

		Index = OpenFiles->IndexOf(ListFiles->Strings[i]);
		if (Index != -1) {
			switch (FVCLIDEAutomation->GetFileType(Index)) {
				case ftf_TextFile:
					TextEditor = FVCLIDEAutomation->GetTextFile(Index);
					CustomEditor = TextEditor;
					break;
				case ftf_HexaFile:
					HexaEditor = FVCLIDEAutomation->GetHexaFile(Index);
					CustomEditor = HexaEditor;
					break;
				default:
					Index = -1;
					break;
			}
		}
		if (Index == -1) {
			if (IsAsciiFile(ListFiles->Strings[i])) {
				TextEditor = new TTextEditor(this);
				CustomEditor = TextEditor;
			}
			else {
				HexaEditor = new THexaEditor(this);
				CustomEditor = HexaEditor;
			}
			CustomEditor->LoadFromFile(ListFiles->Strings[i]);
		}

		CustomEditor->BeginUpdate();
		if (bReplace) CustomEditor->BeginOperation(_TT("Remplacement systématique"));

		if (!bCurrentFile) FSearchParam->SinceCursor = false;
		NumLine0 = -1;
		NumCol0 = -1;
		FilePos0 = -1;

		// Mémorisation de la position du caret
		if (TextEditor) {
			NumLineInit = TextEditor->NumLine;
			NumColInit = TextEditor->NumCol;
		}
		else {
			FilePosInit = HexaEditor->FilePos;
		}

		while (CustomEditor->Search(asSearch, FSearchParam)) {
			if (TextEditor) {
				int NumLine, NumCol;
				NumLine = TextEditor->NumLine;
				NumCol = TextEditor->NumCol;

				// Système pour éviter de boucler indéfiniment
				if (NumLine0 == -1) {
					NumLine0 = NumLine;
					NumCol0 = NumCol;
				}
				else {
					if (NumLine == NumLine0 && NumCol == NumCol0) break;
				}
				if (bReplace &&
						NumLine == NumLine0 && NumCol <= NumCol0) {
					NumCol0 += asReplace.Length() - asSearch.Length();
				}

				// Message log
				asMessage.sprintf(_T("  %s (%i, %i): %s"),
						(LPCTSTR) ListFiles->Strings[i],
						NumLine + 1,
						NumCol - asSearch.Length() + 1,
						(LPCTSTR) TextEditor->Lines_Strings(TextEditor->NumLine));
			}
			else {
				__int64 FilePos;
				FilePos = HexaEditor->FilePos;

				// Système pour éviter de boucler indéfiniment
				if (FilePos0 == -1) {
					FilePos0 = FilePos;
				}
				else {
					if (FilePos0 == FilePos) break;
				}

				// Message log
				asMessage.sprintf(_T("  %s (%04X:%04X)"),
						(LPCTSTR) ListFiles->Strings[i],
						(int) ((FilePos >> 16) & 0xFFFF),
						(int) (FilePos & 0xFFFF));
			}
			FVCLIDEAutomation->AddLogMessage(tl_Search, asMessage);
			NbFound++;
			if (bReplace) {
				if (Index == -1) {
					// Réouverture du fichier dans l'éditeur (pour bénéficier du UNDO)
					if (FVCLIDEAutomation->OpenFile(ListFiles->Strings[i])) {
						OpenFiles->Clear();
						FVCLIDEAutomation->GetListFiles(OpenFiles);
						Index = OpenFiles->IndexOf(ListFiles->Strings[i]);
						if (Index != -1) {
							delete CustomEditor;
							if (FVCLIDEAutomation->GetFileType(Index) == ftf_TextFile) {
								CustomEditor = FVCLIDEAutomation->GetTextFile(Index);
							}
							else {
								CustomEditor = FVCLIDEAutomation->GetHexaFile(Index);
							}
							FVCL_ASSERT(CustomEditor != NULL && "OpenFile a ouvert un fichier qui est ni texte ni hexa");
							CustomEditor->Search(asSearch, FSearchParam);
						}
					}
				}
				CustomEditor->Replace(asReplace, FSearchParam->KeepCase);
			}
			FSearchParam->SinceCursor = true;
		}

		// Positionnement du caret à sa position initiale
		if (TextEditor) {
			TextEditor->NumLine = NumLineInit;
			TextEditor->NumCol = NumColInit;
		}
		else {
			HexaEditor->FilePos = FilePosInit;
		}

		CustomEditor->EndUpdate();
		if (bReplace) CustomEditor->EndOperation();

		if (Index == -1) delete CustomEditor;

	}

	delete OpenFiles;
	delete ListFiles;

	EndSearch();
	Screen->Cursor = crDefault;
	if (!bCurrentFile) *FSearchParam = MemoSearchParam;


	if (bAbort) {
		asMessage = _TT("Opération interrompue par l'utilisateur");
	}
	else {
		if (bReplace) {
			asMessage.sprintf(
				_TT("Texte \"%s\" remplacé %i fois"),
				(LPCTSTR) asSearch,
				NbFound);
		}
		else {
			asMessage.sprintf(
				_TT("Texte \"%s\" trouvé %i fois"),
				(LPCTSTR) asSearch,
				NbFound);
		}
	}
	FVCLIDEAutomation->AddLogMessage(tl_Search, asMessage);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::Button_AbandonClick(TObject *Sender) {
	bAbort = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SEARCHANDREPLACE);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::FormClose(TObject *Sender,
			TCloseAction &Action) {

	if (bBusy) {
		int Rep;

		Rep = DisplayError(_TT("Voulez-vous abandonner le traitement en cours ?"),
											 _TT("Sortie"),
											 _T("ABORTSEARCH"),
											 MB_YESNO | MB_ICONQUESTION);

		if (Rep == IDYES) bAbort = true;
		Action = caNone;
		return;

	}

	FVCLIDEAutomation->SetFocusOnEditor();
	Action = caHide;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::StartSearch(void) {
	TabControl->Enabled = false;
	ProgressBar->Position = 0;
	Label_Progress->Caption = "";

	Button_Chercher->Visible = false;
  Button_Remplacer->Visible = false;
  Button_Tout->Visible = false;
  Button_Annuler->Visible = false;
  Button_Aide->Visible = false;
	ProgressBar->Visible = true;
  Button_Abandon->Visible = true;
	Label_Progress->Visible = true;

	Application->ProcessMessages();
	bAbort = false;
	bBusy = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SearchAndReplace::EndSearch(void) {
	TabControl->Enabled = true;

  Button_Chercher->Visible = true;
  Button_Remplacer->Visible = true;
  Button_Tout->Visible = true;
  Button_Annuler->Visible = true;
  Button_Aide->Visible = true;
	ProgressBar->Visible = false;
  Button_Abandon->Visible = false;
	Label_Progress->Visible = false;

	bBusy = false;
	FVCLIDEAutomation->SetFocusOnEditor();

}

//---------------------------------------------------------------------------
