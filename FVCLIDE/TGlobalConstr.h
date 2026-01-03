//-----------------------------------------------------------------------------
//! @file TGlobalConstr.h
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


#ifndef TGlobalConstrH
#define TGlobalConstrH

//---------------------------------------------------------------------------
// Structures, macros, enums
//---------------------------------------------------------------------------

enum ACTION {acNull, acMove,
             acSizeNW, acSizeN, acSizeNE, acSizeE, acSizeSE, acSizeS, acSizeSW, acSizeW,
             acAnchorsLeft, acAnchorsTop, acAnchorsRight, acAnchorsBottom};


//---------------------------------------------------------------------------
//! @class TGlobalConstr
//!
//! @brief Variables globales de construction
//!
//! Cette classe (@c TGlobalConstr) permet d'accéder aux variables globales de
//! construction
//---------------------------------------------------------------------------

class TGlobalConstr {
private:
  static bool FAffAncrages;
  static ACTION FAction;

protected:
public:

  //! @name Constructeur / destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
  TGlobalConstr();

  //@}


  //! @name Méthodes
  //@{

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété AffAncrages
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TGlobalConstr, bool, AffAncrages);


  //---------------------------------------------------------------------------
  //! @brief Propriété Action
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TGlobalConstr, ACTION, Action);

  //@}

};

//---------------------------------------------------------------------------
#endif
