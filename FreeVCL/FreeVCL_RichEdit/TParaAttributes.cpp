//-----------------------------------------------------------------------------
//! @file TParaAttributes.cpp
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

TParaAttributes::TParaAttributes(HWND HandleParent): TPersistent() {
  this->FHandleParent = HandleParent;
  FBorderLocations.Clear();
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TParaAttributes::~TParaAttributes() {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HandleParent
//---------------------------------------------------------------------------

HWND TParaAttributes::Get_HandleParent() {
  return FHandleParent;
}

bool TParaAttributes::Set_HandleParent(HWND NewHandleParent) {
  if (FHandleParent != NewHandleParent) {
    FHandleParent = NewHandleParent;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TParaAttributes::Get_Alignment() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_ALIGNMENT;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  if (ParaFormat.wAlignment == PFA_RIGHT) return taRightJustify;
  if (ParaFormat.wAlignment == PFA_CENTER) return taCenter;
  return taLeftJustify;
}

bool TParaAttributes::Set_Alignment(TAlignment NewAlignment) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_ALIGNMENT;
  if (NewAlignment == taRightJustify) ParaFormat.wAlignment = PFA_RIGHT;
  else if (NewAlignment == taCenter) ParaFormat.wAlignment = PFA_CENTER;
  else ParaFormat.wAlignment = PFA_LEFT;

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SpaceBefore
//---------------------------------------------------------------------------

int TParaAttributes::Get_SpaceBefore() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_SPACEBEFORE;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return ParaFormat.dySpaceBefore / 20; // dySpaceBefore en twips
}

bool TParaAttributes::Set_SpaceBefore(int NewSpaceBefore) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_SPACEBEFORE;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  ParaFormat.dySpaceBefore = (int) NewSpaceBefore * 20; // dySpaceBefore en twips

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SpaceAfter
//---------------------------------------------------------------------------

int TParaAttributes::Get_SpaceAfter() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_SPACEBEFORE;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return ParaFormat.dySpaceAfter / 20; // dySpaceAfter en twips
}

bool TParaAttributes::Set_SpaceAfter(int NewSpaceAfter) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_SPACEAFTER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  ParaFormat.dySpaceAfter = (int) NewSpaceAfter * 20; // dySpaceAfter en twips

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LineSpacing
//---------------------------------------------------------------------------

int TParaAttributes::Get_LineSpacing() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_LINESPACING;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return ParaFormat.dyLineSpacing / 20; // dyLineSpacing en twips
}

bool TParaAttributes::Set_LineSpacing(int NewLineSpacing) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_LINESPACING;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  ParaFormat.dyLineSpacing = (int) NewLineSpacing * 20; // dyLineSpacing en twips

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LineSpacingRule
//---------------------------------------------------------------------------

TRELineSpacingRule TParaAttributes::Get_LineSpacingRule() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_LINESPACING;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  if (ParaFormat.bLineSpacingRule == 0) return lsrSingle;
  if (ParaFormat.bLineSpacingRule == 1) return lsrHalf;
  if (ParaFormat.bLineSpacingRule == 2) return lsrDouble;
  if (ParaFormat.bLineSpacingRule == 3) return lsrSingleMini;
  if (ParaFormat.bLineSpacingRule == 4) return lsrLineSpacing;
  if (ParaFormat.bLineSpacingRule == 5) return lsrPercent;

  return lsrSingle;
}

bool TParaAttributes::Set_LineSpacingRule(TRELineSpacingRule NewLineSpacingRule) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_LINESPACING;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  if (NewLineSpacingRule == lsrSingle     ) ParaFormat.bLineSpacingRule = 0;
  if (NewLineSpacingRule == lsrHalf       ) ParaFormat.bLineSpacingRule = 1;
  if (NewLineSpacingRule == lsrDouble     ) ParaFormat.bLineSpacingRule = 2;
  if (NewLineSpacingRule == lsrSingleMini ) ParaFormat.bLineSpacingRule = 3;
  if (NewLineSpacingRule == lsrLineSpacing) ParaFormat.bLineSpacingRule = 4;
  if (NewLineSpacingRule == lsrPercent    ) ParaFormat.bLineSpacingRule = 5;

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FirstIndent
//---------------------------------------------------------------------------

int TParaAttributes::Get_FirstIndent() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = CFM_OFFSET;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return ParaFormat.dxStartIndent;
}

bool TParaAttributes::Set_FirstIndent(int NewFirstIndent) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = CFM_OFFSET;
  ParaFormat.dxStartIndent = NewFirstIndent;

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderLocation
//---------------------------------------------------------------------------

TREBorderLocations TParaAttributes::Get_BorderLocations() {
  return FBorderLocations;
}

bool TParaAttributes::Set_BorderLocations(TREBorderLocations NewBorderLocations) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TREBorderStyle TParaAttributes::Get_BorderStyle() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return (TREBorderStyle) ((ParaFormat.wBorders >> 8) & 0x000F);
}

