//-----------------------------------------------------------------------------
//! @file Unit1.cpp
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
#include "Unit1.h"

//---------------------------------------------------------------------------
TUnit1::TUnit1(TComponent* Owner, LPTSTR szName)
        : TForm(Owner, szName, FVCL_ARG_SUPP) {
}

//---------------------------------------------------------------------------
void TUnit1::CallEvent(AnsiString asEvent, TObject *Sender, ...) {
	BYTE *pArgs;


	pArgs = (BYTE *) &Sender;
	pArgs += sizeof(TObject *);

	if (asEvent == _T("Button_AddRootClick")) Button_AddRootClick(Sender);
	else if (asEvent == _T("Button_AddChildClick")) Button_AddChildClick(Sender);
	else if (asEvent == _T("Button_AddClick")) Button_AddClick(Sender);
	else if (asEvent == _T("Button_DeleteClick")) Button_DeleteClick(Sender);

}
//---------------------------------------------------------------------------

void FASTCALL TUnit1::Button_AddRootClick(TObject *Sender) {
  if (Edit1->Text != _T("")) {
    TreeView1->Items->Add(NULL, Edit1->Text);
  }
}
//---------------------------------------------------------------------------
void FASTCALL TUnit1::Button_AddChildClick(TObject *Sender) {
  if (Edit1->Text != _T("")) {
    TreeView1->Items->AddChild(TreeView1->Selected, Edit1->Text);
  }
}
//---------------------------------------------------------------------------
void FASTCALL TUnit1::Button_AddClick(TObject *Sender) {
  if (Edit1->Text != _T("")) {
    TreeView1->Items->Add(TreeView1->Selected, Edit1->Text);
  }
}
//---------------------------------------------------------------------------
void FASTCALL TUnit1::Button_DeleteClick(TObject *Sender) {
  TreeView1->Items->Delete(TreeView1->Selected);
}
//---------------------------------------------------------------------------
