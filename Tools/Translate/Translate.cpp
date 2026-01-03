//-----------------------------------------------------------------------------
//! @file Translate.cpp
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
#include <windows.h>
#include <vector>

#include "Translate.h"

//---------------------------------------------------------------------------
int FindSrc(LPSTR szLigne);
int FindSrcDef(LPSTR szLigne);
bool GetModuleFilePath(LPTSTR szPath, DWORD nSize);
BOOL LoadLanguageDef(LPSTR szNewLanguage);
BOOL LoadLanguage(LPSTR szNewLanguage);

//---------------------------------------------------------------------------
HINSTANCE hModule;
char szLanguage[50];
int FileSize;
int SizeContenu;
char *szContenu;
std::vector<LPSTR> Src;
std::vector<LPSTR> Dest;

int FileSizeDef;
int SizeContenuDef;
char *szContenuDef;
std::vector<LPSTR> SrcDef;
std::vector<LPSTR> DestDef;


//---------------------------------------------------------------------------
EXTERN_C BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ ) {
  char szNewLanguage[50];


	hModule = hInst;

  switch (reason) {
    case DLL_PROCESS_ATTACH:
      LectureConfig(szNewLanguage);
      if (szNewLanguage[0] == '\0') {
        GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SLANGUAGE, szNewLanguage, sizeof(szNewLanguage));
      }
      if (!lstrcmp(szNewLanguage, "Français (France)")) szNewLanguage[0] = '\0';
      Translate_SetLanguage(szNewLanguage);
      break;

    case DLL_PROCESS_DETACH:
      delete[] szContenu;
      szContenu = NULL;
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;
  }

  /* Returns TRUE on success, FALSE on failure */
  return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Translate_ChooseLanguage(HWND hWndParent) {
  int Choix;
  HMENU hMenu;
  POINT Click;
  WIN32_FIND_DATA FindFileData;
  HANDLE hFindFile;
  char szPath[MAX_PATH];
  char szBuf[MAX_PATH];
  int Flags;


  // MenuPopup
  GetCursorPos(&Click);

  GetModuleFilePath(szPath, MAX_PATH);
  wsprintf(szBuf, "%s\\*.lng", szPath);
  hMenu = CreatePopupMenu();
  Choix = 0;
  Flags = MF_ENABLED | MF_STRING;
  if (!lstrcmp(szLanguage, "")) Flags |= MF_CHECKED;
  AppendMenu(hMenu, Flags, ++Choix, "Français (France)");

  hFindFile = FindFirstFile(szBuf, &FindFileData);
  if (hFindFile != INVALID_HANDLE_VALUE) {

    do {

      if (FindFileData.cFileName[0] != '.' &&
          !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
          !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)) {
        lstrcpy(szBuf, FindFileData.cFileName);
        szBuf[lstrlen(szBuf) - 4] = '\0';
        Flags = MF_ENABLED | MF_STRING;
        if (!lstrcmp(szLanguage, szBuf)) Flags |= MF_CHECKED;
        AppendMenu(hMenu, Flags, ++Choix, szBuf);
      }

    } while (FindNextFile(hFindFile, &FindFileData));

    FindClose(hFindFile);

  }

  Choix = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON,
                         Click.x + 5, Click.y + 10, 0, hWndParent, NULL);
  szBuf[0] = '\0';
  if (Choix != 1) GetMenuString(hMenu, Choix, szBuf, sizeof(szBuf), MF_BYCOMMAND);

  DestroyMenu(hMenu);

  if (Choix != 0) {
    Translate_SetLanguage(szBuf);
    return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT LPSTR WINAPI Translate_GetLanguage() {
  return szLanguage;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Translate_SetLanguage(LPSTR szNewLanguage) {

	if (!lstrcmp(szLanguage, szNewLanguage)) return TRUE;

	if (LoadLanguage(szNewLanguage)) {
		LoadLanguageDef(szNewLanguage);
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT LPSTR WINAPI Translate_Translate(LPSTR szLigne) {
  int i, j;


  if (szLanguage[0] && szLigne[0]) {
    i = FindSrc(szLigne);
    if (i >= 0) return Dest[i];

    j = FindSrcDef(szLigne);
    if (j >= 0) {
			Translate_Add(szLigne, DestDef[j]);
			return DestDef[j];
		}
    else Translate_Add(szLigne, szLigne);

  }

  return szLigne;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Translate_Add(LPSTR szSrc, LPSTR szDest) {
  char szPath[MAX_PATH];
  char szFile[MAX_PATH];
  HANDLE hFile;
  DWORD dw;
  BOOL Ok = FALSE;
	char c;
  int i;


  if (!szLanguage[0]) return FALSE;

  GetModuleFilePath(szPath, MAX_PATH);
  wsprintf(szFile, "%s\\%s.lng", szPath, szLanguage);
  hFile = CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    SetFilePointer(hFile, 0, NULL, FILE_END);
		for (i = 0; i < lstrlen(szSrc); i++) {
			c = szSrc[i];
			if (c == '\n') c = '¬';
			else if (c == '\r') c = '¶';
	    WriteFile(hFile, &c, 1, &dw, NULL);
		}
    WriteFile(hFile, "µ", 1, &dw, NULL);
		for (i = 0; i < lstrlen(szDest); i++) {
			c = szDest[i];
			if (c == '\n') c = '¬';
			else if (c == '\r') c = '¶';
	    WriteFile(hFile, &c, 1, &dw, NULL);
		}
    WriteFile(hFile, "\r\n", 2, &dw, NULL);
    CloseHandle(hFile);
    Ok = TRUE;
  }

	// Redimensionnement de la table
  if (SizeContenu < FileSize + lstrlen(szSrc) + lstrlen(szDest) + 2) {
    char *szContenu2;
    SizeContenu = FileSize + lstrlen(szSrc) + lstrlen(szDest) + 1000;
    szContenu2 = new char[SizeContenu];
    memcpy(szContenu2, szContenu, FileSize);
		// Décalage des pointeurs
		for (i = 0; i < (int) Src.size(); i++) {
			Src[i] += szContenu2 - szContenu;
			Dest[i] += szContenu2 - szContenu;
		}
    delete szContenu;
    szContenu = szContenu2;
  }

  Src.push_back(&szContenu[FileSize]);
  lstrcpy(&szContenu[FileSize], szSrc);
  FileSize += (lstrlen(szSrc) + 1) * sizeof(char);

  Dest.push_back(&szContenu[FileSize]);
  lstrcpy(&szContenu[FileSize], szDest);
  FileSize += (lstrlen(szDest) + 1) * sizeof(char);

  return Ok;
}

//---------------------------------------------------------------------------
BOOL LoadLanguageDef(LPSTR szNewLanguage) {
  char szPath[MAX_PATH];
  char szFile[MAX_PATH];
  HANDLE hFile;
  DWORD dw;
  int i;
  BOOL Ok = FALSE;
  bool bSrc, bDest;


  delete szContenuDef;
  szContenuDef = NULL;
  SrcDef.clear();
  DestDef.clear();
  if (!szLanguage[0]) return TRUE;

  GetModuleFilePath(szPath, MAX_PATH);
  wsprintf(szFile, "%s\\%s.def", szPath, szLanguage);
  hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    FileSizeDef = GetFileSize(hFile,	NULL);
    SizeContenuDef = FileSizeDef;
    szContenuDef = new char[SizeContenuDef];
    ReadFile(hFile, szContenuDef, FileSizeDef, &dw, NULL);
    CloseHandle(hFile);
    Ok = TRUE;
  }
  else {
    FileSizeDef = 0;
    SizeContenuDef = 0;
    szContenuDef = NULL;
  }

  bSrc = true;
  bDest = false;
  for (i = 0; i < FileSizeDef; i++) {

    if (szContenuDef[i] == '¤' || szContenuDef[i] == 'µ') {
      szContenuDef[i] = '\0';
      bDest = true;
    }

    else if (szContenuDef[i] == '\r' || szContenuDef[i] == '\n') {
      szContenuDef[i] = '\0';
      bSrc = true;
    }

    else {
      if (bSrc) {
        if (SrcDef.size() == DestDef.size()) {
          SrcDef.push_back(&szContenuDef[i]);
        }
        bSrc = false;
      }
      if (bDest) {
        if (SrcDef.size() == DestDef.size() + 1) {
          DestDef.push_back(&szContenuDef[i]);
        }
        bDest = false;
      }
    }

  }

  return Ok;
}

//---------------------------------------------------------------------------
BOOL LoadLanguage(LPSTR szNewLanguage) {
  char szPath[MAX_PATH];
  char szFile[MAX_PATH];
  HANDLE hFile;
  DWORD dw;
  int i;
  BOOL Ok = FALSE;
  bool bSrc, bDest;


  lstrcpyn(szLanguage, szNewLanguage, sizeof(szLanguage));
  EcritureConfig(szLanguage);

  delete szContenu;
  szContenu = NULL;
  Src.clear();
  Dest.clear();
  if (!szLanguage[0]) return TRUE;

  GetModuleFilePath(szPath, MAX_PATH);
  wsprintf(szFile, "%s\\%s.lng", szPath, szLanguage);
  hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    FileSize = GetFileSize(hFile,	NULL);
    SizeContenu = FileSize + 1000;
    szContenu = new char[SizeContenu];
    ReadFile(hFile, szContenu, FileSize, &dw, NULL);
    CloseHandle(hFile);
    Ok = TRUE;
  }
  else {
    FileSize = 0;
    SizeContenu = 1000;
    szContenu = new char[SizeContenu];
  }

  bSrc = true;
  bDest = false;
  for (i = 0; i < FileSize; i++) {

    if (szContenu[i] == '¤' || szContenu[i] == 'µ') {
      szContenu[i] = '\0';
      bDest = true;
    }

    else if (szContenu[i] == '\r' || szContenu[i] == '\n') {
      szContenu[i] = '\0';
      bSrc = true;
    }

    else if (szContenu[i] == '¶') {
      szContenu[i] = '\r';
    }

    else if (szContenu[i] == '¬') {
      szContenu[i] = '\n';
    }

    else {
      if (bSrc) {
        if (Src.size() == Dest.size()) {
          Src.push_back(&szContenu[i]);
        }
        bSrc = false;
      }
      if (bDest) {
        if (Src.size() == Dest.size() + 1) {
          Dest.push_back(&szContenu[i]);
        }
        bDest = false;
      }
    }

  }

  return Ok;
}

//---------------------------------------------------------------------------
int FindSrcDef(LPSTR szLigne) {
  int i;


  for (i = 0; i < (int) SrcDef.size(); i++) {
    if (!lstrcmp(szLigne, SrcDef[i])) return i;
  }

  return -1;
}

//---------------------------------------------------------------------------
int FindSrc(LPSTR szLigne) {
  int i;


  for (i = 0; i < (int) Src.size(); i++) {
    if (!lstrcmp(szLigne, Src[i])) return i;
  }

  return -1;
}

//---------------------------------------------------------------------------
BOOL LectureConfig(LPSTR szLanguage) {
  HKEY hKey;
  DWORD dwSize, dwRead;
  int rc;


  szLanguage[0] = '\0';

  rc = RegOpenKeyEx(HKEY_CURRENT_USER,
                    "Software\\Denis ROBERT\\Translate",
                    0, KEY_READ, &hKey);
  if (rc != ERROR_SUCCESS) return FALSE;

  // Langage courant
  dwSize = 50;
  rc = RegQueryValueEx(hKey, "Language", NULL, &dwRead,
                       (BYTE*) szLanguage, &dwSize);

  RegCloseKey(hKey);

  return (rc == ERROR_SUCCESS);
}

//---------------------------------------------------------------------------
BOOL EcritureConfig(LPSTR szLanguage) {
  HKEY hKey = NULL;
  DWORD dwSize;
  int rc;


  rc = RegCreateKeyEx(HKEY_CURRENT_USER,
                      "Software\\Denis ROBERT\\Translate",
                      0,
                      0,
                      REG_OPTION_NON_VOLATILE,
                      KEY_ALL_ACCESS,
                      NULL,
                      &hKey,
                      &dwSize);
  if (rc != ERROR_SUCCESS) return FALSE;

  // Langage courant
  rc = RegSetValueEx(hKey, "Language", 0, REG_SZ,
                     (BYTE*) szLanguage, lstrlen(szLanguage));

  RegCloseKey(hKey);

  return (rc == ERROR_SUCCESS);
}

bool GetModuleFilePath(LPTSTR szPath, DWORD nSize) {
	int i;


	GetModuleFileName(hModule, szPath, nSize);
  i = lstrlen(szPath) - 1;
  while (i > 0 && szPath[i] != '\\' && szPath[i] != '/' && szPath[i] != ':') i--;
  szPath[i] = '\0';

	return true;
}

//---------------------------------------------------------------------------
