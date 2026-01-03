//-----------------------------------------------------------------------------
//! @file TBevel.h
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

#ifdef In_TBevelH
#error "Inclusion circulaire de TBevel"
#endif // In_TBevelH

#define In_TBevelH

#ifndef TBevelH
#define TBevelH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TBevelShape {
	bsBox,									      //!< Bordure définie sur les quatre cotés et intérieur rempli
	bsFrame,									    //!< Bordure définie sur les quatre cotés et intérieur transparent
	bsTopLine,									  //!< Bordure définie en haut uniquement
	bsBottomLine,									//!< Bordure définie en bas uniquement
	bsLeftLine,									  //!< Bordure définie à gauche uniquement
	bsRightLine,									//!< Bordure définie à droite uniquement
	bsSpacer									    //!< La bordure est un espace sur les quatre cotés
};

enum TBevelStyle {
	bsLowered,									  //!< Le biseau parait en creux
	bsRaised									    //!< Le biseau parait en relief
};


//-----------------------------------------------------------------------------
//! @class TBevel
//!
//! @brief Définition de la classe TBevel
//! @image html TBevel.jpg "TBevel"
//!
//! Cette classe permet de créer un cadre avec la bordure en relief ou en
//! creux. En général, le but de ce composant est de contenir d'autres
//! composants pour les séparer du reste ou les classer.@n
//! La différence entre ce composant et un TPanel, c'est que le TPanel est
//! opaque alors que celui-ci est transparent (hors bordures, bien entendu).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TBevel: public TWinControl {
private:

protected:
  TBevelShape FShape;
  TBevelStyle FStyle;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TBevel(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TBevel(void);

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
  //! @brief Propriété Shape: Type de bordure.
	//!
	//! Cette propriété permet de définir le style de l'entourage. La valeur par
	//! défaut est bsBox (c'est à dire que l'entourage est dessiné sur les quatre
	//! côtés).
	//! @todo Cette propriété n'est pas encore implémentée
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBevel, TBevelShape, Shape);


  //---------------------------------------------------------------------------
  //! @brief Propriété Style: En relief ou en creux.
	//!
	//! Utilisez cette propriété pour que le biseau de l'entourage apparaisse en
	//! relief ou en creux (valeur par défaut).
	//! @todo Cette propriété n'est pas encore implémentée
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBevel, TBevelStyle, Style);

  //@}


};


#else  // TBevelH

class TBevel;

#endif  // TBevelH


#undef In_TBevelH
