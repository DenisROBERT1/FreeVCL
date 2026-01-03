//-----------------------------------------------------------------------------
//! @file TFontDialog.h
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

#ifdef In_TFontDialogH
#error "Inclusion circulaire de TFontDialog"
#endif // In_TFontDialogH

#define In_TFontDialogH

#ifndef TFontDialogH
#define TFontDialogH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TFont.h"


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TFontDialogOption {
	fdAnsiOnly,										//!< Affiche uniquement les fontes de caractères ANSI (pas les fontes de symboles).
	fdTrueTypeOnly,								//!< Affiche uniquement les fontes true-type.
	fdEffects,										//!< Affiche les options permettant le texte souligné, barré et le changement de couleur.
	fdFixedPitchOnly,							//!< Affiche uniquement les fontes à largeur de caractères fixes.
	fdForceFontExist,							//!< Affiche uniquement les fontes existantes.
	fdNoFaceSel,									//!< Ne présélectionne aucune fonte.
	fdNoOEMFonts,									//!< N'affiche pas les fontes de caractères OEM.
	fdNoSimulations,							//!< N'affiche pas l'option "gras" et "italiques" pour les fontes qui ne le supportent pas nativement (dans ce cas ces options sont habituellement simulées par le GDI).
	fdNoSizeSel,									//!< N'affiche pas l'option de sélection de la taille.
	fdNoStyleSel,									//!< N'affiche pas l'option de sélection du style.
	fdNoVectorFonts,							//!< N'affiche pas les fontes vectorielles.
	fdShowHelp,										//!< Affiche le bouton "Aide".
	fdWysiwyg,										//!< Affiche uniquement les fontes indépendantes du périphérique.
	fdLimitSize,									//!< Limite la taille aux minimum et maximum définies pas les propriétés MaxFontSize et MinFontSize.
	fdScalableOnly,								//!< Affiche uniquement les fontes pouvant être dimensionnées.
	fdApplyButton									//!< Affiche le bouton "Appliquer". Interceptez l'évènement "OnApply" pour appliquer ensuite les modifications dans votre programme.
};

typedef Set<TFontDialogOption, fdAnsiOnly, fdApplyButton> TFontDialogOptions;  //!< Combinaison de plusieurs TFontDialogOption

enum TFontDialogDevice {
	fdScreen,											//!< Fontes affichables à l'écran.
	fdPrinter,										//!< Fontes affichables sur imprimante.
	fdBoth												//!< Fontes affichables à l'écran et sur imprimante.
};

//-----------------------------------------------------------------------------
//! @class TFontDialog
//!
//! @brief Définition de la classe TFontDialog
//!
//! Cette classe définit une boîte de dialogue permettant de choisir une fonte
//! système (taille, style, police, couleur, options).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFontDialog: public TComponent {
private:
protected:
  DWORD Flags;
  HWND Handle;
  int FMinFontSize;
  int FMaxFontSize;

  TFont *FFont;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFontDialog(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFontDialog(void);

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
  //! @brief Propriété Font
	//!
	//! Cette propriété permet d'initialiser et de lire la fonte choisie par
	//! l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontDialog, TCustomFont *, Font);

  //---------------------------------------------------------------------------
  //! @brief Propriété Device
	//!
	//! Cette propriété permet de lister les fontes en fonction d'un
	//! périphérique. Par défaut ce sont les fontes écran qui sont listées.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontDialog, TFontDialogDevice, Device);

  //---------------------------------------------------------------------------
  //! @brief Propriété Options
	//!
	//! Cette propriété permet de définir certaines options de la boîte de
	//! dialogue (paramètres présents ou pas).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontDialog, TFontDialogOptions, Options);

  //---------------------------------------------------------------------------
  //! @brief Propriété MinFontSize
	//!
	//! Cette propriété permet de fixer une taille minimum de la fonte.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontDialog, int, MinFontSize);


  //---------------------------------------------------------------------------
  //! @brief Propriété MaxFontSize
	//!
	//! Cette propriété permet de fixer une taille maximum de la fonte.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFontDialog, int, MaxFontSize);


  //@}

};


#else  // TFontDialogH

class TFontDialog;

#endif  // TFontDialogH


#undef In_TFontDialogH
