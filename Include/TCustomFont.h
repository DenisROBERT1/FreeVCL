//-----------------------------------------------------------------------------
//! @file TCustomFont.h
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

#ifdef In_TCustomFontH
#error "Inclusion circulaire de TCustomFont"
#endif // In_TCustomFontH

#define In_TCustomFontH

#ifndef TCustomFontH
#define TCustomFontH


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

enum TFontStyle {
	fsBold,												//!< Texte en gras
	fsItalic,											//!< Texte en italiques
	fsUnderline,									//!< Texte souligné
	fsStrikeOut										//!< Texte barré
};

typedef Set<TFontStyle, fsBold, fsStrikeOut> TFontStyles;  //!< Combinaison de plusieurs styles

enum TFontPitch {
	fpDefault,										//!< Espacement par défaut
	fpVariable,										//!< Espacement variable
	fpFixed												//!< Espacement fixe
};

enum TTextFlags {
	tfNull = 0,										//!< Indéfini
	tfTransparent = 1,						//!< Entourage du texte transparent
	tfOpaque = 2									//!< Entourage du texte opaque
};


//-----------------------------------------------------------------------------
//! @class TCustomFont
//!
//! @brief Définition de la classe TCustomFont
//!
//! Cette classe est la classe de base des fontes de caractères. Elle englobe
//! le style, la taille des caractères, la police, la couleur, etc...
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomFont: public TPersistent {
protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomFont(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomFont(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Copie les propriétés d'un autre objet TCustomFont.
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
  //! @brief Propriété Width
	//!
	//! Cette propriété définit la largeur en pixels des caractères de la fonte.
	//! Si cette propriété est à zéro, le système définit automatiquement une
	//! largeur standard dépendante de la police de caractères (souvent 50% de la
	//! hauteur).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété définit la hauteur en pixels des caractères de la fonte.
	//! Si la hauteur est négative, c'est la hauteur d'un caractère majuscule non
	//! accentué (un E par exemple). Sinon, c'est la hauteur totale d'un
	//! caractère, c'est à dire en tenant compte des jambages et des accents.
	//!
	//! @sa Size
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Propriété Size
	//!
	//! Cette propriété définit la hauteur totale en points d'un caractère, c'est
	//! à dire en tenant compte des jambages et des accents. La valeur d'un point
	//! se calcule à partir de la hauteur en pixels avec la formule suivante:@n
	//! Un point = un pixel * 72 / PixelsPerInch
	//!
	//! @sa Height, PixelsPerInch
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, int, Size);

  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété définit le nom de la police de caractères.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, AnsiString, Name);

  //---------------------------------------------------------------------------
  //! @brief Propriété Style
	//!
	//! Cette propriété détermine le style de la fonte (caractères gras,
	//! italiques, souligné ou/et barré).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, TFontStyles, Style);

  //---------------------------------------------------------------------------
  //! @brief Propriété Charset
	//!
	//! Cette propriété définit le jeu de caractères de la fonte. 
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, TFontCharset, Charset);

  //---------------------------------------------------------------------------
  //! @brief Propriété Pitch
	//!
	//! Cette propriété définit le type d'espacement des caractères de la fonte
	//! (espacement fixe ou variable).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, TFontPitch, Pitch);

  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété définit la couleur des caractères de la fonte.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, TColor, Color);

  //---------------------------------------------------------------------------
  //! @brief Propriété PixelsPerInch
	//!
	//! Cette propriété définit le nombre de pixels par points de la fonte. Cette
	//! propriété est par défaut de 96 quelle que soit la résolution d'écran.
	//! Cette valeur permet de calculer la correspondance entre les propriétés
	//! @b Size et @b Height.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, int, PixelsPerInch);

  //---------------------------------------------------------------------------
  //! @brief Propriété CharacterExtra
	//!
	//! Cette propriété définit l'espacement en pixels entre les caractères.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomFont, int, CharacterExtra);

  //@}

};


#else  // TCustomFontH

class TCustomFont;

#endif  // TCustomFontH


#undef In_TCustomFontH
