//-----------------------------------------------------------------------------
//! @file TEdit.cpp
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
// TEdit
//---------------------------------------------------------------------------

TEdit::TEdit(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TEdit");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= ES_AUTOHSCROLL;
  FExWindowStyle = WS_EX_CLIENTEDGE;
	FReadOnly = false;
  FSelStart = 0;
  FSelLength = -1;
  FTabStop = true;
  FColor = clWindow;
  FPasswordChar = _T('\0');

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_EDIT, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    SetWindowText(FHandle, FText);

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TEdit::~TEdit(void) {
}

//---------------------------------------------------------------------------
bool TEdit::ProcessCommand(WORD Notify) {
  if (Notify == EN_CHANGE) {
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété MaxLength
//---------------------------------------------------------------------------

int TEdit::Get_MaxLength(void) {
  int MaxLength = 0;


  if (FHandle) {
    MaxLength = (int) SendMessage(FHandle, EM_GETLIMITTEXT, 0, 0);
		if (MaxLength == 30000) MaxLength = 0;
  }

  return MaxLength;
}

bool TEdit::Set_MaxLength(int NewMaxLength) {

  if (FHandle) {
    SendMessage(FHandle, EM_SETLIMITTEXT, (WPARAM) NewMaxLength, 0);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PasswordChar
//---------------------------------------------------------------------------

TCHAR TEdit::Get_PasswordChar(void) {
  return FPasswordChar;
}

bool TEdit::Set_PasswordChar(TCHAR NewPasswordChar) {
  FPasswordChar = NewPasswordChar;

  if (NewPasswordChar) FWindowStyle |= ES_PASSWORD;
  else FWindowStyle &= (DWORD) ~(ES_PASSWORD);
  if (FHandle) {
    SendMessage(FHandle, EM_SETPASSWORDCHAR, (WPARAM) (UINT) NewPasswordChar, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ReadOnly
//---------------------------------------------------------------------------

bool TEdit::Get_ReadOnly(void) {
  return FReadOnly;
}

bool TEdit::Set_ReadOnly(bool NewReadOnly) {
  FReadOnly = NewReadOnly;

  if (NewReadOnly) FWindowStyle |= ES_READONLY;
  else FWindowStyle &= (DWORD) ~(ES_READONLY);
  if (FHandle) {
    SendMessage(FHandle, EM_SETREADONLY, (WPARAM) (BOOL) NewReadOnly, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelStart
//---------------------------------------------------------------------------

int TEdit::Get_SelStart(void) {
  return FSelStart;
}

bool TEdit::Set_SelStart(int NewSelStart) {
  if (FSelStart != NewSelStart) {
    FSelStart = NewSelStart;
    FSelLength = 0;
    if (FHandle) {
      SendMessage(FHandle, EM_SETSEL, (WPARAM) FSelStart,
                                      (LPARAM) FSelStart + FSelLength);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelLength
//---------------------------------------------------------------------------

int TEdit::Get_SelLength(void) {
  return FSelLength;
}

bool TEdit::Set_SelLength(int NewSelLength) {
  if (FSelLength != NewSelLength) {
    FSelLength = NewSelLength;
    if (FHandle) {
      SendMessage(FHandle, EM_SETSEL, (WPARAM) FSelStart,
                                      (LPARAM) FSelStart + FSelLength);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TEdit::Get_Text(void) {
  TCHAR *szBuf;
  int Length;


  Length = GetWindowTextLength(FHandle) + 1;
  szBuf = new TCHAR[Length];
  GetWindowText(FHandle, szBuf, Length);
  FText = szBuf;
  delete[] szBuf;

  return FText;
}

bool TEdit::Set_Text(AnsiString NewText) {
  FText = NewText;
  if (FHandle) {
    SetWindowText(FHandle, NewText);
    SendMessage(FHandle, EM_SETSEL, (WPARAM) 0, (LPARAM) -1);
  }
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TEdit::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("MaxLength"));
  ListProperties->Add(_T("PasswordChar"));
  ListProperties->Add(_T("ReadOnly"));
  ListProperties->Add(_T("SelStart"));
  ListProperties->Add(_T("SelLength"));
  ListProperties->Add(_T("Text"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TEdit::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("MaxLength")) {
    return tpNumber;
  }
  if (asProperty == _T("PasswordChar")) {
    return tpNumber;
  }
  if (asProperty == _T("ReadOnly")) {
    return tpBool;
  }
  if (asProperty == _T("SelStart")) {
    return tpNumber;
  }
  if (asProperty == _T("SelLength")) {
    return tpNumber;
  }
  if (asProperty == _T("Text")) {
    return tpText;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TEdit::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("MaxLength")) {
    return _T("0");
  }
  if (asProperty == _T("PasswordChar")) {
    return _T("0");
  }
  if (asProperty == _T("ReadOnly")) {
    return _T("False");
  }
  if (asProperty == _T("SelStart")) {
    return _T("0");
  }
  if (asProperty == _T("SelLength")) {
    return _T("-1");
  }
  if (asProperty == _T("Text")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  if (asProperty == _T("Color")) {
    return _T("clWindow");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TEdit::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("MaxLength")) {
    return IntToStr(Get_MaxLength());
  }
  if (asProperty == _T("PasswordChar")) {
    return IntToStr(Get_PasswordChar());
  }
  if (asProperty == _T("ReadOnly")) {
    if (Get_ReadOnly()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("SelStart")) {
    return IntToStr(Get_SelStart());
  }
  if (asProperty == _T("SelLength")) {
    return IntToStr(Get_SelLength());
  }
  if (asProperty == _T("Text")) {
    return Get_Text();
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TEdit::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("MaxLength")) {
    Set_MaxLength(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("PasswordChar")) {
    Set_PasswordChar(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("ReadOnly")) {
    Set_ReadOnly(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("SelStart")) {
    Set_SelStart(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("SelLength")) {
    Set_SelLength(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Text")) {
    Set_Text(asValue);
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

