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

	EditPort->Text = ServerSocket->Port;
}

//---------------------------------------------------------------------------
TForm_Principale::~TForm_Principale(void) {
}

//---------------------------------------------------------------------------

void FASTCALL TForm_Principale::Button_ActiverClick(TObject *Sender) {
	ServerSocket->Port = EditPort->Text.ToInt();

	if (((bool) ServerSocket->Active) == false) {
    ServerSocket->Active = true;
    Button_Activer->Caption = "Désactiver";
    LabelStatusContent->Caption = "Actif";
		EditPort->Enabled = false;
  }
  else {
    ServerSocket->Active = false;
    Button_Activer->Caption = "Activer";
    LabelStatusContent->Caption = "Désactivé";
		EditPort->Enabled = true;
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Button_ReadClick(TObject *Sender) {
	ServerSocketClientRead(this, ServerSocket);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketAccept(TObject *Sender,
      TCustomWinSocket *Socket) {
  LabelStatusContent->Caption = "Connexion acceptée";
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket) {
  LabelStatusContent->Caption = "Déconnecté";
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketClientConnect(TObject *Sender,
      TCustomWinSocket *Socket) {
  LabelStatusContent->Caption = "Connecté";
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketClientRead(TObject *Sender,
      TCustomWinSocket *Socket) {
  char szBuf[1000];


  LabelStatusContent->Caption = "Lecture";
  memset(szBuf, 0, sizeof(szBuf));
  Socket->ReceiveBuf(szBuf, sizeof(szBuf));
  Memo->Lines->Add(szBuf);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::ServerSocketClientError(TObject *Sender,
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
