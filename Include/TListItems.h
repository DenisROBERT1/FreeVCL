//-----------------------------------------------------------------------------
//! @file TListItems.h
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

#ifdef In_TListItemsH
#error "Inclusion circulaire de TListItems"
#endif // In_TListItemsH

#define In_TListItemsH

#ifndef TListItemsH
#define TListItemsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <vector>
#include "TListItem.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TListView;


//-----------------------------------------------------------------------------
//! @class TListItems
//!
//! @brief Définition de la classe TListItems
//!
//! Cette classe permet de gérer une collection d'éléments (objets
//! @b TListItem), c'est à dire de lignes d'un objet @b TListView. Ne créez pas
//! d'instances de cette classe, mais utilisez l'instance automatiquement créée
//! avec les objets @b ListView.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TListItems: public TPersistent {
protected:
  HWND FHandle;
  TListView *FOwner;
  std::vector<TListItem *> FItem;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TListItems(TListView* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  ~TListItems(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Vide la liste d'éléments
  //!
  //! Cette méthode vide complètement la liste d'éléments.
  //---------------------------------------------------------------------------
  virtual void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'un élément vide à la liste
  //!
  //! Cette méthode crée et ajoute un élément vide à la fin de la liste
	//! d'éléments.
  //!
  //! @return @c TListItem* Pointeur sur l'élément créé.
  //---------------------------------------------------------------------------
	virtual TListItem* FASTCALL Add(void);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'un élément à la liste.
  //!
  //! Cette méthode ajoute un élément avec un libellé donné à la fin de la
	//! liste des éléments.
  //!
  //! @param[in]        S Libellé du nouvel élément.
  //!
  //! @return @c TListItem* Pointeur sur l'élément créé.
  //---------------------------------------------------------------------------
	virtual TListItem* FASTCALL Add(const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un élément vide dans la liste.
  //!
  //! Cette méthode crée et insère un élément vide dans la liste d'éléments.
  //!
  //! @param[in]        Index Position où insérer le nouvel élément.
  //---------------------------------------------------------------------------
	virtual void FASTCALL Insert(int Index);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un élément dans la liste.
  //!
  //! Cette méthode crée et insère un élément avec un libellé donné dans la
	//! liste d'éléments.
  //!
  //! @param[in]        Index Position où insérer le nouvel élément.
  //! @param[in]        S Libellé du nouvel élément.
  //---------------------------------------------------------------------------
	virtual void FASTCALL Insert(int Index, const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Destruction d'un élément
  //!
  //! Cette méthode supprime un élément de la liste
  //!
  //! @param[in]        Index Position de l'élément à détruire.
  //---------------------------------------------------------------------------
	virtual void FASTCALL Delete(int Index);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant associé
	//!
	//! Cette propriété permet de lire le handle système du composant
	//! @b TListView auquel appartient cet objet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItems, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Item: ligne de l'objet @b TListView.
	//!
	//! Cette propriété permet d'accéder aux éléments de l'objet @b TListView,
	//! c'est à dire aux lignes, puis ensuite aux sous-éléments (aux colonnes)
	//! au travers de la propriété @b SubItems;
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TListItems, TListItem *, Item);

  //---------------------------------------------------------------------------
  //! @brief Propriété Count
	//!
	//! Cette propriété permet de lire le nombre de lignes que contient l'objet
	//! @b TListView.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TListItems, int, Count);

  //@}

};


#else  // TListItemsH

class TListItems;

#endif  // TListItemsH


#undef In_TListItemsH
