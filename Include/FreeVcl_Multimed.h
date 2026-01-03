//-----------------------------------------------------------------------------
//! @file FreeVcl_Multimed.h
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

#ifdef In_FreeVcl_MultimedH
#error "Inclusion circulaire de FreeVcl_Multimed"
#endif // In_FreeVcl_MultimedH

#define In_FreeVcl_MultimedH

#ifndef FreeVcl_MultimedH
#define FreeVcl_MultimedH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>
#include <vector>
#include "Property.h"
#include "FVCLBase.h"
#include "TFactory_FVCL_Multimed.h"
#include "TAnimate.h"
#include "TWebCam.h"


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL_Multimed Factory_FVCL_Multimed;


//---------------------------------------------------------------------------
// Libs FreeVCL
//---------------------------------------------------------------------------

#ifdef _MSC_VER  // Visual Studio
#pragma comment(lib, "Vfw32.lib")
#endif  // _MSC_VER


//---------------------------------------------------------------------------
#endif

#undef In_FreeVcl_MultimedH
