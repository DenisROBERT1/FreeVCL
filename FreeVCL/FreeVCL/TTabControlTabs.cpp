//-----------------------------------------------------------------------------
//! @file TTabControlTabs.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
// extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TTabControlTabs
//---------------------------------------------------------------------------

TTabControlTabs::TTabControlTabs(void): TStrings() {
	FHandle = NULL;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TTabControlTabs::Get_Handle(void) {
  return FHandle;
}

bool TTabControlTabs::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
int FASTCALL TTabControlTabs::GetCount(void) const {
  return (int) SendMessage(FHandle, TCM_GETITEMCOUNT, 0, 0);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TTabControlTabs::Get_Strings(int i) {
  LPTSTR szString;
  AnsiString asRetour;
  TCITEM TcItem;


  if (FHandle) {
    szString = new TCHAR[256];
    memset(&TcItem, 0, sizeof(TCITEM));
    TcItem.mask = TCIF_TEXT;
    TcItem.cchTextMax = 256;
    TcItem.pszText = szString;
    SendMessage(FHandle, TCM_GETITEM, i, (LPARAM) (LPTCITEM) &TcItem);
    asRetour = AnsiString(szString);
    delete[] szString;
  }

  return asRetour;
}

//---------------------------------------------------------------------------
bool TTabControlTabs::Set_Strings(int i, AnsiString NewStrings) {
  TCITEM TcItem;


  if (FHandle) {
    TcItem.mask = TCIF_TEXT;
    TcItem.pszText = const_cast<LPTSTR>((LPCTSTR) NewStrings);
    SendMessage(FHandle, TCM_SETITEM, i, (LPARAM) (LPTCITEM) &TcItem);
		InvalidateRect(FHandle, NULL, TRUE);
  }

  return true;
}

//---------------------------------------------------------------------------
void TTabControlTabs::Clear(void) {
  NMHDR nmhdr;


	if (FHandle) {
		SendMessage(FHandle, TCM_DELETEALLITEMS, 0, 0);
    nmhdr.hwndFrom = FHandle;
    nmhdr.idFrom = GetWindowLongPtr(FHandle, GWL_ID);
    nmhdr.code = TCN_SELCHANGE;
    SendMessage(FHandle, WM_NOTIFY, (WPARAM) nmhdr.idFrom, (LPARAM) &nmhdr);
	}
}

//---------------------------------------------------------------------------
int TTabControlTabs::Add(const AnsiString S) {
  TCITEM TcItem;
  int Index;


  if (FHandle) {
		TcItem.mask = TCIF_TEXT;
		TcItem.pszText = const_cast<LPTSTR>((LPCTSTR) S);
		Index = (int) SendMessage(FHandle, TCM_GETITEMCOUNT, 0, 0) + 1;
    SendMessage(FHandle, TCM_INSERTITEM, Index, (LPARAM) (LPTCITEM) &TcItem);
    InvalidateRect(FHandle, NULL, TRUE);
  }

  return true;
}

//---------------------------------------------------------------------------
void TTabControlTabs::Insert(int Index, const AnsiString S) {
  TCITEM TcItem;
  int TabIndex;
  NMHDR nmhdr;


  if (FHandle) {
    TabIndex = (int) SendMessage(FHandle, TCM_GETCURSEL, 0, 0);
		TcItem.mask = TCIF_TEXT;
		TcItem.pszText = const_cast<LPTSTR>((LPCTSTR) S);
    SendMessage(FHandle, TCM_INSERTITEM, Index, (LPARAM) (LPTCITEM) &TcItem);
		if (Index <= TabIndex) TabIndex++;
    SendMessage(FHandle, TCM_SETCURSEL, TabIndex, 0);
    nmhdr.hwndFrom = FHandle;
    nmhdr.idFrom = GetWindowLongPtr(FHandle, GWL_ID);
    nmhdr.code = TCN_SELCHANGE;
    SendMessage(FHandle, WM_NOTIFY, (WPARAM) nmhdr.idFrom, (LPARAM) &nmhdr);
  }

}

//---------------------------------------------------------------------------
void TTabControlTabs::Delete(int Index) {
  int TabIndex;
  NMHDR nmhdr;


  if (FHandle) {
    TabIndex = (int) SendMessage(FHandle, TCM_GETCURSEL, 0, 0);
    SendMessage(FHandle, TCM_DELETEITEM, Index, 0);
    if (TabIndex >= SendMessage(FHandle, TCM_GETITEMCOUNT, 0, 0)) TabIndex--;
    SendMessage(FHandle, TCM_SETCURSEL, TabIndex, 0);
    nmhdr.hwndFrom = FHandle;
    nmhdr.idFrom = GetWindowLongPtr(FHandle, GWL_ID);
    nmhdr.code = TCN_SELCHANGE;
    SendMessage(FHandle, WM_NOTIFY, (WPARAM) nmhdr.idFrom, (LPARAM) &nmhdr);
  }
}

//---------------------------------------------------------------------------

