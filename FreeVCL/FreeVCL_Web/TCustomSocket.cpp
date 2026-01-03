//-----------------------------------------------------------------------------
//! @file TCustomSocket.cpp
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

TCustomSocket::TCustomSocket(TComponent* AOwner):
    TComponent(AOwner) {
  Port = 0;
  FActive = false;
  Socket = INVALID_SOCKET;
	SocketDistant = INVALID_SOCKET;

  if (WSAStartup(MAKEWORD(1, 1), &WSAData)) {
    MessageBox(NULL, _T("Impossible d'initialiser WSAStartup"),
							 _T("TCustomSocket"), MB_OK | MB_ICONSTOP);
    return;
  }

	WNDCLASS WC;
	if (!GetClassInfo(HInstance, _T("CLASSSOCKET"), &WC)) {
		WC.style = 0;
		WC.lpfnWndProc = WndSocketProc;
		WC.cbClsExtra = 0;
		WC.cbWndExtra = 0;
		WC.hInstance = HInstance;
		WC.hIcon = NULL;
		WC.hCursor = NULL;
		WC.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		WC.lpszMenuName = 0;
		WC.lpszClassName = _T("CLASSSOCKET");
		if (!RegisterClass(&WC)) {
			int LastError = GetLastError();
			OnError(this, this, eeGeneral, LastError);
			if (LastError) {
				DisplayError(_T("TCustomSocket"), _T("RegisterClass CLASSSOCKET"), LastError);
			}
			return;
		}
	}

  hWndSocket = CreateWindow(_T("CLASSSOCKET"),
                            _T("TCustomSocket"),
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                            NULL,
                            NULL, ::HInstance, this);
  if (!hWndSocket) {
    int LastError = GetLastError();
    OnError(this, this, eeGeneral, LastError);
    if (LastError) {
      DisplayError(_T("TCustomSocket"), _T("CreateWindow CLASSSOCKET"), LastError);
    }
    return;
  }

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TCustomSocket::~TCustomSocket() {
  if (Socket != INVALID_SOCKET) {
    closesocket(Socket);
    Socket = INVALID_SOCKET;
  }
  if (hWndSocket) DestroyWindow(hWndSocket);
  WSACleanup();
  UnregisterClass(_T("CLASSSOCKET"), HInstance);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Port
//---------------------------------------------------------------------------

int TCustomSocket::Get_Port(void) {
  return FPort;
}

bool TCustomSocket::Set_Port(int NewPort) {
  if (FPort != NewPort) {
    FPort = NewPort;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété bActive
//---------------------------------------------------------------------------

bool TCustomSocket::Get_Active(void) {
  return FActive;
}

bool TCustomSocket::Set_Active(bool NewActive) {
  if (!FActive && NewActive) {
    if (Activate()) FActive = true;
  }
  else if (FActive && !NewActive) {
    if (Desactivate()) FActive = false;
  }

  return true;
}

//---------------------------------------------------------------------------
int FASTCALL TCustomSocket::SendBuf(const void *Buf, int Count) {
	if (SocketDistant == INVALID_SOCKET) {
    int LastError = WSAENOTCONN;
    OnError(this, this, eeGeneral, LastError);
    if (LastError) {
      DisplayError(_T("TCustomSocket"), _T("SendBuf"), LastError);
    }
		return 0;
	}
  return send(SocketDistant, (const char *) Buf, Count, 0);
}

//---------------------------------------------------------------------------
int FASTCALL TCustomSocket::ReceiveBuf(void *Buf, int Count) {
	if (SocketDistant == INVALID_SOCKET) {
    int LastError = WSAENOTCONN;
    OnError(this, this, eeGeneral, LastError);
    if (LastError) {
      DisplayError(_T("TCustomSocket"), _T("ReceiveBuf"), LastError);
    }
		return 0;
	}
  return recv(SocketDistant, (char *) Buf, Count, 0);
}

//---------------------------------------------------------------------------
bool FASTCALL TCustomSocket::Open(void) {
	return Activate();
}

//---------------------------------------------------------------------------
bool FASTCALL TCustomSocket::Close(void) {
	return Desactivate();
}

//---------------------------------------------------------------------------
int TCustomSocket::InetPton(int Family, const PTSTR pszAddrString, PVOID pAddrBuf) {
	if (Family == AF_INET) {
		* ((ULONG *) pAddrBuf) = inet_addr(CharString(pszAddrString));
		return 1;
	}
	else if (Family == AF_INET6) {
		int i = 0;
		int j = 0;
		USHORT Value = 0;
		char c;
		BYTE b;
		in6_addr *pAddrBufV6 = reinterpret_cast<in6_addr *>(pAddrBuf);
		memset(pAddrBufV6, 0, sizeof(in6_addr));
		do {
			c = (char) pszAddrString[i++];
			if (c == ':' || c == '\0') {
#ifdef __GNUG__
				pAddrBufV6->_S6_un._S6_u8[j++] = Value;
#else
				pAddrBufV6->_S6_un.Word[j++] = Value;
#endif  // __GNUG__
				Value = 0;
			}
			else {
				if ('0' <= c && c <= '9') b = c - '0';
				else if ('A' <= c && c <= 'Z') b = c - 'A' + 10;
				else if ('a' <= c && c <= 'z') b = c - 'a' + 10;
				else b = 0;
				Value = (Value << 8) + b;
			}
		} while (c && j < 8);

		return 1;
	}

	return 0;
}

//---------------------------------------------------------------------------
void TCustomSocket::DisplayError(AnsiString asTitle, AnsiString asFunction, int LastError) {
  AnsiString asMessage;

  asMessage.sprintf(_T("Communication error in function %s (0x%08X)"),
                    (LPCTSTR) asFunction, LastError);
  Application->MessageBox(asMessage, asTitle, MB_OK | MB_ICONSTOP);
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomSocket::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add("Port");
  ListProperties->Add("Active");
  ListProperties->Add("OnError");
  ListProperties->Add("OnAccept");
  ListProperties->Add("OnConnecting");
  ListProperties->Add("OnConnect");
  ListProperties->Add("OnDisconnect");
  ListProperties->Add("OnRead");
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomSocket::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Port")) {
    return tpNumber;
  }
  if (asProperty == _T("Active")) {
    return tpBool;
  }
  if (asProperty == _T("OnError")) {
    *asInfos = _T("(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrNum)");
    return tpEvent;
  }
  if (asProperty == _T("OnAccept")) {
    *asInfos = _T("(TObject *Sender, TCustomWinSocket *Socket)");
    return tpEvent;
  }
  if (asProperty == _T("OnConnecting")) {
    *asInfos = _T("(TObject *Sender, TCustomWinSocket *Socket)");
    return tpEvent;
  }
  if (asProperty == _T("OnConnect")) {
    *asInfos = _T("(TObject *Sender, TCustomWinSocket *Socket)");
    return tpEvent;
  }
  if (asProperty == _T("OnDisconnect")) {
    *asInfos = _T("(TObject *Sender, TCustomWinSocket *Socket)");
    return tpEvent;
  }
  if (asProperty == _T("OnRead")) {
    *asInfos = _T("(TObject *Sender, TCustomWinSocket *Socket)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomSocket::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Port")) {
    return _T("0");
  }
  if (asProperty == _T("Active")) {
    return _T("False");
  }
  if (asProperty == _T("OnError")) {
    return _T("");
  }
  if (asProperty == _T("OnAccept")) {
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
  if (asProperty == _T("OnRead")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomSocket::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Port")) {
    return IntToStr(Get_Port());
  }
  if (asProperty == _T("Active")) {
    if (Get_Active()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("OnError")) {
    return OnError.AsString();
  }
  if (asProperty == _T("OnAccept")) {
    return OnAccept.AsString();
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
  if (asProperty == _T("OnRead")) {
    return OnRead.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomSocket::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Port")) {
    Set_Port(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Active")) {
    if (asValue == _T("True")) Set_Active(true);
    else Set_Active(false);
    return true;
  }
  if (asProperty == _T("OnError")) {
		OnError.ObjCall = Sender;
    OnError = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnAccept")) {
		OnAccept.ObjCall = Sender;
    OnAccept = asValue.c_str();
    return true;
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
  if (asProperty == _T("OnRead")) {
		OnRead.ObjCall = Sender;
    OnRead = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------
// FUNCTION: WndSocketProc (HWND, UINT, WPARAM, LPARAM)
//   Fenetre de gestion d'une socket
//---------------------------------------------------------------------------

LRESULT FAR PASCAL TCustomSocket::WndSocketProc(HWND Handle,
    UINT Message, WPARAM wParam, LPARAM lParam) {
  TCustomSocket *CustomSocket;
  struct sockaddr_in SockAddr;
  int Size;
  int LastError;


  CustomSocket = (TCustomSocket *) (UINT_PTR) GetWindowLongPtr(Handle, GWLP_USERDATA);
  if (Message == WM_CREATE && CustomSocket == NULL) {
    LPCREATESTRUCT lpCreatStruct = (LPCREATESTRUCT) lParam;
    CustomSocket = (TCustomSocket *) lpCreatStruct->lpCreateParams;
    SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG) (UINT_PTR) CustomSocket);
  }

  if (CustomSocket) {

    switch (Message) {
    case WM_CREATE:
      return TRUE;

    case UM_MESSAGE:
      switch (LOWORD(lParam)) {
      case FD_ACCEPT:
        LastError = WSAGETSELECTERROR(lParam);
        if (!LastError) {
          Size = sizeof(SockAddr);
          CustomSocket->SocketDistant = accept(CustomSocket->Socket, (LPSOCKADDR) &SockAddr, &Size);
          if (CustomSocket->SocketDistant != INVALID_SOCKET) {
            CustomSocket->OnAccept(CustomSocket, CustomSocket);
          }
        }
        else {
          CustomSocket->OnError(CustomSocket, CustomSocket, eeAccept, LastError);
          if (LastError) {
            CustomSocket->DisplayError(_T("TCustomSocket"), _T("accept"), LastError);
          }
        }
        break;
      case FD_CONNECT:
        LastError = WSAGETSELECTERROR(lParam);
        if (!LastError) {
          CustomSocket->SocketDistant = (SOCKET) wParam;
          CustomSocket->FActive = true;
          CustomSocket->OnConnect(CustomSocket, CustomSocket);
        }
        else {
          CustomSocket->OnError(CustomSocket, CustomSocket, eeConnect, LastError);
          if (LastError) {
            CustomSocket->DisplayError(_T("TCustomSocket"), _T("connect"), LastError);
          }
        }
        break;
      case FD_CLOSE:
        CustomSocket->SocketDistant = (SOCKET) wParam;
        CustomSocket->OnDisconnect(CustomSocket, CustomSocket);
				// Il semblerait qu'il faille fermer la socket (fuite mémoire avec le serveur Mylène)
				closesocket(CustomSocket->SocketDistant);
				CustomSocket->SocketDistant = INVALID_SOCKET;
        break;
      case FD_READ:
        LastError = WSAGETSELECTERROR(lParam);
        if (!LastError) {
          CustomSocket->SocketDistant = (SOCKET) wParam;
          CustomSocket->OnRead(CustomSocket, CustomSocket);
        }
        else {
          CustomSocket->OnError(CustomSocket, CustomSocket, eeReceive, LastError);
          if (LastError) {
            CustomSocket->DisplayError(_T("TCustomSocket"), _T("read"), LastError);
          }
        }
        break;
      }
      return TRUE;


    case WM_DESTROY:
      return 0;

    }

  }

  return DefWindowProc(Handle, Message, wParam, lParam);
}


//---------------------------------------------------------------------------
