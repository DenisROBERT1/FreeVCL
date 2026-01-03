//-----------------------------------------------------------------------------
//! @file TComboBox.cpp
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
// TComboBox
//---------------------------------------------------------------------------

TComboBox::TComboBox(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TComboBox");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= WS_VSCROLL | CBS_DROPDOWN | CBS_AUTOHSCROLL;
  FItems = new TComboBoxItems();
	SET_EVENT(FItems, TNotifyEvent, OnChange, TComboBox, this, FItemsChange);
  FItemIndex = -1;
  FItemHeight = 15;
  FSelStart = 0;
  FSelLength = -1;
  FTabStop = true;
  FColor = clWindow;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_COMBOBOX, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight + 100,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    FItems->Handle = FHandle;

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TComboBox::~TComboBox(void) {
  delete FItems;
}

//---------------------------------------------------------------------------
void FASTCALL TComboBox::DestroyWnd(void) {
  DestroyWindow(FHandle);
	FHandle = NULL;
}

//---------------------------------------------------------------------------
void FASTCALL TComboBox::CreateWnd(void) {
  HFONT hFont;
  HWND hWndParent = NULL;


  if (FOwner) hWndParent = ((TControl *) FOwner)->Handle;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                           WC_COMBOBOX, FCaption,
                           FWindowStyle,
                           FLeft, FTop,
                           FWidth, FHeight + 100,
                           hWndParent,
                           (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    FItems->Handle = FHandle;
    InitStdControl();
  }

}

//---------------------------------------------------------------------------
bool TComboBox::ProcessCommand(WORD Notify) {
  TCHAR *szBuf;
  int Length;


  if (Notify == CBN_EDITCHANGE) {
    FItemIndex = -1;
    Length = GetWindowTextLength(FHandle) + 1;
    szBuf = new TCHAR[Length];
    GetWindowText(FHandle, szBuf, Length);
    FText = szBuf;
    delete[] szBuf;
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return true;
  }
  if (Notify == CBN_SELCHANGE) {
    FItemIndex = (int) SendMessage(FHandle, CB_GETCURSEL, 0, 0);
    if (FItemIndex != -1) {
      Length = (int) SendMessage(FHandle, CB_GETLBTEXTLEN, (WPARAM) FItemIndex, 0) + 1;
      szBuf = new TCHAR[Length];
      SendMessage(FHandle, CB_GETLBTEXT, (WPARAM) FItemIndex, (LPARAM) szBuf);
      FText = szBuf;
      delete[] szBuf;
    }
    else FText = _T("");
    if (!FComponentState.Contains(csLoading)) {
      OnSelect(this);
      OnChange(this);
    }
    return true;
  }
  if (Notify == CBN_DROPDOWN) {
    if (!FComponentState.Contains(csLoading)) OnDropDown(this);
    return true;
  }
  if (Notify == CBN_CLOSEUP) {
    if (!FComponentState.Contains(csLoading)) OnCloseUp(this);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
bool TComboBox::ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS) {

  OnMeasureItem(this, (int) lpMIS->itemID, &FItemHeight);
  lpMIS->itemHeight = FItemHeight;
  lpMIS->itemWidth = FWidth - 21;

  return true;
}

//---------------------------------------------------------------------------
bool TComboBox::ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS) {
  TRect RectItem(lpDIS->rcItem);
  TOwnerDrawStates OwnerDrawStates;


  OwnerDrawStates.Clear();

	if (lpDIS->itemState & ODS_CHECKED) OwnerDrawStates << odChecked;
	if (lpDIS->itemState & ODS_COMBOBOXEDIT) OwnerDrawStates << odComboBoxEdit;
	if (lpDIS->itemState & ODS_DEFAULT) OwnerDrawStates << odDefault;
	if (lpDIS->itemState & ODS_DISABLED) OwnerDrawStates << odDisabled;
	if (lpDIS->itemState & ODS_FOCUS) OwnerDrawStates << odFocused;
	if (lpDIS->itemState & ODS_GRAYED) OwnerDrawStates << odGrayed;
	if (lpDIS->itemState & ODS_HOTLIGHT) OwnerDrawStates << odHotLight;
	if (lpDIS->itemState & ODS_NOFOCUSRECT) OwnerDrawStates << odNoFocusRect;
	if (lpDIS->itemState & ODS_SELECTED) OwnerDrawStates << odSelected;

	Canvas->Handle = lpDIS->hDC;
	// Le type de itemID est un UINT mais la documentation MSDN dit qu'il
	// peut contenir -1... C'est dont en fait un int.
	OnDrawItem(this, (int) lpDIS->itemID, RectItem, OwnerDrawStates);
	FCanvas->FreeHandle();

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

bool TComboBox::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
    if (FHandle) MoveWindow(FHandle, FLeft, FTop, FWidth, FHeight + 100, FALSE);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TComboBoxStyle TComboBox::Get_Style(void) {
  // L'ordre a une importance (car CBS_DROPDOWNLIST = CBS_SIMPLE | CBS_DROPDOWN)
  if (FWindowStyle & CBS_DROPDOWNLIST) return csDropDownList;
  else if (FWindowStyle & CBS_DROPDOWN) return csDropDown;
  else if (FWindowStyle & CBS_SIMPLE) return csSimple;
  else if (FWindowStyle & CBS_OWNERDRAWFIXED) return csOwnerDrawFixed;
  else if (FWindowStyle & CBS_OWNERDRAWVARIABLE) return csOwnerDrawVariable;
  else return csDropDown;
}

bool TComboBox::Set_Style(TComboBoxStyle NewStyle) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  NewWindowStyle &= ~(CBS_DROPDOWN | CBS_SIMPLE | CBS_DROPDOWNLIST |
                      CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE);
  if (NewStyle == csDropDown) NewWindowStyle |= CBS_DROPDOWN;
  else if (NewStyle == csSimple) NewWindowStyle |= CBS_SIMPLE;
  else if (NewStyle == csDropDownList) NewWindowStyle |= CBS_DROPDOWNLIST;
  else if (NewStyle == csOwnerDrawFixed) NewWindowStyle |= CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS;
  else if (NewStyle == csOwnerDrawVariable) NewWindowStyle |= CBS_DROPDOWNLIST | CBS_OWNERDRAWVARIABLE | CBS_HASSTRINGS;
  // ChangeWindowStyle(NewWindowStyle);  // Ca marche pô
  if (FWindowStyle != NewWindowStyle) {
		bool bReCreate = (FHandle != NULL);
    if (bReCreate) DestroyWnd();
    FWindowStyle = NewWindowStyle;
    if (bReCreate) CreateWnd();
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TComboBox::Get_Text(void) {
  return FText;
}

bool TComboBox::Set_Text(AnsiString NewText) {
  FText = NewText;
  if (FHandle) {
    if ((FWindowStyle & (CBS_DROPDOWN | CBS_SIMPLE | CBS_DROPDOWNLIST)) == CBS_DROPDOWNLIST) {
      FItemIndex = (int) SendMessage(FHandle, CB_FINDSTRINGEXACT, (WPARAM) -1, (LPARAM) (LPCTSTR) NewText);
      SendMessage(FHandle, CB_SETCURSEL, FItemIndex, 0);
    }
    else {
      SetWindowText(FHandle, NewText);
    }
    SendMessage(FHandle, CB_SETEDITSEL, 0, MAKELPARAM(-2, -2));
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemIndex
//---------------------------------------------------------------------------

int TComboBox::Get_ItemIndex(void) {
  return FItemIndex;
}

bool TComboBox::Set_ItemIndex(int NewItemIndex) {

	if (NewItemIndex < -1) NewItemIndex = -1;
	if (NewItemIndex >= FItems->Count) NewItemIndex = FItems->Count - 1;

  if (FItemIndex != NewItemIndex) {
    FItemIndex = NewItemIndex;
    if (FHandle) {
      SendMessage(FHandle, CB_SETCURSEL, NewItemIndex, 0);
      ProcessCommand(CBN_SELCHANGE);  // Non envoyé par le système
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemHeight
//---------------------------------------------------------------------------

int TComboBox::Get_ItemHeight(void) {
  return FItemHeight;
}

bool TComboBox::Set_ItemHeight(int NewItemHeight) {

  if (FItemHeight != NewItemHeight) {
    FItemHeight = NewItemHeight;
    if (FHandle) SendMessage(FHandle, CB_SETITEMHEIGHT, (WPARAM) -1, NewItemHeight);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Items
//---------------------------------------------------------------------------

TStrings *TComboBox::Get_Items(void) {
  return FItems;
}

bool TComboBox::Set_Items(TStrings *NewItems) {

  if (FItems != NewItems) {
    FItems->Assign(NewItems);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelStart
//---------------------------------------------------------------------------

int TComboBox::Get_SelStart(void) {
  return FSelStart;
}

bool TComboBox::Set_SelStart(int NewSelStart) {
  if (FSelStart != NewSelStart) {
    FSelStart = NewSelStart;
    FSelLength = 0;
    if (FHandle) {
      SendMessage(FHandle, CB_SETEDITSEL, (WPARAM) FSelStart,
                                          (LPARAM) FSelStart + FSelLength);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelLength
//---------------------------------------------------------------------------

int TComboBox::Get_SelLength(void) {
  return FSelLength;
}

bool TComboBox::Set_SelLength(int NewSelLength) {
  if (FSelLength != NewSelLength) {
    FSelLength = NewSelLength;
    if (FHandle) {
      SendMessage(FHandle, CB_SETEDITSEL, (WPARAM) FSelStart,
                                          (LPARAM) FSelStart + FSelLength);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Traitement de l'évènement de modification d'item
//---------------------------------------------------------------------------
void FASTCALL TComboBox::FItemsChange(TObject *Sender) {
  int NewItemIndex = (int) SendMessage(FHandle, CB_GETCURSEL, 0, 0);
	if (FItemIndex != NewItemIndex) {
		FItemIndex = NewItemIndex;
		if (!FComponentState.Contains(csLoading)) OnChange(this);
	}
}

//---------------------------------------------------------------------------
// Ajoute un élément dans la liste déroulante.
//---------------------------------------------------------------------------
void FASTCALL TComboBox::AddItem(const AnsiString S, TObject *Object) {
	FItems->AddObject(S, Object);
}

//---------------------------------------------------------------------------
// Vide le contenu de la liste déroulante.
//---------------------------------------------------------------------------
void FASTCALL TComboBox::Clear(void) {
	FItems->Clear();
	FItemIndex = -1;
}

//---------------------------------------------------------------------------
// Désélectionne un élément sélectionné dans la liste déroulante.
//---------------------------------------------------------------------------
void FASTCALL TComboBox::ClearSelection(void) {
	if (FItems->IndexOf(FText) != -1) Set_ItemIndex(-1);
}

//---------------------------------------------------------------------------
// Destruction de l'élément sélectionné dans la liste déroulante.
//---------------------------------------------------------------------------
void FASTCALL TComboBox::DeleteSelected(void) {
	if (FItemIndex != -1) FItems->Delete(FItemIndex);
}

//---------------------------------------------------------------------------
// Sélection du texte complet de la zone d'édition.
//---------------------------------------------------------------------------
void FASTCALL TComboBox::SelectAll(void) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TComboBox::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Style"));  // Style doit rester en premier (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("Items.Strings"));
  ListProperties->Add(_T("ItemIndex"));
  ListProperties->Add(_T("ItemHeight"));
  ListProperties->Add(_T("SelStart"));
  ListProperties->Add(_T("SelLength"));
  ListProperties->Add(_T("Text"));
  ListProperties->Add(_T("OnChange"));
  ListProperties->Add(_T("OnSelect"));
  ListProperties->Add(_T("OnDropDown"));
  ListProperties->Add(_T("OnCloseUp"));
  ListProperties->Add(_T("OnMeasureItem"));
  ListProperties->Add(_T("OnDrawItem"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TComboBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {

  if (asProperty == _T("Style")) {
    *asInfos = _T("csDropDown\ncsSimple\ncsDropDownList\ncsOwnerDrawFixed\ncsOwnerDrawVariable");
    return tpChoice;
  }
  if (asProperty == _T("Items.Strings")) {
    return tpStrings;
  }
  if (asProperty == _T("ItemIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("ItemHeight")) {
    return tpNumber;
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
  if (asProperty == _T("OnSelect")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnDropDown")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnCloseUp")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnMeasureItem")) {
    *asInfos = _T("(TObject *Sender, unsigned int Index, int *Height)");
    return tpEvent;
  }
  if (asProperty == _T("OnDrawItem")) {
    *asInfos = _T("(TObject *Sender, unsigned int Index, const TRect *Rect, TOwnerDrawStates States)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TComboBox::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Style")) {
    return _T("csDropDown");
  }
  if (asProperty == _T("Items.Strings")) {
    return _T("");
  }
  if (asProperty == _T("ItemIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("ItemHeight")) {
    return _T("15");
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
  if (asProperty == _T("OnSelect")) {
    return _T("");
  }
  if (asProperty == _T("OnDropDown")) {
    return _T("");
  }
  if (asProperty == _T("OnCloseUp")) {
    return _T("");
  }
  if (asProperty == _T("OnMeasureItem")) {
    return _T("");
  }
  if (asProperty == _T("OnDrawItem")) {
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

AnsiString TComboBox::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Style")) {
    TComboBoxStyle Style = Get_Style();
    if (Style == csDropDown) return _T("csDropDown");
    if (Style == csSimple) return _T("csSimple");
    if (Style == csDropDownList) return _T("csDropDownList");
    if (Style == csOwnerDrawFixed) return _T("csOwnerDrawFixed");
    if (Style == csOwnerDrawVariable) return _T("csOwnerDrawVariable");
    return _T("");
  }
  if (asProperty == _T("Items.Strings")) {
    return FItems->GetTextStr();
  }
  if (asProperty == _T("ItemIndex")) {
    return IntToStr(Get_ItemIndex());
  }
  if (asProperty == _T("ItemHeight")) {
    return IntToStr(Get_ItemHeight());
  }
  if (asProperty == _T("Text")) {
    return Get_Text();
  }
  if (asProperty == _T("SelStart")) {
    return IntToStr(Get_SelStart());
  }
  if (asProperty == _T("SelLength")) {
    return IntToStr(Get_SelLength());
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  if (asProperty == _T("OnSelect")) {
    return OnSelect.AsString();
  }
  if (asProperty == _T("OnDropDown")) {
    return OnDropDown.AsString();
  }
  if (asProperty == _T("OnCloseUp")) {
    return OnCloseUp.AsString();
  }
  if (asProperty == _T("OnMeasureItem")) {
    return OnMeasureItem.AsString();
  }
  if (asProperty == _T("OnDrawItem")) {
    return OnDrawItem.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TComboBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Style")) {
    if (asValue == _T("csDropDown")) Set_Style(csDropDown);
    else if (asValue == _T("csSimple")) Set_Style(csSimple);
    else if (asValue == _T("csDropDownList")) Set_Style(csDropDownList);
    else if (asValue == _T("csOwnerDrawFixed")) Set_Style(csOwnerDrawFixed);
    else if (asValue == _T("csOwnerDrawVariable")) Set_Style(csOwnerDrawVariable);
  }
  if (asProperty == _T("Items.Strings")) {
    FItems->SetTextStr(asValue);
    return true;
  }
  if (asProperty == _T("ItemIndex")) {
    Set_ItemIndex(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("ItemHeight")) {
    Set_ItemHeight(StrToInt(asValue));
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
  if (asProperty == _T("OnSelect")) {
		OnSelect.ObjCall = Sender;
    OnSelect = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnDropDown")) {
		OnDropDown.ObjCall = Sender;
    OnDropDown = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnCloseUp")) {
		OnCloseUp.ObjCall = Sender;
    OnCloseUp = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnMeasureItem")) {
		OnMeasureItem.ObjCall = Sender;
    OnMeasureItem = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnDrawItem")) {
		OnDrawItem.ObjCall = Sender;
    OnDrawItem = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

