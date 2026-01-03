//-----------------------------------------------------------------------------
//! @file FreeVcl_Web.h
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

#ifdef In_FreeVcl_WebH
#error "Inclusion circulaire de FreeVcl_Web"
#endif // In_FreeVcl_WebH

#define In_FreeVcl_WebH

#ifndef FreeVcl_WebH
#define FreeVcl_WebH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

// Version minimum de windows = VISTA (pour les adresses IP v6) mais ça marche quand même sur XP sp3
#if(_WIN32_WINNT < 0x0600)
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif // _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif // _WIN32_WINNT < 0x0600


#include <string>
#include <vector>
#include <ws2tcpip.h>
#include <winsock2.h>
// #include <wspiapi.h>
#include "Property.h"
#include "FVCLBase.h"
#include "TFactory_FVCL_Web.h"
#include "TComPort.h"
#include "TCustomSocket.h"
#include "TClientSocket.h"
#include "TServerSocket.h"
#include "TCppWebBrowser.h"
#include "TDdeClientConv.h"
#include "TDdeClientItem.h"
#include "TDdeServerConv.h"
#include "TDdeServerItem.h"


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL_Web Factory_FVCL_Web;


// Provoquer le link avec wsock32.lib
#pragma comment(lib, "wsock32.lib")


//---------------------------------------------------------------------------
#endif

#undef In_FreeVcl_WebH
