//-----------------------------------------------------------------------------
//! @file TLabel.cpp
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
// TLabel
//---------------------------------------------------------------------------

TLabel::TLabel(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TLabel");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= SS_NOTIFY | SS_LEFTNOWORDWRAP;
  FWordWrap = false;

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
TLabel::~TLabel(void) {
}

//---------------------------------------------------------------------------
bool FASTCALL TLabel::CanAutoSize(int &NewWidth, int &NewHeight) {
	SIZE SizeLabel;
	BOOL Ok;

	HDC hdc = GetDC(FHandle);
	HFONT hOldFont = (HFONT) SelectObject(hdc, FFont->Handle);
	Ok = GetTextExtentPoint32(hdc, FCaption, FCaption.Length(), &SizeLabel);
	SelectObject(hdc, hOldFont);
	ReleaseDC(FHandle, hdc);

	if (!Ok ||
			(NewWidth == SizeLabel.cx &&
			 NewHeight == SizeLabel.cy)) return false;

	NewWidth = SizeLabel.cx;
	NewHeight = SizeLabel.cy;

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

bool TLabel::Set_Caption(AnsiString NewCaption) {
	TWinControl::Set_Caption(NewCaption);
	if (FAutoSize) {
		int NewLeft = FLeft;
		int NewTop = FTop;
		int NewWidth = FWidth;
		int NewHeight = FHeight;
		if (InternalCanAutoSize(NewLeft, NewTop, NewWidth, NewHeight)) {
			Size(NewLeft, NewTop, NewWidth, NewHeight, false);
		}
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

bool TLabel::Set_Font(TFont *NewFont) {
	TWinControl::Set_Font(NewFont);
	if (FAutoSize) {
		int NewLeft = FLeft;
		int NewTop = FTop;
		int NewWidth = FWidth;
		int NewHeight = FHeight;
		if (InternalCanAutoSize(NewLeft, NewTop, NewWidth, NewHeight)) {
			Size(NewLeft, NewTop, NewWidth, NewHeight, false);
		}
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TLabel::Get_Alignment(void) {
  switch (FWindowStyle & (SS_LEFT | SS_CENTER | SS_RIGHT)) {
    case SS_LEFT: return taLeftJustify;
    case SS_CENTER: return taCenter;
    case SS_RIGHT: return taRightJustify;
  }

  return taLeftJustify;
}

bool TLabel::Set_Alignment(TAlignment NewAlignment) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  NewWindowStyle &= (DWORD) ~(SS_LEFT | SS_CENTER | SS_RIGHT | SS_LEFTNOWORDWRAP);
  if (NewAlignment == taLeftJustify) {
    if (FWordWrap) NewWindowStyle |= (DWORD) SS_LEFTNOWORDWRAP;
    else NewWindowStyle |= (DWORD) SS_LEFT;
  }
  else if (NewAlignment == taCenter) NewWindowStyle |= (DWORD) SS_CENTER;
  else if (NewAlignment == taRightJustify) NewWindowStyle |= (DWORD) SS_RIGHT;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WordWrap
//---------------------------------------------------------------------------

bool TLabel::Get_WordWrap(void) {
  return FWordWrap;
}

bool TLabel::Set_WordWrap(bool NewWordWrap) {
  DWORD NewWindowStyle;


  if (FWordWrap != NewWordWrap) {
    FWordWrap = NewWordWrap;

    NewWindowStyle = FWindowStyle;

    if (NewWordWrap) NewWindowStyle &= (DWORD) ~(SS_LEFTNOWORDWRAP);
    else NewWindowStyle |= (DWORD) SS_LEFTNOWORDWRAP;
    ChangeWindowStyle(NewWindowStyle);
  }

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TLabel::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Alignment"));
  ListProperties->Add(_T("WordWrap"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TLabel::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Alignment")) {
    *asInfos = _T("taLeftJustify\ntaRightJustify\ntaCenter");
    return tpChoice;
  }
  if (asProperty == _T("WordWrap")) {
    return tpBool;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TLabel::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    return _T("taLeftJustify");
  }
  if (asProperty == _T("WordWrap")) {
    return _T("False");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TLabel::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    TAlignment Alignment = Get_Alignment();
    if (Alignment == taLeftJustify) return _T("taLeftJustify");
    if (Alignment == taRightJustify) return _T("taRightJustify");
    if (Alignment == taCenter) return _T("taCenter");
    return _T("taLeftJustify");
  }
  if (asProperty == _T("WordWrap")) {
    if (Get_WordWrap()) return _T("True");
    else return _T("False");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TLabel::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Alignment")) {
    if (asValue == _T("taLeftJustify")) Set_Alignment(taLeftJustify);
    else if (asValue == _T("taRightJustify")) Set_Alignment(taRightJustify);
    else if (asValue == _T("taCenter")) Set_Alignment(taCenter);
    return true;
  }
  if (asProperty == _T("WordWrap")) {
    Set_WordWrap(asValue == _T("True"));
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

