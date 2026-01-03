//-----------------------------------------------------------------------------
//! @file TComboBoxItems.cpp
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


//---------------------------------------------------------------------------
// TComboBoxItems
//---------------------------------------------------------------------------

TComboBoxItems::TComboBoxItems(void): TStrings() {
  FHandle = NULL;
}

TComboBoxItems::~TComboBoxItems(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TComboBoxItems::Get_Handle(void) {
  return FHandle;
}

bool TComboBoxItems::Set_Handle(HWND NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
int FASTCALL TComboBoxItems::GetCount(void) const {
  if (FHandle) return (int) SendMessage(FHandle, CB_GETCOUNT, 0, 0);

  return 0;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TComboBoxItems::Get_Strings(int i) {
  int Len;
  LPTSTR szString;
  AnsiString Retour;


  Len = (int) SendMessage(FHandle, CB_GETLBTEXTLEN, i, 0);
  if (Len != CB_ERR) {
    szString = new TCHAR[Len + 1];
    SendMessage(FHandle, CB_GETLBTEXT, i, (LPARAM) (LPSTR) szString);
    Retour = AnsiString(szString);
    delete[] szString;
  }

  return Retour;
}

//---------------------------------------------------------------------------
bool TComboBoxItems::Set_Strings(int i, AnsiString NewStrings) {
  if (FHandle) {
    SendMessage(FHandle, CB_DELETESTRING, i, 0);
    SendMessage(FHandle, CB_INSERTSTRING, i, (LPARAM) (LPCTSTR) NewStrings);
		OnChange(this);
  }

  return true;
}

//---------------------------------------------------------------------------
void TComboBoxItems::Clear(void) {

  if (FHandle) {
		SendMessage(FHandle, CB_RESETCONTENT, 0, 0);
		OnChange(this);
	}
}

//---------------------------------------------------------------------------
int TComboBoxItems::Add(const AnsiString S) {
  if (FHandle) {
		SendMessage(FHandle, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) S);
		OnChange(this);
	}

  return true;
}

//---------------------------------------------------------------------------
void TComboBoxItems::Insert(int Index, const AnsiString S) {
  if (FHandle) {
		SendMessage(FHandle, CB_INSERTSTRING, 0, (LPARAM) (LPCTSTR) S);
		OnChange(this);
	}
}

//---------------------------------------------------------------------------
void TComboBoxItems::Delete(int Index) {
  if (FHandle) {
		SendMessage(FHandle, CB_DELETESTRING, Index, 0);
		OnChange(this);
	}
}

//---------------------------------------------------------------------------

