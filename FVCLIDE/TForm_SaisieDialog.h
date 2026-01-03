//-----------------------------------------------------------------------------
//! @file TForm_SaisieDialog.h
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

#ifndef TForm_SaisieDialogH
#define TForm_SaisieDialogH



//---------------------------------------------------------------------------
class TForm_SaisieDialog: public TForm {
protected:
	TElemBase *FElemBase;
	TFrame_SaisieDialog *FFrame_SaisieDialog;
	int FIndexDialog;
  int IdClick;

  HCURSOR hCursorWE;
  HCURSOR hCursorNS;
  HCURSOR hCursorNESW;
  HCURSOR hCursorNWSE;
  HCURSOR hCursorMove;
  HCURSOR hCursorArrow;
  HCURSOR hCursorCross;

  // Rectangle d'aperçu
  RECT rcDrawRect;
  bool bDrawRect;

	// Dernières dimensions (pour le undo)
	int LastWidth;
	int LastHeight;

	//---------------------------------------------------------------------------
  //! @brief Dessin d'un rectangle
  //!
  //! Cette méthode dessine un rectangle représentant un nouveau composant, un
  //! rectangle de sélection, etc...
  //---------------------------------------------------------------------------
  void DrawRect(void);

  //---------------------------------------------------------------------------
  //! @brief Méthode à appeler après un redimensionnement
  //!
  //! Cette méthode réajuste la position des ControlSizer après un
	//! redimensionnement.
  //---------------------------------------------------------------------------
  void AfterResize(void);

public:
  FVCL_BEGIN_COMPONENTS
  FVCL_END_COMPONENTS

