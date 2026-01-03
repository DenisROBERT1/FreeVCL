//-----------------------------------------------------------------------------
//! @file TWinControl.h
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

#ifdef In_TWinControlH
#error "Inclusion circulaire de TWinControl"
#endif // In_TWinControlH

#define In_TWinControlH

#ifndef TWinControlH
#define TWinControlH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TControlH
#include "TControl.h"
#endif // TControlH

#ifndef TControlCanvasH
#include "TControlCanvas.h"
#endif // TControlCanvasH


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnShow.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnShow)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnHide.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnHide)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnKeyDown.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnKeyDown)(TObject *Sender, Word Key, TShiftState Shift);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnKeyUp.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnKeyUp)(TObject *Sender, Word Key, TShiftState Shift);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnKeyPress.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnKeyPress)(TObject *Sender, TCHAR Char);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnMouseMove.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnMouseMove)(TObject *Sender, TShiftState Shift, int X, int Y);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnMouseDown.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnMouseDown)(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnMouseUp.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnMouseUp)(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnMouseWheel.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnMouseWheel)(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnEnter.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnEnter)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnExit.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnExit)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDropFile.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDropFile)(TObject *Sender, AnsiString &FileName, TShiftState Shift, int X, int Y);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnMeasureItem.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnMeasureItem)(TObject *Sender, int Index, int *Height);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDrawItem.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDrawItem)(TObject *Sender, int Index, const TRect &Rect, TOwnerDrawStates States);


