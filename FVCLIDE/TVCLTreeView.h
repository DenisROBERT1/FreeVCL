//-----------------------------------------------------------------------------
//! @file TVCLTreeView.h
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

#ifndef TVCLTreeViewH
#define TVCLTreeViewH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "TBaseTree.h"


//---------------------------------------------------------------------------
//! @class TVCLTreeView
//!
//! @brief Tree-view VCL.
//!
//! Cette classe (@c TVCLTreeView) permet de synchroniser un tree-view VCL avec
//! d'autres tree-views.
//!
/*! @verbatim
    Un TTreeView VCL est composé de:

		TTreeView
		  TTreeNodes* Items
			  TTreeNode * Item[]
			  TTreeNode * Item[]
			  TTreeNode * Item[]
			  ...

		Chaque TVCLTreeView contient l'adresse d'un TTreeNode, et l'adresse de
		l'objet TTreeView propriétaire. Le TVCLTreeView racine (et uniquement lui)
    gère les évènements du TTreeView.
		Le TTreeNode est automatiquement créé lorsqu'on crée un TVCLTreeView, et
		est détruit avec lui.
		@endverbatim */
//!
//! @author D. Robert
//---------------------------------------------------------------------------

class TVCLTreeView: public TBaseTree {
private:
  TTreeView *FTreeView;
  TTreeNode *TreeNode;

  bool bNoEvent;

protected:

  //---------------------------------------------------------------------------
  //! @brief Création d'un nouvel objet TVCLTreeView.
  //!
  //! Cette fonction crée un nouvel objet de type TVCLTreeView.
  //!
  //! @return @c TBaseTree * Pointeur sur le nouvel onjet créé.
  //---------------------------------------------------------------------------
  virtual TBaseTree *CreateNodeVirt(void);

  //---------------------------------------------------------------------------
  //! @brief Met à zéro la structure arborescente
  //!
  //! Cette fonction vide entièrement la structure arborescente.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  virtual bool ClearVirt();

  //---------------------------------------------------------------------------
  //! @brief Insère un noeud dans une structure arborescente
  //!
  //! Cette fonction insère un noeud dans le tree à l'index Index.
  //!
  //! @param[in]        Index Index d'insertion (Premier = zéro, -1 = à la fin)
  //! @param[in]        NewNode Noeud à insérer
  //!
  //! @return @c TBaseTree * adresse du nouveau noeud si Ok, NULL sinon. Dans ce
  //! dernier cas, la fonction appelante est responsable de la destruction du
  //! noeud NewNode.
  //---------------------------------------------------------------------------
  virtual TBaseTree * InsertNodeVirt(int Index, TBaseTree &NewNode);

  //---------------------------------------------------------------------------
  //! @brief Supprime un noeud dans une structure arborescente
  //!
  //! Cette fonction supprime le noeud d'index Index du tree.
  //!
  //! @param[in]        Index Index du noeud à supprimer (Premier = zéro)
  //!
  //! @return @c bool true si Ok. Sinon, la fonction appelante est responsable
  //! de la destruction du noeud NewNode.
  //---------------------------------------------------------------------------
  virtual bool SupprNodeVirt(int Index);

  //---------------------------------------------------------------------------
  //! @brief Modification du texte d'un noeud
  //!
  //! Cette fonction change le texte d'un noeud.
  //!
  //! @param[in]        asNewText Nouveau texte du noeud
  //!
  //! @return @c bool true si Ok.
  //!
  //! @note Cette méthode est virtuelle. Par défaut elle ne fait rien. Elle peut
  //! être surchargée par les classes dérivées si besoin.
  //---------------------------------------------------------------------------
  virtual bool SetTextVirt(AnsiString asNewText);

  //---------------------------------------------------------------------------
  //! @brief Renvoie le nombre de noeud enfants de la structure arborescente.
  //!
  //! Cette fonction retourne le nombre de noeuds enfants directs du tree.
  //!
  //! @return @c int Nombre de noeuds. -1 si erreur (je ne vois pas bien quelles
  //! erreurs il pourrait y avoir).
  //---------------------------------------------------------------------------
  virtual int GetCountVirt(void);

  //---------------------------------------------------------------------------
  //! @brief Lecture du noeud d'index Index
  //!
  //! Cette fonction retourne l'adresse du noeud d'index Index
  //!
  //! @param[in]        Index Index du noeud à lire (Premier = zéro)
  //!
  //! @return @c TBaseTree * Pointeur sur le noeud
  //---------------------------------------------------------------------------
  virtual TBaseTree * GetNodeVirt(int Index);

public:


  //! @name Constructeurs / destructeurs
  //@{


  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TVCLTreeView(TTreeView * TreeView, bool bRoot = true);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TVCLTreeView(void);

  //@}

  //! @name Méthodes
  //@{

  FVCL_BEGIN_EVENTS
  void FASTCALL TreeViewEdited(TObject *Sender, TTreeNode *Node,
          AnsiString &S);
  void FASTCALL TreeViewAddition(TObject *Sender, TTreeNode *Node);

  void FASTCALL TreeViewDeletion(TObject *Sender, TTreeNode *Node);
  FVCL_END_EVENTS

  //@}

};


//---------------------------------------------------------------------------
#endif

