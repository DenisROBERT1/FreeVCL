//-----------------------------------------------------------------------------
//! @file TProject.cpp
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


//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------
#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------
#include "TFVCLIDEAutomation.h"
#include "TTypeProject.h"
#include "TListTypeProjects.h"
#include "TProject.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
TProject::TProject(TListTypeProjects *AListTypeProjects) {
	
	// Initialisations
	ListTypeProjects = AListTypeProjects;
	FIndexTypeProject = -3;
	bIsPreCompiled = false;

}

//---------------------------------------------------------------------------
TProject::~TProject(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Built
//---------------------------------------------------------------------------

TDateTime TProject::Get_Built(void) {
	return FBuilt;
}

bool TProject::Set_Built(TDateTime NewBuilt) {
	if (FBuilt != NewBuilt) {
		FBuilt = NewBuilt;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Content
//---------------------------------------------------------------------------

AnsiString TProject::Get_Content(void) {
	return FContent;
}

bool TProject::Set_Content(AnsiString NewContent) {
	if (FContent != NewContent) {
		FContent = NewContent;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FileName
//---------------------------------------------------------------------------

AnsiString TProject::Get_FileName(void) {
	return FFileName;
}

bool TProject::Set_FileName(AnsiString NewFileName) {
	if (FFileName != NewFileName) {
		FFileName = NewFileName;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété IndexTypeProject
//---------------------------------------------------------------------------

int TProject::Get_IndexTypeProject(void) {
	return FIndexTypeProject;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LastChange
//---------------------------------------------------------------------------

TDateTime TProject::Get_LastChange(void) {
	return FLastChange;
}

bool TProject::Set_LastChange(TDateTime NewLastChange) {
	if (FLastChange != NewLastChange) {
		FLastChange = NewLastChange;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TProject::Get_Name(void) {
	return FName;
}

bool TProject::Set_Name(AnsiString NewName) {
	if (FName != NewName) {
		FName = NewName;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Output
//---------------------------------------------------------------------------

AnsiString TProject::Get_Output(void) {
	return FOutput;
}

bool TProject::Set_Output(AnsiString NewOutput) {
	if (FOutput != NewOutput) {
		FOutput = NewOutput;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TypeProject
//---------------------------------------------------------------------------

AnsiString TProject::Get_TypeProject(void) {
	return FTypeProject;
}

bool TProject::Set_TypeProject(AnsiString NewTypeProject) {
	if (FTypeProject != NewTypeProject) {
		FTypeProject = NewTypeProject;
		if (FTypeProject == _T("File")) {
			FIndexTypeProject = -3;
		}
		else if (FTypeProject == _T("Files")) {
			FIndexTypeProject = -2;
		}
		else if (FTypeProject == _T("Directory")) {
			FIndexTypeProject = -1;
		}
		else {
			FIndexTypeProject = ListTypeProjects->LoadTypeProject(FTypeProject);
		}
	}
	return true;
}

//---------------------------------------------------------------------------
TBitmap *TProject::GetBitmap20(void) {
	TBitmap *Bitmap = new TBitmap();

	if (FIndexTypeProject == -3) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILE_20"));
	}
	else if (FIndexTypeProject == -2) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILES_20"));
	}
	else if (FIndexTypeProject == -1) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_DIRECTORY_20"));
	}
	else {
		Bitmap->Width = 20;
		Bitmap->Height = 20;
		Bitmap->Handle = ListTypeProjects->TypeProject[FIndexTypeProject]->GetBitmap20();
	}

	return Bitmap;
}

//---------------------------------------------------------------------------
TBitmap *TProject::GetBitmap32(void) {
	TBitmap *Bitmap = new TBitmap();

	if (FIndexTypeProject == -3) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILE_32"));
	}
	else if (FIndexTypeProject == -2) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILES_32"));
	}
	else if (FIndexTypeProject == -1) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_DIRECTORY_32"));
	}
	else {
		Bitmap->Width = 32;
		Bitmap->Height = 32;
		Bitmap->Handle = ListTypeProjects->TypeProject[FIndexTypeProject]->GetBitmap32();
	}

	return Bitmap;
}

//---------------------------------------------------------------------------
TBitmap *TProject::GetBitmap48(void) {
	TBitmap *Bitmap = new TBitmap();

	if (FIndexTypeProject == -3) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILE_48"));
	}
	else if (FIndexTypeProject == -2) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_TEXTFILES_48"));
	}
	else if (FIndexTypeProject == -1) {
		Bitmap->LoadFromResource(HInstance, _T("BMP_DIRECTORY_48"));
	}
	else {
		Bitmap->Width = 48;
		Bitmap->Height = 48;
		Bitmap->Handle = ListTypeProjects->TypeProject[FIndexTypeProject]->GetBitmap48();
	}

	return Bitmap;
}

//---------------------------------------------------------------------------
AnsiString TProject::GetDescription(void) {
	if (FIndexTypeProject == -3) {
		return _TT("Fichier");
	}
	else if (FIndexTypeProject == -2) {
		return _TT("Liste de fichiers");
	}
	else if (FIndexTypeProject == -1) {
		return _TT("Répertoire");
	}
	else {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->GetDescription();
	}
}

//---------------------------------------------------------------------------
bool TProject::Create(void) {
	AnsiString asProjectFileName;
	AnsiString asProjectContent;
	bool Ok = false;


	if (FIndexTypeProject >= 0) {
		Ok = ListTypeProjects->TypeProject[FIndexTypeProject]->Create(&asProjectFileName, &asProjectContent);
		if (Ok) {
			TDateTime dtLastChange;
			dtLastChange.CurrentDateTime(false);
			FLastChange = dtLastChange;
			FFileName = asProjectFileName;
			FContent = asProjectContent;
		}
	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TProject::Open(void) {
	AnsiString asProjectFileName;
	AnsiString asProjectContent;
	bool Ok = false;
	int Rep;


	if (FIndexTypeProject >= 0) {
		asProjectFileName = FFileName;
		asProjectContent = FContent;

		if (!asProjectFileName.IsEmpty() && asProjectContent.IsEmpty()) {
			// Test de l'existence du fichier
			if (!FileExists(asProjectFileName)) {
				AnsiString asMsg;
				asMsg.sprintf(
						_TT("Le fichier \"%s\" a été supprimé ou déplacé.\nVoulez-vous le rechercher ?"),
						(LPCTSTR) asProjectFileName);
				Rep = FVCLIDEAutomation->DisplayError(asMsg,
										 _TT("Ouverture projet"),
										 _T(""),
										 MB_YESNO | MB_ICONINFORMATION);
				if (Rep == IDYES) {
					TOpenDialog *OpenDialog = new TOpenDialog(Application->MainForm);
					OpenDialog->Caption = _T("Nom du fichier projet");
					OpenDialog->FileName = asProjectFileName;
					if (OpenDialog->Execute()) {
						asProjectFileName = OpenDialog->FileName;
					}
					delete OpenDialog;
				}
				else if (Rep == IDNO) {
					asMsg.sprintf(_TT("Voulez-vous recréer le projet %s ?"), (LPCTSTR) asProjectFileName);
					Rep = FVCLIDEAutomation->DisplayError(asMsg,
											 _TT("Ouverture projet"),
											 _T(""),
											 MB_YESNO | MB_ICONINFORMATION);
					if (Rep == IDYES) {
						asProjectFileName = _T("");
					}
				}

			}
		}

		if (!asProjectFileName.IsEmpty() || !asProjectContent.IsEmpty()) {
			Ok = ListTypeProjects->TypeProject[FIndexTypeProject]->Open(&asProjectFileName, &asProjectContent);
		}
		else {
			Ok = ListTypeProjects->TypeProject[FIndexTypeProject]->Create(&asProjectFileName, &asProjectContent);
		}
		if (Ok) {
			TDateTime dtLastChange;
			dtLastChange.CurrentDateTime(false);
			FLastChange = dtLastChange;
			FFileName = asProjectFileName;
			FContent = asProjectContent;
		}
	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TProject::GetInputs(TStringList *asInputFileNames) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->GetInputs(FFileName, FContent, asInputFileNames);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::GetOutput(AnsiString *asOutputFileName) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->GetOutput(FFileName, FContent, asOutputFileName);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::Can_AddFile(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Can_AddFile();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::AddFile(AnsiString asFileName) {
	bool Ok = false;


	if (FIndexTypeProject >= 0) {
		Ok = ListTypeProjects->TypeProject[FIndexTypeProject]->AddFile(FFileName, FContent, asFileName);
		if (Ok) {
			TDateTime dtLastChange;
			dtLastChange.CurrentDateTime(false);
			FLastChange = dtLastChange;
		}
	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TProject::Can_RemoveFile(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Can_RemoveFile();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::RemoveFile(AnsiString asFileName) {
	bool Ok = false;


	if (FIndexTypeProject >= 0) {
		Ok = ListTypeProjects->TypeProject[FIndexTypeProject]->RemoveFile(FFileName, FContent, asFileName);
		if (Ok) {
			TDateTime dtLastChange;
			dtLastChange.CurrentDateTime(false);
			FLastChange = dtLastChange;
		}
	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TProject::Can_RenameFile(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Can_RenameFile();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::RenameFile(AnsiString asOldFileName, AnsiString asNewFileName) {
	bool Ok = false;


	if (FIndexTypeProject >= 0) {
		Ok = ListTypeProjects->TypeProject[FIndexTypeProject]->RenameFile(FFileName, FContent, asOldFileName, asNewFileName);
		if (Ok) {
			TDateTime dtLastChange;
			dtLastChange.CurrentDateTime(false);
			FLastChange = dtLastChange;
		}
	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TProject::Build(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Build(FFileName, FContent);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::Can_Run(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Can_Run();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::Run(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Run(FFileName, FContent);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::Can_Debug(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Can_Debug();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::StartDebug(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->StartDebug(FFileName, FContent);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::StopDebug(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->StopDebug();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::StepByStep(TYPESTEPBYSTEP TypeStepByStep, AnsiString *asFileName, int *NumLine1, int *NumCol1, int *NumLine2, int *NumCol2) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->StepByStep(TypeStepByStep, asFileName, NumLine1, NumCol1, NumLine2, NumCol2);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::AddBreakpoint(AnsiString asFileName, int NumLine, int NumCol) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->AddBreakpoint(asFileName, NumLine, NumCol);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::DelBreakpoint(AnsiString asFileName, int NumLine, int NumCol) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->DelBreakpoint(asFileName, NumLine, NumCol);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::ClearBreakpoints(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->ClearBreakpoints();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::GetCallStack(AnsiString *asCallStack) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->GetCallStack(asCallStack);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::GetVariableValue(AnsiString asVarName, AnsiString *asVarValue) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->GetVariableValue(asVarName, asVarValue);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::Can_PreCompil(void) {
	if (FIndexTypeProject >= 0) {
		return ListTypeProjects->TypeProject[FIndexTypeProject]->Can_PreCompil();
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
bool TProject::IsPreCompiled(void) {
	return bIsPreCompiled;
}

//---------------------------------------------------------------------------
bool TProject::PreCompilation(void) {
	ListPrograms.Clear();
	ListVariables.clear();
	bool Ok;


	if (FIndexTypeProject >= 0) {
		Ok = ListTypeProjects->TypeProject[FIndexTypeProject]->PreCompil(FFileName, FContent);
		bIsPreCompiled = true;
	}
	else {
		Ok = false;
		bIsPreCompiled = false;
	}

	return Ok;
}

//---------------------------------------------------------------------------
bool TProject::AddVariableDefinition(AnsiString asVarName, int TypeDefinition, AnsiString asDescription, AnsiString asTypeContenant, AnsiString asTypeVariable, AnsiString asFileName, int NumLine, int NumCol) {
	DEF_VARIABLE VariableDefinition;
	int IndexProgram;
	int i;


	// Recherche IndexProgram
	IndexProgram = -1;
	for (i = 0; i < (int) ListPrograms.Count; i++) {
		if (ListPrograms.Strings[i] == asFileName) {
			IndexProgram = i;
			break;
		}
	}
	if (IndexProgram == -1) {
		IndexProgram = ListPrograms.Count;
		ListPrograms.Add(asFileName);
	}

	// Recherche doublons
	for (i = 0; i < (int) ListVariables.size(); i++) {
		if (ListVariables[i].asVarName == asVarName &&
				ListVariables[i].TypeDefinition == TypeDefinition) {
			return true;
		}
	}

	VariableDefinition.asVarName = asVarName;
	VariableDefinition.TypeDefinition = TypeDefinition;
	VariableDefinition.asDescription = asDescription;
	VariableDefinition.asTypeContenant = asTypeContenant;
	VariableDefinition.asTypeVariable = asTypeVariable;
	VariableDefinition.IndexProgram = IndexProgram;
	VariableDefinition.NumLine = NumLine;
	VariableDefinition.NumCol = NumCol;
	ListVariables.push_back(VariableDefinition);

	return true;
}

//---------------------------------------------------------------------------
bool TProject::GetVariableDefinition(AnsiString asVarName, int TypeDefinition, AnsiString *asFileName, int *NumLine, int *NumCol) {
	int i;


	for (i = 0; i < (int) ListVariables.size(); i++) {
		if (ListVariables[i].asVarName == asVarName &&
				ListVariables[i].TypeDefinition == TypeDefinition) {
			*asFileName = ListPrograms.Strings[ListVariables[i].IndexProgram];
			*NumLine = ListVariables[i].NumLine;
			*NumCol = ListVariables[i].NumCol;
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TProject::GetVariableDescription(AnsiString asVarName, AnsiString *asDescription) {
	int i;


	for (i = 0; i < (int) ListVariables.size(); i++) {
		if (ListVariables[i].asVarName == asVarName) {
			*asDescription = ListVariables[i].asDescription;
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TProject::GetTypeVariable(AnsiString asVarName, AnsiString *asTypeVariable) {
	int i;


	for (i = 0; i < (int) ListVariables.size(); i++) {
		if (ListVariables[i].asVarName == asVarName) {
			*asTypeVariable = ListVariables[i].asTypeVariable;
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TProject::GetListAutocompletion(AnsiString asTypeContenant, TStrings *slList) {
	int i;


	for (i = 0; i < (int) ListVariables.size(); i++) {
		if (ListVariables[i].TypeDefinition == 1 &&
				ListVariables[i].asTypeContenant == asTypeContenant) {
			slList->Add(ListVariables[i].asVarName);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
