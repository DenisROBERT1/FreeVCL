//-----------------------------------------------------------------------------
//! @file Replace.cpp
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
#include "Replace.h"
#include "Resource.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
HINSTANCE HInstance;
HWND hWndSaisieParam;

wchar_t szFileSource[2 * MAX_PATH + 1]; // +1 pour avoir une liste de fichiers terminée par deux zéros
wchar_t szFileDest[MAX_PATH];
wchar_t szFileRepl[MAX_PATH];
wchar_t szSearch1[256];
wchar_t szReplace1[256];
wchar_t szSearch2[256];
wchar_t szReplace2[256];
wchar_t szSearch3[256];
wchar_t szReplace3[256];
wchar_t szSearch4[256];
wchar_t szReplace4[256];
wchar_t szSearch5[256];
wchar_t szReplace5[256];

CALLBACK_ERROR CallbackError;

//---------------------------------------------------------------------------
// Prototype des fonctions
//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectContent);
BOOL SaveProject(wchar_t *szProjectContent);
BOOL strcpytok(wchar_t *szToken, const wchar_t **lp);
BOOL Replace(void);
BOOL Replace(std::string *sBuffer, std::string sSearch, std::string sReplace);
bool FileLineInput(HANDLE hFile, std::string *sBuffer);
std::string Ansi(std::wstring sStr);

