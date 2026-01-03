//-----------------------------------------------------------------------------
//! @file TStatusBar.h
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

#ifdef In_TStatusBarH
#error "Inclusion circulaire de TStatusBar"
#endif // In_TStatusBarH

#define In_TStatusBarH

#ifndef TStatusBarH
#define TStatusBarH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TStatusBar
//!
//! @brief Définition de la classe TStatusBar
//! @image html TStatusbar.jpg "TStatusBar"
//!
//! Cette classe représente une barre de statut présente au bas d'une boîte de
//! dialogue, et qui sert généralement à donner à l'utilisateur des
//! informations succintes (état du programme, étape de progression du
//! traitement, par exemple). Ce contrôle est toujours calé en bas de la
//! fenêtre, ce qui signifie qu'il est inutile de lui donner une position ou
//! un alignement.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TStatusBar: public TWinControl {
private:
  bool FSimplePanel;
  AnsiString FSimpleText;
	int FPanels[256];
	int NbPanels;

protected:

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TStatusBar(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TStatusBar(void);

  //@}


  //! @name Méthodes
  //@{

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
  //! @brief Propriété Panels
	//!
	//! Cette propriété définit la largeur des panels. Elle n'a de signification
	//! que si la propriété @b SimplePanel est égale à @b false. La taille de
	//! chaque panel est définie par rapport au bord gauche du composant, ce qui
	//! signifie qu'il faut impérativement entrer des nombres croissants.@n
	//! Le nombre -1 indique la fin de la liste.
	//!
	//! @sa SimplePanel
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TStatusBar, int, Panels);

  //---------------------------------------------------------------------------
  //! @brief Propriété SimplePanel
	//!
	//! Cette propriété détermine si la barre est composée d'une seule partie (si
	//! la propriété est égale à @b true), ou bien de plusieurs.
	//!
	//! @image html TStatusBar_SimplePanel.jpg "SimplePanel = True"
	//! @image html TStatusBar_MultiplePanel.jpg "SimplePanel = False, Panels = <50, 100, 130, -1>"
	//!
	//! @sa SimpleText, Panels
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStatusBar, bool, SimplePanel);

  //---------------------------------------------------------------------------
  //! @brief Propriété SimpleText
	//!
	//! Cette propriété définit le texte à afficher dans la barre si elle est
	//! composée d'une seule partie (c'est à dire si @b SimplePanel est égale
	//! à @b True).
	//!
	//! @sa SimplePanel
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStatusBar, AnsiString, SimpleText);

  //---------------------------------------------------------------------------
  //! @brief Propriété PanelText
	//!
	//! Cette propriété définit le texte à afficher dans une des parties de la
	//! barre de status. Elle n'a de signification que si la propriété
	//! @b SimplePanel est égale à @b false.
	//!
	//! @sa SimplePanel
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TStatusBar, AnsiString, PanelText);

  //@}


  //! @name Evènements
  //@{

  //@}


};


#else  // TStatusBarH

class TStatusBar;

#endif  // TStatusBarH


#undef In_TStatusBarH
