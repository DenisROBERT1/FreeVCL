//-----------------------------------------------------------------------------
//! @file TToolButton.cpp
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
// TToolButton
//---------------------------------------------------------------------------

TToolButton::TToolButton(TComponent* AOwner): TControl(AOwner) {
  // Initialisations
  FClassName = _T("TToolButton");
  FImageIndex = -1;
  FStyle = tbsButton;
}

TToolButton::~TToolButton(void) {
}

//---------------------------------------------------------------------------
bool TToolButton::ProcessCommand(WORD Notify) {
	if (!FComponentState.Contains(csLoading)) {
		if (FAction) FAction->Execute();
		OnClick(this);
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ImageIndex
//---------------------------------------------------------------------------

int TToolButton::Get_ImageIndex(void) {
  return FImageIndex;
}

bool TToolButton::Set_ImageIndex(int NewImageIndex) {
  if (FImageIndex != NewImageIndex) {
    FImageIndex = NewImageIndex;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TToolButtonStyle TToolButton::Get_Style(void) {
  return FStyle;
}

bool TToolButton::Set_Style(TToolButtonStyle NewStyle) {
  if (FStyle != NewStyle) {
    FStyle = NewStyle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TToolButton::GetListProperties(TStrings *ListProperties) {

  TControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("ImageIndex"));
  ListProperties->Add(_T("Style"));
  ListProperties->Add(_T("OnPaint"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TToolButton::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("ImageIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("Style")) {
    *asInfos = _T("tbsButton\ntbsCheck\ntbsDropDown\ntbsSeparator\ntbsDivider");
    return tpChoice;
  }
  if (asProperty == _T("OnPaint")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TToolButton::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("ImageIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("Style")) {
    return _T("tbsButton");
  }
  if (asProperty == _T("OnPaint")) {
    return _T("");
  }
  return TControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TToolButton::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("ImageIndex")) {
    return IntToStr(Get_ImageIndex());
  }
  if (asProperty == _T("Style")) {
    TToolButtonStyle Style = Get_Style();
    if (Style == tbsButton) return _T("tbsButton");
    if (Style == tbsCheck) return _T("tbsCheck");
    if (Style == tbsDropDown) return _T("tbsDropDown");
    if (Style == tbsSeparator) return _T("tbsSeparator");
    if (Style == tbsDivider) return _T("tbsDivider");
    return _T("");
  }
  if (asProperty == _T("OnPaint")) {
    return OnPaint.AsString();
  }
  return TControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TToolButton::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("ImageIndex")) {
    FImageIndex = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("Style")) {
    if (asValue == _T("tbsButton")) FStyle = tbsButton;
    else if (asValue == _T("tbsCheck")) FStyle = tbsCheck;
    else if (asValue == _T("tbsDropDown")) FStyle = tbsDropDown;
    else if (asValue == _T("tbsSeparator")) FStyle = tbsSeparator;
    else if (asValue == _T("tbsDivider")) FStyle = tbsDivider;
    return true;
  }
  if (asProperty == _T("OnPaint")) {
		OnPaint.ObjCall = Sender;
    OnPaint = asValue.c_str();
    return true;
  }
  return TControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

