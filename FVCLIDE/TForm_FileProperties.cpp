//-----------------------------------------------------------------------------
//! @file TForm_FileProperties.cpp
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

#include <FreeVcl.h>

#include "TConfig.h"
#include "TForm_FileProperties.h"
#include "HLP_FVCLIDE_Fr.h"

//---------------------------------------------------------------------------
TForm_FileProperties::TForm_FileProperties(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {

}

//---------------------------------------------------------------------------
TForm_FileProperties::~TForm_FileProperties(void) {
}

//---------------------------------------------------------------------------
void TForm_FileProperties::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	// $$$ Application->HelpCommand(HELP_CONTEXT, HLP_MODIFPROJECT);
}

//---------------------------------------------------------------------------
