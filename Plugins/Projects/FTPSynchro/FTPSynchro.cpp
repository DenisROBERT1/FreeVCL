//-----------------------------------------------------------------------------
//! @file FTPSynchro.cpp
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
#include <string>
#include <vector>
#include <shlobj.h>
#include <Shlwapi.h>
#include <wininet.h>

#include "../../Include/Plugin.h"
#include "FTPSynchro.h"
#include "Resource.h"

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

typedef struct {
	std::wstring asFileName;
  FILETIME ftCreationTime;
  FILETIME ftLastWriteTime;
  DWORD nFileSizeHigh;
  DWORD nFileSizeLow;
} FTPFILE;


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
HINSTANCE HInstance;
HWND hWndSaisieParam;

int Direction;
wchar_t szLocalPath[MAX_PATH];
wchar_t szRemotePath[MAX_PATH];
wchar_t szSiteFTP[MAX_PATH];
wchar_t szIdent[MAX_PATH];
wchar_t szPassword[MAX_PATH];
int Port;
int Action;

std::vector<FTPFILE> FTPFiles;
CALLBACK_ERROR CallbackError;

//---------------------------------------------------------------------------
// Prototype des fonctions
//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectContent);
BOOL SaveProject(wchar_t *szProjectContent);
BOOL Touch(const wchar_t *szFileName);

BOOL CALLBACK SaisieParamProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SaisieParamInitDialog(void);
BOOL Button_OkClick(void);
BOOL Button_ParcourirLocalClick(void);
void CenterWindow(HWND hWnd);

