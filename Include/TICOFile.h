//-----------------------------------------------------------------------------
//! @file TICOFile.h
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

#ifdef In_TICOFileH
#error "Inclusion circulaire de TICOFile"
#endif // In_TICOFileH

#define In_TICOFileH

#ifndef TICOFileH
#define TICOFileH


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
//! @class TICOFile
//!
//! @brief Définition de la classe TICOFile
//!
//! Cette classe encapsule un fichier d'image ICO. Elle permet la lecture et
//! l'écriture dans un fichier.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TICOFile: public TCURICOFile {
private:
protected:
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TICOFile(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TICOFile(void);

  //@}


  //! @name Méthodes
  //@{

  //@}

  //! @name Propriétés
  //@{

  //@}

};



#else  // TICOFileH

class TICOFile;

#endif  // TICOFileH


#undef In_TICOFileH
