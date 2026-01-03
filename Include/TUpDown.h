//-----------------------------------------------------------------------------
//! @file TUpDown.h
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

#ifdef In_TUpDownH
#error "Inclusion circulaire de TUpDown"
#endif // In_TUpDownH

#define In_TUpDownH

#ifndef TUpDownH
#define TUpDownH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TEdit;


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TUDBtnType {
	btNext,												//!< Incrémente la valeur (Flèche vers le haut ou vers la droite)
	btPrev												//!< Décrémente la valeur (Flèche vers le bas ou vers la gauche)
};


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnClick2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnClick2)(TObject *Sender, TUDBtnType Button);


//-----------------------------------------------------------------------------
//! @class TUpDown
//!
//! @brief Définition de la classe TUpDown
//! @image html TUpDown.jpg "TUpDown"
//!
//! Cette classe définit un contrôle associé à un contrôle d'édition et
//! permettant à l'utilisateur d'incrémenter ou de décrémenter la valeur
//! numérique contenue dans le contrôle d'édition.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TUpDown: public TWinControl {
private:
  long FMin;
  long FMax;
  long FIncrement;
  long FPos;
  TEdit *FAssociate;

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

  TUpDown(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TUpDown(void);

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
  //! @brief Propriété Min: valeur minimum
	//!
	//! Cette propriété détermine la valeur minimum que peut prendre le contrôle
	//! associé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TUpDown, long, Min);

  //---------------------------------------------------------------------------
  //! @brief Propriété Max: valeur maximum
	//!
	//! Cette propriété détermine la valeur maximum que peut prendre le contrôle
	//! associé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TUpDown, long, Max);

  //---------------------------------------------------------------------------
  //! @brief Propriété Increment: valeur d'incrémentation
	//!
	//! Cette propriété définit l'incrémentation ou la décrémentation de la
	//! valeur du contrôle associé lorsque l'utilisateur appuie sur le bouton
	//! "haut" ou "bas" du @b TUpDown.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TUpDown, long, Increment);

  //---------------------------------------------------------------------------
  //! @brief Propriété Position: position courante
	//!
	//! Cette propriété permet de lire ou de définir la valeur du contrôle
	//! associé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TUpDown, long, Position);

  //---------------------------------------------------------------------------
  //! @brief Propriété Associate: contrôle associé
	//!
	//! Cette propriété définit le contrôle associé. Ce conrôle doit être un
	//! @b TEdit ou bien un de ses descendants.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TUpDown, TEdit *, Associate);

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

  //---------------------------------------------------------------------------
  //! @brief Evènement OnClick: Appui sur les boutons +/-
  //! 
  //! Cet évènement est déclenché lorsque l'utilisateur appuie sur l'un des
	//! boutons + ou - pour incrémenter ou décrémenter la valeur du contrôle.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Bouton cliqué
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnClick2, OnClick, TObject *, TUDBtnType);

  //@}


};


#else  // TUpDownH

class TUpDown;

#endif  // TUpDownH


#undef In_TUpDownH
