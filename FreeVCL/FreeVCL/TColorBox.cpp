//-----------------------------------------------------------------------------
//! @file TColorBox.cpp
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
// Variables globales
//---------------------------------------------------------------------------

const TCHAR * szListColorNames[] = {
	_T("clNone"),
	_T("clAqua"),
	_T("clBlack"),
	_T("clBlue"),
	_T("clDkGray"),
	_T("clFuchsia"),
	_T("clGray"),
	_T("clGreen"),
	_T("clLime"),
	_T("clLtGray"),
	_T("clMaroon"),
	_T("clNavy"),
	_T("clOlive"),
	_T("clPurple"),
	_T("clRed"),
	_T("clSilver"),
	_T("clTeal"),
	_T("clWhite"),
	_T("clYellow"),
	_T("clMoneyGreen"),
	_T("clSkyBlue"),
	_T("clCream"),
	_T("clMedGray"),
	_T("clScrollBar"),
	_T("clBackground"),
	_T("clActiveCaption"),
	_T("clInactiveCaption"),
	_T("clMenu"),
	_T("clWindow"),
	_T("clWindowFrame"),
	_T("clMenuText"),
	_T("clWindowText"),
	_T("clCaptionText"),
	_T("clActiveBorder"),
	_T("clInactiveBorder"),
	_T("clAppWorkSpace"),
	_T("clHighlight"),
	_T("clHighlightText"),
	_T("clBtnFace"),
	_T("clBtnShadow"),
	_T("clGrayText"),
	_T("clBtnText"),
	_T("clInactiveCaptionText,"),
	_T("clBtnHighlight"),
	_T("cl3DDkShadow"),
	_T("cl3DLight"),
	_T("clInfoText"),
	_T("clInfoBk"),
	_T("clGradientActiveCaption"),
	_T("clGradientInactiveCaption"),
	_T("clDefault")
};

const TCHAR * szListColorPrettyNames[] = {
	_T("Aucune"),
	_T("Eau"),
	_T("Noir"),
	_T("Bleu"),
	_T("Gris foncé"),
	_T("Fuchsia"),
	_T("Gris"),
	_T("Vert"),
	_T("Vert citron"),
	_T("Gris clair"),
	_T("Marron"),
	_T("Bleu marine"),
	_T("Vert olive"),
	_T("Violet"),
	_T("Rouge"),
	_T("Argent"),
	_T("Canard"),
	_T("Blanc"),
	_T("Jaune"),
	_T("Vert menthe"),
	_T("Bleu ciel"),
	_T("Crème"),
	_T("Gris moyen"),
	_T("Ascenseur"),
	_T("Couleur de fond"),
	_T("Titre fenêtre active"),
	_T("Titre fenêtre inactive"),
	_T("Menu"),
	_T("Fenêtre"),
	_T("Cadre de fenêtre"),
	_T("Texte de menu"),
	_T("Texte de fenêtre"),
	_T("Texte de titre"),
	_T("Bordure fenêtre active"),
	_T("Bordure fenêtre inactive"),
	_T("Espace de travail"),
	_T("Sélection"),
	_T("Texte sélection"),
	_T("Bouton"),
	_T("Ombre de bouton"),
	_T("Texte grisé"),
	_T("Texte de bouton"),
	_T("Texte titre fenêtre inactive"),
	_T("Bouton sélectionné"),
	_T("Ombre sombre 3D"),
	_T("Eclairage 3D"),
	_T("Texte info-bulles"),
	_T("Fond info-bulles"),
	_T("Dégradé fenêtre active"),
	_T("Dégradé fenêtre active"),
	_T("Defaut")
};

COLORREF clListColors[] = {
	clNone,
	clAqua,
	clBlack,
	clBlue,
	clDkGray,
	clFuchsia,
	clGray,
	clGreen,
	clLime,
	clLtGray,
	clMaroon,
	clNavy,
	clOlive,
	clPurple,
	clRed,
	clSilver,
	clTeal,
	clWhite,
	clYellow,
	clMoneyGreen,
	clSkyBlue,
	clCream,
	clMedGray,
	clScrollBar,
	clBackground,
	clActiveCaption,
	clInactiveCaption,
	clMenu,
	clWindow,
	clWindowFrame,
	clMenuText,
	clWindowText,
	clCaptionText,
	clActiveBorder,
	clInactiveBorder,
	clAppWorkSpace,
	clHighlight,
	clHighlightText,
	clBtnFace,
	clBtnShadow,
	clGrayText,
	clBtnText,
	clInactiveCaptionText,
	clBtnHighlight,
	cl3DDkShadow,
	cl3DLight,
	clInfoText,
	clInfoBk,
	clGradientActiveCaption,
	clGradientInactiveCaption,
 	clDefault
};

