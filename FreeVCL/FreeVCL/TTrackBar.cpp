//-----------------------------------------------------------------------------
//! @file TTrackBar.cpp
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
// TTrackBar
//---------------------------------------------------------------------------

TTrackBar::TTrackBar(TComponent* AOwner): TWinControl(AOwner) {
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TTrackBar");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
	FWindowStyle |= TBS_FIXEDLENGTH | TBS_AUTOTICKS;
	FFrequency = 0;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         TRACKBAR_CLASS, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {

    InitStdControl();
    bProcessCreateToDo = true;
		SendMessage(FHandle, TBM_CLEARSEL, (WPARAM) TRUE, 0);
  }

}

//---------------------------------------------------------------------------
TTrackBar::~TTrackBar(void) {
}

//---------------------------------------------------------------------------
bool TTrackBar::ProcessHScroll(int Command, int Info) {
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return false;
}

//---------------------------------------------------------------------------
bool TTrackBar::ProcessVScroll(int Command, int Info) {
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété EnableSelRange
//---------------------------------------------------------------------------

bool TTrackBar::Get_EnableSelRange(void) {
  return ((FWindowStyle & TBS_ENABLESELRANGE) != 0);
}

bool TTrackBar::Set_EnableSelRange(bool NewEnableSelRange) {
  DWORD NewWindowStyle;


  NewWindowStyle = GetWindowLong(FHandle, GWL_STYLE);
  if (NewEnableSelRange) NewWindowStyle |= TBS_ENABLESELRANGE;
  else NewWindowStyle &= ~TBS_ENABLESELRANGE;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Frequency
//---------------------------------------------------------------------------

int TTrackBar::Get_Frequency(void) {
  return FFrequency;
}

bool TTrackBar::Set_Frequency(int NewFrequency) {
  if (FFrequency != NewFrequency) {
    FFrequency = NewFrequency;
		if (FHandle) SendMessage(FHandle, TBM_SETTICFREQ, (WPARAM) NewFrequency, 0);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LineSize
//---------------------------------------------------------------------------

int TTrackBar::Get_LineSize(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETLINESIZE, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_LineSize(int NewLineSize) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETLINESIZE, 0, (LPARAM) NewLineSize);
	  return true;
	}

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Max
//---------------------------------------------------------------------------

long TTrackBar::Get_Max(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETRANGEMAX, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_Max(long NewMax) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETRANGEMAX, 0, (LPARAM) NewMax);
	  return true;
	}

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Min
//---------------------------------------------------------------------------

long TTrackBar::Get_Min(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETRANGEMIN, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_Min(long NewMin) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETRANGEMIN, 0, (LPARAM) NewMin);
	  return true;
	}

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Orientation
//---------------------------------------------------------------------------

TTrackBarOrientation TTrackBar::Get_Orientation(void) {
  if (FWindowStyle & TBS_VERT) return trVertical;
	else return trHorizontal;
}

bool TTrackBar::Set_Orientation(TTrackBarOrientation NewOrientation) {
  DWORD NewWindowStyle;


  NewWindowStyle = GetWindowLong(FHandle, GWL_STYLE);
  NewWindowStyle &= ~(TBS_HORZ | TBS_VERT);
  if (NewOrientation == trVertical) NewWindowStyle |= TBS_VERT;
  else NewWindowStyle |= TBS_HORZ;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PageSize
//---------------------------------------------------------------------------

long TTrackBar::Get_PageSize(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETPAGESIZE, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_PageSize(long NewPageSize) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETPAGESIZE, 0, (LPARAM) NewPageSize);
	  return true;
	}

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

long TTrackBar::Get_Position(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETPOS, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_Position(long NewPosition) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) NewPosition);
	  return true;
	}

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SelEnd
//---------------------------------------------------------------------------

int TTrackBar::Get_SelEnd(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETSELEND, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_SelEnd(int NewSelEnd) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETSELEND, (WPARAM) TRUE, (LPARAM) NewSelEnd);
	  return true;
	}

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SelStart
//---------------------------------------------------------------------------

int TTrackBar::Get_SelStart(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETSELSTART, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_SelStart(int NewSelStart) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETSELSTART, (WPARAM) TRUE, (LPARAM) NewSelStart);
	  return true;
	}

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SliderVisible
//---------------------------------------------------------------------------

bool TTrackBar::Get_SliderVisible(void) {
  return ((FWindowStyle & TBS_NOTHUMB) == 0);
}

