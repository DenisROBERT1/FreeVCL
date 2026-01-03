//-----------------------------------------------------------------------------
//! @file TTimer.cpp
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
// TTimer
//---------------------------------------------------------------------------

TTimer::TTimer(TComponent* AOwner): TComponent(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TTimer");
  FEnabled = true;
  FInterval = 1000;
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;

  // Enregistrement de la classe "TTimer":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TTimer");

  RegisterClass(&wc);

  // Création de la boîte
  Handle = CreateWindow(_T("TTimer"), _T(""),
                         0,
                         CW_USEDEFAULT, CW_USEDEFAULT,
                         CW_USEDEFAULT, CW_USEDEFAULT,
                         hWndParent,
                         (HMENU) 0, ::HInstance, this);

  if (Handle) {
    SetTimer(Handle, 1, FInterval, NULL);

  }

}

TTimer::~TTimer(void) {
  if (Handle) {
    if (FEnabled) KillTimer(Handle, 1);
    DestroyWindow(Handle);
  }
}

//---------------------------------------------------------------------------
bool TTimer::Get_Enabled(void) {
  return FEnabled;
}

//---------------------------------------------------------------------------
bool TTimer::Set_Enabled(bool NewEnabled) {
  if (FEnabled != NewEnabled) {
    FEnabled = NewEnabled;
    if (FEnabled) SetTimer(Handle, 1, FInterval, NULL);
    else KillTimer(Handle, 1);
  }

  return true;
}

//---------------------------------------------------------------------------
int TTimer::Get_Interval(void) {
  return FInterval;
}

//---------------------------------------------------------------------------
bool TTimer::Set_Interval(int NewInterval) {
  if (FInterval != NewInterval) {
    FInterval = NewInterval;
    if (FEnabled) {
      KillTimer(Handle, 1);
      SetTimer(Handle, 1, FInterval, NULL);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
bool TTimer::ProcessTimer(UINT IdTimer) {
  if (!FComponentState.Contains(csLoading)) OnTimer(this);

  return false;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TTimer::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Enabled"));
  ListProperties->Add(_T("Interval"));
  ListProperties->Add(_T("OnTimer"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TTimer::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Enabled")) {
    return tpBool;
  }
  if (asProperty == _T("Interval")) {
    return tpNumber;
  }
  if (asProperty == _T("OnTimer")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TTimer::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Enabled")) {
    return _T("True");
  }
  if (asProperty == _T("Interval")) {
    return _T("1000");
  }
  if (asProperty == _T("OnTimer")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TTimer::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Enabled")) {
    if (Get_Enabled()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Interval")) {
    return IntToStr(Get_Interval());
  }
  if (asProperty == _T("OnTimer")) {
    return OnTimer.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TTimer::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Enabled")) {
    Set_Enabled(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Interval")) {
    Set_Interval(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("OnTimer")) {
		OnTimer.ObjCall = Sender;
    OnTimer = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
LRESULT FAR PASCAL TTimer::WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
  TTimer *Timer;


  Timer = (TTimer *) (SIZE_T) GetWindowLongPtr(Handle, GWLP_USERDATA);
  if (Message == WM_CREATE && Timer == NULL) {
    LPCREATESTRUCT lpCreatStruct = (LPCREATESTRUCT) lParam;
    Timer = (TTimer *) lpCreatStruct->lpCreateParams;
    SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR) (SIZE_T) Timer);
  }

  if (Timer) {

    switch (Message) {

    case WM_TIMER:
      Timer->ProcessTimer(LOWORD(wParam));
      return 0;

    }

  }

  return DefWindowProc(Handle, Message, wParam, lParam);
}
//---------------------------------------------------------------------------

