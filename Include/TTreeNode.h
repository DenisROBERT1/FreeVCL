//-----------------------------------------------------------------------------
//! @file TTreeNode.h
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

#ifdef In_TTreeNodeH
#error "Inclusion circulaire de TTreeNode"
#endif // In_TTreeNodeH

#define In_TTreeNodeH

#ifndef TTreeNodeH
#define TTreeNodeH


//---------------------------------------------------------------------------
// Conflits avec <WindowsX.h>
//---------------------------------------------------------------------------

#ifdef GetFirstChild
#undef GetFirstChild
#endif  // GetFirstChild

#ifdef GetNextSibling
#undef GetNextSibling
#endif  // GetNextSibling

#ifdef GetPrevSibling
#undef GetPrevSibling
#endif  // GetPrevSibling


//---------------------------------------------------------------------------
// Pré-processeur
//---------------------------------------------------------------------------

#ifndef DOXYGEN

#ifndef TreeView_GetCheckState
#define TreeView_GetCheckState(hwndTV, hti) ((((UINT)(::SendMessage((hwndTV), TVM_GETITEMSTATE, (WPARAM)(hti), TVIS_STATEIMAGEMASK))) >> 12) -1)
#endif // TreeView_GetCheckState

#ifndef TreeView_SetCheckState
#define TreeView_SetCheckState(hwndTV, hti, fCheck) TreeView_SetItemState(hwndTV, hti, INDEXTOSTATEIMAGEMASK((fCheck)?2:1), TVIS_STATEIMAGEMASK)
#endif // TreeView_SetCheckState

#endif  // DOXYGEN


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TNodeAttachMode {
	naAdd,                        //!< Ajout ou déplacement au même niveau et à la fin 
	naAddFirst,                   //!< Ajout ou déplacement au même niveau et au début
	naAddChild,                   //!< Ajout ou déplacement au niveau inférieur (enfants) et à la fin
	naAddChildFirst,              //!< Ajout ou déplacement au niveau inférieur (enfants) et au début
	naInsert                      //!< Ajout ou déplacement au même niveau et juste au dessus
};

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TTreeNodes;
class TTreeView;


