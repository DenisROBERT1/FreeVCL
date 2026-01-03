//-----------------------------------------------------------------------------
//! @file TFactory_FVCL.h
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

#ifdef In_TFactory_FVCLH
#error "Inclusion circulaire de TFactory_FVCL"
#endif // In_TFactory_FVCLH

#define In_TFactory_FVCLH

#ifndef TFactory_FVCLH
#define TFactory_FVCLH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TFactory.h"


//-----------------------------------------------------------------------------
//! @class TFactory_FVCL
//!
//! @brief Définition de la classe TFactory_FVCL
//!
//! Cette classe permet de créer les objets FVCL de base (boutons, edit, ...)
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFactory_FVCL: public TFactory {
private:
protected:

  //! @name Méthodes protégées
  //@{

  //---------------------------------------------------------------------------
  //! @brief Liste des objets pouvant être créés
  //!
  //! @param[in, out]   ListObjects Liste des objets pouvant être créés
  //! @param[in, out]   ListTabs Onglet par défaut des objets (en création)
  //!
  //! @return @c bool true si Ok
  //!
  //! @sa TFactory::GetListObjectsVirt
  //---------------------------------------------------------------------------

  virtual bool GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs);

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //!
  //! @param[in]        AOwner Propriétaire du composant à créer
  //! @param[in]        asNom Nom du composant à créer
  //!
  //! @return @c TComponent * Composant créé, ou NULL si erreur
  //!
  //! @sa TFactory::CreateObjectVirt
  //---------------------------------------------------------------------------

  virtual TComponent *CreateObjectVirt(TComponent* AOwner, AnsiString asNom);

  //@}

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFactory_FVCL(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFactory_FVCL(void);

  //@}


  //! @name Méthodes
  //@{

  //@}

};


#else  // TFactory_FVCLH

class TFactory_FVCL;

#endif  // TFactory_FVCLH


#undef In_TFactory_FVCLH
