//-----------------------------------------------------------------------------
//! @file PluginUtilities.h
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
#ifndef PluginUtilities_H
#define PluginUtilities_H

//---------------------------------------------------------------------------
//! @brief Bitmap 20 x 20
//!
//! Cette fonction renvoie le handle d'une bitmap 20 x 20 pixels. Cette image
//! est utilisée pour identifier le type de projet dans les boîtes de
//! dialogues.
//!
//! @return @c HBITMAP Handle de bitmap Windows
//---------------------------------------------------------------------------
BOOL CreateProcessAndRedirectOut(const wchar_t *szProgName, const wchar_t *szArgs);

bool FileLineInput(HANDLE hFile, std::string *sBuffer);
std::wstring Unicode(std::string sStr);
std::string Ansi(std::wstring wsStr);
std::string SuppSpace(std::string sStr);
std::string SuppQuotes(std::string sStr);
std::wstring Translate(std::wstring asTexte);


#endif /* PluginUtilities_H */
