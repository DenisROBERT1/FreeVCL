//-----------------------------------------------------------------------------
//! @file TDdeClientConv.h
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

#ifdef In_TDdeClientConvH
#error "Inclusion circulaire de TDdeClientConv"
#endif // In_TDdeClientConvH

#define In_TDdeClientConvH

#ifndef TDdeClientConvH
#define TDdeClientConvH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH

#ifndef TDdeClientItemH
#include "TDdeClientItem.h"
#endif  // TDdeClientItemH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TDataMode {
	ddeAutomatic,                 //!< Mode automatique
	ddeManual                     //!< Mode manuel
};


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnOpen.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnOpen)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnClose2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnClose2)(TObject *Sender);


//-----------------------------------------------------------------------------
//! @class TDdeClientConv
//!
//! @brief Définition de la classe TDdeClientConv
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur le
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDdeClientConv: public TComponent {
private:
  HWND HandleClient;
  HWND HandleServer;
  UINT MessageDde;  // Mémo message DDE reçu
  LPARAM lParamDde;  // LPARAM du message DDE reçu
protected:
  TDataMode FConnectMode;
  int FConv;
  TDataMode FDataMode;
  int FDdeFmt;
  AnsiString FDdeService;
  AnsiString FDdeTopic;
  bool FFormatChars;
  AnsiString FServiceApplication;
  bool FWaitStat;

  // Liste des items associés à cette liaison DDE
  std::vector<TDdeClientItem *> ListDdeClientItems;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  bool TraitmntDdeAck(HWND hWnd, LPARAM lParam);
  bool TraitmntDdeTerminate(HWND hWnd);
  bool TraitmntDdeData(HWND hWnd, LPARAM lParam);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDdeClientConv(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDdeClientConv(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Met fin à une conversation DDE en cours.
  //---------------------------------------------------------------------------

  void FASTCALL CloseLink(void);

  //---------------------------------------------------------------------------
  //! @brief Envoie une chaîne de macro-commande à l'application serveur DDE.
  //---------------------------------------------------------------------------

  bool FASTCALL ExecuteMacro(char * Cmd, bool WaitFlg);

  //---------------------------------------------------------------------------
  //! @brief Envoie une liste de macro-commandes à une application serveur DDE.
  //---------------------------------------------------------------------------

  bool FASTCALL ExecuteMacroLines(TStrings* Cmd, bool WaitFlg);

  //---------------------------------------------------------------------------
  //! @brief Commence une nouvelle conversation DDE.
  //---------------------------------------------------------------------------

  bool FASTCALL OpenLink(void);

  //---------------------------------------------------------------------------
  //! @brief Ouvre une conversation DDE avec l'objet Presse-papiers en cours.
  //---------------------------------------------------------------------------

  bool FASTCALL PasteLink(void);

  //---------------------------------------------------------------------------
  //! @brief Envoie des données à une application serveur DDE.
  //---------------------------------------------------------------------------

  bool FASTCALL PokeData(const AnsiString Item, char *Data);

  //---------------------------------------------------------------------------
  //! @brief Envoie plusieurs lignes de données à une application serveur DDE.
  //---------------------------------------------------------------------------

  bool FASTCALL PokeDataLines(const AnsiString Item, TStrings* Data);

  //---------------------------------------------------------------------------
  //! @brief Demande des données à un serveur DDE.
  //---------------------------------------------------------------------------

  AnsiString FASTCALL RequestData(const AnsiString Item);

  //---------------------------------------------------------------------------
  //! @brief Spécifie le service et la rubrique d'une conversation DDE et tente
  //! d'ouvrir le lien si ConnectMode vaut ddeAutomatic.
  //---------------------------------------------------------------------------

  bool FASTCALL SetLink(const AnsiString Service, const AnsiString Topic);

  //---------------------------------------------------------------------------
  //! @brief Démarre une boucle d'information pour tous les TDdeClientItems
  //! associés à la conversation DDE en cours.
  //---------------------------------------------------------------------------

  bool FASTCALL StartAdvise(void);


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


  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction de l'objet
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'un client à la liste ListDdeClientItems.
  //!
  //! Ne pas utiliser cette fonction: elle ne doit être utilisée que par
  //! TDdeClientItem. Elle permet d'ajouter à élément de la liste
  //! ListDdeClientItems.
  //---------------------------------------------------------------------------
  bool AddDdeClientItem(TDdeClientItem *DdeClientItem);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un client de la liste ListDdeClientItems.
  //!
  //! Ne pas utiliser cette fonction: elle ne doit être utilisée que par
  //! TDdeClientItem. Elle permet d'enlever un élément de la liste
  //! ListDdeClientItems.
  //---------------------------------------------------------------------------
  bool RemoveDdeClientItem(TDdeClientItem *DdeClientItem);

  //---------------------------------------------------------------------------
  //! @brief Procédure de fenêtre
  //---------------------------------------------------------------------------

  static LRESULT FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);


  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ConnectMode
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, TDataMode, ConnectMode);


  //---------------------------------------------------------------------------
  //! @brief Propriété Conv
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, int, Conv);


  //---------------------------------------------------------------------------
  //! @brief Propriété DataMode
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, TDataMode, DataMode);


  //---------------------------------------------------------------------------
  //! @brief Propriété DdeFmt
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, int, DdeFmt);


  //---------------------------------------------------------------------------
  //! @brief Propriété DdeService
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, AnsiString, DdeService);


  //---------------------------------------------------------------------------
  //! @brief Propriété DdeTopic
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, AnsiString, DdeTopic);


  //---------------------------------------------------------------------------
  //! @brief Propriété FormatChars
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, bool, FormatChars);


  //---------------------------------------------------------------------------
  //! @brief Propriété ServiceApplication
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, AnsiString, ServiceApplication);


  //---------------------------------------------------------------------------
  //! @brief Propriété WaitStat
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDdeClientConv, bool, WaitStat);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnOpen
	//!
	//! Cet évènement est déclenché lorsqu'une liaison DDE est ouverte.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnOpen, OnOpen, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnClose
	//!
	//! Cet évènement est déclenché lorsqu'une liaison DDE est fermée.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnClose2, OnClose, TObject *);

  //@}


};


#else  // TDdeClientConvH

class TDdeClientConv;

#endif  // TDdeClientConvH

#undef In_TDdeClientConvH
