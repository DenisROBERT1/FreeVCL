//-----------------------------------------------------------------------------
//! @file TForm_EdiOptions.cpp
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

#include "TTextEditor.h"
#include "TConfig.h"
#include "TFVCLIDEAutomation.h"
#include "TFrame_EditFichierText.h"
#include "TForm_EdiOptions.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_EdiOptions::TForm_EdiOptions(TComponent *Owner, LPCTSTR szName)
    : TForm(Owner, szName, FVCL_ARG_SUPP) {
  Edit_NbSpacesTab->Text = (int) Config->NbSpacesTab;
  CheckBox_SpacesToTabs->Checked = (bool) Config->SpacesToTabs;
  CheckBox_TabsToSpaces->Checked = (bool) Config->TabsToSpaces;
  CheckBox_LinesNumbers->Checked = (bool) Config->LinesNumbers;
  CheckBox_IconsColumn->Checked = (bool) Config->IconsColumn;
  CheckBox_TypeAuto->Checked = (bool) Config->TypeAuto;
  CheckBox_IndentAuto->Checked = (bool) Config->IndentAuto;
  CheckBox_CurAfterEndLine->Checked = (bool) Config->CurAfterEndLine;
  CheckBox_UndoRedoMoveCur->Checked = (bool) Config->UndoRedoMoveCur;
  CheckBox_MultiLineTabs->Checked = (bool) Config->MultiLineTabs;
  CheckBox_SaveBeforeBuild->Checked = (bool) Config->SaveBeforeBuild;
  CheckBox_Autocompletion->Checked = (bool) Config->Autocompletion;
  CheckBox_AlphaBlend->Checked = (bool) Config->AlphaBlend;

	CheckBox_TabsToSpaces->Enabled = !((bool) CheckBox_SpacesToTabs->Checked);
	CheckBox_SpacesToTabs->Enabled = !((bool) CheckBox_TabsToSpaces->Checked);

}

//---------------------------------------------------------------------------
TForm_EdiOptions::~TForm_EdiOptions(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_EdiOptions::CheckBox_SpacesToTabsClick(TObject *Sender) {
	CheckBox_TabsToSpaces->Enabled = !CheckBox_SpacesToTabs->Checked;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_EdiOptions::CheckBox_TabsToSpacesClick(TObject *Sender) {
	CheckBox_SpacesToTabs->Enabled = !CheckBox_TabsToSpaces->Checked;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_EdiOptions::Button_OkClick(TObject *Sender) {
  Config->NbSpacesTab = ((AnsiString) Edit_NbSpacesTab->Text).ToIntDef(3);
  Config->SpacesToTabs = CheckBox_SpacesToTabs->Checked;
  Config->TabsToSpaces = CheckBox_TabsToSpaces->Checked;
  Config->LinesNumbers = CheckBox_LinesNumbers->Checked;
  Config->IconsColumn = CheckBox_IconsColumn->Checked;
  Config->TypeAuto = CheckBox_TypeAuto->Checked;
  Config->IndentAuto = CheckBox_IndentAuto->Checked;
  Config->CurAfterEndLine = CheckBox_CurAfterEndLine->Checked;
  Config->UndoRedoMoveCur = CheckBox_UndoRedoMoveCur->Checked;
  Config->MultiLineTabs = CheckBox_MultiLineTabs->Checked;
  Config->SaveBeforeBuild = CheckBox_SaveBeforeBuild->Checked;
  Config->Autocompletion = CheckBox_Autocompletion->Checked;
  Config->AlphaBlend = CheckBox_AlphaBlend->Checked;

	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_EdiOptions::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_EDIOPTIONS);
}

//---------------------------------------------------------------------------
