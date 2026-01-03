//-----------------------------------------------------------------------------
//! @file Application.cpp
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

#include "../../Include/Plugin.h"
#include "../../Include/PluginUtilities.h"
#include "Application.h"
#include "Resource.h"
#include "TForm_CreateProject.h"
#include "TForm_EditProject.h"
#include "TPreCompiler.h"

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
extern HINSTANCE HInstance;

CALLBACK_ERROR CallbackError;
AnsiString asProjectFileName;
AnsiString asInputFileNames;
AnsiString asOutputFileNames;


//---------------------------------------------------------------------------
// Prototype des fonctions
//---------------------------------------------------------------------------
BOOL CreateNewProject(AnsiString asProjectPath, AnsiString asProjectName, int ProjectType, AnsiString asOutputFileName);
BOOL CopyProject(AnsiString asSrcPath, AnsiString asDestPath, AnsiString asProjectName, AnsiString asGuid, TStringList *slListSrcFiles);
BOOL CopyReplaceFile(AnsiString asSrcFileName, AnsiString asDestFileName, AnsiString asProjectName, AnsiString asGuid);
BOOL LoadProject(const wchar_t *szProjectFileName, AnsiString *asGuid, TYPECOMPILER *tc, AnsiString *asOutputFileName, TStringList *slListSrcFiles);
BOOL SaveProject(const wchar_t *szProjectFileName, AnsiString asGuid, TYPECOMPILER tc, AnsiString asOutputFileName, TStringList *slListSrcFiles);


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
	HKEY hKey;
  LONG Error;
  DWORD dwSize;
  DWORD dwType;
	BOOL Ok = FALSE;
  wchar_t szCompilerPath[MAX_PATH];


	// Au moins un compilateur installé
	if (GetCompilerPath(tcVS9, szCompilerPath, MAX_PATH)) return TRUE;
	if (GetCompilerPath(tcVS10, szCompilerPath, MAX_PATH)) return TRUE;

	// Si non installé : Recherche automatique d'un compilateur

	// --------------------------------------------------------------------
	// Visual Studio 2009
	// --------------------------------------------------------------------
	Error = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\VCExpress\\9.0\\External Tools"), 0,
                       KEY_READ, &hKey);

  // Lecture de la clé
  if (Error == ERROR_SUCCESS) {

		dwSize = MAX_PATH;
		Error = RegQueryValueEx(hKey, _T("ToolDir2"), 0, &dwType, (LPBYTE) szCompilerPath, &dwSize);
		if (!Error && dwSize > 0) {
			SetCompilerPath(tcVS9, szCompilerPath);
			Ok = TRUE;
		}

    RegCloseKey(hKey);
	}

	// --------------------------------------------------------------------
	// Visual Studio 2010
	// HKEY_CURRENT_USER\Software\Microsoft\VCExpress\10.0\External Tools\ToolDir2
	// --------------------------------------------------------------------
	Error = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\VCExpress\\10.0\\External Tools"), 0,
                       KEY_READ, &hKey);

  // Lecture de la clé
  if (Error == ERROR_SUCCESS) {

		dwSize = MAX_PATH;
		Error = RegQueryValueEx(hKey, _T("ToolDir2"), 0, &dwType, (LPBYTE) szCompilerPath, &dwSize);
		if (!Error && dwSize > 0) {
			SetCompilerPath(tcVS10, szCompilerPath);
			Ok = TRUE;
		}

    RegCloseKey(hKey);
	}

	if (!Ok) {
		// Message d'erreur installation
		CallbackError(Translate(L"Aucun compilateur trouvé").c_str(),
									Translate(L"Installation plugin \"Application FreeVCL\"").c_str(),
									MB_OK | MB_ICONSTOP);
	}

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap20(void) {
	return (HBITMAP) LoadImage(HInstance, _T("BMP_APPLICATION_20"), IMAGE_BITMAP, 20, 20, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void) {
	return (HBITMAP) LoadImage(HInstance, _T("BMP_APPLICATION_32"), IMAGE_BITMAP, 32, 32, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void) {
	return (HBITMAP) LoadImage(HInstance, _T("BMP_APPLICATION_48"), IMAGE_BITMAP, 48, 48, 0);
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *aszDescription, int SizeDescription) {
	lstrcpynW(aszDescription, _TT("Application FreeVCL"), SizeDescription);
	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	TForm_CreateProject *Form_CreateProject;
	AnsiString asProjectPath;
	AnsiString asProjectName;
	AnsiString asOutputFileName;
	int ProjectType;
	bool Ok = false;


	Form_CreateProject = new TForm_CreateProject(hWnd, _T("TForm_CreateProject"));
	if (Form_CreateProject->ShowModal() == mrOk) {
		asProjectPath = Form_CreateProject->Edit_Path->Text;
		asProjectName = Form_CreateProject->Edit_Name->Text;
		ProjectType = Form_CreateProject->ComboBox_Modele->ItemIndex;

		if (CreateNewProject(asProjectPath, asProjectName, ProjectType, asOutputFileName)) {
			lstrcpyn(szProjectFileName, asProjectFileName, MAX_PATH);
			Ok = true;
		}

	}
	delete Form_CreateProject;

	if (Ok) return TRUE;
	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Open(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd) {
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;
	TForm_EditProject *Form_EditProject;
	AnsiString asFileName;
	AnsiString asExt;
	TTreeNode *FirstNode;
	TTreeNode *HeaderNode;
	TTreeNode *SourceNode;
	TTreeNode *ResourceNode;
	TTreeNode *OtherNode;
	TTreeNode *TreeNode;
	int i;
	bool Ok = false;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	Form_EditProject = new TForm_EditProject(hWnd, _T("TForm_EditProject"));
	asProjectFileName = szProjectFileName;

	Form_EditProject->TypeCompiler = TypeCompiler;
	Form_EditProject->Edit_Output->Text = asOutputFileName;

	FirstNode = Form_EditProject->TreeView_SourceFiles->Items->FirstNode;
	HeaderNode = Form_EditProject->TreeView_SourceFiles->Items->Add(FirstNode, _TT("Fichiers d'entête"));
	SourceNode = Form_EditProject->TreeView_SourceFiles->Items->Add(FirstNode, _TT("Fichiers source"));
	ResourceNode = Form_EditProject->TreeView_SourceFiles->Items->Add(FirstNode, _TT("Fichiers de ressource"));
	OtherNode = Form_EditProject->TreeView_SourceFiles->Items->Add(FirstNode, _TT("Autres fichiers"));
	for (i = 0; i < slListSrcFiles.Count; i++) {
		asFileName = ExtractFileName(slListSrcFiles.Strings[i]);
		asExt = ExtractFileExt(asFileName).LowerCase();
		if (asExt == _T(".h")) {
			Form_EditProject->TreeView_SourceFiles->Items->AddChild(HeaderNode, asFileName);
		}
		else if (asExt == _T(".c") || asExt == _T(".cpp")) {
			Form_EditProject->TreeView_SourceFiles->Items->AddChild(SourceNode, asFileName);
		}
		else if (asExt == _T(".rc") || asExt == _T(".dfm")) {
			Form_EditProject->TreeView_SourceFiles->Items->AddChild(ResourceNode, asFileName);
		}
		else {
			Form_EditProject->TreeView_SourceFiles->Items->AddChild(OtherNode, asFileName);
		}
	}

	if (Form_EditProject->ShowModal() == mrOk) {
		TypeCompiler = Form_EditProject->TypeCompiler;
		asOutputFileName = Form_EditProject->Edit_Output->Text;
		slListSrcFiles.Clear();
		for (i = 0; i < Form_EditProject->TreeView_SourceFiles->Items->Count; i++) {
			TreeNode = Form_EditProject->TreeView_SourceFiles->Items->Item[i];
			if (TreeNode && TreeNode->Level == 1) {
				slListSrcFiles.Add(TreeNode->Text);
			}
		}
		if (SaveProject(szProjectFileName, asGuid, TypeCompiler, asOutputFileName, &slListSrcFiles)) Ok = true;
	}
	delete Form_EditProject;

	if (Ok) return TRUE;
	return FALSE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetInputs(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szInputFileNames) {
	AnsiString asProjectPath;
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;
	int i;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	asProjectPath = ExtractFilePath(szProjectFileName);
	asInputFileNames = _T("");
	for (i = 0; i < slListSrcFiles.Count; i++) {
		asInputFileNames += asProjectPath + slListSrcFiles[i];
		asInputFileNames += L'\0';
	}

	*szInputFileNames = asInputFileNames;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetInputs(const wchar_t *szInputFileNames) {

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetOutput(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szOutputFileName) {
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	asOutputFileNames = asOutputFileName;
	*szOutputFileName = asOutputFileNames;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetOutput(const wchar_t *szOutputFileName) {

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_AddFile(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szFileName) {
	AnsiString asProjectPath;
	AnsiString asFileName;
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;
	int i;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	// Suppression du chemin si c'est le même que celui du projet
	asProjectPath = ExtractFilePath(szProjectFileName);
	asFileName = szFileName;
	if (ExtractFilePath(asFileName) == asProjectPath) {
		asFileName = ExtractFileName(asFileName);
	}

	// Si le fichier est déjà dans le projet => pas de doublon
	for (i = 0; i < slListSrcFiles.Count; i++) {
		if (slListSrcFiles[i] == asFileName) {
			return TRUE;
		}
	}

	slListSrcFiles.Add(asFileName);

	if (!SaveProject(szProjectFileName, asGuid, TypeCompiler, asOutputFileName, &slListSrcFiles)) return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_RemoveFile(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szFileName) {
	AnsiString asProjectPath;
	AnsiString asFileName;
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;
	int i;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	// Suppression du chemin si c'est le même que celui du projet
	asProjectPath = ExtractFilePath(szProjectFileName);
	asFileName = szFileName;
	if (ExtractFilePath(asFileName) == asProjectPath) {
		asFileName = ExtractFileName(asFileName);
	}

	for (i = 0; i < slListSrcFiles.Count; i++) {
		if (slListSrcFiles[i] == asFileName) {
			slListSrcFiles.Delete(i);
			break;
		}
	}

	if (!SaveProject(szProjectFileName, asGuid, TypeCompiler, asOutputFileName, &slListSrcFiles)) return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_RenameFile(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szOldFileName, const wchar_t *szNewFileName) {
	AnsiString asProjectPath;
	AnsiString asOldFileName;
	AnsiString asNewFileName;
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;
	int i;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	// Suppression du chemin si c'est le même que celui du projet
	asProjectPath = ExtractFilePath(szProjectFileName);
	asOldFileName = szOldFileName;
	if (ExtractFilePath(asOldFileName) == asProjectPath) {
		asOldFileName = ExtractFileName(asOldFileName);
	}
	asNewFileName = szNewFileName;
	if (ExtractFilePath(asNewFileName) == asProjectPath) {
		asNewFileName = ExtractFileName(asNewFileName);
	}

	for (i = 0; i < slListSrcFiles.Count; i++) {
		if (slListSrcFiles[i] == asOldFileName) {
			slListSrcFiles[i] = asNewFileName;
			break;
		}
	}

	if (!SaveProject(szProjectFileName, asGuid, TypeCompiler, asOutputFileName, &slListSrcFiles)) return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_PreCompil(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, CALLBACK_DEFVAR CallbackDefVar) {
	wchar_t szMessage[MAX_PATH * 2 + 100];
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	AnsiString asProjectPath;
	AnsiString asFileName;
	AnsiString asExt;
	TStringList slListSrcFiles;
	TPreCompiler *PreCompiler;
	BOOL Ok = FALSE;
	int i;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	wsprintf(szMessage, (LPCTSTR) _TT("Pré-compilation \"%s\""), szProjectFileName);
	CallbackError(szMessage, _TT("Information"), MB_OK | MB_ICONINFORMATION);

	asProjectPath = ExtractFilePath(szProjectFileName);
	PreCompiler = new TPreCompiler(CallbackDefVar);
	for (i = 0; i < slListSrcFiles.Count; i++) {
		asFileName = asProjectPath + slListSrcFiles[i];
		asExt = ExtractFileExt(asFileName).LowerCase();
		if (asExt == _T(".c") || asExt == _T(".cpp")) {
			wsprintf(szMessage, _TT("Traitement \"%s\"").c_str(), asFileName.c_str());
			CallbackError(szMessage, _TT("Information").c_str(), MB_OK | MB_ICONINFORMATION);
			PreCompiler->PreCompile(asFileName);
		}
	}
	delete PreCompiler;

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Build(const wchar_t *szProjectFileName, const wchar_t *szProjectContent) {
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;
	wchar_t szCompilerPath[MAX_PATH];
	wchar_t szProgName[MAX_PATH];
	wchar_t szArgs[MAX_PATH];
	wchar_t szMessage[MAX_PATH * 2 + 100];
	BOOL Ok = FALSE;

	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	wsprintf(szMessage, _TT("Compilation \"%s\"").c_str(), szProjectFileName);
	CallbackError(szMessage, _TT("Information").c_str(), MB_OK | MB_ICONINFORMATION);

	if (GetCompilerPath(tcVS10, szCompilerPath, MAX_PATH)) {

		wsprintf(szProgName, _T("%s\\ISCC.exe"), szCompilerPath);
		wsprintf(szArgs, _T(" /cc \"%s\""), szProjectFileName);
		Ok = CreateProcessAndRedirectOut(szProgName, szArgs);

	}
	else {
		// Message d'erreur CompilerPath non trouvé
		CallbackError(Translate(_TT("Programme \"Compilateur\" pas ou mal installé")).c_str(),
									Translate(_TT("Construction du projet impossible")).c_str(),
									MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return Ok;
}

//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Run(const wchar_t *szProjectFileName, const wchar_t *szProjectContent) {
	AnsiString asGuid;
	TYPECOMPILER TypeCompiler;
	AnsiString asOutputFileName;
	TStringList slListSrcFiles;
	wchar_t szMessage[MAX_PATH * 2 + 100];
	wchar_t szErrMsg[128 + 100 + MAX_PATH];
	HINSTANCE hInstShell;


	if (!LoadProject(szProjectFileName, &asGuid, &TypeCompiler, &asOutputFileName, &slListSrcFiles)) return FALSE;

	wsprintf(szMessage, _TT("Lancement du programme \"%s\"").c_str(), asOutputFileName.c_str());
	CallbackError(szMessage, _TT("Information").c_str(), MB_OK | MB_ICONINFORMATION);

	hInstShell = ShellExecute(NULL, NULL, asOutputFileName.c_str(), _T(""), NULL, SW_SHOW);
	if ((int) hInstShell <= 32) {
		wsprintf(szErrMsg, L"La commande a renvoyé l'erreur: %i", hInstShell);
		if ((int) hInstShell == ERROR_FILE_NOT_FOUND) lstrcat(szErrMsg, _TT(" \"Fichier non trouvé\""));
		else if ((int) hInstShell == ERROR_PATH_NOT_FOUND) lstrcat(szErrMsg, _TT(" \"Chemin non trouvé\""));
		else if ((int) hInstShell == ERROR_BAD_FORMAT) lstrcat(szErrMsg, _TT(" \"Programme non compatible\""));
		else if ((int) hInstShell == SE_ERR_ACCESSDENIED) lstrcat(szErrMsg, _TT(" \"Accès interdit\""));
		else if ((int) hInstShell == SE_ERR_DLLNOTFOUND) lstrcat(szErrMsg, _TT(" \"Dll manquante\""));
		else if ((int) hInstShell == SE_ERR_NOASSOC) lstrcat(szErrMsg, _TT(" \"Aucune application associée à l'extension\""));
		else if ((int) hInstShell == SE_ERR_SHARE) lstrcat(szErrMsg, _TT(" \"Violation de partage\""));
		CallbackError(szErrMsg, _TT("Erreur exécution"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CreateNewProject(AnsiString asProjectPath, AnsiString asProjectName, int ProjectType, AnsiString asOutputFileName) {
	wchar_t szModelePath[MAX_PATH];
	UUID Guid;
	RPC_WSTR rastrGuid;
	TStringList slListSrcFiles;
	wchar_t szGuid[40];
	wchar_t c;
	int i;


	GetModuleFileName(HInstance, szModelePath, MAX_PATH);

	for (i = lstrlen(szModelePath) - 1; i >= 0; i--) {
		c = szModelePath[i];
		if (c == L'\\' || c == L'/' || c == L':') {
			szModelePath[i] = L'\0';
			break;
		}
	}

	lstrcat(szModelePath, _T("\\..\\Models\\Projects\\cpp"));
	if (ProjectType == 0) lstrcat(szModelePath, _T("\\Empty project"));
	else if (ProjectType == 1) lstrcat(szModelePath, _T("\\Hello world"));
	else lstrcat(szModelePath, _T("\\Editor"));

	UuidCreate(&Guid);
	UuidToString(&Guid, &rastrGuid);
	lstrcpy(szGuid, (wchar_t *) rastrGuid);

	if (CopyProject(szModelePath, asProjectPath, asProjectName, szGuid, &slListSrcFiles)) {
		asProjectFileName = asProjectPath + DIR_DELIMITER + asProjectName + _T(".fapp");
		SaveProject(asProjectFileName.c_str(), szGuid, tcUnknown, asOutputFileName, &slListSrcFiles);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CopyProject(AnsiString asSrcPath, AnsiString asDestPath, AnsiString asProjectName, AnsiString asGuid, TStringList *slListSrcFiles) {
	HANDLE hFindFile;
	WIN32_FIND_DATA FindFileData;
	wchar_t szPath[MAX_PATH];
	AnsiString asFileName;
	AnsiString asSrcFileName;
	AnsiString asDestFileName;
	TReplaceFlags ReplaceFlags;
	bool Ok;


	lstrcpy(szPath, asSrcPath.c_str());
	lstrcat(szPath, _T("\\*.*"));
	hFindFile = FindFirstFile(szPath, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE) {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 26 + MAX_PATH];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, _TT("Impossible de lister \"%s\": %s").c_str(), asSrcPath.c_str(), szBuf);
	  CallbackError(szErrMsg, _TT("Création du projet impossible").c_str(), MB_OK | MB_ICONSTOP);
		return false;
	}

	Ok = true;
	do {
		asSrcFileName = asSrcPath;
		asSrcFileName += DIR_DELIMITER;
		asSrcFileName += FindFileData.cFileName;
		asDestFileName = asDestPath;
		asDestFileName += DIR_DELIMITER;
		asFileName = FindFileData.cFileName;
		ReplaceFlags << rfReplaceAll;
		asFileName = StringReplace(asFileName, _T("{{{PROJECT_NAME}}}"), asProjectName, ReplaceFlags);
		asDestFileName += asFileName;
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			// Traitement fichier
			CopyReplaceFile(asSrcFileName, asDestFileName, asProjectName, asGuid);
			slListSrcFiles->Add(asFileName);
		}

	} while (FindNextFile(hFindFile, &FindFileData));

	FindClose(hFindFile);

	return Ok;
}

//---------------------------------------------------------------------------
BOOL CopyReplaceFile(AnsiString asSrcFileName, AnsiString asDestFileName, AnsiString asProjectName, AnsiString asGuid) {
  HANDLE hFile1, hFile2;
	DWORD dw;
	CharString csBuffer;
	AnsiString asExt;
	TReplaceFlags ReplaceFlags;


	if (FileExists(asDestFileName)) {
		AnsiString asMsg;
		asMsg = _TT("Ce fichier existe déjà, voulez-vous l'écraser ?\n") + asDestFileName;
		if (CallbackError(asMsg.c_str(),
											_TT("Enregistrement fichier").c_str(),
											MB_YESNOCANCEL | MB_ICONQUESTION) != TRUE) return FALSE;
	}

	asExt = ExtractFileExt(asSrcFileName);

	if (asExt == _T(".ico") || asExt == _T(".cur") || asExt == _T(".bmp")) {
		CopyFile(asSrcFileName.c_str(), asDestFileName.c_str(), FALSE);
	}
	else {

		hFile1 = CreateFile(asSrcFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile1 == INVALID_HANDLE_VALUE) {
			wchar_t szBuf[128];
			wchar_t szErrMsg[128 + 100 + MAX_PATH];

			// Message d'erreur ouverture asSrcFileName
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
			wsprintf(szErrMsg, _TT("Impossible d'ouvrir le fichier \"%s\": %s").c_str(), asSrcFileName.c_str(), szBuf);
			CallbackError(szErrMsg, _TT("Création du projet impossible").c_str(), MB_OK | MB_ICONWARNING);
			return FALSE;
		}

		hFile2 = CreateFile(asDestFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile2 == INVALID_HANDLE_VALUE) {
			wchar_t szBuf[128];
			wchar_t szErrMsg[128 + 100 + MAX_PATH];

			// Message d'erreur ouverture asDestFileName
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
			wsprintf(szErrMsg, _TT("Impossible d'ouvrir le fichier \"%s\": %s").c_str(), asDestFileName.c_str(), szBuf);
			CallbackError(szErrMsg, _TT("Création du projet impossible").c_str(), MB_OK | MB_ICONWARNING);

			CloseHandle(hFile1);

			return FALSE;
		}

		ReplaceFlags << rfReplaceAll;
		while (FileLineInput(hFile1, &csBuffer)) {
			csBuffer = StringReplace(csBuffer, "{{{PROJECT_NAME}}}", asProjectName.AsCharString(), ReplaceFlags);
			csBuffer = StringReplace(csBuffer, "{{{PROJECT_GUID}}}", asGuid.AsCharString(), ReplaceFlags);
			WriteFile(hFile2, csBuffer.c_str(), (int) csBuffer.Length(), &dw, NULL);
			WriteFile(hFile2, "\r\n", 2, &dw, NULL);
		}

		CloseHandle(hFile1);
		CloseHandle(hFile2);

		return TRUE;

	}

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL GetCompilerPath(TYPECOMPILER TypeCompiler, wchar_t *szCompilerPath, DWORD cbMaxCompilerPath) {
	HKEY hKey;
  LONG Error;
  DWORD dwSize;
  DWORD dwType;
	BOOL Ok = FALSE;


	Error = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE\\Plugins\\Application"), 0,
                       KEY_READ, &hKey);

  // Lecture de la clé
  if (Error == ERROR_SUCCESS) {

		switch (TypeCompiler) {
		case tcVS9:
			dwSize = cbMaxCompilerPath;
			Error = RegQueryValueEx(hKey, _T("InstallPath_VS9"), 0, &dwType, (LPBYTE) szCompilerPath, &dwSize);
			if (!Error && dwSize > 0) Ok = TRUE;
			break;
		case tcVS10:
			dwSize = cbMaxCompilerPath;
			Error = RegQueryValueEx(hKey, _T("InstallPath_VS10"), 0, &dwType, (LPBYTE) szCompilerPath, &dwSize);
			if (!Error && dwSize > 0) Ok = TRUE;
			break;
		}

    RegCloseKey(hKey);
	}

	return Ok;
}

//---------------------------------------------------------------------------
BOOL SetCompilerPath(TYPECOMPILER TypeCompiler, const wchar_t *szCompilerPath) {
	HKEY hKey;
  LONG Error;
  DWORD dwSize;
  DWORD dwDisposition;
	BOOL Ok = FALSE;

  Error = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE\\Plugins\\Application"), 0, _T(""),
                          REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hKey,
                          &dwDisposition);

  // Ecriture dans la clé
  if (Error == ERROR_SUCCESS) {

		switch (TypeCompiler) {
		case tcVS9:
			dwSize = lstrlenW(szCompilerPath) * sizeof(TCHAR);
			Error = RegSetValueEx(hKey, _T("InstallPath_VS9"), 0, REG_SZ, (LPBYTE) szCompilerPath, dwSize);
			if (!Error && dwSize > 0) Ok = TRUE;
			break;
		case tcVS10:
			dwSize = lstrlenW(szCompilerPath) * sizeof(TCHAR);
			Error = RegSetValueEx(hKey, _T("InstallPath_VS10"), 0, REG_SZ, (LPBYTE) szCompilerPath, dwSize);
			if (!Error && dwSize > 0) Ok = TRUE;
			break;
		}

    RegCloseKey(hKey);
	}

	return Ok;
}

//---------------------------------------------------------------------------
BOOL GetListCompilers(std::vector<TYPECOMPILER> *ListCompilers) {
  wchar_t szCompilerPath[MAX_PATH];


	if (GetCompilerPath(tcVS9, szCompilerPath, MAX_PATH)) ListCompilers->push_back(tcVS9);
	if (GetCompilerPath(tcVS10, szCompilerPath, MAX_PATH)) ListCompilers->push_back(tcVS10);

	return TRUE;
}

//---------------------------------------------------------------------------
AnsiString GetNameCompiler(TYPECOMPILER TypeCompiler) {
	if (TypeCompiler == tcVS9) return Translate(L"Visual Studio 2008");
	if (TypeCompiler == tcVS10) return Translate(L"Visual Studio 2010");

	return L"";
}

//---------------------------------------------------------------------------
BOOL LoadProject(const wchar_t *szProjectFileName, AnsiString *asGuid, TYPECOMPILER *TypeCompiler, AnsiString *asOutputFileName, TStringList *slListSrcFiles) {
  HANDLE hFile;
	CharString csBuffer;
	CharString csKeyWord;
	CharString csValue;
	AnsiString asProjectPath;
	AnsiString asSrcFileName;
	int Pos;


  hFile = CreateFile(szProjectFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		asProjectPath = ExtractFilePath(szProjectFileName);

		while (FileLineInput(hFile, &csBuffer)) {

			Pos = (int) csBuffer.Pos("=");
			if (Pos != 0) {

				csKeyWord = csBuffer.SubString(1, Pos - 1).Trim();
				csValue = csBuffer.SubString(Pos + 1, csBuffer.Length() - Pos).Trim();

				if (csKeyWord == "GUID") {
					*asGuid = csValue.AsAnsiString();
				}
				else if (csKeyWord == "TYPECOMP") {
					*TypeCompiler = (TYPECOMPILER) csValue.ToIntDef(tcUnknown);
				}
				else if (csKeyWord == "OUTPUT") {
					*asOutputFileName = csValue.AsAnsiString();
				}
				else if (csKeyWord == "SRC") {
					asSrcFileName = csValue.AsAnsiString();
					slListSrcFiles->Add(asSrcFileName);
				}
			}

		}

    CloseHandle(hFile);
		return TRUE;
  }
  else {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szProjectFileName
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, _TT("Impossible d'ouvrir le fichier \"%s\": %s").c_str(), szProjectFileName, szBuf);
	  CallbackError(szErrMsg, _TT("Chargement du projet impossible").c_str(), MB_OK | MB_ICONWARNING);
		return FALSE;
  }

}

//---------------------------------------------------------------------------
BOOL SaveProject(const wchar_t *szProjectFileName, AnsiString asGuid, TYPECOMPILER tc, AnsiString asOutputFileName, TStringList *slListSrcFiles) {
	AnsiString asTypeCompiler;
  HANDLE hFile;
	DWORD dw;
	int i;


  hFile = CreateFile(szProjectFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		WriteFile(hFile, "GUID=", 5 * sizeof(char), &dw, NULL);
		WriteFile(hFile, asGuid.AsCharString().c_str(), (DWORD) asGuid.length() * sizeof(char), &dw, NULL);
		WriteFile(hFile, "\r\n", 2 * sizeof(char), &dw, NULL);

		WriteFile(hFile, "TYPECOMP=", 9 * sizeof(char), &dw, NULL);
		asTypeCompiler.sprintf(_T("%i"), tc);
		WriteFile(hFile, asTypeCompiler.AsCharString().c_str(), (DWORD) asTypeCompiler.length() * sizeof(char), &dw, NULL);
		WriteFile(hFile, "\r\n", 2 * sizeof(char), &dw, NULL);

		WriteFile(hFile, "OUTPUT=", 7 * sizeof(char), &dw, NULL);
		WriteFile(hFile, asOutputFileName.AsCharString().c_str(), (DWORD) asOutputFileName.length() * sizeof(char), &dw, NULL);
		WriteFile(hFile, "\r\n", 2 * sizeof(char), &dw, NULL);

		for (i = 0; i < slListSrcFiles->Count; i++) {

			WriteFile(hFile, "SRC=", 4 * sizeof(char), &dw, NULL);
			WriteFile(hFile, slListSrcFiles->Strings[i].AsCharString().c_str(), slListSrcFiles->Strings[i].Length() * sizeof(char), &dw, NULL);
			WriteFile(hFile, "\r\n", 2 * sizeof(char), &dw, NULL);

		}

		CloseHandle(hFile);
		return TRUE;
  }
  else {
		wchar_t szBuf[128];
		wchar_t szErrMsg[128 + 100 + MAX_PATH];

    // Message d'erreur ouverture szProjectFileName
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, szBuf, 128, NULL);
		wsprintf(szErrMsg, _TT("Impossible d'enregistrer le fichier \"%s\": %s").c_str(), szProjectFileName, szBuf);
	  CallbackError(szErrMsg, _TT("Enregistrement du projet impossible").c_str(), MB_OK | MB_ICONWARNING);
		return FALSE;
  }

}

//---------------------------------------------------------------------------
