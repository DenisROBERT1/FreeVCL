//-----------------------------------------------------------------------------
//! @file TListItems.cpp
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
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TListItems
//---------------------------------------------------------------------------

TListItems::TListItems(TListView* AOwner): TPersistent() {
	FOwner = AOwner;
  FHandle = NULL;
}

//---------------------------------------------------------------------------

TListItems::~TListItems(void) {
  Clear();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TListItems::Get_Handle(void) {
  return FHandle;
}

bool TListItems::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Item
//---------------------------------------------------------------------------

TListItem *TListItems::Get_Item(int i) {

	FVCL_ASSERT(0 <= i && i < (int) FItem.size() && "TListItems::operator[]: Index out of string");

	return FItem[i];
}

bool TListItems::Set_Item(int i, TListItem * NewItem) {
  LVITEM LVItem;
  AnsiString sCaption;


  FVCL_ASSERT(0 <= i && i < (int) FItem.size() && "TListItems::operator[]: Index out of string");

	if (FItem[i] != NewItem) {
    delete FItem[i];
    FItem[i] = NewItem;
    if (FHandle) {
      FItem[i]->Handle = FHandle;
      memset(&LVItem, 0, sizeof(LVITEM));
      sCaption = NewItem->Caption;
      LVItem.mask = LVIF_TEXT;
      LVItem.iItem = i;
      LVItem.iSubItem = 0;
      LVItem.pszText = const_cast<TCHAR *>((LPCTSTR) sCaption);
      LVItem.cchTextMax = 0;
      LVItem.lParam = (LPARAM) NewItem;
      SendMessage(FHandle, LVM_INSERTITEM, 0, (LPARAM) &LVItem);
    }
  }
  return true;
}

//---------------------------------------------------------------------------
int TListItems::Get_Count(void) {
  return (int) FItem.size();
}

//---------------------------------------------------------------------------
void TListItems::Clear(void) {
  int i;


	FOwner->ItemIndex = -1;
  for (i = 0; i < (int) FItem.size(); i++) {
    delete FItem[i];
  }
  FItem.clear();
  if (FHandle) SendMessage(FHandle, LVM_DELETEALLITEMS, 0, 0);
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListItems::Add(void) {
  LVITEM LVItem;
  TListItem * NewItem;


  NewItem = new TListItem(this);
  NewItem->NumItem = (int) FItem.size();
  FItem.push_back(NewItem);
  if (FHandle) {
    NewItem->Handle = FHandle;
    memset(&LVItem, 0, sizeof(LVITEM));
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = (int) FItem.size();
    LVItem.iSubItem = 0;
    LVItem.pszText = const_cast<TCHAR *>(_T(""));
    LVItem.cchTextMax = 0;
    LVItem.lParam = (LPARAM) NewItem;
    SendMessage(FHandle, LVM_INSERTITEM, 0, (LPARAM) &LVItem);
  }

  return NewItem;
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListItems::Add(const AnsiString S) {
  LVITEM LVItem;
  TListItem * NewItem;


  NewItem = new TListItem(this);
  NewItem->Caption = S;
  NewItem->NumItem = (int) FItem.size();
  FItem.push_back(NewItem);
  if (FHandle) {
    NewItem->Handle = FHandle;
    memset(&LVItem, 0, sizeof(LVITEM));
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = (int) FItem.size();
    LVItem.iSubItem = 0;
    LVItem.pszText = const_cast<TCHAR *>((LPCTSTR) S);
    LVItem.cchTextMax = 0;
    LVItem.lParam = (LPARAM) NewItem;
    SendMessage(FHandle, LVM_INSERTITEM, 0, (LPARAM) &LVItem);
  }

  return NewItem;
}

//---------------------------------------------------------------------------
void FASTCALL TListItems::Insert(int Index) {
  Insert(Index, _T(""));
}

//---------------------------------------------------------------------------
void FASTCALL TListItems::Insert(int Index, const AnsiString S) {
  LVITEM LVItem;
  TListItem * NewItem;
  int i;


  NewItem = new TListItem(this);
  NewItem->Caption = S;
  FItem.insert(FItem.begin() + Index, NewItem);
  for (i = Index; i < (int) FItem.size(); i++) FItem[i]->NumItem = i;
  if (FHandle) {
    NewItem->Handle = FHandle;
    memset(&LVItem, 0, sizeof(LVITEM));
    LVItem.mask = LVIF_TEXT;
    LVItem.iItem = Index;
    LVItem.iSubItem = 0;
    LVItem.pszText = const_cast<TCHAR *>((LPCTSTR) S);
    LVItem.cchTextMax = 0;
    LVItem.lParam = (LPARAM) NewItem;
    SendMessage(FHandle, LVM_INSERTITEM, 0, (LPARAM) &LVItem);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TListItems::Delete(int Index) {
  int i;


  FItem[Index]->Selected = false;
  delete FItem[Index];
  FItem.erase(FItem.begin() + Index);
  for (i = Index; i < (int) FItem.size(); i++) FItem[i]->NumItem = i;
  if (FHandle) SendMessage(FHandle, LVM_DELETEITEM, Index, 0);
}

//---------------------------------------------------------------------------

