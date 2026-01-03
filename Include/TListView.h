//-----------------------------------------------------------------------------
//! @file TListView.h
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

#ifdef In_TListViewH
#error "Inclusion circulaire de TListView"
#endif // In_TListViewH

#define In_TListViewH

#ifndef TListViewH
#define TListViewH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#include "TListColumns.h"
#include "TListItems.h"


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TViewStyle {
	vsIcon,												//!< Icônes normales pouvant être déplacées
	vsSmallIcon,									//!< Icônes réduites pouvant être déplacées
	vsList,												//!< Icônes réduites organisées en liste
	vsReport											//!< Icônes réduites organisées en colonnes (avec éventuellement une entête de colonne)
};

enum TListArrangement {
	arAlignBottom,								//!< Alignement en bas
	arAlignLeft,									//!< Alignement à gauche
	arAlignRight,									//!< Alignement à droite
	arAlignTop,										//!< Alignement en haut
	arDefault,										//!< Alignement par défaut
	arSnapToGrid									//!< Calage sur la grille
};

enum TListHotTrackStyle {
	htHandPoint,								  //!< Curseur en forme de main
	htUnderlineCold,							//!< Tous les éléments sont soulignés sauf celui survolé
	htUnderlineHot								//!< L'élément survolé est souligné
};

typedef Set<TListHotTrackStyle, htHandPoint, htUnderlineHot> TListHotTrackStyles;  //!< Combinaison de plusieurs TListHotTrackStyle

enum TItemRequest {
	irText,								        //!< Texte
	irImage,								      //!< Image
	irParam,								      //!< Paramètre
	irState,								      //!< Etat
	irIndent								      //!< Indentation
};
typedef Set<TItemRequest, irText, irIndent> TItemRequests;  //!< Combinaison de plusieurs TItemRequest

enum TCustomDrawState {
	cdsSelected,								  //!< L'élément est sélectionné
	cdsGrayed,								    //!< L'élément est grisé
	cdsDisabled,								  //!< L'élément est désactivé
	cdsChecked,								    //!< L'élément est coché
	cdsFocused,								    //!< L'élément détient le focus
	cdsDefault,								    //!< L'élément est celui par défaut
	cdsHot,								        //!< L'élément doit être dessiné en surbrillance
	cdsMarked,								    //!< L'élément doit être dessiné avec une ombre
	cdsIndeterminate							//!< L'élément est dans l'état indéterminé (ni coché ni non coché)
};
typedef Set<TCustomDrawState, cdsSelected, cdsIndeterminate> TCustomDrawStates;  //!< Combinaison de plusieurs TCustomDrawState

enum TCustomDrawStage {
	cdPrePaint,							      //!< Avant le dessin
	cdPostPaint,							    //!< Après le dessin
	cdPreErase,							      //!< Avant l'effacement
	cdPostErase							      //!< Après l'effacement
};

enum THitTest {
	htAbove,							        //!< Au dessus de la zone client du contrôle
	htBelow,							        //!< En dessous de la zone client du contrôle
	htNowhere,							      //!< Dans le contrôle mais pas sur un élément
	htOnItem,							        //!< Sur un élément, un libellé ou une icône
	htOnButton,							      //!< Sur un bouton
	htOnIcon,							        //!< Sur une icône
	htOnIndent,							      //!< Sur une indentation
	htOnLabel,							      //!< Sur un libellé
	htOnRight,							      //!< À droite d'un élément
	htOnStateIcon,							  //!< Sur une icône d'état
	htToLeft,							        //!< À gauche de la zone client du contrôle
	htToRight							        //!< À droite de la zone client du contrôle
};
typedef Set<THitTest, htAbove, htToRight> THitTests;  //!< Combinaison de plusieurs THitTest

enum TSearchDirection {
	sdLeft,												//!< Recherche vers la gauche
	sdRight,											//!< Recherche vers la droite
	sdAbove,											//!< Recherche vers le haut
	sdBelow,											//!< Recherche vers le bas
	sdAll												  //!< Recherche dans toutes les directions
};

