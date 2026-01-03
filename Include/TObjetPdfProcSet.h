//-----------------------------------------------------------------------------
//! @file TObjetPdfProcSet.h
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

#ifdef In_TObjetPdfProcSetH
#error "Inclusion circulaire de TObjetPdfProcSet"
#endif // In_TObjetPdfProcSetH

#define In_TObjetPdfProcSetH

#ifndef TObjetPdfProcSetH
#define TObjetPdfProcSetH

#include "TObjetPdf.h"


//---------------------------------------------------------------------------
//! @class TObjetPdfProcSet
//!
//! @brief Objet Proset
//!
//! Cette classe (@c TObjetPdfProcSet) représente un objet PDF "ProcSet".
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TObjetPdfProcSet: public TObjetPdf {
private:

protected:
  std::vector<TObjetPdf *> ProcSet;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TObjetPdfProcSet(void *Owner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TObjetPdfProcSet(void);

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


};


#endif

#undef In_TObjetPdfProcSetH
