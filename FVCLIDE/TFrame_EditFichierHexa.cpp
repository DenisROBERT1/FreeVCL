//-----------------------------------------------------------------------------
//! @file TFrame_EditFichierHexa.cpp
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
#include <FreeVcl_ToolsEDI.h>


//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------
#include "TConfig.h"
#include "TFrame_EditFichierHexa.h"
#include "TFVCLIDEAutomation.h"


//---------------------------------------------------------------------------
// Variables externes
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
TFrame_EditFichierHexa::TFrame_EditFichierHexa(TWinControl *AOwner)
  : TFrame_FVCLIDEBase(AOwner) {

  // Initialisations
	FFileType = ftf_HexaFile;

}

//---------------------------------------------------------------------------
TFrame_EditFichierHexa::~TFrame_EditFichierHexa(void) {

}

//---------------------------------------------------------------------------
void TFrame_EditFichierHexa::AfterConstruction(void) {
	int i;

  HexaEditor->IconsColumn = (int) Config->IconsColumn;
	for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
		HexaEditor->ColorParams[i] = Config->ColorParams[i];
	}
  HexaEditor->Font = Config->Font;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Changed
//---------------------------------------------------------------------------

bool TFrame_EditFichierHexa::Get_Changed(void) {
  return HexaEditor->Changed;
}

bool TFrame_EditFichierHexa::Set_Changed(bool NewChanged) {
  HexaEditor->Changed = NewChanged;

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierHexa::OpenFile(AnsiString asFileName) {
	if (HexaEditor->LoadFromFile(asFileName)) {
		FFileName = asFileName;
		HexaEditor->MoveCur(HexaEditor->FilePos, true, false);
		return true;
	}
	else {
		AnsiString asError;
		asError.sprintf(_TT("Impossible d'ouvrir le fichier \"%s\""), (LPCTSTR) asFileName);
		FVCLIDEAutomation->DisplaySystemError(asError, HexaEditor->LastError, _TT("Ouverture"), _T(""), MB_OK | MB_ICONSTOP);
		return false;
	}
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierHexa::SaveToFile(AnsiString asFileName) {
	if (HexaEditor->SaveToFile(asFileName)) {
		FFileName = asFileName;

		// Nom long
		AnsiString asLongFileName;
		TCHAR * szLongFileName = new TCHAR[32767];
		if (GetLongPathName(asFileName, szLongFileName, 32767)) {
			asLongFileName = szLongFileName;
		}
		delete[] szLongFileName;
		if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

		// Enregistrement dans la liste des fichiers récents
		Config->PushLastFile(asLongFileName);

		return true;
	}
	else {
		AnsiString asError;
		asError.sprintf(_TT("Impossible d'enregistrer le fichier \"%s\""), (LPCTSTR) asFileName);
		FVCLIDEAutomation->DisplaySystemError(asError, HexaEditor->LastError, _TT("Enregistrement"), _T(""), MB_OK | MB_ICONSTOP);
		return false;
	}
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierHexa::Undo(void) {
	HexaEditor->Undo();

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierHexa::Redo(void) {
	HexaEditor->Redo();

	return true;
}

//---------------------------------------------------------------------------
AnsiString TFrame_EditFichierHexa::GetModificationsList(void) {
	return HexaEditor->GetModificationsList();
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierHexa::CutToClipboard(void) {
	HexaEditor->CutToClipboard();

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierHexa::CopyToClipboard(void) {
	HexaEditor->CopyToClipboard();

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierHexa::PasteFromClipboard(void) {
	HexaEditor->PasteFromClipboard();

  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::Frame_EditFichierHexaShow(TObject *Sender) {
  SET_EVENT(HexaEditor, TOnCaretMove2, OnCaretMove, TFrame_EditFichierHexa, this, ObjectCaretMove);
	SET_EVENT(HexaEditor, TOnMessage, OnMessage, TFrame_EditFichierHexa, this, ObjectDisplayMessage);
	SET_EVENT(HexaEditor, TNotifyEvent, OnChange, TFrame_EditFichierHexa, this, ObjectChange);
	SET_EVENT(HexaEditor, TOnFlagChanged, OnFlagChanged, TFrame_EditFichierHexa, this, ObjectFlagChanged);
	SET_EVENT(HexaEditor, TNotifyEvent, OnPassBegin, TFrame_EditFichierHexa, this, ObjectPassBegin);
	SET_EVENT(HexaEditor, TNotifyEvent, OnPassEnd, TFrame_EditFichierHexa, this, ObjectPassEnd);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::HexaEditorDropFile(TObject *Sender,
		AnsiString &asFileName, TShiftState Shift, int X, int Y) {
  FVCLIDEAutomation->OpenFile(asFileName);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::ObjectCaretMove(TObject *Sender, __int64 FilePos) {
	AnsiString asMessage;
	asMessage.sprintf(_T("%04X:%04X"), (int) ((FilePos >> 16) & 0xFFFF), (int) (FilePos & 0xFFFF));
	FVCLIDEAutomation->DisplayMessage(td_Position, asMessage);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::ObjectDisplayMessage(TObject *Sender, const AnsiString &asMessage) {
	FVCLIDEAutomation->DisplayMessage(td_Message, asMessage);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::ObjectChange(TObject *Sender) {
  OnChange(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::ObjectFlagChanged(TObject *Sender, bool bChanged) {
  OnFlagChanged(this, bChanged);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::ObjectPassBegin(TObject *Sender) {
  OnPassBegin(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::ObjectPassEnd(TObject *Sender) {
  OnPassEnd(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::HexaEditorContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled) {
	TMenuItem *MenuItem;
	AnsiString asSelText;


	asSelText = HexaEditor->SelText;

	PopupMenu_Context->Clear();

	// Commandes couper, copier, coller

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("Couper");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierHexa, this, MenuItem_CouperClick);
	if (asSelText.IsEmpty()) MenuItem->Enabled = false;
	PopupMenu_Context->Add(MenuItem);

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("Copier");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierHexa, this, MenuItem_CopierClick);
	if (asSelText.IsEmpty()) MenuItem->Enabled = false;
	PopupMenu_Context->Add(MenuItem);

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("Coller");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierHexa, this, MenuItem_CollerClick);
	PopupMenu_Context->Add(MenuItem);

	// Commandes positionner marque-pages

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("-");
	PopupMenu_Context->Add(MenuItem);

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _TT("Positionner un marque-page");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierHexa, this, MenuItem_AddBookmarkClick);
	PopupMenu_Context->Add(MenuItem);

	PopupMenu_Context->Popup(MousePos.x, MousePos.y);
	Handled = true;

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::MenuItem_CouperClick(TObject *Sender) {
	HexaEditor->CutToClipboard();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::MenuItem_CopierClick(TObject *Sender) {
	HexaEditor->CopyToClipboard();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::MenuItem_CollerClick(TObject *Sender) {
	HexaEditor->PasteFromClipboard();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierHexa::MenuItem_AddBookmarkClick(TObject *Sender) {
	__int64 FilePos;

	FilePos = HexaEditor->FilePos;
	if (HexaEditor->GetFlag(FilePos, lfBookmark) == false) {
		FVCLIDEAutomation->AddBookmark();
	}
	else {
		FVCLIDEAutomation->DelBookmark();
	}
}

//---------------------------------------------------------------------------
FVCL_RegisterComponent(TFrame_EditFichierHexa);

//---------------------------------------------------------------------------
