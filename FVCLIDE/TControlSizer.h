//-----------------------------------------------------------------------------
//! @file TControlSizer.h
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


#ifndef TControlSizerH
#define TControlSizerH


//---------------------------------------------------------------------------
// Structures, macros, enums
//---------------------------------------------------------------------------

#define SIZEBITMAP 16
#define SIZECORNERS 3


//---------------------------------------------------------------------------
// Macros, enums
//---------------------------------------------------------------------------

enum ACTION {acNull, acMove,
             acSizeNW, acSizeN, acSizeNE, acSizeE, acSizeSE, acSizeS, acSizeSW, acSizeW,
             acAnchorsLeftTop, acAnchorsRightTop, acAnchorsRightBottom, acAnchorsLeftBottom,
             acAnchorsLeft, acAnchorsTop, acAnchorsRight, acAnchorsBottom};


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnXChange.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnXChange)(TObject *Sender, int NewX);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnYChange.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnYChange)(TObject *Sender, int NewY);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnWidthChange.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnWidthChange)(TObject *Sender, int Width);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnHeightChange.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnHeightChange)(TObject *Sender, int Height);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnAnchorsChange.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnAnchorsChange)(TObject *Sender, TAnchors Anchors);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TClientToScreen.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TClientToScreen)(TObject *Sender, POINT *Pt);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TPlanToClient.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TPlanToClient)(TObject *Sender, POINT *Pt);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TClientToPlan.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TClientToPlan)(TObject *Sender, POINT *Pt);


//---------------------------------------------------------------------------
//! @class TControlSizer
//!
//! @brief Composant de dimensionnement de composants
//!
//! Cette classe (@c TControlSizer) permet de déplacer et de redimensionner un
//! objet rectangulaire (composant, objet, ...). Il comprend un bord hachuré
//! qu'on peut "attraper" et dimensionner comme une boîte de dialogue. Des
//! évènement et des méthodes permettent ensuite de répercuter les modifs sur
//! l'objet sous-jacent.
//---------------------------------------------------------------------------

class TControlSizer: public TObject {
private:

  bool FVisible;
  int FX;
  int FY;
  int FWidth;
  int FHeight;
  TAnchors FAnchors;
	void *FData;

  RECT rcDrawRect;
  int xInit;
  int yInit;


  //---------------------------------------------------------------------------
  //! @brief Détermination de l'action à effectuer.
  //!
  //! Cette fonction détermine l'action à effectuer (dimensionnement,
  //! déplacement) en fonction d'une position (en général, position courante du
  //! curseur).
  //!
  //! @param[in]        x Coordonnée X (Pixels, coordonnées client)
  //! @param[in]        y Coordonnée Y (Pixels, coordonnées client)
  //! @param[in, out]   lpAction Action à effectuer (voir ACTION)
  //!
  //! @return @c bool true si action à effectuer (sinon, lpAction n'est pas
  //! modifié).
  //!
  //! @sa ACTION
  //---------------------------------------------------------------------------

  bool SelectAction(int x, int y, ACTION *lpAction);

protected:

  void PlanToClientRect(LPRECT lpRect);
  void ClientToPlanRect(LPRECT lpRect);

public:

