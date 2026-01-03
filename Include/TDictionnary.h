//-----------------------------------------------------------------------------
//! @file TDictionnary.h
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

#ifdef In_TDictionnaryH
#error "Inclusion circulaire de TDictionnary"
#endif // In_TDictionnaryH

#define In_TDictionnaryH

#ifndef TDictionnaryH
#define TDictionnaryH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TStringsH
#include "TStrings.h"
#endif // TStringsH


//-----------------------------------------------------------------------------
//! @class TDictionnary
//!
//! @brief Définition de la classe TDictionnary
//!
//! Cette classe permet de mémoriser des mots et de les rechercher rapidemment.
//! Elle fonctionne comme la classe TStringList, sauf que la recherche est plus
//! rapide, au détriment de la place mémoire (bien que, s'il y a beaucoup de
//! mots commencant par les mêmes lettres, on n'y perd presque pas).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDictionnary: public TStrings {
private:
	TCHAR FirstLetter;
	int FCount;
protected:
	std::vector<TDictionnary *> Childs;
	bool Add(const TCHAR *szWord);
	bool Seek(TCHAR Letter, bool bCaseSensitive, int *Index);
	bool WriteToFile(HANDLE hFile, CharString asWord);
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

	TDictionnary(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

	~TDictionnary(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Strings
  //!
  //! Cette méthode permet de lire la propriété Strings
  //!
  //! @param[in]        i Index de la propriété à lire
  //!
  //! @return @c AnsiString Propriété Strings
  //---------------------------------------------------------------------------

  virtual AnsiString Get_Strings(int i);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Strings
  //!
  //! Cette méthode permet d'écrire la propriété Strings
  //!
  //! @param[in]        i Index de la propriété à enregistrer
  //! @param[in]        NewStrings Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Strings(int i, AnsiString NewStrings);


  //---------------------------------------------------------------------------
  //! @brief Lecture du nombre de chaînes
  //!
  //! Cette méthode permet de lire le nombre d'éléments chaîne dans la liste.
  //!
  //! @return @c int Nombre de chaînes
  //---------------------------------------------------------------------------

	virtual int FASTCALL GetCount(void) const;

  virtual bool FASTCALL LoadFromFile(const AnsiString FileName);
  virtual bool FASTCALL SaveToFile(const AnsiString FileName);

  virtual void FASTCALL Clear(void);
  virtual int FASTCALL Add(const AnsiString S);
  virtual void FASTCALL Insert(int Index, const AnsiString S);
  virtual void FASTCALL Delete(int Index);

  virtual int FASTCALL IndexOf(const AnsiString S);
	bool FASTCALL IsInDictionnary(const TCHAR *szWord);

  //@}

  //! @name Propriétés
  //@{

  //@}

};


#else  // TDictionnaryH

class TDictionnary;

#endif  // TDictionnaryH


#undef In_TDictionnaryH
