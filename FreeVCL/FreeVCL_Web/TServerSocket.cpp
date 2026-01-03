//-----------------------------------------------------------------------------
//! @file TServerSocket.cpp
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

TServerSocket::TServerSocket(TComponent* AOwner):
    TCustomSocket(AOwner) {
  // Initialisations
  FClassName = _T("TServerSocket");
  FServerType = stThreadBlocking;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TServerSocket::~TServerSocket() {
}

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
ULONG WINAPI TServerSocket::SockAsyncThread(PVOID ThreadParam)
{
	TServerSocket *ServerSocket;
  struct sockaddr_in SockAddr;
  SOCKET ahSocket[3];
  WSAEVENT ahEvents[2];
	bool bExit = false;

	ServerSocket = static_cast<TServerSocket *>(ThreadParam);
	// 3 evenements pour les 3 socket
  ahEvents[0] = WSACreateEvent();
  ahEvents[1] = WSACreateEvent();
  WSAEventSelect(ServerSocket->Socket, ahEvents[0], FD_ACCEPT | FD_CLOSE | FD_READ );
  // Premiere socket qui ecoute
	ahSocket[0] = ServerSocket->Socket;
  // On boucle 
  while (!bExit) 
  {
		DWORD dwEvent;
		WSANETWORKEVENTS NetworkEvents;
	
		// On attend que quelquechose arrive.
		// Typiquement, on va commencer par recevoir la connexion
		// puis ensuite, on sera notifie pour lire les donnees
		dwEvent = WSAWaitForMultipleEvents( 2, ahEvents, FALSE, WSA_INFINITE, FALSE);
		switch (dwEvent) 
		{ 
		case WSA_WAIT_FAILED: 
				printf("WSAEventSelect: %d\n", WSAGetLastError()); 
				break; 
		case WAIT_IO_COMPLETION: 
		case WSA_WAIT_TIMEOUT: 
				break; 
	    
		default: 
	    // On deduit la socket du numero d'evenement
	    dwEvent -= WSA_WAIT_EVENT_0; 
	    
			// On recherche 
			if (SOCKET_ERROR == WSAEnumNetworkEvents(ahSocket[dwEvent], ahEvents[dwEvent], &NetworkEvents)) 
			{
				TCHAR szBuf[256];
				wsprintf(szBuf, _T("WSAEnumNetworkEvent: %d lNetworkEvent %X\n"),  
						WSAGetLastError(), NetworkEvents.lNetworkEvents);
				NetworkEvents.lNetworkEvents = 0; 
			} 
			else  
			{ 
		
				if (FD_ACCEPT & NetworkEvents.lNetworkEvents) 
				{
					memset(&SockAddr, 0, sizeof(SockAddr));
					SockAddr.sin_family = AF_INET;
					SockAddr.sin_port = htons((u_short) ServerSocket->FPort);
					SockAddr.sin_addr.s_addr = INADDR_ANY;
					int Size = sizeof(SockAddr);
					ServerSocket->SocketDistant = accept(ServerSocket->Socket, (LPSOCKADDR) &SockAddr, &Size);
          if (ServerSocket->SocketDistant != INVALID_SOCKET)
					{
						ahSocket[1] = ServerSocket->SocketDistant;
						ServerSocket->OnAccept(ServerSocket, ServerSocket);
					}
				  WSAEventSelect(ServerSocket->SocketDistant, ahEvents[1], FD_CLOSE | FD_READ );
				}
				if (FD_READ & NetworkEvents.lNetworkEvents) 
				{
          ServerSocket->OnRead(ServerSocket, ServerSocket);
				  WSAEventSelect(ServerSocket->SocketDistant, ahEvents[1], FD_CLOSE | FD_READ );
				}
				if (FD_CLOSE & NetworkEvents.lNetworkEvents) 
				{
          ServerSocket->OnDisconnect(ServerSocket, ServerSocket);
					closesocket(ServerSocket->SocketDistant);
					ServerSocket->SocketDistant = INVALID_SOCKET;
					bExit = true;
				}
			}
		}
	}
	WSACloseEvent( ahEvents[0] );
	WSACloseEvent( ahEvents[1] );

	return 0;
}
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//---------------------------------------------------------------------------
// Activation de la connection
//---------------------------------------------------------------------------

bool TServerSocket::Activate(void) {
  struct sockaddr_in SockAddr;
  unsigned int r;
  int LastError;


  Socket = socket(AF_INET, SOCK_STREAM, 0);
  if (Socket == INVALID_SOCKET) {
    LastError = WSAGetLastError();
    OnError(this, this, eeGeneral, LastError);
    if (LastError) {
      DisplayError(_T("TServerSocket"), _T("socket"), LastError);
    }
    return false;
  }

  memset(&SockAddr, 0, sizeof(SockAddr));
  SockAddr.sin_family = AF_INET;
  SockAddr.sin_port = htons((u_short) FPort);
  SockAddr.sin_addr.s_addr = INADDR_ANY;
  r = bind(Socket, (LPSOCKADDR) &SockAddr, sizeof(SockAddr));
  if (r == INVALID_SOCKET) {
    LastError = WSAGetLastError();
    OnError(this, this, eeGeneral, LastError);
    if (LastError) {
      DisplayError(_T("TServerSocket"), _T("bind"), LastError);
    }
    return false;
  }

  r = listen(Socket, 3);
  if (r == INVALID_SOCKET) {
    LastError = WSAGetLastError();
    OnError(this, this, eeGeneral, LastError);
    if (LastError) {
      DisplayError(_T("TServerSocket"), _T("listen"), LastError);
    }
    return false;
  }

  WSAAsyncSelect(Socket, hWndSocket, UM_MESSAGE, FD_ACCEPT | FD_CLOSE | FD_READ);
  /*
	// $$$$$$$$$$$$$$$$$$$$$$$
	// Create the Async Thread
	HANDLE hAsyncThread;
  DWORD AsyncThreadId;
  hAsyncThread = CreateThread(NULL,
                              0,
                              SockAsyncThread,
                              this,
                              0,
                              &AsyncThreadId);

  // Close the Handle
  CloseHandle(hAsyncThread);
	// $$$$$$$$$$$$$$$$$$$$$$$
	//*/

  return true;
}

//---------------------------------------------------------------------------
// Desactivation de la connection
//---------------------------------------------------------------------------

bool TServerSocket::Desactivate(void) {

  if (Socket != INVALID_SOCKET) {
    shutdown(Socket, SD_SEND);
    closesocket(Socket);
    OnDisconnect(this, this);
    Socket = INVALID_SOCKET;
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ServerType
//---------------------------------------------------------------------------

TServerType TServerSocket::Get_ServerType() {
  return FServerType;
}

bool TServerSocket::Set_ServerType(TServerType NewServerType) {
  if (FServerType != NewServerType) {
    FServerType = NewServerType;
  }

  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TServerSocket::GetListProperties(TStrings *ListProperties) {

  TCustomSocket::GetListProperties(ListProperties);

  ListProperties->Add("ServerType");
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TServerSocket::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("ServerType")) {
    *asInfos = _T("stNonBlocking\nstThreadBlocking");
    return tpChoice;
  }
  return TCustomSocket::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TServerSocket::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("ServerType")) {
    return _T("stThreadBlocking");
  }
  return TCustomSocket::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TServerSocket::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("ServerType")) {
    TServerType ServerType = Get_ServerType();
    if (ServerType == stNonBlocking) return _T("stNonBlocking");
    if (ServerType == stThreadBlocking) return _T("stThreadBlocking");
    return _T("stNonBlocking");
  }
  return TCustomSocket::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TServerSocket::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("ServerType")) {
    if (asValue == _T("stNonBlocking")) FServerType = stNonBlocking;
    else if (asValue == _T("stThreadBlocking")) FServerType = stThreadBlocking;
    return true;
  }
  return TCustomSocket::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
