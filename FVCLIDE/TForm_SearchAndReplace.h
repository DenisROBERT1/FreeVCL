//-----------------------------------------------------------------------------
//! @file TForm_SearchAndReplace.h
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

#ifndef TForm_SearchAndReplaceH
#define TForm_SearchAndReplaceH

//---------------------------------------------------------------------------
class TForm_SearchAndReplace : public TForm {
protected:
  SEARCHPARAM *FSearchParam;
	bool bCanReplace;
	bool bAbort;
	bool bBusy;
	int CurrentFileSearch;

	void FASTCALL StartSearch(void);
	void FASTCALL EndSearch(void);
public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_Chercher;
  TComboBox *ComboBox_Chercher;
  TLabel *Label_Remplacer;
  TComboBox *ComboBox_Remplacer;
  TCheckBox *CheckBox_MotEntier;
  TCheckBox *CheckBox_MAJmin;
  TCheckBox *CheckBox_DebutLigne;
  TCheckBox *CheckBox_FinLigne;
  TCheckBox *CheckBox_DansCommentaires;
  TCheckBox *CheckBox_HorsCommentaires;
  TCheckBox *CheckBox_DansGuillemets;
  TCheckBox *CheckBox_HorsGuillemets;
  TCheckBox *CheckBox_ExpressionReg;
  TCheckBox *CheckBox_ConserverCasse;
  TGroupBox *GroupBox_Options;
  TRadioButton *RadioButton_FichiersProjet;
  TRadioButton *RadioButton_FichiersTousProjet;
  TRadioButton *RadioButton_FichiersOuverts;
	TRadioButton *RadioButton_Repertoire;
  TCheckBox *CheckBox_SousRep;
  TGroupBox *GroupBox_ChercherDans;
  TRadioButton *RadioButton_VersLeHaut;
  TRadioButton *RadioButton_VersLeBas;
  TGroupBox *GroupBox_Direction;
  TRadioButton *RadioButton_DepuisDebut;
  TRadioButton *RadioButton_DepuisCurseur;
	TCheckBox *CheckBox_Boucler;
  TGroupBox *GroupBox_Origine;
  TRadioButton *RadioButton_Globale;
  TRadioButton *RadioButton_TexteSelectionne;
  TGroupBox *GroupBox_Portee;
  TLabel *Label_Repertoire;
  TEdit *Edit_Repertoire;
  TButton *Button_Repertoire;
  TLabel *Label_Extensions;
  TComboBox *ComboBox_Extensions;
  TTabControl *TabControl;
  TButton *Button_Chercher;
  TButton *Button_Remplacer;
  TButton *Button_Tout;
  TButton *Button_Annuler;
  TButton *Button_Aide;
	TProgressBar *ProgressBar;
  TButton *Button_Abandon;
	TLabel *Label_Progress;
	TTimer *Timer_AlphaBlend;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL TabControlChange(TObject *Sender);
	void FASTCALL Timer_AlphaBlendTimer(TObject *Sender);
  void FASTCALL CheckBox_DansCommentairesClick(TObject *Sender);
  void FASTCALL CheckBox_HorsCommentairesClick(TObject *Sender);
  void FASTCALL CheckBox_DansGuillemetsClick(TObject *Sender);
  void FASTCALL CheckBox_HorsGuillemetsClick(TObject *Sender);
  void FASTCALL RadioButton_DepuisDebutClick(TObject *Sender);
  void FASTCALL RadioButton_DepuisCurseurClick(TObject *Sender);
  void FASTCALL RadioButton_RepertoireClick(TObject *Sender);
  void FASTCALL RadioButton_FichiersProjetClick(TObject *Sender);
  void FASTCALL RadioButton_FichiersTousProjetClick(TObject *Sender);
  void FASTCALL RadioButton_FichiersOuvertsClick(TObject *Sender);
  void FASTCALL Button_RepertoireClick(TObject *Sender);
  void FASTCALL Button_ChercherClick(TObject *Sender);
  void FASTCALL Button_RemplacerClick(TObject *Sender);
  void FASTCALL Button_ToutClick(TObject *Sender);
  void FASTCALL Button_AbandonClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
  void FASTCALL FormClose(TObject *Sender, TCloseAction &Action);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnChange, TabControlChange, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnTimer, Timer_AlphaBlendTimer, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, CheckBox_DansCommentairesClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, CheckBox_HorsCommentairesClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, CheckBox_DansGuillemetsClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, CheckBox_HorsGuillemetsClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, RadioButton_DepuisDebutClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, RadioButton_DepuisCurseurClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, RadioButton_RepertoireClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, RadioButton_FichiersProjetClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, RadioButton_FichiersTousProjetClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, RadioButton_FichiersOuvertsClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, Button_RepertoireClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, Button_ChercherClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, Button_RemplacerClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, Button_ToutClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, Button_AbandonClick, TObject *);
    SET_EVENT_STR_1(TForm_SearchAndReplace, TNotifyEvent, OnClick, Button_AideClick, TObject *);
    SET_EVENT_STR_2(TForm_SearchAndReplace, TOnClose, OnClose, FormClose, TObject *, TCloseAction &);
  }

  FVCL_END_EVENTS

  TForm_SearchAndReplace(TComponent *Owner, LPCTSTR szName);
  virtual ~TForm_SearchAndReplace(void);


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété SearchParam
	//!
	//! Cette propriété permet de lire et de définir les paramètres de recherche.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_SearchAndReplace, SEARCHPARAM *, SearchParam);

  //@}

};
//---------------------------------------------------------------------------
#endif
