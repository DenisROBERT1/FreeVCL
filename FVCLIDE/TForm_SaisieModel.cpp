//-----------------------------------------------------------------------------
//! @file TForm_SaisieModel.cpp
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
#include <FreeVcl_ToolsEDI.h>

#include "TConfig.h"
#include "TFVCLIDEAutomation.h"
#include "TFrame_EditFichierText.h"
#include "TForm_SaisieModel.h"
#include "HLP_FVCLIDE_Fr.h"

//---------------------------------------------------------------------------
TForm_SaisieModel::TForm_SaisieModel(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {

	FFilePath = (AnsiString) Config->InstallPath + AnsiString(_T("\\Models\\Code\\"));
}

//---------------------------------------------------------------------------
TForm_SaisieModel::~TForm_SaisieModel(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FileName
//---------------------------------------------------------------------------

AnsiString TForm_SaisieModel::Get_FileName(void) {
  return FFileName;
}

bool TForm_SaisieModel::Set_FileName(AnsiString NewFileName) {
	AnsiString asName;
	AnsiString asExt;
	int PosPoint;


  if (FFileName != NewFileName) {
		Frame_EditFichierText->FileName = NewFileName;
		if (Frame_EditFichierText->Open()) {
			FFileName = NewFileName;
			FFilePath = ExtractFilePath(FFileName);
			asName = ExtractFileName(FFileName);
			PosPoint = asName.LastDelimiter(_T("."));
			if (PosPoint != 0) {
				Edit_Name->Text = asName.SubString(1, PosPoint - 1);
				asExt = asName.SubString(PosPoint + 1, asName.Length() - PosPoint);
			}
			else {
				Edit_Name->Text = asName;
			}
			ComboBox_Langage->Text = asExt;
			ComboBox_LangageChange(this);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FilePath
//---------------------------------------------------------------------------

AnsiString TForm_SaisieModel::Get_FilePath(void) {
  return FFilePath;
}

bool TForm_SaisieModel::Set_FilePath(AnsiString NewFilePath) {

  if (FFilePath != NewFilePath) {
		FFilePath = NewFilePath;
  }

	return true;
}


//---------------------------------------------------------------------------
void TForm_SaisieModel::ComboBox_LangageChange(TObject *Sender) {
  AnsiString asExt;


	// Mots-clé et paramètres
	asExt = ComboBox_Langage->Text;
	Frame_EditFichierText->SetLanguage(asExt);

}

//---------------------------------------------------------------------------
void TForm_SaisieModel::Button_OkClick(TObject *Sender) {
  AnsiString asFileName;
	AnsiString asMsg;


	asFileName = FFilePath +
								(AnsiString) Edit_Name->Text + AnsiString(_T(".")) +
								(AnsiString) ComboBox_Langage->Text;

	if (Frame_EditFichierText->Changed || FFileName != asFileName) {

		if (FFileName != asFileName) {
			if (FileExists(asFileName)) {
				asMsg.sprintf(_TT("Modèle déjà existant (%s),\nvoulez-vous le remplacer ?"),
											(LPCTSTR) asFileName);
				if (FVCLIDEAutomation->DisplayError(asMsg,
																						_TT("Attention"),
																						_T("REPLACEMODEL"),
																						MB_YESNO | MB_ICONQUESTION) != IDYES) return;
			}
		}

		if (!Frame_EditFichierText->SaveToFile(asFileName)) return;  // L'erreur est affichée dans SaveToFile

	}

	// Sortie
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void TForm_SaisieModel::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIEMODEL);
}

//---------------------------------------------------------------------------
