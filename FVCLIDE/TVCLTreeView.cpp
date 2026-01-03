//-----------------------------------------------------------------------------
//! @file TVCLTreeView.cpp
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

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

#include "TVCLTreeView.h"

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TVCLTreeView::TVCLTreeView(TTreeView *TreeView, bool bRoot): TBaseTree() {

	bNoEvent = false;

	FTreeView = TreeView;
  if (bRoot) {
		TreeNode = FTreeView->Items->GetFirstNode();
		// Affectation des événements si le TreeView est vide
    SET_EVENT(FTreeView, TOnEdited, OnEdited, TVCLTreeView, this, TreeViewEdited);
    SET_EVENT(FTreeView, TOnAddition, OnAddition, TVCLTreeView, this, TreeViewAddition);
    SET_EVENT(FTreeView, TOnDeletion, OnDeletion, TVCLTreeView, this, TreeViewDeletion);
  }
	else {
		TreeNode = new TTreeNode(FTreeView->Items);
	}
	TreeNode->Data = this;

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TVCLTreeView::~TVCLTreeView(void) {
  /*
	TVCLTreeView *VCLNode;
	int i;
	*/

	// delete TreeNode;  // Pas de delete ! Le TreeNode sera détruit avec le TreeView

	// Par contre les TVCLTreeView enfants ne sont pas détruits
	/*
	for (i = TreeNode->Count - 1; i >= 0; i--) {
		VCLNode = reinterpret_cast<TVCLTreeView *> (TreeNode->Item[i]->Data);
		delete VCLNode;
	}*/

	if (FTreeView->Items->Count == 0) {
		SET_EVENT(FTreeView, TOnEdited, OnEdited, TVCLTreeView, NULL, NULL);
		SET_EVENT(FTreeView, TOnAddition, OnAddition, TVCLTreeView, NULL, NULL);
		SET_EVENT(FTreeView, TOnDeletion, OnDeletion, TVCLTreeView, NULL, NULL);
	}
}

//---------------------------------------------------------------------------
void TVCLTreeView::CallEvent(AnsiString asEvent, TObject *Sender, ...) {
  BYTE *pArgs;


  pArgs = (BYTE *) &Sender;
  pArgs += sizeof(TObject *);

  if (asEvent == _T("TreeViewEdited")) {
    FVCL_ARG_CALLEVENT(TTreeNode *, Node)
    FVCL_ARG_CALLEVENT(AnsiString *, S)
    TreeViewEdited(Sender, Node, *S);
  }
  else if (asEvent == _T("TreeViewAddition")) {
    FVCL_ARG_CALLEVENT(TTreeNode *, Node)
    TreeViewAddition(Sender, Node);
  }
  else if (asEvent == _T("TreeViewDeletion")) {
    FVCL_ARG_CALLEVENT(TTreeNode *, Node)
    TreeViewDeletion(Sender, Node);
  }

}

//---------------------------------------------------------------------------
// Création d'un nouvel objet TVCLTreeView.
//---------------------------------------------------------------------------
TBaseTree *TVCLTreeView::CreateNodeVirt(void) {
  TVCLTreeView *NewNode;

  NewNode = new TVCLTreeView(FTreeView, false);

  return NewNode;
}

//---------------------------------------------------------------------------
// Met à zéro la structure arborescente
//---------------------------------------------------------------------------
bool TVCLTreeView::ClearVirt(void) {
  FTreeView->Items->Clear();

  return true;

}

//---------------------------------------------------------------------------
// Insère un noeud dans une structure arborescente
//---------------------------------------------------------------------------
TBaseTree * TVCLTreeView::InsertNodeVirt(int Index, TBaseTree &NewNode) {
  TVCLTreeView * NewVCLNode;
	TVCLTreeView *VCLBrother;


  NewVCLNode = dynamic_cast<TVCLTreeView *> (&NewNode);
	if (!NewVCLNode) return NULL;  // Impossible d'insérer un noeud qui n'est pas un TVCLTreeView *

	if (Index == -1) {
		bNoEvent = true;
		FTreeView->Items->AddNode(NewVCLNode->TreeNode, TreeNode, NewNode.Text, &NewNode, naAddChild);
		bNoEvent = false;
	}
	else {
		VCLBrother = dynamic_cast<TVCLTreeView *> (GetNodeVirt(Index));
		if (!VCLBrother) return NULL;  // Index incorrect
		bNoEvent = true;
		FTreeView->Items->AddNode(NewVCLNode->TreeNode, VCLBrother->TreeNode, NewNode.Text, &NewNode, naAdd);
		bNoEvent = false;
	}

	return NewVCLNode;
}

//---------------------------------------------------------------------------
// Supprime un noeud dans une structure arborescente
//---------------------------------------------------------------------------
bool TVCLTreeView::SupprNodeVirt(int Index) {
  TVCLTreeView *VCLNode;


	if (Index >= TreeNode->Count) return false;

	if (bNoEvent) return true;

	VCLNode = reinterpret_cast<TVCLTreeView *> (TreeNode->Item[Index]->Data);
	if (!VCLNode) return false;

	VCLNode->bNoEvent = true;
	TreeNode->Item[Index]->Delete();
	VCLNode->bNoEvent = false;

	delete VCLNode;

	return true;
}

//---------------------------------------------------------------------------
// Modification du texte d'un noeud
//---------------------------------------------------------------------------
bool TVCLTreeView::SetTextVirt(AnsiString asNewText) {

  if (bNoEvent) return true;

	bNoEvent = true;

  TreeNode->Text = asNewText;

	bNoEvent = false;

  return true;
}

//---------------------------------------------------------------------------
// Renvoie le nombre de noeud enfants de la structure arborescente.
//---------------------------------------------------------------------------
int TVCLTreeView::GetCountVirt(void) {
  return FTreeView->Items->Count;
}

//---------------------------------------------------------------------------
// Lecture du noeud d'index Index
//---------------------------------------------------------------------------
TBaseTree * TVCLTreeView::GetNodeVirt(int Index) {
  if (Index >= TreeNode->Count) return NULL;
  return reinterpret_cast<TVCLTreeView *> (TreeNode->Item[Index]->Data);
}

//---------------------------------------------------------------------------
// Evènement
//---------------------------------------------------------------------------
void FASTCALL TVCLTreeView::TreeViewEdited(TObject *Sender,
      TTreeNode *Node, AnsiString &S) {
  TVCLTreeView *VCLNode;


  if (bNoEvent) return;

	bNoEvent = true;

  VCLNode = (TVCLTreeView *) (void *) Node->Data;
  if (VCLNode) VCLNode->Text = S;

	bNoEvent = false;

}

//---------------------------------------------------------------------------
void FASTCALL TVCLTreeView::TreeViewAddition(
      TObject *Sender, TTreeNode *Node) {

	if (bNoEvent) return;

  // $$$
}

//---------------------------------------------------------------------------
void FASTCALL TVCLTreeView::TreeViewDeletion(
      TObject *Sender, TTreeNode *Node) {
  TVCLTreeView *VCLNode;
  TVCLTreeView *VCLParent;
  int Index;


  if (bNoEvent) return;

	VCLNode = reinterpret_cast<TVCLTreeView *> (Node->Data);
  if (!VCLNode) return;

  VCLParent = dynamic_cast<TVCLTreeView *> (VCLNode->FParent);
  if (!VCLParent) return;

  Index = VCLNode->Index;
  if (Index == -1) return;

	VCLParent->bNoEvent = true;
  VCLParent->SupprNode(Index);
	VCLParent->bNoEvent = false;

}

//---------------------------------------------------------------------------

