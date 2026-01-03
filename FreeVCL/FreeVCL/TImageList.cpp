//-----------------------------------------------------------------------------
//! @file TImageList.cpp
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
// TImageList
//---------------------------------------------------------------------------

TImageList::TImageList(TComponent* AOwner): TDragImageList(AOwner) {
  // Initialisations
  FClassName = _T("TImageList");
}

TImageList::~TImageList(void) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TImageList::GetListProperties(TStrings *ListProperties) {

  TDragImageList::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TImageList::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TDragImageList::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TImageList::GetDefaultProperty(AnsiString asProperty) {
  return TDragImageList::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TImageList::GetProperty(AnsiString asProperty) {
  return TDragImageList::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TImageList::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TDragImageList::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

