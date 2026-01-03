//-----------------------------------------------------------------------------
//! @file MAJPadFile.cpp
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

#include "../../Include/Plugin.h"
#include "MAJPadFile.h"
#include "Resource.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
HINSTANCE HInstance;
HWND hWndSaisieParam;

wchar_t szFileSource[3 * MAX_PATH + 1]; // +1 pour avoir une liste de fichiers terminée par deux zéros
wchar_t szFileDest[MAX_PATH];
wchar_t szFileSetup[MAX_PATH];

CALLBACK_ERROR CallbackError;

//---------------------------------------------------------------------------
// Prototype des fonctions
//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectContent);
BOOL SaveProject(wchar_t *szProjectContent);
BOOL strcpytok(wchar_t *szToken, const wchar_t **lp);
BOOL MAJPadFile(void);
BOOL ReplaceAttribute(std::string *sBuffer, std::string sTagBegin, std::string sTagEnd, std::string sAttribute);
bool FileLineInput(HANDLE hFile, std::string *sBuffer);
std::string Ansi(std::wstring sStr);

BOOL CALLBACK SaisieParamProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SaisieParamInitDialog(void);
BOOL Button_OkClick(void);
BOOL Button_ParcourirSourceClick(void);
BOOL Button_ParcourirDestClick(void);
BOOL Button_ParcourirFileSetupClick(void);
BOOL Button_ParcourirFileVersionClick(void);
void CenterWindow(HWND hWnd);


