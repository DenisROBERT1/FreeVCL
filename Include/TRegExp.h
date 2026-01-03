//-----------------------------------------------------------------------------
//! @file TRegExp.h
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

#ifdef In_TRegExpH
#error "Inclusion circulaire de TRegExp"
#endif // In_TRegExpH

#define In_TRegExpH

#ifndef TRegExpH
#define TRegExpH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TObject.h"


//-----------------------------------------------------------------------------
//! @class TRegExp
//!
//! @brief Définition de la classe TRegExp
//!
//! Cette classe permet de définit une expression régulière et de parser des
//! chaînes en fonction de cette expression.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TRegExp: public TObject {
protected:
  AnsiString FRegExp;

  //---------------------------------------------------------------------------
  //! @brief Teste si une chaîne correspond à l'expression régulière
  //!
  //! Cette fonction teste si l'expression @b asString correspond à
	//! l'expression régulière.
  //!
  //! @param[in]        Beg Index de début dans l'expression régulière
  //! @param[in]        End Index de fin dans l'expression régulière
  //! @param[in]        szString Chaîne à tester
  //! @param[in]        IndexDeb Index de début de la recherche
  //! @param[in]        bCaseSensitive différence minuscules et majuscules
  //! @param[out]       IndexEnd Index de fin dans le chaîne trouvée
  //!
  //! @return @c bool true si la chaîne correspond
  //---------------------------------------------------------------------------

	bool Match(int Beg, int End, const TCHAR *szString, int IndexDeb, bool bCaseSensitive = true, int *IndexEnd = NULL);
	bool MatchOneChar(int Beg, int End, const TCHAR *szString, int IndexDeb, bool bCaseSensitive = true);
	bool ParseOneExp(int *Beg, int End);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
	//!
  //! @param[in]        asRegExp Expression régulière
  //---------------------------------------------------------------------------

  TRegExp(AnsiString asRegExp);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TRegExp(void);

  //@}


  //! @name Méthodes
  //@{
		
  //---------------------------------------------------------------------------
  //! @brief Teste si une chaîne correspond à l'expression régulière
  //!
  //! Cette fonction teste si l'expression @b asString correspond à
	//! l'expression régulière.
  //!
  //! @param[in]        asString Chaîne à tester
  //! @param[in]        bCaseSensitive différence minuscules et majuscules
  //!
  //! @return @c bool true si la chaîne correspond
  //---------------------------------------------------------------------------

	bool Match(AnsiString asString, bool bCaseSensitive = true);

  //---------------------------------------------------------------------------
  //! @brief Sous-chaîne correspondante à l'expression régulière.
  //!
  //! Cette fonction recherche une sous-chaîne correspondante à l'expression
	//! régulière dans la chaîne asString.
  //!
  //! @param[in]        asString Chaîne dans laquelle rechercher
  //! @param[in]        bCaseSensitive différence minuscules et majuscules
  //! @param[in, out]   IndexBeg position de départ de la recherche
  //! @param[out]       IndexEnd position de fin de la recherche
  //!
  //! @return @c int position de la sous-chaîne trouvée, ou -1
  //---------------------------------------------------------------------------

	bool Search(AnsiString asString, bool bCaseSensitive = true, int *IndexBeg = NULL, int *IndexEnd = NULL);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété RegExp
	//!
	//! Cette propriété permet de définir l'expression régulière.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRegExp, AnsiString, RegExp);

  //@}

};

#else  // TRegExpH

class TRegExp;

#endif  // TRegExpH


#undef In_TRegExpH

