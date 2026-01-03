//-----------------------------------------------------------------------------
//! @file TStringGrid.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TStringGrid
//---------------------------------------------------------------------------

TStringGrid::TStringGrid(TComponent* AOwner): TDrawGrid(AOwner) {

  // Initialisations
  FClassName = _T("TStringGrid");
  FCells = new TStringTab();
  bProcessCreateToDo = true;

}

//---------------------------------------------------------------------------
TStringGrid::~TStringGrid(void) {
  delete FCells;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TStringGrid::GetListProperties(TStrings *ListProperties) {

  TDrawGrid::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TStringGrid::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TDrawGrid::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TStringGrid::GetDefaultProperty(AnsiString asProperty) {
  return TDrawGrid::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TStringGrid::GetProperty(AnsiString asProperty) {
  return TDrawGrid::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TStringGrid::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TDrawGrid::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

