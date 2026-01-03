//-----------------------------------------------------------------------------
//! @file TComboBoxItems.h
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

#ifdef In_TComboBoxItemsH
#error "Inclusion circulaire de TComboBoxItems"
#endif // In_TComboBoxItemsH

#define In_TComboBoxItemsH

#ifndef TComboBoxItemsH
#define TComboBoxItemsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TComboBoxItems
//!
//! @brief Définition de la classe TComboBoxItems
//!
//! Cette classe définit un élément d'une liste déroulante.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TComboBoxItems: public TStrings {
protected:
  HWND FHandle;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TComboBoxItems(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TComboBoxItems(void);

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
  //! @brief Mise à zéro de la liste
  //!
  //! Cette méthode vide entièrement la liste.
  //---------------------------------------------------------------------------
  virtual void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'une chaîne dans la liste
  //!
  //! Cette méthode ajoute une chaîne en fin de liste.
  //!
  //! @param[in]        S Chaîne à ajouter
  //---------------------------------------------------------------------------
	virtual int FASTCALL Add(const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'une chaîne
  //!
  //! Cette méthode ajoute une chaîne dans la liste.
  //!
  //! @param[in]        Index Position d'insertion
  //! @param[in]        S Chaîne à ajouter
  //---------------------------------------------------------------------------
	virtual void FASTCALL Insert(int Index, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une chaîne de la liste
  //!
  //! Cette méthode supprime une chaîne de la liste.
  //!
  //! @param[in]        Index Position de la chaîne à effacer (première = 0)
  //---------------------------------------------------------------------------
	virtual void FASTCALL Delete(int Index);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant associé
	//!
	//! Cette propriété définit le contrôle auquel appartient la liste
	//! d'éléments.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComboBoxItems, HWND, Handle);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque les items sont modifiés, soit par une
	//! action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

	//@}

};


#else  // TComboBoxItemsH

class TComboBoxItems;

#endif  // TComboBoxItemsH


#undef In_TComboBoxItemsH
