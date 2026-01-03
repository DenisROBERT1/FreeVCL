//-----------------------------------------------------------------------------
//! @file Doxygen.cpp
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
#include <shlobj.h>
#include <Shlwapi.h>

#include "../../Include/Plugin.h"
#include "../../Include/PluginUtilities.h"
#include "Doxygen.h"
#include "Resource.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
HINSTANCE HInstance;
HWND hWndSaisieParam;

CALLBACK_ERROR CallbackError;
wchar_t *szPtrProjectFileName;
wchar_t *szPtrInputFileNames;
wchar_t *szPtrOutputFileName;

//---------------------------------------------------------------------------
// Prototype des fonctions
//---------------------------------------------------------------------------
BOOL CALLBACK SaisieParamProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SaisieParamInitDialog(void);
BOOL Button_OkClick(void);
BOOL Button_ParcourirParamClick(void);
void CenterWindow(HWND hWnd);
BOOL GetInstallPath(wchar_t * szInstallPath, DWORD cbMaxInstallPath);
BOOL SetInstallPath(wchar_t * szInstallPath);
BOOL SelectInstallPath(wchar_t *szPath);
int CALLBACK SelectDirectoryCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL ParseFileDoxygen(std::wstring sFileName, std::wstring &sInputFileNames, std::wstring &sOutputFileName);


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
  wchar_t szInstallPath[MAX_PATH];
	wchar_t szProgName[MAX_PATH];
	int Len;


	if (GetInstallPath(szInstallPath, MAX_PATH)) return TRUE;

	// Recherche automatique du programme DOXYGEN
	GetEnvironmentVariable(L"ProgramFiles(x86)", szInstallPath, MAX_PATH);
	lstrcat(szInstallPath, L"\\doxygen\\bin");
	wsprintf(szProgName, L"%s\\doxygen.exe", szInstallPath);
	if (GetFileAttributes(szProgName) == -1) {

		GetEnvironmentVariable(L"ProgramFiles", szInstallPath, MAX_PATH);
		lstrcat(szInstallPath, L"\\doxygen\\bin");
		wsprintf(szProgName, L"%s\\doxygen.exe", szInstallPath);
		if (GetFileAttributes(szProgName) == -1) {

			// Non trouvé automatiquement => recherche manuelle du programme
			if (!SelectInstallPath(szInstallPath)) return FALSE;

			// Si le programme a été sélectionné au lieu du répertoire
			Len = lstrlen(szInstallPath);
			if (Len > 12 && !lstrcmpi(&szInstallPath[Len - 12], L"\\doxygen.exe")) {
				szInstallPath[Len - 12] = L'\0';
			}

		}

	}

	if (SetInstallPath(szInstallPath)) return TRUE;

	// Message d'erreur installation
	CallbackError(Translate(L"Impossible d'écrire dans la base de registre").c_str(),
								Translate(L"Erreur").c_str(),
								MB_OK | MB_ICONSTOP);

	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap20(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_DOXYGEN_20", IMAGE_BITMAP, 20, 20, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_DOXYGEN_32", IMAGE_BITMAP, 32, 32, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_DOXYGEN_48", IMAGE_BITMAP, 48, 48, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *wszDescription, int SizeDescription) {
	lstrcpynW(wszDescription, L"Génération automatique de documentation DOXYGEN", SizeDescription);
	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {

	szProjectContent[0] = L'\0';
	szPtrProjectFileName = szProjectFileName; 
	if (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE) {
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Open(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {

	szProjectContent[0] = L'\0';
	szPtrProjectFileName = szProjectFileName; 
	if (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE) {
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetInputs(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szInputFileNames) {
	std::wstring sInputFileNames;
	std::wstring sOutputFileName;
	int i, Len;


	if (!ParseFileDoxygen(szProjectFileName, sInputFileNames, sOutputFileName)) return FALSE;

	Len = (int) sInputFileNames.length();
	szPtrInputFileNames = new wchar_t[Len + 1];
	for (i = 0; i < Len; i++) {
		if (sInputFileNames[i] == L'|') szPtrInputFileNames[i] = L'\0';
		else szPtrInputFileNames[i] = sInputFileNames[i];
	}
	szPtrInputFileNames[Len] = L'\0';

	*szInputFileNames = szPtrInputFileNames;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetInputs(const wchar_t *szInputFileNames) {

	delete[] szPtrInputFileNames;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetOutput(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szOutputFileName) {
	std::wstring sInputFileNames;
	std::wstring sOutputFileName;
	int Len;


	if (!ParseFileDoxygen(szProjectFileName, sInputFileNames, sOutputFileName)) return FALSE;

	Len = (int) sInputFileNames.length();
	szPtrOutputFileName = new wchar_t[Len];
	lstrcpy(szPtrOutputFileName, sOutputFileName.c_str());

	*szOutputFileName = szPtrOutputFileName;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetOutput(const wchar_t *szOutputFileName) {

	delete[] szPtrOutputFileName;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Build(const wchar_t *szProjectFileName, const wchar_t *szProjectContent) {
	wchar_t szInstallPath[MAX_PATH];
	wchar_t szProgName[MAX_PATH];
	wchar_t szArgs[MAX_PATH];
	wchar_t szMessage[MAX_PATH * 2 + 100];
	BOOL Ok;


	wsprintf(szMessage, L"Compilation \"%s\"", szProjectFileName);
	CallbackError(szMessage, L"Information", MB_OK | MB_ICONINFORMATION);

	if (GetInstallPath(szInstallPath, MAX_PATH)) {

		wsprintf(szProgName, L"%s\\Doxygen.exe", szInstallPath);
		wsprintf(szArgs, L"\"%s\"", szProjectFileName);
		Ok = CreateProcessAndRedirectOut(szProgName, szArgs);

	}
	else {
		// Message d'erreur InstallPath non trouvé
		CallbackError(Translate(L"Programme \"Doxygen\" pas ou mal installé").c_str(),
									Translate(L"Erreur").c_str(),
									MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return Ok;
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

    case BUTTON_PARCOURIRPARAM:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirParamClick();
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
	SetDlgItemText(hWndSaisieParam, EDIT_PARAM, szPtrProjectFileName);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirParamClick(void) {
	OPENFILENAME ofn;
	wchar_t szSourceFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_PARAM, szSourceFileName, sizeof(szSourceFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Fichiers doxygen (*.dox)\0*.dox\0Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szSourceFileName;
  ofn.lpstrTitle = L"Fichier de paramètres";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_PARAM, szSourceFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_PARAM));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_OkClick(void) {
	GetDlgItemText(hWndSaisieParam, EDIT_PARAM, szPtrProjectFileName, MAX_PATH);

  EndDialog(hWndSaisieParam, TRUE);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL GetInstallPath(wchar_t * szInstallPath, DWORD cbMaxInstallPath) {
	HKEY hKey;
  LONG Error;
  DWORD dwSize;
  DWORD dwType;
	BOOL Ok = FALSE;
	wchar_t szProgName[MAX_PATH];


	Error = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Denis ROBERT\\FVCLIDE\\Plugins\\Doxygen", 0,
                       KEY_READ, &hKey);

  // Lecture de la clé
  if (Error == ERROR_SUCCESS) {

		dwSize = cbMaxInstallPath;
		Error = RegQueryValueEx(hKey, L"InstallPath", 0, &dwType, (LPBYTE) szInstallPath, &dwSize);
		if (!Error && dwSize > 0) Ok = TRUE;

    RegCloseKey(hKey);
	}

	// Vérification de l'existence du programme DOXYGEN
	if (Ok) {
		wsprintf(szProgName, L"%s\\Doxygen.exe", szInstallPath);
		if (GetFileAttributes(szProgName) == -1) Ok = FALSE;
	}

	return Ok;
}

//---------------------------------------------------------------------------
BOOL SetInstallPath(wchar_t * szInstallPath) {
	HKEY hKey;
  LONG Error;
  DWORD dwSize;
  DWORD dwDisposition;
	BOOL Ok = FALSE;

  Error = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Denis ROBERT\\FVCLIDE\\Plugins\\Doxygen", 0, L"",
                          REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hKey,
                          &dwDisposition);

  // Ecriture dans la clé
  if (Error == ERROR_SUCCESS) {

    dwSize = lstrlenW(szInstallPath) * sizeof(TCHAR);
    Error = RegSetValueEx(hKey, L"InstallPath", 0, REG_SZ, (LPBYTE) szInstallPath, dwSize);
		if (!Error && dwSize > 0) Ok = TRUE;

    RegCloseKey(hKey);
	}

	return Ok;
}

//---------------------------------------------------------------------------
BOOL SelectInstallPath(wchar_t *szPath) {
  BROWSEINFO BrowseInfo;
  BOOL Ok = FALSE;


  BrowseInfo.hwndOwner = hWndSaisieParam;
  BrowseInfo.pidlRoot = NULL;
  lstrcpyn(szPath, L"", MAX_PATH);
  BrowseInfo.pszDisplayName = szPath;
  BrowseInfo.lpszTitle = L"Sélectionnez le chemin du programme doxygen.exe";
  BrowseInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_NONEWFOLDERBUTTON | BIF_BROWSEINCLUDEFILES;
  BrowseInfo.lpfn = NULL;
  BrowseInfo.lParam = 0;
  BrowseInfo.iImage = 0;
	BrowseInfo.lpfn = SelectDirectoryCallbackProc;

  LPITEMIDLIST lpItemIDList = SHBrowseForFolder(&BrowseInfo);

  if (lpItemIDList != NULL) {
    SHGetPathFromIDList(lpItemIDList, szPath);
    IMalloc *Malloc;
    HRESULT hr = SHGetMalloc(&Malloc);
    if (hr == S_OK) Malloc->Free(lpItemIDList);
		Ok = TRUE;
  }

  return Ok;
}

//---------------------------------------------------------------------------
int CALLBACK SelectDirectoryCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
	if (uMsg == BFFM_INITIALIZED) {
		wchar_t szProgramFiles[MAX_PATH];
		if (!GetEnvironmentVariable(L"ProgramFiles(x86)", szProgramFiles, MAX_PATH)) {
			GetEnvironmentVariable(L"ProgramFiles", szProgramFiles, MAX_PATH);
		}
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM) szProgramFiles);
		SendMessage(hWnd, BFFM_SETEXPANDED, TRUE, (LPARAM) szProgramFiles);
	}

	return 0;
}

//---------------------------------------------------------------------------
BOOL ParseFileDoxygen(std::wstring sFileName, std::wstring &sInputFileNames, std::wstring &sOutputFileName) {
	HANDLE hFile;
	std::string sBuffer;
	std::string sKeyWord;
	std::string sValue;
	std::wstring sOutputDirectory;
	std::wstring sHtmlOutput;
	int PosEqual, Len;


	sInputFileNames = L"";
	sOutputFileName = L"";
	sHtmlOutput = L"html";

  hFile = CreateFile(sFileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		while (FileLineInput(hFile, &sBuffer)) {

			if (!sBuffer.empty() && sBuffer[0] != '#') {

				Len = (int) sBuffer.length();

				PosEqual = (int) sBuffer.find_first_of("=");
				if (PosEqual != -1) {

					sKeyWord = SuppSpace(sBuffer.substr(0, PosEqual));
					sValue = SuppSpace(sBuffer.substr(PosEqual + 1, Len - PosEqual - 1));

					if (sKeyWord.compare("OUTPUT_DIRECTORY") == 0) {
						if (!sValue.empty()) {
							sOutputDirectory = Unicode(sValue);
						}
					}
					else if (sKeyWord.compare("INPUT") == 0) {
						if (!sValue.empty()) {
							sInputFileNames += Unicode(sValue);
							sInputFileNames += L"/|";
						}
					}
					else if (sKeyWord.compare("IMAGE_PATH") == 0) {
						if (!sValue.empty()) {
							sInputFileNames += Unicode(sValue);
							sInputFileNames += L"/|";
						}
					}
					else if (sKeyWord.compare("HTML_OUTPUT") == 0) {
						if (!sValue.empty()) {
							sHtmlOutput = Unicode(sValue);
						}
					}
					else if (sKeyWord.compare("HTML_HEADER") == 0) {
						if (!sValue.empty()) {
							sInputFileNames += Unicode(sValue);
							sInputFileNames += L"|";
						}
					}
					else if (sKeyWord.compare("HTML_FOOTER") == 0) {
						if (!sValue.empty()) {
							sInputFileNames += Unicode(sValue);
							sInputFileNames += L"|";
						}
					}

				}

			}

			if (!sOutputDirectory.empty() && !sHtmlOutput.empty()) {
				sOutputFileName = sOutputDirectory;
				sOutputFileName += L"/";
				sOutputFileName += sHtmlOutput;
				sOutputFileName += L"/index.html";
			}

		}

    CloseHandle(hFile);

    return TRUE;
  }
  else {
    return FALSE;
  }

}
