//-----------------------------------------------------------------------------
//! @file TFrame_SaisieDialog.cpp
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
#include "TElemVcl.h"
#include "TConfig.h"
#include "TForm_Proprietes.h"
#include "TFrame_SaisieDialog.h"
#include "TFileForm.h"
#include "TFVCLIDEAutomation.h"
#include "TForm_SaisieDialog.h"
#include "TForm_DisplayError.h"
#include "HLP_FVCLIDE_Fr.h"

//---------------------------------------------------------------------------
// Variables externes
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// Structures, macros, enums
//---------------------------------------------------------------------------

typedef struct {
  TYPEPROPERTY TypeProperty;
  TWinControl *Control;
} ITEMPROPERTY, *LPITEMPROPERTY;


//---------------------------------------------------------------------------
TFrame_SaisieDialog::TFrame_SaisieDialog(TWinControl *AOwner)
  : TFrame_FVCLIDEBase(AOwner) {

  // Initialisations
	FFileType = ftf_Dialog;
  FModeConstr = mcSelect;
	FObjSelected = NULL;
  FTypeACreer = _T("");
  NumeroElement = 0;
	SelectedForm = -1;
  NbComponents = 0;
	bNotTreeView_FormsChange = false;

  FUndoRedo = new TUndoRedo();

}

//---------------------------------------------------------------------------
TFrame_SaisieDialog::~TFrame_SaisieDialog(void) {
	Clear();
	delete FUndoRedo;
}

