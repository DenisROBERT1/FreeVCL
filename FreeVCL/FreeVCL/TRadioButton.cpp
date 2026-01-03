//-----------------------------------------------------------------------------
//! @file TRadioButton.cpp
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
// TRadioButton
//---------------------------------------------------------------------------

TRadioButton::TRadioButton(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TRadioButton");
	if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= BS_AUTORADIOBUTTON;
  FTabStop = true;

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
TRadioButton::~TRadioButton(void) {
}

//---------------------------------------------------------------------------
bool TRadioButton::ProcessCommand(WORD Notify) {
  if (Notify == BN_CLICKED) {
		// Très certainement un bug de Windows: les radio-boutons recoivent un
		// message BN_CLICKED lorsqu'ils ont le focus. J'ai beaucoup ri en
		// lisant les engueulades entre un utilisateur mécontent et un connard
		// de chez Microsoft...
		// D'où la necessité du test suivant:
		if (SendMessage(FHandle, BM_GETCHECK, 0, 0) == BST_CHECKED) {
			if (!FComponentState.Contains(csLoading)) {
				if (FAction) FAction->Execute();
				OnClick(this);
			}
		}
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TRadioButton::Get_Alignment(void) {
  if (FWindowStyle & BS_LEFTTEXT) return taLeftJustify;
  else return taRightJustify;
}

bool TRadioButton::Set_Alignment(TAlignment NewAlignment) {
  DWORD NewWindowStyle;


  NewWindowStyle = (DWORD) (SIZE_T) GetWindowLongPtr(FHandle, GWL_STYLE);
  if (NewAlignment == taLeftJustify) NewWindowStyle |= (DWORD) BS_LEFTTEXT;
  else NewWindowStyle &= (DWORD) ~(BS_LEFTTEXT);
  ChangeWindowStyle(NewWindowStyle);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Checked
//---------------------------------------------------------------------------

bool TRadioButton::Get_Checked(void) {
  return (SendMessage(FHandle, BM_GETCHECK, 0, 0) == BST_CHECKED);
}


bool TRadioButton::Set_Checked(bool NewChecked) {
  if (FHandle) {
		if (NewChecked) {
			SendMessage(FHandle, BM_SETCHECK, BST_CHECKED, 0);
			if (FOwner) {
				int i;
				TRadioButton *RadioButton;
				for (i = 0; i < FOwner->ComponentCount; i++) {
					RadioButton = dynamic_cast<TRadioButton *>(FOwner->Components[i]);
					if (RadioButton != NULL && RadioButton != this) {
						SendMessage(RadioButton->FHandle, BM_SETCHECK, BST_UNCHECKED, 0);
					}
				}
			}
		}
    else SendMessage(FHandle, BM_SETCHECK, BST_UNCHECKED, 0);
  }

  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TRadioButton::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Alignment"));
  ListProperties->Add(_T("Checked"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TRadioButton::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Alignment")) {
    *asInfos = _T("taLeftJustify\ntaRightJustify");
    return tpChoice;
  }
  if (asProperty == _T("Checked")) {
    return tpBool;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TRadioButton::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    return _T("taRightJustify");
  }
  if (asProperty == _T("Checked")) {
    return _T("False");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TRadioButton::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    TAlignment Alignment = Get_Alignment();
    if (Alignment == taLeftJustify) return _T("taLeftJustify");
    if (Alignment == taRightJustify) return _T("taRightJustify");
    return _T("taRightJustify");
  }
  if (asProperty == _T("Checked")) {
    if (Get_Checked()) return _T("True");
    else return _T("False");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TRadioButton::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Alignment")) {
    if (asValue == _T("taLeftJustify")) Set_Alignment(taLeftJustify);
    else Set_Alignment(taRightJustify);
    return true;
  }
  if (asProperty == _T("Checked")) {
    Set_Checked(asValue == _T("True"));
    return true;
  }

  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

