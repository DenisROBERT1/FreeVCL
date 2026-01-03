//-----------------------------------------------------------------------------
//! @file TCanvasPdf.cpp
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
#include <FreeVcl_XFiles.h>

//---------------------------------------------------------------------------
//
//                            TCanvasPdf
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TCanvasPdf::TCanvasPdf(void): TCustomCanvas() {

  WidthPage = 0;
  HeightPage = 0;

  WindowOrgX = 0;
  WindowOrgY = 0;
  WindowExtX = 1;
  WindowExtY = 1;
  ViewportOrgX = 0;
  ViewportOrgY = 0;
  ViewportExtX = 1;
  ViewportExtY = 1;

  ColorPdf = new TColorPdf();
  FPen = new TPenPdf(ColorPdf);
  FBrush = new TBrushPdf(ColorPdf);
  FFont = new TFontPdf(ColorPdf);
  FTextAlign = TA_LEFT | TA_TOP;
  Clear();
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TCanvasPdf::~TCanvasPdf(void) {
  delete FFont;
  delete FBrush;
  delete FPen;
  delete ColorPdf;
}


//---------------------------------------------------------------------------
//
// Paramètres graphiques
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Lecture de l'origine des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::GetWindowOrg(int *X, int *Y) {

  *X = WindowOrgX;
  *Y = WindowOrgY;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'origine des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::SetWindowOrg(int X, int Y) {

  WindowOrgX = X;
  WindowOrgY = Y;

  return true;
}

//---------------------------------------------------------------------------
// Lecture de l'origine des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::GetViewportOrg(int *X, int *Y) {

  *X = ViewportOrgX;
  *Y = ViewportOrgY;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'origine des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::SetViewportOrg(int X, int Y) {

  ViewportOrgX = X;
  ViewportOrgY = Y;

  return true;
}

//---------------------------------------------------------------------------
// Lecture de l'échelle des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::GetWindowExt(int *XExt, int *YExt) {

  *XExt = WindowExtX;
  *YExt = WindowExtY;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'échelle des coordonnées de la fenêtre
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::SetWindowExt(int XExt, int YExt) {

  if (XExt == 0 || YExt == 0) return false;

  WindowExtX = XExt;
  WindowExtY = YExt;

  return true;
}

//---------------------------------------------------------------------------
// Lecture de l'échelle des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::GetViewportExt(int *XExt, int *YExt) {

  *XExt = WindowExtX;
  *YExt = WindowExtY;

  return true;
}

//---------------------------------------------------------------------------
// Modification de l'échelle des coordonnées du viewport
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::SetViewportExt(int XExt, int YExt) {

  if (XExt == 0 || YExt == 0) return false;

  ViewportExtX = XExt;
  ViewportExtY = YExt;

  return true;
}


//---------------------------------------------------------------------------
// Conversion de coordonnées dispositif vers coordonnées logiques.
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::DPtoLP(int XD, int YD, int *XL, int *YL) {

  *XL = (XD - ViewportOrgX) * WindowExtX / ViewportExtX + WindowOrgX;
  *YL = (YD - ViewportOrgY) * WindowExtY / ViewportExtY + WindowOrgY;

  return true;
}


//---------------------------------------------------------------------------
// Conversion de coordonnées logiques vers coordonnées dispositif.
//---------------------------------------------------------------------------

bool FASTCALL TCanvasPdf::LPtoDP(int XL, int YL, int *XD, int *YD) {

  *XD = (XL - WindowOrgX) * ViewportExtX / WindowExtX + ViewportOrgX;
  *YD = (YL - WindowOrgY) * ViewportExtY / WindowExtY + ViewportOrgY;

  return true;
}


//---------------------------------------------------------------------------
//
// Primitives graphiques
//
//---------------------------------------------------------------------------

