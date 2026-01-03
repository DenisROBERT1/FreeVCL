//-----------------------------------------------------------------------------
//! @file TCustomControl.cpp
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
// TCustomControl
//---------------------------------------------------------------------------

TCustomControl::TCustomControl(TComponent* AOwner): TWinControl(AOwner) {
  WNDCLASS wc;
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TCustomControl");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FTabStop = true;
  FColor = clWindow;

  // Enregistrement de la classe "TCustomControl":
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TCustomControl");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TCustomControl"), FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

    bWantPaint = true;
    FHorzScrollBar = new TControlScrollBar(this, sbHorizontal);
    FVertScrollBar = new TControlScrollBar(this, sbVertical);
    bProcessCreateToDo = true;
  }

}

TCustomControl::~TCustomControl(void) {
  delete FHorzScrollBar;
  delete FVertScrollBar;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété HorzScrollBar
//---------------------------------------------------------------------------

TControlScrollBar *TCustomControl::Get_HorzScrollBar(void) {
  return FHorzScrollBar;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété VertScrollBar
//---------------------------------------------------------------------------

TControlScrollBar *TCustomControl::Get_VertScrollBar(void) {
  return FVertScrollBar;
}


//---------------------------------------------------------------------------
// Dessin du contrôle
//---------------------------------------------------------------------------

bool TCustomControl::ProcessPaint(HDC hdc, RECT rcPaint) {

	Canvas->Handle = hdc;
  SelectObject(hdc, FFont->Handle);
  Paint();
	FCanvas->FreeHandle();

  return true;
}


//---------------------------------------------------------------------------
bool TCustomControl::ProcessHScroll(int Command, int Info) {
  return FHorzScrollBar->ProcessScroll(Command, Info);
}

//---------------------------------------------------------------------------
bool TCustomControl::ProcessVScroll(int Command, int Info) {
  return FVertScrollBar->ProcessScroll(Command, Info);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomControl::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomControl::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomControl::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Color")) {
    return _T("clWindow");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomControl::GetProperty(AnsiString asProperty) {
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomControl::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

