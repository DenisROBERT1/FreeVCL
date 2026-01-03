//-----------------------------------------------------------------------------
//! @file TRect.cpp
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
// TRect
//---------------------------------------------------------------------------

TRect::TRect(void) {
  // Initialisations
  left = 0;
  top = 0;
  right = 0;
  bottom = 0;
}

TRect::TRect(int l, int t, int r, int b) {
  left = l;
  top = t;
  right = r;
  bottom = b;
}

TRect::TRect(const TPoint& Pt1, const TPoint& Pt2) {
  left = Pt1.x;
  top = Pt1.y;
  right = Pt2.x;
  bottom = Pt2.y;
}

TRect::TRect(RECT& r) {
  left = r.left;
  top = r.top;
  right = r.right;
  bottom = r.bottom;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Left
//---------------------------------------------------------------------------

int TRect::Get_Left(void) {
  return left;
}

bool TRect::Set_Left(int NewLeft) {
  if (left != NewLeft) {
    left = NewLeft;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Top
//---------------------------------------------------------------------------

int TRect::Get_Top(void) {
  return top;
}

bool TRect::Set_Top(int NewTop) {
  if (top != NewTop) {
    top = NewTop;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Right
//---------------------------------------------------------------------------

int TRect::Get_Right(void) {
  return right;
}

bool TRect::Set_Right(int NewRight) {
  if (right != NewRight) {
    right = NewRight;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Bottom
//---------------------------------------------------------------------------

int TRect::Get_Bottom(void) {
  return bottom;
}

bool TRect::Set_Bottom(int NewBottom) {
  if (bottom != NewBottom) {
    bottom = NewBottom;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TRect::Get_Width(void) {
  return right - left;
}

bool TRect::Set_Width(int NewWidth) {
  right = left + NewWidth;
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TRect::Get_Height(void) {
  return Bottom - top;
}

bool TRect::Set_Height(int NewHeight) {
  bottom = top + NewHeight;
  return true;
}


bool TRect::operator == (const TRect &Rect) {
  return left == Rect.left &&
         top == Rect.top &&
         right == Rect.right &&
         bottom == Rect.bottom;
}

bool TRect::operator != (const TRect &Rect) {
  return left != Rect.left ||
         top != Rect.top ||
         right != Rect.right ||
         bottom != Rect.bottom;
}

const TRect &TRect::operator = (const RECT &Rect) {
  left = Rect.left;
  top = Rect.top;
  right = Rect.right;
  bottom = Rect.bottom;

  return *this;
}

//---------------------------------------------------------------------------

