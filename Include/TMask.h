//-----------------------------------------------------------------------------
//! @file TMask.h
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

#ifdef In_TMaskH
#error "Inclusion circulaire de TMask"
#endif // In_TMaskH

#define In_TMaskH

#ifndef TMaskH
#define TMaskH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TObject.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TStringList;


//-----------------------------------------------------------------------------
//! @class TMask
//!
//! @brief Définition de la classe TMask
//!
//! Cette classe permet de tester la concordance de noms de fichiers avec un
//! ou des masques (filtres).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMask: public TObject {
protected:
	TStringList *slExtList;
	
	bool MatchExt(const TCHAR* szFileName, const TCHAR* szExt);

public:

  //! @name Constructeur et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //!
  //! Permet de construire l'objet avec des masques de recherche de fichiers.
	//! La chaîne passée en paramètre contient un ou plusieurs masques séparés
	//! par des points-virgules, chaque masque pouvant les jokers (caractères "*"
	//! et "?").@n
	//! Exemples:@n
	//! "*.bmp"@n
	//! "*.bmp;*.jpg;*.png"@n
  //!
  //! @param[in]        AMaskValue Masque(s) de recherche de fichiers
  //---------------------------------------------------------------------------

  TMask(const AnsiString AMaskValue);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TMask(void);

  //@}


	//@}

  //! @name Méthodes
  //@{
	  
  //---------------------------------------------------------------------------
  //! @brief Teste si le nom de fichier correspond au masque.
  //!
  //! Cette fonction teste si le nom de fichier correspond au masque passé dans
  //! du constructeur de l'objet TMask.
  //!
  //! @param[in]        FileName Nom de fichier à tester
  //!
  //! @return @c bool true si le nom correspond
  //---------------------------------------------------------------------------
	bool FASTCALL Matches(const AnsiString FileName);

  //---------------------------------------------------------------------------
  //! @brief Teste si le nom de fichier correspond au masque.
  //!
  //! Cette fonction teste si le nom de fichier correspond au masque passé dans
  //! du constructeur de l'objet TMask.
  //!
  //! @param[in]        szFileName Nom de fichier à tester
  //!
  //! @return @c bool true si le nom correspond
  //---------------------------------------------------------------------------
	bool FASTCALL Matches(const TCHAR* szFileName);

  //@}

};


#else  // TMaskH

class TMask;

#endif  // TMaskH

#undef In_TMaskH
