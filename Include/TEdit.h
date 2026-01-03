//-----------------------------------------------------------------------------
//! @file TEdit.h
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

#ifdef In_TEditH
#error "Inclusion circulaire de TEdit"
#endif // In_TEditH

#define In_TEditH

#ifndef TEditH
#define TEditH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TEdit
//!
//! @brief Définition de la classe TEdit
//! @image html TEdit.jpg "TEdit"
//!
//! Cette classe définit le contrôle permettant à l'utilisateur de saisir du
//! texte sur une ligne (nom ou mot de passe par exemple).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TEdit: public TWinControl {
private:
protected:
  TCHAR FPasswordChar;
  bool FReadOnly;
  int FSelStart;
  int FSelLength;
  AnsiString FText;

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

  TEdit(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TEdit(void);

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
  //! @brief Propriété MaxLength
	//!
	//! Cette propriété définit la longueur maximum du texte.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TEdit, int, MaxLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété PasswordChar
	//!
	//! Cette propriété définit le caractère qui s'affiche à la place du texte
	//! pour masquer la saisie (contrôle d'édition d'un mot de passe). Mettez
	//! cette propriété à zéro pour voir le texte en clair.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TEdit, TCHAR, PasswordChar);

  //---------------------------------------------------------------------------
  //! @brief Propriété ReadOnly
	//!
	//! Cette propriété permet d'avoir une boîte d'édition dans laquelle on peut
	//! copier le texte, mais pas le modifier.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TEdit, bool, ReadOnly);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelStart
	//!
	//! Cette propriété détermine le début du texte sélectionné. Le fait de
	//! changer cette propriété met automatiquement la propriété @b SelLength à
	//! zéro.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TEdit, int, SelStart);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelLength
	//!
	//! Cette propriété détermine la longueur du texte sélectionné. Si cette
	//! longueur est à zéro, ça signifie que le curseur texte (caret) s'affiche.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TEdit, int, SelLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété Text
	//!
	//! Cette propriété permet de lire et d'initialiser le texte du contrôle
	//! d'édition.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TEdit, AnsiString, Text);

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


#else  // TEditH

class TEdit;

#endif  // TEditH


#undef In_TEditH