//---------------------------------------------------------------------------
void TFrame_SaisieDialog::AfterConstruction(void) {
	TStringList *StringList_ObjToCreate;
	TStringList *StringList_Tabs;
	TStringList *StringList_Onglets;
	TTabSheet *TabSheet;
	TPageScroller *PageScroller;
	TPanel *Panel;
	TBitBtn *BitBtn_ObjToCreate;
	TAnchors Anchors;
	int i, j;
	int PanelWidth;
	AnsiString asObjName;
	AnsiString asIconFileName;


	//--------------------------------------------
	// Remplissage de la liste des objets à créer
	//--------------------------------------------

	StringList_ObjToCreate = new TStringList();
	StringList_Tabs = new TStringList();
	StringList_Onglets = new TStringList();
	StringList_Onglets->Duplicates = dupIgnore;
	StringList_Onglets->CaseSensitive = false;

	Factory_FVCL.GetListObjects(StringList_ObjToCreate, StringList_Tabs);

	// Liste des onglets standard (pour qu'ils soient au début)
	StringList_Onglets->Add(_TT("Contrôles courants"));
	StringList_Onglets->Add(_TT("Contrôles étendus"));
	StringList_Onglets->Add(_TT("Système"));

	// Liste des onglets (doublons supprimés avec dupIgnore)
	for (i = 0; i < StringList_Tabs->Count; i++) {
		asObjName = StringList_ObjToCreate->Strings[i];

		if (asObjName != _T("TFrame_EditFichierText") &&
				asObjName != _T("TFrame_EditFichierHexa") &&
				asObjName != _T("TFrame_SaisieDialog") &&
				asObjName != _T("TFrame_SaisieIcon") &&
				asObjName != _T("TFrame_SaisieCursor")) {
			StringList_Onglets->Add(Application->Translate(StringList_Tabs->Strings[i]));
		}
	}

	// Création des onglets
	for (j = 0; j < StringList_Onglets->Count; j++) {

		TabSheet = new TTabSheet(PageControl_ObjToCreate);
		TabSheet->Caption = StringList_Onglets->Strings[j];

		PageScroller = new TPageScroller(TabSheet);
		PageScroller->Orientation = soHorizontal;
		PageScroller->Align = alClient;

		Panel = new TPanel(PageScroller);

		// Création des icônes d'objets dans les onglets

		PanelWidth = 4;

		for (i = 0; i < StringList_ObjToCreate->Count && i < StringList_Tabs->Count; i++) {

			if (StringList_Tabs->Strings[i] == StringList_Onglets->Strings[j]) {

				asObjName = StringList_ObjToCreate->Strings[i];

				if (asObjName != _T("TFrame_EditFichierText") &&
						asObjName != _T("TFrame_EditFichierHexa") &&
						asObjName != _T("TFrame_SaisieDialog") &&
						asObjName != _T("TFrame_SaisieIcon") &&
						asObjName != _T("TFrame_SaisieCursor")) {

					TBitmap *Bitmap_Icon = new TBitmap();
					Bitmap_Icon->Width = 32;
					Bitmap_Icon->Height = 32;

					asIconFileName = (AnsiString) Config->InstallPath + _T("\\Icons\\") + asObjName + _T(".bmp");
					if (FileExists(asIconFileName)) {
						Bitmap_Icon->LoadFromFile(asIconFileName);
					}
					else {
						Bitmap_Icon->LoadFromResource(HInstance, _T("BMP_CONTROLUNKNOWN"));
					}

					BitBtn_ObjToCreate = new TBitBtn(Panel);
					BitBtn_ObjToCreate->Name = asObjName;
					BitBtn_ObjToCreate->Left = PanelWidth;
					BitBtn_ObjToCreate->Top = 8;
					BitBtn_ObjToCreate->Width = 38;
					BitBtn_ObjToCreate->Height = 38;
					BitBtn_ObjToCreate->Glyph = Bitmap_Icon;
					BitBtn_ObjToCreate->ShowHint = true;
					BitBtn_ObjToCreate->Hint = asObjName;
					SET_EVENT(BitBtn_ObjToCreate, TNotifyEvent, OnClick, TFrame_SaisieDialog, this, BitBtn_ObjToCreateClick);

					delete Bitmap_Icon;

					PanelWidth += 42;

				}

			}

		}

		Panel->Width = PanelWidth + 4;
		PageScroller->Control = Panel;

	}



	delete StringList_ObjToCreate;
	delete StringList_Tabs;
	delete StringList_Onglets;

	//-------------------------------
	// Initialisation des paramètres
	//-------------------------------

	Edit_GridSizeX->Text = (int) Config->GridX;
	Edit_GridSizeY->Text = (int) Config->GridY;
	CheckBox_Afficher->Checked = Config->ShowGrid;
	CheckBox_Aligner->Checked = Config->AlignGrid;
	CheckBox_AlignerClick(this);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DisplayedFileName
//---------------------------------------------------------------------------

AnsiString TFrame_SaisieDialog::Get_DisplayedFileName(void) {
	if (FFileName.IsEmpty()) return _TT("Nouveau fichier");
  return ChangeFileExt(ExtractFileName(FFileName), _T(""));
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::Frame_SaisieDialogShow(TObject *Sender) {
  if (SelectedForm != -1) {
    Dialogues[SelectedForm].Form->Show(true);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::Frame_SaisieDialogHide(TObject *Sender) {
  if (SelectedForm != -1) {
    Dialogues[SelectedForm].Form->Show(false);
  }
}

//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::CreateNewDialog(TStrings *Description, HWND hWnd) {
	TElemBase *NewElemForm = NULL;
	TElemBase *NewElement = NULL;
	TAnchors Anchors;
	TFileForm *FileForm;
	AnsiString asLine;
	AnsiString asMot1, asMot2, asMot3;
	int PosSpace;
  int i;


	if (hWnd == NULL) {

		for (i = 0; i < Description->Count; i++) {

			asMot1 = _T("");
			asMot2 = _T("");
			asMot3 = _T("");
			asLine = Description->Strings[i];
			PosSpace = asLine.Pos(_T(" "));
			if (PosSpace) {
				asMot1 = asLine.SubString(1, PosSpace - 1);
				asLine = asLine.SubString(PosSpace + 1, asLine.Length() - PosSpace);
				if (!asLine.IsEmpty()) {
					PosSpace = asLine.Pos(_T(" "));
					asMot2 = asLine.SubString(1, PosSpace - 1);
					asMot3 = asLine.SubString(PosSpace + 1, asLine.Length() - PosSpace);
				}
			}
			else {
				asMot1 = asLine;
			}

			if (asMot1 == _T("object")) {

				NewElement = NULL;

				if (asMot3 == _T("TForm")) {
					NewElement = new TElemVCL(NULL, NULL);
					if (!NewElement->Create(asMot3)) return false;
					NewElemForm = NewElement;
					NewFormInit(this, NewElemForm);
				}
				else {
					if (!NewElemForm) return false;
					NewElement = NewElemForm->NewChild(asMot3);
					if (!NewElement) return false;
				}
				NewElement->Name = asMot2.SubString(1, (int) asMot2.length() - 1);

			}
			else if (asMot2 == _T("=")) {

				if (asMot1 == _T("Left")) {
					if (NewElement) NewElement->Left = asMot3.ToIntDef(0);
				}
				else if (asMot1 == _T("Top")) {
					if (NewElement) NewElement->Top = asMot3.ToIntDef(0);
				}
				else if (asMot1 == _T("Width")) {
					if (NewElement) {
						if (NewElement != NewElemForm) NewElement->Width = asMot3.ToIntDef(0);
						else NewElement->Width = asMot3.ToIntDef(0) + 2 * GetSystemMetrics(SM_CXSIZEFRAME) - 8;
					}
				}
				else if (asMot1 == _T("Height")) {
					if (NewElement) {
						if (NewElement != NewElemForm) NewElement->Height = asMot3.ToIntDef(0);
						else NewElement->Height = asMot3.ToIntDef(0) + 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) - 27;
					}
				}
				else {
					if (asMot3.Length() >= 2 && asMot3[1] == _T('\'')) asMot3 = asMot3.SubString(2, asMot3.Length() - 2);
					NewElement->SetProperty(asMot1, asMot3);
				}

			}
			else if (asMot1 == _T("end")) {
				if (NewElement != NewElemForm && NewElement != NULL) {
					NewComponentInit(this, NewElemForm, NewElemForm, NewElement);
					NewElement = NULL;
				}
			}

		}

	}
	else {

		FileForm = new TFileForm();
		SET_EVENT(FileForm, TOnNewFormInit, OnNewFormInit, TFrame_SaisieDialog, this, NewFormInit);
		SET_EVENT(FileForm, TOnNewComponentInit, OnNewComponentInit, TFrame_SaisieDialog, this, NewComponentInit);
		FileForm->LoadFromWindow(hWnd);
		delete FileForm;

	}

	Changed = true;

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::OpenFile(AnsiString asFileName) {
  AnsiString asExt;
	bool bRet;


	Clear();

  asExt = ExtractFileExt(asFileName).LowerCase();
	int Len = asExt.Length();
	if (Len > 0 && asExt[1] == _T('.')) {
		asExt = asExt.SubString(2, Len - 1);
	}

	TFileForm *FileForm = new TFileForm();
  SET_EVENT(FileForm, TOnNewFormInit, OnNewFormInit, TFrame_SaisieDialog, this, NewFormInit);
  SET_EVENT(FileForm, TOnNewComponentInit, OnNewComponentInit, TFrame_SaisieDialog, this, NewComponentInit);
  if (asExt == _T("rc")) bRet = FileForm->OpenRC(asFileName);
  else if (asExt == _T("dfm")) bRet = FileForm->OpenDFM(asFileName);
  else if (asExt == _T("frm")) bRet = FileForm->OpenFRM(asFileName);
	else bRet = false;
  delete FileForm;

	if (bRet) {
		FFileName = asFileName;
		// La création des composants dans le dialogue met le flag "Changed" à true
		// => il faut le remettre à false après chargement complet
		Changed = false;
	}

	return bRet;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::SaveToFile(AnsiString asFileName) {
  TFileForm *FileForm;
  AnsiString asExt;
	bool bRet = false;


  FileForm = new TFileForm();
  asExt = ExtractFileExt(asFileName).LowerCase();
  if (asExt == _T(".rc")) bRet = FileForm->SaveRC(asFileName, Dialogues);
  else if (asExt == _T(".dfm")) bRet = FileForm->SaveDFM(asFileName, Dialogues);
  delete FileForm;

	if (bRet) {
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
		Config->PushLastDialog(asLongFileName);

	}

	return bRet;
}

//---------------------------------------------------------------------------
// Undo/redo
//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::Undo(void) {
	AnsiString asOperation;
	bool bRet;

  bRet = FUndoRedo->Undo(&asOperation);
  FVCLIDEAutomation->DisplayMessage(td_Message, asOperation);
	if (FUndoRedo->IsEmpty(-1)) {
		if (FChanged) {
			FChanged = false;
			OnFlagChanged(this, FChanged);
		}
	}

	return bRet;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::Redo(void) {
	AnsiString asOperation;
	bool bRet;

  bRet = FUndoRedo->Redo(&asOperation);
  FVCLIDEAutomation->DisplayMessage(td_Message, asOperation);
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}

	return bRet;
}

//---------------------------------------------------------------------------
// Liste des modifications
//---------------------------------------------------------------------------
AnsiString TFrame_SaisieDialog::GetModificationsList(void) {
	return FUndoRedo->GetModificationsList(0);
}

//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::ProcessUndo(void * Undo, int LenUndo) {
	LPUNDODIALCREATEDIALOG lpUndoCreateDialog;
	LPUNDODIALDELETEDIALOG lpUndoDeleteDialog;
	LPUNDODIALCREATEOBJ lpUndoCreateObj;
	LPUNDODIALDELETEOBJ lpUndoDeleteObj;
	LPUNDODIALRESIZE lpUndoResize;
	LPUNDODIALCHANGEVALUE lpUndoChangeValue;
	LPUNDODIALCHANGEPROPERTY lpUndoChangeProperty;
	LPUNDODIALSWAP lpUndoSwap;
	LPUNDODIALMOVE lpUndoMove;
	TElemBase *Element, *ElementParent;
	int IndexDialog, Index;
	TAnchors Anchors;
	AnsiString asProperty, asValue;

	ACTIONUNDODIAL ActionUndoDial = *(ACTIONUNDODIAL *) Undo;

  switch (ActionUndoDial) {

		case auCreateDialog:
			lpUndoCreateDialog = (LPUNDODIALCREATEDIALOG) Undo;
			CreateDialog2(lpUndoCreateDialog->IndexDialog,
										lpUndoCreateDialog->NumeroElement,
										lpUndoCreateDialog->Left, lpUndoCreateDialog->Top,
										lpUndoCreateDialog->Width, lpUndoCreateDialog->Height);
      return true;

		case auDeleteDialog:
			lpUndoDeleteDialog = (LPUNDODIALDELETEDIALOG) Undo;
      DeleteDialog(lpUndoDeleteDialog->IndexDialog);
      return true;

		case auCreateObj:
			lpUndoCreateObj = (LPUNDODIALCREATEOBJ) Undo;
			ElementParent = GetElemTreeview(lpUndoCreateObj->IndexParent);
			CreateObj(ElementParent,
								lpUndoCreateObj->IndexDialog,
								lpUndoCreateObj->NumeroElement,
								lpUndoCreateObj->Left, lpUndoCreateObj->Top,
								lpUndoCreateObj->Width, lpUndoCreateObj->Height,
								lpUndoCreateObj->Anchors,
								lpUndoCreateObj->szTypeObj);
      return true;

		case auDeleteObj:
			lpUndoDeleteObj = (LPUNDODIALDELETEOBJ) Undo;
			Element = GetElemTreeview(lpUndoDeleteObj->Index);
			ElementParent = GetObjParent(Element);
      DeleteObj(ElementParent, Element);
      return true;

    case auResize:
			lpUndoResize = (LPUNDODIALRESIZE) Undo;
			IndexDialog = lpUndoResize->IndexDialog;
			Dialogues[IndexDialog].Form->Left = lpUndoResize->Left;
			Dialogues[IndexDialog].Form->Top = lpUndoResize->Top;
			Dialogues[IndexDialog].Form->Width = lpUndoResize->Width;
			Dialogues[IndexDialog].Form->Height = lpUndoResize->Height;
      return true;

    case auChangeLeft:
			lpUndoChangeValue = (LPUNDODIALCHANGEVALUE) Undo;
			GetElemTreeview(lpUndoChangeValue->Index)->Left = lpUndoChangeValue->Value;
      return true;

    case auChangeTop:
			lpUndoChangeValue = (LPUNDODIALCHANGEVALUE) Undo;
			GetElemTreeview(lpUndoChangeValue->Index)->Top = lpUndoChangeValue->Value;
      return true;

    case auChangeWidth:
			lpUndoChangeValue = (LPUNDODIALCHANGEVALUE) Undo;
			GetElemTreeview(lpUndoChangeValue->Index)->Width = lpUndoChangeValue->Value;
      return true;

    case auChangeHeight:
			lpUndoChangeValue = (LPUNDODIALCHANGEVALUE) Undo;
			GetElemTreeview(lpUndoChangeValue->Index)->Height = lpUndoChangeValue->Value;
      return true;

    case auChangeAnchors:
			lpUndoChangeValue = (LPUNDODIALCHANGEVALUE) Undo;
			Anchors.Clear();
			if (lpUndoChangeValue->Value & 1) Anchors << akLeft;
			if (lpUndoChangeValue->Value & 2) Anchors << akTop;
			if (lpUndoChangeValue->Value & 4) Anchors << akRight;
			if (lpUndoChangeValue->Value & 8) Anchors << akBottom;
			GetElemTreeview(lpUndoChangeValue->Index)->Anchors = Anchors;
      return true;

    case auChangeProperty:
			lpUndoChangeProperty = (LPUNDODIALCHANGEPROPERTY) Undo;
			Index = lpUndoChangeProperty->Index;
			asProperty = lpUndoChangeProperty->szPropertyName;
			asValue = lpUndoChangeProperty->szPropertyValue;
			GetElemTreeview(Index)->SetProperty(asProperty, asValue);
			if (asProperty == _T("Name")) {
				TreeView_Forms->Items->Item[Index]->Text = asValue;
			}

      return true;

    case auSwap:
			lpUndoSwap = (LPUNDODIALSWAP) Undo;
			SwapObjects(lpUndoSwap->Index1, lpUndoSwap->Index2);
      return true;

    case auMove:
			lpUndoMove = (LPUNDODIALMOVE) Undo;
			MoveObject(lpUndoMove->Index, lpUndoMove->IndexParent, lpUndoMove->IndexInsertAfter);
      return true;

  }

  return false;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::ProcessRedo(void * Redo, int LenRedo) {
  // Les commandes undo et redo sont les mêmes
	return ProcessUndo(Redo, LenRedo);
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieDialog::CutToClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieDialog::CopyToClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieDialog::PasteFromClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::TreeView_FormsChange(TObject *Sender, TTreeNode *TreeNodeChanged) {
	TTreeNode *TreeNode;
	TElemBase *ElemBase;
	int i;


	if (bNotTreeView_FormsChange) return;

	for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		TreeNode = TreeView_Forms->Items->Item[i];
		ElemBase = static_cast<TElemBase *>((void *) TreeNode->Data);
		if (ElemBase) {
			if (TreeNode->Selected) {
				ElemBase->Show(true);
				ElemBase->Selected = true;
				FObjSelected = ElemBase;
			}
			else {
				ElemBase->Selected = false;
			}
		}
	}

	Reconst();

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::TreeView_FormsContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled) {

  AffPopupMenu();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::TreeView_FormsDblClick(TObject *Sender) {
  int i;
  AnsiString asText;


  for (i = 0; i < (int) Dialogues.size(); i++) {
    if (Dialogues[i].TreeNode->Selected) {
      Dialogues[i].Form->Show(true);
      SelectedForm = i;
    }
    else {
      Dialogues[i].Form->Show(false);
    }
  }

  Reconst();

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::TreeView_FormsDragOver(TObject *Sender,
        TObject *Source, int X, int Y, TDragState State, bool &Accept) {
	if (Sender == TreeView_Forms) Accept = true;
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::TreeView_FormsEndDrag(TObject *Sender,
        TObject *Target, int X, int Y) {
	TTreeNode *TreeNodeSrc, *TreeNodeDest, *TreeNodeParentSrc, *TreeNodeBeforeSrc;


	TreeNodeSrc = TreeView_Forms->Selected;
	TreeNodeDest = TreeView_Forms->GetNodeAt(X, Y);
	if (TreeNodeDest) {
		UNDODIALMOVE UndoMove;
		int IndexSrc, IndexDest;

		IndexSrc = TreeNodeSrc->Index;
		IndexDest = TreeNodeDest->Index;
		TreeNodeParentSrc = TreeNodeSrc->Parent;
		TreeNodeBeforeSrc = TreeNodeSrc->GetPrevSibling();

		FUndoRedo->BeginOperation(0, _TT("Modification parent d'un élément"));

		// Mémorisation du Redo
		UndoMove.ActionUndoDial = auMove;
		UndoMove.Index = IndexSrc;
		UndoMove.IndexParent = IndexDest;
		UndoMove.IndexInsertAfter = -2;
		FUndoRedo->MemoRedo(this, &UndoMove, sizeof(UndoMove));

		// Déplacement
		MoveObject(IndexSrc, IndexDest, -2);

		// Mémorisation du Undo
		UndoMove.ActionUndoDial = auMove;
		UndoMove.Index = TreeNodeDest->GetLastChild()->Index;
		UndoMove.IndexParent = TreeNodeParentSrc->Index;
		if (TreeNodeBeforeSrc) UndoMove.IndexInsertAfter = TreeNodeBeforeSrc->Index;
		else UndoMove.IndexInsertAfter = -1;
		FUndoRedo->MemoUndo(this, &UndoMove, sizeof(UndoMove));

		FUndoRedo->EndOperation();

	}
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::TreeView_FormsEdited(TObject *Sender, TTreeNode *TreeNode, AnsiString asNewName) {
	LPUNDODIALCHANGEPROPERTY lpUndoChangeProperty;
	int SizeBuffer, SizeUndoBuffer, SizeRedoBuffer;
	TElemBase *Element;
	AnsiString asOldName;


	Element = static_cast<TElemBase *>((void *) TreeNode->Data);
	if (Element) {

		asOldName = Element->GetProperty(_T("Name"));

		FUndoRedo->BeginOperation(0, _TT("Renommage d'un élément : ") + asOldName + _T(" -> ") + asNewName);

		SizeUndoBuffer = sizeof(UNDODIALCHANGEPROPERTY) + (int) asOldName.length() * sizeof(TCHAR);
		SizeRedoBuffer = sizeof(UNDODIALCHANGEPROPERTY) + (int) asNewName.length() * sizeof(TCHAR);
		if (SizeRedoBuffer <= SizeUndoBuffer) SizeBuffer = SizeUndoBuffer;
		else SizeBuffer = SizeRedoBuffer;
		lpUndoChangeProperty = (LPUNDODIALCHANGEPROPERTY) new BYTE[SizeBuffer];


		// Mémorisation du Redo
		lpUndoChangeProperty->ActionUndoDial = auChangeProperty;
		lpUndoChangeProperty->Index = GetIdTreeview(Element);
		lstrcpynA(lpUndoChangeProperty->szPropertyName, "Name", 50);
		lstrcpy(lpUndoChangeProperty->szPropertyValue, asNewName);
		FUndoRedo->MemoRedo(this, lpUndoChangeProperty, SizeRedoBuffer);

		// Mémorisation du Undo
		lpUndoChangeProperty->ActionUndoDial = auChangeProperty;
		lpUndoChangeProperty->Index = GetIdTreeview(Element);
		lstrcpynA(lpUndoChangeProperty->szPropertyName, "Name", 50);
		lstrcpy(lpUndoChangeProperty->szPropertyValue, asOldName);
		FUndoRedo->MemoUndo(this, lpUndoChangeProperty, SizeUndoBuffer);

		Element->SetProperty(_T("Name"), asNewName);

		delete[] lpUndoChangeProperty;

		FUndoRedo->EndOperation();

	  Reconst();

	}

}

//---------------------------------------------------------------------------
void TFrame_SaisieDialog::SetFormSelected(int Index) {
  int i;
  AnsiString asText;


  for (i = 0; i < (int) Dialogues.size(); i++) {
    if (i == Index) {
      Dialogues[i].TreeNode->Selected = true;
      Dialogues[i].Form->Show(Visible);
      SelectedForm = i;
    }
    else {
      Dialogues[i].TreeNode->Selected = false;
      Dialogues[i].Form->Show(false);
    }
  }

  Reconst();

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::NewFormInit(TObject *Sender, TElemBase * NewForm) {
  TForm_SaisieDialog *Form_SaisieDialog;
	TTreeNode * TreeNodeParent;
	TTreeNode * TreeNodeChild;
	TElemVCL * NewFormVCL;
  TComponent *Component;


	NewFormVCL = dynamic_cast<TElemVCL *>(NewForm);

  Component = NewFormVCL->Component;
  Form_SaisieDialog = dynamic_cast<TForm_SaisieDialog *>(Component);
	Form_SaisieDialog->Frame_SaisieDialog = this;
	Form_SaisieDialog->IndexDialog = (int) Dialogues.size();
  Form_SaisieDialog->Visible = false;
  SET_EVENT(Form_SaisieDialog, TNotifyEvent, OnChange, TFrame_SaisieDialog, this, ElemChange);
  SET_EVENT(Form_SaisieDialog, TOnChangeSelection, OnChangeSelection, TFrame_SaisieDialog, this, Form_SaisieDialogChangeSelection);

	// Enregistrement de l'objet en interne
  EnrObjet(NULL, NewForm,
           NewForm->Left, NewForm->Top,
           NewForm->Width, NewForm->Height,
           NewForm->Anchors, true);

	// Mise à jour du TreeView
  TreeNodeParent = TreeView_Forms->Items->FirstNode;
	TreeNodeChild = TreeView_Forms->Items->AddChild(TreeNodeParent, NewForm->Name);
	TreeNodeChild->Data = NewForm;

	// MAJ nombre de composants
	DIALOGUE NewDialog;
	NewDialog.TreeNode = TreeNodeChild;
	NewDialog.Form = NewFormVCL;
	Dialogues.push_back(NewDialog);
	Label_NbDialogues->Caption = AnsiString((int) Dialogues.size());

  SetFormSelected((int) Dialogues.size() - 1);

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::NewComponentInit(TObject *Sender,
                        TElemBase * Form,
                        TElemBase * ElemBaseParent, TElemBase * NewElemBase) {
	TTreeNode * TreeNode;
	TTreeNode * TreeNodeParent;
	TTreeNode * TreeNodeChild;
	TElemVCL * NewElemVCL;
  TComponent *Component;
  TWinControl *WinControl;
	bool bParentPotentiel;
  int i;


	NewElemVCL = dynamic_cast<TElemVCL *>(NewElemBase);

  // Evènement OnClick
	NewElemBase->Parent = ElemBaseParent;
  Component = NewElemVCL->Component;
  WinControl = dynamic_cast<TWinControl *>(Component);
	if (WinControl) {
		bParentPotentiel = true;
	}
	else {
		bParentPotentiel = false;
	}

	// Enregistrement de l'objet en interne
  EnrObjet(ElemBaseParent, NewElemBase,
           NewElemBase->Left, NewElemBase->Top,
           NewElemBase->Width, NewElemBase->Height,
           NewElemBase->Anchors, bParentPotentiel);

	Changed = true;

	// Mise à jour du TreeView
  TreeNodeParent = TreeView_Forms->Items->FirstNode;
	if (ElemBaseParent) {
		for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
			TreeNode = TreeView_Forms->Items->Item[i];
			if (TreeNode->Data == ElemBaseParent) {
				TreeNodeParent = TreeNode;
				break;
			}
		}
	}
	TreeNodeChild = TreeView_Forms->Items->AddChild(TreeNodeParent, NewElemBase->Name);
	TreeNodeChild->Data = NewElemBase;

	// MAJ nombre de composants
	Label_NbControles->Caption = AnsiString(++NbComponents);

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::BitBtn_SelectClick(TObject *Sender) {
	FModeConstr = mcSelect;
	FTypeACreer = _T("");
	Label_ObjName->Caption = FTypeACreer;
	Dialogues[SelectedForm].Form->Show(true);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::CheckBox_AlignerClick(TObject *Sender) {
  AnsiString asText;


	if (CheckBox_Aligner->Checked) {
		Edit_GridSizeX->Enabled = true;
		Edit_GridSizeY->Enabled = true;
		UpDown_GridSizeX->Enabled = true;
		UpDown_GridSizeY->Enabled = true;
		CheckBox_Afficher->Enabled = true;
  }
  else {
    Edit_GridSizeX->Enabled = false;
    Edit_GridSizeY->Enabled = false;
    UpDown_GridSizeX->Enabled = false;
    UpDown_GridSizeY->Enabled = false;
		CheckBox_Afficher->Enabled = false;
  }
	CheckBox_Aligner->Checked = CheckBox_Aligner->Checked;
	Config->AlignGrid = CheckBox_Aligner->Checked;
  Reconst();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::CheckBox_AfficherClick(TObject *Sender) {

	CheckBox_Afficher->Checked = CheckBox_Afficher->Checked;
	Config->ShowGrid = CheckBox_Afficher->Checked;
  Reconst();
	Frame_SaisieDialogShow(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::Edit_GridSizeXChange(TObject *Sender) {

  if (CheckBox_Aligner->Checked) {
    Reconst();
  }
	Config->GridX = ((AnsiString) Edit_GridSizeX->Text).ToIntDef(0);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::Edit_GridSizeYChange(TObject *Sender) {

  if (CheckBox_Aligner->Checked) {
    Reconst();
  }
	Config->GridY = ((AnsiString) Edit_GridSizeY->Text).ToIntDef(0);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::CheckBox_OrdreTabClick(TObject *Sender) {

  Reconst();
	Frame_SaisieDialogShow(this);

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::Button_RenumeroterClick(TObject *Sender) {
	LPUNDODIALCHANGEPROPERTY lpUndoChangeProperty;
	int SizeBuffer, SizeUndoBuffer, SizeRedoBuffer;
	AnsiString asTabStop, asValue, asOldValue;
	TElemBase *Element;
	int PileTabOrder[100];
	int CurLevel, Level;
	int CurTabOrder;
	int i;


	FUndoRedo->BeginOperation(0, _TT("Renumérotation des TabOrder"));

	CurLevel = 0;
	CurTabOrder = 0;

	for (i = 0; i < (int) TreeView_Forms->Items->Count; i++) {
		Level = TreeView_Forms->Items->Item[i]->Level;
		if (Level > CurLevel) {
			PileTabOrder[CurLevel] = CurTabOrder;
			CurLevel = Level;
			CurTabOrder = 0;
		}
		else if (Level < CurLevel) {
			CurLevel = Level;
			CurTabOrder = PileTabOrder[Level];
		}

		Element = GetElemTreeview(i);
		// Les dialogues (Level == 0) n'ont pas d'élément associé
		if (Element) {
			asTabStop = Element->GetProperty(_T("TabStop"));
			if (asTabStop == _T("True")) {

				asOldValue = Element->GetProperty(_T("TabOrder"));
				asValue = IntToStr(CurTabOrder++);

				if (asValue != asOldValue) {

					SizeUndoBuffer = sizeof(UNDODIALCHANGEPROPERTY) + (int) asOldValue.length() * sizeof(TCHAR);
					SizeRedoBuffer = sizeof(UNDODIALCHANGEPROPERTY) + (int) asValue.length() * sizeof(TCHAR);
					if (SizeRedoBuffer <= SizeUndoBuffer) SizeBuffer = SizeUndoBuffer;
					else SizeBuffer = SizeRedoBuffer;
					lpUndoChangeProperty = (LPUNDODIALCHANGEPROPERTY) new BYTE[SizeBuffer];

					// Mémorisation du Redo
					lpUndoChangeProperty->ActionUndoDial = auChangeProperty;
					lpUndoChangeProperty->Index = GetIdTreeview(Element);
					lstrcpynA(lpUndoChangeProperty->szPropertyName, "TabOrder", 50);
					lstrcpy(lpUndoChangeProperty->szPropertyValue, asValue);
					FUndoRedo->MemoRedo(this, lpUndoChangeProperty, SizeRedoBuffer);

					// Mémorisation du Undo
					lpUndoChangeProperty->ActionUndoDial = auChangeProperty;
					lpUndoChangeProperty->Index = GetIdTreeview(Element);
					lstrcpynA(lpUndoChangeProperty->szPropertyName, "TabOrder", 50);
					lstrcpy(lpUndoChangeProperty->szPropertyValue, asOldValue);
					FUndoRedo->MemoUndo(this, lpUndoChangeProperty, SizeUndoBuffer);

					Element->SetProperty(_T("TabOrder"), asValue);

					delete[] lpUndoChangeProperty;

				}

			}

		}

	}

	FUndoRedo->EndOperation();

	Reconst();

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIEDIALOG);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::BitBtn_ObjToCreateClick(TObject *Sender) {
	TBitBtn *BitBtn_ObjToCreate;

  if (SelectedForm != -1) {
		BitBtn_ObjToCreate = dynamic_cast<TBitBtn *>(Sender);
		if (BitBtn_ObjToCreate) {
	    FModeConstr = mcCreat;
			FTypeACreer = BitBtn_ObjToCreate->Name;
			Label_ObjName->Caption = FTypeACreer;
			Dialogues[SelectedForm].Form->Show(true);
		}
  }
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AlignGrid
//---------------------------------------------------------------------------

bool TFrame_SaisieDialog::Get_AlignGrid(void) {
  return CheckBox_Aligner->Checked;
}

bool TFrame_SaisieDialog::Set_AlignGrid(bool NewAlignGrid) {
  if (CheckBox_Aligner->Checked != NewAlignGrid) {
    CheckBox_Aligner->Checked = NewAlignGrid;
    Reconst();
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowGrid
//---------------------------------------------------------------------------

bool TFrame_SaisieDialog::Get_ShowGrid(void) {
  return CheckBox_Afficher->Checked;
}

bool TFrame_SaisieDialog::Set_ShowGrid(bool NewShowGrid) {
  if (CheckBox_Afficher->Checked != NewShowGrid) {
    CheckBox_Afficher->Checked = NewShowGrid;
    Reconst();
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété GridSizeX
//---------------------------------------------------------------------------

int TFrame_SaisieDialog::Get_GridSizeX(void) {
  return ((AnsiString) Edit_GridSizeX->Text).ToInt();
}

bool TFrame_SaisieDialog::Set_GridSizeX(int NewGridSizeX) {
  if (((AnsiString) Edit_GridSizeX->Text).ToInt() != NewGridSizeX) {
    Edit_GridSizeX->Text = NewGridSizeX;
    Reconst();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété GridSizeY
//---------------------------------------------------------------------------

int TFrame_SaisieDialog::Get_GridSizeY(void) {
  return ((AnsiString) Edit_GridSizeY->Text).ToInt();
}

bool TFrame_SaisieDialog::Set_GridSizeY(int NewGridSizeY) {
  if (((AnsiString) Edit_GridSizeY->Text).ToInt() != NewGridSizeY) {
    Edit_GridSizeY->Text = NewGridSizeY;
    Reconst();
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété OrdreTab
//---------------------------------------------------------------------------

bool TFrame_SaisieDialog::Get_OrdreTab(void) {
  return CheckBox_OrdreTab->Checked;
}

bool TFrame_SaisieDialog::Set_OrdreTab(bool NewOrdreTab) {
  if (CheckBox_OrdreTab->Checked != NewOrdreTab) {
    CheckBox_OrdreTab->Checked = NewOrdreTab;
    Reconst();
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ObjSelected
//---------------------------------------------------------------------------

TElemBase *TFrame_SaisieDialog::Get_ObjSelected(void) {
  return FObjSelected;
}

bool TFrame_SaisieDialog::Set_ObjSelected(TElemBase *NewObjSelected) {
  if (FObjSelected != NewObjSelected) {
    FObjSelected = NewObjSelected;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ModeConstr
//---------------------------------------------------------------------------

MODECONSTR TFrame_SaisieDialog::Get_ModeConstr(void) {
  return FModeConstr;
}

bool TFrame_SaisieDialog::Set_ModeConstr(MODECONSTR NewModeConstr) {

  if (FModeConstr != NewModeConstr) {
    FModeConstr = NewModeConstr;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété UndoRedo
//---------------------------------------------------------------------------

TUndoRedo *TFrame_SaisieDialog::Get_UndoRedo(void) {
  return FUndoRedo;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TypeACreer
//---------------------------------------------------------------------------

AnsiString TFrame_SaisieDialog::Get_TypeACreer(void) {
  return FTypeACreer;
}

bool TFrame_SaisieDialog::Set_TypeACreer(AnsiString NewTypeACreer) {
  if (FTypeACreer != NewTypeACreer) {
    FTypeACreer = NewTypeACreer;
  }
  return true;
}


//---------------------------------------------------------------------------
// Recherche de l'objet parent
//---------------------------------------------------------------------------

TElemBase *TFrame_SaisieDialog::GetObjParent(TElemBase *Element) {
  int i;


  // Recherche de l'objet parent
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i) == Element) return GetElemTreeview(i)->Parent;
  }

  return NULL;
}

//---------------------------------------------------------------------------
// Recherche de l'élément de TreeView_Forms auquel est associé un objet
//---------------------------------------------------------------------------

int TFrame_SaisieDialog::GetIdTreeview(TElemBase *Element) {
  int i;


  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i) == Element) return i;
  }

  return -1;
}

//---------------------------------------------------------------------------
// Recherche de l'index du dialogue auquel apartient l'objet
//---------------------------------------------------------------------------

int TFrame_SaisieDialog::GetIndexDialog(TElemBase *Element) {
  TElemBase *ElementParent;
	int i;


	ElementParent = Element;
	while (Element) {
		ElementParent = Element;
		Element = GetObjParent(Element);
	}

	for (i = 0; i < (int) Dialogues.size(); i++) {
    if (Dialogues[i].Form == ElementParent) return i;
  }

  return -1;
}

//---------------------------------------------------------------------------
// Enregistrement des objets
//---------------------------------------------------------------------------

bool TFrame_SaisieDialog::EnrObjet(TElemBase *ElementParent, TElemBase *Element,
                                   int Left, int Top, int Width, int Height,
                                   TAnchors Anchors, bool bParentPotentiel) {

  SET_EVENT(Element, TNotifyEvent, OnChange, TFrame_SaisieDialog, this, ElemChange);
  Element->Selected = true;

  Element->Parent = ElementParent;
  Element->ParentPotentiel = bParentPotentiel;

	Reconst();

	return true;
}

//---------------------------------------------------------------------------
// Alignement des objets sélectionnés sur la grille
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignGrilleClick(TObject *Sender) {
  int i;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Alignement sur la grille"));

  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

      if (CheckBox_Aligner->Checked && ((AnsiString) Edit_GridSizeX->Text).ToInt() != 0) {

			  // Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeWidth;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Width;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

				GetElemTreeview(i)->Width = ((AnsiString) Edit_GridSizeX->Text).ToInt() * (int) ((double) GetElemTreeview(i)->Width / ((AnsiString) Edit_GridSizeX->Text).ToInt() + .5);
				GetElemTreeview(i)->Left = ((AnsiString) Edit_GridSizeX->Text).ToInt() * (int) ((double) GetElemTreeview(i)->Left / ((AnsiString) Edit_GridSizeX->Text).ToInt() + .5);

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeWidth;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Width;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			}
      if (CheckBox_Aligner->Checked && ((AnsiString) Edit_GridSizeY->Text).ToInt() != 0) {

			  // Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeHeight;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Height;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

				GetElemTreeview(i)->Height = ((AnsiString) Edit_GridSizeY->Text).ToInt() * (int) ((double) GetElemTreeview(i)->Height / ((AnsiString) Edit_GridSizeY->Text).ToInt() + .5);
				GetElemTreeview(i)->Top = ((AnsiString) Edit_GridSizeY->Text).ToInt() * (int) ((double) GetElemTreeview(i)->Top / ((AnsiString) Edit_GridSizeY->Text).ToInt() + .5);

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeHeight;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Height;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			}
    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la largeur des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_UnifWidthClick(TObject *Sender) {
  int i;
  int Width;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Uniformisation largeur"));

  Width = FObjSelected->Width;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		if (GetElemTreeview(i)->Selected) {

		  // Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeWidth;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Width;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			GetElemTreeview(i)->Width = Width;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeWidth;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Width;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

		}
  }

  FUndoRedo->EndOperation();

}


//---------------------------------------------------------------------------
// Uniformisation de la hauteur des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_UnifHeightClick(TObject *Sender) {
  int i;
  int Height;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Uniformisation hauteur"));

  Height = FObjSelected->Height;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		if (GetElemTreeview(i)->Selected) {

		  // Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeHeight;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Height;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			GetElemTreeview(i)->Height = Height;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeHeight;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Height;
			FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

		}
	}

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la taille des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_UnifSizeClick(TObject *Sender) {
  int i;
  int Width;
  int Height;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Uniformisation dimension"));

  Width = FObjSelected->Width;
  Height = FObjSelected->Height;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

		  // Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeWidth;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Width;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
			UndoChangeValue.ActionUndoDial = auChangeHeight;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Height;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			GetElemTreeview(i)->Width = Width;
      GetElemTreeview(i)->Height = Height;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeWidth;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Width;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
			UndoChangeValue.ActionUndoDial = auChangeHeight;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Height;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDepLeftClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Alignement sur la gauche"));

  BaseAlign = FObjSelected->Left;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			GetElemTreeview(i)->Left = BaseAlign;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

		}
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDepCenterHClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Alignement horizontal sur le centre"));

  BaseAlign = FObjSelected->Left + FObjSelected->Width / 2;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			GetElemTreeview(i)->Left = BaseAlign - GetElemTreeview(i)->Width / 2;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDepRightClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Alignement sur la droite"));

  BaseAlign = FObjSelected->Left + FObjSelected->Width;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			GetElemTreeview(i)->Left = BaseAlign - GetElemTreeview(i)->Width;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDepTopClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Alignement sur le haut"));

  BaseAlign = FObjSelected->Top + FObjSelected->Height;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			GetElemTreeview(i)->Top = BaseAlign - GetElemTreeview(i)->Height;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

		}
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDepCenterVClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Alignement vertical sur le centre"));

  BaseAlign = FObjSelected->Top + FObjSelected->Height / 2;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      GetElemTreeview(i)->Top = BaseAlign - GetElemTreeview(i)->Height / 2;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDepBottomClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Alignement sur le bas"));

  BaseAlign = FObjSelected->Top;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
		  FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      GetElemTreeview(i)->Top = BaseAlign;

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
		  FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDimLeftClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Dimensionnement sur la gauche"));

  BaseAlign = FObjSelected->Left;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
			FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
			UndoChangeValue.ActionUndoDial = auChangeWidth;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Width;
			FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      if (GetElemTreeview(i)->Left + GetElemTreeview(i)->Width > BaseAlign) {
        GetElemTreeview(i)->Width = GetElemTreeview(i)->Left + GetElemTreeview(i)->Width - BaseAlign;
        GetElemTreeview(i)->Left = BaseAlign;
      }
      else {
        GetElemTreeview(i)->Left = GetElemTreeview(i)->Left + GetElemTreeview(i)->Width;
        GetElemTreeview(i)->Width = BaseAlign - GetElemTreeview(i)->Left;
      }

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeLeft;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Left;
			FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
			UndoChangeValue.ActionUndoDial = auChangeWidth;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Width;
			FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDimRightClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Dimensionnement sur la droite"));

  BaseAlign = FObjSelected->Left + FObjSelected->Width;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {
      if (BaseAlign > GetElemTreeview(i)->Left) {

				// Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeWidth;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Width;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

        GetElemTreeview(i)->Width = BaseAlign - GetElemTreeview(i)->Left;

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeWidth;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Width;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      }
      else {

				// Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeWidth;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Width;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

        GetElemTreeview(i)->Width = GetElemTreeview(i)->Left - BaseAlign;
        GetElemTreeview(i)->Left = BaseAlign;

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeWidth;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Width;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      }
    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDimTopClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Dimensionnement sur le haut"));

  BaseAlign = FObjSelected->Top + FObjSelected->Height;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {
      if (BaseAlign > GetElemTreeview(i)->Top) {

				// Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

        GetElemTreeview(i)->Height = BaseAlign - GetElemTreeview(i)->Top;

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      }
      else {

				// Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeHeight;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Height;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

        GetElemTreeview(i)->Height = GetElemTreeview(i)->Top - BaseAlign;
        GetElemTreeview(i)->Top = BaseAlign;

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeHeight;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Height;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      }
    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignDimBottomClick(TObject *Sender) {
  int i;
  int BaseAlign;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Dimensionnement sur le bas"));

  BaseAlign = FObjSelected->Top;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) {

			// Mémorisation du Undo
			UndoChangeValue.ActionUndoDial = auChangeHeight;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Height;
			FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
			FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

      if (GetElemTreeview(i)->Top + GetElemTreeview(i)->Height > BaseAlign) {
        GetElemTreeview(i)->Height = GetElemTreeview(i)->Top + GetElemTreeview(i)->Height - BaseAlign;
        GetElemTreeview(i)->Top = BaseAlign;
      }
      else {
        GetElemTreeview(i)->Top = GetElemTreeview(i)->Top + GetElemTreeview(i)->Height;
        GetElemTreeview(i)->Height = BaseAlign - GetElemTreeview(i)->Top;
      }

			// Mémorisation du Redo
			UndoChangeValue.ActionUndoDial = auChangeHeight;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Height;
			FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
			UndoChangeValue.ActionUndoDial = auChangeTop;
			UndoChangeValue.Index = i;
			UndoChangeValue.Value = GetElemTreeview(i)->Top;
			FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

    }
  }

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignButtonsBottomClick(TObject *Sender) {
  int i;
	int TotalWidth;
	int NbButtons;
	TElemBase *ElemBaseParent;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Arrangement des boutons en bas"));

	TotalWidth = 0;
	NbButtons = 0;
	ElemBaseParent = NULL;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		if (GetElemTreeview(i)->Selected) {
			TotalWidth += GetElemTreeview(i)->Width;
			NbButtons++;
      ElemBaseParent = GetObjParent(GetElemTreeview(i));
		}
  }

	if (NbButtons != 0 && ElemBaseParent != NULL) {
		int TotalSpace, LeftSpace, MiddleSpace;
		int x;

		TRect ClientRect = ElemBaseParent->ClientRect;
		TotalSpace = ClientRect.Width - TotalWidth;
		if (TotalWidth < 20 * (NbButtons + 1)) {
			MiddleSpace = TotalWidth / (NbButtons + 1);
			LeftSpace = MiddleSpace;
		}
		else {
			MiddleSpace = 20;
			LeftSpace = (TotalSpace - MiddleSpace * (NbButtons - 1)) / 2;
		}
		x = LeftSpace;
		for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
			if (GetElemTreeview(i)->Selected) {

				// Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

				GetElemTreeview(i)->Left = x;
				x += GetElemTreeview(i)->Width + MiddleSpace;
				GetElemTreeview(i)->Top = ClientRect.Height - GetElemTreeview(i)->Height - 14;

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			}
		}

	}

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Uniformisation de la position des objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_AlignButtonsRightClick(TObject *Sender) {
	TElemBase *ElemBaseParent;
  int i;
	UNDODIALCHANGEVALUE UndoChangeValue;


	FUndoRedo->BeginOperation(0, _TT("Arrangement des boutons à droite"));

	ElemBaseParent = NULL;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		if (GetElemTreeview(i)->Selected) {
      ElemBaseParent = GetObjParent(GetElemTreeview(i));
			break;
		}
  }

	if (ElemBaseParent != NULL) {
		int y;

		TRect ClientRect = ElemBaseParent->ClientRect;
		y = 8;
		for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
			if (GetElemTreeview(i)->Selected) {

				// Mémorisation du Undo
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoUndo(this, &UndoChangeValue, sizeof(UndoChangeValue));

				GetElemTreeview(i)->Top = y;
				y += GetElemTreeview(i)->Height + 8;
				GetElemTreeview(i)->Left = ClientRect.Width - GetElemTreeview(i)->Width - 12;

				// Mémorisation du Redo
				UndoChangeValue.ActionUndoDial = auChangeLeft;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Left;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));
				UndoChangeValue.ActionUndoDial = auChangeTop;
				UndoChangeValue.Index = i;
				UndoChangeValue.Value = GetElemTreeview(i)->Top;
				FUndoRedo->MemoRedo(this, &UndoChangeValue, sizeof(UndoChangeValue));

			}
		}

	}

  FUndoRedo->EndOperation();

}

//---------------------------------------------------------------------------
// Suppression d'un objet
//
// Cette méthode supprime les objets sélectionnés
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_SupprimerClick(TObject *Sender) {
	UNDODIALCREATEDIALOG UndoCreateDialog;
	UNDODIALDELETEDIALOG UndoDeleteDialog;
	UNDODIALCREATEOBJ UndoCreateObj;
	UNDODIALDELETEOBJ UndoDeleteObj;
	LPUNDODIALCHANGEPROPERTY lpUndoChangeProperty;
	TTreeNode *TreeNode;
	int Level, LevelParent;
	int SizeUndoBuffer;
  TElemBase *Element;
  TElemBase *ElementParent;
	int IndexParent, Index;
	std::vector<TElemBase *> Elements;
  TStringList *ListeProprietes;
	AnsiString asProperty;
  AnsiString asValue;
  int i, j;


	// Mémorisation des items sélectionnés dans une première phase
	// sinon le DeleteObj provoque un changement de la sélection.
	// Si un élément est sélectionné, on prend également tous les noeuds enfants
	// pour pouvoir faire le undo/redo sur ces éléments
	LevelParent = -1;
	for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		TreeNode = TreeView_Forms->Items->Item[i];
		Level = TreeNode->Level;
		if (GetElemTreeview(i)->Selected) {
			// Elément sélectionné => on mémorise le Level pour sélectionner les enfants
			if (LevelParent > Level || LevelParent == -1) LevelParent = Level;
			Elements.push_back(GetElemTreeview(i));
		}
		else if (LevelParent >= Level) {
			// Elément non sélectionné et non enfant d'un noeud sélectionné
			// => on réinitialise LevelParent
			LevelParent = -1;
		}
		else if (LevelParent != -1) {
			// Elément enfant d'un noeud sélectionné (LevelParent < Level)
			Elements.push_back(GetElemTreeview(i));
		}
	}

	FUndoRedo->BeginOperation(0, _TT("Suppression de composants"));

	// Suppression des éléments (en commençant par la fin pour supprimer les éléments enfants en premier)
  for (j = (int) Elements.size() - 1; j >= 0; j--) {

    Element = Elements[j];
		ElementParent = GetObjParent(Element);
		Index = -1;
		IndexParent = -1;
		for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
			if (GetElemTreeview(i) == Element) Index = i;
			if (GetElemTreeview(i) == ElementParent) IndexParent = i;
			if (Index != -1 && IndexParent != -1) break;
		}

		// Mémorisation du Undo pour les propriétés
		ListeProprietes = new TStringList();
		Element->GetListProperties(ListeProprietes);
		for (i = 0; i < ListeProprietes->Count; i++) {

			asProperty = ListeProprietes->Strings[i];
			if (asProperty != _T("Left") &&
					asProperty != _T("Top") &&
					asProperty != _T("Width") &&
					asProperty != _T("Height") &&
					asProperty != _T("ClientWidth") &&
					asProperty != _T("ClientHeight") &&
					asProperty != _T("Anchors")) {

				asValue = Element->GetProperty(asProperty);

				SizeUndoBuffer = sizeof(UNDODIALCHANGEPROPERTY) + (int) asValue.length() * sizeof(TCHAR);
				lpUndoChangeProperty = (LPUNDODIALCHANGEPROPERTY) new BYTE[SizeUndoBuffer];

				lpUndoChangeProperty->ActionUndoDial = auChangeProperty;
				lpUndoChangeProperty->Index = Index;
				lstrcpynA(lpUndoChangeProperty->szPropertyName, asProperty.AsCharString(), 50);
				lstrcpy(lpUndoChangeProperty->szPropertyValue, asValue);
				FUndoRedo->MemoUndo(this, lpUndoChangeProperty, SizeUndoBuffer);

				delete[] lpUndoChangeProperty;

			}

		}
		delete ListeProprietes;

		if (ElementParent) {
			// Mémorisation du Undo
			UndoCreateObj.ActionUndoDial = auCreateObj;
			UndoCreateObj.IndexDialog = SelectedForm;
			UndoCreateObj.IndexParent = IndexParent;
			UndoCreateObj.NumeroElement = NumeroElement;
			UndoCreateObj.Left = Element->Left;
			UndoCreateObj.Top = Element->Top;
			UndoCreateObj.Width = Element->Width;
			UndoCreateObj.Height = Element->Height;
			UndoCreateObj.Anchors = Element->Anchors;
			lstrcpynA(UndoCreateObj.szTypeObj, ((AnsiString) Element->Type).AsCharString().c_str(), 30);
			FUndoRedo->MemoUndo(this, &UndoCreateObj, sizeof(UndoCreateObj));

			// Mémorisation du Redo
			UndoDeleteObj.ActionUndoDial = auDeleteObj;
			UndoDeleteObj.Index = Index;
			FUndoRedo->MemoRedo(this, &UndoDeleteObj, sizeof(UndoDeleteObj));

	    DeleteObj(ElementParent, Element);

		}
		else {
			// Mémorisation du Undo
			UndoCreateDialog.ActionUndoDial = auCreateDialog;
			UndoCreateDialog.IndexDialog = SelectedForm;
			UndoCreateDialog.NumeroElement = NumeroElement;
			UndoCreateDialog.Left = Element->Left;
			UndoCreateDialog.Top = Element->Top;
			UndoCreateDialog.Width = Element->Width;
			UndoCreateDialog.Height = Element->Height;
			FUndoRedo->MemoUndo(this, &UndoCreateDialog, sizeof(UndoCreateDialog));

			// Mémorisation du Redo
			UndoDeleteDialog.ActionUndoDial = auDeleteDialog;
			UndoDeleteDialog.IndexDialog = SelectedForm;
			FUndoRedo->MemoRedo(this, &UndoDeleteDialog, sizeof(UndoDeleteDialog));

			DeleteDialog(SelectedForm);

		}

	}

	FUndoRedo->EndOperation();

	Reconst();

}

//---------------------------------------------------------------------------
// Changement de l'ordre des composants
//
// Cette méthode traite l'évènement OnClick de l'item "Mettre avant"
//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::MenuItem_MettreAvantClick(TObject *Sender) {
	int Index1, Index2;
	TTreeNode *NodePrevSibling;
	int i;


	if (FObjSelected) {

		Index2 = GetIdTreeview(FObjSelected);
		NodePrevSibling = TreeView_Forms->Items->Item[Index2]->GetPrevSibling();
		if (NodePrevSibling == NULL) return;
		Index1 = NodePrevSibling->Index;

		if (SwapObjects(Index1, Index2)) {
			UNDODIALSWAP UndoSwap;

			FUndoRedo->BeginOperation(0, _TT("Mettre un objet avant"));

			// Mémorisation du Undo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoUndo(this, &UndoSwap, sizeof(UndoSwap));

			// Mémorisation du Redo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoRedo(this, &UndoSwap, sizeof(UndoSwap));

			FUndoRedo->EndOperation();

			for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
				TreeView_Forms->Items->Item[i]->Selected = (i == Index1);
      }
		  Reconst();

		}

	}

}

void FASTCALL TFrame_SaisieDialog::MenuItem_MettreApresClick(TObject *Sender) {
	int Index1, Index2;
	TTreeNode *NodeNextSibling;
	int i;


	if (FObjSelected) {

		Index1 = GetIdTreeview(FObjSelected);
		NodeNextSibling = TreeView_Forms->Items->Item[Index1]->GetNextSibling();
		if (NodeNextSibling == NULL) return;
		Index2 = NodeNextSibling->Index;

		if (SwapObjects(Index1, Index2)) {
			UNDODIALSWAP UndoSwap;

			FUndoRedo->BeginOperation(0, _TT("Mettre un objet après"));

			// Mémorisation du Undo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoUndo(this, &UndoSwap, sizeof(UndoSwap));

			// Mémorisation du Redo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoRedo(this, &UndoSwap, sizeof(UndoSwap));

			FUndoRedo->EndOperation();

      for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
				TreeView_Forms->Items->Item[i]->Selected = (i == Index2);
      }
		  Reconst();

		}

	}

}

//---------------------------------------------------------------------------
// Changement de la hiérarchie des composants
//
// Cette méthode traite l'évènement OnClick de l'item "Mettre comme enfant"
//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::MenuItem_MettreEnfantClick(TObject *Sender) {
	int Index1, Index2;
	TTreeNode *NodePrevSibling;
	int i;


	if (FObjSelected) {

		Index2 = GetIdTreeview(FObjSelected);
		NodePrevSibling = TreeView_Forms->Items->Item[Index2]->GetPrevSibling();
		if (NodePrevSibling == NULL) return;
		Index1 = NodePrevSibling->Index;

		if (SwapObjects(Index1, Index2)) {
			UNDODIALSWAP UndoSwap;

			FUndoRedo->BeginOperation(0, _TT("Mettre un objet comme enfant"));

			// Mémorisation du Undo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoUndo(this, &UndoSwap, sizeof(UndoSwap));

			// Mémorisation du Redo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoRedo(this, &UndoSwap, sizeof(UndoSwap));

			FUndoRedo->EndOperation();

			for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
				TreeView_Forms->Items->Item[i]->Selected = (i == Index1);
      }
		  Reconst();

		}

	}

}

void FASTCALL TFrame_SaisieDialog::MenuItem_MettreFrereClick(TObject *Sender) {
	int Index1, Index2;
	TTreeNode *NodeNextSibling;
	int i;


	if (FObjSelected) {

		Index1 = GetIdTreeview(FObjSelected);
		NodeNextSibling = TreeView_Forms->Items->Item[Index1]->GetNextSibling();
		if (NodeNextSibling == NULL) return;
		Index2 = NodeNextSibling->Index;

		if (SwapObjects(Index1, Index2)) {
			UNDODIALSWAP UndoSwap;

			FUndoRedo->BeginOperation(0, _TT("Mettre un objet comme frère"));

			// Mémorisation du Undo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoUndo(this, &UndoSwap, sizeof(UndoSwap));

			// Mémorisation du Redo
			UndoSwap.ActionUndoDial = auSwap;
			UndoSwap.Index1 = Index1;
			UndoSwap.Index2 = Index2;
			FUndoRedo->MemoRedo(this, &UndoSwap, sizeof(UndoSwap));

			FUndoRedo->EndOperation();

      for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
				TreeView_Forms->Items->Item[i]->Selected = (i == Index2);
      }
		  Reconst();

		}

	}

}

//---------------------------------------------------------------------------
// Affichage de la boîte de propriétés des objets
//
// Cette méthode traite l'évènement OnClick de l'item "Propriétés"
//---------------------------------------------------------------------------

void FASTCALL TFrame_SaisieDialog::MenuItem_ProprietesClick(TObject *Sender) {
	TElemBase * Element;
	std::vector<TElemBase *> Elements;
	int i;


  if (FObjSelected) {
		Element = FObjSelected;
		for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
			if (GetElemTreeview(i)->Selected) Elements.push_back(GetElemTreeview(i));
		}
	}
  else {
		Element = Dialogues[SelectedForm].Form;
		Elements.push_back(Element);
	}

	SaisieProprietes(Element, Elements);

}


//---------------------------------------------------------------------------
// Affichage du menu popup
//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::AffPopupMenu(void) {
  POINT MousePos;

  int i;
  int NbSelected;


  // Comptage des composants cliqués
  NbSelected = 0;
  for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
    if (GetElemTreeview(i)->Selected) NbSelected++;
  }

  // Options grisées en fonction du contexte
  if (((AnsiString) Edit_GridSizeX->Text).ToInt() != 0 && ((AnsiString) Edit_GridSizeY->Text).ToInt() != 0) MenuItem_AlignGrille->Enabled = true;
  else MenuItem_AlignGrille->Enabled = false;

  if (NbSelected == 0) {
    MenuItem_Unif->Enabled = false;
    MenuItem_AlignDep->Enabled = false;
    MenuItem_AlignDim->Enabled = false;
    MenuItem_Couper->Enabled = false;
    MenuItem_Copier->Enabled = false;
    MenuItem_Supprimer->Enabled = false;
    MenuItem_AlignButtons->Enabled = false;
  }
  else if (NbSelected == 1) {
    MenuItem_Unif->Enabled = false;
    MenuItem_AlignDep->Enabled = false;
    MenuItem_AlignDim->Enabled = false;
    MenuItem_Couper->Enabled = false;
    MenuItem_Copier->Enabled = true;
    MenuItem_Supprimer->Enabled = true;
    MenuItem_AlignButtons->Enabled = true;
  }
  else {
    MenuItem_Unif->Enabled = true;
    MenuItem_AlignDep->Enabled = true;
    MenuItem_AlignDim->Enabled = true;
    MenuItem_Couper->Enabled = true;
    MenuItem_Copier->Enabled = true;
    MenuItem_Supprimer->Enabled = true;
    MenuItem_AlignButtons->Enabled = true;
  }

  GetCursorPos(&MousePos);
  PopupMenu_ModifObjet->Popup(MousePos.x, MousePos.y);

  return true;
}

//---------------------------------------------------------------------------
// Un élément a été modifié
//---------------------------------------------------------------------------

void TFrame_SaisieDialog::ElemChange(TObject *Sender) {
  Reconst();
	Changed = true;
}

//---------------------------------------------------------------------------
// La sélection a été modifiée
//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieDialog::Form_SaisieDialogChangeSelection(TObject *Sender) {
	TElemBase * ElemBase;
	TTreeNode * TreeNode;
  int i;


	bNotTreeView_FormsChange = true;

	for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		TreeNode = TreeView_Forms->Items->Item[i];
		ElemBase = static_cast<TElemBase *>((void *) TreeNode->Data);
		TreeNode->Selected = ElemBase->Selected;
	}

	bNotTreeView_FormsChange = false;

}

//---------------------------------------------------------------------------
// Utilitaires
//---------------------------------------------------------------------------

bool TFrame_SaisieDialog::Reconst(void) {
  if (SelectedForm != -1) {
    if (Dialogues[SelectedForm].Form) Dialogues[SelectedForm].Form->Invalidate();
  }

  return true;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::SaisieProprietes(TElemBase *Element, std::vector<TElemBase *> &Elements) {
  TElemBase *ElemParent;
  int IdParent;
  TForm_Proprietes *Form_Proprietes;
  TStringList *ListeProprietesCommunes;
  TStringList *ListeProprietes;
  TYPEPROPERTY TypeProperty;
  AnsiString asProperty;
  AnsiString asValue;
  AnsiString asInfos;
  TLabel *Label;
  TEdit *Edit;
  TUpDown *UpDown;
  TPanel *Panel;
  TColorBox *ColorBox;
  TComboBox *ComboBox;
  TButton *Button;
  TMemo *Memo;
  TCheckBox *CheckBox;
  int ItemIndex;
  AnsiString asItem;
  ITEMPROPERTY NewItemProperty;
  std::vector<ITEMPROPERTY> ItemsProperties;
  TAnchors Anchors;
	TAnchors AnchorsElement;
	TAnchors AnchorsElements;
	AnsiString asAnchors;
	TColorBoxStyles Style;
  TColor Color;
  int TabOrderElement;

  int i, j, k, y;
  bool Ok = false;


  Form_Proprietes = new TForm_Proprietes(this, _T("TForm_Proprietes")); // Position et ancrages

	// Titre de la boîte
	AnsiString asCaption;
	AnsiString asLibObj;
	asCaption = _TT("Propriétés de ");
	for (k = 0; k < (int) Elements.size(); k++) {
		if (k) asCaption += _T(", ");
		asLibObj.sprintf(_T("%s %s"), (LPCTSTR) ((AnsiString) Elements[k]->ClassName), (LPCTSTR) ((AnsiString) Elements[k]->Name));
		asCaption += asLibObj;
	}
	Form_Proprietes->Caption = asCaption;

	if ((int) Elements.size() == 1) {

	  // Nom de l'objet
		Form_Proprietes->NomObj = Element->Name;

		// Position et ancrages
		IdParent = GetIdTreeview(GetObjParent(Element));
		if (IdParent != -1) ElemParent = GetElemTreeview(IdParent);
		else if (SelectedForm != -1) ElemParent = Dialogues[SelectedForm].Form;
		else ElemParent = NULL;

		Form_Proprietes->LeftElement = Element->Left;
		Form_Proprietes->TopElement = Element->Top;
		Form_Proprietes->WidthElement = Element->Width;
		Form_Proprietes->HeightElement = Element->Height;

		Form_Proprietes->AnchorsElement = Element->Anchors;
		AnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
		Form_Proprietes->AnchorsElements = AnchorsElements;

		if (ElemParent) {
			TRect Rect = ElemParent->ClientRect;
			Form_Proprietes->LeftParent = Rect.Left;
			Form_Proprietes->TopParent = Rect.Top;
			Form_Proprietes->WidthParent = Rect.Width;
			Form_Proprietes->HeightParent = Rect.Height;
		}

		Form_Proprietes->MajAncrages();
		Form_Proprietes->MajCoordonnees();

	}
	else {

		Form_Proprietes->AnchorsElement = Element->Anchors;

		// Ancrages communs à tous les éléments
		AnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
	  for (k = 0; k < (int) Elements.size(); k++) {
			if (Elements[k]->Anchors != Element->Anchors) {
				if ((Elements[k]->Anchors.Contains(akLeft) && !Element->Anchors.Contains(akLeft)) ||
						(!Elements[k]->Anchors.Contains(akLeft) && Element->Anchors.Contains(akLeft))) AnchorsElements >> akLeft;  
				if ((Elements[k]->Anchors.Contains(akTop) && !Element->Anchors.Contains(akTop)) ||
						(!Elements[k]->Anchors.Contains(akTop) && Element->Anchors.Contains(akTop))) AnchorsElements >> akTop;  
				if ((Elements[k]->Anchors.Contains(akRight) && !Element->Anchors.Contains(akRight)) ||
						(!Elements[k]->Anchors.Contains(akRight) && Element->Anchors.Contains(akRight))) AnchorsElements >> akRight;  
				if ((Elements[k]->Anchors.Contains(akBottom) && !Element->Anchors.Contains(akBottom)) ||
						(!Elements[k]->Anchors.Contains(akBottom) && Element->Anchors.Contains(akBottom))) AnchorsElements >> akBottom;  
			}
		}
		Form_Proprietes->AnchorsElements = AnchorsElements;
		Form_Proprietes->MajAncrages();

		// La position est (en général) différente : on supprime l'onglet
		delete Form_Proprietes->TabSheet_Position;

	}

  // Propriétés communes à tous les objets
  ListeProprietesCommunes = new TStringList();
  Element->GetListProperties(ListeProprietesCommunes);
  ListeProprietes = new TStringList();
	for (i = 0; i < ListeProprietesCommunes->Count; i++) {

		asProperty = ListeProprietesCommunes->Strings[i];

    if (asProperty == _T("Name") ||
        asProperty == _T("Left") ||
        asProperty == _T("Top") ||
        asProperty == _T("Width") ||
        asProperty == _T("Height") ||
        asProperty == _T("ClientWidth") ||
        asProperty == _T("ClientHeight") ||
        asProperty == _T("Anchors")) {
			ListeProprietesCommunes->Delete(i);
			i--;
		}

	}
  for (k = 0; k < (int) Elements.size(); k++) {

		if (Elements[k] != Element) {
			ListeProprietes->Clear();
			Elements[k]->GetListProperties(ListeProprietes);
			for (i = 0; i < ListeProprietesCommunes->Count; i++) {
				if (ListeProprietes->IndexOf(ListeProprietesCommunes->Strings[i]) == -1) {
					ListeProprietesCommunes->Delete(i);
					i--;
				}
			}
		}

	}
  delete ListeProprietes;

  // Propriétés
  y = 4;
  TabOrderElement = 0;
  Form_Proprietes->ScrollBox_Proprietes->DisableAutoRange();
  for (i = 0; i < ListeProprietesCommunes->Count; i++) {

    asProperty = ListeProprietesCommunes->Strings[i];

    Label = new TLabel(Form_Proprietes->ScrollBox_Proprietes);
		Label->Name = AnsiString(_T("Label_")) + asProperty;
    Label->Left = 4;
    Label->Top = y + 2;
    Label->Width = 102;
    Label->Height = 13;
    Label->Caption = asProperty;

    TypeProperty = Element->GetTypeProperty(asProperty, &asInfos);
		asValue = Element->GetProperty(asProperty);
		for (k = 0; k < (int) Elements.size(); k++) {
			if (Elements[k] != Element) {
				if (asValue != Elements[k]->GetProperty(asProperty)) {
					asValue = _T("- - - -");
					break;
				}
			}
		}
    NewItemProperty.Control = NULL;

    switch (TypeProperty) {

    case tpText:
      Edit = new TEdit(Form_Proprietes->ScrollBox_Proprietes);
			Edit->Name = AnsiString(_T("Edit_")) + asProperty;
      Edit->Left = 108;
      Edit->Top = y;
      Edit->Width = Form_Proprietes->ScrollBox_Proprietes->Width - 132;
      Edit->Height = 21;
      Edit->Text = asValue;
      Edit->TabStop = true;
      Edit->TabOrder = TabOrderElement++;
      Anchors.Clear() << akLeft << akTop << akRight;
      Edit->Anchors = Anchors;
      NewItemProperty.Control = Edit;
	    y += 23;
      break;

    case tpChar:
      Edit = new TEdit(Form_Proprietes->ScrollBox_Proprietes);
			Edit->Name = AnsiString(_T("Edit_")) + asProperty;
      Edit->Left = 108;
      Edit->Top = y;
      Edit->Width = 92;
      Edit->Height = 21;
      Edit->Text = asValue;
      Edit->MaxLength = 1;
      Edit->TabStop = true;
      Edit->TabOrder = TabOrderElement++;
      Anchors.Clear() << akLeft << akTop << akRight;
      Edit->Anchors = Anchors;
      NewItemProperty.Control = Edit;
	    y += 23;
      break;

    case tpNumber:
      Edit = new TEdit(Form_Proprietes->ScrollBox_Proprietes);
			Edit->Name = AnsiString(_T("Edit_")) + asProperty;
      Edit->Left = 108;
      Edit->Top = y;
      Edit->Width = 92;
      Edit->Height = 21;
      Edit->Text = asValue;
      Edit->TabStop = true;
      Edit->TabOrder = TabOrderElement++;
      NewItemProperty.Control = Edit;

      UpDown = new TUpDown(Form_Proprietes->ScrollBox_Proprietes);
			UpDown->Name = AnsiString(_T("UpDown_")) + asProperty;
      UpDown->Left = 200;
      UpDown->Top = y;
      UpDown->Width = 16;
      UpDown->Height = 21;
      UpDown->Position = asValue.ToInt();
      UpDown->Associate = Edit;
      UpDown->Increment = 1;
	    y += 23;
      break;

    case tpBool:
			CheckBox = new TCheckBox(Form_Proprietes->ScrollBox_Proprietes);
			CheckBox->Name = AnsiString(_T("CheckBox_")) + asProperty;
			CheckBox->Left = 108;
			CheckBox->Top = y + 2;
			CheckBox->Width = 13;
			CheckBox->Height = 13;
			CheckBox->Caption = _T("");
			if ((int) Elements.size() > 1) {
				CheckBox->AllowGrayed = true;
			}
      if (asValue == _T("True")) CheckBox->State = cbChecked;
      else if (asValue == _T("False")) CheckBox->State = cbUnchecked;
			else CheckBox->State = cbGrayed;

      NewItemProperty.Control = CheckBox;

			y += 23;

			break;

    case tpColor:
      ColorBox = new TColorBox(Form_Proprietes->ScrollBox_Proprietes);
			ColorBox->Name = AnsiString(_T("ColorBox_")) + asProperty;
      ColorBox->Left = 108;
      ColorBox->Top = y;
      ColorBox->Width = 192;
      ColorBox->Height = 21;
			Style.Clear() << cbStandardColors << cbExtendedColors << cbSystemColors << cbCustomColor << cbPrettyNames;
			ColorBox->Style = Style;
			if (asValue != _T("- - - -")) {
				Color = asValue;
				ColorBox->Selected = Color;
			}
      ColorBox->TabStop = true;
      ColorBox->TabOrder = TabOrderElement++;
      NewItemProperty.Control = ColorBox;
	    y += 23;
      break;

    case tpChoice:
      ComboBox = new TComboBox(Form_Proprietes->ScrollBox_Proprietes);
			ComboBox->Name = AnsiString(_T("ComboBox_")) + asProperty;
      ComboBox->Left = 108;
      ComboBox->Top = y;
      ComboBox->Width = 192;
      ComboBox->Height = 21;
      ComboBox->Style = csDropDownList;
      ComboBox->Items->SetTextStr(asInfos);
      ComboBox->ItemIndex = ComboBox->Items->IndexOf(asValue);
      ComboBox->TabStop = true;
      ComboBox->TabOrder = TabOrderElement++;
      NewItemProperty.Control = ComboBox;
	    y += 23;
      break;

    case tpMultipleChoice:
			Panel = new TPanel(Form_Proprietes->ScrollBox_Proprietes);
			Panel->Left = 108;
			Panel->Top = y;
			Panel->Width = 192;
      Panel->BevelOuter = bvLowered;
			Anchors.Clear() << akLeft << akTop << akRight;
			Panel->Anchors = Anchors;
			Panel->BorderStyle = bsNone;
      Panel->TabStop = true;
      Panel->TabOrder = TabOrderElement++;

			j = 0;
			while (asInfos.Length()) {
				k = asInfos.Pos(_T("\n"));
				if (k == 0) {
					asItem = asInfos;
					asInfos = _T("");
				}
				else {
					asItem = asInfos.SubString(1, k - 1);
					asInfos = asInfos.SubString(k + 1, asInfos.Length());
				}
				CheckBox = new TCheckBox(Panel);
				CheckBox->Left = 4;
				CheckBox->Top = 2 + 17 * j;
				CheckBox->Width = 184;
				CheckBox->Height = 13;
				CheckBox->Caption = asItem;
				if ((int) Elements.size() > 1) {
					CheckBox->AllowGrayed = true;
				}
				if (asValue == _T("- - - -")) {
					CheckBox->State = cbGrayed;
				}
				else {
					if (asValue.Pos(asItem) != 0) CheckBox->Checked = true;
				}
				j++;
			}

			Panel->Height = 17 * j + 4;
      NewItemProperty.Control = Panel;

      y += Panel->Height + 2;

			break;

    case tpNumbers:
      Memo = new TMemo(Form_Proprietes->ScrollBox_Proprietes);
			Memo->Name = AnsiString(_T("Memo_")) + asProperty;
      Memo->Left = 108;
      Memo->Top = y;
      Memo->Width = Form_Proprietes->ScrollBox_Proprietes->Width - 132;
      Memo->Height = 61;
      Memo->Text = asValue;
      Memo->ScrollBars = ssVertical;
      Anchors.Clear() << akLeft << akTop << akRight;
      Memo->Anchors = Anchors;
      Memo->TabStop = true;
      Memo->TabOrder = TabOrderElement++;
      NewItemProperty.Control = Memo;
	    y += 63;
      break;

    case tpStrings:
      Memo = new TMemo(Form_Proprietes->ScrollBox_Proprietes);
			Memo->Name = AnsiString(_T("Memo_")) + asProperty;
      Memo->Left = 108;
      Memo->Top = y;
      Memo->Width = Form_Proprietes->ScrollBox_Proprietes->Width - 132;
      Memo->Height = 61;
      Memo->Text = asValue;
      Memo->ScrollBars = ssVertical;
      Anchors.Clear() << akLeft << akTop << akRight;
      Memo->Anchors = Anchors;
      Memo->TabStop = true;
      Memo->TabOrder = TabOrderElement++;
      NewItemProperty.Control = Memo;
	    y += 63;
      break;

    case tpObject:
      ComboBox = new TComboBox(Form_Proprietes->ScrollBox_Proprietes);
			ComboBox->Name = AnsiString(_T("ComboBox_")) + asProperty;
      ComboBox->Left = 108;
      ComboBox->Top = y;
      ComboBox->Width = 192;
      ComboBox->Height = 21;
      ComboBox->Style = csDropDownList;
			ComboBox->Items->Add(_T(""));
			for (j = 1; j < (int) TreeView_Forms->Items->Count; j++) {
				asItem = GetElemTreeview(j)->Name;
				if (!asItem.IsEmpty()) ComboBox->Items->Add(asItem);
			}
      ComboBox->ItemIndex = ComboBox->Items->IndexOf(asValue);
      ComboBox->TabStop = true;
      ComboBox->TabOrder = TabOrderElement++;
      NewItemProperty.Control = ComboBox;
	    y += 23;
      break;

    case tpData:
      Edit = new TEdit(Form_Proprietes->ScrollBox_Proprietes);
			Edit->Name = AnsiString(_T("Edit_")) + asProperty;
      Edit->Left = 108;
      Edit->Top = y;
      Edit->Width = Form_Proprietes->ScrollBox_Proprietes->Width - 232;
      Edit->Height = 21;
      Edit->Text = asValue;
      Anchors.Clear() << akLeft << akTop << akRight;
      Edit->Anchors = Anchors;
      Edit->TabStop = true;
      Edit->TabOrder = TabOrderElement++;
			Edit->Hint = asInfos;
      NewItemProperty.Control = Edit;

      Button = new TButton(Form_Proprietes->ScrollBox_Proprietes);
			Button->Name = AnsiString(_T("Button_")) + asProperty;
      Button->Left = Form_Proprietes->ScrollBox_Proprietes->Width - 120;
      Button->Top = y;
      Button->Width = 96;
      Button->Height = 21;
      Button->Caption = _T("Parcourir");
      Button->TabStop = true;
      Button->TabOrder = TabOrderElement++;
      Anchors.Clear() << akTop << akRight;
      Button->Anchors = Anchors;
      SET_EVENT(Button, TNotifyEvent, OnClick, TForm_Proprietes, Form_Proprietes, Button_ParcourirClick);
	    y += 23;
      break;

    case tpEvent:
      Edit = new TEdit(Form_Proprietes->ScrollBox_Proprietes);
			Edit->Name = AnsiString(_T("Edit_")) + asProperty;
      Edit->Left = 108;
      Edit->Top = y;
      Edit->Width = Form_Proprietes->ScrollBox_Proprietes->Width - 232;
      Edit->Height = 21;
      Edit->Text = asValue;
      Anchors.Clear() << akLeft << akTop << akRight;
      Edit->Anchors = Anchors;
      Edit->TabStop = true;
      Edit->TabOrder = TabOrderElement++;
			Edit->Hint = asInfos;
      NewItemProperty.Control = Edit;

      Button = new TButton(Form_Proprietes->ScrollBox_Proprietes);
			Button->Name = AnsiString(_T("Button_")) + asProperty;
      Button->Left = Form_Proprietes->ScrollBox_Proprietes->Width - 120;
      Button->Top = y;
      Button->Width = 96;
      Button->Height = 21;
      Button->Caption = _T("Code");
      Button->TabStop = true;
      Button->TabOrder = TabOrderElement++;
      Anchors.Clear() << akTop << akRight;
      Button->Anchors = Anchors;
      SET_EVENT(Button, TNotifyEvent, OnClick, TForm_Proprietes, Form_Proprietes, Button_CodeClick);
	    y += 23;
      break;

    }

    NewItemProperty.TypeProperty = TypeProperty;
    ItemsProperties.push_back(NewItemProperty);

  }
  Form_Proprietes->ScrollBox_Proprietes->EnableAutoRange();

  // Affichage de la boîte de saisie
  if (Form_Proprietes->ShowModal() == mrOk) {

		// Récupération des modifs

		FUndoRedo->BeginOperation(0, _TT("Modification de propriétés"));

		if ((int) Elements.size() == 1) {
			ChangePropriete(Element, _T("Name"), Form_Proprietes->NomObj);
			ChangePropriete(Element, _T("Left"), AnsiString((int) Form_Proprietes->LeftElement));
			ChangePropriete(Element, _T("Top"), AnsiString((int) Form_Proprietes->TopElement));
			ChangePropriete(Element, _T("Width"), AnsiString((int) Form_Proprietes->WidthElement));
			ChangePropriete(Element, _T("Height"), AnsiString((int) Form_Proprietes->HeightElement));
			AnchorsElement = Form_Proprietes->AnchorsElement;
			if (AnchorsElement.Contains(akLeft)) asAnchors += _T("akLeft\n");
			if (AnchorsElement.Contains(akTop)) asAnchors += _T("akTop\n");
			if (AnchorsElement.Contains(akRight)) asAnchors += _T("akRight\n");
			if (AnchorsElement.Contains(akBottom)) asAnchors += _T("akBottom\n");
			ChangePropriete(Element, _T("Anchors"), FormatProperty(asAnchors, _T("["), _T("]"), _T(", ")));
		}
		else {
			AnchorsElement = Form_Proprietes->AnchorsElement;
			AnchorsElements = Form_Proprietes->AnchorsElements;
			for (k = 0; k < (int) Elements.size(); k++) {
				Anchors = Elements[k]->Anchors;
				asAnchors = _T("");

				if (AnchorsElements.Contains(akLeft)) {
					if (AnchorsElement.Contains(akLeft)) asAnchors += _T("akLeft\n");
				}
				else {
					if (Anchors.Contains(akLeft)) asAnchors += _T("akLeft\n");
				}

				if (AnchorsElements.Contains(akTop)) {
					if (AnchorsElement.Contains(akTop)) asAnchors += _T("akTop\n");
				}
				else {
					if (Anchors.Contains(akTop)) asAnchors += _T("akTop\n");
				}

				if (AnchorsElements.Contains(akRight)) {
					if (AnchorsElement.Contains(akRight)) asAnchors += _T("akRight\n");
				}
				else {
					if (Anchors.Contains(akRight)) asAnchors += _T("akRight\n");
				}

				if (AnchorsElements.Contains(akBottom)) {
					if (AnchorsElement.Contains(akBottom)) asAnchors += _T("akBottom\n");
				}
				else {
					if (Anchors.Contains(akBottom)) asAnchors += _T("akBottom\n");
				}

				ChangePropriete(Elements[k], _T("Anchors"), FormatProperty(asAnchors, _T("["), _T("]"), _T(", ")));
			}
		}

    for (i = 0, j = 0; i < ListeProprietesCommunes->Count; i++) {

      asProperty = ListeProprietesCommunes->Strings[i];

      if (ItemsProperties[j].Control) {

        switch (ItemsProperties[j].TypeProperty) {
        case tpText:
        case tpChar:
        case tpNumber:
        case tpEvent:
        case tpData:
          Edit = (TEdit *) ItemsProperties[j].Control;
          asValue = Edit->Text;
          break;
        case tpBool:
          CheckBox = (TCheckBox *) ItemsProperties[j].Control;
					if (CheckBox->State != cbGrayed) {
						if (CheckBox->State == cbChecked) asValue = _T("True");
						else asValue = _T("False");
					}
					else asValue = _T("- - - -");
          break;
        case tpColor:
          ColorBox = (TColorBox *) ItemsProperties[j].Control;
          Color = ColorBox->Selected;
					if (Color != TColor(0xFFFFFFFF)) asValue = Color.ColorToString();
          else asValue = _T("- - - -");
          break;
        case tpChoice:
        case tpObject:
          ComboBox = (TComboBox *) ItemsProperties[j].Control;
          ItemIndex = ComboBox->ItemIndex;
          if (ItemIndex != -1) asValue = ComboBox->Items->Strings[ItemIndex];
          else asValue = _T("- - - -");
          break;
        case tpMultipleChoice:
          Panel = (TPanel *) ItemsProperties[j].Control;
					asValue = _T("[");
					for (k = 0; k < Panel->ComponentCount; k++) {
						CheckBox = dynamic_cast<TCheckBox *> ((TComponent *) Panel->Components[k]);
						if (CheckBox) {
							if (CheckBox->State == cbChecked) {
								if (asValue.Length() > 1) asValue += _T(", ");
								asValue += CheckBox->Caption;
							}
							else if (CheckBox->State == cbGrayed) {
								asValue = _T("- - - -");
								break;
							}
						}
					}
					if (asValue != _T("- - - -")) asValue += _T("]");
          break;
        case tpNumbers:
          Memo = (TMemo *) ItemsProperties[j].Control;
          asValue = Memo->Text;
          break;
        case tpStrings:
          Memo = (TMemo *) ItemsProperties[j].Control;
          asValue = Memo->Text;
          break;
        }

				for (k = 0; k < (int) Elements.size(); k++) {
					if (asValue != _T("- - - -")) {
						ChangePropriete(Elements[k], asProperty, asValue);
					}
				}

      }

      j++;

    }

		FUndoRedo->EndOperation();

  }
  else {
    Ok = false;
  }

  Reconst();

  delete ListeProprietesCommunes;
  delete Form_Proprietes;

  return Ok;
}

//---------------------------------------------------------------------------
// Effacement des données en mémoire
//---------------------------------------------------------------------------
void TFrame_SaisieDialog::Clear(void) {
  int i;

  for (i = 0; i < (int) Dialogues.size(); i++) {
    delete Dialogues[i].Form;
  }
	Dialogues.clear();

	TreeView_Forms->Items->Clear();

	FUndoRedo->Clear();

	FObjSelected = NULL;
  FTypeACreer = _T("");
  NumeroElement = 0;
	SelectedForm = -1;
  NbComponents = 0;
	bNotTreeView_FormsChange = false;
	FChanged = false;
}

//---------------------------------------------------------------------------
// Modification d'une propriété
//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::ChangePropriete(TElemBase *Element, AnsiString asProperty, AnsiString asValue) {
	LPUNDODIALCHANGEPROPERTY lpUndoChangeProperty;
	int SizeBuffer, SizeUndoBuffer, SizeRedoBuffer;
	AnsiString asOldValue;
	int i;


	asOldValue = Element->GetProperty(asProperty);
	if (asValue != asOldValue) {

		SizeUndoBuffer = sizeof(UNDODIALCHANGEPROPERTY) + (int) asOldValue.length() * sizeof(TCHAR);
		SizeRedoBuffer = sizeof(UNDODIALCHANGEPROPERTY) + (int) asValue.length() * sizeof(TCHAR);
		if (SizeRedoBuffer <= SizeUndoBuffer) SizeBuffer = SizeUndoBuffer;
		else SizeBuffer = SizeRedoBuffer;
		lpUndoChangeProperty = (LPUNDODIALCHANGEPROPERTY) new BYTE[SizeBuffer];

		// Mémorisation du Redo
		lpUndoChangeProperty->ActionUndoDial = auChangeProperty;
		lpUndoChangeProperty->Index = GetIdTreeview(Element);
		lstrcpynA(lpUndoChangeProperty->szPropertyName, asProperty.AsCharString(), 50);
		lstrcpy(lpUndoChangeProperty->szPropertyValue, asValue);
		FUndoRedo->MemoRedo(this, lpUndoChangeProperty, SizeRedoBuffer);

		// Mémorisation du Undo
		lpUndoChangeProperty->ActionUndoDial = auChangeProperty;
		lpUndoChangeProperty->Index = GetIdTreeview(Element);
		lstrcpynA(lpUndoChangeProperty->szPropertyName, asProperty.AsCharString(), 50);
		lstrcpy(lpUndoChangeProperty->szPropertyValue, asOldValue);
		FUndoRedo->MemoUndo(this, lpUndoChangeProperty, SizeUndoBuffer);

		Element->SetProperty(asProperty, asValue);

		if (asProperty == _T("Name")) {
			for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
				if (GetElemTreeview(i) == Element) {
					TreeView_Forms->Items->Item[i]->Text = asValue;
					break;
				}
			}
		}

		delete[] lpUndoChangeProperty;

	}

	return true;
}


//---------------------------------------------------------------------------
// Création d'un objet
//---------------------------------------------------------------------------
TElemBase *TFrame_SaisieDialog::CreationObjet(TElemBase *ElementParent,
           int Left, int Top, int Width, int Height,
           TAnchors Anchors) {
	UNDODIALCREATEOBJ UndoCreateObj;
	UNDODIALDELETEOBJ UndoDeleteObj;
  TElemBase * NewElement = NULL;
	TTreeNode *TreeNode;
	TElemBase *ElemBase;
	int IndexParent, Index;
	int i;


  if (SelectedForm != -1 && !FTypeACreer.IsEmpty()) {

		if (ElementParent == NULL) ElementParent = Dialogues[SelectedForm].Form;
    NewElement = CreateObj(ElementParent, SelectedForm, ++NumeroElement, Left, Top, Width, Height, Anchors, FTypeACreer);

		if (NewElement) {

			Index = -1;
			IndexParent = -1;
			for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
				if (GetElemTreeview(i) == ElementParent) {
					IndexParent = i;
					if (Index != -1) break;
				}
				if (GetElemTreeview(i) == NewElement) {
					Index = i;
					if (IndexParent != -1) break;
				}
			}

			FUndoRedo->BeginOperation(0, _TT("Création d'un objet ") + FTypeACreer);

			// Mémorisation du Redo
			UndoCreateObj.ActionUndoDial = auCreateObj;
			UndoCreateObj.IndexDialog = SelectedForm;
			UndoCreateObj.IndexParent = IndexParent;
			UndoCreateObj.NumeroElement = NumeroElement;
			UndoCreateObj.Left = Left;
			UndoCreateObj.Top = Top;
			UndoCreateObj.Width = Width;
			UndoCreateObj.Height = Height;
			UndoCreateObj.Anchors = Anchors;
			lstrcpynA(UndoCreateObj.szTypeObj, FTypeACreer.AsCharString(), 30);
			FUndoRedo->MemoRedo(this, &UndoCreateObj, sizeof(UndoCreateObj));

			// Mémorisation du Undo
			UndoDeleteObj.ActionUndoDial = auDeleteObj;
			UndoDeleteObj.Index = Index;
			FUndoRedo->MemoUndo(this, &UndoDeleteObj, sizeof(UndoDeleteObj));

			FUndoRedo->EndOperation();

      // Sélection du nouveau composant
			for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
				TreeNode = TreeView_Forms->Items->Item[i];
				ElemBase = static_cast<TElemBase *>((void *) TreeNode->Data);
				if (ElemBase == NewElement) {
					TreeNode->Selected = true;
				}
				else {
					TreeNode->Selected = false;
				}
			}

      // Appel automatique des propriétés
			std::vector<TElemBase *> Elements;
			Elements.push_back(NewElement);
			SaisieProprietes(NewElement, Elements);

		}
  }

  return NewElement;
}

//---------------------------------------------------------------------------
// Création d'un dialogue
//---------------------------------------------------------------------------
TElemBase *TFrame_SaisieDialog::CreateDialog2(int IndexDialog, int NumeroElement, int Left, int Top, int Width, int Height) {
	AnsiString asName;
  TElemBase * NewElement;


	asName = _T("TForm") + AnsiString(NumeroElement);
	NewElement = new TElemVCL(NULL, NULL);
	if (!NewElement->Create(_T("TForm"))) return NULL;
	NewElement->Left = Left;
	NewElement->Top = Top;
	NewElement->Width = Width;
	NewElement->Height = Height;
	NewFormInit(this, NewElement);

	Changed = true;

  return NewElement;
}

//---------------------------------------------------------------------------
// Suppression d'un dialogue
//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::DeleteDialog(int IndexDialog) {

	// Mise à jour du TreeView
	Dialogues[IndexDialog].TreeNode->Delete();

	delete Dialogues[IndexDialog].Form;

	Dialogues.erase(Dialogues.begin() + IndexDialog);
	SelectedForm = -1;

	// MAJ nombre de composants
	Label_NbDialogues->Caption = AnsiString((int) Dialogues.size());

	Changed = true;

  return true;
}

//---------------------------------------------------------------------------
// Création d'un objet
//---------------------------------------------------------------------------
TElemBase *TFrame_SaisieDialog::CreateObj(TElemBase *ElementParent, int IndexDialog, int NumeroElement, int Left, int Top, int Width, int Height, TAnchors Anchors, AnsiString asTypeObj) {
	AnsiString asName;
  TElemBase * NewElement;
	int IndexParent, Index;
	int i;


	if (asTypeObj.IsEmpty()) return NULL;
	if (ElementParent == NULL) return NULL;

	NewElement = ElementParent->NewChild(asTypeObj);
	if (NewElement) {
		asName = asTypeObj.SubString(2, asTypeObj.Length() - 1) + AnsiString(NumeroElement);
		IndexParent = -1;
		for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
			if (GetElemTreeview(i) == ElementParent) {
				IndexParent = i;
				break;
			}
		}
		Index = (int) TreeView_Forms->Items->Count;

		NewElement->Left = Left;
		NewElement->Top = Top;
		NewElement->Width = Width;
		NewElement->Height = Height;
		NewElement->Anchors = Anchors;
		NewElement->Name = asName;
		NewElement->SetProperty(_T("Caption"), asTypeObj);

		NewComponentInit(this, Dialogues[IndexDialog].Form, ElementParent, NewElement);

		Dialogues[IndexDialog].Form->Invalidate();

	}

	Changed = true;

  return NewElement;
}

//---------------------------------------------------------------------------
// Suppression d'un objet
//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::DeleteObj(TElemBase *ElementParent, TElemBase *Element) {
	TTreeNode * TreeNode;
	int i;


	if (ElementParent == NULL) return false;

	// Mise à jour du TreeView
	for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		TreeNode = TreeView_Forms->Items->Item[i];
		if (TreeNode->Data == Element) {
			TreeNode->Delete();
			break;
		}
	}

	delete Element;

	Dialogues[GetIndexDialog(ElementParent)].Form->Invalidate();

	// MAJ nombre de composants
	Label_NbControles->Caption = AnsiString(--NbComponents);

	Changed = true;

  return true;
}

