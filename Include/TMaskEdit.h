//-----------------------------------------------------------------------------
//! @file TMaskEdit.h
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

#ifdef In_TMaskEditH
#error "Inclusion circulaire de TMaskEdit"
#endif // In_TMaskEditH

#define In_TMaskEditH

#ifndef TMaskEditH
#define TMaskEditH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TMaskEdit
//!
//! @brief Définition de la classe TMaskEdit
//!
//! Cette classe est un contrôle permettant de saisir un texte selon un format
//! défini par programme. Le format du texte est défini par la propriété
//! @b EditMask.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMaskEdit: public TWinControl {
private:
protected:
  AnsiString FEditMask;
  AnsiString FEditText;
  AnsiString FText;
  int FSelStart;
  int FSelLength;
  bool FReadOnly;
  TCHAR FPasswordChar;

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

  TMaskEdit(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TMaskEdit(void);

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
  //! @brief Propriété EditMask: format de saisie
	//!
	//! Cette propriété permet de définir le masque de formatage du texte saisi.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, AnsiString, EditMask);


  //---------------------------------------------------------------------------
  //! @brief Propriété EditText: texte formaté
	//!
	//! Cette propriété permet de lire le texte formaté. Pour lire le texte
	//! brut, utilisez la propriété @b Text
	//!
	//! @sa Text
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, AnsiString, EditText);


  //---------------------------------------------------------------------------
  //! @brief Propriété Text: texte brut
	//!
	//! Cette propriété permet de lire ou de définir le contenu texte du
	//! composant @b TMaskEdit.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, AnsiString, Text);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelStart: début de sélection
	//!
	//! Cette propriété permet de lire ou de définir le premier caractère
	//! sélectionné dans le texte du composant. Le premier caractère a l'index
	//! zéro.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, int, SelStart);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelLength: longueur de la sélection
	//!
	//! Cette propriété de lire ou de définir la longueur de la sélection dans le
	//! texte du composant. Si cette longueur est à zéro, cela signifie qu'aucun
	//! caractère n'est sélectionné (seul le caret clignotant est visible)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, int, SelLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété MaxLength: longueur maximum du texte saisi
	//!
	//! Cette propriété permet de définir une longueur maximum que pourra saisir
	//! l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, int, MaxLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété ReadOnly: lecture seule
	//!
	//! Cette propriété permet d'interdire toute modification du texte.
	//! L'utilisateur pourra uniquement sélectionner du texte pour le copier dans
	//! le presse-papiers.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, bool, ReadOnly);


  //---------------------------------------------------------------------------
  //! @brief Propriété PasswordChar: caractère de remplacement du texte saisi
	//!
	//! Cette propriété définit le caractère qui s'affichera à la place du texte
	//! (ce qui permet la saisie d'un mot de passe, par exemple). Le caractère
	//! le plus souvent utilisé pour cet usage est l'astérisque (*). Pour ne plus
	//! avoir le texte invisible, affectez le caractère nul à cette propriété.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMaskEdit, TCHAR, PasswordChar);

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


#else  // TMaskEditH

class TMaskEdit;

#endif  // TMaskEditH


#undef In_TMaskEditH
