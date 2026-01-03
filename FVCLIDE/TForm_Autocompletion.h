//-----------------------------------------------------------------------------
//! @file TForm_Autocompletion.h
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


#ifndef TForm_AutocompletionH
#define TForm_AutocompletionH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TFrame_EditFichierText;


//---------------------------------------------------------------------------
class TForm_Autocompletion : public TForm {
private:

protected:
	TStringList *FItems;
	int TopIndex;
	int ItemIndex;
	int ItemHeight;
	unsigned int MinTickCount;

public:

  FVCL_BEGIN_COMPONENTS  // Composants gérés par l'EDI
	TPaintBox *PaintBox_Autocompletion;
	TPanel *Panel_Autocompletion;
	TTimer *Timer_AlphaBlend;
	FVCL_END_COMPONENTS  // Fin des composants gérés par l'EDI

  FVCL_BEGIN_EVENTS  // Evènements gérés par l'EDI
	void FASTCALL Timer_AlphaBlendTimer(TObject *Sender);
	void FASTCALL PaintBox_AutocompletionPaint(TObject *Sender);
	void FASTCALL PaintBox_AutocompletionMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void FASTCALL PaintBox_AutocompletionMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled);
	void FASTCALL PaintBox_AutocompletionMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_Autocompletion, TNotifyEvent, OnTimer, Timer_AlphaBlendTimer, TObject*);
    SET_EVENT_STR_1(TForm_Autocompletion, TNotifyEvent, OnPaint, PaintBox_AutocompletionPaint, TObject*);
    SET_EVENT_STR_4(TForm_Autocompletion, TOnMouseMove, OnMouseMove, PaintBox_AutocompletionMouseMove, TObject*, TShiftState, int, int);
    SET_EVENT_STR_4(TForm_Autocompletion, TOnMouseWheel, OnMouseWheel, PaintBox_AutocompletionMouseWheel, TObject*, int, const TPoint &, bool &);
    SET_EVENT_STR_5(TForm_Autocompletion, TOnMouseDown, OnMouseDown, PaintBox_AutocompletionMouseDown, TObject*, TMouseButton, TShiftState, int, int);
  }

  FVCL_END_EVENTS  // Fin des évènements gérés par l'EDI


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TForm_Autocompletion(TComponent* Owner, LPCTSTR szName);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TForm_Autocompletion(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Items
	//!
	//! Cette propriété permet de lire les libellés des éléments de la liste
	//! de propriétés et méthodes proposées à l'auto-complétion.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Autocompletion, TStrings *, Items);


  //@}


};

//---------------------------------------------------------------------------

#else  // TForm_AutocompletionH

// Déclaration anticipée
class TForm_Autocompletion;

#endif  // TForm_AutocompletionH

//---------------------------------------------------------------------------

