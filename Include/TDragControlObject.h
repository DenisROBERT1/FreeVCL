//-----------------------------------------------------------------------------
//! @file TDragControlObject.h
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

#ifdef In_TDragControlObjectH
#error "Inclusion circulaire de TDragControlObject"
#endif // In_TDragControlObjectH

#define In_TDragControlObjectH

#ifndef TDragControlObjectH
#define TDragControlObjectH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TBaseDragControlObject.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TDragImageList;


//-----------------------------------------------------------------------------
//! @class TDragControlObject
//!
//! @brief Définition de la classe TDragControlObject
//!
//! Cette classe permet de gérer un glissé-déposé (drag and drop) standard.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDragControlObject: public TObject {

protected:
  TControl *FControl;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDragControlObject(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDragControlObject(void);

  //@}


  //! @name Méthodes
  //@{

  virtual TCursor FASTCALL GetDragCursor(bool Accepted, int X, int Y);
  virtual TDragImageList* FASTCALL GetDragImages(void);
  virtual void FASTCALL HideDragImage(void);
  virtual void FASTCALL ShowDragImage(void);

  //@}

  //! @name Propriétés
  //@{

  //@}

};


#else  // TDragControlObjectH

class TDragControlObject;

#endif  // TDragControlObjectH

#undef In_TDragControlObjectH
