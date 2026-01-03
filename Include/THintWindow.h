//-----------------------------------------------------------------------------
//! @file THintWindow.h
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

#ifdef In_THintWindowH
#error "Inclusion circulaire de THintWindow"
#endif // In_THintWindowH

#define In_THintWindowH

#ifndef THintWindowH
#define THintWindowH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class THintWindow
//!
//! @brief Définition de la classe THintWindow
//!
//! Cette classe permet de définir une fenêtre d'info-bulle, son libellé et
//! certaines méthodes spécifiques aux infos bulles (activation, calcul du
//! rectangle d'enveloppe).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class THintWindow: public TWinControl {
private:
protected:

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  THintWindow(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~THintWindow(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Visible
  //!
  //! Cette méthode permet d'écrire la propriété Visible
  //!
  //! @param[in]        NewVisible Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Visible(bool NewVisible);

	//---------------------------------------------------------------------------
	//! @brief Affichage de l'info-bulle
	//!
	//! Cette fonction fait apparaitre l'info-bulle à l'endroit indiqué par le
	//! paramètre @b Rect et avec le texte défini par @b AHint. Si le rectangle
	//! est en dehors de l'écran, il est automatiquement déplacé pour apparaitre
	//! entièrement dans l'écran.
	//!
	//! @param[in]        Rect Rectangle d'affichage
	//! @param[in]        AHint Texte de l'info-bulle
	//---------------------------------------------------------------------------
	virtual void FASTCALL ActivateHint(const TRect &Rect, const AnsiString AHint);

  //---------------------------------------------------------------------------
  //! @brief Calcule la taille de l'info-bulle pour un texte donné
  //!
  //! Cette méthode calcule la taille de l'info-bulle nécessaire à l'affichage
	//! du texte passé en paramètres. Le texte est éventuellement sur plusieurs
	//! lignes pour ne pas dépasser la largeur maximum définie. Le paramètre
	//! @b AData n'est pas utilisé, il peut l'être dans les classes héritées
	//! pour définir des paramètres supplémentaires.
  //!
  //! @param[in]        MaxWidth Largeur maximum de l'info-bulle
  //! @param[in]        AHint Texte à afficher
  //! @param[out]       AData Non utilisé, mettre la valeur NULL
  //!
  //! @return @c TRect Rectangle de la fenêtre de l'info-bulle
  //---------------------------------------------------------------------------
	virtual TRect FASTCALL CalcHintRect(int MaxWidth, const AnsiString AHint, void * AData);

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


#else  // THintWindowH

class THintWindow;

#endif  // THintWindowH


#undef In_THintWindowH
