//-----------------------------------------------------------------------------
//! @file TPopupMenu.cpp
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
extern TApplication *Application;
extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// TPopupMenu
//---------------------------------------------------------------------------

TPopupMenu::TPopupMenu(TComponent* AOwner): TMenu(AOwner) {

  // Initialisations
  FClassName = _T("TPopupMenu");
  FAutoPopup = true;
  TypeMenu = TM_Popup;
  FHandle = NULL;
	FOwner = AOwner;
}

TPopupMenu::~TPopupMenu(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AutoPopup
//---------------------------------------------------------------------------

bool TPopupMenu::Get_AutoPopup(void) {
  return FAutoPopup;
}

bool TPopupMenu::Set_AutoPopup(bool NewAutoPopup) {

  if (FAutoPopup != NewAutoPopup) {
    FAutoPopup = NewAutoPopup;
  }
  return true;
}

//---------------------------------------------------------------------------
// Affichage du popup
//---------------------------------------------------------------------------

bool TPopupMenu::Popup(int X, int Y) {

  // MenuPopup

  if (FHandle == NULL) BuildMenu();
	OnPopup(this);
  if (TrackPopupMenu(FHandle, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON,
                     X + 5, Y + 10, 0, FWindowHandle, NULL) == FALSE) {
		FLastError = GetLastError();
	}

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPopupMenu::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("AutoPopup"));
  ListProperties->Add(_T("OnPopup"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPopupMenu::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("AutoPopup")) {
    return tpBool;
  }
  if (asProperty == _T("OnPopup")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TPopupMenu::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("AutoPopup")) {
    return _T("True");
  }
  if (asProperty == _T("OnPopup")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TPopupMenu::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("AutoPopup")) {
    if (Get_AutoPopup()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("OnPopup")) {
    return OnPopup.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPopupMenu::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("AutoPopup")) {
    Set_AutoPopup(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("OnPopup")) {
		OnPopup.ObjCall = Sender;
    OnPopup = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
