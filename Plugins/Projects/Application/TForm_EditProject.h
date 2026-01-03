//-----------------------------------------------------------------------------
//! @file TForm_EditProject.h
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

#ifndef TForm_EditProjectH
#define TForm_EditProjectH

//---------------------------------------------------------------------------
class TForm_EditProject : public TForm {
private:
  std::vector<TYPECOMPILER> ListCompilers;
public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_Compilateur;
  TComboBox *ComboBox_Compilateur;
	TLabel *Label_Output; 
	TEdit *Edit_Output;
	TButton *Button_ParcourirOutput;
  TSaveDialog *SaveDialog_Output;
  TTabSheet *TabSheet_OptionsGenerales;
  TTabSheet *TabSheet_OptionsCompilation;
  TTreeView *TreeView_SourceFiles;
	TButton *Button_Ajouter;
	TButton *Button_Enlever;
	TOpenDialog *OpenDialog_AddFile;
  TTabSheet *TabSheet_Fichiers;
  TPageControl *PageControl;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_ParcourirOutputClick(TObject *Sender);
  void FASTCALL TreeView_SourceFilesChange(TObject *Sender);
  void FASTCALL Button_AjouterClick(TObject *Sender);
  void FASTCALL Button_EnleverClick(TObject *Sender);
  void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_EditProject, TNotifyEvent, OnClick, Button_ParcourirOutputClick, TObject*);
    SET_EVENT_STR_1(TForm_EditProject, TNotifyEvent, OnChange, TreeView_SourceFilesChange, TObject*);
    SET_EVENT_STR_1(TForm_EditProject, TNotifyEvent, OnClick, Button_AjouterClick, TObject*);
    SET_EVENT_STR_1(TForm_EditProject, TNotifyEvent, OnClick, Button_EnleverClick, TObject*);
		SET_EVENT_STR_1(TForm_EditProject, TNotifyEvent, OnClick, Button_OkClick, TObject*);
		SET_EVENT_STR_1(TForm_EditProject, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_EditProject(HWND hWnd, LPCTSTR szName);

  //! @name Méthodes
  //@{

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété TypeCompiler
	//!
	//! Cette propriété permet de sélectionner le type de compilateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, TYPECOMPILER, TypeCompiler);

  //@}

};

//---------------------------------------------------------------------------
#endif
