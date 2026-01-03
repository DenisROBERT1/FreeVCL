//-----------------------------------------------------------------------------
//! @file TCustomBrush.h
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

#ifdef In_TCustomBrushH
#error "Inclusion circulaire de TCustomBrush"
#endif // In_TCustomBrushH

#define In_TCustomBrushH

#ifndef TCustomBrushH
#define TCustomBrushH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH

#ifndef TColorH
#include "TColor.h"
#endif  // TColorH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TBrushStyle {
	bsSolid,											//!< Trame pleine
	bsClear,											//!< Trame transparente
	bsHorizontal,									//!< Hachures horizontales
	bsVertical,										//!< Hachures verticales
	bsFDiagonal,									//!< / / / / Hachures obliques 
	bsBDiagonal,									//!< \ \ \ \ Hachures obliques
	bsCross,											//!< Hachures horizontales et verticales
	bsDiagCross										//!< Hachures obliques dans les deux sens
};

//-----------------------------------------------------------------------------
//! @class TCustomBrush
//!
//! @brief Définition de la classe TCustomBrush
//!
//! Cette classe est la classe de base des pinceaux permettant de remplir une
//! zone de dessin.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomBrush: public TPersistent {
protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomBrush(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomBrush(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Copie les propriétés d'un autre objet TCustomBrush.
  //!
  //! @param[in]        Source Objet à copier
  //---------------------------------------------------------------------------
  void FASTCALL Assign(TPersistent* Source);

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
  //! @brief Propriété Style
	//!
	//! Cette propriété définit le style du pinceau (plein, pointillés, ...)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomBrush, TBrushStyle, Style);

  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété définit la couleur du pinceau.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomBrush, TColor, Color);

  //@}

};


#else  // TCustomBrushH

class TCustomBrush;

#endif  // TCustomBrushH


#undef In_TCustomBrushH
