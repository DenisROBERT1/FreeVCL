//-----------------------------------------------------------------------------
//! @file TClipboard.cpp
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
// TClipboard
//---------------------------------------------------------------------------

TClipboard::TClipboard(void): TPersistent() {
  // Initialisations
  NbOpen = 0;
}

TClipboard::~TClipboard(void) {
  if (NbOpen) CloseClipboard();
  // Initialisations
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AsText
//---------------------------------------------------------------------------

AnsiString TClipboard::Get_AsText(void) {
  GLOBALHANDLE hValue;
  AnsiString asAsText;


  // Les formats CF_UNICODETEXT et CF_OEMTEXT sont automatiquement convertis par le système

#ifdef UNICODE
	hValue = GetClipboardData(CF_UNICODETEXT);
#else
	hValue = GetClipboardData(CF_TEXT);
#endif
  if (hValue) {
    asAsText = (TCHAR *) GlobalLock(hValue);
    GlobalUnlock(hValue);
  }

  return asAsText;
}


//---------------------------------------------------------------------------
bool FASTCALL TClipboard::Open(void) {
  if (NbOpen++ == 0) OpenClipboard(NULL);

  return true;
}
//---------------------------------------------------------------------------
bool FASTCALL TClipboard::Close(void) {
  if (--NbOpen == 0) CloseClipboard();

  return true;
}
//---------------------------------------------------------------------------
void FASTCALL TClipboard::Clear(void) {
  EmptyClipboard();
}
//---------------------------------------------------------------------------
HANDLE FASTCALL TClipboard::GetAsHandle(UINT Format) {
  return GetClipboardData(Format);
}
//---------------------------------------------------------------------------
void FASTCALL TClipboard::SetAsHandle(UINT Format, HANDLE hValue) {
  SetClipboardData(Format, hValue);
}

//---------------------------------------------------------------------------
void FASTCALL TClipboard::SetTextBuf(const TCHAR * Buffer) {
  GLOBALHANDLE hValue;
  TCHAR *lp;
  int Len;


  Len = lstrlen(Buffer) + 1;
  hValue = GlobalAlloc(GHND, Len * sizeof(TCHAR));
  if (hValue) {
    lp = (TCHAR*) GlobalLock(hValue);
    lstrcpyn(lp, Buffer, Len);
#ifdef UNICODE
    SetClipboardData(CF_UNICODETEXT, hValue);
#else
    SetClipboardData(CF_TEXT, hValue);
#endif
    GlobalUnlock(hValue);
  }
}

//---------------------------------------------------------------------------