bool TTrackBar::Set_SliderVisible(bool NewSliderVisible) {
  DWORD NewWindowStyle;


  NewWindowStyle = GetWindowLong(FHandle, GWL_STYLE);
  if (NewSliderVisible) NewWindowStyle &= ~TBS_NOTHUMB;
  else NewWindowStyle |= TBS_NOTHUMB;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ThumbLength
//---------------------------------------------------------------------------

int TTrackBar::Get_ThumbLength(void) {
	if (FHandle) {
    return (long) SendMessage(FHandle, TBM_GETTHUMBLENGTH, 0, 0);
	}

	return 0;
}

bool TTrackBar::Set_ThumbLength(int NewThumbLength) {
	if (FHandle) {
    SendMessage(FHandle, TBM_SETTHUMBLENGTH, (WPARAM) NewThumbLength, 0);
	  return true;
	}

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TickMarks
//---------------------------------------------------------------------------

TTickMark TTrackBar::Get_TickMarks(void) {
  if (FWindowStyle & TBS_BOTH) return tmBoth;
  else if (FWindowStyle & TBS_TOP) return tmTopLeft;
  else if (FWindowStyle & TBS_LEFT) return tmTopLeft;
	else return tmBottomRight;
}

bool TTrackBar::Set_TickMarks(TTickMark NewTickMarks) {
  DWORD NewWindowStyle;


  NewWindowStyle = GetWindowLong(FHandle, GWL_STYLE);
  NewWindowStyle &= ~(TBS_BOTTOM | TBS_RIGHT | TBS_TOP | TBS_LEFT | TBS_BOTH);
  if (NewTickMarks == tmBoth) NewWindowStyle |= TBS_BOTH;
  else if (NewTickMarks == tmTopLeft) NewWindowStyle |= (TBS_TOP | TBS_LEFT);
	else NewWindowStyle |= (TBS_BOTTOM | TBS_RIGHT);
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TickStyle
//---------------------------------------------------------------------------

TTickStyle TTrackBar::Get_TickStyle(void) {
  if (FWindowStyle & TBS_NOTICKS) return tsNone;
  else if (FWindowStyle & TBS_AUTOTICKS) return tsAuto;
  else return tsManual;
}

bool TTrackBar::Set_TickStyle(TTickStyle NewTickStyle) {
  DWORD NewWindowStyle;


  NewWindowStyle = GetWindowLong(FHandle, GWL_STYLE);
  NewWindowStyle &= ~(TBS_NOTICKS | TBS_AUTOTICKS);
  if (NewTickStyle == tsNone) NewWindowStyle |= TBS_NOTICKS;
  else if (NewTickStyle == tsAuto) NewWindowStyle |= TBS_AUTOTICKS;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Ajoute une marque de graduation à l'emplacement spécifié.
//---------------------------------------------------------------------------
void FASTCALL TTrackBar::SetTick(int Value) {
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TTrackBar::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("EnableSelRange"));
  ListProperties->Add(_T("Frequency"));
  ListProperties->Add(_T("LineSize"));
  ListProperties->Add(_T("Max"));
  ListProperties->Add(_T("Min"));
  ListProperties->Add(_T("Orientation"));
  ListProperties->Add(_T("PageSize"));
  ListProperties->Add(_T("Position"));
  ListProperties->Add(_T("SelEnd"));
  ListProperties->Add(_T("SelStart"));
  ListProperties->Add(_T("SliderVisible"));
  ListProperties->Add(_T("ThumbLength"));
  ListProperties->Add(_T("TickMarks"));
  ListProperties->Add(_T("TickStyle"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TTrackBar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("EnableSelRange")) {
    return tpBool;
  }
  if (asProperty == _T("Position")) {
    return tpNumber;
  }
  if (asProperty == _T("Frequency")) {
    return tpNumber;
  }
  if (asProperty == _T("LineSize")) {
    return tpNumber;
  }
  if (asProperty == _T("Max")) {
    return tpNumber;
  }
  if (asProperty == _T("Min")) {
    return tpNumber;
  }
  if (asProperty == _T("Orientation")) {
    *asInfos = _T("trHorizontal\ntrVertical");
    return tpChoice;
  }
  if (asProperty == _T("PageSize")) {
    return tpNumber;
  }
  if (asProperty == _T("SelEnd")) {
    return tpNumber;
  }
  if (asProperty == _T("SelStart")) {
    return tpNumber;
  }
  if (asProperty == _T("SliderVisible")) {
    return tpBool;
  }
  if (asProperty == _T("ThumbLength")) {
    return tpNumber;
  }
  if (asProperty == _T("TickMarks")) {
    *asInfos = _T("tmBottomRight\ntmTopLeft\ntmBoth");
    return tpChoice;
  }
  if (asProperty == _T("TickStyle")) {
    *asInfos = _T("tsNone\ntsAuto\ntsManual");
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

AnsiString TTrackBar::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("EnableSelRange")) {
    return _T("False");
  }
  if (asProperty == _T("Frequency")) {
    return _T("0");
  }
  if (asProperty == _T("LineSize")) {
    return _T("0");
  }
  if (asProperty == _T("Max")) {
    return _T("100");
  }
  if (asProperty == _T("Min")) {
    return _T("0");
  }
  if (asProperty == _T("Orientation")) {
    return _T("trHorizontal");
  }
  if (asProperty == _T("PageSize")) {
    return _T("0");
  }
  if (asProperty == _T("Position")) {
    return _T("0");
  }
  if (asProperty == _T("SelEnd")) {
    return _T("0");
  }
  if (asProperty == _T("SelStart")) {
    return _T("0");
  }
  if (asProperty == _T("SliderVisible")) {
    return _T("False");
  }
  if (asProperty == _T("ThumbLength")) {
    return _T("0");
  }
  if (asProperty == _T("TickMarks")) {
    return _T("");
  }
  if (asProperty == _T("TickStyle")) {
    return _T("tsNone");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TTrackBar::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("EnableSelRange")) {
    if (Get_EnableSelRange()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Frequency")) {
    return IntToStr(Get_Frequency());
  }
  if (asProperty == _T("LineSize")) {
    return IntToStr(Get_LineSize());
  }
  if (asProperty == _T("Max")) {
    return IntToStr(Get_Max());
  }
  if (asProperty == _T("Min")) {
    return IntToStr(Get_Min());
  }
  if (asProperty == _T("Orientation")) {
    TTrackBarOrientation Orientation = Get_Orientation();
    if (Orientation == trHorizontal) return _T("trHorizontal");
    if (Orientation == trVertical) return _T("trVertical");
    return _T("");
  }
  if (asProperty == _T("PageSize")) {
    return IntToStr(Get_PageSize());
  }
  if (asProperty == _T("Position")) {
    return IntToStr(Get_Position());
  }
  if (asProperty == _T("SelEnd")) {
    return IntToStr(Get_SelEnd());
  }
  if (asProperty == _T("SelStart")) {
    return IntToStr(Get_SelStart());
  }
  if (asProperty == _T("SliderVisible")) {
    if (Get_SliderVisible()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ThumbLength")) {
    return IntToStr(Get_ThumbLength());
  }
  if (asProperty == _T("TickMarks")) {
    TTickMark TickMarks = Get_TickMarks();
    if (TickMarks == tmBottomRight) return _T("tmBottomRight");
    if (TickMarks == tmTopLeft) return _T("tmTopLeft");
    if (TickMarks == tmBoth) return _T("tmBoth");
    return _T("");
  }
  if (asProperty == _T("TickStyle")) {
    TTickStyle TickStyle = Get_TickStyle();
    if (TickStyle == tsNone) return _T("tsNone");
    if (TickStyle == tsAuto) return _T("tsAuto");
    if (TickStyle == tsManual) return _T("tsManual");
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

bool TTrackBar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("EnableSelRange")) {
    Set_EnableSelRange(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Frequency")) {
    Set_Frequency(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("LineSize")) {
    Set_LineSize(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Max")) {
    Set_Max(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Min")) {
    Set_Min(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Orientation")) {
    if      (asValue == _T("trHorizontal")) Set_Orientation(trHorizontal);
    else if (asValue == _T("trVertical")) Set_Orientation(trVertical);
    return true;
  }
  if (asProperty == _T("PageSize")) {
    Set_PageSize(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Position")) {
    Set_Position(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("SelEnd")) {
    Set_SelEnd(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("SelStart")) {
    Set_SelStart(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("SliderVisible")) {
    Set_SliderVisible(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ThumbLength")) {
    Set_ThumbLength(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("TickMarks")) {
    if      (asValue == _T("tmBottomRight")) Set_TickMarks(tmBottomRight);
    else if (asValue == _T("tmTopLeft")) Set_TickMarks(tmTopLeft);
    else if (asValue == _T("tmBoth")) Set_TickMarks(tmBoth);
    return true;
  }
  if (asProperty == _T("TickStyle")) {
    if      (asValue == _T("tsNone")) Set_TickStyle(tsNone);
    else if (asValue == _T("tsAuto")) Set_TickStyle(tsAuto);
    else if (asValue == _T("tsManual")) Set_TickStyle(tsManual);
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

