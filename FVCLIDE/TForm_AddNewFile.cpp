//-----------------------------------------------------------------------------
//! @file TForm_AddNewFile.cpp
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
#include "TForm_AddNewFile.h"
#include "TFVCLIDEAutomation.h"
#include "HLP_FVCLIDE_Fr.h"

//---------------------------------------------------------------------------
TForm_AddNewFile::TForm_AddNewFile(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {

	
	ListBox_Types->Items->Add(_TT("Fichier header (.h)"));
	ListBox_Types->Items->Add(_TT("Fichier source C (.c)"));
	ListBox_Types->Items->Add(_TT("Fichier source C++ (.cpp)"));
	ListBox_Types->Items->Add(_TT("Définition de classe (.h et .cpp)"));
	ListBox_Types->Items->Add(_TT("Nouvelle unité (.h, .cpp et .dfm)"));
}

//---------------------------------------------------------------------------
TForm_AddNewFile::~TForm_AddNewFile(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_AddNewFile::Button_ParcourirPathClick(TObject *Sender) {
  AnsiString asRep;

  asRep = Edit_Path->Text;
  if (SelectDirectory(_TT("Sélectionnez le répertoire de création"), _T(""), asRep)) {
    Edit_Path->Text = asRep;
    Edit_Path->SetFocus();
  }

}

//---------------------------------------------------------------------------
void TForm_AddNewFile::Button_OkClick(TObject *Sender) {

	if (((AnsiString) Edit_Name->Text).IsEmpty()) {
		FVCLIDEAutomation->DisplayError(_TT("Veuillez donner un nom au fichier."),
																		_TT("Erreur saisie"),
																		_T(""),
																		MB_OK | MB_ICONWARNING);
		Edit_Name->SetFocus();
		return;
	}

	if (ListBox_Types->ItemIndex < 0) {
		FVCLIDEAutomation->DisplayError(_TT("Veuillez choisir un type de fichier."),
																		_TT("Erreur saisie"),
																		_T(""),
																		MB_OK | MB_ICONWARNING);
		ListBox_Types->SetFocus();
		return;
	}

	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void TForm_AddNewFile::Button_AnnulerClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------
void TForm_AddNewFile::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	// $$$ Application->HelpCommand(HELP_CONTEXT, HLP_ADDNEWFILE);
}

//---------------------------------------------------------------------------
