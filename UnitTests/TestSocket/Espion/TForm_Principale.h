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
  TLabel *LabelStatusServer;
  TLabel *LabelStatusContentServer;
	TLabel *LabelHostServer;
	TEdit *EditHostServer;
	TLabel *LabelPortServer;
	TEdit *EditPortServer;
  TButton *Button_ConnecterServer;
	TGroupBox *GroupBoxParamServer;
  TLabel *LabelStatusClient;
  TLabel *LabelStatusContentClient;
	TLabel *LabelPortClient;
	TEdit *EditPortClient;
  TButton *Button_ActiverClient;
	TGroupBox *GroupBoxParamClient;
  TMemo *Memo;
  TClientSocket *ClientSocket;
  TServerSocket *ServerSocket;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_ActiverClientClick(TObject *Sender);
  void FASTCALL Button_ConnecterServerClick(TObject *Sender);
  void FASTCALL ServerSocketAccept(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ServerSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ServerSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ServerSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ClientSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ClientSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ClientSocketConnecting(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ClientSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL SocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_ActiverClientClick, TObject*);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_ConnecterServerClick, TObject*);
    SET_EVENT_STR_2(TForm_Principale, TOnAccept, OnAccept, ServerSocketAccept, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnDisconnect, OnDisconnect, ServerSocketDisconnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnConnect, OnConnect, ServerSocketConnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnRead, OnRead, ServerSocketRead, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnConnect, OnConnect, ClientSocketConnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnConnect, OnConnecting, ClientSocketConnecting, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnDisconnect, OnDisconnect, ClientSocketDisconnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnRead, OnRead, ClientSocketRead, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_4(TForm_Principale, TOnError, OnError, SocketError, TObject*, TCustomWinSocket *, TErrorEvent, int &);
  }

  FVCL_END_EVENTS

  TForm_Principale(HWND hWndParent, LPCTSTR szName);
  virtual ~TForm_Principale(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Principale *Form_Principale;
//---------------------------------------------------------------------------
#endif
