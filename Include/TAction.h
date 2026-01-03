//-----------------------------------------------------------------------------
//! @file TAction.h
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

#ifdef In_TActionH
#error "Inclusion circulaire de TAction"
#endif // In_TActionH

#define In_TActionH

#ifndef TActionH
#define TActionH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum THelpType {
	htKeyword,										//!< Aide à partir d'un mot clé
	htContext											//!< Aide à partir d'un identifiant
};


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnExecute.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnExecute)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnUpdate.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnUpdate)(TObject *Sender);


//-----------------------------------------------------------------------------
//! @class TAction
//!
//! @brief Définition de la classe TAction
//!
//! Cette classe permet de centraliser les actions pouvant venir de différentes
//! sources (menu, raccourcis clavier, boutons, etc...).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TAction: public TComponent {
private:
protected:
	bool FAutoCheck;
  bool FChecked;
  bool FDisableIfNoHandler;
  bool FEnabled;
  int FGroupIndex;
  THelpContext FHelpContext;
  AnsiString FHelpKeyword;
  THelpType FHelpType;
  AnsiString FHint;
  TImageIndex FImageIndex;
  TShortCut FShortCut;
  bool FVisible;

	//---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessShortCut(int nVirtKey, TShiftState Shift);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TAction(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TAction(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Caption
  //!
	//! Cette méthode permet d'écrire la propriété Caption. Les contrôles et les
	//! éléments de menu qui auront cette action en propriété @b Action et dont
	//! le libellé est vide prendront automatiquement ce libellé, et il sera
	//! automatiquement mis à jour en cas de changement.
  //!
  //! @param[in]        NewCaption Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Caption(AnsiString NewCaption);

  //---------------------------------------------------------------------------
  //! @brief Exécution de l'action
	//!
	//! Cette méthode permet de lancer l'éxécution de l'action.
  //---------------------------------------------------------------------------

  virtual bool Execute(void);

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
	//! @brief Propriété AutoCheck: gestion automatique de l'état @b Checked.
	//!
	//! Cette propriété permet de cocher ou de décocher automatiquement les
	//! objets associés lorsque l'action est exécutée, à condition que ces objets
	//! ne gèrent pas eux-même l'état @b Checked (c'est à dire que leur
	//! propriété @b AutoCheck s'ils en ont une est à @b false).
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TAction, bool, AutoCheck);

  //---------------------------------------------------------------------------
  //! @brief Propriété Checked
	//!
	//! Cette propriété indique si le contrôle ou l'élément de menu associé à
	//! l'action ont une marque indiquant qu'ils sont actifs (cochés). Le
	//! changement de cette propriété est automatiquement répercuté sur les
	//! composants associés, sauf si ceux-ci ont une gestion propre de cette
	//! propriété (@b AutoCheck pour les éléments de menu par exemple).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, bool, Checked);


  //---------------------------------------------------------------------------
  //! @brief Propriété DisableIfNoHandler
	//!
	//! Si cette propriété est à @b true, l'action est automatiquement désactivée
	//! si aucun gestionnaire n'est associé à l'évènement OnExecute.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, bool, DisableIfNoHandler);


  //---------------------------------------------------------------------------
  //! @brief Propriété Enabled
	//!
	//! Cette propriété indique si l'action est activée. Cette propriété est
	//! automatiquement répercutée sur tous les composants associés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, bool, Enabled);


  //---------------------------------------------------------------------------
  //! @brief Propriété GroupIndex
	//!
	//! Cette propriété permet de n'avoir qu'une seule action cochée (et donc
	//! pareil pour les composants associés) dans un groupe d'index. Ceci permet
	//! de reproduire le comportement des radio-boutons lorsqu'une action y est
	//! associée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, int, GroupIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété HelpContext
	//!
	//! Lorsque cette propriété est différente de zéro, l'appui sur la touche F1
	//! provoquera l'ouverture de la page d'aide associée au contrôle qui a le
	//! focus.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, THelpContext, HelpContext);


  //---------------------------------------------------------------------------
  //! @brief Propriété HelpKeyword
	//!
	//! Cette propriété provoque l'ouverture de l'aide en relation avec un
	//! mot-clé (onglet d'index).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, AnsiString, HelpKeyword);


  //---------------------------------------------------------------------------
  //! @brief Propriété HelpType
	//!
	//! Cette propriété permet de définir si le programme d'aide s'ouvre sur la
	//! page "Sommaire" ou bien sur la page définie par la propriété
	//! @b HelpContext.
	//!
	//! @sa HelpContext
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, THelpType, HelpType);


  //---------------------------------------------------------------------------
  //! @brief Propriété Hint
	//!
	//! Cette propriété définit la contenu de l'info-bulle qui apparait lorsque
	//! la souris est sur le contrôle associé à l'action.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, AnsiString, Hint);


  //---------------------------------------------------------------------------
  //! @brief Propriété ImageIndex
	//!
	//! Cette propriété définit l'image qui apparait sur le contrôle associé à
	//! l'action (n'est utile que sur certains contrôles comme les @b TTreeView
	//! ou les @b TListView)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, TImageIndex, ImageIndex);


  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété indique le nom de l'action. Il doit être unique.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, AnsiString, Name);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShortCut
	//!
	//! Cette propriété permet d'associer un raccourci clavier à l'action. Les
	//! éléments de menu associés à cette action indiquent automatiquement le
	//! raccourci dans leur libellé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, TShortCut, ShortCut);


  //---------------------------------------------------------------------------
  //! @brief Propriété Visible
	//!
	//! Cette propriété indique si les composants associés à l'action sont
	//! visibles.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAction, bool, Visible);

	//@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnHint
	//!
	//! Cet évènement est déclenché lorsque le contrôle auquel est associé
	//! l'action ouvre l'info-bulle. Vous pouvez intercepter cet évènement pour
	//! effectuer une action particulière (par exemple changer le contenu de
	//! l'info-bulle en fonction des circonstances).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnHint, OnHint, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnExecute
	//!
	//! Cet évènement est déclenché lorsque l'action est exécutée, soit parce que
	//! l'utilisateur a cliqué sur un contrôle auquel est associé l'action, soit
	//! parce que la méthode @b Execute est appelée.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//!
	//! @sa Execute
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnExecute, OnExecute, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnUpdate
	//!
	//! Cet évènement est déclenché lorsque les propriétés de l'action ont été
	//! changées. Elle est en général utilisée par les contrôles associés pour
	//! mettre à jour leurs propriétés (Texte, raccourci clavier, par exemple).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnUpdate, OnUpdate, TObject *);

  //@}

};


#else  // TActionH

class TAction;

#endif  // TActionH


#undef In_TActionH
