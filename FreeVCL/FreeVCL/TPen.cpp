//-----------------------------------------------------------------------------
//! @file TPen.cpp
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
// TPen
//---------------------------------------------------------------------------

TPen::TPen(void): TCustomPen() {
  // Initialisations
  FHandle = NULL;
  bChanged = false;
  LogPen.lopnStyle = psSolid;
  LogPen.lopnWidth.x = 0;
  LogPen.lopnColor = RGB(0, 0, 0);
  GetObject(GetStockObject(BLACK_PEN), sizeof(LOGPEN), &LogPen);
}

TPen::~TPen(void) {
  if (FHandle) DeleteObject(FHandle);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HPEN TPen::Get_Handle(void) {
  if (bChanged) {
    if (FHandle) {
      DeleteObject(FHandle);
      FHandle = NULL;
    }
    bChanged = false;
  }

  if (FHandle == NULL) FHandle = CreatePenIndirect(&LogPen);

  return FHandle;
}

bool TPen::Set_Handle(HPEN NewHandle) {
  if (FHandle != NewHandle) {
    if (FHandle) DeleteObject(FHandle);
    FHandle = NewHandle;
    memset(&LogPen, 0, sizeof(LOGPEN));
    GetObject(NewHandle, sizeof(LOGPEN), &LogPen);
    bChanged = false;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TPenStyle TPen::Get_Style(void) {
  switch (LogPen.lopnStyle) {
  case PS_NULL:
    return psClear;
  case PS_SOLID:
    return psSolid;
  case PS_DASH:
    return psDash;
  case PS_DOT:
    return psDot;
  case PS_DASHDOT:
    return psDashDot;
  case PS_DASHDOTDOT:
    return psDashDotDot;
  case PS_INSIDEFRAME:
    return psInsideFrame;
  }

  return psSolid;
}

bool TPen::Set_Style(TPenStyle NewStyle) {

  switch (NewStyle) {
  case psClear:
    LogPen.lopnStyle = PS_NULL;
    break;
  case psSolid:
    LogPen.lopnStyle = PS_SOLID;
    break;
  case psDash:
    LogPen.lopnStyle = PS_DASH;
    break;
  case psDot:
    LogPen.lopnStyle = PS_DOT;
    break;
  case psDashDot:
    LogPen.lopnStyle = PS_DASHDOT;
    break;
  case psDashDotDot:
    LogPen.lopnStyle = PS_DASHDOTDOT;
    break;
  case psInsideFrame:
    LogPen.lopnStyle = PS_INSIDEFRAME;
    break;
  }

  bChanged = true;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TPen::Get_Width(void) {
  return LogPen.lopnWidth.x;
}

bool TPen::Set_Width(int NewWidth) {
  if (LogPen.lopnWidth.x != NewWidth) {
    LogPen.lopnWidth.x = NewWidth;
    bChanged = true;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TPen::Get_Color(void) {
  return LogPen.lopnColor;
}

bool TPen::Set_Color(TColor NewColor) {
  if (NewColor != LogPen.lopnColor) {
    LogPen.lopnColor = NewColor.cr;
    bChanged = true;
  }
  return true;
}

//---------------------------------------------------------------------------

