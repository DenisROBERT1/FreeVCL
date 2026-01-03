//-----------------------------------------------------------------------------
//! @file TPrinter.h
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

#ifdef In_TPrinterH
#error "Inclusion circulaire de TPrinter"
#endif // In_TPrinterH

#define In_TPrinterH

#ifndef TPrinterH
#define TPrinterH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TObject.h"


//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

#define CONVERT_TWIPS_X(hdc, cxDeviceUnits) ((int) ((double) (cxDeviceUnits) * 1440 / GetDeviceCaps(hdc, LOGPIXELSX)))
#define CONVERT_TWIPS_Y(hdc, cyDeviceUnits) ((int) ((double) (cyDeviceUnits) * 1440 / GetDeviceCaps(hdc, LOGPIXELSY)))


//-----------------------------------------------------------------------------
//! @class TPrinter
//!
//! @brief Définition de la classe TPrinter: périphérique d'impression.
//!
//! Cette classe encapsule un périphérique d'impression, soit physique
//! (imprimante), soit virtuel (fichier d'impression).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPrinter: public TObject {
private:

protected:
  HDC FHandle;
  AnsiString FTitle;
  DOCINFO DocInfo;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPrinter(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPrinter(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  void BeginDoc(void);

  void EndDoc(void);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système
	//!
	//! Cette propriété permet de lire le handle de dessin associée à l'objet
	//! @b TPrinter. En lecture, ce handle permet de dessiner sur la page.
	//! N'utilisez pas cette propriété en écriture, seul le système est autorisé
	//! à le faire, avec un HDC venant d'un @b TPrintDialog.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrinter, HDC, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété PageWidth
	//!
	//! Cette propriété permet de lire la largeur de la page en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TPrinter, int, PageWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété PageHeight
	//!
	//! Cette propriété permet de lire la hauteur de la page en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TPrinter, int, PageHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété Title
	//!
	//! Cette propriété permet de définir le titre de la page.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrinter, AnsiString, Title);

  //@}
};


TPrinter * Printer();

//---------------------------------------------------------------------------
#endif

#undef In_TPrinterH
