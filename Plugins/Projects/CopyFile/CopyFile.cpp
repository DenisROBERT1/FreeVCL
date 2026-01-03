//-----------------------------------------------------------------------------
//! @file CopyFile.cpp
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
#include "CopyFile.h"
#include "Resource.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
HINSTANCE HInstance;
HWND hWndSaisieParam;

wchar_t szFileSource[MAX_PATH + 1]; // +1 pour avoir une liste de fichiers terminée par deux zéros
wchar_t szFileDest[MAX_PATH];

CALLBACK_ERROR CallbackError;

//---------------------------------------------------------------------------
// Prototype des fonctions
//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectContent);
BOOL SaveProject(wchar_t *szProjectContent);
BOOL ReadLine(HANDLE hFile, wchar_t * szBuffer, DWORD cbMaxBuffer);
BOOL Touch(const wchar_t *szFileName);

BOOL CALLBACK SaisieParamProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SaisieParamInitDialog(void);
BOOL Button_OkClick(void);
BOOL Button_ParcourirSourceClick(void);
BOOL Button_ParcourirDestClick(void);
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
	return (HBITMAP) LoadImage(HInstance, L"BMP_COPYFILE", IMAGE_BITMAP, 20, 20, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_COPYFILE", IMAGE_BITMAP, 32, 32, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_COPYFILE", IMAGE_BITMAP, 48, 48, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *wszDescription, int SizeDescription) {

	lstrcpynW(wszDescription, L"Copie d'un fichier", SizeDescription);

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	szFileSource[0] = L'\0';
	szFileDest[0] = L'\0';

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
		Ok = TRUE;
	}

	if (Ok) {
		Ok = (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE);
	}

	/*
	if (Ok) {
		if (!szProjectFileName[0] && !szProjectContent[0]) {
			if (!GetProjectFileName(hWnd, szProjectFileName)) {
				Ok = FALSE;
			}
		}
	}
	*/

	if (Ok) {
		szProjectFileName[0] = L'\0';
		if (!SaveProject(szProjectContent)) Ok = FALSE;
	}

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetInputs(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szInputFileNames) {

	if (LoadProject(szProjectContent)) {
		szFileSource[lstrlen(szFileSource) + 1] = L'\0';
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
	wchar_t szMessage[MAX_PATH * 2 + 11];  // 11 = sizeof("COPY") + 2 espaces + 4 guillemets + zéro terminal

	if (LoadProject(szProjectContent)) {

		wsprintf(szMessage, L"COPY \"%s\" \"%s\"", szFileSource, szFileDest);
		CallbackError(szMessage, L"Information", MB_OK | MB_ICONINFORMATION);

		if (CopyFile(szFileSource, szFileDest, FALSE)) {
			Touch(szFileDest);
			return TRUE;
		}
		else {
			wchar_t szBuf[128];
			wchar_t szErrMsg[128 + 100 + MAX_PATH];

			// Message d'erreur copie impossible
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
			wsprintf(szErrMsg, L"Copie impossible: %s", szBuf);
			CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
			return FALSE;
		}

	}
	else {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szProjectFileName
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\": %s", szProjectFileName, szBuf);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

}

//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectContent) {
  int Pos;
	int i;
	wchar_t c;


	i = -1;
	Pos = -1;
	while (c = szProjectContent[++i]) {
		if (c == L'|') {
			Pos = i;
			break;
		}
	}
		
	if (Pos == -1) return FALSE;

	lstrcpyn(szFileSource, szProjectContent, Pos + 1);
	lstrcpyn(szFileDest, &szProjectContent[Pos + 1], lstrlen(szProjectContent) - Pos);
	
	return TRUE;
}

//---------------------------------------------------------------------------
BOOL SaveProject(wchar_t *szProjectContent) {

	if (lstrlen(szFileSource) + lstrlen(szFileDest) + 2 > 2048) {
	  CallbackError(L"Dépassement de la longueur maximum de fichiers (2048)", L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	wsprintfW(szProjectContent, L"%s|%s", szFileSource, szFileDest);
	
	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Touch(const wchar_t *szFileName) {
  HANDLE hFile;


  hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, 0, NULL, FILE_END);
		SetEndOfFile(hFile);
    CloseHandle(hFile);
		return TRUE;
  }
  else {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szFileName
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\": %s", szFileName, szBuf);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
  }

}

//---------------------------------------------------------------------------
/*
BOOL GetProjectFileName(HWND hWnd, wchar_t *szProjectFileName) {
	OPENFILENAME ofn;


	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWnd;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szProjectFileName;
  ofn.lpstrTitle = L"Nom du fichier paramètres";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
  if (GetSaveFileName((LPOPENFILENAME) &ofn)) return TRUE;

  return FALSE;
} */

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

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirSourceClick(void) {
	OPENFILENAME ofn;
	wchar_t szSourceFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szSourceFileName, sizeof(szSourceFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szSourceFileName;
  ofn.lpstrTitle = L"Fichier à copier";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szSourceFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_SOURCE));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirDestClick(void) {
	OPENFILENAME ofn;
	wchar_t szDestFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_DEST, szDestFileName, sizeof(szDestFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szDestFileName;
  ofn.lpstrTitle = L"Fichier destination";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_DEST, szDestFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_DEST));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_OkClick(void) {
	GetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szFileSource, sizeof(szFileSource) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_DEST, szFileDest, sizeof(szFileDest) / sizeof(wchar_t));

  EndDialog(hWndSaisieParam, TRUE);

	return TRUE;
}

//---------------------------------------------------------------------------
//   BOOL ReadLine(HFILE hFile, BYTE * szBuffer, DWORD cbMaxLenght)
// Description:
//   Lecture d'une donnée dans un fichier texte (séparateur = CR)
// Paramètres:
//   [in]  HFILE hFile = handle du fichier
//   [out] szBuffer = ligne lue
//   [in]  DWORD cbMaxBuffer = longueur du buffer
// Sortie:
//   FALSE si fin de fichier atteinte ou erreur de lecture
//---------------------------------------------------------------------------

BOOL ReadLine(HANDLE hFile, wchar_t * szBuffer, DWORD cbMaxBuffer) {
  wchar_t c;
  DWORD i = 0;
  DWORD dw;


  while (i < cbMaxBuffer - 1) {
    ReadFile(hFile, &c, sizeof(wchar_t), &dw, NULL);
    if (dw <= 0) {
      if (i) break;
      else return FALSE;
    }
    if (c == L'\n') break;
    if (c != L'\r') szBuffer[i++] = c;
  }
  szBuffer[i] = L'\0';

  return TRUE;
}

//---------------------------------------------------------------------------
