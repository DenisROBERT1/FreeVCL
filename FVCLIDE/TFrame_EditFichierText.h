//-----------------------------------------------------------------------------
//! @file TFrame_EditFichierText.h
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

#ifndef TFrame_EditFichierTextH
#define TFrame_EditFichierTextH

//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------
#include <FreeVCL.h>
#include <FreeVCL_ToolsEDI.h>
#include "TFrame_FVCLIDEBase.h"


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum TCodeFormatType {
	cfNone,												//!< Aucun formatage de code
	cfCPP,												//!< Formatage de code de type (C, C++)
	cfPAS,												//!< Formatage de code de type Pascal
	cfDFM,												//!< Formatage de code de type DFM
	cfHTML												//!< Formatage de code de type HTML ou XML
};

typedef struct {
  TCHAR szExt[4];
  TDictionnary * Keywords;
} LIST_KEYWORDS;


//---------------------------------------------------------------------------
class TFrame_EditFichierText: public TFrame_FVCLIDEBase {
private:
protected:
	TCodeFormatType FCodeFormatType;

	static int NbFrame_EditFichierText;
	static std::vector<LIST_KEYWORDS> ListsKeywords;

	bool SetKeyWords(AnsiString asExt);
	bool SetParameters(AnsiString asExt);

	AnsiString asThisFile;
	AnsiString asVarName;
	AnsiString asConvert;

