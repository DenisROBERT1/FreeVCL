//-----------------------------------------------------------------------------
//! @file TSynchroTree.cpp
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

#include "TSynchroTree.h"
#include "TBaseTree.h"

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TSynchroTree::TSynchroTree(void):TPersistent() {
  bNoEvent = false;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TSynchroTree::~TSynchroTree(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Trees
//---------------------------------------------------------------------------

TBaseTree * TSynchroTree::Get_Tree(int i) {
  return FTree[i];
}

//---------------------------------------------------------------------------
bool FASTCALL TSynchroTree::RegisterTree(TBaseTree &NewTree) {
  FTree.push_back(&NewTree);
  SET_EVENT(NewTree, TNotifyEvent, OnClear, TSynchroTree, this, TreeClear);
  SET_EVENT(NewTree, TNotifyEvent, OnInsertNode, TSynchroTree, this, TreeInsertNode);
  SET_EVENT(NewTree, TNotifyEvent, OnSupprNode, TSynchroTree, this, TreeSupprNode);
  SET_EVENT(NewTree, TNotifyEvent, OnChangeText, TSynchroTree, this, TreeChangeText);

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TSynchroTree::UnregisterTree(TBaseTree &Tree) {
  int i;


  SET_EVENT(Tree, TNotifyEvent, OnClear, TSynchroTree, NULL, NULL);
  SET_EVENT(Tree, TNotifyEvent, OnInsertNode, TSynchroTree, NULL, NULL);
  SET_EVENT(Tree, TNotifyEvent, OnSupprNode, TSynchroTree, NULL, NULL);
  SET_EVENT(Tree, TNotifyEvent, OnChangeText, TSynchroTree, NULL, NULL);
  for (i = 0; i < (int) FTree.size(); i++) {
    if (FTree[i] == &Tree) {
      FTree.erase(FTree.begin() + i);
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TSynchroTree::RegisterNode(TBaseTree &NewNode) {
  SET_EVENT(NewNode, TNotifyEvent, OnClear, TSynchroTree, this, TreeClear);
  SET_EVENT(NewNode, TNotifyEvent, OnInsertNode, TSynchroTree, this, TreeInsertNode);
  SET_EVENT(NewNode, TNotifyEvent, OnSupprNode, TSynchroTree, this, TreeSupprNode);
  SET_EVENT(NewNode, TNotifyEvent, OnChangeText, TSynchroTree, this, TreeChangeText);

  return true;
}

//---------------------------------------------------------------------------
// Evènement "Clear"
//---------------------------------------------------------------------------
void FASTCALL TSynchroTree::TreeClear(TObject *Sender) {
  int i;


  if (bNoEvent) return;

  bNoEvent = true;
	// On synchronise le clear => on applique le clear sur tous les autres trees
  for (i = 0; i < (int) FTree.size(); i++) {
    if (FTree[i] != Sender) FTree[i]->Clear();
  }
  bNoEvent = false;

}

//---------------------------------------------------------------------------
// Evènement "Ajout d'un noeud"
//---------------------------------------------------------------------------
void FASTCALL TSynchroTree::TreeInsertNode(TObject *Sender, int Index, TBaseTree &Node) {
  int i;
  TBaseTree* Tree;
  TBaseTree* NodeSender;
  TBaseTree* ParentNode;
  TBaseTree* NewNode;


  if (bNoEvent) return;

  bNoEvent = true;
  RegisterNode(Node);
  NodeSender = dynamic_cast<TBaseTree *> (Sender);
  if (NodeSender == NULL) return;
  Tree = NodeSender->RootNode();
  for (i = 0; i < (int) FTree.size(); i++) {
    if (FTree[i] != Tree) {
      ParentNode = CorrespNode(*FTree[i], *Tree, *NodeSender);
      if (ParentNode) {
        NewNode = FTree[i]->CreateNode();
        NewNode->Text = Node.Text;
        ParentNode->InsertNode(Index, *NewNode);
        RegisterNode(*NewNode);
      }
    }
  }
  bNoEvent = false;

}

//---------------------------------------------------------------------------
// Evènement "Suppression d'un noeud"
//---------------------------------------------------------------------------
void FASTCALL TSynchroTree::TreeSupprNode(TObject *Sender, int Index) {
  int i;
  TBaseTree* Tree;
  TBaseTree* NodeSender;
  TBaseTree* ParentNode;


  if (bNoEvent) return;

  bNoEvent = true;
  NodeSender = dynamic_cast<TBaseTree *> (Sender);
  if (NodeSender == NULL) return;
  Tree = NodeSender->RootNode();
  for (i = 0; i < (int) FTree.size(); i++) {
    if (FTree[i] != Tree) {
      ParentNode = CorrespNode(*FTree[i], *Tree, *NodeSender);
      if (ParentNode) ParentNode->SupprNode(Index);
    }
  }
  bNoEvent = false;

}

//---------------------------------------------------------------------------
// Evènement "Renommage d'un noeud"
//---------------------------------------------------------------------------
void FASTCALL TSynchroTree::TreeChangeText(TObject *Sender, AnsiString asNewText) {
  int i;
  TBaseTree* Tree;
  TBaseTree* NodeSender;
  TBaseTree* Node;


  if (bNoEvent) return;

  bNoEvent = true;
  NodeSender = dynamic_cast<TBaseTree *> (Sender);
  if (NodeSender == NULL) return;
  Tree = NodeSender->RootNode();
  for (i = 0; i < (int) FTree.size(); i++) {
    if (FTree[i] != Tree) {
      Node = CorrespNode(*FTree[i], *Tree, *NodeSender);
      if (Node) {
        Node->Text = asNewText;
      }
    }
  }
  bNoEvent = false;

}

//---------------------------------------------------------------------------
// Correspondance d'un noeud d'un tree dans un autre tree.
//---------------------------------------------------------------------------
TBaseTree * FASTCALL TSynchroTree::CorrespNode(TBaseTree &TreeDest, TBaseTree &TreeSrc, TBaseTree &Node) {
  TBaseTree* ParentNode;


  if (&Node == &TreeSrc) return &TreeDest;
  if (Node.Parent == NULL) return NULL;
  ParentNode = CorrespNode(TreeDest, TreeSrc, *Node.Parent);
  if (!ParentNode) return NULL;

  return ParentNode->Node[Node.Index];
}

//---------------------------------------------------------------------------

