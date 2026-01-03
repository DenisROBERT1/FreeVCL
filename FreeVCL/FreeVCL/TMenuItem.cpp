//-----------------------------------------------------------------------------
//! @file TMenuItem.cpp
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
// TMenuItem
//---------------------------------------------------------------------------

TMenuItem::TMenuItem(TComponent* AOwner): TComponent(AOwner) {

  // Initialisations
  FClassName = _T("TMenuItem");
	FAction = NULL;
	FBreak = mbNone;
	FAutoCheck = false;
  FChecked = false;
  FEnabled = true;
  FHandle = NULL;
  FImageIndex = -1;
  FRightJustify = false;
  FShortCut = 0;
  FSubMenuImages = NULL;
}

TMenuItem::~TMenuItem(void) {
  if (FHandle) DestroyMenu(FHandle);
}

//---------------------------------------------------------------------------
bool TMenuItem::ProcessCommand(WORD Notify) {
	if (!FComponentState.Contains(csLoading)) {
		if (FAutoCheck) Set_Checked(!FChecked);
		if (FAction) FAction->Execute();
		OnClick(this);
	}
  return true;
}

//---------------------------------------------------------------------------
bool TMenuItem::ProcessShortCut(int nVirtKey, TShiftState Shift) {
  WORD ShortCutKey;
  TShiftState ShortCutShift;


  if (!FComponentState.Contains(csLoading) && FEnabled) {
    ShortCutToKey(FShortCut, ShortCutKey, ShortCutShift);
    if (ShortCutKey == (WORD) nVirtKey && ShortCutShift == Shift) {
			if (FAction) FAction->Execute();
			OnClick(this);
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
bool TMenuItem::ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS) {
  HDC hdc = GetDC(NULL);
  HFONT hOldFont = (HFONT) SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
  SIZE Size;
  GetTextExtentPoint32(hdc, FCaption, FCaption.Length(), &Size);
  if (FSubMenuImages) {
    lpMIS->itemWidth = 20 + FSubMenuImages->Width + Size.cx;
    lpMIS->itemHeight = FSubMenuImages->Height + 4;
  }
  else {
    lpMIS->itemWidth = 20 + Size.cx;
    lpMIS->itemHeight = 4;
  }
  if (lpMIS->itemHeight < (unsigned int) Size.cy + 4) {
    lpMIS->itemHeight = (unsigned int) Size.cy + 4;
  }
  SelectObject(hdc, hOldFont);
  ReleaseDC(NULL, hdc);

  return true;
}

//---------------------------------------------------------------------------
bool TMenuItem::ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS) {
  int OldBkMode;
  UINT OldAlign;
  COLORREF OldColor;
  int Color;
  int WidthImage, HeightImage;


  HDC hdc = lpDIS->hDC;
  RECT Rect = lpDIS->rcItem;

  // Fond
  Color = (lpDIS->itemState & ODS_SELECTED)? COLOR_HIGHLIGHT: COLOR_MENU;
  HBRUSH hBrush = CreateSolidBrush(GetSysColor(Color));
  FillRect(hdc, &Rect, hBrush);
  DeleteObject(hBrush);

  // Dessin de l'icône
  int Marge;
  if (FSubMenuImages) {
    Marge = (Rect.bottom - Rect.top - FSubMenuImages->Height) / 2;
    FSubMenuImages->Draw(hdc, Rect.left + 18 + Marge, Rect.top + Marge, FImageIndex);
    WidthImage = FSubMenuImages->Width;
    HeightImage = FSubMenuImages->Height;
  }
  else {
    Marge = 0;
    WidthImage = 0;
    HeightImage = Rect.bottom - Rect.top;
  }

  // Ecriture du texte
  OldBkMode = SetBkMode(hdc, TRANSPARENT);
  OldAlign = SetTextAlign(hdc, TA_BOTTOM);
  if (lpDIS->itemState & ODS_GRAYED) Color = COLOR_GRAYTEXT;
  else if (lpDIS->itemState & ODS_SELECTED) Color = COLOR_HIGHLIGHTTEXT;
  else Color = COLOR_MENUTEXT;
  OldColor = SetTextColor(hdc, GetSysColor(Color));
  TextOut(hdc, Rect.left + 24 + WidthImage, Rect.top + Marge + HeightImage / 2 + 4,
    FCaption, FCaption.Length());
  SetTextColor(hdc, OldColor);
  SetTextAlign(hdc, OldAlign);
  SetBkMode(hdc, OldBkMode);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Action
//---------------------------------------------------------------------------

TAction *TMenuItem::Get_Action(void) {
  return FAction;
}

bool TMenuItem::Set_Action(TAction *NewAction) {
  if (FAction != NewAction) {
    FAction = NewAction;
		if (FAction) {
			UpdateMenuText();
			SET_EVENT(FAction, TOnUpdate, OnUpdate, TMenuItem, this, ActionUpdate);
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Break
//---------------------------------------------------------------------------

TMenuBreak TMenuItem::Get_Break(void) {
  return FBreak;
}

bool TMenuItem::Set_Break(TMenuBreak NewBreak) {
  if (FBreak != NewBreak) {
    FBreak = NewBreak;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

bool TMenuItem::Set_Caption(AnsiString NewCaption) {

  if (FCaption != NewCaption) {
    FCaption = NewCaption;
		UpdateMenuText();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AutoCheck
//---------------------------------------------------------------------------

bool TMenuItem::Get_AutoCheck(void) {
  return FAutoCheck;
}

bool TMenuItem::Set_AutoCheck(bool NewAutoCheck) {
  if (FAutoCheck != NewAutoCheck) {
    FAutoCheck = NewAutoCheck;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Checked
//---------------------------------------------------------------------------

bool TMenuItem::Get_Checked(void) {
  return FChecked;
}

bool TMenuItem::Set_Checked(bool NewChecked) {
  HMENU hMenu;

  if (FChecked != NewChecked && FAction == NULL) {
    FChecked = NewChecked;
		TMenuItem *MenuItemParent = dynamic_cast<TMenuItem *>(FOwner);
		if (MenuItemParent) {
			hMenu = MenuItemParent->Handle;
			CheckMenuItem(hMenu, FCommand,
					FChecked? MF_BYCOMMAND | MF_CHECKED: MF_BYCOMMAND | MF_UNCHECKED);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TMenuItem::Get_Count(void) {
  return (int) FComponents.size();
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Enabled
//---------------------------------------------------------------------------

bool TMenuItem::Get_Enabled(void) {
  return FEnabled;
}

bool TMenuItem::Set_Enabled(bool NewEnabled) {

  if (FEnabled != NewEnabled) {
    FEnabled = NewEnabled;
		TMenuItem *MenuItemParent = dynamic_cast<TMenuItem *>(FOwner);
		if (MenuItemParent) {
      HMENU hMenu = MenuItemParent->Handle;
			EnableMenuItem(hMenu,
				FHandle? (UINT) (UINT_PTR) FHandle: FCommand,
				FEnabled? MF_BYCOMMAND | MF_ENABLED: MF_BYCOMMAND | MF_GRAYED);
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HMENU TMenuItem::Get_Handle(void) {
  return FHandle;
}

bool TMenuItem::Set_Handle(HMENU NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ImageIndex
//---------------------------------------------------------------------------

TImageIndex TMenuItem::Get_ImageIndex(void) {
  return FImageIndex;
}

bool TMenuItem::Set_ImageIndex(TImageIndex NewImageIndex) {
  if (FImageIndex != NewImageIndex) {
    FImageIndex = NewImageIndex;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Items
//---------------------------------------------------------------------------

TMenuItem * TMenuItem::Get_Items(void) {
  return this;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété RightJustify
//---------------------------------------------------------------------------

bool TMenuItem::Get_RightJustify(void) {
  return FRightJustify;
}

bool TMenuItem::Set_RightJustify(bool NewRightJustify) {
  if (FRightJustify != NewRightJustify) {
    FRightJustify = NewRightJustify;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShortCut
//---------------------------------------------------------------------------

TShortCut TMenuItem::Get_ShortCut(void) {
  return FShortCut;
}

bool TMenuItem::Set_ShortCut(TShortCut NewShortCut) {
  if (FShortCut != NewShortCut) {
    FShortCut = NewShortCut;
		UpdateMenuText();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SubMenuImages
//---------------------------------------------------------------------------

TImageList * TMenuItem::Get_SubMenuImages(void) {
  return FSubMenuImages;
}

bool TMenuItem::Set_SubMenuImages(TImageList * NewSubMenuImages) {
  if (FSubMenuImages != NewSubMenuImages) {
    FSubMenuImages = NewSubMenuImages;
  }
  return true;
}


//---------------------------------------------------------------------------
void FASTCALL TMenuItem::ActionUpdate(TObject *Sender) {
	UpdateMenuText();
}

//---------------------------------------------------------------------------
bool TMenuItem::UpdateMenuText(void) {

  if (FOwner) {

		AnsiString asLibelle = FCaption;
		if (FCaption == _T("") && FAction) asLibelle = FAction->Caption;

		TShortCut AShortCut = FShortCut;
		if (AShortCut == (TShortCut) 0 && FAction) AShortCut = FAction->ShortCut;

		if (AShortCut != (TShortCut) 0) {
			asLibelle += _T("\t");
			asLibelle += ShortCutToText(AShortCut);
		}

    TMenuItem *MenuItemParent = dynamic_cast<TMenuItem *>(FOwner);
		if (MenuItemParent) {
			HMENU hMenu = MenuItemParent->Handle;
			MENUITEMINFO MenuItemInfo;
			memset(&MenuItemInfo, 0, sizeof(MENUITEMINFO));
			MenuItemInfo.cbSize = sizeof(MENUITEMINFO);
			MenuItemInfo.fMask = MIIM_DATA | MIIM_TYPE;
			MenuItemInfo.fType = MFT_STRING;
			MenuItemInfo.dwTypeData = const_cast<TCHAR *>((LPCTSTR) asLibelle);
			MenuItemInfo.cch = asLibelle.Length();
			SetMenuItemInfo(hMenu, FCommand, FALSE, &MenuItemInfo);
		}

		if (FAction) {
			TMenuItem *MenuItemParent = dynamic_cast<TMenuItem *>(FOwner);
			if (MenuItemParent) {
				HMENU hMenu = MenuItemParent->Handle;
				CheckMenuItem(hMenu, FCommand,
						FAction->Checked? MF_BYCOMMAND | MF_CHECKED: MF_BYCOMMAND | MF_UNCHECKED);
			}
		}

	}

	return true;
}


//---------------------------------------------------------------------------
void TMenuItem::Add(TMenuItem* MenuItem) {

  if (MenuItem->Owner) {
    // Pas de double insertion d'un item
    // Cas classique:
    // TMenuItem MenuItem1 = new TMenuItem(PopupMenu);  // Première insertion
    // PopupMenu->Items->Add(MenuItem1);                // Seconde
    if (MenuItem->Owner != this) {
      MenuItem->Owner->RemoveComponent(MenuItem);
      FComponents.push_back(MenuItem);
    }
  }
  else {
    FComponents.push_back(MenuItem);
  }
  MenuItem->FOwner = this;

  // Création d'un MenuItem "on fly"
  if (FHandle) {
    AppendMenu(MenuItem);
    if (TypeMenu == TM_MainMenu) {
      TWinControl *RootWindow = (TWinControl *) FindRootComponent();
      DrawMenuBar(RootWindow->Handle);
    }
  }

}

//---------------------------------------------------------------------------
void TMenuItem::Clear(void) {
	int i;

  // Suppression de tous les items du menu
	for (i = Count - 1; i >= 0; i--) {
		Delete(i);
	}

}

//---------------------------------------------------------------------------
void TMenuItem::Delete(int Index) {

  // Suppression d'un MenuItem "on fly"
  if (FHandle) {
    DeleteMenu(FHandle, Index, MF_BYPOSITION);
    if (TypeMenu == TM_MainMenu) {
      TWinControl *RootWindow = (TWinControl *) FindRootComponent();
      DrawMenuBar(RootWindow->Handle);
    }
  }

  delete FComponents[Index];  // Ce qui a pour effet de le supprimer de FComponents

}

//---------------------------------------------------------------------------
void TMenuItem::BuildMenu(void) {
  int i;
  AnsiString asLibelle;
  TMenuItem * MenuItem;


  if (FComponents.size() != 0) {

    for (i = 0; i < (int) FComponents.size(); i++) {
      if (FComponents[i]->ClassNameIs("TMenuItem")) {
        MenuItem = (TMenuItem *) FComponents[i];
        MenuItem->TypeMenu = TypeMenu;
        MenuItem->BuildMenu();
        AppendMenu(MenuItem);
      }
    }

  }

}

//---------------------------------------------------------------------------
void TMenuItem::AppendMenu(TMenuItem* MenuItem) {
  UINT Flag;
  AnsiString asLibelle;
  UINT_PTR IdMenu;
  TShortCut AShortCut;


  if (FHandle == NULL) {
    if (TypeMenu == TM_MainMenu) FHandle = CreateMenu();
    else FHandle = CreatePopupMenu();
  }

  Flag = 0;
  asLibelle = MenuItem->Caption;

	AShortCut = MenuItem->ShortCut;
  if (AShortCut == (TShortCut) 0 && MenuItem->Action) AShortCut = MenuItem->Action->ShortCut;
	if (AShortCut != (TShortCut) 0) {
    asLibelle += _T("\t");
    asLibelle += ShortCutToText(AShortCut);
  }

	if (asLibelle == _T("-")) {
    // Séparateur horizontal
    Flag = MF_SEPARATOR;
    IdMenu = 0;
  }
  else if (MenuItem->Handle != NULL) {
    // Sous-menu
    if (MenuItem->FImageIndex == -1) Flag = MF_STRING | MF_POPUP;
    else Flag = MF_OWNERDRAW | MF_POPUP;
    IdMenu = (UINT_PTR) (HMENU) MenuItem->Handle;
  }
  else {
    // Ligne de menu
    if (MenuItem->FImageIndex == -1) Flag = MF_STRING;
    else Flag = MF_OWNERDRAW;
    IdMenu = (UINT_PTR) (int) MenuItem->Command;
  }
  // Séparateur vertical
  if (MenuItem->FBreak == mbBarBreak) Flag |= MF_MENUBARBREAK;
  else if (MenuItem->FBreak == mbBreak) Flag |= MF_MENUBREAK;
  // Enabled, checked
  if (!MenuItem->Enabled) Flag |= MF_GRAYED;
  if (MenuItem->Checked) Flag |= MF_CHECKED;
  // Aligné à droite
  if (MenuItem->RightJustify) Flag |= MF_RIGHTJUSTIFY;

  ::AppendMenu(FHandle, Flag, IdMenu, asLibelle);

}



//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TMenuItem::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Action"));
  ListProperties->Add(_T("Break"));
  ListProperties->Add(_T("AutoCheck"));
  ListProperties->Add(_T("Checked"));
  ListProperties->Add(_T("Enabled"));
  ListProperties->Add(_T("ImageIndex"));
  ListProperties->Add(_T("RightJustify"));
  ListProperties->Add(_T("ShortCut"));
  ListProperties->Add(_T("SubMenuImages"));
  ListProperties->Add(_T("OnClick"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TMenuItem::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Action")) {
    return tpObject;
  }
  if (asProperty == _T("Break")) {
    *asInfos = _T("mbNone\nmbBreak\nmbBarBreak");
    return tpChoice;
  }
  if (asProperty == _T("AutoCheck")) {
    return tpBool;
  }
  if (asProperty == _T("Checked")) {
    return tpBool;
  }
  if (asProperty == _T("Enabled")) {
    return tpBool;
  }
  if (asProperty == _T("ImageIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("RightJustify")) {
    return tpBool;
  }
  if (asProperty == _T("ShortCut")) {
    return tpText;
  }
  if (asProperty == _T("SubMenuImages")) {
    return tpObject;
  }
  if (asProperty == _T("OnClick")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TMenuItem::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Action")) {
    return _T("");
  }
  if (asProperty == _T("Break")) {
    return _T("mbNone");
  }
  if (asProperty == _T("AutoCheck")) {
    return _T("False");
  }
  if (asProperty == _T("Checked")) {
    return _T("False");
  }
  if (asProperty == _T("Enabled")) {
    return _T("True");
  }
  if (asProperty == _T("ImageIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("RightJustify")) {
    return _T("False");
  }
  if (asProperty == _T("ShortCut")) {
    return _T("0");
  }
  if (asProperty == _T("SubMenuImages")) {
    return _T("");
  }
  if (asProperty == _T("OnClick")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TMenuItem::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Action")) {
    TAction *Action = Get_Action();
    if (Action) return Action->Name;
    return _T("");
  }
  if (asProperty == _T("Break")) {
    TMenuBreak Break = Get_Break();
    if (Break == mbNone) return _T("mbNone");
    if (Break == mbBreak) return _T("mbBreak");
    if (Break == mbBarBreak) return _T("mbBarBreak");
    return _T("");
  }
  if (asProperty == _T("AutoCheck")) {
    if (Get_AutoCheck()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Checked")) {
    if (Get_Checked()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Enabled")) {
    if (Get_Enabled()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ImageIndex")) {
    return IntToStr(Get_ImageIndex());
  }
  if (asProperty == _T("RightJustify")) {
    if (Get_RightJustify()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ShortCut")) {
    return AnsiString((int) Get_ShortCut());
  }
  if (asProperty == _T("SubMenuImages")) {
    TImageList *SubMenuImages = Get_SubMenuImages();
    if (SubMenuImages) return SubMenuImages->Name;
    return _T("");
  }
  if (asProperty == _T("OnClick")) {
    return OnClick.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TMenuItem::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Action")) {
    TAction *Action;
		if (asValue.IsEmpty()) Action = NULL;
		else Action = dynamic_cast<TAction *>(FindRootComponent()->FindComponent(asValue));
    Set_Action(Action);
    return true;
  }
  if (asProperty == _T("Break")) {
    if      (asValue == _T("mbNone")) Set_Break(mbNone);
    else if (asValue == _T("mbBreak")) Set_Break(mbBreak);
    else if (asValue == _T("mbBarBreak")) Set_Break(mbBarBreak);
    return true;
  }
  if (asProperty == _T("AutoCheck")) {
    Set_AutoCheck(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Checked")) {
    Set_Checked(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Enabled")) {
    Set_Enabled(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ImageIndex")) {
    FImageIndex = (TImageIndex) StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("RightJustify")) {
    Set_RightJustify(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ShortCut")) {
    Set_ShortCut((TShortCut) asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("SubMenuImages")) {
    TImageList *ImageList;
		if (asValue.IsEmpty()) ImageList = NULL;
		else ImageList = dynamic_cast<TImageList *>(FindRootComponent()->FindComponent(asValue));
    Set_SubMenuImages(ImageList);
    return true;
  }
  if (asProperty == _T("OnClick")) {
		OnClick.ObjCall = Sender;
    OnClick = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}
//---------------------------------------------------------------------------

