//-----------------------------------------------------------------------------
//! @file TListColumn.h
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

#ifdef In_TListColumnH
#error "Inclusion circulaire de TListColumn"
#endif // In_TListColumnH

#define In_TListColumnH

#ifndef TListColumnH
#define TListColumnH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//! @class TListColumn
//!
//! @brief Définition de la classe TListColumn
//!
//! Cette classe permet de paramétrer une colonne d'un objet @b TDrawGrid et
//! de ses descendants(@b TStringGrid).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TListColumn: public TPersistent {
protected:
  HWND FHandle;
  AnsiString FCaption;
  int FWidth;
  TAlignment FAlignment;
  int FNumColumn;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TListColumn(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant
	//!
	//! Cette propriété permet de lire le handle système du composant
	//! @b TDrawGrid ou @b TStringGrid auquel appartient cet objet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListColumn, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Caption: texte d'une colonne.
	//!
	//! Cette propriété définit le libellé d'une colonne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListColumn, AnsiString, Caption);

  //---------------------------------------------------------------------------
  //! @brief Propriété Width: largeur de la colonne.
	//!
	//! Cette propriété définit la largeur en pixels d'une colonne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListColumn, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Alignment
	//!
	//! Cette propriété permet d'aligner le texte à gauche, au centre ou à droite
	//! dans la colonne correspondante.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListColumn, TAlignment, Alignment);

  //---------------------------------------------------------------------------
  //! @brief Propriété NumColumn
	//!
	//! Cette propriété permet de lire le numéro de la colonne. Cette propriété
	//! doit être modifiée uniquement par le système. Ne modifiez pas cette
	//! propriété, cela pourrait entrainer des dysfonctionnements dans le
	//! composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TListColumn, int, NumColumn);


  //@}

};


#else  // TListColumnH

class TListColumn;

#endif  // TListColumnH


#undef In_TListColumnH
