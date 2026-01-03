//-----------------------------------------------------------------------------
//! @file TDateTime.cpp
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
// Variables globales
//---------------------------------------------------------------------------

AnsiString ShortDateFormat;
AnsiString LongDateFormat;
AnsiString ShortTimeFormat;
AnsiString LongTimeFormat;


//---------------------------------------------------------------------------
// TDateTime
//---------------------------------------------------------------------------

TDateTime::TDateTime(void) {
  TimeMS = 0;
  FYear = 1899;
  FMonth = 12;
  FDay = 30;
  FHour = 12;
  FMin = 00;
  FSec = 00;
  FMSec = 00;
	TCHAR szFormat[20];


	if (ShortDateFormat.IsEmpty()) {
		if (GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, szFormat, sizeof(szFormat) / sizeof(TCHAR))) {
		  ShortDateFormat = AnsiString(szFormat).LowerCase();
		}
		else {
			ShortDateFormat = _T("dd/mm/yyyy");
		}
	}

	if (LongDateFormat.IsEmpty()) {
		if (GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLONGDATE, szFormat, sizeof(szFormat) / sizeof(TCHAR))) {
			LongDateFormat = AnsiString(szFormat).LowerCase();
		}
		else {
			LongDateFormat = _T("dddd d mmmm yyyy");
		}
	}

	if (ShortTimeFormat.IsEmpty()) {
		if (GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, szFormat, sizeof(szFormat) / sizeof(TCHAR))) {
			ShortTimeFormat = AnsiString(szFormat).LowerCase();
			TReplaceFlags ReplaceFlags;
			ReplaceFlags.Clear() << rfReplaceAll;
			ShortTimeFormat = StringReplace(ShortTimeFormat, _T("m"), _T("n"), ReplaceFlags);
		}
		else {
			ShortTimeFormat = _T("hh:nn:ss");
		}
	}

	if (LongTimeFormat.IsEmpty()) {
		if (GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, szFormat, sizeof(szFormat) / sizeof(TCHAR))) {
			LongTimeFormat = AnsiString(szFormat).LowerCase();
			TReplaceFlags ReplaceFlags;
			ReplaceFlags.Clear() << rfReplaceAll;
			LongTimeFormat = StringReplace(LongTimeFormat, _T("m"), _T("n"), ReplaceFlags);
		}
		else {
			LongTimeFormat = _T("h:n:s");
		}
	}

}

TDateTime::TDateTime(const TDateTimeBase& src) {
  operator =(src);
}

TDateTime::TDateTime(const TDateTime& src) {
  operator =(src);
}

TDateTime::TDateTime(const double src) {
  operator =(src);
}

TDateTime::TDateTime(int src) {
  operator =(src);
}

TDateTime::TDateTime(const time_t src) {
  operator =(src);
}

TDateTime::TDateTime(const FILETIME src) {
  operator =(src);
}

TDateTime::TDateTime(const AnsiString& src, TDateTimeFlag flag) {
  AnsiString asDate;
  AnsiString asMot;
  AnsiString format;
	int FormatLength;
  TCHAR c, dc = 0;
  int i, j;
	int Nb[6];	// Nombres
  int NbNb;  // Nombre de nombres
  bool bGuillemets = false;


  TimeMS = 0;
  FYear = 1899;
  FMonth = 12;
  FDay = 30;
  FHour = 12;
  FMin = 00;
  FSec = 00;
  FMSec = 00;

	asDate = src + _T('/');
  NbNb = 0;
  for (i = 1; i <= (int) asDate.Length() && NbNb < 6; i++) {
    c = asDate[i];
    if (_T('0') <= c && c <= _T('9')) {
      asMot += c;
    }
    else {
			if (!asMot.IsEmpty()) {
				Nb[NbNb++] = StrToInt(asMot);
				asMot = _T("");
			}
    }
  }

	// Reconnaissance du format de la date si possible
	if (flag == Date || flag == DateTime) {
		if (Nb[0] > 1900) {
			format = _T("yyyy/mm/dd");
		}
		else if (Nb[2] > 1900) {
			format = _T("dd/mm/yyyy");
		}
		else {
			format = ShortDateFormat;
		}
	}

	// Format de l'heure
	if (flag == Time || flag == DateTime) {
		format += _T(" ");
		format += ShortTimeFormat;
	}

	// Décodage de la date (et éventuellement de l'heure)
	j = 0;
	FormatLength = (int) format.Length();
  for (i = 1; i <= FormatLength + 1 && j < NbNb; i++) {
    if (i <= FormatLength) c = format[i];
    else c = _T('\0');
    if (i > 0 && c != dc && !asMot.IsEmpty()) {
      if (asMot == _T("d") || asMot == _T("dd")) {
        FDay = Nb[j++];
      }
      else if (asMot == _T("m") || asMot == _T("mm")) {
        FMonth = Nb[j++];
      }
      else if (asMot == _T("yy") || asMot == _T("yyyy")) {
        FYear = Nb[j++];
      }
      else if (asMot == _T("h") || asMot == _T("hh")) {
        FHour = Nb[j++];
      }
      else if (asMot == _T("n") || asMot == _T("nn")) {
        FMin = Nb[j++];
      }
      else if (asMot == _T("s") || asMot == _T("ss")) {
        FSec = Nb[j++];
      }
      asMot = _T("");
    }
    if (_T('a') <= c && c <= _T('z')) {
      asMot += c;
    }
    dc = c;
  }

  // Si année sur deux chiffres => +2000
	if (FYear < 100) FYear += 2000;

  CodeDateTime();
}

