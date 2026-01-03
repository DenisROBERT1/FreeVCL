//-----------------------------------------------------------------------------
//! @file TBitBtn.h
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

#ifdef In_TBitBtnH
#error "Inclusion circulaire de TBitBtn"
#endif // In_TBitBtnH

#define In_TBitBtnH

#ifndef TBitBtnH
#define TBitBtnH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TButtonH
#include "TButton.h"
#endif  // TButtonH


//---------------------------------------------------------------------------
// enum
//---------------------------------------------------------------------------

enum TButtonLayout {
	blGlyphLeft,				//!< Image à gauche du texte
	blGlyphRight,				//!< Image à droite du texte
	blGlyphTop,					//!< Image au dessus du texte
	blGlyphBottom				//!< Image en dessous du texte
};

enum TButtonStyle {
	bsAutoDetect,				//!< Style dépendant du système
	bsWin31,						//!< Style Windows 3.1
	bsNew								//!< Nouveau style 
};


//-----------------------------------------------------------------------------
//! @class TBitBtn
//!
//! @brief Bouton avec bitmap.
//! @image html TBitBtn.jpg "TBitBtn"
//!
//! Cette classe permet de créer un bouton avec une image. L'image peut être
//! positionnée par rapport au texte (à gauche ou à droite, au dessus ou en
//! dessous) avec un espacement et une marge paramétrables. Elle hérite de
//! @b TButton, donc elle possède aussi les méthodes et propriétés des boutons
//! standard.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TBitBtn: public TButton {
private:
protected:
  TBitmap * FGlyph;
  TButtonLayout FLayout;
  int FMargin;
  int FNumGlyphs;
  int FSpacing;
  TButtonStyle FStyle;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS);
  virtual bool ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TBitBtn(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TBitBtn(void);

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
  //! @brief Propriété Glyph: Image(s) du bouton.
	//!
	//! Cette propriété permet de définir l'image (ou les images) qui apparaît
	//! dans le bouton. Il est possible de définir une bitmap comprenant
	//! plusieurs images cote à cote de gauche à droite (jusqu'à quatre images).
	//! Le nombre d'image est défini par la propriété @b NumGlyphs. Ces images
	//! sont affichées en fonction de l'état du bouton:
	//! - Première image: état normal (haut) du bouton.
	//! - Deuxième image: état désactivé (grisé).
	//! - Troisième image: état cliqué (bas).
	//! - Quatrième image: état sélectionné (boutons à deux états).
	//!
	//! @note Le composant fait une copie de l'image, mais n'en devient pas
	//! propriétaire. La fonction appelante est donc chargée de sa destruction
	//! après utilisation.
	//!
	//! @sa NumGlyphs
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitBtn, TBitmap *, Glyph);

  //---------------------------------------------------------------------------
  //! @brief Propriété Layout: Position de l'image par rapport au texte
	//!
	//! Cette propriété permet de définir la position de l'image par rapport au
	//! text: à gauche (@b blGlyphLeft), à droite (@b blGlyphRight), au dessus
	//! (@b blGlyphTop) ou en dessous (@b blGlyphBottom).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitBtn, TButtonLayout, Layout);

  //---------------------------------------------------------------------------
  //! @brief Propriété Margin: Espace entre le bord du bouton et l'image
	//!
	//! Cette propriété permet de définir l'espace entre le bord intérieur du
	//! bouton et l'image en pixels. Si @b Margin = 0, l'image sera collée à la
	//! bordure du bouton. Si @b Margin = -1 (valeur par défaut), le bouton et le
	//! texte seront centrés: il y aura le même espacement entre le bord du
	//! bouton et l'image d'un coté, et entre l'autre bord du bouton et le texte.
	//!
	//! @image html TBitBtn_2_2.jpg "Margin = 2 Spacing = 2"
	//! @image html TBitBtn_2_m1.jpg "Margin = 2 Spacing = -1"
	//! @image html TBitBtn_m1_2.jpg "Margin = -1 Spacing = 2"
	//! @image html TBitBtn_m1_m1.jpg "Margin = -1 Spacing = -1"
	//!
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitBtn, int, Margin);


  //---------------------------------------------------------------------------
  //! @brief Propriété NumGlyphs: nombre d'images dans la bitmap
	//!
	//! Cette propriété permet de définir le nombre d'images dans la bitmap de la
	//! propriété @b Glyph. Ces différentes images permettent d'avoir une
	//! représentation différente du bouton suivant son état (voir la propriété 
	//! TBitBtn::Glyph pour plus de détails). La valeur par défaut est de 1
	//! (c'est à dire une seule image).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitBtn, int, NumGlyphs);

  //---------------------------------------------------------------------------
  //! @brief Propriété Spacing: Espace entre l'image et le texte.
	//!
	//! Cette propriété permet de définir l'espace entre l'image et le texte en
	//! pixels. La valeur par défaut est de 4 pixels. Si @b Spacing = -1, le
	//! texte sera centré entre l'image et le bord du bouton.
	//!
	//! @sa Margin
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitBtn, int, Spacing);


  //---------------------------------------------------------------------------
  //! @brief Propriété Style: Style du bouton.
	//!
	//! Cette propriété permettait de définir le style du bouton en fonction du
	//! système. Elle permettait d'avoir un style identique à celui de Windows
	//! 3.1, par exemple. Elle n'est conservée que par souci de compatibilité,
	//! seuls les fous ayant encore envie d'avoir des boutons de style 3.1. La
	//! valeur par défaut (@b bsAutoDetect) est toujours appliquée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitBtn, TButtonStyle, Style);


  //@}


};


#else  // TBitBtnH

class TBitBtn;

#endif  // TBitBtnH


#undef In_TBitBtnH
