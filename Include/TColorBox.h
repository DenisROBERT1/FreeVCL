//-----------------------------------------------------------------------------
//! @file TColorBox.h
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

#ifdef In_TColorBoxH
#error "Inclusion circulaire de TColorBox"
#endif // In_TColorBoxH

#define In_TColorBoxH

#ifndef TColorBoxH
#define TColorBoxH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TColorBoxStyle {
	cbStandardColors,								//!< La liste comprend les 18 couleurs standard
	cbExtendedColors,								//!< La liste comprend les 4 couleurs étendues
	cbSystemColors,									//!< La liste comprend les couleurs système
	cbIncludeNone,									//!< La liste comprend la couleur "Aucune couleur"
	cbIncludeDefault,								//!< La liste comprend la couleur "Couleur par défaut"
	cbCustomColor,									//!< La liste comprend une couleur personnalisable
	cbPrettyNames										//!< Nom explicites
};

typedef Set<TColorBoxStyle, cbStandardColors, cbPrettyNames> TColorBoxStyles;  //!< Combinaison de plusieurs TColorBoxStyle


//-----------------------------------------------------------------------------
//! @class TColorBox
//!
//! @brief Définition de la classe TColorBox
//! @image html TColorBox.jpg "TColorBox"
//!
//! Cette classe est un contrôle permettant de choisir une couleur parmi une
//! liste de couleurs pré-définies: soit les couleurs système, soit les
//! couleurs définies dans le style graphique, et l'utilisateur peut aussi
//! définir une couleur personnalisée.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TColorBox: public TWinControl {
private:
protected:
  TColor FColors[51];  // countof(clListColors)
	TStringList *FItems;
  int FItemIndex;
  int FItemHeight;
  TColor FNoneColorColor;
  TColorBoxStyles FStyle;

  bool bNoColorDialog;             //!< true pour éviter la boîte de dialogue de sélection de couleur

	bool UpdateColorBox(void);

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessCommand(WORD Notify);
  virtual bool ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS);
  virtual bool ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TColorBox(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TColorBox(void);

  //@}


  //! @name Méthodes
  //@{

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
  //! @brief Propriété Colors
	//!
	//! Cette propriété permet de lire une couleur dans la liste déroulante à
	//! partir de son index.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TColorBox, TColor, Colors);

  //---------------------------------------------------------------------------
  //! @brief Propriété ColorNames
	//!
	//! Cette propriété permet de lire le nom d'une couleur dans la liste
	//! déroulante à partir de son index.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TColorBox, AnsiString, ColorNames);

  //---------------------------------------------------------------------------
  //! @brief Propriété ItemIndex
	//!
	//! Cette propriété définit l'index sélectionné dans la liste déroulante (ou
	//! -1 si aucune couleur n'est sélectionnée).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorBox, int, ItemIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété ItemHeight
	//!
	//! Cette propriété définit la hauteur d'un élément de la liste déroulante.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorBox, int, ItemHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété Items
	//!
	//! Cette propriété définit la liste des libellés de la liste déroulante.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorBox, TStrings *, Items);

  //---------------------------------------------------------------------------
  //! @brief Propriété NoneColorColor
	//!
	//! Lorsque le contrôle a le style @b cbSystemColors ou @b cbIncludeNone,
	//! cette propriété définit la couleur utilisée pour dessiner la couleur
	//! @b NoneColor.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorBox, TColor, NoneColorColor);

  //---------------------------------------------------------------------------
  //! @brief Propriété Selected
	//!
	//! Cette propriété définit la couleur sélectionnée. Si aucune n'est
	//! sélectionnée, cette propriété renvoie la couleur 0xFFFFFFFF.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorBox, TColor, Selected);

  //---------------------------------------------------------------------------
  //! @brief Propriété Style
	//!
	//! Cette propriété définit le style du contrôle. Vous pouver paramétrer les
	//! couleurs proposées (système, utilisateur).
	//!
	//! @sa TColorBoxStyle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorBox, TColorBoxStyles, Style);

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
	//! couleur dans la liste déroulante.
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

  //@}

};


#else  // TColorBoxH

class TColorBox;

#endif  // TColorBoxH


#undef In_TColorBoxH
