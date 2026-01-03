//-----------------------------------------------------------------------------
//! @file TTrackBar.h
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

#ifdef In_TTrackBarH
#error "Inclusion circulaire de TTrackBar"
#endif // In_TTrackBarH

#define In_TTrackBarH

#ifndef TTrackBarH
#define TTrackBarH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TTrackBarOrientation {
	trHorizontal,	   //!< Curseur horizontal
	trVertical       //!< Curseur vertical
};

enum TTickMark {
	tmBottomRight,	 //!< Graduations en dessous (si curseur horizontal) ou à droite (si vertical)
	tmTopLeft,	     //!< Graduations au dessus (si curseur horizontal) ou à gauche (si vertical)
	tmBoth					 //!< Graduations des deux cotés
};

enum TTickStyle {
	tsNone,					 //!< Pas de graduations
	tsAuto,					 //!< Graduations automatique
	tsManual			   //!< Graduations manuelles (utiliser la méthode SetTick pour les positionner)
};


//-----------------------------------------------------------------------------
//! @class TTrackBar
//!
//! @brief Curseur de réglage.
//! @image html TTrackBar.jpg "TTrackBar 1" 
//! @image html TTrackBar_vert.jpg "TTrackBar 2"
//!
//! Cette classe permet de créer un curseur de réglage. Ce curseur peut prendre
//! différentes formes, être horizontal ou vertical, les graduations peuvent
//! également être paramétrées.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTrackBar: public TWinControl {
private:
protected:
  int FFrequency;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessHScroll(int Command, int Info);
  virtual bool ProcessVScroll(int Command, int Info);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTrackBar(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTrackBar(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ajoute une marque de graduation à l'emplacement spécifié.
  //!
	//! Cette méthode ajoute une graduation à l'emplacement Value. Elle ne doit
	//! être utilisée que si la propriété @b TickStyle est à @b tsManual.
	//!
  //! @param[in]        Value Emplacement de la graduation.
	//!
	//! @sa TickStyle
  //---------------------------------------------------------------------------
  void FASTCALL SetTick(int Value);

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
  //! @brief Propriété EnableSelRange. Activation de la sélection.
	//! @image html TTrackBar_esr.jpg "EnableSelRange = True"
	//!
	//! Cette propriété permet la possibilité de définir une sélection pour le
	//! curseur. Lorsque cette propriété est activée, la glissière de réglage
	//! devient plus large pour afficher la sélection.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, bool, EnableSelRange);


  //---------------------------------------------------------------------------
  //! @brief Propriété Frequency. Fréquence des graduations.
	//!
	//! Cette propriété détermine la fréquence des graduations. Elle ne doit être
	//! utilisée que si la propriété @b TickStyle est à @b tsAuto.
	//!
	//! @sa TickStyle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, int, Frequency);


  //---------------------------------------------------------------------------
  //! @brief Propriété LineSize. Granularité du déplacement.
	//!
	//! Cette propriété définit le déplacement du curseur lorsque l'utilisateur
	//! utilise les flèches de direction. Quelles que soient les valeurs de
	//! @b Min et @b Max, Les touches droite et bas déplacent toutes deux le
	//! curseur vers le bas ou la droite (suivant la valeur de la propriété
	//! @b Orientation), les touches haut et gauche vers le haut ou vers la
	//! gauche, sauf si la valeur de @b LineSize est négative (dans ce cas, les
	//! déplacements sont inversés).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, int, LineSize);


  //---------------------------------------------------------------------------
  //! @brief Propriété Max. Maximum de l'échelle des graduations.
	//!
	//! Cette propriété définit la valeur maximum des graduations. La valeur par
	//! défaut est 10.
	//!
	//! @sa Min
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, long, Max);

  //---------------------------------------------------------------------------
  //! @brief Propriété Min. Minimum de l'échelle des graduations.
	//!
	//! Cette propriété définit la valeur minimum des graduations. La valeur par
	//! défaut est zéro. Il est possible d'avoir une valeur @b Min supérieure à
	//! la valeur @b Max pour avoir un curseur allant de droite à gauche ou de
	//! bas en haut. La molette de la souris déplace toujours le curseur vers la
	//! valeur la plus élevée des propriétés @b Min et @b Max lorsqu'on pousse la
	//! molette vers le haut, et inversement.@n
	//! Si les propriétés @b LineSize et @b PageSize ont des valeurs positives
	//! (cas le plus courant), les flèches de déplacement bougent le curseur dans
	//! le sens le plus intuitif pour l'utilisateur (mais pas forcément pour le
	//! programmeur, notamment lorsque le zéro est en haut ou à droite).
	//!
	//! @sa Max, LineSize, PageSize
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, long, Min);

  //---------------------------------------------------------------------------
  //! @brief Propriété Orientation. Orientation du curseur.
	//!
	//! Cette propriété définit l'orientation de la barre du curseur (horizontal
	//! ou vertical). La valeur par défaut est horizontal. La valeur Min du
	//! curseur est à gauche pour un curseur horizontal, en haut pour un curseur
	//! vertical.
	//!
	//! @sa Min, Max
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, TTrackBarOrientation, Orientation);

  //---------------------------------------------------------------------------
  //! @brief Propriété PageSize. Granularité du déplacement rapide.
	//!
	//! Cette propriété définit le déplacement du curseur lorsque l'utilisateur
	//! utilise les touches "Page haut" ou "Page bas", ou bien clique sur la
	//! glissière à coté du bouton de réglage. Quelles que soient les valeurs de
	//! @b Min et @b Max, La touche "Page bas" déplace le curseur vers le bas ou
	//! la droite (suivant la valeur de la propriété @b Orientation), la touche
	//! "Page haut" vers le haut ou vers la gauche, sauf si la valeur de
	//! @b PageSize est négative (dans ce cas, les déplacements sont inversés).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, long, PageSize);

  //---------------------------------------------------------------------------
  //! @brief Propriété Position. Position du bouton de réglage.
	//!
	//! Utilisez cette propriété pour lire ou pour définir la position du curseur
	//! de réglage. Si la valeur définie n'est pas comprise entre @b Min et
	//! @b Max, la position est automatiquement réajustée à la valeur Min ou Max.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, long, Position);

  //---------------------------------------------------------------------------
  //! @brief Propriété SelEnd. Fin de sélection.
	//!
	//! Cette propriété définit la fin de la sélection. La propriété
	//! @b EnableSelRange doit être activée.
	//!
	//! @sa EnableSelRange
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, int, SelEnd);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelStart. Début de sélection.
	//!
	//! Cette propriété définit le début de la sélection. La propriété
	//! @b EnableSelRange doit être activée.
	//!
	//! @sa EnableSelRange
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, int, SelStart);


  //---------------------------------------------------------------------------
  //! @brief Propriété SliderVisible. Visibilité du bouton de réglage.
	//!
	//! Cette propriété définit la visibilité du bouton de réglage.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, bool, SliderVisible);


  //---------------------------------------------------------------------------
  //! @brief Propriété ThumbLength. Largeur du bouton de réglage.
	//!
	//! Cette propriété définit la largeur du bouton de réglage.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, int, ThumbLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété TickMarks. Position des graduations.
	//!
	//! Cette propriété définit la position des graduations par rapport à la
	//! glissière de réglage. Si cette propriété est définie à @b tmBoth, la
	//! forme du bouton de réglage est modifiée (il devient rectangulaire).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, TTickMark, TickMarks);


  //---------------------------------------------------------------------------
  //! @brief Propriété TickStyle. Affichage des graduations.
	//!
	//! Cette propriété définit la façon d'afficher les graduations. Lorsque
	//! cette propriété est égale à @b tsManual, utilisez la méthode @b SetTick
	//! pour positionner les graduations.
	//!
	//! @sa SetTick
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTrackBar, TTickStyle, TickStyle);


  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange.
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur modifie la position du
	//! curseur, par le clavier ou par la souris. Il est aussi déclenché lorsque
	//! la position est modifiée par programme (propriété @b Position).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //@}


};


#else  // TTrackBarH

class TTrackBar;

#endif  // TTrackBarH


#undef In_TTrackBarH
