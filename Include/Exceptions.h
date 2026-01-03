//-----------------------------------------------------------------------------
//! @file Exceptions.h
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

#ifdef In_ExceptionsH
#error "Inclusion circulaire de Exceptions"
#endif // In_ExceptionsH

#define In_ExceptionsH

#ifndef ExceptionsH
#define ExceptionsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <exception>
#include <TObject.h>

#ifndef PropertyH
#include "Property.h"
#endif  // PropertyH


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                                 Exceptions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! @class Exception
//!
//! @brief Exception est la classe de base de toutes les exceptions d'exécution.
//!
//! Cette classe est la classe de base de toutes les exceptions d'exécution
//! (c'est à dire non système, mais déclenchées par programme). Vous pouvez
//! créer des exceptions héritées de cette classe pour vos propres besoins.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class Exception: public std::exception, public TObject {
protected:
  int HelpContext;
  AnsiString asWhat;
  AnsiString FMessage;
public:
  Exception(const AnsiString Msg);
  Exception(const AnsiString Msg, const TVarRec * Args, int Args_Size);
  Exception(int Ident);
  Exception(int Ident, const TVarRec * Args, int Args_Size);

  Exception(const AnsiString Msg, int AHelpContext);
  Exception(const AnsiString Msg, const TVarRec * Args, int Args_Size, int AHelpContext);
  Exception(int Ident, int AHelpContext);
  Exception(int Ident, const TVarRec * Args, int Args_Size, int AHelpContext);

  ~Exception(void) throw() {}

  virtual const char* what () const throw();

  /* PResStringRec ? Kekséksa ?
  Exception(PResStringRec ResStringRec);
  Exception(PResStringRec ResStringRec, const TVarRec * Args, int Args_Size);
  Exception(PResStringRec ResStringRec, int AHelpContext);
  Exception(PResStringRec ResStringRec, const TVarRec * Args, int Args_Size, int AHelpContext);
  */

	//! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Message: Message affiché lorsque l'exception est déclenchée.
	//!
	//! Cette propriété contient la chaîne à afficher dans la boîte de dialogue
	//! lorsque l'exception est déclenchée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(Exception, AnsiString, Message);

  //@}

};

class EConvertError: public Exception {
public:
  EConvertError(void);
};

#endif  // ExceptionsH

#undef In_ExceptionsH
