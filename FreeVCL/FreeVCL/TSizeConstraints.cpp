//-----------------------------------------------------------------------------
//! @file TSizeConstraints.cpp
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
// TSizeConstraints
//---------------------------------------------------------------------------

TSizeConstraints::TSizeConstraints(TControl* Control): TPersistent() {
  // Initialisations
  MinWidth = 0;
  MinHeight = 0;
  MaxWidth = 0;
  MaxHeight = 0;
  FControl = Control;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété MinWidth
//---------------------------------------------------------------------------

int TSizeConstraints::Get_MinWidth(void) {
  return FMinWidth;
}

bool TSizeConstraints::Set_MinWidth(int NewMinWidth) {

  if (FMinWidth != NewMinWidth) {
    FMinWidth = NewMinWidth;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MinHeight
//---------------------------------------------------------------------------

int TSizeConstraints::Get_MinHeight(void) {
  return FMinHeight;
}

bool TSizeConstraints::Set_MinHeight(int NewMinHeight) {

  if (FMinHeight != NewMinHeight) {
    FMinHeight = NewMinHeight;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MaxWidth
//---------------------------------------------------------------------------

int TSizeConstraints::Get_MaxWidth(void) {
  return FMaxWidth;
}

bool TSizeConstraints::Set_MaxWidth(int NewMaxWidth) {

  if (FMaxWidth != NewMaxWidth) {
    FMaxWidth = NewMaxWidth;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MaxHeight
//---------------------------------------------------------------------------

int TSizeConstraints::Get_MaxHeight(void) {
  return FMaxHeight;
}

bool TSizeConstraints::Set_MaxHeight(int NewMaxHeight) {

  if (FMaxHeight != NewMaxHeight) {
    FMaxHeight = NewMaxHeight;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Control
//---------------------------------------------------------------------------

TControl * TSizeConstraints::Get_Control(void) {
  return FControl;
}

//---------------------------------------------------------------------------

