//-----------------------------------------------------------------------------
//! @file TMouse.cpp
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
// TMouse
//---------------------------------------------------------------------------

TMouse::TMouse(void): TObject() {
  // Initialisations
  FDragImmediate = false;
	FDragThreshold = 5;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Capture
//---------------------------------------------------------------------------
HWND TMouse::Get_Capture(void) {
  return GetCapture();
}

bool TMouse::Set_Capture(HWND NewCapture) {
  if (NewCapture) SetCapture(NewCapture);
  else ReleaseCapture();

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CursorPos
//---------------------------------------------------------------------------
TPoint TMouse::Get_CursorPos(void) {
  POINT Pos;


  GetCursorPos(&Pos);

  return TPoint(Pos);
}

bool TMouse::Set_CursorPos(TPoint NewCursorPos) {
  SetCursorPos(NewCursorPos.x, NewCursorPos.y);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WheelScrollLines
//---------------------------------------------------------------------------

int TMouse::Get_WheelScrollLines(void) {
	int WheelScrollLines;

	if (SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &WheelScrollLines, 0)) {
		return WheelScrollLines;
	}

	return 3;  // Default value (MSDN)
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DragImmediate
//---------------------------------------------------------------------------

bool TMouse::Get_DragImmediate() {
  return FDragImmediate;
}

bool TMouse::Set_DragImmediate(bool NewDragImmediate) {
  if (FDragImmediate != NewDragImmediate) {
    FDragImmediate = NewDragImmediate;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DragThreshold
//---------------------------------------------------------------------------

int TMouse::Get_DragThreshold() {
  return FDragThreshold;
}

bool TMouse::Set_DragThreshold(int NewDragThreshold) {
  if (FDragThreshold != NewDragThreshold) {
    FDragThreshold = NewDragThreshold;
  }
  return true;
}

//---------------------------------------------------------------------------

