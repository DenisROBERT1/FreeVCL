//-----------------------------------------------------------------------------
//! @file TTextEditor.cpp
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



#include <FreeVcl.h>

#include <TTextEditor.h>
#include <TTextEditorLines.h>
#include <TDictionnary.h>
#include "TTooltip.h"

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum ACTIONUNDO {
	auMoveCur,						               //!< Déplacement du curseur texte
	auChange,						                 //!< Modification d'une ligne
	auDeleteSubString,						       //!< Suppression d'une partie de ligne
	auInsertSubString,						       //!< Insertion dans une ligne
	auSplit,						                 //!< Séparation d'une ligne en deux parties
	auMerge,						                 //!< Concaténation de lignes
	auDeleteLine,						             //!< Suppression d'une ligne
	auInsertLine,						             //!< Insertion d'une ligne
	auSetLines							             //!< Modification de toutes les lignes
};

typedef struct {
	ACTIONUNDO ActionUndo;
	int FirstCol;
	int FirstLine;
	int NumCol;
	int NumLine;
	int NumColBegSel;
	int NumLineBegSel;
	int NumColEndSel;
	int NumLineEndSel;
	bool bRectSel;
} UNDOMOVECUR, *LPUNDOMOVECUR;

typedef struct {
	ACTIONUNDO ActionUndo;
	int NumLine;
	TCHAR szText[1];
} UNDOCHANGE, *LPUNDOCHANGE;

typedef struct {
	ACTIONUNDO ActionUndo;
	int NumLine;
	int Index;
	int Count;
} UNDODELETESUBSTRING, *LPUNDODELETESUBSTRING;

typedef struct {
	ACTIONUNDO ActionUndo;
	int NumLine;
	int Index;
	TCHAR szText[1];
} UNDOINSERTSUBSTRING, *LPUNDOINSERTSUBSTRING;

typedef struct {
	ACTIONUNDO ActionUndo;
	int NumLine;
	int Index;
} UNDOSPLIT, *LPUNDOSPLIT;

typedef struct {
	ACTIONUNDO ActionUndo;
	int NumLine;
} UNDOMERGE, *LPUNDOMERGE;

typedef struct {
	ACTIONUNDO ActionUndo;
	int NumLine;
} UNDODELETELINE, *LPUNDODELETELINE;

typedef struct {
	ACTIONUNDO ActionUndo;
	int NumLine;
	TCHAR szText[1];
} UNDOINSERTLINE, *LPUNDOINSERTLINE;

typedef struct {
	ACTIONUNDO ActionUndo;
	TCHAR szText[1];
} UNDOSETLINES, *LPUNDOSETLINES;


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

