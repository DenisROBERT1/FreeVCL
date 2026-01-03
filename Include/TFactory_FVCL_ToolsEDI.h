//-----------------------------------------------------------------------------
//! @file TFactory_FVCL_ToolsEDI.h
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

#ifdef In_TFactory_FVCL_ToolsEDIH
#error "Inclusion circulaire de TFactory_FVCL_ToolsEDI"
#endif // In_TFactory_FVCL_ToolsEDIH

#define In_TFactory_FVCL_ToolsEDIH

#ifndef TFactory_FVCL_ToolsEDIH
#define TFactory_FVCL_ToolsEDIH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TFactory.h"


//---------------------------------------------------------------------------
//
//                         class TFactory_FVCL_ToolsEDI
//
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! @class TFactory_FVCL_ToolsEDI
//!
//! @brief Création d'un objet TToolsEDI
//!
//! Cette classe (@c TFactory_FVCL_ToolsEDI) permet de créer un objet internet.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFactory_FVCL_ToolsEDI: public TFactory {
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
  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFactory_FVCL_ToolsEDI(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFactory_FVCL_ToolsEDI(void);

  //! @name Méthodes
  //@{

  //@}

};


#else  // TFactory_FVCL_ToolsEDIH

class TFactory_FVCL_ToolsEDI;

#endif  // TFactory_FVCL_ToolsEDIH


#undef In_TFactory_FVCL_ToolsEDIH
