//-----------------------------------------------------------------------------
//! @file TObjetPdfPage.h
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

#ifdef In_TObjetPdfPageH
#error "Inclusion circulaire de TObjetPdfPage"
#endif // In_TObjetPdfPageH

#define In_TObjetPdfPageH

#ifndef TObjetPdfPageH
#define TObjetPdfPageH

#include "TObjetPdf.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TObjetPdfPages;
class TObjetPdfProcSet;
class TObjetPdfFont;


//---------------------------------------------------------------------------
//! @class TObjetPdfPage
//!
//! @brief Une page PDF
//!
//! Cette classe (@c TObjetPdfPage) représente une page PDF isolée.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TObjetPdfPage: public TObjetPdf {
private:

protected:
  TObjetPdfPages *FObjetPdfParent;
  TObjetPdfFont *FObjetPdfFont[14];
  TObjetPdfProcSet *FObjetPdfProcSet;
  std::vector<TObjetPdfContent *> Contents;
  int FWidth;
  int FHeight;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TObjetPdfPage(void *Owner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TObjetPdfPage(void);

  //@}


  //! @name Méthodes
  //@{

  void AddContent(TObjetPdfContent *ObjetPdfContent);

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
  //! @brief Propriété Width
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TObjetPdfPage, int, Width);


  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TObjetPdfPage, int, Height);


  //---------------------------------------------------------------------------
  //! @brief Propriété ObjetPdfParent
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TObjetPdfPage, TObjetPdfPages *, ObjetPdfParent);


  //---------------------------------------------------------------------------
  //! @brief Propriété ObjetPdfProcSet
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TObjetPdfPage, TObjetPdfProcSet *, ObjetPdfProcSet);


  //---------------------------------------------------------------------------
  //! @brief Propriété ObjetPdfFont: Fonts de la page
	//!
	//! Utilisez la propriété @b ObjetPdfFont en lecture pour connaître les fonts
	//! standard utilisées dans la page. L'indice doit être compris entre 1 et
	//! 14.@n
	//! En utilisation normale, il n'est pas nécessaire d'utiliser cette
	//! propriété en écriture. Elle est uniquement affectée en interne par
	//! l'objet @b TPDFFile lors de l'écriture du fichier PDF. L'objet
	//! @b TObjetPdfPage ne devient pas propriétaire de l'objet mais ne mémorise
	//! que l'adresse (car une même font peut être utilisée par plusieurs pages),
	//! c'est à dire que l'objet appelant (en principe uniquement @b TPDFFile)
	//! est responsable de sa destruction (après que @b TObjetPdfPage soit
	//! lui-même détruit pour éviter des ennuis).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TObjetPdfPage, TObjetPdfFont *, ObjetPdfFont);


  //@}

};


#endif

#undef In_TObjetPdfPageH
