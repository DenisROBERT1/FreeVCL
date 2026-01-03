//-----------------------------------------------------------------------------
//! @file TDragImageList.cpp
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
// TDragImageList
//---------------------------------------------------------------------------

TDragImageList::TDragImageList(TComponent* AOwner): TCustomImageList(AOwner) {
  // Initialisations
  FClassName = _T("TDragImageList");
  FDragCursor = (TCursor) 0;
  FDragging = false;
}

TDragImageList::~TDragImageList(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DragCursor
//---------------------------------------------------------------------------

TCursor TDragImageList::Get_DragCursor(void) {
  return FDragCursor;
}

bool TDragImageList::Set_DragCursor(TCursor NewDragCursor) {
  if (FDragCursor != NewDragCursor) {
    FDragCursor = NewDragCursor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Dragging
//---------------------------------------------------------------------------

bool TDragImageList::Get_Dragging(void) {
  return FDragging;
}

bool TDragImageList::Set_Dragging(bool NewDragging) {
  if (FDragging != NewDragging) {
    FDragging = NewDragging;
  }
  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TDragImageList::BeginDrag(HWND Window, int X, int Y) {
  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TDragImageList::DragLock(HWND Window, int XPos, int YPos) {
  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TDragImageList::DragMove(int X, int Y) {
  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TDragImageList::DragUnlock(void) {
}

//---------------------------------------------------------------------------
bool FASTCALL TDragImageList::EndDrag(void) {
  return true;
}

//---------------------------------------------------------------------------
TPoint FASTCALL TDragImageList::GetHotSpot(void) {
  return TPoint(0, 0);
}

//---------------------------------------------------------------------------
void FASTCALL TDragImageList::HideDragImage(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TDragImageList::Initialize(void) {
}

//---------------------------------------------------------------------------
bool FASTCALL TDragImageList::SetDragImage(int Index, int HotSpotX, int HotSpotY) {
  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TDragImageList::ShowDragImage(void) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDragImageList::GetListProperties(TStrings *ListProperties) {

  TCustomImageList::GetListProperties(ListProperties);

  ListProperties->Add(_T("DragCursor"));
  ListProperties->Add(_T("Dragging"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDragImageList::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("DragCursor")) {
    return tpNumber;
  }
  if (asProperty == _T("Dragging")) {
    return tpBool;
  }
  return TCustomImageList::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDragImageList::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("DragCursor")) {
    return _T("O");
  }
  if (asProperty == _T("Dragging")) {
    return _T("False");
  }
  return TCustomImageList::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDragImageList::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("DragCursor")) {
    return IntToStr(Get_DragCursor());
  }
  if (asProperty == _T("Dragging")) {
    if (Get_Dragging()) return _T("True");
    else return _T("False");
  }
  return TCustomImageList::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDragImageList::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  if (asProperty == _T("DragCursor")) {
    Set_DragCursor((TCursor) StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Dragging")) {
    Set_Dragging(asValue == _T("True"));
    return true;
  }
  return TCustomImageList::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

