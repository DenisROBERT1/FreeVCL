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

#include "TForm_Principale.h"

//---------------------------------------------------------------------------
TForm_Principale::TForm_Principale(HWND hWndParent, LPCTSTR szName)
  : TForm(hWndParent, szName, FVCL_ARG_SUPP) {
  Factory_FVCL_Multimed.Init();

  bCapture = false;
  CheckBox_Loop->Checked = (Animate->Repetitions == 0);
}

//---------------------------------------------------------------------------
TForm_Principale::~TForm_Principale(void) {
}

//---------------------------------------------------------------------------
void TForm_Principale::MenuItem_OuvrirClick(TObject *Sender) {

  if (OpenDialog->Execute()) {
    Animate->FileName = (AnsiString) OpenDialog->FileName;
  }
}

//---------------------------------------------------------------------------
void TForm_Principale::MenuItem_PlayClick(TObject *Sender) {
  Animate->Play(1, 0, CheckBox_Loop->Checked? 0: 1);
}

//---------------------------------------------------------------------------
void TForm_Principale::MenuItem_StopClick(TObject *Sender) {
  Animate->Stop();
}

//---------------------------------------------------------------------------
void TForm_Principale::MenuItem_QuitterClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------

