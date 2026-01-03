//-----------------------------------------------------------------------------
//! @file TListColumns.cpp
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
// TListColumns
//---------------------------------------------------------------------------

TListColumns::TListColumns(void): TPersistent() {
  FHandle = NULL;
}

//---------------------------------------------------------------------------
TListColumns::~TListColumns(void) {
	Clear();
	FItems.clear();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TListColumns::Get_Handle(void) {
  return FHandle;
}

bool TListColumns::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
int TListColumns::Get_Count(void) {
  return (int) FItems.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Items
//---------------------------------------------------------------------------

TListColumn *TListColumns::Get_Items(int i) {
  return FItems[i];
}

//---------------------------------------------------------------------------
bool TListColumns::Set_Items(int i, TListColumn *NewItems) {
  LVCOLUMN LVColumn;
  AnsiString sCaption;


  FItems[i] = NewItems;
  if (FHandle) {
    memset(&LVColumn, 0, sizeof(LVCOLUMN));
    LVColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    LVColumn.fmt = LVCFMT_CENTER;
    LVColumn.cx = NewItems->Width;
    sCaption = NewItems->Caption;
    LVColumn.pszText = const_cast<TCHAR *>((LPCTSTR) sCaption);
    LVColumn.cchTextMax = 0;
    LVColumn.iSubItem = 0;
    SendMessage(FHandle, LVM_SETCOLUMN, (WPARAM) i, (LPARAM) &LVColumn);
  }

  return true;
}

//---------------------------------------------------------------------------
void TListColumns::Clear(void) {
  int i;


  for (i = (int) FItems.size() - 1; i >= 0; i--) Delete(i);
}

//---------------------------------------------------------------------------
TListColumn *TListColumns::Add(void) {
  TListColumn *ListColumn;
  LVCOLUMN LVColumn;
  int Index;


  Index = (int) FItems.size();
  ListColumn = new TListColumn();
  ListColumn->Caption = _T("");
  ListColumn->Width = 50;
  ListColumn->NumColumn = (int) FItems.size();
  FItems.push_back(ListColumn);
  if (FHandle) {
    ListColumn->Handle = FHandle;
    memset(&LVColumn, 0, sizeof(LVCOLUMN));
    LVColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    LVColumn.fmt = LVCFMT_LEFT;
    LVColumn.cx = 50;
    LVColumn.pszText = const_cast<TCHAR *>(_T(""));
    LVColumn.cchTextMax = 0;
    LVColumn.iSubItem = 0;
    SendMessage(FHandle, LVM_INSERTCOLUMN, (WPARAM) Index, (LPARAM) &LVColumn);
  }

  return ListColumn;
}

//---------------------------------------------------------------------------
TListColumn *TListColumns::Add(const AnsiString S, int Width) {
  TListColumn *ListColumn;
  LVCOLUMN LVColumn;
  int Index;


  Index = (int) FItems.size();
  ListColumn = new TListColumn();
  ListColumn->Caption = S;
  ListColumn->Width = Width;
  ListColumn->NumColumn = (int) FItems.size();
  FItems.push_back(ListColumn);
  if (FHandle) {
    ListColumn->Handle = FHandle;
    memset(&LVColumn, 0, sizeof(LVCOLUMN));
    LVColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    LVColumn.fmt = LVCFMT_LEFT;
    LVColumn.cx = Width;
    LVColumn.pszText = const_cast<TCHAR *>((LPCTSTR) S);
    LVColumn.cchTextMax = 0;
    LVColumn.iSubItem = 0;
    SendMessage(FHandle, LVM_INSERTCOLUMN, (WPARAM) Index, (LPARAM) &LVColumn);
  }

  return ListColumn;
}

//---------------------------------------------------------------------------
void TListColumns::Insert(int Index, const AnsiString S, int Width) {
  TListColumn *ListColumn;
  LVCOLUMN LVColumn;
  int i;


  ListColumn = new TListColumn();
  ListColumn->Caption = S;
  ListColumn->Width = Width;
  FItems.insert(FItems.begin() + Index, ListColumn);
  for (i = Index; i < (int) FItems.size(); i++) FItems[i]->NumColumn = i;
  if (FHandle) {
    ListColumn->Handle = FHandle;
    memset(&LVColumn, 0, sizeof(LVCOLUMN));
    LVColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    LVColumn.fmt = LVCFMT_CENTER;
    LVColumn.cx = Width;
    LVColumn.pszText = const_cast<TCHAR *>((LPCTSTR) S);
    LVColumn.cchTextMax = 0;
    LVColumn.iSubItem = 0;
    SendMessage(FHandle, LVM_INSERTCOLUMN, (WPARAM) Index, (LPARAM) &LVColumn);
  }

}

//---------------------------------------------------------------------------
void TListColumns::Delete(int Index) {
  int i;


  delete FItems[Index];
  FItems.erase(FItems.begin() + Index);
  if (FHandle) SendMessage(FHandle, LVM_DELETECOLUMN, Index, 0);
  for (i = Index; i < (int) FItems.size(); i++) FItems[i]->NumColumn = i;
}


//---------------------------------------------------------------------------

