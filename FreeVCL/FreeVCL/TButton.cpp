//-----------------------------------------------------------------------------
//! @file TButton.cpp
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
// TButton
//---------------------------------------------------------------------------

TButton::TButton(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TButton");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FModalResult = mrNone;
  FCancel = false;
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
TButton::~TButton(void) {
}

//---------------------------------------------------------------------------
bool TButton::ProcessCommand(WORD Notify) {
	if (Notify == BN_CLICKED) {
		Click();
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Default
//---------------------------------------------------------------------------

bool TButton::Get_Default(void) {
  return ((FWindowStyle & BS_DEFPUSHBUTTON) != 0);
}

bool TButton::Set_Default(bool NewDefault) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewDefault) NewWindowStyle |= BS_DEFPUSHBUTTON;
  else NewWindowStyle &= ~BS_DEFPUSHBUTTON;
  if (FWindowStyle != NewWindowStyle) {
    FWindowStyle = NewWindowStyle;
    SendMessage(FHandle, BM_SETSTYLE, (WPARAM) FWindowStyle, MAKELPARAM(TRUE, 0));
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Cancel
//---------------------------------------------------------------------------

bool TButton::Get_Cancel(void) {
  return FCancel;
}

bool TButton::Set_Cancel(bool NewCancel) {

  if (FCancel != NewCancel) {
    FCancel = NewCancel;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ModalResult
//---------------------------------------------------------------------------

int TButton::Get_ModalResult(void) {
  return FModalResult;
}

bool TButton::Set_ModalResult(int NewModalResult) {

  if (FModalResult != NewModalResult) {
    FModalResult = NewModalResult;
  }
  return true;
}

//---------------------------------------------------------------------------
// Simule un clic sur le bouton
//---------------------------------------------------------------------------
void FASTCALL TButton::Click(void) {
	if (!FComponentState.Contains(csLoading)) {
		if (FAction) FAction->Execute();
		OnClick(this);
	}
  if (FModalResult != mrNone) {
		TControl *ControlParent = FParent;
		TForm *FormParent;
		while (ControlParent) {
			FormParent = dynamic_cast<TForm *>(ControlParent);
			if (FormParent) {
				FormParent->ModalResult = FModalResult;
				break;
			}
			ControlParent = ControlParent->Parent;
		}
  }
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TButton::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Default"));
  ListProperties->Add(_T("Cancel"));
  ListProperties->Add(_T("ModalResult"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TButton::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Default")) {
    return tpBool;
  }
  if (asProperty == _T("Cancel")) {
    return tpBool;
  }
  if (asProperty == _T("ModalResult")) {
    *asInfos = _T("mrNone\nmrOk\nmrCancel\nmrAbort\nmrRetry\nmrIgnore\nmrYes\nmrNo\nmrAll\nmrNoToAll\nmrYesToAll");
    return tpChoice;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TButton::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Default")) {
    return _T("False");
  }
  if (asProperty == _T("Cancel")) {
    return _T("False");
  }
  if (asProperty == _T("ModalResult")) {
    return _T("mrNone");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TButton::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Default")) {
    if (Get_Default()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Cancel")) {
    if (Get_Cancel()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ModalResult")) {
    TModalResult ModalResult = Get_ModalResult();
    if (ModalResult == mrNone) return _T("mrNone");
    if (ModalResult == mrOk) return _T("mrOk");
    if (ModalResult == mrCancel) return _T("mrCancel");
    if (ModalResult == mrAbort) return _T("mrAbort");
    if (ModalResult == mrRetry) return _T("mrRetry");
    if (ModalResult == mrIgnore) return _T("mrIgnore");
    if (ModalResult == mrYes) return _T("mrYes");
    if (ModalResult == mrNo) return _T("mrNo");
    if (ModalResult == mrAll) return _T("mrAll");
    if (ModalResult == mrNoToAll) return _T("mrNoToAll");
    if (ModalResult == mrYesToAll) return _T("mrYesToAll");
    return _T("");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TButton::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Default")) {
    Set_Default(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Cancel")) {
    Set_Cancel(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ModalResult")) {
    if (asValue == _T("mrNone")) FModalResult = mrNone;
    else if (asValue == _T("mrOk")) FModalResult = mrOk;
    else if (asValue == _T("mrCancel")) FModalResult = mrCancel;
    else if (asValue == _T("mrAbort")) FModalResult = mrAbort;
    else if (asValue == _T("mrRetry")) FModalResult = mrRetry;
    else if (asValue == _T("mrIgnore")) FModalResult = mrIgnore;
    else if (asValue == _T("mrYes")) FModalResult = mrYes;
    else if (asValue == _T("mrNo")) FModalResult = mrNo;
    else if (asValue == _T("mrAll")) FModalResult = mrAll;
    else if (asValue == _T("mrNoToAll")) FModalResult = mrNoToAll;
    else if (asValue == _T("mrYesToAll")) FModalResult = mrYesToAll;
    else FModalResult = StrToInt(asValue);
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

