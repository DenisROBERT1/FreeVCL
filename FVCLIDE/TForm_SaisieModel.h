//-----------------------------------------------------------------------------
//! @file TForm_SaisieModel.h
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

#ifndef TForm_SaisieModelH
#define TForm_SaisieModelH

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TTextEditor;
class TFrame_EditFichierText;


//---------------------------------------------------------------------------
class TForm_SaisieModel : public TForm {
private:
protected:
  AnsiString FFileName;
  AnsiString FFilePath;
public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_Name;
  TEdit *Edit_Name;
  TLabel *Label_Langage;
  TComboBox *ComboBox_Langage;
  TLabel *Label_Model;
  TFrame_EditFichierText *Frame_EditFichierText;
  TPanel *Panel;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL ComboBox_LangageChange(TObject *Sender);
  void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_SaisieModel, TNotifyEvent, OnChange, ComboBox_LangageChange, TObject*);
    SET_EVENT_STR_1(TForm_SaisieModel, TNotifyEvent, OnClick, Button_OkClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieModel, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_SaisieModel(TComponent *Owner, LPCTSTR szName);
  ~TForm_SaisieModel(void);

  //! @name Méthodes
  //@{

  //@}


  //! @name Propriétés
  //@{

	//---------------------------------------------------------------------------
  //! @brief Propriété FileName
	//!
	//! Cette propriété permet de lire et de définir le nom du fichier modèle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_SaisieModel, AnsiString, FileName);


	//---------------------------------------------------------------------------
  //! @brief Propriété FilePath
	//!
	//! Cette propriété permet de lire et de définir le chemin du fichier modèle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_SaisieModel, AnsiString, FilePath);

  //@}

};

//---------------------------------------------------------------------------
#endif