  //! @name Constructeur / destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //!
  //! @param[in]        X Position initiale en X (Pixels, coordonnées écran ou relatives au dialogue)
  //! @param[in]        Y Position initiale en Y (Pixels, coordonnées écran ou relatives au dialogue)
  //! @param[in]        Width Largeur initiale de la fenêtre
  //! @param[in]        Height Hauteur initiale de la fenêtre
  //! @param[in]        Anchors Ancrages initiaux
  //---------------------------------------------------------------------------
  TControlSizer(int x, int y, int Width, int Height, TAnchors Anchors);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
  ~TControlSizer(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Dessin du rectangle "fantôme"
  //!
  //! Cette fonction dessine le rectangle représentant le composant lors des
  //! déplacements et dimensionnements.
  //---------------------------------------------------------------------------
  void DrawRect(void);

  //---------------------------------------------------------------------------
  //! @brief Dessin de la fenêtre
  //!
  //! Cette fonction permet de dessiner le composant en réponse à un message
  //! WM_PAINT ou AfterPaint (dans le cas des plugins).
  //!
  //! @param[in]        hdc Contexte de dispositif destination
  //---------------------------------------------------------------------------
  void Paint(HDC hdc);

  //---------------------------------------------------------------------------
  //! @brief Traitement d'un déplacement souris
  //!
  //! Cette fonction effectue les opérations consécutives à un déplacement de la
  //! souris (déplacement de la fenêtre si une action est en cours).
  //!
  //! @param[in]        X Position X de la souris (Pixels, coordonnées client)
  //! @param[in]        Y Position Y de la souris (Pixels, coordonnées client)
  //! @param[in]        XRound X calé sur la grille
  //! @param[in]        YRound Y calé sur la grille
  //! @param[in, out]   lpAction Action à effectuer
  //!
  //! @return @c bool true uniquement dans le cas où lpAction a été modifié.
  //---------------------------------------------------------------------------
  bool TraitMouseMove(int x, int y, int XRound, int YRound, ACTION *lpAction);

  //---------------------------------------------------------------------------
  //! @brief Traitement de l'appui sur le bouton gauche de la souris.
  //!
  //! Cette fonction effectue les opérations consécutives à un clic gauche de la
  //! souris (initialisation des fonctions de déplacement ou de dimensionnement).
  //!
  //! @param[in]        X Position X de la souris (Pixels, coordonnées client)
  //! @param[in]        Y Position Y de la souris (Pixels, coordonnées client)
  //! @param[in]        XRound X calé sur la grille
  //! @param[in]        YRound Y calé sur la grille
  //! @param[in, out]   lpAction Action à effectuer
  //!
  //! @return @c bool toujours true
  //---------------------------------------------------------------------------
  bool TraitLButtonDown(int x, int y, int XRound, int YRound, ACTION *lpAction);

  //---------------------------------------------------------------------------
  //! @brief Traitement du relachement du bouton gauche de la souris.
  //!
  //! Cette fonction effectue les opérations consécutives à une fin de clic
  //! gauche de la souris (déplacement ou dimensionnement définitif).
  //!
  //! @param[in]        X Position X de la souris (Pixels, coordonnées client)
  //! @param[in]        Y Position Y de la souris (Pixels, coordonnées client)
  //! @param[in]        XRound X calé sur la grille
  //! @param[in]        YRound Y calé sur la grille
  //! @param[in]        lpAction Action à effectuer
  //!
  //! @return @c bool toujours true
  //!
  //! @note le paramètre lpAction est passé par adresse uniquement pour
  //! uniformisation des fonctions de traitement des messages souris.
  //---------------------------------------------------------------------------
  bool TraitLButtonUp(int x, int y, int XRound, int YRound, ACTION *lpAction);

  //---------------------------------------------------------------------------
  //! @brief Détermine si un point est dans la fenêtre
  //!
  //! Cette fonction détermine si le point de coordonnées (x, y) est dans la
  //! fenêtre (en prenant en compte l'épaisseur de la bordure).
  //!
  //! @param[in]        X Position X (pixels, coordonnées client)
  //! @param[in]        Y Position Y (pixels, coordonnées client)
  //!
  //! @return @c bool true si dans le cadre
  //---------------------------------------------------------------------------
  bool IsInFrame(int x, int y);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Position et dimension
	//!
	//! Ces propriétés mémorisent la position et les dimensions.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlSizer, int, X);
  DECLARE_PROPERTY(TControlSizer, int, Y);
  DECLARE_PROPERTY(TControlSizer, int, Width);
  DECLARE_PROPERTY(TControlSizer, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Position et dimension du rectangle de redimensionnement
	//!
	//! Ces propriétés renvoient la position et les dimensions du rectangle
	//! de redimensionnement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TControlSizer, int, XDrawRect);
  DECLARE_PROPERTY_GET(TControlSizer, int, YDrawRect);
  DECLARE_PROPERTY_GET(TControlSizer, int, WidthDrawRect);
  DECLARE_PROPERTY_GET(TControlSizer, int, HeightDrawRect);

  //---------------------------------------------------------------------------
  //! @brief Propriété Anchors
	//!
	//! Cette propriété mémorise les ancrages
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlSizer, TAnchors, Anchors);


  //---------------------------------------------------------------------------
  //! @brief Propriété bVisible
	//!
	//! Cette propriété mémorise la visibilité.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlSizer, bool, Visible);

  //---------------------------------------------------------------------------
  //! @brief Propriété Data
	//!
	//! Cette propriété permet d'associer une donnée quelconque à un élément.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlSizer, void *, Data);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement déclenché lorsque la propriété @b X change
	//!
	//! Cette évènement est appelé lorsque l'utilisateur redimensionne le
	//! contrôle et que la propriété @b X change.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]				Arg2 X: Coordonnées X du point supérieur gauche
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TOnXChange, OnXChange, TObject *, int);

