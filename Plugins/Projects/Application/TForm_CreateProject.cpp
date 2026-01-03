//-----------------------------------------------------------------------------
//! @file TForm_CreateProject.cpp
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

#include "TForm_CreateProject.h"

//---------------------------------------------------------------------------
TForm_CreateProject::TForm_CreateProject(HWND hWnd, LPCTSTR szName)
  : TForm(hWnd, szName, FVCL_ARG_SUPP) {

}

//---------------------------------------------------------------------------
void FASTCALL TForm_CreateProject::Button_ParcourirPathClick(TObject *Sender) {
  AnsiString asRep;
	TSelectDirOpts SelectDirOpts;

  asRep = Edit_Path->Text;
	SelectDirOpts.Clear();
	SelectDirOpts << sdAllowCreate << sdPerformCreate << sdPrompt << sdShowEdit;
  if (SelectDirectory(_TT("Sélectionnez le répertoire de création"), _T(""), asRep, SelectDirOpts)) {
    Edit_Path->Text = asRep;
    Edit_Path->SetFocus();
  }

}

//---------------------------------------------------------------------------
void FASTCALL TForm_CreateProject::TButton_ParcourirPathAppliVCLClick(TObject *Sender) {
  AnsiString asRep;
	TSelectDirOpts SelectDirOpts;

  asRep = Edit_PathAppliVCL->Text;
	SelectDirOpts.Clear();
	SelectDirOpts << sdPrompt << sdShowEdit;
  if (SelectDirectory(_TT("Sélectionnez le répertoire du projet VCL"), _T(""), asRep, SelectDirOpts)) {
    Edit_PathAppliVCL->Text = asRep;
    Edit_PathAppliVCL->SetFocus();
  }

}

//---------------------------------------------------------------------------
void FASTCALL TForm_CreateProject::RadioButton_ModeleClick(TObject *Sender) {
	Label_Modele->Enabled = true;
	ComboBox_Modele->Enabled = true;
	Label_PathAppliVCL->Enabled = false;
	Edit_PathAppliVCL->Enabled = false;
	Button_ParcourirPathAppliVCL->Enabled = false;	
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CreateProject::RadioButton_AppliVCLClick(TObject *Sender) {
	Label_Modele->Enabled = false;
	ComboBox_Modele->Enabled = false;
	Label_PathAppliVCL->Enabled = true;
	Edit_PathAppliVCL->Enabled = true;
	Button_ParcourirPathAppliVCL->Enabled = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CreateProject::Button_AideClick(TObject *Sender) {
}

//---------------------------------------------------------------------------
