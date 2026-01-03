//-----------------------------------------------------------------------------
//! @file TObjetPdfFont.cpp
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
//                            TObjetPdfFont
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TObjetPdfFont::TObjetPdfFont(void *Owner): TObjetPdf(Owner) {
  FTypeStandard = 1;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TObjetPdfFont::~TObjetPdfFont(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TypeStandard
//---------------------------------------------------------------------------

int TObjetPdfFont::Get_TypeStandard(void) {
  return FTypeStandard;
}

bool TObjetPdfFont::Set_TypeStandard(int NewTypeStandard) {
  if (FTypeStandard != NewTypeStandard) {
    FTypeStandard = NewTypeStandard;
  }

  return true;
}


//---------------------------------------------------------------------------
// Chaîne de description
//---------------------------------------------------------------------------

CharString TObjetPdfFont::GetDescription(void) {
  CharString csDescription;


  static const char * szTypesStandard[] = {
    "Courier",
    "Courier-Bold",
    "Courier-BoldOblique",
    "Courier-Oblique",
    "Helvetica",
    "Helvetica-Bold",
    "Helvetica-BoldOblique",
    "Helvetica-Oblique",
    "Times-Roman",
    "Times-Bold",
    "Times-BoldItalic",
    "Times-Italic",
    "Symbol",
    "ZapfDingbats"
  };

  csDescription.sprintf("<<\n/Type /Font\n/Name /F%i\n/Subtype /Type1\n/Encoding /WinAnsiEncoding\n/BaseFont /%s\n>>\n",
                        FTypeStandard,
                        szTypesStandard[FTypeStandard - 1]);

  return csDescription;
}

//---------------------------------------------------------------------------
bool TObjetPdfFont::SetDescription(CharString csDescription) {
	CharString csTypeStandard;


	csTypeStandard = ExtractValue(csDescription, "/Name");
	if (csTypeStandard.SubString(1, 2) == "/F") {
		csTypeStandard = csTypeStandard.SubString(3, 2);
		FTypeStandard = csTypeStandard.ToIntDef(0);
		if (FTypeStandard < 1 || 14 < FTypeStandard) FTypeStandard = 1;
	}

	return true;
}

//---------------------------------------------------------------------------

