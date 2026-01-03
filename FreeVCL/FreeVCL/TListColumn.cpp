//-----------------------------------------------------------------------------
//! @file TListColumn.cpp
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
// TListColumn
//---------------------------------------------------------------------------

TListColumn::TListColumn(void): TPersistent() {
  FWidth = 50;
  FNumColumn = -1;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TListColumn::Get_Handle(void) {
  return FHandle;
}

bool TListColumn::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

AnsiString TListColumn::Get_Caption(void) {
  return FCaption;
}

bool TListColumn::Set_Caption(AnsiString NewCaption) {
  LVCOLUMN LVColumn;


  if (FCaption != NewCaption) {
    FCaption = NewCaption;
    if (FHandle) {
      memset(&LVColumn, 0, sizeof(LVCOLUMN));
      LVColumn.mask = LVCF_TEXT;
      LVColumn.pszText = const_cast<TCHAR *>((LPCTSTR) FCaption);
      LVColumn.cchTextMax = 0;
      LVColumn.iSubItem = 0;
      SendMessage(FHandle, LVM_SETCOLUMN, (WPARAM) NumColumn, (LPARAM) &LVColumn);
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TListColumn::Get_Width(void) {
  return FWidth;
}

bool TListColumn::Set_Width(int NewWidth) {
  LVCOLUMN LVColumn;


  if (FWidth != NewWidth) {
    FWidth = NewWidth;
    if (FHandle) {
      memset(&LVColumn, 0, sizeof(LVCOLUMN));
      LVColumn.mask = LVCF_WIDTH;
      LVColumn.cx = FWidth;
      LVColumn.cchTextMax = 0;
      LVColumn.iSubItem = 0;
      SendMessage(FHandle, LVM_SETCOLUMN, (WPARAM) NumColumn, (LPARAM) &LVColumn);
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TListColumn::Get_Alignment(void) {
  return FAlignment;
}

bool TListColumn::Set_Alignment(TAlignment NewAlignment) {
  LVCOLUMN LVColumn;


  if (FAlignment != NewAlignment) {
    FAlignment = NewAlignment;
    if (FHandle) {
      memset(&LVColumn, 0, sizeof(LVCOLUMN));
      LVColumn.mask = LVCF_FMT;
      switch (FAlignment) {
        case taLeftJustify: LVColumn.fmt = LVCFMT_LEFT; break;
        case taCenter: LVColumn.fmt = LVCFMT_CENTER; break;
        case taRightJustify: LVColumn.fmt = LVCFMT_RIGHT; break;
      }
      LVColumn.cchTextMax = 0;
      LVColumn.iSubItem = 0;
      SendMessage(FHandle, LVM_SETCOLUMN, (WPARAM) NumColumn, (LPARAM) &LVColumn);
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NumColumn
//---------------------------------------------------------------------------

int TListColumn::Get_NumColumn(void) {
  return FNumColumn;
}

bool TListColumn::Set_NumColumn(int NewNumColumn) {

  if (FNumColumn != NewNumColumn) {
    FNumColumn = NewNumColumn;
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TListColumn::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Caption"));
  ListProperties->Add(_T("Width"));
  ListProperties->Add(_T("Alignment"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TListColumn::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Caption")) {
    return tpText;
  }
  if (asProperty == _T("Width")) {
    return tpNumber;
  }
  if (asProperty == _T("Alignment")) {
    *asInfos = _T("taLeftJustify\ntaRightJustify\ntaCenter");
    return tpChoice;
  }
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TListColumn::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Caption")) {
    return _T("");
  }
  if (asProperty == _T("Width")) {
    return _T("50");
  }
  if (asProperty == _T("Alignment")) {
    return _T("taLeftJustify");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TListColumn::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Caption")) {
    return Get_Caption();
  }
  if (asProperty == _T("Width")) {
    return IntToStr(Get_Width());
  }
  if (asProperty == _T("Alignment")) {
    TAlignment Alignment = Get_Alignment();
    if (Alignment == taLeftJustify) return _T("taLeftJustify");
    if (Alignment == taRightJustify) return _T("taRightJustify");
    if (Alignment == taCenter) return _T("taCenter");
    return _T("taLeftJustify");
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TListColumn::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Caption")) {
    Set_Caption(asValue);
    return true;
  }
  if (asProperty == _T("Width")) {
    Set_Width(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Alignment")) {
    Set_Alignment((TAlignment) StrToInt(asValue));
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