void FASTCALL TCanvasPdf::MoveTo(int X, int Y) {
  CharString csBuf;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  LPtoPdf(X, Y, &X, &Y);

  FStream += FPen->GetStream();
  csBuf.sprintf("%i %i m\n", X, Y);
  FStream += csBuf;
  bLine = true;
}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::LineTo(int X, int Y) {
  CharString csBuf;


  LPtoPdf(X, Y, &X, &Y);

  csBuf.sprintf("%i %i l\n", X, Y);
  FStream += csBuf;
  bLine = true;
}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::Polyline(TPoint *ptPolyline, int NbPts) {
  // $$$$$$$$
}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::Polygon(TPoint *ptPolygon, int NbPts) {
  // $$$$$$$$
}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::Rectangle(int X1, int Y1, int X2, int Y2) {
  CharString csBuf;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  LPtoPdf(X1, Y1, &X1, &Y1);
  LPtoPdf(X2, Y2, &X2, &Y2);
  ReOrdonne(&X1, &X2);
  ReOrdonne(&Y1, &Y2);

  FStream += FPen->GetStream();
  csBuf.sprintf("%i %i %i %i re\n", X1, Y1, X2 - X1, Y2 - Y1);
  FStream += csBuf;

  if (FBrush->Style != bsClear) {
    csBuf.sprintf("B\n");
    FStream += csBuf;
  }
  else bLine = true;

}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::FillRect(int X1, int Y1, int X2, int Y2) {
  CharString csBuf;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  LPtoPdf(X1, Y1, &X1, &Y1);
  LPtoPdf(X2, Y2, &X2, &Y2);
  ReOrdonne(&X1, &X2);
  ReOrdonne(&Y1, &Y2);

  FStream += FPen->GetStream();
  FStream += FBrush->GetStream();
  csBuf.sprintf("%i %i %i %i re f\nB\n", X1, Y1, X2 - X1, Y2 - Y1);
  FStream += csBuf;
}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::RoundRect(int X1, int Y1, int X2, int Y2, int X3, int Y3) {
  CharString csBuf;
  int TX, TY;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  LPtoPdf(X1, Y1, &X1, &Y1);
  LPtoPdf(X2, Y2, &X2, &Y2);
  ReOrdonne(&X1, &X2);
  ReOrdonne(&Y1, &Y2);
  if (X3 > X2 - X1) X3 = X2 - X1;
  if (Y3 > Y2 - Y1) Y3 = Y2 - Y1;

  // Equation des courbes de bézier:
  // R(t) = (1 – t)^3 P0 + 3t(1 – t)^2 P1 + 3t^2(1 – t)P2 + t^3 P3
  // Si P0 = (0, 1), P3 = (1, 0), t = 0.5:
  // R(t) = (0.5)^3 P0 + 3(0.5)^3 P1 + 3(0.5)^3P2 + (0.5)^3 P3
  // = (0.5)^3 (P0 + 3P1 + 3P2 + P3)
  // = (0.5)^3 (0 + 3X1 + 3 + 1)  en X
  // = (0.5)^3 (3X1 + 4) = SQR(2) / 2
  // X1 = (SQR(2) / ( 2 (0.5)^3) - 4) / 3
  //  = 0,5522847498307933984022516322796

  TX = (int) (0.4477152501692066 * X3 + .5);
  TY = (int) (0.4477152501692066 * Y3 + .5);

  FStream += FPen->GetStream();
  FStream += FBrush->GetStream();
  csBuf.sprintf("%i %i m\n", X1, Y1 + Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X1, Y1 + TY, X1 + TX, Y1, X1 + X3, Y1);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X2 - X3, Y1);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X2 - TX, Y1, X2, Y1 + TY, X2, Y1 + Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X2, Y2 - Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X2, Y2 - TY, X2 - TX, Y2, X2 - X3, Y2);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X1 + X3, Y2);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X1 + TX, Y2, X1, Y2 - TY, X1, Y2 - Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X1, Y1 + Y3);
  FStream += csBuf;

  if (FBrush->Style != bsClear) {
    csBuf.sprintf("B\n");
    FStream += csBuf;
  }
  else bLine = true;

}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::Ellipse(int X1, int Y1, int X2, int Y2) {
  CharString csBuf;
  int X3, Y3, TX, TY;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  LPtoPdf(X1, Y1, &X1, &Y1);
  LPtoPdf(X2, Y2, &X2, &Y2);
  ReOrdonne(&X1, &X2);
  ReOrdonne(&Y1, &Y2);

  X3 = (X2 - X1) / 2;
  Y3 = (Y2 - Y1) / 2;
  TX = (int) (0.4477152501692066 * X3 + .5);
  TY = (int) (0.4477152501692066 * Y3 + .5);

  FStream += FPen->GetStream();
  FStream += FBrush->GetStream();
  csBuf.sprintf("%i %i m\n", X1, Y1 + Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X1, Y1 + TY, X1 + TX, Y1, X1 + X3, Y1);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X2 - X3, Y1);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X2 - TX, Y1, X2, Y1 + TY, X2, Y1 + Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X2, Y2 - Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X2, Y2 - TY, X2 - TX, Y2, X2 - X3, Y2);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X1 + X3, Y2);
  FStream += csBuf;
  csBuf.sprintf("%i %i %i %i %i %i c\n", X1 + TX, Y2, X1, Y2 - TY, X1, Y2 - Y3);
  FStream += csBuf;
  csBuf.sprintf("%i %i l\n", X1, Y1 + Y3);
  FStream += csBuf;

  if (FBrush->Style != bsClear) {
    csBuf.sprintf("B\n");
    FStream += csBuf;
  }
  else bLine = true;

}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::TextOut(int X, int Y, const AnsiString asText) {
  CharString csBuf;
  CharString csText;        // Texte sans anti-slashs ni parenthèses
  int i;
  char c;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  LPtoPdf(X, Y, &X, &Y);

  switch (FTextAlign & (TA_LEFT | TA_CENTER | TA_RIGHT)) {
  case TA_CENTER:
    X -= (FFont->Width * asText.Length() + FFont->CharacterExtra * (asText.Length() - 1)) / 2;
    break;
  case TA_RIGHT:
    X -= FFont->Width * asText.Length() + FFont->CharacterExtra * (asText.Length() - 1);
    break;
  }

  switch (FTextAlign & (TA_TOP | TA_BASELINE | TA_BOTTOM)) {
  case TA_TOP:
    Y -= FFont->Height;
    break;
  case TA_BOTTOM:
    Y += FFont->Height / 2;  // $$$ Comment calculer ça exactement ?
    break;
  }

  // Suppression des anti-slashs et des parenthèses
  for (i = 1; i <= asText.Length(); i++) {
    c = (char) asText[i];
    if (c == '\\' || c == '(' || c == ')') {
      csText += '\\';
    }
    csText += c;
  }

  FStream += "BT\n";
  FStream += FFont->GetStream();
  csBuf.sprintf("%i %i TD [(%s)] TJ\n", X, Y, csText.c_str());
  FStream += csBuf;
  FStream += "ET\n";
}

