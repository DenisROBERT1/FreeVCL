//-----------------------------------------------------------------------------
//! @file TBrush.cpp
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
// TBrush
//---------------------------------------------------------------------------

TBrush::TBrush(void): TCustomBrush() {
  // Initialisations
  FHandle = NULL;
  bChanged = false;
  LogBrush.lbStyle = bsSolid;
  LogBrush.lbColor = RGB(0, 0, 0);
  GetObject(GetStockObject(BLACK_BRUSH), sizeof(LOGBRUSH), &LogBrush);
}

TBrush::~TBrush(void) {
  if (FHandle) DeleteObject(FHandle);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HBRUSH TBrush::Get_Handle(void) {
  if (bChanged) {
    if (FHandle) {
      DeleteObject(FHandle);
      FHandle = NULL;
    }
    bChanged = false;
  }

  if (FHandle == NULL) FHandle = CreateBrushIndirect(&LogBrush);

  return FHandle;
}

bool TBrush::Set_Handle(HBRUSH NewHandle) {
  if (FHandle != NewHandle) {
    if (FHandle) DeleteObject(FHandle);
    FHandle = NewHandle;
    memset(&LogBrush, 0, sizeof(LOGBRUSH));
    GetObject(NewHandle, sizeof(LOGBRUSH), &LogBrush);
    bChanged = false;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TBrushStyle TBrush::Get_Style(void) {
  switch (LogBrush.lbStyle) {
  case BS_SOLID:
    return bsSolid;
  case BS_NULL:
    return bsClear;
  case BS_HATCHED:
    switch (LogBrush.lbHatch) {
    case HS_HORIZONTAL:
      return bsHorizontal;
    case HS_VERTICAL:
      return bsVertical;
    case HS_FDIAGONAL:
      return bsFDiagonal;
    case HS_BDIAGONAL:
      return bsBDiagonal;
    case HS_CROSS:
      return bsCross;
    case HS_DIAGCROSS:
      return bsDiagCross;
    }
  }

  return bsSolid;
}

bool TBrush::Set_Style(TBrushStyle NewStyle) {

  switch (NewStyle) {
  case bsSolid:
    LogBrush.lbStyle = BS_SOLID;
    break;
  case bsClear:
    LogBrush.lbStyle = BS_NULL;
    break;
  case bsHorizontal:
    LogBrush.lbStyle = BS_HATCHED;
    LogBrush.lbStyle = HS_HORIZONTAL;
    break;
  case bsVertical:
    LogBrush.lbStyle = BS_HATCHED;
    LogBrush.lbStyle = HS_VERTICAL;
    break;
  case bsFDiagonal:
    LogBrush.lbStyle = BS_HATCHED;
    LogBrush.lbStyle = HS_FDIAGONAL;
    break;
  case bsBDiagonal:
    LogBrush.lbStyle = BS_HATCHED;
    LogBrush.lbStyle = HS_BDIAGONAL;
    break;
  case bsCross:
    LogBrush.lbStyle = BS_HATCHED;
    LogBrush.lbStyle = HS_CROSS;
    break;
  case bsDiagCross:
    LogBrush.lbStyle = BS_HATCHED;
    LogBrush.lbStyle = HS_DIAGCROSS;
    break;
  }

  bChanged = true;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TBrush::Get_Color(void) {
  return LogBrush.lbColor;
}

bool TBrush::Set_Color(TColor NewColor) {
  if (NewColor != LogBrush.lbColor) {
    LogBrush.lbColor = NewColor.cr;
    bChanged = true;
  }
  return true;
}

//---------------------------------------------------------------------------

