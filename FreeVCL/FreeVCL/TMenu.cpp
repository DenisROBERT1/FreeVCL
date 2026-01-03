//-----------------------------------------------------------------------------
//! @file TMenu.cpp
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
// TMenu
//---------------------------------------------------------------------------

TMenu::TMenu(TComponent* AOwner): TMenuItem(AOwner) {
  TControl *Control;


  // Initialisations
  FClassName = _T("TMenu");
  FWindowHandle = NULL;
  if (AOwner) {
    Control = dynamic_cast<TControl *>(AOwner);
    if (Control) {
      FWindowHandle = Control->Handle;
    }
  }

}

TMenu::~TMenu(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WindowHandle
//---------------------------------------------------------------------------

HWND TMenu::Get_WindowHandle(void) {
  return FWindowHandle;
}

bool TMenu::Set_WindowHandle(HWND NewWindowHandle) {
  if (FWindowHandle != NewWindowHandle) {
    FWindowHandle = NewWindowHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TMenu::GetListProperties(TStrings *ListProperties) {

  TMenuItem::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TMenu::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TMenuItem::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TMenu::GetDefaultProperty(AnsiString asProperty) {
  return TMenuItem::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TMenu::GetProperty(AnsiString asProperty) {
  return TMenuItem::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TMenu::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TMenuItem::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

