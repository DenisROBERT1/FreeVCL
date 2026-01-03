//-----------------------------------------------------------------------------
//! @file TTreeView.cpp
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


//---------------------------------------------------------------------------
// TTreeView
//---------------------------------------------------------------------------

TTreeView::TTreeView(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TTreeView");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_EDITLABELS;
  FExWindowStyle = WS_EX_CLIENTEDGE;
  FTabStop = true;
  FColor = clWindow;
	FAutoExpand = false;
  FBorderStyle = bsNone;
  FChangeDelay = 0;
  FDropTarget = NULL;
  FHotTrack = false;
  FImages = NULL;
	FItems = new TTreeNodes(this);
  FMultiSelectStyle = msControlSelect;
  FRightClickSelect = false;
  FRowSelect = false;
  FSelectionCount = 0;
  FSortType = stNone;
  FStateImages = NULL;
  FToolTips = false;
  FTopItem = NULL;

  // Enregistrement de la classe TreeView
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_TREEVIEW_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_TREEVIEW /*"SysTreeView32"*/, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    SendMessage(FHandle, TVM_SETBKCOLOR, 0, (LPARAM)(COLORREF) FColor.cr);
    FItems->Handle = FHandle;

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TTreeView::~TTreeView(void) {
  delete FItems;
}

//---------------------------------------------------------------------------
/* Inutile jusqu'à présent
//---------------------------------------------------------------------------
void FASTCALL TTreeView::DestroyWnd(void) {
  DestroyWindow(FHandle);
	FHandle = NULL;
}

//---------------------------------------------------------------------------
void FASTCALL TTreeView::CreateWnd(void) {
  HFONT hFont;
  HWND hWndParent = NULL;


  if (FOwner) hWndParent = ((TControl *) FOwner)->Handle;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
													 WC_TREEVIEW / *"SysTreeView32"* /, FCaption,
													 FWindowStyle,
													 FLeft, FTop,
													 FWidth, FHeight,
													 hWndParent,
													 (HMENU) (int) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    FItems->Handle = FHandle;
    InitStdControl();
  }

} */

//---------------------------------------------------------------------------
int TTreeView::ProcessNotify(LPNMHDR pnmh) {

  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
  case NM_CLICK:
		if (!FComponentState.Contains(csLoading)) {
			if (FAction) FAction->Execute();
			OnClick(this);
		}
    return TRUE;
  case NM_RCLICK:
		{
			POINT Pt;
			GetCursorPos(&Pt);
			if (FRightClickSelect) {
				TVHITTESTINFO tvhti;
				tvhti.pt = ScreenToClient(Pt);
				TreeView_HitTest(FHandle, &tvhti);
				if (tvhti.flags & TVHT_ONITEM) {
					TreeView_SelectItem(FHandle, tvhti.hItem);
				}
			}
			ProcessContextMenu(Pt.x, Pt.y);
		}
		break;
  case TVN_ITEMEXPANDING:
		if (!FComponentState.Contains(csLoading)) {
			LPNMTREEVIEW pNmtv;
			pNmtv = (LPNMTREEVIEW) pnmh;
			TTreeNode *TreeNode = reinterpret_cast<TTreeNode*> (pNmtv->itemNew.lParam);
      if (pNmtv->action == TVE_EXPAND) OnExpanding(this, TreeNode);
			else OnCollapsing(this, TreeNode);
		}
    return TRUE;
  case TVN_ITEMEXPANDED:
		if (!FComponentState.Contains(csLoading)) {
			LPNMTREEVIEW pNmtv;
			pNmtv = (LPNMTREEVIEW) pnmh;
			TTreeNode *TreeNode = reinterpret_cast<TTreeNode*> (pNmtv->itemNew.lParam);
      if (pNmtv->action == TVE_EXPAND) OnExpanded(this, TreeNode);
			else OnCollapsed(this, TreeNode);
		}
    return TRUE;
  case TVN_SELCHANGED:
		if (!FComponentState.Contains(csLoading)) {
			LPNMTREEVIEW pNmtv;
			pNmtv = (LPNMTREEVIEW) pnmh;
			TTreeNode *TreeNode = reinterpret_cast<TTreeNode*> (pNmtv->itemNew.lParam);
			OnChange(this, TreeNode);
		}
    return TRUE;
	case TVN_BEGINLABELEDIT:
		if (!FComponentState.Contains(csLoading)) {
			LPNMTVDISPINFO ptvdi;
			ptvdi = (LPNMTVDISPINFO) pnmh;
			if (ptvdi->item.pszText != NULL) {
				TTreeNode *TreeNode;
				TVITEM TVItem;
				bool AllowEdit;
				TVItem.mask = TVIF_HANDLE | TVIF_PARAM;
				TVItem.hItem = ptvdi->item.hItem;
				if (TreeView_GetItem(FHandle, &TVItem)) {
				  AnsiString asNewLabel;
					TreeNode = (TTreeNode *) TVItem.lParam;
					AllowEdit = true;
		      OnEditing(this, TreeNode, AllowEdit);
					if (!AllowEdit) {
						HWND hWndEditControl;
						hWndEditControl = TreeView_GetEditControl(FHandle);
						if (hWndEditControl) SendMessage(hWndEditControl, WM_CLOSE, 0, 0L);
					}
				}
			}
		}
    return TRUE;
	case TVN_ENDLABELEDIT:
		if (!FComponentState.Contains(csLoading)) {
			LPNMTVDISPINFO ptvdi;
			ptvdi = (LPNMTVDISPINFO) pnmh;
			if (ptvdi->item.pszText != NULL) {
				TTreeNode *TreeNode;
		    TreeView_SetItem(FHandle, &ptvdi->item);
				TVITEM TVItem;
				TVItem.mask = TVIF_HANDLE | TVIF_PARAM;
				TVItem.hItem = ptvdi->item.hItem;
				if (TreeView_GetItem(FHandle, &TVItem)) {
				  AnsiString asNewLabel;
					TreeNode = (TTreeNode *) TVItem.lParam;
					asNewLabel = ptvdi->item.pszText;
          OnEdited(this, TreeNode, asNewLabel);
				}
			}
		}
    return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AutoExpand
//---------------------------------------------------------------------------

bool TTreeView::Get_AutoExpand(void) {
  return ((FWindowStyle & TVS_SINGLEEXPAND) != 0);
}

bool TTreeView::Set_AutoExpand(bool NewAutoExpand) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
	if (NewAutoExpand) NewWindowStyle |= TVS_SINGLEEXPAND;
  else NewWindowStyle &= (DWORD) ~(TVS_SINGLEEXPAND);
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TBorderStyle TTreeView::Get_BorderStyle(void) {
  return FBorderStyle;
}

bool TTreeView::Set_BorderStyle(TBorderStyle NewBorderStyle) {
  if (FBorderStyle != NewBorderStyle) {
    FBorderStyle = NewBorderStyle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ChangeDelay
//---------------------------------------------------------------------------

int TTreeView::Get_ChangeDelay(void) {
  return FChangeDelay;
}

bool TTreeView::Set_ChangeDelay(int NewChangeDelay) {
  if (FChangeDelay != NewChangeDelay) {
    FChangeDelay = NewChangeDelay;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CheckBoxes
//---------------------------------------------------------------------------

bool TTreeView::Get_CheckBoxes(void) {
  return ((FWindowStyle & TVS_CHECKBOXES) != 0);
}

bool TTreeView::Set_CheckBoxes(bool NewCheckBoxes) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
	if (NewCheckBoxes) NewWindowStyle |= TVS_CHECKBOXES;
  else NewWindowStyle &= (DWORD) ~(TVS_CHECKBOXES);
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DropTarget
//---------------------------------------------------------------------------

TTreeNode *TTreeView::Get_DropTarget(void) {
  return FDropTarget;
}

bool TTreeView::Set_DropTarget(TTreeNode *NewDropTarget) {
  if (FDropTarget != NewDropTarget) {
    FDropTarget = NewDropTarget;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HideSelection
//---------------------------------------------------------------------------

bool TTreeView::Get_HideSelection(void) {
  return ((FWindowStyle & TVS_SHOWSELALWAYS) == 0);
}

bool TTreeView::Set_HideSelection(bool NewHideSelection) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewHideSelection) NewWindowStyle &= (DWORD) ~TVS_SHOWSELALWAYS;
  else NewWindowStyle |= TVS_SHOWSELALWAYS;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HotTrack
//---------------------------------------------------------------------------

bool TTreeView::Get_HotTrack(void) {
  return FHotTrack;
}

bool TTreeView::Set_HotTrack(bool NewHotTrack) {
  if (FHotTrack != NewHotTrack) {
    FHotTrack = NewHotTrack;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Images
//---------------------------------------------------------------------------

TImageList *TTreeView::Get_Images(void) {
  return FImages;
}

bool TTreeView::Set_Images(TImageList *NewImages) {
  if (FImages != NewImages) {
    FImages = NewImages;
		if (FHandle) {
			TreeView_SetImageList(FHandle, FImages->Handle, TVSIL_NORMAL);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Indent
//---------------------------------------------------------------------------

int TTreeView::Get_Indent(void) {
  return TreeView_GetIndent(FHandle);
}

bool TTreeView::Set_Indent(int NewIndent) {
  if (FHandle) {
    TreeView_SetIndent(FHandle, NewIndent);
    return true;
  }

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Items
//---------------------------------------------------------------------------

TTreeNodes *TTreeView::Get_Items(void) {
  return FItems;
}

bool TTreeView::Set_Items(TTreeNodes *NewItems) {
  if (FItems != NewItems) {
    FItems = NewItems;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiSelect
//---------------------------------------------------------------------------

bool TTreeView::Get_MultiSelect(void) {
  return ((FExWindowStyle & TVS_EX_MULTISELECT) == 0);
}

bool TTreeView::Set_MultiSelect(bool NewMultiSelect) {

	if (NewMultiSelect) {
		TreeView_SetExtendedStyle(FHandle, TVS_EX_MULTISELECT, TVS_EX_MULTISELECT);
	}
	else {
		TreeView_SetExtendedStyle(FHandle, 0, TVS_EX_MULTISELECT);
	}

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiSelectStyle
//---------------------------------------------------------------------------

TMultiSelectStyle TTreeView::Get_MultiSelectStyle(void) {
  return FMultiSelectStyle;
}

bool TTreeView::Set_MultiSelectStyle(TMultiSelectStyle NewMultiSelectStyle) {
  if (FMultiSelectStyle != NewMultiSelectStyle) {
    FMultiSelectStyle = NewMultiSelectStyle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ReadOnly
//---------------------------------------------------------------------------

bool TTreeView::Get_ReadOnly(void) {
  return ((FWindowStyle & TVS_EDITLABELS) == 0);
}

bool TTreeView::Set_ReadOnly(bool NewReadOnly) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewReadOnly) NewWindowStyle |= TVS_EDITLABELS;
  else NewWindowStyle &= (DWORD) ~TVS_EDITLABELS;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété RightClickSelect
//---------------------------------------------------------------------------

bool TTreeView::Get_RightClickSelect(void) {
  return FRightClickSelect;
}

bool TTreeView::Set_RightClickSelect(bool NewRightClickSelect) {
  if (FRightClickSelect != NewRightClickSelect) {
    FRightClickSelect = NewRightClickSelect;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété RowSelect
//---------------------------------------------------------------------------

bool TTreeView::Get_RowSelect(void) {
  return FRowSelect;
}

bool TTreeView::Set_RowSelect(bool NewRowSelect) {
  if (FRowSelect != NewRowSelect) {
    FRowSelect = NewRowSelect;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Selected
//---------------------------------------------------------------------------

TTreeNode *TTreeView::Get_Selected(void) {

  if (FHandle) {
    HTREEITEM hTreeItem = TreeView_GetSelection(FHandle);
    if (hTreeItem) {
      TVITEM TVItem;
      TVItem.mask = TVIF_HANDLE | TVIF_PARAM;
      TVItem.hItem = hTreeItem;
      if (TreeView_GetItem(FHandle, &TVItem)) {
        return (TTreeNode *) TVItem.lParam;
      }
    }
  }

  return NULL;
}

bool TTreeView::Set_Selected(TTreeNode *NewSelected) {

  if (FHandle) {
    if (NewSelected) {
      HTREEITEM hTreeItem = NewSelected->ItemId;
      TreeView_SelectItem(FHandle, hTreeItem);
    }
    else {
      TreeView_SelectItem(FHandle, NULL);
    }
    return true;
  }

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SelectionCount
//---------------------------------------------------------------------------

unsigned TTreeView::Get_SelectionCount(void) {
  return FSelectionCount;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowButtons
//---------------------------------------------------------------------------

bool TTreeView::Get_ShowButtons(void) {
  return ((FWindowStyle & TVS_HASBUTTONS) == 0);
}

bool TTreeView::Set_ShowButtons(bool NewShowButtons) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewShowButtons) NewWindowStyle |= TVS_HASBUTTONS;
  else NewWindowStyle &= (DWORD) ~TVS_HASBUTTONS;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowLines
//---------------------------------------------------------------------------

bool TTreeView::Get_ShowLines(void) {
  return ((FWindowStyle & TVS_HASLINES) == 0);
}

bool TTreeView::Set_ShowLines(bool NewShowLines) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewShowLines) NewWindowStyle |= TVS_HASLINES;
  else NewWindowStyle &= (DWORD) ~TVS_HASLINES;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowRoot
//---------------------------------------------------------------------------

bool TTreeView::Get_ShowRoot(void) {
  return ((FWindowStyle & TVS_LINESATROOT) == 0);
}

bool TTreeView::Set_ShowRoot(bool NewShowRoot) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewShowRoot) NewWindowStyle |= TVS_LINESATROOT;
  else NewWindowStyle &= (DWORD) ~TVS_LINESATROOT;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SortType
//---------------------------------------------------------------------------

TSortType TTreeView::Get_SortType(void) {
  return FSortType;
}

bool TTreeView::Set_SortType(TSortType NewSortType) {
  if (FSortType != NewSortType) {
    FSortType = NewSortType;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété StateImages
//---------------------------------------------------------------------------

TImageList *TTreeView::Get_StateImages(void) {
  return FStateImages;
}

bool TTreeView::Set_StateImages(TImageList *NewStateImages) {
  if (FStateImages != NewStateImages) {
    FStateImages = NewStateImages;
		if (FHandle) {
			TreeView_SetImageList(FHandle, FStateImages->Handle, TVSIL_STATE);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ToolTips
//---------------------------------------------------------------------------

bool TTreeView::Get_ToolTips(void) {
  return FToolTips;
}

bool TTreeView::Set_ToolTips(bool NewToolTips) {
  if (FToolTips != NewToolTips) {
    FToolTips = NewToolTips;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TopItem
//---------------------------------------------------------------------------

TTreeNode *TTreeView::Get_TopItem(void) {
  return FTopItem;
}

bool TTreeView::Set_TopItem(TTreeNode *NewTopItem) {
  if (FTopItem != NewTopItem) {
    FTopItem = NewTopItem;
  }
  return true;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeView::GetNodeAt(int X, int Y) {

  if (FHandle) {
		TVHITTESTINFO tvht;
		memset(&tvht, 0, sizeof(TVHITTESTINFO));
		tvht.pt.x = X;
		tvht.pt.y = Y;
    HTREEITEM hTreeItem = TreeView_HitTest(FHandle, &tvht);
    if (hTreeItem) {
      TVITEM TVItem;
      TVItem.mask = TVIF_HANDLE | TVIF_PARAM;
      TVItem.hItem = hTreeItem;
      if (TreeView_GetItem(FHandle, &TVItem)) {
        return (TTreeNode *) TVItem.lParam;
      }
    }
  }

  return NULL;
}

//---------------------------------------------------------------------------
bool FASTCALL TTreeView::AlphaSort(bool ARecurse) {
  return false;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TTreeView::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("AutoExpand"));
  ListProperties->Add(_T("BorderStyle"));
  ListProperties->Add(_T("ChangeDelay"));
  ListProperties->Add(_T("CheckBoxes"));
  ListProperties->Add(_T("DropTarget"));
  ListProperties->Add(_T("HideSelection"));
  ListProperties->Add(_T("HotTrack"));
  ListProperties->Add(_T("Images"));
  ListProperties->Add(_T("Indent"));
  ListProperties->Add(_T("Items"));
  ListProperties->Add(_T("MultiSelect"));
  ListProperties->Add(_T("MultiSelectStyle"));
  ListProperties->Add(_T("ReadOnly"));
  ListProperties->Add(_T("RightClickSelect"));
  ListProperties->Add(_T("RowSelect"));
  ListProperties->Add(_T("Selected"));
  ListProperties->Add(_T("SelectionCount"));
  ListProperties->Add(_T("ShowButtons"));
  ListProperties->Add(_T("ShowLines"));
  ListProperties->Add(_T("ShowRoot"));
  ListProperties->Add(_T("SortType"));
  ListProperties->Add(_T("StateImages"));
  ListProperties->Add(_T("ToolTips"));
  ListProperties->Add(_T("TopItem"));
  ListProperties->Add(_T("OnAddition"));
  ListProperties->Add(_T("OnDeletion"));
  ListProperties->Add(_T("OnEdited"));
  ListProperties->Add(_T("OnEditing"));
  ListProperties->Add(_T("OnChanging"));
  ListProperties->Add(_T("OnChange"));
  ListProperties->Add(_T("OnExpanding"));
  ListProperties->Add(_T("OnExpanded"));
  ListProperties->Add(_T("OnCollapsing"));
  ListProperties->Add(_T("OnCollapsed"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TTreeView::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("AutoExpand")) {
    return tpBool;
  }
  if (asProperty == _T("BorderStyle")) {
    *asInfos = _T("bsNone\nbsSingle\nbsSizeable\nbsDialog");
    return tpChoice;
  }
  if (asProperty == _T("ChangeDelay")) {
    return tpNumber;
  }
  if (asProperty == _T("CheckBoxes")) {
    return tpBool;
  }
  if (asProperty == _T("DropTarget")) {
    return tpObject;
  }
  if (asProperty == _T("HideSelection")) {
    return tpBool;
  }
  if (asProperty == _T("HotTrack")) {
    return tpBool;
  }
  if (asProperty == _T("Images")) {
    return tpObject;
  }
  if (asProperty == _T("Indent")) {
    return tpNumber;
  }
  if (asProperty == _T("Items")) {
    return tpObject;
  }
  if (asProperty == _T("MultiSelect")) {
    return tpBool;
  }
  if (asProperty == _T("MultiSelectStyle")) {
    *asInfos = _T("msControlSelect\nmsShiftSelect\nmsVisibleOnly\nmsSiblingOnly");
    return tpChoice;
  }
  if (asProperty == _T("ReadOnly")) {
    return tpBool;
  }
  if (asProperty == _T("RightClickSelect")) {
    return tpBool;
  }
  if (asProperty == _T("RowSelect")) {
    return tpBool;
  }
  if (asProperty == _T("Selected")) {
    return tpObject;
  }
  if (asProperty == _T("SelectionCount")) {
    return tpNumber;
  }
  if (asProperty == _T("ShowButtons")) {
    return tpBool;
  }
  if (asProperty == _T("ShowLines")) {
    return tpBool;
  }
  if (asProperty == _T("ShowRoot")) {
    return tpBool;
  }
  if (asProperty == _T("SortType")) {
    *asInfos = _T("stNone\nstData\nstText\nstBoth");
    return tpChoice;
  }
  if (asProperty == _T("StateImages")) {
    return tpObject;
  }
  if (asProperty == _T("ToolTips")) {
    return tpBool;
  }
  if (asProperty == _T("TopItem")) {
    return tpObject;
  }
  if (asProperty == _T("OnAddition")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnDeletion")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnEdited")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *Node, AnsiString Text)");
    return tpEvent;
  }
  if (asProperty == _T("OnEditing")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *Node, bool &AllowEdit)");
    return tpEvent;
  }
  if (asProperty == _T("OnChanging")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *Node, bool &AllowChanging)");
    return tpEvent;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *TreeNode)");
    return tpEvent;
  }
  if (asProperty == _T("OnExpanding")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *TreeNode)");
    return tpEvent;
  }
  if (asProperty == _T("OnExpanded")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *TreeNode)");
    return tpEvent;
  }
  if (asProperty == _T("OnCollapsing")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *TreeNode)");
    return tpEvent;
  }
  if (asProperty == _T("OnCollapsed")) {
    *asInfos = _T("(TObject *Sender, TTreeNode *TreeNode)");
    return tpEvent;
  }

  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TTreeView::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("AutoExpand")) {
    return _T("False");
  }
  if (asProperty == _T("BorderStyle")) {
    return _T("bsSingle");
  }
  if (asProperty == _T("ChangeDelay")) {
    return _T("0");
  }
  if (asProperty == _T("CheckBoxes")) {
    return _T("False");
  }
  if (asProperty == _T("DropTarget")) {
    return _T("");
  }
  if (asProperty == _T("HideSelection")) {
    return _T("True");
  }
  if (asProperty == _T("HotTrack")) {
    return _T("False");
  }
  if (asProperty == _T("Images")) {
    return _T("");
  }
  if (asProperty == _T("Indent")) {
    return _T("0");
  }
  if (asProperty == _T("Items")) {
    return _T("");
  }
  if (asProperty == _T("MultiSelect")) {
    return _T("False");
  }
  if (asProperty == _T("MultiSelectStyle")) {
    return _T("msControlSelect");
  }
  if (asProperty == _T("ReadOnly")) {
    return _T("False");
  }
  if (asProperty == _T("RightClickSelect")) {
    return _T("False");
  }
  if (asProperty == _T("RowSelect")) {
    return _T("False");
  }
  if (asProperty == _T("Selected")) {
    return _T("");
  }
  if (asProperty == _T("SelectionCount")) {
    return _T("");
  }
  if (asProperty == _T("ShowButtons")) {
    return _T("True");
  }
  if (asProperty == _T("ShowLines")) {
    return _T("True");
  }
  if (asProperty == _T("ShowRoot")) {
    return _T("True");
  }
  if (asProperty == _T("SortType")) {
    return _T("stNone");
  }
  if (asProperty == _T("StateImages")) {
    return _T("");
  }
  if (asProperty == _T("ToolTips")) {
    return _T("True");
  }
  if (asProperty == _T("TopItem")) {
    return _T("");
  }
  if (asProperty == _T("OnAddition")) {
    return _T("");
  }
  if (asProperty == _T("OnDeletion")) {
    return _T("");
  }
  if (asProperty == _T("OnEdited")) {
    return _T("");
  }
  if (asProperty == _T("OnEditing")) {
    return _T("");
  }
  if (asProperty == _T("OnChanging")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  if (asProperty == _T("OnExpanding")) {
    return _T("");
  }
  if (asProperty == _T("OnExpanded")) {
    return _T("");
  }
  if (asProperty == _T("OnCollapsing")) {
    return _T("");
  }
  if (asProperty == _T("OnCollapsed")) {
    return _T("");
  }
  if (asProperty == _T("Color")) {
    return _T("clWindow");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TTreeView::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("AutoExpand")) {
    if (Get_AutoExpand()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("BorderStyle")) {
    return _T("bsSingle");
  }
  if (asProperty == _T("ChangeDelay")) {
    return IntToStr(Get_ChangeDelay());
  }
  if (asProperty == _T("CheckBoxes")) {
    if (Get_CheckBoxes()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("DropTarget")) {
    TTreeNode *TreeNode = Get_DropTarget();
    if (TreeNode) return TreeNode->Name;
    return _T("");
  }
  if (asProperty == _T("HideSelection")) {
    if (Get_HideSelection()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("HotTrack")) {
    if (Get_HotTrack()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Images")) {
    TImageList *Images = Get_Images();
    if (Images) return Images->Name;
    return _T("");
  }
  if (asProperty == _T("Indent")) {
    return IntToStr(Get_Indent());
  }
  if (asProperty == _T("Items")) {
    return _T("");
  }
  if (asProperty == _T("MultiSelect")) {
    if (Get_MultiSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("MultiSelectStyle")) {
		TMultiSelectStyle MultiSelectStyle = Get_MultiSelectStyle();
    if (MultiSelectStyle == msControlSelect) return _T("msControlSelect");
    if (MultiSelectStyle == msShiftSelect) return _T("msShiftSelect");
    if (MultiSelectStyle == msVisibleOnly) return _T("msVisibleOnly");
    if (MultiSelectStyle == msSiblingOnly) return _T("msSiblingOnly");
    return _T("");
  }
  if (asProperty == _T("ReadOnly")) {
    if (Get_ReadOnly()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("RightClickSelect")) {
    if (Get_RightClickSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("RowSelect")) {
    if (Get_RowSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Selected")) {
    TTreeNode *TreeNode = Get_Selected();
    if (TreeNode) return TreeNode->Name;
    return _T("");
  }
  if (asProperty == _T("SelectionCount")) {
    return IntToStr(Get_SelectionCount());
  }
  if (asProperty == _T("ShowButtons")) {
    if (Get_ShowButtons()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ShowLines")) {
    if (Get_ShowLines()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ShowRoot")) {
    if (Get_ShowRoot()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("SortType")) {
		TSortType SortType = Get_SortType();
    if (SortType == stNone) return _T("stNone");
    if (SortType == stData) return _T("stData");
    if (SortType == stText) return _T("stText");
    if (SortType == stBoth) return _T("stBoth");
    return _T("");
  }
  if (asProperty == _T("StateImages")) {
    TImageList *StateImages = Get_StateImages();
    if (StateImages) return StateImages->Name;
    return _T("");
  }
  if (asProperty == _T("ToolTips")) {
    if (Get_ToolTips()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("TopItem")) {
    TTreeNode *TreeNode = Get_TopItem();
    if (TreeNode) return TreeNode->Name;
    return _T("");
  }
  if (asProperty == _T("OnAddition")) {
    return OnAddition.AsString();
  }
  if (asProperty == _T("OnDeletion")) {
    return OnDeletion.AsString();
  }
  if (asProperty == _T("OnEdited")) {
    return OnEdited.AsString();
  }
  if (asProperty == _T("OnEditing")) {
    return OnEditing.AsString();
  }
  if (asProperty == _T("OnChanging")) {
    return OnChanging.AsString();
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  if (asProperty == _T("OnExpanding")) {
    return OnExpanding.AsString();
  }
  if (asProperty == _T("OnExpanded")) {
    return OnExpanded.AsString();
  }
  if (asProperty == _T("OnCollapsing")) {
    return OnCollapsing.AsString();
  }
  if (asProperty == _T("OnCollapsed")) {
    return OnCollapsed.AsString();
  }

  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TTreeView::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("AutoExpand")) {
    Set_AutoExpand(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("BorderStyle")) {
    return true;
  }
  if (asProperty == _T("ChangeDelay")) {
    Set_ChangeDelay(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("CheckBoxes")) {
    Set_CheckBoxes(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("DropTarget")) {
    TTreeNode *TreeNode;
		if (asValue.IsEmpty()) TreeNode = NULL;
		else TreeNode = dynamic_cast<TTreeNode *>(FindRootComponent()->FindComponent(asValue));
    Set_DropTarget(TreeNode);
    return true;
  }
  if (asProperty == _T("HideSelection")) {
    Set_HideSelection(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("HotTrack")) {
    Set_HotTrack(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Images")) {
    TImageList *ImageList;
		if (asValue.IsEmpty()) ImageList = NULL;
		else ImageList = dynamic_cast<TImageList *>(FindRootComponent()->FindComponent(asValue));
    Set_Images(ImageList);
    return true;
  }
  if (asProperty == _T("Indent")) {
    Set_Indent(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Items")) {
    return true;
  }
  if (asProperty == _T("MultiSelect")) {
    Set_MultiSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("MultiSelectStyle")) {
    if (asValue == _T("msControlSelect")) Set_MultiSelectStyle(msControlSelect);
    else if (asValue == _T("msShiftSelect")) Set_MultiSelectStyle(msShiftSelect);
    else if (asValue == _T("msVisibleOnly")) Set_MultiSelectStyle(msVisibleOnly);
    else if (asValue == _T("msSiblingOnly")) Set_MultiSelectStyle(msSiblingOnly);
    return true;
  }
  if (asProperty == _T("ReadOnly")) {
    Set_ReadOnly(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("RightClickSelect")) {
    Set_RightClickSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("RowSelect")) {
    Set_RowSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Selected")) {
    TTreeNode *TreeNode;
		if (asValue.IsEmpty()) TreeNode = NULL;
		else TreeNode = dynamic_cast<TTreeNode *>(FindRootComponent()->FindComponent(asValue));
    Set_Selected(TreeNode);
    return true;
  }
  if (asProperty == _T("ShowButtons")) {
    Set_ShowButtons(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ShowLines")) {
    Set_ShowLines(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ShowRoot")) {
    Set_ShowRoot(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("SortType")) {
    if (asValue == _T("stNone")) Set_SortType(stNone);
    else if (asValue == _T("stData")) Set_SortType(stData);
    else if (asValue == _T("stText")) Set_SortType(stText);
    else if (asValue == _T("stBoth")) Set_SortType(stBoth);
    return true;
  }
  if (asProperty == _T("StateImages")) {
    TImageList *ImageList;
		if (asValue.IsEmpty()) ImageList = NULL;
		else ImageList = dynamic_cast<TImageList *>(FindRootComponent()->FindComponent(asValue));
    Set_StateImages(ImageList);
    return true;
  }
  if (asProperty == _T("ToolTips")) {
    Set_ToolTips(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("TopItem")) {
    TTreeNode *TreeNode;
		if (asValue.IsEmpty()) TreeNode = NULL;
		else TreeNode = dynamic_cast<TTreeNode *>(FindRootComponent()->FindComponent(asValue));
    Set_TopItem(TreeNode);
    return true;
  }
  if (asProperty == _T("OnAddition")) {
		OnAddition.ObjCall = Sender;
    OnAddition = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnDeletion")) {
		OnDeletion.ObjCall = Sender;
    OnDeletion = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnEdited")) {
		OnEdited.ObjCall = Sender;
    OnEdited = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnEditing")) {
		OnEditing.ObjCall = Sender;
    OnEditing = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnChanging")) {
		OnChanging.ObjCall = Sender;
    OnChanging = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnExpanding")) {
		OnExpanding.ObjCall = Sender;
    OnExpanding = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnExpanded")) {
		OnExpanded.ObjCall = Sender;
    OnExpanded = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnCollapsing")) {
		OnCollapsing.ObjCall = Sender;
    OnCollapsing = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnCollapsed")) {
		OnCollapsed.ObjCall = Sender;
    OnCollapsed = asValue.c_str();
    return true;
  }

  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

