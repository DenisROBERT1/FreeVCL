//-----------------------------------------------------------------------------
//! @file THotKey.h
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

#ifdef In_THotKeyH
#error "Inclusion circulaire de THotKey"
#endif // In_THotKeyH

#define In_THotKeyH

#ifndef THotKeyH
#define THotKeyH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum THKInvalidKey {
	hcNone,                       //!< Aucune
	hcShift,                      //!< Touche majuscules
	hcCtrl,                       //!< Touche contrôle
	hcAlt,                        //!< Touche alt
	hcShiftCtrl,                  //!< Touches majuscules + contrôle
	hcShiftAlt,                   //!< Touches majuscules + alt
	hcCtrlAlt,                    //!< Touches contrôle + alt
	hcShiftCtrlAlt                //!< Touches majuscules + contrôle + alt
};
typedef Set<THKInvalidKey, hcNone,hcShiftCtrlAlt> THKInvalidKeys;  //!< Combinaison de plusieurs THKInvalidKey

enum THKModifier {
	hkShift,                      //!< Touche majuscules
	hkCtrl,                       //!< Touche contrôle
	hkAlt,                        //!< Touche alt
	hkExt                         //!< Touche Windows
};
typedef Set<THKModifier, hkShift, hkExt> THKModifiers;  //!< Combinaison de plusieurs THKModifier


//-----------------------------------------------------------------------------
//! @class THotKey
//!
//! @brief Définition de la classe THotKey
//! @image html THotKey.jpg "THotKey"
//!
//! Cette classe définit le contrôle permettant à l'utilisateur de saisir une
//! combinaison de touches (raccourci clavier, par exemple). 
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class THotKey: public TWinControl {
private:
protected:
  THKInvalidKeys FInvalidKeys;

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

  THotKey(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~THotKey(void);

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
  //! @brief Propriété HotKey
	//!
	//! Cette propriété permet de lire le raccourci clavier saisi par
	//! l'utilisateur
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THotKey, TShortCut, HotKey);


  //---------------------------------------------------------------------------
  //! @brief Propriété InvalidKeys
	//!
	//! Cette propriété permet de restreindre les touches de contrôle que peut
	//! contenir le raccourci clavier. Par exemple, il est généralement sans
	//! signification de saisir juste une touche du clavier non associée à la
	//! touche CTRL ou ALT. Dans ce cas, le contrôle ajoute automatiquement la
	//! combinaison de touches définies par la propriété @b Modifier.
	//!
	//! @sa THKInvalidKey
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THotKey, THKInvalidKeys, InvalidKeys);


  //---------------------------------------------------------------------------
  //! @brief Propriété Modifiers
	//!
	//! Cette propriété définit la combinaison de touche à ajouter au raccourci
	//! si l'utilisateur saisit un raccourci qui est interdit par la propriété
	//! @b InvalidKeys.
	//!
	//! @sa THKModifier
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THotKey, THKModifiers, Modifiers);


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


#else  // THotKeyH

class THotKey;

#endif  // THotKeyH


#undef In_THotKeyH
