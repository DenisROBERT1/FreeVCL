//-----------------------------------------------------------------------------
//! @file TScrollBox.cpp
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
// TScrollBox
//---------------------------------------------------------------------------

TScrollBox::TScrollBox(TComponent* AOwner): TWinControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TScrollBox");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FExWindowStyle |= WS_EX_CLIENTEDGE;
  FWindowStyle |= WS_HSCROLL | WS_VSCROLL;
	XMaxScroll = 0;
	YMaxScroll = 0;
  XContainer = 0;
  YContainer = 0;
  bAutoRange = true;

  // Enregistrement de la classe "TScrollBox":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TScrollBox");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TScrollBox"), FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TScrollBox::~TScrollBox(void) {
}

//---------------------------------------------------------------------------
// Origine de la fenêtre Windows
//---------------------------------------------------------------------------

int TScrollBox::Get_LeftWnd(void) {
  return XContainer;
}


//---------------------------------------------------------------------------
// Origine de la fenêtre Windows
//---------------------------------------------------------------------------

int TScrollBox::Get_TopWnd(void) {
  return YContainer;
}


//---------------------------------------------------------------------------
// Amène un contrôle dans la partie visible
//---------------------------------------------------------------------------

void TScrollBox::ScrollInView(TControl* AControl) {
  RECT ClientRect;
  int OldXContainer;
  int OldYContainer;
  SCROLLINFO ScrollInfo;


  OldXContainer = XContainer;
  OldYContainer = YContainer;

  ::GetClientRect(FHandle, &ClientRect);
  if (XContainer > ClientRect.right - (int) AControl->Left - (int) AControl->Width) {
    XContainer = ClientRect.right - (int) AControl->Left - (int) AControl->Width;
  }
  if (XContainer < - (int) AControl->Left) {
    XContainer = - (int) AControl->Left;
  }
  if (YContainer > ClientRect.bottom - (int) AControl->Top - (int) AControl->Height) {
    YContainer = ClientRect.bottom - (int) AControl->Top - (int) AControl->Height;
  }
  if (YContainer < - (int) AControl->Top) {
    YContainer = - (int) AControl->Top;
  }

  if (XContainer > 0) XContainer = 0;
  if (YContainer > 0) YContainer = 0;
  if (XContainer != OldXContainer ||
      YContainer != OldYContainer) {
    if (XContainer != OldXContainer) {
      ScrollInfo.cbSize = sizeof(SCROLLINFO);
      ScrollInfo.fMask = SIF_POS;
      ScrollInfo.nPos = -XContainer;
      SetScrollInfo(FHandle, SB_HORZ, &ScrollInfo, TRUE);
    }
    if (YContainer != OldYContainer) {
      ScrollInfo.cbSize = sizeof(SCROLLINFO);
      ScrollInfo.fMask = SIF_POS;
      ScrollInfo.nPos = -YContainer;
      SetScrollInfo(FHandle, SB_VERT, &ScrollInfo, TRUE);
    }
    ScrollControls();
  }
}

//---------------------------------------------------------------------------
bool TScrollBox::ProcessShowWindow(bool bShow) {
  if (bShow) {
    int L = Get_ClientWidth();
    int H = Get_ClientHeight();
    ProcessSize(0, L, H);
  }

  return TWinControl::ProcessShowWindow(bShow);
}

//---------------------------------------------------------------------------
// Insertion d'un composant enfant
//---------------------------------------------------------------------------

void TScrollBox::InsertComponent(TComponent* AComponent, int Index) {
  TComponent::InsertComponent(AComponent, Index);
  CalculRangeScrollBars();
}

//---------------------------------------------------------------------------
// Suppression d'un composant enfant de la liste des composants
//---------------------------------------------------------------------------

void TScrollBox::RemoveComponent(TComponent* AComponent) {
  TComponent::RemoveComponent(AComponent);
  CalculRangeScrollBars();
}

//---------------------------------------------------------------------------
// Désactive le défilement automatique
//---------------------------------------------------------------------------

void FASTCALL TScrollBox::DisableAutoRange(void) {
  if (bAutoRange) {
    bAutoRange = false;
  }
}

//---------------------------------------------------------------------------
// Réactive le défilement automatique
//---------------------------------------------------------------------------

void FASTCALL TScrollBox::EnableAutoRange(void) {
  if (!bAutoRange) {
    bAutoRange = true;
    CalculRangeScrollBars();
  }
}

//---------------------------------------------------------------------------
bool TScrollBox::ProcessSize(int SizeType, int L, int H) {
  RECT Rect;


  if (FHandle) {
    GetWindowRect(FHandle, &Rect);
    L = Rect.right - Rect.left;
    H = Rect.bottom - Rect.top;
  }
  FWidth = L;
  FHeight = H;
  if (FHandle) InvalidateRect(FHandle, NULL, TRUE);

  CalculRangeScrollBars();

  return false;
}


//---------------------------------------------------------------------------
// Calcul des paramètres des scroll-bars
//---------------------------------------------------------------------------

