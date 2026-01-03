//-----------------------------------------------------------------------------
//! @file ApplicationCS.h
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
#ifndef ApplicationCS_H
#define ApplicationCS_H

#include "targetver.h"

BOOL GetCompilerPath(AnsiString TypeCompiler, wchar_t *szCompilerPath, DWORD cbMaxCompilerPath);
BOOL SetCompilerPath(AnsiString TypeCompiler, const wchar_t *szCompilerPath);
BOOL GetListCompilers(std::vector<AnsiString> *ListCompilers);
AnsiString GetNameCompiler(AnsiString TypeCompiler);

#endif /* ApplicationCS_H */
