//-----------------------------------------------------------------------------
//! @file FreeVcl.h
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

#ifdef In_FreeVclH
#error "Inclusion circulaire de FreeVcl"
#endif // In_FreeVclH

#define In_FreeVclH

#ifndef FreeVclH
#define FreeVclH


//---------------------------------------------------------------------------
// Includes système
//---------------------------------------------------------------------------

#ifdef LINUX

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <limits.h>
#include <string.h>
#include <string>
#include <vector>

#else  // LINUX

// Version minimum de windows = XP
#if(_WIN32_WINNT < 0x0501)
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif // _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif // _WIN32_WINNT < 0x0501

#ifdef WINVER
#undef WINVER
#define WINVER _WIN32_WINNT
#endif // WINVER

// Version minimum de IE = 6
#if (_WIN32_IE < 0x0600)
#ifdef _WIN32_IE
#undef _WIN32_IE
#endif // _WIN32_IE
#define _WIN32_IE 0x0600
#endif // _WIN32_IE < 0x0600

// Prevent inclusion of winsock.h in windows.h
#define _WINSOCKAPI_

#include <limits.h>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>
#include <shlobj.h>

#endif  // LINUX


//---------------------------------------------------------------------------
// Macro pour compatibilité entre compilateurs
//---------------------------------------------------------------------------

// Définition de la macro FVCL_ASSERT (assert semble ne pas marcher)

#ifdef __GNUG__

#ifdef NDEBUG

#define assert(x)	((void)0)

#else // NDEBUG

void fvcl_assert(const char* szError, const char* szFile, int Line);

#define FVCL_ASSERT(e) ((e) ? (void)0 : fvcl_assert(#e, __FILE__, __LINE__))

#endif	// NDEBUG

#else // __GNUG__

#include <assert.h>

#define FVCL_ASSERT(e) assert(e)

#endif	// __GNUG__


// Macros pour compatibilité C++ Builder

#ifndef __BORLANDC__

#define PACKAGE
#define USEFORM(NomFichierCpp, NomForm)
#define __classid(TForm_User) ((TForm *) new TForm_User(NULL, _T(#TForm_User)))

#else

#define FASTCALL __fastcall
#define FVCL_BEGIN_COMPONENTS
#define FVCL_END_COMPONENTS
#define FVCL_BEGIN_EVENTS
#define FVCL_END_EVENTS

#endif


// Bug de VISUAL C ?

#ifdef _MSC_VER
#pragma warning( disable : 4786 )  // Message d'erreur incompréhensible dans xstring
#endif  // _MSC_VER


// Constantes non définies

#ifndef WC_STATIC
#define WC_STATIC _T("static")
#endif  // WC_STATIC

#ifndef WC_BUTTON
#define WC_BUTTON _T("button")
#endif  // WC_BUTTON

#ifndef WC_EDIT
#define WC_EDIT _T("edit")
#endif  // WC_EDIT

#ifndef WC_LISTBOX
#define WC_LISTBOX _T("listbox")
#endif  // WC_LISTBOX

#ifndef WC_COMBOBOX
#define WC_COMBOBOX _T("combobox")
#endif  // WC_COMBOBOX

#ifndef WC_SCROLLBAR
#define WC_SCROLLBAR _T("scrollbar")
#endif  // WC_SCROLLBAR

#ifndef WC_LINK
#define WC_LINK _T("SysLink")
#endif  // WC_LINK

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 64
#endif  // BIF_NEWDIALOGSTYLE

#ifndef CFM_BACKCOLOR
#define CFM_BACKCOLOR 0x04000000
#endif  // CFM_BACKCOLOR

#ifndef BIF_NONEWFOLDERBUTTON
#define BIF_NONEWFOLDERBUTTON 0x00000200
#endif  // BIF_NONEWFOLDERBUTTON

#ifndef OFN_ENABLEINCLUDENOTIFY
#define OFN_ENABLEINCLUDENOTIFY 0x00400000
#endif  // OFN_ENABLEINCLUDENOTIFY

#ifndef OFN_ENABLESIZING
#define OFN_ENABLESIZING 0x00800000
#endif  // OFN_ENABLESIZING

#ifndef TVS_EX_MULTISELECT
#define TVS_EX_MULTISELECT          0x0002
#endif  // TVS_EX_MULTISELECT

