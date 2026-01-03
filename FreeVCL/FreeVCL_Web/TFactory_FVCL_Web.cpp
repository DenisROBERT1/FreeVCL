//-----------------------------------------------------------------------------
//! @file TFactory_FVCL_Web.cpp
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
#include <FreeVCL_Web.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

TFactory_FVCL_Web Factory_FVCL_Web;


//---------------------------------------------------------------------------
// TFactory_FVCL_Web
//---------------------------------------------------------------------------

TFactory_FVCL_Web::TFactory_FVCL_Web(void) {
  // Initialisations
}

TFactory_FVCL_Web::~TFactory_FVCL_Web(void) {
}

bool TFactory_FVCL_Web::GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs) {
  ListObjects->Add(_T("TComPort")); ListTabs->Add(_T("Connexions"));
  ListObjects->Add(_T("TClientSocket")); ListTabs->Add(_T("Internet"));
  ListObjects->Add(_T("TServerSocket")); ListTabs->Add(_T("Internet"));
  ListObjects->Add(_T("TCppWebBrowser")); ListTabs->Add(_T("Internet"));
  ListObjects->Add(_T("TDdeClientConv")); ListTabs->Add(_T("Connexions"));
  ListObjects->Add(_T("TDdeServerConv")); ListTabs->Add(_T("Connexions"));
  ListObjects->Add(_T("TDdeClientItem")); ListTabs->Add(_T("Connexions"));
  ListObjects->Add(_T("TDdeServerItem")); ListTabs->Add(_T("Connexions"));

  return true;
}

TComponent *TFactory_FVCL_Web::CreateObjectVirt(TComponent* AOwner, AnsiString asNom) {
  if (asNom == _T("TComPort")) return new TComPort(AOwner);
  if (asNom == _T("TClientSocket")) return new TClientSocket(AOwner);
  if (asNom == _T("TServerSocket")) return new TServerSocket(AOwner);
  if (asNom == _T("TCppWebBrowser")) return new TCppWebBrowser(AOwner);
  if (asNom == _T("TDdeClientConv")) return new TDdeClientConv(AOwner);
  if (asNom == _T("TDdeServerConv")) return new TDdeServerConv(AOwner);
  if (asNom == _T("TDdeClientItem")) return new TDdeClientItem(AOwner);
  if (asNom == _T("TDdeServerItem")) return new TDdeServerItem(AOwner);

  return NULL;
}

//---------------------------------------------------------------------------

