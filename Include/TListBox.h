//-----------------------------------------------------------------------------
//! @file TListBox.h
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

#ifdef In_TListBoxH
#error "Inclusion circulaire de TListBox"
#endif // In_TListBoxH

#define In_TListBoxH

#ifndef TListBoxH
#define TListBoxH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#ifndef TListBoxItemsH
#include "TListBoxItems.h"
#endif // TListBoxItemsH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TListBoxStyle {
	lbStandard,										//!< List-box standard
	lbOwnerDrawFixed,							//!< List-box à éléments dessinés par programme de taille fixe
	lbOwnerDrawVariable,					//!< List-box à éléments dessinés par programme de taille variable
	lbVirtual,										//!< List-box à éléments virtuels
	lbVirtualOwnerDraw						//!< List-box à éléments virtuels dessinés par programme
};


//-----------------------------------------------------------------------------
//! @class TListBox
//!
//! @brief Définition de la classe TListBox
//! @image html TListBox.jpg "TListBox"
//!
//! Cette classe définit le contrôle permettant d'afficher une liste de chaînes
//! parmi laquelle l'utilisateur peut en choisir une.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TListBox: public TWinControl {
private:
protected:
  TBorderStyle FBorderStyle;
  int FItemIndex;
  int FItemHeight;
  bool FExtendedSelect;
  TListBoxItems *FItems;
  int FColumns;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessCommand(WORD Notify);
  virtual bool ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS);
  virtual bool ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS);
  virtual bool ProcessLButtonUp(int X, int Y);
  virtual bool ProcessHScroll(int Command, int Info);
  virtual bool ProcessVScroll(int Command, int Info);

