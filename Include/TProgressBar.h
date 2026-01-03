//-----------------------------------------------------------------------------
//! @file TProgressBar.h
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

#ifdef In_TProgressBarH
#error "Inclusion circulaire de TProgressBar"
#endif // In_TProgressBarH

#define In_TProgressBarH

#ifndef TProgressBarH
#define TProgressBarH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TProgressBarOrientation {
	pbHorizontal,									//!< Barre de progression horizontale (de gauche à droite)
	pbVertical										//!< Barre de progression verticale (de bas en haut)
};


//-----------------------------------------------------------------------------
//! @class TProgressBar
//!
//! @brief Définition de la classe TProgressBar
//! @image html TProgressBar.jpg "TProgressBar"
//!
//! Cette classe représente une barre de progression permettant de faire
//! patienter l'utilisateur lors de traitements longs.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TProgressBar: public TWinControl {
private:
  TColor FFillColor;
  int FStep;

protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TProgressBar(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TProgressBar(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Destruction du contrôle Windows associé au composant
  //!
  //! Cette méthode détruit la fenêtre Windows associée au composant, ceci
	//! pouvant être nécessaire au changement de certaines propriétés. Appeler
	//! ensuite CreateWnd pour recréer une fenêtre avec les mêmes propriétés.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
  //---------------------------------------------------------------------------
  void FASTCALL DestroyWnd(void);

  //---------------------------------------------------------------------------
  //! @brief Création du contrôle Windows associé au composant
  //!
  //! Cette méthode crée la fenêtre Windows associée au composant, ceci pouvant
	//! être nécessaire suite au changement de certaines propriétés. Appeler
	//! DestroyWnd, puis CreateWnd pour recréer la fenêtre. Les propriétés sont
	//! conservées entre la destruction et la création.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
  //---------------------------------------------------------------------------
	void FASTCALL CreateWnd(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Color
  //!
  //! Cette méthode permet d'écrire la propriété Color
  //!
  //! @param[in]        NewColor Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Color(TColor NewColor);

  //---------------------------------------------------------------------------
  //! @brief Avance la position de la barre de la valeur spécifiée (Delta).
  //!
  //! Cette méthode permet d'ajouter la valeur @b Delta à la progression de
	//! la barre. Si la valeur ainsi obtenue dépasse la valeur @b Max, elle est
	//! automatiquement ajustée à cette valeur.
  //!
  //! @param[in]   Delta Valeur à ajouter à Position
  //!
	//! @sa Max
  //---------------------------------------------------------------------------
  void FASTCALL StepBy(int Delta);


  //---------------------------------------------------------------------------
  //! @brief Avance la position de la valeur spécifiée par la propriété @b Step.
  //!
  //! Cette méthode permet d'ajouter la valeur définie par la propriété
	//! @b Step à la progression de la barre. Si la valeur ainsi obtenue dépasse
	//! la valeur @b Max, elle est automatiquement ajustée à cette valeur.
	//!
	//! @sa Max, Step
  //---------------------------------------------------------------------------
  void FASTCALL StepIt(void);


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
  //! @brief Propriété FillColor
	//!
	//! Cette propriété détermine la couleur de la barre de progression elle-même
	//! (la couleur du fond est déterminée par la propriété @b Color du
	//! composant).
	//!
	//! @sa TControl::Color
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProgressBar, TColor, FillColor);

  //---------------------------------------------------------------------------
  //! @brief Propriété Min
	//!
	//! Cette propriété détermine la valeur minimum de la barre de progression
	//! (c'est à dire la valeur lorsqu'elle est à 0%). La valeur par défaut est
	//! de zéro.
	//!
	//! @sa Max
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProgressBar, int, Min);

  //---------------------------------------------------------------------------
  //! @brief Propriété Max
	//!
	//! Cette propriété détermine la valeur maximum de la barre de progression
	//! (c'est à dire la valeur lorsqu'elle est à 100%). La valeur par défaut est
	//! de 100.
	//!
	//! @sa Min
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProgressBar, int, Max);

  //---------------------------------------------------------------------------
  //! @brief Propriété Step
	//!
	//! Cette propriété définit la valeur ajoutée à la progression lorsqu'on
	//! appelle la méthode @b StepIt.
	//!
	//! @sa StepIt
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProgressBar, int, Step);

  //---------------------------------------------------------------------------
  //! @brief Propriété Smooth
	//!
	//! Cette propriété permet de définir si la barre de progression augmente
	//! de façon continue pixel par pixel (lorsque @b Smooth est à Vrai) ou bien
	//! de façon discontinue (la barre est alors représentée avec des petits
	//! carrés dont le nombre dépend de la taille totale de la barre).
	//!
	//! @image html TProgressBar_Smooth.jpg "Smooth = True"
	//! @image html TProgressBar_No_Smooth.jpg "Smooth = False"
	//!
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProgressBar, bool, Smooth);

  //---------------------------------------------------------------------------
  //! @brief Propriété Orientation
	//!
	//! Cette propriété permet d'avoir une barre horizontale (progressant de
	//! gauche à droite) ou verticale (progressant de bas en haut).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProgressBar, TProgressBarOrientation, Orientation);

  //---------------------------------------------------------------------------
  //! @brief Propriété Position
	//!
	//! Cette propriété détermine la progression de la barre, entre les valeurs
	//! des propriétés Min (0%) et Max (100%). Si la valeur affectée n'est pas
	//! dans cet intervalle, elle est automatiquement ajustée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProgressBar, int, Position);

  //@}


  //! @name Evènements
  //@{

  //@}


};


#else  // TProgressBarH

class TProgressBar;

#endif  // TProgressBarH


#undef In_TProgressBarH