TDateTime::TDateTime(unsigned short year, unsigned short month, unsigned short day) {
  FYear = year;
  FMonth = month;
  FDay = day;
  FHour = 00;
  FMin = 00;
  FSec = 00;
  FMSec = 00;
  CodeDateTime();
}

TDateTime::TDateTime(unsigned short hour, unsigned short min, unsigned short sec, unsigned short msec) {
  FYear = 1899;
  FMonth = 12;
  FDay = 30;
  FHour = hour;
  FMin = min;
  FSec = sec;
  FMSec = msec;
  CodeDateTime();
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Year
//---------------------------------------------------------------------------

unsigned short TDateTime::Get_Year(void) {
  return FYear;
}

bool TDateTime::Set_Year(unsigned short NewYear) {
  if (FYear != NewYear) {
    FYear = NewYear;
    CodeDateTime();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Month
//---------------------------------------------------------------------------

unsigned short TDateTime::Get_Month(void) {
  return FMonth;
}

bool TDateTime::Set_Month(unsigned short NewMonth) {
	if (FMonth != NewMonth) {
		FVCL_ASSERT(1 <= NewMonth && NewMonth <= 12);
    FMonth = NewMonth;
    CodeDateTime();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Day
//---------------------------------------------------------------------------

unsigned short TDateTime::Get_Day(void) {
  return FDay;
}

bool TDateTime::Set_Day(unsigned short NewDay) {
  if (FDay != NewDay) {
		FVCL_ASSERT(1 <= NewDay && NewDay <= DaysInMonth());
    FDay = NewDay;
    CodeDateTime();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Hour
//---------------------------------------------------------------------------

unsigned short TDateTime::Get_Hour(void) {
  return FHour;
}

bool TDateTime::Set_Hour(unsigned short NewHour) {
  if (FHour != NewHour) {
		FVCL_ASSERT(NewHour < 24);
    FHour = NewHour;
    CodeDateTime();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Min
//---------------------------------------------------------------------------

unsigned short TDateTime::Get_Min(void) {
  return FMin;
}

bool TDateTime::Set_Min(unsigned short NewMin) {
  if (FMin != NewMin) {
		FVCL_ASSERT(NewMin < 60);
    FMin = NewMin;
    CodeDateTime();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Sec
//---------------------------------------------------------------------------

unsigned short TDateTime::Get_Sec(void) {
  return FSec;
}

bool TDateTime::Set_Sec(unsigned short NewSec) {
  if (FSec != NewSec) {
		FVCL_ASSERT(NewSec < 60);
    FSec = NewSec;
    CodeDateTime();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MSec
//---------------------------------------------------------------------------

unsigned short TDateTime::Get_MSec(void) {
  return FMSec;
}

bool TDateTime::Set_MSec(unsigned short NewMSec) {
  if (FMSec != NewMSec) {
		FVCL_ASSERT(NewMSec < 1000);
    FMSec = NewMSec;
    CodeDateTime();
  }
  return true;
}


//---------------------------------------------------------------------------
TDateTime &TDateTime::CurrentDate(bool bLocalTime) {
  SYSTEMTIME SystemTimeToDay;

  if (bLocalTime) GetLocalTime(&SystemTimeToDay);
	else GetSystemTime(&SystemTimeToDay);

  FDay = SystemTimeToDay.wDay;
  FMonth = SystemTimeToDay.wMonth;
  FYear = SystemTimeToDay.wYear;
  CodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
TDateTime &TDateTime::CurrentTime(bool bLocalTime) {
  SYSTEMTIME SystemTimeToDay;

  if (bLocalTime) GetLocalTime(&SystemTimeToDay);
	else GetSystemTime(&SystemTimeToDay);

  FHour = SystemTimeToDay.wHour;
  FMin = SystemTimeToDay.wMinute;
  FSec = SystemTimeToDay.wSecond;
  FMSec = SystemTimeToDay.wMilliseconds;
  CodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
TDateTime &TDateTime::CurrentDateTime(bool bLocalTime) {
  SYSTEMTIME SystemTimeToDay;

  if (bLocalTime) GetLocalTime(&SystemTimeToDay);
	else GetSystemTime(&SystemTimeToDay);

  FDay = SystemTimeToDay.wDay;
  FMonth = SystemTimeToDay.wMonth;
  FYear = SystemTimeToDay.wYear;
	FHour = SystemTimeToDay.wHour;
  FMin = SystemTimeToDay.wMinute;
  FSec = SystemTimeToDay.wSecond;
  FMSec = SystemTimeToDay.wMilliseconds;
  CodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
AnsiString TDateTime::FormatString(const AnsiString &format) {
  AnsiString asRetour;
  AnsiString asMot;
	int FormatLength;
  AnsiString asBuf;
  TCHAR c, dc = 0;
  int i;
  bool bGuillemets = false;


	FormatLength = (int) format.Length();
  for (i = 1; i <= FormatLength + 1; i++) {
    if (i <= FormatLength) c = format[i];
    else c = _T('\0');
    if (i > 1 && c != dc && !asMot.IsEmpty() && !bGuillemets) {
      if (asMot == _T("yyyy")) {
        asBuf.sprintf(_T("%04i"), FYear);
        asRetour += asBuf;
      }
      else if (asMot == _T("yy")) {
        asBuf.sprintf(_T("%02i"), FYear % 100);
        asRetour += asBuf;
      }
      else if (asMot == _T("mmmm")) {
        switch (FMonth) {
          case 1: asRetour += _TT("janvier"); break;
          case 2: asRetour += _TT("février"); break;
          case 3: asRetour += _TT("mars"); break;
          case 4: asRetour += _TT("avril"); break;
          case 5: asRetour += _TT("mai"); break;
          case 6: asRetour += _TT("juin"); break;
          case 7: asRetour += _TT("juillet"); break;
          case 8: asRetour += _TT("août"); break;
          case 9: asRetour += _TT("septembre"); break;
          case 10: asRetour += _TT("octobre"); break;
          case 11: asRetour += _TT("novembre"); break;
          case 12: asRetour += _TT("décembre"); break;
        }
      }
      else if (asMot == _T("mm")) {
        asBuf.sprintf(_T("%02i"), FMonth);
        asRetour += asBuf;
      }
      else if (asMot == _T("dddd")) {
        switch (DayOfWeek()) {
          case 1: asRetour += _TT("dimanche"); break;
          case 2: asRetour += _TT("lundi"); break;
          case 3: asRetour += _TT("mardi"); break;
          case 4: asRetour += _TT("mercredi"); break;
          case 5: asRetour += _TT("jeudi"); break;
          case 6: asRetour += _TT("vendredi"); break;
          case 7: asRetour += _TT("samedi"); break;
        }
      }
      else if (asMot == _T("dd")) {
        asBuf.sprintf(_T("%02i"), FDay);
        asRetour += asBuf;
      }
      else if (asMot == _T("hh")) {
        asBuf.sprintf(_T("%02i"), FHour);
        asRetour += asBuf;
      }
      else if (asMot == _T("h")) {
        asBuf.sprintf(_T("%i"), FHour);
        asRetour += asBuf;
      }
      else if (asMot == _T("nn")) {
        asBuf.sprintf(_T("%02i"), FMin);
        asRetour += asBuf;
      }
      else if (asMot == _T("n")) {
        asBuf.sprintf(_T("%i"), FMin);
        asRetour += asBuf;
      }
      else if (asMot == _T("ss")) {
        asBuf.sprintf(_T("%02i"), FSec);
        asRetour += asBuf;
      }
      else if (asMot == _T("s")) {
        asBuf.sprintf(_T("%i"), FSec);
        asRetour += asBuf;
      }
      else if (asMot == _T("c")) {
        asRetour += FormatString(ShortDateFormat);
        asRetour += _T(" ");
        asRetour += FormatString(LongTimeFormat);
      }
      else if (asMot == _T("t")) {
        asRetour += FormatString(ShortTimeFormat);
      }
      else if (asMot == _T("tt")) {
        asRetour += FormatString(LongTimeFormat);
      }
      asMot = _T("");
    }
    if (_T('a') <= c && c <= _T('z') && !bGuillemets) {
      asMot += c;
    }
    else if (c == _T('/')) {
      asRetour += _T('/');
    }
    else if (c == _T(':')) {
      asRetour += _T(':');
    }
    else if (c == _T('\"') || c == _T('\'')) {
      bGuillemets = !bGuillemets;
    }
    else if (c != _T('\0')) {
      asRetour += c;
    }
    dc = c;
  }

  return asRetour;
}

//---------------------------------------------------------------------------
AnsiString TDateTime::DateString(void) {
  return FormatString(ShortDateFormat);
}

//---------------------------------------------------------------------------
void TDateTime::CodeDateTime(void) {
  unsigned short year0;
  int i;

  // Pour éviter trop de calculs: on part d'une date connue
  if (FYear < 1600) {
    // $$$ A compléter mais c'est compliqué
    year0 = 0;
  }
  else if (FYear < 2000) {
    TimeMS = DT_TIME1600;
    year0 = 1600;
  }
  else {
    TimeMS = DT_TIME2000;
    year0 = 2000;
  }

  if (FYear > 2100) {
    TimeMS = 0;
  }

  // Années
  for (i = year0; i < FYear; i++) {
    if (IsLeapYear(i)) TimeMS += DT_DELTALEAPYEAR;
    else TimeMS += DT_DELTAYEAR;
  }

  // Mois
  for (i = 0; i < FMonth - 1; i++) {
    TimeMS += DT_MSECINDAY * DaysInMonth(i + 1, FYear);
  }

  // Jours, heures, minutes, secondes
  TimeMS += DT_MSECINDAY * (FDay - 1);
  TimeMS += DT_MSECINHOUR * FHour;
  TimeMS += DT_MSECINMIN * FMin;
  TimeMS += 1000 * FSec;
  TimeMS += FMSec;

}

//---------------------------------------------------------------------------
void TDateTime::DecodeDateTime(void) {
  int DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  LongLong MSTime0;
  LongLong Delta;
  int i;

  MSTime0 = TimeMS;
  if (TimeMS < DT_TIME1600) {
    // $$$ A compléter mais c'est compliqué
  }
  else if (TimeMS < DT_TIME2000) {
    MSTime0 -= DT_TIME1600;
    FYear = 1600;
  }
  else {
    MSTime0 -= DT_TIME2000;
    FYear = 2000;
  }
  while (true) {
    if (IsLeapYear(FYear)) Delta = DT_DELTALEAPYEAR;
    else Delta = DT_DELTAYEAR;
    if (MSTime0 < Delta) break;
    MSTime0 -= Delta;
    FYear++;
  }

  FMonth = 1;
  for (i = 0; i < 12; i++) {
    Delta = DT_MSECINDAY * DaysInMonth[FMonth - 1];
    if (IsLeapYear(FYear) && i == 1) Delta += DT_MSECINDAY;
    if (MSTime0 < Delta) break;
    MSTime0 -= Delta;
    FMonth++;
  }

  FDay = (unsigned short) (MSTime0 / DT_MSECINDAY + 1);
  MSTime0 %= DT_MSECINDAY;
  FHour = (unsigned short) (MSTime0 / DT_MSECINHOUR);
  MSTime0 %= DT_MSECINHOUR;
  FMin = (unsigned short) (MSTime0 / DT_MSECINMIN);
  MSTime0 %= DT_MSECINMIN;
  FSec = (unsigned short) (MSTime0 / 1000);
  FMSec = (unsigned short) (MSTime0 % 1000);
}

//---------------------------------------------------------------------------
void TDateTime::DecodeDate(unsigned short *year,
                           unsigned short *month,
                           unsigned short *day) const {
  *year = FYear;
  *month = FMonth;
  *day = FDay;
}

//---------------------------------------------------------------------------
void TDateTime::DecodeTime(unsigned short *hour,
                           unsigned short *min,
                           unsigned short *sec,
                           unsigned short *msec) const {
  *hour = FHour;
  *min = FMin;
  *sec = FSec;
  *msec = FMSec;
}

//---------------------------------------------------------------------------
bool TDateTime::operator ==(TDateTime Date) {
  return (TimeMS == Date.TimeMS);
}

//---------------------------------------------------------------------------
bool TDateTime::operator !=(TDateTime Date) {
  return (TimeMS != Date.TimeMS);
}

//---------------------------------------------------------------------------
bool TDateTime::operator <(TDateTime Date) {
  return (TimeMS < Date.TimeMS);
}

//---------------------------------------------------------------------------
bool TDateTime::operator >(TDateTime Date) {
  return (TimeMS > Date.TimeMS);
}

//---------------------------------------------------------------------------
bool TDateTime::operator <=(TDateTime Date) {
  return (TimeMS <= Date.TimeMS);
}

//---------------------------------------------------------------------------
bool TDateTime::operator >=(TDateTime Date) {
  return (TimeMS >= Date.TimeMS);
}

//---------------------------------------------------------------------------
TDateTime& TDateTime::operator =(AnsiString asDate) {
	operator =(TDateTime(asDate, DateTime));

  return *this;
}

//---------------------------------------------------------------------------
TDateTime& TDateTime::operator =(const TDateTimeBase& src) {
	TimeMS = src.TimeMS;
  DecodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
TDateTime& TDateTime::operator =(const TDateTime& src) {
  TimeMS = src.TimeMS;
  FYear = src.FYear;
  FMonth = src.FMonth;
  FDay = src.FDay;
  FHour = src.FHour;
  FMin = src.FMin;
  FSec = src.FSec;
  FMSec = src.FMSec;

  return *this;
}

//---------------------------------------------------------------------------
TDateTime& TDateTime::operator =(const double src) {
	TimeMS = (LongLong) (src * DT_MSECINDAY);
  DecodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
TDateTime& TDateTime::operator =(int src) {
  TimeMS = src * DT_MSECINDAY;
  DecodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
TDateTime& TDateTime::operator =(const time_t src) {
  TimeMS = (LongLong) src * 1000 + DT_TIME1970;
  DecodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
TDateTime& TDateTime::operator =(const FILETIME src) {
	__int64 FileDateTime;  // 1/10000 de millisecondes depuis le 01/01/1601
	FileDateTime = ((__int64) src.dwHighDateTime << 32) + src.dwLowDateTime;
  TimeMS = DT_TIME1600 + DT_DELTALEAPYEAR + FileDateTime / 10000;  // 1600 était bissextile
  DecodeDateTime();

  return *this;
}

//---------------------------------------------------------------------------
// Ajout de deux dates
//---------------------------------------------------------------------------

TDateTime FASTCALL TDateTime::operator +(const TDateTime& RValue) const {
  TDateTime LValue(*this);
  LValue += RValue;
  return LValue;
}

//---------------------------------------------------------------------------
// Ajout d'un nombre de jours donné
//---------------------------------------------------------------------------

TDateTime FASTCALL TDateTime::operator +(const double RValue) const {
  TDateTime LValue(*this);
  LValue += RValue;
  return LValue;
}

TDateTime FASTCALL TDateTime::operator +(int RValue) const {
  TDateTime LValue(*this);
  LValue += RValue;
  return LValue;
}

//---------------------------------------------------------------------------
// Soustraction entre deux dates
//---------------------------------------------------------------------------

TDateTime FASTCALL TDateTime::operator -(const TDateTime& RValue) const {
  TDateTime LValue(*this);
  LValue -= RValue;
  return LValue;
}

//---------------------------------------------------------------------------
// Soustraction d'un nombre de jours donné
//---------------------------------------------------------------------------

TDateTime FASTCALL TDateTime::operator -(const double RValue) const {
  TDateTime LValue(*this);
  LValue -= RValue;
  return LValue;
}

TDateTime FASTCALL TDateTime::operator -(int RValue) {
  TDateTime LValue(*this);
  LValue -= RValue;
  return LValue;
}

//---------------------------------------------------------------------------
TDateTime& FASTCALL TDateTime::operator +=(const TDateTime& RValue) {
  TimeMS += RValue.TimeMS;
  DecodeDateTime();
  return *this;
}

//---------------------------------------------------------------------------
TDateTime& FASTCALL TDateTime::operator +=(const double RValue) {
  TimeMS += (LongLong) (RValue * DT_MSECINDAY);
  DecodeDateTime();
  return *this;
}

//---------------------------------------------------------------------------
TDateTime& FASTCALL TDateTime::operator +=(int RValue) {
  TimeMS += RValue * DT_MSECINDAY;
  DecodeDateTime();
  return *this;
}

//---------------------------------------------------------------------------
TDateTime& FASTCALL TDateTime::operator -=(const TDateTime& RValue) {
  TimeMS -= RValue.TimeMS;
  DecodeDateTime();
  return *this;
}

//---------------------------------------------------------------------------
TDateTime& FASTCALL TDateTime::operator -=(const double RValue) {
  TimeMS -= (LongLong) (RValue * DT_MSECINDAY);
  DecodeDateTime();
  return *this;
}

//---------------------------------------------------------------------------
TDateTime& FASTCALL TDateTime::operator -=(int RValue) {
  TimeMS -= RValue * DT_MSECINDAY;
  DecodeDateTime();
  return *this;
}

//---------------------------------------------------------------------------
void FASTCALL TDateTime::IncYear(int ANumberOfYears) {
  FYear += ANumberOfYears;
  CodeDateTime();
}

//---------------------------------------------------------------------------
void FASTCALL TDateTime::IncMonth(int ANumberOfMonthes) {
  FMonth += ANumberOfMonthes;
	while (FMonth > 12) {
		FMonth -= 12;
		FYear++;
	}
	while (FMonth < 1) {
		FMonth += 12;
		FYear--;
	}
  CodeDateTime();
}

//---------------------------------------------------------------------------
void FASTCALL TDateTime::IncWeek(int ANumberOfWeeks) {
  TimeMS += ANumberOfWeeks * DT_MSECINWEEK;
  DecodeDateTime();
}

//---------------------------------------------------------------------------
void FASTCALL TDateTime::IncDay(int ANumberOfDays) {
  TimeMS += ANumberOfDays * DT_MSECINDAY;
  DecodeDateTime();
}

//---------------------------------------------------------------------------
void FASTCALL TDateTime::IncHour(__int64 ANumberOfHours) {
  TimeMS += ANumberOfHours * DT_MSECINHOUR;
  DecodeDateTime();
}

//---------------------------------------------------------------------------
void FASTCALL TDateTime::IncSecond(__int64 ANumberOfSeconds) {
  TimeMS += ANumberOfSeconds * 1000;
  DecodeDateTime();
}

//---------------------------------------------------------------------------
void FASTCALL TDateTime::IncMilliSecond(__int64 ANumberOfMilliSeconds) {
  TimeMS += ANumberOfMilliSeconds;
  DecodeDateTime();
}

//---------------------------------------------------------------------------
int TDateTime::DayOfWeek() const {
	int Day;

	Day = (TimeMS / DT_MSECINDAY) % 7;
	if (Day == 0) Day = 7;

	return Day;
  /*
  static int DM[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
  int DOW;

  if (FYear < 1900) DOW = 5 * ((FYear - 1860) / 4);
  else DOW = 5 * ((FYear - 1900) / 4);
  DOW += FYear % 4;
  DOW += DM[FMonth - 1];
  DOW += FDay;
  if (FYear % 4 == 0 && FMonth <= 2) DOW += 6;
  DOW = DOW % 7 + 1;

  return DOW;
  */
}

//---------------------------------------------------------------------------
// Détermine les années bissextiles
//---------------------------------------------------------------------------

bool FASTCALL TDateTime::IsLeapYear(void) const {
  return IsLeapYear(FYear);
}

//---------------------------------------------------------------------------
// Détermine les années bissextiles
//---------------------------------------------------------------------------

bool FASTCALL TDateTime::IsLeapYear(unsigned short year) const {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

//---------------------------------------------------------------------------
// Calcule le nombre de jours du mois
//---------------------------------------------------------------------------

int FASTCALL TDateTime::DaysInMonth(void) const {
  return DaysInMonth(FMonth, FYear);
}

//---------------------------------------------------------------------------
// Calcule le nombre de jours d'un mois d'une année
//---------------------------------------------------------------------------

int FASTCALL TDateTime::DaysInMonth(unsigned short month, unsigned short year) const {
  static int DIM[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	int Ret = DIM[month - 1];
	if (month == 2 && IsLeapYear(year)) Ret++;  // 29 février

	return Ret;
}

//---------------------------------------------------------------------------
// Fonctions globales
//---------------------------------------------------------------------------

TDateTime FASTCALL IncSecond(const TDateTime &AValue, __int64 ANumberOfSeconds) {
	TDateTime dtRet(AValue);
	dtRet.IncSecond(ANumberOfSeconds);

	return dtRet;
}


//---------------------------------------------------------------------------

