//-----------------------------------------------------------------------------
//! @file TColorDialog.h
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

#ifdef In_TColorDialogH
#error "Inclusion circulaire de TColorDialog"
#endif // In_TColorDialogH

#define In_TColorDialogH

#ifndef TColorDialogH
#define TColorDialogH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TColorDialogOption {
	cdFullOpen,										//!< A l'ouverture, affiche la partie droite du dialogue permettant de choisir les couleurs personnalisées
	cdPreventFullOpen,						//!< Désactive le bouton permettant l'affichage de la partie droite du dialogue
	cdShowHelp,										//!< Affichage du bouton "Aide"
	cdSolidColor,									//!< Force la couleur choisie à la couleur affichable la plus proche
	cdAnyColor										//!< Permet de choisir n'importe quelle couleur (y compris celles non affichable par le système)
};

typedef Set<TColorDialogOption, cdFullOpen, cdAnyColor> TColorDialogOptions;  //!< Combinaison de plusieurs TColorDialogOption

//-----------------------------------------------------------------------------
//! @class TColorDialog
//!
//! @brief Définition de la classe TColorDialog
//! @image html TColorDialog.jpg "TColorDialog"
//!
//! Cette classe définit un boîte de dialogue permettant de choisir une couleur
//! soit parmi une liste de couleurs prédéfinies, soit à partir de ses trois
//! composantes rouge vert et bleu.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TColorDialog: public TComponent {
private:
protected:
  COLORREF CustColor[16];
  DWORD Flags;
  HWND Handle;

  TColor FColor;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TColorDialog(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TColorDialog(void);

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
  //! @brief Ouverture dialogue
  //---------------------------------------------------------------------------

  virtual bool Execute(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Color
	//!
	//! Cette propriété définit la couleur choisie par l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorDialog, TColor, Color);

  //---------------------------------------------------------------------------
  //! @brief Propriété Options
	//!
	//! Cette propriété permet de définir les options de la boîte de dialogue,
	//! comme la présence des couleurs personnalisées, des couleurs étendues.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TColorDialog, TColorDialogOptions, Options);

  //@}

};


#else  // TColorDialogH

class TColorDialog;

#endif  // TColorDialogH


#undef In_TColorDialogH
