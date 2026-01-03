//-----------------------------------------------------------------------------
//! @file TCustomPen.cpp
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
// TCustomPen
//---------------------------------------------------------------------------

TCustomPen::TCustomPen(void): TPersistent() {
}

TCustomPen::~TCustomPen(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TPenStyle TCustomPen::Get_Style(void) {
  return psSolid;
}

bool TCustomPen::Set_Style(TPenStyle NewStyle) {
  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TCustomPen::Get_Width(void) {
  return 0;
}

bool TCustomPen::Set_Width(int NewWidth) {
  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

TColor TCustomPen::Get_Color(void) {
  return 0;
}

bool TCustomPen::Set_Color(TColor NewColor) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TCustomPen::Assign(TPersistent* Source) {

  TCustomPen *CustomPenSource = dynamic_cast<TCustomPen *> (Source);
  if (CustomPenSource) {
    Set_Width(CustomPenSource->Width);
    Set_Style(CustomPenSource->Style);
    Set_Color(CustomPenSource->Color);
    return;
  }

  // Source->AssignTo(this);

  return;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomPen::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Width"));
  ListProperties->Add(_T("Name"));
  ListProperties->Add(_T("Style"));
  ListProperties->Add(_T("Color"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomPen::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Width")) {
    return tpNumber;
  }
  if (asProperty == _T("Style")) {
    *asInfos = _T("psSolid\npsDash\npsDot\npsDashDot\npsDashDotDot\npsClear\npsInsideFrame");
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

AnsiString TCustomPen::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Width")) {
    return _T("0");
  }
  if (asProperty == _T("Style")) {
    return _T("psSolid");
  }
  if (asProperty == _T("Color")) {
    return _T("clBlack");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomPen::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Width")) {
    return IntToStr(Get_Width());
  }
  if (asProperty == _T("Style")) {
    TPenStyle Style = Get_Style();
    if (Style == psSolid) return _T("psSolid");
    if (Style == psDash) return _T("psDash");
    if (Style == psDot) return _T("psDot");
    if (Style == psDashDot) return _T("psDashDot");
    if (Style == psDashDotDot) return _T("psDashDotDot");
    if (Style == psClear) return _T("psClear");
    if (Style == psInsideFrame) return _T("psInsideFrame");
    return _T("psSolid");
  }
  if (asProperty == _T("Color")) {
    return ColorToString(Get_Color());
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomPen::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  AnsiString asMot;


  if (asProperty == _T("Width")) {
    Set_Width(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Style")) {
    if (asMot == _T("psSolid")) Set_Style(psSolid);
    else if (asMot == _T("psDash")) Set_Style(psSolid);
    else if (asMot == _T("psDot")) Set_Style(psSolid);
    else if (asMot == _T("psDashDot")) Set_Style(psSolid);
    else if (asMot == _T("psDashDotDot")) Set_Style(psSolid);
    else if (asMot == _T("psClear")) Set_Style(psSolid);
    else if (asMot == _T("psInsideFrame")) Set_Style(psSolid);
    return true;
  }
  if (asProperty == _T("Color")) {
    TColor Color;
    Color = asValue;
    Set_Color(Color);
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

