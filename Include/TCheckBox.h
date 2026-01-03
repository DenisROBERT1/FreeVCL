//-----------------------------------------------------------------------------
//! @file TCheckBox.h
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

#ifdef In_TCheckBoxH
#error "Inclusion circulaire de TCheckBox"
#endif // In_TCheckBoxH

#define In_TCheckBoxH

#ifndef TCheckBoxH
#define TCheckBoxH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TCheckBoxState {
	cbUnchecked,									// Case non cochée
	cbChecked,									  // Case cochée
	cbGrayed									    // Etat indéterminé (case grisée)
};


//-----------------------------------------------------------------------------
//! @class TCheckBox
//!
//! @brief Définition de la classe TCheckBox
//! @image html TCheckBox.jpg "TCheckBox"
//!
//! Cette classe définit le contrôle graphique "case à cocher". Elle se
//! représente par une case où l'utilisateur peut cliquer, et un texte
//! d'explication associé.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCheckBox: public TWinControl {
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

  TCheckBox(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCheckBox(void);

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
	//! Cette propriété définit la position du texte par rapport à la case à
	//! cocher.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCheckBox, TAlignment, Alignment);

  //---------------------------------------------------------------------------
  //! @brief Propriété AllowGrayed
	//!
	//! Si cette propriété est à @b true, la case à cocher a trois états: cochée,
	//! décochée ou indéterminée (grisée).
	//!
	//! @sa State
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCheckBox, bool, AllowGrayed);

  //---------------------------------------------------------------------------
  //! @brief Propriété Checked
	//!
	//! Cette propriété définit l'état coché ou décoché du contrôle. Dans le cas
	//! d'une case à trois états, utilisez plutôt la propriété @b State, car
	//! la propriété @b Checked renverra indifféremment faux dans les deux états
	//! où la case n'est pas cochée (@b cbUnchecked et @b cbGrayed).
	//!
	//! @sa AllowGrayed, State
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCheckBox, bool, Checked);

  //---------------------------------------------------------------------------
  //! @brief Propriété State
	//!
	//! Cette propriété permet de lire ou de définir l'état le la case à cocher.
	//! Utilisez cette propriété de préférence à la propriété @b Checked dans
	//! le cas d'une case à trois états (car la propriété @b Checked n'a que
	//! deux réponses possibles: cochée ou décochée)
	//!
	//! @sa AllowGrayed, Checked
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCheckBox, TCheckBoxState, State);


  //@}


};


#else  // TCheckBoxH

class TCheckBox;

#endif  // TCheckBoxH


#undef In_TCheckBoxH