//---------------------------------------------------------------------------
EXTERN_C BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ ) {

	HInstance = hInst;

  switch (reason) {
    case DLL_PROCESS_ATTACH:
      break;

    case DLL_PROCESS_DETACH:
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
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Init(CALLBACK_ERROR ACallbackError) {
	CallbackError = ACallbackError;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Install(void) {
	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap20(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_MAJPADFILE_20", IMAGE_BITMAP, 20, 20, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_MAJPADFILE_32", IMAGE_BITMAP, 32, 32, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_MAJPADFILE_48", IMAGE_BITMAP, 48, 48, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *wszDescription, int SizeDescription) {

	lstrcpynW(wszDescription, L"Mise à jour fichier PAD", SizeDescription);

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	szFileSource[0] = L'\0';
	szFileDest[0] = L'\0';
	szFileSetup[0] = L'\0';

	Ok = (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE);

	if (Ok) {
		szProjectFileName[0] = L'\0';
		if (!SaveProject(szProjectContent)) Ok = FALSE;
	}

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Open(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	if (szProjectContent[0]) {
		Ok = LoadProject(szProjectContent);
	}
	else {
		szFileSource[0] = L'\0';
		szFileDest[0] = L'\0';
		szFileSetup[0] = L'\0';
		Ok = TRUE;
	}

	if (Ok) {
		Ok = (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE);
	}

	if (Ok) {
		szProjectFileName[0] = L'\0';
		if (!SaveProject(szProjectContent)) Ok = FALSE;
	}

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetInputs(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szInputFileNames) {
	int i;


	if (LoadProject(szProjectContent)) {
		i = lstrlen(szFileSource) + 1;
		lstrcpy(&szFileSource[i], szFileSetup);
		i += lstrlen(szFileSetup) + 1;
		szFileSource[i] = L'\0';
		*szInputFileNames = szFileSource;
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetInputs(const wchar_t *szInputFileNames) {
	// Rien à faire (le buffer est statique)

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetOutput(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szOutputFileName) {

	if (LoadProject(szProjectContent)) {
		*szOutputFileName = szFileDest;
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetOutput(const wchar_t *szOutputFileName) {
	// Rien à faire (le buffer est statique)

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Build(const wchar_t *szProjectFileName, const wchar_t *szProjectContent) {
	wchar_t szMessage[MAX_PATH * 2 + 30];  // 30 = sizeof("Mise à jour fichier PAD") + 2 espaces + 4 guillemets + zéro terminal

	if (LoadProject(szProjectContent)) {

		wsprintf(szMessage, L"Mise à jour fichier PAD \"%s\" -> \"%s\"", szFileSource, szFileDest);
		CallbackError(szMessage, L"Information", MB_OK | MB_ICONINFORMATION);

		return MAJPadFile();

	}
	else {
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szProjectFileName
		wsprintf(szErrMsg, L"Impossible d'ouvrir le charger le projet");
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

}

//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectContent) {
	LPCWSTR lp;


	lp = szProjectContent;
	strcpytok(szFileSource, &lp);
	strcpytok(szFileDest, &lp);
	strcpytok(szFileSetup, &lp);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL strcpytok(wchar_t *szToken, const wchar_t **lp) {
	wchar_t c;
	int i;


	i = 0;
	while ((c = **lp) != '\0') {
		(*lp)++;
		if (c == L'|') break;
		szToken[i++] = c;
	}
		
	szToken[i++] = '\0';

	return true;
}

//---------------------------------------------------------------------------
BOOL SaveProject(wchar_t *szProjectContent) {

	if (lstrlen(szFileSource) + lstrlen(szFileDest) + lstrlen(szFileSetup) + 3 > 2048) {
	  CallbackError(L"Dépassement de la longueur maximum de fichiers (2048)", L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	wsprintfW(szProjectContent, L"%s|%s|%s", szFileSource, szFileDest, szFileSetup);
	
	return TRUE;
}

//---------------------------------------------------------------------------
BOOL MAJPadFile(void) {
  HANDLE hFile1, hFile2;
	DWORD dw;
	std::string sBuffer, sVersion, sFileSizeByte, sFileSizeK, sFileSizeMB;
	WIN32_FILE_ATTRIBUTE_DATA FileAttributeData;
	unsigned long long nFileSize;
	char szSize[40];
	BYTE FileVersionInfo[1024];
	VS_FIXEDFILEINFO *FixedFileInfo;
	UINT SizeFixedFileInfo;
	char szVersionInfo[128];


  hFile1 = CreateFile(szFileSource, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile1 == INVALID_HANDLE_VALUE) {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szFileSource
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\": %s", szFileSource, szBuf);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
  }

  hFile2 = CreateFile(szFileDest, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile2 == INVALID_HANDLE_VALUE) {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szFileDest
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\": %s", szFileDest, szBuf);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);

		CloseHandle(hFile1);

		return FALSE;
  }

	nFileSize = 0;
	if (GetFileAttributesEx(szFileSetup, GetFileExInfoStandard, &FileAttributeData) == FALSE) {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

		// Message d'erreur ouverture szFileSetup
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible de lire la taille du fichier \"%s\": %s", szFileSetup, szBuf);
		CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);

		CloseHandle(hFile1);
		CloseHandle(hFile2);

		return FALSE;
	}
	nFileSize = ((unsigned long long) FileAttributeData.nFileSizeHigh << 32) + FileAttributeData.nFileSizeLow;
	sprintf_s(szSize, sizeof(szSize), "%I64u", nFileSize); sFileSizeByte = szSize;
	sprintf_s(szSize, sizeof(szSize), "%I64u", (nFileSize + 512) / 1024); sFileSizeK = szSize;
	sprintf_s(szSize, sizeof(szSize), "%0.2lf", (double) nFileSize / (1024 * 1024)); sFileSizeMB = szSize;

	memset(FileVersionInfo, 0, sizeof(FileVersionInfo));
	if (GetFileVersionInfo(szFileSetup, NULL, sizeof(FileVersionInfo), FileVersionInfo) == FALSE) {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

		// Message d'erreur GetFileVersionInfo
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible de lire les invormations de versions du fichier \"%s\": %s", szFileSetup, szBuf);
		CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);

		CloseHandle(hFile1);
		CloseHandle(hFile2);

		return FALSE;
	}

	if (VerQueryValue(FileVersionInfo, L"\\", (LPVOID *) &FixedFileInfo, &SizeFixedFileInfo) == FALSE) {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

		// Message d'erreur VerQueryValue
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible de lire la version du fichier \"%s\": %s", szFileSetup, szBuf);
		CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);

		CloseHandle(hFile1);
		CloseHandle(hFile2);

		return FALSE;
	}

	wsprintfA(szVersionInfo, "%i.%i.%i.%i",
						FixedFileInfo->dwProductVersionMS >> 16,
						FixedFileInfo->dwProductVersionMS & 0xFFFF,
						FixedFileInfo->dwProductVersionLS >> 16,
						FixedFileInfo->dwProductVersionLS & 0xFFFF);
	sVersion = szVersionInfo;

	/*
	hFile3 = CreateFile(szFileVersion, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile3 == INVALID_HANDLE_VALUE) {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

		// Message d'erreur ouverture szFileVersion
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\": %s", szFileVersion, szBuf);
		CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);

		CloseHandle(hFile1);
		CloseHandle(hFile2);

		return FALSE;
	}

	FileLineInput(hFile3, &sVersion);
	*/

	while (FileLineInput(hFile1, &sBuffer)) {
		ReplaceAttribute(&sBuffer, "<Program_Version>", "</Program_Version>", sVersion);
		ReplaceAttribute(&sBuffer, "<File_Size_Bytes>", "</File_Size_Bytes>", sFileSizeByte);
		ReplaceAttribute(&sBuffer, "<File_Size_K>", "</File_Size_K>", sFileSizeK);
		ReplaceAttribute(&sBuffer, "<File_Size_MB>", "</File_Size_MB>", sFileSizeMB);
		WriteFile(hFile2, sBuffer.c_str(), (int) sBuffer.length(), &dw, NULL);
		WriteFile(hFile2, "\r\n", 2, &dw, NULL);
	}

	CloseHandle(hFile1);
  CloseHandle(hFile2);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL ReplaceAttribute(std::string *sBuffer, std::string sTagBegin, std::string sTagEnd, std::string sAttribute) {
	size_t PosTagBegin, PosTagEnd;


	PosTagBegin = sBuffer->find(sTagBegin, 0);
	PosTagEnd = sBuffer->find(sTagEnd, PosTagBegin);
	if (PosTagBegin != std::string::npos && PosTagEnd != std::string::npos) {
		*sBuffer = sBuffer->replace(PosTagBegin + sTagBegin.length(), PosTagEnd - PosTagBegin - sTagBegin.length(), sAttribute);
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
// SaisieParamProc
//---------------------------------------------------------------------------
BOOL CALLBACK SaisieParamProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

  switch (message) {
  case WM_INITDIALOG:
    hWndSaisieParam = hDlg;
		SaisieParamInitDialog();
    return TRUE;

  case WM_COMMAND:

    switch (LOWORD(wParam)) {

    case BUTTON_PARCOURIRSOURCE:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirSourceClick();
      }
      break;

    case BUTTON_PARCOURIRDEST:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirDestClick();
      }
      break;

    case BUTTON_PARCOURIRFILESETUP:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirFileSetupClick();
     }
      break;

    case BUTTON_OK:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_OkClick();
      }
      break;

    case BUTTON_ANNULER:
      if (HIWORD(wParam) == BN_CLICKED) {
			  EndDialog(hWndSaisieParam, FALSE);
      }
      break;

    }

    return TRUE;

  case WM_CLOSE:
	  EndDialog(hWndSaisieParam, FALSE);
    return TRUE;

  }

  return 0;
}

//---------------------------------------------------------------------------
void CenterWindow(HWND hWnd) {
	HWND hWndParent;
  RECT Rect, RectParent;
  int WidthParent;
  int HeightParent;
  int Width;
  int Height;


	hWndParent = GetParent(hWnd);
	if (hWndParent == NULL) hWndParent = GetDesktopWindow();

	GetWindowRect(hWnd, &Rect);
	GetWindowRect(hWndParent, &RectParent);
  Width = Rect.right - Rect.left;
  Height = Rect.bottom - Rect.top;
  WidthParent = RectParent.right - RectParent.left;
  HeightParent = RectParent.bottom - RectParent.top;

  MoveWindow(hWnd,
						 RectParent.left + WidthParent / 2 - Width / 2,
						 RectParent.top + HeightParent / 2 - Height / 2,
						 Width, Height,
						 TRUE);

}

//---------------------------------------------------------------------------
BOOL SaisieParamInitDialog(void) {
	CenterWindow(hWndSaisieParam);
	SetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szFileSource);
	SetDlgItemText(hWndSaisieParam, EDIT_DEST, szFileDest);
	SetDlgItemText(hWndSaisieParam, EDIT_FILESETUP, szFileSetup);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirSourceClick(void) {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szFileName, sizeof(szFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Fichiers XML (*.xml)\0*.xml\0Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.lpstrTitle = L"Fichier PAD de départ";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_SOURCE));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirDestClick(void) {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_DEST, szFileName, sizeof(szFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Fichiers XML (*.xml)\0*.xml\0Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.lpstrTitle = L"Fichier PAD destination";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_DEST, szFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_DEST));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirFileSetupClick(void) {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_FILESETUP, szFileName, sizeof(szFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Fichiers exécutables (*.exe)\0*.exe\0Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.lpstrTitle = L"Fichier de Setup";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_FILESETUP, szFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_FILESETUP));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_OkClick(void) {
	GetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szFileSource, sizeof(szFileSource) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_DEST, szFileDest, sizeof(szFileDest) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_FILESETUP, szFileSetup, sizeof(szFileSetup) / sizeof(wchar_t));

  EndDialog(hWndSaisieParam, TRUE);

	return TRUE;
}

//---------------------------------------------------------------------------
//   bool FileLineInput(HFILE hFile, BYTE * szBuffer, DWORD cbMaxLenght)
// Description:
//   Lecture d'une donnée dans un fichier texte (séparateur = CR)
// Paramètres:
//   [in]  HFILE hFile = handle du fichier
//   [out] szBuffer = ligne lue
//   [in]  DWORD cbMaxBuffer = longueur du buffer
// Sortie:
//   FALSE si fin de fichier atteinte ou erreur de lecture
//---------------------------------------------------------------------------

bool FileLineInput(HANDLE hFile, std::string *sBuffer) {
  char c;
  DWORD dwBytesRead;


  *sBuffer = "";
  while (true) {
    ReadFile(hFile, &c, 1, &dwBytesRead, NULL);
    if (dwBytesRead != 1) {
      if (!sBuffer->empty()) break;
      else return false;
    }
    if (c == '\n') break;
    if (c != '\r') *sBuffer += c;
  }

  return true;
}

//---------------------------------------------------------------------------
std::string Ansi(std::wstring wsStr) {
  std::string sRet;
  int Len;
	int i;


  Len = (int) wsStr.length();
	for (i = 0; i < Len; i++) {
		sRet += (char) (BYTE) wsStr[i];
	}

  return sRet;
}

//---------------------------------------------------------------------------
