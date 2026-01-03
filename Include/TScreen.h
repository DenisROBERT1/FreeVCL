//-----------------------------------------------------------------------------
//! @file TScreen.h
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

#ifdef In_TScreenH
#error "Inclusion circulaire de TScreen"
#endif // In_TScreenH

#define In_TScreenH

#ifndef TScreenH
#define TScreenH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//-----------------------------------------------------------------------------
//! @class TScreen
//!
//! @brief Définition de la classe TScreen
//!
//! Cette classe permet de gérer les paramètres d'affichage communs à toute
//! l'application (le curseur, la fenêtre active, les dimensions de l'écran,
//! etc...). Ne créez pas d'instance de @b TScreen, une instance nommée
//! @b Screen est automatiquement créée en début de programme.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TScreen: public TObject {
protected:
  TCursor FCursor;
  HCURSOR FCursors[22];
  bool bOwned[22];

public:

  //! @name Constructeur et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TScreen(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TScreen(void);

	//@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Cursor: type de curseur
	//!
	//! Cette propriété détermine le curseur affiché lorsque la souris passe sur
	//! une des fenêtres de l'application. Si la propriété @b Cursor est égale à
	//! @b crDefault, alors chaque contrôle détermine individuellement le curseur
	//! affiché lorsque la souris passe dessus.@n
	//! Une des applications les plus classiques est d'afficher un sablier
	//! lorsqu'un traitement long doit être effectué:
	//! @code
	//! Screen->Cursor = crHourGlass;
	//! // Traitement long
	//! CalculFactorielle(10000);
	//! Screen->Cursor = crDefault;
	//! @endcode
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScreen, TCursor, Cursor);

  //---------------------------------------------------------------------------
  //! @brief Propriété Cursors: liste des curseurs disponibles
	//!
	//! Cette propriété permet d'obtenir un handle de curseur en fonction de son
	//! type, ou bien de modifier ou d'ajouter un type de curseur pour toute
	//! l'application. Affectez un handle valide de curseur (obtenu par un
	//! @b LoadCurseur d'une ressource, par exemple) à un des curseurs existants
	//! ou à un nouveau numéro.
	//! @note Les curseurs ainsi affectés sont automatiquement détruits lors de
	//! la sortie de l'application ou lors de son remplacement par une nouvelle
	//! valeur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TScreen, HCURSOR, Cursors);

  //@}

};


#else  // TScreenH

class TScreen;

#endif  // TScreenH

#undef In_TScreenH
