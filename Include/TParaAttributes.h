//-----------------------------------------------------------------------------
//! @file TParaAttributes.h
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

#ifdef In_TParaAttributesH
#error "Inclusion circulaire de TParaAttributes"
#endif // In_TParaAttributesH

#define In_TParaAttributesH

#ifndef TParaAttributesH
#define TParaAttributesH

//---------------------------------------------------------------------------
enum TREBorderColor {bcBlack, bcBlue, bcCyan, bcGreen, bcMagenta, bcRed, bcYellow, bcWhite,
                  bcDarkBlue, bcDarkCyan, bcDarkGreen, bcDarkMagenta, bcDarkRed, bcDarkYellow, bcDarkGray, bcLightGray};

enum TREBorderStyle {bsNoneBorder, bsPoint1, bsPoint2, bsPoint3, bsPoint4, bsPoint5, bsPoint6, bsPointDouble1, bsPointDouble2, bsPointDouble3, bsPointGray, bsPointGrayDashed};

enum TREBorderLocation {blLeftBorder, blRightBorder, blTopBorder, blBottomBorder, blInsideBorder, blOutsideBorder};
typedef Set<TREBorderLocation, blLeftBorder, blOutsideBorder> TREBorderLocations;


enum TRELineSpacingRule {lsrSingle, lsrHalf, lsrDouble, lsrSingleMini, lsrLineSpacing, lsrPercent};


//-----------------------------------------------------------------------------
//! @class TParaAttributes
//!
//! @brief Définition de la classe TParaAttributes
//!
//! Cette classe permet de paramétrer le style de paragraphes des RichEdit.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TParaAttributes: public TPersistent {
private:
  HWND FHandleParent;
  TREBorderLocations FBorderLocations;

protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TParaAttributes(HWND HandleParent);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TParaAttributes(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Transfert des propriétés
  //!
  //! Cette méthode permet de recopier un TParaAttributes externe
  //!
  //! @param[in]        src Objet TParaAttributes à copier
  //---------------------------------------------------------------------------

  void FASTCALL Assign(TParaAttributes * src);


  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété HandleParent
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, HWND, HandleParent);

  //---------------------------------------------------------------------------
  //! @brief Propriété Alignment
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, TAlignment, Alignment);

  //---------------------------------------------------------------------------
  //! @brief Propriété SpaceBefore
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, int, SpaceBefore);

  //---------------------------------------------------------------------------
  //! @brief Propriété SpaceAfter
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, int, SpaceAfter);

  //---------------------------------------------------------------------------
  //! @brief Propriété LineSpacing
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, int, LineSpacing);

  //---------------------------------------------------------------------------
  //! @brief Propriété LineSpacingRule
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, TRELineSpacingRule, LineSpacingRule);

  //---------------------------------------------------------------------------
  //! @brief Propriété FirstIndent
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, int, FirstIndent);


  //---------------------------------------------------------------------------
  //! @brief Propriété BorderLocations
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, TREBorderLocations, BorderLocations);

  //---------------------------------------------------------------------------
  //! @brief Propriété BorderStyle
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, TREBorderStyle, BorderStyle);

  //---------------------------------------------------------------------------
  //! @brief Propriété BorderWidth
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, int, BorderWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété BorderSpace
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, int, BorderSpace);

  //---------------------------------------------------------------------------
  //! @brief Propriété BorderColor
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TParaAttributes, TREBorderColor, BorderColor);

  //@}
};


//---------------------------------------------------------------------------
#endif

#undef In_TParaAttributesH
