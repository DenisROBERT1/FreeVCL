//-----------------------------------------------------------------------------
//! @file TListItem.cpp
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
// TListItem
//---------------------------------------------------------------------------

TListItem::TListItem(TListItems* AOwner): TPersistent() {
  Owner = AOwner;
  FHandle = NULL;
  FCaption = _T("");
  FChecked = false;
  FSelected = false;
  FNumItem = -1;
  FSubItems = new TListViewSubItems();
  FData = NULL;
}

TListItem::~TListItem(void) {
  delete FSubItems;
}

//---------------------------------------------------------------------------
void FASTCALL TListItem::Delete(void) {
  if (Owner) Owner->Delete(FNumItem);
  else delete this;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TListItem::Get_Handle(void) {
  return FHandle;
}

bool TListItem::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
    FSubItems->Handle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

AnsiString TListItem::Get_Caption(void) {
  return FCaption;
}

bool TListItem::Set_Caption(AnsiString NewCaption) {

  if (FCaption != NewCaption) {
    FCaption = NewCaption;
    // FSubItems->Strings[0] = NewCaption; $$$ ne marche pas (bug connu)
    FSubItems->Set_Strings(0, NewCaption);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Checked
//---------------------------------------------------------------------------

bool TListItem::Get_Checked(void) {
  return FChecked;
}

bool TListItem::Set_Checked(bool NewChecked) {

  if (FChecked != NewChecked) {
    FChecked = NewChecked;
    if (FHandle) {
      ListView_SetCheckState(FHandle, FNumItem, NewChecked)
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Selected
//---------------------------------------------------------------------------

bool TListItem::Get_Selected(void) {
  return FSelected;
}

bool TListItem::Set_Selected(bool NewSelected) {
  LVITEM LVItem;


  if (FSelected != NewSelected) {
    FSelected = NewSelected;
    if (FHandle) {
      memset(&LVItem, 0, sizeof(LVITEM));
      LVItem.mask = LVIF_STATE;
      LVItem.state = NewSelected? LVIS_SELECTED: 0;
      LVItem.stateMask = LVIS_SELECTED;
      SendMessage(FHandle, LVM_SETITEMSTATE, FNumItem, (LPARAM) &LVItem);
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NumItem
//---------------------------------------------------------------------------

int TListItem::Get_NumItem(void) {
  return FNumItem;
}

bool TListItem::Set_NumItem(int NewNumItem) {

  if (FNumItem != NewNumItem) {
    FNumItem = NewNumItem;
    FSubItems->NumItem = NewNumItem;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SubItems
//---------------------------------------------------------------------------


TListViewSubItems *TListItem::Get_SubItems(void) {
  return FSubItems;
}

bool TListItem::Set_SubItems(TListViewSubItems *NewSubItems) {
  if (FSubItems != NewSubItems) {
    FSubItems = NewSubItems;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Data
//---------------------------------------------------------------------------

void * TListItem::Get_Data(void) {
  return FData;
}

bool TListItem::Set_Data(void *NewData) {
  if (FData != NewData) {
    FData = NewData;
  }
  return true;
}


//---------------------------------------------------------------------------

