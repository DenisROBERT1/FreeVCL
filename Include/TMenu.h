//-----------------------------------------------------------------------------
//! @file TMenu.h
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

#ifdef In_TMenuH
#error "Inclusion circulaire de TMenu"
#endif // In_TMenuH

#define In_TMenuH

#ifndef TMenuH
#define TMenuH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH

#ifndef TMenuItemH
#include "TMenuItem.h"
#endif  // TMenuItemH


//-----------------------------------------------------------------------------
//! @class TMenu
//!
//! @brief Définition de la classe TMenu
//!
//! Cette classe est la classe de base des menus: menus popup et menus
//! principaux des applications, par exemple. N'implémentez pas cette classe
//! mais utilisez ses descendants @b TPopupMenu et @b TMainMenu.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMenu: public TMenuItem {
private:
protected:
	HWND FWindowHandle;

public:

	//! @name Constructeurs et destructeur
  //@{

	//---------------------------------------------------------------------------
	//! @brief Constructeur
	//---------------------------------------------------------------------------

	TMenu(TComponent* AOwner);

	//---------------------------------------------------------------------------
	//! @brief Destructeur
	//---------------------------------------------------------------------------

	virtual ~TMenu(void);

  //@}


	//! @name Méthodes
	//@{

	//---------------------------------------------------------------------------
	//! @brief Liste des propriétés publiées
	//!
	//! @param[in, out] 	ListProperties Liste des propriétés de l'objet
	//!
	//! @sa TPersistent::GetListProperties
	//---------------------------------------------------------------------------
	virtual void GetListProperties(TStrings *ListProperties);

	//---------------------------------------------------------------------------
	//! @brief Type des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété
	//! @param[out] 			asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
	//!
	//! @return @c TYPEPROPERTY Type de la propriété concernée.
	//!
	//! @sa TPersistent::GetTypeProperty
	//---------------------------------------------------------------------------

	virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

	//---------------------------------------------------------------------------
	//! @brief Valeur par défaut d'une propriété
	//!
	//! @param[in]				asProperty Nom de la propriété
	//!
	//! @return @c AnsiString Valeur par défaut de la propriété.
	//!
	//! @sa TPersistent::GetDefaultProperty
	//---------------------------------------------------------------------------

	virtual AnsiString GetDefaultProperty(AnsiString asProperty);

	//---------------------------------------------------------------------------
	//! @brief Lecture des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété à lire
	//!
	//! @return @c AnsiString Valeur de la propriété.
	//---------------------------------------------------------------------------

	virtual AnsiString GetProperty(AnsiString asProperty);

	//---------------------------------------------------------------------------
	//! @brief Affectation des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété à enregistrer
	//! @param[in]				asValue Valeur de la propriété à enregistrer
	//! @param[in]				Sender Boîte de dialogue (utilisé par les évènements)
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
													 TPersistent *Sender);

	//@}

	//! @name Propriétés
	//@{

	//---------------------------------------------------------------------------
	//! @brief Propriété WindowHandle: Handle de la fenêtre.
	//!
	//! Cette propriété permet de lire le Handle de la fenêtre à laquelle
	//! appartient l'objet TMenu.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMenu, HWND, WindowHandle);

	//@}

};


#else  // TMenuH

class TMenu;

#endif	// TMenuH


#undef In_TMenuH
