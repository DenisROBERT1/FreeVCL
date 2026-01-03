//-----------------------------------------------------------------------------
//! @file TForm_ConvertNumbers.h
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

#ifndef TForm_ConvertNumbersH
#define TForm_ConvertNumbersH

//---------------------------------------------------------------------------
#define MAX_LEN_STRING 1024

//---------------------------------------------------------------------------
class TForm_ConvertNumbers: public TForm {
protected:
	char szString[MAX_LEN_STRING];
	int LngString;
	BOOL bNoEvent;
	void FASTCALL UpdateDialog(TControl *Control);

	//---------------------------------------------------------------------------
	//    void AsciiToHexa(LPSTR szString1, int lng, LPSTR szString2)
	// Description:
	//    Transformation d'une chaîne ASCII ("BF15" par exemple)
	//    en chaîne hexa ("42 46 31 35" dans l'exemple).
	// Entrées
	//    LPSTR szString1: Chaîne ASCII
	//    int lng: nombre de caractères à transformer
	// Sorties
	//    LPSTR szString2: Chaîne hexa
	//---------------------------------------------------------------------------

	void AsciiToHexa(LPCSTR szString1, int lng, LPTSTR szString2);


	//---------------------------------------------------------------------------
	//    int HexaToAscii(LPSTR szString1, LPSTR szString2)
	// Description:
	//    Transformation d'une chaîne hexa ("4246313500" par exemple)
	//    en chaîne ASCII ("BF15" dans l'exemple).
	// Entrées
	//    LPSTR szString1: Chaîne hexa
	// Sorties
	//    LPSTR szString2: Chaîne ASCII
	// Retour:
	//    Nombre de caractères transformés
	//---------------------------------------------------------------------------

	int HexaToAscii(LPCTSTR szString1, LPSTR szString2, int SizeString2);

public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_ChaineHexa;
  TEdit *Edit_ChaineHexa;
  TLabel *Label_ChaineAscii;
  TEdit *Edit_ChaineAscii;
  TLabel *Label_ChaineUnicode;
  TEdit *Edit_ChaineUnicode;
  TLabel *Label_ShortInt;
  TEdit *Edit_ShortInt;
  TLabel *Label_LongInt;
  TEdit *Edit_LongInt;
  TLabel *Label_Int64;
  TEdit *Edit_Int64;
  TLabel *Label_Float;
  TEdit *Edit_Float;
  TLabel *Label_Double;
  TEdit *Edit_Double;
  TButton *Button_RotLeft;
  TButton *Button_RotRight;
  TButton *Button_Invert;
  TShape *Shape_2;
  TShape *Shape_1;
  TCheckBox *CheckBox_Signe;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Edit_ChaineHexaChange(TObject *Sender);
  void FASTCALL Edit_ChaineAsciiChange(TObject *Sender);
  void FASTCALL Edit_ChaineUnicodeChange(TObject *Sender);
  void FASTCALL Edit_ShortIntChange(TObject *Sender);
  void FASTCALL Edit_LongIntChange(TObject *Sender);
  void FASTCALL Edit_Int64Change(TObject *Sender);
  void FASTCALL Edit_FloatChange(TObject *Sender);
  void FASTCALL Edit_DoubleChange(TObject *Sender);
  void FASTCALL Button_RotLeftClick(TObject *Sender);
  void FASTCALL Button_RotRightClick(TObject *Sender);
  void FASTCALL Button_InvertClick(TObject *Sender);
  void FASTCALL CheckBox_SigneClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_ChaineHexaChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_ChaineHexaChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_ChaineAsciiChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_ChaineUnicodeChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_ShortIntChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_LongIntChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_Int64Change, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_FloatChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnChange, Edit_DoubleChange, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnClick, Button_RotLeftClick, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnClick, Button_RotRightClick, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnClick, Button_InvertClick, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnClick, CheckBox_SigneClick, TObject*);
    SET_EVENT_STR_1(TForm_ConvertNumbers, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_ConvertNumbers(TComponent *Owner, LPCTSTR szName);
  virtual ~TForm_ConvertNumbers(void);


  //! @name Propriétés
  //@{

  //@}

};
//---------------------------------------------------------------------------
#endif
