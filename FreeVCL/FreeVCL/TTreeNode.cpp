//-----------------------------------------------------------------------------
//! @file TTreeNode.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TTreeNode
//---------------------------------------------------------------------------

TTreeNode::TTreeNode(TTreeNodes* AOwner): TPersistent() {

  // Initialisations
  FClassName = _T("TTreeNode");
	FOwner = AOwner;
	if (FOwner) {
		FTreeView = FOwner->Owner;
		FOwner->InsertComponent(this);
	}
	else FTreeView = NULL;
  FAbsoluteIndex = 0;
	FCount = 0;
  FCut = 0;
  FData = NULL;
  FDeleting = false;
  FDropTarget = false;
  FFocused = false;
  FHandle = NULL;
  FHasChildren = false;
  FImageIndex = -1;
  FItemId = NULL;
  FLevel = 0;
  FOverlayIndex = 0;
  FParent = NULL;
  FSelectedIndex = -1;
  FStateIndex = -1;
}

TTreeNode::~TTreeNode(void) {
	if (FTreeView) {
		FTreeView->OnDeletion(this);
	}
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AbsoluteIndex
//---------------------------------------------------------------------------

int TTreeNode::Get_AbsoluteIndex(void) {
  return FAbsoluteIndex;
}

bool TTreeNode::Set_AbsoluteIndex(int NewAbsoluteIndex) {
  if (FAbsoluteIndex != NewAbsoluteIndex) {
    FAbsoluteIndex = NewAbsoluteIndex;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TTreeNode::Get_Count(void) {
	return FCount;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Cut
//---------------------------------------------------------------------------

bool TTreeNode::Get_Cut(void) {
  return FCut;
}

bool TTreeNode::Set_Cut(bool NewCut) {
  if (FCut != NewCut) {
    FCut = NewCut;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Data
//---------------------------------------------------------------------------

void * TTreeNode::Get_Data(void) {
  return FData;
}

bool TTreeNode::Set_Data(void *NewData) {
  if (FData != NewData) {
    FData = NewData;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Deleting
//---------------------------------------------------------------------------

bool TTreeNode::Get_Deleting(void) {
  return FDeleting;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DropTarget
//---------------------------------------------------------------------------

bool TTreeNode::Get_DropTarget(void) {
  return FDropTarget;
}

bool TTreeNode::Set_DropTarget(bool NewDropTarget) {
  if (FDropTarget != NewDropTarget) {
    FDropTarget = NewDropTarget;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Expanded
//---------------------------------------------------------------------------

bool TTreeNode::Get_Expanded(void) {
  if (FItemId == TVI_ROOT) return false;

	TVITEM TVItem;
	TVItem.mask = TVIF_HANDLE | TVIF_STATE;
	TVItem.hItem = FItemId;
	TreeView_GetItem(FHandle, &TVItem);

	return (TVItem.state & TVIS_EXPANDED) != 0;
}

bool TTreeNode::Set_Expanded(bool NewExpanded) {
  if (FItemId == TVI_ROOT) return false;

	if (NewExpanded) Expand(false);
	else Collapse(false);

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Focused
//---------------------------------------------------------------------------

bool TTreeNode::Get_Focused(void) {
  return FFocused;
}

bool TTreeNode::Set_Focused(bool NewFocused) {
  if (FFocused != NewFocused) {
    FFocused = NewFocused;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TTreeNode::Get_Handle(void) {
  return FHandle;
}

bool TTreeNode::Set_Handle(HWND NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HasChildren
//---------------------------------------------------------------------------

bool TTreeNode::Get_HasChildren(void) {
  return FHasChildren;
}

bool TTreeNode::Set_HasChildren(bool NewHasChildren) {
  if (FHasChildren != NewHasChildren) {
    FHasChildren = NewHasChildren;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ImageIndex
//---------------------------------------------------------------------------

TImageIndex TTreeNode::Get_ImageIndex(void) {
  return FImageIndex;
}

bool TTreeNode::Set_ImageIndex(TImageIndex NewImageIndex) {
  if (FImageIndex != NewImageIndex) {
    FImageIndex = NewImageIndex;
		if (FHandle) {
			TVITEM TVItem;
			TVItem.mask = TVIF_HANDLE | TVIF_IMAGE;
			TVItem.hItem = FItemId;
			TVItem.iImage = FImageIndex;
			if (TreeView_SetItem(FHandle, &TVItem)) {
				return true;
			}
		}
  }

	return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Index
//---------------------------------------------------------------------------

int TTreeNode::Get_Index(void) {
	int i;


	for (i = 0; i < FTreeView->Items->Count; i++) {
		if (FTreeView->Items->Item[i] == this) return i;
	}

  return -1;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété IsFirstNode
//---------------------------------------------------------------------------

bool TTreeNode::Get_IsFirstNode(void) {
  return FItemId == TVI_ROOT;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Item
//---------------------------------------------------------------------------

TTreeNode *TTreeNode::Get_Item(int Index) {
	int Index2 = 0;

	if (FHandle) {
		HTREEITEM HNextChild = TreeView_GetChild(FHandle, FItemId);
		while (HNextChild) {
			if (Index == Index2) {
				TVITEM TVItem;
				TVItem.mask = TVIF_HANDLE | TVIF_PARAM;
				TVItem.hItem = HNextChild;
				TreeView_GetItem(FHandle, &TVItem);

				return reinterpret_cast<TTreeNode*> (TVItem.lParam);
			}
			Index2++;
			HNextChild = TreeView_GetNextSibling(FHandle, HNextChild);
		}
	}

  return NULL;
}

bool TTreeNode::Set_Item(int Index, TTreeNode *NewItem) {
	TTreeNode *FItem = Get_Item(Index);
  if (FItem != NewItem) {
		delete FItem;
		AnsiString asText = NewItem->Text;

		TVITEM TVItem;
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
    TVItem.pszText = const_cast<TCHAR *> ((LPCTSTR) asText);
    TVItem.lParam = (LPARAM) NewItem;

		TreeView_SetItem(FHandle, &TVItem);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemId
//---------------------------------------------------------------------------

HTREEITEM TTreeNode::Get_ItemId(void) {
  return FItemId;
}

bool TTreeNode::Set_ItemId(HTREEITEM NewItemId) {
  if (FItemId != NewItemId) {
    FItemId = NewItemId;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Level
//---------------------------------------------------------------------------

int TTreeNode::Get_Level(void) {
  return FLevel;
}

bool TTreeNode::Set_Level(int NewLevel) {
  if (FLevel != NewLevel) {
    FLevel = NewLevel;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété OverlayIndex
//---------------------------------------------------------------------------

int TTreeNode::Get_OverlayIndex(void) {
  return FOverlayIndex;
}

bool TTreeNode::Set_OverlayIndex(int NewOverlayIndex) {
  if (FOverlayIndex != NewOverlayIndex) {
    FOverlayIndex = NewOverlayIndex;
		if (FHandle) {
			TVITEM TVItem;
			TVItem.mask = TVIF_HANDLE | TVIF_STATE;
			TVItem.hItem = FItemId;
			TVItem.stateMask = TVIS_OVERLAYMASK;
			TVItem.state = INDEXTOOVERLAYMASK(FOverlayIndex + 1);
			TreeView_SetItem(FHandle, &TVItem);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Owner
//---------------------------------------------------------------------------

TTreeNodes *TTreeNode::Get_Owner(void) {
  return FOwner;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Parent
//---------------------------------------------------------------------------

TTreeNode *TTreeNode::Get_Parent(void) {
  return FParent;
}

bool TTreeNode::Set_Parent(TTreeNode *NewParent) {
  if (FParent != NewParent) {
		if (FParent) {
			FParent->FCount--;
		}
    FParent = NewParent;
		if (FParent) {
			FParent->FCount++;
			FLevel = FParent->FLevel + 1;
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Selected
//---------------------------------------------------------------------------

bool TTreeNode::Get_Selected(void) {
  if (FHandle) return (TreeView_GetSelection(FHandle) == FItemId);
  return false;
}

bool TTreeNode::Set_Selected(bool NewSelected) {
  if (FHandle) {
		if (FItemId == TVI_ROOT) return false;  // Sinon plantage
		if (NewSelected) {
			TreeView_SelectItem(FHandle, FItemId);
		}
		else {
			TVITEM TVItem;
			TVItem.mask = TVIF_HANDLE | TVIF_STATE;
			TVItem.hItem = FItemId;
			TVItem.stateMask = TVIS_SELECTED;
			TVItem.state = 0;
			TreeView_SetItem(FHandle, &TVItem);
		}
	}
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Checked
//---------------------------------------------------------------------------

bool TTreeNode::Get_Checked(void) {
  if (FHandle) return (TreeView_GetCheckState(FHandle, FItemId) == TRUE);
  return false;
}

bool TTreeNode::Set_Checked(bool NewChecked) {
  if (FHandle) TreeView_SetCheckState(FHandle, FItemId, NewChecked);
	// Le TreeView_SetCheckState change le StateIndex
	// => il faut le mettre à jour en interne
	if (NewChecked) FStateIndex = 1;
	else FStateIndex = 0;
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SelectedIndex
//---------------------------------------------------------------------------

int TTreeNode::Get_SelectedIndex(void) {
  return FSelectedIndex;
}

bool TTreeNode::Set_SelectedIndex(int NewSelectedIndex) {
  if (FSelectedIndex != NewSelectedIndex) {
    FSelectedIndex = NewSelectedIndex;
		if (FHandle) {
			if (FItemId == TVI_ROOT) return false;  // Sinon plantage
			TVITEM TVItem;
			TVItem.mask = TVIF_HANDLE | TVIF_SELECTEDIMAGE;
			TVItem.hItem = FItemId;
			TVItem.iSelectedImage = FSelectedIndex;
			TreeView_SetItem(FHandle, &TVItem);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété StateIndex
//---------------------------------------------------------------------------

int TTreeNode::Get_StateIndex(void) {
  return FStateIndex;
}

bool TTreeNode::Set_StateIndex(int NewStateIndex) {
  if (FStateIndex != NewStateIndex) {
    FStateIndex = NewStateIndex;
		if (FHandle) {
			if (FItemId == TVI_ROOT) return false;  // Sinon plantage
			TVITEM TVItem;
			memset(&TVItem, 0, sizeof(TVITEM));
			TVItem.mask = TVIF_HANDLE | TVIF_STATE;
			TVItem.hItem = FItemId;
			TVItem.stateMask = TVIS_STATEIMAGEMASK;
			TVItem.state = INDEXTOSTATEIMAGEMASK(FStateIndex + 1);
			TreeView_SetItem(FHandle, &TVItem);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TTreeNode::Get_Text(void) {
  TCHAR szText[256];  // $$$ Je ne sais pas comment avoir la longueur du texte

  szText[0] = _T('\0');
  if (FHandle) {
		if (FItemId == TVI_ROOT) return false;  // Sinon plantage
    TVITEM TVItem;
    TVItem.mask = TVIF_HANDLE | TVIF_TEXT;
    TVItem.hItem = FItemId;
    TVItem.pszText = szText;
    TVItem.cchTextMax = 256;
    TreeView_GetItem(FHandle, &TVItem);
  }

  return szText;
}

bool TTreeNode::Set_Text(AnsiString NewText) {

  if (FHandle) {
		if (FItemId == TVI_ROOT) return false;  // Sinon plantage
    TVITEM TVItem;
    TVItem.mask = TVIF_HANDLE | TVIF_TEXT;
    TVItem.hItem = FItemId;
    TVItem.pszText = const_cast<TCHAR *> ((LPCTSTR) NewText);
    if (TreeView_SetItem(FHandle, &TVItem)) {
      return true;
    }
  }

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TreeView
//---------------------------------------------------------------------------

TTreeView *TTreeNode::Get_TreeView(void) {
  return FTreeView;
}

bool TTreeNode::Set_TreeView(TTreeView *NewTreeView) {
  if (FTreeView != NewTreeView) {
    FTreeView = NewTreeView;
  }
  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TTreeNode::AlphaSort(bool ARecurse) {
  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNode::Assign(TPersistent* Source) {
	TTreeNode *NodeSource;


	NodeSource = dynamic_cast<TTreeNode *>(Source);
	if (NodeSource == NULL) return;

	Text = NodeSource->Text;
	// Le TreeView_SetCheckState utilise le TreeView_SetItemState
	// => le flag Checked sera mis à jour avec la propriété StateIndex
	// Checked = NodeSource->Checked;
	ImageIndex = NodeSource->ImageIndex;
	OverlayIndex = NodeSource->OverlayIndex;
  SelectedIndex = NodeSource->SelectedIndex;
  StateIndex = NodeSource->StateIndex;
	Expanded = NodeSource->Expanded;
	FData = NodeSource->FData;
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNode::Collapse(bool Recurse) {

	TreeView_Expand(FHandle, FItemId, TVE_COLLAPSE);

	if (Recurse) {

		TTreeNode *NodeChild;

		NodeChild = GetFirstChild();
		while (NodeChild) {
			NodeChild->Collapse(Recurse);
			NodeChild = GetNextChild(NodeChild);
		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL TTreeNode::Delete(void) {
	FTreeView->Items->Delete(this);
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNode::DeleteChildren(void) {
}

//---------------------------------------------------------------------------
TRect FASTCALL TTreeNode::DisplayRect(bool TextOnly) {
  return TRect(0, 0, 0, 0);
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNode::EditText(void) {
	TreeView_EditLabel(FHandle, FItemId);
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNode::EndEdit(bool Cancel) {
	TreeView_EndEditLabelNow(FHandle, (Cancel == true));
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNode::Expand(bool Recurse) {

	TreeView_Expand(FHandle, FItemId, TVE_EXPAND);

	if (Recurse) {

		TTreeNode *NodeChild;

		NodeChild = GetFirstChild();
		while (NodeChild) {
			NodeChild->Expand(Recurse);
			NodeChild = GetNextChild(NodeChild);
		}

	}

}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetFirstChild(void) {
	HTREEITEM HFirstChild = TreeView_GetChild(FHandle, FItemId);
	if (HFirstChild == 0) return NULL;
	return FTreeView->Items->GetNode(HFirstChild);
}

//---------------------------------------------------------------------------
HWND FASTCALL TTreeNode::GetHandle(void) {
  return FHandle;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetLastChild(void) {
	HTREEITEM HPrevChild;
	HTREEITEM HNextChild = TreeView_GetChild(FHandle, FItemId);
	if (HNextChild == 0) return NULL;
	while (HNextChild != 0) {
		HPrevChild = HNextChild;
		HNextChild = TreeView_GetNextSibling(FHandle, HPrevChild);
	}
	return FTreeView->Items->GetNode(HPrevChild);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetNext(void) {
  return NULL;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetNextChild(TTreeNode* TreeNode) {
  return TreeNode->GetNextSibling();
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetNextSibling(void) {
	if (FItemId == TVI_ROOT) return NULL;
  HTREEITEM HNextSibling = TreeView_GetNextSibling(FHandle, FItemId);
	if (HNextSibling == 0) return NULL;
	return FTreeView->Items->GetNode(HNextSibling);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetNextVisible(void) {
	if (FItemId == TVI_ROOT) return NULL;
	HTREEITEM HNextVisible = TreeView_GetNextVisible(FHandle, FItemId);
	if (HNextVisible == 0) return NULL;
	return FTreeView->Items->GetNode(HNextVisible);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetPrev(void) {
  return NULL;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetPrevChild(TTreeNode* TreeNode) {
  return TreeNode->GetPrevSibling();
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetPrevSibling(void) {
  HTREEITEM HPrevSibling = TreeView_GetPrevSibling(FHandle, FItemId);
	if (HPrevSibling == 0) return NULL;
	return FTreeView->Items->GetNode(HPrevSibling);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNode::GetPrevVisible(void) {
  HTREEITEM HPrevVisible = TreeView_GetPrevVisible(FHandle, FItemId);
	if (HPrevVisible == 0) return NULL;
	return FTreeView->Items->GetNode(HPrevVisible);
}

//---------------------------------------------------------------------------
bool FASTCALL TTreeNode::HasAsParent(TTreeNode* TreeNode) {
	TTreeNode* TreeNodeParent = FParent;

	while (TreeNodeParent) {
		if (TreeNode == TreeNodeParent) return true;
		TreeNodeParent = TreeNode->FParent;
	}

	return false;
}

//---------------------------------------------------------------------------
// Renvoie la position d'un noeud enfant immédiat du noeud parent.
//---------------------------------------------------------------------------

int FASTCALL TTreeNode::IndexOf(TTreeNode* TreeNode) {
	TTreeNode *NodeChild;
	int Index;


	Index = 0;
	NodeChild = GetFirstChild();
	while (NodeChild) {
		if (NodeChild == TreeNode) return Index;
		Index++;
		NodeChild = NodeChild->GetNextSibling();
	}

	return -1;
}

//---------------------------------------------------------------------------
// Développe le noeud parent (et grand parent, ...) d'un noeud.
//---------------------------------------------------------------------------

void FASTCALL TTreeNode::MakeVisible(void) {
	TreeView_EnsureVisible(FHandle, FItemId);
}

//---------------------------------------------------------------------------
// Déplace le noeud à un autre emplacement du TreeView.
//---------------------------------------------------------------------------

void FASTCALL TTreeNode::MoveTo(TTreeNode* Destination, TNodeAttachMode Mode) {
}

//---------------------------------------------------------------------------
// Comme la propriété Item.
//---------------------------------------------------------------------------

TTreeNode* TTreeNode::operator[](int Index) {
	return Get_Item(Index);
}

//---------------------------------------------------------------------------
