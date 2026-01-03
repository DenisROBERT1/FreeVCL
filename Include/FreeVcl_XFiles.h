//-----------------------------------------------------------------------------
//! @file FreeVcl_XFiles.h
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

#ifdef In_FreeVcl_XFilesH
#error "Inclusion circulaire de FreeVcl_XFiles"
#endif // In_FreeVcl_XFilesH

#define In_FreeVcl_XFilesH

#ifndef FreeVcl_XFilesH
#define FreeVcl_XFilesH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TDBFFile.h"

#include "TColorPdf.h"
#include "TPenPdf.h"
#include "TBrushPdf.h"
#include "TFontPdf.h"
#include "TCanvasPdf.h"
#include "TObjetPdfCatalog.h"
#include "TObjetPdfContent.h"
#include "TObjetPdfFont.h"
#include "TObjetPdfOutlines.h"
#include "TObjetPdfPage.h"
#include "TObjetPdfPages.h"
#include "TObjetPdfProcSet.h"
#include "TObjetPdfUnknown.h"
#include "TPDFFile.h"
#include "TMetafile.h"
#include "TXMLFile.h"
#include "TFactory_FVCL_XFiles.h"

//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL_XFiles Factory_FVCL_XFiles;


//---------------------------------------------------------------------------
#endif

#undef In_FreeVcl_XFilesH
