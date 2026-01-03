//-----------------------------------------------------------------------------
//! @file TFontPdf.cpp
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
// Constructeur
//---------------------------------------------------------------------------

TFontPdf::TFontPdf(TColorPdf *CommonColorPdf) {
  int i;


  // Initialiations
  ColorPdf = CommonColorPdf;
  Clear();
  for (i = 0; i < 14; i++) FObjetPdfFont[i] = NULL;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TFontPdf::~TFontPdf(void) {
	for (int i = 0; i < 14; i++) {
		if (FObjetPdfFont[i] && FObjetPdfFont[i]->Owner == this) delete FObjetPdfFont[i];
	}
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TFontPdf::Get_Height(void) {
  return FHeight;
}

bool TFontPdf::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
    bChangeHeight = true;
    if (FCharacterExtra) bChangeCharacterExtra = true;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TFontPdf::Get_Width(void) {
  return FWidth;
}

bool TFontPdf::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
    bChangeWidth = true;
    if (FCharacterExtra) bChangeCharacterExtra = true;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CharacterExtra
//---------------------------------------------------------------------------

int TFontPdf::Get_CharacterExtra(void) {
  return FCharacterExtra;
}

bool TFontPdf::Set_CharacterExtra(int NewCharacterExtra) {
  if (FCharacterExtra != NewCharacterExtra) {
    FCharacterExtra = NewCharacterExtra;
    bChangeCharacterExtra = true;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TFontStyles TFontPdf::Get_Style(void) {
  return FStyle;
}

bool TFontPdf::Set_Style(TFontStyles NewStyle) {
  if (FStyle != NewStyle) {
    FStyle = NewStyle;
    bChangeStyle = true;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TFontPdf::Get_Name(void) {
  return FName;
}

bool TFontPdf::Set_Name(AnsiString NewName) {
  if (FName != NewName) {
    FName = NewName;
    bChangeName = true;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TFontPdf::Get_Color(void) {
  return FColor;
}

bool TFontPdf::Set_Color(TColor NewColor) {
  if (FColor != NewColor) {
    FColor = NewColor;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ObjetPdfFont
//---------------------------------------------------------------------------

TObjetPdfFont * TFontPdf::Get_ObjetPdfFont(int i) {
  return FObjetPdfFont[i];
}

//---------------------------------------------------------------------------
void TFontPdf::Clear(void) {
  FHeight = 10;
  FWidth = 0;
  FStyle.Clear();
  FColor = clBlack;
  CharacterExtra = 2;
  bChangeHeight = false;
  bChangeWidth = false;
  bChangeCharacterExtra = false;
  bChangeStyle = false;
  bChangeName = false;
}

//---------------------------------------------------------------------------
// Traduction de la police en instructions PDF
//---------------------------------------------------------------------------

CharString TFontPdf::GetStream(void) {
  CharString csStream, csNamePdf, csBuf;
  int NumFontStandard;
  int RealWidth;


  if (FName == _T("Courier")) {
    if (FStyle.Contains(fsBold) && FStyle.Contains(fsItalic)) {
      NumFontStandard = 3;
    }
    else if (FStyle.Contains(fsItalic)) {
      NumFontStandard = 4;
    }
    else if (FStyle.Contains(fsBold)) {
      NumFontStandard = 2;
    }
    else {
      NumFontStandard = 1;
    }
  }
  else if (FName == _T("Times New Roman")) {
    if (FStyle.Contains(fsBold) && FStyle.Contains(fsItalic)) {
      NumFontStandard = 11;
    }
    else if (FStyle.Contains(fsItalic)) {
      NumFontStandard = 12;
    }
    else if (FStyle.Contains(fsBold)) {
      NumFontStandard = 10;
    }
    else {
      NumFontStandard = 9;
    }
  }
  else if (FName == _T("Symbol")) {
    NumFontStandard = 13;
  }
  else if (FName == _T("Monotype Sorts")) {
    NumFontStandard = 14;
  }
  else {
    if (FStyle.Contains(fsBold) && FStyle.Contains(fsItalic)) {
      NumFontStandard = 7;
    }
    else if (FStyle.Contains(fsItalic)) {
      NumFontStandard = 8;
    }
    else if (FStyle.Contains(fsBold)) {
      NumFontStandard = 6;
    }
    else {
      NumFontStandard = 5;
    }
  }

  csNamePdf.sprintf("F%i", NumFontStandard);
  if (FObjetPdfFont[NumFontStandard - 1] == NULL) {
    FObjetPdfFont[NumFontStandard - 1] = new TObjetPdfFont(this);
    FObjetPdfFont[NumFontStandard - 1]->TypeStandard = NumFontStandard;
  }

  RealWidth = (FWidth != 0)? 100 * FWidth / FHeight: 80;
  csStream.sprintf("/%s %i Tf %i Tz\n", (LPCSTR) csNamePdf, FHeight, RealWidth);

  if (bChangeCharacterExtra) {
    csBuf.sprintf("%i Tc\n", FCharacterExtra);
    csStream += csBuf;
    bChangeCharacterExtra = false;
  }

  ColorPdf->ColorBack = FColor;
  csStream += ColorPdf->GetStream(true, false);

  return csStream;
}

//---------------------------------------------------------------------------

