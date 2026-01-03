//-----------------------------------------------------------------------------
//! @file TPanel.cpp
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


//---------------------------------------------------------------------------
// TPanel
//---------------------------------------------------------------------------

TPanel::TPanel(TComponent* AOwner): TCustomPanel(AOwner) {
  // Initialisations
  FClassName = _T("TPanel");
}

//---------------------------------------------------------------------------
TPanel::~TPanel(void) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPanel::GetListProperties(TStrings *ListProperties) {

  TCustomPanel::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPanel::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TCustomPanel::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TPanel::GetDefaultProperty(AnsiString asProperty) {
  return TCustomPanel::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TPanel::GetProperty(AnsiString asProperty) {
  return TCustomPanel::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPanel::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TCustomPanel::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
