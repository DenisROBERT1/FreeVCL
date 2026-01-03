//-----------------------------------------------------------------------------
//! @file TForm_Principale.cpp
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

#include "TForm_Principale.h"

//---------------------------------------------------------------------------
TForm_Principale::TForm_Principale(HWND hWndParent, LPCTSTR szName)
  : TForm(hWndParent, szName, FVCL_ARG_SUPP) {
  Factory_FVCL_Web.Init();

	EditHostServer->Text = ClientSocket->Host;
	EditPortServer->Text = ClientSocket->Port;
	EditPortClient->Text = ServerSocket->Port;
}

//---------------------------------------------------------------------------
TForm_Principale::~TForm_Principale(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Button_ConnecterServerClick(TObject *Sender) {
	ClientSocket->Host = EditHostServer->Text;
	ClientSocket->Port = EditPortServer->Text.ToInt();
  if (((bool) ClientSocket->Active) == false) {
    ClientSocket->Active = true;
  }
  else {
    ClientSocket->Active = false;
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Button_ActiverClientClick(TObject *Sender) {
	ServerSocket->Port = EditPortClient->Text.ToInt();

  if (((bool) ServerSocket->Active) == false) {
    ServerSocket->Active = true;
    Button_ActiverClient->Caption = "Désactiver";
    LabelStatusContentClient->Caption = "Actif";
		EditPortClient->Enabled = false;
  }
  else {
    ServerSocket->Active = false;
    Button_ActiverClient->Caption = "Activer";
    LabelStatusContentClient->Caption = "Désactivé";
		EditPortClient->Enabled = true;
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketAccept(TObject *Sender,
      TCustomWinSocket *Socket) {
  LabelStatusContentClient->Caption = "Connexion acceptée";
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket) {
  LabelStatusContentClient->Caption = "Client déconnecté";
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket) {
  LabelStatusContentClient->Caption = "Client connecté";
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketRead(TObject *Sender,
      TCustomWinSocket *Socket) {
  char szBuf[1000];
	int NbBytes;


  memset(szBuf, 0, sizeof(szBuf));
  NbBytes = Socket->ReceiveBuf(szBuf, sizeof(szBuf));
  ClientSocket->SendBuf(szBuf, NbBytes);
  Memo->Lines->Add("------ Message client -------");
  Memo->Lines->Add(szBuf);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket) {
  SocketDistant = Socket;
  LabelStatusContentServer->Caption = "Actif";
  Button_ConnecterServer->Caption = "Désactiver";
	EditHostServer->Enabled = false;
	EditPortServer->Enabled = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket) {
  SocketDistant = NULL;
  LabelStatusContentServer->Caption = "Désactivé";
  Button_ConnecterServer->Caption = "Activer";
	EditHostServer->Enabled = true;
	EditPortServer->Enabled = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ClientSocketConnecting(TObject *Sender,
      TCustomWinSocket *Socket) {
  LabelStatusContentServer->Caption = "En attente de connexion";
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ClientSocketRead(TObject *Sender,
      TCustomWinSocket *Socket) {
  char szBuf[1000];
	int NbBytes;


  memset(szBuf, 0, sizeof(szBuf));
  NbBytes = Socket->ReceiveBuf(szBuf, sizeof(szBuf));
  ServerSocket->SendBuf(szBuf, NbBytes);
  Memo->Lines->Add("------ Message server -------");
  Memo->Lines->Add(szBuf);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::SocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode) {
  TCHAR szBuf[50];
  LPTSTR lp;


  switch (ErrorCode) {
    case WSAEINTR: lp = _T("Fonction interrompue"); break;
    case WSAEINVAL: lp = _T("Déjà connecté à une adresse"); break;
    case WSAEWOULDBLOCK: lp = _T("Socket temporairement indisponible"); break;
		case WSAEAFNOSUPPORT: lp = _T("Adresse incompatible avec le protocole"); break;
    case WSAEADDRNOTAVAIL: lp = _T("Adresse invalide"); break;
    case WSAECONNABORTED: lp = _T("Connexion interrompue"); break;
    case WSAENOTCONN: lp = _T("Non connecté"); break;
    case WSAETIMEDOUT: lp = _T("Délai d'attente dépassé"); break;
    case WSAECONNREFUSED: lp = _T("Connexion refusée"); break;
		case WSAEHOSTUNREACH: lp = _T("Serveur non trouvé"); break;
    case WSAHOST_NOT_FOUND: lp = _T("Host non trouvé"); break;
    case WSANO_DATA: lp = _T("Data non trouvé"); break;
    default: wsprintf(szBuf, _T("Erreur %i"), ErrorCode); lp = szBuf; break;
  }

  MessageBox(Handle, lp, _T("Erreur de connexion"), MB_OK | MB_ICONSTOP);
  ErrorCode = 0;
}
//---------------------------------------------------------------------------

