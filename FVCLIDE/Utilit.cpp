//-----------------------------------------------------------------------------
//! @file Utilit.cpp
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


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <FreeVcl.h>

#include "Utilit.h"


//---------------------------------------------------------------------------
bool FASTCALL IsAsciiFile(AnsiString asFileName) {
	HANDLE hFile;
	char sBuf[1024];
	wchar_t *swBuf;
	DWORD dw = 0;
	int i;


  hFile = CreateFile(asFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    ReadFile(hFile, sBuf, 1024, &dw, NULL);
	  CloseHandle(hFile);
		bool bIsAscii;

		// Test fichier ASCII de char
		bIsAscii = true;
		for (i = 0; i < (int) dw; i++) {
			if ((BYTE) sBuf[i] < ' ' && sBuf[i] != '\r' && sBuf[i] != '\n' && sBuf[i] != '\t') {
				bIsAscii = false;
				break;
			}
		}

		// Test fichier ASCII unicode
		if (!bIsAscii) {
			bIsAscii = true;
			swBuf = (wchar_t *) sBuf; 
			for (i = 0; i < (int) (dw / sizeof(wchar_t)); i++) {
				if (swBuf[i] < ' ' && swBuf[i] != '\r' && swBuf[i] != '\n' && swBuf[i] != '\t') {
					bIsAscii = false;
					break;
				}
			}
		}

		return bIsAscii;

	}

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL IsCorrectFileName(AnsiString asFileName) {
	int i;
	TCHAR c;


	i = 1;

	// Nom de disque :
	if (asFileName.Length() >= 2 && asFileName[2] == _T(':')) {
		c = asFileName[1];
		if (_T('A') <= c && c <= _T('Z') ||
				_T('a') <= c && c <= _T('z')) {
			i += 2;
		}
		else {
			return false;
		}
	}
	if (i > asFileName.Length()) return false;

	// Chemin, nom de fichier et extension:
	while (i <= asFileName.Length()) {
		c = asFileName[i++];
		if (c == _T(':') ||	c == _T('*') || c == _T('?') ||
				c == _T('"') || c == _T('<') || c == _T('>') ||
				c == _T('|')) return false;
	}

	return true;
}

bool IsDecimalOrHexa(AnsiString asText, unsigned long *Number, bool *bIsDec, bool *bIsHexa) {
	TCHAR c;
	int i;

	*bIsHexa = false;
	*bIsDec = true;

	*Number = 0;
	for (i = 1; i <= asText.Length(); i++) {
		c = asText[i];
		*Number *= 10;
		if (_T('0') <= c && c <= _T('9')) *Number += (BYTE) (c - _T('0'));
		else {
			*bIsDec = false;
			break;
		}
	}

	if (asText.Length() >= 3) {
		if (asText[1] == _T('0') && asText[2] == _T('x')) {
			*bIsHexa = true;
			*Number = 0;
			for (i = 3; i <= asText.Length(); i++) {
				c = asText[i];
				*Number <<= 4;
				if (_T('0') <= c && c <= _T('9')) *Number += (BYTE) (c - _T('0'));
				else if (_T('a') <= c && c <= _T('f')) *Number += (BYTE) (c - _T('a') + 10);
				else if (_T('A') <= c && c <= _T('F')) *Number += (BYTE) (c - _T('A') + 10);
				else {
					*bIsHexa = false;
					break;
				}
			}
		}
	}

	return (*bIsDec || *bIsHexa);
}

//---------------------------------------------------------------------------
