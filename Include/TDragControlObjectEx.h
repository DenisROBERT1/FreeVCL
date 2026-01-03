//-----------------------------------------------------------------------------
//! @file TDragControlObjectEx.h
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

#ifdef In_TDragControlObjectExH
#error "Inclusion circulaire de TDragControlObjectEx"
#endif // In_TDragControlObjectExH

#define In_TDragControlObjectExH

#ifndef TDragControlObjectExH
#define TDragControlObjectExH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TDragControlObject.h"


//-----------------------------------------------------------------------------
//! @class TDragControlObjectEx
//!
//! @brief Définition de la classe TDragControlObjectEx
//!
//! Cette classe permet de gérer un glissé-déposé (drag and drop) étendu.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDragControlObjectEx: public TObject {

protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDragControlObjectEx(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDragControlObjectEx(void);

  //@}


  //! @name Méthodes
  //@{

  virtual void FASTCALL BeforeDestruction(void);

  //@}

  //! @name Propriétés
  //@{

  //@}

};


#else  // TDragControlObjectExH

class TDragControlObjectEx;

#endif  // TDragControlObjectExH

#undef In_TDragControlObjectExH
