//-----------------------------------------------------------------------------
//! @file TCommonCalendar.h
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

#ifdef In_TCommonCalendarH
#error "Inclusion circulaire de TCommonCalendar"
#endif // In_TCommonCalendarH

#define In_TCommonCalendarH

#ifndef TCommonCalendarH
#define TCommonCalendarH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#ifndef TDateTimeH
#include "TDateTime.h"
#endif // TDateTimeH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TCalDayOfWeek {
	dowMonday,										//!< Lundi
	dowTuesday,										//!< Mardi
	dowWednesday,									//!< Mercredi
	dowThursday,									//!< Jeudi
	dowFriday,										//!< Vendredi
	dowSaturday,									//!< Samedi
	dowSunday,										//!< Dimanche
	dowLocaleDefault							//!< Jour défini par les options régionales
};


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnGetMonthInfo.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnGetMonthInfo)(TObject *Sender, int NbMonths, MONTHDAYSTATE *State);


//-----------------------------------------------------------------------------
//! @class TCommonCalendar
//!
//! @brief classe de base des contrôles de date
//!
//! Cette classe est la classe de base des contrôles TDateTimePicker et
//! TMonthCalendar qui permettent d'afficher un calendrier où l'utilisateur
//! peut choisir une ou plusieurs dates.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCommonCalendar: public TWinControl {
private:
protected:
  TMonthCalColors* FCalColors;
  TDateTime FDate;
  TDateTime FEndDate;
  TCalDayOfWeek FFirstDayOfWeek;
  TDateTime FMaxDate;
  int FMaxSelectRange;
  TDateTime FMinDate;
  bool FMultiSelect;
  bool FShowToday;
  bool FShowTodayCircle;
  bool FWeekNumbers;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual int ProcessNotify(LPNMHDR pnmh);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCommonCalendar(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCommonCalendar(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Code un ensemble de dates pour évènement OnGetMonthInfo.
  //!
	//! Cette méthode code un ensemble de dates pour l'utiliser dans le
	//! gestionnaire d'évènement OnGetMonthInfo.
	//!
  //! @param[in]        Days Tableau de jours à mettre en gras
  //! @param[in]        Days_Size Taille du tableau MOINS UN
	//!                   (c'est à dire indice du dernier élément)
  //! @param[out]       MonthBoldInfo Valeur à renvoyer à l'évènement
  //---------------------------------------------------------------------------
	void FASTCALL BoldDays(const unsigned * Days, int Days_Size, unsigned long &MonthBoldInfo);

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété CalColors: Propriété CalColors: couleurs du calendrier.
  //! 
  //! Cette propriété détermine les couleurs du calendrier (textes, titre,
	//! fond). Utilisez les propriétés de l'objet retourné pour paramétrer
	//! chaque couleur.
  //! 
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, TMonthCalColors*, CalColors);


  //---------------------------------------------------------------------------
  //! @brief Propriété Date: Date sélectionnée
  //! 
  //! Cette propriété permet de lire la date sélectionnée par l'utilisateur, ou
	//! bien d'en initialiser une avant l'ouverture du composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, TDateTime, Date);


  //---------------------------------------------------------------------------
  //! @brief Propriété EndDate: Date de fin de sélection
  //! 
  //! Cette propriété permet de lire ou de déterminer la borne supérieure des
	//! dates sélectionnées.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, TDateTime, EndDate);


  //---------------------------------------------------------------------------
  //! @brief Propriété FirstDayOfWeek: Premier jour de la semaine
  //! 
  //! Cette propriété permet de déterminer le premier jour de la semaine. Le
	//! premier jour est généralement le dimanche dans les pays anglo-saxons et
	//! le lundi dans les autres pays.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, TCalDayOfWeek, FirstDayOfWeek);


  //---------------------------------------------------------------------------
  //! @brief Propriété MaxDate: Date maximum que l'utilisateur peut afficher et sélectionner.
  //! 
  //! Cette propriété permet de déterminer une borne supérieure au delà de
	//! laquelle l'utilisateur ne pourra plus faire défiler le calendrier, ni
	//! sélectionner de date.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, TDateTime, MaxDate);


  //---------------------------------------------------------------------------
  //! @brief Propriété MaxSelectRange: Nombre maximum de jours que peut sélectionner l'utilisateur.
  //! 
  //! Cette propriété permet de déterminer le nombre maximum de dates que peut
	//! sélectionner l'utilisateur, si la propriété @b MultiSelect est à @b true.
	//! Sinon, on ne peut sélectionner qu'une seule date.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, int, MaxSelectRange);


  //---------------------------------------------------------------------------
  //! @brief Propriété MinDate: Date minimum que l'utilisateur peut afficher et sélectionner.
  //! 
  //! Cette propriété permet de déterminer une borne inférieure avant laquelle
	//! l'utilisateur ne pourra plus faire défiler le calendrier, ni sélectionner
	//! de date.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, TDateTime, MinDate);


  //---------------------------------------------------------------------------
  //! @brief Propriété MultiSelect: Multi-sélection
  //! 
  //! Cette propriété permet à l'utilisateur de choisir plusieurs dates.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, bool, MultiSelect);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowToday: Propriété ShowToday: affichage de la date d'aujourd'hui
  //! 
  //! Cette propriété permet d'afficher ou de cacher la partie "aujourd'hui" en
	//! bas du cadre du mois.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, bool, ShowToday);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowTodayCircle: entourage de la date d'aujourd'hui
  //! 
  //! Cette propriété permet d'afficher ou de cacher le cercle autour de la
	//! date d'aujourd'hui dans le cadre du mois.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, bool, ShowTodayCircle);


  //---------------------------------------------------------------------------
  //! @brief Propriété WeekNumbers: affichage du numéro de semaine. 
  //! 
  //! Cette propriété permet d'afficher ou de cacher le numéro de semaine à
	//! gauche du calendrier. La première semaine de l'année a le numéro un.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCommonCalendar, bool, WeekNumbers);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnGetMonthInfo: affichage d'un mois
  //! 
  //! Cet évènement est déclenché avant l'affichage d'un mois. L'interception
	//! de cet évènement permet d'initialiser l'affichage en gras de certains
	//! jours (les jours fériés par exemple). Chaque mois est défini par un
	//! paramètre DWORD (32 bits), chaque jour du mois pouvant être mis en gras
	//! en positionnant le bit correspondant (celui de plus faible valeur
	//! correspondant au premier jour du mois). Les bits supérieurs au nombre de
	//! jours dans le mois sont ignorés.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nombre de mois à définir (d'éléments dans le tableau Arg3)
  //! @param[out]       Arg3 Tableau de valeurs (une pour chaque mois)
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnGetMonthInfo, OnGetMonthInfo, TObject *, int, MONTHDAYSTATE *);

  //@}


};


#else  // TCommonCalendarH

class TCommonCalendar;

#endif  // TCommonCalendarH


#undef In_TCommonCalendarH
