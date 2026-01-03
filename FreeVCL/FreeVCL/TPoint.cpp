//-----------------------------------------------------------------------------
//! @file TPoint.cpp
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

#include <FreeVcl.h>


//---------------------------------------------------------------------------
// TPoint
//---------------------------------------------------------------------------

TPoint::TPoint(void) {
  // Initialisations
  x = 0;
  y = 0;
}

TPoint::TPoint(int New_x, int New_y) {
  // Initialisations
  x = New_x;
  y = New_y;
}
TPoint::TPoint(POINT& Pt) {
  // Initialisations
  x = Pt.x;
  y = Pt.y;
}

TPoint::operator POINT() const {
  POINT Pt;
  Pt.x = x;
  Pt.y = y;
  return Pt;
}

const TPoint & TPoint::operator = (const POINT &Pt) {
  x = Pt.x;
  y = Pt.y;
  return *this;
}

bool TPoint::operator == (const TPoint &Point) {
  return x == Point.x &&
         y == Point.y;
}

bool TPoint::operator != (const TPoint &Point) {
  return x != Point.x ||
         y != Point.y;
}

//---------------------------------------------------------------------------

