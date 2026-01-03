//-----------------------------------------------------------------------------
//! @file TComboBox.h
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

#ifdef In_TComboBoxH
#error "Inclusion circulaire de TComboBox"
#endif // In_TComboBoxH

#define In_TComboBoxH

#ifndef TComboBoxH
#define TComboBoxH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#ifndef TComboBoxItemsH
#include "TComboBoxItems.h"
#endif // TComboBoxItemsH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TComboBoxStyle {
	csDropDown,								    //!< Combo-box permettant une saisie manuelle
	csSimple,								      //!< Combo-box avec liste déroulante fixe
	csDropDownList,								//!< Combo-box permettant uniquement un choix dans la liste déroulante
	csOwnerDrawFixed,							//!< Eléments de hauteur fixe dessinés par programme
	csOwnerDrawVariable						//!< Eléments de hauteur variable dessinés par programme
};


//-----------------------------------------------------------------------------
//! @class TComboBox
//!
//! @brief Définition de la classe TComboBox
//! @image html TComboBox.jpg "TComboBox"
//!
//! Cette classe définit le contrôle graphique permettant à l'utilisateur de
//! choisir une valeur parmi plusieurs, dans une liste déroulante le plus
//! souvent. La liste peut également être non déroulante, le contrôle peut
//! aussi permettre la saisie d'une valeur qui n'est pas dans la liste.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TComboBox: public TWinControl {
private:
protected:
  int FItemIndex;
  int FItemHeight;
  TComboBoxItems *FItems;
  int FSelStart;
  int FSelLength;
  AnsiString FText;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessCommand(WORD Notify);
  virtual bool ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS);
  virtual bool ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS);

public:

  FVCL_BEGIN_EVENTS
  void FASTCALL FItemsChange(TObject *Sender);

	virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TComboBox, TNotifyEvent, OnChange, FItemsChange, TObject*);
	}

	FVCL_END_EVENTS

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TComboBox(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TComboBox(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ajoute un élément dans la liste déroulante.
  //!
  //! Cette méthode ajoute un élément dans la liste d'éléments prédéfinis de
	//! la combo-box. Un objet quelconque peut être associé à cet élément. Pour
	//! le lire, utiliser la propriété Objects de Items.
  //!
  //! @param[in]        S Texte à ajouter
  //! @param[in]        Object Objet associé, ou NULL si aucun
  //---------------------------------------------------------------------------
  virtual void FASTCALL AddItem(const AnsiString S, TObject *Object);

  //---------------------------------------------------------------------------
  //! @brief Vide le contenu de la liste déroulante.
  //!
  //! Cette méthode vide la liste d'éléments prédéfinis de la combo-box.
  //---------------------------------------------------------------------------
	virtual void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Désélectionne un élément sélectionné dans la liste déroulante.
  //!
  //! Cette méthode vérifie si l'élément actuellement sélectionné est dans la
	//! liste d'éléments prédéfinis. Si c'est le cas, elle le désélectionne
	//! (attribue la valeur –1 à ItemIndex) et la propriété Text devient vide.
  //---------------------------------------------------------------------------
	virtual void FASTCALL ClearSelection(void);

  //---------------------------------------------------------------------------
  //! @brief Destruction de l'élément sélectionné dans la liste déroulante.
  //!
  //! Cette méthode détruit l'élément sélectionné (propriété ItemIndex).
  //---------------------------------------------------------------------------
	virtual void FASTCALL DeleteSelected(void);

  //---------------------------------------------------------------------------
  //! @brief Sélection du texte complet de la zone d'édition.
  //!
  //! Cette méthode sélectionne la totalité du texte de la zone d'édition
	//! (si le style n'est pas csDropDownList). Equivaut à mettre les propriétés
	//! SelStart à 0 et SelLength à -1.
  //---------------------------------------------------------------------------
	virtual void FASTCALL SelectAll(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Height
  //!
  //! Cette méthode permet d'écrire la propriété Height
  //!
  //! @param[in]        NewHeight Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Height(int NewHeight);

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
  //! @brief Propriété ItemIndex
	//!
	//! Cette propriété définit l'élément sélectionné dans la liste déroulante,
	//! ou -1 si aucun élément n'est sélectionné.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBox, int, ItemIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété ItemHeight
	//!
	//! Cette propriété définit la hauteur d'un élément de la liste déroulante
	//! (ce qui modifie également la hauteur du contrôle lui-même).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBox, int, ItemHeight);


  //---------------------------------------------------------------------------
  //! @brief Propriété Items
	//!
	//! Cette propriété permet de lire les libellés des éléments de la liste
	//! déroulante.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBox, TStrings *, Items);


  //---------------------------------------------------------------------------
  //! @brief Propriété Style
	//!
	//! Cette propriété détermine le style de la combo-box. Si le style est
	//! défini à @b csOwnerDrawFixed ou à @b csOwnerDrawVariable, il vous faudra
	//! traiter les évènements @b OnDrawItem, et éventuellement @b OnMeasureItem
	//! pour obtenir une représentation correcte.
	//!
	//! @sa OnMeasureItem, OnDrawItem
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBox, TComboBoxStyle, Style);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelStart
	//!
	//! Cette propriété définit le début du texte sélectionné dans la boîte
	//! d'édition. Elle n'a de signification que si le texte peut être édité
	//! (styles @b csDropDown ou @b csSimple).
	//!
	//! @sa Style
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBox, int, SelStart);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelLength
	//!
	//! Cette propriété définit la longueur du texte sélectionné dans la boîte
	//! d'édition. Elle n'a de signification que si le texte peut être édité
	//! (styles @b csDropDown ou @b csSimple).
	//!
	//! @sa Style
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBox, int, SelLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété Text
	//!
	//! Cette propriété permet de lire et de définir le texte de la boîte
	//! d'édition. Si ce texte correspond à un élément de la liste déroulante,
	//! la propriété @b ItemIndex se modifie automatiquement pour refléter la
	//! nouvelle position.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBox, AnsiString, Text);

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
  //! @brief Evènement OnSelect
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur sélectionne une
	//! valeur dans la liste déroulante.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnSelect, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDropDown
	//!
	//! Cet évènement est déclenché lorsque la liste déroulante est affichée (par
	//! un clic sur le bouton de droite, par exemple).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnDropDown, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnCloseUp
	//!
	//! Cet évènement est déclenché lorsque la liste déroulante se referme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnCloseUp, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnMeasureItem
	//!
	//! Cet évènement est déclenché lorsque le système a besoin de connaitre la
	//! hauteur d'un élément de hauteur variable. Si la propriété @b Style est
	//! égale à csOwnerDrawVariable, le programmeur doit traiter cet évènement et
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
	//! élément d'une combo-box de style @b csOwnerDrawFixed ou
	//! @b csOwnerDrawVariable. Le programmeur d'un contrôle de ce style doit
	//! traiter cet évènement.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Index de l'élément à dessiner
  //! @param[in]        Arg3 Rectangle d'enveloppe de l'élément
  //! @param[in]        Arg4 Etat de l'élément à dessiner
	//!
	//! @sa Style
  //---------------------------------------------------------------------------

  DECLARE_EVENT_4(TOnDrawItem, OnDrawItem, TObject *, int, const TRect &, TOwnerDrawStates);

  //@}

};


#else  // TComboBoxH

class TComboBox;

#endif  // TComboBoxH


#undef In_TComboBoxH