  bool FASTCALL CodeFormatCPP(TStrings *sIn, TStrings *sOut, bool bEndLineAcc, bool bSpacesAfterPar);
  bool FASTCALL CodeFormatPAS(TStrings *sIn, TStrings *sOut, bool bSpacesAfterPar);
  bool FASTCALL CodeFormatDFM(TStrings *sIn, TStrings *sOut, bool bSpacesAfterPar);
  bool FASTCALL CodeFormatHTML(TStrings *sIn, TStrings *sOut);

public:
  FVCL_BEGIN_COMPONENTS
  TTextEditor *TextEditor;
  TPanel *Panel;
	TPopupMenu *PopupMenu_Context;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Frame_EditFichierTextShow(TObject *Sender);
	void FASTCALL TextEditorContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled);
  void FASTCALL TextEditorDropFile(TObject *Sender, AnsiString &asFileName, TShiftState Shift, int X, int Y);
	void FASTCALL ObjectCaretMove(TObject *Sender, int X, int Y);
	void FASTCALL ObjectDisplayMessage(TObject *Sender, const AnsiString &asMessage);
	void FASTCALL ObjectChange(TObject *Sender);
	void FASTCALL ObjectFlagChanged(TObject *Sender, bool bChanged);
	void FASTCALL ObjectPassBegin(TObject *Sender);
	void FASTCALL ObjectPassEnd(TObject *Sender);
	void FASTCALL ObjectAutocompletionOn(TObject *Sender, AnsiString asParent, int X, int Y, int NumCompletion);
	void FASTCALL ObjectAutocompletionOff(TObject *Sender);
  void FASTCALL MenuItem_CouperClick(TObject *Sender);
  void FASTCALL MenuItem_CopierClick(TObject *Sender);
  void FASTCALL MenuItem_CollerClick(TObject *Sender);
	void FASTCALL MenuItem_AddBookmarkClick(TObject *Sender);
	void FASTCALL MenuItem_AddBreakpointClick(TObject *Sender);
  void FASTCALL MenuItem_SelectBlockClick(TObject *Sender);
  void FASTCALL MenuItem_SelectParenthesisClick(TObject *Sender);
	void FASTCALL MenuItem_SearchDeclarationClick(TObject *Sender);
	void FASTCALL MenuItem_SearchImplementationClick(TObject *Sender);
	void FASTCALL MenuItem_OpenThisFileClick(TObject *Sender);
	void FASTCALL MenuItem_ConvertClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TFrame_EditFichierText, TOnShow, OnShow, Frame_EditFichierTextShow, TObject *);
		SET_EVENT_STR_3(TFrame_EditFichierText, TOnContextPopup, OnContextPopup, TextEditorContextPopup, TObject *, const TPoint &, bool &);
		SET_EVENT_STR_5(TFrame_EditFichierText, TOnDropFile, OnDropFile, TextEditorDropFile, TObject *, AnsiString, TShiftState, int, int);
		SET_EVENT_STR_3(TFrame_EditFichierText, TOnCaretMove, OnCaretMove, ObjectCaretMove, TObject *, int, int);
		SET_EVENT_STR_2(TFrame_EditFichierText, TOnMessage, OnMessage, ObjectDisplayMessage, TObject *, const AnsiString &);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnChange, ObjectChange, TObject *);
		SET_EVENT_STR_2(TFrame_EditFichierText, TOnFlagChanged, OnFlagChanged, ObjectFlagChanged, TObject *, bool);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnPassBegin, ObjectPassBegin, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnPassEnd, ObjectPassEnd, TObject *);
		SET_EVENT_STR_5(TFrame_EditFichierText, TOnAutocompletionOn, OnAutocompletionOn, ObjectAutocompletionOn, TObject *, AnsiString, int, int, int);
		SET_EVENT_STR_1(TFrame_EditFichierText, TOnAutocompletionOff, OnAutocompletionOff, ObjectAutocompletionOff, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_CouperClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_CopierClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_CollerClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_AddBookmarkClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_AddBreakpointClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_SelectBlockClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_SelectParenthesisClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_SearchDeclarationClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_SearchImplementationClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_OpenThisFileClick, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierText, TNotifyEvent, OnClick, MenuItem_ConvertClick, TObject *);
  }

  FVCL_END_EVENTS

  //! @name Constructeur, destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFrame_EditFichierText(TWinControl *AOwner);

	//---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFrame_EditFichierText(void);

  //@}

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction du cadre
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété FileName
  //!
  //! Cette méthode permet d'écrire la propriété FileName
  //!
  //! @param[in]        NewFileName Propriété à enregistrer
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  virtual bool Set_FileName(AnsiString NewFileName);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Changed
  //!
  //! Cette méthode permet de lire la propriété Changed
  //!
  //! @return @c bool Propriété Changed
  //---------------------------------------------------------------------------

  virtual bool Get_Changed(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Changed
  //!
  //! Cette méthode permet d'écrire la propriété Changed
  //!
  //! @param[in]        NewChanged Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Changed(bool NewChanged);

  //---------------------------------------------------------------------------
  //! @brief Ouverture d'un fichier
  //!
  //! Cette méthode ouvre un fichier.
  //!
  //! @param[in]        asFileName Nom du fichier à ouvrir.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool OpenFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un fichier
  //!
  //! Cette méthode enregistre les données dans le fichier asFileName. Si le
  //! fichier existe déjà, il est écrasé sans confirmation de l'utilisateur.
  //!
  //! @param[in]        asFileName Nom du fichier à enregistrer.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool SaveToFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération undo
  //!
  //! Cette fonction effectue une opération undo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Undo(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette fonction effectue une opération redo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Redo(void);

  //---------------------------------------------------------------------------
  //! @brief Liste des trois premières modifications
  //!
  //! Cette méthode renvoie le libellé des premières modifications
  //!
  //! @return @c AnsiString Liste des modifications (séparées par un retour à
	//! la ligne)
  //---------------------------------------------------------------------------
  virtual AnsiString FASTCALL GetModificationsList(void);

  //---------------------------------------------------------------------------
  //! @brief Couper et copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CutToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CopyToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Coller depuis le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL PasteFromClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Formatage du code
  //!
  //! Cette méthode formate automatiquement le code en utilisant l'algorithme
	//! défini par la propriété @b CodeFormatType.
  //!
  //! @param[in]        bEndLineAcc Si @b true: les accolades sont en fin de ligne (ligne suivante sinon)
  //! @param[in]        bSpacesAfterPar Si @b true: espace après parenthèse ouvrante (et avant parenthèse fermante)
  //!
  //! @return @c bool true si Ok.
  //!
  //! @sa CodeFormatType
  //---------------------------------------------------------------------------

  bool FASTCALL CodeFormat(bool bEndLineAcc, bool bSpacesAfterPar);

  //---------------------------------------------------------------------------
  //! @brief Coloration syntaxique et paramètres
  //!
  //! Cette méthode force la coloration syntaxique et les paramètres liés à une
	//! extension.
  //!
  //! @param[in]        asExt Extension
  //!
  //! @return @c bool true si extension connue
  //---------------------------------------------------------------------------
  bool FASTCALL SetLanguage(AnsiString asExt);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété CodeFormatType: Type de formatage du code
	//!
	//! Cette propriété définit le traitement à effectuer lorsque l'utilisateur
	//! demande le formatage du code.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_EditFichierText, TCodeFormatType, CodeFormatType);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnAutocompletionOn
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur tape une chaîne
	//! correspondante à l'autocomplétion.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//! @param[in]        Arg2 Objet parent des attributs, propriétés ou méthodes à proposer
  //! @param[in]        Arg3 Coordonnée X du curseur (pixels)
  //! @param[in]        Arg4 Coordonnée Y du curseur (pixels)
  //! @param[in]        Arg5 Type d'autocomplétion reconnue (. ou -> par exemple) ou -1 si commande lancée manuellement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_5(TOnAutocompletionOn, OnAutocompletionOn, TObject *, AnsiString, int, int, int);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnAutocompletionOff
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur effectue une action
	//! qui stoppe l'autocomplétion (déplacement curseur par exemple).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnAutocompletionOff, OnAutocompletionOff, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnHint
	//!
	//! Cet évènement est déclenché lorsque l'info-bulle est sur le point
	//! d'apparaitre.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnHint, OnHint, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnPassBegin
	//!
	//! Cet évènement est déclenché lorsque la position dans le fichier passe
	//! le début du fichier, soit suite à une recherche, soit par l'exécution
	//! d'une macro.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnPassBegin, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnPassEnd
	//!
	//! Cet évènement est déclenché lorsque la position dans le fichier passe
	//! la fin du fichier, soit suite à une recherche, soit par l'exécution
	//! d'une macro.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnPassEnd, TObject *);

  //@}

};
//---------------------------------------------------------------------------
#endif
