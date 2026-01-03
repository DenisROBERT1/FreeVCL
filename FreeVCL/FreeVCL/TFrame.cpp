//-----------------------------------------------------------------------------
//! @file TFrame.cpp
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

//---------------------------------------------------------------------------
// TFrame
//---------------------------------------------------------------------------

TFrame::TFrame(TComponent *AOwner):
           TFormFrame(AOwner) {
  HWND hWndParent = NULL;
  WNDCLASS wc;
  HFONT hFont;


  // Initialisations
  FClassName = _T("TFrame");
  TWinControl *Parent = dynamic_cast<TWinControl *>(AOwner);
  if (Parent) hWndParent = Parent->Handle;
  FTabStop = true;
  FTabOrder = 0;

  // Enregistrement de la classe "TFrame":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TFrame");

  RegisterClass(&wc);

  // Création de la fenêtre
  FHandle = CreateWindowEx(FExWindowStyle,
                           _T("TFrame"), FCaption,
                           FWindowStyle,
                           FLeft, FTop,
                           FWidth, FHeight,
                           hWndParent,
                           NULL, HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

    bProcessCreateToDo = true;
  }
}

//---------------------------------------------------------------------------
TFrame::~TFrame(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

bool TFrame::Set_Name(AnsiString NewName) {
  if (FName != NewName) {
    AnsiString ResourceName;
    TPersistent::Set_Name(NewName);
    // $$$ C'est pas bon, ça: ça empêche d'avoir deux frames de même type dans une même form
    // => le nom du dialogue devrait être indépendant de la propriété Name.
    ResourceName = AnsiString(_T("T")) + NewName;
    LoadDialog(ResourceName, GetListComponents());
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TFrame::GetListProperties(TStrings *ListProperties) {

  TFormFrame::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TFrame::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TFormFrame::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TFrame::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TFormFrame::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TFrame::GetProperty(AnsiString asProperty) {
  return TFormFrame::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TFrame::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TFormFrame::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