#ifndef TVM_SETEXTENDEDSTYLE
#define TVM_SETEXTENDEDSTYLE        (TV_FIRST + 44)
#endif  // TVM_SETEXTENDEDSTYLE

#ifndef TreeView_SetExtendedStyle
#define TreeView_SetExtendedStyle(hwnd, dw, mask) (DWORD) SendMessage((hwnd), TVM_SETEXTENDEDSTYLE, mask, dw)
#endif  // TreeView_SetExtendedStyle

#ifndef PGM_SETCHILD
#define PGM_SETCHILD        (PGM_FIRST + 1)
#endif  // PGM_SETCHILD

#ifndef Pager_SetChild
#define Pager_SetChild(hwnd, hwndChild) (int) SendMessage((hwnd), PGM_SETCHILD, 0, (LPARAM) (hwndChild))
#endif  // Pager_SetChild

#ifndef PGM_GETBUTTONSIZE
#define PGM_GETBUTTONSIZE        (PGM_FIRST + 11)
#endif  // PGM_GETBUTTONSIZE

#ifndef Pager_GetButtonSize
#define Pager_GetButtonSize(hwnd) (int) SendMessage((hwnd), PGM_GETBUTTONSIZE, 0, 0)
#endif  // Pager_GetButtonSize

#ifndef PGM_SETBUTTONSIZE
#define PGM_SETBUTTONSIZE        (PGM_FIRST + 10)
#endif  // PGM_SETBUTTONSIZE

#ifndef Pager_SetButtonSize
#define Pager_SetButtonSize(hwnd, iSize) (int) SendMessage((hwnd), PGM_SETBUTTONSIZE, 0, (LPARAM) (iSize))
#endif  // Pager_SetButtonSize

#ifndef Pager_SetBkColor
#define Pager_SetBkColor(hwnd, clr) (COLORREF) SendMessage((hwnd), PGM_SETBKCOLOR, 0, (LPARAM)(clr))
#endif  // Pager_SetBkColor

#ifndef Pager_RecalcSize
#define Pager_RecalcSize(hwnd) (void) SendMessage((hwnd), PGM_RECALCSIZE, 0, 0)
#endif  // Pager_RecalcSize

#ifndef ODS_HOTLIGHT
#define ODS_HOTLIGHT        0x0040
#endif  // ODS_HOTLIGHT

#ifndef ODS_NOFOCUSRECT
#define ODS_NOFOCUSRECT     0x0200
#endif  // ODS_NOFOCUSRECT

#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif  // _MAX_FNAME

#ifndef _countof
#define _countof(table) (sizeof(table) / sizeof(table[0]))
#endif  // _countof

#ifdef __GNUG__
#define _memicmp memcmp
#endif  // __GNUG__

#ifdef __GNUG__
#define MUST_DEFINE_ITOA_S
#endif  // __GNUG__
#ifdef _MSC_VER
#if (_MSC_VER <= 1200)  // Pas sûr de la version. Entre 1200 (Visual Studio 6) et 1500 (Visual studio express 2008)
#define MUST_DEFINE_ITOA_S
#endif // (_MSC_VER < 1200)
#endif // _MSC_VER
#ifdef MUST_DEFINE_ITOA_S
#define _itoa_s(Value, szBuf, BufLen, Base) _itoa(Value, szBuf, Base)
#define _i64toa_s(Value, szBuf, BufLen, Base) _itoa(Value, szBuf, Base)
#define _itow_s(Value, szBuf, BufLen, Base) _itow(Value, szBuf, Base)
#define _i64tow_s(Value, szBuf, BufLen, Base) _itow(Value, szBuf, Base)
#undef MUST_DEFINE_ITOA_S
#endif  // MUST_DEFINE_ITOA_S


#ifdef MUST_DEFINE_INT_PTR  // A l'utilisateur de le définir si besoin

#ifndef INT_PTR
#define INT_PTR INT
#endif  // INT_PTR

#ifndef LONG_PTR
#define LONG_PTR LONG
#endif  // LONG_PTR

#ifndef GetClassLongPtr
#define GetClassLongPtr GetClassLong
#endif  // GetClassLongPtr

#ifndef SetClassLongPtr
#define SetClassLongPtr SetClassLong
#endif  // SetClassLongPtr

