//-----------------------------------------------------------------------------
//! @file TForm_Principale.h
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

#ifndef Form_PrincipaleH
#define Form_PrincipaleH

#include <FreeVcl.h>
#include <FreeVcl_Web.h>

//---------------------------------------------------------------------------
class TForm_Principale : public TForm {
private:
  TCustomWinSocket *SocketDistant;
public:
  FVCL_BEGIN_COMPONENTS
  TLabel *LabelStatus;
  TLabel *LabelStatusContent;
	TLabel *LabelHost;
	TEdit *EditHost;
	TLabel *LabelPort;
	TEdit *EditPort;
  TButton *Button_Connecter;
	TGroupBox *GroupBoxParam;
  TEdit *EditText;
  TButton *Button_Envoyer;
  TClientSocket *ClientSocket;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_ConnecterClick(TObject *Sender);
  void FASTCALL Button_EnvoyerClick(TObject *Sender);
  void FASTCALL ClientSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ClientSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ClientSocketConnecting(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ClientSocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_ConnecterClick, TObject*);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_EnvoyerClick, TObject*);
    SET_EVENT_STR_2(TForm_Principale, TOnConnect, OnConnect, ClientSocketConnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnConnect, OnConnecting, ClientSocketConnecting, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnDisconnect, OnDisconnect, ClientSocketDisconnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_4(TForm_Principale, TOnError, OnError, ClientSocketError, TObject*, TCustomWinSocket *, TErrorEvent, int &);
  }

  FVCL_END_EVENTS

  TForm_Principale(HWND hWndParent, LPCTSTR szName);
  virtual ~TForm_Principale(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Principale *Form_Principale;
//---------------------------------------------------------------------------
#endif
