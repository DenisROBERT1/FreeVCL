//-----------------------------------------------------------------------------
//! @file TObjetPdfPages.cpp
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
//                            TObjetPdfPages
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TObjetPdfPages::TObjetPdfPages(void *Owner): TObjetPdf(Owner) {
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TObjetPdfPages::~TObjetPdfPages(void) {
}


//---------------------------------------------------------------------------
// Ajout d'une page
//---------------------------------------------------------------------------

void TObjetPdfPages::AddPage(TObjetPdfPage *ObjetPdfPage) {
  ObjetPdfPage->ObjetPdfParent = this;
  Pages.push_back(ObjetPdfPage);
}

//---------------------------------------------------------------------------
// Chaîne de description
//---------------------------------------------------------------------------

CharString TObjetPdfPages::GetDescription(void) {
  CharString csDescription, csBuf;
  int NumObj;
  int i;


  csDescription.sprintf("<<\n/Type /Pages\n/Count %i\n", Pages.size());
  if (Pages.size() > 0) {
    csDescription += "/Kids [";
    for (i = 0; i < (int) Pages.size(); i++) {
      if (i) csDescription += " ";
      NumObj = Pages[i]->NumObj;
      csBuf.sprintf("%i 0 R", NumObj);
      csDescription += csBuf;
    }
    csDescription += "]\n";
  }
  csDescription += ">>\n";

  return csDescription;
}

//---------------------------------------------------------------------------
bool TObjetPdfPages::SetDescription(CharString csDescription) {
	return true;
}

//---------------------------------------------------------------------------

