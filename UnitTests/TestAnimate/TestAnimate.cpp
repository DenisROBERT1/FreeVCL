//-----------------------------------------------------------------------------
//! @file TestAnimate.cpp
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
#include "TForm_Principale.h"


//---------------------------------------------------------------------------
// Fonction principale:
//---------------------------------------------------------------------------

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  int Retour;
  TForm_Principale * Form_Principale;


  try {

    HInstance = hInstance;

    Form_Principale = new TForm_Principale(NULL, _T("TForm_Principale"));

    Retour = Application->Run();

    delete Form_Principale;

  }
  catch (...) {

    MessageBox(NULL,
               _T("\"TestAnimate\" a planté avec succès"),
               _T("Erreur exception"),
               MB_OK | MB_ICONSTOP);

    Retour = -1;

  }

  return Retour;
}
//---------------------------------------------------------------------------
