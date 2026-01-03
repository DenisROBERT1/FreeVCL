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
public:
	FVCL_BEGIN_COMPONENTS
	// FVCL automatic code ! Don't modify this section.
	TMenuItem *MenuItem_Quitter;
	TMenuItem *MenuItem_Fichiers;
	TMainMenu *MainMenu;
	TLabel *Label_Hello;
	TButton *Button_Ok;
	FVCL_END_COMPONENTS

	FVCL_BEGIN_EVENTS
	// FVCL automatic code ! Don't modify this section.
	void FASTCALL MenuItem_QuitterClick(TObject *Sender);
	void FASTCALL Button_OkClick(TObject *Sender);

	virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, MenuItem_QuitterClick, TObject*);
		SET_EVENT_STR_1(TForm_Principale, TNotifyEvent, OnClick, Button_OkClick, TObject*);
	}

	FVCL_END_EVENTS

	TForm_Principale(HWND hWndParent, LPCTSTR szName);
	virtual ~TForm_Principale(void);
};
//---------------------------------------------------------------------------

#endif
