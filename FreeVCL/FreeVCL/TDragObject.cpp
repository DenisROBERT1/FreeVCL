//-----------------------------------------------------------------------------
//! @file TDragObject.cpp
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
// TDragObject
//---------------------------------------------------------------------------

TDragObject::TDragObject(void): TObject() {
  // Initialisations
}

TDragObject::~TDragObject(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Cancelling
//---------------------------------------------------------------------------

bool TDragObject::Get_Cancelling(void) {
  return FCancelling;
}

bool TDragObject::Set_Cancelling(bool NewCancelling) {
  if (FCancelling != NewCancelling) {
    FCancelling = NewCancelling;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DragHandle
//---------------------------------------------------------------------------

HWND TDragObject::Get_DragHandle(void) {
  return FDragHandle;
}

bool TDragObject::Set_DragHandle(HWND NewDragHandle) {
  if (FDragHandle != NewDragHandle) {
    FDragHandle = NewDragHandle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DragPos
//---------------------------------------------------------------------------

TPoint TDragObject::Get_DragPos(void) {
  return FDragPos;
}

bool TDragObject::Set_DragPos(TPoint NewDragPos) {
  if (FDragPos != NewDragPos) {
    FDragPos = NewDragPos;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DragTarget
//---------------------------------------------------------------------------

void * TDragObject::Get_DragTarget(void) {
  return FDragTarget;
}

bool TDragObject::Set_DragTarget(void * NewDragTarget) {
  if (FDragTarget != NewDragTarget) {
    FDragTarget = NewDragTarget;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DragTargetPos
//---------------------------------------------------------------------------

TPoint TDragObject::Get_DragTargetPos(void) {
  return FDragTargetPos;
}

bool TDragObject::Set_DragTargetPos(TPoint NewDragTargetPos) {
  if (FDragTargetPos != NewDragTargetPos) {
    FDragTargetPos = NewDragTargetPos;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Dropped
//---------------------------------------------------------------------------

bool TDragObject::Get_Dropped(void) {
  return FDropped;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MouseDeltaX
//---------------------------------------------------------------------------

double TDragObject::Get_MouseDeltaX(void) {
  return FMouseDeltaX;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MouseDeltaY
//---------------------------------------------------------------------------

double TDragObject::Get_MouseDeltaY(void) {
  return FMouseDeltaY;
}

//---------------------------------------------------------------------------
void FASTCALL TDragObject::Assign(TPersistent* Source) {
}

//---------------------------------------------------------------------------
void FASTCALL TDragObject::Finished(TObject* Target, int X, int Y, bool Accepted) {
}

//---------------------------------------------------------------------------
TCursor FASTCALL TDragObject::GetDragCursor(bool Accepted, int X, int Y) {
  return (TCursor) 0;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL TDragObject::GetName(void) {
  return _T("");
}

//---------------------------------------------------------------------------
void FASTCALL TDragObject::HideDragImage(void) {
}

//---------------------------------------------------------------------------
int FASTCALL TDragObject::Instance(void) {
  return 0;
}

//---------------------------------------------------------------------------
void FASTCALL TDragObject::ShowDragImage(void) {
}

//---------------------------------------------------------------------------

