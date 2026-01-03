//-----------------------------------------------------------------------------
//! @file TPrinter.cpp
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
// Constructeur
//---------------------------------------------------------------------------

TPrinter::TPrinter(void): TObject() {
  FHandle = NULL;
  memset(&DocInfo, 0, sizeof(DOCINFO));
  DocInfo.cbSize = sizeof(DOCINFO);
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TPrinter::~TPrinter(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HDC TPrinter::Get_Handle(void) {
  return FHandle;
}

bool TPrinter::Set_Handle(HDC NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PageWidth
//---------------------------------------------------------------------------

int TPrinter::Get_PageWidth(void) {
  return GetDeviceCaps(FHandle, LOGPIXELSX);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PageHeight
//---------------------------------------------------------------------------

int TPrinter::Get_PageHeight(void) {
  return GetDeviceCaps(FHandle, LOGPIXELSY);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Title
//---------------------------------------------------------------------------

AnsiString TPrinter::Get_Title(void) {
  return FTitle;
}

bool TPrinter::Set_Title(AnsiString NewTitle) {
  if (FTitle != NewTitle) {
    FTitle = NewTitle;
  }
  return true;
}


//---------------------------------------------------------------------------
void TPrinter::BeginDoc(void) {

  StartDoc(FHandle, &DocInfo);
}

//---------------------------------------------------------------------------
void TPrinter::EndDoc(void) {

  ::EndDoc(FHandle);
  FHandle = NULL;
}

//---------------------------------------------------------------------------
TPrinter * Printer(void) {
  static TPrinter * PrinterSingle;


  if (PrinterSingle == NULL) {
    PrinterSingle = new TPrinter();
  }

  return PrinterSingle;
}

//---------------------------------------------------------------------------
