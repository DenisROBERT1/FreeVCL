//-----------------------------------------------------------------------------
//! @file TGlobalConstr.cpp
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


//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------
#include <windows.h>
#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------
#include <windows.h>
#include <assert.h>
#include "TGlobalConstr.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
bool TGlobalConstr::FAffAncrages;
ACTION TGlobalConstr::FAction;


//---------------------------------------------------------------------------
TGlobalConstr::TGlobalConstr(void) {

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété bAffAncrages
//---------------------------------------------------------------------------

bool TGlobalConstr::Get_AffAncrages(void) {
  return FAffAncrages;
}

bool TGlobalConstr::Set_AffAncrages(bool NewAffAncrages) {
  if (FAffAncrages != NewAffAncrages) {
    FAffAncrages = NewAffAncrages;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Action
//---------------------------------------------------------------------------

ACTION TGlobalConstr::Get_Action(void) {
  return FAction;
}

bool TGlobalConstr::Set_Action(ACTION NewAction) {
  if (FAction != NewAction) {
    FAction = NewAction;
  }
  return true;
}

//---------------------------------------------------------------------------