//-----------------------------------------------------------------------------
//! @class TWinControl
//!
//! @brief Définition de la classe TWinControl
//!
//! Cette classe est la classe de base de tous les contrôles pouvant recevoir
//! le focus. Utilisez cette classe comme ancêtre des contrôles que vous créez
//! dans le cas où vous vous avez besoin de recevoir des entrées clavier ou
//! d'avoir un handle système. Dans le cas contraire, il est préférable
//! d'hériter de la classe @b TControl qui utilise moins de ressources.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TWinControl: public TControl {
private:
protected:
	DWORD FWindowStyle;
	DWORD FExWindowStyle;
	bool FTabStop;
	int FTabOrder;
	bool FWantReturns;
	bool FWantTabs;
	TRect FClientRect;
	TControlCanvas *FCanvas;
	int CurrentFocus;
	HBRUSH hBrushColorBack;
	bool FDragAcceptFiles;
	TDragKind FDragKind;
	bool bProcessCreateToDo;
	bool bProcessShowWindowToDo;

	DLGPROC DialogProc;  //!< Procédure de fenêtre par défaut (contrôles Windows)
	bool bWantPaint;		 //!< true si interception des messages Paint

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
	//! @brief Recherche des fenêtres enfants interceptant la touche tabulation
	//!
	//! Cette méthode recherche si des fenêtres enfants ont demandé à recevoir
	//! les messages d'appui sur la touche tabulation.
	//!
	//! @return @c TComponent * Composant enfant ayant demandé l'interception.
	//---------------------------------------------------------------------------

	virtual TComponent * ChildWantTabs(void);

	//---------------------------------------------------------------------------
	//! @brief Initialisation de tous les focus
	//!
	//! Cette méthode initialise (met à la valeur -1) le focus du composant et de
	//! tous ses enfants. Cette méthode est appelée lors de l'appel à un
	//! ShowModal d'une fenêtre.
	//---------------------------------------------------------------------------

	void InitFocus();

	//---------------------------------------------------------------------------
	//! @brief Message de l'enfant au parent pour signaler l'affectation du focus
	//!
	//! Cette méthode est appelée lorsqu'un composant reçoit le message
	//! WM_SETFOCUS (suite à un clic de l'utilisateur, par exemple). Elle met à
	//! à jour les focus mémorisés en accord avec la réalité.
	//---------------------------------------------------------------------------

	bool SetCurrentFocus(void);

	//---------------------------------------------------------------------------
	//! @brief Message de l'enfant au parent pour signaler la perte du focus
	//!
	//! Cette méthode est appelée lorsqu'un composant devient invisible. Elle
	//! propage la perte de focus aux parents, grands parents, ..., le focus
	//! courant de ces objets passe à -1.
	//---------------------------------------------------------------------------

	bool KillCurrentFocus(void);

	//---------------------------------------------------------------------------
	//! @brief Positionne le focus sur le contrôle enfant courant
	//!
	//! Cette méthode passe le focus au contrôle enfant ayant le focus déclaré.
	//!
	//! @return @c bool true si le focus est passé à un contrôle enfant.
	//---------------------------------------------------------------------------

	bool SetFocusOnCurrentControl(void);

	//---------------------------------------------------------------------------
	//! @brief Positionne le focus sur le contrôle suivant
	//!
	//! Cette méthode passe le focus au contrôle suivant en tenant compte de
	//! toutes les contraintes (propriété TabStop, composant visible et actif,
	//! parcours des composants enfants, etc...).
	//!
	//! @param[in]				bDown Si true: vers le bas. Sinon vers le haut.
	//!
	//! @return @c bool true si un contrôle enfant a été trouvé. false s'il n'y
	//! a aucun contrôle enfant ou bien qu'ils sont tous désactivés ou/et non
	//! visibles.
	//---------------------------------------------------------------------------

	bool SetFocusOnNextControl(bool bDown);

	//---------------------------------------------------------------------------
	//! @brief Initialisation des contrôles Windows (Edit, Button, ...)
	//---------------------------------------------------------------------------

	void InitStdControl(void);

	//---------------------------------------------------------------------------
	//! @brief Traitement des messages Windows
	//---------------------------------------------------------------------------

	virtual bool ProcessCommand(WORD Notify) {return false;}
	virtual void ProcessCreate(void) {}
	virtual bool ProcessPaint(HDC hdc, RECT rcPaint) {return true;}
	virtual bool ProcessPrintClient(HDC hdc, DWORD Flags);
	virtual bool ProcessActivateApp(bool bActive) {return true;}
	virtual bool ProcessActivate(int fActive) {return true;}
	virtual bool ProcessShowWindow(bool bShow);
	virtual bool ProcessSetFocus(HWND hWndLoseFocus);
	virtual bool ProcessKillFocus(HWND hWndGetfocus);
	virtual bool ProcessMoving(int fwSide, LPRECT lprc) {return true;}
	virtual bool ProcessMove(int X, int Y);
	virtual bool ProcessSizing(int fwSide, LPRECT lprc);
	virtual bool ProcessSize(int SizeType, int L, int H);
	virtual bool DistributionHScroll(HWND hWnd, int Command, int Info);
	virtual bool ProcessHScroll(int Command, int Info) {return false;}
	virtual bool DistributionVScroll(HWND hWnd, int Command, int Info);
	virtual bool ProcessVScroll(int Command, int Info) {return false;}
	virtual bool DistributionKeyDown(HWND hWnd, int nVirtKey);
	virtual bool ProcessKeyDown(int nVirtKey);
	virtual bool DistributionKeyUp(HWND hWnd, int nVirtKey);
	virtual bool ProcessKeyUp(int nVirtKey);
	virtual bool DistributionChar(HWND hWnd, TCHAR chCharCode);
	virtual bool ProcessChar(TCHAR chCharCode);
	virtual bool ProcessMouseMove(int X, int Y);
	virtual bool ProcessLButtonDown(int X, int Y);
	virtual bool ProcessLButtonUp(int X, int Y);
	virtual bool ProcessLButtonDblClk(int X, int Y);
	virtual bool ProcessMButtonDown(int X, int Y);
	virtual bool ProcessMButtonUp(int X, int Y);
	virtual bool ProcessMButtonDblClk(int X, int Y);
	virtual bool ProcessRButtonDown(int X, int Y);
	virtual bool ProcessRButtonUp(int X, int Y);
	virtual bool ProcessRButtonDblClk(int X, int Y);
	virtual bool ProcessMouseWheel(WORD fwKeys, short zDelta, int X, int Y);
	virtual bool ProcessCaptureChanged(HWND hWndNewCapture) {return false;}
	virtual bool DistributionColor(HWND hWnd, HDC hdc, HBRUSH *hBrush);
	virtual bool ProcessColor(HDC hdc, HBRUSH *hBrush);
	virtual bool DistributionContextMenu(HWND hWnd, int X, int Y);
	virtual bool ProcessContextMenu(int X, int Y);
	virtual bool ProcessDropFiles(HDROP hDrop);
	virtual bool ProcessTimer(UINT IdTimer) {return false;}
	virtual bool ProcessUser(UINT Message, WPARAM wParam, LPARAM lParam) {return false;}
	virtual bool ProcessClose(void);
	virtual void ProcessDestroy(void) {}

	// MESSAGE_MAP
	virtual void FASTCALL Dispatch(TMessage *Message) {}

