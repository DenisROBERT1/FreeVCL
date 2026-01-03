//-----------------------------------------------------------------------------
//! @file TPageScroller.cpp
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
// TPageScroller
//---------------------------------------------------------------------------

TPageScroller::TPageScroller(TComponent* AOwner): TWinControl(AOwner) {
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TPageScroller");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= PGS_HORZ;
	FControl = NULL;
	FIncrement = 10;
	FMargin = 0;

  // Enregistrement de la classe PageScroller
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_PAGESCROLLER_CLASS;
  InitCommonControlsEx(&InitCtrls);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_PAGESCROLLER, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    InitStdControl();
    bProcessCreateToDo = true;

		Pager_SetButtonSize(FHandle, 12);

	}

}

//---------------------------------------------------------------------------
TPageScroller::~TPageScroller(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TPageScroller::DestroyWnd(void) {
  DestroyWindow(FHandle);
}

//---------------------------------------------------------------------------
void FASTCALL TPageScroller::CreateWnd(void) {
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
                         WC_PAGESCROLLER, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    InitStdControl();

		Pager_SetButtonSize(FHandle, 12);

  }

}

//---------------------------------------------------------------------------
int TPageScroller::ProcessNotify(LPNMHDR pnmh) {

  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
	case PGN_CALCSIZE:
		LPNMPGCALCSIZE pCalcSize;
		pCalcSize = (LPNMPGCALCSIZE) pnmh;
		if (pCalcSize->dwFlag == PGF_CALCWIDTH) {
			if (Control) pCalcSize->iWidth = Control->Width;
			else pCalcSize->iWidth = 0;
		}
		else {
			if (Control) pCalcSize->iHeight = Control->Height;
			else pCalcSize->iHeight = 0;
		}

		return TRUE;
	case PGN_SCROLL:
		LPNMPGSCROLL pnms;
		pnms = (LPNMPGSCROLL) pnmh;
		pnms->iScroll = FIncrement;
		OnScroll(this);

		return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------
bool TPageScroller::Set_Color(TColor NewColor) {
  if (FColor != NewColor) {
		FColor = NewColor; 
    Pager_SetBkColor(FHandle, FColor.cr);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientRect
//---------------------------------------------------------------------------

TRect TPageScroller::Get_ClientRect(void) {
  TRect Retour;
	int ButtonSize;


	ButtonSize = Pager_GetButtonSize(FHandle);
	if ((FWindowStyle & PGS_HORZ) != 0) {
		Retour.Left = ButtonSize + FMargin;
		Retour.Top = FMargin;
		Retour.Right = FWidth - 2 * FMargin - 2 * ButtonSize;
		Retour.Bottom = FHeight - 2 * FMargin;
	}
	else {
		Retour.Left = FMargin;
		Retour.Top = ButtonSize + FMargin;
		Retour.Right = FWidth - 2 * FMargin;
		Retour.Bottom = FHeight - 2 * FMargin - 2 * ButtonSize;
	}

  return Retour;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Orientation
//---------------------------------------------------------------------------

TPageScrollerOrientation TPageScroller::Get_Orientation() {
  if ((FWindowStyle & PGS_HORZ) != 0) return soHorizontal;
	else return soVertical;
}

bool TPageScroller::Set_Orientation(TPageScrollerOrientation NewOrientation) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle & ~(PGS_HORZ | PGS_VERT);
  if (NewOrientation == soHorizontal) NewWindowStyle |= PGS_HORZ;
  else if (NewOrientation == soVertical) NewWindowStyle |= PGS_VERT;
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
// Accesseurs de la propriété ButtonSize
//---------------------------------------------------------------------------

int TPageScroller::Get_ButtonSize(void) {

	if (FHandle) return Pager_GetButtonSize(FHandle);

	return 12;
}

bool TPageScroller::Set_ButtonSize(int NewButtonSize) {
	if (FHandle) {
		Pager_SetButtonSize(FHandle, NewButtonSize);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Control
//---------------------------------------------------------------------------

TWinControl *TPageScroller::Get_Control(void) {
	return FControl;
}

bool TPageScroller::Set_Control(TWinControl *NewControl) {
	if (FControl != NewControl) {
		FControl = NewControl;
		if (FHandle) {
			Pager_SetChild(FHandle, (HANDLE) FControl->Handle);
			Pager_RecalcSize(FHandle);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Increment
//---------------------------------------------------------------------------

int TPageScroller::Get_Increment(void) {
	return FIncrement;
}

bool TPageScroller::Set_Increment(int NewIncrement) {
	if (FIncrement != NewIncrement) {
		FIncrement = NewIncrement;
		Pager_RecalcSize(FHandle);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Margin
//---------------------------------------------------------------------------

int TPageScroller::Get_Margin(void) {
	return FMargin;
}

bool TPageScroller::Set_Margin(int NewMargin) {
	if (FMargin != NewMargin) {
		FMargin = NewMargin;
		Pager_RecalcSize(FHandle);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

int TPageScroller::Get_Position(void) {

	// if (FHandle) return Pager_GetPos(FHandle);
	if (FHandle) return (int) SendMessage(FHandle, PGM_GETPOS, 0, 0);

	return 0;
}

bool TPageScroller::Set_Position(int NewPosition) {
	if (FHandle) {
		// Pager_SetPos(FHandle, NewPosition);
		SendMessage(FHandle, PGM_SETPOS, 0, (LPARAM) NewPosition);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// Etat d'un bouton
//---------------------------------------------------------------------------
TPageScrollerButtonState FASTCALL TPageScroller::GetButtonState(TPageScrollerButton Button) {
	// return static_cast<TPageScrollerButtonState>(Pager_GetButtonState(FHandle, (int) Button));
	return static_cast<TPageScrollerButtonState>(SendMessage(FHandle, PGM_GETBUTTONSTATE, 0, (LPARAM) (int) Button));
}

//---------------------------------------------------------------------------
// Insertion d'un composant enfant
//---------------------------------------------------------------------------

void TPageScroller::InsertComponent(TComponent* AComponent, int Index) {
  TComponent::InsertComponent(AComponent, Index);
	if (FControl == NULL) {
		TWinControl *WinControl = dynamic_cast<TWinControl *>(AComponent);
		if (WinControl) Set_Control(WinControl);
	}
}

//---------------------------------------------------------------------------
// Suppression d'un composant enfant de la liste des composants
//---------------------------------------------------------------------------

void TPageScroller::RemoveComponent(TComponent* AComponent) {
	if (AComponent == FControl) FControl = NULL;
  TComponent::RemoveComponent(AComponent);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPageScroller::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Orientation"));  // Orientation doit rester en premier (parce que ça provoque un CreateWnd)
  ListProperties->Add(_T("ButtonSize"));
  ListProperties->Add(_T("Control"));
  ListProperties->Add(_T("Increment"));
  ListProperties->Add(_T("Margin"));
  ListProperties->Add(_T("Position"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPageScroller::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Orientation")) {
    *asInfos = _T("soHorizontal\nsoVertical");
    return tpChoice;
  }
  if (asProperty == _T("ButtonSize")) {
    return tpNumber;
  }
  if (asProperty == _T("Control")) {
    return tpObject;
  }
  if (asProperty == _T("Increment")) {
    return tpNumber;
  }
  if (asProperty == _T("Margin")) {
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

AnsiString TPageScroller::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Orientation")) {
    return _T("soHorizontal");
  }
  if (asProperty == _T("ButtonSize")) {
    return _T("12");
  }
  if (asProperty == _T("Control")) {
    return _T("");
  }
  if (asProperty == _T("Increment")) {
    return _T("10");
  }
  if (asProperty == _T("Margin")) {
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

AnsiString TPageScroller::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Orientation")) {
    TPageScrollerOrientation Orientation = Get_Orientation();
    if (Orientation == soHorizontal) return _T("soHorizontal");
    if (Orientation == soVertical) return _T("soVertical");
    return _T("");
  }
  if (asProperty == _T("ButtonSize")) {
    return IntToStr(Get_ButtonSize());
  }
  if (asProperty == _T("Control")) {
    TWinControl *WinControl = Get_Control();
    if (WinControl) return WinControl->Name;
    return _T("");
  }
  if (asProperty == _T("Increment")) {
    return IntToStr(Get_Increment());
  }
  if (asProperty == _T("Margin")) {
    return IntToStr(Get_Margin());
  }
  if (asProperty == _T("Position")) {
    return IntToStr(Get_Position());
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPageScroller::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Orientation")) {
    if      (asValue == _T("soHorizontal")) Set_Orientation(soHorizontal);
    else if (asValue == _T("soVertical")) Set_Orientation(soVertical);
    return true;
  }
  if (asProperty == _T("ButtonSize")) {
    Set_ButtonSize(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Control")) {
    TWinControl *WinControl;
		if (asValue.IsEmpty()) WinControl = NULL;
		else WinControl = dynamic_cast<TWinControl *>(FindRootComponent()->FindComponent(asValue));
    Set_Control(WinControl);
    return true;
  }
  if (asProperty == _T("Increment")) {
    Set_Increment(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Margin")) {
    Set_Margin(StrToLong(asValue));
    return true;
  }
  if (asProperty == _T("Position")) {
    Set_Position(StrToLong(asValue));
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

