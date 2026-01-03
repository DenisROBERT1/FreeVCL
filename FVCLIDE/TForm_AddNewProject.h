//-----------------------------------------------------------------------------
//! @file TForm_AddNewProject.h
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

#ifndef TForm_AddNewProjectH
#define TForm_AddNewProjectH
//---------------------------------------------------------------------------
#include <vector>

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TTypeProject;


//---------------------------------------------------------------------------
class TForm_AddNewProject : public TForm {
private:

	std::vector<TTypeProject *> ListProjects;
public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_Name;
  TEdit *Edit_Name;
	TLabel *Label_Types;
  TListBox *ListBox_Types;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AnnulerClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
	void FASTCALL ListBox_TypesDrawItem(TObject *Sender, int Index, const TRect &Rect, TOwnerDrawStates States);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_AddNewProject, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_AddNewProject, TNotifyEvent, OnClick, Button_AnnulerClick, TObject*);
    SET_EVENT_STR_1(TForm_AddNewProject, TNotifyEvent, OnClick, Button_AideClick, TObject*);
    SET_EVENT_STR_4(TForm_AddNewProject, TOnDrawItem, OnDrawItem, ListBox_TypesDrawItem, TObject*, int, const TRect &, TOwnerDrawStates);
  }

  FVCL_END_EVENTS

  TForm_AddNewProject(TComponent *Owner, LPCTSTR szName);
  ~TForm_AddNewProject(void);

  //---------------------------------------------------------------------------
  //! Propriété TypeProject: type de projet choisi
	//!
	//! Cette propriété indique le type du projet choisi dans la list-box.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TForm_AddNewProject, AnsiString, TypeProject);

};
//---------------------------------------------------------------------------
#endif
