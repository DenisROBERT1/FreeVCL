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

#ifndef TForm_PrincipaleH
#define TForm_PrincipaleH

#include <FreeVcl.h>

//---------------------------------------------------------------------------
class TForm_Principale: public TForm {
private:
	AnsiString asFileName;
public:
	FVCL_BEGIN_COMPONENTS
	// FVCL automatic code ! Don't modify this section.
	TMenuItem *MenuItem_New;
	TMenuItem *MenuItem_Open;
	TMenuItem *MenuItem_Save;
	TMenuItem *MenuItem_SaveAs;
	TMenuItem *MenuItem_Separator1;
	TMenuItem *MenuItem_Print;
	TMenuItem *MenuItem_Separator2;
	TMenuItem *MenuItem_Quit;
	TMenuItem *MenuItem_Fichiers;
	TMenuItem *MenuItem_OnlineHelp;
	TMenuItem *MenuItem_About;
	TMenuItem *MenuItem_Help;
	TMainMenu *MainMenu;
	TStatusBar *StatusBar;
	TMemo *Memo_Text;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TPrintDialog *PrintDialog;
	FVCL_END_COMPONENTS

	FVCL_BEGIN_EVENTS
	// FVCL automatic code ! Don't modify this section.
	void MenuItem_NewClick(TObject *Sender);
	void MenuItem_OpenClick(TObject *Sender);
	void MenuItem_SaveClick(TObject *Sender);
	void MenuItem_SaveAsClick(TObject *Sender);
	void MenuItem_PrintClick(TObject *Sender);
	void MenuItem_QuitClick(TObject *Sender);
	void MenuItem_OnlineHelpClick(TObject *Sender);
	void MenuItem_AboutClick(TObject *Sender);
	void Memo_TextChange(TObject *Sender);

	virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_NewClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_OpenClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_SaveClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_SaveAsClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_PrintClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_QuitClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_OnlineHelpClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_AboutClick, TObject *);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnChange, Memo_TextChange, TObject *);
	}

	FVCL_END_EVENTS

	TForm_Principale(HWND hWndParent, LPCTSTR szName);
	virtual ~TForm_Principale(void);
};
//---------------------------------------------------------------------------

#endif
