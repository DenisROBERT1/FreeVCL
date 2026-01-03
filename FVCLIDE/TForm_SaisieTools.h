//-----------------------------------------------------------------------------
//! @file TForm_SaisieTools.h
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

#ifndef TForm_SaisieToolsH
#define TForm_SaisieToolsH

//---------------------------------------------------------------------------
class TForm_SaisieTools : public TForm {
private:
  int FIdTool;
public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_Name;
  TEdit *Edit_Name;
  TLabel *Label_Prog;
  TEdit *Edit_Prog;
  TButton *Button_BrowseProg;
  TOpenDialog *OpenDialog_Prog;
  TLabel *Label_Args;
  TEdit *Edit_Args;
  TButton *Button_BrowseArgs;
  TOpenDialog *OpenDialog_Args;
  TLabel *Label_InitDir;
  TEdit *Edit_InitDir;
  TButton *Button_BrowseDir;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Edit_ProgExit(TObject *Sender);
  void FASTCALL Edit_ArgsExit(TObject *Sender);
  void FASTCALL Button_BrowseProgClick(TObject *Sender);
  void FASTCALL Button_BrowseArgsClick(TObject *Sender);
  void FASTCALL Button_BrowseDirClick(TObject *Sender);
  void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_SaisieTools, TOnExit, OnExit, Edit_ProgExit, TObject*);
    SET_EVENT_STR_1(TForm_SaisieTools, TOnExit, OnExit, Edit_ArgsExit, TObject*);
    SET_EVENT_STR_1(TForm_SaisieTools, TNotifyEvent, OnClick, Button_BrowseProgClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieTools, TNotifyEvent, OnClick, Button_BrowseArgsClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieTools, TNotifyEvent, OnClick, Button_BrowseDirClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieTools, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieTools, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_SaisieTools(TComponent *Owner, LPCTSTR szName);

  //! @name Méthodes
  //@{

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété IdTool
	//!
	//! Cette propriété permet de lire et de définir le numéro d'outil choisi
	//! par l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_SaisieTools, int, IdTool);

  //@}

};

//---------------------------------------------------------------------------
#endif
