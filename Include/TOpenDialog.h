//-----------------------------------------------------------------------------
//! @file TOpenDialog.h
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

#ifdef In_TOpenDialogH
#error "Inclusion circulaire de TOpenDialog"
#endif // In_TOpenDialogH

#define In_TOpenDialogH

#ifndef TOpenDialogH
#define TOpenDialogH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TOpenSaveDialog.h"


//-----------------------------------------------------------------------------
//! @class TOpenDialog
//!
//! @brief Définition de la classe TOpenDialog
//! @image html TOpenDialog.jpg "TOpenDialog"
//!
//! Cette classe est un contrôle boîte de dialogue permettant de saisir un nom
//! de fichier sur le disque ou le réseau. La plupart de ses propriétés et
//! méthodes sont héritées de la classe de base @b TOpenSaveDialog.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TOpenDialog: public TOpenSaveDialog {
private:
protected:
  TStrings *FFiles;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TOpenDialog(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TOpenDialog(void);

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
  //! @brief Ouverture fichier
  //---------------------------------------------------------------------------

  virtual bool Execute(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Files
	//!
	//! Cette propriété permet de lire la liste des fichiers qu'a sélectionné
	//! l'utilisateur en retour du dialogue. Si la propriété @b Options ne
	//! contient pas l'option @b ofAllowMultiSelect, cette liste ne contiendra
	//! qu'un seul élément.
	//!
	//! @sa TOpenSaveDialog::Options
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TOpenDialog, TStrings *, Files);

	//@}

};


#else  // TOpenDialogH

class TOpenDialog;

#endif  // TOpenDialogH


#undef In_TOpenDialogH
