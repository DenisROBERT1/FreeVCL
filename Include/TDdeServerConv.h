//-----------------------------------------------------------------------------
//! @file TDdeServerConv.h
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

#ifdef In_TDdeServerConvH
#error "Inclusion circulaire de TDdeServerConv"
#endif // In_TDdeServerConvH

#define In_TDdeServerConvH

#ifndef TDdeServerConvH
#define TDdeServerConvH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH

#ifndef TDdeServerItemH
#include "TDdeServerItem.h"
#endif  // TDdeServerItemH


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnExecuteMacro.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnExecuteMacro)(TObject *Sender);


//-----------------------------------------------------------------------------
//! @class TDdeServerConv
//!
//! @brief Définition de la classe TDdeServerConv
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur le
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDdeServerConv: public TComponent {
private:
  HWND HandleClient;
  HWND HandleServer;
  UINT MessageDde;
  LPARAM lParamDde;
protected:

  // Liste des items associés à cette liaison DDE
  std::vector<TDdeServerItem *> ListDdeServerItems;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  bool TraitmntDdeInitiate(HWND hWnd, LPARAM lParam);
  bool TraitmntDdeAdvise(HWND hWnd, LPARAM lParam);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDdeServerConv(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDdeServerConv(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Répond aux macros envoyées par le client DDE.
  //---------------------------------------------------------------------------

  int FASTCALL ExecuteMacro(int Data);

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
  //! @brief Ajout d'un client à la liste ListDdeServerItems.
  //!
  //! Ne pas utiliser cette fonction: elle ne doit être utilisée que par
  //! TDdeServerItem. Elle permet d'ajouter à élément de la liste
  //! ListDdeServerItems.
  //---------------------------------------------------------------------------
  bool AddDdeServerItem(TDdeServerItem *DdeServerItem);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un client de la liste ListDdeServerItems.
  //!
  //! Ne pas utiliser cette fonction: elle ne doit être utilisée que par
  //! TDdeServerItem. Elle permet d'enlever un élément de la liste
  //! ListDdeServerItems.
  //---------------------------------------------------------------------------
  bool RemoveDdeServerItem(TDdeServerItem *DdeServerItem);

  //---------------------------------------------------------------------------
  //! @brief Procédure de fenêtre
  //---------------------------------------------------------------------------

  static LRESULT FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);


  //@}

  //! @name Propriétés
  //@{


  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnExecuteMacro
	//!
	//! Cet évènement est déclenché lorsqu'une macro DDE a été exécutée.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnExecuteMacro, OnExecuteMacro, TObject *);

  //@}


};


#else  // TDdeServerConvH

class TDdeServerConv;

#endif  // TDdeServerConvH

#undef In_TDdeServerConvH