bool TParaAttributes::Set_BorderStyle(TREBorderStyle NewBorderStyles) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  if (FBorderLocations.Contains(blLeftBorder)) ParaFormat.wBorders |= 1;
  if (FBorderLocations.Contains(blRightBorder)) ParaFormat.wBorders |= 2;
  if (FBorderLocations.Contains(blTopBorder)) ParaFormat.wBorders |= 4;
  if (FBorderLocations.Contains(blBottomBorder)) ParaFormat.wBorders |= 8;
  if (FBorderLocations.Contains(blInsideBorder)) ParaFormat.wBorders |= 16;
  if (FBorderLocations.Contains(blOutsideBorder)) ParaFormat.wBorders |= 32;

  ParaFormat.wBorders |= (int) NewBorderStyles << 8;

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderWidth
//---------------------------------------------------------------------------

int TParaAttributes::Get_BorderWidth() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return (ParaFormat.wBorderWidth);
}

bool TParaAttributes::Set_BorderWidth(int NewBorderWidth) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  if (FBorderLocations.Contains(blLeftBorder)) ParaFormat.wBorders |= 1;
  if (FBorderLocations.Contains(blRightBorder)) ParaFormat.wBorders |= 2;
  if (FBorderLocations.Contains(blTopBorder)) ParaFormat.wBorders |= 4;
  if (FBorderLocations.Contains(blBottomBorder)) ParaFormat.wBorders |= 8;
  if (FBorderLocations.Contains(blInsideBorder)) ParaFormat.wBorders |= 16;
  if (FBorderLocations.Contains(blOutsideBorder)) ParaFormat.wBorders |= 32;

  ParaFormat.wBorderWidth |= (int) NewBorderWidth << 12;

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderSpace
//---------------------------------------------------------------------------

int TParaAttributes::Get_BorderSpace() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return (ParaFormat.wBorderSpace);
}

bool TParaAttributes::Set_BorderSpace(int NewBorderSpace) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  if (FBorderLocations.Contains(blLeftBorder)) ParaFormat.wBorders |= 1;
  if (FBorderLocations.Contains(blRightBorder)) ParaFormat.wBorders |= 2;
  if (FBorderLocations.Contains(blTopBorder)) ParaFormat.wBorders |= 4;
  if (FBorderLocations.Contains(blBottomBorder)) ParaFormat.wBorders |= 8;
  if (FBorderLocations.Contains(blInsideBorder)) ParaFormat.wBorders |= 16;
  if (FBorderLocations.Contains(blOutsideBorder)) ParaFormat.wBorders |= 32;

  ParaFormat.wBorderSpace = NewBorderSpace;

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderColor
//---------------------------------------------------------------------------

TREBorderColor TParaAttributes::Get_BorderColor() {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return (TREBorderColor) ((ParaFormat.wBorders >> 12) & 0x000F);
}

bool TParaAttributes::Set_BorderColor(TREBorderColor NewBorderColor) {
  PARAFORMAT2 ParaFormat;


  memset(&ParaFormat, 0, sizeof(PARAFORMAT2));
  ParaFormat.cbSize = sizeof(PARAFORMAT2);
  ParaFormat.dwMask = PFM_BORDER;
  SendMessage(FHandleParent, EM_GETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  if (FBorderLocations.Contains(blLeftBorder)) ParaFormat.wBorders |= 1;
  if (FBorderLocations.Contains(blRightBorder)) ParaFormat.wBorders |= 2;
  if (FBorderLocations.Contains(blTopBorder)) ParaFormat.wBorders |= 4;
  if (FBorderLocations.Contains(blBottomBorder)) ParaFormat.wBorders |= 8;
  if (FBorderLocations.Contains(blInsideBorder)) ParaFormat.wBorders |= 16;
  if (FBorderLocations.Contains(blOutsideBorder)) ParaFormat.wBorders |= 32;

  ParaFormat.wBorders |= (int) NewBorderColor << 12;

  SendMessage(FHandleParent, EM_SETPARAFORMAT, (WPARAM) 0, (LPARAM) (PARAFORMAT2 FAR *) &ParaFormat);

  return true;
}


//---------------------------------------------------------------------------
// Transfert des propriétés
//---------------------------------------------------------------------------

void FASTCALL TParaAttributes::Assign(TParaAttributes * src) {

  Alignment = src->Alignment;
  SpaceBefore = src->SpaceBefore;
  SpaceAfter = src->SpaceAfter;
  LineSpacing = src->LineSpacing;
  LineSpacingRule = src->LineSpacingRule;
  FirstIndent = src->FirstIndent;
  BorderLocations = src->BorderLocations;
  BorderStyle = src->BorderStyle;
  BorderWidth = src->BorderWidth;
  BorderColor = src->BorderColor;
}


//---------------------------------------------------------------------------

