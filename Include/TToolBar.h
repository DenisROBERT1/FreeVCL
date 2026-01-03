//-----------------------------------------------------------------------------
//! @file TToolBar.h
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

#ifdef In_TToolBarH
#error "Inclusion circulaire de TToolBar"
#endif // In_TToolBarH

#define In_TToolBarH

#ifndef TToolBarH
#define TToolBarH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TToolBar
//!
//! @brief Définition de la classe TToolBar
//! @image html TToolBar.jpg "TToolBar"
//!
//! Cette classe est un contrôle représentant une barre d'outils (barre
//! généralement en haut d'une fenêtre, juste sous le menu, et qui contient des
//! boutons). Pour ajouter des boutons, utilisez la propriété @b Images de ce
//! contrôle.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TToolBar: public TWinControl {
private:
protected:
  bool FFlat;
  int FButtonWidth;
  int FButtonHeight;
  TImageList * FHotImages;
  TImageList * FImages;


  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TToolBar(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TToolBar(void);

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

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction de la fiche
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Flat: boutons plats
	//!
	//! Cette propriété permet de définir l'aspect plat ou en relief des boutons
	//! de la ToolBar.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TToolBar, bool, Flat);

  //---------------------------------------------------------------------------
  //! @brief Propriété ButtonWidth: largeur des boutons
	//!
	//! Cette propriété définit la largeur des boutons de la ToolBar.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TToolBar, int, ButtonWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété ButtonHeight: hauteur des boutons
	//!
	//! Cette propriété définit la hauteur des boutons de la ToolBar.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TToolBar, int, ButtonHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété HotImages
	//!
	//! Cette propriété permet de définir une liste d'images utilisée pour
	//! dessiner les boutons lorsque la souris survole un bouton. Si cette
	//! propriété est à NULL, le dessin du bouton n'est pas modifié lorsque la
	//! souris passe dessus.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TToolBar, TImageList *, HotImages);

  //---------------------------------------------------------------------------
  //! @brief Propriété Images
	//!
	//! Cette propriété permet de définir la liste d'images utilisée pour
	//! dessiner les boutons.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TToolBar, TImageList *, Images);

  //@}


};


#else  // TToolBarH

class TToolBar;

#endif  // TToolBarH


#undef In_TToolBarH
