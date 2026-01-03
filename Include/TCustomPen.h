//-----------------------------------------------------------------------------
//! @file TCustomPen.h
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

#ifdef In_TCustomPenH
#error "Inclusion circulaire de TCustomPen"
#endif // In_TCustomPenH

#define In_TCustomPenH

#ifndef TCustomPenH
#define TCustomPenH


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

enum TPenStyle {
	psSolid,											//!< Trait plein 				___________
	psDash,												//!< Tirets							_ _ _ _ _ _
	psDot,												//!< Trait pointillé 		...........
	psDashDot,										//!< Trait-point				._._._._._.
	psDashDotDot,									//!< Trait-point-point	.._.._.._..
	psClear,											//!< Trait invisible
	psInsideFrame									//!< Trait décalé à l'intérieur
};


//-----------------------------------------------------------------------------
//! @class TCustomPen
//!
//! @brief Définition de la classe TCustomPen
//!
//! Cette classe est la classe de base des stylos permettant de dessiner des
//! traits dans une fenêtre de dessin. Elle gère le style, la couleur et
//! l'épaisseur des traits.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomPen: public TPersistent {
protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomPen(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomPen(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Copie les propriétés d'un autre objet TCustomPen.
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
	//! Cette propriété définit le style du stylo (trait plein, pointillés, ...)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPen, TPenStyle, Style);

  //---------------------------------------------------------------------------
  //! @brief Propriété Width
	//!
	//! Cette propriété définit la largeur du stylo en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPen, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété définit la couleur du stylo.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPen, TColor, Color);

  //@}

};


#else  // TCustomPenH

class TCustomPen;

#endif  // TCustomPenH


#undef In_TCustomPenH
