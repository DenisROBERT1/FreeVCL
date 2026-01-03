//-----------------------------------------------------------------------------
//! @file TSpeedButton.h
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

#ifdef In_TSpeedButtonH
#error "Inclusion circulaire de TSpeedButton"
#endif // In_TSpeedButtonH

#define In_TSpeedButtonH

#ifndef TSpeedButtonH
#define TSpeedButtonH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TSpeedButton
//!
//! @brief Définition de la classe TSpeedButton : bouton à deux états.
//! @image html TSpeedButton.jpg "TSpeedButton"
//!
//! Cette classe est un contrôle semblable à un bouton, sauf qu'il peut avoir
//! deux états, position haute et position enfoncée. S'il y a plusieurs boutons
//! dans une boîte de dialogue, on peut leur y affecter un numéro de groupe de
//! telle manière qu'un seul bouton du groupe puisse être en position basse en
//! même temps.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TSpeedButton: public TWinControl {
private:
protected:
  bool FAllowAllUp;
  int FGroupIndex;
  int FNumGlyph;
  TBitmap * FGlyph;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessCommand(WORD Notify);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TSpeedButton(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TSpeedButton(void);

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

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété AllowAllUp: tous les boutons hauts autorisés
	//!
	//! Cette propriété détermine si tous les boutons d'un groupe peuvent être
	//! en position haute en même temps.
	//!
	//! @sa GroupIndex
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSpeedButton, bool, AllowAllUp);

  //---------------------------------------------------------------------------
  //! @brief Propriété Down: position basse
	//!
	//! Cette propriété permet de lire et de définir la position d'un bouton :
	//! position enfoncée si cette propriété est à @b true, position haute sinon.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSpeedButton, bool, Down);

  //---------------------------------------------------------------------------
  //! @brief Propriété Flat: boutons plats
	//!
	//! Cette propriété détermine l'aspect des boutons, qui peuvent être en
	//! relief ou bien avoir un aspect plus plat.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSpeedButton, bool, Flat);

  //---------------------------------------------------------------------------
  //! @brief Propriété GroupIndex
	//!
	//! Cette propriété définit le groupe de boutons auquel il appartient.
	//! Certaines propriétés s'appliquent ensuite à tout le groupe de boutons,
	//! comme la propriété @b AllowAllUp.
	//!
	//! @sa AllowAllUp
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSpeedButton, int, GroupIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété NumGlyphs: nombre d'images dans la bitmap
	//!
	//! Cette propriété permet de définir le nombre d'images dans la bitmap de la
	//! propriété @b Glyph. Ces différentes images permettent d'avoir une
	//! représentation différente du bouton suivant son état (voir la propriété 
	//! TSpeedButton::Glyph pour plus de détails). La valeur par défaut est de 1
	//! (c'est à dire une seule image).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSpeedButton, int, NumGlyph);

  //---------------------------------------------------------------------------
  //! @brief Propriété Glyph: Image(s) du bouton.
	//!
	//! Cette propriété permet de définir l'image (ou les images) qui apparaît
	//! dans le bouton. Il est possible de définir une bitmap comprenant
	//! plusieurs images cote à cote de gauche à droite (jusqu'à quatre images).
	//! Le nombre d'image est défini par la propriété @b NumGlyphs. Ces images
	//! sont affichées en fonction de l'état du bouton:
	//! - Première image: état normal (haut) du bouton.
	//! - Deuxième image: état désactivé (grisé).
	//! - Troisième image: état cliqué (bas).
	//! - Quatrième image: état sélectionné (boutons à deux états).
	//!
	//! @note Le composant fait une copie de l'image, mais n'en devient pas
	//! propriétaire. La fonction appelante est donc chargée de sa destruction
	//! après utilisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSpeedButton, TBitmap *, Glyph);

  //@}


};


#else  // TSpeedButtonH

class TSpeedButton;

#endif  // TSpeedButtonH


#undef In_TSpeedButtonH
