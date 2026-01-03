//-----------------------------------------------------------------------------
//! @file TMenuItem.h
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

#ifdef In_TMenuItemH
#error "Inclusion circulaire de TMenuItem"
#endif // In_TMenuItemH

#define In_TMenuItemH

#ifndef TMenuItemH
#define TMenuItemH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH

#ifndef TImageListH
#include "TImageList.h"
#endif  // TImageListH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TYPEMENU {
	TM_MainMenu = 1,              //!< Menu de fenêtre
	TM_Popup                      //!< Menu contextuel
};

enum TMenuBreak {
	mbNone,                       //!< Aucune séparation
	mbBreak,                      //!< Séparation horizontale
	mbBarBreak                    //!< Séparation verticale
};


//-----------------------------------------------------------------------------
//! @class TMenuItem
//!
//! @brief Définition de la classe TMenuItem
//!
//! Cette classe représente un élément de menu principal ou de menu popup.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMenuItem: public TComponent {
private:
protected:
	HMENU FHandle;
	TMenuBreak FBreak;
	bool FAutoCheck;
	bool FChecked;
	bool FEnabled;
	bool FRightJustify;
	TShortCut FShortCut;
	TImageIndex FImageIndex;
	TImageList * FSubMenuImages;
	TYPEMENU TypeMenu;
	TAction *FAction;

	//---------------------------------------------------------------------------
	//! @brief Traitement des messages Windows
	//---------------------------------------------------------------------------

	virtual bool ProcessCommand(WORD Notify);
	virtual bool ProcessShortCut(int nVirtKey, TShiftState Shift);
	virtual bool ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual bool ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS);

	// Ajout dans le menu Windows
	void AppendMenu(TMenuItem* MenuItem);

	bool UpdateMenuText(void);

