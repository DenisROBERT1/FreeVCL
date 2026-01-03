//-----------------------------------------------------------------------------
//! @file TComponent.h
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

#ifdef In_TComponentH
#error "Inclusion circulaire de TComponent"
#endif // In_TComponentH

#define In_TComponentH

#ifndef TComponentH
#define TComponentH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TWinControl;


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TComponentState {
	csLoading,										//!< Le composant est en train d'être chargé
	csReading,										//!< Le composant est en cours de lecture
	csWriting,										//!< Le composant est en train de s'enregistrer
	csDestroying,									//!< Le composant est en train d'être détruit
	csDesigning,									//!< Le composant est en train d'être modifié dans l'éditeur
	csAncestor,										//!< Le composant a été inséré dans une fenêtre ancêtre
	csUpdating,										//!< Le composant est en cours d'actualisation suite à modification dans la fenêtre ancêtre
	csFixups,											//!< Le composant est dépendant d'un autre et attend la fin de son chargement
	csFreeNotification,						//!< Demande de notification au moment de la libération d'autres composants
	csInline,											//!< Le composant peut être inséré dans une autre fenêtre
	csDesignInstance							//!< Objet principal de l'élément modifié dans l'éditeur
};

typedef Set<TComponentState, csLoading, csDesignInstance> TComponentStates;  //!< Combinaison de plusieurs TComponentState


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnHint.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnHint)(TObject *Sender);


//-----------------------------------------------------------------------------
//! @class TComponent
//!
//! @brief Définition de la classe TComponent
//!
//! Cette classe est la classe de base de tous les composants graphiques ou non
//! graphiques. Elle gère la hiérarchie des composants entre eux, chaque
//! composant appartient généralement à un autre composant qui est responsable
//! de sa destruction lorsque le parent est détruit. Cette classe implémente
//! également des fonctions de recherche à partir de différents critères
//! (identifiant, handle système, nom, ...).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TComponent: public TPersistent {
private:
  static WORD NumCommand;
protected:
  WORD FCommand;
  AnsiString FCaption;
  TComponent* FOwner;
  std::vector<TComponent *> FComponents;
  int FTag;
  TComponentStates FComponentState;


  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  bool DistributionCommand(WORD Command, WORD Notify);
  virtual bool ProcessCommand(WORD Notify) {return false;}
  HBRUSH DistributionColor(HWND hWnd);
  virtual HBRUSH ProcessColor(void) {return (HBRUSH) FALSE;}
  int DistributionNotify(int Command, LPNMHDR pnmh);
  virtual int ProcessNotify(LPNMHDR pnmh) {return FALSE;}
  virtual bool DistributionShortCut(int nVirtKey, TShiftState Shift);
  virtual bool ProcessShortCut(int nVirtKey, TShiftState Shift) {return false;}
  virtual bool DistributionMeasureItem(UINT idCtrl, LPMEASUREITEMSTRUCT lpMIS);
  virtual bool ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS) {lpMIS->itemWidth = 16; lpMIS->itemHeight = 16; return true;}
  virtual bool DistributionDrawItem(UINT idCtrl, LPDRAWITEMSTRUCT lpDIS);
  virtual bool ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS) {return false;}


