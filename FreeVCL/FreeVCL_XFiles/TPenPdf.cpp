//-----------------------------------------------------------------------------
//! @file TPenPdf.cpp
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
//                            TPenPdf
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TPenPdf::TPenPdf(TColorPdf *CommonColorPdf) {
  // Initialiations
  ColorPdf = CommonColorPdf;
  Clear();
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TPenPdf::~TPenPdf(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TPenStyle TPenPdf::Get_Style(void) {
  return FStyle;
}

bool TPenPdf::Set_Style(TPenStyle NewStyle) {
  if (FStyle != NewStyle) {
    FStyle = NewStyle;
    bChangeStyle = true;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TPenPdf::Get_Width(void) {
  return FWidth;
}

bool TPenPdf::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
    bChangeWidth = true;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TPenPdf::Get_Color(void) {
  return FColor;
}

bool TPenPdf::Set_Color(TColor NewColor) {
  if (FColor != NewColor) {
    FColor = NewColor;
  }
  return true;
}

//---------------------------------------------------------------------------
void TPenPdf::Clear(void) {
  FStyle = psSolid;
  FColor = clBlack;
  ColorPdf->ColorFore = clBlack;
  FWidth = 0;
  bChangeStyle = false;
  bChangeWidth = false;
}

//---------------------------------------------------------------------------
// Traduction du stylo en instructions PDF
//---------------------------------------------------------------------------

CharString TPenPdf::GetStream(void) {
  CharString csStream, csBuf;


  if (bChangeStyle) {
    switch (FStyle) {
    case psSolid:
      csStream += "[] 0 d\n";
      break;
    case psDash:
      csStream += "[3 3] 0 d\n";
      break;
    case psDot:
      csStream += "[1 5] 0 d\n";
      break;
    case psDashDot:
      csStream += "[3 3 1 3] 0 d\n";
      break;
    case psDashDotDot:
      csStream += "[3 3 1 3 1 3] 0 d\n";
      break;
    case psClear:
      csStream += "[0 1000] 0 d\n";
      break;
    }
    bChangeStyle = false;
  }
  if (bChangeWidth) {
    csBuf.sprintf("%i w\n", FWidth);
    csStream += csBuf;
    bChangeWidth = false;
  }
  ColorPdf->ColorFore = FColor;
  csStream += ColorPdf->GetStream(false, true);

  return csStream;
}

//---------------------------------------------------------------------------

