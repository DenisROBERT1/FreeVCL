//-----------------------------------------------------------------------------
//! @file TRadioGroup.cpp
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
// Variables externes
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// TRadioGroup
//---------------------------------------------------------------------------

TRadioGroup::TRadioGroup(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TRadioGroup");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= BS_GROUPBOX;
  FItemIndex = -1;

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
TRadioGroup::~TRadioGroup(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemIndex
//---------------------------------------------------------------------------

int TRadioGroup::Get_ItemIndex(void) {
  return FItemIndex;
}

bool TRadioGroup::Set_ItemIndex(int NewItemIndex) {
  int i;
  TRadioButton *RadioButton;


	if (NewItemIndex < -1) NewItemIndex = -1;
	if (NewItemIndex >= (int) FComponents.size()) NewItemIndex = (int) FComponents.size() - 1;

  if (FItemIndex != NewItemIndex) {
    FItemIndex = NewItemIndex;
    for (i = 0; i < (int) FComponents.size(); i++) {
      RadioButton = dynamic_cast<TRadioButton *> (FComponents[i]);
      if (RadioButton) RadioButton->Checked = (i == FItemIndex);
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TRadioGroup::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Items.Strings"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TRadioGroup::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Items.Strings")) {
    return tpStrings;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TRadioGroup::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Items.Strings")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TRadioGroup::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Items.Strings")) {
    return _T("");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TRadioGroup::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  AnsiString asLigne;
  TRadioButton *NewRadioButton;
  int Cur, y;


  if (asProperty == _T("Items.Strings")) {
    Cur = 0;
    y = 18;
    while (Cur < (int) asValue.Length()) {
      asLigne = UnMot(asValue, _T(""), _T("\n"), &Cur);
      NewRadioButton = (TRadioButton *) Factory_FVCL.CreateObject(this, _T("TRadioButton"));
      NewRadioButton->Caption = asLigne;
      NewRadioButton->ExWindowStyle = 0;
      NewRadioButton->WindowStyle = WS_CHILD;
      if (FVisible) NewRadioButton->WindowStyle = WS_CHILD | WS_VISIBLE;
      NewRadioButton->Left = 10;
      NewRadioButton->Top = y;
      NewRadioButton->Width = Width - 20;
      NewRadioButton->Height = 13;
      y += 18;
    }
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

