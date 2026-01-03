//-----------------------------------------------------------------------------
//! @file TForm_Tools.cpp
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
#include "TForm_Tools.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_Tools::TForm_Tools(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {

  MAJListBox();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Tools::Button_NouveauClick(TObject *Sender) {
  TForm_SaisieTools *Form_SaisieTools;
  int i;


  Form_SaisieTools = new TForm_SaisieTools(this, _T("TForm_SaisieTools"));
  if (Form_SaisieTools->ShowModal() == mrOk) {
    MAJListBox();
    i = ListBox_Tools->Items->Count - 1;
    ListBox_Tools->ItemIndex = i;
  }
  delete Form_SaisieTools;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Tools::Button_ModifClick(TObject *Sender) {
  TForm_SaisieTools *Form_SaisieTools;
  int i;


  i = ListBox_Tools->ItemIndex;
  if (i != -1) {
    Form_SaisieTools = new TForm_SaisieTools(this, _T("TForm_SaisieTools"));
    Form_SaisieTools->IdTool = i;
    if (Form_SaisieTools->ShowModal() == mrOk) {
      MAJListBox();
      ListBox_Tools->ItemIndex = i;
    }
    delete Form_SaisieTools;
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Tools::Button_SupprClick(TObject *Sender) {
  int i;
  const USERTOOL *UserTool;


  i = ListBox_Tools->ItemIndex;
  if (i != -1) {
    UserTool = Config->UserTools[i];
    Config->DeleteUserTool(UserTool->Name);
    MAJListBox();
    if (i >= ListBox_Tools->Items->Count) i--;
    ListBox_Tools->ItemIndex = i;
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Tools::Button_SortieClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Tools::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIETOOLS);
}

//---------------------------------------------------------------------------
void TForm_Tools::MAJListBox(void) {
  int i;
  const USERTOOL *UserTool;


  ListBox_Tools->Items->Clear();
  for (i = 0; i < (int) Config->UserToolsCount; i++) {
    UserTool = Config->UserTools[i];
    ListBox_Tools->Items->Add(UserTool->Name);
  }

}
//---------------------------------------------------------------------------
