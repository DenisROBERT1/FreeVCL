//-----------------------------------------------------------------------------
//! @file TDateTimePicker.h
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

#ifdef In_TDateTimePickerH
#error "Inclusion circulaire de TDateTimePicker"
#endif // In_TDateTimePickerH

#define In_TDateTimePickerH

#ifndef TDateTimePickerH
#define TDateTimePickerH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TCommonCalendarH
#include "TCommonCalendar.h"
#endif // TCommonCalendarH

#ifndef TDateTimeH
#include "TDateTime.h"
#endif // TDateTimeH


//-----------------------------------------------------------------------------
//! @class TDateTimePicker
//!
//! @brief Contrôle de sélection d'une date
//! @image html TDateTimePicker.jpg "TDateTimePicker"
//!
//! Cette classe affiche un contrôle permettant de choisir une date, avec une
//! fenêtre popup contenant un calendrier où l'utilisateur peut faire défiler
//! les mois et les années, puis choisir une date. La date peut également être
//! entrée au clavier.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDateTimePicker: public TCommonCalendar {
private:
protected:
  TDateTime FDate;

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

  TDateTimePicker(TComponent* AOwner);

  //@}


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDateTimePicker(void);

  //! @name Méthodes
  //@{

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
  //! @brief Propriété Text
	//!
	//! Cette propriété permet de lire la date entrée par l'utilisateur. Elle est
	//! au format défini par les paramètres locaux du système.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTimePicker, AnsiString, Text);


  //---------------------------------------------------------------------------
  //! @brief Propriété Date
	//!
	//! Cette propriété permet de lire la date entrée par l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDateTimePicker, TDateTime, Date);


  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque l'objet est modifié, soit par une
	//! action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //@}


};


#else  // TDateTimePickerH

class TDateTimePicker;

#endif  // TDateTimePickerH


#undef In_TDateTimePickerH
