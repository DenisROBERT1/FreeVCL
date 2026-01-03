//-----------------------------------------------------------------------------
//! @file Exceptions.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// Exception
//---------------------------------------------------------------------------

Exception::Exception(const AnsiString Msg) {
  HelpContext = 0;
}

Exception::Exception(const AnsiString Msg, const TVarRec * Args, int Args_Size) {
  HelpContext = 0;
}

Exception::Exception(int Ident) {
  HelpContext = 0;
}

Exception::Exception(int Ident, const TVarRec * Args, int Args_Size) {
  HelpContext = 0;
}

Exception::Exception(const AnsiString Msg, int AHelpContext) {
  HelpContext = AHelpContext;
}

Exception::Exception(const AnsiString Msg, const TVarRec * Args, int Args_Size, int AHelpContext) {
  HelpContext = AHelpContext;
}

Exception::Exception(int Ident, int AHelpContext) {
  HelpContext = AHelpContext;
}

Exception::Exception(int Ident, const TVarRec * Args, int Args_Size, int AHelpContext) {
  HelpContext = AHelpContext;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Message
//---------------------------------------------------------------------------

AnsiString Exception::Get_Message(void) {
  return FMessage;
}

bool Exception::Set_Message(AnsiString NewMessage) {
  if (FMessage != NewMessage) {
    FMessage = NewMessage;
  }
  return true;
}

//---------------------------------------------------------------------------
const char* Exception::what () const throw() {

  CharString csWhat = asWhat;

  return csWhat;  // $$$ Pointeur sur variable locale !

}

//---------------------------------------------------------------------------
// EConvertError
//---------------------------------------------------------------------------

EConvertError::EConvertError(void):
		Exception(_T("Impossible d'assigner cet objet")) {
}

//---------------------------------------------------------------------------