enum TItemState {
	isNone,							          //!< L'élément est dans l'état par défaut
	isCut,							          //!< L'élément est la source d'un copié-collé
	isDropHilited,							  //!< L'élément est la destination d'un copié-collé
	isFocused,							      //!< L'élément a le focus
	isSelected,							      //!< L'élément est sélectionné
	isActivating							    //!< L'élément est activé
};
typedef Set<TItemState, isNone, isActivating> TItemStates;  //!< Combinaison de plusieurs TItemState

enum TItemChange {
	ctText,												//!< Type de changement = texte
	ctImage,											//!< Type de changement = image
	ctState												//!< Type de changement = état
};

enum TCustomDrawTarget {
	dtControl,							      //!< Tout le contrôle doit être redessiné
	dtItem,							          //!< Un élément doit être redessiné
	dtSubItem							        //!< Un sous-élément doit être redessiné
};

enum TItemFind {
	ifData,							          //!< Recherche correspondance des données
	ifPartialString,							//!< Recherche sur une partie du libellé
	ifExactString,							  //!< Recherche sur la totalité du libellé
	ifNearest							        //!< Recherche du libellé le plus proche
};


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnEdited2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnEdited2)(TObject *Sender, TListItem *Item, AnsiString Text);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnEditing2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnEditing2)(TObject *Sender, TListItem *Item, bool &AllowEdit);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnChanging2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnChanging2)(TObject *Sender, TListItem *Item, TItemChange Type, bool &AllowChanging);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnSelectItem.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnSelectItem)(TObject *Sender, TListItem *Item, bool Selected);


