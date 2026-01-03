//-----------------------------------------------------------------------------
//! @file TForm_Statistics.h
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

#ifndef TForm_StatisticsH
#define TForm_StatisticsH

//---------------------------------------------------------------------------
class TForm_Statistics: public TForm {
protected:
  int NbCharacters;
  int NbLines;
  int NbLinesCode;
  int NbLinesComment;
  int NbLinesCommentEmpty;
  int NbLinesCodeComment;
  int NbFichiersTraites;
  int NbLinesEmpty;
  bool bRecursif;
  bool bAbort;
	bool bBusy;

  bool FASTCALL ParseFichiersOuverts(void);
  bool FASTCALL ParseFichiersRep(const TCHAR* szPath);
  bool FASTCALL TraitementFichier(AnsiString asFileNameIn);
  bool FASTCALL TraitementFichier(TStream *Stream, AnsiString asCommentSingle, AnsiString asCommentSingleBeg, AnsiString asCommentBegin, AnsiString asCommentEnd, AnsiString asCommentBegin2, AnsiString asCommentEnd2, AnsiString asEscapeString,
																	TCHAR QuoteBegin, TCHAR QuoteEnd, TCHAR QuoteBegin2, TCHAR QuoteEnd2, TCHAR QuoteBegin3, TCHAR QuoteEnd3);

public:
  FVCL_BEGIN_COMPONENTS
  TRadioButton *RadioButton_FichiersOuverts;
  TRadioButton *RadioButton_FichiersProjet;
  TRadioButton *RadioButton_Repertoire;
  TCheckBox *CheckBox_SousRep;
  TLabel *Label_Repertoire;
  TEdit *Edit_Repertoire;
  TButton *Button_Repertoire;
  TLabel *Label_Extensions;
  TComboBox *ComboBox_Extensions;
  TMemo *Memo_Statistics;
  TCheckBox *CheckBox_Cumul;
  TButton *Button_Statistics;
  TButton *Button_Sortie;
  TButton *Button_Aide;
	TProgressBar *ProgressBar;
  TButton *Button_Abandon;
	TLabel *Label_Progress;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL RadioButton_FichiersOuvertsClick(TObject *Sender);
  void FASTCALL RadioButton_FichiersProjetClick(TObject *Sender);
  void FASTCALL RadioButton_RepertoireClick(TObject *Sender);
  void FASTCALL Button_RepertoireClick(TObject *Sender);
  void FASTCALL Button_StatisticsClick(TObject *Sender);
  void FASTCALL Button_AbandonClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
  void FASTCALL FormClose(TObject *Sender, TCloseAction &Action);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TForm_Statistics, TNotifyEvent, OnClick, RadioButton_FichiersOuvertsClick, TObject*);
		SET_EVENT_STR_1(TForm_Statistics, TNotifyEvent, OnClick, RadioButton_FichiersProjetClick, TObject*);
		SET_EVENT_STR_1(TForm_Statistics, TNotifyEvent, OnClick, RadioButton_RepertoireClick, TObject*);
		SET_EVENT_STR_1(TForm_Statistics, TNotifyEvent, OnClick, Button_RepertoireClick, TObject*);
		SET_EVENT_STR_1(TForm_Statistics, TNotifyEvent, OnClick, Button_StatisticsClick, TObject*);
		SET_EVENT_STR_1(TForm_Statistics, TNotifyEvent, OnClick, Button_AbandonClick, TObject*);
		SET_EVENT_STR_1(TForm_Statistics, TNotifyEvent, OnClick, Button_AideClick, TObject*);
		SET_EVENT_STR_2(TForm_Statistics, TOnClose, OnClose, FormClose, TObject *, TCloseAction &);
  }

  FVCL_END_EVENTS

  TForm_Statistics(TComponent *Owner, LPCTSTR szName);
  virtual ~TForm_Statistics(void);


  //! @name Propriétés
  //@{

  //@}

};
//---------------------------------------------------------------------------
#endif
