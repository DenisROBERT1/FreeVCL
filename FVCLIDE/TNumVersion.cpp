//-----------------------------------------------------------------------------
//! @file TNumVersion.cpp
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

#include "TNumVersion.h"


//---------------------------------------------------------------------------
// Constructeurs
//---------------------------------------------------------------------------

TNumVersion::TNumVersion(void) {
	Num[0] = 0;
	Num[1] = 0;
	Num[2] = 0;
	Num[3] = 0;
}


TNumVersion::TNumVersion(const TCHAR *szNumVersion) {
	operator = (szNumVersion);
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TNumVersion::~TNumVersion(void) {

}

//---------------------------------------------------------------------------
TNumVersion & TNumVersion::operator = (const TCHAR *szNumVersion) {
	int i, j;
	TCHAR c;
	AnsiString asNum;


	for (i = 0, j = 0; j < 5; i++) {
		c = szNumVersion[i];
		if (c == _T('.') || c == _T('\0')) {
			Num[j++] = asNum.ToIntDef(0);
			if (j == 5 || c == _T('\0')) break;
			asNum = _T("");
		}
		else {
			asNum += c;
		}
	}

	return *this;
}

//---------------------------------------------------------------------------
bool TNumVersion::operator == (TNumVersion &src) const {
	return !operator != (src);
}

//---------------------------------------------------------------------------
bool TNumVersion::operator != (TNumVersion &src) const {
	if (Num[0] != src.Num[0]) return true;
	if (Num[1] != src.Num[1]) return true;
	if (Num[2] != src.Num[2]) return true;
	if (Num[3] != src.Num[3]) return true;

	return false;
}

//---------------------------------------------------------------------------
bool TNumVersion::operator < (TNumVersion &src) const {
	if (Num[0] < src.Num[0]) return true;
	if (Num[1] < src.Num[1]) return true;
	if (Num[2] < src.Num[2]) return true;
	if (Num[3] < src.Num[3]) return true;

	return false;
}

//---------------------------------------------------------------------------
bool TNumVersion::operator > (TNumVersion &src) const {
	if (Num[0] > src.Num[0]) return true;
	if (Num[1] > src.Num[1]) return true;
	if (Num[2] > src.Num[2]) return true;
	if (Num[3] > src.Num[3]) return true;

	return false;
}

//---------------------------------------------------------------------------
bool TNumVersion::operator <= (TNumVersion &src) const {
	return !operator > (src);
}

//---------------------------------------------------------------------------
bool TNumVersion::operator >= (TNumVersion &src) const {
	return !operator < (src);
}

//---------------------------------------------------------------------------
