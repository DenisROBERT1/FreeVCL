//-----------------------------------------------------------------------------
//! @file TObjetPdf.cpp
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
//                            TObjetPdf
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TObjetPdf::TObjetPdf(void *Owner) {
	FOwner = Owner;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TObjetPdf::~TObjetPdf(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NumObj
//---------------------------------------------------------------------------

int TObjetPdf::Get_NumObj(void) {
  return FNumObj;
}

bool TObjetPdf::Set_NumObj(int NewNumObj) {
  if (FNumObj != NewNumObj) {
    FNumObj = NewNumObj;
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Owner
//---------------------------------------------------------------------------

void * TObjetPdf::Get_Owner(void) {
  return FOwner;
}

//---------------------------------------------------------------------------
bool TObjetPdf::IsDelimiter(const char c, const char *szDelimiters) const {
	int i;

	for (i = 0; i < (int) strlen(szDelimiters); i++) {
		if (szDelimiters[i] == c) return true;
	}

	return false;
}

//---------------------------------------------------------------------------
CharString TObjetPdf::ExtractFirstWord(const char *szString, const char *szDelimiters, int *Pos) const {
	char c;
	CharString csWord;

	while ((c = szString[*Pos]) != '\0') {
		if (!IsDelimiter(c, szDelimiters)) break;
		(*Pos)++;
	}

	while ((c = szString[*Pos]) != '\0') {
		if (IsDelimiter(c, szDelimiters)) break;
		csWord += c;
		(*Pos)++;
	}

	while ((c = szString[*Pos]) != '\0') {
		if (!IsDelimiter(c, szDelimiters)) break;
		(*Pos)++;
	}

	return csWord;
}

//---------------------------------------------------------------------------
CharString TObjetPdf::ExtractLastWord(const char *szString, const char *szDelimiters, int *Pos) const {
	char c;
	CharString csWord;

	while ((*Pos) > 0) {
		c = szString[*Pos];
		if (!IsDelimiter(c, szDelimiters)) break;
		(*Pos)--;
	}

	while ((*Pos) > 0) {
		c = szString[*Pos];
		if (IsDelimiter(c, szDelimiters)) break;
		csWord = CharString(c) + csWord;
		(*Pos)--;
	}

	(*Pos)++;

	return csWord;
}

//---------------------------------------------------------------------------
CharString TObjetPdf::ExtractValue(const char *szDescription, const CharString csName) const {
	int Pos = 0;
	bool bKeyword = false;
	CharString csWord;

	csWord = ExtractFirstWord(szDescription, " \t\n", &Pos);
	while (!csWord.IsEmpty()) {
		if (bKeyword) return csWord;
		if (csWord == csName) bKeyword = true;
		csWord = ExtractFirstWord(szDescription, " \t\n", &Pos);
	}

	return "";
}

//---------------------------------------------------------------------------

