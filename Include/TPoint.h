//-----------------------------------------------------------------------------
//! @file TPoint.h
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

#ifdef In_TPointH
#error "Inclusion circulaire de TPoint"
#endif // In_TPointH

#define In_TPointH

#ifndef TPointH
#define TPointH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//! @class TPoint
//!
//! @brief Définition de la classe TPoint
//!
//! Cette classe représente un point d'un plan, défini par ses coordonnées X et
//! Y.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPoint {
public:

  int  x;
  int  y;

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TPoint(void);
  TPoint(int _x, int _y);
  TPoint(POINT& Pt);

  //@}


  //---------------------------------------------------------------------------
  //! @brief Opérateurs
  //---------------------------------------------------------------------------

  operator POINT(void) const;

  const TPoint & operator = (const POINT &Pt);

  bool operator == (const TPoint &Point);

  bool operator != (const TPoint &Point);

};


#else  // TPointH

class TPoint;

#endif  // TPointH


#undef In_TPointH
