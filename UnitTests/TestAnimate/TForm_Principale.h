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
  TMenuItem *MenuItem_Ouvrir;
  TMenuItem *MenuItem_Play;
  TMenuItem *MenuItem_Stop;
  TMenuItem *MenuItem_Separator1;
  TMenuItem *MenuItem_Quitter;
  TMenuItem *MenuItem_Fichiers;
  TMainMenu *MainMenu;
  TAnimate *Animate;
  TCheckBox *CheckBox_Loop;
  TOpenDialog *OpenDialog;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void MenuItem_OuvrirClick(TObject *Sender);
  void MenuItem_PlayClick(TObject *Sender);
  void MenuItem_StopClick(TObject *Sender);
  void MenuItem_QuitterClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_OuvrirClick, TObject *);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_PlayClick, TObject*);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_StopClick, TObject*);
    SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_QuitterClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_Principale(HWND hWndParent, LPCTSTR szName);
  virtual ~TForm_Principale(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Principale *Form_Principale;
//---------------------------------------------------------------------------
#endif
