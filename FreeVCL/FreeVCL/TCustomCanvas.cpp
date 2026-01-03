//-----------------------------------------------------------------------------
//! @file TCustomCanvas.cpp
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
// TCustomCanvas
//---------------------------------------------------------------------------

TCustomCanvas::TCustomCanvas(void): TPersistent() {
  // Initialisations
}

TCustomCanvas::~TCustomCanvas(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Pen
//---------------------------------------------------------------------------

TCustomPen *TCustomCanvas::Get_Pen(void) {
  return NULL;
}

bool TCustomCanvas::Set_Pen(TCustomPen *NewPen) {
  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Brush
//---------------------------------------------------------------------------

TCustomBrush *TCustomCanvas::Get_Brush(void) {
  return NULL;
}

bool TCustomCanvas::Set_Brush(TCustomBrush *NewBrush) {
  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

TCustomFont *TCustomCanvas::Get_Font(void) {
  return NULL;
}

bool TCustomCanvas::Set_Font(TCustomFont *NewFont) {
  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TextAlignH
//---------------------------------------------------------------------------

TAlignment TCustomCanvas::Get_TextAlignH(void) {
  return (TAlignment) 0;
}

bool TCustomCanvas::Set_TextAlignH(TAlignment NewTextAlignH) {
  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TextAlignV
//---------------------------------------------------------------------------

TAlignmentV TCustomCanvas::Get_TextAlignV(void) {
  return (TAlignmentV) 0;
}

bool TCustomCanvas::Set_TextAlignV(TAlignmentV NewTextAlignV) {
  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TextFlags
//---------------------------------------------------------------------------

TTextFlags TCustomCanvas::Get_TextFlags(void) {
  return tfNull;
}

bool TCustomCanvas::Set_TextFlags(TTextFlags NewTextFlags) {
  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Pixels
//---------------------------------------------------------------------------

TColor TCustomCanvas::Get_Pixels(int X, int Y) {
  return 0;
}

//---------------------------------------------------------------------------
bool TCustomCanvas::Set_Pixels(int X, int Y, TColor crColor) {
  return false;
}

//---------------------------------------------------------------------------
//
// Paramètres graphiques
//
//---------------------------------------------------------------------------

bool FASTCALL TCustomCanvas::DPtoLP(TPoint * lpPoints, int nCount) {
  int i;
  int XL, YL;

  for (i = 0; i < nCount; i++) {
    if (DPtoLP(lpPoints[i].x, lpPoints[i].y, &XL, &YL) == FALSE) return false;
    lpPoints[i].x = XL;
    lpPoints[i].y = YL;
  }

  return true;
}


//---------------------------------------------------------------------------
bool FASTCALL TCustomCanvas::LPtoDP(TPoint * lpPoints, int nCount) {
  int i;
  int XD, YD;

  for (i = 0; i < nCount; i++) {
    if (LPtoDP(lpPoints[i].x, lpPoints[i].y, &XD, &YD) == FALSE) return false;
    lpPoints[i].x = XD;
    lpPoints[i].y = YD;
  }

  return true;
}


//---------------------------------------------------------------------------
//
// Primitives graphiques
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Accesseurs de la propriété ClipRect
//---------------------------------------------------------------------------

TRect TCustomCanvas::Get_ClipRect(void) {
  return TRect(0, 0, 0, 0);
}

bool TCustomCanvas::Set_ClipRect(TRect NewClipRect) {
  return false;
}


//---------------------------------------------------------------------------
// Primitives graphiques
//---------------------------------------------------------------------------

void FASTCALL TCustomCanvas::Rectangle(const TRect &Rect) {
  Rectangle(const_cast<TRect &>(Rect).Left,
            const_cast<TRect &>(Rect).Top,
            const_cast<TRect &>(Rect).Right,
            const_cast<TRect &>(Rect).Bottom);
}

//---------------------------------------------------------------------------
void FASTCALL TCustomCanvas::RoundRect(const TRect &Rect, int X3, int Y3) {
  RoundRect(const_cast<TRect &>(Rect).Left,
            const_cast<TRect &>(Rect).Top,
            const_cast<TRect &>(Rect).Right,
            const_cast<TRect &>(Rect).Bottom,
            X3, Y3);
}

//---------------------------------------------------------------------------
void FASTCALL TCustomCanvas::FillRect(const TRect &Rect) {
  FillRect(const_cast<TRect &>(Rect).Left,
           const_cast<TRect &>(Rect).Top,
           const_cast<TRect &>(Rect).Right,
           const_cast<TRect &>(Rect).Bottom);
}

//---------------------------------------------------------------------------
void FASTCALL TCustomCanvas::Ellipse(const TRect &Rect) {
  Ellipse(const_cast<TRect &>(Rect).Left,
          const_cast<TRect &>(Rect).Top,
          const_cast<TRect &>(Rect).Right,
          const_cast<TRect &>(Rect).Bottom);
}

//---------------------------------------------------------------------------
void FASTCALL TCustomCanvas::StretchDraw(const TRect &Rect, TBitmap *Bitmap) {
  StretchDraw(const_cast<TRect &>(Rect).Left,
              const_cast<TRect &>(Rect).Top,
              const_cast<TRect &>(Rect).Right,
              const_cast<TRect &>(Rect).Bottom,
              Bitmap);
}

//---------------------------------------------------------------------------

