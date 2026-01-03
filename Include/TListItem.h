//-----------------------------------------------------------------------------
//! @file TListItem.h
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

#ifdef In_TListItemH
#error "Inclusion circulaire de TListItem"
#endif // In_TListItemH

#define In_TListItemH

#ifndef TListItemH
#define TListItemH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TListViewSubItems.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TListItems;


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#ifndef ListView_SetCheckState
   #define ListView_SetCheckState(hwndLV, i, fCheck) \
      ListView_SetItemState(hwndLV, i, \
      INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif


//-----------------------------------------------------------------------------
//! @class TListItem
//!
//! @brief Définition de la classe TListItem
//!
//! Cette classe permet de paramétrer un élément (une ligne) d'un objet
//! @b TListView.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TListItem: public TPersistent {
private:
protected:
  HWND FHandle;
  AnsiString FCaption;
  bool FChecked;
  bool FSelected;
  int FNumItem;
  TListViewSubItems *FSubItems;
  void *FData;

  TListItems* Owner;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TListItem(TListItems* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TListItem(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un élément de la liste
  //!
  //! Cette fonction supprime l'élément de la liste défini par l'objet
	//! lui-même. Les sous-éléments sont automatiquement détruits.
  //---------------------------------------------------------------------------
  void FASTCALL Delete(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant associé
	//!
	//! Cette propriété permet de lire le handle système du composant
	//! @b TListView auquel appartient cet objet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItem, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Caption
	//!
	//! Cette propriété définit le libellé de l'élément de la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItem, AnsiString, Caption);

  //---------------------------------------------------------------------------
  //! @brief Propriété Checked
	//!
	//! Cette propriété permet de lire ou de définir l'état coché d'un élément de
	//! la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItem, bool, Checked);

  //---------------------------------------------------------------------------
  //! @brief Propriété Selected
	//!
	//! Cette propriété permet de sélectionner ou de déselectionner un élément.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItem, bool, Selected);

  //---------------------------------------------------------------------------
  //! @brief Propriété NumItem
	//!
	//! Cette propriété permet de lire le numéro de l'élément. Cette propriété
	//! doit être modifiée uniquement par le système. Ne modifiez pas cette
	//! propriété, cela pourrait entrainer des dysfonctionnements dans le
	//! composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItem, int, NumItem);

  //---------------------------------------------------------------------------
  //! @brief Propriété SubItems
	//!
	//! Cette propriété permet d'accéder à chaque sous-élément de l'objet
	//! @b TListView. Un TListView contient un certain nombre d'éléments (Les
	//! lignes), et chaque élément contient ensuite des sous-éléments (les
	//! colonnes). Le premier sous-élément (celui d'indice zéro) correspond à
	//! la deuxième colonne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItem, TListViewSubItems *, SubItems);

  //---------------------------------------------------------------------------
  //! @brief Propriété Data
	//!
	//! Cette propriété permet d'associer un objet quelconque à un élément.
	//! Utilisez cette propriété pour mémoriser l'adresse d'un objet. L'élément
	//! ne devient pas propriétaire de l'objet, c'est à dire que sa destruction
	//! n'est pas effectuée lorsqu'on détruit l'élément correspondant.
	//! L'utilisateur devra donc gérer lui-même la création et la destruction de
	//! ces objets.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListItem, void *, Data);


  //@}

};


#else  // TListItemH

class TListItem;

#endif  // TListItemH


#undef In_TListItemH
