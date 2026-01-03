//-----------------------------------------------------------------------------
//! @file FreeVcl_RichEdit.h
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

#ifdef In_FreeVcl_RichEditH
#error "Inclusion circulaire de FreeVcl_RichEdit"
#endif // In_FreeVcl_RichEditH

#define In_FreeVcl_RichEditH

#ifndef FreeVcl_RicheditH
#define FreeVcl_RicheditH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>
#include <vector>
#include <commdlg.h>
#include <commctrl.h>
#include "property.h"
#include "FVCLBase.h"
#include "TFactory_FVCL_RichEdit.h"
#include "TRichEdit.h"


#ifdef __BORLANDC__
#pragma link "COMCTL32.LIB"
#endif

//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL_RichEdit Factory_FVCL_RichEdit;


//---------------------------------------------------------------------------
#endif

#undef In_FreeVcl_RichEditH
