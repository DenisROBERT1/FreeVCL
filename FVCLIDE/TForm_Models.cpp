//-----------------------------------------------------------------------------
//! @file TForm_Models.cpp
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

//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------
#include <FreeVcl.h>
#include <FreeVcl_ToolsEDI.h>


//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------
#include "TConfig.h"
#include "TFrame_FVCLIDEBase.h"
#include "TForm_SaisieModel.h"
#include "TForm_Models.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_Models::TForm_Models(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {
	AnsiString asPathModels;

	asPathModels = (AnsiString) Config->InstallPath + _T("\\Models\\Code");
	DirectoryListBox_Models->Directory = asPathModels;
}

//---------------------------------------------------------------------------
void TForm_Models::DirectoryListBox_Models_Change(TObject *Sender, TTreeNode *TreeNode) {

  if (DirectoryListBox_Models->IsFile(TreeNode)) {
		Button_Insert->Enabled = true;
		Button_Modif->Enabled = true;
		Button_Suppr->Enabled = true;
	}
	else {
		Button_Insert->Enabled = false;
		Button_Modif->Enabled = false;
		Button_Suppr->Enabled = false;
	}
}

//---------------------------------------------------------------------------
void TForm_Models::Button_InsertClick(TObject *Sender) {
	TTreeNode *SelectedNode;
	AnsiString asFileName;
	TStringList *Model;
	TStringList *Variables;
	TStringList *Values;
	AnsiString asLine;
	AnsiString asLineRemp;
	AnsiString asVariable;
	int i, j;
	TCHAR c;
	int DebVar, FinVar;
	bool bVar;
	int Index;


	SelectedNode = DirectoryListBox_Models->Selected;
  if (SelectedNode != NULL) {
		if (DirectoryListBox_Models->IsFile(SelectedNode)) {
			asFileName = DirectoryListBox_Models->GetItemPath(SelectedNode);

			Model = new TStringList();
			Variables = new TStringList();
			Variables->Duplicates = dupIgnore;
			Values = new TStringList();
			if (Model->LoadFromFile(asFileName)) {

				// Recherche des variables
				for (i = 0; i < Model->Count; i++) {
					DebVar = 0;
					FinVar = 0;
					bVar = false;
					asLine = Model->Strings[i];
					for (j = 1; j <= asLine.Length(); j++) {
						// Début de variable = "{{{"
						c = asLine[j];
						if (c == _T('{')) DebVar++;
						else DebVar = 0;
						if (bVar) {
							// Fin de variable = "}}}"
							if (c == _T('}')) FinVar++;
							else {
								FinVar = 0;
								asVariable += c;
							}
							if (FinVar == 3) {
								Variables->Add(asVariable);
								asVariable = _T("");
								bVar = false;
							}
						}
						if (DebVar == 3) bVar = true;
					}
				}

				// Saisie des variables
				if (SaisieParams(Variables, Values)) {

					TTextEditor *TextEditor = FVCLIDEAutomation->GetCurTextFile();
					if (TextEditor) {
						TextEditor->BeginOperation(_TT("Insertion d'un modèle"));
						int NumLine = TextEditor->NumLine;
						for (i = 0; i < Model->Count; i++) {

							// Remplacement des variables
							DebVar = 0;
							FinVar = 0;
							bVar = false;
							asLine = Model->Strings[i];
							asLineRemp = "";
							for (j = 1; j <= asLine.Length(); j++) {
								// Début de variable = "{{{"
								c = asLine[j];
								if (c == _T('{')) DebVar++;
								else DebVar = 0;
								if (bVar) {
									// Fin de variable = "}}}"
									if (c == _T('}')) FinVar++;
									else {
										FinVar = 0;
										asVariable += c;
									}
									if (FinVar == 3) {
										if (Variables->Find(asVariable, Index)) {
											asLineRemp += Values->Strings[Index];
										}
										asVariable = _T("");
										bVar = false;
									}
								}
								else {
									asLineRemp += c;
								}
								if (DebVar == 3) {
									asLineRemp = asLineRemp.SubString(1, asLineRemp.Length() - 3);
									bVar = true;
								}
							}

							// Insertion dans le fichier courant
							TextEditor->Lines_InsertLine(NumLine, asLineRemp);
							NumLine++;
						}
						TextEditor->NumLine = NumLine;
						TextEditor->EndOperation();
						TextEditor->Invalidate();
					}

					Close();

				}

			}
			else {
				AnsiString asError;
				asError.sprintf(_TT("Impossible d'ouvrir le fichier \"%s\""), (LPCTSTR) asFileName);
				FVCLIDEAutomation->DisplaySystemError(asError, Model->LastError, _TT("Ouverture"), _T(""), MB_OK | MB_ICONSTOP);
			}
			delete Values;
			delete Variables;
			delete Model;
		}
		else {
			FVCLIDEAutomation->DisplayError(_TT("Veuillez sélectionner un fichier"), _TT("Application d'un modèle"), _T("NOT_A_FILE"), MB_OK | MB_ICONWARNING);
		}

	}

}

