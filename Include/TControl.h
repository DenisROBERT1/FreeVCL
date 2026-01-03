//-----------------------------------------------------------------------------
//! @file TControl.h
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

#ifdef In_TControlH
#error "Inclusion circulaire de TControl"
#endif // In_TControlH

#define In_TControlH

#ifndef TControlH
#define TControlH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH

#include "TPoint.h"
#include "TFont.h"
#include "TRect.h"
#include "TSizeConstraints.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TAction;
class TPopupMenu;


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TAnchorKind {
	akLeft,												//!< Ancrage à gauche
	akTop,												//!< Ancrage en haut
	akRight,											//!< Ancrage à droite
	akBottom											//!< Ancrage en bas
};

typedef Set<TAnchorKind, akLeft, akBottom> TAnchors;  //!< Combinaison de plusieurs ancrages

enum TAlign {
	alNone,												//!< Pas d'alignement
	alTop,												//!< Alignement en haut
	alBottom,											//!< Alignement en bas
	alLeft,												//!< Alignement à gauche
	alRight,											//!< Alignement à droite
	alClient,											//!< Alignement sur la zone client
	alCustom											//!< Alignement paramétrable
};

typedef Set<TAlign, alNone, alClient> TAlignSet;  //!< Combinaison de plusieurs alignements

//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnStartDrag.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnStartDrag)(TObject *Sender, TDragObject *DragObject);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDragOver.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDragOver)(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnEndDrag.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnEndDrag)(TObject *Sender, TObject *Target, int X, int Y);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnContextPopup.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnContextPopup)(TObject *Sender, const TPoint &MousePos, bool &Handled);


//-----------------------------------------------------------------------------
//! @class TControl
//!
//! @brief Définition de la classe TControl
//!
//! Cette classe est la classe de base de tous les contrôles VCL (en règle
//! générale des composants visuels). Elle gère le dimensionnement des objets
//! en fonction des ancrages et des redimensionnements, la visibilité et le
//! glissé-déposé (drag and drop).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TControl: public TComponent {
private:
protected:
  HWND FHandle;
  int FLeft;
  int FTop;
  int FWidth;
  int FHeight;
  TSizeConstraints* FConstraints;
  TAnchors FAnchors;
  TAlign FAlign;
  int FAlignDisabled;  // int et non bool pour pouvoir compter les DisableAlign imbriqués
  bool FVisible; // Attention: Un contrôle peut avoir la propriété visible sans être réellement visible
  bool FEnabled;
  bool FAutoSize;
  TColor FColor;
  TCursor FCursor;
  TFont *FFont;
  TControl *FParent;
  bool FShowHint;
  bool FParentShowHint;
  AnsiString FHint;
  TPopupMenu *FPopupMenu;
  THintWindow *HintWindow;
  TAction *FAction;
  TDragMode FDragMode;
	TDragObject *DragObject;

	bool bDragAndDrop;
	bool bDragNotImmediate;
	int XDragBegin1, YDragBegin1, XDragBegin2, YDragBegin2;

	bool InternalCanAutoSize(int &NewLeft, int &NewTop, int &NewWidth, int &NewHeight);
	bool InternalShowHint(void);

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual int ProcessNotify(LPNMHDR pnmh);

