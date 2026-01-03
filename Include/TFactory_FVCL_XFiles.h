//-----------------------------------------------------------------------------
//! @file TFactory_FVCL_XFiles.h
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

#ifdef In_TFactory_FVCL_XFilesH
#error "Inclusion circulaire de TFactory_FVCL_XFiles"
#endif // In_TFactory_FVCL_XFilesH

#define In_TFactory_FVCL_XFilesH

#ifndef TFactory_FVCL_XFilesH
#define TFactory_FVCL_XFilesH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TFactory.h"


//---------------------------------------------------------------------------
//
//                         class TFactory_FVCL_XFiles
//
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! @class TFactory_FVCL_XFiles
//!
//! @brief Création d'un objet TXFiles
//!
//! Cette classe (@c TFactory_FVCL_XFiles) permet de gérer divers types de
//! fichiers: format PDF, DBF, ...
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFactory_FVCL_XFiles: public TFactory {
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

  TFactory_FVCL_XFiles(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFactory_FVCL_XFiles(void);

  //@}


  //! @name Méthodes
  //@{

  //@}

};


#else  // TFactory_FVCL_XFilesH

class TFactory_FVCL_XFiles;

#endif  // TFactory_FVCL_XFilesH


#undef In_TFactory_FVCL_XFilesH
