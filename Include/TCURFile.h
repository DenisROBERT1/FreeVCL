//-----------------------------------------------------------------------------
//! @file TCURFile.h
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

#ifdef In_TCURFileH
#error "Inclusion circulaire de TCURFile"
#endif // In_TCURFileH

#define In_TCURFileH

#ifndef TCURFileH
#define TCURFileH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH

#ifndef TCURICOFileH
#include "TCURICOFile.h"
#endif  // TCURICOFileH


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//! @class TCURFile
//!
//! @brief Définition de la classe TCURFile
//!
//! Cette classe encapsule un fichier d'image CUR. Elle permet la lecture et
//! l'écriture dans un fichier.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCURFile: public TCURICOFile {
private:
protected:
  int FXHotSpot;
  int FYHotSpot;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCURFile(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCURFile(void);

  //@}


  //! @name Méthodes
  //@{

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété XHotSpot Position en X du hotspot
	//!
	//! Cette propriété définit la position X en pixels du hotspot
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCURFile, int, XHotSpot);

  //---------------------------------------------------------------------------
  //! @brief Propriété YHotSpot Position en Y du hotspot
	//!
	//! Cette propriété définit la position Y en pixels du hotspot
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCURFile, int, YHotSpot);

  //@}

};



#else  // TCURFileH

class TCURFile;

#endif  // TCURFileH


#undef In_TCURFileH
