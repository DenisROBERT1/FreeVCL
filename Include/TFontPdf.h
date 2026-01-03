//-----------------------------------------------------------------------------
//! @file TFontPdf.h
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

#ifdef In_TFontPdfH
#error "Inclusion circulaire de TFontPdf"
#endif // In_TFontPdfH

#define In_TFontPdfH

#ifndef TFontPdfH
#define TFontPdfH

#include <TCustomFont.h>


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TObjetPdfFont;


//---------------------------------------------------------------------------
//! @class TFontPdf
//!
//! @brief Font PDF
//!
//! Cette classe (@c TFontPdf) représente la font d'un Canvas PDF. Elle a
//! les mêmes méthodes que la classe TFont d'un Canvas classique.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TFontPdf: public TCustomFont {
private:

protected:
  int FHeight;
  int FWidth;
  int FCharacterExtra;
  TFontStyles FStyle;
  AnsiString FName;
  TColor FColor;
  TColorPdf *ColorPdf;

  bool bChangeHeight;
  bool bChangeWidth;
  bool bChangeCharacterExtra;
  bool bChangeStyle;
  bool bChangeName;

  TObjetPdfFont *FObjetPdfFont[14];

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //!
  //! @param[in] CommonColorPdf Couleur commune
  //!
  //! @note Dans un fichier PDF, le fait de changer de couleur de brush change
  //! la couleur du texte. L'objet CommonColorPdf passé en paramètres représente
  //! cette couleur unique.
  //---------------------------------------------------------------------------

  TFontPdf(TColorPdf *CommonColorPdf);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFontPdf(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  void Clear(void);
  CharString GetStream(void);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontPdf, int, Height);


  //---------------------------------------------------------------------------
  //! @brief Propriété Width
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontPdf, int, Width);


  //---------------------------------------------------------------------------
  //! @brief Propriété CharacterExtra
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontPdf, int, CharacterExtra);


  //---------------------------------------------------------------------------
  //! @brief Propriété Style
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontPdf, TFontStyles, Style);


  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontPdf, AnsiString, Name);


  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontPdf, TColor, Color);


  //---------------------------------------------------------------------------
  //! @brief Propriété ObjetPdfFont
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TFontPdf, TObjetPdfFont *, ObjetPdfFont);


  //@}

};


#endif

#undef In_TFontPdfH
