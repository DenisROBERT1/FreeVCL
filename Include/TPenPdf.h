//-----------------------------------------------------------------------------
//! @file TPenPdf.h
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

#ifdef In_TPenPdfH
#error "Inclusion circulaire de TPenPdf"
#endif // In_TPenPdfH

#define In_TPenPdfH

#ifndef TPenPdfH
#define TPenPdfH

#include <TCustomPen.h>


//---------------------------------------------------------------------------
//! @class TPenPdf
//!
//! @brief Pen PDF
//!
//! Cette classe (@c TBrushPdf) représente un stylo d'un Canvas PDF. Elle a
//! les mêmes méthodes que la classe TPen d'un Canvas classique.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TPenPdf: public TCustomPen {
private:

protected:
  TPenStyle FStyle;
  int FWidth;
  TColor FColor;
  TColorPdf *ColorPdf;

  bool bChangeStyle;
  bool bChangeWidth;

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

  TPenPdf(TColorPdf *CommonColorPdf);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPenPdf(void);

  //@}


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

  DECLARE_PROPERTY(TPenPdf, TPenStyle, Style);


  //---------------------------------------------------------------------------
  //! @brief Propriété Width
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPenPdf, int, Width);


  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPenPdf, TColor, Color);


  //@}

};


#endif

#undef In_TPenPdfH