public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TComponent(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TComponent(void);

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

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un composant enfant
	//!
	//! Cette méthode ajoute le composant passé en paramètres dans la liste des
	//! composants. L'objet conteneur devient propriétaire de du composant ainsi
	//! inséré. Il détruira automatiquement ses composants enfants lorsqu'il sera
	//! lui-même détruit. Ne détruisez surtout pas ce composant vous-même, ni
	//! avant ni après la destruction du conteneur, ça provoquerait un plantage.
  //!
  //! @param[in]        AComponent Adresse du composant à ajouter à la liste.
  //! @param[in]        Index Endroit de l'insertion (par défaut -1 = à la fin).
  //---------------------------------------------------------------------------

  virtual void InsertComponent(TComponent* AComponent, int Index = -1);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un composant enfant de la liste des composants
	//!
	//! Cette méthode enlève le composant passé en paramètres de la liste des
	//! composants. Ce composant n'est pas détruit. Comme il n'a plus de
	//! propriétaire, vous devrez soit le détruire explicitement, soit l'insérer
	//! à nouveau dans la liste d'un autre conteneur (ou du même à un autre
	//! endroit).
  //!
  //! @param[in]        AComponent Adresse du composant à retirer de la liste.
  //---------------------------------------------------------------------------

  virtual void RemoveComponent(TComponent* AComponent);

  //---------------------------------------------------------------------------
  //! @brief Initialise le composant après la lecture complète du fichier DFM.
  //---------------------------------------------------------------------------

  virtual void FASTCALL Loaded(void);

  //---------------------------------------------------------------------------
  //! @brief Indique que le composant et ses composants possédés vont être détruits.
  //---------------------------------------------------------------------------

  void FASTCALL Destroying(void);

  //---------------------------------------------------------------------------
  //! @brief Pointeur sur composant racine
  //---------------------------------------------------------------------------

  TComponent * FindRootComponent(void);

  //---------------------------------------------------------------------------
  //! @brief Pointeur sur composant à partir du nom
  //---------------------------------------------------------------------------

  TComponent * FindComponent(const AnsiString AName);

  //---------------------------------------------------------------------------
  //! @brief Pointeur sur composant à partir du numéro de commande
  //---------------------------------------------------------------------------

  TComponent * FindComponentFromId(WORD Id, bool bRecursif);

  //---------------------------------------------------------------------------
  //! @brief Pointeur sur composant à partir du handle
  //---------------------------------------------------------------------------

  TWinControl * FindComponentFromHWnd(HWND hWnd);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Caption
	//!
	//! Cette propriété définit le titre du contrôle, qui peut avoir différents
	//! effets suivant le type du contrôle: par exemple pour un @b TLabel il
	//! représente le texte lui-même, pour une fenêtre @b TForm le titre de la
	//! fenêtre, tandis que pour un @b TEdit cette propriété n'a aucun effet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComponent, AnsiString, Caption);


  //---------------------------------------------------------------------------
  //! @brief Propriété Owner: propriétaire de l'objet
	//!
	//! Cette propriété renvoie le propriétaire de l'objet @b this (objet chargé
	//! de sa destruction).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComponent, TComponent *, Owner);


  //---------------------------------------------------------------------------
  //! @brief Propriété Components[]: liste des composants
	//!
	//! Cette propriété permet de lire la liste des composants dont l'objet est
	//! propriétaire.
	//!
	//! @sa Owner
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TComponent, TComponent *, Components);

  //---------------------------------------------------------------------------
  //! @brief Propriété ComponentCount: nombre de composants
	//!
	//! Cette propriété permet de lire le nombre d'éléments de la liste
	//! @b Components.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TComponent, int, ComponentCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété ComponentIndex: index de composant
	//!
	//! Cette propriété permet de parcourir la liste des composants. Elle est
	//! utilisée de manière interne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComponent, int, ComponentIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété Command
	//!
	//! Cette propriété définit le numéro de commande associé au contrôle. Ce
	//! numéro est défini automatiquement. En général vous n'avez pas besoin de
	//! le connaitre, sauf pour effectuer une commande depuis un programme
	//! externe (bien que cette méthode soit peu fiable, le numéro pouvant
	//! changer lorsque vous ajoutez des composants dans votre dialogue).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TComponent, WORD, Command);

  //---------------------------------------------------------------------------
  //! @brief Propriété ComponentState
	//!
	//! Cette propriété définit l'état du composant. Elle permet d'éviter
	//! d'effectuer des action qui n'auraient pas de signification (un @b Show
	//! sur un composant qui est en train d'être détruit, par exemple)
  //!
  /*! @verbatim
      A la création, les composants prennent le même ComponentState que leur
      parent ou, s'il n'y a pas de parent, il est vide.
      Lorsqu'un composant (ou une fiche) est créé par programme, il a donc le
      ComponentState de son parent (en général vide).
      Lorsqu'une nouvelle fiche est créée avec un DFM ou un RC, elle prend
      l'état csLoading, et donc les composants qui vont être créés dedans
      aussi. Lorsque la création est finie, la méthode "loaded()" est appelée,
      ce qui remet l'état à zéro, et en cascade l'état des sous-composants
      également. @endverbatim */
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TComponent, TComponentStates, ComponentState);

  //---------------------------------------------------------------------------
  //! @brief Propriété Tag
	//!
	//! Cette propriété permet de définir une valeur quelconque associée au
	//! contrôle. Il n'est pas recommandé d'y stocker un pointeur sur une
	//! adresse, pour des raisons de compatibilité 32 - 64 bits.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComponent, int, Tag);

  //@}

};


#else  // TComponentH

class TComponent;

#endif  // TComponentH


#undef In_TComponentH
