//-----------------------------------------------------------------------------
//! @file TForm_SaisieColors.h
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

#ifndef TForm_SaisieColorsH
#define TForm_SaisieColorsH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <FreeVCL.h>
#include <vector>


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TFrame_EditFichierText;
class TDictionnary;


//---------------------------------------------------------------------------
class TForm_SaisieColors : public TForm {
private: // Déclarations de l'utilisateur

	TDictionnary *ListKeywords;

  bool bModifs;
  bool bNotChange;

  bool LectureFichier();
  bool EcritureFichier();

	COLORPARAM FColorParams[MAX_TYPESYNTAX + 1];  // tsNormal = 0 à tsIconsColumn

public:  // Déclarations de l'utilisateur
  FVCL_BEGIN_COMPONENTS
	TLabel *Label_Categories;
	TListBox *ListBox_Categories;
	TLabel *Label_ColorText;
	TColorBox *ColorBox_ColorText;
	TLabel *Label_ColorBack;
	TColorBox *ColorBox_ColorBack;
	TCheckBox *CheckBox_Bold;
	TCheckBox *CheckBox_Italic;
  TLabel *Label_Standard;
  TComboBox *ComboBox_Standard;
	TLabel *Label_Apercu;
  TLabel *Label_Langage;
  TComboBox *ComboBox_Langage;
	TFrame_EditFichierText *Frame_EditFichierText;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL ListBox_CategoriesChange(TObject *Sender);
  void FASTCALL ColorBox_ColorBackChange(TObject *Sender);
  void FASTCALL ColorBox_ColorTextChange(TObject *Sender);
  void FASTCALL CheckBox_BoldClick(TObject *Sender);
  void FASTCALL CheckBox_ItalicClick(TObject *Sender);
  void FASTCALL ComboBox_StandardChange(TObject *Sender);
  void FASTCALL ComboBox_LangageChange(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
  void FASTCALL FormClose(TObject *Sender, TCloseAction &Action);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnChange, ListBox_CategoriesChange, TObject*);
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnChange, ColorBox_ColorBackChange, TObject*);
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnChange, ColorBox_ColorTextChange, TObject*);
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnClick, CheckBox_BoldClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnClick, CheckBox_ItalicClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnChange, ComboBox_StandardChange, TObject*);
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnChange, ComboBox_LangageChange, TObject*);
    SET_EVENT_STR_1(TForm_SaisieColors, TNotifyEvent, OnClick, Button_AideClick, TObject*);
    SET_EVENT_STR_2(TForm_SaisieColors, TOnClose, OnClose, FormClose, TObject *, TCloseAction &);
  }

  FVCL_END_EVENTS

  TForm_SaisieColors(TComponent *Owner, LPCTSTR szName);
  virtual ~TForm_SaisieColors(void);

  //! @name Méthodes
  //@{

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ColorParams
	//!
	//! Cette propriété permet de lire les paramètres de couleur choisis par
	//! l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TForm_SaisieColors, COLORPARAM, ColorParams);

  //@}


  //! @name Evènements
  //@{

  //@}

};
//---------------------------------------------------------------------------
#endif
