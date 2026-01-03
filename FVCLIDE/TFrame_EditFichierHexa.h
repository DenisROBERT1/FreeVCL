//-----------------------------------------------------------------------------
//! @file TFrame_EditFichierHexa.h
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

#ifndef TFrame_EditFichierHexaH
#define TFrame_EditFichierHexaH

//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------
#include <FreeVCL.h>
#include <FreeVCL_ToolsEDI.h>
#include "TFrame_FVCLIDEBase.h"


//---------------------------------------------------------------------------
class TFrame_EditFichierHexa: public TFrame_FVCLIDEBase {
private:
protected:

public:
  FVCL_BEGIN_COMPONENTS
  THexaEditor *HexaEditor;
  TPanel *Panel;
	TPopupMenu *PopupMenu_Context;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Frame_EditFichierHexaShow(TObject *Sender);
  void FASTCALL HexaEditorDropFile(TObject *Sender, AnsiString &asFileName, TShiftState Shift, int X, int Y);
	void FASTCALL ObjectCaretMove(TObject *Sender, __int64 FilePos);
	void FASTCALL ObjectDisplayMessage(TObject *Sender, const AnsiString &asMessage);
	void FASTCALL ObjectChange(TObject *Sender);
	void FASTCALL ObjectFlagChanged(TObject *Sender, bool bChanged);
	void FASTCALL ObjectPassBegin(TObject *Sender);
	void FASTCALL ObjectPassEnd(TObject *Sender);
	void FASTCALL HexaEditorContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled);
  void FASTCALL MenuItem_CouperClick(TObject *Sender);
  void FASTCALL MenuItem_CopierClick(TObject *Sender);
  void FASTCALL MenuItem_CollerClick(TObject *Sender);
	void FASTCALL MenuItem_AddBookmarkClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TOnShow, OnShow, Frame_EditFichierHexaShow, TObject*);
		SET_EVENT_STR_5(TFrame_EditFichierHexa, TOnDropFile, OnDropFile, HexaEditorDropFile, TObject *, AnsiString, TShiftState, int, int);
		SET_EVENT_STR_2(TFrame_EditFichierHexa, TOnCaretMove2, OnCaretMove, ObjectCaretMove, TObject *, __int64);
		SET_EVENT_STR_2(TFrame_EditFichierHexa, TOnMessage, OnMessage, ObjectDisplayMessage, TObject *, const AnsiString &);
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TNotifyEvent, OnClick, ObjectChange, TObject*);
		SET_EVENT_STR_2(TFrame_EditFichierHexa, TOnFlagChanged, OnFlagChanged, ObjectFlagChanged, TObject *, bool);
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TNotifyEvent, OnPassBegin, ObjectPassBegin, TObject *);
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TNotifyEvent, OnPassEnd, ObjectPassEnd, TObject *);
		SET_EVENT_STR_3(TFrame_EditFichierHexa, TOnContextPopup, OnContextPopup, HexaEditorContextPopup, TObject *, const TPoint &, bool &);
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TNotifyEvent, OnClick, MenuItem_CouperClick, TObject*);
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TNotifyEvent, OnClick, MenuItem_CopierClick, TObject*);
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TNotifyEvent, OnClick, MenuItem_CollerClick, TObject*);
		SET_EVENT_STR_1(TFrame_EditFichierHexa, TNotifyEvent, OnClick, MenuItem_AddBookmarkClick, TObject*);
  }

  FVCL_END_EVENTS

  //! @name Constructeur, destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFrame_EditFichierHexa(TWinControl *AOwner);

	//---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFrame_EditFichierHexa(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction du cadre
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

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

  //@}


  //! @name Propriétés
  //@{

  //@}

  //! @name Evènements
  //@{

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
