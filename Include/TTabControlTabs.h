//-----------------------------------------------------------------------------
//! @file TTabControlTabs.h
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

#ifdef In_TTabControlTabsH
#error "Inclusion circulaire de TTabControlTabs"
#endif // In_TTabControlTabsH

#define In_TTabControlTabsH

#ifndef TTabControlTabsH
#define TTabControlTabsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TStringsH
#include "TStrings.h"
#endif // TStringsH


//-----------------------------------------------------------------------------
//! @class TTabControlTabs
//!
//! @brief Définition de la classe TTabControlTabs
//!
//! Cette classe représente la liste des onglets d'un objet @b TTabControl.
//! Elle implémente simplement les méthodes abstraites de la classe ancêtre
//! @b TStrings, sans autres fonctionnalités.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTabControlTabs: public TStrings {
protected:
  HWND FHandle;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TTabControlTabs(void);

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


  //---------------------------------------------------------------------------
  virtual void FASTCALL Clear(void);
  virtual int FASTCALL Add(const AnsiString S);
  virtual void FASTCALL Insert(int Index, const AnsiString S);
  virtual void FASTCALL Delete(int Index);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant
	//!
	//! Cette propriété permet de lire le handle du contrôle @b TTabControl
	//! associé à cet objet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTabControlTabs, HWND, Handle);

  //@}

};


#else  // TTabControlTabsH

class TTabControlTabs;

#endif  // TTabControlTabsH


#undef In_TTabControlTabsH
