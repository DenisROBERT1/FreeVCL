//-----------------------------------------------------------------------------
//! @file TTreeView.h
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

#ifdef In_TTreeViewH
#error "Inclusion circulaire de TTreeView"
#endif // In_TTreeViewH

#define In_TTreeViewH

#ifndef TTreeViewH
#define TTreeViewH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#include "TTreeNodes.h"


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TMultiSelectStyles {
	msControlSelect,              //!< Multi-sélection avec la touche contrôle
	msShiftSelect,                //!< Multi-sélection avec la touche majuscules
	msVisibleOnly,                //!< Multi-sélection des contrôles visibles uniquement
	msSiblingOnly                 //!< Multi-sélection à un seul niveau
};

typedef Set<TMultiSelectStyles, msControlSelect, msSiblingOnly> TMultiSelectStyle;  //!< Combinaison de plusieurs TColorBoxStyle

enum TSortType {
	stNone,                        //!< Aucun tri
	stData,                        //!< Tri sur la valeur Data
	stText,                        //!< Tri sur le texte (Caption)
	stBoth                         //!< Tri sur le texte d'abord, puis sur la valeur Data
};


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnAddition.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnAddition)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDeletion.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDeletion)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnEdited.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnEdited)(TObject *Sender, TTreeNode *Node, AnsiString Text);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnEditing.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnEditing)(TObject *Sender, TTreeNode *Node, bool &AllowEdit);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnChanging.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnChanging)(TObject *Sender, TTreeNode *Node, bool &AllowChanging);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnChange2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnChange2)(TObject *Sender, TTreeNode *TreeNode);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnExpanding.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnExpanding)(TObject *Sender, TTreeNode *TreeNode);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnExpanded.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnExpanded)(TObject *Sender, TTreeNode *TreeNode);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnCollapsing.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnCollapsing)(TObject *Sender, TTreeNode *TreeNode);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnCollapsed.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnCollapsed)(TObject *Sender, TTreeNode *TreeNode);


