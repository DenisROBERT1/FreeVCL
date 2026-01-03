//-----------------------------------------------------------------------------
//! @file TGroupBox.cpp
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
// TGroupBox
//---------------------------------------------------------------------------

TGroupBox::TGroupBox(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TGroupBox");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= BS_GROUPBOX;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                           WC_BUTTON, FCaption,
                           FWindowStyle,
                           FLeft, FTop,
                           FWidth, FHeight,
                           hWndParent,
                           (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TGroupBox::~TGroupBox(void) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TGroupBox::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool TGroupBox::Set_Visible(bool NewVisible) {
  int i;
  TControl *Control;


  if (FVisible != NewVisible) {
    FVisible = NewVisible;
    ShowWindow(FHandle, FVisible? SW_SHOW: SW_HIDE);
    for (i = 0; i < (int) FComponents.size(); i++) {
      Control = dynamic_cast<TControl *>(FComponents[i]);
      if (Control) Control->Visible = NewVisible;
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TGroupBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TGroupBox::GetDefaultProperty(AnsiString asProperty) {
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TGroupBox::GetProperty(AnsiString asProperty) {
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TGroupBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

