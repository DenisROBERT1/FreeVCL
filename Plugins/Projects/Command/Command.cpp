//-----------------------------------------------------------------------------
//! @file Command.cpp
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
#include <time.h>
#include <vector>

#include "../../Include/Plugin.h"
#include "Command.h"
#include "Resource.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
HINSTANCE HInstance;
HWND hWndSaisieParam;

wchar_t szProgram[MAX_PATH + 1]; // +1 pour avoir une liste de fichiers terminée par deux zéros
wchar_t szParameters[MAX_PATH];
wchar_t szOutput[MAX_PATH];

CALLBACK_ERROR CallbackError;

//---------------------------------------------------------------------------
// Prototype des fonctions
//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectFileName);
BOOL SaveProject(const wchar_t *szProjectFileName);
BOOL GetProjectFileName(HWND hWnd, wchar_t *szProjectFileName);
BOOL ReadLine(HANDLE hFile, wchar_t * szBuffer, DWORD cbMaxBuffer);

BOOL CALLBACK SaisieParamProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SaisieParamInitDialog(void);
BOOL Button_OkClick(void);
BOOL Button_ParcourirProgClick(void);
BOOL Button_ParcourirOutputClick(void);
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
	return (HBITMAP) LoadImage(HInstance, L"BMP_COMMAND", IMAGE_BITMAP, 20, 20, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_COMMAND", IMAGE_BITMAP, 32, 32, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_COMMAND", IMAGE_BITMAP, 48, 48, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *wszDescription, int SizeDescription) {

	lstrcpynW(wszDescription, L"Commande DOS", SizeDescription);

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	szProgram[0] = L'\0';
	szParameters[0] = L'\0';
	szOutput[0] = L'\0';

	Ok = (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE);

	if (Ok) {
		if (!szProjectFileName[0]) {
			if (!GetProjectFileName(hWnd, szProjectFileName)) {
				Ok = FALSE;
			}
		}
	}

	if (Ok) {
		if (!SaveProject(szProjectFileName)) Ok = FALSE;
	}

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Open(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	if (szProjectFileName[0]) {
		Ok = LoadProject(szProjectFileName);
	}
	else {
		szProgram[0] = L'\0';
		szParameters[0] = L'\0';
		szOutput[0] = L'\0';
		Ok = TRUE;
	}

	if (Ok) {
		Ok = (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE);
	}

	if (Ok) {
		if (!szProjectFileName[0]) {
			if (!GetProjectFileName(hWnd, szProjectFileName)) {
				Ok = FALSE;
			}
		}
	}

	if (Ok) {
		if (!SaveProject(szProjectFileName)) Ok = FALSE;
	}

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetInputs(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szInputFileNames) {

	// Aucun input (les fichiers input devront être ajoutés à la main)
	*szInputFileNames = L"\0\0";
	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetInputs(const wchar_t *szInputFileNames) {
	// Rien à faire (le buffer est statique)

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetOutput(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szOutputFileName) {

	if (LoadProject(szProjectFileName)) {
		*szOutputFileName = szOutput;
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
	wchar_t szMessage[MAX_PATH * 2 + 4];  // 11 = 1 espace + 2 guillemets + zéro terminal
	wchar_t szErrMsg[128 + 100 + MAX_PATH];
	HINSTANCE hInstShell;
	WIN32_FILE_ATTRIBUTE_DATA FileAttributeData;
	FILETIME ftOutputLastWriteTime1, ftOutputLastWriteTime2;
	time_t Timeout1, Timeout2;


	if (LoadProject(szProjectFileName)) {
		if (szProgram[0]) {
			wsprintf(szMessage, L"\"%s\" %s", szProgram, szParameters);
			CallbackError(szMessage, L"Information", MB_OK | MB_ICONINFORMATION);

			if (GetFileAttributesEx(szOutput, GetFileExInfoStandard, &FileAttributeData)) {
				ftOutputLastWriteTime1 = FileAttributeData.ftLastWriteTime;
			}
			else {
				ftOutputLastWriteTime1.dwHighDateTime = 0;
				ftOutputLastWriteTime1.dwLowDateTime = 0;
			}

			hInstShell = ShellExecute(NULL, NULL, szProgram, szParameters, NULL, SW_SHOW);
			if ((int) hInstShell <= 32) {
				wsprintf(szErrMsg, L"La commande a renvoyé l'erreur: %i", hInstShell);
				if ((int) hInstShell == ERROR_FILE_NOT_FOUND) lstrcat(szErrMsg, L" \"Fichier non trouvé\"");
				else if ((int) hInstShell == ERROR_PATH_NOT_FOUND) lstrcat(szErrMsg, L" \"Chemin non trouvé\"");
				else if ((int) hInstShell == ERROR_BAD_FORMAT) lstrcat(szErrMsg, L" \"Programme non compatible\"");
				else if ((int) hInstShell == SE_ERR_ACCESSDENIED) lstrcat(szErrMsg, L" \"Accès interdit\"");
				else if ((int) hInstShell == SE_ERR_DLLNOTFOUND) lstrcat(szErrMsg, L" \"Dll manquante\"");
				else if ((int) hInstShell == SE_ERR_NOASSOC) lstrcat(szErrMsg, L" \"Aucune application associée à l'extension\"");
				else if ((int) hInstShell == SE_ERR_SHARE) lstrcat(szErrMsg, L" \"Violation de partage\"");
				CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
				return FALSE;
			}
			// Attente de la création du fichier de sortie (qui peut être postérieure à la
			// fin de la commande). Tester l'existence ne suffit pas, il peut déjà exister
			// mais ne pas être à jour => il faut tester un changement de date.
			Timeout1 = time(NULL);
			do {
				Timeout2 = time(NULL);
				if (Timeout2 - Timeout1 > 300) {
					CallbackError(L"Délai maximum écoulé.", L"Erreur", MB_OK | MB_ICONWARNING);
					return FALSE;
				}
				if (GetFileAttributesEx(szOutput, GetFileExInfoStandard, &FileAttributeData)) {
					ftOutputLastWriteTime2 = FileAttributeData.ftLastWriteTime;
				}
				else {
					ftOutputLastWriteTime2.dwHighDateTime = 0;
					ftOutputLastWriteTime2.dwLowDateTime = 0;
				}
			} while (ftOutputLastWriteTime1.dwLowDateTime == ftOutputLastWriteTime2.dwLowDateTime &&
							 ftOutputLastWriteTime1.dwHighDateTime == ftOutputLastWriteTime2.dwHighDateTime);
		}
		else {
			lstrcpy(szErrMsg, L"Commande vide");
			CallbackError(szErrMsg, L"Information", MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	}
	else {
		wchar_t szBuf[128];

    // Message d'erreur ouverture szProjectFileName
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\": %s", szProjectFileName, szBuf);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectFileName) {
  HANDLE hFile;


  hFile = CreateFile(szProjectFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
		ReadLine(hFile, szProgram, MAX_PATH);
		ReadLine(hFile, szParameters, MAX_PATH);
		ReadLine(hFile, szOutput, MAX_PATH);
    CloseHandle(hFile);
		return TRUE;
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
BOOL SaveProject(const wchar_t *szProjectFileName) {
  HANDLE hFile;
	DWORD dw;


  hFile = CreateFile(szProjectFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
		WriteFile(hFile, szProgram, lstrlen(szProgram) * sizeof(wchar_t), &dw, NULL);
		WriteFile(hFile, L"\r\n", 2 * sizeof(wchar_t), &dw, NULL);
		WriteFile(hFile, szParameters, lstrlen(szParameters) * sizeof(wchar_t), &dw, NULL);
		WriteFile(hFile, L"\r\n", 2 * sizeof(wchar_t), &dw, NULL);
		WriteFile(hFile, szOutput, lstrlen(szOutput) * sizeof(wchar_t), &dw, NULL);
		WriteFile(hFile, L"\r\n", 2 * sizeof(wchar_t), &dw, NULL);
    CloseHandle(hFile);
		return TRUE;
  }
  else {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szProjectFileName
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'enregistrer le fichier \"%s\": %s", szProjectFileName, szBuf);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
  }

}

//---------------------------------------------------------------------------
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

    case BUTTON_PARCOURIRPROG:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirProgClick();
      }
      break;

    case BUTTON_PARCOURIROUTPUT:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirOutputClick();
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
	SetDlgItemText(hWndSaisieParam, EDIT_PROG, szProgram);
	SetDlgItemText(hWndSaisieParam, EDIT_ARGS, szParameters);
	SetDlgItemText(hWndSaisieParam, EDIT_OUTPUT, szOutput);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirProgClick(void) {
	OPENFILENAME ofn;
	wchar_t szProgFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_PROG, szProgFileName, sizeof(szProgFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Exécutables (*.exe)\0*.exe\0Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szProgFileName;
  ofn.lpstrTitle = L"Programme";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_PROG, szProgFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_PROG));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirOutputClick(void) {
	OPENFILENAME ofn;
	wchar_t szOutputFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_OUTPUT, szOutputFileName, sizeof(szOutputFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szOutputFileName;
  ofn.lpstrTitle = L"Fichier de sortie";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_OUTPUT, szOutputFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_OUTPUT));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirDestClick(void) {
	OPENFILENAME ofn;
	wchar_t szDestFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_ARGS, szDestFileName, sizeof(szDestFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szDestFileName;
  ofn.lpstrTitle = L"Fichier à copier";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_ARGS, szDestFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_ARGS));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_OkClick(void) {
	GetDlgItemText(hWndSaisieParam, EDIT_PROG, szProgram, sizeof(szProgram) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_ARGS, szParameters, sizeof(szProgram) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_OUTPUT, szOutput, sizeof(szOutput) / sizeof(wchar_t));

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
