//-----------------------------------------------------------------------------
//! @file TColorDialog.cpp
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
// TColorDialog
//---------------------------------------------------------------------------

TColorDialog::TColorDialog(TComponent* AOwner): TComponent(AOwner) {

  // Initialisations
  if (AOwner) Handle = ((TControl *) AOwner)->Handle;
  else Handle = NULL;
  Flags = CC_RGBINIT;
  CustColor[0] = clBtnFace;
  CustColor[1] = clWindow;
  CustColor[2] = clHighlight;
  CustColor[3] = clActiveCaption;
  CustColor[4] = clInactiveCaption;
  CustColor[5] = clInfoBk;
  CustColor[6] = clGrayText;
  CustColor[7] = clBtnText;
  CustColor[8] = clActiveBorder;
  CustColor[9] = clInactiveBorder;
  CustColor[10] = clScrollBar;
  CustColor[11] = clWindowFrame;
  CustColor[12] = clInfoText;
  CustColor[13] = clMenuText;
  CustColor[14] = clWindowText;
  CustColor[15] = clCaptionText;
}

TColorDialog::~TColorDialog(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TColorDialog::Get_Color(void) {
  return FColor;
}

bool TColorDialog::Set_Color(TColor NewColor) {
  if (FColor != NewColor) {
    FColor = NewColor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Options
//---------------------------------------------------------------------------

TColorDialogOptions TColorDialog::Get_Options(void) {
  TColorDialogOptions Options;
  Options.Clear();
  if (Flags & CC_FULLOPEN) Options << cdFullOpen;
  if (Flags & CC_PREVENTFULLOPEN) Options << cdPreventFullOpen;
  if (Flags & CC_SHOWHELP) Options << cdShowHelp;
  if (Flags & CC_SOLIDCOLOR) Options << cdSolidColor;
  if (Flags & CC_ANYCOLOR) Options << cdAnyColor;

  return Options;
}

bool TColorDialog::Set_Options(TColorDialogOptions NewOptions) {
  Flags = CC_RGBINIT;
  if (NewOptions.Contains(cdFullOpen)) Flags |= CC_FULLOPEN;
  if (NewOptions.Contains(cdPreventFullOpen)) Flags |= CC_PREVENTFULLOPEN;
  if (NewOptions.Contains(cdShowHelp)) Flags |= CC_SHOWHELP;
  if (NewOptions.Contains(cdSolidColor)) Flags |= CC_SOLIDCOLOR;
  if (NewOptions.Contains(cdAnyColor)) Flags |= CC_ANYCOLOR;

  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TColorDialog::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Title"));
  ListProperties->Add(_T("Color"));
  ListProperties->Add(_T("Filter"));
  ListProperties->Add(_T("DefaultExt"));
  ListProperties->Add(_T("Options"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TColorDialog::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Color")) {
    return tpColor;
  }
  if (asProperty == _T("Options")) {
    *asInfos = _T("cdFullOpen\ncdPreventFullOpen\ncdShowHelp\ncdSolidColor\ncdAnyColor");
    return tpMultipleChoice;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TColorDialog::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Color")) {
    return _T("clBlack");
  }
  if (asProperty == _T("Options")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TColorDialog::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Color")) {
    return ColorToString(Get_Color());
  }
  if (asProperty == _T("Options")) {
    AnsiString asOptions;
    TColorDialogOptions Options = Get_Options();
    if (Flags & CC_FULLOPEN) asOptions += _T("cdFullOpen\n");
    if (Flags & CC_PREVENTFULLOPEN) asOptions += _T("cdPreventFullOpen\n");
    if (Flags & CC_SHOWHELP) asOptions += _T("cdShowHelp\n");
    if (Flags & CC_SOLIDCOLOR) asOptions += _T("cdSolidColor\n");
    if (Flags & CC_ANYCOLOR) asOptions += _T("cdAnyColor\n");
    return FormatProperty(asOptions, _T("["), _T("]"), _T(", "));
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TColorDialog::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  int Cur;
  AnsiString asMot;


  if (asProperty == _T("Color")) {
    FColor = asValue;
    return true;
  }
  if (asProperty == _T("Options")) {
    Flags = OFN_EXPLORER;
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("cdFullOpen")) Flags |= CC_FULLOPEN;
      else if (asMot == _T("cdPreventFullOpen")) Flags |= CC_PREVENTFULLOPEN;
      else if (asMot == _T("cdShowHelp")) Flags |= CC_SHOWHELP;
      else if (asMot == _T("cdSolidColor")) Flags |= CC_SOLIDCOLOR;
      else if (asMot == _T("cdAnyColor")) Flags |= CC_ANYCOLOR;
    } while (!asMot.IsEmpty());

    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
bool TColorDialog::Execute(void) {
  CHOOSECOLOR ccd;


  memset(&ccd, 0, sizeof(CHOOSECOLOR));
  ccd.lStructSize = sizeof(CHOOSECOLOR);
  ccd.hwndOwner = Handle;
  ccd.rgbResult = FColor.cr;
  ccd.lpCustColors = CustColor;
  ccd.Flags = Flags;
  if (ChooseColor(&ccd)) {
    FColor.cr = ccd.rgbResult;
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------

