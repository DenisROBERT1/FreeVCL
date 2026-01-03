//-----------------------------------------------------------------------------
//! @file TBrushPdf.h
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

#ifdef In_TBrushPdfH
#error "Inclusion circulaire de TBrushPdf"
#endif // In_TBrushPdfH

#define In_TBrushPdfH

#ifndef TBrushPdfH
#define TBrushPdfH

#include <TCustomBrush.h>


//---------------------------------------------------------------------------
//! @class TBrushPdf
//!
//! @brief Brush PDF
//!
//! Cette classe (@c TBrushPdf) représente un pinceau d'un Canvas PDF. Elle a
//! les mêmes méthodes que la classe TBrush d'un Canvas classique.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TBrushPdf: public TCustomBrush {
private:

protected:
  TBrushStyle FStyle;
  TColor FColor;
  TColorPdf *ColorPdf;

  bool bChangeStyle;

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

  TBrushPdf(TColorPdf *CommonColorPdf);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TBrushPdf(void);

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
  //! @brief Propriété Style
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBrushPdf, TBrushStyle, Style);


  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBrushPdf, TColor, Color);


  //@}

};


#endif

#undef In_TBrushPdfH