public:

  FVCL_BEGIN_EVENTS
  void FASTCALL FItemsChange(TObject *Sender);

	virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TListBox, TNotifyEvent, OnChange, FItemsChange, TObject*);
	}

	FVCL_END_EVENTS

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TListBox(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TListBox(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Destruction du contrôle Windows associé au composant
  //!
  //! Cette méthode détruit la fenêtre Windows associée au composant, ceci
	//! pouvant être nécessaire au changement de certaines propriétés. Appeler
	//! ensuite CreateWnd pour recréer une fenêtre avec les mêmes propriétés.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
  //---------------------------------------------------------------------------
  void FASTCALL DestroyWnd(void);

  //---------------------------------------------------------------------------
  //! @brief Création du contrôle Windows associé au composant
  //!
  //! Cette méthode crée la fenêtre Windows associée au composant, ceci pouvant
	//! être nécessaire suite au changement de certaines propriétés. Appeler
	//! DestroyWnd, puis CreateWnd pour recréer la fenêtre. Les propriétés sont
	//! conservées entre la destruction et la création.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
  //---------------------------------------------------------------------------
	void FASTCALL CreateWnd(void);

  //---------------------------------------------------------------------------
  //! @brief Effacement de la list-box
  //!
  //! Cette méthode permet d'effacer les items de la list-box
  //---------------------------------------------------------------------------

  virtual void Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Effacement de la sélection
  //!
  //! Cette méthode permet de désélectionner tous les éléments de la list-box
  //---------------------------------------------------------------------------

  void FASTCALL ClearSelection(void);

  //---------------------------------------------------------------------------
  //! @brief Sélectionner tout
  //!
  //! Cette méthode permet de sélectionner tous les éléments de la list-box
  //---------------------------------------------------------------------------

  void FASTCALL SelectAll(void);

  //---------------------------------------------------------------------------
  //! @brief Item à une position donnée
  //!
  //! Cette méthode permet de déterminer quel item est à une position donnée.
  //---------------------------------------------------------------------------
  int ItemAtPos(TPoint &Pos, bool Existing);


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
  //! @brief Propriété Style
	//!
	//! Cette propriété définit le style de la list-box, en particulier certains
	//! styles permettent de dessiner par programme chaque ligne de la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, TListBoxStyle, Style);

	//---------------------------------------------------------------------------
  //! @brief Propriété BorderStyle
	//!
	//! Cette propriété permet de définir le style de bordure de la list-box.
	//! Seuls les styles @b bsNone et @b bsSingle peuvent être utilisés pour ce
	//! composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, TBorderStyle, BorderStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété MultiSelect
	//!
	//! Cette propriété offre la possibilité de sélectionner simultanément
	//! plusieurs lignes de la list-box, soit en cliquant chaque ligne, soit en
	//! utilisant les touches SHIFT et CTRL. La façon de le faire dépend de la
	//! propriété @b ExtendedSelect.
	//!
	//! @sa ExtendedSelect
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, bool, MultiSelect);

  //---------------------------------------------------------------------------
  //! @brief Propriété ExtendedSelect
	//!
	//! Cette propriété définit la manière de sélectionner plusieurs lignes dans
	//! une list-box @b MultiSelect. Si cette propriété est à @b false, chaque
	//! clic sur une ligne inverse la sélection (sélectionne la ligne si elle
	//! n'était pas sélectionnée, et vice-versa). Si cette propriété est à
	//! @b true, la multi-sélection se fait comme dans l'explorateur de fichiers,
	//! en utilisant les touches SHIFT (touche de majuscules) et CTRL (touche
	//! contrôle). Cette propriété n'a aucun effet si la list-box n'a pas la
	//! propriété @b MultiSelect activée.
	//!
	//! @sa MultiSelect
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, bool, ExtendedSelect);

  //---------------------------------------------------------------------------
  //! @brief Propriété Selected
	//!
	//! Cette propriété permet de lire ou de définir l'état sélectionné d'une
	//! ligne de la list-box.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TListBox, bool, Selected);

  //---------------------------------------------------------------------------
  //! @brief Propriété SelCount
	//!
	//! Cette propriété de lire le nombre de lignes sélectionnées dans la
	//! list-box.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TListBox, int, SelCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété Sorted
	//!
	//! Cette propriété permet de trier automatiquement la list-box par ordre
	//! alphabétique.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, bool, Sorted);

  //---------------------------------------------------------------------------
  //! @brief Propriété TopIndex
	//!
	//! Cette propriété permet de définir la première ligne affichée dans la
	//! fenêtre, sauf si cela n'est pas possible (c'est à dire si le nombre de
	//! lignes restant est inférieur au nombre de lignes que contient la
	//! fenêtre).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, int, TopIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété ItemIndex
	//!
	//! Cette propriété permet de lire ou de définir la ligne sélectionnée. Si la
	//! list-box est de type multi-sélection (si la propriété @b MultiSelect est
	//! à @b true), ne pas utiliser cette propriété mais la propriété
	//! @b Selected.
	//!
	//! @sa MultiSelect, Selected
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, int, ItemIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété ItemHeight
	//!
	//! Cette propriété permet de paramétrer la hauteur d'une ligne, dans le cas
	//! où la list-box contient des éléments de taille fixe dessinés par
	//! l'utilisateur (c'est à dire si la propriété @b Style est égale à
	//! @b lbOwnerDrawFixed). Dans le cas contraire, cette propriété est sans
	//! effet.
	//!
	//! @sa Style
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, int, ItemHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété Items
	//!
	//! Cette propriété permet d'accéder à l'objet de type @b TStrings contenant
	//! les textes de la list-box. Il est possible de l'utiliser pour insérer,
	//! supprimer et lire des lignes.
	//!
	//! @sa TSrings
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, TStrings *, Items);

  //---------------------------------------------------------------------------
  //! @brief Propriété Count
	//!
	//! Cette propriété permet de lire le nombre d'éléments (lignes) de la
	//! list-box.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TListBox, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété Columns
	//!
	//! Cette propriété permet de lire ou de définir le nombre de colonnes de la
	//! list-box.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListBox, int, Columns);

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
  //! @brief Evènement OnMeasureItem
	//!
	//! Cet évènement est déclenché lorsque le système a besoin de connaitre la
	//! hauteur d'un élément de hauteur variable. Si la propriété @b Style est
	//! égale à lbOwnerDrawVariable, le programmeur doit traiter cet évènement et
	//! renseigner la hauteur pour avoir une représentation correcte.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Index de l'élément
  //! @param[out]       Arg3 Hauteur de l'élément (pixels)
	//!
	//! @sa Style
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnMeasureItem, OnMeasureItem, TObject *, int, int *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDrawItem
	//!
	//! Cet évènement est déclenché lorsque le système a besoin de dessiner un
	//! élément d'une list-box de style @b lbOwnerDrawFixed ou
	//! @b lbOwnerDrawVariable. Le programmeur d'un contrôle de ce style doit
	//! traiter cet évènement.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Index de l'élément à dessiner (-1 si aucun n'est sélectionné)
  //! @param[in]        Arg3 Rectangle d'enveloppe de l'élément
  //! @param[in]        Arg4 Etat de l'élément à dessiner
	//!
	//! @sa Style
  //---------------------------------------------------------------------------

  DECLARE_EVENT_4(TOnDrawItem, OnDrawItem, TObject *, int, const TRect &, TOwnerDrawStates);

  //@}

};


#else  // TListBoxH

class TListBox;

#endif  // TListBoxH


#undef In_TListBoxH
