//-----------------------------------------------------------------------------
//! @file TControlCanvas.cpp
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
// TControlCanvas
//---------------------------------------------------------------------------

TControlCanvas::TControlCanvas(void): TCanvas() {
  // Initialisations
  FControl = NULL;
	bReleaseDC = false;
}

TControlCanvas::~TControlCanvas(void) {
	if (bReleaseDC) ReleaseDC(FControl->Handle, FHandle);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HDC TControlCanvas::Get_Handle(void) {
	if (FControl && FHandle == NULL) {
		FHandle = GetDC(FControl->Handle);
		bReleaseDC = true;
	}
  return FHandle;
}

bool TControlCanvas::Set_Handle(HDC NewHandle) {
	FreeHandle();
	TCanvas::Set_Handle(NewHandle);
	bReleaseDC = false;

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Control
//---------------------------------------------------------------------------

TControl *TControlCanvas::Get_Control(void) {
  return FControl;
}

bool TControlCanvas::Set_Control(TControl *NewControl) {
  if (FControl != NewControl) {
    FControl = NewControl;
  }
  return true;
}

//---------------------------------------------------------------------------
// Libère le HDC utilisé par l'objet canevas du contrôle.
//---------------------------------------------------------------------------

void FASTCALL TControlCanvas::FreeHandle(void) {
	if (bReleaseDC) ReleaseDC(FControl->Handle, FHandle);
	FHandle = NULL;
}

//---------------------------------------------------------------------------
