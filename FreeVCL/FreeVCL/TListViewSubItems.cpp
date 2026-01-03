//-----------------------------------------------------------------------------
//! @file TListViewSubItems.cpp
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
#include <TListViewSubItems.h>



//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TListViewSubItems
//---------------------------------------------------------------------------

TListViewSubItems::TListViewSubItems(void): TStrings() {
  FHandle = NULL;
  FNumItem = -1;
  NbSubItems = 1;  // Parce que ListItem contient toujours un Caption (même vide)
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TListViewSubItems::Get_Handle(void) {
  return FHandle;
}

bool TListViewSubItems::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NumItem
//---------------------------------------------------------------------------

int TListViewSubItems::Get_NumItem(void) {
  return FNumItem;
}

bool TListViewSubItems::Set_NumItem(int NewNumItem) {

  if (FNumItem != NewNumItem) {
    FNumItem = NewNumItem;
  }
  return true;
}

//---------------------------------------------------------------------------
int FASTCALL TListViewSubItems::GetCount(void) const {
  return NbSubItems;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TListViewSubItems::Get_Strings(int i) {
  TCHAR szString[256];  // $$$
  LVITEM LVItem;


  if (FHandle) {
    memset(&LVItem, 0, sizeof(LVITEM));
    szString[0] = _T('\0');
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = 0;
    LVItem.iSubItem = i;
    LVItem.pszText = szString;
    LVItem.cchTextMax = 256;
    SendMessage(FHandle, LVM_GETITEMTEXT, FNumItem, (LPARAM) (LVITEM *) &LVItem);
  }

  return AnsiString(szString);
}

//---------------------------------------------------------------------------
bool TListViewSubItems::Set_Strings(int i, AnsiString NewStrings) {
  LVITEM LVItem;


  if (FHandle) {
    memset(&LVItem, 0, sizeof(LVITEM));
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = 0;
    LVItem.iSubItem = i;
    LVItem.pszText = const_cast<TCHAR *>((LPCTSTR) NewStrings);
    LVItem.cchTextMax = 0;
    SendMessage(FHandle, LVM_SETITEMTEXT, FNumItem, (LPARAM) (LVITEM *) &LVItem);
  }
  if (NbSubItems < i + 1) NbSubItems = i + 1;

  return true;
}

//---------------------------------------------------------------------------
void TListViewSubItems::Clear(void) {
  int i;


  if (FHandle) {
    for (i = NbSubItems - 1; i >= 0; i--) Delete(i);
  }
}

//---------------------------------------------------------------------------
int TListViewSubItems::Add(const AnsiString S) {
  LVITEM LVItem;


  if (FHandle) {
    memset(&LVItem, 0, sizeof(LVITEM));
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = 0;
    LVItem.iSubItem = NbSubItems;
    LVItem.pszText = const_cast<TCHAR *>((LPCTSTR) S);
    LVItem.cchTextMax = 0;
    SendMessage(FHandle, LVM_SETITEMTEXT, FNumItem, (LPARAM) (LVITEM *) &LVItem);
  }
  NbSubItems++;

  return true;
}

//---------------------------------------------------------------------------
void TListViewSubItems::Insert(int Index, const AnsiString S) {
  LVITEM LVItem;


  if (FHandle) {
    memset(&LVItem, 0, sizeof(LVITEM));
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = 0;
    LVItem.iSubItem = Index;
    LVItem.pszText = const_cast<TCHAR *>((LPCTSTR) S);
    LVItem.cchTextMax = 0;
    SendMessage(FHandle, LVM_SETITEMTEXT, FNumItem, (LPARAM) (LVITEM *) &LVItem);
  }
  NbSubItems++;

}

//---------------------------------------------------------------------------
void TListViewSubItems::Delete(int Index) {
  LVITEM LVItem;


  if (FHandle) {
    memset(&LVItem, 0, sizeof(LVITEM));
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = 0;
    LVItem.iSubItem = Index;
    LVItem.pszText = const_cast<TCHAR *>(_T(""));
    LVItem.cchTextMax = 0;
    SendMessage(FHandle, LVM_SETITEMTEXT, FNumItem, (LPARAM) (LVITEM *) &LVItem);
  }
  NbSubItems--;
}


//---------------------------------------------------------------------------

