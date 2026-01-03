//-----------------------------------------------------------------------------
//! @file TBaseDragControlObject.cpp
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
// TBaseDragControlObject
//---------------------------------------------------------------------------

TBaseDragControlObject::TBaseDragControlObject(void): TObject() {
  // Initialisations
}

TBaseDragControlObject::~TBaseDragControlObject(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Control
//---------------------------------------------------------------------------

TControl *TBaseDragControlObject::Get_Control() {
  return FControl;
}

bool TBaseDragControlObject::Set_Control(TControl *NewControl) {
  if (FControl != NewControl) {
    FControl = NewControl;
  }
  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TBaseDragControlObject::Assign(TPersistent* Source) {
}

//---------------------------------------------------------------------------
void FASTCALL TBaseDragControlObject::EndDrag(TObject* Target, int X, int Y) {
}

//---------------------------------------------------------------------------
void FASTCALL TBaseDragControlObject::Finished(TObject* Target, int X, int Y, bool Accepted) {
}

//---------------------------------------------------------------------------

