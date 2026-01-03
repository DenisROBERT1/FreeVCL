//-----------------------------------------------------------------------------
//! @file TOpenSaveDialog.h
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

#ifdef In_TOpenSaveDialogH
#error "Inclusion circulaire de TOpenSaveDialog"
#endif // In_TOpenSaveDialogH

#define In_TOpenSaveDialogH

#ifndef TOpenSaveDialogH
#define TOpenSaveDialogH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TOpenOption {
	ofReadOnly,								//!< Etat de la case à cocher "Lecture seule" (entrée-sortie).
	ofOverwritePrompt,				//!< Demande de confirmation avant d'écraser un fichier existant.
	ofHideReadOnly,						//!< Rend invisible la case à cocher "Lecture seule".
	ofNoChangeDir,						//!< Ne change pas le répertoire courant si l'utilisateur change de répertoire.
  ofShowHelp,								//!< Fait apparaître le bouton d'aide.
	ofNoValidate,							//!< Ne contrôle pas la validité du nom de ficher entré.
	ofAllowMultiSelect,				//!< Autorise la saisie de plusieurs fichiers.
  ofExtensionDifferent,			//!< En sortie, indique que l'utilisateur a entré un nom de fichier avec une extension différente de l'extension par défaut.
	ofPathMustExist,					//!< Indique que l'utilisateur ne peut choisir qu'un répertoire existant. Cette option est automatiquement activée avec l'option @b ofFileMustExist.
	ofFileMustExist,					//!< Indique que l'utilisateur ne peut entrer qu'un nom de fichier existant.
  ofCreatePrompt,						//!< Demande confirmation de création si le fichier n'existe pas.
	ofShareAware,							//!< Autorise le choix d'un fichier utilisé par un autre processus.
	ofNoReadOnlyReturn,				//!< Interdit le choix d'un fichier en lecture seule.
  ofNoTestFileCreate,				//!< N'essaye pas de créer le fichier. A utiliser sur les périphériques sur lesquels la réouverture de fichier n'est pas autorisée.
	ofNoNetworkButton,				//!< Interdit le choix d'un fichier sur un autre ordinateur du réseau.
	ofNoLongNames,						//!< Affiche les noms courts (à utiliser avec l'option @b ofOldStyleDialog).
  ofOldStyleDialog,					//!< Boîte de dialogue à la mode Windows 3.1 .
	ofNoDereferenceLinks,			//!< Autorise le choix d'un raccourci (sinon, ça renvoie le fichier référencé par le raccourci).
	ofEnableIncludeNotify,		//!< Envoie un message CDN_INCLUDEITEM lorsque l'utilisateur ouvre un répertoire.
  ofEnableSizing						//!< Autorise le redimensionnement de la fenêtre.
};

typedef Set<TOpenOption, ofReadOnly, ofNoDereferenceLinks> TOpenOptions;  //!< Combinaison de plusieurs TOpenOption


//-----------------------------------------------------------------------------
//! @class TOpenSaveDialog
//!
//! @brief Définition de la classe TOpenSaveDialog
//!
//! Cette classe est la classe de base des boîtes de dialogue permettant la
//! saisie ou l'enregistrement d'un fichier. De nombreuses options permettent
//! de modifier l'aspect ou le comportement de ce contrôle, essentiellemnt au
//! travers de la propriété @b Options.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TOpenSaveDialog: public TComponent {
private:
protected:
  DWORD Flags;
  HWND Handle;

  AnsiString FTitle;
  AnsiString FInitialDir;
  AnsiString FFileName;
  AnsiString FFilter;
  int FFilterIndex;
  AnsiString FDefaultExt;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TOpenSaveDialog(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TOpenSaveDialog(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ouverture de la boîte de dialogue
	//!
	//! Cette méthode ouvre une boîte modale permettant de choisir un nom de
	//! fichier à ouvrir ou à enregistrer.
	//!
  //! @return @c bool false si l'utilisateur a cliqué sur "Annuler" ou bien a
	//! fermé la boîte avec la croix. true s'il a choisi un nom de fichier.
  //---------------------------------------------------------------------------

  virtual bool Execute(void) = 0;

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
  //! @brief Propriété Title: titre da la boîte de dialogue
	//!
	//! Cette propriété permet de définir le titre de la boîte d'ouverture ou
	//! d'enregistrement d'un fichier.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TOpenSaveDialog, AnsiString, Title);

  //---------------------------------------------------------------------------
  //! @brief Propriété InitialDir: répertoire initial
	//!
	//! Cette propriété indique le répertoire initial du fichier. Si cette
	//! propriété est vide, la recherche se positionnera sur le répertoire
	//! courant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TOpenSaveDialog, AnsiString, InitialDir);

  //---------------------------------------------------------------------------
  //! @brief Propriété FileName: Nom de fichier
	//!
	//! En entrée, définir cette propriété pour proposer à l'utilisateur un nom
	//! de fichier par défaut. En sortie, elle indique le fichier choisi par
	//! l'utilisateur. Vous pouver entrer le chemin complet, ou bien juste un
	//! nom de fichier dans le répertoire initial. En sortie, la propriété
	//! contient le chemin complet (disque, répertoire, nom de fichier et
	//! extension).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TOpenSaveDialog, AnsiString, FileName);

  //---------------------------------------------------------------------------
  //! @brief Propriété Filter: filtres
	//!
	//! Indique les filtres présents dans la combo-box permettant de choisir
	//! certains types de fichiers. Chaque filtre contient le texte présent dans
	//! la combo-box, et les extensions (pouvant comporter les jokers '?' et
	//! '*'), séparées par un point-virgule. Entre ces valeurs (filtres et
	//! extensions), le caractère séparateur est le caractère '|'.@n
	//! Exemples de filtres:@n
	//! Fichiers texte (*.txt;*.ini)|*.txt;*.ini|Tous (*.*)|*.*@n
	//! Bitmaps Windows|*.bmp;*.dib|Fichiers GIF|*.gif@n
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TOpenSaveDialog, AnsiString, Filter);

  //---------------------------------------------------------------------------
  //! @brief Propriété FilterIndex
	//!
	//! Cette propriété indique l'item sélectionné par défaut dans la combo-box
	//! du type de fichiers (déterminé par la propriété @b Filter). Le premier
	//! item de la liste a comme numéro @b 1, le second @b 2 et ainsi de suite.
	//! Si le numéro est incorrect (trop petit ou trop grand), l'item sélectionné
	//! sera le premier.
	//!
	//! @sa TOpenSaveDialog::Filter
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TOpenSaveDialog, int, FilterIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété DefaultExt: extension par défaut
	//!
	//! Cette propriété détermine l'extension par défaut du fichier. Ça signifie
	//! que si l'utilisateur indique un nom de fichier sans extension, cette
	//! extension sera automatiquement ajoutée au nom de fichier. Si cette
	//! extension fait plus de trois caractères, seuls les trois premiers seront
	//! pris en compte. Ne pas mettre de point au début de l'extension.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TOpenSaveDialog, AnsiString, DefaultExt);

  //---------------------------------------------------------------------------
  //! @brief Propriété Options: Combinaison d'options d'affichage.
	//!
	//! Cette propriété permet de définir une ou plusieurs options permettant de
	//! changer l'affichage ou le comportement de la fenêtre d'ouverture d'un
	//! fichier.
	//!
	//! @sa TOpenOption
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TOpenSaveDialog, TOpenOptions, Options);

  //@}

};


#else  // TOpenSaveDialogH

class TOpenSaveDialog;

#endif  // TOpenSaveDialogH


#undef In_TOpenSaveDialogH
