//-----------------------------------------------------------------------------
//! @file TForm_Tools.h
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

#ifndef TForm_ToolsH
#define TForm_ToolsH
//---------------------------------------------------------------------------
#include <vector>

//---------------------------------------------------------------------------
class TForm_Tools : public TForm {
private:
  void MAJListBox(void);

public:
  FVCL_BEGIN_COMPONENTS
  TListBox *ListBox_Tools;
  TButton *Button_Nouveau;
  TButton *Button_Modif;
  TButton *Button_Suppr;
  TButton *Button_Sortie;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_NouveauClick(TObject *Sender);
  void FASTCALL Button_ModifClick(TObject *Sender);
  void FASTCALL Button_SupprClick(TObject *Sender);
  void FASTCALL Button_SortieClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TForm_Tools, TNotifyEvent, OnClick, Button_NouveauClick, TObject*);
		SET_EVENT_STR_1(TForm_Tools, TNotifyEvent, OnClick, Button_ModifClick, TObject*);
		SET_EVENT_STR_1(TForm_Tools, TNotifyEvent, OnClick, Button_SupprClick, TObject*);
		SET_EVENT_STR_1(TForm_Tools, TNotifyEvent, OnClick, Button_SortieClick, TObject*);
		SET_EVENT_STR_1(TForm_Tools, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_Tools(TComponent *Owner, LPCTSTR szName);

};
//---------------------------------------------------------------------------
#endif
