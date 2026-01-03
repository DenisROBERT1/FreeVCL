//-----------------------------------------------------------------------------
//! @file FreeVcl_ToolsEDI.h
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

#ifdef In_FreeVcl_ToolsEDIH
#error "Inclusion circulaire de FreeVcl_ToolsEDI"
#endif // In_FreeVcl_ToolsEDIH

#define In_FreeVcl_ToolsEDIH

#ifndef FreeVcl_ToolsEDIH
#define FreeVcl_ToolsEDIH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>
#include <vector>
#include "Property.h"
#include "FVCLBase.h"
#include "TFactory_FVCL_ToolsEDI.h"
#include "TDictionnary.h"
#include "TTextEditor.h"
#include "TTextEditorLines.h"
#include "THexaEditor.h"


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL_ToolsEDI Factory_FVCL_ToolsEDI;


//---------------------------------------------------------------------------
#endif

#undef In_FreeVcl_ToolsEDIH
