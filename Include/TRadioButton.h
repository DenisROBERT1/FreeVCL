//-----------------------------------------------------------------------------
//! @file TRadioButton.h
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

#ifdef In_TRadioButtonH
#error "Inclusion circulaire de TRadioButton"
#endif // In_TRadioButtonH

#define In_TRadioButtonH

#ifndef TRadioButtonH
#define TRadioButtonH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TRadioButton
//!
//! @brief Définition de la classe TRadioButton
//! @image html TRadioButton.jpg "TRadioButton"
//!
//! Cette classe est un contrôle de forme bouton-radio. Il permet à
//! l'utilisateur de choisir une option unique parmi une liste pré-définie. Un
//! seul bouton-radio peut être coché parmi tous ceux appartenant à un même
//! parent. Si vous souhaitez avoir plusieurs séries d'option, isolez chaque
//! série de boutons-radio dans des composants distincts (@b TPanel ou
//! @b TGroupBox par exemple). Vous pouvez également utiliser le contrôle
//! @b TRadioGroup.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TRadioButton: public TWinControl {
private:
protected:

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

  TRadioButton(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TRadioButton(void);

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
  //! @brief Propriété Alignment
	//!
	//! Cette propriété indique si le texte se trouve à droite (défaut) ou à
	//! gauche de la case à cocher.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRadioButton, TAlignment, Alignment);

  //---------------------------------------------------------------------------
  //! @brief Propriété Checked
	//!
	//! Cette propriété détermine si le bouton-radio est coché. Le fait de
	//! cocher un bouton-radio décoche automatiquement tous les boutons-radios
	//! du même groupe (c'est à dire appartenant au même parent).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRadioButton, bool, Checked);


  //@}


};


#else  // TRadioButtonH

class TRadioButton;

#endif  // TRadioButtonH


#undef In_TRadioButtonH
