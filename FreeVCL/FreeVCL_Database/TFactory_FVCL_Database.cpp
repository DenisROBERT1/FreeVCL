//-----------------------------------------------------------------------------
//! @file TFactory_FVCL_Database.cpp
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
#include <FreeVCL_Database.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

TFactory_FVCL_Database Factory_FVCL_Database;


//---------------------------------------------------------------------------
// TFactory_FVCL_Database
//---------------------------------------------------------------------------

TFactory_FVCL_Database::TFactory_FVCL_Database(void) {
  // Initialisations
}

TFactory_FVCL_Database::~TFactory_FVCL_Database(void) {
}

bool TFactory_FVCL_Database::GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs) {
  ListObjects->Add(_T("TDatabase")); ListTabs->Add(_T("Bases de données"));
  ListObjects->Add(_T("TQuery")); ListTabs->Add(_T("Bases de données"));

  return true;
}

TComponent *TFactory_FVCL_Database::CreateObjectVirt(TComponent* AOwner, AnsiString asNom) {
  /*
	if (asNom == _T("TDatabase")) return new TDatabase(AOwner);
  if (asNom == _T("TQuery")) return new TQuery(AOwner);
	*/

  return NULL;
}

//---------------------------------------------------------------------------

