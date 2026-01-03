//-----------------------------------------------------------------------------
//! @file TFactory_FVCL_Database.h
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

#ifdef In_TFactory_FVCL_DatabaseH
#error "Inclusion circulaire de TFactory_FVCL_Database"
#endif // In_TFactory_FVCL_DatabaseH

#define In_TFactory_FVCL_DatabaseH

#ifndef TFactory_FVCL_DatabaseH
#define TFactory_FVCL_DatabaseH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TFactory.h"


//---------------------------------------------------------------------------
//
//                         class TFactory_FVCL_Database
//
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! @class TFactory_FVCL_Database
//!
//! @brief Création d'un objet TDatabase
//!
//! Cette classe (@c TFactory_FVCL_Database) permet de créer un objet bases
//! de données.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFactory_FVCL_Database: public TFactory {
private:
protected:

  //! @name Méthodes protégées
  //@{

  //---------------------------------------------------------------------------
  //! @brief Liste des objets pouvant être créés
  //---------------------------------------------------------------------------

  virtual bool GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs);

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //---------------------------------------------------------------------------

  virtual TComponent *CreateObjectVirt(TComponent* AOwner, AnsiString asNom);

  //@}

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFactory_FVCL_Database(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFactory_FVCL_Database(void);

  //@}


  //! @name Méthodes
  //@{

  //@}

};


#else  // TFactory_FVCL_DatabaseH

class TFactory_FVCL_Database;

#endif  // TFactory_FVCL_DatabaseH


#undef In_TFactory_FVCL_DatabaseH