public:
	FVCL_BEGIN_EVENTS
	void FASTCALL ActionUpdate(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TMenuItem, TOnUpdate, OnUpdate, ActionUpdate, TObject*);
  }

	FVCL_END_EVENTS

	//! @name Constructeurs et destructeur
  //@{

	//---------------------------------------------------------------------------
	//! @brief Constructeur
	//---------------------------------------------------------------------------

	TMenuItem(TComponent* AOwner);

	//---------------------------------------------------------------------------
	//! @brief Destructeur
	//---------------------------------------------------------------------------

	virtual ~TMenuItem(void);

  //@}


	//! @name Méthodes
	//@{

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Caption
	//!
	//! Cette méthode permet d'écrire la propriété Caption. Si une action est
	//! associé à cet élément de menu, le libellé du menu prend automatiquement
	//! le libellé de l'action si le libellé est vide. L'affectation de la
	//! propriété @b Caption aura donc pour effet de faire perdre cette
	//! correspondance.@b
	//! Le signe "&" aura pour effet de souligner le caractère suivant, ce qui
	//! indique le raccourci clavier à utiliser pour lancer la commande sans
	//! utiliser la souris. Si vous souhaitez réellement afficher le signe "&"
	//! dans le libellé du menu, entrez le deux fois : "&&".
	//!
	//! @param[in]				NewCaption Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Caption(AnsiString NewCaption);

	//---------------------------------------------------------------------------
	//! @brief Liste des propriétés publiées
	//!
	//! @param[in, out] 	ListProperties Liste des propriétés de l'objet
	//!
	//! @sa TPersistent::GetListProperties
	//---------------------------------------------------------------------------
	virtual void GetListProperties(TStrings *ListProperties);

	//---------------------------------------------------------------------------
	//! @brief Type des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété
	//! @param[out] 			asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
	//!
	//! @return @c TYPEPROPERTY Type de la propriété concernée.
	//!
	//! @sa TPersistent::GetTypeProperty
	//---------------------------------------------------------------------------

	virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

	//---------------------------------------------------------------------------
	//! @brief Valeur par défaut d'une propriété
	//!
	//! @param[in]				asProperty Nom de la propriété
	//!
	//! @return @c AnsiString Valeur par défaut de la propriété.
	//!
	//! @sa TPersistent::GetDefaultProperty
	//---------------------------------------------------------------------------

	virtual AnsiString GetDefaultProperty(AnsiString asProperty);

	//---------------------------------------------------------------------------
	//! @brief Lecture des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété à lire
	//!
	//! @return @c AnsiString Valeur de la propriété.
	//---------------------------------------------------------------------------

	virtual AnsiString GetProperty(AnsiString asProperty);

	//---------------------------------------------------------------------------
	//! @brief Affectation des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété à enregistrer
	//! @param[in]				asValue Valeur de la propriété à enregistrer
	//! @param[in]				Sender Boîte de dialogue (utilisé par les évènements)
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
													 TPersistent *Sender);

	//---------------------------------------------------------------------------
	//! @brief Ajout d'une ligne de menu
	//!
	//! Cette méthode permet d'ajouter une ligne de menu ou un sous-menu.
	//!
	//! @param[in]				MenuItem Ligne à ajouter
	//---------------------------------------------------------------------------

	void Add(TMenuItem* MenuItem);

	//---------------------------------------------------------------------------
	//! @brief Suppression de toutes les lignes du menu
	//!
	//! Cette méthode permet de supprimer toutes les lignes de menu et de ses 
	//! sous-menu.
	//---------------------------------------------------------------------------

	void Clear(void);

	//---------------------------------------------------------------------------
	//! @brief Suppression d'une ligne de menu
	//!
	//! Cette méthode permet de supprimer une ligne de menu ou un sous-menu.
	//!
	//! @param[in]				Index Ligne à supprimer
	//---------------------------------------------------------------------------

	void Delete(int Index);

	//---------------------------------------------------------------------------
	//! @brief Construction effective du menu
	//---------------------------------------------------------------------------

	void BuildMenu();

	//@}

	//! @name Propriétés
	//@{

	//---------------------------------------------------------------------------
	//! @brief Propriété Action: action déclenchée automatiquement.
	//! 
	//! Cette propriété permet de définir une action déclenchée automatiquement
	//! lorsque l'utilisateur clique sur un élément du menu. L'action est
	//! déclenchée juste avant l'évènement OnClick. Les propriétés @b Caption,
	//! @b Checked, @b Enabled de l'objet Action sont automatiquement répercutées
	//! sur les composants associés (à certaines conditions, voir chaque
	//! propriété pour plus de détails).
	//!
	//! @sa TAction::Caption, TAction::Checked, TAction::Enabled
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, TAction *, Action);

	//---------------------------------------------------------------------------
	//! @brief Propriété AutoCheck: gestion automatique de l'état @b Checked.
	//! 
	//! Cette propriété permet de cocher ou de décocher automatiquement un
	//! élément de menu lorsqu'on clique dessus. Si une action est associée à cet
	//! élément, le fait d'activer la propriété @b AutoCheck désactive la
	//! synchronisation avec l'objet @b TAction. Si vous souhaitez garder cette
	//! synchronisation, activez la propriété @b AutoCheck de l'action et
	//! désactivez-la pour cet objet.
	//!
	//! @sa Checked, TAction::AutoCheck
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, bool, AutoCheck);

	//---------------------------------------------------------------------------
	//! @brief Propriété Break: Séparateur vertical.
	//! 
	//! Cette propriété permet de définir un élément du menu comme un séparateur
	//! vertical. Le sous-menu apparait donc sur plusieurs colonnes.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, TMenuBreak, Break);

	//---------------------------------------------------------------------------
	//! @brief Propriété Checked: marque cochée.
	//! 
	//! Cette propriété permet de cocher un élément du menu. Lorsque l'élément
	//! est coché, une croix apparait automatiquement à gauche du libellé. Si
	//! une action est associé à cet élément de menu, l'état @b Checked de
	//! l'action est automatiquement répercuté sur ce composant.
	//!
	//! @sa AutoCheck, TAction::Checked
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, bool, Checked);

	//---------------------------------------------------------------------------
	//! @brief Propriété Count: nombre de sous-menus.
	//! 
	//! Cette propriété permet de lire le nombre de sous-menus de l'élément de
	//! menu (nombre d'éléments de la propriété TMenuItem::Items).
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TMenuItem, int, Count);

	//---------------------------------------------------------------------------
	//! @brief Propriété Enabled: activation.
	//! 
	//! Cette propriété permet de désactiver ou d'activer un élément du menu. Un
	//! élément désactivé apparait automatiquement grisé. Si une action est
	//! associée à cet élément, l'état @b Enabled de l'action est automatiquement
	//! répercuté sur ce composant.
	//!
	//! @sa TAction::Enabled
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, bool, Enabled);

	//---------------------------------------------------------------------------
	//! @brief Propriété Handle: handle Windows
	//! 
	//! Cette propriété permet d'obtenir le handle Windows de l'élément du menu.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, HMENU, Handle);

	//---------------------------------------------------------------------------
	//! @brief Propriété ImageIndex: Index de l'image.
	//! 
	//! Cette propriété permet de définir une image à afficher à gauche du
	//! libellé du menu. Il faut pour cela que la propriété
	//! TMenuItem::SubMenuImages soit définie. La première image de la liste
	//! a le numéro 0, la valeur -1 signifie qu'il ne faut pas afficher d'image
	//! pour cet item.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, TImageIndex, ImageIndex);

	//---------------------------------------------------------------------------
	//! @brief Propriété Items: éléments du menu.
	//! 
	//! Cette propriété permet d'accéder aux éléments du menu.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TMenuItem, TMenuItem *, Items);

	//---------------------------------------------------------------------------
	//! @brief Propriété RightJustify: élément aligné à droite.
	//! 
	//! Cette propriété permet d'aligner un élément du menu principal sur le bord
	//! droit de la fenêtre. Elle n'a aucun effet sur les éléments de sous-menu.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, bool, RightJustify);

	//---------------------------------------------------------------------------
	//! @brief Propriété ShortCut: raccourci clavier.
	//! 
	//! Cette propriété permet d'affecter un raccourci clavier à un élément de
	//! menu. L'appui sur ce raccourci aura le même effet qu'un clic. Le texte du
	//! raccourci est automatiquement affiché à droite du libellé.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, TShortCut, ShortCut);

	//---------------------------------------------------------------------------
	//! @brief Propriété SubMenuImages: Liste d'images
	//! 
	//! Cette propriété permet de définir la liste d'image à utiliser pour
	//! afficher des images dans les menus. Utilisez ensuite la propriété
	//! TMenuItem::ImageIndex pour déterminer quelle image dans la liste il faut
	//! afficher.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenuItem, TImageList *, SubMenuImages);

	//@}

	//! @name Evènements
	//@{

	//---------------------------------------------------------------------------
	//! @brief Evènement OnClick: click sur l'élément du menu
	//! 
	//! Cet évènement est déclenché lorsque l'utilisateur clique sur l'élément
	//! du menu.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//---------------------------------------------------------------------------

	DECLARE_EVENT_1(TNotifyEvent, OnClick, TObject *);


	//@}


};


#else  // TMenuItemH

class TMenuItem;

#endif	// TMenuItemH


#undef In_TMenuItemH