void TScrollBox::CalculRangeScrollBars(void) {
  int i;
  int LeftControl, TopControl, WidthControl, HeightControl;
  TControl *Control;
  SCROLLINFO ScrollInfo;


  if (bAutoRange) {
    XMaxScroll = 0;
    YMaxScroll = 0;
    for (i = 0; i < (int) FComponents.size(); i++) {
      Control = dynamic_cast<TControl *> (FComponents[i]);
      if (Control) {
        LeftControl = Control->Left;
        TopControl = Control->Top;
        WidthControl = Control->Width;
        HeightControl = Control->Height;
        if (XMaxScroll < LeftControl + WidthControl) XMaxScroll = LeftControl + WidthControl;
        if (YMaxScroll < TopControl + HeightControl) YMaxScroll = TopControl + HeightControl;
      }
    }

    ScrollInfo.cbSize = sizeof(SCROLLINFO);
    ScrollInfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
    ScrollInfo.nMin = 0;
    ScrollInfo.nMax = XMaxScroll + 2;  // 2 pixels en plus pour laisser une marge après le dernier contrôle
    ScrollInfo.nPage = FWidth - 4;  // 4 pixels en moins à cause des borders
    ScrollInfo.nPos = -XContainer;
    ScrollInfo.nTrackPos = 0;
    SetScrollInfo(FHandle, SB_HORZ, &ScrollInfo, TRUE);

    ScrollInfo.nMax = YMaxScroll + 2;  // 2 pixels en plus pour laisser une marge après le dernier contrôle
    ScrollInfo.nPage = FHeight - 4;  // 4 pixels en moins à cause des borders
    ScrollInfo.nPos = -YContainer;
    SetScrollInfo(FHandle, SB_VERT, &ScrollInfo, TRUE);
  }
}

//---------------------------------------------------------------------------
bool TScrollBox::ProcessHScroll(int Command, int Info) {
  int OldXContainer;
	int MinX;
  SCROLLINFO ScrollInfo;


  OldXContainer = XContainer;
  switch (Command) {
  case SB_LINELEFT:
    if (FWidth < 100) XContainer++;
    else XContainer += 10;
    break;
  case SB_LINERIGHT:
    if (FWidth < 100) XContainer--;
    else XContainer -= 10;
    break;
  case SB_PAGELEFT:
    XContainer += FWidth / 2;
    break;
  case SB_PAGERIGHT:
    XContainer -= FWidth / 2;
    break;
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    XContainer = -Info;
    break;
  }

  if (XContainer > 0) XContainer = 0;
	MinX = -(XMaxScroll + 2 - (FWidth - 4));
  if (XContainer < MinX) XContainer = MinX;
  if (XContainer != OldXContainer) {
    ScrollInfo.cbSize = sizeof(SCROLLINFO);
    ScrollInfo.fMask = SIF_POS;
    ScrollInfo.nPos = -XContainer;
    SetScrollInfo(FHandle, SB_HORZ, &ScrollInfo, TRUE);
    ScrollControls();
  }

  return true;
}

//---------------------------------------------------------------------------
bool TScrollBox::ProcessVScroll(int Command, int Info) {
  int OldYContainer;
	int MinY;
  SCROLLINFO ScrollInfo;


  OldYContainer = YContainer;
  switch (Command) {
  case SB_LINEUP:
    if (FHeight < 100) YContainer++;
    else YContainer += 10;
    break;
  case SB_LINEDOWN:
    if (FHeight < 100) YContainer--;
    else YContainer -= 10;
    break;
  case SB_PAGEUP:
    YContainer += FHeight / 2;
    break;
  case SB_PAGEDOWN:
    YContainer -= FHeight / 2;
    break;
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    YContainer = -Info;
    break;
  }

  if (YContainer > 0) YContainer = 0;
	MinY = -(YMaxScroll + 2 - (FHeight - 4));
  if (YContainer < MinY) YContainer = MinY;
  if (YContainer != OldYContainer) {
    ScrollInfo.cbSize = sizeof(SCROLLINFO);
    ScrollInfo.fMask = SIF_POS;
    ScrollInfo.nPos = -YContainer;
    SetScrollInfo(FHandle, SB_VERT, &ScrollInfo, TRUE);
    ScrollControls();
  }

  return true;
}

//---------------------------------------------------------------------------
bool TScrollBox::ProcessMouseWheel(WORD fwKeys, short zDelta, int X, int Y) {
  int OldYContainer;
	int MinY;
  SCROLLINFO ScrollInfo;


  OldYContainer = YContainer;
  if (FHeight < 100) {
		if (zDelta < 0) YContainer--;
		else YContainer++;
	}
  else YContainer += zDelta / 4;
  if (YContainer > 0) YContainer = 0;
	MinY = -(YMaxScroll + 2 - (FHeight - 4));
  if (YContainer < MinY) YContainer = MinY;

	if (YContainer != OldYContainer) {
    ScrollInfo.cbSize = sizeof(SCROLLINFO);
    ScrollInfo.fMask = SIF_POS;
    ScrollInfo.nPos = -YContainer;
    SetScrollInfo(FHandle, SB_VERT, &ScrollInfo, TRUE);
    ScrollControls();
  }

  return true;
}

//---------------------------------------------------------------------------
bool TScrollBox::ScrollControls(void) {
  int i;
  int X, Y, L, H;
  TControl *Control;


  XMaxScroll = 0;
  YMaxScroll = 0;
  for (i = 0; i < (int) FComponents.size(); i++) {
    Control = dynamic_cast<TControl *> (FComponents[i]);
    if (Control) {
      X = Control->Left;
      Y = Control->Top;
      L = Control->Width;
      H = Control->Height;
      if (XMaxScroll < X + L) XMaxScroll = X + L;
      if (YMaxScroll < Y + H) YMaxScroll = Y + H;
      Control->Size(X, Y, L, H, true);
    }
  }

  InvalidateRect(FHandle, NULL, TRUE);
  UpdateWindow(FHandle);

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TScrollBox::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TScrollBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TScrollBox::GetDefaultProperty(AnsiString asProperty) {
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TScrollBox::GetProperty(AnsiString asProperty) {
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TScrollBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