//-----------------------------------------------------------------------------
//! @class TListView
//!
//! @brief Définition de la classe TListView
//! @image html TListView.jpg "TListView"
//!
//! Cette classe est un contrôle permettant l'affichage d'une liste d'objets,
//! soit sous forme de liste verticale, soit sous forme d'un ensemble d'icônes
//! comme dans l'explorateur de fichiers.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TListView: public TWinControl {
private:
protected:
  int FAllocBy;
  TBorderStyle FBorderStyle;
  TRect FBoundingRect;
  TListColumns *FColumns;
  bool FColumnClick;
  TListItem *FDropTarget;
  bool FFlatScrollBars;
  bool FFullDrag;
  bool FHotTrack;
  TListHotTrackStyles FHotTrackStyles;
  int FHoverTime;
  TListItems *FItems;
  TListItem *FItemFocused;
  int FItemIndex;
  TImageList *FLargeImages;
  bool FOwnerData;
  bool FOwnerDraw;
  int FSelCount;
  TListItem *FSelected;
  bool FShowColumnHeaders;
  bool FShowWorkAreas;
  TImageList *FSmallImages;
  TSortType FSortType;
  TImageList *FStateImages;
  TPoint FViewOrigin;
  TViewStyle FViewStyle;

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

  TListView(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TListView(void);

  //@}


  //! @name Méthodes
  //@{

  void FASTCALL ActionChange(TObject* Sender, bool CheckDefaults);
  virtual void FASTCALL AddItem(AnsiString Item, TObject* AObject);
  bool FASTCALL AlphaSort(void);
  void FASTCALL Arrange(TListArrangement Code);
  bool FASTCALL CanChange(TListItem* Item, int Change);
  bool FASTCALL CanEdit(TListItem* Item);
  void FASTCALL Change(TListItem* Item, int Change);
  void FASTCALL ChangeScale(int M, int D);

  //---------------------------------------------------------------------------
  //! @brief Effacement du list-view
  //!
  //! Cette méthode permet d'effacer les items du list-view
  //---------------------------------------------------------------------------

  virtual void FASTCALL Clear(void);


  virtual void FASTCALL ClearSelection(void);
  void FASTCALL ColClick(TListColumn* Column);
  void FASTCALL ColRightClick(TListColumn* Column, const TPoint &Point);
  bool FASTCALL ColumnsShowing(void);
  virtual TListItem* FASTCALL CreateListItem(void);
  virtual TListItems* FASTCALL CreateListItems(void);
  virtual void FASTCALL CreateParams(TCreateParams &Params);
  virtual void FASTCALL CreateWnd(void);
  virtual bool FASTCALL CustomDraw(const TRect &ARect, TCustomDrawStage Stage);
  virtual bool FASTCALL CustomDrawItem(TListItem* Item, TCustomDrawStates State, TCustomDrawStage Stage);
  virtual bool FASTCALL CustomDrawSubItem(TListItem* Item, int SubItem, TCustomDrawStates State, TCustomDrawStage Stage);
  void FASTCALL Delete(TListItem* Item);
  virtual void FASTCALL DeleteSelected(void);
  virtual void FASTCALL DestroyWnd(void);
  void FASTCALL DoEndDrag(TObject* Target, int X, int Y);
  virtual void FASTCALL DoInfoTip(TListItem* Item, AnsiString &InfoTip);
  void FASTCALL DoStartDrag(TDragObject* &DragObject);
  virtual void FASTCALL DrawItem(TListItem* Item, const TRect &Rect, TOwnerDrawStates States);
  void FASTCALL Edit(const LVITEM &Item);
  TListItem* FASTCALL FindCaption(int StartIndex, AnsiString Value, bool Partial, bool Inclusive, bool Wrap);
  TListItem* FASTCALL FindData(int StartIndex, void * Value, bool Inclusive, bool Wrap);
  virtual int FASTCALL GetCount(void) const;
  // $$$ virtual TDragImageList* FASTCALL GetDragImages(void);
  THitTests FASTCALL GetHitTestInfoAt(int X, int Y);
  TListItem* FASTCALL GetItemAt(int X, int Y);
  int FASTCALL GetItemIndex(TListItem* Value);
  virtual int FASTCALL GetItemIndex(void);
  TListItem* FASTCALL GetNearestItem(const TPoint &Point, TSearchDirection Direction);
  TListItem* FASTCALL GetNextItem(TListItem* StartItem, TSearchDirection Direction, TItemStates States);
  AnsiString FASTCALL GetSearchString();
  virtual int FASTCALL GetSelCount(void);
  void FASTCALL InsertItem(TListItem* Item);
  virtual bool FASTCALL IsCustomDrawn(TCustomDrawTarget Target, TCustomDrawStage Stage);
  bool FASTCALL IsEditing(void);
  void FASTCALL MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
  virtual bool FASTCALL OwnerDataFetch(TListItem* Item, TItemRequest Request);
  virtual int FASTCALL OwnerDataFind(TItemFind Find, const AnsiString FindString, const TPoint &FindPosition, void * FindData, int StartIndex, TSearchDirection Direction, bool Wrap);
  virtual bool FASTCALL OwnerDataHint(int StartIndex, int EndIndex);
  virtual bool FASTCALL OwnerDataStateChange(int StartIndex, int EndIndex, TItemStates OldState, TItemStates NewState);
  void FASTCALL Scroll(int DX, int DY);
  virtual void FASTCALL SelectAll(void);
  virtual void FASTCALL SetItemIndex(int Value);
  int FASTCALL StringWidth(AnsiString S);
  void FASTCALL UpdateColumn(int AnIndex);
  void FASTCALL UpdateColumns(void);
  void FASTCALL UpdateItems(int FirstIndex, int LastIndex);


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
  //! @brief Propriété AllocBy
	//!
	//! Cette propriété définit le nombre d'éléments alloués en mémoire lorsqu'il
	//! devient nécessaire d'allouer un nouveau bloc. Une petite valeur permet
	//! d'économiser de la mémoire
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, int, AllocBy);


  //---------------------------------------------------------------------------
  //! @brief Propriété BorderStyle
	//!
	//! Cette propriété définit le style de bordure de la ListView.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TBorderStyle, BorderStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété BoundingRect
	//!
	//! Cette propriété définit le rectangle englobant tous les éléments. Ceci
	//! permet de dimensionner un contrôle en fonction des éléments qu'il
	//! contient, par exemple.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TRect, BoundingRect);


  //---------------------------------------------------------------------------
  //! @brief Propriété CheckBoxes
	//!
	//! Cette propriété permet d'afficher une check-box sur chaque ligne de la
	//! list-view. Utilisez ensuite la propriété @b Checked de chaque élément
	//! pour effectuer un traitement particulier sur les éléments sélectionnés.
	//!
	//! @sa TListItem::Checked
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, CheckBoxes);


  //---------------------------------------------------------------------------
  //! @brief Propriété Columns
	//!
	//! Cette propriété permet d'accéder à la liste des colonnes de la ListView.
	//! Utilisez cette propriété pour ajouter, supprimer ou modifier une colonne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TListColumns *, Columns);


  //---------------------------------------------------------------------------
  //! @brief Propriété ColumnClick
	//!
	//! Cette propriété définit le comportement des titres de colonnes lorsque
	//! l'utilisateur clique dessus. Si cette propriété est à @b true, le système
	//! envoie un évènement @b OnClick comme un bouton.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, ColumnClick);


  //---------------------------------------------------------------------------
  //! @brief Propriété DropTarget
	//!
	//! Cette propriété définit l'élément de la list-view sur lequel
	//! l'utilisateur a fait un copié-collé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TListItem *, DropTarget);


  //---------------------------------------------------------------------------
  //! @brief Propriété FlatScrollBars
	//!
	//! Cette propriété définit l'aspect des barres de défilement de la ListView.
	//! Si cette propriété est à @b false (valeur par défaut), les barres ont un
	//! aspect plus en relief.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, FlatScrollBars);


  //---------------------------------------------------------------------------
  //! @brief Propriété FullDrag
	//!
	//! Si cette propriété est à @b true, les titres de colonnes sont redessinés
	//! lorsque l'utilisateur fait un drag and drop.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, FullDrag);


  //---------------------------------------------------------------------------
  //! @brief Propriété GridLines
	//!
	//! Cette propriété définit la visibilité de la grille de séparation des
	//! éléménts.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, GridLines);


  //---------------------------------------------------------------------------
  //! @brief Propriété HideSelection
	//!
	//! Cette propriété détermine la visibilité de d'état sélectionné lorsque le
	//! contrôle n'a plus le focus. Si cette propriété est à true, l'élément
	//! sélectionné reste en surbrillance avec une couleur plus claire (en
	//! général, selon la configuration) que lorsque le contrôle a le focus.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, HideSelection);


  //---------------------------------------------------------------------------
  //! @brief Propriété HotTrack
	//!
	//! Cette propriété définit le comportement des éléments de la ListView
	//! lorsque la souris les survole. Si cette propriété est à @b true,
	//! l'élément survolé est mis en surbrillance, ou bien la forme du curseur
	//! change, selon la valeur de la propriété @b HotTrackStyles.
	//!
	//! @sa HotTrackStyles
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, HotTrack);


  //---------------------------------------------------------------------------
  //! @brief Propriété HotTrackStyles
	//!
	//! Cette propriété définit la façon de mettre en surbrillance un élément
	//! de la ListView. Lorsque la souris survole un élément, l'élément qui va
	//! être sélectionné peut être mis en évidence de différentes façons: soit
	//! par un changement de couleur, soit par un changement de la forme du
	//! curseur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TListHotTrackStyles, HotTrackStyles);


  //---------------------------------------------------------------------------
  //! @brief Propriété HoverTime
	//!
	//! Cette propriété définit le délai avant qu'un élément soit sélectionné.
	//! Une valeur de -1 définit la valeur par défaut dépendant du système.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, int, HoverTime);


  //---------------------------------------------------------------------------
  //! @brief Propriété ItemFocused
	//!
	//! Cette propriété définit l'élément qui possède le focus (que l'utilisateur
	//! est en train de modifier). Si aucun élément a le focus, cette propriété
	//! renvoie la valeur -1.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TListItem *, ItemFocused);


  //---------------------------------------------------------------------------
  //! @brief Propriété ItemIndex
	//!
	//! Cette propriété définit l'élément sélectionné. Le premier élément de la
	//! liste a l'indice zéro. Si aucun n'est sélectionné, cette propriété
	//! renvoie la valeur -1. Si plusieurs sont sélectionnés (lorsque la
	//! propriété @b MultiSelect est à @b true), cette propriété ne renvoie que
	//! le premier sélectionné.
	//!
	//! @sa SelCount, Selected, GetNextItem
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, int, ItemIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété Items
	//!
	//! Cette propriété permet d'accéder aux éléments de la ListView. Utilisez
	//! cette propriété pour ensuite modifier les propriétés des éléments eux-
	//! même.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TListItems *, Items);


  //---------------------------------------------------------------------------
  //! @brief Propriété LargeImages
	//!
	//! Cette propriété définit l'icône associée à un élément de la liste. Cette
	//! icône n'est visible que lorsque la propriété @b ViewStyle du contrôle a
	//! la valeur @b vsIcon.
	//!
	//! @sa ViewStyle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TImageList *, LargeImages);


  //---------------------------------------------------------------------------
  //! @brief Propriété MultiSelect
	//!
	//! Cette propriété permet à l'utilisateur de sélectionner simultanément
	//! plusieurs éléments de la liste. Lorsque cette propriété est à @b true,
	//! utilisez la propriété @b Selected et la méthode @b GetNextItem pour
	//! connaitre les éléments sélectionnés.
	//!
	//! @sa SelCount, Selected, GetNextItem
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, MultiSelect);


  //---------------------------------------------------------------------------
  //! @brief Propriété OwnerData
	//!
	//! Cette propriété permet de définir une liste virtuelle, c'est à dire que
	//! chaque élément de la liste n'est pas un objet VCL, mais un objet virtuel
	//! dont l'état dépendra de la réponse aux évènements @b OnData,
	//! @b OnDataFind, @b OnDataHint et @b OnDataStateChange.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, OwnerData);


  //---------------------------------------------------------------------------
  //! @brief Propriété OwnerDraw
	//!
	//! Cette propriété permet de dessiner par programme chaque élément de la
	//! liste. Si cette propriété est à true, le système envoie un évènement
	//! @b OnDrawItem lorsqu'un élément doit être redessiné.
	//!
	//! @sa OnDrawItem
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, OwnerDraw);


  //---------------------------------------------------------------------------
  //! @brief Propriété ReadOnly
	//!
	//! Cette propriété rend non modifiable les éléments de la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, ReadOnly);


  //---------------------------------------------------------------------------
  //! @brief Propriété RowSelect
	//!
	//! Cette propriété permet à l'utilisateur de sélectionner une ligne entière.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, RowSelect);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelCount
	//!
	//! Cette propriété définit le nombre d'éléments sélectionnés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TListView, int, SelCount);


  //---------------------------------------------------------------------------
  //! @brief Propriété Selected
	//!
	//! Cette propriété définit l'élément sélectionné, ou bien le premier élément
	//! si la propriété @b MultiSelect est à @b true. Dans ce cas, utilisez
	//! ensuite la méthode @b GetNextItem pour connaitre les éléments suivants
	//! sélectionnés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TListItem *, Selected);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowColumnHeaders
	//!
	//! Cette propriété permet de rendre visible les titres de colonnes. Cette
	//! propriété n'a de signification que si la propriété @b ViewStyle a la
	//! valeur @b vsReport.
	//!
	//! @sa ViewStyle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, ShowColumnHeaders);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowWorkAreas
	//!
	//! Cette propriété permet de personnaliser chaque élément de la ListView
	//! (couleur, libellé). Elle n'est active que si la propriété @b ViewStyle a
	//! la valeur @b vsIcon ou @b vsSmallIcon.
	//!
	//! @sa ViewStyle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, bool, ShowWorkAreas);


  //---------------------------------------------------------------------------
  //! @brief Propriété SmallImages
	//!
	//! Cette propriété définit la liste d'images à utiliser pour afficher une
	//! icône à coté de l'élément. Cette propriété n'est active que si la
	//! propriété @b ViewStyle a la valeur @b vsSmallIcon, @b vsList ou
	//! @b vsReport. L'image à afficher dans cette liste peut être définie avec
	//! la propriété ImageIndex.
	//!
	//! @sa ViewStyle, TListItem::ImageIndex
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TImageList *, SmallImages);


  //---------------------------------------------------------------------------
  //! @brief Propriété SortType
	//!
	//! Cette propriété permet de trier la ListView, et détermine la façon de
	//! la trier. Si cette propriété a la valeur @b stData ou @b stBoth, l'objet
	//! doit traiter l'évènement @b OnCompare pour déterminer l'ordre de tri en
	//! fonction de la donnée associée.
	//!
	//! @sa OnCompare
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TSortType, SortType);


  //---------------------------------------------------------------------------
  //! @brief Propriété StateImages
	//!
	//! Cette propriété définit la liste d'images à utiliser pour afficher une
	//! icône à coté de l'élément en fonction de l'état. L'image à afficher dans
	//! cette liste peut être définie avec la propriété StateIndex.
	//!
	//! @sa TListItem::StateIndex
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TImageList *, StateImages);


  //---------------------------------------------------------------------------
  //! @brief Propriété TopIndex
	//!
	//! Cette propriété permet de lire ou de définir l'index du premier élément
	//! affiché de la ListView. Cette propriété n'est active que si la propriété
	//! @b ViewStyle a la valeur @b vsSmallIcon, @b vsList ou @b vsReport. Sinon
	//! utilisez la propriété @b ViewOrigin.
	//!
	//! @sa ViewStyle, ViewOrigin
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, int, TopIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété TopItem
	//!
	//! Cette propriété permet de lire ou de définir le premier élément affiché
	//! de la ListView. Cette propriété n'est active que si la propriété
	//! @b ViewStyle a la valeur @b vsSmallIcon, @b vsList ou @b vsReport. Sinon
	//! utilisez la propriété @b ViewOrigin.
	//!
	//! @sa ViewStyle, ViewOrigin
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TListItem *, TopItem);


  //---------------------------------------------------------------------------
  //! @brief Propriété ViewOrigin
	//!
	//! Cette propriété permet de définir les coordonnées de l'origine de la
	//! @b ListView par rapport au point supérieur gauche de la partie affichée.
	//! Cette propriété n'est active que si la propriété @b ViewStyle a la valeur
	//! @b vsIcon ou @b vsSmallIcon. Sinon utilisez la propriété @b TopIndex ou
	//! @b TopItem.
	//!
	//! @sa ViewStyle, TopIndex, TopItem
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TPoint, ViewOrigin);


  //---------------------------------------------------------------------------
  //! @brief Propriété ViewStyle
	//!
	//! Cette propriété permet de définir le mode d'affichage de la @b ListView.
	//! Les éléments peuvent être affichés soit sous forme de liste, soit sous
	//! forme d'icônes petites ou grandes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListView, TViewStyle, ViewStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété VisibleRowCount
	//!
	//! Cette propriété permet de lire le nombre d'élément affichés dans la liste
	//! (en fonction de la taille du contrôle et de la hauteur d'un élément).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TListView, int, VisibleRowCount);


  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque l'objet est modifié, soit par une
	//! action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChanging
	//!
	//! Cet évènement lorsqu'un élément de la liste va être modifié. Vous pouvez
	//! intercepter cet évènement et positionner le flag Allow, pour interdire
	//! un changement.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Elément qui va être modifié.
  //! @param[in]        Arg3 Type de changement
  //! @param[in]        Arg4 Flag AllowChange à éventuellement mettre à faux
  //---------------------------------------------------------------------------

  DECLARE_EVENT_4(TOnChanging2, OnChanging, TObject *, TListItem *, TItemChange, bool &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnEdited
	//!
	//! Cet évènement est déclenché lorsque le texte d'un élément de la liste a
	//! été édité.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Elément qui a été modifié.
  //! @param[in]        Arg3 Nouveau texte de l'élément.
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnEdited2, OnEdited, TObject *, TListItem *, AnsiString);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnEditing
	//!
	//! Cet évènement est déclenché lorsqu'un élément va être édité. Vous pouvez
	//! intercepter cet évènement pour interdire la modification de certains
	//! éléments. Si vous souhaitez interdire toutes les modifications,
	//! positionnez plutôt la propriété @b ReadOnly à true.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Elément qui va être modifié.
  //! @param[in]        Arg3 AllowEdit Flag à positionner à false pour interdire la modification.
	//!
	//! @sa ReadOnly
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnEditing2, OnEditing, TObject *, TListItem *, bool &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnSelectItem
	//!
	//! Cet évènement est déclenché lorsqu'un élément est sélectionné ou
	//! déselectionné, soit suite à un clic utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Elément sélectionné
  //! @param[in]        Arg3 true si sélectionné
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnSelectItem, OnSelectItem, TObject *, TListItem *, bool);

  //@}


};


#else  // TListViewH

class TListView;

#endif  // TListViewH


#undef In_TListViewH