	FVCL_BEGIN_EVENTS
  void FormPaint(TObject *Sender);
  void FormResize(TObject *Sender);
  void FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
  void FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void FormClose(TObject *Sender, TCloseAction &Action);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_SaisieDialog, TNotifyEvent, OnPaint, FormPaint, TObject*);
    SET_EVENT_STR_1(TForm_SaisieDialog, TNotifyEvent, OnResize, FormResize, TObject*);
    SET_EVENT_STR_5(TForm_SaisieDialog, TOnMouseDown, OnMouseDown, FormMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_4(TForm_SaisieDialog, TOnMouseMove, OnMouseMove, FormMouseMove, TObject*, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_SaisieDialog, TOnMouseUp, OnMouseUp, FormMouseUp, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_2(TForm_SaisieDialog, TOnClose, OnClose, FormClose, TObject *, TCloseAction &);
  }

	FVCL_END_EVENTS

	// Pointeur partagé (par chaque composant de la form) sur l'ensemble des contrôleurs
	// Il faut utiliser un shared_ptr car l'ordre de destruction est difficilement prévisible
	std::shared_ptr<TControlSizer_Collection> shared_ptr_ControlSizer_Collection;

  //! @name Constructeur / destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
	TForm_SaisieDialog(TComponent *Owner, LPCTSTR szName);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
	virtual ~TForm_SaisieDialog(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Width
  //!
  //! @param[in]        NewWidth Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Width(int NewWidth);

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Height
  //!
  //! @param[in]        NewHeight Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Height(int NewHeight);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées client en coordonnées écran
  //!
  //! Cette méthode transforme les coordonnées client (par rapport au coin
  //! supérieur gauche de la fenêtre) en coordonnées écran.
  //!
  //! @param[in, out]    lpPoint Coordonnées
  //---------------------------------------------------------------------------
  void ClientToScreen(TObject *Sender, LPPOINT lpPoint);

  //---------------------------------------------------------------------------
  //! @brief Transformation d'un rectangle des coordonnées plan en coordonnées
  //! client.
  //!
  //! Cette méthode transforme un rectangle des coordonnées plan en coordonnées
  //! client.
  //!
  //! @param[in, out]    lpRect Rectangle à transformer
  //!
  //! @note Les coordonnées sont automatiquement inversées si, suite à la
  //! transformation, left est supérieur à right ou top à bottom.
  //---------------------------------------------------------------------------
  void PlanToClientRect(LPRECT lpRect);

  //---------------------------------------------------------------------------
  //! @brief Transformation d'un rectangle des coordonnées client en coordonnées
  //! plan.
  //!
  //! Cette méthode transforme un rectangle des coordonnées client en
  //! coordonnées plan.
  //!
  //! @param[in, out]    lpRect Rectangle à transformer
  //!
  //! @warning Suivant le sens du système de coordonnées plan, on peut avoir
  //! bottom supérieur à top ou top à bottom.
  //---------------------------------------------------------------------------
  void ClientToPlanRect(LPRECT lpRect);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées écran en coordonnées client
  //!
  //! Cette méthode transforme les coordonnées client en coordonnées plan
  //! (coordonnées réelles) en fonction des paramètres de zoom.
  //!
  //! @param[in, out]   X Coordonnée X du point
  //! @param[in, out]   Y Coordonnée Y du point
  //---------------------------------------------------------------------------
  void ClientToPlanXY(int *X, int *Y);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées écran en coordonnées client
  //!
  //! Cette méthode transforme les coordonnées plan (coordonnées réelles) en
  //! coordonnées client en fonction des paramètres de zoom.
  //!
  //! @param[in, out]    lpPoint Coordonnées
  //---------------------------------------------------------------------------
  void PlanToClient(TObject *Sender, LPPOINT lpPoint);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées écran en coordonnées client
  //!
  //! Cette méthode transforme les coordonnées client en coordonnées plan
  //! (coordonnées réelles) en fonction des paramètres de zoom.
  //!
  //! @param[in, out]    lpPoint Coordonnées
  //---------------------------------------------------------------------------
  void ClientToPlan(TObject *Sender, LPPOINT lpPoint);

  //---------------------------------------------------------------------------
  //! @brief Régénération de l'image
  //!
  //! Cette méthode doit être appelée par l'objet dérivé en fin de
  //! régénération de l'image. C'est dans cette fonction qu'est dessinée la
  //! grille, les TControlSizer, éventuellement les ancrages, etc...
  //!
  //! @param[in]        hdc HDC dans lequel dessiner
  //! @param[in]        Rect Rectangle de dessin
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool TraitAfterPaint(HDC hdc, TRect &Rect);

  //---------------------------------------------------------------------------
  //! @brief Déplacement souris
  //!
  //! Cette méthode doit être appelée par l'objet dérivé en réponse à
  //! l'évènement OnMouseMove.
  //!
  //! @param[in]        X Position de la souris en X
  //! @param[in]        Y Position de la souris en Y
  //!
  //! @return @c bool true si message traité
  //---------------------------------------------------------------------------
  bool TraitMouseMove(int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Clic gauche
  //!
  //! Cette méthode doit être appelée par l'objet dérivé en réponse à
  //! l'évènement OnLButtonDown.
  //!
  //! @param[in]        X Position de la souris en X
  //! @param[in]        Y Position de la souris en Y
  //!
  //! @return @c bool true si message traité
  //---------------------------------------------------------------------------
  bool TraitLButtonDown(int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Fin de clic gauche
  //!
  //! Cette méthode doit être appelée par l'objet dérivé en réponse à
  //! l'évènement OnLButtonUp.
  //!
  //! @param[in]        X Position de la souris en X
  //! @param[in]        Y Position de la souris en Y
  //!
  //! @return @c bool true si message traité
  //---------------------------------------------------------------------------
  bool TraitLButtonUp(int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Clic droit
  //!
  //! Cette méthode doit être appelée par l'objet dérivé en réponse à
  //! l'évènement OnRButtonDown.
  //!
  //! @param[in]        X Position de la souris en X
  //! @param[in]        Y Position de la souris en Y
  //!
  //! @return @c bool true si message traité
  //---------------------------------------------------------------------------
  bool TraitRButtonDown(int X, int Y);

  //@}

	//! @name Propriétés
	//@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ElemBase
	//!
	//! Cette propriété permet de lire l'objet ElemBase sous-jacent.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_SaisieDialog, TElemBase *, ElemBase);

  //---------------------------------------------------------------------------
  //! @brief Propriété Frame_SaisieDialog
	//!
	//! Cette propriété permet de lire la fenêtre de saisie des dialogues.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_SaisieDialog, TFrame_SaisieDialog *, Frame_SaisieDialog);

  //---------------------------------------------------------------------------
  //! @brief Propriété IndexDialog
	//!
	//! Cette propriété permet de mémoriser l'index de la fenêtre dans la liste
	//! des dialogues.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_SaisieDialog, int, IndexDialog);


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

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChangeSelection
	//!
	//! Cet évènement est déclenché lorsque la sélection est modifiée.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnChangeSelection, OnChangeSelection, TObject *);

	//@}

};
//---------------------------------------------------------------------------
#endif
