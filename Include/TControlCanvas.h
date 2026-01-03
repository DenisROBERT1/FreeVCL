//-----------------------------------------------------------------------------
//! @file TControlCanvas.h
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

#ifdef In_TControlCanvasH
#error "Inclusion circulaire de TControlCanvas"
#endif // In_TControlCanvasH

#define In_TControlCanvasH

#ifndef TControlCanvasH
#define TControlCanvasH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TCanvas.h"


//-----------------------------------------------------------------------------
//! @class TControlCanvas
//!
//! @brief Définition de la classe TControlCanvas
//!
//! Cette classe représente une zone de dessin associée à un contrôle.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TControlCanvas: public TCanvas {
protected:
  TControl *FControl;
	bool bReleaseDC;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TControlCanvas(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TControlCanvas(void);

  //@}


  //! @name Méthodes
  //@{

	//---------------------------------------------------------------------------
	//! @brief Accesseurs de la propriété Handle
	//---------------------------------------------------------------------------

	virtual HDC Get_Handle(void);

	virtual bool Set_Handle(HDC NewHandle);

	//---------------------------------------------------------------------------
	//! @brief Libère le HDC utilisé par l'objet canevas du contrôle.
	//---------------------------------------------------------------------------
	
	void FASTCALL FreeHandle(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Control
	//!
	//! Cette propriété définit le contrôle associé au canevas.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlCanvas, TControl *, Control);

  //@}

};


#else  // TControlCanvasH

class TControlCanvas;

#endif  // TControlCanvasH


#undef In_TControlCanvasH
