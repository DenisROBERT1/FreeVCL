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

#ifdef BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */


typedef BOOL (WINAPI * TRANSLATE_CHOOSELANGUAGE) (HWND hWndParent);
typedef LPSTR (WINAPI * TRANSLATE_GETLANGUAGE) ();
typedef BOOL (WINAPI * TRANSLATE_SETLANGUAGE) (LPSTR szLanguage);
typedef LPSTR (WINAPI * TRANSLATE_TRANSLATE) (LPSTR szLigne);
typedef BOOL (WINAPI * TRANSLATE_ADD) (LPSTR szSrc, LPSTR szDest);

EXTERN_C DLLIMPORT BOOL WINAPI Translate_ChooseLanguage(HWND hWndParent);
EXTERN_C DLLIMPORT LPSTR WINAPI Translate_GetLanguage();
EXTERN_C DLLIMPORT BOOL WINAPI Translate_SetLanguage(LPSTR szLanguage);
EXTERN_C DLLIMPORT LPSTR WINAPI Translate_Translate(LPSTR szLigne);
EXTERN_C DLLIMPORT BOOL WINAPI Translate_Add(LPSTR szSrc, LPSTR szDest);

BOOL LectureConfig(LPSTR szLanguage);
BOOL EcritureConfig(LPSTR szLanguage);

#endif /* Translate_H */
