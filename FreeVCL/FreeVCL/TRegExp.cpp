//-----------------------------------------------------------------------------
//! @file TRegExp.cpp
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
// Constructeur
//---------------------------------------------------------------------------

TRegExp::TRegExp(AnsiString asRegExp): TObject() {
	FRegExp = asRegExp;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TRegExp::~TRegExp(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété RegExp
//---------------------------------------------------------------------------

AnsiString TRegExp::Get_RegExp() {
  return FRegExp;
}

bool TRegExp::Set_RegExp(AnsiString NewRegExp) {
  if (FRegExp != NewRegExp) {
    FRegExp = NewRegExp;
  }
  return true;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

bool TRegExp::Match(int Beg, int End, const TCHAR *szString, int IndexDeb, bool bCaseSensitive, int *IndexEnd) {
	int Beg2;


	if (End < Beg) {
		// RegExp vide => ça matche toujours
		if (IndexEnd) *IndexEnd = IndexDeb;
		return true;
	}

	Beg2 = Beg;
	ParseOneExp(&Beg2, End);

	if (Beg2 <= End && FRegExp[Beg2] == _T('*')) {
		// 0-n fois
		while (MatchOneChar(Beg, Beg2, szString, IndexDeb, bCaseSensitive)) {
			IndexDeb++;
		}
		Beg2++;
		return Match(Beg2, End, szString, IndexDeb, bCaseSensitive, IndexEnd);
	}
	else if (Beg2 <= End && FRegExp[Beg2] == _T('+')) {
		// 1-n fois

		if (!MatchOneChar(Beg, Beg2, szString, IndexDeb, bCaseSensitive)) return false;
		IndexDeb++;

		while (MatchOneChar(Beg, Beg2, szString, IndexDeb, bCaseSensitive)) {
			IndexDeb++;
		}

		Beg2++;
		return Match(Beg2, End, szString, IndexDeb, bCaseSensitive, IndexEnd);
	}
	else {
		// 1 fois
		if (!MatchOneChar(Beg, Beg2, szString, IndexDeb, bCaseSensitive)) return false;
		IndexDeb++;

		return Match(Beg2, End, szString, IndexDeb, bCaseSensitive, IndexEnd);
	}

}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

bool TRegExp::MatchOneChar(int Beg, int End, const TCHAR *szString, int IndexDeb, bool bCaseSensitive) {

	if (FRegExp[Beg] == _T('.')) {
		// N'importe quel caractère
		return true;
	}
	if (FRegExp[Beg] == _T('\\')) {
		// Caractère spécial
		return true;
	}
	if (FRegExp[Beg] == szString[IndexDeb]) {
		// Caractère autre
		return true;
	}

  return false;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

bool TRegExp::ParseOneExp(int *Beg, int End) {
	if (End < *Beg) return false;

	if (FRegExp[*Beg] == _T('\\')) {
		if (End < *Beg) return false;
		(*Beg)++;
	}
	else if (FRegExp[*Beg] == _T('[')) {
		do {
			(*Beg)++;
			if (End < *Beg) return false;
		} while (FRegExp[*Beg] != _T(']'));
	}

	(*Beg)++;

	return true;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

bool TRegExp::Match(AnsiString asString, bool bCaseSensitive) {

	return Match(1, FRegExp.Length(), asString, 0, bCaseSensitive);
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

bool TRegExp::Search(AnsiString asString, bool bCaseSensitive, int *IndexBeg, int *IndexEnd) {
	int i;
	int Len, LenRegExp;


	Len = asString.Length();
	LenRegExp = FRegExp.Length();
	for (i = *IndexBeg; i < asString.Length(); i++) {
		if (Match(1, LenRegExp, asString, i, bCaseSensitive, IndexEnd)) {
			if (IndexBeg) *IndexBeg = i;
			return true;
		}
	}

  return false;
}
