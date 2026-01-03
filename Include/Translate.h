//-----------------------------------------------------------------------------
//! @file Translate.h
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
#ifndef Translate_H
#define Translate_H

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */


typedef BOOL (WINAPI *TRANSLATE_CHOOSELANGUAGE) (HWND hWndParent);
typedef LPCSTR (WINAPI *TRANSLATE_GETLANGUAGE) ();
typedef BOOL (WINAPI *TRANSLATE_SETLANGUAGE) (LPCSTR szLanguage);
typedef LPCSTR (WINAPI *TRANSLATE_TRANSLATE) (LPCSTR szLigne);
typedef BOOL (WINAPI *TRANSLATE_ADD) (LPCSTR szSrc, LPCSTR szDest);

DLLIMPORT BOOL WINAPI Translate_ChooseLanguage(HWND hWndParent);
DLLIMPORT LPCSTR WINAPI Translate_GetLanguage();
DLLIMPORT BOOL WINAPI Translate_SetLanguage(LPCSTR szLanguage);
DLLIMPORT LPCSTR WINAPI Translate_Translate(LPCSTR szLigne);
DLLIMPORT BOOL WINAPI Translate_Add(LPCSTR szSrc, LPCSTR szDest);

#endif /* Translate_H */
