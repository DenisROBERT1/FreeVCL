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
public:
  FVCL_BEGIN_COMPONENTS
  TLabel *LabelStatus;
  TLabel *LabelStatusContent;
	TLabel *LabelPort;
	TEdit *EditPort;
  TButton *Button_Activer;
  TButton *Button_Read;
	TGroupBox *GroupBoxParam;
  TMemo *Memo;
  TServerSocket *ServerSocket;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_ActiverClick(TObject *Sender);
  void FASTCALL Button_ReadClick(TObject *Sender);
  void FASTCALL ServerSocketAccept(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ServerSocketClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ServerSocketClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ServerSocketClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
  void FASTCALL ServerSocketClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_ActiverClick, TObject*);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_ReadClick, TObject*);
    SET_EVENT_STR_2(TForm_Principale, TOnAccept, OnAccept, ServerSocketAccept, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnDisconnect, OnDisconnect, ServerSocketClientDisconnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnConnect, OnConnect, ServerSocketClientConnect, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_2(TForm_Principale, TOnRead, OnRead, ServerSocketClientRead, TObject*, TCustomWinSocket *);
    SET_EVENT_STR_4(TForm_Principale, TOnError, OnError, ServerSocketClientError, TObject*, TCustomWinSocket *, TErrorEvent, int &);
  }

  FVCL_END_EVENTS

  TForm_Principale(HWND hWndParent, LPCTSTR szName);
  virtual ~TForm_Principale(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Principale *Form_Principale;
//---------------------------------------------------------------------------
#endif
