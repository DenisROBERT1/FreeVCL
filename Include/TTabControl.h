//-----------------------------------------------------------------------------
//! @file TTabControl.h
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

#ifdef In_TTabControlH
#error "Inclusion circulaire de TTabControl"
#endif // In_TTabControlH

#define In_TTabControlH

#ifndef TTabControlH
#define TTabControlH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#include "TTabControlTabs.h"


//-----------------------------------------------------------------------------
//! @class TTabControl
//!
//! @brief Définition de la classe TTabControl
//! @image html TTabControl.jpg "TTabControl"
//!
//! Cette classe est un contrôle représentant une série d'onglets. À la
//! différence de la classe @b TPageControl, cette classe ne gère pas le
//! changement de page. C'est donc au programmeur de traiter l'évènement
//! @b OnChange et de faire les modifications nécessaires à ce moment.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTabControl: public TWinControl {
private:
  int FTabIndex;
  TTabControlTabs *FTabs;

protected:

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

  TTabControl(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTabControl(void);

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

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction de la fiche
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété TabIndex: index de l'onglet sélectionné
 	//!
	//! Cette propriété définit l'index de l'onglet sélectionné.
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTabControl, int, TabIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété Tabs
	//!
	//! Cette propriété permet d'accéder à la liste des libellés des onglets.
	//! Utilisez cette propriété pour ajouter, supprimer ou modifier des onglets.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTabControl, TStrings *, Tabs);

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


#else  // TTabControlH

class TTabControl;

#endif  // TTabControlH


#undef In_TTabControlH
