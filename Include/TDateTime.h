//-----------------------------------------------------------------------------
//! @file TDateTime.h
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


//-----------------------------------------------------------------------------
// Dispositif anti-redéfinition
//-----------------------------------------------------------------------------

#ifdef In_TDateTimeH
#error "Inclusion circulaire de TDateTime"
#endif // In_TDateTimeH

#define In_TDateTimeH

#ifndef TDateTimeH
#define TDateTimeH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Enums et autres définitions
//---------------------------------------------------------------------------

enum TDateTimeFlag {
	Date,                         //!< Date
	Time,                         //!< Heure
	DateTime                      //!< Date et heure
};

#define DT_MSECINMIN ((LongLong) (60 * 1000))
#define DT_SECINMIN ((LongLong) (60))

#define DT_MSECINHOUR ((LongLong) (60 * 60 * 1000))
#define DT_SECINHOUR ((LongLong) (60 * 60))
#define DT_MININHOUR ((LongLong) (60))

#define DT_MSECINDAY ((LongLong) (24 * 60 * 60 * 1000))
#define DT_SECINDAY ((LongLong) (24 * 60 * 60))
#define DT_MININDAY ((LongLong) (24 * 60))
#define DT_HOURINDAY ((LongLong) (24))

#define DT_MSECINWEEK ((LongLong) (7 * 24 * 60 * 60 * 1000))
#define DT_SECINWEEK ((LongLong) (7 * 24 * 60 * 60))
#define DT_MININWEEK ((LongLong) (7 * 24 * 60))
#define DT_HOURINWEEK ((LongLong) (7 * 24))

// Années bissextiles
// 1904
// 1908
// 1912
// 1916
// 1920
// 1924
// 1928
// 1932
// 1936
// 1940
// 1944
// 1948
// 1952
// 1956
// 1960
// 1964
// 1968
// 1972
// 1976
// 1980
// 1984
// 1988
// 1992
// 1996

#define DT_DELTA400 ((LongLong) (400 * 365 + 97) * DT_MSECINDAY)  // 400 ans * 365 jours + 97 jours des années bissextiles
#define DT_DELTALEAPYEAR ((LongLong) 366 * DT_MSECINDAY)  // 1 année bissextile
#define DT_DELTAYEAR ((LongLong) 365 * DT_MSECINDAY)  // 1 année normale

#define DT_TIME1900 ((LongLong) DT_MSECINDAY + 12 * DT_MSECINHOUR)  // 1 jour (le zéro est le 30/12) + 12 heures
#define DT_TIME1970 ((LongLong) (70 * 365 + 18 + 1) * DT_MSECINDAY + 12 * DT_MSECINHOUR)  // 70 ans depuis 1899 * 365 jours + 18 jours des années bissextiles + 1 jour (le zéro est le 30/12) + 12 heures
#define DT_TIME2000 ((LongLong) (100 * 365 + 24 + 1) * DT_MSECINDAY + 12 * DT_MSECINHOUR)  // 100 ans depuis 1899 * 365 jours + 24 jours des années bissextiles + 1 jour (le zéro est le 30/12) + 12 heures
#define DT_TIME1600 (DT_TIME2000 - DT_DELTA400)


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

extern AnsiString ShortDateFormat;
extern AnsiString LongDateFormat;
extern AnsiString ShortTimeFormat;
extern AnsiString LongTimeFormat;


//---------------------------------------------------------------------------
//! @class TDateTimeBase
//!
//! @brief Classe de base de la classe TDateTime
//!
//---------------------------------------------------------------------------

class TDateTimeBase {
public:
  LongLong TimeMS;  // millisecondes depuis le 30/12/1899 à 12h (pourquoi cette date ?)
};


//-----------------------------------------------------------------------------
//! @class TDateTime
//!
//! @brief Définition de la classe TDateTime
//!
//! Cette classe représente une date et une heure. Elle contient des méthodes
//! de conversion, d'ajout et de suppression de durées, entre autres.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDateTime: protected TDateTimeBase {
private:
  void CodeDateTime(void);  // Calcule TimeMS à partir de FYear, FMonth, ...
  void DecodeDateTime(void);  // Calcule FYear, FMonth, ..., à partir de TimeMS
