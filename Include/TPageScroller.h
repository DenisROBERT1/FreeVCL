//-----------------------------------------------------------------------------
//! @file TPageScroller.h
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

#ifdef In_TPageScrollerH
#error "Inclusion circulaire de TPageScroller"
#endif // In_TPageScrollerH

#define In_TPageScrollerH

#ifndef TPageScrollerH
#define TPageScrollerH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TPageScrollerButton {
	sbFirst = PGB_TOPORLEFT,			//!< Bouton de gauche ou du haut (suivant orientation)
	sbLast = PGB_BOTTOMORRIGHT		//!< Bouton de droite ou du bas (suivant orientation)
};

enum TPageScrollerOrientation {
	soHorizontal,									//!< Défilement horizontal
	soVertical										//!< Défilement vertical
};

enum TPageScrollerButtonState {
	bsNormal = PGF_NORMAL,				//!< Le bouton est dans son état normal
	bsInvisible = PGF_INVISIBLE,	//!< Le bouton est invisible
	bsGrayed = PGF_GRAYED,				//!< Le bouton est grisé
	bsDepressed = PGF_DEPRESSED,	//!< Le bouton est enfoncé
	bsHot	= PGF_HOT								//!< Le bouton est actif (c'est à dire que la souris survole le bouton)
};


//-----------------------------------------------------------------------------
//! @class TPageScroller
//!
//! @brief Définition de la classe TPageScroller
//! @image html TPageScroller.jpg "TPageScroller"
//!
//! Cette classe est un contrôle permettant d'organiser plusieurs sous-objets
//! dans un espace réduit, les sous-objets étant accessibles à l'aide d'une
//! barre de défilement. l'utilisation classique est la conception de barres
//! d'outils.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPageScroller: public TWinControl {
private:
protected:
	TWinControl *FControl;
	int FIncrement;
	int FMargin;

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

  TPageScroller(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPageScroller(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Etat d'un bouton
  //!
  //! Cette méthode renvoie l'état du bouton défini par l'argument Button.
  //!
  //! @param[in]        Button Détermine le bouton à tester
  //!
  //! @return @c TPageScrollerButtonState Etat du bouton
  //---------------------------------------------------------------------------
	TPageScrollerButtonState FASTCALL GetButtonState(TPageScrollerButton Button);

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
  //! @brief Setter de la propriété Color
  //!
  //! Cette méthode permet d'écrire la propriété Color
  //!
  //! @param[in]        NewColor Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Color(TColor NewColor);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété ClientRect
  //!
  //! Cette méthode permet de lire la propriété ClientRect
  //!
  //! @return @c int Propriété ClientRect
  //---------------------------------------------------------------------------

	virtual TRect Get_ClientRect(void);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un composant enfant
  //---------------------------------------------------------------------------

  virtual void InsertComponent(TComponent* AComponent, int Index = -1);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un composant enfant de la liste des composants
  //---------------------------------------------------------------------------

  virtual void RemoveComponent(TComponent* AComponent);

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
  //! @brief Propriété ButtonSize: taille des boutons de défilement
	//!
	//! Cette propriété définit la taille des boutons de défilement. La valeur
	//! par défaut est de 12 pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageScroller, int, ButtonSize);

  //---------------------------------------------------------------------------
  //! @brief Propriété Control: Fenêtre enfant
	//!
	//! Cette propriété définit la fenêtre enfant de l'objet @b TPageScroller. Si
	//! le @b TPageScroller est horizontal, la fenêtre enfant doit être moins
	//! haute que la zone client. Et s'il est vertical, moins large que la zone
	//! client. En général, vous n'avez pas à définir cette propriété, le premier
	//! control enfant est automatiquement affecté à cette propriété.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageScroller, TWinControl *, Control);

  //---------------------------------------------------------------------------
  //! @brief Propriété Increment: valeur de décalage
	//!
	//! Cette propriété définit la valeur du scroll en pixels lorsque
	//! l'utilisateur appuie sur un bouton de défilement. La valeur par défaut
	//! est de 10 pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageScroller, int, Increment);

  //---------------------------------------------------------------------------
  //! @brief Propriété Margin: marge intérieure
	//!
	//! Cette propriété définit la marge intérieure entre le @b TPageScroller et
	//! le composant enfant qu'il contient.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageScroller, int, Margin);

  //---------------------------------------------------------------------------
  //! @brief Propriété Orientation: orientation de la barre de défilement.
	//!
	//! Cette propriété définit l'orientation de la barre de défilement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageScroller, TPageScrollerOrientation, Orientation);

  //---------------------------------------------------------------------------
  //! @brief Propriété Position: position du scroll
	//!
	//! Cette propriété définit le décalage de la fenêtre de défilement en
	//! pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageScroller, int, Position);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnScroll
	//!
	//! Cet évènement est déclenché lorsque la position de l'ascenseur est
	//! modifié.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnScroll, TObject *);

  //@}


};


#else  // TPageScrollerH

class TPageScroller;

#endif  // TPageScrollerH


#undef In_TPageScrollerH
