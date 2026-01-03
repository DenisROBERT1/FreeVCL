//-----------------------------------------------------------------------------
//! @file TObjetPdfUnknown.cpp
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
//                            TObjetPdfUnknown
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TObjetPdfUnknown::TObjetPdfUnknown(void *Owner): TObjetPdf(Owner) {
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TObjetPdfUnknown::~TObjetPdfUnknown(void) {
}


//---------------------------------------------------------------------------
// Chaîne de description
//---------------------------------------------------------------------------

CharString TObjetPdfUnknown::GetDescription(void) {
  return csObjDescription;
}

//---------------------------------------------------------------------------
bool TObjetPdfUnknown::SetDescription(CharString csDescription) {
	int Begin, End;
	CharString csWord1, csWord2, csWord3, csWord4;

	if (csDescription == "") {
		csObjDescription = "";
		return false;
	}

	Begin = 0;
	csWord1 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	csWord2 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	csWord3 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	Begin++;  // Parce que ExtractFirstWord renvoie l'indice par rapport à zéro

	End = csDescription.Length() - 1;
	csWord4 = ExtractLastWord(csDescription, " \t\n", &End);
	End++;  // Idem

	if (csWord3 == "obj" && csWord4 == "endobj") {
		FNumObj = csWord1.ToIntDef(0);
		csObjDescription = csDescription.SubString(Begin, End - Begin);
	}
	
	return true;
}

//---------------------------------------------------------------------------
