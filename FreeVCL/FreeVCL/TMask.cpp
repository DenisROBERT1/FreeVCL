//-----------------------------------------------------------------------------
//! @file TMask.cpp
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
// TMask
//---------------------------------------------------------------------------

TMask::TMask(const AnsiString AMaskValue): TObject() {
	int i;
	AnsiString asListExt;
	
  asListExt = AMaskValue;
  for (i = 1; i <= (int) asListExt.length(); i++) {
    if (asListExt[i] == _T(';')) asListExt[i] = _T('\n');
  }

  slExtList = new TStringList();
  slExtList->SetText(asListExt);
}

//---------------------------------------------------------------------------
TMask::~TMask(void) {
  delete slExtList;
}

//---------------------------------------------------------------------------
bool FASTCALL TMask::Matches(const AnsiString FileName) {
  return Matches((LPCTSTR) FileName);
}

//---------------------------------------------------------------------------
bool FASTCALL TMask::Matches(const TCHAR* szFileName) {
  int i;


  for (i = 0; i < slExtList->Count; i++) {
    if (MatchExt(szFileName, slExtList->Strings[i])) return true;
  }

  return false;
}

//---------------------------------------------------------------------------
bool TMask::MatchExt(const TCHAR* szFileName, const TCHAR* szExt) {
  int i, j;
  int li, lj;
	int PosSep;
  TCHAR ci, cj;


  li = lstrlen(szFileName);
  lj = lstrlen(szExt);

	// Position du dernier séparateur (pour que "v4.0.30319" puisse matcher avec "*.*" ou "v4??.*" par exemple)
	PosSep = li;
	for (i = li - 1; i >= 0; i--) {
		if (szFileName[i] == _T('.')) {
			PosSep = i;
			break;
		}
	}

  i = 0;
  j = 0;
  do {
    if (i < li) ci = szFileName[i]; else ci = _T('\0');
    if (_T('A') <= ci && ci <= _T('Z')) ci += _T('a') - _T('A');  // Transformation en minuscules
    if (j < lj) cj = szExt[j]; else cj = _T('\0');
    if (_T('A') <= cj && cj <= _T('Z')) cj += _T('a') - _T('A');  // Transformation en minuscules
		if (cj == _T('*')) {
			if (i != PosSep && i != li) j--;
			else i--;
		}
		else if (ci != cj && cj != _T('?')) {
			if (ci == '\0' && j < lj && !lstrcmp(&szExt[j], _T(".*"))) return true;
			return false;
		}
    i++;
    j++;
  } while (ci || cj);

  return true;
}

//---------------------------------------------------------------------------
