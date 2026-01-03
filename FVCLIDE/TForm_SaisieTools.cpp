//-----------------------------------------------------------------------------
//! @file TForm_SaisieTools.cpp
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

#include "TConfig.h"
#include "TForm_SaisieTools.h"
#include "TFVCLIDEAutomation.h"
#include "HLP_FVCLIDE_Fr.h"

//---------------------------------------------------------------------------
TForm_SaisieTools::TForm_SaisieTools(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {

  FIdTool = -1;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété IdTool
//---------------------------------------------------------------------------

int TForm_SaisieTools::Get_IdTool() {
  return FIdTool;
}

bool TForm_SaisieTools::Set_IdTool(int NewIdTool) {
  const USERTOOL *UserTool;


  if (FIdTool != NewIdTool) {
    FIdTool = NewIdTool;

		UserTool = Config->UserTools[FIdTool];
		Edit_Name->Text = UserTool->Name;
		Edit_Prog->Text = UserTool->Prog;
		Edit_Args->Text = UserTool->Args;
		Edit_InitDir->Text = UserTool->InitDir;
  }

  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieTools::Edit_ProgExit(TObject *Sender) {

	if (((AnsiString) Edit_InitDir->Text).IsEmpty() && !((AnsiString) Edit_Prog->Text).IsEmpty()) {
		Edit_InitDir->Text = ExtractFilePath((AnsiString) Edit_Prog->Text);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieTools::Edit_ArgsExit(TObject *Sender) {
	TCHAR szProgName[MAX_PATH];


	if (((AnsiString) Edit_Prog->Text).IsEmpty() && !((AnsiString) Edit_Args->Text).IsEmpty()) {
		if (FindExecutable(((AnsiString) Edit_Args->Text), NULL, szProgName) > (HINSTANCE) 32) {
			Edit_Prog->Text = szProgName;
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieTools::Button_BrowseProgClick(TObject *Sender) {

	if (OpenDialog_Prog->Execute()) {
		Edit_Prog->Text = (AnsiString) OpenDialog_Prog->FileName;
		Edit_Prog->SetFocus();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieTools::Button_BrowseArgsClick(TObject *Sender) {

	if (OpenDialog_Args->Execute()) {
		Edit_Args->Text = (AnsiString) OpenDialog_Args->FileName;
		Edit_Args->SetFocus();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieTools::Button_BrowseDirClick(TObject *Sender) {
  AnsiString asRep;

  if (SelectDirectory(_TT("Répertoire d'exécution"), _T(""), asRep)) {
    Edit_InitDir->Text = asRep;
    Edit_InitDir->SetFocus();
  }

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieTools::Button_OkClick(TObject *Sender) {
  const USERTOOL *OldTools;
  USERTOOL NewTools;
	AnsiString asMsg;
  int i;


	for (i = 0; i < Config->UserToolsCount; i++) {
		if (i != FIdTool) {
			OldTools = Config->UserTools[i];
			if ((AnsiString) OldTools->Name == (AnsiString) Edit_Name->Text) {
				asMsg.sprintf(_TT("Outil déjà existant (%s),\nvoulez-vous le remplacer ?"),
											(LPCTSTR) OldTools->Name);
				if (FVCLIDEAutomation->DisplayError(asMsg,
																						_TT("Attention"),
																						_T("REPLACETOOL"),
																						MB_YESNO | MB_ICONQUESTION) != IDYES) return;
				Config->DeleteUserTool(OldTools->Name);
				break;
			}
		}
	}

  // Destruction ancien outil
	if (FIdTool != -1) {
		OldTools = Config->UserTools[FIdTool];
		Config->DeleteUserTool(OldTools->Name);
	}

	// Insertion outil
	Config->AddUserTool(Edit_Name->Text, Edit_Prog->Text, Edit_Args->Text, Edit_InitDir->Text);

  // Sortie
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieTools::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIETOOLS);
}

//---------------------------------------------------------------------------
