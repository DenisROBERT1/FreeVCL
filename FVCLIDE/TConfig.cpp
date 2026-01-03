//-----------------------------------------------------------------------------
//! @file TConfig.cpp
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

#include "TConfig.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

TConfig * Config;

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TConfig::TConfig(void) {
  HKEY hKey;
  LONG Error;
  DWORD dwSize;
  DWORD dwType;
  DWORD dwValeur;


	// Valeurs par défaut
  FEndOfLine = eol_CRLF;
  FNbSpacesTab = 3;
  FSpacesToTabs = true;
  FTabsToSpaces = false;
  FTypeAuto = true;
  FIndentAuto = true;
  FCurAfterEndLine = true;
  FUndoRedoMoveCur = true;
  FLinesNumbers = true;
	FIconsColumn = true;
  FMultiLineTabs = true;
  FSaveBeforeBuild = false;
  FAutocompletion = false;
  FAlphaBlend = true;
  FEndLineAcc = true;
  FSpacesAfterPar = false;
	FLastWorkSpaces = new TStringList();
  FLastProjects = new TStringList();
  FLastFiles = new TStringList();
  FLastDialogs = new TStringList();
  FLastGraphics = new TStringList();
  FActivePageIndex = -1;
  FSearchStrings = new TStringList();
  FReplaceStrings = new TStringList();
  FEdiLeft = -1;
  FEdiTop = -1;
  FEdiWidth = -1;
  FEdiHeight = -1;
  FEdiFullScreen = false;
  FLeftPaneWidth = -1;
  FBottomPaneHeight = -1;
	FAlignGrid = false;
	FShowGrid = false;
	FGridX = 4;
	FGridY = 4;
	FWholeWord = false;
  FCaseSensitive = false;
  FBegLine = false;
  FEndLine = false;
  FOnlyInComments = false;
  FNotInComments = false;
  FOnlyInQuotes = false;
  FNotInQuotes = false;
  FRegExp = false;
  FKeepCase = false;
  FLoop = false;
	SearchIn = 0;
  FRecurse = false;

	// Couleurs par défaut
	static COLORPARAM DefaultColorParams[MAX_TYPESYNTAX + 1] = {
		{clWhite, clBlack, false, false},
		{RGB(10, 36, 106), clWhite, true, false},
		{clWhite, clBlue, true, false},
		{clWhite, clMaroon, true, false},
		{clWhite, clPurple, true, false},
		{clWhite, clNavy, true, false},
		{clWhite, clGreen, true, false},
		{clAqua, clGray, false, false},
		{clLtGray, clBlack, false, false},
		{clMoneyGreen, clBlack, false, false}
	};

	int i;
	for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
		FColorParams[i] = DefaultColorParams[i];
	}

  FFont = new TFont();
  FFont->Name = _T("Courier New");
	FFont->Charset = ANSI_CHARSET;
  FFont->Size = 8;
  FFont->Pitch = fpFixed;

  // Ouverture du registre LOCAL MACHINE
  Error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Denis ROBERT\\FVCLIDE"), 0,
                        KEY_READ, &hKey);

  // Lecture de la clé
  if (Error == ERROR_SUCCESS) {

    TCHAR szFileName[MAX_PATH];

		// -----------------------------------------------------------

		dwSize = MAX_PATH;
		Error = RegQueryValueEx(hKey, _T("InstallPath"), 0, &dwType, (LPBYTE) szFileName, &dwSize);
		if (!Error && dwSize > 0) FInstallPath = szFileName;

		if (FInstallPath == _T("")) {
			FInstallPath = Application->ExePath;
			if (FInstallPath.Length() > 4 && FInstallPath.SubString(FInstallPath.Length() - 3, 4) == _T("\\bin")) {
				FInstallPath = FInstallPath.SubString(1, FInstallPath.Length() - 4);
			}
		}

		RegCloseKey(hKey);

	}

  // Ouverture du registre CURRENT USER
  Error = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE"), 0,
                        KEY_READ, &hKey);

  // Lecture de la clé
  if (Error == ERROR_SUCCESS) {

    HKEY hSubKey;
    AnsiString asKey;
    TCHAR szFileName[MAX_PATH];
    int i;

		// ---------------------- MainWindow -------------------------

    Error = RegOpenKeyEx(hKey, _T("MainWindow"), 0, KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("Left"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEdiLeft = dwValeur;

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("Top"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEdiTop = dwValeur;

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("Width"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEdiWidth = dwValeur;

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("Height"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEdiHeight = dwValeur;

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("FullScreen"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEdiFullScreen = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("LeftPaneWidth"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FLeftPaneWidth = dwValeur;

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("BottomPaneHeight"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FBottomPaneHeight = dwValeur;

      dwSize = MAX_PATH;
      Error = RegQueryValueEx(hSubKey, _T("OpenWorkSpace"), 0, &dwType, (LPBYTE) szFileName, &dwSize);
      if (!Error && dwSize > 0) FOpenWorkSpace = szFileName;

      RegCloseKey(hSubKey);

    }

		// ---------------------- Files\LastFiles -------------------------

    Error = RegOpenKeyEx(hKey, _T("Files\\LastFiles"), 0,
                          KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

      for (i = 0; i < 10; i++) {

        asKey.sprintf(_T("LastWorkSpace%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szFileName, &dwSize);
        if (Error || dwSize <= 0) break;
        FLastWorkSpaces->Add(szFileName);

      }

      for (i = 0; i < 10; i++) {

        asKey.sprintf(_T("LastProject%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szFileName, &dwSize);
        if (Error || dwSize <= 0) break;
        FLastProjects->Add(szFileName);

      }

      for (i = 0; i < 10; i++) {

        asKey.sprintf(_T("LastFile%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szFileName, &dwSize);
        if (Error || dwSize <= 0) break;
        FLastFiles->Add(szFileName);

      }

      for (i = 0; i < 10; i++) {

        asKey.sprintf(_T("LastDialog%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szFileName, &dwSize);
        if (Error || dwSize <= 0) break;
        FLastDialogs->Add(szFileName);

      }

      for (i = 0; i < 10; i++) {

        asKey.sprintf(_T("LastGraphic%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szFileName, &dwSize);
        if (Error || dwSize <= 0) break;
        FLastGraphics->Add(szFileName);

      }

      RegCloseKey(hSubKey);

    }

		// ---------------------- Files -------------------------

    Error = RegOpenKeyEx(hKey, _T("Files"), 0,
                          KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("EndOfLine"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEndOfLine = (TEndOfLine) dwValeur;

      RegCloseKey(hSubKey);

    }

		// ---------------------- Editor -------------------------

    Error = RegOpenKeyEx(hKey, _T("Editor"), 0,
                          KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("NbSpacesTab"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FNbSpacesTab = (int) dwValeur;

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("SpacesToTabs"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FSpacesToTabs = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("TabsToSpaces"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FTabsToSpaces = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("TypeAuto"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FTypeAuto = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("IndentAuto"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FIndentAuto = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("CurAfterEndLine"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FCurAfterEndLine = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("UndoRedoMoveCur"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FUndoRedoMoveCur = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("LinesNumbers"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FLinesNumbers = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("IconsColumn"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FIconsColumn = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("MultiLineTabs"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FMultiLineTabs = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("SaveBeforeBuild"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FSaveBeforeBuild = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("Autocompletion"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FAutocompletion = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("AlphaBlend"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FAlphaBlend = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("EndLineAcc"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEndLineAcc = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("SpacesAfterPar"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FSpacesAfterPar = (dwValeur != 0);

      RegCloseKey(hSubKey);

    }

		// ---------------------- Editor\Colors -------------------------

    Error = RegOpenKeyEx(hKey, _T("Editor\\Colors"), 0,
                          KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

			for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {

				asKey.sprintf(_T("Background%i"), i);
				dwSize = sizeof(dwValeur);
				Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
				if (!Error && dwSize > 0) FColorParams[i].Background = (int) dwValeur;

				asKey.sprintf(_T("Foreground%i"), i);
				dwSize = sizeof(dwValeur);
				Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
				if (!Error && dwSize > 0) FColorParams[i].Foreground = (int) dwValeur;

				asKey.sprintf(_T("Bold%i"), i);
				dwSize = sizeof(dwValeur);
				Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
				if (!Error && dwSize > 0) FColorParams[i].bBold = (dwValeur != 0);

				asKey.sprintf(_T("Italic%i"), i);
				dwSize = sizeof(dwValeur);
				Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
				if (!Error && dwSize > 0) FColorParams[i].bItalic = (dwValeur != 0);

			}

      RegCloseKey(hSubKey);

    }

		// ---------------------- Editor\Font -------------------------

    Error = RegOpenKeyEx(hKey, _T("Editor\\Font"), 0,
                          KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

      dwSize = MAX_PATH;
      Error = RegQueryValueEx(hSubKey, _T("Name"), 0, &dwType, (LPBYTE) szFileName, &dwSize);
			if (!Error && dwSize > 0) FFont->Name = szFileName;

			dwSize = sizeof(dwValeur);
			Error = RegQueryValueEx(hSubKey, _T("Size"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
			if (!Error && dwSize > 0) FFont->Size = (int) dwValeur;

      RegCloseKey(hSubKey);

    }

		// ---------------------- Dialogs -------------------------

    Error = RegOpenKeyEx(hKey, _T("Dialogs"), 0,
                          KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("AlignGrid"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FAlignGrid = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("ShowGrid"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FShowGrid = (dwValeur != 0);

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("GridX"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FGridX = (int) dwValeur;

      dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("GridY"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FGridY = (int) dwValeur;

      RegCloseKey(hSubKey);

    }

		// ---------------------- SearchAndReplace -------------------------

    Error = RegOpenKeyEx(hKey, _T("SearchAndReplace"), 0,
                          KEY_READ, &hSubKey);

		if (Error == ERROR_SUCCESS) {
      TCHAR szValue[256];

      for (i = 0; i < 10; i++) {

        asKey.sprintf(_T("SearchStrings%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szValue, &dwSize);
        if (Error || dwSize <= 0) break;
        FSearchStrings->Add(szValue);

      }

      for (i = 0; i < 10; i++) {

        asKey.sprintf(_T("ReplaceString%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szValue, &dwSize);
        if (Error || dwSize <= 0) break;
        FReplaceStrings->Add(szValue);

      }

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("WholeWord"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FWholeWord = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("CaseSensitive"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FCaseSensitive = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("BegLine"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FBegLine = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("EndLine"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FEndLine = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("OnlyInComments"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FOnlyInComments = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("NotInComments"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FNotInComments = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("OnlyInQuotes"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FOnlyInQuotes = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("NotInQuotes"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FNotInQuotes = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("RegExp"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FRegExp = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("KeepCase"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FKeepCase = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("Loop"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FLoop = (dwValeur != 0);

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("SearchIn"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FSearchIn = dwValeur;

      dwSize = MAX_PATH;
      Error = RegQueryValueEx(hSubKey, _T("SearchDirectory"), 0, &dwType, (LPBYTE) szValue, &dwSize);
      if (!Error && dwSize > 0) SearchDirectory = szValue;

      dwSize = MAX_PATH;
      Error = RegQueryValueEx(hSubKey, _T("SearchExtensions"), 0, &dwType, (LPBYTE) szValue, &dwSize);
      if (!Error && dwSize > 0) SearchExtensions = szValue;

			dwSize = sizeof(dwValeur);
      Error = RegQueryValueEx(hSubKey, _T("Recurse"), 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
      if (!Error && dwSize > 0) FRecurse = (dwValeur != 0);

			RegCloseKey(hSubKey);

    }

		// ---------------------- ShortCuts -------------------------

    Error = RegOpenKeyEx(hKey, _T("ShortCuts"), 0,
                          KEY_READ, &hSubKey);

		if (Error == ERROR_SUCCESS) {

			TCHAR szKeyName[128];

			i = 0;

			while (true) {

	      dwSize = sizeof(szKeyName) / sizeof(TCHAR);
				Error = RegEnumValue(hSubKey, i++, szKeyName, &dwSize, NULL, NULL, NULL, NULL);
				if (Error != ERROR_SUCCESS) break;

				dwSize = sizeof(dwValeur);
				Error = RegQueryValueEx(hSubKey, szKeyName, 0, &dwType, (LPBYTE) &dwValeur, &dwSize);
				if (!Error && dwSize > 0) {
					ShortCuts.insert( std::pair<AnsiString, TShortCut>(szKeyName, (TShortCut) dwValeur) );
				}

			}

			RegCloseKey(hSubKey);

		}

		// ---------------------- UserTools -------------------------

    Error = RegOpenKeyEx(hKey, _T("UserTools"), 0,
                          KEY_READ, &hSubKey);

    if (Error == ERROR_SUCCESS) {

	    USERTOOL UserTool;
			TCHAR szToolName[MAX_PATH];
	    TCHAR szToolProg[MAX_PATH];
	    TCHAR szToolArgs[MAX_PATH];
	    TCHAR szToolInitDir[MAX_PATH];

			i = 0;

			while (true) {

        asKey.sprintf(_T("Name%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szToolName, &dwSize);
        if (Error || dwSize <= 0) break;
        UserTool.Name = szToolName;

        asKey.sprintf(_T("Prog%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szToolProg, &dwSize);
        if (Error || dwSize <= 0) break;
        UserTool.Prog = szToolProg;

        asKey.sprintf(_T("Args%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szToolArgs, &dwSize);
        if (Error) break;  //  dwSize peut être égal à zéro
				UserTool.Args = szToolArgs;

        asKey.sprintf(_T("InitDir%i"), i);
        dwSize = MAX_PATH;
        Error = RegQueryValueEx(hSubKey, (LPCTSTR) asKey, 0, &dwType, (LPBYTE) szToolInitDir, &dwSize);
        if (Error) break;  //  dwSize peut être égal à zéro
				UserTool.InitDir = szToolInitDir;

				FUserTools.push_back(UserTool);
				i++;

      }

      RegCloseKey(hSubKey);

    }

    RegCloseKey(hKey);
  }

  bModifs = false;

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TConfig::~TConfig(void) {
  HKEY hKey;
  LONG Error;
  DWORD dwSize;
  DWORD dwDisposition;
  DWORD dwValeur;
  AnsiString asValeur;


  if (bModifs) {

    // Ouverture du registre
    Error = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE"), 0, NULL,
                            REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hKey,
                            &dwDisposition);

    // Ecriture de la clé
    if (Error == ERROR_SUCCESS) {

      HKEY hSubKey;
      AnsiString asKey;
      int i;

			// ---------------------- MainWindow -------------------------

      Error = RegCreateKeyEx(hKey, _T("MainWindow"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

      if (Error == ERROR_SUCCESS) {

        dwValeur = FEdiLeft;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("Left"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwValeur = FEdiTop;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("Top"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwValeur = FEdiWidth;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("Width"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwValeur = FEdiHeight;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("Height"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwValeur = FEdiFullScreen;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("FullScreen"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwValeur = FLeftPaneWidth;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("LeftPaneWidth"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwValeur = FBottomPaneHeight;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("BottomPaneHeight"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				asKey = _T("OpenWorkSpace");
				if (!FOpenWorkSpace.IsEmpty()) {
					asValeur = FOpenWorkSpace;
					dwSize = asValeur.Length() * sizeof(TCHAR);
					Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
				}
        else {
          Error = RegDeleteValue(hSubKey, (LPCTSTR) asKey);
        }

        RegCloseKey(hSubKey);

      }

			// ---------------------- Files\LastFiles -------------------------

      Error = RegCreateKeyEx(hKey, _T("Files\\LastFiles"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

      if (Error == ERROR_SUCCESS) {

        for (i = 0; i < 10; i++) {

          asKey.sprintf(_T("LastWorkSpace%i"), i);

          if (i < FLastWorkSpaces->Count) {
            asValeur = FLastWorkSpaces->Strings[i];
            dwSize = asValeur.Length() * sizeof(TCHAR);
            Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

        }

        for (i = 0; i < 10; i++) {

          asKey.sprintf(_T("LastProject%i"), i);

          if (i < FLastProjects->Count) {
            asValeur = FLastProjects->Strings[i];
            dwSize = asValeur.Length() * sizeof(TCHAR);
            Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

        }

        for (i = 0; i <= /* <= c'est exprès */ FLastFiles->Count; i++) {

          asKey.sprintf(_T("LastFile%i"), i);

          if (i < FLastFiles->Count) {
            asValeur = FLastFiles->Strings[i];
            dwSize = asValeur.Length() * sizeof(TCHAR);
            Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

        }

        for (i = 0; i <= /* <= c'est exprès */ FLastDialogs->Count; i++) {

          asKey.sprintf(_T("LastDialog%i"), i);

          if (i < FLastDialogs->Count) {
            asValeur = FLastDialogs->Strings[i];
            dwSize = asValeur.Length() * sizeof(TCHAR);
            Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

        }

        for (i = 0; i <= /* <= c'est exprès */ FLastGraphics->Count; i++) {

          asKey.sprintf(_T("LastGraphic%i"), i);

          if (i < FLastGraphics->Count) {
            asValeur = FLastGraphics->Strings[i];
            dwSize = asValeur.Length() * sizeof(TCHAR);
            Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

        }

        RegCloseKey(hSubKey);

      }

			// ---------------------- Files -------------------------

      Error = RegCreateKeyEx(hKey, _T("Files"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

      if (Error == ERROR_SUCCESS) {

        dwSize = sizeof(dwValeur);
        dwValeur = FEndOfLine;
        Error = RegSetValueEx(hSubKey, _T("EndOfLine"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);


        RegCloseKey(hSubKey);

      }

			// ---------------------- Editor -------------------------

      Error = RegCreateKeyEx(hKey, _T("Editor"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

      if (Error == ERROR_SUCCESS) {

        dwSize = sizeof(dwValeur);
        dwValeur = FNbSpacesTab;
        Error = RegSetValueEx(hSubKey, _T("NbSpacesTab"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FSpacesToTabs;
        Error = RegSetValueEx(hSubKey, _T("SpacesToTabs"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FTabsToSpaces;
        Error = RegSetValueEx(hSubKey, _T("TabsToSpaces"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FTypeAuto;
        Error = RegSetValueEx(hSubKey, _T("TypeAuto"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FIndentAuto;
        Error = RegSetValueEx(hSubKey, _T("IndentAuto"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FCurAfterEndLine;
        Error = RegSetValueEx(hSubKey, _T("CurAfterEndLine"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FUndoRedoMoveCur;
        Error = RegSetValueEx(hSubKey, _T("UndoRedoMoveCur"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FLinesNumbers;
        Error = RegSetValueEx(hSubKey, _T("LinesNumbers"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FIconsColumn;
        Error = RegSetValueEx(hSubKey, _T("IconsColumn"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FMultiLineTabs;
        Error = RegSetValueEx(hSubKey, _T("MultiLineTabs"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FSaveBeforeBuild;
        Error = RegSetValueEx(hSubKey, _T("SaveBeforeBuild"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FAutocompletion;
        Error = RegSetValueEx(hSubKey, _T("Autocompletion"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FAlphaBlend;
        Error = RegSetValueEx(hSubKey, _T("AlphaBlend"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FEndLineAcc;
        Error = RegSetValueEx(hSubKey, _T("EndLineAcc"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FSpacesAfterPar;
        Error = RegSetValueEx(hSubKey, _T("SpacesAfterPar"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        RegCloseKey(hSubKey);

      }

			// ---------------------- Editor\Colors -------------------------

      Error = RegCreateKeyEx(hKey, _T("Editor\\Colors"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

			if (Error == ERROR_SUCCESS) {

				for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {

					asKey.sprintf(_T("Background%i"), i);
          dwSize = sizeof(dwValeur);
					dwValeur = FColorParams[i].Background.cr;
					Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

					asKey.sprintf(_T("Foreground%i"), i);
          dwSize = sizeof(dwValeur);
					dwValeur = FColorParams[i].Foreground.cr;
					Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

					asKey.sprintf(_T("Bold%i"), i);
          dwSize = sizeof(dwValeur);
					dwValeur = FColorParams[i].bBold;
					Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

					asKey.sprintf(_T("Italic%i"), i);
          dwSize = sizeof(dwValeur);
					dwValeur = FColorParams[i].bItalic;
					Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				}

				RegCloseKey(hSubKey);

      }

			// ---------------------- Editor\Font -------------------------

      Error = RegCreateKeyEx(hKey, _T("Editor\\Font"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

			if (Error == ERROR_SUCCESS) {

				asValeur = FFont->Name;
        dwSize = asValeur.Length() * sizeof(TCHAR);
        Error = RegSetValueEx(hSubKey, _T("Name"), 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);

				dwValeur = FFont->Size;
        dwSize = sizeof(dwValeur);
        Error = RegSetValueEx(hSubKey, _T("Size"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				RegCloseKey(hSubKey);

      }

			// ---------------------- Dialogs -------------------------

      Error = RegCreateKeyEx(hKey, _T("Dialogs"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

			if (Error == ERROR_SUCCESS) {

        dwSize = sizeof(dwValeur);
        dwValeur = FAlignGrid;
        Error = RegSetValueEx(hSubKey, _T("AlignGrid"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FShowGrid;
        Error = RegSetValueEx(hSubKey, _T("ShowGrid"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FGridX;
        Error = RegSetValueEx(hSubKey, _T("GridX"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        dwSize = sizeof(dwValeur);
        dwValeur = FGridY;
        Error = RegSetValueEx(hSubKey, _T("GridY"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);


				RegCloseKey(hSubKey);

      }

			// ---------------------- SearchAndReplace -------------------------

      Error = RegCreateKeyEx(hKey, _T("SearchAndReplace"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

      if (Error == ERROR_SUCCESS) {

        for (i = 0; i < 10; i++) {

          asKey.sprintf(_T("SearchStrings%i"), i);

          if (i < FSearchStrings->Count) {
            dwSize = FSearchStrings->Strings[i].Length() * sizeof(TCHAR);
            asValeur = FSearchStrings->Strings[i];
            Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

        }

        for (i = 0; i < FReplaceStrings->Count; i++) {

          asKey.sprintf(_T("ReplaceString%i"), i);

          if (i < FReplaceStrings->Count) {
            dwSize = FReplaceStrings->Strings[i].Length() * sizeof(TCHAR);
            asValeur = FReplaceStrings->Strings[i];
            Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

				}

				dwValeur = FWholeWord;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("WholeWord"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FCaseSensitive;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("CaseSensitive"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FBegLine;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("BegLine"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FEndLine;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("EndLine"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FOnlyInComments;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("OnlyInComments"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FNotInComments;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("NotInComments"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FOnlyInQuotes;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("OnlyInQuotes"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FNotInQuotes;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("NotInQuotes"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FRegExp;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("RegExp"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FKeepCase;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("KeepCase"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FLoop;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("Loop"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FSearchIn;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("SearchIn"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

				dwValeur = FRecurse;
				dwSize = sizeof(dwValeur);
				Error = RegSetValueEx(hSubKey, _T("Recurse"), 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);

        asValeur = FSearchDirectory;
        dwSize = FSearchDirectory.Length() * sizeof(TCHAR);
        Error = RegSetValueEx(hSubKey, _T("SearchDirectory"), 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);

        asValeur = FSearchExtensions;
        dwSize = FSearchExtensions.Length() * sizeof(TCHAR);
        Error = RegSetValueEx(hSubKey, _T("SearchExtensions"), 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);

				RegCloseKey(hSubKey);

      }

			// ---------------------- ShortCuts -------------------------

      Error = RegCreateKeyEx(hKey, _T("ShortCuts"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

			if (Error == ERROR_SUCCESS) {

				std::map<AnsiString, TShortCut>::iterator it;

				for (it = ShortCuts.begin(); it != ShortCuts.end(); it++) {

					dwValeur = it->second;
					if (dwValeur) {
						dwSize = sizeof(dwValeur);
						Error = RegSetValueEx(hSubKey, it->first, 0, REG_DWORD, (LPBYTE) &dwValeur, dwSize);
					}
					else {
						Error = RegDeleteValue(hSubKey, it->first);
					}

				}

				RegCloseKey(hSubKey);

			}

			// ---------------------- UserTools -------------------------

      Error = RegCreateKeyEx(hKey, _T("UserTools"), 0, NULL,
                              REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hSubKey,
                              &dwDisposition);

			if (Error == ERROR_SUCCESS) {

				for (i = 0; i <= (int) FUserTools.size(); i++) {

          asKey.sprintf(_T("Name%i"), i);

					if (i < (int) FUserTools.size()) {
						asValeur = FUserTools[i].Name;
						dwSize = asValeur.Length() * sizeof(TCHAR);
						Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

          asKey.sprintf(_T("Prog%i"), i);

					if (i < (int) FUserTools.size()) {
						asValeur = FUserTools[i].Prog;
						dwSize = asValeur.Length() * sizeof(TCHAR);
						Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

          asKey.sprintf(_T("Args%i"), i);

					if (i < (int) FUserTools.size()) {
						asValeur = FUserTools[i].Args;
						dwSize = asValeur.Length() * sizeof(TCHAR);
						Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

          asKey.sprintf(_T("InitDir%i"), i);

					if (i < (int) FUserTools.size()) {
						asValeur = FUserTools[i].InitDir;
						dwSize = asValeur.Length() * sizeof(TCHAR);
						Error = RegSetValueEx(hSubKey, (LPCTSTR) asKey, 0, REG_SZ, (LPBYTE) (LPCTSTR) asValeur, dwSize);
          }
          else {
            RegDeleteValue(hSubKey, (LPCTSTR) asKey);
          }

				}


				RegCloseKey(hSubKey);

			}

      RegCloseKey(hKey);
    }
    else {
      MessageBox(NULL,
                 _TT("Impossible d'enregistrer la configuration"),
                 _TT("Erreur"),
                 MB_OK | MB_ICONSTOP);
    }

  }

  delete FFont;
  delete FLastWorkSpaces;
  delete FLastProjects;
  delete FLastFiles;
  delete FLastDialogs;
  delete FLastGraphics;
  delete FSearchStrings;
  delete FReplaceStrings;

}


/*


//---------------------------------------------------------------------------
// Accesseurs de la propriété {{Property}}
//---------------------------------------------------------------------------

{{Type}} TConfig::Get_{{Property}}(void) {
  return F{{Property}};
}

bool TConfig::Set_{{Property}}({{Type}} New{{Property}}) {
  if (F{{Property}} != New{{Property}}) {
    F{{Property}} = New{{Property}};
    bModifs = true;
  }

  return true;
}
*/


//---------------------------------------------------------------------------
// Accesseurs de la propriété InstallPath
//---------------------------------------------------------------------------

AnsiString TConfig::Get_InstallPath(void) {
  return FInstallPath;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété EdiLeft
//---------------------------------------------------------------------------

int TConfig::Get_EdiLeft(void) {
  return FEdiLeft;
}

bool TConfig::Set_EdiLeft(int NewEdiLeft) {
  if (FEdiLeft != NewEdiLeft) {
    FEdiLeft = NewEdiLeft;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EdiTop
//---------------------------------------------------------------------------

int TConfig::Get_EdiTop(void) {
  return FEdiTop;
}

bool TConfig::Set_EdiTop(int NewEdiTop) {
  if (FEdiTop != NewEdiTop) {
    FEdiTop = NewEdiTop;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EdiWidth
//---------------------------------------------------------------------------

int TConfig::Get_EdiWidth(void) {
  return FEdiWidth;
}

bool TConfig::Set_EdiWidth(int NewEdiWidth) {
  if (FEdiWidth != NewEdiWidth) {
    FEdiWidth = NewEdiWidth;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EdiHeight
//---------------------------------------------------------------------------

int TConfig::Get_EdiHeight(void) {
  return FEdiHeight;
}

bool TConfig::Set_EdiHeight(int NewEdiHeight) {
  if (FEdiHeight != NewEdiHeight) {
    FEdiHeight = NewEdiHeight;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EdiFullScreen
//---------------------------------------------------------------------------

bool TConfig::Get_EdiFullScreen(void) {
  return FEdiFullScreen;
}

bool TConfig::Set_EdiFullScreen(bool NewEdiFullScreen) {
  if (FEdiFullScreen != NewEdiFullScreen) {
    FEdiFullScreen = NewEdiFullScreen;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LeftPaneWidth
//---------------------------------------------------------------------------

int TConfig::Get_LeftPaneWidth(void) {
  return FLeftPaneWidth;
}

bool TConfig::Set_LeftPaneWidth(int NewLeftPaneWidth) {
  if (FLeftPaneWidth != NewLeftPaneWidth) {
    FLeftPaneWidth = NewLeftPaneWidth;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BottomPaneHeight
//---------------------------------------------------------------------------

int TConfig::Get_BottomPaneHeight(void) {
  return FBottomPaneHeight;
}

bool TConfig::Set_BottomPaneHeight(int NewBottomPaneHeight) {
  if (FBottomPaneHeight != NewBottomPaneHeight) {
    FBottomPaneHeight = NewBottomPaneHeight;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LastWorkSpaces
//---------------------------------------------------------------------------

AnsiString TConfig::Get_LastWorkSpaces(int Index) {
  if (Index < FLastWorkSpaces->Count) return FLastWorkSpaces->Strings[Index];
  return _T("");
}

bool TConfig::Set_LastWorkSpaces(int Index, AnsiString NewLastWorkSpaces) {
  if (NewLastWorkSpaces != FLastWorkSpaces->Strings[Index]) {
    FLastWorkSpaces->Set_Strings(Index, NewLastWorkSpaces);
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LastProjects
//---------------------------------------------------------------------------

AnsiString TConfig::Get_LastProjects(int Index) {
  if (Index < FLastProjects->Count) return FLastProjects->Strings[Index];
  return _T("");
}

bool TConfig::Set_LastProjects(int Index, AnsiString NewLastProjects) {
  if (NewLastProjects != FLastProjects->Strings[Index]) {
    FLastProjects->Set_Strings(Index, NewLastProjects);
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LastFiles
//---------------------------------------------------------------------------

AnsiString TConfig::Get_LastFiles(int Index) {
  if (Index < FLastFiles->Count) return FLastFiles->Strings[Index];
  return _T("");
}

bool TConfig::Set_LastFiles(int Index, AnsiString NewLastFiles) {
  if (NewLastFiles != FLastFiles->Strings[Index]) {
    FLastFiles->Set_Strings(Index, NewLastFiles);
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LastDialogs
//---------------------------------------------------------------------------

AnsiString TConfig::Get_LastDialogs(int Index) {
  if (Index < FLastDialogs->Count) return FLastDialogs->Strings[Index];
  return _T("");
}

bool TConfig::Set_LastDialogs(int Index, AnsiString NewLastDialogs) {
  if (NewLastDialogs != FLastDialogs->Strings[Index]) {
    FLastDialogs->Set_Strings(Index, NewLastDialogs);
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LastGraphics
//---------------------------------------------------------------------------

AnsiString TConfig::Get_LastGraphics(int Index) {
  if (Index < FLastGraphics->Count) return FLastGraphics->Strings[Index];
  return _T("");
}

bool TConfig::Set_LastGraphics(int Index, AnsiString NewLastGraphics) {
  if (NewLastGraphics != FLastGraphics->Strings[Index]) {
    FLastGraphics->Set_Strings(Index, NewLastGraphics);
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété OpenWorkSpace
//---------------------------------------------------------------------------

AnsiString TConfig::Get_OpenWorkSpace(void) {
  return FOpenWorkSpace;
}

bool TConfig::Set_OpenWorkSpace(AnsiString NewOpenWorkSpace) {
  if (FOpenWorkSpace != NewOpenWorkSpace) {
    FOpenWorkSpace = NewOpenWorkSpace;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ActivePageIndex
//---------------------------------------------------------------------------

int TConfig::Get_ActivePageIndex(void) {
  return FActivePageIndex;
}

bool TConfig::Set_ActivePageIndex(int NewActivePageIndex) {
  if (FActivePageIndex != NewActivePageIndex) {
    FActivePageIndex = NewActivePageIndex;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EndOfLine
//---------------------------------------------------------------------------

TEndOfLine TConfig::Get_EndOfLine(void) {
  return FEndOfLine;
}

bool TConfig::Set_EndOfLine(TEndOfLine NewEndOfLine) {
  if (FEndOfLine != NewEndOfLine) {
    FEndOfLine = NewEndOfLine;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NbSpacesTab
//---------------------------------------------------------------------------

int TConfig::Get_NbSpacesTab(void) {
  return FNbSpacesTab;
}

bool TConfig::Set_NbSpacesTab(int NewNbSpacesTab) {
  if (FNbSpacesTab != NewNbSpacesTab) {
    FNbSpacesTab = NewNbSpacesTab;
		if (FNbSpacesTab < 1) FNbSpacesTab = 1;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SpacesToTabs
//---------------------------------------------------------------------------

bool TConfig::Get_SpacesToTabs(void) {
  return FSpacesToTabs;
}

bool TConfig::Set_SpacesToTabs(bool NewSpacesToTabs) {
  if (FSpacesToTabs != NewSpacesToTabs) {
    FSpacesToTabs = NewSpacesToTabs;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TabsToSpaces
//---------------------------------------------------------------------------

bool TConfig::Get_TabsToSpaces(void) {
  return FTabsToSpaces;
}

bool TConfig::Set_TabsToSpaces(bool NewTabsToSpaces) {
  if (FTabsToSpaces != NewTabsToSpaces) {
    FTabsToSpaces = NewTabsToSpaces;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TypeAuto
//---------------------------------------------------------------------------

bool TConfig::Get_TypeAuto(void) {
  return FTypeAuto;
}

bool TConfig::Set_TypeAuto(bool NewTypeAuto) {
  if (FTypeAuto != NewTypeAuto) {
    FTypeAuto = NewTypeAuto;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété IndentAuto
//---------------------------------------------------------------------------

bool TConfig::Get_IndentAuto(void) {
  return FIndentAuto;
}

bool TConfig::Set_IndentAuto(bool NewIndentAuto) {
  if (FIndentAuto != NewIndentAuto) {
    FIndentAuto = NewIndentAuto;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CurAfterEndLine
//---------------------------------------------------------------------------

bool TConfig::Get_CurAfterEndLine(void) {
  return FCurAfterEndLine;
}

bool TConfig::Set_CurAfterEndLine(bool NewCurAfterEndLine) {
  if (FCurAfterEndLine != NewCurAfterEndLine) {
    FCurAfterEndLine = NewCurAfterEndLine;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété UndoRedoMoveCur
//---------------------------------------------------------------------------

bool TConfig::Get_UndoRedoMoveCur(void) {
  return FUndoRedoMoveCur;
}

bool TConfig::Set_UndoRedoMoveCur(bool NewUndoRedoMoveCur) {
  if (FUndoRedoMoveCur != NewUndoRedoMoveCur) {
    FUndoRedoMoveCur = NewUndoRedoMoveCur;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LinesNumbers
//---------------------------------------------------------------------------

bool TConfig::Get_LinesNumbers(void) {
  return FLinesNumbers;
}

bool TConfig::Set_LinesNumbers(bool NewLinesNumbers) {
  if (FLinesNumbers != NewLinesNumbers) {
    FLinesNumbers = NewLinesNumbers;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété IconsColumn
//---------------------------------------------------------------------------

bool TConfig::Get_IconsColumn(void) {
  return FIconsColumn;
}

bool TConfig::Set_IconsColumn(bool NewIconsColumn) {
  if (FIconsColumn != NewIconsColumn) {
    FIconsColumn = NewIconsColumn;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiLineTabs
//---------------------------------------------------------------------------

bool TConfig::Get_MultiLineTabs(void) {
  return FMultiLineTabs;
}

bool TConfig::Set_MultiLineTabs(bool NewMultiLineTabs) {
  if (FMultiLineTabs != NewMultiLineTabs) {
    FMultiLineTabs = NewMultiLineTabs;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SaveBeforeBuild
//---------------------------------------------------------------------------

bool TConfig::Get_SaveBeforeBuild(void) {
  return FSaveBeforeBuild;
}

bool TConfig::Set_SaveBeforeBuild(bool NewSaveBeforeBuild) {
  if (FSaveBeforeBuild != NewSaveBeforeBuild) {
    FSaveBeforeBuild = NewSaveBeforeBuild;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Autocompletion
//---------------------------------------------------------------------------

bool TConfig::Get_Autocompletion(void) {
  return FAutocompletion;
}

bool TConfig::Set_Autocompletion(bool NewAutocompletion) {
  if (FAutocompletion != NewAutocompletion) {
    FAutocompletion = NewAutocompletion;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AlphaBlend
//---------------------------------------------------------------------------

bool TConfig::Get_AlphaBlend(void) {
  return FAlphaBlend;
}

bool TConfig::Set_AlphaBlend(bool NewAlphaBlend) {
  if (FAlphaBlend != NewAlphaBlend) {
    FAlphaBlend = NewAlphaBlend;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EndLineAcc
//---------------------------------------------------------------------------

bool TConfig::Get_EndLineAcc(void) {
  return FEndLineAcc;
}

bool TConfig::Set_EndLineAcc(bool NewEndLineAcc) {
  if (FEndLineAcc != NewEndLineAcc) {
    FEndLineAcc = NewEndLineAcc;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SpacesAfterPar
//---------------------------------------------------------------------------

bool TConfig::Get_SpacesAfterPar(void) {
  return FSpacesAfterPar;
}

bool TConfig::Set_SpacesAfterPar(bool NewSpacesAfterPar) {
  if (FSpacesAfterPar != NewSpacesAfterPar) {
    FSpacesAfterPar = NewSpacesAfterPar;
    bModifs = true;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SearchStrings
//---------------------------------------------------------------------------

TStrings * TConfig::Get_SearchStrings(void) {
  return FSearchStrings;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ReplaceStrings
//---------------------------------------------------------------------------

TStrings * TConfig::Get_ReplaceStrings(void) {
  return FReplaceStrings;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WholeWord
//---------------------------------------------------------------------------

bool TConfig::Get_WholeWord() {
  return FWholeWord;
}

bool TConfig::Set_WholeWord(bool NewWholeWord) {
  if (FWholeWord != NewWholeWord) {
    FWholeWord = NewWholeWord;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CaseSensitive
//---------------------------------------------------------------------------

bool TConfig::Get_CaseSensitive() {
  return FCaseSensitive;
}

bool TConfig::Set_CaseSensitive(bool NewCaseSensitive) {
  if (FCaseSensitive != NewCaseSensitive) {
    FCaseSensitive = NewCaseSensitive;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BegLine
//---------------------------------------------------------------------------

bool TConfig::Get_BegLine() {
  return FBegLine;
}

bool TConfig::Set_BegLine(bool NewBegLine) {
  if (FBegLine != NewBegLine) {
    FBegLine = NewBegLine;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EndLine
//---------------------------------------------------------------------------

bool TConfig::Get_EndLine() {
  return FEndLine;
}

bool TConfig::Set_EndLine(bool NewEndLine) {
  if (FEndLine != NewEndLine) {
    FEndLine = NewEndLine;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété OnlyInComments
//---------------------------------------------------------------------------

bool TConfig::Get_OnlyInComments() {
  return FOnlyInComments;
}

bool TConfig::Set_OnlyInComments(bool NewOnlyInComments) {
  if (FOnlyInComments != NewOnlyInComments) {
    FOnlyInComments = NewOnlyInComments;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NotInComments
//---------------------------------------------------------------------------

bool TConfig::Get_NotInComments() {
  return FNotInComments;
}

bool TConfig::Set_NotInComments(bool NewNotInComments) {
  if (FNotInComments != NewNotInComments) {
    FNotInComments = NewNotInComments;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété OnlyInQuotes
//---------------------------------------------------------------------------

bool TConfig::Get_OnlyInQuotes() {
  return FOnlyInQuotes;
}

bool TConfig::Set_OnlyInQuotes(bool NewOnlyInQuotes) {
  if (FOnlyInQuotes != NewOnlyInQuotes) {
    FOnlyInQuotes = NewOnlyInQuotes;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NotInQuotes
//---------------------------------------------------------------------------

bool TConfig::Get_NotInQuotes() {
  return FNotInQuotes;
}

bool TConfig::Set_NotInQuotes(bool NewNotInQuotes) {
  if (FNotInQuotes != NewNotInQuotes) {
    FNotInQuotes = NewNotInQuotes;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété RegExp
//---------------------------------------------------------------------------

bool TConfig::Get_RegExp() {
  return FRegExp;
}

bool TConfig::Set_RegExp(bool NewRegExp) {
  if (FRegExp != NewRegExp) {
    FRegExp = NewRegExp;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété KeepCase
//---------------------------------------------------------------------------

bool TConfig::Get_KeepCase() {
  return FKeepCase;
}

bool TConfig::Set_KeepCase(bool NewKeepCase) {
  if (FKeepCase != NewKeepCase) {
    FKeepCase = NewKeepCase;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Loop
//---------------------------------------------------------------------------

bool TConfig::Get_Loop() {
  return FLoop;
}

bool TConfig::Set_Loop(bool NewLoop) {
  if (FLoop != NewLoop) {
    FLoop = NewLoop;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SearchIn
//---------------------------------------------------------------------------

int TConfig::Get_SearchIn() {
  return FSearchIn;
}

bool TConfig::Set_SearchIn(int NewSearchIn) {
  if (FSearchIn != NewSearchIn) {
    FSearchIn = NewSearchIn;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Recurse
//---------------------------------------------------------------------------

bool TConfig::Get_Recurse() {
  return FRecurse;
}

bool TConfig::Set_Recurse(bool NewRecurse) {
  if (FRecurse != NewRecurse) {
    FRecurse = NewRecurse;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SearchDirectory
//---------------------------------------------------------------------------

AnsiString TConfig::Get_SearchDirectory() {
  return FSearchDirectory;
}

bool TConfig::Set_SearchDirectory(AnsiString NewSearchDirectory) {
  if (FSearchDirectory != NewSearchDirectory) {
    FSearchDirectory = NewSearchDirectory;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SearchExtensions
//---------------------------------------------------------------------------

AnsiString TConfig::Get_SearchExtensions() {
  return FSearchExtensions;
}

bool TConfig::Set_SearchExtensions(AnsiString NewSearchExtensions) {
  if (FSearchExtensions != NewSearchExtensions) {
    FSearchExtensions = NewSearchExtensions;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ColorParams
//---------------------------------------------------------------------------

COLORPARAM TConfig::Get_ColorParams(int Index) {
	if ((TYPESYNTAX) Index < tsNormal || MAX_TYPESYNTAX < (TYPESYNTAX) Index) Index = 0;
  return FColorParams[Index];
}

bool TConfig::Set_ColorParams(int Index, COLORPARAM NewColorParams) {
	if ((TYPESYNTAX) Index < tsNormal || MAX_TYPESYNTAX < (TYPESYNTAX) Index) return false;
  FColorParams[Index] = NewColorParams;

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

TCustomFont *TConfig::Get_Font(void) {
  return FFont;
}

bool TConfig::Set_Font(TCustomFont *NewFont) {
  FFont->Assign(NewFont);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AlignGrid
//---------------------------------------------------------------------------

bool TConfig::Get_AlignGrid() {
  return FAlignGrid;
}

bool TConfig::Set_AlignGrid(bool NewAlignGrid) {
  if (FAlignGrid != NewAlignGrid) {
    FAlignGrid = NewAlignGrid;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowGrid
//---------------------------------------------------------------------------

bool TConfig::Get_ShowGrid() {
  return FShowGrid;
}

bool TConfig::Set_ShowGrid(bool NewShowGrid) {
  if (FShowGrid != NewShowGrid) {
    FShowGrid = NewShowGrid;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété GridX
//---------------------------------------------------------------------------

int TConfig::Get_GridX() {
  return FGridX;
}

bool TConfig::Set_GridX(int NewGridX) {
  if (FGridX != NewGridX) {
    FGridX = NewGridX;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété GridY
//---------------------------------------------------------------------------

int TConfig::Get_GridY() {
  return FGridY;
}

bool TConfig::Set_GridY(int NewGridY) {
  if (FGridY != NewGridY) {
    FGridY = NewGridY;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété UserToolsCount
//---------------------------------------------------------------------------

int TConfig::Get_UserToolsCount(void) {
  return (int) FUserTools.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété UserTools
//---------------------------------------------------------------------------

const USERTOOL * TConfig::Get_UserTools(int Index) {
  return &FUserTools[Index];
}


//---------------------------------------------------------------------------
bool TConfig::PushLastWorkSpace(AnsiString asFileName) {
  int i;


  // Elimination des doublons
  for (i = 0; i < FLastWorkSpaces->Count; i++) {
    if (FLastWorkSpaces->Strings[i] == asFileName) {
      FLastWorkSpaces->Delete(i);
      break;
    }
  }

  // Insertion au début
  FLastWorkSpaces->Insert(0, asFileName);
  bModifs = true;

  // Pas la peine de mémoriser plus de 10 noms
  if (FLastWorkSpaces->Count > 10) {
      FLastWorkSpaces->Delete(10);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TConfig::PushLastProject(AnsiString asFileName) {
  int i;


  // Elimination des doublons
  for (i = 0; i < FLastProjects->Count; i++) {
    if (FLastProjects->Strings[i] == asFileName) {
      FLastProjects->Delete(i);
      break;
    }
  }

  // Insertion au début
  FLastProjects->Insert(0, asFileName);
  bModifs = true;

  // Pas la peine de mémoriser plus de 10 noms
  if (FLastProjects->Count > 10) {
      FLastProjects->Delete(10);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TConfig::PushLastFile(AnsiString asFileName) {
  int i;


  // Elimination des doublons
  for (i = 0; i < FLastFiles->Count; i++) {
    if (FLastFiles->Strings[i] == asFileName) {
      FLastFiles->Delete(i);
      break;
    }
  }

  // Insertion au début
  FLastFiles->Insert(0, asFileName);
  bModifs = true;

  // Pas la peine de mémoriser plus de 10 noms
  if (FLastFiles->Count > 10) {
      FLastFiles->Delete(10);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TConfig::PushLastDialog(AnsiString asFileName) {
  int i;


  // Elimination des doublons
  for (i = 0; i < FLastDialogs->Count; i++) {
    if (FLastDialogs->Strings[i] == asFileName) {
      FLastDialogs->Delete(i);
      break;
    }
  }

  // Insertion au début
  FLastDialogs->Insert(0, asFileName);
  bModifs = true;

  // Pas la peine de mémoriser plus de 10 noms
  if (FLastDialogs->Count > 10) {
      FLastDialogs->Delete(10);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TConfig::PushLastGraphic(AnsiString asFileName) {
  int i;


  // Elimination des doublons
  for (i = 0; i < FLastGraphics->Count; i++) {
    if (FLastGraphics->Strings[i] == asFileName) {
      FLastGraphics->Delete(i);
      break;
    }
  }

  // Insertion au début
  FLastGraphics->Insert(0, asFileName);
  bModifs = true;

  // Pas la peine de mémoriser plus de 10 noms
  if (FLastGraphics->Count > 10) {
      FLastGraphics->Delete(10);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TConfig::PushSearchString(AnsiString asString) {
  int i;


  // Elimination des doublons
  for (i = 0; i < FSearchStrings->Count; i++) {
    if (FSearchStrings->Strings[i] == asString) {
      FSearchStrings->Delete(i);
      break;
    }
  }

  // Insertion au début
  FSearchStrings->Insert(0, asString);
  bModifs = true;

  // Pas la peine de mémoriser plus de 10 noms
  if (FSearchStrings->Count > 10) {
      FSearchStrings->Delete(10);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TConfig::PushReplaceString(AnsiString asString) {
  int i;


  // Elimination des doublons
  for (i = 0; i < FReplaceStrings->Count; i++) {
    if (FReplaceStrings->Strings[i] == asString) {
      FReplaceStrings->Delete(i);
      break;
    }
  }

  // Insertion au début
  FReplaceStrings->Insert(0, asString);
  bModifs = true;

  // Pas la peine de mémoriser plus de 10 noms
  if (FReplaceStrings->Count > 10) {
      FReplaceStrings->Delete(10);
  }

  return true;
}

//---------------------------------------------------------------------------
TShortCut TConfig::GetShortCut(AnsiString asActionName) {
	return ShortCuts[asActionName];
}

//---------------------------------------------------------------------------
bool TConfig::SetShortCut(AnsiString asActionName, TShortCut ShortCut) {
	std::map<AnsiString, TShortCut>::iterator it;

	it = ShortCuts.find(asActionName);
	if (it != ShortCuts.end()) {
		it->second = ShortCut;
	}
	else {
		ShortCuts.insert( std::pair<AnsiString, TShortCut>(asActionName, ShortCut) );
	}

	return true;
}

//---------------------------------------------------------------------------
bool TConfig::AddUserTool(AnsiString asToolName, AnsiString asToolProg, AnsiString asToolArgs, AnsiString asToolInitDir) {
	int i;


	for (i = 0; i < (int) FUserTools.size(); i++) {
		if (FUserTools[i].Name == asToolName) {
			FUserTools[i].Prog = asToolProg;
			FUserTools[i].Args = asToolArgs;
			FUserTools[i].InitDir = asToolInitDir;
			return true;
		}
	}

	USERTOOL UserTool;
	UserTool.Name = asToolName;
	UserTool.Prog = asToolProg;
	UserTool.Args = asToolArgs;
	UserTool.InitDir = asToolInitDir;
	FUserTools.push_back(UserTool);

	return true;
}

//---------------------------------------------------------------------------
bool TConfig::DeleteUserTool(AnsiString asToolName) {
	int i;


	for (i = 0; i < (int) FUserTools.size(); i++) {
		if (FUserTools[i].Name == asToolName) {
			FUserTools.erase(FUserTools.begin() + i);
			break;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
