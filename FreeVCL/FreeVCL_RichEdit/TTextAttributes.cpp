//-----------------------------------------------------------------------------
//! @file TTextAttributes.cpp
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
#include <TFactory_FVCL_RichEdit.h>
#include <TRichEdit.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL_RichEdit Factory_FVCL_RichEdit;


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TTextAttributes::TTextAttributes(HWND HandleParent, BOOL bSelection): TPersistent() {
  this->FHandleParent = HandleParent;
  this->bSelection = bSelection;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TTextAttributes::~TTextAttributes() {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HandleParent
//---------------------------------------------------------------------------

HWND TTextAttributes::Get_HandleParent() {
  return FHandleParent;
}

bool TTextAttributes::Set_HandleParent(HWND NewHandleParent) {
  if (FHandleParent != NewHandleParent) {
    FHandleParent = NewHandleParent;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ConsistentAttributes
//---------------------------------------------------------------------------

TConsistentAttributes TTextAttributes::Get_ConsistentAttributes() {
  return FConsistentAttributes;
}

bool TTextAttributes::Set_ConsistentAttributes(TConsistentAttributes NewConsistentAttributes) {

  if (FConsistentAttributes != NewConsistentAttributes) {
    FConsistentAttributes = NewConsistentAttributes;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TTextAttributes::Get_Height() {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_SIZE;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  if (CharFormat.yHeight < 0) return -CharFormat.yHeight / 20; // yHeight en twips et non en points
  else return CharFormat.yHeight / 14;
}

bool TTextAttributes::Set_Height(int NewHeight) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  if (FConsistentAttributes.Contains(caSize)) {
    CharFormat.dwMask = CFM_SIZE;
    if (NewHeight > 0) CharFormat.yHeight = (int) -NewHeight * 20; // yHeight en twips et non en points
    else CharFormat.yHeight = (int) NewHeight * 14;

    SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) SCF_SELECTION, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Size
//---------------------------------------------------------------------------

int TTextAttributes::Get_Size() {
  int ScreenPixelsPerInch;


  ScreenPixelsPerInch = (int) ((double) GetSystemMetrics(SM_CYSCREEN) / (15.00 / 1.414));  // Ecran de 15 pouces / racine(2)

  return (int) ((double) Height * 72 / ScreenPixelsPerInch + .5);
}

bool TTextAttributes::Set_Size(int NewSize) {
  int ScreenPixelsPerInch;


  ScreenPixelsPerInch = (int) ((double) GetSystemMetrics(SM_CYSCREEN) / (15.00 / 1.414));  // Ecran de 15 pouces / racine(2)
  Height = (int) ((double) NewSize * ScreenPixelsPerInch / 72 + .5);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Offset
//---------------------------------------------------------------------------

int TTextAttributes::Get_Offset() {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_OFFSET;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return CharFormat.yOffset;
}

bool TTextAttributes::Set_Offset(int NewOffset) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  if (FConsistentAttributes.Contains(caOffset)) {
    CharFormat.dwMask = CFM_OFFSET;
    CharFormat.yOffset = NewOffset;

    SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) SCF_SELECTION, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TFontStyles TTextAttributes::Get_Style() {
  CHARFORMAT2 CharFormat;
  TFontStyles FStyle;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_BOLD | CFM_ITALIC | CFM_STRIKEOUT | CFM_UNDERLINE;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  FStyle.Clear();
  if (CharFormat.dwEffects & CFE_BOLD) FStyle << fsBold;
  if (CharFormat.dwEffects & CFE_ITALIC) FStyle << fsItalic;
  if (CharFormat.dwEffects & CFE_STRIKEOUT) FStyle << fsStrikeOut;
  if (CharFormat.dwEffects & CFE_UNDERLINE) FStyle << fsUnderline;

  return FStyle;
}

bool TTextAttributes::Set_Style(TFontStyles NewStyle) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  if (FConsistentAttributes.Contains(caBold)) CharFormat.dwMask |= CFM_BOLD;
  if (FConsistentAttributes.Contains(caItalic)) CharFormat.dwMask |= CFM_ITALIC;
  if (FConsistentAttributes.Contains(caStrikeOut)) CharFormat.dwMask |= CFM_STRIKEOUT;
  if (FConsistentAttributes.Contains(caUnderline)) CharFormat.dwMask |= CFM_UNDERLINE;

  if (NewStyle.Contains(fsBold)) CharFormat.dwEffects |= CFE_BOLD;
  if (NewStyle.Contains(fsItalic)) CharFormat.dwEffects |= CFE_ITALIC;
  if (NewStyle.Contains(fsStrikeOut)) CharFormat.dwEffects |= CFE_STRIKEOUT;
  if (NewStyle.Contains(fsUnderline)) CharFormat.dwEffects |= CFE_UNDERLINE;

  SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Charset
//---------------------------------------------------------------------------

TFontCharset TTextAttributes::Get_Charset() {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_CHARSET;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return CharFormat.bCharSet;
}

bool TTextAttributes::Set_Charset(TFontCharset NewCharset) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_CHARSET;

  CharFormat.bCharSet = NewCharset;

  SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Pitch
//---------------------------------------------------------------------------

TFontPitch TTextAttributes::Get_Pitch() {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_FACE;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  if (CharFormat.bPitchAndFamily == DEFAULT_PITCH) return fpDefault;
  if (CharFormat.bPitchAndFamily == VARIABLE_PITCH) return fpVariable;
  if (CharFormat.bPitchAndFamily == FIXED_PITCH) return fpFixed;

  return fpDefault;
}

bool TTextAttributes::Set_Pitch(TFontPitch NewPitch) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_FACE;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  if (NewPitch == fpVariable) CharFormat.bPitchAndFamily = VARIABLE_PITCH;
  else if (NewPitch == fpFixed) CharFormat.bPitchAndFamily = VARIABLE_PITCH;
  else CharFormat.bPitchAndFamily = DEFAULT_PITCH;

  SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TTextAttributes::Get_Color() {
  CHARFORMAT2 CharFormat;
  TColor FColor;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_COLOR;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return TColor(CharFormat.crTextColor);
}

bool TTextAttributes::Set_Color(TColor NewColor) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  if (FConsistentAttributes.Contains(caColor)) {
    CharFormat.dwMask = CFM_COLOR;
    CharFormat.crTextColor = NewColor.cr;
    SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BackColor
//---------------------------------------------------------------------------

TColor TTextAttributes::Get_BackColor() {
  CHARFORMAT2 CharFormat;
  TColor FBackColor;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_BACKCOLOR;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return TColor(CharFormat.crBackColor);
}

bool TTextAttributes::Set_BackColor(TColor NewBackColor) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  if (FConsistentAttributes.Contains(caBackColor)) {
    CharFormat.dwMask = CFM_BACKCOLOR;
    CharFormat.crBackColor = NewBackColor.cr;
    SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Protected
//---------------------------------------------------------------------------

bool TTextAttributes::Get_Protected() {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_PROTECTED;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return ((CharFormat.dwEffects & CFM_PROTECTED) != 0);
}

bool TTextAttributes::Set_Protected(bool NewProtected) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  if (FConsistentAttributes.Contains(caProtected)) {
    CharFormat.dwMask = CFM_PROTECTED;
    if (NewProtected) CharFormat.dwEffects |= CFE_PROTECTED;
    SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TTextAttributes::Get_Name() {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  CharFormat.dwMask = CFM_FACE;
  SendMessage(FHandleParent, EM_GETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);

  return AnsiString(CharFormat.szFaceName);
}

bool TTextAttributes::Set_Name(AnsiString NewName) {
  CHARFORMAT2 CharFormat;


  memset(&CharFormat, 0, sizeof(CHARFORMAT2));
  CharFormat.cbSize = sizeof(CHARFORMAT2);
  if (FConsistentAttributes.Contains(caFace)) {
    CharFormat.dwMask = CFM_FACE;
    lstrcpyn(CharFormat.szFaceName, NewName, LF_FACESIZE);
    SendMessage(FHandleParent, EM_SETCHARFORMAT, (WPARAM) bSelection, (LPARAM) (CHARFORMAT2 FAR *) &CharFormat);
  }

  return true;
}

//---------------------------------------------------------------------------
// Transfert des propriétés
//---------------------------------------------------------------------------

void FASTCALL TTextAttributes::Assign(TTextAttributes * src) {
  FConsistentAttributes = src->FConsistentAttributes;
  Size = src->Size;
  Style = src->Style;
  Charset = src->Charset;
  Pitch = src->Pitch;
  Color = src->Color;
  BackColor = src->BackColor;
  Protected = src->Protected;
}


//---------------------------------------------------------------------------

