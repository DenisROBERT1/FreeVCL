//-----------------------------------------------------------------------------
//! @file TObjetPdfContent.cpp
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
//                            TObjetPdfContent
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TObjetPdfContent::TObjetPdfContent(void *Owner): TObjetPdf(Owner) {
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TObjetPdfContent::~TObjetPdfContent(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Stream
//---------------------------------------------------------------------------

CharString TObjetPdfContent::Get_Stream(void) {
  return FStream;
}

bool TObjetPdfContent::Set_Stream(CharString NewStream) {
  if (FStream != NewStream) {
    FStream = NewStream;
  }

  return true;
}


//---------------------------------------------------------------------------
// Chaîne de description
//---------------------------------------------------------------------------

CharString TObjetPdfContent::GetDescription(void) {
  CharString csDescription;


  csDescription.sprintf("<< /Length %i >>\nstream\n", FStream.Length());
  csDescription += FStream;
  csDescription += "\nendstream\n";

  return csDescription;
}

//---------------------------------------------------------------------------
bool TObjetPdfContent::SetDescription(CharString csDescription) {
	int Begin, End;
	CharString csWord1, csWord2, csWord3, csWord4, csWord5, csWord6;

	if (csDescription == "") return false;

	Begin = 0;
	csWord1 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	csWord2 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	csWord3 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	csWord4 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	csWord5 = ExtractFirstWord(csDescription, " \t\n", &Begin);
	Begin++;  // Parce que ExtractFirstWord renvoie l'indice par rapport à zéro

	End = csDescription.Length() - 1;
	csWord6 = ExtractLastWord(csDescription, " \t\n", &End);
	End++;  // Idem

	if (csWord1 == "<<" && csWord2 == "/Length" && csWord4 == ">>" &&
		  csWord5 == "stream" && csWord6 == "endstream") {
		FStream = csDescription.SubString(Begin, End - Begin);
	}
	
	return true;
}

//---------------------------------------------------------------------------

