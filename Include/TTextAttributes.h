//-----------------------------------------------------------------------------
//! @file TTextAttributes.h
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

#ifdef In_TTextAttributesH
#error "Inclusion circulaire de TTextAttributes"
#endif // In_TTextAttributesH

#define In_TTextAttributesH

#ifndef TTextAttributesH
#define TTextAttributesH

//---------------------------------------------------------------------------
enum TConsistentAttribute {caBold, caColor, caBackColor, caFace, caItalic, caSize, caOffset, caStrikeOut, caUnderline, caProtected};

typedef Set<TConsistentAttribute, ::caBold, ::caProtected> TConsistentAttributes;

typedef BYTE TFontCharset;



//-----------------------------------------------------------------------------
//! @class TTextAttributes
//!
//! @brief Définition de la classe TTextAttributes
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur le
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTextAttributes: public TPersistent {
private:
  HWND FHandleParent;
  BOOL bSelection;

protected:
  TConsistentAttributes FConsistentAttributes;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTextAttributes(HWND HandleParent, BOOL bSelection);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTextAttributes(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Transfert des propriétés
  //!
  //! Cette méthode permet de recopier un TTextAttributes externe
  //!
  //! @param[in]        src Objet TTextAttributes à copier
  //---------------------------------------------------------------------------

  void FASTCALL Assign(TTextAttributes * src);


  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété HandleParent
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, HWND, HandleParent);

  //---------------------------------------------------------------------------
  //! @brief Propriété ConsistentAttributes
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, TConsistentAttributes, ConsistentAttributes);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Propriété Size
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, int, Size);


  //---------------------------------------------------------------------------
  //! @brief Propriété Offset
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, int, Offset);


  //---------------------------------------------------------------------------
  //! @brief Propriété Style
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, TFontStyles, Style);

  //---------------------------------------------------------------------------
  //! @brief Propriété Charset
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, TFontCharset, Charset);

  //---------------------------------------------------------------------------
  //! @brief Propriété Pitch
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, TFontPitch, Pitch);

  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, TColor, Color);

  //---------------------------------------------------------------------------
  //! @brief Propriété BackColor
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, TColor, BackColor);

  //---------------------------------------------------------------------------
  //! @brief Propriété Protected
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, bool, Protected);

  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextAttributes, AnsiString, Name);

  //@}
};


//---------------------------------------------------------------------------
#endif

#undef In_TTextAttributesH
