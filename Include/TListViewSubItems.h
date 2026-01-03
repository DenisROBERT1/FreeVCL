//-----------------------------------------------------------------------------
//! @file TListViewSubItems.h
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

#ifdef In_TListViewSubItemsH
#error "Inclusion circulaire de TListViewSubItems"
#endif // In_TListViewSubItemsH

#define In_TListViewSubItemsH

#ifndef TListViewSubItemsH
#define TListViewSubItemsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TStringsH
#include "TStrings.h"
#endif // TStringsH


//-----------------------------------------------------------------------------
//! @class TListViewSubItems
//!
//! @brief Définition de la classe TListViewSubItems
//!
//! Cette classe permet de gérer les sous-éléments (les colonnes) d'un objet
//! @b TListView. Elle hérite de la classe @b TString, dont elle implémente les
//! méthodes abstraites.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TListViewSubItems: public TStrings {
protected:
  HWND FHandle;
  int FNumItem;
  int NbSubItems;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TListViewSubItems(void);

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
  //! @brief Effacement complet de la liste
  //!
  //! Cette méthode met à zéro la liste.
  //---------------------------------------------------------------------------
  virtual void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'une chaîne à la fin 
  //!
  //! Cette méthode ajoute une chaîne de caractères en fin de liste.
  //!
  //! @param[in]        S Chaîne de caractères à ajouter
  //!
  //! @return @c int Position de la nouvelle chaîne (premier = 0)
  //---------------------------------------------------------------------------
	virtual int FASTCALL Add(const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'une chaîne dans la liste
  //!
  //! Cette méthode insère une chaîne dans la liste de chaînes.
  //!
  //! @param[in]        Index Position d'insertion
  //! @param[in]        S Chaîne de caractères à insérer
  //---------------------------------------------------------------------------
	virtual void FASTCALL Insert(int Index, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une chaîne de la liste
  //!
  //! Cette méthode détruit une chaîne de la liste
  //!
  //! @param[in]        Index Position de la chaîne à détruire
  //---------------------------------------------------------------------------
	virtual void FASTCALL Delete(int Index);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant associé
	//!
	//! Cette propriété permet de lire le handle système du composant
	//! @b TListView auquel appartient indirectement cet objet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListViewSubItems, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété NumItem
	//!
	//! Cette propriété permet de lire le numéro de l'élément. Cette propriété
	//! doit être modifiée uniquement par le système. Ne modifiez pas cette
	//! propriété, cela pourrait entrainer des dysfonctionnements dans le
	//! composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListViewSubItems, int, NumItem);

  //@}

};


#else  // TListViewSubItemsH

class TListViewSubItems;

#endif  // TListViewSubItemsH


#undef In_TListViewSubItemsH
