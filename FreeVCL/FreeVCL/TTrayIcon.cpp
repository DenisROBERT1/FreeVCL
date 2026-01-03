//-----------------------------------------------------------------------------
//! @file TTrayIcon.cpp
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
// Constructeur
//---------------------------------------------------------------------------

TTrayIcon::TTrayIcon(TComponent* AOwner):
           TWinControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;
  NOTIFYICONDATA tnd;


  // Initialisations
  FClassName = _T("TTrayIcon");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle &= ~WS_VISIBLE;

  // Enregistrement de la classe "TTrayIcon":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TTrayIcon");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TTrayIcon"), FCaption,
                         FWindowStyle,
                         0, 0,
                         1, 1,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    memset(&tnd, 0, sizeof(NOTIFYICONDATA));
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = FHandle;
    tnd.uID = IDC_TRAYICON;
    tnd.uFlags = NIF_MESSAGE | NIF_TIP;
    tnd.uCallbackMessage = MYWM_NOTIFY;
    lstrcpyn(tnd.szTip, FHint, sizeof(tnd.szTip));

    Shell_NotifyIcon(NIM_ADD, &tnd);

    bProcessCreateToDo = true;
  }

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TTrayIcon::~TTrayIcon(void) {
}

//---------------------------------------------------------------------------
void TTrayIcon::ProcessDestroy(void) {
  NOTIFYICONDATA tnd;


  memset(&tnd, 0, sizeof(NOTIFYICONDATA));
  tnd.cbSize = sizeof(NOTIFYICONDATA);
  tnd.hWnd = FHandle;
  tnd.uID = IDC_TRAYICON;

  Shell_NotifyIcon(NIM_DELETE, &tnd);
}

//---------------------------------------------------------------------------
bool TTrayIcon::ProcessUser(UINT Message, WPARAM wParam, LPARAM lParam) {
  TShiftState ShiftState;


  if (Message == MYWM_NOTIFY) {
    ShiftState.Clear();

    switch (lParam) {
    case WM_LBUTTONDOWN:
			OnMouseDown(this, mbLeft, ShiftState, 0, 0);
      break;
    case WM_LBUTTONUP:
			OnMouseUp(this, mbLeft, ShiftState, 0, 0);
      break;
    case WM_RBUTTONDOWN:
			OnMouseDown(this, mbRight, ShiftState, 0, 0);
      break;
    case WM_RBUTTONUP:
			OnMouseUp(this, mbRight, ShiftState, 0, 0);
      break;
    default:
      break;
    }

  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Icon
//---------------------------------------------------------------------------

TIcon *TTrayIcon::Get_Icon(void) {
  return FIcon;
}

bool TTrayIcon::Set_Icon(TIcon *NewIcon) {
  NOTIFYICONDATA tnd;


  if (FIcon != NewIcon) {
    FIcon = NewIcon;

    memset(&tnd, 0, sizeof(NOTIFYICONDATA));
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = FHandle;
    tnd.uID = IDC_TRAYICON;
    tnd.uFlags = NIF_ICON;
    tnd.hIcon = FIcon->Handle;

    Shell_NotifyIcon(NIM_MODIFY, &tnd);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Hint
//---------------------------------------------------------------------------

bool TTrayIcon::Set_Hint(AnsiString NewHint) {
  NOTIFYICONDATA tnd;


  if (FHint != NewHint) {
    FHint = NewHint;

    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = FHandle;
    tnd.uID = IDC_TRAYICON;
    tnd.uFlags = NIF_TIP;
    lstrcpyn(tnd.szTip, FHint, sizeof(tnd.szTip));

    Shell_NotifyIcon(NIM_MODIFY, &tnd);
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TTrayIcon::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("OnPaint"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TTrayIcon::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("OnPaint")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TTrayIcon::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("OnPaint")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TTrayIcon::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("OnPaint")) {
    return OnPaint.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TTrayIcon::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("OnPaint")) {
		OnPaint.ObjCall = Sender;
    OnPaint = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
/*
void TTrayIcon::NotifyMsg(TMessage& Msg) {
  TShiftState ShiftState;

  switch (Msg.LParam) {
  case WM_LBUTTONDOWN:
    ShiftState.Clear();
    OnMouseDown(this, mbLeft, ShiftState, 0, 0);
    break;
  case WM_RBUTTONUP:
    ShiftState.Clear();
    OnMouseDown(this, mbRight, ShiftState, 0, 0);
    break;
  default:
    break;
  }
  TWinControl::Dispatch(&Msg);
} */

//---------------------------------------------------------------------------
