//-----------------------------------------------------------------------------
//! @file THexaEditor.cpp
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

#include <THexaEditor.h>
#include <TDictionnary.h>

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum ACTIONUNDOHEXA {
	auhMoveCur,						               //!< Déplacement du curseur texte
	auhDeleteString,						         //!< Suppression d'une partie de ligne
	auhInsertString,						         //!< Insertion dans une ligne
	auhChangeString,						         //!< Insertion dans une ligne
};

typedef struct {
	ACTIONUNDOHEXA ActionUndo;
	int FirstLine;
	__int64 FilePos;
	__int64 FilePosBegSel;
	__int64 FilePosEndSel;
	bool bRightColumn;
	bool bHighValue;
} UNDOMOVECURHEXA, *LPUNDOMOVECURHEXA;

typedef struct {
	ACTIONUNDOHEXA ActionUndo;
	__int64 FilePos;
	__int64 Count;
} UNDODELETESTRINGHEXA, *LPUNDODELETESTRINGHEXA;

typedef struct {
	ACTIONUNDOHEXA ActionUndo;
	__int64 FilePos;
	__int64 Count;
	char szText[1];
} UNDOINSERTSTRINGHEXA, *LPUNDOINSERTSTRINGHEXA;

typedef struct {
	ACTIONUNDOHEXA ActionUndo;
	__int64 FilePos;
	__int64 Count;
	char szText[1];
} UNDOCHANGESTRINGHEXA, *LPUNDOCHANGESTRINGHEXA;

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

