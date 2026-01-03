//-----------------------------------------------------------------------------
//! @file Application.h
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
#ifndef Application_H
#define Application_H

#include "targetver.h"

enum TYPECOMPILER {tcUnknown, tcVS9, tcVS10, tcCB};

BOOL GetCompilerPath(TYPECOMPILER TypeCompiler, wchar_t *szCompilerPath, DWORD cbMaxCompilerPath);
BOOL SetCompilerPath(TYPECOMPILER TypeCompiler, const wchar_t *szCompilerPath);
BOOL GetListCompilers(std::vector<TYPECOMPILER> *ListCompilers);
AnsiString GetNameCompiler(TYPECOMPILER TypeCompiler);

#endif /* Application_H */
