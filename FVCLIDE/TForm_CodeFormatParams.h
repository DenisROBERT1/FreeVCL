//-----------------------------------------------------------------------------
//! @file TForm_CodeFormatParams.h
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


#ifndef TForm_CodeFormatParamsH
#define TForm_CodeFormatParamsH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TFrame_EditFichierText;
class TDictionnary;


//---------------------------------------------------------------------------
class TForm_CodeFormatParams : public TForm {
private:

	TDictionnary *ListKeywords;

  void FASTCALL CodeFormat(void);
protected:

public:

  FVCL_BEGIN_COMPONENTS  // Composants gérés par l'EDI
	TCheckBox *CheckBox_EndLineAcc;
	TCheckBox *CheckBox_SpacesAfterPar;
	TLabel *Label_Apercu;
  TLabel *Label_Langage;
  TComboBox *ComboBox_Langage;
	TFrame_EditFichierText *Frame_EditFichierText;
	TTextEditor *TTextEditor_Apercu;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
	FVCL_END_COMPONENTS  // Fin des composants gérés par l'EDI

  FVCL_BEGIN_EVENTS  // Evènements gérés par l'EDI
  void FASTCALL CheckBox_EndLineAccClick(TObject *Sender);
  void FASTCALL CheckBox_SpacesAfterParClick(TObject *Sender);
  void FASTCALL ComboBox_LangageChange(TObject *Sender);
	void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_CodeFormatParams, TNotifyEvent, OnClick, CheckBox_EndLineAccClick, TObject*);
    SET_EVENT_STR_1(TForm_CodeFormatParams, TNotifyEvent, OnClick, CheckBox_SpacesAfterParClick, TObject*);
    SET_EVENT_STR_1(TForm_CodeFormatParams, TNotifyEvent, OnChange, ComboBox_LangageChange, TObject*);
    SET_EVENT_STR_1(TForm_CodeFormatParams, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_CodeFormatParams, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS  // Fin des évènements gérés par l'EDI


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TForm_CodeFormatParams(TComponent* Owner, LPCTSTR szName);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TForm_CodeFormatParams(void);

  //@}


};

//---------------------------------------------------------------------------

#else  // TForm_CodeFormatParamsH

// Déclaration anticipée
class TForm_CodeFormatParams;

#endif  // TForm_CodeFormatParamsH

//---------------------------------------------------------------------------