//---------------------------------------------------------------------------
// Echange de deux objets
//
// Cette méthode echange les composants Index1 et Index2
//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::SwapObjects(int Index1, int Index2) {
  TElemBase *Element1;
  TElemBase *Element2;
  TElemBase *ElementParent;
  TTreeNode *TreeNode;
  TTreeNode *TreeNode1;
  TTreeNode *TreeNode2;
  TTreeNode *TreeNode1Bis;
  TTreeNode *TreeNode2Bis;
	int i;


	if (Index1 < 1 || Index1 >= (int) TreeView_Forms->Items->Count) return false;
	if (Index2 < 1 || Index2 >= (int) TreeView_Forms->Items->Count) return false;

	Element1 = GetElemTreeview(Index1);
	Element2 = GetElemTreeview(Index2);
	if (Element1->Parent != Element2->Parent) return false;
	ElementParent = Element1->Parent;

	// Inversion dans le TreeView
	TreeNode1 = NULL;
	TreeNode2 = NULL;
	for (i = 1; i < (int) TreeView_Forms->Items->Count; i++) {
		TreeNode = TreeView_Forms->Items->Item[i];
		if (TreeNode->Data == Element1) TreeNode1 = TreeNode;
		if (TreeNode->Data == Element2) TreeNode2 = TreeNode;
	}
	if (TreeNode1 == NULL || TreeNode2 == NULL) return false;

	TreeNode1Bis = TreeView_Forms->Items->AddNode(NULL, TreeNode2, TreeNode1->Text, NULL, naInsert);
	TreeNode2Bis = TreeView_Forms->Items->AddNode(NULL, TreeNode1, TreeNode2->Text, NULL, naInsert);
	TreeView_Forms->Items->CopyNode(TreeNode1, TreeNode1Bis);
	TreeView_Forms->Items->CopyNode(TreeNode2, TreeNode2Bis);
	TreeView_Forms->Items->Delete(TreeNode1);
	TreeView_Forms->Items->Delete(TreeNode2);

	// Inversion dans l'arborescence des composants
	if (ElementParent) ElementParent->SwapComponents(Element1, Element2);

  Changed = true;

	return true;
}

