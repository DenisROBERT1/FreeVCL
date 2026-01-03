//-----------------------------------------------------------------------------
//! @file TProgressBar.cpp
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
// TProgressBar
//---------------------------------------------------------------------------

TProgressBar::TProgressBar(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TProgressBar");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FFillColor = clActiveCaption;
  FStep = 10;

  // Enregistrement de la classe ProgressBar
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_PROGRESS_CLASS;
  InitCommonControlsEx(&InitCtrls);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         PROGRESS_CLASS, FCaption,
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
TProgressBar::~TProgressBar(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TProgressBar::DestroyWnd(void) {
  DestroyWindow(FHandle);
}

//---------------------------------------------------------------------------
void FASTCALL TProgressBar::CreateWnd(void) {
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
                         PROGRESS_CLASS, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    InitStdControl();
  }

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FillColor
//---------------------------------------------------------------------------
TColor TProgressBar::Get_FillColor(void) {
  return FFillColor;
}

//---------------------------------------------------------------------------
bool TProgressBar::Set_FillColor(TColor NewFillColor) {
  if (NewFillColor != FFillColor) {
    FFillColor = NewFillColor;
    if (FHandle) SendMessage(FHandle, PBM_SETBARCOLOR, 0, (LPARAM) FFillColor.cr);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool TProgressBar::Set_Color(TColor NewColor) {
  if (NewColor != FColor) {
    FColor = NewColor;
    if (FHandle) SendMessage(FHandle, PBM_SETBKCOLOR, 0, (LPARAM) FColor.cr);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Min
//---------------------------------------------------------------------------

int TProgressBar::Get_Min(void) {
  if (!FHandle) return 0;
	return (int) SendMessage(FHandle, PBM_GETRANGE, TRUE, 0);
}

bool TProgressBar::Set_Min(int NewMin) {
  if (FHandle) {
    int FMax = (int) SendMessage(FHandle, PBM_GETRANGE, FALSE, 0);
    SendMessage(FHandle, PBM_SETRANGE32, NewMin, FMax);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Max
//---------------------------------------------------------------------------

int TProgressBar::Get_Max(void) {
  if (!FHandle) return 0;
	return (int) SendMessage(FHandle, PBM_GETRANGE, FALSE, 0);
}

bool TProgressBar::Set_Max(int NewMax) {
  if (FHandle) {
    int FMin = (int) SendMessage(FHandle, PBM_GETRANGE, TRUE, 0);
    SendMessage(FHandle, PBM_SETRANGE32, FMin, NewMax);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Step
//---------------------------------------------------------------------------

int TProgressBar::Get_Step(void) {
  return FStep;
}

bool TProgressBar::Set_Step(int NewStep) {
  if (FStep != NewStep) {
    FStep = NewStep;
    if (FHandle) SendMessage(FHandle, PBM_SETSTEP, FStep, 0);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Smooth
//---------------------------------------------------------------------------

bool TProgressBar::Get_Smooth(void) {
  return ((FWindowStyle & PBS_SMOOTH) != 0);
}

bool TProgressBar::Set_Smooth(bool NewSmooth) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewSmooth) NewWindowStyle |= PBS_SMOOTH;
  else NewWindowStyle &= ~PBS_SMOOTH;
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
// Accesseurs de la propriété Orientation
//---------------------------------------------------------------------------

TProgressBarOrientation TProgressBar::Get_Orientation(void) {
  if (FWindowStyle & PBS_VERTICAL) return pbVertical;
  else return pbHorizontal;
}

bool TProgressBar::Set_Orientation(TProgressBarOrientation NewOrientation) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewOrientation == pbVertical) NewWindowStyle |= PBS_VERTICAL;
  else NewWindowStyle &= ~PBS_VERTICAL;
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
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

int TProgressBar::Get_Position(void) {
  if (FHandle) return (int) SendMessage(FHandle, PBM_GETPOS, 0, 0);
  return 0;
}

bool TProgressBar::Set_Position(int NewPosition) {
  if (FHandle) SendMessage(FHandle, PBM_SETPOS, NewPosition, 0);
  return true;
}


//---------------------------------------------------------------------------
// Avance la position de la barre de la valeur spécifiée (Delta).
//---------------------------------------------------------------------------
void FASTCALL TProgressBar::StepBy(int Delta) {
  int FPosition = Get_Position();
  Set_Position(FPosition + Delta);
}


//---------------------------------------------------------------------------
// Avance la position de la valeur spécifiée par la propriété Step.
//---------------------------------------------------------------------------
void FASTCALL TProgressBar::StepIt(void) {
  if (FHandle) SendMessage(FHandle, PBM_STEPIT, 0, 0);
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TProgressBar::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Orientation"));  // Orientation doit rester en premier (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("Smooth"));  // Smooth doit rester en début (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("FillColor"));
  ListProperties->Add(_T("Min"));
  ListProperties->Add(_T("Max"));
  ListProperties->Add(_T("Step"));
  ListProperties->Add(_T("Position"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TProgressBar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Orientation")) {
    *asInfos = _T("pbHorizontal\npbVertical");
    return tpChoice;
  }
  if (asProperty == _T("Smooth")) {
    return tpBool;
  }
  if (asProperty == _T("FillColor")) {
    return tpColor;
  }
  if (asProperty == _T("Min")) {
    return tpNumber;
  }
  if (asProperty == _T("Max")) {
    return tpNumber;
  }
  if (asProperty == _T("Step")) {
    return tpNumber;
  }
  if (asProperty == _T("Position")) {
    return tpNumber;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TProgressBar::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Orientation")) {
    return _T("pbHorizontal");
  }
  if (asProperty == _T("Smooth")) {
    return _T("False");
  }
  if (asProperty == _T("Color")) {
    return _T("clActiveCaption");
  }
  if (asProperty == _T("Min")) {
    return _T("0");
  }
  if (asProperty == _T("Max")) {
    return _T("100");
  }
  if (asProperty == _T("Step")) {
    return _T("0");
  }
  if (asProperty == _T("Position")) {
    return _T("0");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TProgressBar::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Orientation")) {
    TProgressBarOrientation FOrientation = Get_Orientation();
    if (FOrientation == pbVertical) return _T("pbVertical");
    return _T("pbHorizontal");
  }
  if (asProperty == _T("Smooth")) {
    if (Get_Smooth()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("FillColor")) {
    return ColorToString(Get_FillColor());
  }
  if (asProperty == _T("Min")) {
    return IntToStr(Get_Min());
  }
  if (asProperty == _T("Max")) {
    return IntToStr(Get_Max());
  }
  if (asProperty == _T("Step")) {
    return IntToStr(Get_Step());
  }
  if (asProperty == _T("Position")) {
    return IntToStr(Get_Position());
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TProgressBar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Orientation")) {
    if (asValue == _T("pbHorizontal")) Set_Orientation(pbHorizontal);
    else if (asValue == _T("pbVertical")) Set_Orientation(pbVertical);
    return true;
  }
  if (asProperty == _T("Smooth")) {
    Set_Smooth(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("FillColor")) {
    TColor FillColor;
    FillColor = asValue;
    Set_Color(FillColor);
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
  if (asProperty == _T("Step")) {
    Set_Step(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Position")) {
    Set_Position(StrToLong(asValue));
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

