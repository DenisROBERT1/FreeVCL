//-----------------------------------------------------------------------------
//! @file TBrushPdf.cpp
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
//                            TBrushPdf
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TBrushPdf::TBrushPdf(TColorPdf *CommonColorPdf) {
  // Initialiations
  ColorPdf = CommonColorPdf;
  Clear();
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TBrushPdf::~TBrushPdf(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TBrushStyle TBrushPdf::Get_Style(void) {
  return FStyle;
}

bool TBrushPdf::Set_Style(TBrushStyle NewStyle) {
  if (FStyle != NewStyle) {
    FStyle = NewStyle;
    bChangeStyle = true;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TBrushPdf::Get_Color(void) {
  return FColor;
}

bool TBrushPdf::Set_Color(TColor NewColor) {
  if (FColor != NewColor) {
    FColor = NewColor;
  }
  return true;
}

//---------------------------------------------------------------------------
void TBrushPdf::Clear(void) {
  FStyle = bsSolid;
  FColor = clBlack;
  ColorPdf->ColorFore = clBlack;
  bChangeStyle = false;
}

//---------------------------------------------------------------------------
// Traduction du pinceau en instructions PDF
//---------------------------------------------------------------------------

CharString TBrushPdf::GetStream(void) {
  CharString csStream, asBuf;


  if (FStyle != bsClear) {
    if (bChangeStyle) {
      bChangeStyle = false;
    }
    ColorPdf->ColorBack = FColor;
    csStream += ColorPdf->GetStream(true, false);
  }

  return csStream;
}

//---------------------------------------------------------------------------