//---------------------------------------------------------------------------
void TForm_Models::Button_NouveauClick(TObject *Sender) {
  TForm_SaisieModel *Form_SaisieModel;
	AnsiString asFilePath;
	AnsiString asFileName;
	TTreeNode *SelectedNode;


	SelectedNode = DirectoryListBox_Models->Selected;
  if (SelectedNode != NULL) {
		asFileName = DirectoryListBox_Models->GetItemPath(SelectedNode);
		if (DirectoryListBox_Models->IsFile(SelectedNode)) {
			asFilePath = ExtractFilePath(asFileName);
		}
		else {
			asFilePath = asFileName + DIR_DELIMITER;
		}
	}
	else {
		asFilePath = (AnsiString) Config->InstallPath + AnsiString(_T("\\Models\\Code\\"));
	}

	Form_SaisieModel = new TForm_SaisieModel(this, _T("TForm_SaisieModel"));
  Form_SaisieModel->FilePath = asFilePath;
  if (Form_SaisieModel->ShowModal() == mrOk) {
	  DirectoryListBox_Models->Update();
	}
  delete Form_SaisieModel;

}

//---------------------------------------------------------------------------
void TForm_Models::Button_ModifClick(TObject *Sender) {
  TForm_SaisieModel *Form_SaisieModel;
	AnsiString asFileName;
	TTreeNode *SelectedNode;


	SelectedNode = DirectoryListBox_Models->Selected;
  if (SelectedNode != NULL) {

		if (DirectoryListBox_Models->IsFile(SelectedNode)) {
			asFileName = DirectoryListBox_Models->GetItemPath(SelectedNode);

			Form_SaisieModel = new TForm_SaisieModel(this, _T("TForm_SaisieModel"));
			Form_SaisieModel->FileName = asFileName;
			if (Form_SaisieModel->ShowModal() == mrOk) {
				DirectoryListBox_Models->Update();
			}
			delete Form_SaisieModel;
		}
  }

}

//---------------------------------------------------------------------------
void TForm_Models::Button_SupprClick(TObject *Sender) {
	AnsiString asFileName;
	TTreeNode *SelectedNode;


	SelectedNode = DirectoryListBox_Models->Selected;
  if (SelectedNode != NULL) {
		if (DirectoryListBox_Models->IsFile(SelectedNode)) {
			asFileName = DirectoryListBox_Models->GetItemPath(SelectedNode);
			DeleteFile(asFileName, true);

			DirectoryListBox_Models->Update();
		}
	}
}

//---------------------------------------------------------------------------
void TForm_Models::Button_SortieClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------
void TForm_Models::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIEMODEL);
}

//---------------------------------------------------------------------------
bool TForm_Models::SaisieParams(TStringList *Variables, TStringList *Values) {
  TForm *Form;
  TLabel *Label = NULL;
  TButton *Button = NULL;
  TEdit *Edit = NULL;
	AnsiString asName;
  int i, y;
	bool bRet;


	if (Variables->Count == 0) return true;

  Form = new TForm(this);

	Values->Clear();
	y = 4;

  for (i = 0; i < (int) Variables->Count; i++) {

		Label = new TLabel(Form);
    Label->Left = 8;
    Label->Top = y + 2;
    Label->Width = 100;
    Label->Height = 13;
		asName.sprintf(_T("Label_%i"), i);
    Label->Name = asName;
    Label->Caption = Variables->Strings[i];

		Edit = new TEdit(Form);
    Edit->Left = 116;
    Edit->Top = y;
    Edit->Width = 200;
    Edit->Height = 21;
		asName.sprintf(_T("Edit_%i"), i);
    Edit->Name = asName;
    Edit->TabOrder = i;

		y += 30;

  }

	y += 10;

	Button = new TButton(Form);
  Button->Left = 92;
  Button->Top = y;
  Button->Width = 60;
  Button->Height = 23;
  Button->Name = _T("Button_Ok");
  Button->Caption = _TT("Ok");
  Button->TabOrder = i;
	Button->Default = true;
  Button->ModalResult = mrOk;

	Button = new TButton(Form);
  Button->Left = 172;
  Button->Top = y;
  Button->Width = 60;
  Button->Height = 23;
  Button->Name = _T("Button_Cancel");
  Button->Caption = _TT("Annuler");
  Button->TabOrder = i + 1;
	Button->Cancel = true;
  Button->ModalResult = mrCancel;

	y += 30;

  Form->Caption = _TT("Paramètres");
	Form->ClientWidth = 324;
  Form->ClientHeight = y;
  Form->BorderStyle = bsDialog;
  Form->Position = poOwnerFormCenter;

	bRet = (Form->ShowModal() == mrOk);

  for (i = 0; i < (int) Variables->Count; i++) {
		Edit = dynamic_cast<TEdit *> (Form->Components[2 * i + 1]);
		if (Edit) Values->Add(Edit->Text);
	}

	delete Form;

	return bRet;
}

//---------------------------------------------------------------------------
