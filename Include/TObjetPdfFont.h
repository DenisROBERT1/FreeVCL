//-----------------------------------------------------------------------------
//! @file TObjetPdfFont.h
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

#ifdef In_TObjetPdfFontH
#error "Inclusion circulaire de TObjetPdfFont"
#endif // In_TObjetPdfFontH

#define In_TObjetPdfFontH

#ifndef TObjetPdfFontH
#define TObjetPdfFontH

#include "TObjetPdf.h"


//---------------------------------------------------------------------------
//! @class TObjetPdfFont
//!
//! @brief Font PDF
//!
//! Cette classe (@c TObjetPdfFont) représente une font PDF.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TObjetPdfFont: public TObjetPdf {
private:

protected:
  int FTypeStandard;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TObjetPdfFont(void *Owner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TObjetPdfFont(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture de la description de l'objet.
  //!
  //! Cette méthode permet de lire la description de l'objet, telle qu'elle
	//! doit être enregistrée dans le fichier.
  //!
  //! @return @c CharString Description de l'objet.
  //---------------------------------------------------------------------------

  virtual CharString GetDescription(void);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement de la description de l'objet
  //!
  //! Cette fonction permet de mémoriser la description de l'objet depuis un
	//! fichier.
  //!
  //! @param[in]        csDescription Description de l'objet.
  //!
  //! @return @c bool True si Ok
  //---------------------------------------------------------------------------

  virtual bool SetDescription(CharString csDescription);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété TypeStandard
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TObjetPdfFont, int, TypeStandard);


  //@}

};


#endif

#undef In_TObjetPdfFontH
