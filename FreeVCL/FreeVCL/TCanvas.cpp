//-----------------------------------------------------------------------------
//! @file TCanvas.cpp
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
// TCanvas
//---------------------------------------------------------------------------

TCanvas::TCanvas(void): TCustomCanvas() {
  // Initialisations
  FHandle = NULL;
  FPen = new TPen();
  FBrush = new TBrush();
  FFont = new TFont();
  FTextAlign = TA_LEFT | TA_TOP;
  FTextFlags = tfTransparent;
  hClipRect = NULL;

}

TCanvas::~TCanvas(void) {
  delete FPen;
  delete FBrush;
  delete FFont;
  if (hClipRect) DeleteObject(hClipRect);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HDC TCanvas::Get_Handle(void) {
  return FHandle;
}

bool TCanvas::Set_Handle(HDC NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
    if (hClipRect) {
      DeleteObject(hClipRect);
      hClipRect = NULL;
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Pen
//---------------------------------------------------------------------------

TCustomPen *TCanvas::Get_Pen(void) {
  return FPen;
}

bool TCanvas::Set_Pen(TCustomPen *NewPen) {
  FPen->Assign(NewPen);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Brush
//---------------------------------------------------------------------------

TCustomBrush *TCanvas::Get_Brush(void) {
  return FBrush;
}

bool TCanvas::Set_Brush(TCustomBrush *NewBrush) {
  FBrush->Assign(NewBrush);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

TCustomFont *TCanvas::Get_Font(void) {
  return FFont;
}

bool TCanvas::Set_Font(TCustomFont *NewFont) {
  FFont->Assign(NewFont);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TextAlignH
//---------------------------------------------------------------------------

TAlignment TCanvas::Get_TextAlignH(void) {
  switch (FTextAlign & (TA_LEFT | TA_CENTER | TA_RIGHT)) {
    case TA_LEFT: return taLeftJustify;
    case TA_CENTER: return taCenter;
    case TA_RIGHT: return taRightJustify;
  }

  return taLeftJustify;
}

bool TCanvas::Set_TextAlignH(TAlignment NewTextAlignH) {

  FTextAlign &= ~(TA_LEFT | TA_CENTER | TA_RIGHT);

  switch (NewTextAlignH) {
    case taLeftJustify: FTextAlign |= TA_LEFT; break;
    case taCenter: FTextAlign |= TA_CENTER; break;
    case taRightJustify: FTextAlign |= TA_RIGHT; break;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TextAlignV
//---------------------------------------------------------------------------

TAlignmentV TCanvas::Get_TextAlignV(void) {
  switch (FTextAlign & (TA_TOP | TA_BASELINE | TA_BOTTOM)) {
    case TA_TOP: return taTop;
    case TA_BASELINE: return taBaseLine;
    case TA_BOTTOM: return taBottom;
  }

  return taTop;
}

bool TCanvas::Set_TextAlignV(TAlignmentV NewTextAlignV) {

  FTextAlign &= ~(TA_TOP | TA_BASELINE | TA_BOTTOM);

  switch (NewTextAlignV) {
    case taTop: FTextAlign |= TA_TOP; break;
    case taBaseLine: FTextAlign |= TA_BASELINE; break;
    case taBottom: FTextAlign |= TA_BOTTOM; break;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TextFlags
//---------------------------------------------------------------------------

TTextFlags TCanvas::Get_TextFlags(void) {
  return FTextFlags;
}

bool TCanvas::Set_TextFlags(TTextFlags NewTextFlags) {
  if (FTextFlags != NewTextFlags) {
    FTextFlags = NewTextFlags;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Pixels
//---------------------------------------------------------------------------

TColor TCanvas::Get_Pixels(int X, int Y) {
	HDC AHandle = Get_Handle();
  return ::GetPixel(AHandle, X, Y);
}

//---------------------------------------------------------------------------
bool TCanvas::Set_Pixels(int X, int Y, TColor crColor) {
	HDC AHandle = Get_Handle();
  ::SetPixel(AHandle, X, Y, crColor.cr);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ClipRect
//---------------------------------------------------------------------------

TRect TCanvas::Get_ClipRect(void) {
  return FClipRect;
}

bool TCanvas::Set_ClipRect(TRect NewClipRect) {
  if (FClipRect != NewClipRect) {
    FClipRect = NewClipRect;
    if (hClipRect == NULL) {
      hClipRect = CreateRectRgnIndirect(&NewClipRect);
    }
    else {
      SetRectRgn(hClipRect,
                 NewClipRect.Left, NewClipRect.Top,
                 NewClipRect.Right, NewClipRect.Bottom);
    }
    SelectObject(Handle, hClipRect);
  }
  return true;
}

//---------------------------------------------------------------------------
//
// Paramètres graphiques
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Lecture de l'origine des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::GetWindowOrg(int *X, int *Y) {
  POINT Pt;

	HDC AHandle = Get_Handle();
  if (GetWindowOrgEx(AHandle, &Pt) == FALSE) return false;

  *X = Pt.x;
  *Y = Pt.y;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'origine des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::SetWindowOrg(int X, int Y) {

	HDC AHandle = Get_Handle();
  if (SetWindowOrgEx(AHandle, X, Y, NULL) == FALSE) return false;

  return true;
}

//---------------------------------------------------------------------------
// Lecture de l'origine des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::GetViewportOrg(int *X, int *Y) {
  POINT Pt;

	HDC AHandle = Get_Handle();
  if (GetViewportOrgEx(AHandle, &Pt) == FALSE) return false;

  *X = Pt.x;
  *Y = Pt.y;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'origine des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::SetViewportOrg(int X, int Y) {

	HDC AHandle = Get_Handle();
  if (SetViewportOrgEx(AHandle, X, Y, NULL) == FALSE) return false;

  return true;
}

//---------------------------------------------------------------------------
// Lecture de l'échelle des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::GetWindowExt(int *XExt, int *YExt) {
  SIZE Size;

	HDC AHandle = Get_Handle();
  if (GetWindowExtEx(AHandle, &Size) == FALSE) return false;

  *XExt = Size.cx;
  *YExt = Size.cy;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'échelle des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::SetWindowExt(int XExt, int YExt) {

	HDC AHandle = Get_Handle();
  if (SetWindowExtEx(AHandle, XExt, YExt, NULL) == FALSE) return false;

  return true;
}

//---------------------------------------------------------------------------
// Lecture de l'échelle des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::GetViewportExt(int *XExt, int *YExt) {
  SIZE Size;

	HDC AHandle = Get_Handle();
  if (GetViewportExtEx(AHandle, &Size) == FALSE) return false;

  *XExt = Size.cx;
  *YExt = Size.cy;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'échelle des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::SetViewportExt(int XExt, int YExt) {

	HDC AHandle = Get_Handle();
  if (SetViewportExtEx(AHandle, XExt, YExt, NULL) == FALSE) return false;

  return true;
}


//---------------------------------------------------------------------------
// Conversion de coordonnées dispositif vers coordonnées logiques.
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::DPtoLP(int XD, int YD, int *XL, int *YL) {
  POINT Pt;

  Pt.x = XD;
  Pt.y = YD;

	HDC AHandle = Get_Handle();
  if (::DPtoLP(AHandle, &Pt, 1) == FALSE) return false;

  *XL = Pt.x;
  *YL = Pt.y;

  return true;
}


//---------------------------------------------------------------------------
// Conversion de coordonnées logiques vers coordonnées dispositif.
//---------------------------------------------------------------------------

bool FASTCALL TCanvas::LPtoDP(int XL, int YL, int *XD, int *YD) {
  POINT Pt;

  Pt.x = XL;
  Pt.y = YL;

	HDC AHandle = Get_Handle();
  if (::LPtoDP(AHandle, &Pt, 1) == FALSE) return false;

  *XD = Pt.x;
  *YD = Pt.y;

  return true;
}


//---------------------------------------------------------------------------
//
// Primitives graphiques
//
//---------------------------------------------------------------------------

void FASTCALL TCanvas::MoveTo(int X, int Y) {
	HDC AHandle = Get_Handle();
  ::MoveToEx(AHandle, X, Y, NULL);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::LineTo(int X, int Y) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FPen->Handle);
  ::LineTo(AHandle, X, Y);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::Polyline(TPoint *Polyline, int NbPts) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FPen->Handle);
  ::Polyline(AHandle, (LPPOINT) Polyline, NbPts);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::Polygon(TPoint *Polygon, int NbPts) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FPen->Handle);
  SelectObject(AHandle, FBrush->Handle);
  ::Polygon(AHandle, (LPPOINT) Polygon, NbPts);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::Rectangle(int X1, int Y1, int X2, int Y2) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FPen->Handle);
  SelectObject(AHandle, FBrush->Handle);
  ::Rectangle(AHandle, X1, Y1, X2, Y2);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::RoundRect(int X1, int Y1, int X2, int Y2, int X3, int Y3) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FPen->Handle);
  SelectObject(AHandle, FBrush->Handle);
  ::RoundRect(AHandle, X1, Y1, X2, Y2, X3, Y3);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::FillRect(int X1, int Y1, int X2, int Y2) {
  RECT Rect;

  SetRect(&Rect, X1, Y1, X2, Y2);
	HDC AHandle = Get_Handle();
  ::FillRect(AHandle, &Rect, FBrush->Handle);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::Ellipse(int X1, int Y1, int X2, int Y2) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FPen->Handle);
  SelectObject(AHandle, FBrush->Handle);
  ::Ellipse(AHandle, X1, Y1, X2, Y2);
}

//---------------------------------------------------------------------------
bool FASTCALL TCanvas::CopyRect(TRect RectDest, TCanvas *Canvas, TRect RectSrc) {
	HDC AHandle = Get_Handle();
  return (StretchBlt(AHandle,
                     RectDest.Left, RectDest.Top,
                     RectDest.Width, RectDest.Height,
                     Canvas->Handle,
                     RectSrc.Left, RectSrc.Top,
                     RectSrc.Width, RectSrc.Height,
                     SRCCOPY) != 0);
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::TextOut(int X, int Y, const AnsiString asText) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FFont->Handle);
  ::SetTextAlign(AHandle, FTextAlign);
  ::SetBkMode(AHandle, (FTextFlags == tfOpaque)? OPAQUE: TRANSPARENT);
  if (FTextFlags == tfOpaque) ::SetBkColor(AHandle, ((TColor) FBrush->Color).cr);
  ::SetTextColor(AHandle, ((TColor) FFont->Color).cr);
  ::SetTextCharacterExtra(AHandle, FFont->CharacterExtra);
	::TextOut(AHandle, X, Y, asText, asText.Length());
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::Draw(int X, int Y, TBitmap* Bitmap) {

  TCanvas* Canvas = (TCanvas *) Bitmap->Canvas;
  if (Canvas) {
		HDC AHandle = Get_Handle();
    BitBlt(AHandle,
           X, Y,
           Bitmap->Width, Bitmap->Height,
           Canvas->Handle,
           0, 0,
           SRCCOPY);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TCanvas::StretchDraw(int X1, int Y1, int X2, int Y2, TBitmap *Bitmap) {

  TCanvas* Canvas = dynamic_cast<TCanvas *>((TCanvas *) Bitmap->Canvas);
  if (Canvas) {
		HDC AHandle = Get_Handle();
    ::StretchBlt(AHandle,
                 X1, Y1,
                 X2 - X1, Y2 - Y1,
                 Canvas->Handle,
                 0, 0,
                 Bitmap->Width, Bitmap->Height,
                 SRCCOPY);
  }
}

//---------------------------------------------------------------------------
bool FASTCALL TCanvas::HandleAllocated(void) {
  return (FHandle != NULL);
}

//---------------------------------------------------------------------------
bool FASTCALL TCanvas::GetTextMetrics(LPTEXTMETRIC lptm) {
	HDC AHandle = Get_Handle();
  SelectObject(AHandle, FFont->Handle);

  return (::GetTextMetrics(AHandle, lptm) == TRUE);
}

//---------------------------------------------------------------------------

