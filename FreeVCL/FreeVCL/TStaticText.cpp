//-----------------------------------------------------------------------------
//! @file TStaticText.cpp
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
// TStaticText
//---------------------------------------------------------------------------

TStaticText::TStaticText(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TStaticText");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= SS_NOTIFY;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_STATIC, FCaption,
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
TStaticText::~TStaticText(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TStaticText::Get_Alignment(void) {
  switch (FWindowStyle & (ES_LEFT | ES_CENTER | ES_RIGHT)) {
    case ES_LEFT: return taLeftJustify;
    case ES_CENTER: return taCenter;
    case ES_RIGHT: return taRightJustify;
  }

  return taLeftJustify;
}

bool TStaticText::Set_Alignment(TAlignment NewAlignment) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  NewWindowStyle &= (DWORD) ~(ES_LEFT | ES_CENTER | ES_RIGHT);
  if (NewAlignment == taLeftJustify) NewWindowStyle |= (DWORD) ES_LEFT;
  else if (NewAlignment == taCenter) NewWindowStyle |= (DWORD) ES_CENTER;
  else if (NewAlignment == taRightJustify) NewWindowStyle |= (DWORD) ES_RIGHT;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TStaticText::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Alignment"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TStaticText::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Alignment")) {
    *asInfos = _T("taLeftJustify\ntaRightJustify\ntaCenter");
    return tpChoice;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TStaticText::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    return _T("taLeftJustify");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TStaticText::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    TAlignment Alignment = Get_Alignment();
    if (Alignment == taLeftJustify) return _T("taLeftJustify");
    if (Alignment == taRightJustify) return _T("taRightJustify");
    if (Alignment == taCenter) return _T("taCenter");
    return _T("taLeftJustify");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TStaticText::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Alignment")) {
    if (asValue == _T("taLeftJustify")) Set_Alignment(taLeftJustify);
    else if (asValue == _T("taRightJustify")) Set_Alignment(taRightJustify);
    else if (asValue == _T("taCenter")) Set_Alignment(taCenter);
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