protected:
  unsigned short FYear;
  unsigned short FMonth;  // Janvier = 1
  unsigned short FDay;  // Premier jour du mois = 1
  unsigned short FHour;
  unsigned short FMin;
  unsigned short FSec;
  unsigned short FMSec;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TDateTime(void);
  TDateTime(const TDateTimeBase& src);
  TDateTime(const TDateTime& src);
  TDateTime(const double src);
  TDateTime(int src);
  TDateTime(const time_t src);
	TDateTime(const FILETIME src);
  TDateTime(const AnsiString& src, TDateTimeFlag flag = DateTime);
  TDateTime(unsigned short year, unsigned short month, unsigned short day);
  TDateTime(unsigned short hour, unsigned short min, unsigned short sec, unsigned short msec);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Date courante
  //!
  //! Cette méthode renvoie la date du jour.
  //!
  //! @return @c TDateTime Date du jour
  //---------------------------------------------------------------------------

  TDateTime &CurrentDate(bool bLocalTime = true);

  //---------------------------------------------------------------------------
  //! @brief Heure courante
  //!
  //! Cette méthode renvoie l'heure courante.
  //!
  //! @return @c TDateTime Heure
  //---------------------------------------------------------------------------

  TDateTime &CurrentTime(bool bLocalTime = true);

  //---------------------------------------------------------------------------
  //! @brief Date et heure courante
  //!
  //! Cette méthode renvoie la date et l'heure du jour.
  //!
  //! @return @c TDateTime Date et l'heure  du jour
  //---------------------------------------------------------------------------

  TDateTime &CurrentDateTime(bool bLocalTime = true);

  //---------------------------------------------------------------------------
  //! @brief Jour de la semaine
  //!
  //! Cette méthode renvoie le jour de la semaine (1 = dimanche, 2 = lundi, ...)
  //!
  //! @return @c int Numéro du jour (entre 1 et 7)
  //---------------------------------------------------------------------------

  int DayOfWeek() const;

  //---------------------------------------------------------------------------
  //! @brief Date formatée
  //!
  //! Cette méthode renvoie la date selon un format.
  //!
  //! @return @c AnsiString Date formatée
  //---------------------------------------------------------------------------

  AnsiString FormatString(const AnsiString& format);

  //---------------------------------------------------------------------------
  //! @brief Date formatée
  //!
  //! Cette méthode renvoie la date selon le format ShortDateFormat. Ce format
	//! est une variable globale qui est initialisée avec les paramètres du
	//! panneau de configuration (options régionales). Vous pouvez modifier cette
	//! variable pour formater les dates selon un autre format que vous aurez
	//! choisi.
  //!
  //! @return @c AnsiString Date formatée
  //---------------------------------------------------------------------------

  AnsiString DateString();

  //---------------------------------------------------------------------------
  //! @brief Date en chiffres
  //!
  //! Cette méthode renvoie la date en chiffres.
  //---------------------------------------------------------------------------

  void DecodeDate(unsigned short *year,
                  unsigned short *month,
                  unsigned short *day) const;


  //---------------------------------------------------------------------------
  //! @brief Heure en chiffres
  //!
  //! Cette méthode renvoie l'heure en chiffres.
  //---------------------------------------------------------------------------

  void DecodeTime(unsigned short *hour,
                  unsigned short *min,
                  unsigned short *sec,
                  unsigned short *msec) const;


  //---------------------------------------------------------------------------
  //! @brief Opérateur d'égalité
  //!
  //! Cette méthode teste l'égalité avec Date.
  //---------------------------------------------------------------------------

  bool operator ==(TDateTime Date);

  //---------------------------------------------------------------------------
  //! @brief Opérateur d'inégalité
  //!
  //! Cette méthode teste l'inégalité avec Date.
  //---------------------------------------------------------------------------

  bool operator !=(TDateTime Date);

  //---------------------------------------------------------------------------
  //! @brief Opérateur d'infériorité stricte
  //!
  //! Cette méthode teste l'infériorité stricte avec Date.
  //---------------------------------------------------------------------------

  bool operator <(TDateTime Date);

  //---------------------------------------------------------------------------
  //! @brief Opérateur d'infériorité
  //!
  //! Cette méthode teste l'infériorité avec Date.
  //---------------------------------------------------------------------------

  bool operator <=(TDateTime Date);

  //---------------------------------------------------------------------------
  //! @brief Opérateur de supériorité stricte
  //!
  //! Cette méthode teste la supériorité stricte avec Date.
  //---------------------------------------------------------------------------

  bool operator >(TDateTime Date);

  //---------------------------------------------------------------------------
  //! @brief Opérateur de supériorité
  //!
  //! Cette méthode teste la supériorité avec Date.
  //---------------------------------------------------------------------------

  bool operator >=(TDateTime Date);

  //---------------------------------------------------------------------------
  //! @brief Affectation d'une date à partir d'un texte
  //!
  //! Cette méthode permet d'affecter une date à partir d'un texte. Le texte
	//! peut être sous différentes formes suivant les paramètres régionaux (par
	//! exemple jj/mm/aaaa ou aaaa/mm/jj). Si le texte a une forme non ambigüe
	//! (année sur quatre chiffres), le format correct est automatiquement
	//! choisi. Sinon, le format du texte doit correspondre au format des dates
	//! défini par la variable globale ShortDateFormat. Si l'année est sur deux
	//! chiffres uniquement, cette méthode ajoute automatiquement 2000 pour avoir
	//! l'année sur quatre chiffres.
  //---------------------------------------------------------------------------

  TDateTime& operator =(AnsiString sDate);

  //---------------------------------------------------------------------------
  //! @brief Affectation d'une date à partir de src
  //---------------------------------------------------------------------------

	TDateTime& operator =(const TDateTimeBase& src);

	//---------------------------------------------------------------------------
  //! @brief Affectation d'une date à partir de src
  //---------------------------------------------------------------------------

  TDateTime& operator =(const TDateTime& src);

	//---------------------------------------------------------------------------
  //! @brief Affectation d'une date à partir de src
  //---------------------------------------------------------------------------

  TDateTime& operator =(const double src);

	//---------------------------------------------------------------------------
  //! @brief Affectation d'une date à partir de src
  //---------------------------------------------------------------------------

  TDateTime& operator =(int src);

  //---------------------------------------------------------------------------
  //! @brief Affectation d'une date à partir de src
  //---------------------------------------------------------------------------

	TDateTime& operator =(const time_t src);

	//---------------------------------------------------------------------------
  //! @brief Affectation d'une date à partir de src
  //---------------------------------------------------------------------------

	TDateTime& operator =(const FILETIME src);

  //---------------------------------------------------------------------------
  //! @brief Ajout de deux dates
  //!
  //! Cette méthode ajoute deux dates (généralement une date et un intervalle).
  //---------------------------------------------------------------------------

  TDateTime FASTCALL operator +(const TDateTimeBase& RValue) const;
  TDateTime FASTCALL operator +(const TDateTime& RValue) const;

  //---------------------------------------------------------------------------
  //! @brief Ajout d'un nombre de jours donné
  //!
  //! Ces méthodes ajoutent un nombre n de jours à la date.
  //---------------------------------------------------------------------------

  TDateTime FASTCALL operator +(const double RValue) const;
  TDateTime FASTCALL operator +(int RValue) const;

  //---------------------------------------------------------------------------
  //! @brief Soustraction entre deux dates
  //!
  //! Cette méthode fait la soustraction entre deux dates. Le résultat est donc
	//! un intervalle de temps.
  //---------------------------------------------------------------------------

  TDateTime FASTCALL operator -(const TDateTimeBase& RValue) const;
  TDateTime FASTCALL operator -(const TDateTime& RValue) const;

  //---------------------------------------------------------------------------
  //! @brief Soustraction d'un nombre de jours donné
  //!
  //! Ces méthodes enlèvent un nombre n de jours à la date.
  //---------------------------------------------------------------------------

  TDateTime FASTCALL operator -(const double RValue) const;
  TDateTime FASTCALL operator -(int RValue);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'un nombre de jours donné
  //!
  //! Cette méthode ajoute un nombre n de jours à la date.
  //---------------------------------------------------------------------------

  TDateTime& FASTCALL operator +=(const TDateTimeBase& rhs);
  TDateTime& FASTCALL operator +=(const TDateTime& rhs);
  TDateTime& FASTCALL operator +=(const double rhs);
  TDateTime& FASTCALL operator +=(int rhs);

  //---------------------------------------------------------------------------
  //! @brief Soustraction d'un nombre de jours donné
  //!
  //! Cette méthode enlève un nombre n de jours à la date.
  //---------------------------------------------------------------------------

  TDateTime& FASTCALL operator -=(const TDateTimeBase& rhs);
  TDateTime& FASTCALL operator -=(const TDateTime& rhs);
  TDateTime& FASTCALL operator -=(const double rhs);
  TDateTime& FASTCALL operator -=(int rhs);

  //---------------------------------------------------------------------------
  //! @brief Incrémentation d'un nombre d'années
  //!
  //! Cette méthode ajoute un nombre n d'années à la date.
  //---------------------------------------------------------------------------

	void FASTCALL IncYear(int ANumberOfYears = 1);
	void FASTCALL IncMonth(int ANumberOfMonthes = 1);
	void FASTCALL IncWeek(int ANumberOfWeeks = 1);
	void FASTCALL IncDay(int ANumberOfDays = 1);
	void FASTCALL IncHour(__int64 ANumberOfHours = 1);
	void FASTCALL IncSecond(__int64 ANumberOfSeconds = 1);
	void FASTCALL IncMilliSecond(__int64 ANumberOfMilliSeconds = 1);

  //---------------------------------------------------------------------------
  //! @brief Détermine les années bissextiles
  //!
  //! Cette méthode renvoie true si l'année est bissextile.
  //---------------------------------------------------------------------------

  bool FASTCALL IsLeapYear(void) const;

  //---------------------------------------------------------------------------
  //! @brief Détermine les années bissextiles
  //!
  //! Cette méthode renvoie true si l'année passée en argument est bissextile.
  //---------------------------------------------------------------------------

  bool FASTCALL IsLeapYear(unsigned short year) const;

  //---------------------------------------------------------------------------
  //! @brief Calcule le nombre de jours du mois
  //!
  //! Cette méthode renvoie le nombre de jours du mois.
  //---------------------------------------------------------------------------

  int FASTCALL DaysInMonth(void) const;

  //---------------------------------------------------------------------------
  //! @brief Calcule le nombre de jours d'un mois
  //!
  //! Cette méthode renvoie le nombre de jours d'un mois d'une année.
  //---------------------------------------------------------------------------

  int FASTCALL DaysInMonth(unsigned short month, unsigned short year) const;

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Year
	//!
	//! Cette propriété représente l'année de la date.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTime, unsigned short, Year);

  //---------------------------------------------------------------------------
  //! @brief Propriété Month
	//!
	//! Cette propriété représente le mois de la date.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTime, unsigned short, Month);

  //---------------------------------------------------------------------------
  //! @brief Propriété Day
	//!
	//! Cette propriété représente le jour de la date.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTime, unsigned short, Day);

  //---------------------------------------------------------------------------
  //! @brief Propriété Hour
	//!
	//! Cette propriété représente l'heure.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTime, unsigned short, Hour);

  //---------------------------------------------------------------------------
  //! @brief Propriété Min
	//!
	//! Cette propriété représente les minutes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTime, unsigned short, Min);

  //---------------------------------------------------------------------------
  //! @brief Propriété Sec
	//!
	//! Cette propriété représente les secondes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTime, unsigned short, Sec);

  //---------------------------------------------------------------------------
  //! @brief Propriété MSec
	//!
	//! Cette propriété représente les millisecondes.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTime, unsigned short, MSec);

  //@}

};


//---------------------------------------------------------------------------
// Fonctions globales
//---------------------------------------------------------------------------

TDateTime FASTCALL IncYear(const TDateTime &AValue, int ANumberOfYears = 1);
TDateTime FASTCALL IncMonth(const TDateTime &AValue, int ANumberOfMonthes = 1);
TDateTime FASTCALL IncWeek(const TDateTime &AValue, int ANumberOfWeeks = 1);
TDateTime FASTCALL IncDay(const TDateTime &AValue, int ANumberOfDays = 1);
TDateTime FASTCALL IncHour(const TDateTime &AValue, __int64 ANumberOfHours = 1);
TDateTime FASTCALL IncSecond(const TDateTime &AValue, __int64 ANumberOfSeconds = 1);
TDateTime FASTCALL IncMilliSecond(const TDateTime &AValue, __int64 ANumberOfMilliSeconds = 1);


#else  // TDateTimeH

class TDateTime;

#endif  // TDateTimeH

#undef In_TDateTimeH
