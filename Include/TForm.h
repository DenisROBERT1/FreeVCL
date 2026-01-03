//-----------------------------------------------------------------------------
//! @file TForm.h
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

#ifdef In_TFormH
#error "Inclusion circulaire de TForm"
#endif // In_TFormH

#define In_TFormH

#ifndef TFormH
#define TFormH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TFormFrame.h"


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define mrNone     0              //!< Aucun bouton
#define mrOk       IDOK           //!< Bouton "Ok"
#define mrCancel   IDCANCEL       //!< Bouton "Annuler"
#define mrAbort    IDABORT        //!< Bouton "Abandonner"
#define mrRetry    IDRETRY        //!< Bouton "Réessayer"
#define mrIgnore   IDIGNORE       //!< Bouton "Ignorer"
#define mrYes      IDYES          //!< Bouton "Oui"
#define mrNo       IDNO           //!< Bouton "Non"
#define mrAll      mrNo + 1       //!< Bouton "Tous"
#define mrNoToAll  mrAll + 1      //!< Bouton "Non pour tous"
#define mrYesToAll mrNoToAll + 1  //!< Bouton "Oui pour tous"

//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TBorderIcon {
	biSystemMenu,									//!< Présence de l'icône système (menu déplacer, réduire, fermer, ...)
	biMinimize,										//!< Présence de l'icône permettant de réduire la fenêtre
	biMaximize,										//!< Présence de l'icône permettant de mettre la fenêtre en plein écran
	biHelp												//!< Présence de l'icône permettant d'afficher l'aide
};

typedef Set<TBorderIcon, biSystemMenu, biHelp> TBorderIcons;  //!< Combinaison de plusieurs TBorderIcon

enum TPosition {
	poDesigned,										//!< Fenêtre positionnée et dimensionnée à l'endroit défini lors de la conception
	poDefault,										//!< Fenêtre positionnée et dimensionnée aux valeurs par défaut définie par le système
	poDefaultPosOnly,							//!< Fenêtre positionnée à la position par défaut définie par le système
	poDefaultSizeOnly,						//!< Fenêtre dimensionnée à la dimension par défaut définie par le système
  poScreenCenter,								//!< Fenêtre positionnée au centre de l'écran
	poDesktopCenter,							//!< Fenêtre positionnée au centre du bureau (différent du centre de l'écran si plusieurs moniteurs)
  poMainFormCenter,							//!< Fenêtre positionnée au centre de la fenêtre principale de l'application
	poOwnerFormCenter							//!< Fenêtre positionnée au centre de la fenêtre propriétaire
};

enum TWindowState {
	wsNormal,											//!< Position normale
	wsMinimized,									//!< Fenêtre réduite (mise en icône)
	wsMaximized										//!< Fenêtre agrandie (en plein écran)
};

enum TFormState_1 {
	fsCreating,
	fsVisible,
	fsShowing,
	fsModal,
	fsCreatedMDIChild,
	fsActivated
};

typedef Set<TFormState_1, fsCreating, fsActivated>  TFormState;

enum TFormStyle {
	fsNormal,											//!< Fenêtre normale
	fsMDIChild,										//!< Fenêtre enfant multi-documents
	fsMDIForm,										//!< Fenêtre principale multi-documents
	fsStayOnTop										//!< Fenêtre toujours au premier plan
};

enum TShowWindowOption {
	swShow,												//!< Fenêtre visible
	swShowDefault,								//!< Visibilité définie à sa valeur par défaut (comme lors du premier affichage)
	swShowMinNoActivate,					//!< Fenêtre réduite et non active
	swShowNA,											//!< Fenêtre visible et non activée
	swShowNoActivate,							//!< Visibilité définie à sa valeur par défaut et non active
	swShowNormal		 							//!< Fenêtre visible et activée. Si la fenêtre avait l'état MINIMIZE ou MAXIMIZE, cet état est restauré.
};

enum TCloseAction {
	caNone,												//!< Aucune action
	caHide,												//!< La fenêtre est cachée mais existe encore
	caFree												//!< La fenêtre est fermée et les ressources libérées
};

