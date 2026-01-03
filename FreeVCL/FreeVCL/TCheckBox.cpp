//-----------------------------------------------------------------------------
//! @file TCheckBox.cpp
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
// TCheckBox
//---------------------------------------------------------------------------

TCheckBox::TCheckBox(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TCheckBox");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= BS_AUTOCHECKBOX;
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
TCheckBox::~TCheckBox(void) {
}

//---------------------------------------------------------------------------
bool TCheckBox::ProcessCommand(WORD Notify) {
  if (Notify == BN_CLICKED) {
		if (!FComponentState.Contains(csLoading)) {
			if (FAction) FAction->Execute();
			OnClick(this);
		}
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TCheckBox::Get_Alignment(void) {
  if (FWindowStyle & BS_LEFTTEXT) return taLeftJustify;
  else return taRightJustify;
}

bool TCheckBox::Set_Alignment(TAlignment NewAlignment) {
  DWORD NewWindowStyle;


  NewWindowStyle = (DWORD) (SIZE_T) GetWindowLongPtr(FHandle, GWL_STYLE);
  if (NewAlignment == taLeftJustify) NewWindowStyle |= (DWORD) BS_LEFTTEXT;
  else NewWindowStyle &= (DWORD) ~(BS_LEFTTEXT);
  ChangeWindowStyle(NewWindowStyle);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AllowGrayed
//---------------------------------------------------------------------------

bool TCheckBox::Get_AllowGrayed(void) {
  return ((FWindowStyle & 0x0000000F) == BS_AUTO3STATE);
}


bool TCheckBox::Set_AllowGrayed(bool NewAllowGrayed) {
  DWORD NewWindowStyle;


  NewWindowStyle = (DWORD) (SIZE_T) GetWindowLongPtr(FHandle, GWL_STYLE);
  NewWindowStyle &= 0xFFFFFFF0;
  if (NewAllowGrayed) NewWindowStyle |= (DWORD) BS_AUTO3STATE;
  else NewWindowStyle |= (DWORD) BS_AUTOCHECKBOX;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Checked
//---------------------------------------------------------------------------

bool TCheckBox::Get_Checked(void) {
  return (SendMessage(FHandle, BM_GETCHECK, 0, 0) == BST_CHECKED);
}


bool TCheckBox::Set_Checked(bool NewChecked) {
  if (FHandle) {
    if (NewChecked) SendMessage(FHandle, BM_SETCHECK, BST_CHECKED, 0);
    else SendMessage(FHandle, BM_SETCHECK, BST_UNCHECKED, 0);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété State
//---------------------------------------------------------------------------

TCheckBoxState TCheckBox::Get_State() {
	int WState;
  WState = (int) SendMessage(FHandle, BM_GETCHECK, 0, 0);
  if (WState == BST_UNCHECKED) return cbUnchecked;
  if (WState == BST_CHECKED) return cbChecked;
  if (WState == BST_INDETERMINATE) return cbGrayed;

	return cbUnchecked;
}

bool TCheckBox::Set_State(TCheckBoxState NewState) {
	int WState = BST_UNCHECKED;

	if (NewState == cbUnchecked) WState = BST_UNCHECKED;
  else if (NewState == cbChecked) WState = BST_CHECKED;
  else if (NewState == cbGrayed) WState = BST_INDETERMINATE;

	SendMessage(FHandle, BM_SETCHECK, WState, 0);

	return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCheckBox::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Alignment"));
  ListProperties->Add(_T("AllowGrayed"));
  ListProperties->Add(_T("Checked"));
  ListProperties->Add(_T("State"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCheckBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Alignment")) {
    *asInfos = _T("taLeftJustify\ntaRightJustify");
    return tpChoice;
  }
  if (asProperty == _T("AllowGrayed")) {
    return tpBool;
  }
  if (asProperty == _T("Checked")) {
    return tpBool;
  }
  if (asProperty == _T("State")) {
    *asInfos = _T("cbUnchecked\ncbChecked\ncbGrayed");
    return tpChoice;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCheckBox::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    return _T("taRightJustify");
  }
  if (asProperty == _T("AllowGrayed")) {
    return _T("False");
  }
  if (asProperty == _T("Checked")) {
    return _T("False");
  }
  if (asProperty == _T("State")) {
    return _T("cbUnchecked");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCheckBox::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    TAlignment Alignment = Get_Alignment();
    if (Alignment == taLeftJustify) return _T("taLeftJustify");
    if (Alignment == taRightJustify) return _T("taRightJustify");
    return _T("taRightJustify");
  }
  if (asProperty == _T("AllowGrayed")) {
    if (Get_AllowGrayed()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Checked")) {
    if (Get_Checked()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("State")) {
    TCheckBoxState State = Get_State();
    if (State == cbUnchecked) return _T("cbUnchecked");
    if (State == cbChecked) return _T("cbChecked");
    if (State == cbGrayed) return _T("cbGrayed");
    return _T("");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCheckBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Alignment")) {
    if (asValue == _T("taLeftJustify")) Set_Alignment(taLeftJustify);
    else Set_Alignment(taRightJustify);
    return true;
  }
  if (asProperty == _T("AllowGrayed")) {
    Set_AllowGrayed(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Checked")) {
    Set_Checked(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("State")) {
    if (asValue == _T("cbUnchecked")) Set_State(cbUnchecked);
    else if (asValue == _T("cbChecked")) Set_State(cbChecked);
    else if (asValue == _T("cbGrayed")) Set_State(cbGrayed);
    return true;
  }

  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

