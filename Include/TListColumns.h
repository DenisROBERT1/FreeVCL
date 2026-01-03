//-----------------------------------------------------------------------------
//! @file TListColumns.h
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

#ifdef In_TListColumnsH
#error "Inclusion circulaire de TListColumns"
#endif // In_TListColumnsH

#define In_TListColumnsH

#ifndef TListColumnsH
#define TListColumnsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TListcolumn.h"


//-----------------------------------------------------------------------------
//! @class TListColumns
//!
//! @brief Définition de la classe TListColumns
//!
//! Cette classe permet de gérer les colonnes d'un objet @b TDrawGrid ou de
//! ses descendants, elle offre la possibilité d'insérer, ou de supprimer des
//! colonnes, par exemple.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TListColumns: public TPersistent {
protected:
  HWND FHandle;
  std::vector<TListColumn *> FItems;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TListColumns(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TListColumns(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Mise à zéro
  //!
  //! Cette méthode supprime toutes les colonnes de la string-grid.
  //---------------------------------------------------------------------------
  virtual void Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'une colonne vide.
  //!
  //! Cette méthode ajoute une colonne vide à la fin de la string-grid.
  //!
  //! @return @c TListColumn Objet représentant la nouvelle colonne.
  //---------------------------------------------------------------------------
	virtual TListColumn *Add(void);

	//---------------------------------------------------------------------------
  //! @brief Ajout d'une colonne
  //!
  //! Cette méthode ajoute une colonne à la fin de la string-grid. Cette
	//! nouvelle colonne a pour titre la chaîne S, et aura pour largeur la valeur
	//! Width.
  //!
  //! @param[in]				S Libellé de la nouvelle colonne
  //! @param[in]        Width Largeur de la nouvelle colonne
  //!
  //! @return @c TListColumn Objet représentant la nouvelle colonne.
  //---------------------------------------------------------------------------
  virtual TListColumn *Add(const AnsiString S, int Width);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'une colonne
  //!
  //! Cette méthode permet d'insérer une colonne
  //!
  //! @param[in]        Index Position de l'insertion
  //! @param[in]				S Libellé de la nouvelle colonne
  //! @param[in]        Width Largeur de la nouvelle colonne
  //---------------------------------------------------------------------------
	virtual void Insert(int Index, const AnsiString S, int Width);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une colonne
  //!
  //! Cette méthode permet de supprimer une colonne.
  //!
  //! @param[in]        Index Position de la colonne à supprimer (première = 0)
  //---------------------------------------------------------------------------
	virtual void Delete(int Index);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant associé.
	//!
	//! Cette propriété permet de lire le handle système du composant
	//! @b TDrawGrid ou @b TStringGrid auquel appartient cet objet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListColumns, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Items: liste des colonnes.
	//!
	//! Cette propriété permet de paramétrer chaque colonne de la string-list,
	//! la largeur et l'alignement du texte, entre autres.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TListColumns, TListColumn *, Items);

  //---------------------------------------------------------------------------
  //! @brief Propriété Count: Nombre de colonnes.
	//!
	//! Cette propriété permet de lire le nombre de colonnes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TListColumns, int, Count);

  //@}

};


#else  // TListColumnsH

class TListColumns;

#endif  // TListColumnsH


#undef In_TListColumnsH
