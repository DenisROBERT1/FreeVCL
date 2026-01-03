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
}

//---------------------------------------------------------------------------
TForm_Principale::~TForm_Principale(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::MenuItem_QuitterClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Button_OkClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------

