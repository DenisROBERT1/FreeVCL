//-----------------------------------------------------------------------------
//! @file TForm_DisplayError.h
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

#ifndef TForm_DisplayErrorH
#define TForm_DisplayErrorH

//---------------------------------------------------------------------------
class TForm_DisplayError : public TForm {
protected:
  bool FNePlusAfficher;
public:
  FVCL_BEGIN_COMPONENTS
  TImage *Image;
  TLabel *Label_Message;
  TCheckBox *CheckBox_NePlusAfficher;
  TButton *Button_Oui;
  TButton *Button_Non;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
	void FASTCALL Button_OkClick(TObject *Sender);
	void FASTCALL Button_AnnulerClick(TObject *Sender);
	void FASTCALL Button_OuiClick(TObject *Sender);
	void FASTCALL Button_NonClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
	void FASTCALL FormClose(TObject *Sender, TCloseAction &Action);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_DisplayError, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_DisplayError, TNotifyEvent, OnClick, Button_AnnulerClick, TObject*);
    SET_EVENT_STR_1(TForm_DisplayError, TNotifyEvent, OnClick, Button_OuiClick, TObject*);
    SET_EVENT_STR_1(TForm_DisplayError, TNotifyEvent, OnClick, Button_NonClick, TObject*);
    SET_EVENT_STR_1(TForm_DisplayError, TNotifyEvent, OnClick, Button_AideClick, TObject*);
    SET_EVENT_STR_2(TForm_DisplayError, TOnClose, OnClose, FormClose, TObject*, TCloseAction &);
  }

  FVCL_END_EVENTS

  TForm_DisplayError(HWND hWndParent, LPCTSTR szName);

	// Positionnement des boutons visibles
	void ReArrangeBoutons(void);

  //---------------------------------------------------------------------------
  //! @brief Propriété NePlusAfficher
	//!
	//! Cette propriété indique l'état de la case à cocher "Ne plus afficher"
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TForm_DisplayError, bool, NePlusAfficher);


};

int DisplayError(const AnsiString &asMessage, const AnsiString &asTitle, const AnsiString &asKey, int Flags);
int DisplaySystemError(const AnsiString &asMessage, int SystemError, const AnsiString &asTitle, const AnsiString &asKey, int Flags);
int DisplaySystemErrorArgs(const AnsiString &asMessage, int SystemError, const AnsiString &asTitle, const AnsiString &asKey, int Flags, ...);

//---------------------------------------------------------------------------
#endif
