//-----------------------------------------------------------------------------
//! @file FVCLIDE.cpp
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
#include "TTypeProject.h"
#include "TForm_Principale.h"


//---------------------------------------------------------------------------
USEFORM(_T("TForm_Principale.cpp"), Form_Principale);

//---------------------------------------------------------------------------
// Fonction principale:
//---------------------------------------------------------------------------

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

#ifndef __GNUG__

#define NOEXCEPT
#define CRTDBG

#endif // __GNUG__

#ifdef CRTDBG
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	// tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
#endif


#ifndef NOEXCEPT
  try {
#endif // NOEXCEPT

    HInstance = hInstance;

    Application->Initialize();
    Application->CreateForm(__classid(TForm_Principale), &Form_Principale);
    Application->Run();

    delete Form_Principale;

#ifndef NOEXCEPT
  }
  catch (...) {

    MessageBox(NULL,
               Application->Translate(
               _T("FreeVCL EDI a rencontré un problème grave\n")
               _T("Nous vous prions de nous en excuser\n")
               _T("Pour nous aider à le résoudre, un rapport a été édité")),
               _TT("Erreur exception"),
               MB_OK | MB_ICONSTOP);

  }
#endif // NOEXCEPT

  return 0;
}
//---------------------------------------------------------------------------
