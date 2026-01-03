//-----------------------------------------------------------------------------
//! @file TElemBase.h
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



//---------------------------------------------------------------------------

#ifndef TElemBaseH
#define TElemBaseH


//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------

#include "TControlSizer.h"


//---------------------------------------------------------------------------
// Pré-déclarations
//---------------------------------------------------------------------------

class TControlSizer_Collection;


//---------------------------------------------------------------------------
//! @class TElemBase
//!
//! @brief Classe de base des composants pouvant être construits avec l'objet
//! ConstrForm
//!
//! @author Denis ROBERT
//!
//---------------------------------------------------------------------------

class TElemBase: public TPersistent {
private:
protected:
  int FLeft;
  int FTop;
  int FWidth;
  int FHeight;
  TAnchors FAnchors;
  TElemBase *FParent;
  bool FParentPotentiel;
  std::vector<TElemBase *> FComponents;
  TControlSizer *FControlSizer;
	bool UpdateInfos(void);

public:

	// Pointeur partagé (par chaque composant de la form) sur l'ensemble des contrôleurs
	// Il faut utiliser un shared_ptr car l'ordre de destruction est difficilement prévisible
	std::shared_ptr<TControlSizer_Collection> shared_ptr_ControlSizer_Collection;

  FVCL_BEGIN_EVENTS
  void FASTCALL ControlSizerXChange(TObject *Sender, int NewX);
  void FASTCALL ControlSizerYChange(TObject *Sender, int NewY);
  void FASTCALL ControlSizerWidthChange(TObject *Sender, int NewWidth);
  void FASTCALL ControlSizerHeightChange(TObject *Sender, int NewHeight);
  void FASTCALL ControlSizerAnchorsChange(TObject *Sender, TAnchors NewAnchors);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_2(TElemBase, TOnXChange, OnXChange, ControlSizerXChange, TObject*, int);
    SET_EVENT_STR_2(TElemBase, TOnYChange, OnYChange, ControlSizerYChange, TObject*, int);
    SET_EVENT_STR_2(TElemBase, TOnWidthChange, OnWidthChange, ControlSizerWidthChange, TObject*, int);
    SET_EVENT_STR_2(TElemBase, TOnHeightChange, OnHeightChange, ControlSizerHeightChange, TObject*, int);
    SET_EVENT_STR_2(TElemBase, TOnAnchorsChange, OnAnchorsChange, ControlSizerAnchorsChange, TObject*, TAnchors);
  }

	FVCL_END_EVENTS

  //! @name Constructeurs / destructeurs
  //@{


  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
  TElemBase(TElemBase *ElemParent);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
  virtual ~TElemBase(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Régénération d'un objet
  //---------------------------------------------------------------------------
  virtual void Invalidate() = 0;

  //---------------------------------------------------------------------------
  //! @brief Mise à jour des informations
	//!
	//! Cette méthode met à jour les informations de l'objet suite à une
	//! modification des propriétés de base (nom, position, dimensions). Ces
	//! informations sont généralement affichées dans une info-bulle.
  //!
  //! @param[in]        asInfos Informations sous forme de chaîne.
  //---------------------------------------------------------------------------
	virtual bool UpdateInfos(AnsiString asInfos) = 0;

  //---------------------------------------------------------------------------
  //! @brief Visualisation d'un objet
  //!
  //! @param[in]        bVisible true pour rendre l'objet visible
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Show(bool bVisible) = 0;

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //!
  //! @param[in]        asTypeACreer Type de l'objet à créer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Create(AnsiString asTypeACreer) = 0;

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet enfant
  //!
  //! @return @c bool Adresse du nouvel élément, ou NULL si échec
  //---------------------------------------------------------------------------
  virtual TElemBase *NewChild(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet enfant
  //!
  //! @param[in]        asTypeACreer Type de l'objet à créer
  //!
  //! @return @c bool Adresse du nouvel élément, ou NULL si échec
  //---------------------------------------------------------------------------
  virtual TElemBase *NewChild(AnsiString asTypeACreer);

  //---------------------------------------------------------------------------
  //! @brief Echange de deux composants
  //!
  //! @param[in]  Element1 Adresse du premier composant à échanger.
  //! @param[in]  Element2 Adresse du second composant à échanger.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool SwapComponents(TElemBase *Element1, TElemBase *Element2);

  //---------------------------------------------------------------------------
  //! @brief Met à jour la taille du TControlSizer
  //!
  //! Cette fonction met à jout les propriétés Left, Top, Width et Height de
	//! l'objet TControlSizer. Elle doit être appelée suite à un
	//! redimensionnement de l'objet parent qui peut entrainer une modification
	//! de ces données en fonction des ancrages et des alignements.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool UpdateControlSizer(void);

  //---------------------------------------------------------------------------
  //! @brief liste des propriétés
  //!
  //! @param[in]        ListeProperties Liste à remplir
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListeProperties) = 0;

  //---------------------------------------------------------------------------
  //! @brief lecture du type d'une propriété
  //!
  //! @param[in]        asProperty Propriété à lire
  //! @param[out]       asInfos Choix si type == tcChoix ou tcChoixMultiple
  //!
  //! @return @c TYPEPROPERTY Type de la propriété
  //---------------------------------------------------------------------------
  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) = 0;

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Propriété
  //!
  //! @return @c std::string Valeur par défaut de la propriété
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty) = 0;

  //---------------------------------------------------------------------------
  //! @brief lecture d'une propriété
  //!
  //! @param[in]        asProperty Propriété à lire
  //!
  //! @return @c AnsiString Propriété lue
  //---------------------------------------------------------------------------
  virtual AnsiString GetProperty(AnsiString asProperty) = 0;

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une propriété
  //!
  //! @param[in]        asProperty Propriété à écrire
  //! @param[out]       asValue Valeur de la propriété
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue) = 0;

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Type
	//!
	//! Cette propriété détermine le type (classe) du composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TElemBase, AnsiString, Type);

  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété détermine le nom (propriété @b Name) du composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, AnsiString, Name);

  //---------------------------------------------------------------------------
  //! @brief Propriété X
	//!
	//! Cette propriété représente les coordonnées en X du composant (pixels par
	//! rapport au dialogue).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TElemBase, int, X);

  //---------------------------------------------------------------------------
  //! @brief Propriété Y
	//!
	//! Cette propriété représente les coordonnées en Y du composant (pixels par
	//! rapport au dialogue).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TElemBase, int, Y);

  //---------------------------------------------------------------------------
  //! @brief Propriété Left
	//!
	//! Cette propriété représente les coordonnées en X du composant (pixels par
	//! rapport à la zone client du parent).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, int, Left);

  //---------------------------------------------------------------------------
  //! @brief Propriété Top
	//!
	//! Cette propriété représente les coordonnées en Y du composant (pixels par
	//! rapport à la zone client du parent).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, int, Top);

  //---------------------------------------------------------------------------
  //! @brief Propriété Width
	//!
	//! Cette propriété représente la largeur du composant en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété représente la hauteur du composant en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Propriété ClientRect
	//!
	//! Cette propriété représente le rectangle client du composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TElemBase, TRect, ClientRect);

  //---------------------------------------------------------------------------
  //! @brief Propriété Anchors
	//!
	//! Cette propriété permet de lire et de définir les ancrages.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, TAnchors, Anchors);

  //---------------------------------------------------------------------------
  //! @brief Propriété Parent
	//!
	//! Cette propriété définit le contrôle parent.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, TElemBase *, Parent);

  //---------------------------------------------------------------------------
  //! @brief Propriété ParentPotentiel
	//!
	//! Cette propriété est à @b true si le composant peut être un parent.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, bool, ParentPotentiel);

  //---------------------------------------------------------------------------
  //! @brief Propriété Components
	//!
	//! Cette propriété permet de lire tous les composants possédés par le
	//! composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TElemBase, TElemBase *, Components);

  //---------------------------------------------------------------------------
  //! @brief Propriété ComponentCount
	//!
	//! Cette propriété permet de lire le nombre de composants.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TElemBase, int, ComponentCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété ControlSizer
	//!
	//! Cette propriété permet de lire le contrôle de redimensionnement associé
	//! au composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TElemBase, TControlSizer *, ControlSizer);

  //---------------------------------------------------------------------------
  //! @brief Propriété Selected
	//!
	//! Cette propriété permet de lire et de définir l'état sélectionné du
	//! composant (le contrôle de redimensionnement associé devient visible pour
	//! les composants sélectionnés).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TElemBase, bool, Selected);

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


//---------------------------------------------------------------------------
#endif