//---------------------------------------------------------------------------
bool FASTCALL TCanvasPdf::GetTextMetrics(LPTEXTMETRIC lptm) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::Draw(int X, int Y, TBitmap *Bitmap) {
  int x, y;
  TColor Color;
  int WidthBitmap, HeightBitmap;

  CharString csBuf;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  WidthBitmap = Bitmap->Width;
  HeightBitmap = Bitmap->Height;
  csBuf.sprintf("q\n%i 0 0 %i %i %i cm\n", WidthBitmap, HeightBitmap, X, Y);
  FStream += csBuf;
  FStream += "BI\n";
  csBuf.sprintf("/W %i\n/H %i\n/CS /RGB\n/BPC 8\n/F /A85\n", // AHx = AsciiHexa  A85 = Ascii85
      WidthBitmap, HeightBitmap);
  FStream += csBuf;
  FStream += "ID\n";

  csBuf = "";
  for (y = 0; y < Bitmap->Height; y++) {
    for (x = 0; x < Bitmap->Width; x++) {
      Color = Bitmap->Canvas->Get_Pixels(x, y);
      csBuf += Color.RValue();
      csBuf += Color.GValue();
      csBuf += Color.BValue();
    }
  }
  FStream += Ascii85(csBuf);

  FStream += "EI\nQ\n";

}

