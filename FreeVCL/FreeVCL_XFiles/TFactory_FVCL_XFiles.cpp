//-----------------------------------------------------------------------------
//! @file TFactory_FVCL_XFiles.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

TFactory_FVCL_XFiles Factory_FVCL_XFiles;


//---------------------------------------------------------------------------
// TFactory_FVCL_XFiles
//---------------------------------------------------------------------------

TFactory_FVCL_XFiles::TFactory_FVCL_XFiles(void) {
  // Initialisations
}

TFactory_FVCL_XFiles::~TFactory_FVCL_XFiles(void) {
}

bool TFactory_FVCL_XFiles::GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs) {
  ListObjects->Add(_T("TDBFFile")); ListTabs->Add(_T("Formats spéciaux"));
  ListObjects->Add(_T("TPDFFile")); ListTabs->Add(_T("Formats spéciaux"));
  // $$$ ListObjects->Add(_T("TMetafile")); ListTabs->Add(_T("Formats spéciaux"));
  ListObjects->Add(_T("TXMLFile")); ListTabs->Add(_T("Formats spéciaux"));

  return true;
}

TComponent *TFactory_FVCL_XFiles::CreateObjectVirt(TComponent* AOwner, AnsiString asNom) {
  if (asNom == _T("TDBFFile")) return new TDBFFile(AOwner);
  if (asNom == _T("TPDFFile")) return new TPDFFile(AOwner);
  // $$$ if (asNom == _T("TMetafile")) return new TMetafile(AOwner);
  if (asNom == _T("TXMLFile")) return new TXMLFile(AOwner);

  return NULL;
}

//---------------------------------------------------------------------------