//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnCreate.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnCreate)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnActivate.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnActivate)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDeactivate.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDeactivate)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnShortCut.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnShortCut)(TObject *Sender, int VirtKey, TShiftState Shift, bool &Handled);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnClose.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnClose)(TObject *Sender, TCloseAction &Action);


//-----------------------------------------------------------------------------
//! @class TForm
//!
//! @brief Définition de la classe TForm
//!
//! Cette classe est la classe de base d'une fenêtre de dialogue. Elle possède
//! des propriétés permettant de positionner et redimensionner la fenêtre, de
//! modifier le style, et de gérer la hiérarchie entre les fenêtres.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TForm: public TFormFrame {
private:
protected:
	Byte FAlphaBlendValue;
  int FModalResult;
  TPosition FPosition;
  TWindowState FWindowState;
	bool FFormActivation;
	TFormState FFormState;
  TFormStyle FFormStyle;
  TBorderIcons FBorderIcons;
  TFormBorderStyle FBorderStyle;
  TCloseAction FDefaultCloseAction;
  TMainMenu *FMenu;
  HWND FHandleParent;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual void ProcessCreate(void);
  virtual bool ProcessActivateApp(bool bActive);
  virtual bool ProcessActivate(int fActive);
  virtual bool ProcessPaint(HDC hdc, RECT rcPaint);
  virtual bool ProcessMove(int X, int Y);
  virtual bool ProcessSize(int SizeType, int L, int H);
  virtual bool ProcessShortCut(int nVirtKey, TShiftState Shift);
  virtual bool ProcessClose(void);
  virtual void ProcessDestroy(void);

  //---------------------------------------------------------------------------
  //! @brief Création de la fenêtre
  //---------------------------------------------------------------------------

  bool CreateForm(const TCHAR * szName, TComponent *ListComponents[]);

