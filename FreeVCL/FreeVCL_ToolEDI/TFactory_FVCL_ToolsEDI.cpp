//-----------------------------------------------------------------------------
//! @file TFactory_FVCL_ToolsEDI.cpp
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
#include <FreeVCL_ToolsEDI.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

TFactory_FVCL_ToolsEDI Factory_FVCL_ToolsEDI;


//---------------------------------------------------------------------------
// TFactory_FVCL_ToolsEDI
//---------------------------------------------------------------------------

TFactory_FVCL_ToolsEDI::TFactory_FVCL_ToolsEDI(void) {
  // Initialisations
}

TFactory_FVCL_ToolsEDI::~TFactory_FVCL_ToolsEDI(void) {
}

bool TFactory_FVCL_ToolsEDI::GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs) {
  ListObjects->Add(_T("TTextEditor")); ListTabs->Add(_T("Outils éditeur"));
  ListObjects->Add(_T("THexaEditor")); ListTabs->Add(_T("Outils éditeur"));

  return true;
}

TComponent *TFactory_FVCL_ToolsEDI::CreateObjectVirt(TComponent* AOwner, AnsiString asNom) {
  if (asNom == _T("TTextEditor")) return new TTextEditor(AOwner);
  if (asNom == _T("THexaEditor")) return new THexaEditor(AOwner);

  return NULL;
}

//---------------------------------------------------------------------------

