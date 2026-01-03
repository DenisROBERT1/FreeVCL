//-----------------------------------------------------------------------------
//! @file TDateTimePicker.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TDateTimePicker
//---------------------------------------------------------------------------

TDateTimePicker::TDateTimePicker(TComponent* AOwner): TCommonCalendar(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TDateTimePicker");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= DTS_RIGHTALIGN;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         DATETIMEPICK_CLASS, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TDateTimePicker::~TDateTimePicker(void) {
}

//---------------------------------------------------------------------------
int TDateTimePicker::ProcessNotify(LPNMHDR pnmh) {

  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
	case DTN_DATETIMECHANGE:
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TDateTimePicker::Get_Text(void) {
  TCHAR *szBuf;
  int Length;
  AnsiString sRetour;


  if (FHandle) {
    Length = GetWindowTextLength(FHandle) + 1;
    szBuf = new TCHAR[Length];
    GetWindowText(FHandle, szBuf, Length);
    FDate = szBuf;
    delete[] szBuf;
  }

  sRetour = FDate.FormatString("dd/mm/yyyy");

  return sRetour;
}

bool TDateTimePicker::Set_Text(AnsiString NewText) {
  TDateTime DateTime;
  SYSTEMTIME SysTime;


  FDate = NewText;
  if (FHandle) {
    memset(&SysTime, 0, sizeof(SYSTEMTIME));
    SysTime.wYear = FDate.Year;
    SysTime.wMonth = FDate.Month;
    SysTime.wDay = FDate.Day;
    SendMessage(FHandle, DTM_SETSYSTEMTIME, (WPARAM) GDT_VALID, (LPARAM) &SysTime);
  }
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Date
//---------------------------------------------------------------------------

TDateTime TDateTimePicker::Get_Date(void) {
  TCHAR *szBuf;
  int Length;


  if (FHandle) {
    Length = GetWindowTextLength(FHandle) + 1;
    szBuf = new TCHAR[Length];
    GetWindowText(FHandle, szBuf, Length);
    FDate = szBuf;
    delete[] szBuf;
  }

  return FDate;
}

bool TDateTimePicker::Set_Date(TDateTime NewDate) {
  SYSTEMTIME SysTime;


  FDate = NewDate;
  if (FHandle) {
    memset(&SysTime, 0, sizeof(SYSTEMTIME));
    SysTime.wYear = FDate.Year;
    SysTime.wMonth = FDate.Month;
    SysTime.wDay = FDate.Day;
    SendMessage(FHandle, DTM_SETSYSTEMTIME, (WPARAM) GDT_VALID, (LPARAM) &SysTime);
  }
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDateTimePicker::GetListProperties(TStrings *ListProperties) {

  TCommonCalendar::GetListProperties(ListProperties);

  ListProperties->Add(_T("Text"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDateTimePicker::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Text")) {
    return tpText;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TCommonCalendar::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDateTimePicker::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Text")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  return TCommonCalendar::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDateTimePicker::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Text")) {
    return Get_Text();
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TCommonCalendar::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDateTimePicker::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Text")) {
    Set_Text(asValue);
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  return TCommonCalendar::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

