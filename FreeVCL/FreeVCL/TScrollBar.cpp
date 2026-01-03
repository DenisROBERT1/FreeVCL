//-----------------------------------------------------------------------------
//! @file TScrollBar.cpp
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
// TScrollBar
//---------------------------------------------------------------------------

TScrollBar::TScrollBar(TComponent* AOwner): TWinControl(AOwner) {
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TScrollBar");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= SBS_HORZ;
  ScrollInfo.cbSize = sizeof(SCROLLINFO);
  ScrollInfo.nMin = 0;
  ScrollInfo.nMax = 100;
  ScrollInfo.nPage = 0;
  ScrollInfo.nPos = 0;
  ScrollInfo.nTrackPos = 0;
  FSmallChange = 1;
  FLargeChange = 5;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_SCROLLBAR, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {

    InitStdControl();
    bProcessCreateToDo = true;
    ScrollInfo.fMask = SIF_ALL;
    SetScrollInfo(FHandle, SB_CTL, &ScrollInfo, TRUE);
  }

}

//---------------------------------------------------------------------------
TScrollBar::~TScrollBar(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TScrollBar::DestroyWnd(void) {
  DestroyWindow(FHandle);
}

//---------------------------------------------------------------------------
void FASTCALL TScrollBar::CreateWnd(void) {
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
                         WC_SCROLLBAR, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    InitStdControl();
    ScrollInfo.fMask = SIF_ALL;
    SetScrollInfo(FHandle, SB_CTL, &ScrollInfo, TRUE);
  }

}

//---------------------------------------------------------------------------
bool TScrollBar::ProcessHScroll(int Command, int Info) {
  switch (Command) {
  case SB_LINEUP:
    Set_Position(ScrollInfo.nPos - FSmallChange);
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    break;
  case SB_LINEDOWN:
    Set_Position(ScrollInfo.nPos + FSmallChange);
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    break;
  case SB_PAGEUP:
    Set_Position(ScrollInfo.nPos - FLargeChange);
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    break;
  case SB_PAGEDOWN:
    Set_Position(ScrollInfo.nPos + FLargeChange);
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    break;
  case SB_THUMBTRACK:
    Set_Position(Info);
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    break;
  }

  return false;
}

