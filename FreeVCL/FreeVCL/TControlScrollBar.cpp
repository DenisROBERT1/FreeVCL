//-----------------------------------------------------------------------------
//! @file TControlScrollBar.cpp
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
// TControlScrollBar
//---------------------------------------------------------------------------

TControlScrollBar::TControlScrollBar(TWinControl* AControl, TScrollBarKind AKind): TPersistent() {

  // Initialisations
  FClassName = _T("TControlScrollBar");
  FHandleParent = NULL;
  if (AControl) FHandleParent = AControl->Handle;
  ScrollInfo.cbSize = sizeof(SCROLLINFO);
  ScrollInfo.nMin = 0;
  ScrollInfo.nMax = 100;
  ScrollInfo.nPage = 0;
  ScrollInfo.nPos = 0;
  FSmallChange = 1;
  FLargeChange = 5;
  FKind = AKind;

}

//---------------------------------------------------------------------------
TControlScrollBar::~TControlScrollBar(void) {
}

//---------------------------------------------------------------------------
bool TControlScrollBar::ProcessScroll(int Command, int Info) {
  int NewPos;


  switch (Command) {
  case SB_LINELEFT:
    NewPos = ScrollInfo.nPos - FSmallChange;
    OnScroll(this, scLineUp, NewPos);
    Set_Position(NewPos);
    OnChange(this);
    break;
  case SB_LINERIGHT:
    NewPos = ScrollInfo.nPos + FSmallChange;
    OnScroll(this, scLineDown, NewPos);
    Set_Position(NewPos);
    OnChange(this);
    break;
  case SB_PAGELEFT:
    NewPos = ScrollInfo.nPos - FLargeChange;
    OnScroll(this, scPageUp, NewPos);
    Set_Position(NewPos);
    OnChange(this);
    break;
  case SB_PAGERIGHT:
    NewPos = ScrollInfo.nPos + FLargeChange;
    OnScroll(this, scPageDown, NewPos);
    Set_Position(NewPos);
    OnChange(this);
    break;
  case SB_THUMBTRACK:
    NewPos = Info;
    OnScroll(this, scTrack, NewPos);
    Set_Position(NewPos);
    OnChange(this);
    break;
	case SB_THUMBPOSITION:
    NewPos = Info;
    OnScroll(this, scPosition, NewPos);
    Set_Position(NewPos);
    OnChange(this);
    break;
	case SB_ENDSCROLL:
    NewPos = ScrollInfo.nPos;
    OnScroll(this, scEndScroll, NewPos);
    Set_Position(NewPos);
    OnChange(this);
    break;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Kind
//---------------------------------------------------------------------------

TScrollBarKind TControlScrollBar::Get_Kind(void) {
  return FKind;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Min
//---------------------------------------------------------------------------

long TControlScrollBar::Get_Min(void) {
  return ScrollInfo.nMin;
}

bool TControlScrollBar::Set_Min(long NewMin) {
  if (ScrollInfo.nMin != NewMin) {
    ScrollInfo.nMin = NewMin;
    ScrollInfo.fMask = SIF_RANGE;
    SetScrollInfo(FHandleParent, (int) FKind, &ScrollInfo, TRUE);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Max
//---------------------------------------------------------------------------

long TControlScrollBar::Get_Max(void) {
  return ScrollInfo.nMax;
}

bool TControlScrollBar::Set_Max(long NewMax) {
  if (ScrollInfo.nMax != NewMax) {
    ScrollInfo.nMax = NewMax;
    ScrollInfo.fMask = SIF_RANGE | SIF_PAGE;  // Les deux, parce que si précédemment max < page, le système a automatiquement ajusté max en interne
    SetScrollInfo(FHandleParent, (int) FKind, &ScrollInfo, TRUE);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Increment
//---------------------------------------------------------------------------

long TControlScrollBar::Get_Increment(void) {
  return Get_SmallChange();
}

bool TControlScrollBar::Set_Increment(long NewIncrement) {
  return Set_SmallChange(NewIncrement);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PageSize
//---------------------------------------------------------------------------

long TControlScrollBar::Get_PageSize(void) {
  return ScrollInfo.nPage;
}

bool TControlScrollBar::Set_PageSize(long NewPageSize) {
  if ((long) ScrollInfo.nPage != NewPageSize) {
    ScrollInfo.nPage = NewPageSize;
    ScrollInfo.fMask = SIF_PAGE;
    SetScrollInfo(FHandleParent, (int) FKind, &ScrollInfo, TRUE);
    FLargeChange = NewPageSize;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SmallChange
//---------------------------------------------------------------------------

long TControlScrollBar::Get_SmallChange(void) {
  return FSmallChange;
}

bool TControlScrollBar::Set_SmallChange(long NewSmallChange) {
  if (FSmallChange != NewSmallChange) {
    FSmallChange = NewSmallChange;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LargeChange
//---------------------------------------------------------------------------

long TControlScrollBar::Get_LargeChange(void) {
  return FLargeChange;
}

bool TControlScrollBar::Set_LargeChange(long NewLargeChange) {
  if (FLargeChange != NewLargeChange) {
    FLargeChange = NewLargeChange;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

long TControlScrollBar::Get_Position(void) {
  return ScrollInfo.nPos;
}

bool TControlScrollBar::Set_Position(long NewPosition) {
  if (NewPosition < ScrollInfo.nMin) NewPosition = ScrollInfo.nMin;
  if (NewPosition > ScrollInfo.nMax) NewPosition = ScrollInfo.nMax;
  if (ScrollInfo.nPos != NewPosition) {
    ScrollInfo.nPos = NewPosition;
    ScrollInfo.fMask = SIF_POS;
    SetScrollInfo(FHandleParent, (int) FKind, &ScrollInfo, TRUE);
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TControlScrollBar::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Kind"));
  ListProperties->Add(_T("Min"));
  ListProperties->Add(_T("Max"));
  ListProperties->Add(_T("Increment"));
  ListProperties->Add(_T("PageSize"));
  ListProperties->Add(_T("SmallChange"));
  ListProperties->Add(_T("LargeChange"));
  ListProperties->Add(_T("Position"));
  ListProperties->Add(_T("OnChange"));
  ListProperties->Add(_T("OnScroll"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TControlScrollBar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
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
  if (asProperty == _T("Increment")) {
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
  if (asProperty == _T("OnScroll")) {
    *asInfos = _T("(TObject *Sender, TScrollCode ScrollCode, int *ScrollPos)");
    return tpEvent;
  }
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TControlScrollBar::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Kind")) {
    return _T("");
  }
  if (asProperty == _T("Min")) {
    return _T("0");
  }
  if (asProperty == _T("Max")) {
    return _T("100");
  }
  if (asProperty == _T("Increment")) {
    return _T("1");
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
  if (asProperty == _T("OnScroll")) {
    return _T("");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TControlScrollBar::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Kind")) {
    TScrollBarKind ScrollBarKind = Get_Kind();
    if (ScrollBarKind == sbHorizontal) return _T("sbHorizontal");
    if (ScrollBarKind == sbVertical) return _T("sbVertical");
    return _T("");
  }
  if (asProperty == _T("Min")) {
    return IntToStr(Get_Min());
  }
  if (asProperty == _T("Max")) {
    return IntToStr(Get_Max());
  }
  if (asProperty == _T("Increment")) {
    return IntToStr(Get_Increment());
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
  if (asProperty == _T("OnScroll")) {
    return OnScroll.AsString();
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TControlScrollBar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Min")) {
    Set_Min(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Max")) {
    Set_Max(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Increment")) {
    Set_Increment(StrToLong(asValue));
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
  if (asProperty == _T("OnScroll")) {
		OnScroll.ObjCall = Sender;
    OnScroll = asValue.c_str();
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

