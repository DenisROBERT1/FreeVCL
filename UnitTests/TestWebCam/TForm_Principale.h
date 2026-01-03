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
#include <FreeVcl_Multimed.h>

//---------------------------------------------------------------------------
class TForm_Principale : public TForm {
private:
  bool bCapture;
public:
  FVCL_BEGIN_COMPONENTS
  TWebCam *WebCam;
  TButton *Button_Param;
  TButton *Button_Capture;
  TSaveDialog *SaveDialog;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_ParamClick(TObject *Sender);
  void FASTCALL Button_CaptureClick(TObject *Sender);
  void FASTCALL WebCamFrame(TObject *Sender, TBitmap *Frame);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_ParamClick, TObject*);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_CaptureClick, TObject*);
    SET_EVENT_STR_2(TForm_Principale, TOnFrame, OnFrame, WebCamFrame, TObject*, TBitmap*);
  }
  FVCL_END_EVENTS

  TForm_Principale(HWND hWndParent, LPCTSTR szName);
  virtual ~TForm_Principale(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Principale *Form_Principale;
//---------------------------------------------------------------------------
#endif