//---------------------------------------------------------------------------
bool TScrollBar::ProcessVScroll(int Command, int Info) {
	return ProcessHScroll(Command, Info);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Kind
//---------------------------------------------------------------------------

TScrollBarKind TScrollBar::Get_Kind() {
  if ((FWindowStyle & SBS_VERT) != 0) return sbVertical;
	else return sbHorizontal;
}

bool TScrollBar::Set_Kind(TScrollBarKind NewKind) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle & ~(SBS_HORZ | SBS_VERT);
  if (NewKind == sbHorizontal) NewWindowStyle |= SBS_HORZ;
  else if (NewKind == sbVertical) NewWindowStyle |= SBS_VERT;
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
// Accesseurs de la propriété Min
//---------------------------------------------------------------------------

long TScrollBar::Get_Min(void) {
  return ScrollInfo.nMin;
}

bool TScrollBar::Set_Min(long NewMin) {
  if (ScrollInfo.nMin != NewMin) {
    ScrollInfo.nMin = NewMin;
    ScrollInfo.fMask = SIF_RANGE;
    SetScrollInfo(FHandle, SB_CTL, &ScrollInfo, TRUE);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Max
//---------------------------------------------------------------------------

long TScrollBar::Get_Max(void) {
  return ScrollInfo.nMax;
}

bool TScrollBar::Set_Max(long NewMax) {
  if (ScrollInfo.nMax != NewMax) {
    ScrollInfo.nMax = NewMax;
    ScrollInfo.fMask = SIF_RANGE;
    SetScrollInfo(FHandle, SB_CTL, &ScrollInfo, TRUE);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PageSize
//---------------------------------------------------------------------------

long TScrollBar::Get_PageSize(void) {
  return ScrollInfo.nPage;
}

bool TScrollBar::Set_PageSize(long NewPageSize) {
  if ((long) ScrollInfo.nPage != NewPageSize) {
    ScrollInfo.nPage = NewPageSize;
    ScrollInfo.fMask = SIF_PAGE;
    SetScrollInfo(FHandle, SB_CTL, &ScrollInfo, TRUE);
		FLargeChange = NewPageSize;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SmallChange
//---------------------------------------------------------------------------

long TScrollBar::Get_SmallChange(void) {
  return FSmallChange;
}

bool TScrollBar::Set_SmallChange(long NewSmallChange) {
  if (FSmallChange != NewSmallChange) {
    FSmallChange = NewSmallChange;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LargeChange
//---------------------------------------------------------------------------

long TScrollBar::Get_LargeChange(void) {
  return FLargeChange;
}

bool TScrollBar::Set_LargeChange(long NewLargeChange) {
  if (FLargeChange != NewLargeChange) {
    FLargeChange = NewLargeChange;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

long TScrollBar::Get_Position(void) {
  return ScrollInfo.nPos;
}

bool TScrollBar::Set_Position(long NewPosition) {
  if (NewPosition < ScrollInfo.nMin) NewPosition = ScrollInfo.nMin;
  if (NewPosition > ScrollInfo.nMax) NewPosition = ScrollInfo.nMax;
  if (ScrollInfo.nPos != NewPosition) {
    ScrollInfo.nPos = NewPosition;
    ScrollInfo.fMask = SIF_POS;
    SetScrollInfo(FHandle, SB_CTL, &ScrollInfo, TRUE);
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TScrollBar::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Kind"));  // Kind doit rester en premier (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("Min"));
  ListProperties->Add(_T("Max"));
  ListProperties->Add(_T("PageSize"));
  ListProperties->Add(_T("SmallChange"));
  ListProperties->Add(_T("LargeChange"));
  ListProperties->Add(_T("Position"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TScrollBar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Kind")) {
    *asInfos = _T("sbHorizontal\nsbVertical");
    return tpChoice;
  }
  if (asProperty == _T("Min")) {
    return tpNumber;
  }
  if (asProperty == _T("Max")) {
    return tpNumber;
  }
  if (asProperty == _T("PageSize")) {
    return tpNumber;
  }
  if (asProperty == _T("SmallChange")) {
    return tpNumber;
  }
  if (asProperty == _T("LargeChange")) {
    return tpNumber;
  }
  if (asProperty == _T("Position")) {
    return tpNumber;
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

AnsiString TScrollBar::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Kind")) {
    return _T("sbHorizontal");
  }
  if (asProperty == _T("Min")) {
    return _T("0");
  }
  if (asProperty == _T("Max")) {
    return _T("100");
  }
  if (asProperty == _T("PageSize")) {
    return _T("5");
  }
  if (asProperty == _T("SmallChange")) {
    return _T("1");
  }
  if (asProperty == _T("LargeChange")) {
    return _T("5");
  }
  if (asProperty == _T("Position")) {
    return _T("0");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TScrollBar::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Kind")) {
    TScrollBarKind Kind = Get_Kind();
    if (Kind == sbHorizontal) return _T("sbHorizontal");
    if (Kind == sbVertical) return _T("sbVertical");
    return _T("");
  }
  if (asProperty == _T("Min")) {
    return IntToStr(Get_Min());
  }
  if (asProperty == _T("Max")) {
    return IntToStr(Get_Max());
  }
  if (asProperty == _T("PageSize")) {
    return IntToStr(Get_PageSize());
  }
  if (asProperty == _T("SmallChange")) {
    return IntToStr(Get_SmallChange());
  }
  if (asProperty == _T("LargeChange")) {
    return IntToStr(Get_LargeChange());
  }
  if (asProperty == _T("Position")) {
    return IntToStr(Get_Position());
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TScrollBar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Kind")) {
    if      (asValue == _T("sbHorizontal")) Set_Kind(sbHorizontal);
    else if (asValue == _T("sbVertical")) Set_Kind(sbVertical);
    return true;
  }
  if (asProperty == _T("Min")) {
    Set_Min(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Max")) {
    Set_Max(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("PageSize")) {
    Set_PageSize(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("SmallChange")) {
    Set_SmallChange(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("LargeChange")) {
    Set_LargeChange(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Position")) {
    Set_Position(StrToLong(asValue));
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

