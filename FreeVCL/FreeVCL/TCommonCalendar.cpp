//-----------------------------------------------------------------------------
//! @file TCommonCalendar.cpp
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
// TCommonCalendar
//---------------------------------------------------------------------------

TCommonCalendar::TCommonCalendar(TComponent* AOwner): TWinControl(AOwner) {

  // Initialisations
  FClassName = _T("TCommonCalendar");
  FTabStop = true;
	FCalColors = NULL;
  FMaxSelectRange = 0;
  FMultiSelect = false;
  FShowToday = true;
  FShowTodayCircle = true;
  FWeekNumbers = true;

  // Enregistrement de la classe CommonCalendar
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_DATE_CLASSES;
  InitCommonControlsEx(&InitCtrls);

}

//---------------------------------------------------------------------------
TCommonCalendar::~TCommonCalendar(void) {
}

//---------------------------------------------------------------------------
int TCommonCalendar::ProcessNotify(LPNMHDR pnmh) {

  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
	case MCN_GETDAYSTATE:
		static MONTHDAYSTATE mds[12];
		NMDAYSTATE * lpnmDS = (NMDAYSTATE *) pnmh;
		int i, iMax;

		iMax = lpnmDS->cDayState;
		        
		for (i = 0; i < iMax; i++) {
			mds[i] = (MONTHDAYSTATE) 0;
			if (!FComponentState.Contains(csLoading)) {
				OnGetMonthInfo(this, i + 1, &mds[i]);
			}
		}
		lpnmDS->prgDayState = mds;

		return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CalColors
//---------------------------------------------------------------------------

TMonthCalColors* TCommonCalendar::Get_CalColors(void) {
  return FCalColors;
}

bool TCommonCalendar::Set_CalColors(TMonthCalColors* NewCalColors) {
  if (FCalColors != NewCalColors) {
    FCalColors = NewCalColors;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Date
//---------------------------------------------------------------------------

TDateTime TCommonCalendar::Get_Date(void) {
  return FDate;
}

bool TCommonCalendar::Set_Date(TDateTime NewDate) {
  if (FDate != NewDate) {
    FDate = NewDate;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété EndDate
//---------------------------------------------------------------------------

TDateTime TCommonCalendar::Get_EndDate(void) {
  return FEndDate;
}

bool TCommonCalendar::Set_EndDate(TDateTime NewEndDate) {
  if (FEndDate != NewEndDate) {
    FEndDate = NewEndDate;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FirstDayOfWeek
//---------------------------------------------------------------------------

TCalDayOfWeek TCommonCalendar::Get_FirstDayOfWeek(void) {
  return FFirstDayOfWeek;
}

bool TCommonCalendar::Set_FirstDayOfWeek(TCalDayOfWeek NewFirstDayOfWeek) {
  if (FFirstDayOfWeek != NewFirstDayOfWeek) {
    FFirstDayOfWeek = NewFirstDayOfWeek;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MaxDate
//---------------------------------------------------------------------------

TDateTime TCommonCalendar::Get_MaxDate(void) {
  return FMaxDate;
}

bool TCommonCalendar::Set_MaxDate(TDateTime NewMaxDate) {
  if (FMaxDate != NewMaxDate) {
    FMaxDate = NewMaxDate;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MaxSelectRange
//---------------------------------------------------------------------------

int TCommonCalendar::Get_MaxSelectRange(void) {
  return FMaxSelectRange;
}

bool TCommonCalendar::Set_MaxSelectRange(int NewMaxSelectRange) {
  if (FMaxSelectRange != NewMaxSelectRange) {
    FMaxSelectRange = NewMaxSelectRange;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MinDate
//---------------------------------------------------------------------------

TDateTime TCommonCalendar::Get_MinDate(void) {
  return FMinDate;
}

bool TCommonCalendar::Set_MinDate(TDateTime NewMinDate) {
  if (FMinDate != NewMinDate) {
    FMinDate = NewMinDate;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiSelect
//---------------------------------------------------------------------------

bool TCommonCalendar::Get_MultiSelect(void) {
  return FMultiSelect;
}

bool TCommonCalendar::Set_MultiSelect(bool NewMultiSelect) {
  if (FMultiSelect != NewMultiSelect) {
    FMultiSelect = NewMultiSelect;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowToday
//---------------------------------------------------------------------------

bool TCommonCalendar::Get_ShowToday(void) {
  return FShowToday;
}

bool TCommonCalendar::Set_ShowToday(bool NewShowToday) {
  if (FShowToday != NewShowToday) {
    FShowToday = NewShowToday;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowTodayCircle
//---------------------------------------------------------------------------

bool TCommonCalendar::Get_ShowTodayCircle(void) {
  return FShowTodayCircle;
}

bool TCommonCalendar::Set_ShowTodayCircle(bool NewShowTodayCircle) {
  if (FShowTodayCircle != NewShowTodayCircle) {
    FShowTodayCircle = NewShowTodayCircle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WeekNumbers
//---------------------------------------------------------------------------

bool TCommonCalendar::Get_WeekNumbers(void) {
  return FWeekNumbers;
}

bool TCommonCalendar::Set_WeekNumbers(bool NewWeekNumbers) {
  if (FWeekNumbers != NewWeekNumbers) {
    FWeekNumbers = NewWeekNumbers;
  }
  return true;
}


//---------------------------------------------------------------------------
// Code un ensemble de date pour évènement OnGetMonthInfo.
//---------------------------------------------------------------------------
void FASTCALL TCommonCalendar::BoldDays(const unsigned * Days,
																			 int Days_Size,
																			 unsigned long &MonthBoldInfo) {
	int i, iDay;

	MonthBoldInfo = 0;
	for (i = 0; i <= Days_Size; i++) {
		iDay = Days[i];
		if (1 <= iDay && iDay <= 31) {
			(MonthBoldInfo) |= (0x00000001 << (iDay - 1));
		}
	}
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCommonCalendar::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("CalColors"));
  ListProperties->Add(_T("Date"));
  ListProperties->Add(_T("EndDate"));
  ListProperties->Add(_T("FirstDayOfWeek"));
  ListProperties->Add(_T("MaxDate"));
  ListProperties->Add(_T("MaxSelectRange"));
  ListProperties->Add(_T("MinDate"));
  ListProperties->Add(_T("MultiSelect"));
  ListProperties->Add(_T("ShowToday"));
  ListProperties->Add(_T("ShowTodayCircle"));
  ListProperties->Add(_T("WeekNumbers"));
  ListProperties->Add(_T("OnGetMonthInfo"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCommonCalendar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("CalColors")) {
    return tpObject;
  }
  if (asProperty == _T("Date")) {
    return tpText;
  }
  if (asProperty == _T("EndDate")) {
    return tpText;
  }
  if (asProperty == _T("FirstDayOfWeek")) {
    *asInfos = "dowMonday\ndowTuesday\ndowWednesday\ndowThursday\ndowFriday\ndowSaturday\ndowSunday\ndowLocaleDefault";
    return tpChoice;
  }
  if (asProperty == _T("MaxDate")) {
    return tpText;
  }
  if (asProperty == _T("MaxSelectRange")) {
    return tpNumber;
  }
  if (asProperty == _T("MinDate")) {
    return tpText;
  }
  if (asProperty == _T("MultiSelect")) {
    return tpBool;
  }
  if (asProperty == _T("ShowToday")) {
    return tpBool;
  }
  if (asProperty == _T("ShowTodayCircle")) {
    return tpBool;
  }
  if (asProperty == _T("WeekNumbers")) {
    return tpBool;
  }
  if (asProperty == _T("OnGetMonthInfo")) {
    *asInfos = _T("(TObject *Sender, int NbMonths, MONTHDAYSTATE *State)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCommonCalendar::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("CalColors")) {
    return _T("");
  }
  if (asProperty == _T("Date")) {
    return _T("");
  }
  if (asProperty == _T("EndDate")) {
    return _T("");
  }
  if (asProperty == _T("FirstDayOfWeek")) {
    return _T("");
  }
  if (asProperty == _T("MaxDate")) {
    return _T("");
  }
  if (asProperty == _T("MaxSelectRange")) {
    return _T("");
  }
  if (asProperty == _T("MinDate")) {
    return _T("");
  }
  if (asProperty == _T("MultiSelect")) {
    return _T("");
  }
  if (asProperty == _T("ShowToday")) {
    return _T("");
  }
  if (asProperty == _T("ShowTodayCircle")) {
    return _T("");
  }
  if (asProperty == _T("WeekNumbers")) {
    return _T("");
  }
  if (asProperty == _T("OnGetMonthInfo")) {
    return _T("");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCommonCalendar::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("CalColors")) {
    TMonthCalColors *CalColors = Get_CalColors();
    if (CalColors) return CalColors->Name;
    return _T("");
  }
  if (asProperty == _T("Date")) {
    return _T("");
  }
  if (asProperty == _T("EndDate")) {
    return _T("");
  }
  if (asProperty == _T("FirstDayOfWeek")) {
    TCalDayOfWeek FirstDayOfWeek = Get_FirstDayOfWeek();
    if (FirstDayOfWeek == dowMonday) return _T("dowMonday");
    if (FirstDayOfWeek == dowTuesday) return _T("dowTuesday");
    if (FirstDayOfWeek == dowWednesday) return _T("dowWednesday");
    if (FirstDayOfWeek == dowThursday) return _T("dowThursday");
    if (FirstDayOfWeek == dowFriday) return _T("dowFriday");
    if (FirstDayOfWeek == dowSaturday) return _T("dowSaturday");
    if (FirstDayOfWeek == dowSunday) return _T("dowSunday");
    if (FirstDayOfWeek == dowLocaleDefault) return _T("dowLocaleDefault");
    return _T("");
  }
  if (asProperty == _T("MaxDate")) {
    return _T("");
  }
  if (asProperty == _T("MaxSelectRange")) {
    return IntToStr(Get_MaxSelectRange());
  }
  if (asProperty == _T("MinDate")) {
    return _T("");
  }
  if (asProperty == _T("MultiSelect")) {
    if (Get_MultiSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ShowToday")) {
    if (Get_ShowToday()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ShowTodayCircle")) {
    if (Get_ShowTodayCircle()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("WeekNumbers")) {
    if (Get_WeekNumbers()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("OnGetMonthInfo")) {
    return OnGetMonthInfo.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCommonCalendar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("CalColors")) {
    TMonthCalColors *CalColors;
		if (asValue.IsEmpty()) CalColors = NULL;
		else CalColors = dynamic_cast<TMonthCalColors *>(FindRootComponent()->FindComponent(asValue));
    Set_CalColors(CalColors);
    return true;
  }
  if (asProperty == _T("Date")) {
    return true;
  }
  if (asProperty == _T("EndDate")) {
    return true;
  }
  if (asProperty == _T("FirstDayOfWeek")) {
    if      (asValue == _T("dowMonday")) Set_FirstDayOfWeek(dowMonday);
    else if (asValue == _T("dowTuesday")) Set_FirstDayOfWeek(dowTuesday);
    else if (asValue == _T("dowWednesday")) Set_FirstDayOfWeek(dowWednesday);
    else if (asValue == _T("dowThursday")) Set_FirstDayOfWeek(dowThursday);
    else if (asValue == _T("dowFriday")) Set_FirstDayOfWeek(dowFriday);
    else if (asValue == _T("dowSaturday")) Set_FirstDayOfWeek(dowSaturday);
    else if (asValue == _T("dowSunday")) Set_FirstDayOfWeek(dowSunday);
    else if (asValue == _T("dowLocaleDefault")) Set_FirstDayOfWeek(dowLocaleDefault);
    return true;
  }
  if (asProperty == _T("MaxDate")) {
    return true;
  }
  if (asProperty == _T("MaxSelectRange")) {
    Set_MaxSelectRange(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("MinDate")) {
    return true;
  }
  if (asProperty == _T("MultiSelect")) {
    Set_MultiSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ShowToday")) {
    Set_ShowToday(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ShowTodayCircle")) {
    Set_ShowTodayCircle(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("WeekNumbers")) {
    Set_WeekNumbers(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("OnGetMonthInfo")) {
		OnGetMonthInfo.ObjCall = Sender;
    OnGetMonthInfo = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