#ifndef GetWindowLongPtr
#define GetWindowLongPtr GetWindowLong
#endif  // GetWindowLongPtr

#ifndef SetWindowLongPtr
#define SetWindowLongPtr SetWindowLong
#endif  // SetWindowLongPtr

#endif  // MUST_DEFINE_INT_PTR

// Alignement des paramètres de fonctions (variable suivant les compilateurs)
// A définir par l'utilisateur si différent de 1
// Par exemple c'est 8 sous Visual Studio 2010 64 bits
//             c'est 4 sous Visual Studio 2010 32 bits

#if (_MSC_VER >= 1600)  // 1600 = Visual Studio 2010
#ifdef _WIN64  // 64 bits
#define ARGS_ALIGN 8
#else  // 64 bits
#define ARGS_ALIGN 4
#endif  // _WIN64
#endif  // (_MSC_VER >= 1600)

#ifndef ARGS_ALIGN
#define ARGS_ALIGN 1
#endif  // ARGS_ALIGN

#define ARG_SIZE(arg_size) ((((arg_size) - 1) / ARGS_ALIGN + 1) * ARGS_ALIGN)


//---------------------------------------------------------------------------
// Macro pour caractères unicodes
//---------------------------------------------------------------------------

#ifdef UNICODE

#ifndef __BORLANDC__
#define AnsiString WideString
#define IntToStr IntToStrW
#define StrToInt StrToIntW
#define StrToIntDef StrToIntDefW
#define HexToByte HexToByteW
#define HexToBin HexToBinW
#define BinToHex BinToHexW
#define HexToInt HexToIntW
#define IntToHex IntToHexW
#define lstrcpyn lstrcpynW
#endif // __BORLANDC__

#ifndef _T
#define _T(x) L##x
#endif

#ifndef TCHAR
#define TCHAR wchar_t
#endif

#else // UNICODE

#ifndef __BORLANDC__
#define AnsiString CharString
#define IntToStr IntToStrA
#define StrToInt StrToIntA
#define StrToIntDef StrToIntDefA
#define HexToByte HexToByteA
#define HexToBin HexToBinA
#define BinToHex BinToHexA
#define HexToInt HexToIntA
#define IntToHex IntToHexA
#ifndef lstrcpyn
#define lstrcpyn lstrcpynA
#endif // lstrcpyn
#endif // __BORLANDC__

#ifndef _T
#define _T(x) x
#endif

#ifndef TCHAR
#define TCHAR char
#endif

#endif // UNICODE

#define LongToStr IntToStr
#define StrToLong StrToInt

//---------------------------------------------------------------------------
// Includes VCL
//---------------------------------------------------------------------------

#ifndef __BORLANDC__

#include "Property.h"
#include "FVCLBase.h"
#include "TObject.h"
#include "Event.h"
#include "Collections.h"
#include "Objects.h"
#include "Persistents.h"
#include "Components.h"
#include "WinControls.h"
#pragma hdrstop

#else // __BORLANDC__

#include <vcl.h>
#include "Property.h"
#pragma hdrstop

#endif // __BORLANDC__

//---------------------------------------------------------------------------
// Libs FreeVCL
//---------------------------------------------------------------------------

#ifdef _MSC_VER  // Visual Studio
#pragma comment(lib, "FreeVCL.lib")
#pragma comment(lib, "ComCtl32.lib")
#endif  // _MSC_VER


//---------------------------------------------------------------------------
// Ressources pour LINUX
//---------------------------------------------------------------------------

#ifdef LINUX

#include "ResourcesLinux.h"

#endif  // LINUX


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

#ifndef __BORLANDC__

extern HINSTANCE HInstance;  // Handle d'instance
extern TApplication *Application;
extern TMouse *Mouse;
extern TScreen *Screen;
extern TClipboard *Clipboard;
extern TDragObject *FVCL_DragObject;

#ifdef LINUX

extern GC X11_gc;
extern Display *X11_display;
extern int X11_screen;
extern Window X11_root;
extern unsigned long X11_white_pixel;
extern unsigned long X11_black_pixel;

#endif  // LINUX

#endif // __BORLANDC__

//---------------------------------------------------------------------------
#endif  // FreeVclH

#undef In_FreeVclH
