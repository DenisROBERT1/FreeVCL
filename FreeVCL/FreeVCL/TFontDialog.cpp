//-----------------------------------------------------------------------------
//! @file TFontDialog.cpp
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
// TFontDialog
//---------------------------------------------------------------------------

TFontDialog::TFontDialog(TComponent* AOwner): TComponent(AOwner) {

  // Initialisations
  if (AOwner) Handle = ((TControl *) AOwner)->Handle;
  else Handle = NULL;
  FFont = new TFont();
  Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
}

TFontDialog::~TFontDialog(void) {
  delete FFont;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

TCustomFont *TFontDialog::Get_Font(void) {
  return FFont;
}

bool TFontDialog::Set_Font(TCustomFont *NewFont) {
  FFont->Assign(NewFont);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Device
//---------------------------------------------------------------------------

TFontDialogDevice TFontDialog::Get_Device(void) {

  // L'ordre a une importance (car CF_BOTH = CF_SCREENFONTS | CF_PRINTERFONTS)
  if (Flags & CF_BOTH) return(fdBoth);
  if (Flags & CF_PRINTERFONTS) return(fdPrinter);

  return fdScreen;
}

bool TFontDialog::Set_Device(TFontDialogDevice NewDevice) {
  Flags &= ~(CF_SCREENFONTS | CF_PRINTERFONTS | CF_BOTH);
  if (NewDevice == fdScreen) Flags |= CF_SCREENFONTS;
  else if (NewDevice == fdPrinter) Flags |= CF_PRINTERFONTS;
  else if (NewDevice == fdBoth) Flags |= CF_BOTH;

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Options
//---------------------------------------------------------------------------

TFontDialogOptions TFontDialog::Get_Options(void) {
  TFontDialogOptions Options;
  Options.Clear();

  if (Flags & CF_APPLY) Options << fdApplyButton;
  if (Flags & CF_ANSIONLY) Options << fdAnsiOnly;
  if (Flags & CF_TTONLY) Options << fdTrueTypeOnly;
  if (Flags & CF_EFFECTS) Options << fdEffects;
  if (Flags & CF_FIXEDPITCHONLY) Options << fdFixedPitchOnly;
  if (Flags & CF_FORCEFONTEXIST) Options << fdForceFontExist;
  if (Flags & CF_LIMITSIZE) Options << fdLimitSize;
  if (Flags & CF_NOOEMFONTS) Options << fdNoOEMFonts;
  if (Flags & CF_NOFACESEL) Options << fdNoFaceSel;
  if (Flags & CF_NOSTYLESEL) Options << fdNoStyleSel;
  if (Flags & CF_NOSIZESEL) Options << fdNoSizeSel;
  if (Flags & CF_NOSIMULATIONS) Options << fdNoSimulations;
  if (Flags & CF_NOVECTORFONTS) Options << fdNoVectorFonts;
  if (Flags & CF_SCALABLEONLY) Options << fdScalableOnly;
  if (Flags & CF_SHOWHELP) Options << fdShowHelp;
  if (Flags & CF_WYSIWYG) Options << fdWysiwyg;

  return Options;
}

bool TFontDialog::Set_Options(TFontDialogOptions NewOptions) {
  Flags &= (CF_SCREENFONTS | CF_PRINTERFONTS | CF_BOTH | CF_INITTOLOGFONTSTRUCT);
  if (NewOptions.Contains(fdApplyButton)) Flags |= CF_APPLY;
  if (NewOptions.Contains(fdAnsiOnly)) Flags |= CF_ANSIONLY;
  if (NewOptions.Contains(fdTrueTypeOnly)) Flags |= CF_TTONLY;
  if (NewOptions.Contains(fdEffects)) Flags |= CF_EFFECTS;
  if (NewOptions.Contains(fdFixedPitchOnly)) Flags |= CF_FIXEDPITCHONLY;
  if (NewOptions.Contains(fdForceFontExist)) Flags |= CF_FORCEFONTEXIST;
  if (NewOptions.Contains(fdLimitSize)) Flags |= CF_LIMITSIZE;
  if (NewOptions.Contains(fdNoOEMFonts)) Flags |= CF_NOOEMFONTS;
  if (NewOptions.Contains(fdNoFaceSel)) Flags |= CF_NOFACESEL;
  if (NewOptions.Contains(fdNoStyleSel)) Flags |= CF_NOSTYLESEL;
  if (NewOptions.Contains(fdNoSizeSel)) Flags |= CF_NOSIZESEL;
  if (NewOptions.Contains(fdNoSimulations)) Flags |= CF_NOSIMULATIONS;
  if (NewOptions.Contains(fdNoVectorFonts)) Flags |= CF_NOVECTORFONTS;
  if (NewOptions.Contains(fdScalableOnly)) Flags |= CF_SCALABLEONLY;
  if (NewOptions.Contains(fdShowHelp)) Flags |= CF_SHOWHELP;
  if (NewOptions.Contains(fdWysiwyg)) Flags |= CF_WYSIWYG;

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MinFontSize
//---------------------------------------------------------------------------

int TFontDialog::Get_MinFontSize(void) {
  return FMinFontSize;
}

bool TFontDialog::Set_MinFontSize(int NewMinFontSize) {
  if (FMinFontSize != NewMinFontSize) {
    FMinFontSize = NewMinFontSize;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MaxFontSize
//---------------------------------------------------------------------------

int TFontDialog::Get_MaxFontSize(void) {
  return FMaxFontSize;
}

bool TFontDialog::Set_MaxFontSize(int NewMaxFontSize) {
  if (FMaxFontSize != NewMaxFontSize) {
    FMaxFontSize = NewMaxFontSize;
  }
  return true;
}

//---------------------------------------------------------------------------
bool TFontDialog::Execute(void) {
  CHOOSEFONT cf;
  LOGFONT lf;


  memset(&lf, 0, sizeof(LOGFONT));
  lf.lfHeight = FFont->Height;
  lstrcpyn(lf.lfFaceName, ((AnsiString) FFont->Name), LF_FACESIZE);
  TFontStyles Style = FFont->Style;
  if (Style.Contains(fsBold)) lf.lfWeight = FW_BOLD;
  else lf.lfWeight = FW_NORMAL;
  if (Style.Contains(fsItalic)) lf.lfItalic = TRUE;
  else lf.lfItalic = FALSE;
  if (Style.Contains(fsUnderline)) lf.lfUnderline = TRUE;
  else lf.lfUnderline = FALSE;
  if (Style.Contains(fsStrikeOut)) lf.lfStrikeOut = TRUE;
  else lf.lfStrikeOut = FALSE;

  memset(&cf, 0, sizeof(CHOOSEFONT));
  cf.lStructSize = sizeof(CHOOSEFONT);
  cf.hwndOwner = Handle;
  cf.lpLogFont = &lf;
  cf.Flags = Flags;
  cf.rgbColors = ((TColor) FFont->Color).cr;
  cf.hInstance = HInstance;
  cf.nSizeMin = FMinFontSize;
  cf.nSizeMax = FMaxFontSize;
  if (ChooseFont(&cf)) {
		FFont->Height = lf.lfHeight;
		FFont->Name = lf.lfFaceName;
    Style.Clear();
    if (lf.lfWeight == FW_BOLD) Style << fsBold;
    if (lf.lfItalic == TRUE) Style << fsItalic;
    if (lf.lfUnderline == TRUE) Style << fsUnderline;
    if (lf.lfStrikeOut == TRUE) Style << fsStrikeOut;
    FFont->Style = Style;
    FFont->Color = cf.rgbColors;
    return true;
  }
  // DWORD dw = CommDlgExtendedError();

  return false;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TFontDialog::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Title"));
  ListProperties->Add(_T("Font"));
  ListProperties->Add(_T("Filter"));
  ListProperties->Add(_T("DefaultExt"));
  ListProperties->Add(_T("Options"));
  ListProperties->Add(_T("MinFontSize"));
  ListProperties->Add(_T("MaxFontSize"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TFontDialog::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Font")) {
    return tpObject;
  }
  if (asProperty == _T("Device")) {
    *asInfos = _T("fdScreen\nfdPrinter\nfdBoth");
    return tpChoice;
  }
  if (asProperty == _T("Options")) {
    *asInfos = _T("fdAnsiOnly\nfdTrueTypeOnly\nfdEffects\nfdFixedPitchOnly\nfdForceFontExist\nfdNoFaceSel\nfdNoOEMFonts\nfdNoSimulations\nfdNoSizeSel\nfdNoStyleSel\nfdNoVectorFonts\nfdShowHelp\nfdWysiwyg\nfdLimitSize\nfdScalableOnly\nfdApplyButton");
    return tpMultipleChoice;
  }
  if (asProperty == _T("MinFontSize")) {
    return tpNumber;
  }
  if (asProperty == _T("MaxFontSize")) {
    return tpNumber;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TFontDialog::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Font")) {
    return _T("");
  }
  if (asProperty == _T("Device")) {
    return _T("[fdScreen]");
  }
  if (asProperty == _T("Options")) {
    return _T("[fdEffects]");
  }
  if (asProperty == _T("MinFontSize")) {
    return _T("0");
  }
  if (asProperty == _T("MaxFontSize")) {
    return _T("0");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TFontDialog::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Font")) {
    TCustomFont *Font = Get_Font();
    if (Font) return Font->Name;
    return _T("");
  }
  if (asProperty == _T("Device")) {
    if (Flags & CF_BOTH) return _T("fdBoth");
    if (Flags & CF_PRINTERFONTS) return _T("fdPrinter");
    if (Flags & CF_SCREENFONTS) return _T("fdScreen");
    return _T("");
  }
  if (asProperty == _T("Options")) {
    AnsiString asOptions;
    TFontDialogOptions Options = Get_Options();

    if (Flags & CF_APPLY) asOptions += _T("fdApplyButton\n");
    if (Flags & CF_ANSIONLY) asOptions += _T("fdAnsiOnly\n");
    if (Flags & CF_TTONLY) asOptions += _T("fdTrueTypeOnly\n");
    if (Flags & CF_EFFECTS) asOptions += _T("fdEffects\n");
    if (Flags & CF_FIXEDPITCHONLY) asOptions += _T("fdFixedPitchOnly\n");
    if (Flags & CF_FORCEFONTEXIST) asOptions += _T("fdForceFontExist\n");
    if (Flags & CF_LIMITSIZE) asOptions += _T("fdLimitSize\n");
    if (Flags & CF_NOOEMFONTS) asOptions += _T("fdNoOEMFonts\n");
    if (Flags & CF_NOFACESEL) asOptions += _T("fdNoStyleSel\n");
    if (Flags & CF_NOSTYLESEL) asOptions += _T("cdPreventFullOpen\n");
    if (Flags & CF_NOSIZESEL) asOptions += _T("fdNoSizeSel\n");
    if (Flags & CF_NOSIMULATIONS) asOptions += _T("fdNoSimulations\n");
    if (Flags & CF_NOVECTORFONTS) asOptions += _T("fdNoVectorFonts\n");
    if (Flags & CF_SCALABLEONLY) asOptions += _T("fdScalableOnly\n");
    if (Flags & CF_SHOWHELP) asOptions += _T("fdShowHelp\n");
    if (Flags & CF_WYSIWYG) asOptions += _T("fdWysiwyg\n");
    return FormatProperty(asOptions, _T("["), _T("]"), _T(", "));
  }
  if (asProperty == _T("MinFontSize")) {
    return IntToStr(Get_MinFontSize());
  }
  if (asProperty == _T("MaxFontSize")) {
    return IntToStr(Get_MaxFontSize());
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TFontDialog::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  int Cur;
  AnsiString asMot;


  if (asProperty == _T("Font")) {
    TCustomFont *Font;
		if (asValue.IsEmpty()) Font = NULL;
		else Font = dynamic_cast<TCustomFont *>(FindRootComponent()->FindComponent(asValue));
    Set_Font(Font);
    return true;
  }
  if (asProperty == _T("Device")) {
	  Flags &= ~(CF_SCREENFONTS | CF_PRINTERFONTS | CF_BOTH);
    if (asValue == _T("fdBoth")) Flags |= CF_BOTH;
    else if (asValue == _T("fdPrinter")) Flags |= CF_PRINTERFONTS;
    else if (asValue == _T("fdScreen")) Flags |= CF_SCREENFONTS;
    return true;
  }
  if (asProperty == _T("Options")) {
	  Flags &= (CF_SCREENFONTS | CF_PRINTERFONTS | CF_BOTH | CF_INITTOLOGFONTSTRUCT);
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);

      if (asMot == _T("fdApplyButton")) Flags |= CF_APPLY;
      else if (asMot == _T("fdAnsiOnly")) Flags |= CF_ANSIONLY;
      else if (asMot == _T("fdTrueTypeOnly")) Flags |= CF_TTONLY;
      else if (asMot == _T("fdEffects")) Flags |= CF_EFFECTS;
      else if (asMot == _T("fdFixedPitchOnly")) Flags |= CF_FIXEDPITCHONLY;
      else if (asMot == _T("fdForceFontExist")) Flags |= CF_FORCEFONTEXIST;
      else if (asMot == _T("fdLimitSize")) Flags |= CF_LIMITSIZE;
      else if (asMot == _T("fdNoOEMFonts")) Flags |= CF_NOOEMFONTS;
      else if (asMot == _T("fdNoFaceSel")) Flags |= CF_NOFACESEL;
      else if (asMot == _T("fdNoStyleSel")) Flags |= CF_NOSTYLESEL;
      else if (asMot == _T("fdNoSizeSel")) Flags |= CF_NOSIZESEL;
      else if (asMot == _T("fdNoSimulations")) Flags |= CF_NOSIMULATIONS;
      else if (asMot == _T("fdNoVectorFonts")) Flags |= CF_NOVECTORFONTS;
      else if (asMot == _T("fdScalableOnly")) Flags |= CF_SCALABLEONLY;
      else if (asMot == _T("fdShowHelp")) Flags |= CF_SHOWHELP;
      else if (asMot == _T("fdWysiwyg")) Flags |= CF_WYSIWYG;
    } while (!asMot.IsEmpty());

    return true;
  }
  if (asProperty == _T("MinFontSize")) {
    Set_MinFontSize(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("MaxFontSize")) {
    Set_MaxFontSize(StrToInt(asValue));
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
