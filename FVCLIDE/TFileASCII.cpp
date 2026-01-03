//-----------------------------------------------------------------------------
//! @file TFileASCII.cpp
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

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

#include "TFileASCII.h"

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TFileASCII::TFileASCII(void) {
  // Initialisations
  hFile = INVALID_HANDLE_VALUE;
  FReadOnly = true;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TFileASCII::~TFileASCII(void) {
  if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ReadOnly
//---------------------------------------------------------------------------

bool TFileASCII::Get_ReadOnly(void) {
  return FReadOnly;
}

bool TFileASCII::Set_ReadOnly(bool NewReadOnly) {
  if (FReadOnly != NewReadOnly) {
    FReadOnly = NewReadOnly;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TFileASCII::Get_Name(void) {
  return FName;
}

bool TFileASCII::Set_Name(AnsiString NewName) {
  if (FName != NewName) {
    FName = NewName;
  }
  return true;
}


//---------------------------------------------------------------------------
// Ouverture d'un fichier ASCII
//---------------------------------------------------------------------------

bool TFileASCII::OpenFile(AnsiString asFileName) {

  CloseFile();

  if (FReadOnly) {
    hFile = CreateFile(asFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  }
  else {
    hFile = CreateFile(asFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  }

  if (hFile == INVALID_HANDLE_VALUE) {
    // Message d'erreur ouverture asFileName
    return false;
  }

  FName = asFileName;

  return true;
}

//---------------------------------------------------------------------------
// Fermeture du fichier.
//---------------------------------------------------------------------------

bool TFileASCII::CloseFile(void) {
  if (hFile != INVALID_HANDLE_VALUE) {
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    FName = _T("");
  }

  return true;
}

//---------------------------------------------------------------------------
// Lecture d'une donnée dans un fichier texte (séparateur = CR)
//---------------------------------------------------------------------------

bool TFileASCII::ReadLine(char *szBuffer, DWORD cchMaxBuffer) {
  char c;
  DWORD i = 0;
  DWORD dw;


  while (i < cchMaxBuffer - 1) {
    ReadFile(hFile, &c, 1, &dw, NULL);
    if (dw <= 0) {
      if (i) break;
      else return false;
    }
    if (c == '\n') break;
    if (c != '\r') szBuffer[i++] = (TCHAR) (BYTE) c;
  }
  szBuffer[i] = '\0';

  return true;
}

//---------------------------------------------------------------------------
// Lecture d'une donnée dans un fichier texte (séparateur = CR)
//---------------------------------------------------------------------------

bool TFileASCII::ReadLine(wchar_t *szBuffer, DWORD cchMaxBuffer) {
  char c;
  DWORD i = 0;
  DWORD dw;


  while (i < cchMaxBuffer - 1) {
    ReadFile(hFile, &c, 1, &dw, NULL);
    if (dw <= 0) {
      if (i) break;
      else return false;
    }
    if (c == '\n') break;
    if (c != '\r') szBuffer[i++] = (TCHAR) (BYTE) c;
  }
  szBuffer[i] = '\0';

  return true;
}

//---------------------------------------------------------------------------
// Lecture d'une donnée dans un fichier texte (séparateur = CR)
//---------------------------------------------------------------------------

bool TFileASCII::ReadLine(CharString *csBuffer) {
  char c;
  DWORD dw;


  *csBuffer = _T("");
  while (true) {
    ReadFile(hFile, &c, 1, &dw, NULL);
    if (dw <= 0) {
      if (!csBuffer->IsEmpty()) break;
      else return false;
    }
    if (c == '\n') break;
    if (c != '\r') *csBuffer += (BYTE) c;
  }

  return true;
}

//---------------------------------------------------------------------------
// Lecture d'une donnée dans un fichier texte (séparateur = CR)
//---------------------------------------------------------------------------

bool TFileASCII::ReadLine(WideString *wsBuffer) {
  char c;
  DWORD dw;


  *wsBuffer = _T("");
  while (true) {
    ReadFile(hFile, &c, 1, &dw, NULL);
    if (dw <= 0) {
      if (!wsBuffer->IsEmpty()) break;
      else return false;
    }
    if (c == '\n') break;
    if (c != '\r') *wsBuffer += (BYTE) c;
  }

  return true;
}

//---------------------------------------------------------------------------
// Ecriture d'une donnée dans un fichier texte (séparateur = CR LF)
//---------------------------------------------------------------------------

bool TFileASCII::WriteLine(const char * szBuffer, DWORD cbBuffer) {
  DWORD dw;


  if (!WriteFile(hFile, szBuffer, cbBuffer, &dw, NULL)) return false;
  if (!WriteFile(hFile, "\r\n", 2, &dw, NULL)) return false;

  return true;
}

//---------------------------------------------------------------------------
// Ecriture d'une donnée dans un fichier texte (séparateur = CR LF)
//---------------------------------------------------------------------------

bool TFileASCII::WriteLine(AnsiString asBuffer) {
  return WriteLine((LPCSTR) asBuffer.AsCharString(), asBuffer.Length());
}

//---------------------------------------------------------------------------


