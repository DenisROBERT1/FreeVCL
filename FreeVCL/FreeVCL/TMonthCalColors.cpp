//-----------------------------------------------------------------------------
//! @file TMonthCalColors.cpp
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

#define _WIN32_WINNT 0x500

#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TMonthCalColors::TMonthCalColors(HWND HandleParent): TPersistent() {
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TMonthCalColors::~TMonthCalColors(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BackColor
//---------------------------------------------------------------------------

TColor TMonthCalColors::Get_BackColor(void) {
  return FBackColor;
}

bool TMonthCalColors::Set_BackColor(TColor NewBackColor) {
  if (FBackColor != NewBackColor) {
    FBackColor = NewBackColor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MonthBackColor
//---------------------------------------------------------------------------

TColor TMonthCalColors::Get_MonthBackColor(void) {
  return FMonthBackColor;
}

bool TMonthCalColors::Set_MonthBackColor(TColor NewMonthBackColor) {
  if (FMonthBackColor != NewMonthBackColor) {
    FMonthBackColor = NewMonthBackColor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TextColor
//---------------------------------------------------------------------------

TColor TMonthCalColors::Get_TextColor(void) {
  return FTextColor;
}

bool TMonthCalColors::Set_TextColor(TColor NewTextColor) {
  if (FTextColor != NewTextColor) {
    FTextColor = NewTextColor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TitleBackColor
//---------------------------------------------------------------------------

TColor TMonthCalColors::Get_TitleBackColor(void) {
  return FTitleBackColor;
}

bool TMonthCalColors::Set_TitleBackColor(TColor NewTitleBackColor) {
  if (FTitleBackColor != NewTitleBackColor) {
    FTitleBackColor = NewTitleBackColor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TitleTextColor
//---------------------------------------------------------------------------

TColor TMonthCalColors::Get_TitleTextColor(void) {
  return FTitleTextColor;
}

bool TMonthCalColors::Set_TitleTextColor(TColor NewTitleTextColor) {
  if (FTitleTextColor != NewTitleTextColor) {
    FTitleTextColor = NewTitleTextColor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TrailingTextColor
//---------------------------------------------------------------------------

TColor TMonthCalColors::Get_TrailingTextColor(void) {
  return FTrailingTextColor;
}

bool TMonthCalColors::Set_TrailingTextColor(TColor NewTrailingTextColor) {
  if (FTrailingTextColor != NewTrailingTextColor) {
    FTrailingTextColor = NewTrailingTextColor;
  }
  return true;
}


//---------------------------------------------------------------------------
// Transfert des propriétés
//---------------------------------------------------------------------------

void FASTCALL TMonthCalColors::Assign(TPersistent* Source) {
	TMonthCalColors *MonthCalColorsSource;

	MonthCalColorsSource = dynamic_cast<TMonthCalColors *>(Source);
	if (MonthCalColorsSource == NULL) return;

	FBackColor = MonthCalColorsSource->FBackColor;
  FMonthBackColor = MonthCalColorsSource->FMonthBackColor;
  FTextColor = MonthCalColorsSource->FTextColor;
  FTitleBackColor = MonthCalColorsSource->FTitleBackColor;
  FTitleTextColor = MonthCalColorsSource->FTitleTextColor;
  FTrailingTextColor = MonthCalColorsSource->FTrailingTextColor;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TMonthCalColors::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("BackColor"));
  ListProperties->Add(_T("MonthBackColor"));
  ListProperties->Add(_T("TextColor"));
  ListProperties->Add(_T("TitleBackColor"));
  ListProperties->Add(_T("TitleTextColor"));
  ListProperties->Add(_T("TrailingTextColor"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TMonthCalColors::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("BackColor")) {
    return tpColor;
  }
  if (asProperty == _T("MonthBackColor")) {
    return tpColor;
  }
  if (asProperty == _T("TextColor")) {
    return tpColor;
  }
  if (asProperty == _T("TitleBackColor")) {
    return tpColor;
  }
  if (asProperty == _T("TitleTextColor")) {
    return tpColor;
  }
  if (asProperty == _T("TrailingTextColor")) {
    return tpColor;
  }
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TMonthCalColors::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("BackColor")) {
    return _T("clBlack");
  }
  if (asProperty == _T("MonthBackColor")) {
    return _T("clBlack");
  }
  if (asProperty == _T("TextColor")) {
    return _T("clBlack");
  }
  if (asProperty == _T("TitleBackColor")) {
    return _T("clBlack");
  }
  if (asProperty == _T("TitleTextColor")) {
    return _T("clBlack");
  }
  if (asProperty == _T("TrailingTextColor")) {
    return _T("clBlack");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TMonthCalColors::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("BackColor")) {
    return ColorToString(Get_BackColor());
  }
  if (asProperty == _T("MonthBackColor")) {
    return ColorToString(Get_MonthBackColor());
  }
  if (asProperty == _T("TextColor")) {
    return ColorToString(Get_TextColor());
  }
  if (asProperty == _T("TitleBackColor")) {
    return ColorToString(Get_TitleBackColor());
  }
  if (asProperty == _T("TitleTextColor")) {
    return ColorToString(Get_TitleTextColor());
  }
  if (asProperty == _T("TrailingTextColor")) {
    return ColorToString(Get_TrailingTextColor());
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TMonthCalColors::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("BackColor")) {
    TColor Color;
    Color = asValue;
    Set_BackColor(Color);
    return true;
  }
  if (asProperty == _T("MonthBackColor")) {
    TColor Color;
    Color = asValue;
    Set_MonthBackColor(Color);
    return true;
  }
  if (asProperty == _T("TextColor")) {
    TColor Color;
    Color = asValue;
    Set_TextColor(Color);
    return true;
  }
  if (asProperty == _T("TitleBackColor")) {
    TColor Color;
    Color = asValue;
    Set_TitleBackColor(Color);
    return true;
  }
  if (asProperty == _T("TitleTextColor")) {
    TColor Color;
    Color = asValue;
    Set_TitleTextColor(Color);
    return true;
  }
  if (asProperty == _T("TrailingTextColor")) {
    TColor Color;
    Color = asValue;
    Set_TrailingTextColor(Color);
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

