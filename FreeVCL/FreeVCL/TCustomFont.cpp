//-----------------------------------------------------------------------------
//! @file TCustomFont.cpp
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
// TCustomFont
//---------------------------------------------------------------------------

TCustomFont::TCustomFont(void): TPersistent() {
}

TCustomFont::~TCustomFont(void) {
}

//---------------------------------------------------------------------------
int TCustomFont::Get_Width(void) {
  return 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Width(int NewWidth) {
  return false;
}

//---------------------------------------------------------------------------
int TCustomFont::Get_Height(void) {
  return 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Height(int NewHeight) {
  return false;
}

//---------------------------------------------------------------------------
int TCustomFont::Get_Size(void) {
  return 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Size(int NewSize) {
  return false;
}

//---------------------------------------------------------------------------
AnsiString TCustomFont::Get_Name(void) {
  return L"";
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Name(AnsiString NewName) {
  return false;
}

//---------------------------------------------------------------------------
TFontStyles TCustomFont::Get_Style(void) {
  TFontStyles FontStyles;
  FontStyles.Clear();

  return FontStyles;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Style(TFontStyles NewStyle) {
  return false;
}

//---------------------------------------------------------------------------
TFontCharset TCustomFont::Get_Charset(void) {
  return (TFontCharset) 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Charset(TFontCharset NewCharset) {
  return false;
}

//---------------------------------------------------------------------------
TFontPitch TCustomFont::Get_Pitch(void) {
  return (TFontPitch) 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Pitch(TFontPitch NewPitch) {
  return false;
}

//---------------------------------------------------------------------------
TColor TCustomFont::Get_Color(void) {
  return 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_Color(TColor NewColor) {
  return false;
}

//---------------------------------------------------------------------------
int TCustomFont::Get_PixelsPerInch(void) {
  return 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_PixelsPerInch(int NewPixelsPerInch) {
  return false;
}

//---------------------------------------------------------------------------
int TCustomFont::Get_CharacterExtra(void) {
  return 0;
}

//---------------------------------------------------------------------------
bool TCustomFont::Set_CharacterExtra(int NewCharacterExtra) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TCustomFont::Assign(TPersistent* Source) {

  TCustomFont *CustomFontSource = dynamic_cast<TCustomFont *> (Source);
  if (CustomFontSource) {
    Set_Width(CustomFontSource->Width);
    Set_Height(CustomFontSource->Height);
    Set_Name(CustomFontSource->Name);
    Set_Style(CustomFontSource->Style);
    Set_Pitch(CustomFontSource->Pitch);
    Set_Color(CustomFontSource->Color);
    Set_PixelsPerInch(CustomFontSource->PixelsPerInch);
    Set_CharacterExtra(CustomFontSource->CharacterExtra);
    return;
  }

  // Source->AssignTo(this);

  return;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomFont::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Height"));
  ListProperties->Add(_T("Name"));
  ListProperties->Add(_T("Style"));
  ListProperties->Add(_T("Charset"));
  ListProperties->Add(_T("Pitch"));
  ListProperties->Add(_T("Color"));
  ListProperties->Add(_T("PixelsPerInch"));
  ListProperties->Add(_T("CharacterExtra"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomFont::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Height")) {
    return tpNumber;
  }
  if (asProperty == _T("Name")) {
    return tpText;
  }
  if (asProperty == _T("Style")) {
    *asInfos = _T("fsBold\nfsItalic\nfsUnderline\nfsStrikeOut");
    return tpMultipleChoice;
  }
  if (asProperty == _T("Charset")) {
    *asInfos = _T("ANSI_CHARSET\nDEFAULT_CHARSET\nSYMBOL_CHARSET\nMAC_CHARSET\nSHIFTJIS_CHARSET\nHANGEUL_CHARSET\nJOHAB_CHARSET\nGB2312_CHARSET\nCHINESEBIG5_CHARSETT\nGREEK_CHARSET\nTURKISH_CHARSET\nVIETNAMESE_CHARSET\nHEBREW_CHARSET\nARABIC_CHARSET\nBALTIC_CHARSET\nRUSSIAN_CHARSET\nTHAI_CHARSET\nEASTEUROPE_CHARSET\nOEM_CHARSET");
    return tpChoice;
  }
  if (asProperty == _T("Pitch")) {
    *asInfos = _T("fpDefault\nfpVariable\nfpFixed");
    return tpChoice;
  }
  if (asProperty == _T("Color")) {
    return tpColor;
  }
  if (asProperty == _T("PixelsPerInch")) {
    return tpNumber;
  }
  if (asProperty == _T("CharacterExtra")) {
    return tpNumber;
  }
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomFont::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Height")) {
    return _T("0");
  }
  if (asProperty == _T("Name")) {
    return _T("");
  }
  if (asProperty == _T("Style")) {
    return _T("[]");
  }
  if (asProperty == _T("Charset")) {
    return _T("DEFAULT_CHARSET");
  }
  if (asProperty == _T("Pitch")) {
    return _T("fpDefault");
  }
  if (asProperty == _T("Color")) {
    return _T("clBlack");
  }
  if (asProperty == _T("PixelsPerInch")) {
    return _T("0");
  }
  if (asProperty == _T("CharacterExtra")) {
    return _T("0");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomFont::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Height")) {
    return IntToStr(Get_Height());
  }
  if (asProperty == _T("Name")) {
    return Get_Name();
  }
  if (asProperty == _T("Style")) {
    AnsiString asStyle;
    TFontStyles Style;
    Style = Get_Style();
    if (Style.Contains(fsBold)) asStyle += _T("fsBold\n");
    if (Style.Contains(fsItalic)) asStyle += _T("fsItalic\n");
    if (Style.Contains(fsUnderline)) asStyle += _T("fsUnderline\n");
    if (Style.Contains(fsStrikeOut)) asStyle += _T("fsStrikeOut\n");
    return FormatProperty(asStyle, _T("["), _T("]"), _T(", "));
  }
  if (asProperty == _T("Charset")) {
    TFontCharset Charset = Get_Charset();
    if (Charset == ANSI_CHARSET) return _T("ANSI_CHARSET");
    if (Charset == DEFAULT_CHARSET) return _T("DEFAULT_CHARSET");
    if (Charset == SYMBOL_CHARSET) return _T("SYMBOL_CHARSET");
    if (Charset == MAC_CHARSET) return _T("MAC_CHARSET");
    if (Charset == SHIFTJIS_CHARSET) return _T("SHIFTJIS_CHARSET");
    if (Charset == HANGEUL_CHARSET) return _T("HANGEUL_CHARSET");
    if (Charset == JOHAB_CHARSET) return _T("JOHAB_CHARSET");
    if (Charset == GB2312_CHARSET) return _T("GB2312_CHARSET");
    if (Charset == CHINESEBIG5_CHARSET) return _T("CHINESEBIG5_CHARSET");
    if (Charset == GREEK_CHARSET) return _T("GREEK_CHARSET");
    if (Charset == TURKISH_CHARSET) return _T("TURKISH_CHARSET");
    if (Charset == VIETNAMESE_CHARSET) return _T("VIETNAMESE_CHARSET");
    if (Charset == HEBREW_CHARSET) return _T("HEBREW_CHARSET");
    if (Charset == ARABIC_CHARSET) return _T("ARABIC_CHARSET");
    if (Charset == BALTIC_CHARSET) return _T("BALTIC_CHARSET");
    if (Charset == RUSSIAN_CHARSET) return _T("RUSSIAN_CHARSET");
    if (Charset == THAI_CHARSET) return _T("THAI_CHARSET");
    if (Charset == EASTEUROPE_CHARSET) return _T("EASTEUROPE_CHARSET");
    if (Charset == OEM_CHARSET) return _T("OEM_CHARSET");
    return _T("");
  }
  if (asProperty == _T("Pitch")) {
    TFontPitch Pitch;
    Pitch = Get_Pitch();
    if (Pitch == fpFixed) return _T("fpFixed\n");
    if (Pitch == fpVariable) return _T("fpVariable\n");
    return _T("fpDefault");
  }
  if (asProperty == _T("Color")) {
    return ColorToString(Get_Color());
  }
  if (asProperty == _T("PixelsPerInch")) {
    return IntToStr(Get_PixelsPerInch());
  }
  if (asProperty == _T("CharacterExtra")) {
    return IntToStr(Get_CharacterExtra());
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomFont::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  int Cur;
  AnsiString asMot;


  if (asProperty == _T("Height")) {
    Set_Height(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Name")) {
    Set_Name(asValue);
    return true;
  }
  if (asProperty == _T("Style")) {

    TFontStyles Style;
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("fsBold")) Style << fsBold;
      else if (asMot == _T("fsItalic")) Style << fsItalic;
      else if (asMot == _T("fsUnderline")) Style << fsUnderline;
      else if (asMot == _T("fsStrikeOut")) Style << fsStrikeOut;
    } while (!asMot.IsEmpty());
    Set_Style(Style);
    return true;
  }
  if (asProperty == _T("Charset")) {
    if      (asValue == _T("ANSI_CHARSET")) Set_Charset(ANSI_CHARSET);
    else if (asValue == _T("DEFAULT_CHARSET")) Set_Charset(DEFAULT_CHARSET);
    else if (asValue == _T("SYMBOL_CHARSET")) Set_Charset(SYMBOL_CHARSET);
    else if (asValue == _T("MAC_CHARSET")) Set_Charset(MAC_CHARSET);
    else if (asValue == _T("SHIFTJIS_CHARSET")) Set_Charset(SHIFTJIS_CHARSET);
    else if (asValue == _T("HANGEUL_CHARSET")) Set_Charset(HANGEUL_CHARSET);
    else if (asValue == _T("JOHAB_CHARSET")) Set_Charset(JOHAB_CHARSET);
    else if (asValue == _T("GB2312_CHARSET")) Set_Charset(GB2312_CHARSET);
    else if (asValue == _T("CHINESEBIG5_CHARSET")) Set_Charset(CHINESEBIG5_CHARSET);
    else if (asValue == _T("GREEK_CHARSET")) Set_Charset(GREEK_CHARSET);
    else if (asValue == _T("TURKISH_CHARSET")) Set_Charset(TURKISH_CHARSET);
    else if (asValue == _T("VIETNAMESE_CHARSET")) Set_Charset(VIETNAMESE_CHARSET);
    else if (asValue == _T("HEBREW_CHARSET")) Set_Charset(HEBREW_CHARSET);
    else if (asValue == _T("ARABIC_CHARSET")) Set_Charset(ARABIC_CHARSET);
    else if (asValue == _T("BALTIC_CHARSET")) Set_Charset(BALTIC_CHARSET);
    else if (asValue == _T("RUSSIAN_CHARSET")) Set_Charset(RUSSIAN_CHARSET);
    else if (asValue == _T("THAI_CHARSET")) Set_Charset(THAI_CHARSET);
    else if (asValue == _T("EASTEUROPE_CHARSET")) Set_Charset(EASTEUROPE_CHARSET);
    else if (asValue == _T("OEM_CHARSET")) Set_Charset(OEM_CHARSET);
    return true;
  }
  if (asProperty == _T("Pitch")) {
    if (asMot == _T("fpDefault")) Set_Pitch(fpDefault);
    else if (asMot == _T("fpFixed")) Set_Pitch(fpFixed);
    else if (asMot == _T("fpVariable")) Set_Pitch(fpVariable);
    return true;
  }
  if (asProperty == _T("Color")) {
    TColor Color;
    Color = asValue;
    Set_Color(Color);
    return true;
  }
  if (asProperty == _T("PixelsPerInch")) {
    Set_PixelsPerInch(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("CharacterExtra")) {
    Set_CharacterExtra(StrToInt(asValue));
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