//-----------------------------------------------------------------------------
//! @class TTreeNode
//!
//! @brief Définition de la classe TTreeNode
//!
//! Cette classe décrit un noeud d'un tree-view.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTreeNode: public TPersistent {
protected:
  int FAbsoluteIndex;
  int FCount;
  bool FCut;
  void *FData;
  bool FDeleting;
  bool FDropTarget;
  bool FFocused;
  HWND FHandle;
  bool FHasChildren;
  TImageIndex FImageIndex;
  HTREEITEM FItemId;
  int FLevel;
  int FOverlayIndex;
  TTreeNodes *FOwner;
  TTreeNode *FParent;
  int FSelectedIndex;
  int FStateIndex;
  TTreeView *FTreeView;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTreeNode(TTreeNodes* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTreeNode(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Tri alphabétique
  //!
  //! Cette méthode permet de trier les enfants d'un noeud par ordre
	//! alphabétique.
  //!
  //! @param[in]        ARecurse Si positionné à true: les enfants des enfants sont également triés
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool FASTCALL AlphaSort(bool ARecurse = false);

  //---------------------------------------------------------------------------
  //! @brief Copie d'un noeud
  //!
  //! Cette méthode copie un objet externe. 
  //!
  //! @param[out]       Source Objet à copier
  //---------------------------------------------------------------------------
	virtual void FASTCALL Assign(TPersistent* Source);

  //---------------------------------------------------------------------------
  //! @brief Réduction d'un noeud
  //!
  //! Cette méthode réduit un noeud, comme l'appui sur la croix correspondante
	//! du noeud.
  //!
  //! @param[in]        Recurse Si true: les enfants sont également réduits
  //---------------------------------------------------------------------------
	void FASTCALL Collapse(bool Recurse);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un noeud
  //!
  //! Cette méthode supprime le noeud du tree-view et le détruit.
  //---------------------------------------------------------------------------
	void FASTCALL Delete(void);

  //---------------------------------------------------------------------------
  //! @brief Suppression des noeuds enfants
  //!
  //! Cette méthode supprime et détruit tous les noeuds enfants.
  //---------------------------------------------------------------------------
	void FASTCALL DeleteChildren(void);

  //---------------------------------------------------------------------------
  //! @brief Rectangle enveloppe du noeud
  //!
  //! Cette méthode renvoie le rectangle enveloppant le noeud. Si le paramètre
	//! TextOnly est à true, seul le texte du noeud est pris en compte. Sinon
	//! la branche qui relie le noeud est aussi incluse dans le rectangle.
  //!
  //! @param[in]        TextOnly Enveloppe du texte uniquement
  //!
  //! @return @c TRect Rectangle d'enveloppe
  //---------------------------------------------------------------------------
	TRect FASTCALL DisplayRect(bool TextOnly);

  //---------------------------------------------------------------------------
  //! @brief Edition du texte du noeud
  //!
  //! Cette méthode permet à l'utilisateur de renommer un noeud. Elle ouvre
	//! une boîte d'édition dans lequel on peut entrer le nouveau nom, à
	//! condition que le tree-view n'ait pas la propriété @b ReadOnly. A noter:
	//! l'utilisateur peut également modifier le texte en faisant un double-clic
	//! "lent".
  //---------------------------------------------------------------------------
	void FASTCALL EditText(void);

  //---------------------------------------------------------------------------
  //! @brief Fin d'édition du texte du noeud
  //!
  //! Cette méthode provoque la fin de opération de saisie du texte d'un
	//! noeud. Si l'argument Cancel est à true, la modification est annulée. Sinon
	//! le noeud est renommé avec le texte saisi.
  //!
  //! @param[in]        Cancel Annulation de la saisie
	//!
	//! @sa EditText
  //---------------------------------------------------------------------------
	void FASTCALL EndEdit(bool Cancel);

  //---------------------------------------------------------------------------
  //! @brief Développement d'un noeud
  //!
  //! Cette méthode développe un noeud et éventuellement aussi ses noeuds
	//! enfants.
  //!
  //! @param[in]        Recurse Si true: les noeuds enfants sont développés aussi.
  //---------------------------------------------------------------------------
	void FASTCALL Expand(bool Recurse);

  //---------------------------------------------------------------------------
  //! @brief Premier enfant d'un noeud
  //!
  //! Cette méthode renvoie le premier noeud enfant du noeud.
  //!
  //! @return @c TTreeNode Noeud enfant, ou NULL si aucun.
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetFirstChild(void);

  //---------------------------------------------------------------------------
  //! @brief Handle du tree-view auquel appartient le noeud.
  //!
  //! Cette méthode renvoie le handle de l'objet TreeView auquel appartient le
	//! noeud.
  //!
  //! @return @c HWND Handle de tree-view
  //---------------------------------------------------------------------------
	HWND FASTCALL GetHandle(void);

  //---------------------------------------------------------------------------
  //! @brief Dernier enfant d'un noeud.
  //!
  //! Cette méthode renvoie le dernier noeud enfant du noeud.
  //!
  //! @return @c TTreeNode Noeud enfant, ou NULL si aucun.
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetLastChild(void);

  //---------------------------------------------------------------------------
  //! @brief Noeud suivant
  //!
  //! Cette méthode renvoie le noeud suivant dans le tree-view, quel que soit
	//! le niveau de celui-ci (noeud enfant, frère ou noeud du niveau supérieur).
  //!
  //! @return @c TTreeNode Noeud suivant ou NULL si c'est le dernier
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetNext(void);

  //---------------------------------------------------------------------------
  //! @brief Enfant suivant
  //!
  //! Cette méthode renvoie le noeud suivant d'un noeud enfant, c'est à dire
	//! le premier noeud de même niveau que le noeud enfant qui se trouve sous le
	//! noeud enfant.
  //!
  //! @param[in]        TreeNode Noeud enfant
  //!
  //! @return @c TTreeNode Noeud enfant suivant, ou NULL
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetNextChild(TTreeNode* TreeNode);

  //---------------------------------------------------------------------------
  //! @brief Noeud frère suivant
  //!
  //! Cette méthode renvoie le premier frère du noeud, c'est à dire le premier
	//! noeud de même niveau qui se trouve sous le noeud courant.
  //!
  //! @return @c TTreeNode Noeud frère, ou NULL si aucun
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetNextSibling(void);

  //---------------------------------------------------------------------------
  //! @brief Noeud visible suivant
  //!
  //! Cette méthode renvoie le noeud suivant, en tenant compte de l'état
	//! développé ou réduit des noeuds.
  //!
  //! @return @c TTreeNode Noeud visible suivant, ou NULL si aucun.
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetNextVisible(void);

  //---------------------------------------------------------------------------
  //! @brief Noeud précédent
  //!
  //! Cette méthode renvoie le noeud précédent dans le tree-view, quel que soit
	//! le niveau de celui-ci (noeud enfant, frère ou parent).
  //!
  //! @return @c TTreeNode Noeud précédent ou NULL si c'est le premier
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetPrev(void);

  //---------------------------------------------------------------------------
  //! @brief Enfant précédent
  //!
  //! Cette méthode renvoie le noeud précédent d'un noeud enfant, c'est à dire
	//! le premier noeud de même niveau que le noeud enfant qui se trouve au
	//! dessus du noeud enfant.
  //!
  //! @param[in]        TreeNode Noeud enfant
  //!
  //! @return @c TTreeNode Noeud enfant précédent, ou NULL
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetPrevChild(TTreeNode* TreeNode);

  //---------------------------------------------------------------------------
  //! @brief Noeud frère précédent
  //!
  //! Cette méthode renvoie le premier frère du noeud, c'est à dire le premier
	//! noeud de même niveau qui se trouve au dessus du noeud courant.
  //!
  //! @return @c TTreeNode Noeud frère, ou NULL si aucun
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetPrevSibling(void);

  //---------------------------------------------------------------------------
  //! @brief Noeud visible précédent
  //!
  //! Cette méthode renvoie le noeud précédent, en tenant compte de l'état
	//! développé ou réduit des noeuds.
  //!
  //! @return @c TTreeNode Noeud visible précédent, ou NULL si aucun.
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetPrevVisible(void);

  //---------------------------------------------------------------------------
  //! @brief Test si un noeud a un parent
  //!
  //! Cette méthode teste si un noeud a un parent, ou si c'est le noeud racine.
  //!
  //! @param[in]        TreeNode Noeud à tester
  //!
  //! @return @c bool true si le noeud a un parent
  //---------------------------------------------------------------------------
	bool FASTCALL HasAsParent(TTreeNode* TreeNode);

  //---------------------------------------------------------------------------
  //! @brief Renvoie la position d'un noeud enfant immédiat du noeud parent.
  //!
  //! Cette méthode renvoie le numéro d'ordre du noeud enfant passé en
	//! paramètre, en ne comptant que les noeuds enfants (et non les petits-
	//! enfants) du noeud courant.
  //!
  //! @param[in]        TreeNode Noeud enfant à tester
  //!
  //! @return @c int Numéro d'ordre
  //---------------------------------------------------------------------------

  int FASTCALL IndexOf(TTreeNode* TreeNode);

  //---------------------------------------------------------------------------
  //! @brief Rend visible un noeud.
  //!
  //! Cette méthode développe le noeud parent (et grand parent, ...) d'un
	//! noeud, afin qu'il soit visible.
  //---------------------------------------------------------------------------

  void FASTCALL MakeVisible(void);

  //---------------------------------------------------------------------------
  //! @brief Déplace le noeud à un autre emplacement du TreeView.
  //!
  //! Cette méthode déplace le noeud courant de telle sorte qu'il devienne
	//! frère ou bien enfant (suivant paramètre Mode) du noeud destination.
  //!
  //! @param[in]        Destination Noeud desination
  //! @param[in]        Mode Mode de rattachement (voir @b TNodeAttachMode)
  //---------------------------------------------------------------------------

  virtual void FASTCALL MoveTo(TTreeNode* Destination, TNodeAttachMode Mode);

  //---------------------------------------------------------------------------
  //! @brief Comme la propriété Item.
  //!
  //! Cet opérateur retourne un noeud à partir de son index.
  //!
  //! @param[in]        Index Index du noeud à lire
  //!
  //! @return @c TTreeNode Noeud d'index @b Index
  //---------------------------------------------------------------------------

  TTreeNode* operator[](int Index);

  //---------------------------------------------------------------------------
  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété AbsoluteIndex: Index absolu
	//!
	//! Cette propriété définit l'index absolu du noeud dans le tree-view, en
	//! comptant tous les noeuds enfants et parents qu'ils soient visibles ou
	//! pas. Le premier noeud du tree-view (noeud racine) a l'index zéro.
	//!
	//! @sa Index
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, int, AbsoluteIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété Count: Nombre total de noeuds
	//!
	//! Cette propriété indique le nombre total de noeuds dans le tree-view.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNode, int, Count);


  //---------------------------------------------------------------------------
  //! @brief Propriété Cut: noeud source d'un copié-collé
	//!
	//! Cette propriété indique les noeuds qui ont été sélectionnés dans un
	//! coupé-collé. Ces noeuds sont représentés d'une façon légèrement
	//! différente.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, bool, Cut);


  //---------------------------------------------------------------------------
  //! @brief Propriété Data: donnée associée
	//!
	//! Cette propriété permet d'associer un objet à un noeud du tree-view.
	//! Utilisez cette propriété pour associer une valeur quelconque à un ou
	//! plusieurs noeuds (en principe un pointeur, mais éventuellement, avec
	//! précautions, un entier avec un @b static_cast)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, void *, Data);


  //---------------------------------------------------------------------------
  //! @brief Propriété Deleting: Noeud en attente de suppression
	//!
	//! Cette propriété indique que le noeud est en train d'être supprimé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNode, bool, Deleting);


  //---------------------------------------------------------------------------
  //! @brief Propriété DropTarget: Noeud destination d'un copié-collé
	//!
	//! Cette propriété indique que le noeud est la destination d'une opération
	//! de copié-collé ou de coupé-collé. Dans ce cas, le noeud est dessiné de
	//! façon légèrement différente.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, bool, DropTarget);


  //---------------------------------------------------------------------------
  //! @brief Propriété Expanded: Noeud développé
	//!
	//! Cette propriété indique si le noeud est développé (c'est à dire si les
	//! noeuds enfants sont visibles). Dans ce cas, un signe "-" est dessiné dans
	//! le carré de l'arborescence à gauche du libellé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, bool, Expanded);


  //---------------------------------------------------------------------------
  //! @brief Propriété Focused: Focus sur le noeud
	//!
	//! Cette propriété indique que le noeud a le focus et que l'utilisateur
	//! peut modifier son libellé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, bool, Focused);


  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant
	//!
	//! Cette propriété permet de lire le handle système du composant
	//! @b TTreeView auquel appartient cet objet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, HWND, Handle);


  //---------------------------------------------------------------------------
  //! @brief Propriété HasChildren: Présence de noeuds enfants
	//!
	//! Cette propriété indique si le noeud a des noeuds enfants. Le fait
	//! d'affecter une valeur à cette propriété change la représentation du noeud
	//! (signe "+" ou "-" à gauche du libellé), mais ne modifie en rien le nombre
	//! des noeuds enfant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, bool, HasChildren);


  //---------------------------------------------------------------------------
  //! @brief Propriété ImageIndex: image associée au noeud
	//!
	//! Cette propriété permet de définir une image affichée à gauche du noeud
	//! lorsque celui-ci n'est pas sélectionné. L'index de l'image correspond à
	//! l'index dans l'ImageList définie par la propriété @b Images du TTreeView.
	//!
	//! @sa TTreeView::Images, TTreeNode::SelectedIndex
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, TImageIndex, ImageIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété Index: Index du noeud par rapport au parent
	//!
	//! Cette propriété indique la position du noeud par rapport à son parent.
	//! ne pas confondre cette propriété avec la propriété @b AbsoluteIndex qui
	//! indique la position dans l'ensemble du tree-view.
	//!
	//! @sa AbsoluteIndex
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNode, int, Index);


  //---------------------------------------------------------------------------
  //! @brief Propriété IsFirstNode: Premier noeud du tree-view
	//!
	//! Cette propriété indique si le noeud est le noeud racine du tree-view.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNode, bool, IsFirstNode);


  //---------------------------------------------------------------------------
  //! @brief Propriété Item: noeuds enfants
	//!
	//! Cette propriété permet d'accéder à la liste des enfants d'un noeud.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTreeNode, TTreeNode *, Item);


  //---------------------------------------------------------------------------
  //! @brief Propriété ItemId: Handle système du noeud
	//!
	//! Cette propriété permet de lire ou d'affecter le handle système d'item de
	//! tree-view à l'objet TTreeNode. Cette propriété est essentiellement
	//! utilisée en interne. Le fait de modifier cette propriété ne modifie pas
	//! le tree-view lui-même. Si vous souhaitez ajouter des noeuds enfants,
	//! utilisez les méthodes @b Add de l'objet TTreeNodes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, HTREEITEM, ItemId);


  //---------------------------------------------------------------------------
  //! @brief Propriété Level: niveau d'indentation du noeud
	//!
	//! Cette propriété indique le niveau du noeud dans le tree-view: zéro pour
	//! le noeud racine, 1 pour ses enfants, 2 pour les enfants des enfants, et
	//! ainsi de suite.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, int, Level);


  //---------------------------------------------------------------------------
  //! @brief Propriété OverlayIndex: image ajoutée
	//!
	//! Cette propriété permet de définir une image affichée en sur-impression
	//! de l'image définie par la propriété @b ImageIndex. L'index de l'image
	//! correspond à l'index d'overlay dans l'ImageList définie par la propriété
	//! @b Images du TTreeView.
	//!
	//! @sa TTreeView::Images, TTreeNode::ImageIndex, TImageList
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, int, OverlayIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété Owner: propriétaire
	//!
	//! Cette propriété permet de lire l'objet TTreeNodes auquel appartient le
	//! noeud.
	//!
	//! @sa TTreeNodes
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeNode, TTreeNodes *, Owner);


  //---------------------------------------------------------------------------
  //! @brief Propriété Parent: noeud parent
	//!
	//! Cette propriété permet de lire ou de définir l'objet TTreeNode parent
	//! du noeud. Le noeud racine n'a pas de parent (c'est à dire que cette
	//! propriété renvoie @b NULL).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, TTreeNode *, Parent);


  //---------------------------------------------------------------------------
  //! @brief Propriété Selected: état sélectionné du noeud
	//!
	//! Cette propriété indique si le noeud est sélectionné. Le ou les noeuds
	//! sélectionnés sont représentés en surbrillance.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, bool, Selected);


  //---------------------------------------------------------------------------
  //! @brief Propriété Checked: état coché du noeud
	//!
	//! Cette propriété permet de définir l'état coché de la check-box associée à
	//! un noeud. Cette propriété n'a d'effet que si le TreeView a la propriété
	//! @b CheckBoxes à @b true.
	//!
	//! @sa TTreeView::CheckBoxes
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, bool, Checked);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelectedIndex: image de sélection
	//!
	//! Cette propriété permet de définir une image affichée à gauche du noeud
	//! lorsque celui-ci est sélectionné. L'index de l'image correspond à l'index
	//! dans l'ImageList définie par la propriété @b Images du TTreeView.
	//!
	//! @sa TTreeView::Images, TTreeNode::ImageIndex, TImageList
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, int, SelectedIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété StateIndex: image d'état
	//!
	//! Cette propriété permet de définir l'image indiquant l'état d'un noeud,
	//! par exemple une case à cocher avec deux états, "coché" et "non coché". Il
	//! est possible de définir jusqu'à 15 états. L'image correspondant à l'état
	//! est l'image d'index N + 1 dans l'ImageList définie par la propriété
	//! @b StateImages du TTreeView. La valeur -1 signifie qu'il n'y a pas
	//! d'image d'état. Cette image est affichée à gauche du texte, et de l'image
	//! définie par @b ImageIndex ou @b SelectedIndex si elle est présente.
	//!
	//! @sa TTreeView::StateImages, TImageList
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, int, StateIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété Text: libellé du noeud
	//!
	//! Cette propriété définit le texte du noeud.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, AnsiString, Text);


  //---------------------------------------------------------------------------
  //! @brief Propriété TreeView: TreeView parent
	//!
	//! Cette propriété permet de définir l'objet TTreeView auquel appartient le
	//! noeud.
	//!
	//! @sa TTreeView
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeNode, TTreeView *, TreeView);


  //@}

};


#else  // TTreeNodeH

class TTreeNode;

#endif  // TTreeNodeH


#undef In_TTreeNodeH
