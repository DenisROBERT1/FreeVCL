//-----------------------------------------------------------------------------
//! @file TTreeNodes.cpp
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
// TTreeNodes
//---------------------------------------------------------------------------

TTreeNodes::TTreeNodes(TTreeView* AOwner): TPersistent() {

  // Initialisations
  FClassName = _T("TTreeNodes");
	FOwner = AOwner;
  FHandle = NULL;
  FUpdateCount = 0;
	TTreeNode *FirstNode = new TTreeNode(this);
	FirstNode->ItemId = TVI_ROOT;
}

//---------------------------------------------------------------------------
TTreeNodes::~TTreeNodes(void) {
	int i;

	// Les noeuds ne sont pas automatiquement détruits car
	// this est un TPersistent et non un TComponent
	for (i = 0; i < (int) ListNodes.size(); i++) {
		delete ListNodes[i];
	}
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------
int TTreeNodes::Get_Count(void) {
	int Count = 1;  // FirstNode est toujours présent


  if (FHandle) {
    Count += TreeView_GetCount(FHandle);
  }

  return Count;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FirstNode
//---------------------------------------------------------------------------

TTreeNode *TTreeNodes::Get_FirstNode(void) {
  return ListNodes[0];
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TTreeNodes::Get_Handle(void) {
  return FHandle;
}

bool TTreeNodes::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
    ListNodes[0]->Handle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Item
//---------------------------------------------------------------------------

TTreeNode *TTreeNodes::Get_Item(int Index) {

	if (Index == 0) return ListNodes[0];

	if (FHandle) {
		HTREEITEM hTreeItem;
		HTREEITEM hTreeNextItem;
		int i;
		hTreeItem = ListNodes[0]->ItemId;
		for (i = 0; i < Index; i++) {
			hTreeNextItem = TreeView_GetChild(FHandle, hTreeItem);
			if (hTreeNextItem == NULL) {
				if (hTreeItem == TVI_ROOT) return NULL;  // Sinon GetNextSibling plante (bug GDI ?) 
				hTreeNextItem = TreeView_GetNextSibling(FHandle, hTreeItem);
			}
			if (hTreeNextItem == NULL) {
				do {
					hTreeItem = TreeView_GetParent(FHandle, hTreeItem);
					if (hTreeItem == NULL || hTreeItem == TVI_ROOT) return NULL;
					hTreeNextItem = TreeView_GetNextSibling(FHandle, hTreeItem);
				} while (hTreeNextItem == NULL);
			}
			hTreeItem = hTreeNextItem;
		}
		return GetNode(hTreeItem);
	}

	return NULL;
}

bool TTreeNodes::Set_Item(int i, TTreeNode * NewItem) {

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Owner
//---------------------------------------------------------------------------

TTreeView *TTreeNodes::Get_Owner(void) {
  return FOwner;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::Add(TTreeNode* Node, const AnsiString S) {
  return AddNode(NULL, Node, S, NULL, naAdd);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddChild(TTreeNode* Node, const AnsiString S) {
  return AddNode(NULL, Node, S, NULL, naAddChild);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddChildFirst(TTreeNode* Node, const AnsiString S) {
  return AddNode(NULL, Node, S, NULL, naAddChildFirst);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddChildObject(TTreeNode* Node, const AnsiString S, void * Ptr) {
  return AddNode(NULL, Node, S, Ptr, naAddChild);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddChildObjectFirst(TTreeNode* Node, const AnsiString S, void * Ptr) {
  return AddNode(NULL, Node, S, Ptr, naAddChildFirst);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddFirst(TTreeNode* Node, const AnsiString S) {
  return AddNode(NULL, Node, S, NULL, naAddFirst);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddNode(TTreeNode* Node, TTreeNode* Relative, const AnsiString S, void * Ptr, TNodeAttachMode Method) {
  TTreeNode * ItemParent = NULL;
  TTreeNode * NewItem = NULL;
  TVINSERTSTRUCT TVInsertStruct;
  HTREEITEM hTreeItem;
	TTreeNode* TreeNodeParent;


  if (FHandle) {

		// On ne peut ajouter qu'un noeud qui vient d'être créé ou qui est indépendant
		// (sinon, risque de lien de parenté cyclique)
		// => test Node->Parent == NULL
    if (Node && Node->Parent == NULL) NewItem = Node;
    else NewItem = new TTreeNode(this);

    if (Relative == NULL) Relative = ListNodes[0];

		switch (Method) {
      case naAdd:
				TreeNodeParent = Relative->Parent;
				TVInsertStruct.hParent = TreeNodeParent? (HTREEITEM) TreeNodeParent->ItemId: NULL;
        TVInsertStruct.hInsertAfter = TVI_LAST;
        NewItem->Parent = TreeNodeParent;
        break;
      case naAddFirst:
				TreeNodeParent = Relative->Parent;
        TVInsertStruct.hParent = TreeNodeParent? (HTREEITEM) TreeNodeParent->ItemId: NULL;
        TVInsertStruct.hInsertAfter = TVI_FIRST;
        NewItem->Parent = TreeNodeParent;
        break;
      case naAddChild:
        TVInsertStruct.hParent = Relative->ItemId;
        TVInsertStruct.hInsertAfter = TVI_LAST;
        NewItem->Parent = Relative;
        break;
      case naAddChildFirst:
        TVInsertStruct.hParent = Relative->ItemId;
        TVInsertStruct.hInsertAfter = TVI_FIRST;
        NewItem->Parent = Relative;
        break;
      case naInsert:
				TreeNodeParent = Relative->Parent;
        TVInsertStruct.hParent = TreeNodeParent? (HTREEITEM) TreeNodeParent->ItemId: NULL;
        TVInsertStruct.hInsertAfter = Relative->ItemId;
        NewItem->Parent = TreeNodeParent;
        break;
    }

		TVInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
    TVInsertStruct.item.pszText = const_cast<TCHAR *> ((LPCTSTR) S);
    TVInsertStruct.item.lParam = (LPARAM) NewItem;
    hTreeItem = TreeView_InsertItem(FHandle, &TVInsertStruct);
    NewItem->ItemId = hTreeItem;
    NewItem->Handle = FHandle;

  }

  return NewItem;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddObject(TTreeNode* Node, const AnsiString S, void * Ptr) {
  return NULL;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::AddObjectFirst(TTreeNode* Node, const AnsiString S, void * Ptr) {
  return NULL;
}

//---------------------------------------------------------------------------
bool FASTCALL TTreeNodes::CopyNode(TTreeNode* NodeSrc, TTreeNode* NodeDest) {
	TTreeNode* NodeChildSrc;
	TTreeNode* NodeChildDest;


	if (IsChild(NodeDest, NodeSrc)) return false;

	NodeDest->Assign(NodeSrc);
	NodeChildSrc = NodeSrc->GetFirstChild();
	while (NodeChildSrc) {
		NodeChildDest = AddChild(NodeDest, NodeSrc->Text);
		CopyNode(NodeChildSrc, NodeChildDest);
		NodeChildSrc = NodeSrc->GetNextChild(NodeChildSrc);
	}
	NodeDest->Expanded = NodeSrc->Expanded;

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TTreeNodes::IsChild(TTreeNode* NodeChild, TTreeNode* NodeParent) {
	while (NodeChild) {
		if (NodeChild == NodeParent) return true;
		NodeChild = NodeChild->Parent;
	}

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TTreeNodes::AlphaSort(bool ARecurse) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNodes::Assign(TPersistent* Source) {
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNodes::BeginUpdate(void) {
  FUpdateCount++;
}

//---------------------------------------------------------------------------
void TTreeNodes::Clear(void) {
	int i;

	if (FHandle) TreeView_DeleteAllItems(FHandle);
	for (i = (int) ListNodes.size() - 1; i >= 1; i--) {
		delete ListNodes[i];
		ListNodes.pop_back();
	}
}

//---------------------------------------------------------------------------
bool FASTCALL TTreeNodes::CustomSort(PFNTVCOMPARE SortProc, int Data, bool ARecurse) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNodes::Delete(TTreeNode* Node) {
	int i;

	if (FHandle) {
		TreeView_DeleteItem(FHandle, Node->ItemId);

		for (i = 0; i < (int) ListNodes.size(); i++) {
			if (ListNodes[i] == Node) {
				ListNodes.erase(ListNodes.begin() + i);
				break;
			}
		}
		delete Node;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNodes::EndUpdate(void) {
  FVCL_ASSERT(FUpdateCount > 0 && "EndUpdate sans BeginUpdate");
  if (FUpdateCount > 0) FUpdateCount--;
}


//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::GetFirstNode(void) {
  return ListNodes[0];
}

//---------------------------------------------------------------------------
void FASTCALL TTreeNodes::InsertComponent(TTreeNode* Node) {
	ListNodes.push_back(Node);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::GetNode(HTREEITEM hItem) {

	if (hItem == NULL || hItem == TVI_ROOT) return ListNodes[0];

  TVITEM TVItem;
  TVItem.mask = TVIF_HANDLE | TVIF_PARAM;
  TVItem.hItem = hItem;
  TreeView_GetItem(FHandle, &TVItem);

	return reinterpret_cast<TTreeNode*> (TVItem.lParam);
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::Insert(TTreeNode* Node, const AnsiString S) {
  TTreeNode * NewItem = NULL;
  TVINSERTSTRUCT TVInsertStruct;
  HTREEITEM hTreeItem;


  if (FHandle) {

    NewItem = new TTreeNode(this);

    if (Node) {
      TVInsertStruct.hParent = Node->ItemId;
      NewItem->Parent = Node;
    }
    else {
      TVInsertStruct.hParent = NULL;
    }
    TVInsertStruct.hInsertAfter = TVI_LAST;
    TVInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
    TVInsertStruct.item.pszText = const_cast<TCHAR *> ((LPCTSTR) S);
    TVInsertStruct.item.lParam = (LPARAM) NewItem;
    hTreeItem = TreeView_InsertItem(FHandle, &TVInsertStruct);
    NewItem->ItemId = hTreeItem;
    NewItem->Handle = FHandle;

  }

  return NewItem;
}

//---------------------------------------------------------------------------
TTreeNode* FASTCALL TTreeNodes::InsertNode(TTreeNode* Node, TTreeNode* Sibling, const AnsiString S, void * Ptr) {
  TTreeNode * NewItem = NULL;
	TTreeNode* SiblingBefore;
  TVINSERTSTRUCT TVInsertStruct;
  HTREEITEM hTreeItem;


	if (FHandle) {

		if (Node == NULL) NewItem = new TTreeNode(this);
		else NewItem = Node;

		SiblingBefore = Sibling->GetPrevSibling();

    if (Node) {
      TVInsertStruct.hParent = Node->ItemId;
      NewItem->Parent = Node;
    }
    else {
      TVInsertStruct.hParent = NULL;
    }
		if (SiblingBefore == NULL) {
			TVInsertStruct.hInsertAfter = TVI_FIRST;
		}
		else {
			TVInsertStruct.hInsertAfter = SiblingBefore->ItemId;
		}
    TVInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
    TVInsertStruct.item.pszText = const_cast<TCHAR *> ((LPCTSTR) S);
    TVInsertStruct.item.lParam = (LPARAM) NewItem;
    hTreeItem = TreeView_InsertItem(FHandle, &TVInsertStruct);
    NewItem->ItemId = hTreeItem;
    NewItem->Handle = FHandle;
    NewItem->Data = Ptr;

  }

  return NewItem;
}

//---------------------------------------------------------------------------
TTreeNode* TTreeNodes::operator[](int Index) {
	return Get_Item(Index);
}

//---------------------------------------------------------------------------

