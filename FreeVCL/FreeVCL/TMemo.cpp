//-----------------------------------------------------------------------------
//! @file TMemo.cpp
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
// TMemo
//---------------------------------------------------------------------------

TMemo::TMemo(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TMemo");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN;
  // $$$ FExWindowStyle = WS_EX_CLIENTEDGE;
  FLines = new TMemoLines();
  FTabStop = true;
  FWantReturns = true;
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
    FLines->Handle = FHandle;

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TMemo::~TMemo(void) {
  delete FLines;
}

//---------------------------------------------------------------------------
void FASTCALL TMemo::DestroyWnd(void) {
  DestroyWindow(FHandle);
}

//---------------------------------------------------------------------------
void FASTCALL TMemo::CreateWnd(void) {
  HFONT hFont;
  HWND hWndParent = NULL;


  if (FOwner) hWndParent = ((TControl *) FOwner)->Handle;

  int FLeftWnd = FLeft;
  int FTopWnd = FTop;
  if (FParent) {
    FLeftWnd += FParent->LeftWnd;
    FTopWnd += FParent->TopWnd;
  }

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                           WC_EDIT, FCaption,
                           FWindowStyle,
                           FLeftWnd, FTopWnd,
                           FWidth, FHeight,
                           hWndParent,
                           (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    FLines->Handle = FHandle;
    InitStdControl();
  }

}

//---------------------------------------------------------------------------
bool TMemo::ProcessCommand(WORD Notify) {
  if (Notify == EN_CHANGE) {
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TMemo::Get_Text(void) {
  return FLines->GetTextStr();
}

bool TMemo::Set_Text(AnsiString NewText) {
  FLines->SetTextStr(NewText);
  if (FHandle) {
    /*
		Le SetWindowText a déjà été fait dans le SetTextStr
		SetWindowText(FHandle, NewText);
		*/
    SendMessage(FHandle, EM_SETSEL, (WPARAM) -2, (LPARAM) -2);
  }
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelStart
//---------------------------------------------------------------------------

int TMemo::Get_SelStart(void) {
  int SelStart;

  SendMessage(FHandle, EM_GETSEL, (WPARAM) &SelStart, 0);

  return SelStart;
}

bool TMemo::Set_SelStart(int NewSelStart) {
  if (FHandle) {
    SendMessage(FHandle, EM_SETSEL, (WPARAM) NewSelStart,
                                    (LPARAM) NewSelStart);
    SendMessage(FHandle, EM_SCROLLCARET, 0, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelLength
//---------------------------------------------------------------------------

int TMemo::Get_SelLength(void) {
  int SelStart, SelEnd;

  SendMessage(FHandle, EM_GETSEL, (WPARAM) &SelStart, (LPARAM) &SelEnd);

  return SelEnd - SelStart;
}

bool TMemo::Set_SelLength(int NewSelLength) {
  int SelStart;

  if (FHandle) {
    SendMessage(FHandle, EM_GETSEL, (WPARAM) &SelStart, 0);
    SendMessage(FHandle, EM_SETSEL, (WPARAM) SelStart,
                                    (LPARAM) SelStart + NewSelLength);
    SendMessage(FHandle, EM_SCROLLCARET, 0, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ReadOnly
//---------------------------------------------------------------------------

bool TMemo::Get_ReadOnly(void) {
  return ((FWindowStyle & ES_READONLY) != 0);
}

bool TMemo::Set_ReadOnly(bool NewReadOnly) {
  if (NewReadOnly) FWindowStyle |= ES_READONLY;
  else FWindowStyle &= (DWORD) ~(ES_READONLY);
  if (FHandle) {
    SendMessage(FHandle, EM_SETREADONLY, (WPARAM) (BOOL) NewReadOnly, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WordWrap
//---------------------------------------------------------------------------

bool TMemo::Get_WordWrap(void) {
  return ((FWindowStyle & ES_AUTOHSCROLL) == 0);
}

bool TMemo::Set_WordWrap(bool NewWordWrap) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewWordWrap) NewWindowStyle &= (DWORD) ~ES_AUTOHSCROLL;
  else NewWindowStyle |= ES_AUTOHSCROLL;
  /*
  // Ca marche pô
  ChangeWindowStyle(NewWindowStyle);
  */
  if (FWindowStyle != NewWindowStyle) {
		bool bReCreate = (FHandle != NULL);
		if (bReCreate) DestroyWnd();
		FWindowStyle = NewWindowStyle;
		if (bReCreate) CreateWnd();
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WantReturns
//---------------------------------------------------------------------------

bool TMemo::Set_WantReturns(bool NewWantReturns) {
  DWORD NewWindowStyle;


  if (FWantReturns != NewWantReturns) {
    FWantReturns = NewWantReturns;
    NewWindowStyle = FWindowStyle;
    if (NewWantReturns) NewWindowStyle |= ES_WANTRETURN;
    else NewWindowStyle &= (DWORD) ~ES_WANTRETURN;
    if (FWindowStyle != NewWindowStyle) {
      FWindowStyle = NewWindowStyle;
      if (FHandle) {
        SetWindowLongPtr(FHandle, GWL_STYLE, FWindowStyle);
      }
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Lines
//---------------------------------------------------------------------------

TStrings *TMemo::Get_Lines(void) {
  return FLines;
}

bool TMemo::Set_Lines(TStrings *NewLines) {

  if (FLines != NewLines) {
    FLines->Assign(NewLines);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ScrollBars
//---------------------------------------------------------------------------

TScrollStyle TMemo::Get_ScrollBars(void) {
  switch (FWindowStyle & (WS_HSCROLL | WS_VSCROLL)) {
    case WS_HSCROLL: return ssHorizontal;
    case WS_VSCROLL: return ssVertical;
    case (WS_HSCROLL | WS_VSCROLL): return ssBoth;
  }

  return ssNone;
}

bool TMemo::Set_ScrollBars(TScrollStyle NewScrollBars) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle & (DWORD) ~(WS_HSCROLL | WS_VSCROLL);
  if (NewScrollBars == ssHorizontal) NewWindowStyle |= (DWORD) WS_HSCROLL;
  else if (NewScrollBars == ssVertical) NewWindowStyle |= (DWORD) WS_VSCROLL;
  else if (NewScrollBars == ssBoth) NewWindowStyle |= (DWORD) (WS_HSCROLL | WS_VSCROLL);
  if (FHandle) {
    SCROLLINFO ScrollInfo;
    memset(&ScrollInfo, 0, sizeof(SCROLLINFO));
    ScrollInfo.cbSize = sizeof(SCROLLINFO);

    // Horizontale
    ScrollInfo.fMask = SIF_ALL;
    GetScrollInfo(FHandle, SB_HORZ, &ScrollInfo);
    if (NewWindowStyle & WS_HSCROLL) {
      ScrollInfo.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
    }
    else {
      ScrollInfo.fMask = SIF_ALL;
      ScrollInfo.nMin = 0;
      ScrollInfo.nMax = 0;
      ScrollInfo.nPage = 0;
      ScrollInfo.nPos = 0;
      ScrollInfo.nTrackPos = 0;
    }
    SetScrollInfo(FHandle, SB_HORZ, &ScrollInfo, TRUE);

    // Verticale
    ScrollInfo.fMask = SIF_ALL;
    GetScrollInfo(FHandle, SB_VERT, &ScrollInfo);
    if (NewWindowStyle & WS_VSCROLL) {
      ScrollInfo.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
    }
    else {
      ScrollInfo.fMask = SIF_ALL;
      ScrollInfo.nMin = 0;
      ScrollInfo.nMax = 0;
      ScrollInfo.nPage = 0;
      ScrollInfo.nPos = 0;
      ScrollInfo.nTrackPos = 0;
    }
    SetScrollInfo(FHandle, SB_VERT, &ScrollInfo, TRUE);

  }

  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
void TMemo::Clear(void) {
  FLines->Clear();
}

//---------------------------------------------------------------------------
bool FASTCALL TMemo::CutToClipboard(void) {
	if (FHandle) SendMessage(FHandle, WM_CUT, 0, 0);

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TMemo::CopyToClipboard(void) {
	if (FHandle) SendMessage(FHandle, WM_COPY, 0, 0);

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TMemo::PasteFromClipboard(void) {
	if (FHandle) SendMessage(FHandle, WM_PASTE, 0, 0);

	return true;
}

//---------------------------------------------------------------------------

void FASTCALL TMemo::SelectAll(void) {
  if (FHandle) SendMessage(FHandle, EM_SETSEL, (WPARAM) 0, (LPARAM) -1);
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TMemo::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("WordWrap"));  // WordWrap doit rester en premier (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("Text"));
  ListProperties->Add(_T("ReadOnly"));
  ListProperties->Add(_T("WantReturns"));
  ListProperties->Add(_T("ScrollBars"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TMemo::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("WordWrap")) {
    return tpBool;
  }
  if (asProperty == _T("Text")) {
    return tpText;
  }
  if (asProperty == _T("ReadOnly")) {
    return tpBool;
  }
  if (asProperty == _T("WantReturns")) {
    return tpBool;
  }
  if (asProperty == _T("ScrollBars")) {
    *asInfos = _T("ssNone\nssHorizontal\nssVertical\nssBoth");
    return tpChoice;
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

AnsiString TMemo::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("WordWrap")) {
    return _T("True");
  }
  if (asProperty == _T("Text")) {
    return _T("");
  }
  if (asProperty == _T("ReadOnly")) {
    return _T("False");
  }
  if (asProperty == _T("WantReturns")) {
    return _T("True");
  }
  if (asProperty == _T("ScrollBars")) {
    return _T("ssBoth");
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

AnsiString TMemo::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("WordWrap")) {
    if (Get_WordWrap()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Text")) {
    return Get_Text();
  }
  if (asProperty == _T("ReadOnly")) {
    if (Get_ReadOnly()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("WantReturns")) {
    if (Get_WantReturns()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ScrollBars")) {
    TScrollStyle ScrollBars = Get_ScrollBars();
    if (ScrollBars == ssNone) return _T("ssNone");
    if (ScrollBars == ssHorizontal) return _T("ssHorizontal");
    if (ScrollBars == ssVertical) return _T("ssVertical");
    if (ScrollBars == ssBoth) return _T("ssBoth");
    return _T("ssNone");
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TMemo::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("WordWrap")) {
    Set_WordWrap(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Text")) {
    Set_Text(asValue);
    return true;
  }
  if (asProperty == _T("ReadOnly")) {
    Set_ReadOnly(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("WantReturns")) {
    Set_WantReturns(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ScrollBars")) {
    if (asValue == _T("ssNone")) Set_ScrollBars(ssNone);
    else if (asValue == _T("ssHorizontal")) Set_ScrollBars(ssHorizontal);
    else if (asValue == _T("ssVertical")) Set_ScrollBars(ssVertical);
    else if (asValue == _T("ssBoth")) Set_ScrollBars(ssBoth);
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