//---------------------------------------------------------------------------
// Déplacement d'un objet
//
// Cette méthode déplace l'objet Index sous IndexParent et après IndexInsertAfter
//---------------------------------------------------------------------------
bool TFrame_SaisieDialog::MoveObject(int Index, int IndexParent, int IndexInsertAfter) {
	TTreeNode *TreeNode, *TreeNodeParent, *TreeNodeInsertAfter, *NewTreeNode;
	TElemBase *ElemBase, *ElemBaseParent;


	TreeNode = TreeView_Forms->Items->Item[Index];
	TreeNodeParent = TreeView_Forms->Items->Item[IndexParent];

	if (IndexInsertAfter == -1) {
		NewTreeNode = TreeView_Forms->Items->AddChildFirst(TreeNodeParent, TreeNode->Text);
	}
	else if (IndexInsertAfter == -2) {
		NewTreeNode = TreeView_Forms->Items->AddChild(TreeNodeParent, TreeNode->Text);
	}
	else {
		TreeNodeInsertAfter = TreeView_Forms->Items->Item[IndexInsertAfter];
		NewTreeNode = TreeView_Forms->Items->AddNode(NULL, TreeNodeInsertAfter, TreeNode->Text, NULL, naInsert);
	}

	if (TreeView_Forms->Items->CopyNode(TreeNode, NewTreeNode)) {
		ElemBase = static_cast<TElemBase *>((void *) TreeNode->Data);
		ElemBaseParent = static_cast<TElemBase *>((void *) TreeNodeParent->Data);
		if (ElemBase && ElemBaseParent) {
			ElemBase->Parent = ElemBaseParent;
		}
		TreeNode->Delete();
	}

	return true;
}

//---------------------------------------------------------------------------
TElemBase *TFrame_SaisieDialog::GetElemTreeview(int Index) {
	TTreeNode *TreeNode;
	TElemBase *ElemBase;

	TreeNode = TreeView_Forms->Items->Item[Index];
	if (TreeNode == NULL) return NULL;

	ElemBase = static_cast<TElemBase *>((void *) TreeNode->Data);

	return ElemBase;
}

//---------------------------------------------------------------------------
FVCL_RegisterComponent(TFrame_SaisieDialog);

//---------------------------------------------------------------------------
