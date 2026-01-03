//-----------------------------------------------------------------------------
//! @file TClientSocket.cpp
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
#include <FreeVcl_Web.h>


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TClientSocket::TClientSocket(TComponent* AOwner):
    TCustomSocket(AOwner) {
  // Initialisations
  FClassName = _T("TClientSocket");
  FClientType = ctBlocking;
	Socket = INVALID_SOCKET;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TClientSocket::~TClientSocket() {
}


//---------------------------------------------------------------------------
// Activation de la connection
//---------------------------------------------------------------------------

bool TClientSocket::Activate(void) {
  struct sockaddr_in SockAddr4;
  struct sockaddr_in6 SockAddr6;
  unsigned int r;
  int LastError;
	bool bIPv6 = false;


  if (!FHost.IsEmpty()) {
		// Ancienne version (compatible IPv4 uniquement)
	  LPHOSTENT lpHostEnt;
    lpHostEnt = gethostbyname(CharString(FHost));
    if (lpHostEnt == NULL) {
      LastError = WSAGetLastError();
      OnError(this, this, eeGeneral, LastError);
      if (LastError) {
        DisplayError(_T("TClientSocket"), _T("gethostbyname"), LastError);
      }
      return false;
    }

    SockAddr4.sin_addr.S_un.S_addr = * ((u_long*) lpHostEnt->h_addr_list[0]);

		/*
		// Nouvelle version (compatible IPv6)
    ADDRINFOT Hints;
    PADDRINFOT pResult;

		memset(&Hints, 0, sizeof(Hints));
		Hints.ai_family = AF_INET; // AF_UNSPEC;
		LastError = GetAddrInfo(FHost, NULL, &Hints, &pResult);
    if (LastError) {
      DisplayError(_T("TClientSocket"), _T("GetAddrInfo"), LastError);
	    return false;
    }
		if (pResult->ai_addrlen == sizeof(SockAddr4)) {
			SockAddr4 = * ((sockaddr_in *) pResult->ai_addr);
			bIPv6 = false;
		}
		else if (pResult->ai_addrlen == sizeof(SockAddr6)) {
			SockAddr6 = * ((sockaddr_in6 *) pResult->ai_addr);
			bIPv6 = true;
		}
		else {
      DisplayError(_T("TClientSocket"), _T("GetAddrInfo"), WSAVERNOTSUPPORTED);
	    return false;
		}
		*/
  }
  else {
		// Ancienne version (compatible IPv4 uniquement)
    SockAddr4.sin_addr.S_un.S_addr = inet_addr(CharString(FAddress));

		/*
		// Nouvelle version (compatible IPv6)
		if (FAddress.Pos(".") != 0) {
			memset(&SockAddr4, 0, sizeof(SockAddr4));
			InetPton(AF_INET, (const PTSTR) FAddress, &SockAddr4.sin_addr);
			bIPv6 = false;
		}
		else {
			memset(&SockAddr6, 0, sizeof(SockAddr6));
			InetPton(AF_INET6, (const PTSTR) FAddress, &SockAddr6.sin6_addr);
			bIPv6 = true;
		}
		*/
  }

	if (!bIPv6) {
	  Socket = socket(AF_INET, SOCK_STREAM, 0);
	}
	else {
	  Socket = socket(AF_INET6, SOCK_STREAM, 0);
	}
  if (Socket == INVALID_SOCKET) {
    LastError = WSAGetLastError();
    OnError(this, this, eeGeneral, LastError);
    if (LastError) {
      DisplayError(_T("TClientSocket"), _T("socket"), LastError);
    }
    return false;
  }

  WSAAsyncSelect(Socket, hWndSocket, UM_MESSAGE, FD_CONNECT | FD_CLOSE | FD_READ);

	if (!bIPv6) {
		SockAddr4.sin_family = AF_INET;
		SockAddr4.sin_port = htons((u_short) FPort);
		r = connect(Socket, (LPSOCKADDR) &SockAddr4, sizeof(SockAddr4));
	}
	else {
		SockAddr6.sin6_family = AF_INET6;
		SockAddr6.sin6_port = htons((u_short) FPort);
		r = connect(Socket, (LPSOCKADDR) &SockAddr6, sizeof(SockAddr6));
	}
  if (r == INVALID_SOCKET) {
    LastError = WSAGetLastError();
    if (LastError == WSAEWOULDBLOCK) {
      OnConnecting(this, this);
	    return true;
    }
    else {
      OnError(this, this, eeConnect, LastError);
      if (LastError) {
        DisplayError(_T("TClientSocket"), _T("connect"), LastError);
      }
			closesocket(Socket);
			Socket = INVALID_SOCKET;
	    return false;
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Desactivation de la connection
//---------------------------------------------------------------------------

bool TClientSocket::Desactivate(void) {

  if (Socket != INVALID_SOCKET) {
    shutdown(Socket, SD_SEND);
    closesocket(Socket);
    OnDisconnect(this, this);
    Socket = INVALID_SOCKET;
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientType
//---------------------------------------------------------------------------

TClientType TClientSocket::Get_ClientType() {
  return FClientType;
}

bool TClientSocket::Set_ClientType(TClientType NewClientType) {
  if (FClientType != NewClientType) {
    FClientType = NewClientType;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Address
//---------------------------------------------------------------------------

AnsiString TClientSocket::Get_Address() {
  return FAddress;
}

bool TClientSocket::Set_Address(AnsiString NewAddress) {
  if (FAddress != NewAddress) {
    FAddress = NewAddress;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Host
//---------------------------------------------------------------------------

AnsiString TClientSocket::Get_Host() {
  return FHost;
}

bool TClientSocket::Set_Host(AnsiString NewHost) {
  if (FHost != NewHost) {
    FHost = NewHost;
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TClientSocket::GetListProperties(TStrings *ListProperties) {

  TCustomSocket::GetListProperties(ListProperties);

  ListProperties->Add("ClientType");
  ListProperties->Add("Address");
  ListProperties->Add("Host");
  ListProperties->Add("OnConnecting");
  ListProperties->Add("OnConnect");
  ListProperties->Add("OnDisconnect");
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TClientSocket::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("ClientType")) {
    *asInfos = _T("ctNonBlocking\nctBlocking");
    return tpChoice;
  }
  if (asProperty == _T("Address")) {
    return tpText;
  }
  if (asProperty == _T("Host")) {
    return tpText;
  }
  if (asProperty == _T("OnConnecting")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnConnect")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnDisconnect")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TCustomSocket::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TClientSocket::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("ClientType")) {
    return _T("ctBlocking");
  }
  if (asProperty == _T("Address")) {
    return _T("");
  }
  if (asProperty == _T("Host")) {
    return _T("");
  }
  if (asProperty == _T("OnConnecting")) {
    return _T("");
  }
  if (asProperty == _T("OnConnect")) {
    return _T("");
  }
  if (asProperty == _T("OnDisconnect")) {
    return _T("");
  }
  return TCustomSocket::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TClientSocket::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("ClientType")) {
    TClientType ClientType = Get_ClientType();
    if (ClientType == ctNonBlocking) return _T("ctNonBlocking");
    if (ClientType == ctBlocking) return _T("ctBlocking");
    return _T("ctNonBlocking");
  }
  if (asProperty == _T("Address")) {
    return Get_Address();
  }
  if (asProperty == _T("Host")) {
    return Get_Host();
  }
  if (asProperty == _T("OnConnecting")) {
    return OnConnecting.AsString();
  }
  if (asProperty == _T("OnConnect")) {
    return OnConnect.AsString();
  }
  if (asProperty == _T("OnDisconnect")) {
    return OnDisconnect.AsString();
  }
  return TCustomSocket::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TClientSocket::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("ClientType")) {
    if (asValue == _T("ctNonBlocking")) FClientType = ctNonBlocking;
    else if (asValue == _T("ctBlocking")) FClientType = ctBlocking;
    return true;
  }
  if (asProperty == _T("Address")) {
    Set_Address(asValue);
  }
  if (asProperty == _T("Host")) {
    Set_Host(asValue);
  }
  if (asProperty == _T("OnConnecting")) {
		OnConnecting.ObjCall = Sender;
    OnConnecting = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnConnect")) {
		OnConnect.ObjCall = Sender;
    OnConnect = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnDisconnect")) {
		OnDisconnect.ObjCall = Sender;
    OnDisconnect = asValue.c_str();
    return true;
  }
  return TCustomSocket::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
