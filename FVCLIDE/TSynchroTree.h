//-----------------------------------------------------------------------------
//! @file TSynchroTree.h
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

#ifndef TSynchroTreeH
#define TSynchroTreeH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <vector>

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TBaseTree;

//---------------------------------------------------------------------------
//! @class SynchroTree
//!
//! @brief Synchronisation de plusieurs tree-views
//!
//! Cette classe (@c SynchroTree) permet de synchroniser plusieurs
//! tree-views pour qu'ils aient tous le même contenu. L'ajout d'un noeud dans
//! un tree-view ajoute automatiquement le noeud dans les autres tree-views, par
//! exemple.
//!
//! @author D. Robert
//---------------------------------------------------------------------------

class TSynchroTree: public TPersistent {
private:
  std::vector<TBaseTree *> FTree;
  bool bNoEvent;

protected:

public:

  //! @name Constructeurs / destructeurs
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TSynchroTree(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TSynchroTree(void);

  //@}


  //! @name Méthodes
  //@{

  FVCL_BEGIN_EVENTS

  //---------------------------------------------------------------------------
  //! @brief Traitement de l'évènement "Clear".
  //!
  //! Cette fonction traite l'évènement déclenclé lorsqu'onde mise à zéro un
  //! tree-view.
  //---------------------------------------------------------------------------
  void FASTCALL TreeClear(TObject *Sender);

  //---------------------------------------------------------------------------
  //! @brief Traitement de l'évènement d'insertion d'un noeud.
  //!
  //! Cette fonction traite l'évènement déclenclé lorsqu'on insère un noeud.
  //!
  //! @param[in]        Sender Noeud parent de celui venant d'être créé
  //! @param[in]        Index Index du noeud (-1 = à la fin)
  //! @param[in]        Node Noeud venant d'être créé
  //---------------------------------------------------------------------------
  void FASTCALL TreeInsertNode(TObject *Sender, int Index, TBaseTree &Node);

  //---------------------------------------------------------------------------
  //! @brief Traitement de l'évènement suppression d'un noeud.
  //!
  //! Cette fonction traite l'évènement déclenclé lorsqu'on supprime un noeud.
  //!
  //! @param[in]        Sender Noeud parent de celui qui va être détruit (NULL si noeud racine)
  //! @param[in]        Index Index du noeud
  //---------------------------------------------------------------------------
  void FASTCALL TreeSupprNode(TObject *Sender, int Index);

  //---------------------------------------------------------------------------
  //! @brief Traitement de l'évènement renommage d'un noeud.
  //!
  //! Cette fonction traite l'évènement déclenclé lorsqu'on modifie le texte
  //! d'un noeud.
  //!
  //! @param[in]        Sender Noeud venant d'être modifié
  //! @param[in]        asNewText Nouveau libellé du noeud
  //---------------------------------------------------------------------------
  void FASTCALL TreeChangeText(TObject *Sender, AnsiString asNewText);

  FVCL_END_EVENTS

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un tree-view
  //!
  //! Cette fonction permet d'enregistrer un nouveau tree-view à synchroniser.
  //! Si ce tree-view n'est pas le premier de la liste, il est automatiquement
  //! initialisé avec les autres.
  //!
  //! @param[in]        NewTree Nouveau tree-view
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool FASTCALL RegisterTree(TBaseTree &NewTree);

  //---------------------------------------------------------------------------
  //! @brief Désenregistrement d'un tree-view
  //!
  //! Cette fonction permet de désactiver la synchronisation d'un tree-view.
  //! Il est conseillé de le faire avant de détruire un tree-view qui sinon
  //! déclencherait plein de OnSupprNode.
  //!
  //! @param[in]        NewTree Nouveau tree-view
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool FASTCALL UnregisterTree(TBaseTree &Tree);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un nouveau noeud
  //!
  //! Cette fonction permet d'enregistrer un nouveau noeud. Elle initialise
  //! les évènements, entre autre.
  //!
  //! @param[in]        NewNode Nouveau noeud
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool FASTCALL RegisterNode(TBaseTree &NewNode);

  //---------------------------------------------------------------------------
  //! @brief Correspondance d'un noeud d'un tree dans un autre tree.
  //!
  //! Cette fonction permet de trouver le noeud correspondant à un noeud donné
  //! Node dans un autre tree que celui auquel appartient Node.
  //!
  //! @param[in]        TreeDest Tree dans lequel rechercher l'équivalent.
  //! @param[in]        TreeSrc Tree contenant Node.
  //! @param[in]        Node Noeud dont il faut rechercher l'équivalent.
  //!
  //! @return @c TBaseTree Adresse du noeud équivalent, ou NULL.
  //---------------------------------------------------------------------------
  TBaseTree * FASTCALL CorrespNode(TBaseTree &TreeDest, TBaseTree &TreeSrc, TBaseTree &Node);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Trees
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TSynchroTree, TBaseTree *, Tree);

  //@}

};


#endif