#define NONECOLOR 0
#define DEBSTANDARDCOLORS 1
#define ENDSTANDARDCOLORS 18
#define DEBEXTENDEDCOLORS 19
#define ENDEXTENDEDCOLORS 22
#define DEBSYSTEMCOLORS 23
#define ENDSYSTEMCOLORS 49
#define DEFAULTCOLOR 50


//---------------------------------------------------------------------------
// TColorBox
//---------------------------------------------------------------------------

TColorBox::TColorBox(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TColorBox");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= WS_VSCROLL | CBS_HASSTRINGS | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED;
	FItems = new TStringList();
  FItemIndex = -1;
  FItemHeight = 15;
	FNoneColorColor = clBlack;
  FTabStop = true;
  FColor = clWindow;
	FStyle << cbStandardColors << cbExtendedColors << cbSystemColors;
	bNoColorDialog = false;

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

    InitStdControl();
    bProcessCreateToDo = true;

		UpdateColorBox();
  }

}

//---------------------------------------------------------------------------
TColorBox::~TColorBox(void) {
  delete FItems;
}

//---------------------------------------------------------------------------
bool TColorBox::ProcessCommand(WORD Notify) {

  if (Notify == CBN_SELCHANGE) {
    FItemIndex = (int) SendMessage(FHandle, CB_GETCURSEL, 0, 0);
    if (!FComponentState.Contains(csLoading)) {
			if (!bNoColorDialog && FStyle.Contains(cbCustomColor) && FItemIndex == 0) {
				TColorDialog *ColorDialog = new TColorDialog(this);
				ColorDialog->Color = FColors[0];
				if (ColorDialog->Execute()) {
					FColors[0] = ColorDialog->Color;
					Invalidate();
				}
			}
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
bool TColorBox::ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS) {

  lpMIS->itemHeight = 17;
  lpMIS->itemWidth = FWidth - 21;

  return true;
}

//---------------------------------------------------------------------------
bool TColorBox::ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS) {
	TCHAR szBuffer[32];
	HDC hdc;
  RECT RectItem = lpDIS->rcItem;
	RECT Rect;
	HBRUSH hBrush, hOldBrush;
	HPEN hOldPen;

	hdc = lpDIS->hDC;
	if (lpDIS->itemID != (DWORD) -1) {

		RectItem = lpDIS->rcItem;

		if (lpDIS->itemState & ODS_SELECTED) {
			hBrush = GetSysColorBrush(COLOR_HIGHLIGHT);
		}
		else {
			hBrush = GetSysColorBrush(COLOR_WINDOW);
		}
		FillRect(hdc, &RectItem, hBrush);

		Rect.left = RectItem.left + 1;
		Rect.top = RectItem.top + 1;
		Rect.right = RectItem.left + 17;
		Rect.bottom = RectItem.bottom - 1;
		hBrush = CreateSolidBrush(FColors[lpDIS->itemID].cr);
		hOldBrush = (HBRUSH) SelectObject(hdc, hBrush);
		hOldPen = (HPEN) SelectObject(hdc, GetStockObject(BLACK_PEN));
		Rectangle(hdc, Rect.left, Rect.top, Rect.right, Rect.bottom);
		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);

		SetBkMode(hdc, TRANSPARENT);
		SendMessage(FHandle, CB_GETLBTEXT, lpDIS->itemID, (LPARAM) (LPCSTR) szBuffer);
		if (lpDIS->itemState & ODS_SELECTED) {
			SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else {
			SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
		}
 		TextOut(hdc, RectItem.left + 20, RectItem.top + 2, szBuffer, lstrlen(szBuffer));

		if (lpDIS->itemState & ODS_FOCUS) {
			InflateRect(&RectItem, -1, -1);
			SetTextColor(hdc, RGB(0, 0, 0));
			DrawFocusRect(hdc, &RectItem);
		}

	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Colors
//---------------------------------------------------------------------------

TColor TColorBox::Get_Colors(int Index) {
  return FColors[Index];
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColorNames
//---------------------------------------------------------------------------

AnsiString TColorBox::Get_ColorNames(int Index) {
	TCHAR szBuffer[32];

	if (FHandle) SendMessage(FHandle, CB_GETLBTEXT, Index, (LPARAM) (LPCSTR) szBuffer);

  return AnsiString(szBuffer);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

bool TColorBox::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
    if (FHandle) MoveWindow(FHandle, FLeft, FTop, FWidth, FHeight + 100, FALSE);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NoneColorColor
//---------------------------------------------------------------------------

TColor TColorBox::Get_NoneColorColor(void) {
  return FNoneColorColor;
}

bool TColorBox::Set_NoneColorColor(TColor NewNoneColorColor) {
  if (FNoneColorColor != NewNoneColorColor) {
    FNoneColorColor = NewNoneColorColor;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Selected
//---------------------------------------------------------------------------

TColor TColorBox::Get_Selected(void) {
	if (FItemIndex >= 0) return FColors[FItemIndex];
	return TColor(0xFFFFFFFF);
}

bool TColorBox::Set_Selected(TColor NewSelected) {
	int i, Deb;
  if (FStyle.Contains(cbCustomColor)) Deb = 1;
	else Deb = 0;
	for (i = Deb; i < FItems->Count; i++) {
		if (FColors[i] == NewSelected) {
			Set_ItemIndex(i);
		  return true;
		}
	}
  if (FStyle.Contains(cbCustomColor)) {
    FColors[0] = NewSelected;
		Set_ItemIndex(0);
  }
	else {
		Set_ItemIndex(-1);
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TColorBoxStyles TColorBox::Get_Style(void) {
  return FStyle;
}

bool TColorBox::Set_Style(TColorBoxStyles NewStyle) {
  if (FStyle != NewStyle) {
    FStyle = NewStyle;
		UpdateColorBox();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemIndex
//---------------------------------------------------------------------------

int TColorBox::Get_ItemIndex(void) {
  return FItemIndex;
}

bool TColorBox::Set_ItemIndex(int NewItemIndex) {

	if (NewItemIndex < -1) NewItemIndex = -1;
	if (NewItemIndex >= FItems->Count) NewItemIndex = FItems->Count - 1;

	if (FItemIndex != NewItemIndex) {
    FItemIndex = NewItemIndex;
    if (FHandle) {
      SendMessage(FHandle, CB_SETCURSEL, NewItemIndex, 0);
			bNoColorDialog = true;
      ProcessCommand(CBN_SELCHANGE);  // Non envoyé par le système
			bNoColorDialog = false;
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemHeight
//---------------------------------------------------------------------------

int TColorBox::Get_ItemHeight(void) {
  return FItemHeight;
}

bool TColorBox::Set_ItemHeight(int NewItemHeight) {

  if (FItemHeight != NewItemHeight) {
    FItemHeight = NewItemHeight;
    if (FHandle) SendMessage(FHandle, CB_SETITEMHEIGHT, (WPARAM) -1, NewItemHeight);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Items
//---------------------------------------------------------------------------

TStrings *TColorBox::Get_Items(void) {
  return FItems;
}

bool TColorBox::Set_Items(TStrings *NewItems) {

  if (FItems != NewItems) {
    FItems->Assign(NewItems);
  }
  return true;
}

//---------------------------------------------------------------------------
bool TColorBox::UpdateColorBox(void) {
	int i, j;
	bool bPrettyNames;
	AnsiString asColorName;


	if (FHandle) {
		SendMessage(FHandle, CB_RESETCONTENT, 0, 0);
		FItems->Clear();
		j = 0;
		bPrettyNames = FStyle.Contains(cbPrettyNames);
		if (FStyle.Contains(cbCustomColor)) {
			asColorName = _TT("Personnaliser...");
			SendMessage(FHandle, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) asColorName);
			FItems->Add(asColorName);
			FColors[j++] = clBlack;
		}
		if (FStyle.Contains(cbIncludeNone)) {
			if (bPrettyNames) {
				asColorName = Application->Translate(szListColorPrettyNames[NONECOLOR]);
			}
			else {
				asColorName = szListColorNames[NONECOLOR];
			}
			SendMessage(FHandle, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) asColorName);
			FItems->Add(asColorName);
			FColors[j++] = clListColors[NONECOLOR];
		}
		if (FStyle.Contains(cbStandardColors)) {
			for (i = DEBSTANDARDCOLORS; i <= ENDSTANDARDCOLORS; i++) {
				if (bPrettyNames) {
					asColorName = Application->Translate(szListColorPrettyNames[i]);
				}
				else {
					asColorName = szListColorNames[i];
				}
				SendMessage(FHandle, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) asColorName);
				FItems->Add(asColorName);
				FColors[j++] = clListColors[i];
			}
		}
		if (FStyle.Contains(cbExtendedColors)) {
			for (i = DEBEXTENDEDCOLORS; i <= ENDEXTENDEDCOLORS; i++) {
				if (bPrettyNames) {
					asColorName = Application->Translate(szListColorPrettyNames[i]);
				}
				else {
					asColorName = szListColorNames[i];
				}
				SendMessage(FHandle, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) asColorName);
				FItems->Add(asColorName);
				FColors[j++] = clListColors[i];
			}
		}
		if (FStyle.Contains(cbSystemColors)) {
			for (i = DEBSYSTEMCOLORS; i <= ENDSYSTEMCOLORS; i++) {
				if (bPrettyNames) {
					asColorName = Application->Translate(szListColorPrettyNames[i]);
				}
				else {
					asColorName = szListColorNames[i];
				}
				SendMessage(FHandle, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) asColorName);
				FItems->Add(asColorName);
				FColors[j++] = clListColors[i];
			}
			if (FStyle.Contains(cbIncludeDefault)) {
				if (bPrettyNames) {
					asColorName = Application->Translate(szListColorPrettyNames[DEFAULTCOLOR]);
				}
				else {
					asColorName = szListColorNames[DEFAULTCOLOR];
				}
				SendMessage(FHandle, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) asColorName);
				FItems->Add(asColorName);
				FColors[j++] = clListColors[DEFAULTCOLOR];
			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TColorBox::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("ItemIndex"));
  ListProperties->Add(_T("ItemHeight"));
  ListProperties->Add(_T("NoneColorColor"));
  ListProperties->Add(_T("Selected"));
  ListProperties->Add(_T("Style"));
  ListProperties->Add(_T("OnChange"));
  ListProperties->Add(_T("OnSelect"));
  ListProperties->Add(_T("OnDropDown"));
  ListProperties->Add(_T("OnCloseUp"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TColorBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("ItemIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("ItemHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("NoneColorColor")) {
    return tpColor;
  }
  if (asProperty == _T("Selected")) {
    return tpColor;
  }
  if (asProperty == _T("Style")) {
    *asInfos = _T("cbStandardColors\ncbExtendedColors\ncbSystemColors\ncbIncludeNone\ncbIncludeDefault\ncbCustomColor\ncbPrettyNames");
    return tpMultipleChoice;
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
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TColorBox::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("ItemIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("ItemHeight")) {
    return _T("15");
  }
  if (asProperty == _T("NoneColorColor")) {
    return _T("clBlack");
  }
  if (asProperty == _T("Selected")) {
    return _T("clBlack");
  }
  if (asProperty == _T("Style")) {
    return _T("[cbStandardColors, cbExtendedColors, cbSystemColors]");
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
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TColorBox::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("ItemIndex")) {
    return IntToStr(Get_ItemIndex());
  }
  if (asProperty == _T("ItemHeight")) {
    return IntToStr(Get_ItemHeight());
  }
  if (asProperty == _T("NoneColorColor")) {
    return ColorToString(Get_NoneColorColor());
  }
  if (asProperty == _T("Selected")) {
    return ColorToString(Get_Selected());
  }
  if (asProperty == _T("Style")) {
    AnsiString asStyles;
    TColorBoxStyles Style = Get_Style();
    if (FStyle.Contains(cbStandardColors)) asStyles += _T("cbStandardColors\n");
    if (FStyle.Contains(cbExtendedColors)) asStyles += _T("cbExtendedColors\n");
    if (FStyle.Contains(cbSystemColors)) asStyles += _T("cbSystemColors\n");
    if (FStyle.Contains(cbIncludeNone)) asStyles += _T("cbIncludeNone\n");
    if (FStyle.Contains(cbIncludeDefault)) asStyles += _T("cbIncludeDefault\n");
    if (FStyle.Contains(cbCustomColor)) asStyles += _T("cbCustomColor\n");
    if (FStyle.Contains(cbPrettyNames)) asStyles += _T("cbPrettyNames\n");
    return FormatProperty(asStyles, _T("["), _T("]"), _T(", "));
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
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TColorBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("ItemIndex")) {
    Set_ItemIndex(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("ItemHeight")) {
    Set_ItemHeight(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("NoneColorColor")) {
    TColor Color;
    Color = asValue;
    Set_NoneColorColor(Color);
    return true;
  }
  if (asProperty == _T("Selected")) {
    TColor Color;
    Color = asValue;
    Set_Selected(Color);
    return true;
  }
  if (asProperty == _T("Style")) {
		int Cur;
    AnsiString asMot;

    TColorBoxStyles NewStyle;
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("cbStandardColors")) NewStyle << cbStandardColors;
      else if (asMot == _T("cbExtendedColors")) NewStyle << cbExtendedColors;
      else if (asMot == _T("cbSystemColors")) NewStyle << cbSystemColors;
      else if (asMot == _T("cbIncludeNone")) NewStyle << cbIncludeNone;
      else if (asMot == _T("cbIncludeDefault")) NewStyle << cbIncludeDefault;
      else if (asMot == _T("cbCustomColor")) NewStyle << cbCustomColor;
      else if (asMot == _T("cbPrettyNames")) NewStyle << cbPrettyNames;
    } while (!asMot.IsEmpty());
		Set_Style(NewStyle);

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
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
