//-----------------------------------------------------------------------------
//! @file TPaintBox.cpp
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
// TPaintBox
//---------------------------------------------------------------------------

TPaintBox::TPaintBox(TComponent* AOwner): TCustomControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TPaintBox");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FColor = clWindow;

  // Enregistrement de la classe "TPaintBox":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TPaintBox");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TPaintBox"), FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    bWantPaint = true;
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TPaintBox::~TPaintBox(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TPaintBox::Paint(void) {
  if (!FComponentState.Contains(csLoading)) OnPaint(this);
}


//---------------------------------------------------------------------------
bool TPaintBox::ProcessLButtonDown(int X, int Y) {
  if (!FComponentState.Contains(csLoading)) {
		if (FAction) FAction->Execute();
		OnClick(this);
		TWinControl::ProcessLButtonDown(X, Y);
  }
  return false;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPaintBox::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("OnPaint"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPaintBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("OnPaint")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TPaintBox::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("OnPaint")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TPaintBox::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("OnPaint")) {
    return OnPaint.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPaintBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("OnPaint")) {
		OnPaint.ObjCall = Sender;
    OnPaint = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

