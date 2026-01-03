//-----------------------------------------------------------------------------
//! @file TDragObject.h
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

#ifdef In_TDragObjectH
#error "Inclusion circulaire de TDragObject"
#endif // In_TDragObjectH

#define In_TDragObjectH

#ifndef TDragObjectH
#define TDragObjectH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TObject.h"


//-----------------------------------------------------------------------------
//! @class TDragObject
//!
//! @brief Définition de la classe TDragObject
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDragObject: public TObject {

protected:
  bool FCancelling;
  HWND FDragHandle;
  TPoint FDragPos;
  void * FDragTarget;
  TPoint FDragTargetPos;
  bool FDropped;
  double FMouseDeltaX;
  double FMouseDeltaY;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDragObject(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDragObject(void);

  //@}


  //! @name Méthodes
  //@{

  virtual void FASTCALL Assign(TPersistent* Source);
  virtual void FASTCALL Finished(TObject* Target, int X, int Y, bool Accepted);
  virtual TCursor FASTCALL GetDragCursor(bool Accepted, int X, int Y);
  virtual AnsiString FASTCALL GetName();
  virtual void FASTCALL HideDragImage(void);
  virtual int FASTCALL Instance(void);
  virtual void FASTCALL ShowDragImage(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Cancelling
	//!
	//! Cette propriété est à @b true si le drag and drop est annulé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDragObject, bool, Cancelling);


  //---------------------------------------------------------------------------
  //! @brief Propriété DragHandle
	//!
	//! Cette propriété définit le handle de l'objet drag and drop.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDragObject, HWND, DragHandle);


  //---------------------------------------------------------------------------
  //! @brief Propriété DragPos
	//!
	//! Cette propriété définit la position de la souris pendant l'opération de
	//! drag and drop.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDragObject, TPoint, DragPos);


  //---------------------------------------------------------------------------
  //! @brief Propriété DragTarget
	//!
	//! Cette propriété _INSERER_LA_DESCRIPTION_
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDragObject, void *, DragTarget);


  //---------------------------------------------------------------------------
  //! @brief Propriété DragTargetPos
	//!
	//! Cette propriété _INSERER_LA_DESCRIPTION_
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDragObject, TPoint, DragTargetPos);


  //---------------------------------------------------------------------------
  //! @brief Propriété Dropped
	//!
	//! Cette propriété _INSERER_LA_DESCRIPTION_
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TDragObject, bool, Dropped);


  //---------------------------------------------------------------------------
  //! @brief Propriété MouseDeltaX
	//!
	//! Cette propriété _INSERER_LA_DESCRIPTION_
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TDragObject, double, MouseDeltaX);


  //---------------------------------------------------------------------------
  //! @brief Propriété MouseDeltaY
	//!
	//! Cette propriété _INSERER_LA_DESCRIPTION_
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TDragObject, double, MouseDeltaY);


  //@}

};


#else  // TDragObjectH

class TDragObject;

#endif  // TDragObjectH

#undef In_TDragObjectH
