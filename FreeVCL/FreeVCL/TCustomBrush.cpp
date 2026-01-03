//-----------------------------------------------------------------------------
//! @file TCustomBrush.cpp
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
// TCustomBrush
//---------------------------------------------------------------------------

TCustomBrush::TCustomBrush(void): TPersistent() {
  // Initialisations
}

TCustomBrush::~TCustomBrush(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TBrushStyle TCustomBrush::Get_Style(void) {
  return (TBrushStyle) 0;
}

bool TCustomBrush::Set_Style(TBrushStyle NewStyle) {
  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TCustomBrush::Get_Color(void) {
  return 0;
}

bool TCustomBrush::Set_Color(TColor NewColor) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TCustomBrush::Assign(TPersistent* Source) {

  TCustomBrush *CustomBrushSource = dynamic_cast<TCustomBrush *> (Source);
  if (CustomBrushSource) {
    Set_Style(CustomBrushSource->Style);
    Set_Color(CustomBrushSource->Color);
    return;
  }

  // Source->AssignTo(this);

  return;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomBrush::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Style"));
  ListProperties->Add(_T("Color"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomBrush::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Style")) {
    *asInfos = _T("bsSolid\nbsClear\nbsHorizontal\nbsVertical\nbsFDiagonal\nbsBDiagonal\nbsCross\nbsDiagCross");
    return tpChoice;
  }
  if (asProperty == _T("Color")) {
    return tpColor;
  }
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomBrush::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Style")) {
    return _T("bsSolid");
  }
  if (asProperty == _T("Color")) {
    return _T("clBlack");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomBrush::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Style")) {
    TBrushStyle Style = Get_Style();
    if (Style == bsSolid) return _T("bsSolid");
    if (Style == bsClear) return _T("bsClear");
    if (Style == bsHorizontal) return _T("bsHorizontal");
    if (Style == bsVertical) return _T("bsVertical");
    if (Style == bsFDiagonal) return _T("bsFDiagonal");
    if (Style == bsBDiagonal) return _T("bsBDiagonal");
    if (Style == bsCross) return _T("bsCross");
    if (Style == bsDiagCross) return _T("bsDiagCross");
    return _T("bsSolid");
  }
  if (asProperty == _T("Color")) {
    return ColorToString(Get_Color());
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomBrush::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  if (asProperty == _T("Style")) {
    if (asValue == _T("bsSolid")) Set_Style(bsSolid);
    else if (asValue == _T("bsClear")) Set_Style(bsClear);
    else if (asValue == _T("bsHorizontal")) Set_Style(bsHorizontal);
    else if (asValue == _T("bsVertical")) Set_Style(bsVertical);
    else if (asValue == _T("bsFDiagonal")) Set_Style(bsFDiagonal);
    else if (asValue == _T("bsBDiagonal")) Set_Style(bsBDiagonal);
    else if (asValue == _T("bsCross")) Set_Style(bsCross);
    else if (asValue == _T("bsDiagCross")) Set_Style(bsDiagCross);
    return true;
  }
  if (asProperty == _T("Color")) {
    TColor NewColor;
    NewColor = asValue;  // L'opérateur = a été redéfini dans TColor
    Set_Color(NewColor);
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

