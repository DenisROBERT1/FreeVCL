//-----------------------------------------------------------------------------
//! @file TForm_Principale.h
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

#ifndef FormPrincipaleH
#define FormPrincipaleH
//---------------------------------------------------------------------------
class TForm_Principale : public TForm
{
private:
public:
  FVCL_BEGIN_COMPONENTS
  THotKey *HotKey1;
  TMonthCalendar *MonthCalendar1;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  FVCL_END_EVENTS

  TForm_Principale(HWND hWndParent, LPSTR szName);
  ~TForm_Principale(void);
};
//---------------------------------------------------------------------------
#endif