public:

	//! @name Constructeurs et destructeur
  //@{

	//---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TForm(HWND HandleParent);
  TForm(TComponent* AOwner);
  TForm(HWND HandleParent, const TCHAR * szName, TComponent* ListComponents[]);
  TForm(TComponent* AOwner, const TCHAR * szName, TComponent* ListComponents[]);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TForm(void);

  //@}

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Left
  //!
  //! Cette méthode redéfinit la lecture de la propriété Left
  //!
  //! @return @c int Propriété Left
	//!
	//! @note Les propriétés @b Left, @b Top, @b Width et @b Height sont définies
	//! pour une taille standard des bordures de fenêtre et de la barre de titre.
	//! La position et la taille réelles peuvent être légèrement différentes en
	//! fonction du thème, afin que la zone client soit indépendante du thème.
  //---------------------------------------------------------------------------

  virtual int Get_Left(void);


	//---------------------------------------------------------------------------
  //! @brief Setter de la propriété Left
  //!
  //! Cette méthode redéfinit l'écriture de la propriété Left
  //!
  //! @param[in]        NewLeft Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
	//!
	//! @sa TForm::Get_Left
  //---------------------------------------------------------------------------

  virtual bool Set_Left(int NewLeft);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Top
  //!
  //! Cette méthode redéfinit la lecture de la propriété Top
  //!
  //! @return @c int Propriété Top
	//!
	//! @sa TForm::Get_Left
  //---------------------------------------------------------------------------

  virtual int Get_Top(void);


	//---------------------------------------------------------------------------
  //! @brief Setter de la propriété Top
  //!
  //! Cette méthode redéfinit l'écriture de la propriété Top
  //!
  //! @param[in]        NewTop Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
	//!
	//! @sa TForm::Get_Left
  //---------------------------------------------------------------------------

  virtual bool Set_Top(int NewTop);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Width
  //!
  //! Cette méthode redéfinit la lecture de la propriété Width
  //!
  //! @return @c int Propriété Width
	//!
	//! @sa TForm::Get_Left
  //---------------------------------------------------------------------------

  virtual int Get_Width(void);


	//---------------------------------------------------------------------------
  //! @brief Setter de la propriété Width
  //!
  //! Cette méthode redéfinit l'écriture de la propriété Width
  //!
  //! @param[in]        NewWidth Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
	//!
	//! @sa TForm::Get_Left
  //---------------------------------------------------------------------------

  virtual bool Set_Width(int NewWidth);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Height
  //!
  //! Cette méthode redéfinit la lecture de la propriété Height
  //!
  //! @return @c int Propriété Height
	//!
	//! @sa TForm::Get_Left
  //---------------------------------------------------------------------------

  virtual int Get_Height(void);


	//---------------------------------------------------------------------------
  //! @brief Setter de la propriété Height
  //!
  //! Cette méthode redéfinit l'écriture de la propriété Height
  //!
  //! @param[in]        NewHeight Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
	//!
	//! @sa TForm::Get_Left
  //---------------------------------------------------------------------------

  virtual bool Set_Height(int NewHeight);


	//---------------------------------------------------------------------------
  //! @brief Setter de la propriété Visible
  //!
  //! Cette méthode permet d'écrire la propriété Visible
  //!
  //! @param[in]        NewVisible Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Visible(bool NewVisible);

	//---------------------------------------------------------------------------
	//! @brief Dimensionnement du contrôle
	//!
	//! Cette méthode redéfinit la méthode de l'objet TWinControl.
	//!
	//! @param[in]				NewLeft Nouvelle valeur de la propriété Left
	//! @param[in]				NewTop Nouvelle valeur de la propriété Top
	//! @param[in]				NewWidth Nouvelle valeur de la propriété Width
	//! @param[in]				NewHeight Nouvelle valeur de la propriété Height
	//! @param[in]				bForceResize Force le redimensionnement (même si NewLeft == left, ...)
	//!
	//! @sa TWinControl::Size, AdjustClientRect
	//---------------------------------------------------------------------------

	virtual void Size(int NewLeft, int NewTop, int NewWidth, int NewHeight, bool bForceResize);

	//---------------------------------------------------------------------------
  //! @brief Rend visible la fenêtre.
  //!
  //! Cette méthode permet de rendre visible la fenêtre, et de l'amener au
	//! premier plan (au dessus des autres).
  //---------------------------------------------------------------------------

	virtual void FASTCALL Show(void);

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
  //! @brief Recherche des fenêtres enfants interceptant la touche tabulation
  //!
  //! Cette méthode recherche si des fenêtres enfants ont demandé à recevoir
	//! les messages d'appui sur la touche tabulation.
  //!
  //! @return @c TComponent * Composant enfant ayant demandé l'interception.
  //---------------------------------------------------------------------------

  virtual TComponent * ChildWantTabs(void);

	//---------------------------------------------------------------------------
  //! @brief Recherche des fenêtres enfants interceptant la touche entrée
  //!
  //! Cette méthode recherche si des fenêtres enfants ont demandé à recevoir
	//! les messages d'appui sur la touche entrée.
  //!
  //! @return @c TComponent * Composant enfant ayant demandé l'interception.
  //---------------------------------------------------------------------------

  virtual TComponent * ChildWantReturns(void);

	//---------------------------------------------------------------------------
  //! @brief Interception des messages système (tabulations, ...)
  //!
  //! Cette méthode recherche gère la ventilation des messages systèmes en
	//! fonction des paramètres de chaque composant. N'appelez pas cette méthode,
	//! elle est uniquement utilisée en interne.
  //!
  //! @param[in]        msg Message système
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  bool InterceptionMessage(MSG &msg);

	//---------------------------------------------------------------------------
  //! @brief Interception de la touche "Tabulation"
  //!
  //! Cette méthode recherche gère l'appui sur la touche tabulation. N'appelez
	//! pas cette méthode, elle est uniquement utilisée en interne.
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  bool TraitementTabulation();

	//---------------------------------------------------------------------------
  //! @brief Interception de la touche "Entrée"
  //!
  //! Cette méthode recherche gère l'appui sur la touche entrée. N'appelez
	//! pas cette méthode, elle est uniquement utilisée en interne.
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  bool TraitementEntree();

	//---------------------------------------------------------------------------
  //! @brief Interception de la touche "Echappement"
  //!
  //! Cette méthode recherche gère l'appui sur la touche d'échappement.
	//! N'appelez pas cette méthode, elle est uniquement utilisée en interne.
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  bool TraitementEscape();

	//---------------------------------------------------------------------------
  //! @brief Affecte le focus à un contrôle.
  //!
  //! Cette méthode affecte le focus à un contrôle. Cela equivaut à faire
	//! Control->SetFocus().
  //!
  //! @param[in]        Control Contrôle à activer
  //---------------------------------------------------------------------------

	void FASTCALL FocusControl(TWinControl* Control);

	//---------------------------------------------------------------------------
  //! @brief Visualisation de la fenêtre en mode modal.
  //!
  //! Cette méthode affiche la fenêtre en mode modal (c'est à dire que les
	//! autres fenêtres du programme sont désactivées jusqu'à la fermeture de
	//! cette fenêtre).@n
	//! Si vous désirez créer une fenêtre non modale, utilisez la méthode Show.
	//!
	//! @sa Show
  //---------------------------------------------------------------------------

  TModalResult ShowModal(void);

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction de la fiche
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

  //---------------------------------------------------------------------------
  //! @brief Ordre de fermeture de la fiche
  //---------------------------------------------------------------------------

  bool Close(void);


  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Active
	//!
	//! Cette propriété détermine si la fenêtre est active, c'est à dire qu'elle
	//! reçoit les entrées claviers (et qu'un de ses composants a le focus). Dans
	//! ce cas, la barre de titre est représentée d'une autre couleur (variable
	//! suivant le style utilisé).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TForm, bool, Active);

  //---------------------------------------------------------------------------
  //! @brief Propriété ActiveControl
	//!
	//! Cette propriété définit le contrôle qui a le focus.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TWinControl *, ActiveControl);

  //---------------------------------------------------------------------------
  //! @brief Propriété AlphaBlend
	//!
	//! Cette propriété permet d'avoir une fenêtre transparente ou
	//! semi-transparente selon la valeur de la propriété @b AlphaBlendValue.
	//!
	//! @sa AlphaBlendValue
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, bool, AlphaBlend);

  //---------------------------------------------------------------------------
  //! @brief Propriété AlphaBlendValue
	//!
	//! Cette propriété définit le degré de transparence de la fenêtre. Pour que
	//! cette transparence soit effective, positionnez la propriété @b AlphaBlend
	//! à @b true. La valeur peut aller de 0 (fenêtre complètement transparente)
	//! à 255 (complètement opaque).
	//!
	//! @sa AlphaBlend
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY(TForm, Byte, AlphaBlendValue);

  //---------------------------------------------------------------------------
  //! @brief Propriété ModalResult
	//!
	//! Cette propriété représente la valeur de retour de la fenêtre. C'est cette
	//! valeur qui est retournée par la méthode @b ShowModal. Le fait d'affecter
	//! une valeur différente de zéro provoque automatiquement la fermeture de la
	//! fenêtre, on le fait habituellement dans le traitement de l'évènement
	//! @b OnClick du bouton "Ok" ou "Annuler".
	//!
	//! @sa ShowModal
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TModalResult, ModalResult);

  //---------------------------------------------------------------------------
  //! @brief Propriété Position
	//!
	//! Cette propriété permet de positionner la fenêtre au centre de l'écran ou
	//! de la fenêtre parente.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TPosition, Position);

  //---------------------------------------------------------------------------
  //! @brief Propriété WindowState
	//!
	//! Cette propriété définit l'état de la fenêtre (normal, en plein écran, ou
	//! en icône).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TWindowState, WindowState);


  //---------------------------------------------------------------------------
  //! @brief Propriété FormActivation
	//!
	//! Cette propriété permet, lorsqu'elle est à @b false, d'avoir une fenêtre
	//! qui ne prend pas le focus quand on clique dessus.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, bool, FormActivation);


	//---------------------------------------------------------------------------
	//! @brief Propriété FormState
	//!
	//! Cette propriété définit l'état de la fenêtre.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TForm, TFormState, FormState);


  //---------------------------------------------------------------------------
  //! @brief Propriété FormStyle
	//!
	//! Cette propriété définit le style de la fenêtre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TFormStyle, FormStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété BorderIcons
	//!
	//! Cette propriété définit les boutons présents dans la barre de titre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TBorderIcons, BorderIcons);


	//---------------------------------------------------------------------------
  //! @brief Propriété BorderStyle
	//!
	//! Cette propriété définit le type de bordure de la fenêtre, ce qui
	//! détermine également si elle est redimensionnable ou pas.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TFormBorderStyle, BorderStyle);


	//---------------------------------------------------------------------------
  //! @brief Propriété DefaultCloseAction: Action par défaut sur l'évènement OnClose
	//!
	//! Cette propriété définit l'option par défaut lorsqu'intervient l'évènement
	//! OnClose. Cette propriété est à caFree pour les fenêtres normales, mais
	//! elle est forcée à caHide si vous faites un ShowModal. Ceci permet de
	//! lire les propriétés des composants après la fermeture de la fenêtre. Car
	//! dans le cas contraire, la fenêtre Windows est détruite (même si l'objet
	//! TForm existe toujours), et il est donc impossible de lire certaines
	//! propriétés de composants liés aux composants Windows sous-jacents.@n
	//! Cette propriété ne définit que l'action par défaut pour vous éviter de
	//! traiter l'évènement OnClose. Dans tous les cas, vous pouvez traiter cet
	//! évènement pour définir comme vous le désirez l'action à effectuer.
	//!
	//! @sa TForm::OnClose
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TCloseAction, DefaultCloseAction);


  //---------------------------------------------------------------------------
  //! @brief Propriété Menu
	//!
	//! Cette propriété définit le menu de la fenêtre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm, TMainMenu *, Menu);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnCreate
	//!
	//! Cet évènement est déclenché à la création de la fenêtre.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnCreate, OnCreate, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnActivate
	//!
	//! Cet évènement est déclenché lors de l'activation de la fenêtre (c'est à
	//! dire avant de recevoir le focus).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnActivate, OnActivate, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDeactivate
	//!
	//! Cet évènement est déclenché lors de la désactivation d'une fenêtre (après
	//! la perte du focus).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnDeactivate, OnDeactivate, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnPaint
	//!
	//! Cet évènement est déclenché lorsque la fenêtre a besoin d'être
	//! redessinée. Vous pouvez intercepter cet évènement pour afficher les
	//! éléments que vous désirez (utiliser pour cela l'objet TCanvas).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//!
	//! @sa TCanvas
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnPaint, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnShortCut
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur appuie sur une touche
	//! ou une combinaison de touches. Vous pouvez intercepter cet évènement pour
	//! effectuer un traitement particulier sur un raccourci donné. Si vous
	//! positionnez le flag Handled à true, le traitement par défaut lié au
	//! raccourci (défini dans les éléments de menu, par exemple) ne sera pas
	//! effectué.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Code de la touche utilisée
  //! @param[in]        Arg3 Etat des touches ALT, SHIFT et CONTROL
  //! @param[in, out]   Arg4 Flag Handled à éventuellement positionner à true
  //---------------------------------------------------------------------------

  DECLARE_EVENT_4(TOnShortCut, OnShortCut, TObject *, int, TShiftState, bool &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnClose
	//!
	//! Cet évènement est déclenché lorsque la fenêtre va être fermée. Vous pouvez
	//! intercepter cet évènement pour interdire la fermeture, ou pour changer le
	//! comportement par défaut de cette fermeture (cacher la fenêtre plutôt que
	//! la fermer réellement, par exemple).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Action à effectuer.
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnClose, OnClose, TObject *, TCloseAction &);

  //@}


};


#else  // TFormH

class TForm;

#endif  // TFormH


#undef In_TFormH
