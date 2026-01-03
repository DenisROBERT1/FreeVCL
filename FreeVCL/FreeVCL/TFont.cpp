//-----------------------------------------------------------------------------
//! @file TFont.cpp
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
// TFont
//---------------------------------------------------------------------------

TFont::TFont(void): TCustomFont() {
  // Initialisations
  FHandle = NULL;
	FColor = clBlack;
  bChanged = false;
  memset(&LogFont, 0, sizeof(LOGFONT));
  GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &LogFont);
  LogFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
  FPixelsPerInch = 96;  // Apparemment en dur dans builder
  FCharacterExtra = 0;
}

TFont::~TFont(void) {
  if (FHandle) DeleteObject(FHandle);
}

//---------------------------------------------------------------------------
HFONT TFont::Get_Handle(void) {
  if (bChanged) {
    if (FHandle) {
      DeleteObject(FHandle);
      FHandle = NULL;
    }
    bChanged = false;
  }

  if (FHandle == NULL) FHandle = CreateFontIndirect(&LogFont);

  return FHandle;
}

//---------------------------------------------------------------------------
bool TFont::Set_Handle(HFONT NewHandle) {
  if (FHandle != NewHandle) {
    if (FHandle) DeleteObject(FHandle);
    FHandle = NewHandle;
    memset(&LogFont, 0, sizeof(LOGFONT));
    GetObject(NewHandle, sizeof(LOGFONT), &LogFont);
    bChanged = false;
  }
  return true;
}

//---------------------------------------------------------------------------
int TFont::Get_Width(void) {
  return LogFont.lfWidth;
}

//---------------------------------------------------------------------------
bool TFont::Set_Width(int NewWidth) {
  if (LogFont.lfWidth != NewWidth) {
    LogFont.lfWidth = NewWidth;
    bChanged = true;
  }
  return true;
}

//---------------------------------------------------------------------------
int TFont::Get_Height(void) {
  return LogFont.lfHeight;
}

//---------------------------------------------------------------------------
bool TFont::Set_Height(int NewHeight) {
  if (LogFont.lfHeight != NewHeight) {
    LogFont.lfHeight = NewHeight;
    LogFont.lfWidth = 0;
    bChanged = true;
  }
  return true;
}

//---------------------------------------------------------------------------
int TFont::Get_Size(void) {
  return (int) ((double) -LogFont.lfHeight * 72 / FPixelsPerInch + .5);
}

//---------------------------------------------------------------------------
bool TFont::Set_Size(int NewSize) {
  int NewHeight;


  NewHeight = -(int) ((double) NewSize * FPixelsPerInch / 72 + .5);
  if (LogFont.lfHeight != NewHeight) {
    LogFont.lfHeight = NewHeight;
    LogFont.lfWidth = 0;
    bChanged = true;
  }

  return true;
}

//---------------------------------------------------------------------------
AnsiString TFont::Get_Name(void) {
  return AnsiString(LogFont.lfFaceName);
}

//---------------------------------------------------------------------------
bool TFont::Set_Name(AnsiString NewName) {
  if (NewName != LogFont.lfFaceName) {
    lstrcpyn(LogFont.lfFaceName, NewName, LF_FACESIZE);
    bChanged = true;
  }
  return true;
}

//---------------------------------------------------------------------------
TFontStyles TFont::Get_Style(void) {
  TFontStyles Style;
  Style.Clear();
  if (LogFont.lfWeight == FW_BOLD) Style << fsBold;
  if (LogFont.lfItalic == TRUE) Style << fsItalic;
  if (LogFont.lfUnderline == TRUE) Style << fsUnderline;
  if (LogFont.lfStrikeOut == TRUE) Style << fsStrikeOut;
  return Style;
}

//---------------------------------------------------------------------------
bool TFont::Set_Style(TFontStyles NewStyle) {
  if (NewStyle.Contains(fsBold)) LogFont.lfWeight = FW_BOLD;
  else LogFont.lfWeight = FW_NORMAL;
  if (NewStyle.Contains(fsItalic)) LogFont.lfItalic = TRUE;
  else LogFont.lfItalic = FALSE;
  if (NewStyle.Contains(fsUnderline)) LogFont.lfUnderline = TRUE;
  else LogFont.lfUnderline = FALSE;
  if (NewStyle.Contains(fsStrikeOut)) LogFont.lfStrikeOut = TRUE;
  else LogFont.lfStrikeOut = FALSE;
  bChanged = true;

  return true;
}

//---------------------------------------------------------------------------
TFontCharset TFont::Get_Charset(void) {
  return (TFontCharset) LogFont.lfCharSet;
}

//---------------------------------------------------------------------------
bool TFont::Set_Charset(TFontCharset NewCharset) {
  LogFont.lfCharSet = NewCharset;
  bChanged = true;

  return true;
}

//---------------------------------------------------------------------------
TFontPitch TFont::Get_Pitch(void) {
  if (LogFont.lfPitchAndFamily & FIXED_PITCH) return fpFixed;
  if (LogFont.lfPitchAndFamily & VARIABLE_PITCH) return fpVariable;

  return fpDefault;
}

//---------------------------------------------------------------------------
bool TFont::Set_Pitch(TFontPitch NewPitch) {
  LogFont.lfPitchAndFamily &= ~(DEFAULT_PITCH | FIXED_PITCH | VARIABLE_PITCH);
  if (NewPitch == fpDefault) LogFont.lfPitchAndFamily |= DEFAULT_PITCH;
  else if (NewPitch == fpFixed) LogFont.lfPitchAndFamily |= FIXED_PITCH;
  else if (NewPitch == fpVariable) LogFont.lfPitchAndFamily |= VARIABLE_PITCH;
  bChanged = true;

  return true;
}

//---------------------------------------------------------------------------
TColor TFont::Get_Color(void) {
  return FColor;
}

//---------------------------------------------------------------------------
bool TFont::Set_Color(TColor NewColor) {
  if (NewColor != FColor) {
    FColor = NewColor;
    bChanged = true;
  }
  return true;
}

//---------------------------------------------------------------------------
int TFont::Get_PixelsPerInch(void) {
  return FPixelsPerInch;
}

//---------------------------------------------------------------------------
bool TFont::Set_PixelsPerInch(int NewPixelsPerInch) {
  if (FPixelsPerInch != NewPixelsPerInch) {
    FPixelsPerInch = NewPixelsPerInch;
  }
  return true;
}

//---------------------------------------------------------------------------
int TFont::Get_CharacterExtra(void) {
  return FCharacterExtra;
}

//---------------------------------------------------------------------------
bool TFont::Set_CharacterExtra(int NewCharacterExtra) {
  if (FCharacterExtra != NewCharacterExtra) {
    FCharacterExtra = NewCharacterExtra;
  }
  return true;
}

//---------------------------------------------------------------------------

