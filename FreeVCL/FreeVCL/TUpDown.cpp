//-----------------------------------------------------------------------------
//! @file TUpDown.cpp
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
// TUpDown
//---------------------------------------------------------------------------

TUpDown::TUpDown(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TUpDown");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= UDS_ARROWKEYS | UDS_NOTHOUSANDS | UDS_SETBUDDYINT;
  FMin = 0;
  FMax = 100;
  FIncrement = 1;
  FPos = 0;
	FAssociate = NULL;

  // Enregistrement de la classe UpDown
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_UPDOWN_CLASS;
  InitCommonControlsEx(&InitCtrls);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         UPDOWN_CLASS, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    SendMessage(FHandle, UDM_SETRANGE32, (WPARAM) FMin, (LPARAM) FMax);

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TUpDown::~TUpDown(void) {
}

//---------------------------------------------------------------------------
int TUpDown::ProcessNotify(LPNMHDR pnmh) {
  // LPNMUPDOWN lpnmud;

  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
	case UDN_DELTAPOS:
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Min
//---------------------------------------------------------------------------

long TUpDown::Get_Min(void) {
  return FMin;
}

bool TUpDown::Set_Min(long NewMin) {
  if (FMin != NewMin) {
    FMin = NewMin;
    SendMessage(FHandle, UDM_SETRANGE32, (WPARAM) FMin, (LPARAM) FMax);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Max
//---------------------------------------------------------------------------

long TUpDown::Get_Max(void) {
  return FMax;
}

bool TUpDown::Set_Max(long NewMax) {
  if (FMax != NewMax) {
    FMax = NewMax;
    SendMessage(FHandle, UDM_SETRANGE32, (WPARAM) FMin, (LPARAM) FMax);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PageSize
//---------------------------------------------------------------------------

long TUpDown::Get_Increment(void) {
  return FIncrement;
}

bool TUpDown::Set_Increment(long NewIncrement) {
  UDACCEL UDAccel;


  if (FIncrement != NewIncrement) {
    FIncrement = NewIncrement;
    UDAccel.nSec = 0;
    UDAccel.nInc = FIncrement;
    SendMessage(FHandle, UDM_SETACCEL, 1, (LPARAM) &UDAccel);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

long TUpDown::Get_Position(void) {
  return FPos;
}

bool TUpDown::Set_Position(long NewPosition) {
  if (NewPosition < FMin) NewPosition = FMin;
  if (NewPosition > FMax) NewPosition = FMax;
  if (FPos != NewPosition) {
    FPos = NewPosition;
    SendMessage(FHandle, UDM_SETPOS, 0, (LPARAM) NewPosition);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Associate
//---------------------------------------------------------------------------

TEdit *TUpDown::Get_Associate(void) {
  return FAssociate;
}

bool TUpDown::Set_Associate(TEdit *NewAssociate) {
  if (FAssociate != NewAssociate) {
    FAssociate = NewAssociate;
    if (FAssociate) SendMessage(FHandle, UDM_SETBUDDY, (WPARAM) (HWND) NewAssociate->Handle, 0);
		else SendMessage(FHandle, UDM_SETBUDDY, (WPARAM) (HWND) NULL, 0);
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TUpDown::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Min"));
  ListProperties->Add(_T("Max"));
  ListProperties->Add(_T("Increment"));
  ListProperties->Add(_T("Position"));
  ListProperties->Add(_T("Associate"));
  ListProperties->Add(_T("OnChange"));
  ListProperties->Add(_T("OnClick"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TUpDown::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Min")) {
    return tpNumber;
  }
  if (asProperty == _T("Max")) {
    return tpNumber;
  }
  if (asProperty == _T("Increment")) {
    return tpNumber;
  }
  if (asProperty == _T("Position")) {
    return tpNumber;
  }
  if (asProperty == _T("Associate")) {
    return tpObject;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnClick")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TUpDown::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Min")) {
    return _T("0");
  }
  if (asProperty == _T("Max")) {
    return _T("100");
  }
  if (asProperty == _T("Increment")) {
    return _T("1");
  }
  if (asProperty == _T("Position")) {
    return _T("0");
  }
  if (asProperty == _T("Associate")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  if (asProperty == _T("OnClick")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TUpDown::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Min")) {
    return IntToStr(Get_Min());
  }
  if (asProperty == _T("Max")) {
    return IntToStr(Get_Max());
  }
  if (asProperty == _T("Increment")) {
    return IntToStr(Get_Increment());
  }
  if (asProperty == _T("Position")) {
    return IntToStr(Get_Position());
  }
  if (asProperty == _T("Associate")) {
    TEdit *Edit = Get_Associate();
    if (Edit) return Edit->Name;
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  if (asProperty == _T("OnClick")) {
    return OnClick.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TUpDown::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Min")) {
    Set_Min(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Max")) {
    Set_Max(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Increment")) {
    Set_Increment(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Position")) {
    Set_Position(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Associate")) {
    TEdit *Edit;
		if (asValue.IsEmpty()) Edit = NULL;
		else Edit = dynamic_cast<TEdit *>(FindRootComponent()->FindComponent(asValue));
    Set_Associate(Edit);
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnClick")) {
		OnClick.ObjCall = Sender;
    OnClick = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

