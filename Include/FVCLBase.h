//-----------------------------------------------------------------------------
//! @file FVCLBase.h
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

#ifdef In_FVCLBaseH
#error "Inclusion circulaire de FVCLBase"
#endif // In_FVCLBaseH

#define In_FVCLBaseH

#ifndef FVCLBaseH
#define FVCLBaseH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "Set.h"
#include "FVCLTypes.h"
#include "CharString.h"
#include "WideString.h"
#include "Exceptions.h"
#include "TPoint.h"
#include "TDateTime.h"
#include "TFactory_FVCL.h"


#endif  // FVCLBaseH


#undef In_FVCLBaseH
