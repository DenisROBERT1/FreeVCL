//-----------------------------------------------------------------------------
//! @file TTimer.h
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

#ifdef In_TTimerH
#error "Inclusion circulaire de TTimer"
#endif // In_TTimerH

#define In_TTimerH

#ifndef TTimerH
#define TTimerH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//-----------------------------------------------------------------------------
//! @class TTimer
//!
//! @brief Définition de la classe TTimer
//!
//! Cette classe définit une temporisation. Elle permet de recevoir à
//! intervalle régulier un évènement que vous pourrez traiter pour effectuer un
//! traitement particulier.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTimer: public TComponent {
private:
  HWND Handle;

protected:
  bool FEnabled;
  int FInterval;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessTimer(UINT IdTimer);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTimer(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTimer(void);

  //@}


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
  //! @brief Propriété Enabled
	//!
	//! Cette propriété permet de savoir si le timer est actif. Le fait de
	//! changer cette propriété démarre ou arrête le timer.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTimer, bool, Enabled);

  //---------------------------------------------------------------------------
  //! @brief Propriété Interval
	//!
	//! Cette propriété définit l'intervalle de temps entre deux évènements
	//! déclenchés par le timer. Si le timer était actif au moment de
	//! l'affectation de cette propriété, le système attendra ensuite un
	//! intervalle complet quel que soit le temps qu'il restait à écouler au
	//! moment du changement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTimer, int, Interval);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnTimer
	//!
	//! Cet évènement est déclenché lorsque l'intervalle défini pour le timer
	//! s'est écoulé.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnTimer, TObject *);

  //@}


  //---------------------------------------------------------------------------
  //! @brief Procédure de fenêtre
  //---------------------------------------------------------------------------

  static LRESULT FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);


};


#else  // TTimerH

class TTimer;

#endif  // TTimerH

#undef In_TTimerH