BOOL CALLBACK SaisieParamProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SaisieParamInitDialog(void);
BOOL Button_OkClick(void);
BOOL Button_ParcourirSourceClick(void);
BOOL Button_ParcourirDestClick(void);
BOOL Button_ParcourirFileReplClick(void);
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
	return (HBITMAP) LoadImage(HInstance, L"BMP_REPLACE_20", IMAGE_BITMAP, 20, 20, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_REPLACE_32", IMAGE_BITMAP, 32, 32, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void) {
	return (HBITMAP) LoadImage(HInstance, L"BMP_REPLACE_48", IMAGE_BITMAP, 48, 48, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *wszDescription, int SizeDescription) {

	lstrcpynW(wszDescription, L"Remplacement dans un fichier", SizeDescription);

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	BOOL Ok;


	szFileSource[0] = L'\0';
	szFileDest[0] = L'\0';
	szFileRepl[0] = L'\0';
	szSearch1[0] = L'\0';
	szReplace1[0] = L'\0';
	szSearch2[0] = L'\0';
	szReplace2[0] = L'\0';
	szSearch3[0] = L'\0';
	szReplace3[0] = L'\0';
	szSearch4[0] = L'\0';
	szReplace4[0] = L'\0';
	szSearch5[0] = L'\0';
	szReplace5[0] = L'\0';

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
		szFileRepl[0] = L'\0';
		szSearch1[0] = L'\0';
		szReplace1[0] = L'\0';
		szSearch2[0] = L'\0';
		szReplace2[0] = L'\0';
		szSearch3[0] = L'\0';
		szReplace3[0] = L'\0';
		szSearch4[0] = L'\0';
		szReplace4[0] = L'\0';
		szSearch5[0] = L'\0';
		szReplace5[0] = L'\0';
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
		if (szFileRepl[0]) {
			lstrcpy(&szFileSource[i], szFileRepl);
			i += lstrlen(szFileRepl) + 1;
		}
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
	wchar_t szMessage[MAX_PATH * 2 + 19];  // 19 = sizeof("Remplacement") + 2 espaces + 4 guillemets + zéro terminal

	if (LoadProject(szProjectContent)) {

		wsprintf(szMessage, L"Remplacement \"%s\" -> \"%s\"", szFileSource, szFileDest);
		CallbackError(szMessage, L"Information", MB_OK | MB_ICONINFORMATION);

		return Replace();

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
	strcpytok(szFileRepl, &lp);
	strcpytok(szSearch1, &lp);
	strcpytok(szReplace1, &lp);
	strcpytok(szSearch2, &lp);
	strcpytok(szReplace2, &lp);
	strcpytok(szSearch3, &lp);
	strcpytok(szReplace3, &lp);
	strcpytok(szSearch4, &lp);
	strcpytok(szReplace4, &lp);
	strcpytok(szSearch5, &lp);
	strcpytok(szReplace5, &lp);

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

	if (lstrlen(szFileSource) + lstrlen(szFileDest) + lstrlen(szFileRepl) +
			lstrlen(szSearch1) + lstrlen(szReplace1) +
			lstrlen(szSearch2) + lstrlen(szReplace2) +
			lstrlen(szSearch3) + lstrlen(szReplace3) +
			lstrlen(szSearch4) + lstrlen(szReplace4) +
			lstrlen(szSearch5) + lstrlen(szReplace5) +
			13 > 2048) {
	  CallbackError(L"Dépassement de la longueur maximum de fichiers (2048)", L"Erreur", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	wsprintfW(szProjectContent, L"%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s",
						szFileSource, szFileDest, szFileRepl,
						szSearch1, szReplace1,
						szSearch2, szReplace2,
						szSearch3, szReplace3,
						szSearch4, szReplace4,
						szSearch5, szReplace5);
	
	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Replace(void) {
  HANDLE hFile1, hFile2, hFile3;
	DWORD dw;
	std::string sBuffer;
	std::string sSearch1;
	std::string sReplace1;
	std::string sSearch2;
	std::string sReplace2;
	std::string sSearch3;
	std::string sReplace3;
	std::string sSearch4;
	std::string sReplace4;
	std::string sSearch5;
	std::string sReplace5;


	sSearch1 = Ansi(szSearch1);
	sReplace1 = Ansi(szReplace1);
	sSearch2 = Ansi(szSearch2);
	sReplace2 = Ansi(szReplace2);
	sSearch3 = Ansi(szSearch3);
	sReplace3 = Ansi(szReplace3);
	sSearch4 = Ansi(szSearch4);
	sReplace4 = Ansi(szReplace4);
	sSearch5 = Ansi(szSearch5);
	sReplace5 = Ansi(szReplace5);

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

  if (szFileRepl[0]) {
	
		hFile3 = CreateFile(szFileRepl, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile3 == INVALID_HANDLE_VALUE) {
			wchar_t szBuf[128];
			wchar_t szErrMsg[128 + 100 + MAX_PATH];

			// Message d'erreur ouverture szFileRepl
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
			wsprintf(szErrMsg, L"Impossible d'ouvrir le fichier \"%s\": %s", szFileRepl, szBuf);
			CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);

			CloseHandle(hFile1);
			CloseHandle(hFile2);

			return FALSE;
		}

		FileLineInput(hFile3, &sBuffer);
		Replace(&sReplace1, "[file]", sBuffer);
		Replace(&sReplace2, "[file]", sBuffer);
		Replace(&sReplace3, "[file]", sBuffer);
		Replace(&sReplace4, "[file]", sBuffer);
		Replace(&sReplace5, "[file]", sBuffer);

	}

	while (FileLineInput(hFile1, &sBuffer)) {
		if (!sSearch1.empty()) Replace(&sBuffer, sSearch1, sReplace1);
		if (!sSearch2.empty()) Replace(&sBuffer, sSearch2, sReplace2);
		if (!sSearch3.empty()) Replace(&sBuffer, sSearch3, sReplace3);
		if (!sSearch4.empty()) Replace(&sBuffer, sSearch4, sReplace4);
		if (!sSearch5.empty()) Replace(&sBuffer, sSearch5, sReplace5);
		WriteFile(hFile2, sBuffer.c_str(), (int) sBuffer.length(), &dw, NULL);
		WriteFile(hFile2, "\r\n", 2, &dw, NULL);
	}

	CloseHandle(hFile1);
  CloseHandle(hFile2);
  CloseHandle(hFile3);

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL Replace(std::string *sBuffer, std::string sSearch, std::string sReplace) {
	size_t PosPattern;


	PosPattern = 0;
	while ((PosPattern = sBuffer->find(sSearch, PosPattern)) != std::string::npos) {
		*sBuffer = sBuffer->replace(PosPattern, sSearch.length(), sReplace);
		PosPattern += sReplace.length();
	}

	return TRUE;
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

    case BUTTON_PARCOURIRFILEREPL:
      if (HIWORD(wParam) == BN_CLICKED) {
				Button_ParcourirFileReplClick();
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
	SetDlgItemText(hWndSaisieParam, EDIT_FILEREPL, szFileRepl);
	SetDlgItemText(hWndSaisieParam, EDIT_SRCH1, szSearch1);
	SetDlgItemText(hWndSaisieParam, EDIT_REPL1, szReplace1);
	SetDlgItemText(hWndSaisieParam, EDIT_SRCH2, szSearch2);
	SetDlgItemText(hWndSaisieParam, EDIT_REPL2, szReplace2);
	SetDlgItemText(hWndSaisieParam, EDIT_SRCH3, szSearch3);
	SetDlgItemText(hWndSaisieParam, EDIT_REPL3, szReplace3);
	SetDlgItemText(hWndSaisieParam, EDIT_SRCH4, szSearch4);
	SetDlgItemText(hWndSaisieParam, EDIT_REPL4, szReplace4);
	SetDlgItemText(hWndSaisieParam, EDIT_SRCH5, szSearch5);
	SetDlgItemText(hWndSaisieParam, EDIT_REPL5, szReplace5);

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
  ofn.lpstrFilter = L"Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.lpstrTitle = L"Fichier de départ";
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
  ofn.lpstrFilter = L"Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.lpstrTitle = L"Fichier destination";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_DEST, szFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_DEST));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_ParcourirFileReplClick(void) {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH];


	GetDlgItemText(hWndSaisieParam, EDIT_FILEREPL, szFileName, sizeof(szFileName) / sizeof(wchar_t));

	memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWndSaisieParam;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = L"Fichiers texte (*.txt)\0*.txt\0Tous (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.lpstrTitle = L"Fichier contenant le texte de remplacement";
  ofn.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {

		SetDlgItemText(hWndSaisieParam, EDIT_FILEREPL, szFileName);
		SetFocus(GetDlgItem(hWndSaisieParam, EDIT_FILEREPL));

	}

  return TRUE;
}

//---------------------------------------------------------------------------
BOOL Button_OkClick(void) {
	GetDlgItemText(hWndSaisieParam, EDIT_SOURCE, szFileSource, sizeof(szFileSource) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_DEST, szFileDest, sizeof(szFileDest) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_FILEREPL, szFileRepl, sizeof(szFileRepl) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_SRCH1, szSearch1, sizeof(szSearch1) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_REPL1, szReplace1, sizeof(szReplace1) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_SRCH2, szSearch2, sizeof(szSearch2) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_REPL2, szReplace2, sizeof(szReplace2) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_SRCH3, szSearch3, sizeof(szSearch3) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_REPL3, szReplace3, sizeof(szReplace3) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_SRCH4, szSearch4, sizeof(szSearch4) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_REPL4, szReplace4, sizeof(szReplace4) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_SRCH5, szSearch5, sizeof(szSearch5) / sizeof(wchar_t));
	GetDlgItemText(hWndSaisieParam, EDIT_REPL5, szReplace5, sizeof(szReplace5) / sizeof(wchar_t));

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
