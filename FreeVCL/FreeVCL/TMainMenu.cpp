//-----------------------------------------------------------------------------
//! @file TMainMenu.cpp
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
// TMainMenu
//---------------------------------------------------------------------------

TMainMenu::TMainMenu(TComponent* AOwner): TMenu(AOwner) {

  // Initialisations
  FClassName = _T("TMainMenu");
  TypeMenu = TM_MainMenu;
  FHandle = CreateMenu();

}

TMainMenu::~TMainMenu(void) {
  if (FHandle) DestroyMenu(FHandle);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TMainMenu::GetListProperties(TStrings *ListProperties) {

  TMenu::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TMainMenu::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TMenu::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TMainMenu::GetDefaultProperty(AnsiString asProperty) {
  return TMenu::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TMainMenu::GetProperty(AnsiString asProperty) {
  return TMenu::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TMainMenu::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TMenu::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
// Méthode appelée en fin de construction de la fiche
//---------------------------------------------------------------------------

void FASTCALL TMainMenu::AfterConstruction(void) {

  BuildMenu();

  if (FWindowHandle) {
    TForm *OwnerForm;
    OwnerForm = dynamic_cast<TForm *> (FOwner);
    if (OwnerForm) OwnerForm->Menu = this;
  }

}

//---------------------------------------------------------------------------