TTooltip *TTextEditor::FTooltip = NULL;

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TTextEditor::TTextEditor(TComponent* AOwner):
	TCustomEditor(AOwner) {

	TTextEditorLines *TextEditorLines = new TTextEditorLines();
	TextEditorLines->WantLastLine = true;
	SET_EVENT(TextEditorLines, TNotifyEvent, OnChange, TTextEditor, this, LinesChange);

  // Initialisations
  FClassName = _T("TTextEditor");
	FBigEndian = false;
	FByteOrderMark = false;
	FLinuxEndLine = false;
	FEncodage = ceASCII;
	FNbSpacesTab = 3;
	FSpacesToTabs = true;
	FTabsToSpaces = false;
	FLines = TextEditorLines;  // $$$ A modifier (on ne devrait pas pouvoir accéder à FLines sans passer par UndoRedo).
	FLinesNumbers = true;
	FIconsColumn = false;
	FKeywords = NULL;
	FTypeAuto = true;
	FIndentAuto = true;
	FCurAfterEndLine = true;
	FUndoRedoMoveCur = true;
	FAlphaChars = _T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	FAutocompletion = false;
	FAutocompletion1 = _T("");
	FAutocompletion2 = _T("");
	FAutocompletion3 = _T("");
	FCaseSensitive = true;
	FCommentSingle = _T("");
	FCommentSingleBeg = _T("");
	FCommentBegin = _T("");
	FCommentEnd = _T("");
	FCommentBegin2 = _T("");
	FCommentEnd2 = _T("");
	FBlockBegin = _T("");
	FBlockEnd = _T("");
	FPreProc = _T("");
	FQuoteBegin = _T('\0');
	FQuoteEnd = _T('\0');
	FQuoteBegin2 = _T('\0');
	FQuoteEnd2 = _T('\0');
	FQuoteBegin3 = _T('\0');
	FQuoteEnd3 = _T('\0');
	FEscapeString = _T("");
	FMultiLine = _T("");
	FAssociated1 = _T("");
	FAssociated2 = _T("");

	WantTabs = true;
	WantReturns = true;
	bCaret = false;
	bMouseCapture = false;
	bReadOnly = false;
	bInsert = true;
	bDblClick = false;
	bNoEvent = false;
	NbCol = 1;
	FFirstLine = 0;
	FFirstCol = 0;
	FNumLine = 0;
	FNumCol = 0;
	NumLineBegSel = 0;
	NumColBegSel = 0;
	NumLineEndSel = 0;
	NumColEndSel = 0;
	CurrentTypeUndo = tuNull;
	bAutocompletionOn = false;
	bScrolling = false;
	BitmapScroll = NULL;
	bNoEndOperation = false;

	SET_EVENT(this, TNotifyEvent, OnResize, TTextEditor, this, TextEditorResize);
	SET_EVENT(this, TOnEnter, OnEnter, TTextEditor, this, TextEditorEnter);
	SET_EVENT(this, TOnExit, OnExit, TTextEditor, this, TextEditorExit);
	SET_EVENT(this, TOnKeyDown, OnKeyDown, TTextEditor, this, TextEditorKeyDown);
	SET_EVENT(this, TOnKeyPress, OnKeyPress, TTextEditor, this, TextEditorKeyPress);
	SET_EVENT(this, TOnMouseDown, OnMouseDown, TTextEditor, this, TextEditorMouseDown);
	SET_EVENT(this, TOnMouseUp, OnMouseUp, TTextEditor, this, TextEditorMouseUp);
	SET_EVENT(this, TOnMouseMove, OnMouseMove, TTextEditor, this, TextEditorMouseMove);
	SET_EVENT(this, TOnMouseWheel, OnMouseWheel, TTextEditor, this, TextEditorMouseWheel);
	SET_EVENT(this, TNotifyEvent, OnDblClick, TTextEditor, this, TextEditorDblClick);

	// Horizontale
	FHorzScrollBar->Min = 0;
	FHorzScrollBar->Max = 1000;
	FHorzScrollBar->PageSize = 0;
	FHorzScrollBar->Position = 0;

	// Verticale
	FVertScrollBar->Min = 0;
	FVertScrollBar->Max = 1000;
	FVertScrollBar->PageSize = 0;
	FVertScrollBar->Position = 0;

	SET_EVENT(FHorzScrollBar, TOnScroll, OnScroll, TTextEditor, this, HorzScrollBarScroll);
	SET_EVENT(FVertScrollBar, TOnScroll, OnScroll, TTextEditor, this, VertScrollBarScroll);

	if (FTooltip == NULL) FTooltip = new TTooltip();
	FTooltip->IncUse();

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TTextEditor::~TTextEditor(void) {
	SetCaretVisible(false);
	if (CurrentTypeUndo != tuNull) UndoRedo->EndOperation();
	delete FLines;
	if (BitmapScroll) delete BitmapScroll;
	if (FTooltip->DecUse() == 0) {
		delete FTooltip;
		FTooltip = NULL;
	}
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BigEndian
//---------------------------------------------------------------------------

bool TTextEditor::Get_BigEndian(void) {
  return FBigEndian;
}

bool TTextEditor::Set_BigEndian(bool NewBigEndian) {
  if (FBigEndian != NewBigEndian) {
    FBigEndian = NewBigEndian;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ByteOrderMark
//---------------------------------------------------------------------------

bool TTextEditor::Get_ByteOrderMark(void) {
  return FByteOrderMark;
}

bool TTextEditor::Set_ByteOrderMark(bool NewByteOrderMark) {
  if (FByteOrderMark != NewByteOrderMark) {
    FByteOrderMark = NewByteOrderMark;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LinuxEndLine
//---------------------------------------------------------------------------

bool TTextEditor::Get_LinuxEndLine(void) {
  return FLinuxEndLine;
}

bool TTextEditor::Set_LinuxEndLine(bool NewLinuxEndLine) {
  if (FLinuxEndLine != NewLinuxEndLine) {
    FLinuxEndLine = NewLinuxEndLine;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Encodage
//---------------------------------------------------------------------------

CHARENCODE TTextEditor::Get_Encodage(void) {
  return FEncodage;
}

bool TTextEditor::Set_Encodage(CHARENCODE NewEncodage) {
  if (FEncodage != NewEncodage) {
    FEncodage = NewEncodage;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FFirstLine
//---------------------------------------------------------------------------

int TTextEditor::Get_FirstLine() {
	return FFirstLine;
}

bool TTextEditor::Set_FirstLine(int NewFirstLine) {
	if (FFirstLine != NewFirstLine) {
		SetFirstLine(NewFirstLine);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FFirstCol
//---------------------------------------------------------------------------

int TTextEditor::Get_FirstCol() {
	return FFirstCol;
}

bool TTextEditor::Set_FirstCol(int NewFirstCol) {
	if (FFirstCol != NewFirstCol) {
		SetFirstCol(NewFirstCol);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FNumLine
//---------------------------------------------------------------------------

int TTextEditor::Get_NumLine() {
	return FNumLine;
}

bool TTextEditor::Set_NumLine(int NewNumLine) {
	if (FNumLine != NewNumLine) {
		MoveCur(NewNumLine, FNumCol, false, false);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FNumCol
//---------------------------------------------------------------------------

int TTextEditor::Get_NumCol() {
	return FNumCol;
}

bool TTextEditor::Set_NumCol(int NewNumCol) {
	if (FNumCol != NewNumCol) {
		MoveCur(NumLine, NewNumCol, false, false);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Keywords
//---------------------------------------------------------------------------

TStrings * TTextEditor::Get_Keywords(void) {
	return FKeywords;
}

bool TTextEditor::Set_Keywords(TStrings *NewKeywords) {
	if (FKeywords != NewKeywords) {
		FKeywords = NewKeywords;
		FKeywords->CaseSensitive = FCaseSensitive;
		InvalidateOrSetFlag();
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Lines
//---------------------------------------------------------------------------

TStrings * TTextEditor::Get_Lines(void) {
	return FLines;
}

bool TTextEditor::Set_Lines(TStrings *NewLines) {
	if (FLines != NewLines) {
		FLines->Assign(NewLines);
		InvalidateOrSetFlag();
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LinesNumbers
//---------------------------------------------------------------------------

bool TTextEditor::Get_LinesNumbers(void) {
	return FLinesNumbers;
}

bool TTextEditor::Set_LinesNumbers(bool NewLinesNumbers) {
	if (FLinesNumbers != NewLinesNumbers) {
		FLinesNumbers = NewLinesNumbers;
		InvalidateOrSetFlag();
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété IconsColumn
//---------------------------------------------------------------------------

bool TTextEditor::Get_IconsColumn(void) {
	return FIconsColumn;
}

bool TTextEditor::Set_IconsColumn(bool NewIconsColumn) {
	if (FIconsColumn != NewIconsColumn) {
		FIconsColumn = NewIconsColumn;
		InvalidateOrSetFlag();
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NbSpacesTab
//---------------------------------------------------------------------------

int TTextEditor::Get_NbSpacesTab(void) {
	return FNbSpacesTab;
}

bool TTextEditor::Set_NbSpacesTab(int NewNbSpacesTab) {
	if (FNbSpacesTab != NewNbSpacesTab) {
		FNbSpacesTab = NewNbSpacesTab;
		if (FNbSpacesTab < 1) FNbSpacesTab = 1;
		InvalidateOrSetFlag();
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SpacesToTabs
//---------------------------------------------------------------------------

bool TTextEditor::Get_SpacesToTabs(void) {
	return FSpacesToTabs;
}

bool TTextEditor::Set_SpacesToTabs(bool NewSpacesToTabs) {
	if (FSpacesToTabs != NewSpacesToTabs) {
		FSpacesToTabs = NewSpacesToTabs;
		if (FSpacesToTabs) FTabsToSpaces = false;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TabsToSpaces
//---------------------------------------------------------------------------

bool TTextEditor::Get_TabsToSpaces(void) {
	return FTabsToSpaces;
}

bool TTextEditor::Set_TabsToSpaces(bool NewTabsToSpaces) {
	if (FTabsToSpaces != NewTabsToSpaces) {
		FTabsToSpaces = NewTabsToSpaces;
		if (FTabsToSpaces) FSpacesToTabs = false;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelText
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_SelText(void) {
	return GetSelText();
}

bool TTextEditor::Set_SelText(AnsiString asNewSelText) {
	SetTypeUndoRedo(tuText);
	SetSelTextBuf(asNewSelText);
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TypeAuto
//---------------------------------------------------------------------------

bool TTextEditor::Get_TypeAuto(void) {
	return FTypeAuto;
}

bool TTextEditor::Set_TypeAuto(bool NewTypeAuto) {
	if (FTypeAuto != NewTypeAuto) {
		FTypeAuto = NewTypeAuto;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété IndentAuto
//---------------------------------------------------------------------------

bool TTextEditor::Get_IndentAuto(void) {
	return FIndentAuto;
}

bool TTextEditor::Set_IndentAuto(bool NewIndentAuto) {
	if (FIndentAuto != NewIndentAuto) {
		FIndentAuto = NewIndentAuto;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CurAfterEndLine
//---------------------------------------------------------------------------

bool TTextEditor::Get_CurAfterEndLine(void) {
	return FCurAfterEndLine;
}

bool TTextEditor::Set_CurAfterEndLine(bool NewCurAfterEndLine) {
	if (FCurAfterEndLine != NewCurAfterEndLine) {
		FCurAfterEndLine = NewCurAfterEndLine;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété UndoRedoMoveCur
//---------------------------------------------------------------------------

bool TTextEditor::Get_UndoRedoMoveCur(void) {
	return FUndoRedoMoveCur;
}

bool TTextEditor::Set_UndoRedoMoveCur(bool NewUndoRedoMoveCur) {
	if (FUndoRedoMoveCur != NewUndoRedoMoveCur) {
		FUndoRedoMoveCur = NewUndoRedoMoveCur;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AlphaChars
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_AlphaChars(void) {
	return FAlphaChars;
}

bool TTextEditor::Set_AlphaChars(AnsiString NewAlphaChars) {
	if (FAlphaChars != NewAlphaChars) {
		FAlphaChars = NewAlphaChars;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Autocompletion
//---------------------------------------------------------------------------

bool TTextEditor::Get_Autocompletion(void) {
	return FAutocompletion;
}

bool TTextEditor::Set_Autocompletion(bool NewAutocompletion) {
	if (FAutocompletion != NewAutocompletion) {
		FAutocompletion = NewAutocompletion;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Autocompletion1
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_Autocompletion1(void) {
	return FAutocompletion1;
}

bool TTextEditor::Set_Autocompletion1(AnsiString NewAutocompletion1) {
	if (FAutocompletion1 != NewAutocompletion1) {
		FAutocompletion1 = NewAutocompletion1;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Autocompletion2
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_Autocompletion2(void) {
	return FAutocompletion2;
}

bool TTextEditor::Set_Autocompletion2(AnsiString NewAutocompletion2) {
	if (FAutocompletion2 != NewAutocompletion2) {
		FAutocompletion2 = NewAutocompletion2;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Autocompletion3
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_Autocompletion3(void) {
	return FAutocompletion3;
}

bool TTextEditor::Set_Autocompletion3(AnsiString NewAutocompletion3) {
	if (FAutocompletion3 != NewAutocompletion3) {
		FAutocompletion3 = NewAutocompletion3;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CaseSensitive
//---------------------------------------------------------------------------

bool TTextEditor::Get_CaseSensitive(void) {
	return FCaseSensitive;
}

bool TTextEditor::Set_CaseSensitive(bool NewCaseSensitive) {
	if (FCaseSensitive != NewCaseSensitive) {
		FCaseSensitive = NewCaseSensitive;
		if (FKeywords) FKeywords->CaseSensitive = FCaseSensitive;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CommentSingle
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_CommentSingle(void) {
	return FCommentSingle;
}

bool TTextEditor::Set_CommentSingle(AnsiString NewCommentSingle) {
	if (FCommentSingle != NewCommentSingle) {
		FCommentSingle = NewCommentSingle;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CommentSingleBeg
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_CommentSingleBeg(void) {
	return FCommentSingleBeg;
}

bool TTextEditor::Set_CommentSingleBeg(AnsiString NewCommentSingleBeg) {
	if (FCommentSingleBeg != NewCommentSingleBeg) {
		FCommentSingleBeg = NewCommentSingleBeg;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CommentBegin
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_CommentBegin(void) {
	return FCommentBegin;
}

bool TTextEditor::Set_CommentBegin(AnsiString NewCommentBegin) {
	if (FCommentBegin != NewCommentBegin) {
		FCommentBegin = NewCommentBegin;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CommentEnd
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_CommentEnd(void) {
	return FCommentEnd;
}

bool TTextEditor::Set_CommentEnd(AnsiString NewCommentEnd) {
	if (FCommentEnd != NewCommentEnd) {
		FCommentEnd = NewCommentEnd;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CommentBegin2
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_CommentBegin2(void) {
	return FCommentBegin2;
}

bool TTextEditor::Set_CommentBegin2(AnsiString NewCommentBegin2) {
	if (FCommentBegin2 != NewCommentBegin2) {
		FCommentBegin2 = NewCommentBegin2;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CommentEnd2
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_CommentEnd2(void) {
	return FCommentEnd2;
}

bool TTextEditor::Set_CommentEnd2(AnsiString NewCommentEnd2) {
	if (FCommentEnd2 != NewCommentEnd2) {
		FCommentEnd2 = NewCommentEnd2;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BlockBegin
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_BlockBegin(void) {
	return FBlockBegin;
}

bool TTextEditor::Set_BlockBegin(AnsiString NewBlockBegin) {
	if (FBlockBegin != NewBlockBegin) {
		FBlockBegin = NewBlockBegin;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BlockEnd
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_BlockEnd(void) {
	return FBlockEnd;
}

bool TTextEditor::Set_BlockEnd(AnsiString NewBlockEnd) {
	if (FBlockEnd != NewBlockEnd) {
		FBlockEnd = NewBlockEnd;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PreProc
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_PreProc(void) {
	return FPreProc;
}

bool TTextEditor::Set_PreProc(AnsiString NewPreProc) {
	if (FPreProc != NewPreProc) {
		FPreProc = NewPreProc;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété QuoteBegin
//---------------------------------------------------------------------------

TCHAR TTextEditor::Get_QuoteBegin(void) {
	return FQuoteBegin;
}

bool TTextEditor::Set_QuoteBegin(TCHAR NewQuoteBegin) {
	if (FQuoteBegin != NewQuoteBegin) {
		FQuoteBegin = NewQuoteBegin;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété QuoteEnd
//---------------------------------------------------------------------------

TCHAR TTextEditor::Get_QuoteEnd(void) {
	return FQuoteEnd;
}

bool TTextEditor::Set_QuoteEnd(TCHAR NewQuoteEnd) {
	if (FQuoteEnd != NewQuoteEnd) {
		FQuoteEnd = NewQuoteEnd;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété QuoteBegin2
//---------------------------------------------------------------------------

TCHAR TTextEditor::Get_QuoteBegin2(void) {
	return FQuoteBegin2;
}

bool TTextEditor::Set_QuoteBegin2(TCHAR NewQuoteBegin2) {
	if (FQuoteBegin2 != NewQuoteBegin2) {
		FQuoteBegin2 = NewQuoteBegin2;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété QuoteEnd2
//---------------------------------------------------------------------------

TCHAR TTextEditor::Get_QuoteEnd2(void) {
	return FQuoteEnd2;
}

bool TTextEditor::Set_QuoteEnd2(TCHAR NewQuoteEnd2) {
	if (FQuoteEnd2 != NewQuoteEnd2) {
		FQuoteEnd2 = NewQuoteEnd2;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété QuoteBegin3
//---------------------------------------------------------------------------

TCHAR TTextEditor::Get_QuoteBegin3(void) {
	return FQuoteBegin3;
}

bool TTextEditor::Set_QuoteBegin3(TCHAR NewQuoteBegin3) {
	if (FQuoteBegin3 != NewQuoteBegin3) {
		FQuoteBegin3 = NewQuoteBegin3;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété QuoteEnd3
//---------------------------------------------------------------------------

TCHAR TTextEditor::Get_QuoteEnd3(void) {
	return FQuoteEnd2;
}

bool TTextEditor::Set_QuoteEnd3(TCHAR NewQuoteEnd3) {
	if (FQuoteEnd3 != NewQuoteEnd3) {
		FQuoteEnd3 = NewQuoteEnd3;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété EscapeString
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_EscapeString(void) {
	return FEscapeString;
}

bool TTextEditor::Set_EscapeString(AnsiString NewEscapeString) {
	if (FEscapeString != NewEscapeString) {
		FEscapeString = NewEscapeString;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiLine
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_MultiLine(void) {
	return FMultiLine;
}

bool TTextEditor::Set_MultiLine(AnsiString NewMultiLine) {
	if (FMultiLine != NewMultiLine) {
		FMultiLine = NewMultiLine;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Associated1
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_Associated1(void) {
	return FAssociated1;
}

bool TTextEditor::Set_Associated1(AnsiString NewAssociated1) {
	if (FAssociated1 != NewAssociated1) {
		FAssociated1 = NewAssociated1;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Associated2
//---------------------------------------------------------------------------

AnsiString TTextEditor::Get_Associated2(void) {
	return FAssociated2;
}

bool TTextEditor::Set_Associated2(AnsiString NewAssociated2) {
	if (FAssociated2 != NewAssociated2) {
		FAssociated2 = NewAssociated2;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Tooltip
//---------------------------------------------------------------------------

TTooltip *TTextEditor::Get_Tooltip(void) {
	return FTooltip;
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnChange de l'objet Lines
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::LinesChange(TObject *Sender) {
	if (!bNoEvent) {
		UpdateVScroll();
		CalculNbCol();
		UpdateVScroll();	// Une seconde fois (parce que l'apparition de la scrollbar horizontale peut changer la hauteur visible)
		InvalidateOrSetFlag();
	}
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnResize
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::TextEditorResize(TObject *Sender) {
	ProcessResize();
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnEnter
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::TextEditorEnter(TObject *Sender) {
	SetCaretVisible(true);
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnExit
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::TextEditorExit(TObject *Sender) {
	HWND hWnd;
	TCHAR szCaption[20];


	SetCaretVisible(false);
	FTooltip->Event(FHandle);

	// Grosse verrue pour éviter qu'un clic sur l'autocomplétion provoque
	// un AutocompletionOff (et donc une fermeture avant le traitement du clic)
	if (bAutocompletionOn) {
		hWnd = ::GetFocus();
		GetWindowText(hWnd, szCaption, 20);
		if (lstrcmp(szCaption, _T("[AUTOCOMP]"))) {
			AutocompletionOff();  // Après le SetCaretVisible, car AutocompletionOff peut rendre le focus
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TextEditorKeyDown(TObject *Sender,
			WORD Key, TShiftState Shift) {

	bNoEvent = true;	// Pour éviter un réaffichage complet de la fenêtre
	FTooltip->Hide();

	bool bAlt = Shift.Contains(ssAlt);
	bool bCtrl = Shift.Contains(ssCtrl);
	bool bShift = Shift.Contains(ssShift);
	switch (Key) {
	case VK_LEFT:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		if (!bCtrl || bAlt) MoveCur(FNumLine, FNumCol - 1, bShift || (bCtrl && bAlt), bCtrl && bAlt);
		else MovePrevWord(bShift);
		break;
	case VK_RIGHT:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		if (!bCtrl || bAlt) MoveCur(FNumLine, FNumCol + 1, bShift || (bCtrl && bAlt), bCtrl && bAlt);
		else MoveNextWord(bShift);
		break;
	case VK_UP:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		if (!bCtrl || bAlt) MoveCur(FNumLine - 1, FNumCol, bShift || (bCtrl && bAlt), bCtrl && bAlt);
		else SetFirstLine(FFirstLine - 1);
		break;
	case VK_DOWN:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		if (!bCtrl || bAlt) MoveCur(FNumLine + 1, FNumCol, bShift || (bCtrl && bAlt), bCtrl && bAlt);
		else SetFirstLine(FFirstLine + 1);
		break;
	case VK_HOME:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		if (bCtrl) MoveCur(0, 0, bShift, bCtrl && bAlt);
		else if (FNumCol != 0) MoveCur(FNumLine, 0, bShift, bCtrl && bAlt);
		else MoveFirstChar(bShift);
		break;
	case VK_END:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		if (bCtrl) MoveCur(FLines->Count - 1, Lines_Length(FLines->Count - 1), bShift, bCtrl && bAlt);
		else MoveCur(FNumLine, Lines_Length(FNumLine), bShift, bCtrl && bAlt);
		break;
	case VK_PRIOR:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		MoveCur(FNumLine - NbLinesWin + 1, FNumCol, bShift, bCtrl && bAlt);
		break;
	case VK_NEXT:
		AutocompletionOff();
		SetTypeUndoRedo(tuMoveCur);
		MoveCur(FNumLine + NbLinesWin - 1, FNumCol, bShift, bCtrl && bAlt);
		break;
	case VK_TAB:
		AutocompletionOff();
		if (NumLineBegSel != NumLineEndSel) {
			if (bShift) UnindentSelection();
			else IndentSelection();
		}
		else {
			int NewNumCol;
			int Index;

			ClearSelection();

			SetTypeUndoRedo(tuText);

			Lines_SetLengthMin(FNumLine, FNumCol); // Espaces entre la fin de ligne et la position du curseur

			if (bShift) {
				NewNumCol = FNumCol;
				Index = Lines_Index2(FNumLine, FNumCol);
				if (Index != -1) {
					AnsiString asLine = FLines->Strings[NumLine];
					if (Index > 0 && asLine[Index] == _T('\t')) {
						Lines_DeleteSubString(FNumLine, Index, 1);
						NewNumCol = Lines_NumCol(FNumLine, Index - 1);
					}
				}
			}
			else {
				Lines_SetLengthMin(FNumLine, FNumCol); // Espaces entre la fin de ligne et la position du curseur
				Index = Lines_Index2(FNumLine, FNumCol);
				AnsiString asIndent;
				if (FTabsToSpaces) {
					int i;
					for (i = 0; i < FNbSpacesTab; i++) asIndent += _T(' ');
				}
				else {
					asIndent = _T('\t');
				}
				Index = Lines_InsertSubString(FNumLine, asIndent, Index);
				NewNumCol = Lines_NumCol(FNumLine, Index);
			}
			DrawLine(Canvas, FNumLine);
			MoveCur(FNumLine, NewNumCol, false, false);

		}
		break;
	case VK_BACK:
		if (!bReadOnly) {
			AutocompletionOff();
			if (NumLineBegSel != NumLineEndSel || NumColBegSel != NumColEndSel) {
				SetTypeUndoRedo(tuDelete);
				ClearSelection();
				MoveCur(FNumLine, FNumCol, false, false);
			}
			else {
				int Index = Lines_Index2(FNumLine, FNumCol);
				if (Index == -1) {
					MoveCur(FNumLine, FNumCol - 1, false, false);
				}
				else if (Index > 0) {
					SetTypeUndoRedo(tuDelete);
					Lines_DeleteSubString(FNumLine, Index - 1, 1);
					DrawLine(Canvas, FNumLine);
					int NewNumCol = Lines_NumCol(FNumLine, Index - 1);
					MoveCur(FNumLine, NewNumCol, false, false);
				}
				else if (FNumLine > 0) {
					SetTypeUndoRedo(tuDelete);
					int NewNumLine = FNumLine - 1;	// Variable temporaire car Lines_Merge peut modifier FNumLine
					int NewNumCol = Lines_Length(NewNumLine);
					Lines_Merge(NewNumLine);
					MoveCur(NewNumLine, NewNumCol, false, false);
					InvalidateOrSetFlag();
				}
			}
		}
		break;
	case VK_INSERT:
		if (bCtrl) {
			// Copier
			CopyToClipboard();
		}
		else if (bShift) {
			if (!bReadOnly) {
				// Coller
				AutocompletionOff();
				SetTypeUndoRedo(tuPaste);
				ClearSelection();
				PasteFromClipboard();
			}
		}
		else {
			bool bMemoCaret = bCaret;
			if (bMemoCaret) SetCaretVisible(false);
			bInsert = !bInsert;
			if (bMemoCaret) SetCaretVisible(true);
		}
		break;
	case VK_DELETE:
		if (NumLineBegSel != NumLineEndSel || NumColBegSel != NumColEndSel) {
			// Copier
			if (bShift) CopyToClipboard();
			if (!bReadOnly) {
				SetTypeUndoRedo(tuDelete);
				ClearSelection();
				MoveCur(FNumLine, FNumCol, false, false);
			}
		}
		else {
			if (!bReadOnly) {
				int Index = Lines_Index(FNumLine, FNumCol);
				if (Index != -1) {
					SetTypeUndoRedo(tuDelete);
					Lines_DeleteSubString(FNumLine, Index, 1);
					DrawLine(Canvas, FNumLine);
					int NewNumCol = Lines_NumCol(FNumLine, Index);
					MoveCur(FNumLine, NewNumCol, false, false);
				}
				else if (FNumLine < FLines->Count - 1) {
					SetTypeUndoRedo(tuDelete);
					Lines_SetLengthMin(FNumLine, FNumCol); // Espaces entre la fin de ligne et la position du curseur
					Lines_Merge(FNumLine);
					MoveCur(FNumLine, FNumCol, false, false);
					InvalidateOrSetFlag();
				}
			}
		}
		break;
	case VK_RETURN:
		if (!bReadOnly) {
			AutocompletionOff();
			int NewNumLine, NewNumCol;
			ClearSelection();
			SetTypeUndoRedo(tuText);
			int Index = Lines_Index(FNumLine, FNumCol);
			if (Index == -1) Lines_InsertLine(FNumLine + 1, "");
			else Lines_Split(FNumLine, Index);
			NewNumLine = FNumLine + 1;
			NewNumCol = 0;
			// Indentation automatique
			if (FIndentAuto && !FBlockBegin.IsEmpty() && !FBlockEnd.IsEmpty()) {
				AnsiString asLine;
				int NumLineIndent = NewNumLine;
				// Recherche de la ligne précédente non vide
				do {
					if (--NumLineIndent < 0) break;
					asLine = FLines->Strings[NumLineIndent];
				} while (asLine.Trim().IsEmpty());
				// Si ligne précédente trouvée
				if (NumLineIndent >= 0) {
					int Indent;
					int i;
					// Comptage de l'indentation de la ligne précédente non vide
					Indent = NbIndentAfterLine(asLine);
					// Si la ligne d'indentation est une fin de bloc: on l'indente correctement
					// Mais uniquement si le return a eu lieu sur cette ligne (c.à.d. NumLineIndent == NumLine)
					// Sinon, comportement non intuitif (ligne au-dessus du curseur modifiée)
					if (asLine.Trim() == FBlockEnd && NumLineIndent == NumLine) {
						// Recherche de la ligne précédente non vide
						do {
							if (--NumLineIndent < 0) break;
							asLine = FLines->Strings[NumLineIndent];
						} while (asLine.Trim().IsEmpty());
						// Si ligne précédente trouvée
						if (NumLineIndent >= 0) {
							// Comptage de l'indentation de la ligne précédente non vide - 1 (- 1 car fin de bloc)
							Indent = NbIndentAfterLine(asLine) - 1;
							// Modification de la ligne d'indentation
							asLine = _T("");
							for (i = 0; i < Indent; i++) {
								asLine += _T('\t');
							}
							asLine += FBlockEnd;
							Lines_Change(NumLine, asLine);
						}
					}
					// Insertion de l'indentation
					for (i = 0; i < Indent; i++) {
						Lines_InsertSubString(NewNumLine, _T('\t'), i);
						NewNumCol += FNbSpacesTab;
					}
				}
			}
			MoveCur(NewNumLine, NewNumCol, false, false);
			InvalidateOrSetFlag();
		}
		break;
	case VK_ESCAPE:
		AutocompletionOff();
		break;
	default:
		if (bCtrl) {
			switch (Key) {
			case _T('X'):
				// CTRL + X
				// Couper
				CutToClipboard();
				break;
			case _T('C'):
				// CTRL + C
				// Copier
				CopyToClipboard();
				break;
			case _T('V'):
				// CTRL + V
				// Coller
				AutocompletionOff();
				PasteFromClipboard();
				break;
			}
		}
	}

	bNoEvent = false;

}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TextEditorKeyPress(TObject *Sender, TCHAR Key) {
	AnsiString asLine;


	bNoEvent = true;	// Pour éviter un réaffichage complet de la fenêtre
	FTooltip->Hide();

	if (!bReadOnly) {
		if ((BYTE) Key >= _T(' ')) {

			if (Key == _T(' ')) AutocompletionOff();

			// Effacement de la sélection éventuelle
			ClearSelection();

			SetTypeUndoRedo(tuText);

			Lines_SetLengthMin(FNumLine, FNumCol); // Espaces entre la fin de ligne et la position du curseur

			bool bInsertTab = false;
			int Index = Lines_Index2(FNumLine, FNumCol);
			int IndexBeg = 0;
			int IndexEnd = 0;
			int NewNumCol;
			if (!bInsert) Lines_DeleteSubString(FNumLine, Index, 1);
			if (Key == _T(' ') && FSpacesToTabs) {
				// Remplacement automatique des espaces par des tabulations
				AnsiString asLine;
				int i;
				asLine = FLines->Strings[NumLine];
				IndexBeg = Lines_Index2(FNumLine, (FNumCol / FNbSpacesTab) * FNbSpacesTab);
				IndexEnd = Lines_Index2(FNumLine, ((FNumCol + 1) / FNbSpacesTab) * FNbSpacesTab);
				if (IndexEnd == -1) IndexEnd = IndexBeg + FNbSpacesTab - 1;
				if (IndexBeg < IndexEnd && IndexEnd <= asLine.Length()) {
					bInsertTab = true;
					for (i = IndexBeg; i < IndexEnd; i++) {
						if (asLine[i + 1] != _T(' ')) {
							bInsertTab = false;
							break;
						}
					}
				}
			}
			if (bInsertTab) {
				// Suppression des espaces et insertion d'une tabulation
				Lines_DeleteSubString(FNumLine, IndexBeg, FNbSpacesTab - 1);
				Lines_InsertSubString(FNumLine, _T('\t'), IndexBeg);
				NewNumCol = Lines_NumCol(FNumLine, IndexBeg + 1);
			}
			else {
				// Insertion du caractère (ne pas convertir en BYTE, Key peut être supérieur à 255 avec les ALT + clavier numérique)
				Lines_InsertSubString(FNumLine, (TCHAR) Key, Index);
				NewNumCol = Lines_NumCol(FNumLine, Index + 1);
			}

			if (FTypeAuto) {
				if (Key == _T('{')) Lines_InsertSubString(FNumLine, _T('}'), Index + 1);
				if (Key == _T('(')) Lines_InsertSubString(FNumLine, _T(')'), Index + 1);
			}
			MoveCur(FNumLine, NewNumCol, false, false);
			DrawLine(Canvas, FNumLine);
			if (FAutocompletion) {
				int NumCompletion = 0;
				int LenAutoComp;
				int i;
				if (!FAutocompletion1.IsEmpty()) {
					LenAutoComp = FAutocompletion1.Length();
					if (Key == FAutocompletion1[LenAutoComp] && LenAutoComp <= FNumCol) {
						asLine = Lines_Strings(FNumLine);
						NumCompletion = 1;
						for (i = 1; i <= LenAutoComp; i++) {
							if (asLine[FNumCol - LenAutoComp + i] != FAutocompletion1[i]) {
								NumCompletion = 0;
								break;
							}
						}
					}
				}
				if (!FAutocompletion2.IsEmpty()) {
					LenAutoComp = FAutocompletion2.Length();
					if (Key == FAutocompletion2[LenAutoComp] && LenAutoComp <= FNumCol) {
						asLine = Lines_Strings(FNumLine);
						NumCompletion = 2;
						for (i = 1; i <= LenAutoComp; i++) {
							if (asLine[FNumCol - LenAutoComp + i] != FAutocompletion2[i]) {
								NumCompletion = 0;
								break;
							}
						}
					}
				}
				if (!FAutocompletion3.IsEmpty()) {
					LenAutoComp = FAutocompletion3.Length();
					if (Key == FAutocompletion3[LenAutoComp] && LenAutoComp <= FNumCol) {
						asLine = Lines_Strings(FNumLine);
						NumCompletion = 3;
						for (i = 1; i <= LenAutoComp; i++) {
							if (asLine[FNumCol - LenAutoComp + i] != FAutocompletion3[i]) {
								NumCompletion = 0;
								break;
							}
						}
					}
				}
				if (NumCompletion != 0) AutocompletionOn(NumCompletion);
			}
		}
	}

	bNoEvent = false;

}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TextEditorMouseDown(
			TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
			int Y) {

	SetFocus();
	FTooltip->Event(FHandle);

	if (!bDblClick) {

		int NewNumCol, NewNumLine;
		bool bCtrl = Shift.Contains(ssCtrl);
		bool bAlt = Shift.Contains(ssAlt);
		bool bShift = Shift.Contains(ssShift);
		AutocompletionOff();
		GetPosAt(Y, X, &NewNumLine, &NewNumCol);
		SetTypeUndoRedo(tuMoveCur);

		if (Button == mbLeft) {
			MoveCur(NewNumLine, NewNumCol, bShift, bShift && !bCtrl && bAlt);
			Mouse->Capture = Handle;
			bMouseCapture = true;
		}
		else if (Button == mbRight) {
			if (!IsInSelection(NewNumLine, NewNumCol)) {
				MoveCur(NewNumLine, NewNumCol, false, false);
			}
		}

		bDblClick = false;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TextEditorMouseUp(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y) {

	FTooltip->Event(FHandle);

	if (bMouseCapture) {
		int NewNumCol, NewNumLine;
		bool bAlt = Shift.Contains(ssAlt);
		bool bCtrl = Shift.Contains(ssCtrl);
		bool bShift = Shift.Contains(ssShift);
		GetPosAt(Y, X, &NewNumLine, &NewNumCol);
		SetTypeUndoRedo(tuMoveCur);
		MoveCur(NewNumLine, NewNumCol, true, (!bShift && bCtrl && bAlt) || (bShift && !bCtrl && bAlt));
		Mouse->Capture = NULL;
		bMouseCapture = false;
	}
	bDblClick = false;
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TextEditorMouseMove(
			TObject *Sender, TShiftState Shift, int X, int Y) {
	int NewNumCol, NewNumLine;

	FTooltip->Event(FHandle);

	GetPosAt(Y, X, &NewNumLine, &NewNumCol);
	if (bMouseCapture) {
		bool bAlt = Shift.Contains(ssAlt);
		bool bCtrl = Shift.Contains(ssCtrl);
		bool bShift = Shift.Contains(ssShift);
		MoveCur(NewNumLine, NewNumCol, (Mouse->Capture == Handle), !bShift && bCtrl && bAlt);
	}
	else {
		if (IsInSelection(NewNumLine, NewNumCol)) Cursor = crArrow;
		else Cursor = crIBeam;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TextEditorMouseWheel(TObject *Sender, TShiftState Shift,
			int WheelDelta, const TPoint &MousePos, bool &Handled) {

	FTooltip->Event(FHandle);
	AutocompletionOff();
	SetTypeUndoRedo(tuMoveCur);
	SetFirstLine(FFirstLine - WheelDelta / 30);
}


//---------------------------------------------------------------------------
void FASTCALL TTextEditor::HorzScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos) {
	FTooltip->Event(FHandle);
	AutocompletionOff();
	SetTypeUndoRedo(tuMoveCur);
	SetFirstCol(ScrollPos);
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::VertScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos) {

	FTooltip->Event(FHandle);
	AutocompletionOff();
	if (ScrollCode == scTrack) bScrolling = true;
	else if (ScrollCode == scEndScroll) {
		bScrolling = false;
		Invalidate();
	}
	SetTypeUndoRedo(tuMoveCur);
	SetFirstLine(ScrollPos);
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TextEditorDblClick(TObject *Sender) {

	FTooltip->Event(FHandle);
	AutocompletionOff();
	SetTypeUndoRedo(tuMoveCur);
	SelectWord(FNumLine, FNumCol);
	Mouse->Capture = NULL;
	bMouseCapture = false;
	bDblClick = true;
}

//---------------------------------------------------------------------------
// Dessin de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::Paint(void) {
	int NumLine;
	int y;


	TRect DrawRect;
	DrawRect = ClientRect;

	// Caret
	bool bMemoCaret = bCaret;
	if (bMemoCaret) SetCaretVisible(false);

	TCanvas *ACanvas = Canvas;

	NumLine = FFirstLine;
	for (y = 0; y < DrawRect.Bottom; y += tmHeight + tmInternalLeading) {
		if (NumLine > 0 && NumLine >= FLines->Count) break;
		DrawLine(ACanvas, NumLine);
		NumLine++;
	}

	if (y < DrawRect.Bottom) {
		// Dessin du fond (parce que les lignes ne remplissent pas l'écran)
		DrawRect.top = y;
		ACanvas->Brush->Color = FColorParams[tsNormal].Background;
		ACanvas->Brush->Style = bsSolid;
		ACanvas->FillRect(DrawRect);
		ACanvas->TextFlags = tfTransparent;
	}

	if (bScrolling) {
		TRect RectClient = ClientRect;
		DrawRect.Left = RectClient.Right - 10 - NbColsWin / 2;
		DrawRect.Top = RectClient.Top + GetSystemMetrics(SM_CYVSCROLL);
		DrawRect.Right = RectClient.Right - 10;
		DrawRect.Bottom = RectClient.Bottom - GetSystemMetrics(SM_CYVSCROLL);
		if (BitmapScroll) {
			BitmapScroll->StretchBltMode = smHalfTone;
			BitmapScroll->Draw(ACanvas->Handle, DrawRect.Left, DrawRect.Top, DrawRect.Right - DrawRect.Left, DrawRect.Bottom - DrawRect.Top);

			ACanvas->Brush->Style = bsClear;
			ACanvas->Pen->Color = clBlack;
			ACanvas->Pen->Style = psSolid;
			DrawRect.Left = DrawRect.Left - 1;
			DrawRect.Top = DrawRect.Top - 1;
			DrawRect.Right = DrawRect.Right + 1;
			DrawRect.Bottom = DrawRect.Bottom + 1;
			ACanvas->Rectangle(DrawRect);
		}
	}

	// Caret
	if (bMemoCaret) SetCaretVisible(true);

}

//---------------------------------------------------------------------------
// Affichage du caret
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::SetCaretVisible(bool bVisible) {

	if (bVisible) {
		CreateCaret(Handle, NULL, bInsert? 1: tmAveCharWidth, tmHeight);
		SetCurrentCaretPos();
		OnCaretMove(this, FNumCol + 1, FNumLine + 1);
		ShowCaret(Handle);
		bCaret = true;
	}
	else {
		HideCaret(Handle);
		DestroyCaret();
		bCaret = false;
	}
}

//---------------------------------------------------------------------------
// Positionnement du caret
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::SetCurrentCaretPos(void) {
	int X, Y;

	X = (FNumCol - FFirstCol + 1) * tmAveCharWidth;
	if (FLinesNumbers) X += tmAveCharWidth * 6 + 4;
	if (FIconsColumn) X += 24;
	Y = (int) (FNumLine - FFirstLine) * (tmHeight + tmInternalLeading);

	SetCaretPos(X, Y);
}


//---------------------------------------------------------------------------
// Lecture depuis un fichier
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::LoadFromFile(const AnsiString FileName) {

	if (BitmapScroll) delete BitmapScroll;

	if (FLines->LoadFromFile(FileName)) {
		BitmapScroll = new TBitmap();
		BitmapScroll->Width = 100;
		BitmapScroll->Height = FLines->Count;
		BitmapScroll->Canvas->Brush->Color = FColorParams[tsNormal].Background;
		BitmapScroll->Canvas->Brush->Style = bsSolid;
		BitmapScroll->Canvas->FillRect(0, 0, BitmapScroll->Width, BitmapScroll->Height);
		bNoEndOperation = false;
		SetTypeUndoRedo(tuNull);
		UndoRedo->Clear();
		FBigEndian = FLines->BigEndian;
		FByteOrderMark = FLines->ByteOrderMark;
		FLinuxEndLine = FLines->LinuxEndLine;
		if (FLines->Utf8) FEncodage = ceUtf8;
		else if (FLines->Unicode) FEncodage = ceUNICODE;
		else FEncodage = ceASCII;
		return true;
	}

	BitmapScroll = NULL;
	FLastError = FLines->LastError;

	return false;
}

//---------------------------------------------------------------------------
// Enregistrement dans un fichier
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::SaveToFile(const AnsiString FileName) {

	FLines->BigEndian = FBigEndian;
	FLines->ByteOrderMark = FByteOrderMark;
	FLines->LinuxEndLine = FLinuxEndLine;
	switch (FEncodage) {
		case ceUtf8:
			FLines->Utf8 = true;
			FLines->Unicode = false;
			break;
		case ceUNICODE:
			FLines->Utf8 = false;
			FLines->Unicode = true;
			break;
		default:
			FLines->Utf8 = false;
			FLines->Unicode = false;
			break;
	}
	if (FLines->SaveToFile(FileName)) return true;

	FLastError = FLines->LastError;

	return false;
}

//---------------------------------------------------------------------------
// Déplacement du curseur
//---------------------------------------------------------------------------

void	FASTCALL TTextEditor::MoveCur(int NewNumLine, int NewNumCol, bool bSelection, bool bNewRectSel) {
	int l;
	int NbLin;
	int NumLineOldSel1 = 0;
	int NumLineOldSel2 = 0;
	int NumLineNewSel1 = 0;
	int NumLineNewSel2 = 0;
	bool bDrawOldSel = false;
	bool bDrawNewSel = false;
	bool bChanged = false;


	NbLin = FLines->Count;
	if (NewNumCol < 0) NewNumCol = 0;
	if (NewNumLine >= NbLin) NewNumLine = NbLin - 1;
	if (NewNumLine < 0) NewNumLine = 0;

	if (!FCurAfterEndLine) {
		int PosEndLine = Lines_Length(NewNumLine);
		if (NewNumCol >= PosEndLine) NewNumCol = PosEndLine;
	}

	if (bSelection) {
		if (bRectSel || bNewRectSel) {
			bRectSel = bNewRectSel;
			// Sélection rectangulaire => il faut redessiner toute la sélection
			NumLineOldSel1 = NumLineBegSel;
			NumLineOldSel2 = NumLineEndSel;
			bDrawOldSel = true;
		}
		NumColEndSel = NewNumCol;
		NumLineEndSel = NewNumLine;
	}
	else {
		// Effacement de l'ancienne sélection (si elle existe)
		if (NumLineBegSel != NumLineEndSel || NumColBegSel != NumColEndSel) {
			NumLineOldSel1 = NumLineBegSel;
			NumLineOldSel2 = NumLineEndSel;
			bDrawOldSel = true;
		}
		NumLineBegSel = NumLineEndSel = NewNumLine;
		NumColBegSel = NumColEndSel = NewNumCol;
	}

	if (NewNumCol != FNumCol || NewNumLine != FNumLine ||
			FFirstCol > /*max*/(NbCol > NewNumCol ? NbCol : NewNumCol) - NbColsWin + 2 ||
			FFirstLine > NbLin - NbLinesWin) {

		if (bSelection) {
			NumLineNewSel1 = FNumLine;
			NumLineNewSel2 = NewNumLine;
			bDrawNewSel = true;
		}

		FNumCol = NewNumCol;
		FNumLine = NewNumLine;

		if (FNumCol < FFirstCol || FFirstCol + NbColsWin - 2 < FNumCol) {
			ScrollInView(FNumLine, FNumCol);
			bDrawOldSel = false;
			bDrawNewSel = false;
		}
		else if (FFirstCol > /*max*/(NbCol > FNumCol ? NbCol : FNumCol) - NbColsWin + 2) {
			ScrollInView(FNumLine, FNumCol);
		}

		if (FNumLine < FFirstLine || FNumLine - NbLinesWin + 1 > FFirstLine) {
			ScrollInView(FNumLine, FNumCol);
			bDrawOldSel = false;
			bDrawNewSel = false;
		}
		else if (FFirstLine > NbLin - NbLinesWin) {
			ScrollInView(FNumLine, FNumCol);
		}

		bChanged = true;

	}

	if (bDrawOldSel || bDrawNewSel) {

		TCanvas *ACanvas = Canvas;

		// Mise en ordre des lignes à redessiner
		if (bDrawOldSel && NumLineOldSel1 > NumLineOldSel2) {
			int NumLineOldSel3;
			NumLineOldSel3 = NumLineOldSel1;
			NumLineOldSel1 = NumLineOldSel2;
			NumLineOldSel2 = NumLineOldSel3;
		}
		if (bDrawNewSel && NumLineNewSel1 > NumLineNewSel2) {
			int NumLineNewSel3;
			NumLineNewSel3 = NumLineNewSel1;
			NumLineNewSel1 = NumLineNewSel2;
			NumLineNewSel2 = NumLineNewSel3;
		}

		// Optimisation pour éviter de redessiner plusieurs fois la même ligne
		if (bDrawOldSel && bDrawNewSel) {
			if (NumLineOldSel1 <= NumLineNewSel1 && NumLineNewSel2 <= NumLineOldSel2) {
				// NewSel incluse dans OldSel
				bDrawNewSel = false;
			}
			else if (NumLineNewSel1 <= NumLineOldSel1 && NumLineOldSel2 <= NumLineNewSel2) {
				// OldSel incluse dans NewSel
				bDrawOldSel = false;
			}
			else if (NumLineNewSel1 < NumLineOldSel2 && NumLineOldSel2 <= NumLineNewSel2) {
				// Recouvrement de NewSel sur OldSel
				NumLineOldSel2 = NumLineNewSel2;
				bDrawNewSel = false;
			}
			else if (NumLineOldSel1 < NumLineNewSel2 && NumLineNewSel2 <= NumLineOldSel2) {
				// Recouvrement de OldSel sur NewSel
				NumLineNewSel2 = NumLineOldSel2;
				bDrawOldSel = false;
			}
			// Sinon: OldSel et NewSel sont deux zones distinctes
		}

		// Dessin
		if (bDrawOldSel) {
			for (l = NumLineOldSel1; l <= NumLineOldSel2; l++) DrawLine(Canvas, l);
		}
		if (bDrawNewSel) {
			for (l = NumLineNewSel1; l <= NumLineNewSel2; l++) DrawLine(Canvas, l);
		}

	}

	if (bChanged) {
		SetCurrentCaretPos();
		OnCaretMove(this, FNumCol + 1, FNumLine + 1);
	}

}


//---------------------------------------------------------------------------
// Scroll horizontal de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::SetFirstCol(int NewFirstCol) {

	if (NewFirstCol > /*max*/(NbCol > FNumCol ? NbCol : FNumCol) - NbColsWin + 2) {
		NewFirstCol = /*max*/(NbCol > FNumCol ? NbCol : FNumCol) - NbColsWin + 2;
	}
	if (NewFirstCol < 0) {	// Pas "else if" (NewFirstCol a pu devenir négatif dans le test précédent)
		NewFirstCol = 0;
	}

	if (NewFirstCol != FFirstCol) {
		FFirstCol = NewFirstCol;
		UpdateHScroll();
		InvalidateOrSetFlag();
	}
}


//---------------------------------------------------------------------------
// Scroll vertical de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::SetFirstLine(int NewFirstLine) {

	if (NewFirstLine > FLines->Count - NbLinesWin) {
		NewFirstLine = FLines->Count - NbLinesWin;
	}
	if (NewFirstLine < 0) {  // Pas "else if" (NewFirstLine a pu devenir négatif dans le test précédent)
		NewFirstLine = 0;
	}

	if (NewFirstLine != FFirstLine) {
		FFirstLine = NewFirstLine;
		UpdateVScroll();
		InvalidateOrSetFlag();
	}
}


//---------------------------------------------------------------------------
// Sélection d'un mot complet
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::SelectWord(int NumLine, int NumCol) {
	int NumLineBegSel2;
	int NumLineEndSel2;
	int NumColBegSel2;
	int NumColEndSel2;
	int LenLine;


	if (NumLine > FLines->Count) return false;
	AnsiString asLine = Lines_Strings(NumLine);
	LenLine = asLine.Length();
	if (NumCol > LenLine) return false;
	NumLineBegSel2 = NumLineEndSel2 = NumLine;
	NumColBegSel2 = NumColEndSel2 = NumCol;
	TCHAR c;
	while (NumColBegSel2 > 0) {
		c = asLine[NumColBegSel2];
		if (!IsAlphaNum(c)) break;
		NumColBegSel2--;
	}
	while (NumColEndSel2 < LenLine) {
		c = asLine[NumColEndSel2 + 1];
		if (!IsAlphaNum(c)) break;
		NumColEndSel2++;
	}
	SetTypeUndoRedo(tuMoveCur);
	MoveCur(NumLineBegSel2, NumColBegSel2, false, false);
	MoveCur(NumLineEndSel2, NumColEndSel2, true, false);

	return true;
}

//---------------------------------------------------------------------------
// Sélection d'un mot complet
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::WordFromPos(int NumLine, int NumCol, AnsiString *asWord) {
	int NumColBeg;
	int NumColEnd;
	int LenLine;


	if (NumCol < 0) return false;
	if (NumLine < 0) return false;
	if (NumLine >= FLines->Count) return false;
	AnsiString asLine = Lines_Strings(NumLine);
	LenLine = asLine.Length();
	if (NumCol >= LenLine) return false;
	NumColBeg = NumColEnd = NumCol;
	TCHAR c;
	while (NumColBeg >= 1) {
		c = asLine[NumColBeg];
		if (!IsAlphaNum(c)) break;
		NumColBeg--;
	}
	while (NumColEnd < LenLine) {
		c = asLine[NumColEnd + 1];
		if (!IsAlphaNum(c)) break;
		NumColEnd++;
	}

	if (NumColEnd <= NumColBeg) return false;

	*asWord = asLine.SubString(NumColBeg + 1, NumColEnd - NumColBeg);

	return true;
}

//---------------------------------------------------------------------------
// Sélection d'un bloc délimité
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::SelectBlockDelimiters(int NumLine, int NumCol, AnsiString Beg, AnsiString End) {
	int NumLineBegSel2;
	int NumLineEndSel2;
	int NumColBegSel2;
	int NumColEndSel2;
	int NbAcc;
	AnsiString asLine;
	int LenLine;
	int LenBeg, LenEnd;
	bool bBegEndIsKeyword;
	bool bIsOk;
	int i;


	if (NumLine > FLines->Count) return false;

	LenBeg = Beg.Length();
	LenEnd = End.Length();

	// Teste si Beg et End sont des mots clés (comme en pascal) ou des caractères spéciaux (comme en C)
	bBegEndIsKeyword = true;
	for (i = 1; i <= LenBeg; i++) {
		if (!IsAlphaNum(Beg[i])) {
			bBegEndIsKeyword = false;
			break;
		}
	}
	if (bBegEndIsKeyword) {
		for (i = 1; i <= LenEnd; i++) {
			if (!IsAlphaNum(End[i])) {
				bBegEndIsKeyword = false;
				break;
			}
		}
	}

	// Recherche du début de bloc
	NumLineBegSel2 = NumLine;
	NumColBegSel2 = NumCol - 1;
	asLine = Lines_Strings(NumLineBegSel2);
	LenLine = asLine.Length();
	if (NumColBegSel2 > LenLine) NumColBegSel2 = LenLine;

	NbAcc = 1;
	while (NbAcc > 0) {
		if (NumColBegSel2 < 0) {
			NumLineBegSel2--;
			if (NumLineBegSel2 < 0) return false;
			asLine = Lines_Strings(NumLineBegSel2);
			LenLine = asLine.Length();
			NumColBegSel2 = asLine.Length() - 1;
		}
		else {
			AnsiString asTestString;

			bIsOk = true;
			if (bBegEndIsKeyword) {
				if (NumColBegSel2 >= 1 && IsAlphaNum(asLine[NumColBegSel2])) bIsOk = false;
				if (NumColBegSel2 + LenEnd + 1 <= LenLine && IsAlphaNum(asLine[NumColBegSel2 + LenEnd + 1])) bIsOk = false;
			}
			if (bIsOk) {
				if (NumColBegSel2 + LenEnd <= LenLine) {
					asTestString = asLine.SubString(NumColBegSel2 + 1, LenEnd);
					if (FCaseSensitive) {
						if (End == asTestString) NbAcc++;
					}
					else {
						if (End.AnsiCompareIC(asTestString) == 0) NbAcc++;
					}
				}
			}

			bIsOk = true;
			if (bBegEndIsKeyword) {
				if (NumColBegSel2 >= 1 && IsAlphaNum(asLine[NumColBegSel2])) bIsOk = false;
				if (NumColBegSel2 + LenBeg + 1 <= LenLine && IsAlphaNum(asLine[NumColBegSel2 + LenBeg + 1])) bIsOk = false;
			}
			if (bIsOk) {
				if (NumColBegSel2 + LenBeg <= LenLine) {
					asTestString = asLine.SubString(NumColBegSel2 + 1, LenBeg);
					if (FCaseSensitive) {
						if (Beg == asTestString) NbAcc--;
					}
					else {
						if (Beg.AnsiCompareIC(asTestString) == 0) NbAcc--;
					}
				}
			}

			if (NbAcc != 0) NumColBegSel2--;

		}
	}

	// Recherche de la fin de bloc
	NumLineEndSel2 = NumLine;
	NumColEndSel2 = NumCol;
	asLine = Lines_Strings(NumLineEndSel2);
	LenLine = asLine.Length();
	if (NumColEndSel2 > LenLine) NumColEndSel2 = LenLine;

	NbAcc = 1;
	while (NbAcc > 0) {
		if (NumColEndSel2 >= LenLine) {
			NumLineEndSel2++;
			if (NumLineEndSel2 >= FLines->Count) return false;
			asLine = Lines_Strings(NumLineEndSel2);
			LenLine = asLine.Length();
			NumColEndSel2 = 0;
		}
		else {
			AnsiString asTestString;

			bIsOk = true;
			if (bBegEndIsKeyword) {
				if (NumColEndSel2 >= 1 && IsAlphaNum(asLine[NumColEndSel2])) bIsOk = false;
				if (NumColEndSel2 + LenBeg + 1 <= LenLine && IsAlphaNum(asLine[NumColEndSel2 + LenBeg + 1])) bIsOk = false;
			}
			if (bIsOk) {
				if (NumColEndSel2 + LenBeg <= LenLine) {
					asTestString = asLine.SubString(NumColEndSel2 + 1, LenBeg);
					if (FCaseSensitive) {
						if (Beg == asTestString) NbAcc++;
					}
					else {
						if (Beg.AnsiCompareIC(asTestString) == 0) NbAcc++;
					}
				}
			}

			bIsOk = true;
			if (bBegEndIsKeyword) {
				if (NumColEndSel2 >= 1 && IsAlphaNum(asLine[NumColEndSel2])) bIsOk = false;
				if (NumColEndSel2 + LenEnd + 1 <= LenLine && IsAlphaNum(asLine[NumColEndSel2 + LenEnd + 1])) bIsOk = false;
			}
			if (bIsOk) {
				if (NumColEndSel2 + LenEnd <= LenLine) {
					asTestString = asLine.SubString(NumColEndSel2 + 1, LenEnd);
					if (FCaseSensitive) {
						if (End == asTestString) NbAcc--;
					}
					else {
						if (End.AnsiCompareIC(asTestString) == 0) NbAcc--;
					}
				}
			}

			NumColEndSel2++;

		}
	}

	NumColEndSel2 += LenEnd - 1;

	// Sélection du bloc
	SetTypeUndoRedo(tuMoveCur);
	MoveCur(NumLineBegSel2, NumColBegSel2, false, false);
	MoveCur(NumLineEndSel2, NumColEndSel2, true, false);

	return true;
}

//---------------------------------------------------------------------------
// Sélection d'un bloc d'instructions
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::SelectBlock(int NumLine, int NumCol) {
	return SelectBlockDelimiters(NumLine, NumCol, FBlockBegin, FBlockEnd);
}

//---------------------------------------------------------------------------
// Sélection d'une parenthèse
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::SelectParenthesis(int NumLine, int NumCol) {
	return SelectBlockDelimiters(NumLine, NumCol, _T("("), _T(")"));
}

//---------------------------------------------------------------------------
// Déplacement du curseur au début du mot précédent
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::MovePrevWord(bool bShift) {
	int NumCol, NumLine;
	int DerNumCol, DerNumLine;
	TCHAR c;
	bool bInWord = false;
	AnsiString asLine;
	int LenLine;


	NumCol = FNumCol;
	NumLine = FNumLine;
	DerNumCol = NumCol;  // Inutile, sauf pour ne pas avoir
	DerNumLine = NumLine;	// d'erreur de compilation
	asLine = Lines_Strings(NumLine);
	LenLine = asLine.Length();
	if (NumCol >= LenLine) NumCol = LenLine - 1;
	do {
		if (NumCol >= 0) {
			c = asLine[NumCol + 1];
			if (!IsAlphaNum(c)) {
				if (bInWord) break;
			}
			else {
				if (NumCol != FNumCol || NumLine != FNumLine) { // Pour ne pas prendre en compte le mot sur lequel on est
					DerNumCol = NumCol;
					DerNumLine = NumLine;
					bInWord = true;
				}
			}
			NumCol--;
		}
		else {
			if (bInWord) break;  // Mot en tout début de ligne
			if (NumLine == 0) {
				DerNumCol = 0;
				DerNumLine = 0;
				break;
			}
			asLine = Lines_Strings(--NumLine);
			LenLine = asLine.Length();
			NumCol = LenLine - 1;
		}

	} while (true);

	SetTypeUndoRedo(tuMoveCur);
	MoveCur(DerNumLine, DerNumCol, bShift, false);

	return true;
}

//---------------------------------------------------------------------------
// Déplacement du curseur au début du mot suivant
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::MoveNextWord(bool bShift) {
	int NumCol, NumLine;
	TCHAR c;
	bool bInWord = true;
	AnsiString asLine;
	int LenLine;


	NumCol = FNumCol;
	NumLine = FNumLine;
	asLine = Lines_Strings(NumLine);
	LenLine = asLine.Length();
	do {
		if (NumCol < LenLine) {
			c = asLine[NumCol + 1];
			if (IsAlphaNum(c)) {
				if (!bInWord) break;
			}
			else {
				bInWord = false;
			}
			NumCol++;
		}
		else {
			if (NumLine == FLines->Count - 1) break;
			asLine = Lines_Strings(++NumLine);
			LenLine = asLine.Length();
			NumCol = 0;
			bInWord = false;	// S'il y a un mot en fin de ligne et un autre en tout début de ligne suivante
		}

	} while (true);

	SetTypeUndoRedo(tuMoveCur);
	MoveCur(NumLine, NumCol, bShift, false);

	return true;
}

//---------------------------------------------------------------------------
// Déplacement du curseur au premier caractère non blanc de la ligne
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::MoveFirstChar(bool bShift) {
	int NumCol;
	TCHAR c;
	AnsiString asLine;
	int LenLine;


	NumCol = 0;
	asLine = Lines_Strings(FNumLine);
	LenLine = asLine.Length();
	while (NumCol < LenLine) {
		c = asLine[NumCol + 1];
		if (!IsBlanc(c)) break;
		NumCol++;
	}

	SetTypeUndoRedo(tuMoveCur);
	MoveCur(FNumLine, NumCol, bShift, false);

	return true;
}

//---------------------------------------------------------------------------
// Déplacement de la fenêtre de visualisation
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::ScrollInView(int NumLine, int NumCol) {
	int LinesCount;


	// Position horizontale

	/* Test supprimé : le curseur peut être après la dernière colonne
	if (NumCol > NbCol) NumCol = NbCol;
	*/
	if (NumCol < 0) NumCol = 0;

	int NewFirstCol = FFirstCol;
	if (NewFirstCol > /*max*/(NbCol > NumCol ? NbCol : NumCol) - NbColsWin + 2) {
		// Pour éviter un scroll sans scroll-bar
		NewFirstCol = /*max*/(NbCol > NumCol ? NbCol : NumCol) - NbColsWin + 2;
		if (NewFirstCol < 0) NewFirstCol = 0;
	}
	else if (NewFirstCol < NumCol - NbColsWin + 2) {
		NewFirstCol = NumCol - NbColsWin + 2;
	}
	else if (NewFirstCol > NumCol) {
		NewFirstCol = NumCol;
	}

	if (FFirstCol != NewFirstCol) {
		FFirstCol = NewFirstCol;
		UpdateHScroll();
		InvalidateOrSetFlag();
	}

	// Position verticale

	LinesCount = FLines->Count;
	if (NumLine > LinesCount) NumLine = LinesCount;
	if (NumLine < 0) NumLine = 0;

	int NewFirstLine = FFirstLine;
	if (NewFirstLine > LinesCount - NbLinesWin) {
		// Pour éviter un scroll sans scroll-bar
		NewFirstLine = LinesCount - NbLinesWin;
		if (NewFirstLine < 0) NewFirstLine = 0;
	}
	else if (NewFirstLine < NumLine - NbLinesWin + 1) {
		NewFirstLine = NumLine - NbLinesWin + 1;
	}
	else if (NewFirstLine > NumLine) {
		NewFirstLine = NumLine;
	}

	if (FFirstLine != NewFirstLine) {
		FFirstLine = NewFirstLine;
		UpdateVScroll();
		InvalidateOrSetFlag();
	}

}

//---------------------------------------------------------------------------
// Dessin d'une ligne
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::DrawLine(TCanvas *ACanvas, int NumLine) {
	TRect Rect;
	int x, y;
	int i, j;
	int IndexChar, NextIndexChar;
	int NbChar;
	std::vector<COLORSYNTAX> ColorSyntax;
	AnsiString asLine;
	int LenLine;
	bool bComment;
	bool bComment2;
	bool bQuote1;
	bool bQuote2;
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;
	TColor PixelForeground, PixelBackground;


	if (NbBeginUpdate == 0) {

		if (bCaret) HideCaret(Handle);

		GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);

		TRect DrawRect;
		DrawRect = ClientRect;

		x = tmAveCharWidth;
		y = (tmHeight + tmInternalLeading) * ((int) (NumLine - FFirstLine));

		if (FLinesNumbers) {
			ACanvas->TextFlags = tfOpaque;
			ACanvas->Brush->Color = FColorParams[tsLinesNumbers].Background;
			Rect.left = DrawRect.Left;
			Rect.top = DrawRect.Top + y;
			Rect.right = DrawRect.Left + tmAveCharWidth * 6 + 4;
			Rect.bottom = y + tmHeight;
			ACanvas->FillRect(Rect);

			AnsiString asBuf;
			asBuf.sprintf(_T("%6i"), NumLine + 1);
			if (asBuf.Length() > 6) {
				// Des fois qu'on aurait plus d'un million de lignes !
				asBuf = asBuf.SubString(asBuf.Length() - 5, 6);
			}
			SetColorSyntax(ACanvas, tsLinesNumbers);
			ACanvas->TextOut(DrawRect.Left + 2, y, asBuf);
			DrawRect.Left = DrawRect.Left + tmAveCharWidth * 6 + 4;  // += ne marche pas sous Code:Blocks
		}

		if (FIconsColumn) {
			ACanvas->TextFlags = tfOpaque;
			ACanvas->Brush->Color = FColorParams[tsIconsColumn].Background;
			Rect.left = DrawRect.Left;
			Rect.top = DrawRect.Top + y;
			Rect.right = DrawRect.Left + 24;
			Rect.bottom = y + tmHeight;
			ACanvas->FillRect(Rect);

			if (NumLine < FLines->Count) {

				// Attention, l'ordre a une importance (sinon la flèche est cachée par les icônes plus grosses)

				if (FLines->Flags[NumLine].Contains(lfBookmark)) {
					ACanvas->Pen->Color = RGB(0, 0, 0xFF);
					ACanvas->Brush->Color = RGB(0xCF, 0xCF, 0xFF);
					Rect.left = DrawRect.Left + 3;
					Rect.top = DrawRect.Top + y + 1;
					Rect.right = DrawRect.Left + 20;
					Rect.bottom = DrawRect.Top + y + tmHeight - 1;
					ACanvas->RoundRect(Rect, 5, 5);
				}

				if (FLines->Flags[NumLine].Contains(lfBreakpoint)) {
					ACanvas->Pen->Color = RGB(0xFF, 0, 0);
					ACanvas->Brush->Color = RGB(0xFF, 0x7F, 0x7F);
					Rect.left = DrawRect.Left + 13 - tmHeight / 2;
					Rect.top = DrawRect.Top + y + 1;
					Rect.right = DrawRect.Left + 11 + tmHeight / 2;
					Rect.bottom = DrawRect.Top + y + tmHeight - 1;
					ACanvas->Ellipse(Rect);
				}

				if (FLines->Flags[NumLine].Contains(lfCurStatement)) {
					TPoint Poly[7];
					ACanvas->Pen->Color = RGB(0x7F, 0x7F, 0);
					ACanvas->Brush->Color = RGB(0xFF, 0xFF, 0x7F);
					Poly[0].x = DrawRect.Left + 4;
					Poly[0].y = DrawRect.Top + y + tmHeight / 2 - 2;
					Poly[1].x = DrawRect.Left + 12;
					Poly[1].y = DrawRect.Top + y + tmHeight / 2 - 2;
					Poly[2].x = DrawRect.Left + 12;
					Poly[2].y = DrawRect.Top + y + tmHeight / 2 - 6;
					Poly[3].x = DrawRect.Left + 18;
					Poly[3].y = DrawRect.Top + y + tmHeight / 2;
					Poly[4].x = DrawRect.Left + 12;
					Poly[4].y = DrawRect.Top + y + tmHeight / 2 + 6;
					Poly[5].x = DrawRect.Left + 12;
					Poly[5].y = DrawRect.Top + y + tmHeight / 2 + 2;
					Poly[6].x = DrawRect.Left + 4;
					Poly[6].y = DrawRect.Top + y + tmHeight / 2 + 2;
					ACanvas->Polygon(Poly, 7);
				}

			}

			DrawRect.Left = DrawRect.Left + 24;  // += ne marche pas sous Code:Blocks
		}

		ACanvas->TextFlags = tfOpaque;
		ACanvas->Brush->Style = bsSolid;
		ACanvas->Brush->Color = FColorParams[tsNormal].Background;
		Rect.left = DrawRect.Left;
		Rect.top = DrawRect.Top + y;
		Rect.right = DrawRect.Right;
		Rect.bottom = y + tmHeight;
		ACanvas->FillRect(Rect);

		if (NumLine < FLines->Count) {
			bComment = FLines->FlagCommentBegin[NumLine];
			bComment2 = FLines->FlagCommentBegin2[NumLine];
			bQuote1 = FLines->FlagQuote1Begin[NumLine];
			bQuote2 = FLines->FlagQuote2Begin[NumLine];
			asLine = Lines_Strings(NumLine);
			LenLine = asLine.Length();
			ColorationSyntaxique(NumLine, asLine, true, &ColorSyntax, &bComment, &bComment2, &bQuote1, &bQuote2);
		}
		else {
			bComment = false;
			asLine = _T("");
			LenLine = 0;
		}

		if (FFirstCol == 0) {
			if (!bRectSel &&
					((NumLineBegSel2 < NumLine && NumLine < NumLineEndSel2) ||
					 (NumLine != NumLineEndSel2 && NumLine == NumLineBegSel2 && NumColBegSel2 == 0) ||
					 (NumLine != NumLineBegSel2 && NumLine == NumLineEndSel2 && NumColEndSel2 > 0))) {
				SetColorSyntax(ACanvas, tsSelect);
				ACanvas->TextOut(DrawRect.Left, DrawRect.Top + y, _T(" "));
			}
		}

		for (i = 0; i < (int) ColorSyntax.size(); i++) {
			SetColorSyntax(ACanvas, ColorSyntax[i].Type);
			IndexChar = ColorSyntax[i].IndexChar;
			if (i < (int) ColorSyntax.size() - 1) NextIndexChar = ColorSyntax[i + 1].IndexChar;
			else NextIndexChar = LenLine;
			NbChar = NextIndexChar - IndexChar;
			FVCL_ASSERT(NbChar >= 0);
			FVCL_ASSERT(NbChar > 0);
			if (IndexChar + NbChar >= FFirstCol) {
				if (IndexChar < FFirstCol) {
					NbChar -= FFirstCol - IndexChar;
					IndexChar = FFirstCol;
				}
				ACanvas->TextOut(DrawRect.Left + x, DrawRect.Top + y, asLine.SubString(IndexChar + 1, NbChar));
				x += NbChar * tmAveCharWidth;
				if (x >= DrawRect.Right) break;
			}
			if (BitmapScroll) {
				PixelForeground = FColorParams[ColorSyntax[i].Type].Foreground;
				PixelBackground = FColorParams[ColorSyntax[i].Type].Background;
				for (j = IndexChar; j < NextIndexChar && j < 100; j++) {
					if (asLine[j + 1] == ' ') {
						BitmapScroll->Canvas->Set_Pixels(j, NumLine, PixelBackground);
					}
					else {
						BitmapScroll->Canvas->Set_Pixels(j, NumLine, PixelForeground);
					}
				}
			}
		}

		if (NumLine < FLines->Count - 1) {
			if (bComment != FLines->FlagCommentBegin[NumLine + 1] ||
					bComment2 != FLines->FlagCommentBegin2[NumLine + 1] ||
					bQuote1 != FLines->FlagQuote1Begin[NumLine + 1] ||
					bQuote2 != FLines->FlagQuote2Begin[NumLine + 1]) {
				FLines->Set_FlagCommentBegin(NumLine + 1, bComment);
				FLines->Set_FlagCommentBegin2(NumLine + 1, bComment2);
				FLines->Set_FlagQuote1Begin(NumLine + 1, bQuote1);
				FLines->Set_FlagQuote2Begin(NumLine + 1, bQuote2);
				if (DrawRect.Top + y + tmHeight + tmInternalLeading < DrawRect.Bottom) {
					DrawLine(ACanvas, NumLine + 1);
				}
			}
		}

		if (bCaret) ShowCaret(Handle);

	}
	else {
		bMustRefresh = true;
	}

}

//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::ColorationSyntaxique(int NumLine, AnsiString asLine,
							bool bSelectColor,
							std::vector<COLORSYNTAX> *ColorSyntax,
							bool *bComment, bool *bComment2, bool *bQuote1, bool *bQuote2) {
	COLORSYNTAX Zone;
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;
	// La sélection peut être à cheval sur un mot-clé ou une chaîne entre guillemets,
	// ou bien totalement incluse. D'où les quatres variables ci-dessous qui peuvent
	// ne pas être dans un ordre logique:
	int DerIndex; 						 // Index du dernier changement.
	TYPESYNTAX Type, DerType;
	PARAMLINE ParamLine;
	AnsiString asMot;
	int LenLine;


	LenLine = asLine.Length();

	if (bSelectColor) {
		GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);
	}
	else {
		NumLineBegSel2 = 0;
		NumColBegSel2 = 0;
		NumLineEndSel2 = 0;
		NumColEndSel2 = 0;
	}

	ColorSyntax->clear();
	DerType = tsUndefined;
	DerIndex = 0;

	memset(&ParamLine, 0, sizeof(ParamLine));
	ParamLine.szLine = asLine;
	ParamLine.Index = -1;
	ParamLine.bBeginComm2 = *bComment;
	ParamLine.bBeginComm3 = *bComment2;
	ParamLine.bBeginQuote1 = *bQuote1;
	ParamLine.bBeginQuote2 = *bQuote2;

	while (ParseLine(&ParamLine)) {

		Type = tsNormal;

		if (ParamLine.Index < LenLine) {
			// Caractère sélectionné ?
			if (NumLineBegSel2 <= NumLine && NumLine <= NumLineEndSel2) {
				if (NumLineBegSel2 == NumLineEndSel2) {
					// Sélection mono-ligne
					if (NumColBegSel2 <= ParamLine.Index && ParamLine.Index < NumColEndSel2) Type = tsSelect;
				}
				else {
					// Sélection multi-ligne
					if (bRectSel) {
						// Sélection carrée
						if (NumLineBegSel2 <= NumLine && NumLine <= NumLineEndSel2 &&
								((NumColBegSel2 <= ParamLine.Index && ParamLine.Index < NumColEndSel2) ||
								 (NumColEndSel2 <= ParamLine.Index && ParamLine.Index < NumColBegSel2))) Type = tsSelect;
					}
					else {
						// Sélection normale
						if (NumLineBegSel2 == NumLine && NumColBegSel2 <= ParamLine.Index) Type = tsSelect;
						else if (NumLineBegSel2 < NumLine && NumLine < NumLineEndSel2) Type = tsSelect;
						else if (NumLineEndSel2 == NumLine && ParamLine.Index < NumColEndSel2) Type = tsSelect;
					}
				}
			}
		}

		if (ParamLine.bIsWord) {
			// asMot est un mot
			asMot = asLine.SubString(ParamLine.IndexBeginWord + 1, ParamLine.Index - ParamLine.IndexBeginWord);
			if (FKeywords && FKeywords->IndexOf(asMot) != -1) {
				// asMot est un mot-clé
				if (ParamLine.IndexBeginWord >= DerIndex) {
					if (DerType != tsSelect) {
						// Cas général: mot clé non sélectionné
						if (ParamLine.IndexBeginWord > DerIndex) {
							Zone.IndexChar = DerIndex;
							Zone.Type = DerType;
							ColorSyntax->push_back(Zone);
						}
						DerIndex = ParamLine.IndexBeginWord;
						DerType = tsKeyWord;
					}
					// else mot-clé entièrement sélectionné => il conserve la
					// couleur de sélection et on ne fait rien
				}
				else {
					// Le dernier changement de type (certainement un début de sélection)
					// est intervenu après le début du mot-clé (i.e. le mot-clé est
					// à moitié ou partiellement sélectionné)
					// => Coloration du début (et peut-être de la fin) du mot-clé
					int i = (int) ColorSyntax->size() - 1;
					FVCL_ASSERT(DerIndex > 0);	// Puisque 0 <= IndexBeginWord < DerIndex
					FVCL_ASSERT(i >= 0);	// Puisque DerIndex > 0
					while (i > 0 && (*ColorSyntax)[i].IndexChar > ParamLine.IndexBeginWord) i--;
					if ((*ColorSyntax)[i].IndexChar == ParamLine.IndexBeginWord) {
						// Le début de mot coïncide avec un autre changement
						if ((*ColorSyntax)[i].Type != tsSelect) {
							// Le début de mot coïncide avec un autre changement
							// (certainement une fin de sélection)
							// => On se contente de changer le type:
							(*ColorSyntax)[i].Type = tsKeyWord;
						}
						// else le début de mot-clé coïncide avec le début de sélection
						// => on ne fait rien
					}
					else {
						// Insertion du changement de type à sa place
						if ((i == 0 && i < (int) ColorSyntax->size() - 1) ||
								(*ColorSyntax)[i].Type != tsSelect) {
							Zone.Type = tsKeyWord;
							Zone.IndexChar = ParamLine.IndexBeginWord;
							ColorSyntax->insert(ColorSyntax->begin() + i + 1, Zone);
						}
					}
					// Coloration éventuelle de la fin du mot-clé
					if (Type == tsNormal) {
						if (DerType != tsSelect) DerType = tsKeyWord;
					}
				}
			}
		}
		else if (ParamLine.bBeginWordPreProc) {
			if (Type == tsNormal) Type = tsPreProc;
		}
		else if (ParamLine.bBeginNombreDec || ParamLine.bBeginNombreHex) {
			if (Type == tsNormal) Type = tsNumber;
		}

		// Pas de else: une fin de mot peut aussi être un début de commentaire ou de chaîne
		if (ParamLine.bBeginQuote1 || ParamLine.bBeginQuote2 || ParamLine.bBeginQuote3) {
			if (Type == tsNormal) Type = tsString;
		}
		else if (ParamLine.bBeginComm1 || ParamLine.bBeginComm2 || ParamLine.bBeginComm3) {
			if (Type == tsNormal) Type = tsComment;
		}

		// Mémorisation des changements
		if (Type != DerType) {
			if (ParamLine.Index > DerIndex) {
				Zone.IndexChar = DerIndex;
				Zone.Type = DerType;
				ColorSyntax->push_back(Zone);
			}
			DerType = Type;
			DerIndex = ParamLine.Index;
		}

	}

	if (--ParamLine.Index > DerIndex) {
		Zone.IndexChar = DerIndex;
		Zone.Type = DerType;
		ColorSyntax->push_back(Zone);
	}

	*bComment = ParamLine.bBeginComm2;
	*bComment2 = ParamLine.bBeginComm3;
	*bQuote1 = ParamLine.bBeginQuote1;
	*bQuote2 = ParamLine.bBeginQuote2;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::ParseLine(PARAMLINE *ParamLine, bool bRight) {
	TCHAR ddc, dc, c, pc, ppc;
	int i;


	if (ParamLine->Index < 0) {
		ParamLine->LenLine = lstrlen(ParamLine->szLine);
		if (!bRight) ParamLine->Index = ParamLine->LenLine;
		ParamLine->bBeginWord = false;
		ParamLine->bBeginWordPreProc = false;
		ParamLine->bBeginNombreDec = false;
		ParamLine->bBeginNombreHex = false;
		ParamLine->bBeginQuote3 = false;
		ParamLine->bPossibleGuill3 = false;
		ParamLine->bBeginComm1 = false;
		ParamLine->IndexBeginWord = -1;
		ParamLine->IndexBeginQuote = -1;
		// bBeginComm2, bBeginComm3, bBeginQuote1 et bBeginQuote2 doivent être initialisés avant l'appel
	}

	if (bRight) {
		if (++ParamLine->Index > ParamLine->LenLine) {
			int Len = FMultiLine.Length();
			if (Len <= 0 || ParamLine->LenLine < Len ||
					memcmp((LPCTSTR) FMultiLine, &ParamLine->szLine[ParamLine->LenLine - Len], Len * sizeof(TCHAR))) {
				ParamLine->bBeginQuote1 = false;
				ParamLine->bBeginQuote2 = false;
			}
			return false;
		}
	}
	else {
		if (--ParamLine->Index < 0) return false;
	}
	i = ParamLine->Index;

	if (i >= 2) ddc = ParamLine->szLine[i - 2];
	else ddc = _T('\0');

	if (i >= 1) dc = ParamLine->szLine[i - 1];
	else dc = _T('\0');

	if (i < ParamLine->LenLine) c = ParamLine->szLine[i];
	else c = _T('\0');

	if (i + 1 < ParamLine->LenLine) pc = ParamLine->szLine[i + 1];
	else pc = _T('\0');

	if (i + 2 < ParamLine->LenLine) ppc = ParamLine->szLine[i + 2];
	else ppc = _T('\0');

	ParamLine->bIsWord = false;

	if (ParamLine->bBeginWord) {
		if (!IsAlphaNum(c)) {
			// Fin de mot
			ParamLine->bIsWord = true;
			ParamLine->bBeginWord = false;
		}
	}
	else if (ParamLine->bBeginWordPreProc) {
		if (!IsAlphaNum(c)) {
			ParamLine->bBeginWordPreProc = false;
		}
	}
	else if (ParamLine->bBeginNombreDec) {
		if (!(IsNum(c) || c == _T('.'))) {
			ParamLine->bBeginNombreDec = false;
		}
	}
	else if (ParamLine->bBeginNombreHex) {
		if (!(IsNumHexa(c) || (c == _T('x') && dc == _T('0')))) {
			ParamLine->bBeginNombreHex = false;
		}
	}
	else if (ParamLine->bBeginQuote1) {
		//-------------------------------------------
		// Recherche la fin des guillemets de type 1
		//-------------------------------------------
		if (i != ParamLine->IndexBeginQuote + 1) {	// Sinon le même guillemet est interprété deux fois
			if (dc == FQuoteEnd) {
				bool bEscapeString = false;
				int Len = FEscapeString.Length();
				if (Len > 0 && i > Len &&
						i - Len - 1 != ParamLine->IndexBeginQuote &&
					  !memcmp((LPCTSTR) FEscapeString, &ParamLine->szLine[i - Len - 1], Len * sizeof(TCHAR))) {
					bEscapeString = true;
				}
				if (!bEscapeString) ParamLine->bBeginQuote1 = false;
			}
		}
	}
	else if (ParamLine->bBeginQuote2) {
		//-------------------------------------------
		// Recherche la fin des guillemets de type 2
		//-------------------------------------------
		if (i != ParamLine->IndexBeginQuote + 1) {	// Idem
			if (dc == FQuoteEnd2) {
				bool bEscapeString = false;
				int Len = FEscapeString.Length();
				if (Len > 0 && i > Len &&
						i - Len - 1 != ParamLine->IndexBeginQuote &&
					  !memcmp((LPCTSTR) FEscapeString, &ParamLine->szLine[i - Len - 1], Len * sizeof(TCHAR))) {
					bEscapeString = true;
				}
				if (!bEscapeString) ParamLine->bBeginQuote2 = false;
			}
		}
	}
	else if (ParamLine->bBeginQuote3) {
		//-------------------------------------------
		// Recherche la fin des guillemets de type 3
		//-------------------------------------------
		if (dc == FQuoteEnd3) {
			ParamLine->bBeginQuote3 = false;
		}
	}
	else if (!bRight && ParamLine->bBeginComm1) {
		//----------------------------------------------
		// Recherche du début de commentaires de type 1
		//----------------------------------------------
		if (FindBeginString(ParamLine, FCommentSingle)) {
			ParamLine->bBeginComm1 = false;
		}
	}
	else if (ParamLine->bBeginComm2) {
		if (bRight) {
			//--------------------------------------------
			// Recherche la fin de commentaires de type 2
			//--------------------------------------------
			if (FindEndString(ParamLine, FCommentEnd)) {
				ParamLine->bBeginComm2 = false;
			}
		}
		else {
			//----------------------------------------------
			// Recherche du début de commentaires de type 2
			//----------------------------------------------
			if (FindBeginString(ParamLine, FCommentBegin)) {
				ParamLine->bBeginComm2 = false;
			}
		}
	}
	else if (ParamLine->bBeginComm3) {
		if (bRight) {
			//--------------------------------------------
			// Recherche la fin de commentaires de type 3
			//--------------------------------------------
			if (FindEndString(ParamLine, FCommentEnd2)) {
				ParamLine->bBeginComm3 = false;
			}
		}
		else {
			//----------------------------------------------
			// Recherche du début de commentaires de type 3
			//----------------------------------------------
			if (FindBeginString(ParamLine, FCommentBegin2)) {
				ParamLine->bBeginComm3 = false;
			}
		}
	}

	// Pas de else if (une fin de commentaire ou de guillemet peut être suivi par le début d'un autre).
	if (i < ParamLine->LenLine &&
			!ParamLine->bBeginWord &&
			!ParamLine->bBeginWordPreProc &&
			!ParamLine->bBeginNombreDec &&
			!ParamLine->bBeginNombreHex &&
			!ParamLine->bBeginQuote1 &&
			!ParamLine->bBeginQuote2 &&
			!ParamLine->bBeginQuote3 &&
			!ParamLine->bBeginComm1 &&
			!ParamLine->bBeginComm2 &&
			!ParamLine->bBeginComm3) {

		//----------------------------------------------------------
		// Aucun flag actif: on recherche le début de quelque chose
		//----------------------------------------------------------

		if (FindBeginString(ParamLine, FPreProc)) {
			ParamLine->bBeginWordPreProc = true;
			ParamLine->bPossibleGuill3 = true;
		}
		else if (IsNum(c) || (c == _T('.') && IsNum(pc))) {
			ParamLine->bBeginNombreDec = true;
		}
		else if (c == _T('0') && c == _T('x')) {
			ParamLine->bBeginNombreHex = true;
		}
		else if (c == FQuoteBegin) {
			ParamLine->bBeginQuote1 = true;
			ParamLine->IndexBeginQuote = i;
		}
		else if (c == FQuoteBegin2) {
			ParamLine->bBeginQuote2 = true;
			ParamLine->IndexBeginQuote = i;
		}
		else if (c == FQuoteBegin3 && ParamLine->bPossibleGuill3) {
			ParamLine->bBeginQuote3 = true;
			ParamLine->IndexBeginQuote = i;
		}
		else if (FindBeginString(ParamLine, FCommentSingle)) {
			ParamLine->bBeginComm1 = true;
		}
		else if (i == 0 && FindBeginString(ParamLine, FCommentSingleBeg)) {
			ParamLine->bBeginComm1 = true;
		}
		else if (FindBeginString(ParamLine, FCommentBegin)) {
			ParamLine->bBeginComm2 = true;
		}
		else if (FindBeginString(ParamLine, FCommentBegin2)) {
			ParamLine->bBeginComm3 = true;
		}
		// Les mots-clé en dernier (car un mot-clé peut aussi être un début de commentaire, REM par exemple)
		else if (IsAlpha(c)) {  // Pas IsAlphaNum (un mot ne peut pas commencer par un chiffre)
			ParamLine->bBeginWord = true;
			ParamLine->IndexBeginWord = i;
		}

	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::FindBeginString(PARAMLINE *ParamLine, AnsiString asSearch) {
	int Len;


	Len = asSearch.Length();
	if (Len == 0) return false;
	if (ParamLine->Index < 0) return false;
	if (ParamLine->Index > ParamLine->LenLine - Len) return false;

	return !memcmp(&ParamLine->szLine[ParamLine->Index], (LPCTSTR) asSearch, Len * sizeof(TCHAR));
}

//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::FindEndString(PARAMLINE *ParamLine, AnsiString asSearch) {
	int Len;


	Len = asSearch.Length();
	if (Len == 0) return false;
	if (ParamLine->Index - Len < 0) return false;
	if (ParamLine->Index > ParamLine->LenLine) return false;

	return !memcmp(&ParamLine->szLine[ParamLine->Index - Len], (LPCTSTR) asSearch, Len * sizeof(TCHAR));
}

//---------------------------------------------------------------------------
// Sélection ordonnée
//---------------------------------------------------------------------------
void TTextEditor::GetSelOrdered(int *NumLineBegSel2, int *NumColBegSel2, int *NumLineEndSel2, int *NumColEndSel2) {

	if (NumLineBegSel > NumLineEndSel ||
			(NumLineBegSel == NumLineEndSel && NumColBegSel > NumColEndSel)) {
		*NumLineBegSel2 = NumLineEndSel;
		*NumColBegSel2 = NumColEndSel;
		*NumLineEndSel2 = NumLineBegSel;
		*NumColEndSel2 = NumColBegSel;
	}
	else {
		*NumLineBegSel2 = NumLineBegSel;
		*NumColBegSel2 = NumColBegSel;
		*NumLineEndSel2 = NumLineEndSel;
		*NumColEndSel2 = NumColEndSel;
	}

}

//---------------------------------------------------------------------------
// Teste si le caractère (NumLine, NumCol) est dans la sélection.
//---------------------------------------------------------------------------

bool TTextEditor::IsInSelection(int NumLine, int NumCol) {
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;

	GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);

	if (bRectSel) {

		if (NumLine < NumLineBegSel2 || NumLineEndSel2 < NumLine ||
				NumCol < NumColBegSel2 || NumColEndSel2 <= NumCol) {
			return false;
		}

	}
	else {

		if (NumLine < NumLineBegSel2 ||
				(NumLine == NumLineBegSel2 && NumCol < NumColBegSel2) ||
				(NumLine == NumLineEndSel2 && NumCol >= NumColEndSel2) ||
				NumLine > NumLineEndSel2) {
			return false;
		}

	}

	return true;
}

//---------------------------------------------------------------------------
// Indentation de la sélection
//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::IndentSelection(void) {
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;
	AnsiString asLine;
	AnsiString asIndent;
	int i;


	SetTypeUndoRedo(tuIndent);

	GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);
	if (NumColEndSel2 == 0 && NumLineEndSel2 > NumLineBegSel2) NumLineEndSel2--;

	if (FTabsToSpaces) {
		for (i = 0; i < FNbSpacesTab; i++) asIndent += _T(' ');
	}
	else {
		asIndent = _T('\t');
	}

	for (i = NumLineBegSel2; i <= NumLineEndSel2; i++) {
		asLine = FLines->Strings[i];
		if (!asLine.IsEmpty()) Lines_InsertSubString(i, asIndent, 0);
	}
	if (NumColBegSel != 0) NumColBegSel += FNbSpacesTab;
	if (NumColEndSel != 0) NumColEndSel += FNbSpacesTab;
	MoveCur(NumLineEndSel, NumColEndSel, true, false);

	InvalidateOrSetFlag();

	return true;
}

//---------------------------------------------------------------------------
// Desindentation de la sélection
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::UnindentSelection(void) {
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;
	AnsiString asLine;
	int NbEsp;
	int NbSuppr;
	int Len;
	int i, j;


	SetTypeUndoRedo(tuIndent);

	GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);
	if (NumColEndSel2 == 0 && NumLineEndSel2 > NumLineBegSel2) NumLineEndSel2--;

	for (i = NumLineBegSel2; i <= NumLineEndSel2; i++) {
		asLine = FLines->Strings[i];
		NbEsp = 0;
		NbSuppr = 0;
		Len = asLine.Length();
		for (j = 0; j < FNbSpacesTab; j++) {
			TCHAR c;
			if (Len < 1) c = _T('\0');
			else c = asLine[1];
			if (c != _T(' ') && c != _T('\t')) break;
			asLine = asLine.SubString(2, Len - 1);
			Len--;
			NbEsp++;
			NbSuppr++;
			if (c == _T('\t')) break;
		}
		Lines_DeleteSubString(i, 0, NbSuppr);
		DrawLine(Canvas, i);
	}
	if (NumColBegSel > 2) NumColBegSel -= FNbSpacesTab;
	else NumColBegSel -= 0;
	if (NumColEndSel > 2) NumColEndSel -= FNbSpacesTab;
	else NumColEndSel -= 0;
	MoveCur(NumLineEndSel, NumColEndSel, true, false);

	return true;
}

//---------------------------------------------------------------------------
// Mise en commentaires de la sélection
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::CommentSelection(void) {
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;
	AnsiString asLine;
	int i, j;


	if (NumColBegSel == NumColEndSel && NumLineBegSel == NumLineEndSel) {
		NumLineBegSel2 = NumLineBegSel;
		NumColBegSel2 = 0;
		NumLineEndSel2 = NumLineBegSel + 1;
		NumColEndSel2 = 0;
	}
	else {
		GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);
	}

	if (!FCommentSingle.IsEmpty() &&
			((NumColBegSel2 == 0 && NumColEndSel2 == 0) ||
			 (FCommentBegin.IsEmpty() || FCommentEnd.IsEmpty()))) {

		int LenCommentSingle = FCommentSingle.Length();

		SetTypeUndoRedo(tuComment);

		if (NumLineBegSel2 == NumLineEndSel2) NumLineEndSel2 = NumLineBegSel2 + 1;

		for (i = NumLineBegSel2; i < NumLineEndSel2; i++) {
			asLine = FLines->Strings[i];
			if (!asLine.IsEmpty()) {
				j = 0;
				while (j < asLine.Length() && (asLine[j + 1] == _T(' ') || asLine[j + 1] == _T('\t'))) j++;
				Lines_InsertSubString(i, FCommentSingle, j);
			}
		}
		if (NumLineBegSel == NumLineEndSel) {
			// Ajout de commentaire sans sélection => on décale le curseur
			MoveCur(FNumLine, FNumCol + LenCommentSingle, false, false);
		}
	}
	else if (!FCommentBegin.IsEmpty() && !FCommentEnd.IsEmpty()) {
		int Index;

		int LenCommentBegin = FCommentBegin.Length();
		int LenCommentEnd = FCommentEnd.Length();

		SetTypeUndoRedo(tuComment);

		Index = Lines_Index3(NumLineEndSel2, NumColEndSel2);
		Lines_InsertSubString(NumLineEndSel2, FCommentEnd, Index);

		Index = Lines_Index3(NumLineBegSel2, NumColBegSel2);
		Lines_InsertSubString(NumLineBegSel2, FCommentBegin, Index);

		int NbChar = LenCommentEnd;
		if (NumLineEndSel2 == NumLineBegSel2) NbChar += LenCommentBegin;
		MoveCur(NumLineBegSel2, NumColBegSel2, false, false);
		MoveCur(NumLineEndSel2, NumColEndSel2 + NbChar, true, false);

	}
	else return false;

	InvalidateOrSetFlag();

	return true;
}

//---------------------------------------------------------------------------
// Decommente la sélection
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::UncommentSelection(void) {
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;
	AnsiString asLine;
	int i, j;


	if (NumColBegSel == NumColEndSel && NumLineBegSel == NumLineEndSel) {
		NumLineBegSel2 = NumLineBegSel;
		NumColBegSel2 = 0;
		NumLineEndSel2 = NumLineBegSel + 1;
		NumColEndSel2 = 0;
	}
	else {
		GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);
	}

	if (!FCommentSingle.IsEmpty() &&
			((NumColBegSel2 == 0 && NumColEndSel2 == 0) ||
			 (FCommentBegin.IsEmpty() || FCommentEnd.IsEmpty()))) {

		int LenCommentSingle = FCommentSingle.Length();

		SetTypeUndoRedo(tuComment);

		bool bDone = false;

		if (NumLineBegSel2 == NumLineEndSel2) NumLineEndSel2 = NumLineBegSel2 + 1;

		for (i = NumLineBegSel2; i < NumLineEndSel2; i++) {
			asLine = FLines->Strings[i];
			if (!asLine.IsEmpty()) {
				j = 0;
				while (j < asLine.Length() && (asLine[j + 1] == _T(' ') || asLine[j + 1] == _T('\t'))) j++;
				if (j + LenCommentSingle <= asLine.Length() &&
						asLine.SubString(j + 1, LenCommentSingle) == FCommentSingle) {
					Lines_DeleteSubString(i, j, LenCommentSingle);
					DrawLine(Canvas, i);
					bDone = true;
				}
			}
		}
		if (bDone && NumLineBegSel == NumLineEndSel) {
			// Ajout de commentaire sans sélection => on décale le curseur
			MoveCur(FNumLine, FNumCol - LenCommentSingle, false, false);
		}
	}
	else if (!FCommentBegin.IsEmpty() && !FCommentEnd.IsEmpty()) {
		int Index;
		AnsiString asSelection;

		int LenCommentBegin = FCommentBegin.Length();
		int LenCommentEnd = FCommentEnd.Length();

		asSelection = GetSelText();
		if (asSelection.Length() >= LenCommentBegin + LenCommentEnd &&
				asSelection.SubString(1, LenCommentBegin) == FCommentBegin &&
				asSelection.SubString(asSelection.Length() - LenCommentEnd + 1, LenCommentEnd) == FCommentEnd) {

			SetTypeUndoRedo(tuComment);

			Index = Lines_Index3(NumLineEndSel2, NumColEndSel2);
			Lines_DeleteSubString(NumLineEndSel2, Index - LenCommentEnd, LenCommentEnd);

			Index = Lines_Index3(NumLineBegSel2, NumColBegSel2);
			Lines_DeleteSubString(NumLineBegSel2, Index, LenCommentBegin);

			int NbChar = LenCommentEnd;
			if (NumLineEndSel2 == NumLineBegSel2) NbChar += LenCommentBegin;
			MoveCur(NumLineBegSel2, NumColBegSel2, false, false);
			MoveCur(NumLineEndSel2, NumColEndSel2 - NbChar, true, false);

			// Le refresh est fait par le MoveCur

		}

	}
	else return false;

	return true;
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::ClearSelection(void) {
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;


	if (NumLineBegSel == NumLineEndSel && NumColBegSel == NumColEndSel) return;

	// Si le ClearSelection ne fait pas partie d'une autre opération
	if (CurrentTypeUndo == tuNull || CurrentTypeUndo == tuMoveCur) SetTypeUndoRedo(tuDelete);

	GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);

	if (bRectSel) {
		if (NumColBegSel2 > NumColEndSel2) {
			int Swap;
			Swap = NumColBegSel2;
			NumColBegSel2 = NumColEndSel2;
			NumColEndSel2 = Swap;
		}
	}

	MoveCur(NumLineBegSel2, NumColBegSel2, false, false);

	if (bRectSel) {

		for (int i = NumLineBegSel2; i <= NumLineEndSel2; i++) {
			int Index1 = Lines_Index2(i, NumColBegSel2);
			if (Index1 != -1) {
				int Index2 = Lines_Index3(i, NumColEndSel2);
				Lines_DeleteSubString(i, Index1, Index2 - Index1);
			}
		}

	}

	else {

		Lines_Change(NumLineBegSel2,
				Lines_Left(NumLineBegSel2, NumColBegSel2) +
				Lines_Right(NumLineEndSel2, NumColEndSel2));

		for (int i = NumLineEndSel2; i > NumLineBegSel2; i--) Lines_DeleteLine(i);
		if (FNumLine >= NumLineEndSel2) FNumLine -= NumLineEndSel2 - NumLineBegSel2;

	}

	if (NumLineBegSel2 != NumLineEndSel2) InvalidateOrSetFlag();
	else DrawLine(Canvas, NumLineBegSel);

}

//---------------------------------------------------------------------------
// Couper et copier dans le presse-papier
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::CutToClipboard(void) {
	CopyToClipboard();
	if (!bReadOnly) ClearSelection();

	return true;
}

//---------------------------------------------------------------------------
// Copier dans le presse-papier
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::CopyToClipboard(void) {
	AnsiString asContenu;
	CharString csRTFContenu;
	GLOBALHANDLE hValue;
	int Len;


	asContenu = GetSelText();

	if (!asContenu.IsEmpty()) {
		// Enregistrement dans le presse-papiers
		Clipboard->Open();
		Clipboard->Clear();

		// Format texte
		Clipboard->SetTextBuf(asContenu);

		// Format RTF
		csRTFContenu = GetSelRtf();
		Len = csRTFContenu.Length() + 1;
		hValue = GlobalAlloc(GHND, Len * sizeof(char));
		if (hValue) {
			char *lp;
			lp = (char *) GlobalLock(hValue);
			lstrcpynA(lp, csRTFContenu, Len);
			GlobalUnlock(hValue);
			Clipboard->SetAsHandle(RegisterClipboardFormat(_T("Rich Text Format")), hValue);
		}
		Clipboard->Close();

	}

	return true;
}

//---------------------------------------------------------------------------
// Coller depuis le presse-papier
//---------------------------------------------------------------------------

bool FASTCALL TTextEditor::PasteFromClipboard(void) {
	AnsiString asContenu;


	SetTypeUndoRedo(tuPaste);

	BeginUpdate();

	// Lecture du presse-papiers
	Clipboard->Open();
	asContenu = Clipboard->AsText;
	Clipboard->Close();

	Lines_SetLengthMin(FNumLine, FNumCol); // Espaces entre la fin de ligne et la position du curseur

	// Coller le contenu du presse-papiers
	SetSelTextBuf(asContenu);

	EndUpdate();

	return true;
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::GetPosAt(int Y, int X, int *NumLine, int *NumCol) {
	if (FLinesNumbers) X -= tmAveCharWidth * 6 + 4;
	if (FIconsColumn) X -= 24;
	if (X < 0) X = 0;
	*NumCol = FFirstCol + (int) ((double) X / tmAveCharWidth - .5);
	*NumLine = FFirstLine + (int) ((double) Y / (tmHeight + tmInternalLeading));
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::GetXYOf(int NumLine, int NumCol, int *Y, int *X) {
	*X = (NumCol - FFirstCol) * tmAveCharWidth;
	*Y = (NumLine - FFirstLine) * (tmHeight + tmInternalLeading);
	if (FLinesNumbers) (*X) += tmAveCharWidth * 6 + 4;
	if (FIconsColumn) (*X) += 24;
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::UpdateHScroll(void) {

	if (NbBeginUpdate == 0) {

		TRect DrawRect = ClientRect;
		if (FLinesNumbers) DrawRect.Left = DrawRect.Left + tmAveCharWidth * 6 + 4;  // += ne marche pas sous Code:Blocks
		if (FIconsColumn) DrawRect.Left = DrawRect.Left + 24;
		NbColsWin = (DrawRect.Right - DrawRect.Left) / tmAveCharWidth;

		// Horizontale
		// max(NbCol, FNumCol) parce que le curseur peut être après la dernière colonne
		FHorzScrollBar->Max = /*max*/(NbCol > FNumCol ? NbCol : FNumCol) + 1;	// + 1 pour la place du prochain caractère
		FHorzScrollBar->PageSize = NbColsWin;
		FHorzScrollBar->Position = FFirstCol;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::UpdateVScroll(void) {

	if (NbBeginUpdate == 0) {

		TRect DrawRect = ClientRect;
		NbLinesWin = DrawRect.Height / (tmHeight + tmInternalLeading);

		// Verticale
		FVertScrollBar->Max = FLines->Count;
		FVertScrollBar->PageSize = NbLinesWin + 1;  // + 1 sinon on a un ascenseur qui ne sert à rien
		FVertScrollBar->Position = FFirstLine;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TestNoHScroll(void) {

	if (FirstCol > /*max*/(NbCol > FNumCol ? NbCol : FNumCol) - NbColsWin + 2) {
		FirstCol = /*max*/(NbCol > FNumCol ? NbCol : FNumCol) - NbColsWin + 2;
	}
	if (FirstCol < 0) {	// Pas "else if" (NewFirstCol a pu devenir négatif dans le test précédent)
		FirstCol = 0;
	}


}

//---------------------------------------------------------------------------
void FASTCALL TTextEditor::TestNoVScroll(void) {

	if (FirstLine > FLines->Count - NbLinesWin) {
		FirstLine = FLines->Count - NbLinesWin;
	}
	if (FirstLine < 0) {  // Pas "else if" (NewFirstLine a pu devenir négatif dans le test précédent)
		FirstLine = 0;
	}

}

//---------------------------------------------------------------------------
// Comptage du nombre d'indentations à appliquer après la ligne asLine
//---------------------------------------------------------------------------
int FASTCALL TTextEditor::NbIndentAfterLine(AnsiString asLine) {
	int NumCol;
	int Indent;
	int Len1, Len2;
	int i, j;


	// Comptage de l'indentation du début de la ligne
	NumCol = 0;
	for (i = 0; i < asLine.Length(); i++) {
		if (asLine[i + 1] == _T(' ')) NumCol++;
		else if (asLine[i + 1] == _T('\t')) NumCol = (NumCol / FNbSpacesTab + 1) * FNbSpacesTab;
		else break;
	}
	Indent = NumCol / FNbSpacesTab;
	// Comptage de l'indentation à ajouter ou à enlever
	Len1 = asLine.Length();
	Len2 = FBlockBegin.Length();
	for (j = i; j < Len1 - Len2 + 1; j++) {
		if (asLine.SubString(j + 1, Len2) == FBlockBegin) Indent++;
	}
	Len2 = FBlockEnd.Length();
	for (j = i; j < Len1 - Len2 + 1; j++) {
		if (asLine.SubString(j + 1, Len2) == FBlockEnd) Indent--;
	}
	// Si asLine contient juste un bloc de fin, la désindentation a déjà été faite sur cette ligne
	if (asLine.Trim() == FBlockEnd) Indent++;

	return Indent;
}

//---------------------------------------------------------------------------
bool TTextEditor::IsAlpha(TCHAR c) {
	return (FAlphaChars.find_first_of(c) != std::string::npos);
}

//---------------------------------------------------------------------------
bool TTextEditor::IsNum(TCHAR c) {
	return (_T('0') <= c && c <= _T('9'));
}

//---------------------------------------------------------------------------
bool TTextEditor::IsNumHexa(TCHAR c) {
	return (_T('0') <= c && c <= _T('9') ||
					_T('a') <= c && c <= _T('f') ||
					_T('A') <= c && c <= _T('F'));
}

//---------------------------------------------------------------------------
bool TTextEditor::IsAlphaNum(TCHAR c) {
	return (IsAlpha(c) || IsNum(c));
}

//---------------------------------------------------------------------------
bool TTextEditor::IsBlanc(TCHAR c) {
	return (c == _T(' ') || c == _T('\t'));
}

//---------------------------------------------------------------------------
AnsiString TTextEditor::Lines_Strings(int NumLine) {
	AnsiString asLine;
	int LenLine;
	AnsiString asRetour;
	int i;
	int Len;
	TCHAR c;


	if (NumLine >= FLines->Count) return _T(""); 	// Dans le cas d'un nouveau fichier: NumLine = 0 et Count = 0

	asLine = FLines->Strings[NumLine];
	LenLine = asLine.Length();
	Len = 0;
	for (i = 1; i <= LenLine; i++) {
		c = asLine[i];
		if (c == _T('\t')) {
			do {
				asRetour += _T(' ');
				Len++;
			} while ((Len % FNbSpacesTab) != 0);
		}
		else {
			asRetour += c;
			Len++;
		}
	}

	return asRetour;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_NumCol(int NumLine, int Index) {
	AnsiString asLine;
	int i;
	int NumCol;
	int LenLine;


	if (NumLine >= FLines->Count) return 0;

	asLine = FLines->Strings[NumLine];
	LenLine = asLine.Length();
	if (Index > LenLine) return -1;

	NumCol = 0;
	for (i = 0; i < Index; i++) {
		if (i < LenLine && asLine[i + 1] == _T('\t')) {
			NumCol = (NumCol / FNbSpacesTab + 1) * FNbSpacesTab;
		}
		else NumCol++;
	}

	return NumCol;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_Index(int NumLine, int NumCol, int *NumColDebChar) {
	AnsiString asLine;
	int LenLine;
	int i;
	int NumCol2, DerNumCol;


	if (NumLine >= FLines->Count) return -1;

	asLine = FLines->Strings[NumLine];
	LenLine = asLine.Length();

	if (NumCol == 0) {
		if (NumColDebChar) *NumColDebChar = 0;
		if (LenLine == 0) return -1;
		return 0;
	}

	NumCol2 = 0;
	DerNumCol = NumCol2;
	for (i = 0; i < LenLine; i++) {
		if (NumCol2 > NumCol) {
			if (NumColDebChar) *NumColDebChar = DerNumCol;
			return i - 1;
		}
		else if (NumCol2 == NumCol) {
			if (NumColDebChar) *NumColDebChar = NumCol2;
			return i;
		}
		DerNumCol = NumCol2;
		if (asLine[i + 1] == _T('\t')) {
			NumCol2 = (NumCol2 / FNbSpacesTab + 1) * FNbSpacesTab;
		}
		else NumCol2++;
	}

	if (NumColDebChar) *NumColDebChar = NumCol2;

	return -1;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_Index2(int NumLine, int NumCol) {
	AnsiString asLine;
	int LenLine;
	int i;
	int NumCol2, DerNumCol;


	if (NumLine >= FLines->Count) return -1;

	asLine = FLines->Strings[NumLine];
	LenLine = asLine.Length();

	NumCol2 = 0;
	DerNumCol = NumCol2;
	for (i = 0; i <= LenLine; i++) {
		if (NumCol2 > NumCol) {
			return i - 1;
		}
		else if (NumCol2 == NumCol) {
			return i;
		}
		DerNumCol = NumCol2;
		if (i < LenLine && asLine[i + 1] == _T('\t')) {
			NumCol2 = (NumCol2 / FNbSpacesTab + 1) * FNbSpacesTab;
		}
		else NumCol2++;
	}

	return -1;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_Index3(int NumLine, int NumCol) {
	AnsiString asLine;
	int LenLine;
	int i;
	int NumCol2, DerNumCol;


	if (NumLine >= FLines->Count) return -1;

	asLine = FLines->Strings[NumLine];
	LenLine = asLine.Length();

	NumCol2 = 0;
	DerNumCol = NumCol2;
	for (i = 0; i < LenLine; i++) {
		if (NumCol2 > NumCol) {
			return i - 1;
		}
		else if (NumCol2 == NumCol) {
			return i;
		}
		DerNumCol = NumCol2;
		if (i < LenLine && asLine[i + 1] == _T('\t')) {
			NumCol2 = (NumCol2 / FNbSpacesTab + 1) * FNbSpacesTab;
		}
		else NumCol2++;
	}

	return i;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_SetLengthMin(int NumLine, int LengthMin) {

	// Dans le cas d'un nouveau fichier: NumLine = 0 et Count = 0
	while (NumLine >= FLines->Count) Lines_InsertLine(FLines->Count, _T(""));

	int LineLength = Lines_Length(NumLine);

	if (LineLength < LengthMin) {

		AnsiString asSpaces;
		while (LineLength < LengthMin) {
			if (FSpacesToTabs && LineLength + FNbSpacesTab < LengthMin) {
				asSpaces += _T("\t");
				LineLength = (LineLength / FNbSpacesTab + 1) * FNbSpacesTab;
			}
			else {
				asSpaces += _T(" ");
				LineLength++;
			}
		}

		AnsiString asLine;
		asLine = FLines->Strings[NumLine];
		Lines_InsertSubString(NumLine, asSpaces, asLine.Length());

	}

	return LineLength;
}

//---------------------------------------------------------------------------
bool TTextEditor::CalculNbCol(void) {
	int i;
	int Len;
	int OldNbCol;


	OldNbCol = NbCol;
	NbCol = 0;
	for (i = 0; i < FLines->Count; i++) {
		Len = Lines_Length(i);
		if (NbCol < Len) NbCol = Len;
	}
	if (NbCol != OldNbCol) UpdateHScroll();

	return true;
}

//---------------------------------------------------------------------------
TCHAR TTextEditor::Lines_Char(int NumLine, int NumCol) {
	AnsiString asLine;
	int Index;

	if (NumLine >= FLines->Count) return _T('\0');

	Index = Lines_Index(NumLine, NumCol);
	if (Index == -1) return _T('\0');

	asLine = FLines->Strings[NumLine];

	return asLine[Index + 1];
}

//---------------------------------------------------------------------------
AnsiString TTextEditor::Lines_Left(int NumLine, int NumCol) {
	AnsiString asLine;
	int Index;

	if (NumLine >= FLines->Count) return _T("");

	Index = Lines_Index(NumLine, NumCol);
	if (Index == -1) return FLines->Strings[NumLine];

	asLine = FLines->Strings[NumLine];
	return asLine.SubString(1, Index);
}

//---------------------------------------------------------------------------
AnsiString TTextEditor::Lines_Right(int NumLine, int NumCol) {
	AnsiString asLine;
	int Index;

	if (NumLine >= FLines->Count) return _T("");

	Index = Lines_Index(NumLine, NumCol);
	if (Index == -1) return _T("");

	asLine = FLines->Strings[NumLine];
	return asLine.SubString(Index + 1, asLine.Length() - Index);
}

//---------------------------------------------------------------------------
AnsiString TTextEditor::Lines_SubString(int NumLine, int NumCol, int Count) {
	AnsiString asLine;
	int Index1, Index2;


	if (NumLine >= FLines->Count) return _T("");

	asLine = FLines->Strings[NumLine];
	Index1 = Lines_Index(NumLine, NumCol);
	if (Index1 == -1) return _T("");
	else {
		Index2 = Lines_Index(NumLine, NumCol + Count);
		if (Index2 == -1) Index2 = asLine.Length();
		return asLine.SubString(Index1 + 1, Index2 - Index1);
	}
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_Length(int NumLine) {
	AnsiString asLine;
	int LenLine;
	int i;
	int Len;


	if (NumLine >= FLines->Count) return 0; 	// Dans le cas d'un nouveau fichier: NumLine = 0 et Count = 0

	asLine = FLines->Strings[NumLine];
	LenLine = asLine.Length();
	Len = 0;
	for (i = 1; i <= LenLine; i++) {
		if (asLine[i] == _T('\t')) Len = (Len / FNbSpacesTab + 1) * FNbSpacesTab;
		else Len++;
	}

	return Len;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_Change(int NumLine, AnsiString str) {
	AnsiString asLine;
	LPUNDOCHANGE lpUndoChange;
	int SizeBuffer;
	int StrLength;
	int LineLength;
	bool bCalculNbCol = false;


	if (NumLine >= FLines->Count) return 0;

	StrLength = str.Length();

	asLine = FLines->Strings[NumLine];

	if (CurrentTypeUndo != tuNull) {

		// Mémorisation du Undo
		SizeBuffer = sizeof(UNDOCHANGE) + asLine.Length() * sizeof(TCHAR);
		lpUndoChange = (LPUNDOCHANGE) new BYTE[SizeBuffer];
		lpUndoChange->ActionUndo = auChange;
		lpUndoChange->NumLine = NumLine;
		lstrcpy(lpUndoChange->szText, asLine);
		UndoRedo->MemoUndo(this, lpUndoChange, SizeBuffer);
		delete[] lpUndoChange;

		// Mémorisation du Redo
		SizeBuffer = sizeof(UNDOCHANGE) + StrLength * sizeof(TCHAR);
		lpUndoChange = (LPUNDOCHANGE) new BYTE[SizeBuffer];
		lpUndoChange->ActionUndo = auChange;
		lpUndoChange->NumLine = NumLine;
		lstrcpy(lpUndoChange->szText, str);
		UndoRedo->MemoRedo(this, lpUndoChange, SizeBuffer);
		delete[] lpUndoChange;

	}

	if (Lines_Length(NumLine) == NbCol) bCalculNbCol = true;

	bNoEvent = true;
	FLines->Set_Strings(NumLine, str);
	bNoEvent = false;

	if (bCalculNbCol) CalculNbCol();
	else {
		LineLength = Lines_Length(NumLine);
		if (NbCol < LineLength) {
			NbCol = LineLength;
			UpdateHScroll();
		}
	}
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

	return StrLength;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_DeleteSubString(int NumLine, int Index, int Count) {
	AnsiString asLine;
	UNDODELETESUBSTRING UndoDeleteSubString;
	LPUNDOINSERTSUBSTRING lpUndoInsertSubString;
	int SizeBuffer;
	bool bCalculNbCol = false;


	if (NumLine >= FLines->Count) return 0;

	asLine = FLines->Strings[NumLine];

	if (CurrentTypeUndo != tuNull) {

		// Mémorisation du Undo
		SizeBuffer = sizeof(UNDOINSERTSUBSTRING) + Count * sizeof(TCHAR);
		lpUndoInsertSubString = (LPUNDOINSERTSUBSTRING) new BYTE[SizeBuffer];
		lpUndoInsertSubString->ActionUndo = auInsertSubString;
		lpUndoInsertSubString->NumLine = NumLine;
		lpUndoInsertSubString->Index = Index;
		lstrcpy(lpUndoInsertSubString->szText, asLine.SubString(Index + 1, Count));
		UndoRedo->MemoUndo(this, lpUndoInsertSubString, SizeBuffer);
		delete[] lpUndoInsertSubString;

		// Mémorisation du Redo
		UndoDeleteSubString.ActionUndo = auDeleteSubString;
		UndoDeleteSubString.NumLine = NumLine;
		UndoDeleteSubString.Index = Index;
		UndoDeleteSubString.Count = Count;
		UndoRedo->MemoRedo(this, &UndoDeleteSubString, sizeof(UNDODELETESUBSTRING));

	}

	if (Lines_Length(NumLine) == NbCol) bCalculNbCol = true;

	asLine = asLine.Delete(Index + 1, Count);
	bNoEvent = true;
	FLines->Set_Strings(NumLine, asLine);
	bNoEvent = false;

	UpdateVScroll();
	if (bCalculNbCol) CalculNbCol();
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

	return Index;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_InsertSubString(int NumLine, AnsiString str, int Index) {
	AnsiString asLine;
	UNDODELETESUBSTRING UndoDeleteSubString;
	LPUNDOINSERTSUBSTRING lpUndoInsertSubString;
	int SizeBuffer;
	int StrLength;
	int LineLength;


	if (NumLine >= FLines->Count) return 0;

	StrLength = str.Length();

	if (CurrentTypeUndo != tuNull) {

		// Mémorisation du Undo
		UndoDeleteSubString.ActionUndo = auDeleteSubString;
		UndoDeleteSubString.NumLine = NumLine;
		UndoDeleteSubString.Index = Index;
		UndoDeleteSubString.Count = StrLength;
		UndoRedo->MemoUndo(this, &UndoDeleteSubString, sizeof(UNDODELETESUBSTRING));

		// Mémorisation du Redo
		SizeBuffer = sizeof(UNDOINSERTSUBSTRING) + StrLength * sizeof(TCHAR);
		lpUndoInsertSubString = (LPUNDOINSERTSUBSTRING) new BYTE[SizeBuffer];
		lpUndoInsertSubString->ActionUndo = auInsertSubString;
		lpUndoInsertSubString->NumLine = NumLine;
		lpUndoInsertSubString->Index = Index;
		lstrcpy(lpUndoInsertSubString->szText, str);
		UndoRedo->MemoRedo(this, lpUndoInsertSubString, SizeBuffer);
		delete[] lpUndoInsertSubString;

	}

	asLine = FLines->Strings[NumLine];
	asLine = asLine.Insert(str, Index + 1);
	bNoEvent = true;
	FLines->Set_Strings(NumLine, asLine);
	bNoEvent = false;
	Index += StrLength;

	UpdateVScroll();
	LineLength = Lines_Length(NumLine);
	if (NbCol < LineLength) {
		NbCol = LineLength;
		UpdateHScroll();
	}
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

	return Index;
}

//---------------------------------------------------------------------------
bool TTextEditor::Lines_Split(int NumLine, int Index) {
	UNDOSPLIT UndoSplit;
	UNDOMERGE UndoMerge;
	bool bCalculNbCol = false;


	if (NumLine >= FLines->Count) return false;

	if (CurrentTypeUndo != tuNull) {

		// Mémorisation du Undo
		UndoMerge.ActionUndo = auMerge;
		UndoMerge.NumLine = NumLine;
		UndoRedo->MemoUndo(this, &UndoMerge, sizeof(UNDOMERGE));

		// Mémorisation du Redo
		UndoSplit.ActionUndo = auSplit;
		UndoSplit.NumLine = NumLine;
		UndoSplit.Index = Index;
		UndoRedo->MemoRedo(this, &UndoSplit, sizeof(UNDOSPLIT));

	}

	if (Lines_Length(NumLine) == NbCol) bCalculNbCol = true;

	AnsiString asLine = FLines->Strings[NumLine];
	bNoEvent = true;
	FLines->Insert(NumLine + 1, asLine.SubString(Index + 1, asLine.Length() - Index));
	FLines->Set_Strings(NumLine, asLine.SubString(1, Index));
	bNoEvent = false;

	UpdateVScroll();
	if (bCalculNbCol) CalculNbCol();
	InvalidateOrSetFlag();
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

	return true;
}
//---------------------------------------------------------------------------
bool TTextEditor::Lines_Merge(int NumLine) {
	UNDOSPLIT UndoSplit;
	UNDOMERGE UndoMerge;
	int LineLength;


	if (NumLine >= FLines->Count - 1) return false;

	if (CurrentTypeUndo != tuNull) {

		// Mémorisation du Undo
		UndoSplit.ActionUndo = auSplit;
		UndoSplit.NumLine = NumLine;
		UndoSplit.Index = Lines_Length(NumLine);
		UndoRedo->MemoUndo(this, &UndoSplit, sizeof(UNDOSPLIT));

		// Mémorisation du Redo
		UndoMerge.ActionUndo = auMerge;
		UndoMerge.NumLine = NumLine;
		UndoRedo->MemoRedo(this, &UndoMerge, sizeof(UNDOMERGE));

	}

	bNoEvent = true;
	FLines->Set_Strings(NumLine,
			FLines->Strings[NumLine] +
			FLines->Strings[NumLine + 1]);
	FLines->Delete(NumLine + 1);
	bNoEvent = false;
	if (FNumLine >= NumLine + 1) FNumLine--;

	UpdateVScroll();
	LineLength = Lines_Length(NumLine);
	if (NbCol < LineLength) {
		NbCol = LineLength;
		UpdateHScroll();
	}
	InvalidateOrSetFlag();
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

	return true;
}

//---------------------------------------------------------------------------
bool TTextEditor::Lines_DeleteLine(int NumLine) {
	AnsiString asLine;
	UNDODELETELINE UndoDeleteLine;
	LPUNDOINSERTLINE lpUndoInsertLine;
	int SizeBuffer;
	bool bCalculNbCol = false;


	if (NumLine >= FLines->Count) return false;

	asLine = FLines->Strings[NumLine];

	if (CurrentTypeUndo != tuNull) {

		// Mémorisation du Undo
		SizeBuffer = sizeof(UNDOINSERTLINE) + asLine.Length() * sizeof(TCHAR);
		lpUndoInsertLine = (LPUNDOINSERTLINE) new BYTE[SizeBuffer];
		lpUndoInsertLine->ActionUndo = auInsertLine;
		lpUndoInsertLine->NumLine = NumLine;
		lstrcpy(lpUndoInsertLine->szText, asLine);
		UndoRedo->MemoUndo(this, lpUndoInsertLine, SizeBuffer);
		delete[] lpUndoInsertLine;

		// Mémorisation du Redo
		UndoDeleteLine.ActionUndo = auDeleteLine;
		UndoDeleteLine.NumLine = NumLine;
		UndoRedo->MemoRedo(this, &UndoDeleteLine, sizeof(UNDODELETELINE));

	}

	if (Lines_Length(NumLine) == NbCol) bCalculNbCol = true;

	bNoEvent = true;
	FLines->Delete(NumLine);
	bNoEvent = false;

	UpdateVScroll();
	if (bCalculNbCol) CalculNbCol();
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

	return true;
}

//---------------------------------------------------------------------------
int TTextEditor::Lines_InsertLine(int NumLine, AnsiString str) {
	AnsiString asLine;
	UNDODELETELINE UndoDeleteLine;
	LPUNDOINSERTLINE lpUndoInsertLine;
	int SizeBuffer;
	int StrLength;
	int LineLength;


	// Dans le cas d'un nouveau fichier: NumLine = 0 et Count = 0
	while (NumLine > FLines->Count) Lines_InsertLine(FLines->Count, _T(""));

	StrLength = str.Length();

	if (CurrentTypeUndo != tuNull) {

		// Mémorisation du Undo
		UndoDeleteLine.ActionUndo = auDeleteLine;
		UndoDeleteLine.NumLine = NumLine;
		UndoRedo->MemoUndo(this, &UndoDeleteLine, sizeof(UNDODELETELINE));

		// Mémorisation du Redo
		SizeBuffer = sizeof(UNDOINSERTLINE) + StrLength * sizeof(TCHAR);
		lpUndoInsertLine = (LPUNDOINSERTLINE) new BYTE[SizeBuffer];
		lpUndoInsertLine->ActionUndo = auInsertLine;
		lpUndoInsertLine->NumLine = NumLine;
		lstrcpy(lpUndoInsertLine->szText, str);
		UndoRedo->MemoRedo(this, lpUndoInsertLine, SizeBuffer);
		delete[] lpUndoInsertLine;

	}

	bNoEvent = true;
	FLines->Insert(NumLine, str);
	bNoEvent = false;

	UpdateVScroll();
	LineLength = Lines_Length(NumLine);
	if (NbCol < LineLength) {
		NbCol = LineLength;
		UpdateHScroll();
	}
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

	return StrLength;
}

//---------------------------------------------------------------------------
bool TTextEditor::Lines_SetLines(TStrings *NewLines) {
	AnsiString asOldLinesBuf;
	AnsiString asNewLinesBuf;
	int SizeOldLinesBuf;
	int SizeNewLinesBuf;
	int SizeBuffer;
	LPUNDOSETLINES lpUndoSetLines;

	if (FLines != NewLines) {
		asOldLinesBuf = FLines->GetTextStr();
		asNewLinesBuf = NewLines->GetTextStr();
		SizeOldLinesBuf = asOldLinesBuf.Length();
		SizeNewLinesBuf = asNewLinesBuf.Length();

		if (CurrentTypeUndo != tuNull) {

			// Mémorisation du Undo
			SizeBuffer = sizeof(UNDOSETLINES) + asOldLinesBuf.Length() * sizeof(TCHAR);
			lpUndoSetLines = (LPUNDOSETLINES) new BYTE[SizeBuffer];
			lpUndoSetLines->ActionUndo = auSetLines;
			lstrcpy(lpUndoSetLines->szText, asOldLinesBuf);
			UndoRedo->MemoUndo(this, lpUndoSetLines, SizeBuffer);
			delete[] lpUndoSetLines;

			// Mémorisation du Redo
			SizeBuffer = sizeof(UNDOSETLINES) + asNewLinesBuf.Length() * sizeof(TCHAR);
			lpUndoSetLines = (LPUNDOSETLINES) new BYTE[SizeBuffer];
			lpUndoSetLines->ActionUndo = auSetLines;
			lstrcpy(lpUndoSetLines->szText, asNewLinesBuf);
			UndoRedo->MemoRedo(this, lpUndoSetLines, SizeBuffer);
			delete[] lpUndoSetLines;

		}

		bNoEvent = true;
		Set_Lines(NewLines);
		bNoEvent = false;

		UpdateVScroll();
		CalculNbCol();
		if (!FChanged) {
			FChanged = true;
			OnFlagChanged(this, FChanged);
		}
		OnChange(this);

	}

	return true;
}

//---------------------------------------------------------------------------
bool TTextEditor::GetFlag(int NumLine, TEditorLinesFlag Flag) {
	TEditorLinesFlags Flags;

	if (NumLine >= FLines->Count) return false;

	Flags = FLines->Flags[NumLine];
	return Flags.Contains(Flag);
}

//---------------------------------------------------------------------------
bool TTextEditor::SetFlag(int NumLine, TEditorLinesFlag Flag, bool bSet) {
	TEditorLinesFlags Flags;
	TEditorLinesFlags NewFlags;


	if (NumLine >= FLines->Count) return false;

	Flags = FLines->Flags[NumLine];
	NewFlags = Flags;
	if (bSet) {
		NewFlags << Flag;
	}
	else {
		NewFlags >> Flag;
	}
	if (NewFlags != Flags) {
		FLines->Flags[NumLine] = NewFlags;
		DrawLine(Canvas, NumLine);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool TTextEditor::ClearFlag(TEditorLinesFlag Flag) {
	int i;
	TEditorLinesFlags Flags;

	for (i = 0; i < FLines->Count; i++) {
		Flags = FLines->Flags[i];
		if (Flags.Contains(Flag)) {
			Flags >> Flag;
			FLines->Flags[i] = Flags;
			InvalidateOrSetFlag();
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Undo/redo
//---------------------------------------------------------------------------
bool TTextEditor::ProcessUndo(void * Undo, int LenUndo) {
	LPUNDOMOVECUR lpUndoMoveCur;
	LPUNDOCHANGE lpUndoChange;
	LPUNDODELETESUBSTRING lpUndoDeleteSubString;
	LPUNDOINSERTSUBSTRING lpUndoInsertSubString;
	LPUNDOSPLIT lpUndoSplit;
	LPUNDOMERGE lpUndoMerge;
	LPUNDODELETELINE lpUndoDeleteLine;
	LPUNDOINSERTLINE lpUndoInsertLine;
	LPUNDOSETLINES lpUndoSetLines;


	ACTIONUNDO ActionUndo = *(ACTIONUNDO *) Undo;

	switch (ActionUndo) {

		case auMoveCur:
			lpUndoMoveCur = (LPUNDOMOVECUR) Undo;
			FFirstCol = lpUndoMoveCur->FirstCol;
			FFirstLine = lpUndoMoveCur->FirstLine;
			FNumCol = lpUndoMoveCur->NumCol;
			FNumLine = lpUndoMoveCur->NumLine;
			NumColBegSel = lpUndoMoveCur->NumColBegSel;
			NumLineBegSel = lpUndoMoveCur->NumLineBegSel;
			NumColEndSel = lpUndoMoveCur->NumColEndSel;
			NumLineEndSel = lpUndoMoveCur->NumLineEndSel;
			bRectSel = lpUndoMoveCur->bRectSel;
			InvalidateOrSetFlag();
			SetCurrentCaretPos();
			OnCaretMove(this, FNumCol + 1, FNumLine + 1);
			return true;

		case auChange:
			lpUndoChange = (LPUNDOCHANGE) Undo;
			Lines_Change(lpUndoChange->NumLine, lpUndoChange->szText);
			InvalidateOrSetFlag();
			return true;

		case auDeleteSubString:
			lpUndoDeleteSubString = (LPUNDODELETESUBSTRING) Undo;
			Lines_DeleteSubString(lpUndoDeleteSubString->NumLine, lpUndoDeleteSubString->Index, lpUndoDeleteSubString->Count);
			InvalidateOrSetFlag();
			return true;

		case auInsertSubString:
			lpUndoInsertSubString = (LPUNDOINSERTSUBSTRING) Undo;
			Lines_InsertSubString(lpUndoInsertSubString->NumLine, lpUndoInsertSubString->szText, lpUndoInsertSubString->Index);
			InvalidateOrSetFlag();
			return true;

		case auSplit:
			lpUndoSplit = (LPUNDOSPLIT) Undo;
			Lines_Split(lpUndoSplit->NumLine, lpUndoSplit->Index);
			InvalidateOrSetFlag();
			return true;

		case auMerge:
			lpUndoMerge = (LPUNDOMERGE) Undo;
			Lines_Merge(lpUndoMerge->NumLine);
			InvalidateOrSetFlag();
			return true;

		case auDeleteLine:
			lpUndoDeleteLine = (LPUNDODELETELINE) Undo;
			Lines_DeleteLine(lpUndoDeleteLine->NumLine);
			InvalidateOrSetFlag();
			return true;

		case auInsertLine:
			lpUndoInsertLine = (LPUNDOINSERTLINE) Undo;
			Lines_InsertLine(lpUndoInsertLine->NumLine, lpUndoInsertLine->szText);
			InvalidateOrSetFlag();
			return true;

		case auSetLines:
			lpUndoSetLines = (LPUNDOSETLINES) Undo;
			FLines->SetText(lpUndoSetLines->szText);
			UpdateVScroll();
			CalculNbCol();
			if (!FChanged) {
				FChanged = true;
				OnFlagChanged(this, FChanged);
			}
			OnChange(this);
			return true;

	}

	return false;
}

bool TTextEditor::ProcessRedo(void * Redo, int LenRedo) {
	// Les commandes undo et redo sont les mêmes
	return ProcessUndo(Redo, LenRedo);
}

//---------------------------------------------------------------------------
bool TTextEditor::SetTypeUndoRedo(TYPEUNDO NewTypeUndo, AnsiString asOperationName) {
	UNDOMOVECUR UndoMoveCur;


	// Flag pour pouvoir encapsuler des opérations.
	if (bNoEndOperation) return true;

	if (!FUndoRedoMoveCur) {
		if (NewTypeUndo == tuMoveCur) NewTypeUndo = tuNull;
	}

	if (NewTypeUndo != CurrentTypeUndo) {

		if (CurrentTypeUndo != tuNull) {

			// Mémorisation du Redo
			UndoMoveCur.ActionUndo = auMoveCur;
			UndoMoveCur.FirstCol = FFirstCol;
			UndoMoveCur.FirstLine = FFirstLine;
			UndoMoveCur.NumCol = FNumCol;
			UndoMoveCur.NumLine = FNumLine;
			UndoMoveCur.NumColBegSel = NumColBegSel;
			UndoMoveCur.NumLineBegSel = NumLineBegSel;
			UndoMoveCur.NumColEndSel = NumColEndSel;
			UndoMoveCur.NumLineEndSel = NumLineEndSel;
			UndoMoveCur.bRectSel = bRectSel;
			UndoRedo->MemoRedo(this, &UndoMoveCur, sizeof(UNDOMOVECUR));

			switch (CurrentTypeUndo) {

				case tuMoveCur:
					break;

				case tuIndent:
					break;

				case tuComment:
					break;

				case tuText:
					break;

				case tuDelete:
					break;

				case tuInsert:
					break;

				case tuPaste:
					break;

				case tuReplace:
					break;

				case tuExterne:
					break;

				default:
					FVCL_ASSERT(false && _T("TypeUndo inconnu"));
					break;

			}

			UndoRedo->EndOperation();
			CurrentTypeUndo = tuNull;

		}

		if (NewTypeUndo != tuNull) {

			switch (NewTypeUndo) {
				case tuMoveCur:
					UndoRedo->BeginOperation(UR_MOVECUR, _TT("Déplacement curseur"));
					break;

				case tuIndent:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Indentation / désindentation"));
					break;

				case tuComment:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Ajout/suppression des commentaires"));
					break;

				case tuText:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Saisie texte"));
					break;

				case tuDelete:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Suppression de texte"));
					break;

				case tuInsert:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Insertion de texte"));
					break;

				case tuPaste:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Insertion de données du presse-papier"));
					break;

				case tuReplace:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Chercher / remplacer"));
					break;

				case tuExterne:
					UndoRedo->BeginOperation(UR_MODIF, asOperationName);
					break;

				default:
					FVCL_ASSERT(false && _T("TypeUndo inconnu"));
					break;

			}

			// Mémorisation du Undo
			UndoMoveCur.ActionUndo = auMoveCur;
			UndoMoveCur.FirstCol = FFirstCol;
			UndoMoveCur.FirstLine = FFirstLine;
			UndoMoveCur.NumCol = FNumCol;
			UndoMoveCur.NumLine = FNumLine;
			UndoMoveCur.NumColBegSel = NumColBegSel;
			UndoMoveCur.NumLineBegSel = NumLineBegSel;
			UndoMoveCur.NumColEndSel = NumColEndSel;
			UndoMoveCur.NumLineEndSel = NumLineEndSel;
			UndoMoveCur.bRectSel = bRectSel;
			UndoRedo->MemoUndo(this, &UndoMoveCur, sizeof(UNDOMOVECUR));

			CurrentTypeUndo = NewTypeUndo;

		}

	}

	return true;
}


//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::BeginOperation(AnsiString asOperationName) {

	SetTypeUndoRedo(tuExterne, asOperationName);
	bNoEndOperation = true;

	return true;
}


//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::EndOperation(void) {

	bNoEndOperation = false;
	SetTypeUndoRedo(tuNull);

	return true;
}


//---------------------------------------------------------------------------
// Effectue une opération undo
//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::Undo(void) {
	AnsiString asOperation;
	bool bRet;

	// Interruption d'une éventuelle opération en cours
	SetTypeUndoRedo(tuNull);

	BeginUpdate();
	bRet = UndoRedo->Undo(&asOperation);
	EndUpdate();
	OnMessage(this, asOperation);
	if (UndoRedo->IsEmpty(UR_MODIF)) {
		if (FChanged) {
			FChanged = false;
			OnFlagChanged(this, FChanged);
		}
	}

	return bRet;
}

//---------------------------------------------------------------------------
// Effectue une opération redo
//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::Redo(void) {
	AnsiString asOperation;
	bool bRet;

	// Interruption d'une éventuelle opération en cours
	SetTypeUndoRedo(tuNull);

	BeginUpdate();
	bRet = UndoRedo->Redo(&asOperation);
	EndUpdate();
	OnMessage(this, asOperation);

	return bRet;
}

//---------------------------------------------------------------------------
// Chercher un texte
//
// !!!!! ATTENTION !!!!!!!
// Fonction fragile: Toute modif doit faire l'objet de tests complets.
// TESTS:
//    Recherche vers le bas et vers le haut
//    Recherche vers le bas et vers le haut d'une chaîne inexistante
//    Pareil lorsque le curseur est après la fin de ligne
//    Pareil lorsque le curseur est sur une ligne vide
//    Recherche vers le haut et vers le bas du premier caractère d'une ligne
//    Recherche vers le haut et vers le bas du dernier caractère d'une ligne
//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::Search(AnsiString asSearch, LPSEARCHPARAM lpSearchParam) {
	AnsiString asLine;
	int i0, j0, i, j, k;
	int LenSearch, LenFound;
	PARAMLINE ParamLine;
	bool bFound = false;
	bool bExitLoop;
	bool bEndFilePassed;
	TRegExp *RegExp = NULL;


	if (FLines->Count == 0) return false;

	LenSearch = asSearch.Length();
	LenFound = LenSearch;

	if (lpSearchParam->RegExp) {
		RegExp = new TRegExp(asSearch);
	}

	if (lpSearchParam->SinceCursor) {
		if (lpSearchParam->Down) {
			i = FNumCol;
			j = FNumLine;
		}
		else {
			i = FNumCol - LenSearch - 1;
			j = FNumLine;
		}
	}
	else {
		if (lpSearchParam->Down) {
			i = 0;
			j = 0;
		}
		else {
			j = FLines->Count - 1;
			FVCL_ASSERT(j >= 0 && _T("Recherche sur fichier vide"));
			asLine = Lines_Strings(j);
			i = asLine.Length() - 1;
		}
	}

	i0 = i;
	j0 = j;
	// Le curseur peut se trouver après la fin de ligne
	asLine = Lines_Strings(j0);
	if (i0 > asLine.Length()) i0 = asLine.Length();
	if (i0 < 0) i0 = 0;

	bExitLoop = false;
	bEndFilePassed = false;
	asLine = Lines_Strings(j);
	do {

		memset(&ParamLine, 0, sizeof(ParamLine));

		//------------------------------
		// Ligne suivante ou précédente
		//------------------------------

		if (lpSearchParam->Down) {

			while (i >= asLine.Length() && !bExitLoop) {
				if (++j >= FLines->Count) {
					if (!lpSearchParam->Loop) {
						bExitLoop = true;
						break;
					}
					j = 0;
					bEndFilePassed = true;
					OnPassEnd(this);
				}
				asLine = Lines_Strings(j);
				i = 0;
				if (i == i0 && j == j0) {
					bExitLoop = true;
					break;
				}
			}
		}
		else {
			while (i < 0 && !bExitLoop) {
				if (--j < 0) {
					if (!lpSearchParam->Loop) {
						bExitLoop = true;
						break;
					}
					j = FLines->Count - 1;
					bEndFilePassed = true;
					OnPassBegin(this);
				}
				asLine = Lines_Strings(j);
				i = asLine.Length() - 1;
				if (i <= i0 && j == j0) {
					bExitLoop = true;
					break;
				}
			}
		}
		if (bExitLoop) break;

		//--------------------------
		// Recherche dans une ligne
		//--------------------------

		if (lpSearchParam->RegExp) {
			int IndexBeg, IndexEnd;

			// Recherche avec expression régulière
			IndexBeg = i;
			if (RegExp->Search(asLine, lpSearchParam->CaseSensitive, &IndexBeg, &IndexEnd)) {
				i = IndexBeg;
				LenFound = IndexEnd - IndexBeg;
				bFound = true;
				break;
			}

			i = asLine.Length();  // Pour passage à la ligne suivante
			if (j == j0 && bEndFilePassed) bExitLoop = true;  // Fin de recherche

		}
		else {

			// Recherche d'une chaîne simple
			ParamLine.szLine = asLine;
			ParamLine.Index = -1;
			ParamLine.bBeginComm2 = FLines->FlagCommentBegin[j];
			ParamLine.bBeginComm3 = FLines->FlagCommentBegin2[j];
			ParamLine.bBeginQuote1 = FLines->FlagQuote1Begin[j];
			ParamLine.bBeginQuote2 = FLines->FlagQuote2Begin[j];

			// Aller à la position du curseur s'il s'agit de la première ligne de recherche
			while (ParamLine.Index < i) ParseLine(&ParamLine, true);

			while (!bExitLoop) {

				// Test des critères de recherche "Dans/hors des guillemets" et "Dans/hors des commentaires"

				bool bSearch = true;

				if (lpSearchParam->NotInComments &&
						(ParamLine.bBeginComm1 || ParamLine.bBeginComm2 || ParamLine.bBeginComm3)){
					bSearch = false;
				}
				else if (lpSearchParam->OnlyInComments &&
								 !(ParamLine.bBeginComm1 || ParamLine.bBeginComm2 || ParamLine.bBeginComm3)) {
					bSearch = false;
				}

				if (lpSearchParam->NotInQuotes && ParamLine.bBeginQuote1) {
					bSearch = false;
				}
				else if (lpSearchParam->OnlyInQuotes && !ParamLine.bBeginQuote1) {
					bSearch = false;
				}

				if (bSearch) {

					// Test égalité de la chaîne
					const TCHAR *szString = &asLine.c_str()[i];
					if (lpSearchParam->CaseSensitive) {
						bFound = !memcmp(szString, (LPCTSTR) asSearch, LenSearch * sizeof(TCHAR));
					}
					else {
						bFound = !_memicmp(szString, (LPCTSTR) asSearch, LenSearch * sizeof(TCHAR));
					}

					if (bFound) {

						// Test mot entier
						if (lpSearchParam->WholeWord) {
							if (i >= 1) {
								TCHAR dc = asLine[i];
								if (IsAlphaNum(dc)) bFound = false;
							}
							if (i + LenSearch < asLine.Length()) {
								TCHAR pc = asLine[i + LenSearch + 1];
								if (IsAlphaNum(pc)) bFound = false;
							}
						}

						// Test début de ligne
						if (lpSearchParam->BegLine) {
							for (k = 0; k < i; k++) {
								TCHAR pc = asLine[k + 1];
								if (pc != _T(' ') && pc != _T('\t')) {
									bFound = false;
									break;
								}
							}
						}

						// Test fin de ligne
						if (lpSearchParam->EndLine) {
							for (k = i + LenSearch; k < asLine.Length(); k++) {
								TCHAR pc = asLine[k + 1];
								if (pc != _T(' ') && pc != _T('\t')) {
									bFound = false;
									break;
								}
							}
						}

					}

					if (bFound) break;

				}

				bool bRet = ParseLine(&ParamLine, lpSearchParam->Down);
				i = ParamLine.Index;
				if (!bRet) break;  // Fin de ligne

				if (i == i0 && j == j0 && bEndFilePassed) bExitLoop = true;  // Fin de recherche

			}

		}

	} while (!bFound && !bExitLoop);

	if (bFound) {
		SetTypeUndoRedo(tuMoveCur);
		MoveCur(j, i, false, false);
		MoveCur(j, i + LenFound, true, false);
	}

	delete RegExp;

	return bFound;
}

//---------------------------------------------------------------------------
// Remplacer un texte
//---------------------------------------------------------------------------
bool FASTCALL TTextEditor::Replace(AnsiString asReplace, bool bKeepCase) {
	int NumLineBegSel2;
	int NumColBegSel2;
	int NumLineEndSel2;
	int NumColEndSel2;


	BeginUpdate();

	SetTypeUndoRedo(tuReplace);

	GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);
	if (bKeepCase) {
		int i;
		bool bUpper;
		AnsiString asSearch = GetSelText();
		AnsiString asSearchUpper = asSearch.UpperCase();
		AnsiString asReplaceUpper = asReplace.UpperCase();
		AnsiString asReplaceLower = asReplace.LowerCase();
		asReplace = "";
		bUpper = false;
		for (i = 1; i <= asReplaceUpper.Length(); i++) {
			if (i <= asSearch.Length()) {
				bUpper = (asSearch[i] == asSearchUpper[i]);
			}
			if (bUpper) asReplace += asReplaceUpper[i];
			else asReplace += asReplaceLower[i];
		}
	}
	SetSelTextBuf((LPCTSTR) asReplace);
	NumLineBegSel = NumLineBegSel2;
	NumColBegSel = NumColBegSel2;
	NumLineEndSel = FNumLine;
	NumColEndSel = FNumCol;

	SetTypeUndoRedo(tuNull);

	EndUpdate();

	return true;
}

//---------------------------------------------------------------------------
// Efface tout le texte du contrôle de saisie.
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::Clear(void) {
	FLines->Clear();
}

//---------------------------------------------------------------------------
// Copie le texte sélectionné dans une chaîne au format texte brut.
//---------------------------------------------------------------------------

AnsiString FASTCALL TTextEditor::GetSelText(void) {
	AnsiString asSelText;


	if (NumLineBegSel != NumLineEndSel || NumColBegSel != NumColEndSel) {

		// Ok, il y a une sélection

		int NumLineBegSel2;
		int NumColBegSel2;
		int NumLineEndSel2;
		int NumColEndSel2;

		GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);

		if (bRectSel) {
			if (NumColBegSel2 > NumColEndSel2) {
				int Swap;
				Swap = NumColBegSel2;
				NumColBegSel2 = NumColEndSel2;
				NumColEndSel2 = Swap;
			}
		}

		// Lecture de la sélection => asSelText

		for (int i = NumLineBegSel2; i <= NumLineEndSel2; i++) {

			if (bRectSel) {

				// asSelText += FLines->Strings[i].SubString(NumColBegSel2 + 1, NumColEndSel2 - NumColBegSel2);
				asSelText += Lines_SubString(i, NumColBegSel2, NumColEndSel2 - NumColBegSel2);
				if (i != NumLineEndSel2) asSelText += _T("\n");

			}

			else {

				if (NumLineBegSel2 == NumLineEndSel2) {
					// asSelText = FLines->Strings[i].SubString(NumColBegSel2 + 1, NumColEndSel2 - NumColBegSel2);
					asSelText = Lines_SubString(i, NumColBegSel2, NumColEndSel2 - NumColBegSel2);
				}
				else {
					if (i == NumLineBegSel2) {
						// Le caret (donc le début de la sélection) peut être après la fin de la ligne
						int Len = FLines->Strings[i].Length();
						if (NumColBegSel2 > Len) NumColBegSel2 = Len;
						// asSelText = FLines->Strings[i].SubString(NumColBegSel2 + 1, Len - NumColBegSel2);
						asSelText = Lines_Right(i, NumColBegSel2);
						asSelText += _T("\r\n");
					}
					else if (i == NumLineEndSel2) {
						// Le caret (donc la fin de la sélection) peut être après la fin de la ligne
						int Len = FLines->Strings[i].Length();
						if (NumColEndSel2 > Len) NumColEndSel2 = Len;
						// asSelText += FLines->Strings[i].SubString(1, NumColEndSel2);
						asSelText += Lines_Left(i, NumColEndSel2);
					}
					else {
						asSelText += FLines->Strings[i];
						asSelText += _T("\r\n");
					}
				}

			}

		}
	}

	return asSelText;
}

//---------------------------------------------------------------------------
// Copie le texte sélectionné au format RTF.
//---------------------------------------------------------------------------

CharString FASTCALL TTextEditor::GetSelRtf(void) {
	CharString csSelText;
	CharString csSubString;
	CharString csBuf;
	int NumLine;
	int i, j;
	char c;
	int IndexChar, NextIndexChar;
	int NbChar;
	std::vector<COLORSYNTAX> ColorSyntax;
	AnsiString asLine;
	int LenLine;
	bool bComment;
	bool bComment2;
	bool bQuote1;
	bool bQuote2;


	if (NumLineBegSel != NumLineEndSel || NumColBegSel != NumColEndSel) {

		// Ok, il y a une sélection

		int NumLineBegSel2;
		int NumColBegSel2;
		int NumLineEndSel2;
		int NumColEndSel2;

		GetSelOrdered(&NumLineBegSel2, &NumColBegSel2, &NumLineEndSel2, &NumColEndSel2);

		if (bRectSel) {
			if (NumColBegSel2 > NumColEndSel2) {
				int Swap;
				Swap = NumColBegSel2;
				NumColBegSel2 = NumColEndSel2;
				NumColEndSel2 = Swap;
			}
		}

		// Entête du fichier RTF
		csSelText += "{\\rtf1\\ansi ";

		// Table des fontes
		csSelText += "{\\fonttbl {\\f0\\fmodern\\fprq1\\fcharset0 Courier New;}}";

		// Table des couleurs
		TColor TextColor;
		csSelText += "{\\colortbl ";
		for (i = MIN_TYPESYNTAX; i <= MAX_TYPESYNTAX; i++) {
			TextColor = FColorParams[i + 1].Foreground;
			csBuf.sprintf("\\red%i \\green%i \\blue%i; ", TextColor.RValue(), TextColor.GValue(), TextColor.BValue());
			csSelText += csBuf;
		}
		csSelText += "}";

		// Lecture de la sélection => csSelText

		csBuf.sprintf("{\\f0\\fs%i ", abs((int) FCanvas->Font->Height));
		csSelText += csBuf;

		bComment = false;
		bComment2 = false;
		bQuote1 = false;
		bQuote2 = false;

		for (NumLine = NumLineBegSel2; NumLine <= NumLineEndSel2; NumLine++) {

			bComment2 = false;
			asLine = Lines_Strings(NumLine);
			LenLine = asLine.Length();
			ColorationSyntaxique(NumLine, asLine, false, &ColorSyntax, &bComment, &bComment2, &bQuote1, &bQuote2);

			for (i = 0; i < (int) ColorSyntax.size(); i++) {

				IndexChar = ColorSyntax[i].IndexChar;
				if (i < (int) ColorSyntax.size() - 1) NextIndexChar = ColorSyntax[i + 1].IndexChar;
				else NextIndexChar = LenLine;
				if (NumLine == NumLineBegSel2 || bRectSel) {
					if (IndexChar < NumColBegSel2) IndexChar = NumColBegSel2;
				}
				if (NumLine == NumLineEndSel2 || bRectSel) {
					if (NextIndexChar > NumColEndSel2) NextIndexChar = NumColEndSel2;
				}
				NbChar = NextIndexChar - IndexChar;

				if (NbChar > 0) {

					csBuf.sprintf("{\\cf%i ", ColorSyntax[i].Type);
					if (FColorParams[ColorSyntax[i].Type].bBold) csBuf += "\\b ";
					if (FColorParams[ColorSyntax[i].Type].bItalic) csBuf += "\\i ";
					csSelText += csBuf;

					csSubString = asLine.SubString(IndexChar + 1, NbChar).AsCharString();
					for (j = 1; j <= csSubString.Length(); j++) {
						c = csSubString[j];
						if (c == '\t') {
							csSelText += "\tab ";
						}
						else if (c == '{') {
							csSelText += "\\{";
						}
						else if (c == '\\') {
							csSelText += "\\\\";
						}
						else if (c == (char) 0xA0) {  // Espace insécable
							csSelText += "\\~";
						}
						else if (c == ':') {
							csSelText += "\\:";
						}
						else if (c < ' ') {
							csBuf.sprintf("\\'%02x", (int) (BYTE) c);
							csSelText += csBuf;
						}
						else {
							csSelText += c;
						}
					}

					csSelText += "}";
				}

			}

			csSelText += "\\par ";

		}

		// Fin du texte de sélection
		csSelText += "}";

		// Fin du fichier RTF
		csSelText += "}";

	}

	return csSelText;
}

//---------------------------------------------------------------------------
// Copie le texte sélectionné dans un tampon.
//---------------------------------------------------------------------------

int FASTCALL TTextEditor::GetSelTextBuf(TCHAR * Buffer, int BufSize) {
	AnsiString asBuf = GetSelText();
	return lstrlen(lstrcpyn(Buffer, asBuf, BufSize));
}

//---------------------------------------------------------------------------
// Sélectionne tout le texte.
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::SelectAll(void) {
	int NumLine, NumCol;

	NumLine = FLines->Count - 1;
	NumCol = Lines_Length(NumLine);

	SetTypeUndoRedo(tuMoveCur);
	MoveCur(0, 0, false, false);
	MoveCur(NumLine, NumCol, true, false);
}


//---------------------------------------------------------------------------
// Remplace le texte sélectionné par une chaîne à zéro terminal.
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::SetSelTextBuf(const TCHAR * Buffer) {
	int i;
	int Deb;	// Début de ligne
	TCHAR c;
	bool bMultiLigne = false;
	int NbCR = 0;
	int NbLF = 0;
	int Index, NewNumLine;
	AnsiString asLine;


	// Algorithme un peu compliqué parce qu'il faut spliter la ligne AVANT
	// d'insérer une ligne de la sélection, pour éviter de rechercher sans
	// arrêt la longueur de la plus grande ligne du fichier, dans le cas où
	// toutes les lignes ont sensiblement la même longueur (et donc où une
	// ligne deux fois plus grande aurait toutes les chances de devenir la
	// plus grande, entrainant un réétalonnage automatique de la scroll-bar).

	// Effacement de la sélection éventuelle
	ClearSelection();

	Index = Lines_Index3(FNumLine, FNumCol);
	NewNumLine = FNumLine;
	Deb = 0;
	i = 0;

	do {

		c = Buffer[i++];
		if (c == _T('\r') || c == _T('\n') || c == _T('\0')) {
			if (i - Deb - 1 > 0 || c == _T('\0')) {
				if (NbCR != 0) {
					while (NbCR) {
						Lines_Split(NewNumLine, Index);
						if (!asLine.IsEmpty()) {
							Lines_InsertSubString(NewNumLine, asLine, Index);
							asLine = _T("");
						}
						NewNumLine++;
						Index = 0;
						bMultiLigne = true;
						NbCR--;
					}
					NbLF = 0;
				}
				else if (NbLF != 0) {
					while (NbLF) {
						if (!asLine.IsEmpty()) {
							Lines_InsertSubString(NewNumLine, asLine, Index);
							asLine = _T("");
						}
						NewNumLine++;
						Index = Lines_Index2(NewNumLine, FNumCol);
						if (Index == -1) {
							Lines_SetLengthMin(NewNumLine, FNumCol);
							Index = Lines_Index2(NewNumLine, FNumCol);
						}
						bMultiLigne = true;
						NbLF--;
					}
				}
				if (i - Deb - 1 > 0) {
					asLine = AnsiString(&Buffer[Deb], i - Deb - 1);
				}
			}
			if (c == _T('\n')) NbLF++;
			else if (c == _T('\r')) NbCR++;
			Deb = i;
		}

	} while (c);

	if (!asLine.IsEmpty()) {
		Index = Lines_InsertSubString(NewNumLine, asLine, Index);
		asLine = _T("");
	}

	if (bMultiLigne) InvalidateOrSetFlag();
	else DrawLine(Canvas, FNumLine);

	int NewNumCol = Lines_NumCol(NewNumLine, Index);
	MoveCur(NewNumLine, NewNumCol, false, false);

}


//---------------------------------------------------------------------------
// Début d'une série de modifications (pas de raffraichissement).
//---------------------------------------------------------------------------
void FASTCALL TTextEditor::BeginUpdate(void) {
	if (NbBeginUpdate++ == 0) bMustRefresh = false;
}

//---------------------------------------------------------------------------
// Fin d'une série de modifications (raffraichissement éventuel).
//---------------------------------------------------------------------------
void FASTCALL TTextEditor::EndUpdate(void) {
	if (--NbBeginUpdate == 0) {
		if (bMustRefresh) {
			ScrollInView(FNumLine, FNumCol);
			InvalidateOrSetFlag();
			UpdateHScroll();
			TestNoVScroll();
			UpdateVScroll();
			TestNoHScroll();
		}
	}
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnResize
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::ProcessResize(void) {
	UpdateVScroll();
	TestNoVScroll();
	UpdateHScroll();
	TestNoHScroll();
}

//---------------------------------------------------------------------------
// Déclenchement de l'autocomplétion
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::AutocompletionOn(int NumCompletion) {
	int X, Y;
	AnsiString asParent;
	int LenAutoComp;

	AutocompletionOff();
	GetXYOf(FNumLine + 1, FNumCol + 1, &Y, &X);
	if (NumCompletion == 1) LenAutoComp = FAutocompletion1.Length();
	else if (NumCompletion == 2) LenAutoComp = FAutocompletion2.Length();
	else if (NumCompletion == 3) LenAutoComp = FAutocompletion3.Length();
	else LenAutoComp = 0;
	if (LenAutoComp <= FNumCol) {
		WordFromPos(FNumLine, FNumCol - LenAutoComp, &asParent);
	}
	OnAutocompletionOn(this, asParent, X, Y, NumCompletion);
	bAutocompletionOn = true;
}

//---------------------------------------------------------------------------
// Arrêt de l'autocomplétion
//---------------------------------------------------------------------------

void FASTCALL TTextEditor::AutocompletionOff(void) {
	if (bAutocompletionOn) {
		OnAutocompletionOff(this);
		bAutocompletionOn = false;
	}
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TTextEditor::GetListProperties(TStrings *ListProperties) {

	TCustomEditor::GetListProperties(ListProperties);

	ListProperties->Add(_T("BigEndian"));
	ListProperties->Add(_T("ByteOrderMark"));
	ListProperties->Add(_T("Encodage"));
	ListProperties->Add(_T("LinuxEndLine"));
	ListProperties->Add(_T("FirstLine"));
	ListProperties->Add(_T("FirstCol"));
	ListProperties->Add(_T("NumLine"));
	ListProperties->Add(_T("NumCol"));
	ListProperties->Add(_T("Keywords"));
	ListProperties->Add(_T("Lines"));
	ListProperties->Add(_T("LinesNumbers"));
	ListProperties->Add(_T("IconsColumn"));
	ListProperties->Add(_T("NbSpacesTab"));
	ListProperties->Add(_T("SpacesToTabs"));
	ListProperties->Add(_T("TabsToSpaces"));
	ListProperties->Add(_T("SelText"));
	ListProperties->Add(_T("TypeAuto"));
	ListProperties->Add(_T("IndentAuto"));
	ListProperties->Add(_T("CurAfterEndLine"));
	ListProperties->Add(_T("UndoRedoMoveCur"));
	ListProperties->Add(_T("AlphaChars"));
	ListProperties->Add(_T("Autocompletion"));
	ListProperties->Add(_T("Autocompletion1"));
	ListProperties->Add(_T("Autocompletion2"));
	ListProperties->Add(_T("Autocompletion3"));
	ListProperties->Add(_T("CaseSensitive"));
	ListProperties->Add(_T("CommentSingle"));
	ListProperties->Add(_T("CommentSingleBeg"));
	ListProperties->Add(_T("CommentBegin"));
	ListProperties->Add(_T("CommentEnd"));
	ListProperties->Add(_T("CommentBegin2"));
	ListProperties->Add(_T("CommentEnd2"));
	ListProperties->Add(_T("BlockBegin"));
	ListProperties->Add(_T("BlockEnd"));
	ListProperties->Add(_T("PreProc"));
	ListProperties->Add(_T("QuoteBegin"));
	ListProperties->Add(_T("QuoteEnd"));
	ListProperties->Add(_T("QuoteBegin2"));
	ListProperties->Add(_T("QuoteEnd2"));
	ListProperties->Add(_T("QuoteBegin3"));
	ListProperties->Add(_T("QuoteEnd3"));
	ListProperties->Add(_T("EscapeString"));
	ListProperties->Add(_T("MultiLine"));
	ListProperties->Add(_T("Associated1"));
	ListProperties->Add(_T("Associated2"));
	ListProperties->Add(_T("Tooltip"));
	ListProperties->Add(_T("OnCaretMove"));
	ListProperties->Add(_T("OnAutocompletionOn"));
	ListProperties->Add(_T("OnAutocompletionOff"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TTextEditor::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("BigEndian")) {
		return tpBool;
	}
	if (asProperty == _T("ByteOrderMark")) {
		return tpBool;
	}
	if (asProperty == _T("Encodage")) {
    *asInfos = _T("ceASCII\nceUtf8\nceUNICODE");
		return tpChoice;
	}
	if (asProperty == _T("LinuxEndLine")) {
		return tpBool;
	}
	if (asProperty == _T("FirstLine")) {
		return tpNumber;
	}
	if (asProperty == _T("FirstCol")) {
		return tpNumber;
	}
	if (asProperty == _T("NumLine")) {
		return tpNumber;
	}
	if (asProperty == _T("NumCol")) {
		return tpNumber;
	}
	if (asProperty == _T("Keywords")) {
		return tpObject;
	}
	if (asProperty == _T("Lines")) {
		return tpObject;
	}
	if (asProperty == _T("LinesNumbers")) {
		return tpBool;
	}
	if (asProperty == _T("IconsColumn")) {
		return tpBool;
	}
	if (asProperty == _T("NbSpacesTab")) {
		return tpNumber;
	}
	if (asProperty == _T("SpacesToTabs")) {
		return tpBool;
	}
	if (asProperty == _T("TabsToSpaces")) {
		return tpBool;
	}
	if (asProperty == _T("SelText")) {
		return tpText;
	}
	if (asProperty == _T("TypeAuto")) {
		return tpBool;
	}
	if (asProperty == _T("IndentAuto")) {
		return tpBool;
	}
	if (asProperty == _T("CurAfterEndLine")) {
		return tpBool;
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		return tpBool;
	}
	if (asProperty == _T("AlphaChars")) {
		return tpText;
	}
	if (asProperty == _T("Autocompletion")) {
		return tpBool;
	}
	if (asProperty == _T("Autocompletion1")) {
		return tpText;
	}
	if (asProperty == _T("Autocompletion2")) {
		return tpText;
	}
	if (asProperty == _T("Autocompletion3")) {
		return tpText;
	}
	if (asProperty == _T("CaseSensitive")) {
		return tpBool;
	}
	if (asProperty == _T("CommentSingle")) {
		return tpText;
	}
	if (asProperty == _T("CommentSingleBeg")) {
		return tpText;
	}
	if (asProperty == _T("CommentBegin")) {
		return tpText;
	}
	if (asProperty == _T("CommentEnd")) {
		return tpText;
	}
	if (asProperty == _T("CommentBegin2")) {
		return tpText;
	}
	if (asProperty == _T("CommentEnd2")) {
		return tpText;
	}
	if (asProperty == _T("BlockBegin")) {
		return tpText;
	}
	if (asProperty == _T("BlockEnd")) {
		return tpText;
	}
	if (asProperty == _T("PreProc")) {
		return tpText;
	}
	if (asProperty == _T("QuoteBegin")) {
		return tpChar;
	}
	if (asProperty == _T("QuoteEnd")) {
		return tpChar;
	}
	if (asProperty == _T("QuoteBegin2")) {
		return tpChar;
	}
	if (asProperty == _T("QuoteEnd2")) {
		return tpChar;
	}
	if (asProperty == _T("QuoteBegin3")) {
		return tpChar;
	}
	if (asProperty == _T("QuoteEnd3")) {
		return tpChar;
	}
	if (asProperty == _T("EscapeString")) {
		return tpText;
	}
	if (asProperty == _T("MultiLine")) {
		return tpText;
	}
	if (asProperty == _T("Associated1")) {
		return tpText;
	}
	if (asProperty == _T("Associated2")) {
		return tpText;
	}
	if (asProperty == _T("Tooltip")) {
		return tpObject;
	}
	if (asProperty == _T("OnCaretMove")) {
    *asInfos = _T("(TObject *Sender, int NumCol, int NumLine)");
		return tpEvent;
	}
	if (asProperty == _T("OnAutocompletionOn")) {
    *asInfos = _T("(TObject *Sender, AnsiString asParent, int X, int Y, int Type)");
		return tpEvent;
	}
	if (asProperty == _T("OnAutocompletionOff")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	return TCustomEditor::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TTextEditor::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("BigEndian")) {
		return _T("False");
	}
	if (asProperty == _T("ByteOrderMark")) {
		return _T("False");
	}
	if (asProperty == _T("Encodage")) {
		return _T("ceASCII");
	}
	if (asProperty == _T("LinuxEndLine")) {
		return _T("False");
	}
	if (asProperty == _T("FirstLine")) {
		return _T("0");
	}
	if (asProperty == _T("FirstCol")) {
		return _T("0");
	}
	if (asProperty == _T("NumLine")) {
		return _T("0");
	}
	if (asProperty == _T("NumCol")) {
		return _T("0");
	}
	if (asProperty == _T("Keywords")) {
		return _T("");
	}
	if (asProperty == _T("Lines")) {
		return _T("");
	}
	if (asProperty == _T("LinesNumbers")) {
		return _T("True");
	}
	if (asProperty == _T("IconsColumn")) {
		return _T("True");
	}
	if (asProperty == _T("NbSpacesTab")) {
		return _T("3");
	}
	if (asProperty == _T("SpacesToTabs")) {
		return _T("True");
	}
	if (asProperty == _T("TabsToSpaces")) {
		return _T("False");
	}
	if (asProperty == _T("SelText")) {
		return _T("");
	}
	if (asProperty == _T("TypeAuto")) {
		return _T("True");
	}
	if (asProperty == _T("IndentAuto")) {
		return _T("True");
	}
	if (asProperty == _T("CurAfterEndLine")) {
		return _T("True");
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		return _T("True");
	}
	if (asProperty == _T("AlphaChars")) {
		return _T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	}
	if (asProperty == _T("Autocompletion")) {
		return _T("False");
	}
	if (asProperty == _T("Autocompletion1")) {
		return _T("");
	}
	if (asProperty == _T("Autocompletion2")) {
		return _T("");
	}
	if (asProperty == _T("Autocompletion3")) {
		return _T("");
	}
	if (asProperty == _T("CaseSensitive")) {
		return _T("True");
	}
	if (asProperty == _T("CommentSingle")) {
		return _T("");
	}
	if (asProperty == _T("CommentSingleBeg")) {
		return _T("");
	}
	if (asProperty == _T("CommentBegin")) {
		return _T("");
	}
	if (asProperty == _T("CommentEnd")) {
		return _T("");
	}
	if (asProperty == _T("CommentBegin2")) {
		return _T("");
	}
	if (asProperty == _T("CommentEnd2")) {
		return _T("");
	}
	if (asProperty == _T("BlockBegin")) {
		return _T("");
	}
	if (asProperty == _T("BlockEnd")) {
		return _T("");
	}
	if (asProperty == _T("PreProc")) {
		return _T("");
	}
	if (asProperty == _T("QuoteBegin")) {
		return _T("");
	}
	if (asProperty == _T("QuoteEnd")) {
		return _T("");
	}
	if (asProperty == _T("QuoteBegin2")) {
		return _T("");
	}
	if (asProperty == _T("QuoteEnd2")) {
		return _T("");
	}
	if (asProperty == _T("QuoteBegin3")) {
		return _T("");
	}
	if (asProperty == _T("QuoteEnd3")) {
		return _T("");
	}
	if (asProperty == _T("EscapeString")) {
		return _T("");
	}
	if (asProperty == _T("MultiLine")) {
		return _T("");
	}
	if (asProperty == _T("Associated1")) {
		return _T("");
	}
	if (asProperty == _T("Associated2")) {
		return _T("");
	}
	if (asProperty == _T("Tooltip")) {
		return _T("");
	}
	if (asProperty == _T("OnCaretMove")) {
		return _T("");
	}
	if (asProperty == _T("OnAutocompletionOn")) {
		return _T("");
	}
	if (asProperty == _T("OnAutocompletionOff")) {
		return _T("");
	}
	return TCustomEditor::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TTextEditor::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("BigEndian")) {
		if (Get_BigEndian()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("ByteOrderMark")) {
		if (Get_ByteOrderMark()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("Encodage")) {
    CHARENCODE Encodage = Get_Encodage();
    if (Encodage == ceASCII) return _T("ceASCII");
    if (Encodage == ceUtf8) return _T("ceUtf8");
    if (Encodage == ceUNICODE) return _T("ceUNICODE");
    return _T("");
	}
	if (asProperty == _T("LinuxEndLine")) {
		if (Get_LinuxEndLine()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("FirstLine")) {
		return IntToStr(Get_FirstLine());
	}
	if (asProperty == _T("FirstCol")) {
		return IntToStr(Get_FirstCol());
	}
	if (asProperty == _T("NumLine")) {
		return IntToStr(Get_NumLine());
	}
	if (asProperty == _T("NumCol")) {
		return IntToStr(Get_NumCol());
	}
	if (asProperty == _T("Keywords")) {
		if (FKeywords) {
			return FKeywords->GetTextStr();
		}
		return "";
	}
	if (asProperty == _T("Lines")) {
		return FLines->GetTextStr();
	}
	if (asProperty == _T("LinesNumbers")) {
		if (Get_LinesNumbers()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("IconsColumn")) {
		if (Get_IconsColumn()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("NbSpacesTab")) {
		return IntToStr(Get_NbSpacesTab());
	}
	if (asProperty == _T("SpacesToTabs")) {
		if (Get_SpacesToTabs()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("TabsToSpaces")) {
		if (Get_TabsToSpaces()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("SelText")) {
		return Get_SelText();
	}
	if (asProperty == _T("TypeAuto")) {
		if (Get_TypeAuto()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("IndentAuto")) {
		if (Get_IndentAuto()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("CurAfterEndLine")) {
		if (Get_CurAfterEndLine()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		if (Get_UndoRedoMoveCur()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("AlphaChars")) {
		return Get_AlphaChars();
	}
	if (asProperty == _T("Autocompletion")) {
		if (Get_Autocompletion()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("Autocompletion1")) {
		return Get_Autocompletion1();
	}
	if (asProperty == _T("Autocompletion2")) {
		return Get_Autocompletion2();
	}
	if (asProperty == _T("Autocompletion3")) {
		return Get_Autocompletion3();
	}
	if (asProperty == _T("CaseSensitive")) {
		return Get_CaseSensitive();
	}
	if (asProperty == _T("CommentSingle")) {
		return Get_CommentSingle();
	}
	if (asProperty == _T("CommentSingleBeg")) {
		return Get_CommentSingleBeg();
	}
	if (asProperty == _T("CommentBegin")) {
		return Get_CommentBegin();
	}
	if (asProperty == _T("CommentEnd")) {
		return Get_CommentEnd();
	}
	if (asProperty == _T("CommentBegin2")) {
		return Get_CommentBegin2();
	}
	if (asProperty == _T("CommentEnd2")) {
		return Get_CommentEnd2();
	}
	if (asProperty == _T("BlockBegin")) {
		return Get_BlockBegin();
	}
	if (asProperty == _T("BlockEnd")) {
		return Get_BlockEnd();
	}
	if (asProperty == _T("PreProc")) {
		return Get_PreProc();
	}
	if (asProperty == _T("QuoteBegin")) {
		return AnsiString(Get_QuoteBegin());
	}
	if (asProperty == _T("QuoteEnd")) {
		return AnsiString(Get_QuoteEnd());
	}
	if (asProperty == _T("QuoteBegin2")) {
		return AnsiString(Get_QuoteBegin2());
	}
	if (asProperty == _T("QuoteEnd2")) {
		return AnsiString(Get_QuoteEnd2());
	}
	if (asProperty == _T("QuoteBegin3")) {
		return AnsiString(Get_QuoteBegin3());
	}
	if (asProperty == _T("QuoteEnd3")) {
		return AnsiString(Get_QuoteEnd3());
	}
	if (asProperty == _T("EscapeString")) {
		return Get_EscapeString();
	}
	if (asProperty == _T("MultiLine")) {
		return Get_MultiLine();
	}
	if (asProperty == _T("Associated1")) {
		return Get_Associated1();
	}
	if (asProperty == _T("Associated2")) {
		return Get_Associated2();
	}
	if (asProperty == _T("Tooltip")) {
		return Get_Tooltip()->Name;
	}
	if (asProperty == _T("OnCaretMove")) {
		return OnCaretMove.AsString();
	}
	if (asProperty == _T("OnAutocompletionOn")) {
		return OnAutocompletionOn.AsString();
	}
	if (asProperty == _T("OnAutocompletionOff")) {
		return OnAutocompletionOff.AsString();
	}
	return TCustomEditor::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TTextEditor::SetProperty(AnsiString asProperty, AnsiString asValue,
															TPersistent *Sender) {
	if (asProperty == _T("BigEndian")) {
		Set_BigEndian(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("ByteOrderMark")) {
		Set_ByteOrderMark(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("Encodage")) {
    if      (asValue == _T("ceASCII")) Set_Encodage(ceASCII);
    else if (asValue == _T("ceUtf8")) Set_Encodage(ceUtf8);
    else if (asValue == _T("ceUNICODE")) Set_Encodage(ceUNICODE);
    return true;
	}
	if (asProperty == _T("LinuxEndLine")) {
		Set_LinuxEndLine(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("FirstLine")) {
		Set_FirstLine(asValue.ToIntDef(0));
		return true;
	}
	if (asProperty == _T("FirstCol")) {
		Set_FirstCol(asValue.ToIntDef(0));
		return true;
	}
	if (asProperty == _T("NumLine")) {
		Set_NumLine(asValue.ToIntDef(0));
		return true;
	}
	if (asProperty == _T("NumCol")) {
		Set_NumCol(asValue.ToIntDef(0));
		return true;
	}
	if (asProperty == _T("Keywords")) {
		if (FKeywords) {
			FKeywords->SetTextStr(asValue);
			return true;
		}
		return false;
	}
	if (asProperty == _T("Lines")) {
		FLines->SetTextStr(asValue);
		return true;
	}
	if (asProperty == _T("LinesNumbers")) {
		Set_LinesNumbers(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("IconsColumn")) {
		Set_IconsColumn(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("NbSpacesTab")) {
		Set_NbSpacesTab(asValue.ToIntDef(0));
		return true;
	}
	if (asProperty == _T("SpacesToTabs")) {
		Set_SpacesToTabs(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("TabsToSpaces")) {
		Set_TabsToSpaces(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("SelText")) {
		Set_SelText(asValue);
		return true;
	}
	if (asProperty == _T("TypeAuto")) {
		Set_TypeAuto(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("IndentAuto")) {
		Set_IndentAuto(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("CurAfterEndLine")) {
		Set_CurAfterEndLine(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		Set_UndoRedoMoveCur(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("AlphaChars")) {
		Set_AlphaChars(asValue);
		return true;
	}
	if (asProperty == _T("Autocompletion")) {
		Set_Autocompletion(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("Autocompletion1")) {
		Set_Autocompletion1(asValue);
		return true;
	}
	if (asProperty == _T("Autocompletion2")) {
		Set_Autocompletion2(asValue);
		return true;
	}
	if (asProperty == _T("Autocompletion3")) {
		Set_Autocompletion3(asValue);
		return true;
	}
	if (asProperty == _T("CaseSensitive")) {
		Set_CaseSensitive(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("CommentSingle")) {
		Set_CommentSingle(asValue);
		return true;
	}
	if (asProperty == _T("CommentSingleBeg")) {
		Set_CommentSingleBeg(asValue);
		return true;
	}
	if (asProperty == _T("CommentBegin")) {
		Set_CommentBegin(asValue);
		return true;
	}
	if (asProperty == _T("CommentEnd")) {
		Set_CommentEnd(asValue);
		return true;
	}
	if (asProperty == _T("CommentBegin2")) {
		Set_CommentBegin2(asValue);
		return true;
	}
	if (asProperty == _T("CommentEnd2")) {
		Set_CommentEnd2(asValue);
		return true;
	}
	if (asProperty == _T("BlockBegin")) {
		Set_BlockBegin(asValue);
		return true;
	}
	if (asProperty == _T("BlockEnd")) {
		Set_BlockEnd(asValue);
		return true;
	}
	if (asProperty == _T("PreProc")) {
		Set_PreProc(asValue);
		return true;
	}
	if (asProperty == _T("QuoteBegin")) {
		Set_QuoteBegin(asValue[1]);
		return true;
	}
	if (asProperty == _T("QuoteEnd")) {
		Set_QuoteEnd(asValue[1]);
		return true;
	}
	if (asProperty == _T("QuoteBegin2")) {
		Set_QuoteBegin2(asValue[1]);
		return true;
	}
	if (asProperty == _T("QuoteEnd2")) {
		Set_QuoteEnd2(asValue[1]);
		return true;
	}
	if (asProperty == _T("QuoteBegin3")) {
		Set_QuoteBegin3(asValue[1]);
		return true;
	}
	if (asProperty == _T("QuoteEnd3")) {
		Set_QuoteEnd3(asValue[1]);
		return true;
	}
	if (asProperty == _T("EscapeString")) {
		Set_EscapeString(asValue);
		return true;
	}
	if (asProperty == _T("MultiLine")) {
		Set_MultiLine(asValue);
		return true;
	}
	if (asProperty == _T("Associated1")) {
		Set_Associated1(asValue);
		return true;
	}
	if (asProperty == _T("Associated2")) {
		Set_Associated2(asValue);
		return true;
	}
	if (asProperty == _T("OnCaretMove")) {
		OnCaretMove.ObjCall = Sender;
		OnCaretMove = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnAutocompletionOn")) {
		OnAutocompletionOn.ObjCall = Sender;
		OnAutocompletionOn = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnAutocompletionOff")) {
		OnAutocompletionOff.ObjCall = Sender;
		OnAutocompletionOff = asValue.c_str();
		return true;
	}
	return TCustomEditor::SetProperty(asProperty, asValue, Sender);
}
//---------------------------------------------------------------------------
