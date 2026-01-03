//-----------------------------------------------------------------------------
//! @file TTypeProject.cpp
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
#include "TTypeProject.h"
#include "TFVCLIDEAutomation.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
bool bInBuild;
bool bRunning;


//---------------------------------------------------------------------------
// Fonctions globales
//---------------------------------------------------------------------------

BOOL WINAPI CallbackError(const wchar_t *wszMessage, const wchar_t *wszTitle, int Flags) {
	int Ret;


	if (bInBuild) {
		FVCLIDEAutomation->AddLogMessage(tl_Build, wszMessage);
	}
	else if (bRunning) {
		FVCLIDEAutomation->AddLogMessage(tl_Process, wszMessage);
	}
	else {
		Ret = FVCLIDEAutomation->DisplayError(wszMessage, wszTitle, "", Flags);
		if ((Flags & MB_TYPEMASK) == MB_YESNO || (Flags & MB_TYPEMASK) == MB_YESNOCANCEL) {
			if (Ret != mrYes) return FALSE;
		}
		else {
			if (Ret != mrOk) return FALSE;
		}
	}

	return TRUE;
}

BOOL WINAPI CallbackDefVar(const wchar_t *wszVarName, int TypeDefinition, const wchar_t *wszDescription, const wchar_t *wszTypeContenant, const wchar_t *wszTypeVariable, const wchar_t *wszFilename, int NumLine, int NumCol) {
	return FVCLIDEAutomation->AddVariableDefinition(wszVarName, TypeDefinition, wszDescription, wszTypeContenant, wszTypeVariable, wszFilename, NumLine, NumCol);
}


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TTypeProject::TTypeProject(AnsiString asName) {
	FName = asName;
	AnsiString asLibName;


	asLibName = (AnsiString) Config->InstallPath + _T("\\Plugins\\") + FName + _T(".dll");
	hDll = LoadLibrary(asLibName);
	if (hDll) {
		Plugin_Init = (PLUGIN_INIT) (void *) GetProcAddress(hDll, "Plugin_Init");
		Plugin_Install = (PLUGIN_INSTALL) (void *) GetProcAddress(hDll, "Plugin_Install");
		Plugin_GetBitmap20 = (PLUGIN_GETBITMAP20) (void *) GetProcAddress(hDll, "Plugin_GetBitmap20");
		Plugin_GetBitmap32 = (PLUGIN_GETBITMAP32) (void *) GetProcAddress(hDll, "Plugin_GetBitmap32");
		Plugin_GetBitmap48 = (PLUGIN_GETBITMAP48) (void *) GetProcAddress(hDll, "Plugin_GetBitmap48");
		Plugin_GetDescription = (PLUGIN_GETDESCRIPTION) (void *) GetProcAddress(hDll, "Plugin_GetDescription");
		Plugin_Create = (PLUGIN_CREATE) (void *) GetProcAddress(hDll, "Plugin_Create");
		Plugin_Open = (PLUGIN_OPEN) (void *) GetProcAddress(hDll, "Plugin_Open");
		Plugin_GetInputs = (PLUGIN_GETINPUTS) (void *) GetProcAddress(hDll, "Plugin_GetInputs");
		Plugin_Free_GetInputs = (PLUGIN_FREE_GETINPUTS) (void *) GetProcAddress(hDll, "Plugin_Free_GetInputs");
		Plugin_GetOutput = (PLUGIN_GETOUTPUT) (void *) GetProcAddress(hDll, "Plugin_GetOutput");
		Plugin_Free_GetOutput = (PLUGIN_FREE_GETOUTPUT) (void *) GetProcAddress(hDll, "Plugin_Free_GetOutput");
		Plugin_AddFile = (PLUGIN_ADDFILE) (void *) GetProcAddress(hDll, "Plugin_AddFile");
		Plugin_RemoveFile = (PLUGIN_REMOVEFILE) (void *) GetProcAddress(hDll, "Plugin_RemoveFile");
		Plugin_RenameFile = (PLUGIN_RENAMEFILE) (void *) GetProcAddress(hDll, "Plugin_RenameFile");
		Plugin_PreCompil = (PLUGIN_PRECOMPIL) (void *) GetProcAddress(hDll, "Plugin_PreCompil");
		Plugin_Build = (PLUGIN_BUILD) (void *) GetProcAddress(hDll, "Plugin_Build");
		Plugin_Run = (PLUGIN_RUN) (void *) GetProcAddress(hDll, "Plugin_Run");
		Plugin_StartDebug = (PLUGIN_STARTDEBUG) (void *) GetProcAddress(hDll, "Plugin_StartDebug");
		Plugin_StopDebug = (PLUGIN_STOPDEBUG) (void *) GetProcAddress(hDll, "Plugin_StopDebug");
		Plugin_StepByStep = (PLUGIN_STEPBYSTEP) (void *) GetProcAddress(hDll, "Plugin_StepByStep");
		Plugin_AddBreakpoint = (PLUGIN_ADDBREAKPOINT) (void *) GetProcAddress(hDll, "Plugin_AddBreakpoint");
		Plugin_DelBreakpoint = (PLUGIN_DELBREAKPOINT) (void *) GetProcAddress(hDll, "Plugin_DelBreakpoint");
		Plugin_ClearBreakpoints = (PLUGIN_CLEARBREAKPOINTS) (void *) GetProcAddress(hDll, "Plugin_ClearBreakpoints");
		Plugin_GetCallStack = (PLUGIN_GETCALLSTACK) (void *) GetProcAddress(hDll, "Plugin_GetCallStack");
		Plugin_GetVariableValue = (PLUGIN_GETVARIABLEVALUE) (void *) GetProcAddress(hDll, "Plugin_GetVariableValue");
		if (Plugin_Init == NULL ||
				Plugin_Install == NULL ||
				Plugin_GetBitmap20 == NULL ||
				Plugin_GetBitmap32 == NULL ||
				Plugin_GetBitmap48 == NULL ||
				Plugin_GetDescription == NULL ||
				Plugin_Create == NULL ||
				Plugin_Open == NULL ||
				Plugin_GetInputs == NULL ||
				Plugin_Free_GetInputs == NULL ||
				Plugin_GetOutput == NULL ||
				Plugin_Free_GetOutput == NULL ||
				Plugin_Build == NULL) {

			AnsiString asErrMessage;
			asErrMessage.sprintf(_TT("Impossible d'ouvrir le plugin %s:\nFonction non trouvée."), (LPCTSTR) asLibName);
			FVCLIDEAutomation->DisplayError(asErrMessage, _T("Erreur de plugin"), _T(""), MB_OK | MB_ICONWARNING);

			FreeLibrary(hDll);
			hDll = NULL;
		}
		else {
			if (!Plugin_Init(CallbackError)) {

				AnsiString asErrMessage;
				asErrMessage.sprintf(_TT("Impossible d'ouvrir le plugin %s:\nInit a renvoyé faux."), (LPCTSTR) asLibName);
				FVCLIDEAutomation->DisplayError(asErrMessage, _TT("Erreur de plugin"), _T(""), MB_OK | MB_ICONWARNING);

				hDll = NULL;
			}
		}
	}
	else {
		int LastError = GetLastError();
		AnsiString asErrMessage;
		asErrMessage.sprintf(_TT("Impossible d'ouvrir le plugin %s"), (LPCTSTR) asLibName);
		FVCLIDEAutomation->DisplaySystemError(asErrMessage, LastError, _TT("Erreur de plugin"), _T(""), MB_OK | MB_ICONWARNING);
	}

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TTypeProject::~TTypeProject(void) {
	if (hDll) FreeLibrary(hDll);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TTypeProject::Get_Name(void) {
  return FName;
}

//---------------------------------------------------------------------------
bool TTypeProject::Install(void) {

	if (!hDll) return false;

	return (Plugin_Install() == TRUE);

}

//---------------------------------------------------------------------------
HBITMAP TTypeProject::GetBitmap20(void) {
	HBITMAP hBitmap = NULL;

	if (hDll) hBitmap = Plugin_GetBitmap20();

	if (hBitmap == NULL) {
		hBitmap = (HBITMAP) LoadImage(HInstance, _T("BMP_PLUGINUNKNOWN"), IMAGE_BITMAP, 20, 20, 0);
	}

	return hBitmap;
}

//---------------------------------------------------------------------------
HBITMAP TTypeProject::GetBitmap32(void) {
	HBITMAP hBitmap = NULL;

	if (hDll) hBitmap = Plugin_GetBitmap32();

	if (hBitmap == NULL) {
		hBitmap = (HBITMAP) LoadImage(HInstance, _T("BMP_PLUGINUNKNOWN"), IMAGE_BITMAP, 32, 32, 0);
	}

	return hBitmap;
}

//---------------------------------------------------------------------------
HBITMAP TTypeProject::GetBitmap48(void) {
	HBITMAP hBitmap = NULL;

	if (hDll) hBitmap = Plugin_GetBitmap48();

	if (hBitmap == NULL) {
		hBitmap = (HBITMAP) LoadImage(HInstance, _T("BMP_PLUGINUNKNOWN"), IMAGE_BITMAP, 48, 48, 0);
	}

	return hBitmap;
}

//---------------------------------------------------------------------------
AnsiString TTypeProject::GetDescription(void) {
	wchar_t wszDescription[256];

	if (!hDll) return _T("");

	wszDescription[0] = L'\0';
	Plugin_GetDescription(wszDescription, sizeof(wszDescription) / sizeof(wchar_t));

	return wszDescription;
}

//---------------------------------------------------------------------------
bool TTypeProject::Create(AnsiString *asProjectFileName, AnsiString *asProjectContent) {

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	wchar_t szProjectFileName[MAX_PATH];
	wchar_t szProjectContent[2048];
	memset(szProjectFileName, 0, sizeof(szProjectFileName));
	memset(szProjectContent, 0, sizeof(szProjectContent));
	if (Plugin_Create(szProjectFileName, szProjectContent, Application->Handle)) {
		*asProjectFileName = szProjectFileName;
		*asProjectContent = szProjectContent;

		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool TTypeProject::Open(AnsiString *asProjectFileName, AnsiString *asProjectContent) {

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	wchar_t szProjectFileName[MAX_PATH];
	wchar_t szProjectContent[2048];
	lstrcpynW(szProjectFileName, asProjectFileName->AsWideString(), MAX_PATH);
	lstrcpynW(szProjectContent, asProjectContent->AsWideString(), 2048);
	if (Plugin_Open(szProjectFileName, szProjectContent, Application->Handle)) {
		*asProjectFileName = szProjectFileName;
		*asProjectContent = szProjectContent;

		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool TTypeProject::GetInputs(AnsiString asProjectFileName, AnsiString asProjectContent, TStringList *asInputFileNames) {
	const wchar_t *szInputFileNames = NULL;
	int i;
	bool Ok = false;

	if (Plugin_GetInputs(asProjectFileName.AsWideString(),
											 asProjectContent.AsWideString(),
											 &szInputFileNames)) {

		if (szInputFileNames) {
			i = 0;
			while (szInputFileNames[i]) {
				asInputFileNames->Add(&szInputFileNames[i]);
				i += lstrlenW(&szInputFileNames[i]) + 1;
			}
			Plugin_Free_GetInputs(szInputFileNames);
			Ok = true;
		}

	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TTypeProject::GetOutput(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString *asOutputFileName) {
	const wchar_t *szOutputFileName = NULL;
	bool Ok = false;

	if (Plugin_GetOutput(asProjectFileName.AsWideString(),
											 asProjectContent.AsWideString(),
											 &szOutputFileName)) {

		if (szOutputFileName) {
			if (szOutputFileName[0]) *asOutputFileName = szOutputFileName;
			Plugin_Free_GetOutput(szOutputFileName);
			Ok = true;
		}

	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TTypeProject::Can_AddFile(void) {
	return (Plugin_AddFile != NULL);
}

//---------------------------------------------------------------------------
bool TTypeProject::AddFile(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString asFileName) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_AddFile != NULL) {
		bRunning = true;
		Ok = Plugin_AddFile(asProjectFileName.AsWideString(),
												asProjectContent.AsWideString(),
												asFileName.AsWideString());
		bRunning = false;
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Impossible d'ajouter des fichiers à ce projet."),
			_TT("Ajout impossible"),
			_T("CANT_ADD"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::Can_RemoveFile(void) {
	return (Plugin_RemoveFile != NULL);
}

//---------------------------------------------------------------------------
bool TTypeProject::RemoveFile(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString asFileName) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_RemoveFile != NULL) {
		bRunning = true;
		Ok = Plugin_RemoveFile(asProjectFileName.AsWideString(),
													 asProjectContent.AsWideString(),
													 asFileName.AsWideString());
		bRunning = false;
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Impossible de supprimer des fichiers de ce projet."),
			_TT("Suppression impossible"),
			_T("CANT_REMOVE"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::Can_RenameFile(void) {
	return (Plugin_RenameFile != NULL);
}

//---------------------------------------------------------------------------
bool TTypeProject::RenameFile(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString asOldFileName, AnsiString asNewFileName) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_AddFile != NULL) {
		bRunning = true;
		Ok = Plugin_RenameFile(asProjectFileName.AsWideString(),
												asProjectContent.AsWideString(),
												asOldFileName.AsWideString(),
												asNewFileName.AsWideString());
		bRunning = false;
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Impossible de renommer des fichiers dans ce projet."),
			_TT("Renommage impossible"),
			_T("CANT_RENAME"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::Can_PreCompil(void) {
	return (Plugin_PreCompil != NULL);
}

//---------------------------------------------------------------------------
bool TTypeProject::PreCompil(AnsiString asProjectFileName, AnsiString asProjectContent) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_PreCompil != NULL) {
		bInBuild = true;
		Ok = Plugin_PreCompil(asProjectFileName.AsWideString(),
													asProjectContent.AsWideString(),
													CallbackDefVar);
		bInBuild = false;
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Impossible de pré-compiler ce type de projet."),
			_TT("Pré-compilation impossible"),
			_T("CANT_PRECOMPIL"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::Build(AnsiString asProjectFileName, AnsiString asProjectContent) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	bInBuild = true;
	Ok = Plugin_Build(asProjectFileName.AsWideString(),
									  asProjectContent.AsWideString());
	bInBuild = false;

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::Can_Run(void) {
	return (Plugin_Run != NULL);
}

//---------------------------------------------------------------------------
bool TTypeProject::Run(AnsiString asProjectFileName, AnsiString asProjectContent) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_Run != NULL) {
		bRunning = true;
		Ok = Plugin_Run(asProjectFileName.AsWideString(),
										asProjectContent.AsWideString());
		bRunning = false;
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Ce projet ne peut être lancé directement. Veuillez lancer un des sous-projet."),
			_TT("Lancement impossible"),
			_T("CANT_RUN"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::Can_Debug(void) {
	return (Plugin_StartDebug != NULL);
}

//---------------------------------------------------------------------------
bool TTypeProject::StartDebug(AnsiString asProjectFileName, AnsiString asProjectContent) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_StartDebug != NULL) {
		bRunning = true;
		Ok = Plugin_StartDebug(asProjectFileName.AsWideString(),
													 asProjectContent.AsWideString());
		bRunning = false;
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Ce projet ne peut être débuggé. Veuillez lancer un des sous-projet."),
			_TT("Débug impossible"),
			_T("CANT_DEBUG"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}


//---------------------------------------------------------------------------
bool TTypeProject::StopDebug(void) {

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_StopDebug != NULL) Plugin_StopDebug();

	return true;
}

//---------------------------------------------------------------------------
bool TTypeProject::StepByStep(TYPESTEPBYSTEP TypeStepByStep, AnsiString *asFileName, int *NumLine1, int *NumCol1, int *NumLine2, int *NumCol2) {
	BOOL Ok;
	const wchar_t *szFileName;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_StepByStep != NULL) {
		bRunning = true;
		Ok = Plugin_StepByStep(TypeStepByStep, &szFileName, NumLine1, NumCol1, NumLine2, NumCol2);
		*asFileName = szFileName;
		bRunning = false;
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Ce projet ne peut être débuggé. Veuillez lancer un des sous-projet."),
			_TT("Débug impossible"),
			_T("CANT_DEBUG"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::AddBreakpoint(AnsiString asFileName, int NumLine, int NumCol) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_AddBreakpoint != NULL) {
		Ok = Plugin_AddBreakpoint(asFileName.AsWideString(), NumLine, NumCol);
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Ce projet ne permet pas de positionner un point d'arrêt."),
			_TT("Point d'arrêt"),
			_T("ERROR_BREAKPOINT"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::DelBreakpoint(AnsiString asFileName, int NumLine, int NumCol) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_DelBreakpoint != NULL) {
		Ok = Plugin_DelBreakpoint(asFileName.AsWideString(), NumLine, NumCol);
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Ce projet ne permet pas de positionner un point d'arrêt."),
			_TT("Point d'arrêt"),
			_T("ERROR_BREAKPOINT"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::ClearBreakpoints(void) {
	BOOL Ok;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_ClearBreakpoints != NULL) {
		Ok = Plugin_ClearBreakpoints();
	}
	else {
		FVCLIDEAutomation->DisplayError(
			_TT("Ce projet ne permet pas de positionner un point d'arrêt."),
			_TT("Point d'arrêt"),
			_T("ERROR_BREAKPOINT"),
			MB_OK | MB_ICONWARNING);
		Ok = FALSE;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::GetCallStack(AnsiString *asCallStack) {
	BOOL Ok = FALSE;
	const wchar_t *szFileName;

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_GetCallStack != NULL) {
		bRunning = true;
		Ok = Plugin_GetCallStack(&szFileName);
		*asCallStack = szFileName;
		bRunning = false;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
bool TTypeProject::GetVariableValue(AnsiString asVarName, AnsiString *asVarValue) {
	BOOL Ok = FALSE;
	wchar_t szVarValue[128];

	if (!hDll) {
		FVCLIDEAutomation->DisplayError(
			_TT("Le plugin n'a pas pu être chargé."),
			_TT("Handle de DLL null"),
			_T(""),
			MB_OK | MB_ICONWARNING);
		return false;
	}

	if (Plugin_GetVariableValue != NULL) {
		bRunning = true;
		Ok = Plugin_GetVariableValue(asVarName.AsWideString(), szVarValue, sizeof(szVarValue) / sizeof(TCHAR));
		if (Ok) *asVarValue = szVarValue;
		bRunning = false;
	}

	return (Ok == TRUE);
}

//---------------------------------------------------------------------------
