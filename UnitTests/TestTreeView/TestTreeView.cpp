//-----------------------------------------------------------------------------
//! @file TestTreeView.cpp
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

#include <FreeVCL.h>
#include "Unit1.h"

//---------------------------------------------------------------------------
USEFORM(_T("Unit1.cpp"), Unit1);
//---------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HInstance = hInstance;
	TUnit1 *Unit1;

    try
    {
       Application->Initialize();
       Application->CreateForm(__classid(TUnit1), &Unit1);
			 Application->Run();
    }
    catch (Exception &exception)
    {
       Application->ShowException(&exception);
    }
    catch (...)
    {
       try
       {
           throw Exception(_T(""));
       }
       catch (Exception &exception)
       {
           Application->ShowException(&exception);
       }
    }
    return 0;
}
//---------------------------------------------------------------------------
