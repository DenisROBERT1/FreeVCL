//-----------------------------------------------------------------------------
//! @file TColorPdf.cpp
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
//                            TColorPdf
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TColorPdf::TColorPdf(void) {
  // Initialiations
  Clear();
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TColorPdf::~TColorPdf(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ColorBack
//---------------------------------------------------------------------------

TColor TColorPdf::Get_ColorBack(void) {
  return FColorBack;
}

bool TColorPdf::Set_ColorBack(TColor NewColorBack) {
  if (FColorBack != NewColorBack) {
    FColorBack = NewColorBack;
    bChangeColorBack = true;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColorFore
//---------------------------------------------------------------------------

TColor TColorPdf::Get_ColorFore(void) {
  return FColorFore;
}

bool TColorPdf::Set_ColorFore(TColor NewColorFore) {
  if (FColorFore != NewColorFore) {
    FColorFore = NewColorFore;
    bChangeColorFore = true;
  }
  return true;
}

//---------------------------------------------------------------------------
void TColorPdf::Clear(void) {
  FColorBack = clWhite;
  FColorFore = clBlack;

  bChangeColorBack = false;
  bChangeColorFore = false;
}

//---------------------------------------------------------------------------
// Traduction des couleurs en instructions PDF
//---------------------------------------------------------------------------

CharString TColorPdf::GetStreamColor(TColor Color, const char * szCode) {
  CharString csStream;
  int R, G, B;
  char szRed[4];
  char szGreen[4];
  char szBlue[4];


  R = Color.RValue();
  G = Color.GValue();
  B = Color.BValue();
  wsprintfA(szRed, "%03i", (int) (R * 100 / 255));
  wsprintfA(szGreen, "%03i", (int) (G * 100 / 255));
  wsprintfA(szBlue, "%03i", (int) (B * 100 / 255));
  csStream.sprintf("%c.%c%c %c.%c%c %c.%c%c %s\n",
                szRed[0], szRed[1], szRed[2],
                szGreen[0], szGreen[1], szGreen[2],
                szBlue[0], szBlue[1], szBlue[2],
                szCode);

  return csStream;
}


CharString TColorPdf::GetStream(bool bColorBack, bool bColorFore) {
  CharString csStream;

  if (bColorBack && bChangeColorBack) {
    csStream += GetStreamColor(FColorBack, "rg");
    bChangeColorBack = false;
  }
  if (bColorFore && bChangeColorFore) {
    csStream += GetStreamColor(FColorFore, "RG");
    bChangeColorFore = false;
  }

  return csStream;
}
//---------------------------------------------------------------------------

