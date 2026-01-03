//-----------------------------------------------------------------------------
//! @file Components.h
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

#ifdef In_ComponentsH
#error "Inclusion circulaire de Components"
#endif // In_ComponentsH

#define In_ComponentsH

#ifndef ComponentsH
#define ComponentsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TAction.h"
#include "TOpenDialog.h"
#include "TSaveDialog.h"
#include "TPrintDialog.h"
#include "TPrinterSetupDialog.h"
#include "TColorDialog.h"
#include "TFileList.h"
#include "TFontDialog.h"
#include "TMenuItem.h"
#include "TMainMenu.h"
#include "TPopupMenu.h"
#include "TImageList.h"
#include "TTimer.h"


#endif  // ComponentsH

#undef In_ComponentsH