public:

	//! @name Constructeurs et destructeur
	//@{

	//---------------------------------------------------------------------------
	//! @brief Constructeur
	//---------------------------------------------------------------------------

	TWinControl(TComponent* AOwner);

	//---------------------------------------------------------------------------
	//! @brief Destructeur
	//---------------------------------------------------------------------------

	virtual ~TWinControl(void);

	//@}


	//! @name Méthodes
	//@{

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Caption
	//!
	//! Cette méthode permet d'écrire la propriété Caption
	//!
	//! @param[in]				NewCaption Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Caption(AnsiString NewCaption);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Left
	//!
	//! Cette méthode permet d'écrire la propriété Left
	//!
	//! @param[in]				NewLeft Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Left(int NewLeft);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Top
	//!
	//! Cette méthode permet d'écrire la propriété Top
	//!
	//! @param[in]				NewTop Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Top(int NewTop);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Width
	//!
	//! Cette méthode permet d'écrire la propriété Width
	//!
	//! @param[in]				NewWidth Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Width(int NewWidth);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Height
	//!
	//! Cette méthode permet d'écrire la propriété Height
	//!
	//! @param[in]				NewHeight Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Height(int NewHeight);

	//---------------------------------------------------------------------------
	//! @brief Getter de la propriété ClientWidth
	//!
	//! Cette méthode permet de lire la propriété ClientWidth
	//!
	//! @return @c int Propriété ClientWidth
	//---------------------------------------------------------------------------

	virtual int Get_ClientWidth(void);


	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Font
	//!
	//! Cette méthode permet d'écrire la propriété Font
	//!
	//! @param[in]				NewFont Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Font(TFont *NewFont);


	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété ClientWidth
	//!
	//! Cette méthode permet d'écrire la propriété ClientWidth
	//!
	//! @param[in]				NewClientWidth Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_ClientWidth(int NewClientWidth);


	//---------------------------------------------------------------------------
	//! @brief Getter de la propriété ClientHeight
	//!
	//! Cette méthode permet de lire la propriété ClientHeight
	//!
	//! @return @c int Propriété ClientHeight
	//---------------------------------------------------------------------------

	virtual int Get_ClientHeight(void);


	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété ClientHeight
	//!
	//! Cette méthode permet d'écrire la propriété ClientHeight
	//!
	//! @param[in]				NewClientHeight Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_ClientHeight(int NewClientHeight);


	//---------------------------------------------------------------------------
	//! @brief Getter de la propriété LeftWnd
	//!
	//! Cette méthode permet de lire la propriété LeftWnd
	//!
	//! @return @c int Propriété LeftWnd
	//---------------------------------------------------------------------------

	virtual int Get_LeftWnd(void);

	//---------------------------------------------------------------------------
	//! @brief Getter de la propriété TopWnd
	//!
	//! Cette méthode permet de lire la propriété TopWnd
	//!
	//! @return @c int Propriété TopWnd
	//---------------------------------------------------------------------------

	virtual int Get_TopWnd(void);

	//---------------------------------------------------------------------------
	//! @brief Getter de la propriété ClientRect
	//!
	//! Cette méthode permet de lire la propriété ClientRect. Cette propriété
	//! retourne le rectangle de la fonction Windows GetClientRect, ajustée
	//! avec la méthode AdjustClientRect.
	//!
	//! @return @c TRect Propriété ClientRect
	//!
	//! @sa AdjustClientRect
	//---------------------------------------------------------------------------

	virtual TRect Get_ClientRect(void);


	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Visible
	//!
	//! Cette méthode permet d'écrire la propriété Visible
	//!
	//! @param[in]				NewVisible Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Visible(bool NewVisible);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Enabled
	//!
	//! Cette méthode permet d'écrire la propriété Enabled
	//!
	//! @param[in]				NewEnabled Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Enabled(bool NewEnabled);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Color
	//!
	//! Cette méthode permet d'écrire la propriété Color
	//!
	//! @param[in]				NewColor Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Color(TColor NewColor);

	//---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Parent
  //!
  //! @param[in]        NewParent Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Set_Parent(TControl * NewParent);

	//---------------------------------------------------------------------------
	//! @brief Ajustement de la zone client
	//!
	//! Cette méthode ajuste le rectangle client en fonction des bordures. Dans
	//! la classe TWinControl elle ne fait rien, mais certains contrôles hérités
	//! la réimplémentent en fonction de leur besoin (TPanel, par exemple).
	//!
	//! @param[in, out] 	 Rect Rectangle client
	//!
  /*! @verbatim
      Cette méthode doit être réimplémentée par les composants qui dessinent
			certains éléments par programme (généralement leur bordure). À l'inverse,
			les composants dont la bordure est gérée par le système n'ont pas à
			implémenter cette fonction, la fonction ::GetClientRect du système tient
			déjà compte des bordures et autres éléments du système. @endverbatim */
	//---------------------------------------------------------------------------

	virtual void FASTCALL AdjustClientRect(TRect &Rect) {}

	//---------------------------------------------------------------------------
	//! @brief Transformation des coordonnées client en coordonnées écran
	//!
	//! Cette méthode transforme des coordonnées écran en coordonnées client.
	//!
	//! @param[in, out] 	 Point Coordonnées à transformer
	//!
	//! @warning Ne pas utiliser la fonction système @b ClientToScreen qui ne
	//! tient pas compte des spécificités de certains composants (bordures).
	//---------------------------------------------------------------------------

	virtual TPoint ClientToScreen(const TPoint &Point);

	//---------------------------------------------------------------------------
	//! @brief Transformation des coordonnées écran en coordonnées client
	//!
	//! Cette méthode transforme des coordonnées client en coordonnées écran.
	//!
	//! @param[in, out] 	 Point Coordonnées à transformer
	//!
	//! @warning Ne pas utiliser la fonction système @b ScreenToClient qui ne
	//! tient pas compte des spécificités de certains composants (bordures).
	//---------------------------------------------------------------------------

	virtual TPoint ScreenToClient(const TPoint &Point);

	//---------------------------------------------------------------------------
	//! @brief Détermine si le contrôle a le focus
	//!
	//! Cette méthode renvoie vrai si le contrôle détient le focus, et faux si
	//! c'est un autre contrôle qui l'a (y compris si ce contrôle est un enfant
	//! de celui-ci). Si vous souhaitez tester que le focus est détenu par ce
	//! contrôle ou bien un de ses enfants, utilisez la méthode
	//! TWinControl::FocusedChild.
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	bool Focused(void);

	//---------------------------------------------------------------------------
	//! @brief Détermine si le contrôle ou un de ses enfants a le focus
	//!
	//! Cette méthode renvoie vrai si le contrôle lui-même ou bien un de ses
	//! enfants détient le focus. Si vous souhaitez tester que le focus est
	//! détenu par ce contrôle et uniquement celui-ci, utilisez la méthode
	//! TWinControl::Focused.
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	bool FocusedChild(void);

	//---------------------------------------------------------------------------
	//! @brief Détermine si le contrôle peut recevoir le focus
	//!
	//! Cette méthode teste si le contrôle peut recevoir le focus, c'est à dire
	//! s'il est visible et actif, ainsi que ses parents.
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	bool FASTCALL CanFocus(void);

	//---------------------------------------------------------------------------
	//! @brief Lecture du contrôle ayant le focus
	//!
	//! Cette méthode renvoie le contrôle ayant théoriquement le focus, c'est à
	//! dire le premier enfant (ou enfant d'un enfant) visible, actif, et pouvant
	//! recevoir le focus. Si aucun enfant ne répond a ces condition, la méthode
	//! revoie l'objet courant @b this.
	//!
	//! @return @c TWinControl * Contrôle ayant le focus
	//---------------------------------------------------------------------------

	virtual TWinControl * FASTCALL GetFocus(void);

	//---------------------------------------------------------------------------
	//! @brief Affectation du focus au contrôle
	//!
	//! Cette méthode passe le focus (élément recevant les évènement clavier) au
	//! contrôle. Le focus est réellement passé à ce contrôle, et non aux
	//! contrôles enfants si le contrôle est un conteneur (TPanel ou TPageControl
	//! par exemple). Pour faire ceci, utilisez la méthode @b SetFocusChild.
	//!
	//! @sa SetFocusChild
	//---------------------------------------------------------------------------

	virtual void FASTCALL SetFocus(void);

	//---------------------------------------------------------------------------
	//! @brief Affectation du focus au contrôle ou à l'un de ses enfants
	//!
	//! Cette méthode passe le focus (élément recevant les évènement clavier) au
	//! contrôle, ou bien à l'un de ses enfants si le contrôle en contient et
	//! qu'ils sont actifs. L'élément recevant le focus dépend de la propriété
	//! TabOrder, des actions précédemment faites par l'utilisateur, des éléments
	//! non visibles et désactivés. Si vous souhaitez réellement passer le focus
	//! au contrôle, et non aux contrôles enfants, utilisez la méthode
	//! @b SetFocus.
	//!
	//! @sa SetFocus
	//---------------------------------------------------------------------------

	virtual void FASTCALL SetFocusChild(void);

	//---------------------------------------------------------------------------
	//! @brief Dimensionnement du contrôle
	//!
	//! Cette méthode redimensionne le contrôle. Les coordonnées sont en
	//! coordonnées client "vraies" (c'est à dire après ajustement par
	//! AdjustClientRect).
	//!
	//! @param[in]				NewLeft Nouvelle valeur de la propriété Left
	//! @param[in]				NewTop Nouvelle valeur de la propriété Top
	//! @param[in]				NewWidth Nouvelle valeur de la propriété Width
	//! @param[in]				NewHeight Nouvelle valeur de la propriété Height
	//! @param[in]				bForceResize Force le redimensionnement (même si NewLeft == left, ...)
	//!
	//! @sa TControl::Size, AdjustClientRect
	//---------------------------------------------------------------------------

	virtual void Size(int NewLeft, int NewTop, int NewWidth, int NewHeight, bool bForceResize);

	//---------------------------------------------------------------------------
	//! @brief Changement du style de la fenêtre
	//!
	//! Cette méthode change le style windows d'un composant avec la fonction
	//! système SetWindowLong. Ceci permet de changer dynamiquement certains
	//! paramètres, mais pas tous: il y a des composants qui ne peuvent être
	//! changés de cette façon. Dans ce cas, il faut les détruire et les recréer
	//! avec le nouveau style.
	//!
	//! @param[in]				dwNewWindowStyle Nouveau style
	//!
	//! @return @c bool true si la fonction système a renvoyé TRUE (ce qui ne
	//! veut pas dire que le changement a été réellement effectué).
	//---------------------------------------------------------------------------

	bool ChangeWindowStyle(DWORD dwNewWindowStyle);

	//---------------------------------------------------------------------------
	//! @brief Changement du style étendu de la fenêtre
	//!
	//! Cette méthode change le style windows étendu d'un composant avec la
	//! fonction système SetWindowLong. Ceci permet de changer dynamiquement
	//! certains paramètres, mais pas tous: il y a des composants qui ne peuvent
	//! être changés de cette façon. Dans ce cas, il faut les détruire et les
	//! recréer avec le nouveau style.
	//!
	//! @param[in]				dwNewExWindowStyle Nouveau style étendu
	//!
	//! @return @c bool true si la fonction système a renvoyé TRUE (ce qui ne
	//! veut pas dire que le changement a été réellement effectué).
	//---------------------------------------------------------------------------
	bool ChangeExWindowStyle(DWORD dwNewExWindowStyle);

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
	//! @brief Demande de dessin du composant
	//!
	//! Cette méthode provoque le dessin du composant. Le dessin n'est pas
	//! immédiat: l'ordre de dessin est mis dans la pile de message, il sera
	//! traité lors du traitement du message WM_PAINT. Si vous souhaitez
	//! dessiner immédiatement le composant, utilisez la méthode @b Repaint.
	//!
	//! @param[in]				bErase Si true : le composant est rempli avec la
	//! 									couleur de fond avant dessin.
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	virtual bool FASTCALL Invalidate(bool bErase = false);

	//---------------------------------------------------------------------------
	//! @brief Dessin immédiat du composant si besoin
	//!
	//! Cette méthode force le le dessin immédiat du composant.
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	virtual bool FASTCALL Update(void);

	//---------------------------------------------------------------------------
	//! @brief Dessin forcé du composant
	//!
	//! Cette méthode provoque et force le le dessin immédiat du composant.
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	virtual bool FASTCALL Repaint(void);

	//---------------------------------------------------------------------------
	//! @brief Suppression d'un composant enfant
	//!
	//! Cette méthode enlève un composant de la liste des composants. Le
	//! composant n'est pas détruit.
	//!
	//! @param[out] 			AComponent Composant à enlever
	//---------------------------------------------------------------------------

	virtual void RemoveComponent(TComponent* AComponent);

	//---------------------------------------------------------------------------
	//! @brief Renvoie la longueur du texte du contrôle.
	//!
	//! Cette méthode renvoie la longueur du texte (propriété @b Caption) du
	//! composant.
	//!
	//! @return @c int Longueur du texte.
	//---------------------------------------------------------------------------
	virtual int FASTCALL GetTextLen(void);

	//---------------------------------------------------------------------------
	//! @brief Lecture du texte du contrôle.
	//!
	//! Cette méthode récupère le texte du contrôle (propriété @b Caption) et le
	//! copie dans un tampon.
	//!
	//! @param[out] 			Buffer Tampon destination
	//! @param[in]				BufSize Longueur du tampon
	//!
	//! @return @c int Nombre de caractères copiés.
	//---------------------------------------------------------------------------
	virtual int FASTCALL GetTextBuf(TCHAR * Buffer, int BufSize);

	//---------------------------------------------------------------------------
	//! @brief Définit le texte du contrôle.
	//!
	//! Cette méthode définit le texte du contrôle (propriété @b Caption).
	//!
	//! @param[in]				Buffer Nouveau texte du contrôle
	//---------------------------------------------------------------------------
	virtual void FASTCALL SetTextBuf(const TCHAR * Buffer);

	//@}

	//! @name Propriétés
	//@{

	//---------------------------------------------------------------------------
	//! @brief Propriété WindowStyle: style système
	//!
	//! Cette propriété permet de lire et de définir le style système du
	//! composant.
	//!
	//! @warning Certains composant ne sont pas prévus pour changer dynamiquement
	//! de style. Dans la mesure du possible, utilisez plutôt les propriétés de
	//! chaque composant.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, DWORD, WindowStyle);

	//---------------------------------------------------------------------------
	//! @brief Propriété ExWindowStyle: style système étendu
	//!
	//! Cette propriété permet de lire et de définir le style système étendu du
	//! composant.
	//!
	//! @warning Certains composant ne sont pas prévus pour changer dynamiquement
	//! de style. Dans la mesure du possible, utilisez plutôt les propriétés de
	//! chaque composant.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, DWORD, ExWindowStyle);

	//---------------------------------------------------------------------------
	//! @brief Propriété TabStop
	//!
	//! Cette propriété permet de définir si le curseur texte (caret) s'arrête
	//! sur le composant lorsqu'on fait ALT + TAB.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, bool, TabStop);

	//---------------------------------------------------------------------------
	//! @brief Propriété TabOrder
	//!
	//! Cette propriété permet de définir l'ordre dans lequel les composants sont
	//! sélectionnés lorsqu'on fait ALT + TAB. L'ordre n'est défini que par
	//! rapport au parent (et non par rapport à la fenêtre globale). C'est à dire
	//! que les enfants d'un objet @b TPanel, par exemple, devront avoir une
	//! propriété TabOrder qui commence à zéro (quel que soit la valeur du
	//! TabOrder du @b TPanel lui-même.@n
	//! Le fait d'affecter un TabOrder de -1 est équivalent à mettre à false la
	//! propriété @b TabStop.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, int, TabOrder);

	//---------------------------------------------------------------------------
	//! @brief Propriété WantReturns
	//!
	//! Cette propriété permet de définir si le composant intercepte la touche
	//! entrée (cas d'un TMemo, par exemple). Dans le cas contraire, l'appui sur
	//! la touche entrée provoquera le comportement par défaut de la fenêtre
	//! (c'est à dire généralement l'appui sur le bouton défini par défaut).
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, bool, WantReturns);

	//---------------------------------------------------------------------------
	//! @brief Propriété WantTabs
	//!
	//! Cette propriété permet de définir si le composant intercepte la touche
	//! tabulation (cas d'un TMemo ou d'un TRichEdit, par exemple). Dans le cas
	//! contraire, l'appui sur la touche tabulation provoquera un changement de
	//! focus. Cette propriété ne concerne que la sortie du contrôle. Il est
	//! toujours possible de positionner le curseur dans le contrôle (c'est à
	//! dire d'y entrer) avec la touche tabulation, sauf si le contrôle précédent
	//! a lui-même la propriété @b WantTabs à @b true.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, bool, WantTabs);

	//---------------------------------------------------------------------------
	//! @brief Propriété Canvas
	//!
	//! Cette propriété permet de récupérer un objet TCanvas permettant le
	//! dessin.
	//!
	//! @warning Ne pas mémoriser l'adresse du TCanvas en dehors d'un fil
	//! d'exécution bien déterminé, mais le redemander à chaque fois qu'il est
	//! nécessaire. En effet, le système utilise différents objets TCanvas en
	//! fonction du contexte. Par exemple, dans un OnPaint, il est initialisé
	//! avec le HDC et le rectangle de clipping fournis par le système, alors
	//! qu'en dehors de cette fonction c'est un autre TCanvas qui sera utilisé.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TWinControl, TCanvas *, Canvas);

	//---------------------------------------------------------------------------
	//! @brief Propriété DragAcceptFiles
	//!
	//! Cette propriété permet de définir si le composant accepte un drag and
	//! drop de fichier depuis l'explorateur. Lorsque cette propriété est à
	//! @b true, le drag and drop d'un fichier change automatiquement la forme
	//! du curseur lorsqu'on se positionne sur le composant et lorsque
	//! l'opération est finie un évènement @b OnDropFile est déclenché.
	//!
	//! @sa OnDropFile
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, bool, DragAcceptFiles);

	//---------------------------------------------------------------------------
	//! @brief Propriété DragKind
	//!
	//! Cette propriété permet de définir le type de drag and drop supporté. Ne
	//! par confondre cette propriété avec la propriété @b DragAcceptFiles qui
	//! ne concerne que les fichiers déplacés depuis l'explorateur.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TWinControl, TDragKind, DragKind);

	//@}

	//! @name Evènements
	//@{

	//---------------------------------------------------------------------------
	//! @brief Evènement OnShow
	//!
	//! Cet évènement est déclenché lorsque la fenêtre va devenir visible.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//---------------------------------------------------------------------------

	DECLARE_EVENT_1(TOnShow, OnShow, TObject *);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnHide
	//!
	//! Cet évènement est déclenché lorsque la fenêtre va devenir invisible.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//---------------------------------------------------------------------------

	DECLARE_EVENT_1(TOnHide, OnHide, TObject *);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnKeyDown
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur appuie sur une touche
	//! du clavier.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Code de la touche appuyée
	//! @param[in]				Arg3 Etat des touches ALT, SHIFT et CONTROL
	//---------------------------------------------------------------------------

	DECLARE_EVENT_3(TOnKeyDown, OnKeyDown, TObject *, Word, TShiftState);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnKeyUp
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur relâche une touche
	//! du clavier précédemment appuyée.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Code de la touche relâchée
	//! @param[in]				Arg3 Etat des touches ALT, SHIFT et CONTROL
	//---------------------------------------------------------------------------

	DECLARE_EVENT_3(TOnKeyUp, OnKeyUp, TObject *, Word, TShiftState);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnKeyPress
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur a enfoncé et relâché
	//! une touche du clavier.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Caractère correspondant à la touche
	//---------------------------------------------------------------------------

	DECLARE_EVENT_2(TOnKeyPress, OnKeyPress, TObject *, TCHAR);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnMouseMove
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur a bougé la souris.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Etat des touches ALT, SHIFT et CONTROL
	//! @param[in]				Arg3 Coordonnée X de la souris (coordonnées écran)
	//! @param[in]				Arg4 Coordonnée Y de la souris (coordonnées écran)
	//---------------------------------------------------------------------------

	DECLARE_EVENT_4(TOnMouseMove, OnMouseMove, TObject *, TShiftState, int, int);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnButtonDown
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur appuie sur une touche
	//! de la souris.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Bouton de la souris appuyé
	//! @param[in]				Arg3 Etat des touches ALT, SHIFT et CONTROL
	//! @param[in]				Arg4 Coordonnée X de la souris (coordonnées écran)
	//! @param[in]				Arg5 Coordonnée Y de la souris (coordonnées écran)
	//---------------------------------------------------------------------------

	DECLARE_EVENT_5(TOnMouseDown, OnMouseDown, TObject *, TMouseButton, TShiftState, int, int);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnButtonUp
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur relâche une touche
	//! de la souris précédemment appuyée.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Bouton de la souris relâché
	//! @param[in]				Arg3 Etat des touches ALT, SHIFT et CONTROL
	//! @param[in]				Arg4 Coordonnée X de la souris (coordonnées écran)
	//! @param[in]				Arg5 Coordonnée Y de la souris (coordonnées écran)
	//---------------------------------------------------------------------------

	DECLARE_EVENT_5(TOnMouseUp, OnMouseUp, TObject *, TMouseButton, TShiftState, int, int);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnMouseWheel
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur tourne la molette de la
	//! souris. La valeur de la rotation est de 30 fois la valeur de la propriété
	//! @b WheelScrollLines. Le signe est positif lorsque l'utilisateur pousse la
	//! molette, négatif dans le cas contraire.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Etat des touches ALT, SHIFT et CONTROL
	//! @param[in]				Arg3 Valeur de la rotation (positif = vers l'avant)
	//! @param[in]				Arg4 Coordonnées (X, Y) de la souris (coordonnées écran)
	//! @param[in]				Arg5 Flag Handled laisser à false pour transférer l'évènement au parent
	//!
	//! @sa WheelScrollLines
	//---------------------------------------------------------------------------

	DECLARE_EVENT_5(TOnMouseWheel, OnMouseWheel, TObject *, TShiftState, int, const TPoint &, bool &);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnEnter
	//!
	//! Cet évènement est déclenché lorsque le focus de l'application est passé
	//! au composant ou à un de ses sous-composants.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//---------------------------------------------------------------------------

	DECLARE_EVENT_1(TOnEnter, OnEnter, TObject *);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnExit
	//!
	//! Cet évènement est déclenché lorsque le focus de l'application est passé
	//! à un composant extérieur.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//---------------------------------------------------------------------------

	DECLARE_EVENT_1(TOnExit, OnExit, TObject *);

	//---------------------------------------------------------------------------
	//! @brief Evènement OnDropFile
	//!
	//! Evènement envoyé lorsqu'on fait un drag and drop depuis l'explorateur.
	//! Cet évènement n'est déclenché que si la propriété @b DragAcceptFiles est
	//! à true. Si plusieurs fichiers sont dragués en même temps, chaque fichier
	//! provoque un évènement distinct.
	//!
	//! @param[in]				Sender Objet à l'origine de l'évènement
	//! @param[in]				Arg2 Nom du fichier dragué
	//! @param[in]				Arg3 Etat des touches ALT, SHIFT et CONTROL
	//! @param[in]				Arg4 Coordonnée X du point de drop
	//! @param[in]				Arg5 Coordonnée Y du point de drop
	//!
	//! @sa DragAcceptFiles
	//---------------------------------------------------------------------------

	DECLARE_EVENT_5(TOnDropFile, OnDropFile, TObject *, AnsiString, TShiftState, int, int);


	//@}


	//---------------------------------------------------------------------------
	//! @brief Procédure de fenêtre
	//!
	//! Cette fonction est utilisée en interne pour traiter les messages
	//! systèmes. Ne pas l'utiliser.
	//---------------------------------------------------------------------------

	static LRESULT FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);


};


#else  // TWinControlH

class TWinControl;

#endif	// TWinControlH


#undef In_TWinControlH
