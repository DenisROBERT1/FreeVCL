//-----------------------------------------------------------------------------
//! @file TForm_CreateProject.h
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

#ifndef TForm_CreateProjectH
#define TForm_CreateProjectH

//---------------------------------------------------------------------------
class TForm_CreateProject : public TForm {
private:
  int FIdTool;
public:
  FVCL_BEGIN_COMPONENTS
	// FVCL automatic code ! Don't modify this section.
	TLabel *Label_Name;
	TEdit *Edit_Name;
	TLabel *Label_Path;
	TEdit *Edit_Path;
	TButton *Button_ParcourirPath;
	TRadioButton *RadioButton_Modele;
	TLabel *Label_Modele;
	TComboBox *ComboBox_Modele;
	TRadioButton *RadioButton_AppliVCL;
	TLabel *Label_PathAppliVCL;
	TEdit *Edit_PathAppliVCL;
	TButton *Button_ParcourirPathAppliVCL;
	TGroupBox *GroupBox_Creation;
	TButton *Button_Create;
	TButton *Button_Annuler;
	TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
	// FVCL automatic code ! Don't modify this section.
	void Button_ParcourirPathClick(TObject *Sender);
	void RadioButton_ModeleClick(TObject *Sender);
	void RadioButton_AppliVCLClick(TObject *Sender);
	void TButton_ParcourirPathAppliVCLClick(TObject *Sender);
	void Button_AideClick(TObject *Sender);

	virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TForm_CreateProject, TNotifyEvent, OnClick, Button_ParcourirPathClick, TObject *);
		SET_EVENT_STR_1(TForm_CreateProject, TNotifyEvent, OnClick, RadioButton_ModeleClick, TObject *);
		SET_EVENT_STR_1(TForm_CreateProject, TNotifyEvent, OnClick, RadioButton_AppliVCLClick, TObject *);
		SET_EVENT_STR_1(TForm_CreateProject, TNotifyEvent, OnClick, TButton_ParcourirPathAppliVCLClick, TObject *);
		SET_EVENT_STR_1(TForm_CreateProject, TNotifyEvent, OnClick, Button_AideClick, TObject *);
	}

  FVCL_END_EVENTS

  TForm_CreateProject(HWND hWnd, LPCTSTR szName);

  //! @name Méthodes
  //@{

  //@}


  //! @name Propriétés
  //@{

  //@}

};

//---------------------------------------------------------------------------
#endif
