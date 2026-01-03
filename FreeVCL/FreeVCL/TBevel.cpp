//-----------------------------------------------------------------------------
//! @file TBevel.cpp
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
// TBevel
//---------------------------------------------------------------------------

TBevel::TBevel(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TBevel");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FExWindowStyle |= WS_EX_STATICEDGE;
	FShape = bsBox;
	FStyle = bsLowered;

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
TBevel::~TBevel(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Shape
//---------------------------------------------------------------------------

TBevelShape TBevel::Get_Shape(void) {
  return FShape;
}

bool TBevel::Set_Shape(TBevelShape NewShape) {
  if (FShape != NewShape) {
    FShape = NewShape;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TBevelStyle TBevel::Get_Style(void) {
  return FStyle;
}

bool TBevel::Set_Style(TBevelStyle NewStyle) {
  if (FStyle != NewStyle) {
    FStyle = NewStyle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TBevel::GetListProperties(TStrings *ListProperties) {

	TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Shape"));
  ListProperties->Add(_T("Style"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TBevel::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Shape")) {
    *asInfos = _T("bsBox\nbsFrame\nbsTopLine\nbsBottomLine\nbsLeftLine\nbsRightLine\nbsSpacer");
    return tpChoice;
  }
  if (asProperty == _T("Style")) {
    *asInfos = _T("bsLowered\nbsRaised");
    return tpChoice;
  }

	return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TBevel::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Shape")) {
    return _T("bsBox");
  }
  if (asProperty == _T("Style")) {
    return _T("bsLowered");
  }

	return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TBevel::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Shape")) {
    TBevelShape Shape = Get_Shape();
    if (Shape == bsBox) return _T("bsBox");
    if (Shape == bsFrame) return _T("bsFrame");
    if (Shape == bsTopLine) return _T("bsTopLine");
    if (Shape == bsBottomLine) return _T("bsBottomLine");
    if (Shape == bsLeftLine) return _T("bsLeftLine");
    if (Shape == bsRightLine) return _T("bsRightLine");
    if (Shape == bsSpacer) return _T("bsSpacer");
    return _T("");
  }
  if (asProperty == _T("Style")) {
    TBevelStyle Style = Get_Style();
    if (Style == bsLowered) return _T("bsLowered");
    if (Style == bsRaised) return _T("bsRaised");
    return _T("");
  }

  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TBevel::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Shape")) {
    if      (asValue == _T("bsBox")) Set_Shape(bsBox);
    else if (asValue == _T("bsFrame")) Set_Shape(bsFrame);
    else if (asValue == _T("bsTopLine")) Set_Shape(bsTopLine);
    else if (asValue == _T("bsBottomLine")) Set_Shape(bsBottomLine);
    else if (asValue == _T("bsLeftLine")) Set_Shape(bsLeftLine);
    else if (asValue == _T("bsRightLine")) Set_Shape(bsRightLine);
    else if (asValue == _T("bsSpacer")) Set_Shape(bsSpacer);
    return true;
  }
  if (asProperty == _T("Style")) {
    if      (asValue == _T("bsLowered")) Set_Style(bsLowered);
    else if (asValue == _T("bsRaised")) Set_Style(bsRaised);
    return true;
  }

  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

