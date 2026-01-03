//-----------------------------------------------------------------------------
//! @file TColorPdf.h
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

#ifdef In_TColorPdfH
#error "Inclusion circulaire de TColorPdf"
#endif // In_TColorPdfH

#define In_TColorPdfH

#ifndef TColorPdfH
#define TColorPdfH


//---------------------------------------------------------------------------
//! @class TColorPdf
//!
//! @brief Color PDF
//!
//! Cette classe (@c TColorPdf) représente une couleur PDF.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TColorPdf {
private:

protected:
  TColor FColorBack;
  TColor FColorFore;

  bool bChangeColorBack;
  bool bChangeColorFore;

  CharString GetStreamColor(TColor Color, const char * szCode);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TColorPdf(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TColorPdf(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  void Clear(void);
  CharString GetStream(bool bColorBack, bool bColorFore);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ColorBack
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorPdf, TColor, ColorBack);

  //---------------------------------------------------------------------------
  //! @brief Propriété ColorFore
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorPdf, TColor, ColorFore);

  //@}

};


#endif

#undef In_TColorPdfH