bool FTPSynchro(void);
bool LocalToFTP(HINTERNET hFTPConn, std::wstring asLocalPath, std::wstring asRemotePath);
bool FTPToLocal(HINTERNET hFTPConn, std::wstring asLocalPath, std::wstring asRemotePath);
BOOL FtpGetFileAttributesEx(const wchar_t *szFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation);
bool DirFTP(HINTERNET hFTPConn, std::wstring asRemotePath);
BOOL FtpPutFile_n_essais(HINTERNET hConnect, LPCWSTR lpszLocalFile, LPCWSTR lpszNewRemoteFile, DWORD dwFlags, DWORD_PTR dwContext, int NbEssais);
void AddFileTime(FILETIME *ft, int ms);


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
	return (HBITMAP) LoadImage(HInstance, L"BMP_FTPSYNCHRO_20", IMAGE_BITMAP, 20, 20, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_FTPSYNCHRO_32", IMAGE_BITMAP, 32, 32, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_FTPSYNCHRO_48", IMAGE_BITMAP, 48, 48, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *wszDescription, int SizeDescription) {

	lstrcpynW(wszDescription, L"Synchronisation FTP", SizeDescription);

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	Direction = 0;
	szLocalPath[0] = L'\0';
	szRemotePath[0] = L'\0';
	szSiteFTP[0] = L'\0';
	szIdent[0] = L'\0';
	szPassword[0] = L'\0';
	Port = 21;
	Action = 0;

	Ok = (DialogBox(HInstance, MAKEINTRESOURCE(FORM_SAISIEPARAMS), hWnd, (DLGPROC) SaisieParamProc) == TRUE);

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
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Open(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	if (szProjectContent[0]) {
		Ok = LoadProject(szProjectContent);
	}
	else {
		Direction = 0;
		szLocalPath[0] = L'\0';
		szRemotePath[0] = L'\0';
		szSiteFTP[0] = L'\0';
		szIdent[0] = L'\0';
		szPassword[0] = L'\0';
		Port = 21;
		Action = 0;
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
	*szInputFileNames = L"\0";
	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetInputs(const wchar_t *szInputFileNames) {
	// Rien à faire (le buffer est statique)

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetOutput(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szOutputFileName) {
	*szOutputFileName = L"\0";
	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetOutput(const wchar_t *szOutputFileName) {
	// Rien à faire (le buffer est statique)

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Build(const wchar_t *szProjectFileName, const wchar_t *szProjectContent) {
	wchar_t szMessage[MAX_PATH * 2 + 22];  // 22 = sizeof("Synchronisation") + 2 espaces + 4 guillemets + zéro terminal

	if (LoadProject(szProjectContent)) {

		if (Direction == 0) {
			wsprintf(szMessage, L"Synchronisation \"%s\" vers \"%s\"", szLocalPath, szRemotePath);
		}
		else if (Direction == 1) {
			wsprintf(szMessage, L"Synchronisation \"%s\" vers \"%s\"", szRemotePath, szLocalPath);
		}
		else {
			wsprintf(szMessage, L"Synchronisation bi-directionnelle \"%s\" - \"%s\"", szLocalPath, szRemotePath);
		}
		CallbackError(szMessage, L"Information", MB_OK | MB_ICONINFORMATION);

		if (FTPSynchro()) return TRUE;

		return FALSE;
	}
	else {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];
		int LastError;

    // Message d'erreur ouverture szProjectFileName
		LastError = GetLastError();
		memset(szBuf, 0, sizeof(szBuf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError, 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\" : erreur %i - %s", szProjectFileName, LastError, szBuf);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

}

//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectContent) {
  int Deb, Fin;
	int i;
	wchar_t c;
	int NbValues;
	wchar_t * szValues[8];
	int MaxLng[8];
	wchar_t szPort[10];
	wchar_t szAction[10];
	wchar_t szDirection[10];


	i = -1;
	Deb = 0;
	NbValues = 0;
	szValues[0] = szLocalPath;
	MaxLng[0] = MAX_PATH;
	szValues[1] = szRemotePath;
	MaxLng[1] = MAX_PATH;
	szValues[2] = szSiteFTP;
	MaxLng[2] = MAX_PATH;
	szValues[3] = szIdent;
	MaxLng[3] = MAX_PATH;
	szValues[4] = szPassword;
	MaxLng[4] = MAX_PATH;
	szValues[5] = szPort;
	MaxLng[5] = 10;
	szValues[6] = szAction;
	MaxLng[6] = 10;
	szValues[7] = szDirection;
	MaxLng[7] = 10;

	do {
		c = szProjectContent[++i];
		if (c == L'|' || c == L'\0') {
			if (NbValues < 8) {
				Fin = i;
				if (Fin > Deb + MaxLng[NbValues] + 1) Fin = Deb + MaxLng[NbValues] + 1;
				lstrcpyn(szValues[NbValues], &szProjectContent[Deb], Fin - Deb + 1);
				Deb = i + 1;
				NbValues++;
			}
		}
	} while (c);

	Port = _wtoi(szPort);
	if (Port == 0) Port = 21;

	Action = _wtoi(szAction);

	Direction = _wtoi(szDirection);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL SaveProject(wchar_t *szProjectContent) {

	if (lstrlen(szLocalPath) + lstrlen(szRemotePath) + lstrlen(szSiteFTP) +
		  lstrlen(szIdent) + lstrlen(szPassword) + 5 + 10 + 1 > 2048) {
	  CallbackError(L"Dépassement de la longueur maximum de fichiers (2048)", L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	wsprintfW(szProjectContent, L"%s|%s|%s|%s|%s|%i|%i|%i",
						szLocalPath, szRemotePath, szSiteFTP, szIdent, szPassword, Port, Action, Direction);
	
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
		int LastError;

    // Message d'erreur ouverture szFileName
		LastError = GetLastError();
		memset(szBuf, 0, sizeof(szBuf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError, 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\" : erreur %i - %s", szFileName, LastError, szBuf);
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

    case BUTTON_PARCOURIRLOCAL:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirLocalClick();
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
	SendDlgItemMessage(hWndSaisieParam, COMBOBOX_SENS, CB_ADDSTRING, 0, (LPARAM) (LPWSTR) L"Local vers FTP");
	SendDlgItemMessage(hWndSaisieParam, COMBOBOX_SENS, CB_ADDSTRING, 0, (LPARAM) (LPWSTR) L"FTP vers local");
	SendDlgItemMessage(hWndSaisieParam, COMBOBOX_SENS, CB_ADDSTRING, 0, (LPARAM) (LPWSTR) L"Dans les deux sens");
	SendDlgItemMessage(hWndSaisieParam, COMBOBOX_SENS, CB_SETCURSEL, Direction, 0);
	SetDlgItemText(hWndSaisieParam, EDIT_LOCAL, szLocalPath);
	SetDlgItemText(hWndSaisieParam, EDIT_REMOTE, szRemotePath);
	SetDlgItemText(hWndSaisieParam, EDIT_FTP, szSiteFTP);
	SetDlgItemText(hWndSaisieParam, EDIT_IDENT, szIdent);
	SetDlgItemText(hWndSaisieParam, EDIT_PASSWORD, szPassword);
	SetDlgItemInt(hWndSaisieParam, EDIT_PORT, Port, FALSE);
	CheckRadioButton(hWndSaisieParam, RADIO_NOTHING, RADIO_DELETE, RADIO_NOTHING + Action);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirLocalClick(void) {
  BROWSEINFO BrowseInfo;
  wchar_t szPath[MAX_PATH];
  BOOL Ok = FALSE;


  BrowseInfo.hwndOwner = hWndSaisieParam;
  BrowseInfo.pidlRoot = NULL;
  lstrcpyn(szPath, L"", MAX_PATH);
  BrowseInfo.pszDisplayName = szPath;
  BrowseInfo.lpszTitle = L"Sélectionnez le répertoire à synchroniser";
  BrowseInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_NONEWFOLDERBUTTON;
  BrowseInfo.lpfn = NULL;
  BrowseInfo.lParam = 0;
  BrowseInfo.iImage = 0;

  LPITEMIDLIST lpItemIDList = SHBrowseForFolder(&BrowseInfo);

  if (lpItemIDList != NULL) {
    SHGetPathFromIDList(lpItemIDList, szPath);
    IMalloc *Malloc;
    HRESULT hr = SHGetMalloc(&Malloc);
    if (hr == S_OK) Malloc->Free(lpItemIDList);

		SetDlgItemText(hWndSaisieParam, EDIT_LOCAL, szPath);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_LOCAL));

		Ok = TRUE;
  }

  return Ok;
}

//---------------------------------------------------------------------------
BOOL Button_OkClick(void) {
	GetDlgItemText(hWndSaisieParam, EDIT_LOCAL, szLocalPath, sizeof(szLocalPath) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_REMOTE, szRemotePath, sizeof(szRemotePath) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_FTP, szSiteFTP, sizeof(szSiteFTP) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_IDENT, szIdent, sizeof(szIdent) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_PASSWORD, szPassword, sizeof(szPassword) / sizeof(wchar_t));
	Port = GetDlgItemInt(hWndSaisieParam, EDIT_PORT, NULL, FALSE);

	if (IsDlgButtonChecked(hWndSaisieParam, RADIO_COPY)) Action = 1;
	else if (IsDlgButtonChecked(hWndSaisieParam, RADIO_DELETE)) Action = 2;
	else Action = 0;

	Direction = (int) SendDlgItemMessage(hWndSaisieParam, COMBOBOX_SENS, CB_GETCURSEL, 0, 0);
	if (Direction == -1) {
		MessageBox(hWndSaisieParam, L"Veuillez choisir un sens de synchronisation", L"Erreur de saisie", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

  EndDialog(hWndSaisieParam, TRUE);

	return TRUE;
}

//---------------------------------------------------------------------------
bool FTPSynchro(void) {
	HINTERNET hNetConn;
	HINTERNET hFTPConn;
	int LastError;
	DWORD LastErrorFTP;
	DWORD dwBufferLength;
	wchar_t szBuf[128];
	wchar_t szBuf2[128];
	wchar_t szErrMsg[128 + 128 + 100 + MAX_PATH];


	hNetConn = InternetOpen(L"FTPSynchro", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hNetConn == NULL) {
    // Message d'erreur
		LastError = GetLastError();
		memset(szBuf, 0, sizeof(szBuf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError, 0, szBuf, 128, NULL);
		dwBufferLength = sizeof(szBuf2) / sizeof(wchar_t);
		InternetGetLastResponseInfo(&LastErrorFTP, szBuf2, &dwBufferLength);
		wsprintf(szErrMsg, L"Impossible d'ouvrir la connexion internet : erreur %i - %s (%i - %s)", LastError, szBuf, LastErrorFTP, szBuf2);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONSTOP);
		return false;
	}

	hFTPConn = InternetConnect(hNetConn, szSiteFTP, Port, szIdent, szPassword, 1, 0, 0);
	if (hFTPConn == NULL) {
    // Message d'erreur
		LastError = GetLastError();
		memset(szBuf, 0, sizeof(szBuf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError, 0, szBuf, 128, NULL);
		dwBufferLength = sizeof(szBuf2) / sizeof(wchar_t);
		InternetGetLastResponseInfo(&LastErrorFTP, szBuf2, &dwBufferLength);
		wsprintf(szErrMsg, L"Impossible d'ouvrir la connexion FTP : erreur %i - %s (%i - %s)", LastError, szBuf, LastErrorFTP, szBuf2);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONSTOP);
		return false;
	}

	FTPFiles.clear();
	if (!DirFTP(hFTPConn, szRemotePath)) return false;
	if (Direction == 0 || Direction == 2) {
		if (!LocalToFTP(hFTPConn, szLocalPath, szRemotePath)) return false;
	}
	if (Direction == 1 || Direction == 2) {
		if (!FTPToLocal(hFTPConn, szLocalPath, szRemotePath)) return false;
	}

	InternetCloseHandle(hFTPConn);
	InternetCloseHandle(hNetConn);

	return true;
}

//---------------------------------------------------------------------------
bool LocalToFTP(HINTERNET hFTPConn, std::wstring asLocalPath, std::wstring asRemotePath) {
	HANDLE hFindFile;
	WIN32_FIND_DATA FindFileData;
	std::wstring asPath;
	bool Ok;


	asPath = asLocalPath + L"\\*.*";
	hFindFile = FindFirstFile(asPath.c_str(), &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE) {
		DWORD LastErrorFTP;
		DWORD dwBufferLength;
		wchar_t szBuf[128];
		wchar_t szBuf2[128];
		wchar_t szErrMsg[128 + 128 + 100 + MAX_PATH];
		int LastError;
		LastError = GetLastError();
		memset(szBuf, 0, sizeof(szBuf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError, 0, szBuf, 128, NULL);
		dwBufferLength = sizeof(szBuf2) / sizeof(wchar_t);
		InternetGetLastResponseInfo(&LastErrorFTP, szBuf2, &dwBufferLength);
		wsprintf(szErrMsg, L"Impossible de lister \"%s\" : erreur %i - %s (%i - %s)", asPath.c_str(), LastError, szBuf, LastErrorFTP, szBuf2);
	  CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONSTOP);
		return false;
	}

	Ok = true;
	do {
		std::wstring asLocalFileName = asLocalPath + L"\\" + FindFileData.cFileName;
		std::wstring asRemoteFileName = asRemotePath + L"/" + FindFileData.cFileName;
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			if (FindFileData.cFileName[0] != L'.') {
				// Traitement répertoire
				FtpCreateDirectory(hFTPConn, asRemoteFileName.c_str());
				if (!LocalToFTP(hFTPConn, asLocalFileName, asRemoteFileName)) {
					Ok = false;
					break;
				}
			}
		}
		else {
			// Traitement fichier
			WIN32_FILE_ATTRIBUTE_DATA RemoteFileAttributes;
			bool bMustCopy = false;
			if (FtpGetFileAttributesEx(asRemoteFileName.c_str(), GetFileExInfoStandard, &RemoteFileAttributes)) {
				// Apparemment, si les deux fichiers font zéro octets, la copie ne se fait pas (en tous
				// cas la date du fichier distant n'est pas changée, et le test sur la date ne marche pas).
				if (RemoteFileAttributes.nFileSizeHigh != 0 ||
						RemoteFileAttributes.nFileSizeLow != 0 ||
						FindFileData.nFileSizeHigh != 0 ||
						FindFileData.nFileSizeLow != 0) {
					// Ajout d'une seconde pour éviter les petites différences dues à des systèmes de fichiers différents
					AddFileTime(&RemoteFileAttributes.ftLastWriteTime, 1000);
					if (CompareFileTime(&FindFileData.ftLastWriteTime, &RemoteFileAttributes.ftLastWriteTime) > 0) {
						// Fichier local plus récent
						bMustCopy = true;
					}
				}
			}
			else {
				// Fichier absent du FTP
				bMustCopy = true;
			}
			if (bMustCopy) {
				wchar_t szBuf[21 + MAX_PATH * 2];
				wsprintf(szBuf, L"Copie fichier : %s -> %s", asLocalFileName.c_str(), asRemoteFileName.c_str());
				CallbackError(szBuf, L"Synchro FTP", MB_OK | MB_ICONINFORMATION);
				if (!FtpPutFile_n_essais(hFTPConn, asLocalFileName.c_str(), asRemoteFileName.c_str(), FTP_TRANSFER_TYPE_BINARY, NULL, 5)) { 
					DWORD LastErrorFTP;
					DWORD dwBufferLength;
					wchar_t szBuf[128];
					wchar_t szBuf2[128];
					wchar_t szErrMsg[128 + 128 + 100 + MAX_PATH];
					int LastError;
					LastError = GetLastError();
					memset(szBuf, 0, sizeof(szBuf));
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError, 0, szBuf, 128, NULL);
					dwBufferLength = sizeof(szBuf2) / sizeof(wchar_t);
					InternetGetLastResponseInfo(&LastErrorFTP, szBuf2, &dwBufferLength);
					wsprintf(szErrMsg, L"Impossible de copier \"%s\" : erreur %i - %s (%i - %s)", asLocalFileName.c_str(), LastError, szBuf, LastErrorFTP, szBuf2);
					CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONSTOP);
					Ok = false;
					break;
				}
			}
		}

	} while (FindNextFile(hFindFile, &FindFileData));

	FindClose(hFindFile);

	return Ok;
}


//---------------------------------------------------------------------------
bool FTPToLocal(HINTERNET hFTPConn, std::wstring asLocalPath, std::wstring asRemotePath) {
	std::wstring asRemoteFileName;
	std::wstring asLocalFileName;
	WIN32_FILE_ATTRIBUTE_DATA FileAttributes;
	int LenRemotePath;
	wchar_t c;
	int i, j;


	LenRemotePath = (int) asRemotePath.length();
	for (i = 0; i < (int) FTPFiles.size(); i++) {
		asRemoteFileName = FTPFiles[i].asFileName;
		asLocalFileName = asLocalPath;
		for (j = LenRemotePath; j < (int) asRemoteFileName.length(); j++) {
			c = asRemoteFileName[j];
			if (c != L'/') asLocalFileName += c;
			else asLocalFileName += L'\\';
		}
		if (!GetFileAttributesEx(asLocalFileName.c_str(), GetFileExInfoStandard, &FileAttributes)) {

			if (Action == 1) {
				// Option copie des fichiers en local
				wchar_t szBuf[21 + MAX_PATH * 2];
				wsprintf(szBuf, L"Copie fichier : %s -> %s", asRemoteFileName.c_str(), asLocalFileName.c_str());
				CallbackError(szBuf, L"Synchro FTP", MB_OK | MB_ICONINFORMATION);
				if (FtpGetFile(hFTPConn, asRemoteFileName.c_str(), asLocalFileName.c_str(), true, NULL, FTP_TRANSFER_TYPE_BINARY, NULL)) {
					HANDLE hFile;
				  hFile = CreateFile(asLocalFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile != INVALID_HANDLE_VALUE) {
						SYSTEMTIME stNow;
						FILETIME ftNow;
						GetLocalTime(&stNow);
						SystemTimeToFileTime(&stNow, &ftNow);
						SetFileTime(hFile, &FTPFiles[i].ftCreationTime, &FTPFiles[i].ftLastWriteTime, &ftNow);
						CloseHandle(hFile);
					}
				}
				else {
					DWORD LastErrorFTP;
					DWORD dwBufferLength;
					wchar_t szBuf[128];
					wchar_t szBuf2[128];
					wchar_t szErrMsg[128 + 128 + 100 + MAX_PATH];
					int LastError;
					LastError = GetLastError();
					memset(szBuf, 0, sizeof(szBuf));
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError, 0, szBuf, 128, NULL);
					dwBufferLength = sizeof(szBuf2) / sizeof(wchar_t);
					InternetGetLastResponseInfo(&LastErrorFTP, szBuf2, &dwBufferLength);
					wsprintf(szErrMsg, L"Impossible de copier \"%s\" : erreur %i - %s (%i - %s)", asRemoteFileName.c_str(), LastError, szBuf, LastErrorFTP, szBuf2);
					CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONSTOP);
					return false;
				}
			}

			else if (Action == 2) {
				// Option suppression des fichiers
				wchar_t szBuf[23 + MAX_PATH];
				wsprintf(szBuf, L"Suppression fichier : %s", asRemoteFileName.c_str());
				CallbackError(szBuf, L"Synchro FTP", MB_OK | MB_ICONINFORMATION);
				FtpDeleteFile(hFTPConn, asRemoteFileName.c_str());
			}

		}
	}

	return true;
}

//---------------------------------------------------------------------------
BOOL FtpGetFileAttributesEx(const wchar_t *szFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation) {
	int i;


	if (fInfoLevelId == GetFileExInfoStandard) {

		LPWIN32_FILE_ATTRIBUTE_DATA lpFileAttributesData = (LPWIN32_FILE_ATTRIBUTE_DATA) lpFileInformation;

		for (i = 0; i < (int) FTPFiles.size(); i++) {
			if (FTPFiles[i].asFileName == szFileName) {
				memset(lpFileAttributesData, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
				lpFileAttributesData->ftCreationTime = FTPFiles[i].ftCreationTime;
				lpFileAttributesData->ftLastWriteTime = FTPFiles[i].ftLastWriteTime;
				lpFileAttributesData->nFileSizeHigh = FTPFiles[i].nFileSizeHigh;
				lpFileAttributesData->nFileSizeLow = FTPFiles[i].nFileSizeLow;
				return TRUE;
			}
		}

	}
	else {
		CallbackError(L"Incorrect GET_FILEEX_INFO_LEVELS", L"Synchro FTP", MB_OK | MB_ICONSTOP);
	}

	return FALSE;
}

//---------------------------------------------------------------------------
bool DirFTP(HINTERNET hFTPConn, std::wstring asRemotePath) {
	HINTERNET hFind;
	WIN32_FIND_DATA FindFileData;
	std::wstring asRemoteName;
	std::vector<std::wstring> ListRemotePath;
	int i;


	FtpSetCurrentDirectory(hFTPConn, asRemotePath.c_str());

	hFind = FtpFindFirstFile(hFTPConn, L"", &FindFileData, 0, 0);
	if (hFind != NULL) {
		do {
			asRemoteName = asRemotePath + L"/" + FindFileData.cFileName;
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (FindFileData.cFileName[0] != '.') {
					ListRemotePath.push_back(asRemoteName);
				}
			}
			else {
				FTPFILE FTPFile;
				FTPFile.asFileName = asRemoteName;
				LocalFileTimeToFileTime(&FindFileData.ftCreationTime, &FTPFile.ftCreationTime);
				LocalFileTimeToFileTime(&FindFileData.ftLastWriteTime, &FTPFile.ftLastWriteTime);
				FTPFile.nFileSizeHigh = FindFileData.nFileSizeHigh;
				FTPFile.nFileSizeLow = FindFileData.nFileSizeLow;
				FTPFiles.push_back(FTPFile);
			}
		} while (InternetFindNextFile(hFind, &FindFileData));
	}
	InternetCloseHandle(hFind);

	for (i = 0; i < (int) ListRemotePath.size(); i++) {
		DirFTP(hFTPConn, ListRemotePath[i]);
	}

	return true;
}

//---------------------------------------------------------------------------
BOOL FtpPutFile_n_essais(HINTERNET hConnect, LPCWSTR lpszLocalFile, LPCWSTR lpszNewRemoteFile, DWORD dwFlags, DWORD_PTR dwContext, int NbEssais) {
	int i;


	for (i = 0; i < NbEssais; i++) {
		if (FtpPutFile(hConnect, lpszLocalFile, lpszNewRemoteFile, dwFlags, dwContext) == TRUE) return TRUE;
		Sleep(1000);
	}

	return FALSE;
}

//---------------------------------------------------------------------------
void AddFileTime(FILETIME *ft, int ms) {
	ULARGE_INTEGER Tmp;
	Tmp.LowPart = ft->dwLowDateTime;
	Tmp.HighPart = ft->dwHighDateTime;
	Tmp.QuadPart += (LONGLONG) ms * 10000;
	ft->dwLowDateTime = Tmp.LowPart;
	ft->dwHighDateTime = Tmp.HighPart;
}
//---------------------------------------------------------------------------
