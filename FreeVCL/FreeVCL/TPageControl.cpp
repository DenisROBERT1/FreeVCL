//-----------------------------------------------------------------------------
//! @file TPageControl.cpp
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
// Déclarations anticipées
//---------------------------------------------------------------------------

class TTabSheet;


//---------------------------------------------------------------------------
// TPageControl
//---------------------------------------------------------------------------

TPageControl::TPageControl(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TPageControl");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= (TCS_TABS | TCS_SINGLELINE);
  FActivePageIndex = -1;
  FTabStop = true;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_TABCONTROL, FCaption,
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
TPageControl::~TPageControl(void) {
}

//---------------------------------------------------------------------------
int TPageControl::ProcessNotify(LPNMHDR pnmh) {
  int i;


  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
	case TCN_SELCHANGE:
    FActivePageIndex = (int) SendMessage(FHandle, TCM_GETCURSEL, 0, 0);
		bool bHaveFocus = FocusedChild();
    for (i = 0; i < (int) FPages.size(); i++) {
      FPages[i]->Visible = (i == FActivePageIndex);
    }
		CurrentFocus = FActivePageIndex;
		if (bHaveFocus) SetFocusChild();
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
bool TPageControl::ProcessSize(int SizeType, int L, int H) {
  if (FAlignDisabled == 0) {
    RECT Rect;
    if (FHandle) { // Parce que Size peut être appelé dans CreateWindow (donc avant que FHandle soit défini)
      GetWindowRect(FHandle, &Rect);
      L = Rect.right - Rect.left;
      H = Rect.bottom - Rect.top;
    }
    if (FCanvas != NULL) {
      FCanvas->ClipRect = ClientRect;
    }
    if (L != FWidth || H != FHeight) {
      FWidth = L;
      FHeight = H;
      OnResize(this);
      if (FHandle) InvalidateRect(FHandle, NULL, TRUE);
    }
    UpdateSizeTabSheets();
  }

  return false;
}

//---------------------------------------------------------------------------
// Renvoie la position du contrôle enfant à alignement personnalisé (Align = alCustom).
//---------------------------------------------------------------------------

void FASTCALL TPageControl::CustomAlignPosition(TControl* Control,
    int &NewLeft, int &NewTop, int &NewWidth, int &NewHeight,
    TRect &AlignRect) {
  NewLeft = AlignRect.Left;
  NewTop = AlignRect.Top;
  NewWidth = AlignRect.Width;
  NewHeight = AlignRect.Height;
}

//---------------------------------------------------------------------------
// Insertion d'un onglet
//---------------------------------------------------------------------------

void FASTCALL TPageControl::InsertTab(int Index, TTabSheet *TabSheet) {

  if (FHandle) {
    TCITEM TcItem;

    TcItem.mask = TCIF_TEXT;
    TcItem.pszText = (LPTSTR) (LPCTSTR) ((AnsiString) TabSheet->Caption);
    SendMessage(FHandle, TCM_INSERTITEM, (WPARAM) Index, (LPARAM) (LPTCITEM) &TcItem);
  }
  if (Index < (int) FPages.size()) FPages.insert(FPages.begin() + Index, TabSheet);
  else FPages.push_back(TabSheet);
	if (FActivePageIndex >= Index) FActivePageIndex++;
  UpdateActivePage();
  UpdateSizeTabSheets();
}

//---------------------------------------------------------------------------
// Suppression d'un onglet
//---------------------------------------------------------------------------

void FASTCALL TPageControl::RemoveTab(int Index) {
  if (!FComponentState.Contains(csDestroying)) {
    if (FHandle) {
      SendMessage(FHandle, TCM_DELETEITEM, (WPARAM) Index, 0L);
    }
    FPages.erase(FPages.begin() + Index);
		if (FActivePageIndex == Index) FActivePageIndex = -1;
		else if (FActivePageIndex > Index) FActivePageIndex--;
    UpdateActivePage();
	  UpdateSizeTabSheets();
  }
}

//---------------------------------------------------------------------------
// Mise à jour des onglets
//---------------------------------------------------------------------------

void FASTCALL TPageControl::UpdateActivePage(void) {
  int i;
  int PageIndex;


  PageIndex = 0;
  for (i = 0; i < (int) FPages.size(); i++) {
    FPages[i]->PageIndex = PageIndex++;
  }

  if (FHandle) {
		int NewActivePageIndex = FActivePageIndex;
	  if (NewActivePageIndex == -1) NewActivePageIndex = 0;
		if (NewActivePageIndex >= (int) FPages.size()) NewActivePageIndex--;
		Set_ActivePageIndex(NewActivePageIndex);
  }

}

//---------------------------------------------------------------------------
// Mise à jour de la taille des pages
//---------------------------------------------------------------------------

void FASTCALL TPageControl::UpdateSizeTabSheets(void) {
  int i;

	TRect Rect;
  Rect = ClientRect;
  for (i = 0; i < PageCount; i++) {
		FPages[i]->Visible = false;
    FPages[i]->Size(0, 0, Rect.Width, Rect.Height, false);
		FPages[i]->Visible = (i == FActivePageIndex);
  }
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool TPageControl::Set_Visible(bool NewVisible) {
  int i;


  if (FVisible != NewVisible) {
    FVisible = NewVisible;
    ShowWindow(FHandle, FVisible? SW_SHOW: SW_HIDE);
    for (i = 0; i < (int) FPages.size(); i++) {
      if (!FVisible) FPages[i]->Visible = false;
      else FPages[i]->Visible = (i == FActivePageIndex);
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ActivePage
//---------------------------------------------------------------------------

TTabSheet* TPageControl::Get_ActivePage(void) {
  if (FActivePageIndex == -1) return NULL;
  return (TTabSheet *) FPages[FActivePageIndex];
}

bool TPageControl::Set_ActivePage(TTabSheet* NewActivePage) {
  int i;


  if (FActivePageIndex == -1 ||
      FPages[FActivePageIndex] != NewActivePage) {
    for (i = 0; i < (int) FPages.size(); i++) {
      if (FPages[i] == NewActivePage) {
        Set_ActivePageIndex(i);
        return true;
      }
    }
    Set_ActivePageIndex(-1);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ActivePageIndex
//---------------------------------------------------------------------------

int TPageControl::Get_ActivePageIndex(void) {
  return FActivePageIndex;
}

bool TPageControl::Set_ActivePageIndex(int NewActivePageIndex) {
  int i;


	if (NewActivePageIndex >= (int) FPages.size()) NewActivePageIndex = -1;

  if (FActivePageIndex != NewActivePageIndex) {
    FActivePageIndex = NewActivePageIndex;
		bool bHaveFocus = FocusedChild();
    SendMessage(FHandle, TCM_SETCURSEL, FActivePageIndex, 0);
    for (i = 0; i < (int) FPages.size(); i++) {
      FPages[i]->Visible = (i == FActivePageIndex);
    }
		CurrentFocus = FActivePageIndex;
		if (bHaveFocus) SetFocusChild();

		// Evènement
		if (!FComponentState.Contains(csLoading)) OnChange(this);

	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiLine
//---------------------------------------------------------------------------

bool TPageControl::Get_MultiLine(void) {
  return ((FWindowStyle & TCS_MULTILINE) != 0);
}

bool TPageControl::Set_MultiLine(bool NewMultiLine) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewMultiLine) NewWindowStyle |= TCS_MULTILINE;
  else NewWindowStyle &= ~TCS_MULTILINE;
  ChangeWindowStyle(NewWindowStyle);
  UpdateSizeTabSheets();

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PageCount
//---------------------------------------------------------------------------

int TPageControl::Get_PageCount(void) {
  return (int) SendMessage(FHandle, TCM_GETITEMCOUNT, 0, 0);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Pages
//---------------------------------------------------------------------------

TTabSheet * TPageControl::Get_Pages(int Index) {
  return (TTabSheet *) FPages[Index];
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TTabStyle TPageControl::Get_Style(void) {
  if ((FWindowStyle & TCS_FLATBUTTONS) != 0) return tsFlatButtons;
  if ((FWindowStyle & TCS_BUTTONS) != 0) return tsButtons;

  return tsTabs;
}

bool TPageControl::Set_Style(TTabStyle NewStyle) {
  DWORD NewWindowStyle;
  DWORD NewExWindowStyle;


  NewWindowStyle = FWindowStyle & ~(TCS_TABS | TCS_BUTTONS | TCS_FLATBUTTONS);
  NewExWindowStyle = FExWindowStyle & ~TCS_EX_FLATSEPARATORS;
  if (NewStyle == tsTabs) {
    NewWindowStyle |= TCS_TABS;
  }
  else if (NewStyle == tsButtons) {
    NewWindowStyle |= TCS_BUTTONS;
  }
  else if (NewStyle == tsFlatButtons) {
    NewWindowStyle |= (TCS_BUTTONS | TCS_FLATBUTTONS);
    NewExWindowStyle |= TCS_EX_FLATSEPARATORS;
  }
  ChangeWindowStyle(NewWindowStyle);
  ChangeExWindowStyle(NewExWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Ajustement du rectangle client
//---------------------------------------------------------------------------
void FASTCALL TPageControl::AdjustClientRect(TRect &Rect) {

  if (FHandle) {
    SendMessage(FHandle, TCM_ADJUSTRECT, (WPARAM) FALSE, (LPARAM) (LPRECT) &Rect);
  }

}

//---------------------------------------------------------------------------
// Indique l'index de l'onglet à un point spécifié.
//---------------------------------------------------------------------------

int FASTCALL TPageControl::IndexOfTabAt(int X, int Y) {
  int Index = -1;


  if (FHandle) {
    TCHITTESTINFO TCHitTestInfo;
    TCHitTestInfo.pt.x = X;
    TCHitTestInfo.pt.y = Y;
    TCHitTestInfo.flags = 0;
    Index = (int) SendMessage(FHandle, TCM_HITTEST, 0, (LPARAM) &TCHitTestInfo);
  }

  return Index;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPageControl::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("ActivePage"));
  ListProperties->Add(_T("ActivePageIndex"));
  ListProperties->Add(_T("MultiLine"));
  ListProperties->Add(_T("PageCount"));
  ListProperties->Add(_T("Style"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPageControl::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("ActivePage")) {
    return tpObject;
  }
  if (asProperty == _T("ActivePageIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("MultiLine")) {
    return tpBool;
  }
  if (asProperty == _T("PageCount")) {
    return tpNumber;
  }
  if (asProperty == _T("Style")) {
    *asInfos = _T("tsTabs\ntsButtons\ntsFlatButtons");
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

AnsiString TPageControl::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("ActivePage")) {
    return _T("");
  }
  if (asProperty == _T("ActivePageIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("MultiLine")) {
    return _T("False");
  }
  if (asProperty == _T("PageCount")) {
    return _T("0");
  }
  if (asProperty == _T("Style")) {
    return _T("tsTabs");
  }
  if (asProperty == _T("OnChange")) {
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

AnsiString TPageControl::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("ActivePage")) {
    TTabSheet *TabSheet = Get_ActivePage();
    if (TabSheet) return TabSheet->Name;
    return _T("");
  }
  if (asProperty == _T("ActivePageIndex")) {
    return IntToStr(Get_ActivePageIndex());
  }
  if (asProperty == _T("MultiLine")) {
    if (Get_MultiLine()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("PageCount")) {
    return IntToStr(Get_PageCount());
  }
  if (asProperty == _T("Style")) {
    TTabStyle Style = Get_Style();
    if (Style == tsTabs) return _T("tsTabs");
    if (Style == tsButtons) return _T("tsButtons");
    if (Style == tsFlatButtons) return _T("tsFlatButtons");
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPageControl::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("ActivePage")) {
    TTabSheet *TabSheet;
		if (asValue.IsEmpty()) TabSheet = NULL;
		else TabSheet = dynamic_cast<TTabSheet *>(FindRootComponent()->FindComponent(asValue));
    Set_ActivePage(TabSheet);
    return true;
  }
  if (asProperty == _T("ActivePageIndex")) {
    Set_ActivePageIndex(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("MultiLine")) {
    Set_MultiLine(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("PageCount")) {
    return false;
  }
  if (asProperty == _T("Style")) {
    if (asValue == _T("tsTabs")) Set_Style(tsTabs);
    else if (asValue == _T("tsButtons")) Set_Style(tsButtons);
    else if (asValue == _T("tsFlatButtons")) Set_Style(tsFlatButtons);
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
// Méthode appelée en fin de construction de la fiche
//---------------------------------------------------------------------------

void FASTCALL TPageControl::AfterConstruction(void) {
  int i;


  SendMessage(FHandle, TCM_SETCURSEL, FActivePageIndex, 0);
  for (i = 0; i < (int) FPages.size(); i++) {
    FPages[i]->Visible = (i == FActivePageIndex);
  }

}


//---------------------------------------------------------------------------

