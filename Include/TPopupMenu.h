//-----------------------------------------------------------------------------
//! @file TPopupMenu.h
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

#ifdef In_TPopupMenuH
#error "Inclusion circulaire de TPopupMenu"
#endif // In_TPopupMenuH

#define In_TPopupMenuH

#ifndef TPopupMenuH
#define TPopupMenuH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TMenu.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TPoint;


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnPopup.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnPopup)(TObject *Sender);


//-----------------------------------------------------------------------------
//! @class TPopupMenu
//!
//! @brief Définition de la classe TPopupMenu
//!
//! Cette classe représente un menu popup (menu "surgissant"). Utilisez les
//! méthodes et propriétés de la classe ancêtre @b TMenu pour le remplir
//! dynamiquement.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPopupMenu: public TMenu {
private:
  // std::vector <ITEMMENU> ItemsMenu;
protected:
  bool FAutoPopup;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPopupMenu(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPopupMenu(void);

  //@}


  //! @name Méthodes
  //@{

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

  //---------------------------------------------------------------------------
  //! @brief Affichage du menu
	//!
	//! Cette méthode fait apparaitre le menu popup aux coordonnées X et Y plus
	//! un petit décalage (5 pixels en X et 10 en Y).
  //!
  //! @param[in]        X Coordonnées en X par rapport à l'origine du contrôle parent (coordonnées client)
  //! @param[in]        Y Coordonnées en Y par rapport à l'origine du contrôle parent (coordonnées client)
  //!
  //! @return @c bool True si Ok.
	//!
	//! @note Le choix de l'utilisateur est sous forme d'un message système
	//! WM_COMMAND mémorisé dans la pile des messages. Or, dans le cas d'un menu
	//! provisoire, l'appel de cette fonction est certainement suivi d'un delete
	//! de l'objet, ce qui signifie que l'objet ne sera plus existant lorsque la
	//! commande sera traitée, et donc aucun évènement ne sera déclenché. Il est
	//! donc conseillé dans ce cas de faire un appel à
	//! TApplication::ProcessMessages avant de détruire l'objet pour remédier à
	//! cet inconvénient.
  //---------------------------------------------------------------------------

  virtual bool Popup(int X, int Y);


  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété AutoPopup: ouverture automatique du menu.
	//!
	//! Cette propriété permet de faire apparaître automatiquement le menu popup.
	//! Sinon, utilisez la méthode @b Popup pour afficher le menu.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPopupMenu, bool, AutoPopup);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnPopup: Apparition du menu.
	//!
	//! Cet évènement est déclenché juste avant que l'objet TPopup devienne
	//! visible. C'est donc en traitant cet évènement que l'utilisateur peut
	//! griser, cocher ou décocher un item du menu en fonction du contexte.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnPopup, OnPopup, TObject *);


  //@}

};


#else  // TPopupMenuH

class TPopupMenu;

#endif  // TPopupMenuH


#undef In_TPopupMenuH