  //---------------------------------------------------------------------------
  //! @brief Evènement déclenché lorsque la propriété @b Y change
	//!
	//! Cette évènement est appelé lorsque l'utilisateur redimensionne le
	//! contrôle et que la propriété @b Y change.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]				Arg2 Y: Coordonnées Y du point supérieur gauche
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TOnYChange, OnYChange, TObject *, int);

  //---------------------------------------------------------------------------
  //! @brief Evènement déclenché lorsque la propriété @b Width change
	//!
	//! Cette évènement est appelé lorsque l'utilisateur redimensionne le
	//! contrôle et que la propriété @b Width change.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]				Arg2 Width: Largeur du contrôle
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TOnWidthChange, OnWidthChange, TObject *, int);

  //---------------------------------------------------------------------------
  //! @brief Evènement déclenché lorsque la propriété @b Height change
	//!
	//! Cette évènement est appelé lorsque l'utilisateur redimensionne le
	//! contrôle et que la propriété @b Height change.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]				Arg2 Height: Hauteur du contrôle
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TOnHeightChange, OnHeightChange, TObject *, int);

  //---------------------------------------------------------------------------
  //! @brief Evènement déclenché lorsque la propriété @b Anchor change
	//!
	//! Cette évènement est appelé lorsque l'utilisateur change la propriété
	//! @b Anchor avec la souris.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]				Arg2 Anchor: Ancrages
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TOnAnchorsChange, OnAnchorsChange, TObject *, TAnchors);

  //---------------------------------------------------------------------------
  //! @brief Changement de coordonnées client -> écran
	//!
	//! Cette évènement est appelé lorsqu'il est nécessaire de transformer les
	//! coordonnées client (relatives au coin supérieur gauche de la fenêtre
	//! parent) en coordonnées écran (relatives au coin supérieur gauche de
	//! l'écran).@n
	//! L'utilisateur de cette classe doit impérativement traiter cet évènement
	//! pour assurer son bon fonctionnement.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Coordonnées du point à transformer
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TClientToScreen, ClientToScreen, TObject *, POINT *);

  //---------------------------------------------------------------------------
  //! @brief Changement de coordonnées plan -> client
	//!
	//! Cette évènement est appelé lorsqu'il est nécessaire de transformer les
	//! coordonnées plan (coordonnées réelles) en coordonnées client en fonction
	//! des paramètres de zoom.@n
	//! Si l'utilisateur de cette classe ne traite pas cet évènement, cela
	//! signifie que les coordonnées plan et client sont les mêmes (c'est à dire
	//! qu'il n'y a pas de zoom).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Coordonnées du point à transformer
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TPlanToClient, PlanToClient, TObject *, POINT *);

  //---------------------------------------------------------------------------
  //! @brief Changement de coordonnées client -> plan
	//!
	//! Cette évènement est appelé lorsqu'il est nécessaire de transformer les
	//! coordonnées client en coordonnées plan (coordonnées réelles) en fonction
	//! des paramètres de zoom.@n
	//! Si l'utilisateur de cette classe ne traite pas cet évènement, cela
	//! signifie que les coordonnées plan et client sont les mêmes (c'est à dire
	//! qu'il n'y a pas de zoom).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Coordonnées du point à transformer
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TClientToPlan, ClientToPlan, TObject *, POINT *);

  //@}

};

//---------------------------------------------------------------------------
#endif
