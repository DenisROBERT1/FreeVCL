//-----------------------------------------------------------------------------
//! @file TForm_NewDialog.h
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

#ifndef TForm_NewDialogH
#define TForm_NewDialogH

//---------------------------------------------------------------------------
typedef struct {
  int Left;
  int Top;
  int Width;
  int Height;
  int TabOrder;
  bool AnchorLeft;
  bool AnchorTop;
  bool AnchorRight;
  bool AnchorBottom;
  bool OnClick;
  AnsiString asClass;
  AnsiString asName;
  AnsiString asCaption;
} CONTROL;

//---------------------------------------------------------------------------
class TForm_NewDialog: public TForm {
protected:
	int Choice;
  TStringList *FDescription;
	HWND FHandleNewWindow;
  std::vector<CONTROL> Controls;
	bool bListBoxUpdated;
  int FormWidth, FormHeight;
  AnsiString asFormName;
  AnsiString asFormCaption;

	bool ConstructionApercu(void);
	bool DialogueStandard(bool bButtons);
  bool AnalyzeMemo(void);
  bool AnalyzeLine(AnsiString asLigne, TStrings *asMots);
public:
	FVCL_BEGIN_COMPONENTS
	TLabel *Label_Unite;
	TLabel *Label_T;
	TEdit *Edit_Unite;
	TLabel *Label_Commentaire;
	TLabel *Label_Caption;
	TEdit *Edit_Caption;
	TRadioButton *RadioButton_Empty;
	TRadioButton *RadioButton_OkCancelHelp;
	TRadioButton *RadioButton_Structure;
	TRadioButton *RadioButton_FromWindow;
	TGroupBox *GroupBox_Options;
	TLabel *Label_Struct;
	TMemo *Memo_Struct;
  TLabel *Label_Windows;
  TListBox *ListBox_Windows;
	TGroupBox *GroupBox_Informations;
	TButton *Button_Ok;
	TButton *Button_Apercu;
	TButton *Button_Annuler;
	TButton *Button_Aide;
	FVCL_END_COMPONENTS

	FVCL_BEGIN_EVENTS
	void FASTCALL RadioButton_EmptyClick(TObject *Sender);
	void FASTCALL RadioButton_OkCancelHelpClick(TObject *Sender);
	void FASTCALL RadioButton_StructureClick(TObject *Sender);
	void FASTCALL RadioButton_FromWindowClick(TObject *Sender);
	void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_ApercuClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_NewDialog, TNotifyEvent, OnClick, RadioButton_EmptyClick, TObject*);
    SET_EVENT_STR_1(TForm_NewDialog, TNotifyEvent, OnClick, RadioButton_OkCancelHelpClick, TObject*);
    SET_EVENT_STR_1(TForm_NewDialog, TNotifyEvent, OnClick, RadioButton_StructureClick, TObject*);
    SET_EVENT_STR_1(TForm_NewDialog, TNotifyEvent, OnClick, RadioButton_FromWindowClick, TObject*);
    SET_EVENT_STR_1(TForm_NewDialog, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_NewDialog, TNotifyEvent, OnClick, Button_ApercuClick, TObject*);
    SET_EVENT_STR_1(TForm_NewDialog, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

	FVCL_END_EVENTS

	TForm_NewDialog(TComponent *Owner, LPCTSTR szName);
	virtual ~TForm_NewDialog(void);

	static BOOL CALLBACK EnumWnd(HWND hWnd, LPARAM lParam);
	BOOL EnumWnd(HWND hWnd);

	//! @name Propriétés
	//@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Description
	//!
	//! Cette propriété décrit les contrôles de la nouvelle boîte de dialogue.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TForm_NewDialog, TStrings *, Description);

  //---------------------------------------------------------------------------
  //! @brief Propriété hWnd
	//!
	//! Cette propriété indique la fenêtre choisie (dans le cas où l'utilisateur
	//! a choisi "À partir d'une fenêtre existante").
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TForm_NewDialog, HWND, HandleNewWindow);

	//@}

};
//---------------------------------------------------------------------------
#endif
