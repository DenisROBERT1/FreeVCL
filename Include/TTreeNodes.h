//-----------------------------------------------------------------------------
//! @file TTreeNodes.h
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


//-----------------------------------------------------------------------------
// Dispositif anti-redéfinition
//-----------------------------------------------------------------------------

#ifdef In_TTreeNodesH
#error "Inclusion circulaire de TTreeNodes"
#endif // In_TTreeNodesH

#define In_TTreeNodesH

#ifndef TTreeNodesH
#define TTreeNodesH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <vector>
#include "TTreeNode.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TTreeView;


//---------------------------------------------------------------------------
// Enums et macros
//---------------------------------------------------------------------------

typedef int (CALLBACK *PFNTVCOMPARE)(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);


//-----------------------------------------------------------------------------
//! @class TTreeNodes
//!
//! @brief Définition de la classe TTreeNodes
//!
//! Cette classe encapsule une collection de noeuds d'un tree-view. Elle
//! possède les méthodes qui gèrent les noeuds dans leur ensemble (ajout,
//! suppression, déplacement de noeuds, lecture, parcours du tree-view).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTreeNodes: public TPersistent {
protected:
  HWND FHandle;
  TTreeView *FOwner;
	std::vector<TTreeNode *>ListNodes;
  int FUpdateCount;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TTreeNodes(TTreeView* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeurs
  //---------------------------------------------------------------------------

  virtual ~TTreeNodes(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud à la fin du tree-view
  //!
  //! Cette méthode crée et ajoute un noeud qui sera inséré au même niveau que
	//! le noeud @b Node et juste après lui (sauf si le tree-view est trié).
  //!
  //! @param[in]        Node Noeud frère du nouveau noeud
  //! @param[in]        S Libellé du nouveau noeud
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL Add(TTreeNode* Node, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud enfant d'un autre noeud
  //!
  //! Cette méthode crée et ajoute un noeud qui sera inséré à la fin des
	//! enfants du noeud @b Node (sauf si le tree-view est trié).
  //!
  //! @param[in]        Node Noeud parent du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddChild(TTreeNode* Node, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud enfant d'un autre noeud
  //!
  //! Cette méthode crée et ajoute un noeud qui sera inséré au début des
	//! enfants du noeud @b Node (sauf si le tree-view est trié).
  //!
  //! @param[in]        Node Noeud parent du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddChildFirst(TTreeNode* Node, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud enfant d'un autre noeud avec un objet
  //!
  //! Cette méthode fait comme TTreeNodes::AddChild, avec en plus
	//! l'enregistrement de données associées au noeud.
  //!
  //! @param[in]        Node Noeud parent du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //! @param[in]        Ptr Pointeur sur les données associées
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
	//!
	//! @warning Le tree-view ne devient pas propriétaire des données pointées
	//! par @b Ptr, c'est à dire que l'utilisateur est responsable de la
	//! libération de la mémoire correspondante après la destruction
	//! du tree-view.
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddChildObject(TTreeNode* Node, const AnsiString S, void * Ptr);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud enfant d'un autre noeud avec un objet
  //!
  //! Cette méthode fait comme TTreeNodes::AddChildFirst, avec en plus
	//! l'enregistrement de données associées au noeud.
  //!
  //! @param[in]        Node Noeud parent du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //! @param[in]        Ptr Pointeur sur les données associées
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
	//!
	//! @warning Le tree-view ne devient pas propriétaire des données pointées
	//! par @b Ptr, c'est à dire que l'utilisateur est responsable de la
	//! libération de la mémoire correspondante après la destruction
	//! du tree-view.
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddChildObjectFirst(TTreeNode* Node, const AnsiString S, void * Ptr);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud avant un autre noeud
  //!
  //! Cette méthode crée et ajoute un noeud qui sera inséré au même niveau que
	//! le noeud @b Node et au début de la liste (sauf si le tree-view est trié).
  //!
  //! @param[in]        Node Noeud frère du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddFirst(TTreeNode* Node, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Ajout après création éventuelle d'un noeud relativement à un autre noeud
  //!
  //! Cette méthode crée un noeud si besoin (c'est à dire si Node est égal à NULL),
	//! et dans tous les cas l'insère dans le tree-view relativement au noeud
	//! @b Relative, avec une relation déterminée par le paramètre Method.
  //!
  //! @param[in]        Node Noeud à insérer (si NULL, il sera automatiquement créé)
  //! @param[in]        Relative Noeud parent ou frère (suivant Method) du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //! @param[in]        Ptr Pointeur sur les données associées, ou NULL
  //! @param[in]        Method Relation entre le nouveau noeud et le noeud @b Relative
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
	//!
	//! @warning Le tree-view ne devient pas propriétaire des données pointées
	//! par @b Ptr, c'est à dire que l'utilisateur est responsable de la
	//! libération de la mémoire correspondante après la destruction
	//! du tree-view.
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddNode(TTreeNode* Node, TTreeNode* Relative, const AnsiString S, void * Ptr, TNodeAttachMode Method);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud à la fin du tree-view
  //!
  //! Cette méthode fait comme TTreeNodes::Add, avec en plus l'enregistrement
	//! de données associées au noeud.
  //!
  //! @param[in]        Node Noeud frère du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //! @param[in]        Ptr Pointeur sur les données associées
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
	//!
	//! @warning Le tree-view ne devient pas propriétaire des données pointées
	//! par @b Ptr, c'est à dire que l'utilisateur est responsable de la
	//! libération de la mémoire correspondante après la destruction
	//! du tree-view.
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddObject(TTreeNode* Node, const AnsiString S, void * Ptr);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud au début du tree-view
  //!
  //! Cette méthode fait comme TTreeNodes::AddFirst, avec en plus
	//! l'enregistrement de données associées au noeud.
  //!
  //! @param[in]        Node Noeud frère du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //! @param[in]        Ptr Pointeur sur les données associées
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
	//!
	//! @warning Le tree-view ne devient pas propriétaire des données pointées
	//! par @b Ptr, c'est à dire que l'utilisateur est responsable de la
	//! libération de la mémoire correspondante après la destruction
	//! du tree-view.
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL AddObjectFirst(TTreeNode* Node, const AnsiString S, void * Ptr);

  //---------------------------------------------------------------------------
  //! @brief Création et ajout d'un noeud à la fin du tree-view
  //!
  //! Cette méthode crée et ajoute un noeud qui sera inséré au même niveau que
	//! le noeud @b Node et juste avant lui (sauf si le tree-view est trié).
  //!
  //! @param[in]        Node Noeud frère du noeud à créer
  //! @param[in]        S Libellé du nouveau noeud
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL Insert(TTreeNode* Node, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Ajout après création éventuelle d'un noeud au même niveau qu'un autre noeud
  //!
  //! Cette méthode crée un noeud si besoin (c'est à dire si Node est égal à NULL),
	//! et dans tous les cas l'insère dans le tree-view au même niveau que le noeud
	//! @b Sibling et juste avant lui (sauf si le tree-view est trié).
  //!
  //! @param[in]        Node Noeud à insérer (si NULL, il sera automatiquement créé)
  //! @param[in]        Sibling Noeud après lequel insérer le nouveau noeud
  //! @param[in]        S Libellé du nouveau noeud
  //! @param[in]        Ptr Pointeur sur les données associées
  //!
  //! @return @c TTreeNode * Pointeur sur le nouveau noeud
	//!
	//! @warning Le tree-view ne devient pas propriétaire des données pointées
	//! par @b Ptr, c'est à dire que l'utilisateur est responsable de la
	//! libération de la mémoire correspondante après la destruction
	//! du tree-view.
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL InsertNode(TTreeNode* Node, TTreeNode* Sibling, const AnsiString S, void * Ptr);

  //---------------------------------------------------------------------------
  //! @brief Copie d'un noeud et de ses enfants.
  //!
  //! Cette méthode copie un noeud et ses enfants (ses petits enfants, etc...)
	//! depuis un noeud source. L'arborescence, le libellé des noeuds, les
	//! images éventuelles et la propriété Data sont copiés intégralement.@n
	//! La méthode renvoie @b false si on tente de copier un noeud sur l'un de
	//! ses noeuds enfants (pour éviter une boucle infinie).
  //!
  //! @param[in]        NodeSrc Noeud à copier
  //! @param[in]        NodeDest Noeud destination
  //!
  //! @return @c bool true si Ok
	//!
	//! @warning Cette méthode copie également la propriété Data, si elle est
	//! présente. Attention donc si cette valeur Data contient un pointeur sur un
	//! objet, vous aurez deux pointeurs sur le même objet, avec les risques de
	//! destruction multiples que ça peut entraîner.
  //---------------------------------------------------------------------------

	bool FASTCALL CopyNode(TTreeNode* NodeSrc, TTreeNode* NodeDest);

  //---------------------------------------------------------------------------
  //! @brief Détermine si un noeud est enfant d'un autre noeud.
  //!
  //! Cette méthode détermine si le second noeud est le parent du premier, avec
	//! éventuellement des intermédiaires (c'est à dire qu'il peut aussi être le
	//! grand-parent, l'arrière grand-parent, etc...).@n
	//! La méthode renvoie @b true si les deux noeuds sont égaux.
  //!
  //! @param[in]        NodeChild Noeud enfant potentiel
  //! @param[in]        NodeParent Noeud parent potentiel
  //!
  //! @return @c bool true si NodeParent est le parent de NodeChild.
  //---------------------------------------------------------------------------

	bool FASTCALL IsChild(TTreeNode* NodeChild, TTreeNode* NodeParent);

  //---------------------------------------------------------------------------
  //! @brief Tri du tree-view
  //!
  //! Cette méthode trie les noeuds de premier niveau du tree-view, et
	//! éventuellement les sous-noeuds (si @b ARecurse == @b true). Le tri est
	//! fait par ordre alphabétique indépendant de la casse, sauf si l'évènement
	//! @b OnCompare est traité, ce qui permet dans ce cas d'avoir n'importe
	//! quel critère de tri.
  //!
  //! @param[in]        ARecurse true s'il faut aussi trier les sous-noeuds. false par défaut.
  //!
  //! @return @c bool @b true si Ok
  //---------------------------------------------------------------------------

	bool FASTCALL AlphaSort(bool ARecurse = false);

  //---------------------------------------------------------------------------
  //! @brief Copie d'un tree-view sur un autre
  //!
  //! Cette méthode initialise un tree-view à partir d'une autre. Les noeuds
	//! sont copiés, ainsi que les données associés aux noeuds.
  //!
  //! @param[in]        Source Treeview d'origine
  //---------------------------------------------------------------------------

	virtual void FASTCALL Assign(TPersistent* Source);

  //---------------------------------------------------------------------------
  //! @brief Début d'une série de modifications
  //!
  //! Cette méthode permet de faire une série de modifications sans que ça
	//! déclenche un raffraichissement de la fenêtre. L'utilisateur doit ensuite
	//! appeler la méthode TTreeNodes::EndUpdate autant de fois qu'il a appelé
	//! @b BeginUpdate (si ce n'est pas fait, les modifications risquent de ne
	//! plus être visibles).
	//!
	//! @sa TTreeNodes::EndUpdate
  //---------------------------------------------------------------------------

	void FASTCALL BeginUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Destruction de tous les noeuds
  //!
  //! Cette méthode detruit tous les noeuds et les sous-noeuds du tree-view.
	//! Si vous avez affecté des données aux noeuds, n'oubliez pas de détruire
	//! ces données juste avant ou après l'appel de cette méthode.
  //---------------------------------------------------------------------------

	void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Tri du tree-view en utilisant une fonction personnalisée.
  //!
  //! Cette méthode permet de trier les noeuds en utilisant une fonction de
	//! tri personnalisée.
  //!
  //! @param[in]        SortProc Fonction de tri
  //! @param[in]        Data Donnée passée en paramètre à la fonction de tri
  //! @param[in]        ARecurse Tri récursif
  //!
  //! @return @c bool @b true si Ok
  //---------------------------------------------------------------------------

	bool FASTCALL CustomSort(PFNTVCOMPARE SortProc, int Data, bool ARecurse = false);

  //---------------------------------------------------------------------------
  //! @brief Destruction d'un noeud
  //!
  //! Cette méthode détruit le noeud @b Node. Si vous avez affecté des données
	//! à ce noeud, n'oubliez pas de détruire ces données juste avant ou après
	//! l'appel de cette méthode.
  //!
  //! @param[in]        Node Noeud à détruire
  //---------------------------------------------------------------------------

	void FASTCALL Delete(TTreeNode* Node);

  //---------------------------------------------------------------------------
  //! @brief Fin d'une série de modifications
  //!
  //! Cette méthode termine une série de modification après l'appel de
	//! TTreeNodes::BeginUpdate.
	//!
	//! @sa TTreeNodes::BeginUpdate
  //---------------------------------------------------------------------------

	void FASTCALL EndUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Noeud racine du tree-view
  //!
  //! Cette méthode renvoie le noeud racine du tree-view.
  //!
  //! @return @c TTreeNode * Pointeur sur le noeud racine
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL GetFirstNode(void);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un noeud dans la liste des noeuds
  //!
  //! Cette méthode insère un noeud dans la liste des noeuds.
  //!
  //! @param[in]        Node Noeud à insérer
  //!
	//! @warning Ne pas utiliser cette méthode, elle est uniquement utilisée en
	//! interne pour mémoriser les noeuds dans le tree-view.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	void FASTCALL InsertComponent(TTreeNode* Node);

  //---------------------------------------------------------------------------
  //! @brief Noeud à partir d'un handle
  //!
  //! Cette méthode renvoie le noeud à partir d'un handle défini.
  //!
  //! @param[in]        hItem Handle système du noeud à trouver
  //!
  //! @return @c TTreeNode * Pointeur sur le noeud trouvé, ou NULL sinon.
  //---------------------------------------------------------------------------

	TTreeNode* FASTCALL GetNode(HTREEITEM hItem);

  //---------------------------------------------------------------------------
  //! @brief Noeud à partir d'un index
  //!
  //! Cette méthode renvoie le noeud d'index @b index. Le comptage prend en
	//! compte les noeuds et les sous-noeuds.
  //!
  //! @param[in]        Index Index du noeud
  //!
  //! @return @c TTreeNode * Pointeur sur le noeud d'index @b Index
  //---------------------------------------------------------------------------

	TTreeNode* operator[](int Index);


  //---------------------------------------------------------------------------

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Count: Nombre total de noeuds
	//!
	//! Cette propriété renvoie le nombre total de noeuds, en comptant les sous-
	//! noeuds et le noeud racine (c'est à dire qu'elle est toujours supérieure
	//! ou égale à 1).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNodes, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété FirstNode: Noeud racine
	//!
	//! Cette propriété renvoie le noeud racine du tree-view.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNodes, TTreeNode *, FirstNode);

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Handle du tree-view associé
	//!
	//! Cette propriété renvoie le handle du tree-view auquel appartiennent les
	//! noeuds.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNodes, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Item: Liste des noeuds
	//!
	//! Cette propriété renvoie un noeud à partir de son indice. Le premier noeud
	//! (d'indice 0) est le noeud racine.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTreeNodes, TTreeNode *, Item);

  //---------------------------------------------------------------------------
  //! @brief Propriété Owner: propriétaire de l'objet
	//!
	//! Cette propriété renvoie le propriétaire de l'objet @b this (objet chargé
	//! de sa destruction).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNodes, TTreeView *, Owner);


  //@}

};


#else  // TTreeNodesH

class TTreeNodes;

#endif  // TTreeNodesH


#undef In_TTreeNodesH
