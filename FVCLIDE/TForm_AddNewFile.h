//-----------------------------------------------------------------------------
//! @file TForm_AddNewFile.h
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

#ifndef TForm_AddNewFileH
#define TForm_AddNewFileH
//---------------------------------------------------------------------------
#include <vector>

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TTypeProject;


//---------------------------------------------------------------------------
class TForm_AddNewFile : public TForm {
private:

public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_Name;
  TEdit *Edit_Name;
  TLabel *Label_Path;
  TEdit *Edit_Path;
  TButton *Button_ParcourirPath;
	TLabel *Label_Types;
  TListBox *ListBox_Types;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_ParcourirPathClick(TObject *Sender);
  void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AnnulerClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_AddNewFile, TNotifyEvent, OnClick, Button_ParcourirPathClick, TObject*);
    SET_EVENT_STR_1(TForm_AddNewFile, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_AddNewFile, TNotifyEvent, OnClick, Button_AnnulerClick, TObject*);
    SET_EVENT_STR_1(TForm_AddNewFile, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_AddNewFile(TComponent *Owner, LPCTSTR szName);
  ~TForm_AddNewFile(void);

};
//---------------------------------------------------------------------------
#endif