//-----------------------------------------------------------------------------
//! @class TTreeView
//!
//! @brief Définition de la classe TTreeView
//! @image html TTreeView.jpg "TTreeView"
//!
//! Cette classe définit un composant représentant une liste arborescente. La
//! liste est composée de noeuds contenant d'autres noeuds enfants, que
//! l'utilisateur peut développer ou replier. Il est possible de mettre des
//! images ou des cases à cocher sur chaque noeud, et de faire des déplacements
//! d'un tree-view à un autre ou à un autre endroit du même tree-view.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTreeView: public TWinControl {
private:
protected:

  bool FAutoExpand;
  TBorderStyle FBorderStyle;
  int FChangeDelay;
  TTreeNode *FDropTarget;
  bool FHotTrack;
  TImageList *FImages;
  TTreeNodes *FItems;
  TMultiSelectStyle FMultiSelectStyle;
  bool FRightClickSelect;
  bool FRowSelect;
  unsigned FSelectionCount;
  TSortType FSortType;
  TImageList *FStateImages;
  bool FToolTips;
  TTreeNode *FTopItem;

  /* Inutile jusqu'à présent
  void FASTCALL DestroyWnd(void);
  void FASTCALL CreateWnd(void);
  */

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual int ProcessNotify(LPNMHDR pnmh);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTreeView(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTreeView(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Tri par ordre alphabétique
  //!
  //! @param[in]        ARecurse Si @b true les noeuds enfants sont également triés.
  //!
  //! @return @c true si Ok.
  //---------------------------------------------------------------------------
  bool FASTCALL AlphaSort(bool ARecurse = false);

  //---------------------------------------------------------------------------
  //! @brief Noeud présent à une position donée
  //!
	//! Cette méthode retourne le noeud présent à la position (X, Y), compte tenu
	//! des scrolls verticaux et horizontaux, des noeuds développés ou pas et des
	//! tailles des items.
	//!
  //! @param[in]        X Coordonnée X en pixels.
  //! @param[in]        Y Coordonnée Y en pixels.
  //!
  //! @return @c TTreeNode Noeud présent à la position (X, Y) ou NULL si aucun.
  //---------------------------------------------------------------------------
	TTreeNode* FASTCALL GetNodeAt(int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété AutoExpand: développement automatique du noeud sélectionné
	//!
	//! Losque cette propriété est à @b true, le noeud est automatiquement
	//! développé lorsqu'il est sélectionné (les noeuds enfants deviennent
	//! visibles).
	//!
	//! @sa HideSelection
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, AutoExpand);


  //---------------------------------------------------------------------------
  //! @brief Propriété BorderStyle: style de bordure
	//!
	//! Cette propriété définit le type de bordure du contrôle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TBorderStyle, BorderStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété ChangeDelay: Délai de l'évènement @b OnChange
	//!
	//! Cette propriété définit le délai entre le moment où l'utilisateur
	//! sélectionne un noeud et le moment où le système envoie un évènement
	//! @b OnChange.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, int, ChangeDelay);


  //---------------------------------------------------------------------------
  //! @brief Propriété CheckBoxes: affichage de check-boxes
	//!
	//! Cette propriété permet d'afficher une check-box (case à cocher) à gauche
	//! de chaque noeud du tree-view. Vous pouvez ensuite définir l'état de
	//! la check-box de chaque noeud avec sa propriété @b Checked.
	//!
	//! @sa TTreeNode::Checked
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, CheckBoxes);


  //---------------------------------------------------------------------------
  //! @brief Propriété DropTarget: Destination du copié-collé
	//!
	//! Cette propriété permet de définit le noeud destination d'un opération de
	//! copié-collé ou de coupé-collé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TTreeNode *, DropTarget);


  //---------------------------------------------------------------------------
  //! @brief Propriété HideSelection: Réduction automatique du noeud désélectionné
	//!
	//! Losque cette propriété est à @b true, le noeud est automatiquement
	//! réduit lorsqu'il est désélectionné (les noeuds enfants ne sont alors plus
	//! visibles).
	//!
	//! @sa AutoExpand
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, HideSelection);


  //---------------------------------------------------------------------------
  //! @brief Propriété HotTrack: Surbrillance au survol de la souris
	//!
	//! Cette propriété permet de mettre en évidence les noeuds lorsque la souris
	//! passe dessus.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, HotTrack);


  //---------------------------------------------------------------------------
  //! @brief Propriété Images: liste d'images
	//!
	//! Cette propriété définit la liste d'images à utiliser pour le dessin des
	//! noeuds avec image. Le tree-view ne devient pas propriétaire de l'objet
	//! TImageList. C'est à l'objet appelant de détruire cet objet après la
	//! destruction du tree-view.
	//!
	//! @sa TTreeNode::ImageIndex
	//!
	//! @note Attention à ne pas affecter un TImageListe ayant la propriété
	//! @b Width ou @b Height à zéro. Le système adaptant la taille des items et
	//! des boutons +/- à la taille des images, des bugs graphiques peuvent se
	//! manifester si cette taille est à zéro.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TImageList *, Images);


  //---------------------------------------------------------------------------
  //! @brief Propriété Indent: dimension de l'indentation
	//!
	//! Cette propriété définit l'indentation entre un noeud et un de ses noeud
	//! enfant en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, int, Indent);


  //---------------------------------------------------------------------------
  //! @brief Propriété Items: liste des noeuds
	//!
	//! Cette propriété permet d'accéder à l'objet @b TTreeNodes décrivant la
	//! liste des noeuds.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TTreeNodes *, Items);


  //---------------------------------------------------------------------------
  //! @brief Propriété MultiSelect: multi-sélection
	//!
	//! Cette propriété indique qu'il est possible de sélectionner plusieurs
	//! noeuds simultanément.
	//!
	//! @sa MultiSelectStyle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, MultiSelect);


  //---------------------------------------------------------------------------
  //! @brief Propriété MultiSelectStyle: style de multi-sélection
	//!
	//! Cette propriété définit la façon de sélectionner plusieurs noeuds (à
	//! condition que le tree-view ait la propriété @b MultiSelect à @b true):
	//! soit en cliquant chaque noeud pour les sélectionner ou les
	//! désélectionner, soit à l'aide des touches SHIFT et CTRL (comme dans
	//! l'explorateur de fichiers).
	//!
	//! @sa MultiSelect
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TMultiSelectStyle, MultiSelectStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété ReadOnly: lecture seule
	//!
	//! Cette propriété indique que l'utilisateur ne pourra pas changer le
	//! libellé des noeuds.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, ReadOnly);


  //---------------------------------------------------------------------------
  //! @brief Propriété RightClickSelect: sélection par clic droit
	//!
	//! Cette propriété permet de sélectionner un noeud avec la touche droite de
	//! la souris. C'est en général le comportement le plus intuitif lorsqu'il y
	//! a un menu contextuel (qui apparait donc lorsqu'on appuie sur la touche
	//! droite de la souris) associé à chaque noeud.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, RightClickSelect);


  //---------------------------------------------------------------------------
  //! @brief Propriété RowSelect: sélection de la ligne entière
	//!
	//! Cette propriété permet de mettre en surbrillance l'ensemble de la ligne
	//! lorsque l'utilisateur sélectionne un noeud. Cette propriété est
	//! incompatible avec la propriété @b ShowLines (elle est inactive lorsque
	//! @b ShowLines est à @b true).
	//!
	//! @sa ShowLines
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, RowSelect);


  //---------------------------------------------------------------------------
  //! @brief Propriété Selected: noeud sélectionné
	//!
	//! Cette propriété permet de lire ou de définir le noeud sélectionné. Le
	//! fait de sélectionner un noeud de cette façon désélectionne
	//! automatiquement les autres. Si le tree-view a la propriété @b MultiSelect
	//! à @b true, il est donc préférable d'utiliser la méthode @b Selected de
	//! chaque noeud.
	//!
	//! @sa TTreeNode::Selected
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TTreeNode *, Selected);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelectionCount: nombre de noeuds sélectionnés
	//!
	//! Cette propriété indique le nombre de noeuds sélectionnés. Elle est donc
	//! utilisée dans le cas où la propriété @b MultiSelect est à @b true.
	//!
	//! @sa MultiSelect
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTreeView, unsigned, SelectionCount);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowButtons: boutons visibles
	//!
	//! Cette propriété permet de rendre visible ou invisible les boutons à
	//! gauche de chaque noeud contenant le signe "+" ou "-" selon que le noeud
	//! est développé ou réduit.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, ShowButtons);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowLines: lignes visibles
	//!
	//! Cette propriété permet de rendre visible ou invisible les lignes reliant
	//! les noeuds entre eux.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, ShowLines);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowRoot: noeud racine visible
	//!
	//! Cette propriété indique s'il faut représenter le noeud racine. Sinon,
	//! seuls ses noeuds enfants sont représentés au niveau le plus haut.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, ShowRoot);


  //---------------------------------------------------------------------------
  //! @brief Propriété SortType: type de tri
	//!
	//! Cette propriété définit le type de tri à utiliser: aucun, sur le libellé,
	//! sur les données associées, ou sur les deux. Si le tri se fait sur les
	//! données (propriété @b Data), vous devez intercepter l'évènement
	//! @b OnCompare et le traiter pour indiquer au système l'ordre de tri en
	//! fonction des données.
	//!
	//! @sa TTreeNode::Data OnCompare
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TSortType, SortType);


  //---------------------------------------------------------------------------
  //! @brief Propriété StateImages: images d'état
	//!
	//! Cette propriété indique la liste d'images à utiliser pour l'affichage de
	//! l'état d'un noeud. Pour des raisons de compatibilité avec le système, la
	//! première image n'est pas utilisée. La seconde (d'indice 1) correspond à
	//! la valeur zéro de la propriété TTreeNode::StateIndex.@n
	//! Le tree-view ne devient pas propriétaire de l'objet TImageList. C'est à
	//! l'objet appelant de détruire cet objet après la destruction du tree-view.
	//!
	//! @sa TTreeNode::StateIndex
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TImageList *, StateImages);


  //---------------------------------------------------------------------------
  //! @brief Propriété ToolTips: affichage des infos-bulles
	//!
	//! Cette propriété indique s'il faut afficher les infos-bulles.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, bool, ToolTips);


  //---------------------------------------------------------------------------
  //! @brief Propriété TopItem: premier noeud visible
	//!
	//! Cette propriété détermine le premier noeud visible dans le tree-view.
	//! Modifiez cette propriété pour faire défiler verticalement les éléments.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTreeView, TTreeNode *, TopItem);


  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnAddition
	//!
	//! Cet évènement est déclenché lorsqu'un nouveau noeud est ajouté au tree-
	//! view.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnAddition, OnAddition, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDeletion
	//!
	//! Cet évènement est déclenché lorsqu'un noeud est supprimé du tree-view.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnDeletion, OnDeletion, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnEdited Noeud édité
	//!
	//! Cet évènement est déclenché lorsqu'un noeud du tree-view a été édité.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Noeud modifié
  //! @param[in]        Arg3 Nouveau libellé du noeud
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnEdited, OnEdited, TObject *, TTreeNode *, AnsiString);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnEditing Edition d'un noeud
	//!
	//! Cet évènement est déclenché lorsqu'un noeud du tree-view va être édité.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Noeud qui va être modifié
  //! @param[in]        Arg3 AllowEdit Flag à positionner à false pour interdire la modification.
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnEditing, OnEditing, TObject *, TTreeNode *, bool &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChanging
	//!
	//! Cet évènement est déclenché lorsque la sélection d'un noeud va être
	//! modifiée. Vous pouvez intercepter cet évènement pour interdire la
	//! sélection de certains noeuds. Pour cela, mettez à false la valeur du
	//! flag AllowChange.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Noeud actuellement sélectionné (avant changement)
  //! @param[in]        Arg3 flag AllowChange à éventuellement mettre à faux
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnChanging, OnChanging, TObject *, TTreeNode *, bool &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque un nouveau noeud est sélectionné,
	//! soit par une action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nouveau noeud sélectionné
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnChange2, OnChange, TObject *, TTreeNode *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnExpanding
	//!
	//! Cet évènement est déclenché lorsqu'un noeud va être développé, soit par
	//! une action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Noeud qui va être développé
	//!
	//! @sa TreeNode::Expand
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnExpanding, OnExpanding, TObject *, TTreeNode *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnExpanded
	//!
	//! Cet évènement est déclenché lorsqu'un noeud vient d'être développé, soit
	//! par une action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Noeud qui vient d'être développé
	//!
	//! @sa TreeNode::Expand
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnExpanded, OnExpanded, TObject *, TTreeNode *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnCollapsing
	//!
	//! Cet évènement est déclenché lorsqu'un noeud va être réduit, soit par une
	//! action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Noeud qui va être réduit
	//!
	//! @sa TreeNode::Collapse
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnCollapsing, OnCollapsing, TObject *, TTreeNode *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnCollapsed
	//!
	//! Cet évènement est déclenché lorsqu'un noeud vient d'être réduit, soit
	//! par une action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Noeud qui vient d'être réduit
	//!
	//! @sa TreeNode::Collapse
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnCollapsed, OnCollapsed, TObject *, TTreeNode *);


  //@}

};


#else  // TTreeViewH

class TTreeView;

#endif  // TTreeViewH


#undef In_TTreeViewH