THexaEditor::THexaEditor(TComponent* AOwner):
	TCustomEditor(AOwner) {

  // Initialisations
  FClassName = _T("THexaEditor");
	WantTabs = true;
	WantReturns = true;
	bCaret = false;
	bMouseCapture = false;
	bReadOnly = false;
	bInsert = true;
	bDblClick = false;
	FFilePos = 0;
	FFirstLine = 0;
	FilePosBegSel = 0;
	FilePosEndSel = 0;
	NbBytesPerLine = 1;
	LinesCount = 0;
	bRightColumn = false;
	bHighValue = true;
	FIconsColumn = true;
	CurrentTypeUndo = tuhNull;

	FileSize = 0;
	AllocatedSize = 0;
	FileContent = NULL;
	FUndoRedoMoveCur = true;

	SET_EVENT(this, TNotifyEvent, OnResize, THexaEditor, this, HexaEditorResize);
	SET_EVENT(this, TOnEnter, OnEnter, THexaEditor, this, HexaEditorEnter);
	SET_EVENT(this, TOnExit, OnExit, THexaEditor, this, HexaEditorExit);
	SET_EVENT(this, TOnKeyDown, OnKeyDown, THexaEditor, this, HexaEditorKeyDown);
	SET_EVENT(this, TOnKeyPress, OnKeyPress, THexaEditor, this, HexaEditorKeyPress);
	SET_EVENT(this, TOnMouseDown, OnMouseDown, THexaEditor, this, HexaEditorMouseDown);
	SET_EVENT(this, TOnMouseUp, OnMouseUp, THexaEditor, this, HexaEditorMouseUp);
	SET_EVENT(this, TOnMouseMove, OnMouseMove, THexaEditor, this, HexaEditorMouseMove);
	SET_EVENT(this, TOnMouseWheel, OnMouseWheel, THexaEditor, this, HexaEditorMouseWheel);
	SET_EVENT(this, TNotifyEvent, OnDblClick, THexaEditor, this, HexaEditorDblClick);

	// Verticale
	FVertScrollBar->Min = 0;
	FVertScrollBar->Max = 1000;
	FVertScrollBar->PageSize = 0;
	FVertScrollBar->Position = 0;

	SET_EVENT(FVertScrollBar, TOnScroll, OnScroll, THexaEditor, this, VertScrollBarScroll);

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

THexaEditor::~THexaEditor(void) {
	SetCaretVisible(false);
	if (CurrentTypeUndo != tuhNull) UndoRedo->EndOperation();
	if (FileContent) delete[] FileContent;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FFilePos
//---------------------------------------------------------------------------

__int64 THexaEditor::Get_FilePos() {
	return FFilePos;
}

bool THexaEditor::Set_FilePos(__int64 NewFilePos) {
	if (FFilePos != NewFilePos) {
		MoveCur(NewFilePos, true, false);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FFirstLine
//---------------------------------------------------------------------------

int THexaEditor::Get_FirstLine() {
	return FFirstLine;
}

bool THexaEditor::Set_FirstLine(int NewFirstLine) {
	if (FFirstLine != NewFirstLine) {
		SetFirstLine(NewFirstLine);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété IconsColumn
//---------------------------------------------------------------------------

bool THexaEditor::Get_IconsColumn(void) {
	return FIconsColumn;
}

bool THexaEditor::Set_IconsColumn(bool NewIconsColumn) {
	if (FIconsColumn != NewIconsColumn) {
		FIconsColumn = NewIconsColumn;
		ProcessResize();
		InvalidateOrSetFlag();
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelText
//---------------------------------------------------------------------------

AnsiString THexaEditor::Get_SelText(void) {
	return GetSelText();
}

bool THexaEditor::Set_SelText(AnsiString asNewSelText) {
	SetSelTextBuf(asNewSelText);
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété UndoRedoMoveCur
//---------------------------------------------------------------------------

bool THexaEditor::Get_UndoRedoMoveCur(void) {
	return FUndoRedoMoveCur;
}

bool THexaEditor::Set_UndoRedoMoveCur(bool NewUndoRedoMoveCur) {
	if (FUndoRedoMoveCur != NewUndoRedoMoveCur) {
		FUndoRedoMoveCur = NewUndoRedoMoveCur;
	}
	return true;
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnResize
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::HexaEditorResize(TObject *Sender) {
	ProcessResize();
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnEnter
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::HexaEditorEnter(TObject *Sender) {
	SetCaretVisible(true);
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnExit
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::HexaEditorExit(TObject *Sender) {
	SetCaretVisible(false);
}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::HexaEditorKeyDown(TObject *Sender,
			WORD Key, TShiftState Shift) {

	bool bAlt = Shift.Contains(ssAlt);
	bool bCtrl = Shift.Contains(ssCtrl);
	bool bShift = Shift.Contains(ssShift);
	switch (Key) {
	case VK_LEFT:
		SetTypeUndoRedo(tuhMoveCur);
		if (bRightColumn) {
			MoveCur(FFilePos - 1, true, bShift);
		}
		else if (bHighValue) {
			MoveCur(FFilePos - 1, false, bShift);
		}
		else {
			MoveCur(FFilePos, true, bShift);
		}
		break;
	case VK_RIGHT:
		SetTypeUndoRedo(tuhMoveCur);
		if (bRightColumn) {
			MoveCur(FFilePos + 1, true, bShift);
		}
		else if (bHighValue) {
			MoveCur(FFilePos, false, bShift);
		}
		else {
			MoveCur(FFilePos + 1, true, bShift);
		}
		break;
	case VK_UP:
		SetTypeUndoRedo(tuhMoveCur);
		if (FFilePos - NbBytesPerLine >= 0) MoveCur(FFilePos - NbBytesPerLine, bHighValue, bShift);
		break;
	case VK_DOWN:
		SetTypeUndoRedo(tuhMoveCur);
		if (FFilePos + NbBytesPerLine <= FileSize) MoveCur(FFilePos + NbBytesPerLine, bHighValue, bShift);
		break;
	case VK_HOME:
		SetTypeUndoRedo(tuhMoveCur);
		if (bCtrl) MoveCur(0, true, bShift);
		else MoveCur((FFilePos / NbBytesPerLine) * NbBytesPerLine, true, bShift);
		break;
	case VK_END:
		SetTypeUndoRedo(tuhMoveCur);
		if (bCtrl) MoveCur(FileSize, true, bShift);
		else MoveCur((FFilePos / NbBytesPerLine + 1) * NbBytesPerLine - 1, true, bShift);
		break;
	case VK_PRIOR:
		SetTypeUndoRedo(tuhMoveCur);
		if (FFilePos - (NbLinesWin - 1) * NbBytesPerLine >= 0) {
			MoveCur(FFilePos - (NbLinesWin - 1) * NbBytesPerLine, bHighValue, bShift);
		}
		else {
			MoveCur(FFilePos % NbBytesPerLine, bHighValue, bShift);
		}
		break;
	case VK_NEXT:
		SetTypeUndoRedo(tuhMoveCur);
		if (FFilePos + (NbLinesWin - 1) * NbBytesPerLine <= FileSize) {
			MoveCur(FFilePos + (NbLinesWin - 1) * NbBytesPerLine, bHighValue, bShift);
		}
		else {
			MoveCur(FFilePos + ((FileSize - FFilePos) / NbBytesPerLine) * NbBytesPerLine, bHighValue, bShift);
		}
		break;
	case VK_BACK:
		if (!bReadOnly) {
			SetTypeUndoRedo(tuhDelete);
			if (FilePosBegSel != FilePosEndSel) {
				SetTypeUndoRedo(tuhDelete);
				ClearSelection();
			}
			else {
				SetTypeUndoRedo(tuhDelete);
				if (FFilePos > 0) {
					FFilePos--;
					DeleteString(FFilePos, 1);
					MoveCur(FFilePos, true, false);
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
		if (FilePosBegSel != FilePosEndSel) {
			// Copier
			if (bShift) CopyToClipboard();
			if (!bReadOnly) {
				SetTypeUndoRedo(tuhDelete);
				ClearSelection();
			}
		}
		else {
			if (!bReadOnly) {
				if (FilePosBegSel != FilePosEndSel) {
					SetTypeUndoRedo(tuhDelete);
					ClearSelection();
				}
				else {
					if (FFilePos < FileSize - 1) {
						SetTypeUndoRedo(tuhDelete);
						DeleteString(FFilePos, 1);
					}
				}
			}
		}
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
				PasteFromClipboard();
				break;
			}
		}
	}

}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::HexaEditorKeyPress(TObject *Sender, TCHAR Key) {

	if (!bReadOnly) {
		if ((BYTE) Key >= _T(' ')) {

			// Effacement de la sélection éventuelle
			ClearSelection();

			SetTypeUndoRedo(tuhText);

			if (bRightColumn) {

				if (bInsert) {
					// Insertion du caractère
					InsertString(FFilePos, 1, (char *) &Key);
					FFilePos = FFilePos + 1;
				}
				else {
					// Modification du caractère
					ChangeString(FFilePos, 1, (char *) &Key);
					FFilePos = FFilePos + 1;
				}

			}

			else {

				char c = (char) Key;
				BYTE b;
				if (('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f')) {

					if ('0' <= c && c <= '9') b = c - '0';
					else if ('A' <= c && c <= 'F') b = c - 'A' + 10;
					else if ('a' <= c && c <= 'f') b = c - 'a' + 10;

					if (bInsert) {
						if (bHighValue) {
							// Insertion du caractère
							b <<= 4;
							InsertString(FFilePos, 1, (char *) &b);
							bHighValue = false;
						}
						else {
							// Modification du caractère
							b |= (FileContent[FFilePos] & 0xF0);
							ChangeString(FFilePos, 1, (char *) &b);
							FFilePos = FFilePos + 1;
							bHighValue = true;
						}
					}
					else {
						if (FFilePos < FileSize) {
							// Modification du caractère
							if (bHighValue) {
								b <<= 4;
								b |= (FileContent[FFilePos] & 0x0F);
								ChangeString(FFilePos, 1, (char *) &b);
								bHighValue = false;
							}
							else {
								b |= (FileContent[FFilePos] & 0xF0);
								ChangeString(FFilePos, 1, (char *) &b);
								FFilePos = FFilePos + 1;
								bHighValue = true;
							}
						}
					}

				}
			}

		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::HexaEditorMouseDown(
			TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
			int Y) {
	bool bARightColumn;
	bool bAHighValue;

	SetFocus();

	if (!bDblClick) {

		__int64 NewFilePos;
		bool bCtrl = Shift.Contains(ssCtrl);
		bool bAlt = Shift.Contains(ssAlt);
		bool bShift = Shift.Contains(ssShift);
		GetPosAt(Y, X, &NewFilePos, &bARightColumn, &bAHighValue);
		if (Button == mbLeft) {
			SetTypeUndoRedo(tuhMoveCur);
			bRightColumn = bARightColumn;
			MoveCur(NewFilePos, bAHighValue, bShift);
			Mouse->Capture = Handle;
			bMouseCapture = true;
		}
		else if (Button == mbRight) {
			if (!IsInSelection(NewFilePos, bARightColumn)) {
				bRightColumn = bARightColumn;
				MoveCur(NewFilePos, bAHighValue, false);
			}
		}

		bDblClick = false;
	}
}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::HexaEditorMouseUp(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y) {
	if (bMouseCapture) {
		__int64 NewFilePos;
		bool bAHighValue;

		bool bAlt = Shift.Contains(ssAlt);
		bool bCtrl = Shift.Contains(ssCtrl);
		bool bShift = Shift.Contains(ssShift);
		GetPosAt(Y, X, &NewFilePos, &bRightColumn, &bAHighValue);
		SetTypeUndoRedo(tuhMoveCur);
		MoveCur(NewFilePos, bAHighValue, true);
		Mouse->Capture = NULL;
		bMouseCapture = false;
	}
	bDblClick = false;
}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::HexaEditorMouseMove(
			TObject *Sender, TShiftState Shift, int X, int Y) {
	__int64 NewFilePos;
	bool bARightColumn;
	bool bAHighValue;

	GetPosAt(Y, X, &NewFilePos, &bARightColumn, &bAHighValue);
	if (bMouseCapture) {
		bool bAlt = Shift.Contains(ssAlt);
		bool bCtrl = Shift.Contains(ssCtrl);
		bool bShift = Shift.Contains(ssShift);
		bRightColumn = bARightColumn;
		MoveCur(NewFilePos, bAHighValue, (Mouse->Capture == Handle));
	}
	else {
		if (IsInSelection(NewFilePos, bARightColumn)) Cursor = crArrow;
		else Cursor = crIBeam;
	}
}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::HexaEditorMouseWheel(TObject *Sender, TShiftState Shift,
			int WheelDelta, const TPoint &MousePos, bool &Handled) {
	SetFirstLine(FFirstLine - WheelDelta / 30);
}


//---------------------------------------------------------------------------
void FASTCALL THexaEditor::VertScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos) {
	SetFirstLine(ScrollPos);
}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::HexaEditorDblClick(TObject *Sender) {
	Mouse->Capture = NULL;
	bMouseCapture = false;
	bDblClick = true;
}

//---------------------------------------------------------------------------
// Lecture depuis un fichier
//---------------------------------------------------------------------------

bool FASTCALL THexaEditor::LoadFromFile(const AnsiString FileName) {
  HANDLE hFile;
  DWORD dw;
	bool Ok;


  BeginUpdate();
  Clear();
  hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		if (FileContent) delete[] FileContent;

		FileSize = GetFileSize(hFile, NULL);
		AllocatedSize = FileSize + 1024;
		FileContent = new char[(unsigned int) AllocatedSize];
    ReadFile(hFile, FileContent, (unsigned int) FileSize, &dw, NULL);
    CloseHandle(hFile);
		LinesCount = (int) (FileSize / NbBytesPerLine + 1);
		Ok = true;
  }
  else {
    // Erreur ouverture FileName
		FLastError = GetLastError();
		Ok = false;
  }
  EndUpdate();

	if (Ok) UpdateVScroll();

	return Ok;
}

//---------------------------------------------------------------------------
//! Enregistrement dans un fichier
//---------------------------------------------------------------------------

bool FASTCALL THexaEditor::SaveToFile(const AnsiString FileName) {
  HANDLE hFile;
  DWORD dw;
	bool Ok;


  hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    WriteFile(hFile, FileContent, (unsigned int) FileSize, &dw, NULL);
    CloseHandle(hFile);

		Ok = true;
  }
  else {
    // Erreur ouverture FileName
		FLastError = GetLastError();
		Ok = false;
  }

	return Ok;
}

//---------------------------------------------------------------------------
// Dessin de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::Paint(void) {
	int NumLine;
	int y;


	TRect DrawRect;
	DrawRect = ClientRect;

	// Caret
	bool bMemoCaret = bCaret;
	if (bMemoCaret) SetCaretVisible(false);

	// Dessin du fond (parce que les lignes ne remplissent peut-être pas l'écran)
	TCanvas *ACanvas = Canvas;
	ACanvas->Brush->Color = FColorParams[tsNormal].Background;
	ACanvas->Brush->Style = bsSolid;
	ACanvas->FillRect(DrawRect);
	ACanvas->TextFlags = tfTransparent;

	for (y = 0, NumLine = FFirstLine;
		   y < DrawRect.Bottom && NumLine < LinesCount;
			 y += tmHeight + tmInternalLeading, NumLine++) {
		DrawLine(ACanvas, NumLine);
	}

	// Caret
	if (bMemoCaret) SetCaretVisible(true);

}

//---------------------------------------------------------------------------
// Affichage du caret
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::SetCaretVisible(bool bVisible) {

	if (bVisible) {
		CreateCaret(Handle, NULL, bInsert? 1: tmAveCharWidth, tmHeight);
		SetCurrentCaretPos();
		OnCaretMove(this, FFilePos);
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

void FASTCALL THexaEditor::SetCurrentCaretPos(void) {
	int X, Y;
	int NumLine, NumCol;


	NumLine = (int) (FFilePos / NbBytesPerLine);
	NumCol = (int) (FFilePos % NbBytesPerLine);
	if (!bRightColumn) {
		X = (10 /*9 + 1*/ + NumCol * 3) * tmAveCharWidth + 4;
		if (!bHighValue) X += tmAveCharWidth;
	}
	else {
		X = (12 /*9 + 3*/ + NbBytesPerLine * 3 + NumCol) * tmAveCharWidth + 4;
	}
	if (FIconsColumn) X += 24;
	Y = (int) (NumLine - FFirstLine) * (tmHeight + tmInternalLeading);

	SetCaretPos(X, Y);
}


//---------------------------------------------------------------------------
// Déplacement du curseur
//---------------------------------------------------------------------------

void	FASTCALL THexaEditor::MoveCur(__int64 NewFilePos, bool bNewHighValue, bool bSelection) {
	int l;
	int NumLineOldSel1 = 0;
	int NumLineOldSel2 = 0;
	int NumLineNewSel1 = 0;
	int NumLineNewSel2 = 0;
	bool bDrawOldSel = false;
	bool bDrawNewSel = false;
	bool bChanged = false;
	int NumLine, NewNumLine;


	if (NewFilePos < 0) {
		NewFilePos = 0;
		bNewHighValue = true;
	}
	if (NewFilePos >= FileSize) {
		NewFilePos = FileSize;
		bNewHighValue = true;
	}

	if (bSelection) {
		FilePosEndSel = NewFilePos;
	}
	else {
		// Effacement de l'ancienne sélection (si elle existe)
		if (FilePosBegSel != FilePosEndSel) {
			NumLineOldSel1 = (int) (FilePosBegSel / NbBytesPerLine);
			NumLineOldSel2 = (int) (FilePosEndSel / NbBytesPerLine);
			bDrawOldSel = true;
		}
		FilePosBegSel = FilePosEndSel = NewFilePos;
	}

	if (NewFilePos != FFilePos || bNewHighValue != bHighValue) {

		NumLine = (int) (FFilePos / NbBytesPerLine);
		NewNumLine = (int) (NewFilePos / NbBytesPerLine);

		if (bSelection) {
			NumLineNewSel1 = NumLine;
			NumLineNewSel2 = NewNumLine;
			bDrawNewSel = true;
		}

		FFilePos = NewFilePos;
		bHighValue = bNewHighValue;
		if (NewNumLine < FFirstLine || NewNumLine - NbLinesWin + 1 > FFirstLine) {
			ScrollInView(FFilePos);
			bDrawOldSel = false;
			bDrawNewSel = false;
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

	SetCurrentCaretPos();  // En dehors de la parenthèse car il faut pouvoir passer d'une colonne à l'autre
	if (bChanged) {
		OnCaretMove(this, FFilePos);
	}

}


//---------------------------------------------------------------------------
// Scroll vertical de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::SetFirstLine(int NewFirstLine) {

	if (NewFirstLine > LinesCount - NbLinesWin) {
		NewFirstLine = LinesCount - NbLinesWin;
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
// Déplacement de la fenêtre de visualisation
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::ScrollInView(__int64 AFilePos) {
	int NumLine;

	if (AFilePos > FileSize) AFilePos = FileSize;
	if (AFilePos < 0) AFilePos = 0;

	NumLine = (int) (AFilePos / NbBytesPerLine);
	int NewFirstLine = FFirstLine;
	if (NewFirstLine < NumLine - NbLinesWin + 1) {
		NewFirstLine = (int) NumLine - NbLinesWin + 1;
	}
	else if (NewFirstLine > NumLine) {
		NewFirstLine = (int) NumLine;
	}

	if (NewFirstLine > LinesCount - NbLinesWin) {
		NewFirstLine = LinesCount - NbLinesWin;
	}
	if (NewFirstLine < 0) {  // Pas "else if" (NewFirstLine a pu devenir négatif dans le test précédent)
		NewFirstLine = 0;
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

void FASTCALL THexaEditor::DrawLine(TCanvas *ACanvas, int NumLine) {
	TRect Rect;
	int x, y;
	int i;
	__int64 Adr;
	TYPESYNTAX TypeSyntax, NewTypeSyntax;
	__int64 FilePosBegSel2;
	__int64 FilePosEndSel2;
	bool bBookmark;


	if (NbBeginUpdate == 0) {

		if (bCaret) HideCaret(Handle);

		TRect DrawRect;
		DrawRect = ClientRect;
		GetSelOrdered(&FilePosBegSel2, &FilePosEndSel2);

		x = 0;
		y = (tmHeight + tmInternalLeading) * ((int) (NumLine - FFirstLine));

		ACanvas->TextFlags = tfOpaque;
		ACanvas->Brush->Color = FColorParams[tsLinesNumbers].Background;
		Rect.left = DrawRect.Left;
		Rect.top = DrawRect.Top + y;
		Rect.right = DrawRect.Left + tmAveCharWidth * 9 + 4;
		Rect.bottom = y + tmHeight;
		ACanvas->FillRect(Rect);

		Adr = (__int64) NumLine * NbBytesPerLine;
		TCHAR szBuf[10];
		wsprintf(szBuf, _T("%04X:%04X"), (unsigned int) ((Adr >> 16) & 0xFFFF), (unsigned int) (Adr & 0xFFFF));
		SetColorSyntax(ACanvas, tsLinesNumbers);
		ACanvas->TextOut(DrawRect.Left + 2, y, szBuf);
		DrawRect.Left = DrawRect.Left + tmAveCharWidth * 9 + 4;

		if (FIconsColumn) {
			ACanvas->TextFlags = tfOpaque;
			ACanvas->Brush->Color = FColorParams[tsIconsColumn].Background;
			Rect.left = DrawRect.Left;
			Rect.top = DrawRect.Top + y;
			Rect.right = DrawRect.Left + 24;
			Rect.bottom = y + tmHeight;
			ACanvas->FillRect(Rect);

			bBookmark = false;
			for (i = 0; i < (int) Bookmarks.size(); i++) {
				if (Adr <= Bookmarks[i] && Bookmarks[i] < Adr + NbBytesPerLine) {
					bBookmark = true;
					break;
				}
				if (Bookmarks[i] >= Adr + NbBytesPerLine) break;
			}
			if (bBookmark) {
				ACanvas->Pen->Color = RGB(0, 0, 0xFF);
				ACanvas->Brush->Color = RGB(0xCF, 0xCF, 0xFF);
				Rect.left = DrawRect.Left + 3;
				Rect.top = DrawRect.Top + y + 1;
				Rect.right = DrawRect.Left + 20;
				Rect.bottom = y + tmHeight - 1;
				ACanvas->RoundRect(Rect, 5, 5);
			}

			DrawRect.Left = DrawRect.Left + 24;
		}

		SetColorSyntax(ACanvas, tsNumber);
		TypeSyntax = tsNumber;
		for (i = 0; i < NbBytesPerLine && Adr < FileSize; i++) {

			if (!bRightColumn && FilePosBegSel2 <= Adr && Adr < FilePosEndSel2) NewTypeSyntax = tsSelect;
			else NewTypeSyntax = tsNumber;
			if (TypeSyntax != NewTypeSyntax) {
				if (TypeSyntax == tsNumber) {
					ACanvas->TextOut(DrawRect.Left + x, DrawRect.Top + y, _T(" "));
					x += tmAveCharWidth;
				}
				SetColorSyntax(ACanvas, NewTypeSyntax);
				TypeSyntax = NewTypeSyntax;
				if (TypeSyntax == tsNumber) {
					ACanvas->TextOut(DrawRect.Left + x, DrawRect.Top + y, _T(" "));
					x += tmAveCharWidth;
				}
			}
			else {
				ACanvas->TextOut(DrawRect.Left + x, DrawRect.Top + y, _T(" "));
				x += tmAveCharWidth;
			}

			wsprintf(szBuf, _T("%02X"), (BYTE) FileContent[Adr++]);
			ACanvas->TextOut(DrawRect.Left + x, DrawRect.Top + y, szBuf);
			x += tmAveCharWidth * 2;

		}

		x = tmAveCharWidth * (3 + 3 * NbBytesPerLine);
		Adr = (__int64) NumLine * NbBytesPerLine;

		for (i = 0; i < NbBytesPerLine && Adr < FileSize; i++) {
			if (bRightColumn && FilePosBegSel2 <= Adr && Adr < FilePosEndSel2) NewTypeSyntax = tsSelect;
			else NewTypeSyntax = tsNormal;
			if (TypeSyntax != NewTypeSyntax) {
				SetColorSyntax(ACanvas, NewTypeSyntax);
				TypeSyntax = NewTypeSyntax;
			}
			if ((BYTE) FileContent[Adr] < ' ') {
				szBuf[0] = _T('.');
			}
			else {
				szBuf[0] = (BYTE) FileContent[Adr];
			}
			szBuf[1] = _T('\0');
			ACanvas->TextOut(DrawRect.Left + x, DrawRect.Top + y, szBuf);
			Adr++;
			x += tmAveCharWidth;
		}

		if (bCaret) ShowCaret(Handle);

	}
	else {
		bMustRefresh = true;
	}

}

//---------------------------------------------------------------------------
// Sélection ordonnée
//---------------------------------------------------------------------------
void THexaEditor::GetSelOrdered(__int64 *FilePosBegSel2, __int64 *FilePosEndSel2) {

	if (FilePosBegSel > FilePosEndSel) {
		*FilePosBegSel2 = FilePosEndSel;
		*FilePosEndSel2 = FilePosBegSel;
	}
	else {
		*FilePosBegSel2 = FilePosBegSel;
		*FilePosEndSel2 = FilePosEndSel;
	}

}

//---------------------------------------------------------------------------
// Teste si le caractère (AFilePos) est dans la sélection.
//---------------------------------------------------------------------------

bool THexaEditor::IsInSelection(__int64 AFilePos, bool bARightColumn) {
	__int64 FilePosBegSel2;
	__int64 FilePosEndSel2;

	GetSelOrdered(&FilePosBegSel2, &FilePosEndSel2);

	if (AFilePos < FilePosBegSel2 ||
			AFilePos > FilePosEndSel2 ||
			bARightColumn != bRightColumn) {
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::ClearSelection(void) {
	__int64 FilePosBegSel2, FilePosEndSel2;


	if (FilePosBegSel == FilePosEndSel) return;

	// Si le ClearSelection ne fait pas partie d'une autre opération
	if (CurrentTypeUndo == tuhNull || CurrentTypeUndo == tuhMoveCur) SetTypeUndoRedo(tuhDelete);

	GetSelOrdered(&FilePosBegSel2, &FilePosEndSel2);

	MoveCur(FilePosBegSel2, true, false);

	DeleteString(FilePosBegSel2, FilePosEndSel2 - FilePosBegSel2);

}

//---------------------------------------------------------------------------
// Couper et copier dans le presse-papier
//---------------------------------------------------------------------------

bool FASTCALL THexaEditor::CutToClipboard(void) {
	CopyToClipboard();
	if (!bReadOnly) ClearSelection();

	return true;
}

//---------------------------------------------------------------------------
// Copier dans le presse-papier
//---------------------------------------------------------------------------

bool FASTCALL THexaEditor::CopyToClipboard(void) {
	AnsiString asContenu;
	CharString csRTFContenu;


	asContenu = GetSelText();

	if (!asContenu.IsEmpty()) {
		// Enregistrement dans le presse-papiers
		Clipboard->Open();
		Clipboard->Clear();
		Clipboard->SetTextBuf(asContenu);
		Clipboard->Close();
	}

	return true;
}

//---------------------------------------------------------------------------
// Coller depuis le presse-papier
//---------------------------------------------------------------------------

bool FASTCALL THexaEditor::PasteFromClipboard(void) {

	return true;
}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::GetPosAt(int Y, int X, __int64 *AFilePos, bool *bRightColumn, bool *bHighValue) {
	int NumCol, NumLine;


	X -= tmAveCharWidth * 9 + 4;
	if (FIconsColumn) X -= 24;
	if (X < 0) X = 0;
	if (X < (1 + 3 * NbBytesPerLine) * tmAveCharWidth) {
		NumCol = (int) (((double) X / tmAveCharWidth) / 3);
		*bRightColumn = false;
		if (X - tmAveCharWidth < NumCol * 3 * tmAveCharWidth + tmAveCharWidth / 2) *bHighValue = true;
		else *bHighValue = false;
	}
	else {
		X -= tmAveCharWidth * (3 + 3 * NbBytesPerLine);
		if (X < 0) X = 0;
		NumCol = (int) (((double) X / tmAveCharWidth) + .5);
		*bRightColumn = true;
	}
	if (NumCol >= NbBytesPerLine) NumCol = NbBytesPerLine;

	NumLine = FFirstLine + (int) ((double) Y / (tmHeight + tmInternalLeading));
	*AFilePos = (__int64) NumLine * NbBytesPerLine + NumCol;
	if (*AFilePos >= FileSize) *AFilePos = FileSize;

}

//---------------------------------------------------------------------------
void FASTCALL THexaEditor::UpdateVScroll(void) {

	if (NbBeginUpdate == 0) {
		// Verticale
		FVertScrollBar->Max = LinesCount;
		FVertScrollBar->PageSize = NbLinesWin + 1;  // + 1 sinon on a un ascenseur qui ne sert à rien
		FVertScrollBar->Position = FFirstLine;

	}

}

//---------------------------------------------------------------------------
void THexaEditor::DeleteString(__int64 AFilePos, __int64 Count) {
	AnsiString asLine;
	UNDODELETESTRINGHEXA UndoDeleteString;
	LPUNDOINSERTSTRINGHEXA lpUndoInsertString;
	int SizeBuffer;


	if (AFilePos + Count > FileSize) return;

	if (CurrentTypeUndo != tuhNull) {

		// Mémorisation du Undo
		SizeBuffer = sizeof(UNDOINSERTSTRINGHEXA) + (int) Count * sizeof(TCHAR);
		lpUndoInsertString = (LPUNDOINSERTSTRINGHEXA) new BYTE[SizeBuffer];
		lpUndoInsertString->ActionUndo = auhInsertString;
		lpUndoInsertString->FilePos = AFilePos;
		lpUndoInsertString->Count = Count;
		memcpy(lpUndoInsertString->szText, &FileContent[AFilePos], (int) Count);
		UndoRedo->MemoUndo(this, lpUndoInsertString, SizeBuffer);
		delete[] lpUndoInsertString;

		// Mémorisation du Redo
		UndoDeleteString.ActionUndo = auhDeleteString;
		UndoDeleteString.FilePos = AFilePos;
		UndoDeleteString.Count = Count;
		UndoRedo->MemoRedo(this, &UndoDeleteString, sizeof(UNDODELETESTRINGHEXA));

	}

	memcpy(&FileContent[AFilePos], &FileContent[AFilePos + Count], (unsigned int) (FileSize - AFilePos - Count));
	FileSize -= Count;

	UpdateVScroll();
	InvalidateOrSetFlag();
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

}

//---------------------------------------------------------------------------
void THexaEditor::InsertString(__int64 AFilePos, __int64 Count, char *str) {
	AnsiString asLine;
	UNDODELETESTRINGHEXA UndoDeleteString;
	LPUNDOINSERTSTRINGHEXA lpUndoInsertString;
	int SizeBuffer;


	if (AFilePos > FileSize) return;

	if (CurrentTypeUndo != tuhNull) {

		// Mémorisation du Undo
		UndoDeleteString.ActionUndo = auhDeleteString;
		UndoDeleteString.FilePos = AFilePos;
		UndoDeleteString.Count = Count;
		UndoRedo->MemoUndo(this, &UndoDeleteString, sizeof(UNDODELETESTRINGHEXA));

		// Mémorisation du Redo
		SizeBuffer = sizeof(UNDOINSERTSTRINGHEXA) + (int) Count * sizeof(TCHAR);
		lpUndoInsertString = (LPUNDOINSERTSTRINGHEXA) new BYTE[SizeBuffer];
		lpUndoInsertString->ActionUndo = auhInsertString;
		lpUndoInsertString->FilePos = AFilePos;
		lpUndoInsertString->Count = Count;
		memcpy(lpUndoInsertString->szText, str, (int) Count);
		UndoRedo->MemoRedo(this, lpUndoInsertString, SizeBuffer);
		delete[] lpUndoInsertString;

	}

	if (AllocatedSize < FileSize + Count) {
		char *NewFileContent;

		AllocatedSize = FileSize + Count + 1024;
		NewFileContent = new char[(unsigned int) AllocatedSize];
		memcpy(NewFileContent, FileContent, (unsigned int) AFilePos);
		memcpy(&NewFileContent[AFilePos + Count], &FileContent[AFilePos], (unsigned int) (FileSize - AFilePos));
		delete[] FileContent;
		FileContent = NewFileContent;
	}
	else {
		__int64 i;

		for (i = FileSize; i >= AFilePos; i--) {
			FileContent[i + Count] = FileContent[i];
		}
	}
	memcpy(&FileContent[AFilePos], str, (int) Count);
	FileSize += Count;

	UpdateVScroll();
	InvalidateOrSetFlag();
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

}

//---------------------------------------------------------------------------
void THexaEditor::ChangeString(__int64 AFilePos, __int64 Count, char *str) {
	AnsiString asLine;
	LPUNDOCHANGESTRINGHEXA lpUndoChangeString;
	int SizeBuffer;


	if (AFilePos + Count > FileSize) return;

	if (CurrentTypeUndo != tuhNull) {

		// Mémorisation du Undo
		SizeBuffer = sizeof(UNDOCHANGESTRINGHEXA) + (int) Count * sizeof(TCHAR);
		lpUndoChangeString = (LPUNDOCHANGESTRINGHEXA) new BYTE[SizeBuffer];
		lpUndoChangeString->ActionUndo = auhChangeString;
		lpUndoChangeString->FilePos = AFilePos;
		memcpy(lpUndoChangeString->szText, &FileContent[AFilePos], (int) Count);
		lpUndoChangeString->Count = Count;
		UndoRedo->MemoUndo(this, lpUndoChangeString, SizeBuffer);

		// Mémorisation du Redo
		lpUndoChangeString->ActionUndo = auhChangeString;
		lpUndoChangeString->FilePos = AFilePos;
		memcpy(lpUndoChangeString->szText, str, (int) Count);
		UndoRedo->MemoRedo(this, lpUndoChangeString, SizeBuffer);
		delete[] lpUndoChangeString;

	}

	memcpy(&FileContent[AFilePos], str, (int) Count);

	InvalidateOrSetFlag();
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}
	OnChange(this);

}

//---------------------------------------------------------------------------
bool THexaEditor::GetFlag(__int64 AFilePos, TEditorLinesFlag Flag) {
	int i;


	if (Flag == lfBookmark) {
		for (i = 0; i < (int) Bookmarks.size(); i++) {
			if (Bookmarks[i] == AFilePos) return true;
			if (Bookmarks[i] > AFilePos) break;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool THexaEditor::SetFlag(__int64 AFilePos, TEditorLinesFlag Flag, bool bSet) {
	int i;
	int NumLine;


	if (Flag == lfBookmark) {

		for (i = 0; i < (int) Bookmarks.size(); i++) {
			if (Bookmarks[i] == AFilePos) {
				if (!bSet) {
					Bookmarks.erase(Bookmarks.begin() + i);
					NumLine = (int) (AFilePos / NbBytesPerLine);
					DrawLine(Canvas, NumLine);
				}
				return true;
			}
			if (Bookmarks[i] > AFilePos) break;
		}

		if (bSet) {
			Bookmarks.insert(Bookmarks.begin() + i, AFilePos);
			NumLine = (int) (AFilePos / NbBytesPerLine);
			DrawLine(Canvas, NumLine);
			return true;
		}

	}

	return false;
}

//---------------------------------------------------------------------------
bool THexaEditor::ClearFlag(TEditorLinesFlag Flag) {

	if (Flag == lfBookmark) {

		if (Bookmarks.size() != 0) {
			Bookmarks.clear();
			InvalidateOrSetFlag();
		}

		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// Undo/redo
//---------------------------------------------------------------------------
bool THexaEditor::ProcessUndo(void * Undo, int LenUndo) {
	LPUNDOMOVECURHEXA lpUndoMoveCur;
	LPUNDODELETESTRINGHEXA lpUndoDeleteString;
	LPUNDOINSERTSTRINGHEXA lpUndoInsertString;
	LPUNDOCHANGESTRINGHEXA lpUndoChangeString;


	ACTIONUNDOHEXA ActionUndo = *(ACTIONUNDOHEXA *) Undo;

	switch (ActionUndo) {

		case auhMoveCur:
			lpUndoMoveCur = (LPUNDOMOVECURHEXA) Undo;
			FFirstLine = lpUndoMoveCur->FirstLine;
			FFilePos = lpUndoMoveCur->FilePos;
			FilePosBegSel = lpUndoMoveCur->FilePosBegSel;
			FilePosEndSel = lpUndoMoveCur->FilePosEndSel;
			bRightColumn = lpUndoMoveCur->bRightColumn;
			bHighValue = lpUndoMoveCur->bHighValue;
			InvalidateOrSetFlag();
			SetCurrentCaretPos();
			OnCaretMove(this, FFilePos);
			return true;

		case auhDeleteString:
			lpUndoDeleteString = (LPUNDODELETESTRINGHEXA) Undo;
			DeleteString(lpUndoDeleteString->FilePos, lpUndoDeleteString->Count);
			InvalidateOrSetFlag();
			return true;

		case auhInsertString:
			lpUndoInsertString = (LPUNDOINSERTSTRINGHEXA) Undo;
			InsertString(lpUndoInsertString->FilePos, lpUndoInsertString->Count, lpUndoInsertString->szText);
			InvalidateOrSetFlag();
			return true;

		case auhChangeString:
			lpUndoChangeString = (LPUNDOCHANGESTRINGHEXA) Undo;
			ChangeString(lpUndoChangeString->FilePos, lpUndoChangeString->Count, lpUndoChangeString->szText);
			InvalidateOrSetFlag();
			return true;

	}

	return false;
}

bool THexaEditor::ProcessRedo(void * Redo, int LenRedo) {
	// Les commandes undo et redo sont les mêmes
	return ProcessUndo(Redo, LenRedo);
}

//---------------------------------------------------------------------------
bool THexaEditor::SetTypeUndoRedo(TYPEUNDOHEXA NewTypeUndo, AnsiString asOperationName) {
	UNDOMOVECURHEXA UndoMoveCur;


	if (!FUndoRedoMoveCur) {
		if (NewTypeUndo == tuhMoveCur) NewTypeUndo = tuhNull;
	}

	if (NewTypeUndo != CurrentTypeUndo) {

		if (CurrentTypeUndo != tuhNull) {

			// Mémorisation du Redo
			UndoMoveCur.ActionUndo = auhMoveCur;
			UndoMoveCur.FirstLine = FFirstLine;
			UndoMoveCur.FilePos = FFilePos;
			UndoMoveCur.FilePosBegSel = FilePosBegSel;
			UndoMoveCur.FilePosEndSel = FilePosEndSel;
			UndoMoveCur.bRightColumn = bRightColumn;
			UndoMoveCur.bHighValue = bHighValue;
			UndoRedo->MemoRedo(this, &UndoMoveCur, sizeof(UNDOMOVECURHEXA));

			switch (CurrentTypeUndo) {

				case tuhMoveCur:
					break;

				case tuhText:
					break;

				case tuhDelete:
					break;

				case tuhReplace:
					break;

				default:
					FVCL_ASSERT(false && _T("TypeUndo inconnu"));
					break;

			}

			UndoRedo->EndOperation();
			CurrentTypeUndo = tuhNull;

		}

		if (NewTypeUndo != tuhNull) {

			switch (NewTypeUndo) {
				case tuhMoveCur:
					UndoRedo->BeginOperation(UR_MOVECUR, _TT("Déplacement curseur"));
					break;

				case tuhText:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Saisie texte"));
					break;

				case tuhDelete:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Suppression de texte"));
					break;

				case tuhReplace:
					UndoRedo->BeginOperation(UR_MODIF, _TT("Chercher / remplacer"));
					break;

				default:
					FVCL_ASSERT(false && _T("TypeUndo inconnu"));
					break;

			}

			// Mémorisation du Undo
			UndoMoveCur.ActionUndo = auhMoveCur;
			UndoMoveCur.FirstLine = FFirstLine;
			UndoMoveCur.FilePos = FFilePos;
			UndoMoveCur.FilePosBegSel = FilePosBegSel;
			UndoMoveCur.FilePosEndSel = FilePosEndSel;
			UndoMoveCur.bRightColumn = bRightColumn;
			UndoMoveCur.bHighValue = bHighValue;
			UndoRedo->MemoUndo(this, &UndoMoveCur, sizeof(UNDOMOVECURHEXA));

			CurrentTypeUndo = NewTypeUndo;

		}

	}

	return true;
}


//---------------------------------------------------------------------------
bool FASTCALL THexaEditor::BeginOperation(AnsiString asOperationName) {

	SetTypeUndoRedo(tuhExterne, asOperationName);

	return true;
}


//---------------------------------------------------------------------------
bool FASTCALL THexaEditor::EndOperation(void) {

	SetTypeUndoRedo(tuhNull);

	return true;
}


//---------------------------------------------------------------------------
// Effectue une opération undo
//---------------------------------------------------------------------------
bool FASTCALL THexaEditor::Undo(void) {
	AnsiString asOperation;
	bool bRet;

	// Interruption d'une éventuelle opération en cours
	SetTypeUndoRedo(tuhNull);

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
bool FASTCALL THexaEditor::Redo(void) {
	AnsiString asOperation;
	bool bRet;

	// Interruption d'une éventuelle opération en cours
	SetTypeUndoRedo(tuhNull);

	BeginUpdate();
	bRet = UndoRedo->Redo(&asOperation);
	EndUpdate();
	OnMessage(this, asOperation);

	return bRet;
}

//---------------------------------------------------------------------------
// Chercher un texte
//---------------------------------------------------------------------------
bool FASTCALL THexaEditor::Search(AnsiString asSearch, LPSEARCHPARAM lpSearchParam) {
	__int64 PosSearch0, PosSearch;
	int LenSearch;
	bool bExitLoop;
	CharString csSearch = asSearch.AsCharString();


	LenSearch = csSearch.Length();

	if (FileSize < LenSearch) return false;

	if (lpSearchParam->SinceCursor) {
		if (lpSearchParam->Down) {
			PosSearch = FFilePos;
		}
		else {
			PosSearch = FFilePos - LenSearch - 1;
		}
	}
	else {
		if (lpSearchParam->Down) {
			PosSearch = 0;
		}
		else {
			PosSearch = FileSize - LenSearch;
		}
	}

	PosSearch0 = PosSearch;
	bExitLoop = false;
	do {

		if (lpSearchParam->Down) {
			if (PosSearch >= FileSize) {
				if (!lpSearchParam->Loop) {
					bExitLoop = true;
					break;
				}
				PosSearch = 0;
				OnPassEnd(this);
				if (PosSearch == PosSearch0) {
					bExitLoop = true;
					break;
				}
			}
		}
		else {
			if (PosSearch < 0) {
				if (!lpSearchParam->Loop) {
					bExitLoop = true;
					break;
				}
				PosSearch = FileSize - LenSearch;
				OnPassBegin(this);
				if (PosSearch == PosSearch0) {
					bExitLoop = true;
					break;
				}
			}
		}

		bool bFound;

		// Test égalité de la chaîne
		const char *szString = &FileContent[PosSearch];
		if (lpSearchParam->CaseSensitive) {
			bFound = !memcmp(szString, (LPCSTR) csSearch, LenSearch * sizeof(char));
		}
		else {
			bFound = !_memicmp(szString, (LPCSTR) csSearch, LenSearch * sizeof(char));
		}

		if (bFound) {
			SetTypeUndoRedo(tuhMoveCur);
			bRightColumn = true;
			MoveCur(PosSearch, true, false);
			MoveCur(PosSearch + LenSearch, true, true);
			return true;
		}

		if (lpSearchParam->Down) PosSearch++; else PosSearch--;

		if (PosSearch == PosSearch0) bExitLoop = true;

	} while (!bExitLoop);


	return false;
}

//---------------------------------------------------------------------------
// Remplacer un texte
//---------------------------------------------------------------------------
bool FASTCALL THexaEditor::Replace(AnsiString asReplace, bool bKeepCase) {

	return true;
}

//---------------------------------------------------------------------------
// Efface tout le texte du contrôle de saisie.
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::Clear(void) {
}

//---------------------------------------------------------------------------
// Copie le texte sélectionné dans une chaîne.
//---------------------------------------------------------------------------

AnsiString FASTCALL THexaEditor::GetSelText(void) {
	__int64 Adr;
	TCHAR szBuf[3];
	AnsiString asSelText;
	__int64 FilePosBegSel2;
	__int64 FilePosEndSel2;


	if (FilePosBegSel != FilePosEndSel) {

		GetSelOrdered(&FilePosBegSel2, &FilePosEndSel2);

		// Ok, il y a une sélection
		if (!bRightColumn) {
			for (Adr = FilePosBegSel2; Adr < FilePosEndSel2; Adr++) {
				if (Adr > FilePosBegSel2) asSelText += _T(" ");
				wsprintf(szBuf, _T("%02X"), (BYTE) FileContent[Adr]);
				asSelText += szBuf;
			}
		}
		else {
			for (Adr = FilePosBegSel2; Adr < FilePosEndSel2; Adr++) {
				asSelText += (BYTE) FileContent[Adr];
			}
		}

	}

	return asSelText;
}

//---------------------------------------------------------------------------
// Copie le texte sélectionné dans un tampon.
//---------------------------------------------------------------------------

int FASTCALL THexaEditor::GetSelTextBuf(TCHAR * Buffer, int BufSize) {
	AnsiString asBuf = GetSelText();
	return lstrlen(lstrcpyn(Buffer, asBuf, BufSize));
}

//---------------------------------------------------------------------------
// Sélectionne tout le texte.
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::SelectAll(void) {
}


//---------------------------------------------------------------------------
// Remplace le texte sélectionné par une chaîne à zéro terminal.
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::SetSelTextBuf(const TCHAR * Buffer) {
}


//---------------------------------------------------------------------------
// Début d'une série de modifications (pas de raffraichissement).
//---------------------------------------------------------------------------
void FASTCALL THexaEditor::BeginUpdate(void) {
	if (NbBeginUpdate++ == 0) bMustRefresh = false;
}

//---------------------------------------------------------------------------
// Fin d'une série de modifications (raffraichissement éventuel).
//---------------------------------------------------------------------------
void FASTCALL THexaEditor::EndUpdate(void) {
	if (--NbBeginUpdate == 0) {
		if (bMustRefresh) {
			ScrollInView(FFilePos);
			InvalidateOrSetFlag();
		}
	}
}

//---------------------------------------------------------------------------
// Traitement de l'évènement OnResize
//---------------------------------------------------------------------------

void FASTCALL THexaEditor::ProcessResize(void) {
	TRect DrawRect = ClientRect;
	if (FIconsColumn) DrawRect.Left = DrawRect.Left + 24;
	NbLinesWin = DrawRect.Height / (tmHeight + tmInternalLeading);
	NbColsWin = (DrawRect.Width - GetSystemMetrics(SM_CXVSCROLL)) / tmAveCharWidth + 1;
	NbBytesPerLine = 1;
	while (NbBytesPerLine <= (NbColsWin - 13) / 8) NbBytesPerLine *= 2;
	LinesCount = (int) (FileSize / NbBytesPerLine + 1);
	ScrollInView(FFilePos);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void THexaEditor::GetListProperties(TStrings *ListProperties) {

	TCustomEditor::GetListProperties(ListProperties);

	ListProperties->Add(_T("FilePos"));
	ListProperties->Add(_T("FirstLine"));
	ListProperties->Add(_T("IconsColumn"));
	ListProperties->Add(_T("SelText"));
	ListProperties->Add(_T("UndoRedoMoveCur"));
	ListProperties->Add(_T("OnCaretMove"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY THexaEditor::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("FilePos")) {
		return tpNumber;
	}
	if (asProperty == _T("FirstLine")) {
		return tpNumber;
	}
	if (asProperty == _T("IconsColumn")) {
		return tpBool;
	}
	if (asProperty == _T("SelText")) {
		return tpText;
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		return tpBool;
	}
	if (asProperty == _T("OnCaretMove")) {
    *asInfos = _T("(TObject *Sender, __int64 Pos)");
		return tpEvent;
	}
	return TCustomEditor::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString THexaEditor::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("FilePos")) {
		return _T("0");
	}
	if (asProperty == _T("FirstLine")) {
		return _T("0");
	}
	if (asProperty == _T("IconsColumn")) {
		return _T("True");
	}
	if (asProperty == _T("SelText")) {
		return _T("");
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		return _T("True");
	}
	if (asProperty == _T("OnCaretMove")) {
		return _T("");
	}
	return TCustomEditor::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString THexaEditor::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("FilePos")) {
		return IntToStr((unsigned int) Get_FilePos());
	}
	if (asProperty == _T("FirstLine")) {
		return IntToStr((unsigned int) Get_FirstLine());
	}
	if (asProperty == _T("IconsColumn")) {
		if (Get_IconsColumn()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("SelText")) {
		return Get_SelText();
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		if (Get_UndoRedoMoveCur()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("OnCaretMove")) {
		return OnCaretMove.AsString();
	}
	return TCustomEditor::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool THexaEditor::SetProperty(AnsiString asProperty, AnsiString asValue,
															TPersistent *Sender) {
	if (asProperty == _T("FilePos")) {
		Set_FilePos(asValue.ToIntDef(0));
		return true;
	}
	if (asProperty == _T("FirstLine")) {
		Set_FirstLine(asValue.ToIntDef(0));
		return true;
	}
	if (asProperty == _T("IconsColumn")) {
		Set_IconsColumn(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("SelText")) {
		Set_SelText(asValue);
		return true;
	}
	if (asProperty == _T("UndoRedoMoveCur")) {
		Set_UndoRedoMoveCur(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("OnCaretMove")) {
		OnCaretMove.ObjCall = Sender;
		OnCaretMove = asValue.c_str();
		return true;
	}
	return TCustomEditor::SetProperty(asProperty, asValue, Sender);
}
//---------------------------------------------------------------------------
