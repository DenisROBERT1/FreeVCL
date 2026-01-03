//-----------------------------------------------------------------------------
//! @file TBaseTree.cpp
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

#include "TBaseTree.h"

//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TBaseTree::TBaseTree(void): TPersistent() {
  FParent = NULL;
	FIndex = -1;
}

//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TBaseTree::~TBaseTree(void) {
}

//---------------------------------------------------------------------------
bool TBaseTree::Clear(void) {
  OnClear(this);

  return ClearVirt();
}

//---------------------------------------------------------------------------
bool TBaseTree::ClearVirt(void) {
  int i;
  int Count;
  TBaseTree * Node;


  Count = Get_Count();
  for (i = Count - 1; i >= 0; i--) {
    Node = GetNodeVirt(i);
    Node->Clear();
    if (!SupprNodeVirt(i)) return false;
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TBaseTree::Get_Count(void) {
  return GetCountVirt();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Node
//---------------------------------------------------------------------------

TBaseTree * TBaseTree::Get_Node(int Index) {
  return GetNodeVirt(Index);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TBaseTree::Get_Text(void) {
  return FText;
}

bool TBaseTree::Set_Text(AnsiString NewText) {
  if (FText != NewText) {
    FText = NewText;
    SetTextVirt(NewText);
    OnChangeText(this, NewText);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Parent
//---------------------------------------------------------------------------

TBaseTree *TBaseTree::Get_Parent(void) {
  return FParent;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Index
//---------------------------------------------------------------------------

int TBaseTree::Get_Index(void) {
  return FIndex;
}

//---------------------------------------------------------------------------
// Copie d'une structure arborescente dans une autre.
//---------------------------------------------------------------------------
bool TBaseTree::Assign(TBaseTree &Source) {
  int i;
  TBaseTree *NewNode;


  Clear();
  for (i = 0; i < Source.Count; i++) {
    NewNode = CreateNodeVirt();
    NewNode->Assign(*Source.GetNodeVirt(i));
    AddNode(*NewNode);
  }

  return true;
}

//---------------------------------------------------------------------------
// Ajout d'un noeud
//---------------------------------------------------------------------------
TBaseTree * TBaseTree::CreateNode(void) {
  return CreateNodeVirt();
}

//---------------------------------------------------------------------------
// Ajout d'un noeud
//---------------------------------------------------------------------------
TBaseTree * TBaseTree::InsertNode(int Index, TBaseTree &Node) {
  TBaseTree * NewNode;


  NewNode = InsertNodeVirt(Index, Node);
  if (NewNode) {
    NewNode->FParent = this;
		if (Index != -1) NewNode->FIndex = Index;
		else NewNode->FIndex = GetCountVirt() - 1;
    OnInsertNode(this, Index, NewNode);
  }

  return NewNode;
}

//---------------------------------------------------------------------------
// Insère un noeud dans une structure arborescente
//---------------------------------------------------------------------------
TBaseTree * TBaseTree::InsertNode(int Index, AnsiString asText) {
  TBaseTree * NewNode = NULL;

  NewNode = CreateNodeVirt();
  NewNode->FText = asText;
  if (!InsertNode(Index, *NewNode)) {
    delete NewNode;
    NewNode = NULL;
  }

  return NewNode;
}

//---------------------------------------------------------------------------
// Ajout d'un noeud
//---------------------------------------------------------------------------
TBaseTree * TBaseTree::AddNode(TBaseTree &NewNode) {
  return InsertNode(-1, NewNode);
}

//---------------------------------------------------------------------------
// Ajout d'un noeud
//---------------------------------------------------------------------------
TBaseTree * TBaseTree::AddNode(AnsiString asText) {
  return InsertNode(-1, asText);
}

//---------------------------------------------------------------------------
// Supprime un noeud dans une structure arborescente
//---------------------------------------------------------------------------
bool TBaseTree::SupprNode(int Index) {

  OnSupprNode(this, Index);
  SupprNodeVirt(Index);

  return true;
}

//---------------------------------------------------------------------------
// Noeud racine
//---------------------------------------------------------------------------
TBaseTree * TBaseTree::RootNode(void) {
  if (FParent) return FParent->RootNode();
  return this;
}

//---------------------------------------------------------------------------

