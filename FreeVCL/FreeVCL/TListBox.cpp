//-----------------------------------------------------------------------------
//! @file TListBox.cpp
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
// TListBox
//---------------------------------------------------------------------------

TListBox::TListBox(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TListBox");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_VSCROLL;
  FExWindowStyle = WS_EX_CLIENTEDGE;
  FItems = new TListBoxItems();
	SET_EVENT(FItems, TNotifyEvent, OnChange, TListBox, this, FItemsChange);
  FItemIndex = -1;
  FItemHeight = 15;
  FTabStop = true;
  FColor = clWindow;
	FBorderStyle = bsSingle;
  FExtendedSelect = true;
	FColumns = 1;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_LISTBOX, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
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
TListBox::~TListBox(void) {
  delete FItems;
}

//---------------------------------------------------------------------------
void FASTCALL TListBox::DestroyWnd(void) {
  DestroyWindow(FHandle);
	FHandle = NULL;
}

//---------------------------------------------------------------------------
void FASTCALL TListBox::CreateWnd(void) {
  HFONT hFont;
  HWND hWndParent = NULL;


  if (FOwner) hWndParent = ((TControl *) FOwner)->Handle;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                           WC_LISTBOX, FCaption,
                           FWindowStyle,
                           FLeft, FTop,
                           FWidth, FHeight,
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
bool TListBox::ProcessCommand(WORD Notify) {

  if (Notify == LBN_SELCHANGE) {
    FItemIndex = (int) SendMessage(FHandle, LB_GETCURSEL, 0, 0);
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
bool TListBox::ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS) {

  OnMeasureItem(this, (int) lpMIS->itemID, &FItemHeight);
  lpMIS->itemHeight = FItemHeight;
  lpMIS->itemWidth = FWidth - 21;

  return true;
}

//---------------------------------------------------------------------------
bool TListBox::ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS) {
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
bool TListBox::ProcessLButtonUp(int X, int Y) {
  FItemIndex = (int) SendMessage(FHandle, LB_GETCURSEL, 0, 0);
  bool bRetour = TWinControl::ProcessLButtonUp(X, Y);
	if (!FComponentState.Contains(csLoading)) {
		if (FAction) FAction->Execute();
		OnClick(this);
	}

  return bRetour;
}

//---------------------------------------------------------------------------
bool TListBox::ProcessHScroll(int Command, int Info) {
	// Bug (?) avec windows 7: les lignes sont parfois mal redessinées
	if (Command == SB_ENDSCROLL) {
		Invalidate();
	}

  return false;
}

//---------------------------------------------------------------------------
bool TListBox::ProcessVScroll(int Command, int Info) {
	// Bug (?) avec windows 7: les lignes sont parfois mal redessinées
	if (Command == SB_ENDSCROLL) {
		Invalidate();
	}

	return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TListBoxStyle TListBox::Get_Style(void) {
  if (FWindowStyle & LBS_OWNERDRAWFIXED) return lbOwnerDrawFixed;
  else if (FWindowStyle & LBS_OWNERDRAWVARIABLE) return lbOwnerDrawVariable;
  else return lbStandard;
}

bool TListBox::Set_Style(TListBoxStyle NewStyle) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  NewWindowStyle &= ~(LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE);
  if (NewStyle == lbOwnerDrawFixed) NewWindowStyle |= LBS_OWNERDRAWFIXED | LBS_HASSTRINGS;
  else if (NewStyle == lbOwnerDrawVariable) NewWindowStyle |= LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS;
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
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TBorderStyle TListBox::Get_BorderStyle(void) {
	return FBorderStyle;
}

bool TListBox::Set_BorderStyle(TBorderStyle NewBorderStyle) {
	DWORD NewWindowStyle;
	DWORD NewExWindowStyle;


	if (FBorderStyle != NewBorderStyle) {
		FBorderStyle = NewBorderStyle;
		if (FHandle) {
			NewWindowStyle = (FWindowStyle & ~WS_BORDER);
			NewExWindowStyle = (FExWindowStyle & ~WS_EX_CLIENTEDGE);
			if (FBorderStyle == bsSingle) {
				NewWindowStyle |= WS_BORDER;
				NewExWindowStyle |= WS_EX_CLIENTEDGE;
			}
			ChangeWindowStyle(NewWindowStyle);
			ChangeExWindowStyle(NewExWindowStyle);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiSelect
//---------------------------------------------------------------------------

bool TListBox::Get_MultiSelect(void) {
  return ((FWindowStyle & LBS_MULTIPLESEL) != 0);
}

bool TListBox::Set_MultiSelect(bool NewMultiSelect) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewMultiSelect) {
    NewWindowStyle |= LBS_MULTIPLESEL;
    if (FExtendedSelect) NewWindowStyle |= LBS_EXTENDEDSEL;
  }
  else {
    NewWindowStyle &= (DWORD) ~(LBS_MULTIPLESEL | LBS_EXTENDEDSEL);
  }
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
// Accesseurs de la propriété ExtendedSelect
//---------------------------------------------------------------------------

bool TListBox::Get_ExtendedSelect(void) {
  return FExtendedSelect;
}

bool TListBox::Set_ExtendedSelect(bool NewExtendedSelect) {

  if (FExtendedSelect != NewExtendedSelect) {
    FExtendedSelect = NewExtendedSelect;
    if (FWindowStyle & LBS_MULTIPLESEL) {
      DWORD NewWindowStyle;

      NewWindowStyle = FWindowStyle;
      if (NewExtendedSelect) NewWindowStyle |= LBS_EXTENDEDSEL;
      else NewWindowStyle &= (DWORD) ~LBS_EXTENDEDSEL;
      // ChangeWindowStyle(NewWindowStyle);  // Ca marche pô
      if (FWindowStyle != NewWindowStyle) {
				bool bReCreate = FHandle != 0;
				if (bReCreate) DestroyWnd();
				FWindowStyle = NewWindowStyle;
				if (bReCreate) CreateWnd();
      }
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Selected
//---------------------------------------------------------------------------

bool TListBox::Get_Selected(int i) {
  if (FHandle) {
    return (SendMessage(FHandle, LB_GETSEL, (WPARAM) i, 0) > 0);
  }

  return false;
}


bool TListBox::Set_Selected(int i, bool NewSelected) {
  if (FHandle) SendMessage(FHandle, LB_SETSEL, (WPARAM) NewSelected, (LPARAM) i);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SelCount
//---------------------------------------------------------------------------

int TListBox::Get_SelCount(void) {

  if (FHandle) {
    return (int) SendMessage(FHandle, LB_GETSELCOUNT, 0, 0);
  }

  return 0;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Sorted
//---------------------------------------------------------------------------

bool TListBox::Get_Sorted(void) {
  return ((FWindowStyle & LBS_SORT) == 0);
}


bool TListBox::Set_Sorted(bool NewSorted) {
  DWORD NewWindowStyle;

  NewWindowStyle = FWindowStyle;
  if (NewSorted) NewWindowStyle |= LBS_SORT;
  else NewWindowStyle &= (DWORD) ~(LBS_SORT);
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
// Accesseurs de la propriété TopIndex
//---------------------------------------------------------------------------

int TListBox::Get_TopIndex(void) {

  if (FHandle) {
    return (int) SendMessage(FHandle, LB_GETTOPINDEX, 0, 0);
  }

  return 0;
}

bool TListBox::Set_TopIndex(int NewTopIndex) {

  if (FHandle) {
		SendMessage(FHandle, LB_SETTOPINDEX, NewTopIndex, 0);
		// Bug (?) avec windows 7: les lignes sont parfois mal redessinées
		Invalidate();
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemIndex
//---------------------------------------------------------------------------

int TListBox::Get_ItemIndex(void) {
  return FItemIndex;
}

bool TListBox::Set_ItemIndex(int NewItemIndex) {

	if (NewItemIndex < -1) NewItemIndex = -1;
	if (NewItemIndex >= FItems->Count) NewItemIndex = FItems->Count - 1;

  if (FItemIndex != NewItemIndex) {
    FItemIndex = NewItemIndex;
    if (FHandle) {
      if ((FWindowStyle & LBS_MULTIPLESEL) == 0) {
        SendMessage(FHandle, LB_SETCURSEL, NewItemIndex, 0);
      }
      else {
        SendMessage(FHandle, LB_SETSEL, (WPARAM) FALSE, (LPARAM) -1);
        SendMessage(FHandle, LB_SETSEL, (WPARAM) TRUE, (LPARAM) NewItemIndex);
      }
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemHeight
//---------------------------------------------------------------------------

int TListBox::Get_ItemHeight(void) {
  return FItemHeight;
}

bool TListBox::Set_ItemHeight(int NewItemHeight) {

  if (FItemHeight != NewItemHeight) {
    FItemHeight = NewItemHeight;
    if (FHandle) SendMessage(FHandle, LB_SETITEMHEIGHT, (WPARAM) -1, NewItemHeight);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Items
//---------------------------------------------------------------------------

TStrings *TListBox::Get_Items(void) {
  return FItems;
}

bool TListBox::Set_Items(TStrings *NewItems) {

  if (FItems != NewItems) {
    FItems->Assign(NewItems);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TListBox::Get_Count(void) {
  return (int) FItems->Count;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Columns
//---------------------------------------------------------------------------

int TListBox::Get_Columns() {
  return FColumns;
}

bool TListBox::Set_Columns(int NewColumns) {
  DWORD NewWindowStyle;


  if (FColumns != NewColumns) {
    FColumns = NewColumns;
	  NewWindowStyle = FWindowStyle;
		if (FColumns > 1) {
			NewWindowStyle |= LBS_MULTICOLUMN;
		}
		else {
			NewWindowStyle &= (DWORD) ~LBS_MULTICOLUMN;
		}
	  // ChangeWindowStyle(NewWindowStyle);  // Ca marche pô
		if (FWindowStyle != NewWindowStyle) {
			bool bReCreate = (FHandle != NULL);
			if (bReCreate) DestroyWnd();
			FWindowStyle = NewWindowStyle;
			if (bReCreate) CreateWnd();
		}
		if (FHandle && FColumns > 1) {
			SendMessage(FHandle, LB_SETCOLUMNWIDTH, FWidth / FColumns, 0);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Traitement de l'évènement de modification d'item
//---------------------------------------------------------------------------
void FASTCALL TListBox::FItemsChange(TObject *Sender) {
  int NewItemIndex = (int) SendMessage(FHandle, LB_GETCURSEL, 0, 0);
	if (FItemIndex != NewItemIndex) {
		FItemIndex = NewItemIndex;
		if (!FComponentState.Contains(csLoading)) OnChange(this);
	}
}

//---------------------------------------------------------------------------
// Effacement de la list-box
//---------------------------------------------------------------------------

void TListBox::Clear(void) {
  FItems->Clear();
}


//---------------------------------------------------------------------------
// Effacement de la sélection
//---------------------------------------------------------------------------

void FASTCALL TListBox::ClearSelection(void) {
  if (FHandle) SendMessage(FHandle, LB_SETSEL, (WPARAM) FALSE, (LPARAM) -1);
}

//---------------------------------------------------------------------------
// Sélectionne tous les items de la list-box
//---------------------------------------------------------------------------

void FASTCALL TListBox::SelectAll(void) {
  if (FHandle) SendMessage(FHandle, LB_SETSEL, (WPARAM) TRUE, (LPARAM) -1);
}

//---------------------------------------------------------------------------
// Item à une position donnée
//---------------------------------------------------------------------------

int TListBox::ItemAtPos(TPoint &Pos, bool Existing) {
  int Retour = -1;


  if (FHandle) {
    Retour = (int) SendMessage(FHandle, LB_ITEMFROMPOINT, 0, MAKELPARAM(Pos.x, Pos.y));
    if (Retour & 0x10000) {
      if (Existing) Retour = -1;
      else Retour = FItems->Count + 1;
    }
  }

  return Retour;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TListBox::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Style"));  // Style doit rester en premier (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("MultiSelect"));  // MultiSelect doit rester en début (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("Sorted"));  // Sorted doit rester en début (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("Columns"));  // Columns doit rester en début (parce que ça provoque un CreateWnd)
	ListProperties->Add(_T("BorderStyle"));
  ListProperties->Add(_T("ExtendedSelect"));
  ListProperties->Add(_T("SelCount"));
  ListProperties->Add(_T("TopIndex"));
  ListProperties->Add(_T("ItemIndex"));
  ListProperties->Add(_T("ItemHeight"));
  ListProperties->Add(_T("Items.Strings"));
  ListProperties->Add(_T("OnChange"));
  ListProperties->Add(_T("OnMeasureItem"));
  ListProperties->Add(_T("OnDrawItem"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TListBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Style")) {
    *asInfos = _T("lbStandard\nlbOwnerDrawFixed\nlbOwnerDrawVariable\nlbVirtual\nlbVirtualOwnerDraw");
    return tpChoice;
  }
  if (asProperty == _T("MultiSelect")) {
    return tpBool;
  }
  if (asProperty == _T("Sorted")) {
    return tpBool;
  }
  if (asProperty == _T("Columns")) {
    return tpNumber;
  }
	if (asProperty == _T("BorderStyle")) {
		*asInfos = _T("bsNone\nbsSingle");
		return tpChoice;
	}
  if (asProperty == _T("ExtendedSelect")) {
    return tpBool;
  }
  if (asProperty == _T("SelCount")) {
    return tpNumber;
  }
  if (asProperty == _T("TopIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("ItemIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("ItemHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("Items.Strings")) {
    return tpStrings;
  }
  if (asProperty == _T("OnChange")) {
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

AnsiString TListBox::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Style")) {
    return _T("lbStandard");
  }
  if (asProperty == _T("MultiSelect")) {
    return _T("False");
  }
  if (asProperty == _T("Sorted")) {
    return _T("False");
  }
  if (asProperty == _T("Columns")) {
    return _T("1");
  }
	if (asProperty == _T("BorderStyle")) {
		return _T("bsSingle");
	}
  if (asProperty == _T("ExtendedSelect")) {
    return _T("True");
  }
  if (asProperty == _T("SelCount")) {
    return _T("-1");
  }
  if (asProperty == _T("TopIndex")) {
    return _T("0");
  }
  if (asProperty == _T("ItemIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("ItemHeight")) {
    return _T("15");
  }
  if (asProperty == _T("Items.Strings")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
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

AnsiString TListBox::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Style")) {
    TListBoxStyle Style = Get_Style();
    if (Style == lbStandard) return _T("lbStandard");
    if (Style == lbOwnerDrawFixed) return _T("lbOwnerDrawFixed");
    if (Style == lbOwnerDrawVariable) return _T("lbOwnerDrawVariable");
    if (Style == lbVirtual) return _T("lbVirtual");
    if (Style == lbVirtualOwnerDraw) return _T("lbVirtualOwnerDraw");
    return _T("");
  }
  if (asProperty == _T("MultiSelect")) {
    if (Get_MultiSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Sorted")) {
    if (Get_Sorted()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Columns")) {
    return IntToStr(Get_Columns());
  }
	if (asProperty == _T("BorderStyle")) {
		TBorderStyle BorderStyle = Get_BorderStyle();
		if (BorderStyle == bsNone) return _T("bsNone");
		if (BorderStyle == bsSingle) return _T("bsSingle");
		return _T("");
	}
  if (asProperty == _T("ExtendedSelect")) {
    if (Get_ExtendedSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("SelCount")) {
    return IntToStr(Get_SelCount());
  }
  if (asProperty == _T("TopIndex")) {
    return IntToStr(Get_TopIndex());
  }
  if (asProperty == _T("ItemIndex")) {
    return IntToStr(Get_ItemIndex());
  }
  if (asProperty == _T("ItemHeight")) {
    return IntToStr(Get_ItemHeight());
  }
  if (asProperty == _T("Items.Strings")) {
    return FItems->GetTextStr();
  }
  if (asProperty == _T("Columns")) {
    return IntToStr(Get_Columns());
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
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

bool TListBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Style")) {
    if (asValue == _T("lbStandard")) Set_Style(lbStandard);
    else if (asValue == _T("lbOwnerDrawFixed")) Set_Style(lbOwnerDrawFixed);
    else if (asValue == _T("lbOwnerDrawVariable")) Set_Style(lbOwnerDrawVariable);
    else if (asValue == _T("lbVirtual")) Set_Style(lbVirtual);
    else if (asValue == _T("lbVirtualOwnerDraw")) Set_Style(lbVirtualOwnerDraw);
		return true;
  }
  if (asProperty == _T("MultiSelect")) {
    Set_MultiSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Sorted")) {
    Set_Sorted(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Columns")) {
    Set_Columns(asValue.ToIntDef(0));
    return true;
  }
	if (asProperty == _T("BorderStyle")) {
		if (asValue == _T("bsNone")) Set_BorderStyle(bsNone);
		else if (asValue == _T("bsSingle")) Set_BorderStyle(bsSingle);
		return true;
	}
  if (asProperty == _T("ExtendedSelect")) {
    Set_ExtendedSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("TopIndex")) {
    Set_TopIndex(asValue.ToInt());
    return true;
  }
  if (asProperty == _T("ItemIndex")) {
    Set_ItemIndex(asValue.ToInt());
    return true;
  }
  if (asProperty == _T("ItemHeight")) {
    Set_ItemHeight(asValue.ToInt());
    return true;
  }
  if (asProperty == _T("Items.Strings")) {
    FItems->SetTextStr(asValue);
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
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

