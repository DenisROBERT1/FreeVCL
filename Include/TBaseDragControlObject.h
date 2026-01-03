//-----------------------------------------------------------------------------
//! @file TBaseDragControlObject.h
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

#ifdef In_TBaseDragControlObjectH
#error "Inclusion circulaire de TBaseDragControlObject"
#endif // In_TBaseDragControlObjectH

#define In_TBaseDragControlObjectH

#ifndef TBaseDragControlObjectH
#define TBaseDragControlObjectH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TDragObjectH
#include "TDragObject.h"
#endif  // TDragObjectH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TControl;


//-----------------------------------------------------------------------------
//! @class TBaseDragControlObject
//!
//! @brief Définition de la classe TBaseDragControlObject
//!
//! Cette classe est la classe de base pour toutes les opérations de
//! glissé-déposé (drag and drop).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TBaseDragControlObject: public TObject {

protected:
  TControl *FControl;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TBaseDragControlObject(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TBaseDragControlObject(void);

  //@}


  //! @name Méthodes
  //@{

  virtual void FASTCALL Assign(TPersistent* Source);
  virtual void FASTCALL EndDrag(TObject* Target, int X, int Y);
  virtual void FASTCALL Finished(TObject* Target, int X, int Y, bool Accepted);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Control
	//!
	//! Cette propriété représente le contrôle en train d'être déplacé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBaseDragControlObject, TControl *, Control);


  //@}

};


#else  // TBaseDragControlObjectH

class TBaseDragControlObject;

#endif  // TBaseDragControlObjectH

#undef In_TBaseDragControlObjectH
