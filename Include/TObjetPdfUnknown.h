//-----------------------------------------------------------------------------
//! @file TObjetPdfUnknown.h
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

#ifdef In_TObjetPdfUnknownH
#error "Inclusion circulaire de TObjetPdfUnknown"
#endif // In_TObjetPdfUnknownH

#define In_TObjetPdfUnknownH

#ifndef TObjetPdfUnknownH
#define TObjetPdfUnknownH

#include "TObjetPdf.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TObjetPdfOutlines;
class TObjetPdfPages;


//---------------------------------------------------------------------------
//! @class TObjetPdfUnknown
//!
//! @brief Objet Unknown
//!
//! Cette classe (@c TObjetPdfUnknown) est un objet PDF inconnu. Il sert à
//! lire et à réécrire un objet PDF sans perte d'information. La chaîne lue
//! est réécrite telle quelle sans modification.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TObjetPdfUnknown: public TObjetPdf {
private:

protected:
	CharString csObjDescription;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TObjetPdfUnknown(void *Owner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TObjetPdfUnknown(void);

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

  //---------------------------------------------------------------------------
  //@}


  //! @name Propriétés
  //@{

  //@}

};


#endif

#undef In_TObjetPdfUnknownH
