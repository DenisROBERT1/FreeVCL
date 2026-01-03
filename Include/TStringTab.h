//-----------------------------------------------------------------------------
//! @file TStringTab.h
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

#ifdef In_TStringTabH
#error "Inclusion circulaire de TStringTab"
#endif // In_TStringTabH

#define In_TStringTabH

#ifndef TStringTabH
#define TStringTabH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TStringList.h"


//-----------------------------------------------------------------------------
//! @class TStringTab
//!
//! @brief Définition de la classe TStringTab
//!
//! Cette classe est un tableau bi-dimensionnel de strings.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TStringTab: public TPersistent {
protected:
  int FColCount;
  bool Updating;
  std::vector<TStringList *> FItems;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TStringTab(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TStringTab(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Strings
  //!
  //! Cette méthode permet de lire la propriété Strings
  //!
  //! @param[in]        ACol Index de la colonne à lire
  //! @param[in]        ARow Index de la ligne à lire
  //!
  //! @return @c AnsiString Propriété Strings
  //---------------------------------------------------------------------------

  virtual AnsiString Get_Strings(int ACol, int ARow);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Strings
  //!
  //! Cette méthode permet d'écrire la propriété Strings
  //!
  //! @param[in]        ACol Index de la colonne à écrire
  //! @param[in]        ARow Index de la ligne à écrire
  //! @param[in]        NewStrings Chaîne à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Strings(int ACol, int ARow, AnsiString NewStrings);


  //---------------------------------------------------------------------------
  virtual void FASTCALL SetUpdateState(bool Updating);

  virtual void FASTCALL Clear(void);
  virtual int AddCol(void);
  virtual int AddRow(void);
  virtual void InsertCol(int Index);
  virtual void InsertRow(int Index);
  virtual void DeleteCol(int Index);
  virtual void DeleteRow(int Index);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ColCount: nombre de colonnes
	//!
	//! Cette propriété permet de lire ou de définir le nombre de colonnes. La
	//! valeur par défaut est de un.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStringTab, int, ColCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété RowCount: nombre de lignes
	//!
	//! Cette propriété permet de lire ou de définir le nombre de lignes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStringTab, int, RowCount);

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

  //@}


};


#else  // TStringTabH

class TStringTab;

#endif  // TStringTabH


#undef In_TStringTabH
