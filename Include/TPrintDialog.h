//-----------------------------------------------------------------------------
//! @file TPrintDialog.h
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

#ifdef In_TPrintDialogH
#error "Inclusion circulaire de TPrintDialog"
#endif // In_TPrintDialogH

#define In_TPrintDialogH

#ifndef TPrintDialogH
#define TPrintDialogH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TPrintDialogOption {
	poPrintToFile,								//!< Active l'option permettant d'imprimer dans un fichier
	poPageNums,										//!< Active l'option permettant d'imprimer d'une série de pages
	poSelection,									//!< Active l'option permettant d'imprimer les pages sélectionnées
	poWarning,										//!< Affichage d'un avertissement si l'imprimante est déconnectée
	poHelp,												//!< Affichage du bouton "Aide"
	poDisablePrintToFile					//!< L'option "Impression dans un fichier" est grisée (uniquement si poPrintToFile est positionné)
};

typedef Set<TPrintDialogOption, poPrintToFile, poDisablePrintToFile> TPrintDialogOptions;  //!< Combinaison de plusieurs TPrintDialogOption

enum TPrintRange {
	prAllPages,										//!< Impression de toutes les pages
	prSelection,									//!< Impression des pages sélectionnées
	prPageNums										//!< Impression d'une série de pages
};


//-----------------------------------------------------------------------------
//! @class TPrintDialog
//!
//! @brief Définition de la classe TPrintDialog
//!
//! Cette classe est une boîte de dialogue permettant à l'utilisateur de saisir
//! les paramètres nécéssaires à une impression (pages à imprimer, nombre de
//! copies, options, etc...). Vous pouvez éventuellement initialiser ces
//! paramètres avec vos propres valeurs avant d'appeler la méthode @b Execute.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPrintDialog: public TComponent {
private:

protected:
  PRINTDLG PrintDlgStruct;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPrintDialog(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPrintDialog(void);

  //@}


  //! @name Méthodes
  //@{

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
  //! @brief Propriété Copies: nombre de copies
	//!
	//! Cette propriété permet de définir et de lire le nombre de copies demandé
	//! par l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, int, Copies);


  //---------------------------------------------------------------------------
  //! @brief Propriété MinPage: numéro de page minimum
	//!
	//! Cette propriété permet d'afficher une erreur si l'utilisateur entre un
	//! numéro de page inférieur à cette valeur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, int, MinPage);


  //---------------------------------------------------------------------------
  //! @brief Propriété MaxPage: numéro de page maximum
	//!
	//! Cette propriété permet d'afficher une erreur si l'utilisateur entre un
	//! numéro de page supérieur à cette valeur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, int, MaxPage);


  //---------------------------------------------------------------------------
  //! @brief Propriété FromPage: première page à imprimer
	//!
	//! Cette propriété permet de définir la première page à imprimer.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, int, FromPage);


  //---------------------------------------------------------------------------
  //! @brief Propriété ToPage: dernière page à imprimer
	//!
	//! Cette propriété permet de définir la dernière page à imprimer.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, int, ToPage);


  //---------------------------------------------------------------------------
  //! @brief Propriété Collate: case "Copies triées"
	//!
	//! Cette propriété détermine l'état coché de la case "Copies triées".
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, bool, Collate);


  //---------------------------------------------------------------------------
  //! @brief Propriété PrintToFile
	//!
	//! Cette propriété permet d'imprimer le document par l'intermédiaire d'un
	//! fichier d'impression. Si cette propriété est à @b true, le programme
	//! demandera ensuite le nom du fichier d'impression.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, bool, PrintToFile);


  //---------------------------------------------------------------------------
  //! @brief Propriété Options: options d'impression
	//!
	//! Cette propriété permet de lire ou de définir les options d'impression.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, TPrintDialogOptions, Options);


  //---------------------------------------------------------------------------
  //! @brief Propriété PrintRange: Sélection de pages à imprimer
	//!
	//! Cette propriété permet de lire ou de définir les options sélectionnées
	//! dans la partie "pages à imprimer".
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPrintDialog, TPrintRange, PrintRange);


  //@}
};


//---------------------------------------------------------------------------
#endif

#undef In_TPrintDialogH