public:

  //! @name Constructeur et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TControl(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TControl(void);

	//@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Simule un clic sur le bouton.
  //!
  //! Cette méthode fait exactement comme si l'utilisateur avait cliqué sur le
	//! bouton, c'est à dire:
	//! @li Déclenchement d'un évènement OnClick.
  //! @li Traitement de l'action associée au clic, par exemple dans le cas d'un
	//! bouton "Ok" avec une propriété @b ModalResult, l'appel de cette méthode
	//! déclenche la fermeture de la boîte de dialogue correspondante.
  //---------------------------------------------------------------------------
	void FASTCALL Click(void);

  //---------------------------------------------------------------------------
  //! @brief Arrête le glissement d'un contrôle.
  //!
  //! Cette méthode permet de stopper le drag and drop
  //!
  //! @param[in]        Drop Si true: le drop est effectué (sinon: abort)
  //---------------------------------------------------------------------------

  void FASTCALL EndDrag(bool Drop);

  //---------------------------------------------------------------------------
  //! @brief Rend le contrôle visible
  //!
  //! Cette méthode rend le contrôle visible, c'est à dire qu'elle met la
	//! propriété Visible à true, ainsi que la propriété Visible des parents.
  //---------------------------------------------------------------------------

	virtual void FASTCALL Show(void);

  //---------------------------------------------------------------------------
  //! @brief Rend le contrôle invisible.
  //!
  //! Cette méthode met la propriété Visible à false. La visibilité des parents
	//! n'est pas modifiée.
  //---------------------------------------------------------------------------

	void FASTCALL Hide(void);

  //---------------------------------------------------------------------------
  //! @brief Demande de dessin du composant
  //---------------------------------------------------------------------------

  virtual bool FASTCALL Invalidate(void);

  //---------------------------------------------------------------------------
  //! @brief Dessin immédiat du composant si besoin
  //---------------------------------------------------------------------------

  virtual bool FASTCALL Update(void);

  //---------------------------------------------------------------------------
  //! @brief Dessin forcé du composant
  //---------------------------------------------------------------------------

  virtual bool FASTCALL Repaint(void);

	//---------------------------------------------------------------------------
  //! @brief Dessin forcé du composant (idem @b Repaint)
	//!
	//! @sa Repaint
  //---------------------------------------------------------------------------

  virtual bool FASTCALL Refresh(void);

  //---------------------------------------------------------------------------
  //! @brief Renvoie la longueur du texte du contrôle.
	//!
	//! Cette méthode renvoie la longueur du texte du contrôle (propriété
	//! @b Caption), en nombre de caractères.
  //!
  //! @return @c int Longueur du texte du contrôle.
  //---------------------------------------------------------------------------
  virtual int FASTCALL GetTextLen(void);

  //---------------------------------------------------------------------------
  //! @brief Lecture du texte du contrôle.
  //!
  //! Cette méthode récupère le texte du contrôle, le copie dans un tampon et
  //! renvoie le nombre de caractères copiés.
  //!
  //! @param[out]       Buffer Tampon de retour
  //! @param[in]        BufSize Taille du tampon en nombre de caractères
  //!
  //! @return @c int Nombre de caractères copiés
  //---------------------------------------------------------------------------
  virtual int FASTCALL GetTextBuf(TCHAR * Buffer, int BufSize);

  //---------------------------------------------------------------------------
  //! @brief Définit le texte du contrôle.
	//!
	//! Cette méthode définit le texte du contrôle (propriété Caption). Elle
	//! équivaut à faire Control->Caption = Buffer;
  //!
  //! @param[in]        Buffer Nouveau texte du contrôle
  //---------------------------------------------------------------------------
  virtual void FASTCALL SetTextBuf(const TCHAR * Buffer);

  //---------------------------------------------------------------------------
  //! @brief Indique si le contrôle peut se redimensionner automatiquement.
  //!
  //! Cette fonction indique si le contrôle peut se redimensionner
	//! automatiquement aux valeurs définies. Elle est appelée lorsque la
	//! propriété @b AutoSize est à @b true et que le contrôle nécéssite un
	//! redimensionnement. Si elle échoue, le contrôle sera tronqué au lieu
	//! d'être redimensionné.@n
	//! N'utilisez pas cette méthode, elle n'est destinée à être utilisée qu'en
	//! interne.
  //!
  //! @param[in]        NewWidth Nouvelle largeur souhaitée
  //! @param[in]        NewHeight Nouvelle hauteur souhaitée
  //!
  //! @return @c bool true si la place disponible permet le redimensionement.
  //---------------------------------------------------------------------------
	virtual bool FASTCALL CanAutoSize(int &NewWidth, int &NewHeight);

  //---------------------------------------------------------------------------
  //! @brief Dimensionnement du contrôle
  //!
  //! Cette méthode redimensionne le contrôle. Les coordonnées sont en
  //! coordonnées client "vraies" (c'est à dire après ajustement par
  //! AdjustClientRect). Cette méthode est équivalente, mais plus rapide, que
  //! de fixer successivement les propriétés Left, Top, Width et Height (car
  //! chaque changement de dimension entraîne un redimensionnement des
  //! contrôles enfants).
  //!
  //! @param[in]        NewLeft Nouvelle valeur de la propriété Left
  //! @param[in]        NewTop Nouvelle valeur de la propriété Top
  //! @param[in]        NewWidth Nouvelle valeur de la propriété Width
  //! @param[in]        NewHeight Nouvelle valeur de la propriété Height
  //! @param[in]        bForceResize Force le redimensionnement (même si NewLeft == left, ...)
	//!
	//! @sa AdjustClientRect
  //---------------------------------------------------------------------------

  virtual void Size(int NewLeft, int NewTop, int NewWidth, int NewHeight, bool bForceResize);

  //---------------------------------------------------------------------------
  //! @brief Dimensionnement des sous-contrôles
  //!
  //! Cette méthode redimensionne les sous-contrôles suite à un
	//! redimensionnement.@n
	//! N'utilisez pas cette méthode, elle ne doit être utilisée qu'en interne.
  //!
  //! @param[in]        DX Différence en X de la largueur
  //! @param[in]        DY Différence en Y de la hauteur
  //! @param[in]        bForceResize Redimensionnement même si la taille n'a pas changé.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	bool SizeSubComponents(int DX, int DY, bool bForceResize);

  //---------------------------------------------------------------------------
  //! @brief Renvoie l'ordre de placement du contrôle enfant à alignement
  //! personnalisé (Align = alCustom).
  //---------------------------------------------------------------------------

	virtual bool FASTCALL CustomAlignInsertBefore(TControl* C1, TControl* C2) {return false;}

  //---------------------------------------------------------------------------
  //! @brief Renvoie la position du contrôle enfant à alignement personnalisé
  //! (Align = alCustom).
  //---------------------------------------------------------------------------

  virtual void FASTCALL CustomAlignPosition(TControl* Control,
      int &NewLeft, int &NewTop, int &NewWidth, int &NewHeight,
      TRect &AlignRect) {}

  //---------------------------------------------------------------------------
  //! @brief Désactive le réalignement des contrôles enfants.
  //---------------------------------------------------------------------------

  void FASTCALL DisableAlign(void);

  //---------------------------------------------------------------------------
  //! @brief Réactive le réalignement des contrôles enfants.
  //---------------------------------------------------------------------------

  void FASTCALL EnableAlign(void);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées client en coordonnées écran
  //---------------------------------------------------------------------------

  virtual TPoint ClientToScreen(const TPoint &Point);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées écran en coordonnées client
  //---------------------------------------------------------------------------

  virtual TPoint ScreenToClient(const TPoint &Point);

  //---------------------------------------------------------------------------
  //! @brief Contrôle au premier plan
  //---------------------------------------------------------------------------

  virtual void BringToFront(void);

	//---------------------------------------------------------------------------
  //! @brief Amorce une opération de drag and drop
  //!
  //! Cette fonction permet de débuter une opération de drag and drop
	//! (glisser-déposer). Si la propriété @b DragMode est à @b dmManual, vous
	//! devez appeler cette méthode dans l'évènement @b OnClick pour commencer
	//! l'opération de glisser-déposer. Sinon, si @b DragMode est à
	//! @b dmAutomatic, cet appel est fait automatiquement.@n
	//! L'opération commence immédiatement si le paramètre @b Immediate est à
	//! @b true, ou bien s'il est à false après un petit déplacement dépendant
	//! du paramètre Threshold.
  //!
  //! @param[in]        Immediate Si true : Démarre immédiatement le drag and drop.
  //! @param[in]        Threshold Valeur du déplacement (si -1, valeur @b DragThreshold de l'objet @b Mouse).
  //---------------------------------------------------------------------------
	void FASTCALL BeginDrag(bool Immediate, int Threshold = -1);

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
  //! @brief Propriété Handle: Identifiant système du contrôle
  //!
  //! Cette propriété permet de lire le Handle système du contrôle. Si le
  //! contrôle n'a pas de fenêtre sous-jacente (contrôle non visuel par
  //! exemple), la propriété Handle renvoie Null.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Left
  //!
  //! Cette propriété détermine la position en X du bord gauche du contrôle par
	//! rapport au contrôle parent. Elle est ignorée si la propriété
	//! TControl::Align est égale à alLeft ou alClient.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, int, Left);

  //---------------------------------------------------------------------------
  //! @brief Propriété Top
  //!
  //! Cette propriété détermine la position en Y du bord supérieur du contrôle
	//! par rapport au contrôle parent. Elle est ignorée si la propriété
	//! TControl::Align est égale à alTop ou alClient.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, int, Top);

  //---------------------------------------------------------------------------
  //! @brief Propriété Width
  //!
  //! Cette propriété détermine la largeur du contrôle. Elle est ignorée si la
	//! propriété TControl::Align est égale à alClient.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
  //!
  //! Cette propriété détermine la hauteur du contrôle. Elle est ignorée si la
	//! propriété TControl::Align est égale à alClient.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Propriété Action
  //!
  //! Cette propriété définit l'action associée au contrôle. Cette action est
	//! automatiquement déclenchée lorsque l'utilisateur clique sur le composant.
	//! Mettez cette propriété à NULL (valeur par défaut) pour qu'aucune action
	//! ne soit déclenchée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TAction *, Action);

  //---------------------------------------------------------------------------
  //! @brief Propriété LeftWnd
  //!
  //! Cette propriété détermine la position du contrôle par rapport à la
  //! première fenêtre Windows parente, en coordonnées client de ce parent.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TControl, int, LeftWnd);

  //---------------------------------------------------------------------------
  //! @brief Propriété TopWnd
  //!
  //! Cette propriété détermine la position du contrôle par rapport à la
  //! première fenêtre Windows parente, en coordonnées client de ce parent.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TControl, int, TopWnd);

  //---------------------------------------------------------------------------
  //! @brief Propriété ClientWidth
  //!
  //! Cette propriété détermine la largeur de la zone client du contrôle (c'est
	//! à dire la zone du contrôle moins les cadres, éventuellement la zone de
	//! titre, etc...
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, int, ClientWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété ClientHeight
  //!
  //! Cette propriété détermine la hauteur de la zone client du contrôle (c'est
	//! à dire la zone du contrôle moins les cadres, éventuellement la zone de
	//! titre, etc...
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, int, ClientHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété ClientRect
  //!
  //! Cette propriété permet de lire la zone client du contrôle sous forme de
	//! rectangle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TControl, TRect, ClientRect);

  //---------------------------------------------------------------------------
  //! @brief Propriété Constraints
  //!
  //! Cette propriété permet de définir des valeurs minimum et maximum pour la
	//! hauteur et la largeur du contrôle. Des valeurs à zéro signifient qu'il
	//! n'y a aucune contrainte.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TSizeConstraints *, Constraints);

  //---------------------------------------------------------------------------
  //! @brief Propriété Anchors
  //!
  //! Cette propriété permet de définir les ancrages du contrôle. Ces ancrages
	//! permettent de redimensionner automatiquement un contrôle lorsque le
	//! contrôle parent est redimensionné, ce qui permet par exemple d'avoir un
	//! espace constant entre le bord droit du contrôle et le bord droit de la
	//! boîte de dialogue qui le contient (ou entre les bords inférieurs).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TAnchors, Anchors);

  //---------------------------------------------------------------------------
  //! @brief Propriété Align
  //!
  //! Cette propriété permet de redimensionner automatiquement un contrôle
	//! lorsque le contrôle parent est redimensionné. Cette propriété pourrait
	//! être remplacée par une définition appropriée des ancrages, mais elle est
	//! plus rapide et plus simple.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TAlign, Align);

  //---------------------------------------------------------------------------
  //! @brief Propriété AlignDisabled
  //!
  //! Cette propriété permet de désactiver temporairement la propriété
	//! TControl::Align, notamment lors de la construction de l'objet. Ca permet
	//! d'éviter un inutile redimensionnement en cascade de tous les
	//! sous-contrôles.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TControl, bool, AlignDisabled);

  //---------------------------------------------------------------------------
  //! @brief Propriété Visible
	//!
	//! Cette propriété détermine si l'élément est visible ou pas. Si cette
	//! propriété est à false, le contrôle est invisible. Sinon, sa visibilité
	//! dépend de la visibilité du contrôle parent. Ca signifie qu'il ne suffit
	//! pas de tester cette propriété pour savoir si le composant est réellement
	//! visible à l'écran. De même, mettre cette propriété à true ne garantit pas
	//! que le contrôle sera visible.@n
	//! Si vous voulez réellement faire apparaître un contrôle à l'écran,
	//! utilisez la méthode TControl::Show .
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, bool, Visible);

  //---------------------------------------------------------------------------
  //! @brief Propriété Enabled: activation / désactivation d'un contrôle
	//!
	//! Affectez ou lisez cette propriété pour gérer l'activation d'un contrôle.
	//! Un contrôle désactivé est automatiquement grisé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, bool, Enabled);

  //---------------------------------------------------------------------------
  //! @brief Propriété AutoSize
  //!
  //! Cette propriété permet de redimensionner automatiquement un contrôle
	//! lorsque ses constituants changent de dimension (texte d'un TLabel, par
	//! exemple). Si le contrôle parent a également la propriété @b AutoSize,
	//! il va à son tour changer de taille et ainsi de suite éventuellement
	//! jusqu'à la fenêtre principale
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, bool, AutoSize);

  //---------------------------------------------------------------------------
  //! @brief Propriété Color
  //!
  //! Cette propriété permet de définir la couleur principale (en général la
	//! couleur de fond) d'un contrôle. Certains contrôles possèdent d'autres
	//! propriétés qui permettent aussi de changer la couleur, prioritairement
	//! sur celle définie ici.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TColor, Color);

  //---------------------------------------------------------------------------
  //! @brief Propriété Cursor
  //!
  //! Cette propriété permet de définir la forme du curseur lorsque la souris
	//! survole le contrôle. Si la forme du curseur doit changer en fonction de
	//! la position de la souris, il est possible de définir un curseur invisible
	//! (crNone), puis de le redéfinir à chaque évènement OnMouseMove en fonction
	//! de la zone où se trouve la souris.
	//!
	//! @sa TScreen::Cursor
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TCursor, Cursor);

  //---------------------------------------------------------------------------
  //! @brief Propriété Font
  //!
  //! Cette propriété permet de définir la fonte utilisée par les textes du
	//! contrôle (lorsqu'il y en a, ce qui n'est pas toujours le cas).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TCustomFont *, Font);

  //---------------------------------------------------------------------------
  //! @brief Propriété Parent: contrôle ancêtre
	//!
	//! Cette propriété représente le contrôle parent. Elle peut être nulle si le
	//! composant n'a pas de parent (cas général des fenêtres).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TControl *, Parent);

  //---------------------------------------------------------------------------
  //! @brief Propriété ShowHint: visibilité des infos-bulles
	//!
	//! Détermine si les infos-bulles doivent être affichées. Cette propriété est
	//! inactive si le contrôle a un parent et que la propriété @b ParentShowHint
	//! est à @b true.
	//!
	//! @sa ParentShowHint
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, bool, ShowHint);

  //---------------------------------------------------------------------------
  //! @brief Propriété ParentShowHint: Infos-bulles en fonction du parent
	//!
	//! Détermine si la visibilité des infos-bulles dépendent du parent. Si cette
	//! propriété est à @b true, les infos-bulles ne sont visibles que si elles
	//! le sont pour le parent. Le parent peut lui-même avoir la propriété
	//! @b ParentShowHint à @b true. On peut ainsi activer ou désactiver toutes
	//! les infos-bulle d'une boîte de dialogue juste en les activant ou en les
	//! désactivant pour la fenêtre principale.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, bool, ParentShowHint);

  //---------------------------------------------------------------------------
  //! @brief Propriété Hint: Contenu de l'info-bulle
	//!
	//! Détermine le texte contenu dans l'info-bulle. L'info-bulle n'est visible
	//! que si:@n
	//! - la propriété TControl::ShowHint est à true@n
	//! - ou bien si elle est à false, que la propriété TControl::ParentShowHint
	//! est à true et que l'affichage des infobulles du parent est activée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, AnsiString, Hint);

  //---------------------------------------------------------------------------
  //! @brief Propriété PopupMenu: Menu contextuel
	//!
	//! Cette propriété détermine le menu contextuel à afficher sur un clic
	//! droit. L'objet reçoit un évènement @b OnContextPopup, et si le flag
	//! @b Handled n'a pas été mis à true dans cet évènement, le menu contextuel
	//! défini ici est automatiquement affiché.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TPopupMenu *, PopupMenu);

  //---------------------------------------------------------------------------
  //! @brief Propriété DragMode: Mode de gestion du drag and drop
	//!
	//! Cette propriété détermine le mode de gestion du drag and drop (glisser-
	//! déposer) : si cette propriété est mise à @b dmAutomatic, le drag and drop
	//! est automatiquement activé lorsque l'utilisateur clique sur le contrôle
	//! et amorce un déplacement. Dans le cas contraire, si elle est à
	//! @b dmManual, il faut appeler la méthode @b BeginDrag sur l'évènement
	//! @b OnClick pour activer le drag and drop,sinon il sera désactivé.
	//!
	//! @sa BeginDrag
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControl, TDragMode, DragMode);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnClick: clic sur un élément de menu
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur fait un clic gauche
	//! sur un contrôle, ou bien lorsque le programme a déclenché artificiellement
	//! un clic avec la méthode Click.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnClick, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDblClick
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur fait un double clic
	//! gauche sur un contrôle.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnDblClick, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnContextPopup: Apparition du menu contextuel
	//!
	//! Cet évènement est déclenché juste avant d'ouvrir le menu contextuel,
	//! lorsque l'utilisateur fait un clic droit sur le composant. vous pouvez
	//! intercepter cet évènement pour effectuer un traitement particulier
	//! lorsque l'utilisateur fait un clic droit sur un composant. si vous
	//! positionnez le flag @b Handled à true dans le traitement de l'évènement,
	//! le processus s'interromp est le menu défini par la propriété @b PopupMenu
	//! n'est pas affiché. Dans le cas contraire et si la propriété @b AutoPopup
	//! est positionnée à true, ce menu s'affiche.@n
	//! Exemples d'utilisation:@n
	//! Vous interceptez l'évènement pour ajouter ou enlever certains éléments du
	//! menu. Vous laissez le flag @b Handled à false pour que le menu contextuel
	//! s'affiche normalement.@n
	//! Vous interceptez l'évènement pour afficher votre propre menu avec la
	//! méthode @b Popup, puis vous positionnez le flag @b Handled à true pour
	//! bloquer l'affichage du menu standard.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Coordonnées du point cliqué
  //! @param[in, out]   Arg3 Flag Handled à positionner pour stopper l'affichage du menu
  //!
	//! @note l'évènement OnContextPopup est appelé même en l'absence de menu
	//! contextuel défini pour le composant.
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnContextPopup, OnContextPopup, TObject *, const TPoint &, bool &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnHint
	//!
	//! Cet évènement est déclenché lorsque le contrôle ouvre l'info-bulle. Vous
	//! pouvez intercepter cet évènement pour effectuer une action particulière
	//! (par exemple changer le contenu de l'info-bulle en fonction des
	//! circonstances).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnHint, OnHint, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnResize
	//!
	//! Cet évènement est déclenché lorsque le contrôle est redimensionné
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnResize, TObject *);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnStartDrag
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur commence un drag and
	//! drop (glissé-déposé).
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Objet déplacé
	//---------------------------------------------------------------------------

	DECLARE_EVENT_2(TOnStartDrag, OnStartDrag, TObject *, TDragObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDragOver: Drag and drop sur un contrôle
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur effectue un drag and
	//! drop et survole un contrôle. Positionnez le flag @b Accept en fonction
	//! des arguments, à @b true si la dépose de l'objet est possible, ou bien
	//! laissez-le à @b false sinon. Si l'évènement n'est pas intercepté, la
	//! valeur par défaut est automatiquement positionné à @b false.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Source Objet déplacé par l'utilisateur
  //! @param[in]        Arg3 Coordonnée en X du curseur (pixels en coordonnées écran)
  //! @param[in]        Arg4 Coordonnée en Y du curseur (pixels en coordonnées écran)
  //! @param[in]        Arg5 Indique la façon dont l'objet se déplace
  //! @param[in, out]   Arg6 Flag Accept : détermine si l'utilisateur peut déposer l'objet (et donc la forme du curseur)
  //---------------------------------------------------------------------------

	DECLARE_EVENT_6(TOnDragOver, OnDragOver, TObject *, TObject *, int, int, TDragState, bool &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnEndDrag: Fin d'opération drag end drop
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur relâche un objet sur
	//! un contrôle.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Objet cible sur lequel est déposé l'objet
  //! @param[in]        Arg3 Coordonnée en X du curseur (pixels en coordonnées écran)
  //! @param[in]        Arg4 Coordonnée en Y du curseur (pixels en coordonnées écran)
  //---------------------------------------------------------------------------

	DECLARE_EVENT_4(TOnEndDrag, OnEndDrag, TObject *, TObject *, int, int);


  //@}


};


#else  // TControlH

class TControl;

#endif  // TControlH


#undef In_TControlH
