//-----------------------------------------------------------------------------
//! @file TObjetPdfCatalog.cpp
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

#include <FreeVcl.h>
#include <FreeVcl_XFiles.h>

//---------------------------------------------------------------------------
//
//                            TObjetPdfCatalog
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TObjetPdfCatalog::TObjetPdfCatalog(void *Owner): TObjetPdf(Owner) {
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TObjetPdfCatalog::~TObjetPdfCatalog(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Outlines
//---------------------------------------------------------------------------

TObjetPdfOutlines *TObjetPdfCatalog::Get_Outlines(void) {
  return FOutlines;
}

bool TObjetPdfCatalog::Set_Outlines(TObjetPdfOutlines *NewOutlines) {
  if (FOutlines != NewOutlines) {
    FOutlines = NewOutlines;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Pages
//---------------------------------------------------------------------------

TObjetPdfPages *TObjetPdfCatalog::Get_Pages(void) {
  return FPages;
}

bool TObjetPdfCatalog::Set_Pages(TObjetPdfPages *NewPages) {
  if (FPages != NewPages) {
    FPages = NewPages;
  }

  return true;
}


//---------------------------------------------------------------------------
// Chaîne de description
//---------------------------------------------------------------------------

CharString TObjetPdfCatalog::GetDescription(void) {
  CharString csDescription;
  int NumObjPages, NumObjOutlines;


  NumObjPages = FPages->NumObj;
  NumObjOutlines = FOutlines->NumObj;
  csDescription.sprintf("<<\n/Type /Catalog\n/Pages %i 0 R\n/Outlines %i 0 R\n>>\n",
                        NumObjPages, NumObjOutlines);

  return csDescription;
}

//---------------------------------------------------------------------------
bool TObjetPdfCatalog::SetDescription(CharString csDescription) {
	return true;
}

//---------------------------------------------------------------------------

