//-----------------------------------------------------------------------------
//! @file TMemoLines.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TMemoLines
//---------------------------------------------------------------------------

TMemoLines::TMemoLines(void): TStrings() {
  FHandle = NULL;
}

TMemoLines::~TMemoLines(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TMemoLines::Get_Handle(void) {
  return FHandle;
}

bool TMemoLines::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseur de la propriété Count
//---------------------------------------------------------------------------

int FASTCALL TMemoLines::GetCount(void) const {
  if (!FHandle) return 0;

  return (int) SendMessage(FHandle, EM_GETLINECOUNT, 0, 0);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TMemoLines::Get_Strings(int Index) {
  int ich, Length;
  TCHAR *szBuf;
  AnsiString asRetour;


  if (FHandle) {
    ich = (int) SendMessage(FHandle, EM_LINEINDEX, (WPARAM) Index, 0);
    Length = (int) SendMessage(FHandle, EM_LINELENGTH, (WPARAM) ich, 0) + 2;  // + 2 pour être sûr qu'il soit d'une longueur d'au moins 2
    szBuf = new TCHAR[Length];
    memcpy(szBuf, &Length, sizeof(WORD));
    Length = (int) SendMessage(FHandle, EM_GETLINE, (WPARAM) Index, (LPARAM) (LPCTSTR) szBuf);
    szBuf[Length] = _T('\0');
    asRetour = szBuf;
    delete[] szBuf;
  }

  return asRetour;
}

//---------------------------------------------------------------------------
bool TMemoLines::Set_Strings(int Index, AnsiString NewStrings) {
  TCHAR *szBuf;
  int LengthBuffer, Length, LengthOld, Dec;
  int ich, j;


  if (FHandle) {
		if (Index >= GetCount()) Add(NewStrings);
    ich = (int) SendMessage(FHandle, EM_LINEINDEX, (WPARAM) Index, 0);
    LengthOld = (int) SendMessage(FHandle, EM_LINELENGTH, (WPARAM) ich, 0);
    Length = NewStrings.Length();
    LengthBuffer = GetWindowTextLength(FHandle) + Length + 1;
    szBuf = new TCHAR[LengthBuffer];
    j = GetWindowText(FHandle, szBuf, LengthBuffer);
		if (Length < LengthOld) {
			Dec = LengthOld - Length;
    	for (j = ich + Length; j < LengthBuffer; j++) szBuf[j] = szBuf[j + Dec];
		}
		else if (Length > LengthOld) {
			Dec = Length - LengthOld;
    	for (j = LengthBuffer - 1; j >= ich + Length; j--) szBuf[j] = szBuf[j - Dec];
		}
    memcpy(&szBuf[ich], NewStrings, Length * sizeof(TCHAR));
    SetWindowText(FHandle, szBuf);
    delete[] szBuf;
  }

  return true;
}

//---------------------------------------------------------------------------
void TMemoLines::Clear(void) {
  if (FHandle) SetWindowText(FHandle, _T(""));
}

//---------------------------------------------------------------------------
int TMemoLines::Add(const AnsiString S) {
  TCHAR *szBuf;
  int LengthBuffer;


  if (FHandle) {
    LengthBuffer = GetWindowTextLength(FHandle) + S.Length() + 3;
    szBuf = new TCHAR[LengthBuffer];
    GetWindowText(FHandle, szBuf, LengthBuffer);
    if (szBuf[0]) lstrcat(szBuf, _T("\r\n"));
    lstrcat(szBuf, S);
    SetWindowText(FHandle, szBuf);
    delete[] szBuf;
  }

  return (int) SendMessage(FHandle, EM_GETLINECOUNT, 0, 0) - 1;
}

//---------------------------------------------------------------------------
void TMemoLines::Insert(int Index, const AnsiString S) {
  TCHAR *szBuf;
  int LengthBuffer;
  int ich, j, Length;


  if (FHandle) {
    LengthBuffer = GetWindowTextLength(FHandle) + S.Length() + 2;
    ich = (int) SendMessage(FHandle, EM_LINEINDEX, (WPARAM) Index, 0);
    Length = S.Length();
    szBuf = new TCHAR[LengthBuffer];
    GetWindowText(FHandle, szBuf, LengthBuffer);
    for (j = LengthBuffer - 1 - Length - 2; j >= ich; j--) szBuf[j + Length + 2] = szBuf[j];
    memcpy(&szBuf[ich], S, Length);
    szBuf[ich + Length] = _T('\r');
    szBuf[ich + Length + 1] = _T('\n');
    SetWindowText(FHandle, szBuf);
    delete[] szBuf;
  }

}

//---------------------------------------------------------------------------
void TMemoLines::Delete(int Index) {
  TCHAR *szBuf;
  int LengthBuffer;
  int ich, Length;


  if (FHandle) {
    LengthBuffer = GetWindowTextLength(FHandle) + 2;
    ich = (int) SendMessage(FHandle, EM_LINEINDEX, (WPARAM) Index, 0);
    Length = (int) SendMessage(FHandle, EM_LINELENGTH, (WPARAM) ich, 0);
    szBuf = new TCHAR[LengthBuffer];
    GetWindowText(FHandle, szBuf, LengthBuffer);
    lstrcpy(&szBuf[ich], &szBuf[ich + Length + 2]);
    SetWindowText(FHandle, szBuf);
    delete[] szBuf;
  }

}

//---------------------------------------------------------------------------

