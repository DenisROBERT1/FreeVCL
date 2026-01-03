//-----------------------------------------------------------------------------
//! @file TButton.h
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

#ifdef In_TButtonH
#error "Inclusion circulaire de TButton"
#endif // In_TButtonH

#define In_TButtonH

#ifndef TButtonH
#define TButtonH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TButton
//!
//! @brief Bouton standard
//! @image html TButton.jpg "TButton"
//!
//! Cette classe permet de créer un bouton standard. Les boutons sont
//! généralement utilisés pour effectuer une action. Pour créer un bouton
//! affichant une bitmap au lieu d'un texte, utilisez TBitBtn. Pour créer un
//! bouton pouvant rester en position enfoncée, utilisez TSpeedButton.
//-----------------------------------------------------------------------------

class TButton: public TWinControl {
private:
protected:
  bool FCancel;
  int FModalResult;

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

  TButton(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TButton(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Simule un clic sur le bouton.
  //!
  //! Cette méthode fait exactement comme si l'utilisateur avait cliqué sur le
	//! bouton, c'est à dire:
	//! @li Déclenchement d'un évènement OnClick.
  //! @li Affectation de la valeur de la propriété ModalResult (et par
	//! conséquent, suivant cette valeur, fermeture éventuelle de la fiche).
  //---------------------------------------------------------------------------
	virtual void FASTCALL Click(void);

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
  //! @brief Propriété Default: Bouton associé à l'appui sur la touche Entrée.
	//!
	//! Si cette propriété est à true, l'appui sur la touche "Retour" (ou
	//! bien la touche "Entrée" du clavier numérique) a le même effet qu'un clic
	//! souris sur ce bouton (évènement @b OnClic, affectation de la valeur de la
	//! propriété @b ModalResult à la boîte de dialogue).
	//! Si un dialogue contient plusieurs boutons ayant @b Default à true,
	//! l'appui sur la touche "Retour" n'effectue que le clic du premier bouton
	//! visible et actif dans l'ordre des TabStop.
	//! @note Tout bouton détenant le focus devient le bouton par défaut, c'est à
	//! dire que si l'utilisateur sélectionne un autre bouton avant d'appuyer sur
	//! "Retour", l'action effectuée sera celle associée à ce bouton sélectionné.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TButton, bool, Default);

  //---------------------------------------------------------------------------
  //! @brief Propriété Cancel: Bouton associé à l'appui sur la touche Echap.
	//!
	//! Si cette propriété est à true, l'appui sur la touche "Echap" a le même
	//! effet qu'un clic souris sur ce bouton (évènement @b OnClic, affectation
	//! de la valeur de la propriété @b ModalResult à la boîte de dialogue).
	//! Si un dialogue contient plusieurs boutons ayant @b Cancel à true,
	//! l'appui sur la touche "Echap" n'effectue que le clic du premier bouton
	//! visible et actif dans l'ordre des TabStop.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TButton, bool, Cancel);

  //---------------------------------------------------------------------------
  //! @brief Propriété ModalResult: Ferme la boîte de dialogue et détermine la valeur de retour.
	//!
	//! L'affectation de la propriété @b ModalResult permet de fermer
	//! automatiquement la boîte de dialogue lorsque l'utilisateur clique sur
	//! le bouton (ou bien déclenche l'action du bouton par une autre façon,
	//! clavier ou méthode @b Click), et ceci sans avoir à gérer l'évènement
	//! @b OnClick. La valeur retournée par la méthode d'ouverture de la
	//! fenêtre (par exemple @b ShowModal s'il s'agit d'une fenêtre modale) sera
	//! celle correspondant au bouton cliqué.
	//!
	//! @sa TForm::ModalResult
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TButton, int, ModalResult);

  //@}


};


#else  // TButtonH

class TButton;

#endif  // TButtonH


#undef In_TButtonH