//---------------------------------------------------------------------------
void FASTCALL TCanvasPdf::StretchDraw(int X1, int Y1, int X2, int Y2, TBitmap *Bitmap) {
  int x, y, h, l;
  TColor Color;
  int WidthDessin, HeightDessin;
  int WidthBitmap, HeightBitmap;
  bool bInvX, bInvY;

  CharString csBuf;


  if (bLine) {
    FStream += "S\n";
    bLine = false;
  }

  LPtoPdf(X1, Y1, &X1, &Y1);
  LPtoPdf(X2, Y2, &X2, &Y2);
  bInvX = (X1 > X2);
  bInvY = (Y1 < Y2);
  ReOrdonne(&X1, &X2);
  ReOrdonne(&Y1, &Y2);

  WidthBitmap = Bitmap->Width;
  HeightBitmap = Bitmap->Height;
  WidthDessin = X2 - X1;
  HeightDessin = Y2 - Y1;
  if (WidthDessin < 0) WidthDessin = -WidthDessin;
  if (HeightDessin < 0) HeightDessin = -HeightDessin;
  csBuf.sprintf("q\n%i 0 0 %i %i %i cm\n", WidthDessin, HeightDessin, X1, Y1);
  FStream += csBuf;
  FStream += "BI\n";
  csBuf.sprintf("/W %i\n/H %i\n/CS /RGB\n/BPC 8\n/F /A85\n", // AHx = AsciiHexa  A85 = Ascii85
      WidthBitmap, HeightBitmap);
  FStream += csBuf;
  FStream += "ID\n";

  csBuf = "";
  l = Bitmap->Width;
  h = Bitmap->Height;
  for (y = 0; y < h; y++) {
    for (x = 0; x < l; x++) {
      Color = Bitmap->Canvas->Get_Pixels(
          bInvX? l - x - 1: x,
          bInvY? h - y - 1: y);
      csBuf += Color.RValue();
      csBuf += Color.GValue();
      csBuf += Color.BValue();
    }
  }
  FStream += Ascii85(csBuf);

  FStream += "EI\nQ\n";

}

//---------------------------------------------------------------------------
void TCanvasPdf::Clear(void) {
  FStream = "";
  bLine = false;
  FPen->Clear();
  FBrush->Clear();
  FFont->Clear();
}

//---------------------------------------------------------------------------
void TCanvasPdf::EndDraw(void) {
  if (bLine) FStream += "S";
  bLine = false;
}

//---------------------------------------------------------------------------
CharString TCanvasPdf::AsciiHexa(CharString csIn) {
  CharString csBuf;
  CharString csOut;
  int i, n;
  BYTE b;


  n = 0;
  csBuf = "";
  for (i = 1; i <= csIn.Length(); i++) {
    b = (BYTE) csIn[i];
    csBuf.sprintf("%02X", b);
    csOut += csBuf;
    if (++n == 40) {
      csOut += "\n";
      n = 0;
    }
  }
  csOut += ">\n";

  return csOut;
}

