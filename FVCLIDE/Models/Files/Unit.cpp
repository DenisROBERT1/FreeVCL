//-----------------------------------------------------------------------------
//! @file Unit.cpp
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

#include "{{{FILE_NAME}}}.h"


/* Exemple d'appel (mode modal)
	{{{FILE_NAME}}} *My_{{{FILE_NAME}}};
	TModalResult Result;
	My_{{{FILE_NAME}}} = new {{{FILE_NAME}}}(this, _T("{{{FILE_NAME}}}"));
	Result = My_{{{FILE_NAME}}}->ShowModal();
	if (Result == mrOk) {
		// Traitement
	}
	delete My_{{{FILE_NAME}}};
*/

//---------------------------------------------------------------------------
{{{FILE_NAME}}}::{{{FILE_NAME}}}(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {
}

//---------------------------------------------------------------------------
{{{FILE_NAME}}}::~{{{FILE_NAME}}}(void) {
}

//---------------------------------------------------------------------------
void FASTCALL {{{FILE_NAME}}}::Button_OkClick(TObject *Sender) {
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------

