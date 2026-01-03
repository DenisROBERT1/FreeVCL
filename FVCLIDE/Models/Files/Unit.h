//-----------------------------------------------------------------------------
//! @file Unit.h
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

#ifdef In_{{{FILE_NAME}}}H
#error "Inclusion circulaire de {{{FILE_NAME}}}"
#endif // In_{{{FILE_NAME}}}H

#define In_{{{FILE_NAME}}}H

#ifndef {{{FILE_NAME}}}H
#define {{{FILE_NAME}}}H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <FreeVcl.h>

//---------------------------------------------------------------------------
class {{{FILE_NAME}}}: public TForm {
private:
protected:
public:
  FVCL_BEGIN_COMPONENTS
  TButton *Button_Ok;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_OkClick(TObject *Sender);

	virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1({{{FILE_NAME}}}, TNotifyEvent, OnClick, Button_OkClick, TObject*);
  }

  FVCL_END_EVENTS

  {{{FILE_NAME}}}(TComponent *Owner, LPCTSTR szName);
  virtual ~{{{FILE_NAME}}}(void);
};

//---------------------------------------------------------------------------

#endif // {{{FILE_NAME}}}H

#undef In_{{{FILE_NAME}}}H
