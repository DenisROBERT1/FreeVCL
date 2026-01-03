//-----------------------------------------------------------------------------
//! @file TForm_EditProject.cpp
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

#include "ApplicationVHDL.h"
#include "TForm_EditProject.h"

//---------------------------------------------------------------------------
TForm_EditProject::TForm_EditProject(HWND hWnd, LPCTSTR szName)
  : TForm(hWnd, szName, FVCL_ARG_SUPP) {

}

//---------------------------------------------------------------------------
void FASTCALL TForm_EditProject::Button_ParcourirOutputClick(TObject *Sender) {

	if (SaveDialog_Output->Execute()) {
		Edit_Output->Text = (AnsiString) SaveDialog_Output->FileName;
		Edit_Output->SetFocus();
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_EditProject::TreeView_SourceFilesChange(TObject *Sender) {
	TTreeNode *SelectedNode;

	SelectedNode = TreeView_SourceFiles->Selected;
	if (SelectedNode && SelectedNode->Level == 1) {
		Button_Enlever->Enabled = true;
	}
	else {
		Button_Enlever->Enabled = false;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_EditProject::Button_AjouterClick(TObject *Sender) {
	TTreeNode *FirstNode;
	TTreeNode *SourceNode = NULL;
	TTreeNode *OtherNode = NULL;
	AnsiString asFileName;
	AnsiString asExt;
	int i;


	OpenDialog_AddFile->FilterIndex = 1;
	if (OpenDialog_AddFile->Execute()) {
		FirstNode = TreeView_SourceFiles->Items->FirstNode;
		if (FirstNode) {
			SourceNode = FirstNode->GetFirstChild();
			if (SourceNode) {
				OtherNode = SourceNode->GetNextSibling();
			}
		}
		if (SourceNode && OtherNode) {
			for (i = 0; i < OpenDialog_AddFile->Files->Count; i++) {
				asFileName = OpenDialog_AddFile->Files->Strings[i];
				asFileName = ExtractFileName(asFileName);  // $$$ Si le path est le même que le projet
				asExt = ExtractFileExt(asFileName).LowerCase();
				if (asExt == _T(".vhd")) {
					TreeView_SourceFiles->Items->AddChild(SourceNode, asFileName);
				}
				else {
					TreeView_SourceFiles->Items->AddChild(OtherNode, asFileName);
				}
			}
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_EditProject::Button_EnleverClick(TObject *Sender) {
	TTreeNode *SelectedNode;

	SelectedNode = TreeView_SourceFiles->Selected;
	if (SelectedNode && SelectedNode->Level == 1) {
		TreeView_SourceFiles->Items->Delete(SelectedNode);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_EditProject::Button_OkClick(TObject *Sender) {

  // Sortie
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_EditProject::Button_AideClick(TObject *Sender) {
}

//---------------------------------------------------------------------------
