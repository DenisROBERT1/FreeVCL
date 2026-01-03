//-----------------------------------------------------------------------------
//! @file TSizeConstraints.h
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

#ifdef In_TSizeConstraintsH
#error "Inclusion circulaire de TSizeConstraints"
#endif // In_TSizeConstraintsH

#define In_TSizeConstraintsH

#ifndef TSizeConstraintsH
#define TSizeConstraintsH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TControl;


//-----------------------------------------------------------------------------
//! @class TSizeConstraints
//!
//! @brief Définition de la classe TSizeConstraints
//!
//! Cette classe encapsule les contraintes de taille d'une fenêtre ou d'un
//! contrôle. On peut définir la hauteur et la largeur minimum et maximum. La
//! valeur zéro indique qu'il n'y a pas de contrainte. Les hauteurs et largeurs
//! maximum d'une fenêtre sont définies pour une configuration "normale" (style
//! classique windows XP). Elles peuvent donc légèrement dépasser la valeur
//! que vous avez indiqué si vous avez une configuration différente, de telle
//! manière que la taille de la zone client ne soit pas dépendante du système
//! d'exploitation.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TSizeConstraints: public TPersistent {

protected:
  int FMinWidth;
  int FMinHeight;
  int FMaxWidth;
  int FMaxHeight;
  TControl* FControl;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TSizeConstraints(TControl* Control);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété MinWidth: largeur minimum
	//!
	//! Cette propriété détermine la largeur minimum du contrôle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSizeConstraints, int, MinWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété MinHeight: hauteur minimum
	//!
	//! Cette propriété détermine la hauteur minimum du contrôle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSizeConstraints, int, MinHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété MaxWidth: largeur maximum
	//!
	//! Cette propriété détermine la largeur maximum du contrôle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSizeConstraints, int, MaxWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété MaxHeight: hauteur maximum
	//!
	//! Cette propriété détermine la hauteur maximum du contrôle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TSizeConstraints, int, MaxHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété Control
	//!
	//! Cette propriété permet de connaitre l'adresse de l'objet auquel
	//! s'appliquent les contraintes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TSizeConstraints, TControl *, Control);

  //@}

};


#else  // TSizeConstraintsH

class TSizeConstraints;

#endif  // TSizeConstraintsH


#undef In_TSizeConstraintsH
