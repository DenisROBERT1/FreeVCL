//-----------------------------------------------------------------------------
//! @file THotKey.cpp
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
// THotKey
//---------------------------------------------------------------------------

THotKey::THotKey(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("THotKey");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FExWindowStyle = WS_EX_CLIENTEDGE;
  FAutoSize = true;

  // Enregistrement de la classe HotKey
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_HOTKEY_CLASS;
  InitCommonControlsEx(&InitCtrls);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         HOTKEY_CLASS, FCaption,
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
THotKey::~THotKey(void) {
}

//---------------------------------------------------------------------------
bool THotKey::ProcessCommand(WORD Notify) {
  if (Notify == EN_CHANGE) {
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété HotKey
//---------------------------------------------------------------------------

TShortCut THotKey::Get_HotKey(void) {
  TShortCut ShortCut = 0;
  WORD bfMods = 0;

  if (FHandle) {
    unsigned int HotKeyAndMod = (unsigned int) SendMessage(FHandle, HKM_GETHOTKEY, 0, 0);
    ShortCut = LOBYTE(HotKeyAndMod);
    bfMods = HIBYTE(HotKeyAndMod);
  }

  if (bfMods & HOTKEYF_ALT) ShortCut |= 0x8000;
  if (bfMods & HOTKEYF_CONTROL) ShortCut |= 0x4000;
  if (bfMods & HOTKEYF_SHIFT) ShortCut |= 0x2000;
  if (bfMods & HOTKEYF_EXT) ShortCut |= 0x1000;

  return ShortCut;
}

bool THotKey::Set_HotKey(TShortCut NewHotKey) {
  if (FHandle) {
    unsigned int bfMods = 0;
    if (NewHotKey & 0x8000) bfMods |= HOTKEYF_ALT;
    if (NewHotKey & 0x4000) bfMods |= HOTKEYF_CONTROL;
    if (NewHotKey & 0x2000) bfMods |= HOTKEYF_SHIFT;
    if (NewHotKey & 0x1000) bfMods |= HOTKEYF_EXT;
    SendMessage(FHandle, HKM_SETHOTKEY, MAKEWORD(NewHotKey & 0xFF, bfMods), 0);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété InvalidKeys
//---------------------------------------------------------------------------

THKInvalidKeys THotKey::Get_InvalidKeys(void) {
  return FInvalidKeys;
}

bool THotKey::Set_InvalidKeys(THKInvalidKeys NewInvalidKeys) {
  if (FInvalidKeys != NewInvalidKeys) {
    FInvalidKeys = NewInvalidKeys;
    int fwCombInv = 0;
    if (NewInvalidKeys.Contains(hcNone)) fwCombInv |= HKCOMB_NONE;
    if (NewInvalidKeys.Contains(hcShift)) fwCombInv |= HKCOMB_S;
    if (NewInvalidKeys.Contains(hcCtrl)) fwCombInv |= HKCOMB_C;
    if (NewInvalidKeys.Contains(hcAlt)) fwCombInv |= HKCOMB_A;
    if (NewInvalidKeys.Contains(hcShiftCtrl)) fwCombInv |= HKCOMB_SC;
    if (NewInvalidKeys.Contains(hcShiftAlt)) fwCombInv |= HKCOMB_SA;
    if (NewInvalidKeys.Contains(hcCtrlAlt)) fwCombInv |= HKCOMB_CA;
    if (NewInvalidKeys.Contains(hcShiftCtrlAlt)) fwCombInv |= HKCOMB_SCA;
    SendMessage(FHandle, HKM_SETRULES, fwCombInv, MAKELPARAM(HOTKEYF_CONTROL, 0));
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Modifiers
//---------------------------------------------------------------------------

THKModifiers THotKey::Get_Modifiers(void) {
  THKModifiers Modifiers;

  if (FHandle) {
    unsigned int HotKeyAndMod = (unsigned int) SendMessage(FHandle, HKM_GETHOTKEY, 0, 0);
    unsigned int bfMods = HIBYTE(HotKeyAndMod);
    if (bfMods & HOTKEYF_SHIFT) Modifiers << hkShift;
    if (bfMods & HOTKEYF_CONTROL) Modifiers << hkCtrl;
    if (bfMods & HOTKEYF_ALT) Modifiers << hkAlt;
    if (bfMods & HOTKEYF_EXT) Modifiers << hkExt;
  }
  return Modifiers;
}

bool THotKey::Set_Modifiers(THKModifiers NewModifiers) {
  if (FHandle) {
    unsigned int HotKeyAndMod = (unsigned int) SendMessage(FHandle, HKM_GETHOTKEY, 0, 0);
    WORD bfMods = 0;
    if (NewModifiers.Contains(hkShift)) bfMods |= HOTKEYF_SHIFT;
    if (NewModifiers.Contains(hkCtrl)) bfMods |= HOTKEYF_CONTROL;
    if (NewModifiers.Contains(hkAlt)) bfMods |= HOTKEYF_ALT;
    if (NewModifiers.Contains(hkExt)) bfMods |= HOTKEYF_EXT;
    SendMessage(FHandle, HKM_SETHOTKEY, MAKEWORD(LOBYTE(HotKeyAndMod), bfMods), 0);
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void THotKey::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("HotKey"));
  ListProperties->Add(_T("InvalidKeys"));
  ListProperties->Add(_T("Modifiers"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY THotKey::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("HotKey")) {
    return tpNumber;
  }
  if (asProperty == _T("InvalidKeys")) {
    *asInfos = _T("hcNone\nhcShift\nhcCtrl\nhcAlt\nhcShiftCtrl\nhcShiftAlt\nhcCtrlAlt\nhcShiftCtrlAlt");
    return tpMultipleChoice;
  }
  if (asProperty == _T("Modifiers")) {
    *asInfos = _T("hkShift, hkCtrl, hkAlt, hkExt");
    return tpMultipleChoice;
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

AnsiString THotKey::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("HotKey")) {
    return _T("0");
  }
  if (asProperty == _T("InvalidKeys")) {
    return _T("");
  }
  if (asProperty == _T("Modifiers")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString THotKey::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("HotKey")) {
    return IntToStr(Get_HotKey());
  }
  if (asProperty == _T("InvalidKeys")) {
    AnsiString asInvalidKeys;
    THKInvalidKeys InvalidKeys = Get_InvalidKeys();
    if (InvalidKeys.Contains(hcNone)) asInvalidKeys += _T("hcNone\n");
    if (InvalidKeys.Contains(hcShift)) asInvalidKeys += _T("hcShift\n");
    if (InvalidKeys.Contains(hcCtrl)) asInvalidKeys += _T("hcCtrl\n");
    if (InvalidKeys.Contains(hcAlt)) asInvalidKeys += _T("hcAlt\n");
    if (InvalidKeys.Contains(hcShiftCtrl)) asInvalidKeys += _T("hcShiftCtrl\n");
    if (InvalidKeys.Contains(hcShiftAlt)) asInvalidKeys += _T("hcShiftAlt\n");
    if (InvalidKeys.Contains(hcCtrlAlt)) asInvalidKeys += _T("hcCtrlAlt\n");
    if (InvalidKeys.Contains(hcShiftCtrlAlt)) asInvalidKeys += _T("hcShiftCtrlAlt\n");
    return FormatProperty(asInvalidKeys, _T("["), _T("]"), _T(", "));
  }
  if (asProperty == _T("Modifiers")) {
    AnsiString asModifiers;
    THKModifiers Modifiers = Get_Modifiers();
    if (Modifiers.Contains(hkShift)) asModifiers += _T("hkShift\n");
    if (Modifiers.Contains(hkCtrl)) asModifiers += _T("hkCtrl\n");
    if (Modifiers.Contains(hkAlt)) asModifiers += _T("hkAlt\n");
    if (Modifiers.Contains(hkExt)) asModifiers += _T("hkExt\n");
    return FormatProperty(asModifiers, _T("["), _T("]"), _T(", "));
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool THotKey::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("HotKey")) {
    Set_HotKey(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("InvalidKeys")) {
    int Cur;
    AnsiString asMot;

    THKInvalidKeys NewInvalidKeys;
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("hcNone")) NewInvalidKeys << hcNone;
      else if (asMot == _T("hcShift")) NewInvalidKeys << hcShift;
      else if (asMot == _T("hcCtrl")) NewInvalidKeys << hcCtrl;
      else if (asMot == _T("hcAlt")) NewInvalidKeys << hcAlt;
      else if (asMot == _T("hcShiftCtrl")) NewInvalidKeys << hcShiftCtrl;
      else if (asMot == _T("hcShiftAlt")) NewInvalidKeys << hcShiftAlt;
      else if (asMot == _T("hcCtrlAlt")) NewInvalidKeys << hcCtrlAlt;
      else if (asMot == _T("hcShiftCtrlAlt")) NewInvalidKeys << hcShiftCtrlAlt;
    } while (!asMot.IsEmpty());
    Set_InvalidKeys(NewInvalidKeys);

    return true;
  }
  if (asProperty == _T("Modifiers")) {
    int Cur;
    AnsiString asMot;

    THKModifiers NewModifiers;
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("hkShift")) NewModifiers << hkShift;
      else if (asMot == _T("hkCtrl")) NewModifiers << hkCtrl;
      else if (asMot == _T("hkAlt")) NewModifiers << hkAlt;
      else if (asMot == _T("hkExt")) NewModifiers << hkExt;
    } while (!asMot.IsEmpty());
    Set_Modifiers(NewModifiers);

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

