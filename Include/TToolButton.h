//-----------------------------------------------------------------------------
//! @file TToolButton.h
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

#ifdef In_TToolButtonH
#error "Inclusion circulaire de TToolButton"
#endif // In_TToolButtonH

#define In_TToolButtonH

#ifndef TToolButtonH
#define TToolButtonH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TControlH
#include "TControl.h"
#endif // TControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TToolButtonStyle {
	tbsButton,										//!< Type bouton
	tbsCheck,											//!< Type case à cocher
	tbsDropDown,									//!< Type liste déroulante
	tbsSeparator,									//!< Séparateur (espace)
	tbsDivider										//!< Séparateur (barre verticale)
};


//-----------------------------------------------------------------------------
//! @class TToolButton
//!
//! @brief Définition de la classe TToolButton
//! @image html TToolButton.jpg "TToolButton"
//!
//! Cette classe définit un bouton de barre d'outil. Ce type de bouton ne
//! contient qu'une image. Il doit être intégré dans un objet TToolBar, dans
//! lequel Les images sont définies (dans un objet ImageList).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TToolButton: public TControl {
private:
protected:
  int FImageIndex;
  TToolButtonStyle FStyle;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessCommand(WORD Notify);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TToolButton(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TToolButton(void);

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
  //! @brief Propriété ImageIndex
	//!
	//! Cette propriété permet de définir le numéro d'image à utiliser dans la
	//! liste d'images pour dessiner le bouton.
	//!
	//! @sa TToolBar::Images
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TToolButton, int, ImageIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété Style
	//!
	//! Cette propriété permet de définir le style du bouton. Notamment, utilisez
	//! cette propriété pour créer un séparateur ou un espace entre les boutons.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TToolButton, TToolButtonStyle, Style);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnPaint
	//!
	//! Cet évènement est déclenché lorsque le composant doit être redessiné.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnPaint, TObject *);

  //@}

};

#else  // TToolButtonH

class TToolButton;

#endif  // TToolButtonH


#undef In_TToolButtonH
