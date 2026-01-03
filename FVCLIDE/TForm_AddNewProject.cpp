//-----------------------------------------------------------------------------
//! @file TForm_AddNewProject.cpp
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
#include "TForm_AddNewProject.h"
#include "TFVCLIDEAutomation.h"
#include "TTypeProject.h"
#include "HLP_FVCLIDE_Fr.h"

//---------------------------------------------------------------------------
TForm_AddNewProject::TForm_AddNewProject(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {
	AnsiString asPluginsPath;
	AnsiString asPluginsName;
	TTypeProject *TypeProject;
	int i, PosPoint;

	
	TStringList *ListPlugins = new TStringList();

	asPluginsPath = (AnsiString) Config->InstallPath + _T("\\Plugins");
	TFileList *FileList = new TFileList(this);
	FileList->Recursive = false;
	FileList->Filter = _T("*.dll");
	FileList->GetFileList(asPluginsPath, false, ListPlugins);
	delete FileList;


	for (i = 0; i < ListPlugins->Count; i++) {
		asPluginsName = ListPlugins->Strings[i];
		PosPoint = asPluginsName.LastDelimiter(_T("."));
		if (PosPoint != 0) {
			asPluginsName = asPluginsName.SubString(1, PosPoint - 1);
		}
		TypeProject = new TTypeProject(asPluginsName);
		if (TypeProject) {
			ListBox_Types->Items->Add(TypeProject->GetDescription());
			ListProjects.push_back(TypeProject);
		}
	}

	delete ListPlugins;
}

//---------------------------------------------------------------------------
TForm_AddNewProject::~TForm_AddNewProject(void) {
	int i;


	for (i = 0; i < (int) ListProjects.size(); i++) {
		delete ListProjects[i];
	}

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TypeProject
//---------------------------------------------------------------------------

AnsiString TForm_AddNewProject::Get_TypeProject(void) {
	int ItemIndex;
	
	ItemIndex = ListBox_Types->ItemIndex;
	if (ItemIndex < 0) return _T("");

	return ListProjects[ItemIndex]->Name;
}

//---------------------------------------------------------------------------
void TForm_AddNewProject::Button_OkClick(TObject *Sender) {

	if (((AnsiString) Edit_Name->Text).IsEmpty()) {
		FVCLIDEAutomation->DisplayError(_TT("Veuillez donner un nom au projet."),
																		_TT("Erreur saisie"),
																		_T(""),
																		MB_OK | MB_ICONWARNING);
		Edit_Name->SetFocus();
		return;
	}

	if (ListBox_Types->ItemIndex < 0) {
		FVCLIDEAutomation->DisplayError(_TT("Veuillez choisir un type de projet."),
																		_TT("Erreur saisie"),
																		_T(""),
																		MB_OK | MB_ICONWARNING);
		ListBox_Types->SetFocus();
		return;
	}

	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void TForm_AddNewProject::Button_AnnulerClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------
void TForm_AddNewProject::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_MODIFPROJECT);
}

//---------------------------------------------------------------------------
void TForm_AddNewProject::ListBox_TypesDrawItem(TObject *Sender, int Index, const TRect &Rect, TOwnerDrawStates States) {
	TCanvas *ACanvas;
	TColor clBack, clFore;
	TRect DrawRect;
	TTypeProject *TypeProject;
	TBitmap *Bitmap;


	DrawRect = Rect;

	ACanvas = ListBox_Types->Canvas;
	if (States.Contains(odSelected)) {
		clBack = clHighlight;
		clFore = clHighlightText;
	}
	else {
		clBack = clWindow;
		clFore = clWindowText;
	}

	ACanvas->Brush->Color = clBack;
	ACanvas->Brush->Style = bsSolid;
	ACanvas->FillRect(DrawRect);

	InflateRect(&DrawRect, -2, -2);
	if (States.Contains(odFocused)) DrawFocusRect(ACanvas->Handle, &DrawRect);

	InflateRect(&DrawRect, -2, -2);
	TypeProject = ListProjects[Index];
	if (TypeProject) {
		Bitmap = new TBitmap();
		Bitmap->Width = 20;
		Bitmap->Height = 20;
		Bitmap->Handle = TypeProject->GetBitmap20();
		ACanvas->Draw(DrawRect.Left, DrawRect.Top, Bitmap);
		delete Bitmap;
	}
	ACanvas->Font->Color = clFore;
	TFont *CanvasFont;
	CanvasFont = dynamic_cast<TFont *>((TCustomFont *) ACanvas->Font);
	CanvasFont->Handle = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	ACanvas->TextOut(DrawRect.Left + 24, DrawRect.Top + 2, ListBox_Types->Items->Strings[Index]);

}

//---------------------------------------------------------------------------
