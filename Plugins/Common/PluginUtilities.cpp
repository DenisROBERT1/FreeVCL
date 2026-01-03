//-----------------------------------------------------------------------------
//! @file PluginUtilities.cpp
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

#include "../Include/Plugin.h"
#include "../Include/PluginUtilities.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
extern CALLBACK_ERROR CallbackError;

//---------------------------------------------------------------------------
BOOL CreateProcessAndRedirectOut(const wchar_t *szProgName, const wchar_t *szArgs) {
	wchar_t szCommand[MAX_PATH * 2 + 7];
	wchar_t szMessage[MAX_PATH * 2 + 100];
	wchar_t szErrMsg[35 + MAX_PATH];
	wchar_t szBuf[128];
	std::wstring sErrFmt;
	std::wstring sMessageFmt;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD ReturnCode;
	BOOL Ok = FALSE;


	// Informations
	wsprintf(szCommand, L"\"%s\" %s", szProgName, szArgs);
	wsprintf(szMessage, Translate(L"Exécution du programme : %s").c_str(), szCommand);
	CallbackError(szMessage, Translate(L"Information").c_str(), MB_OK | MB_ICONINFORMATION);

	HANDLE hRdOut = NULL;
	HANDLE hWrOut = NULL;
	HANDLE hWrOutDup = NULL;

	// Creation du pipe pour la sortie
	if (!CreatePipe(&hRdOut, &hWrOut, NULL, 0)) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		sErrFmt = Translate(L"Impossible de créer le pipe de sortie : %s");
		wsprintf(szErrMsg, sErrFmt.c_str(), szBuf);
		CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
		// On continue quand même (la sortie standard sera utilisée)
	}

	// Duplication du handle de sortie (pour avoir un handle hérité)
	HANDLE hProcess = GetCurrentProcess();
	if (!DuplicateHandle(hProcess, hWrOut, hProcess, &hWrOutDup, NULL, TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE)) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		sErrFmt = Translate(L"Impossible de dupliquer le handle du pipe : %s");
		wsprintf(szErrMsg, sErrFmt.c_str(), szBuf);
		CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
	}

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = hWrOutDup;
	si.hStdError = hWrOutDup;

	// Start the child process. 
	if (CreateProcess(
			szProgName,     // Module name
			szCommand,      // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			TRUE,          // Set handle inheritance to TRUE
			CREATE_NO_WINDOW, // No window creation
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)            // Pointer to PROCESS_INFORMATION structure
			) {


		CallbackError(Translate(L"Résultat du traitement :").c_str(), Translate(L"Information").c_str(), MB_OK | MB_ICONINFORMATION);
		
		char c;
		DWORD dwBytesRead, dwBytesAvailable;
		std::string sBuffer;

		// Boucle infinie dans laquelle on attend soit la fin du process, soit des données du pipe
		while (true) {

			// Test de la fin du process
			if (WaitForSingleObject(pi.hProcess, 100) != WAIT_TIMEOUT) break;

			// Lecture du pipe de sortie
			dwBytesAvailable = 0;
			PeekNamedPipe(hRdOut, NULL, NULL, NULL, &dwBytesAvailable, NULL);
			while (dwBytesAvailable) {
				if (ReadFile(hRdOut, &c, 1, &dwBytesRead, NULL) == FALSE) break;
				if (dwBytesRead == 1) {
					if (c == '\n') {
						CallbackError(Unicode(sBuffer).c_str(), Translate(L"Information").c_str(), MB_OK | MB_ICONINFORMATION);
						sBuffer = "";
					}
					else if (c != '\r') sBuffer += c;
					dwBytesAvailable--;
				}
			}

		}

		// Fermeture du handle d'écriture du pipe (sinon, le ReadFile reste bloqué)
		CloseHandle(hWrOutDup);

		// Affichage du résultat au travers du pipe (données restantes dans le buffer)
		while (ReadFile(hRdOut, &c, 1, &dwBytesRead, NULL) == TRUE && dwBytesRead == 1) {
			if (c == '\n') {
				CallbackError(Unicode(sBuffer).c_str(), Translate(L"Information").c_str(), MB_OK | MB_ICONINFORMATION);
				sBuffer = "";
			}
			else if (c != '\r') sBuffer += c;
		}
		if (!sBuffer.empty()) {
			// Dernière ligne sans '\n' ?
			CallbackError(Unicode(sBuffer).c_str(), Translate(L"Information").c_str(), MB_OK | MB_ICONINFORMATION);
		}

		// Valeur de retour
		if (GetExitCodeProcess(pi.hProcess, &ReturnCode)) {
			if (ReturnCode == 0) {
				// Retour Ok
				Ok = TRUE;
			}
			else {
				wsprintf(szErrMsg, Translate(L"Le processus a retourné le code d'erreur %i").c_str(), ReturnCode);
				CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
			}
		}


		// Fermeture des handles 
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		CloseHandle(hRdOut);

	}
	else {
		// Message d'erreur exécution du programme
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		sErrFmt = Translate(L"Erreur d'exécution du programme \"%s\": %s");
		wsprintf(szErrMsg, sErrFmt.c_str(), szProgName, szBuf);
		CallbackError(szErrMsg, L"Erreur", MB_OK | MB_ICONWARNING);
	}

	return Ok;
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
std::wstring Unicode(std::string sStr) {
  std::wstring wsRet;
  int Len;
	int i;


  Len = (int) sStr.length();
	for (i = 0; i < Len; i++) {
		wsRet += (wchar_t) (BYTE) sStr[i];
	}

  return wsRet;
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
std::string SuppSpace(std::string sStr) {
	int Deb, Fin;

	Deb = (int) sStr.find_first_not_of(' ');
	Fin = (int) sStr.find_last_not_of(' ');
	if (Deb < 0 || Fin < 0) return "";
	return sStr.substr(Deb, Fin - Deb + 1);
}

//---------------------------------------------------------------------------
std::string SuppQuotes(std::string sStr) {
	int Deb, Fin;

	Deb = 0;
	Fin = (int) sStr.length() - 1;
	if (Fin < 1) return sStr;

	if (sStr[0] == '"' && sStr[Fin] == '"') {
		Deb++;
		Fin--;
	}

	return sStr.substr(Deb, Fin - Deb + 1);
}

//---------------------------------------------------------------------------
std::wstring Translate(std::wstring sTexte) {
	return sTexte;
}
