//-----------------------------------------------------------------------------
//! @file TForm_EdiOptions.h
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


#ifndef TForm_EdiOptionsH
#define TForm_EdiOptionsH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TFrame_EditFichierText;


//---------------------------------------------------------------------------
class TForm_EdiOptions : public TForm {
private:
protected:

public:

  FVCL_BEGIN_COMPONENTS  // Composants gérés par l'EDI
  TLabel *Label_NbSpacesTab;
  TEdit *Edit_NbSpacesTab;
  TUpDown *UpDown_NbSpacesTab;
  TCheckBox *CheckBox_SpacesToTabs;
  TCheckBox *CheckBox_TabsToSpaces;
  TCheckBox *CheckBox_LinesNumbers;
  TCheckBox *CheckBox_IconsColumn;
  TCheckBox *CheckBox_TypeAuto;
  TCheckBox *CheckBox_IndentAuto;
  TCheckBox *CheckBox_CurAfterEndLine;
  TCheckBox *CheckBox_UndoRedoMoveCur;
  TCheckBox *CheckBox_MultiLineTabs;
	TCheckBox *CheckBox_SaveBeforeBuild;
	TCheckBox *CheckBox_Autocompletion;
	TCheckBox *CheckBox_AlphaBlend;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
	FVCL_END_COMPONENTS  // Fin des composants gérés par l'EDI

  FVCL_BEGIN_EVENTS  // Evènements gérés par l'EDI
  void FASTCALL CheckBox_SpacesToTabsClick(TObject *Sender);
  void FASTCALL CheckBox_TabsToSpacesClick(TObject *Sender);
	void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_EdiOptions, TNotifyEvent, OnClick, CheckBox_SpacesToTabsClick, TObject*);
    SET_EVENT_STR_1(TForm_EdiOptions, TNotifyEvent, OnClick, CheckBox_TabsToSpacesClick, TObject*);
    SET_EVENT_STR_1(TForm_EdiOptions, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_EdiOptions, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS  // Fin des évènements gérés par l'EDI


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TForm_EdiOptions(TComponent* Owner, LPCTSTR szName);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TForm_EdiOptions(void);

  //@}


};

//---------------------------------------------------------------------------

#else  // TForm_EdiOptionsH

// Déclaration anticipée
class TForm_EdiOptions;

#endif  // TForm_EdiOptionsH

//---------------------------------------------------------------------------