//---------------------------------------------------------------------------
CharString TCanvasPdf::Ascii85(CharString csIn) {
  CharString csOut;
  int i, j, n;
  char c[5];
  int cbc;
  DWORD dwIn;
  int cbIn;
  int Len;


  n = 0;
  dwIn = 0;
  cbIn = 0;
  Len = 4 * ((csIn.Length() + 3) / 4);  // Premier multiple de 4 >= Length()
  for (i = 1; i <= Len; i++) {
    dwIn <<= 8;
    if (i <= csIn.Length()) dwIn += (BYTE) csIn[i];
    if (++cbIn == 4) {
      if (dwIn == 0) {
        c[0] = 'z';
        cbc = 1;
      }
      else {
        c[4] = (char) ((dwIn % 85) + 33); dwIn /= 85;
        c[3] = (char) ((dwIn % 85) + 33); dwIn /= 85;
        c[2] = (char) ((dwIn % 85) + 33); dwIn /= 85;
        c[1] = (char) ((dwIn % 85) + 33); dwIn /= 85;
        c[0] = (char) ((dwIn % 85) + 33);
        cbc = 5;
      }
      for (j = 0; j < cbc; j++) {
        csOut += c[j];
        if (++n == 80) {
          csOut += '\n';
          n = 0;
        }
      }
      dwIn = 0;
      cbIn = 0;
    }
  }
  csOut += "~>\n";

  return csOut;
}

//---------------------------------------------------------------------------
void TCanvasPdf::ReOrdonne(int *X1, int *X2) {
  if (*X1 > *X2) {
    int X3;
    X3 = *X1;
    *X1 = *X2;
    *X2 = X3;
  }
}

//---------------------------------------------------------------------------
bool TCanvasPdf::LPtoPdf(int XL, int YL, int *XPdf, int *YPdf) {
  int XD, YD;

  // Coordonnées logique => coordonnées "dispositif"
  LPtoDP(XL, YL, &XD, &YD);

  // Coordonnées "dispositif" => Coordonnées PDF
  *XPdf = XD;
  *YPdf = HeightPage - YD;

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Stream
//---------------------------------------------------------------------------

CharString TCanvasPdf::Get_Stream(void) {
  return FStream;
}

bool TCanvasPdf::Set_Stream(CharString NewStream) {
  FStream = NewStream;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Pen
//---------------------------------------------------------------------------

TCustomPen *TCanvasPdf::Get_Pen(void) {
  return FPen;
}

bool TCanvasPdf::Set_Pen(TCustomPen *NewPen) {
  FPen->Assign(NewPen);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Brush
//---------------------------------------------------------------------------

TCustomBrush *TCanvasPdf::Get_Brush(void) {
  return FBrush;
}

bool TCanvasPdf::Set_Brush(TCustomBrush *NewBrush) {
  FBrush->Assign(NewBrush);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

TCustomFont *TCanvasPdf::Get_Font(void) {
  return FFont;
}

bool TCanvasPdf::Set_Font(TCustomFont *NewFont) {
  FFont->Assign(NewFont);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TextAlignH
//---------------------------------------------------------------------------

TAlignment TCanvasPdf::Get_TextAlignH(void) {
  switch (FTextAlign & (TA_LEFT | TA_CENTER | TA_RIGHT)) {
    case TA_LEFT: return taLeftJustify;
    case TA_CENTER: return taCenter;
    case TA_RIGHT: return taRightJustify;
  }

  return taLeftJustify;
}

bool TCanvasPdf::Set_TextAlignH(TAlignment NewTextAlignH) {

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

TAlignmentV TCanvasPdf::Get_TextAlignV(void) {
  switch (FTextAlign & (TA_TOP | TA_BASELINE | TA_BOTTOM)) {
    case TA_TOP: return taTop;
    case TA_BASELINE: return taBaseLine;
    case TA_BOTTOM: return taBottom;
  }

  return taTop;
}

bool TCanvasPdf::Set_TextAlignV(TAlignmentV NewTextAlignV) {

  FTextAlign &= ~(TA_TOP | TA_BASELINE | TA_BOTTOM);

  switch (NewTextAlignV) {
    case taTop: FTextAlign |= TA_TOP; break;
    case taBaseLine: FTextAlign |= TA_BASELINE; break;
    case taBottom: FTextAlign |= TA_BOTTOM; break;
  }

  return true;
}

//---------------------------------------------------------------------------
void TCanvasPdf::SetPageSize(int Width, int Height) {
  WidthPage = Width;
  HeightPage = Height;
}

//---------------------------------------------------------------------------

