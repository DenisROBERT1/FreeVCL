//-----------------------------------------------------------------------------
//! @file FreeVcl.cpp
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
#include <assert.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
HINSTANCE HInstance;
extern TApplication *Application;

TMouse StaticMouse;
TMouse *Mouse = &StaticMouse;
TScreen StaticScreen;
TScreen *Screen = &StaticScreen;
TClipboard StaticClipboard;
TClipboard *Clipboard = &StaticClipboard;
TDragObject StaticDragObject;
TDragObject *FVCL_DragObject = &StaticDragObject;

//---------------------------------------------------------------------------

#ifdef __GNUG__

#ifndef NDEBUG

void fvcl_assert(const char* szError, const char* szFile, int Line) {
  int Rep;
  char *szMsg = (char *) malloc(strlen(szError) + strlen(szFile) + 55);
  wsprintf(szMsg, "Assertion failed:\n%s\nin file %s, line %i", szError, szFile, Line);
  Rep = MessageBox(NULL, szMsg, "Assert", MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
  free(szMsg);
  if (Rep != IDIGNORE) _assert(szError, szFile, Line);
}

#endif	/* NDEBUG */

#endif	/* __GNUG__ */

//---------------------------------------------------------------------------

