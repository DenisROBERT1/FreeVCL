//-----------------------------------------------------------------------------
//! @file TLink.h
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

#ifdef In_TLinkH
#error "Inclusion circulaire de TLink"
#endif // In_TLinkH

#define In_TLinkH

#ifndef TLinkH
#define TLinkH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

//-----------------------------------------------------------------------------
//! @class TLink
//!
//! @brief Définition de la classe TLink
//!
//! Cette classe permet de faire un hyper-lien (comme dans une page HTML).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TLink: public TWinControl {
private:
	TPopupMenu *PopupMenuLink;
protected:
	TAlignment FAlignment;
  AnsiString FHRef;
  TColor FLinkColor;
  TColor FALinkColor;
  TColor FVLinkColor;
  bool FVisited;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessPaint(HDC hdc, RECT rcPaint);
  virtual bool ProcessMouseMove(int X, int Y);
  virtual bool ProcessLButtonUp(int X, int Y);
  virtual bool ProcessCaptureChanged(HWND hWndNewCapture);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TLink(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TLink(void);

  //@}


  //---------------------------------------------------------------------------
  //! @brief Traitement des évènements
  //---------------------------------------------------------------------------

  FVCL_BEGIN_EVENTS
  void OuvrirClick(TObject *Sender);
  void CopierClick(TObject *Sender);
  void ProprietesClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TLink, TNotifyEvent, OnClick, OuvrirClick, TObject*);
		SET_EVENT_STR_1(TLink, TNotifyEvent, OnClick, CopierClick, TObject*);
		SET_EVENT_STR_1(TLink, TNotifyEvent, OnClick, ProprietesClick, TObject*);
  }

  FVCL_END_EVENTS

  //---------------------------------------------------------------------------
  //! @brief Ouvre la page de lien
	//!
	//! Cette méthode ouvre l'URL définie dans la propriété @b HRef. L'appel de
	//! cette méthode a pour effet de mettre la propriété @b Visited à true;
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool Execute(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Caption
  //!
  //! Cette méthode permet d'écrire la propriété Caption
  //!
  //! @param[in]        NewCaption Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Caption(AnsiString NewCaption);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Font
  //!
  //! Cette méthode permet d'écrire la propriété Font
  //!
  //! @param[in]        NewFont Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Font(TFont *NewFont);


  //---------------------------------------------------------------------------
  //! @brief Indique si le contrôle peut se redimensionner automatiquement.
	//!
  //! Cette méthode est appelée lorsque le composant est redimensionné par
	//! l'utilisateur ou par programme. Ce composant renvoie false si les
	//! dimensions proposées sont inférieures aux dimensions minimum nécessaires
	//! pour afficher le composant. Dans ce cas, le redimensionnement est annulé.
  //!
  //! @param[in]        NewWidth Nouvelle largeur du composant.
  //! @param[in]        NewHeight Nouvelle hauteur du composant.
  //!
  //! @return @c bool true si nouvelles dimensions Ok. false sinon.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL CanAutoSize(int &NewWidth, int &NewHeight);

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
  //! @brief Propriété Alignment: Alignement horizontal.
	//!
	//! Cette méthode permet d'aligner horizontalement le texte du composant à
	//! gauche, au centre ou à droite (fonctionne comme TLabel::Alignment).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLink, TAlignment, Alignment);


  //---------------------------------------------------------------------------
  //! @brief Propriété HRef: Adresse du lien.
	//!
	//! Affectez cette propriété pour indiquer l'adresse (URL) vers laquelle sera
	//! dirigé l'utilisateur lorsqu'il cliquera sur le lien (ou bien lors de
	//! l'appel de la fonction @b Execute par le programme.@n
	//! @b HRef peut être une adresse internet, une adresse mail, ou un fichier
	//! local (la syntaxe est la même que dans une page HTML).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLink, AnsiString, HRef);


	//---------------------------------------------------------------------------
  //! @brief Propriété LinkColor: Couleur du lien non visité.
	//!
	//! Cette propriété représente la couleur du lien lorsque l'adresse n'a pas
	//! encore été visitée (c'est à dire lorsque la propriété @b Visited est à
	//! false).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLink, TColor, LinkColor);


  //---------------------------------------------------------------------------
  //! @brief Propriété ALinkColor: Couleur du lien sur un MouseMove.
	//!
	//! Cette propriété représente la couleur du lien lorsque l'utilisateur
	//! déplace la souris sur le lien.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLink, TColor, ALinkColor);


  //---------------------------------------------------------------------------
  //! @brief Propriété VLinkColor: Couleur du lien visité.
	//!
	//! Cette propriété représente la couleur du lien lorsque l'adresse a été
	//! visitée (c'est à dire lorsque la propriété @b Visited est à true).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLink, TColor, VLinkColor);


	//---------------------------------------------------------------------------
  //! @brief Propriété Visited
	//!
	//! Forcez cette valeur pour indiquer que le lien a déjà été visité (ce qui
	//! change potentiellement sa couleur). Cette propriété se met
	//! automatiquement à true lorsque l'utilisateur clique sur le lien ou que le
	//! programme appelle la méthode @b Execute.@n
	//! Par contre, l'état "visité" d'un lien n'est pas mémorisé lors de la
	//! destruction du composant, ou bien lorsque deux liens pointent vers la
	//! même adresse. Ce qui signifie que si vous voulez avoir une représentation
	//! exacte de l'état visité, il vous faut mémoriser de façon externe cet
	//! état et affecter la propriété en fonction de cet état lors de la création
	//! du composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLink, bool, Visited);


  //@}


};


#else  // TLinkH

class TLink;

#endif  // TLinkH


#undef In_TLinkH
