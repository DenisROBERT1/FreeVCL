//-----------------------------------------------------------------------------
//! @file TMaskEdit.cpp
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
// TMaskEdit
//---------------------------------------------------------------------------

TMaskEdit::TMaskEdit(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TMaskEdit");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FExWindowStyle = WS_EX_CLIENTEDGE;
	FReadOnly = false;
  FSelStart = 0;
  FSelLength = -1;
  FTabStop = true;
  FColor = clWindow;

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
TMaskEdit::~TMaskEdit(void) {
}

//---------------------------------------------------------------------------
bool TMaskEdit::ProcessCommand(WORD Notify) {
  if (Notify == EN_CHANGE) {
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété EditMask
//---------------------------------------------------------------------------

AnsiString TMaskEdit::Get_EditMask(void) {
  return FEditMask;
}

bool TMaskEdit::Set_EditMask(AnsiString NewEditMask) {
	if (FEditMask != NewEditMask) {
		FEditMask = NewEditMask;
	}

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EditText
//---------------------------------------------------------------------------

AnsiString TMaskEdit::Get_EditText(void) {
  return FEditText;
}

bool TMaskEdit::Set_EditText(AnsiString NewEditText) {
	if (FEditText != NewEditText) {
		FEditText = NewEditText;
	}

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TMaskEdit::Get_Text(void) {
  TCHAR *szBuf;
  int Length;


  Length = GetWindowTextLength(FHandle) + 1;
  szBuf = new TCHAR[Length];
  GetWindowText(FHandle, szBuf, Length);
  FText = szBuf;
  delete[] szBuf;

  return FText;
}

bool TMaskEdit::Set_Text(AnsiString NewText) {
  FText = NewText;
  if (FHandle) {
    SetWindowText(FHandle, NewText);
    SendMessage(FHandle, EM_SETSEL, (WPARAM) 0, (LPARAM) -1);
  }
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SelStart
//---------------------------------------------------------------------------

int TMaskEdit::Get_SelStart(void) {
  return FSelStart;
}

bool TMaskEdit::Set_SelStart(int NewSelStart) {
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

int TMaskEdit::Get_SelLength(void) {
  return FSelLength;
}

bool TMaskEdit::Set_SelLength(int NewSelLength) {
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
// Accesseurs de la propriété MaxLength
//---------------------------------------------------------------------------

int TMaskEdit::Get_MaxLength(void) {
  int MaxLength = 0;


  if (FHandle) {
    MaxLength = (int) SendMessage(FHandle, EM_GETLIMITTEXT, 0, 0);
		if (MaxLength == 30000) MaxLength = 0;
  }

  return MaxLength;
}

bool TMaskEdit::Set_MaxLength(int NewMaxLength) {

  if (FHandle) {
    SendMessage(FHandle, EM_SETLIMITTEXT, (WPARAM) NewMaxLength, 0);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ReadOnly
//---------------------------------------------------------------------------

bool TMaskEdit::Get_ReadOnly(void) {
  return FReadOnly;
}

bool TMaskEdit::Set_ReadOnly(bool NewReadOnly) {
  FReadOnly = NewReadOnly;

  if (NewReadOnly) FWindowStyle |= ES_READONLY;
  else FWindowStyle &= (DWORD) ~(ES_READONLY);
  if (FHandle) {
    SendMessage(FHandle, EM_SETREADONLY, (WPARAM) (BOOL) NewReadOnly, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PasswordChar
//---------------------------------------------------------------------------

TCHAR TMaskEdit::Get_PasswordChar(void) {
  return FPasswordChar;
}

bool TMaskEdit::Set_PasswordChar(TCHAR NewPasswordChar) {
  FPasswordChar = NewPasswordChar;

  if (NewPasswordChar) FWindowStyle |= ES_READONLY;
  else FWindowStyle &= (DWORD) ~(ES_READONLY);
  if (FHandle) {
    SendMessage(FHandle, EM_SETPASSWORDCHAR, (WPARAM) (UINT) NewPasswordChar, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TMaskEdit::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("EditMask"));
  ListProperties->Add(_T("EditText"));
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

TYPEPROPERTY TMaskEdit::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("EditMask")) {
    return tpText;
  }
  if (asProperty == _T("EditText")) {
    return tpText;
  }
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

AnsiString TMaskEdit::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("EditMask")) {
    return _T("");
  }
  if (asProperty == _T("EditText")) {
    return _T("");
  }
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

AnsiString TMaskEdit::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("EditMask")) {
    return Get_EditMask();
  }
  if (asProperty == _T("EditText")) {
    return Get_EditText();
  }
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

bool TMaskEdit::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("EditMask")) {
    Set_EditMask(asValue);
    return true;
  }
  if (asProperty == _T("EditText")) {
    Set_EditText(asValue);
    return true;
  }
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
  if (asProperty == _T("Text")) {
    Set_Text(asValue);
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
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

