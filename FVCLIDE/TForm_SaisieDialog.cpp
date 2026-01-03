//-----------------------------------------------------------------------------
//! @file TForm_SaisieDialog.cpp
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



//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------

#include <FreeVCL.h>

//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------

#include "TControlSizer.h"
#include "TControlSizer_Collection.h"
#include "TFrame_SaisieDialog.h"
#include "TForm_SaisieDialog.h"
#include "TElemBase.h"


//---------------------------------------------------------------------------
TForm_SaisieDialog::TForm_SaisieDialog(TComponent *Owner, LPCTSTR szName)
		: TForm(Owner, szName, FVCL_ARG_SUPP) {

  // Initialisations
  hCursorWE = LoadCursor(NULL, IDC_SIZEWE);
  hCursorNS = LoadCursor(NULL, IDC_SIZENS);
  hCursorNESW = LoadCursor(NULL, IDC_SIZENESW);
  hCursorNWSE = LoadCursor(NULL, IDC_SIZENWSE);
  hCursorMove = LoadCursor(NULL, IDC_SIZEALL);
  hCursorArrow = LoadCursor(NULL, IDC_ARROW);
  hCursorCross = LoadCursor(NULL, IDC_CROSS);
	FElemBase = NULL;
	FFrame_SaisieDialog = NULL;
	FIndexDialog = -1;
	IdClick = -1;

	LastWidth = -1;
	LastHeight = -1;

  bDrawRect = false;

	shared_ptr_ControlSizer_Collection =
			std::shared_ptr<TControlSizer_Collection>(new TControlSizer_Collection(this));
	TControlSizer_Collection *ControlSizers;
	ControlSizers = shared_ptr_ControlSizer_Collection.get();
  SET_EVENT(ControlSizers, TClientToScreen, ClientToScreen_Collection, TForm_SaisieDialog, this, ClientToScreen);
  SET_EVENT(ControlSizers, TPlanToClient, PlanToClient_Collection, TForm_SaisieDialog, this, PlanToClient);
  SET_EVENT(ControlSizers, TClientToPlan, ClientToPlan_Collection, TForm_SaisieDialog, this, ClientToPlan);

	// Capture permanente de la souris
	Mouse->Capture = FHandle;

}

//---------------------------------------------------------------------------
TForm_SaisieDialog::~TForm_SaisieDialog(void) {
  Mouse->Capture = NULL;

	shared_ptr_ControlSizer_Collection.reset();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ElemBase
//---------------------------------------------------------------------------

TElemBase *TForm_SaisieDialog::Get_ElemBase(void) {
  return FElemBase;
}

bool TForm_SaisieDialog::Set_ElemBase(TElemBase *NewElemBase) {

  if (FElemBase != NewElemBase) {
    FElemBase = NewElemBase;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Frame_SaisieDialog
//---------------------------------------------------------------------------

TFrame_SaisieDialog *TForm_SaisieDialog::Get_Frame_SaisieDialog(void) {
  return FFrame_SaisieDialog;
}

bool TForm_SaisieDialog::Set_Frame_SaisieDialog(TFrame_SaisieDialog *NewFrame_SaisieDialog) {

  if (FFrame_SaisieDialog != NewFrame_SaisieDialog) {
    FFrame_SaisieDialog = NewFrame_SaisieDialog;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété IndexDialog
//---------------------------------------------------------------------------

int TForm_SaisieDialog::Get_IndexDialog(void) {
  return FIndexDialog;
}

bool TForm_SaisieDialog::Set_IndexDialog(int NewIndexDialog) {

  if (FIndexDialog != NewIndexDialog) {
    FIndexDialog = NewIndexDialog;
  }
  return true;
}

//---------------------------------------------------------------------------
// Dessin de la grille et des sizers à l'écran
//---------------------------------------------------------------------------

void TForm_SaisieDialog::FormPaint(TObject *Sender) {
  TRect Rect;
  HDC hdc;


	Rect = ClientRect;
	hdc = Canvas->Handle;
	TraitAfterPaint(hdc, Rect);

}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::FormResize(TObject *Sender) {
	UNDODIALRESIZE UndoResize;


	if (FFrame_SaisieDialog) {

		FFrame_SaisieDialog->UndoRedo->BeginOperation(0, _TT("Redimensionnement fenêtre principale"));

		// Mémorisation du Undo
		UndoResize.ActionUndoDial = auResize;
		UndoResize.IndexDialog = FIndexDialog;
		UndoResize.Left = FLeft;
		UndoResize.Top = FTop;
		UndoResize.Width = LastWidth;
		UndoResize.Height = LastHeight;
		FFrame_SaisieDialog->UndoRedo->MemoUndo(FFrame_SaisieDialog, &UndoResize, sizeof(UndoResize));

		// Mémorisation du Redo
		UndoResize.ActionUndoDial = auResize;
		UndoResize.IndexDialog = FIndexDialog;
		UndoResize.Left = FLeft;
		UndoResize.Top = FTop;
		UndoResize.Width = FWidth;
		UndoResize.Height = FHeight;
		FFrame_SaisieDialog->UndoRedo->MemoRedo(FFrame_SaisieDialog, &UndoResize, sizeof(UndoResize));

		FFrame_SaisieDialog->UndoRedo->EndOperation();

	}

	LastWidth = FWidth;
	LastHeight = FHeight;

	AfterResize();

	OnChange(this);

}

//---------------------------------------------------------------------------
bool TForm_SaisieDialog::TraitAfterPaint(HDC hdc, TRect &Rect) {
  int i, j;
	bool bAlignGrid = false;
	bool bShowGrid = false;
	int GridSizeX = 0;
	int GridSizeY = 0;
	bool OrdreTab = false;
	HBRUSH hBrush;
	HPEN hOldPen;
	RECT rc;
	int X, Y;
  TElemBase *ElemBase;


	if (FFrame_SaisieDialog) {
		bAlignGrid = FFrame_SaisieDialog->AlignGrid;
		bShowGrid = FFrame_SaisieDialog->ShowGrid;
		GridSizeX = FFrame_SaisieDialog->GridSizeX;
		GridSizeY = FFrame_SaisieDialog->GridSizeY;
		OrdreTab = FFrame_SaisieDialog->OrdreTab;
	}
  if (bAlignGrid && bShowGrid && GridSizeX && GridSizeY) {
    for (i = Rect.Left; i < Rect.Right; i += GridSizeX) {
      for (j = Rect.Top; j < Rect.Bottom; j += GridSizeY) {
        SetPixel(hdc, i, j, RGB(0, 0, 0));
      }
    }
  }

	TControlSizer_Collection *ControlSizers;
	ControlSizers = shared_ptr_ControlSizer_Collection.get();
	for (i = 1; i < ControlSizers->Count; i++) {
    ControlSizers->Item[i]->Paint(hdc);
  }

	if (OrdreTab) {
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		hOldPen = (HPEN) SelectObject(hdc, GetStockObject(BLACK_PEN));
		SelectObject(hdc, hBrush);
		SetTextColor(hdc, RGB(255, 255, 255));


		for (i = 1; i < ControlSizers->Count; i++) {
			ElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
			AnsiString asTabStop;
			AnsiString asTabOrder;
			asTabStop = ElemBase->GetProperty(_T("TabStop"));
			asTabOrder = ElemBase->GetProperty(_T("TabOrder"));
			if (asTabStop == _T("True") && asTabOrder != _T("-1")) {
				X = ControlSizers->Item[i]->X;
				Y = ControlSizers->Item[i]->Y;
				rc.left = X;
				rc.top = Y - 16;
				rc.right = X + 32;
				rc.bottom =  Y;
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				DrawText(hdc, asTabOrder.c_str(), asTabOrder.Length(), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
		}

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		DeleteObject(hBrush);

	}

	if (LastWidth == -1 && LastHeight == -1) {
		LastWidth = FWidth;
		LastHeight = FHeight;
	}

	return true;
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y) {
  switch (Button) {
    case mbLeft: TraitLButtonDown(X, Y); break;
    case mbRight: TraitRButtonDown(X, Y); break;
  }
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y) {
	if (!bDrawRect && Mouse->Capture == FHandle) {
		TRect Rect = ClientRect;
		if (X < Rect.Left || Rect.Right <= X ||
				Y < Rect.Top || Rect.Bottom <= Y) {
			Mouse->Capture = NULL;
			return;
		}
	}
	else if (GetActiveWindow() == FHandle) {
		Mouse->Capture = FHandle;
	}

  TraitMouseMove(X, Y);
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y) {
  switch (Button) {
    case mbLeft: TraitLButtonUp(X, Y); break;
  }
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::FormClose(TObject *Sender,
      TCloseAction &Action) {
  Action = caHide;
}


//---------------------------------------------------------------------------
//                            METHODES PROTECTED
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool TForm_SaisieDialog::Set_Width(int NewWidth) {

  if (FWidth != NewWidth) {
		TForm::Set_Width(NewWidth);
		AfterResize();
  }

  return true;
}

//---------------------------------------------------------------------------
bool TForm_SaisieDialog::Set_Height(int NewHeight) {

  if (FHeight != NewHeight) {
		TForm::Set_Height(NewHeight);
		AfterResize();
  }

  return true;
}

//---------------------------------------------------------------------------
// Dessin d'un rectangle
//---------------------------------------------------------------------------

void TForm_SaisieDialog::DrawRect(void) {
  HDC hdc;
  POINT Pt1, Pt2;


  hdc = GetDC(NULL);
  SetROP2(hdc, R2_NOT);
  Pt1.x = rcDrawRect.left;
  Pt1.y = rcDrawRect.top;
  Pt2.x = rcDrawRect.right;
  Pt2.y = rcDrawRect.bottom;
  ClientToScreen(this, &Pt1);
  ClientToScreen(this, &Pt2);
  SelectObject(hdc, GetStockObject(BLACK_PEN));
  SelectObject(hdc, GetStockObject(NULL_BRUSH));
  if (Pt1.x > Pt2.x) Pt1.x++; else Pt2.x++;
  if (Pt1.y > Pt2.y) Pt1.y++; else Pt2.y++;
  Rectangle(hdc, Pt1.x, Pt1.y, Pt2.x, Pt2.y);
  ReleaseDC(NULL, hdc);
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::AfterResize(void) {
	TElemBase *AElemBase;
	int i;


	TControlSizer_Collection *ControlSizers;
	ControlSizers = shared_ptr_ControlSizer_Collection.get();
	for (i = 1; i < ControlSizers->Count; i++) {
    AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
		AElemBase->UpdateControlSizer();
  }

	AnsiString asHint;
	asHint.sprintf(_T("TForm %s\n%i %i (%i x %i)"),
									(LPCTSTR) ((AnsiString) FName),
									FLeft,
									FTop,
									FWidth,
									FHeight);
	Hint = asHint;

}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::ClientToScreen(TObject *Sender, LPPOINT lpPoint) {
	::ClientToScreen(FHandle, lpPoint);
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::PlanToClientRect(LPRECT lpRect) {
  POINT Pt1, Pt2;


  Pt1.x = lpRect->left;
  Pt1.y = lpRect->top;
  Pt2.x = lpRect->right;
  Pt2.y = lpRect->bottom;

  PlanToClient(this, &Pt1);
  PlanToClient(this, &Pt2);

  if (Pt1.x < Pt2.x) {
    lpRect->left = Pt1.x;
    lpRect->right = Pt2.x;
  }
  else {
    lpRect->left = Pt2.x;
    lpRect->right = Pt1.x;
  }

  if (Pt1.y < Pt2.y) {
    lpRect->top = Pt1.y;
    lpRect->bottom = Pt2.y;
  }
  else {
    lpRect->top = Pt2.y;
    lpRect->bottom = Pt1.y;
  }
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::ClientToPlanRect(LPRECT lpRect) {
  POINT Pt1, Pt2;


  if (lpRect->left < lpRect->right) {
    Pt1.x = lpRect->left;
    Pt2.x = lpRect->right;
  }
  else {
    Pt1.x = lpRect->right;
    Pt2.x = lpRect->left;
  }

  if (lpRect->top < lpRect->bottom) {
    Pt1.y = lpRect->top;
    Pt2.y = lpRect->bottom;
  }
  else {
    Pt1.y = lpRect->bottom;
    Pt2.y = lpRect->top;
  }

  ClientToPlan(this, &Pt1);
  ClientToPlan(this, &Pt2);

  lpRect->left = Pt1.x;
  lpRect->top = Pt1.y;
  lpRect->right = Pt2.x;
  lpRect->bottom = Pt2.y;
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::ClientToPlanXY(int *X, int *Y) {
  POINT Pt;


  Pt.x = *X;
  Pt.y = *Y;
  ClientToPlan(this, &Pt);
  *X = Pt.x;
  *Y = Pt.y;

}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::PlanToClient(TObject *Sender, LPPOINT lpPoint) {
}

//---------------------------------------------------------------------------
void TForm_SaisieDialog::ClientToPlan(TObject *Sender, LPPOINT lpPoint) {
}

//---------------------------------------------------------------------------
//                            ACTIONS UTILISATEUR
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Mouvement souris
//---------------------------------------------------------------------------

bool TForm_SaisieDialog::TraitMouseMove(int X, int Y) {
  int i;
  ACTION ActionMove, ActionMoveTmp;
	AnsiString asMessage;
	bool bAlignGrid = false;
	int GridSizeX = 0;
	int GridSizeY = 0;
	int XRound, YRound;
	int NbVisibles;
	MODECONSTR ModeConstr = mcSelect;
	bool bResizeComponent = false;
  int XDrawRect;
  int YDrawRect;
  int WidthDrawRect;
  int HeightDrawRect;


	TControlSizer_Collection *ControlSizers;
	ControlSizers = shared_ptr_ControlSizer_Collection.get();

	if (FFrame_SaisieDialog) {
		bAlignGrid = FFrame_SaisieDialog->AlignGrid;
		GridSizeX = FFrame_SaisieDialog->GridSizeX;
		GridSizeY = FFrame_SaisieDialog->GridSizeY;
		ModeConstr = FFrame_SaisieDialog->ModeConstr;
	}
  if (bAlignGrid && GridSizeX) XRound = GridSizeX * (int) ((double) X / GridSizeX + .5); else XRound = X;
  if (bAlignGrid && GridSizeY) YRound = GridSizeY * (int) ((double) Y / GridSizeY + .5); else YRound = Y;
	ActionMove = ControlSizers->Action;
  if (ActionMove == acSizeNW || ActionMove == acSizeN || ActionMove == acSizeNE || ActionMove == acSizeE ||
		  ActionMove == acSizeSE || ActionMove == acSizeS || ActionMove == acSizeSW || ActionMove == acSizeW) {
		NbVisibles = 0;
		for (i = 1; i < ControlSizers->Count; i++) {
			if (ControlSizers->Item[i]->Visible) {
				if (++NbVisibles == 1) {
					XDrawRect = ControlSizers->Item[i]->XDrawRect;
					YDrawRect = ControlSizers->Item[i]->YDrawRect;
					WidthDrawRect = ControlSizers->Item[i]->WidthDrawRect;
					HeightDrawRect = ControlSizers->Item[i]->HeightDrawRect;
				}
			}
		}
		if (NbVisibles == 1) bResizeComponent = true;
	}

  if (bDrawRect) {
		asMessage.sprintf(_T("%i, %i (%i x %i)"), XRound, YRound, abs(XRound - rcDrawRect.left), abs(YRound - rcDrawRect.top));
	}
  else if (bResizeComponent) {
		asMessage.sprintf(_T("%i, %i (%i x %i)"), XDrawRect, YDrawRect, WidthDrawRect, HeightDrawRect);
	}
	else {
		asMessage.sprintf(_T("%i, %i"), XRound, YRound);
	}
	FVCLIDEAutomation->DisplayMessage(td_Position, asMessage);

  switch (ModeConstr) {

  case mcSelect:

    if (bDrawRect) {
      // Sélection dans un rectangle
      DrawRect();
      rcDrawRect.right = X;
      rcDrawRect.bottom = Y;
      DrawRect();
    }

    //--------------------------------------------------
    // Mode sélection (et déplacement/redimensionnement)
    //--------------------------------------------------

    ActionMove = ControlSizers->Action;
		for (i = 1; i < ControlSizers->Count; i++) {
			ActionMoveTmp = ControlSizers->Action;
			ControlSizers->Item[i]->TraitMouseMove(X, Y, XRound, YRound, &ActionMoveTmp);
			if (ActionMoveTmp != ControlSizers->Action) ActionMove = ActionMoveTmp;
		}

    switch (ActionMove) {
    case acSizeNW:
    case acSizeSE:
      SetCursor(hCursorNWSE);
      break;
    case acSizeNE:
    case acSizeSW:
      SetCursor(hCursorNESW);
      break;
    case acSizeN:
    case acSizeS:
      SetCursor(hCursorNS);
      break;
    case acSizeW:
    case acSizeE:
      SetCursor(hCursorWE);
      break;
    case acMove:
      SetCursor(hCursorMove);
      break;
    default:
      SetCursor(hCursorArrow);
      break;
    }

    break;

  case mcCreat:

    //--------------------------------------------------
    // Mode création
    //--------------------------------------------------

    SetCursor(hCursorCross);

    if (bDrawRect) {
      DrawRect();
      rcDrawRect.right = XRound;
      rcDrawRect.bottom = YRound;
      DrawRect();
    }

    break;

  default:

    //--------------------------------------------------
    // Autres (mcNull)
    //--------------------------------------------------

    SetCursor(hCursorArrow);
    break;

  }

  return true;
}

//---------------------------------------------------------------------------
bool TForm_SaisieDialog::TraitLButtonDown(int X, int Y) {
  TElemBase *AElemBase;
  int i;
  ACTION Action;
	bool bAlignGrid = false;
	int GridSizeX = 0;
	int GridSizeY = 0;
	int XRound, YRound;
  int NewSelected;
	MODECONSTR ModeConstr = mcSelect;


	TControlSizer_Collection *ControlSizers;
	ControlSizers = shared_ptr_ControlSizer_Collection.get();

	if (FFrame_SaisieDialog) {
		bAlignGrid = FFrame_SaisieDialog->AlignGrid;
		GridSizeX = FFrame_SaisieDialog->GridSizeX;
		GridSizeY = FFrame_SaisieDialog->GridSizeY;
		ModeConstr = FFrame_SaisieDialog->ModeConstr;
	}
  if (bAlignGrid && GridSizeX) XRound = GridSizeX * (int) ((double) X / GridSizeX + .5); else XRound = X;
  if (bAlignGrid && GridSizeY) YRound = GridSizeY * (int) ((double) Y / GridSizeY + .5); else YRound = Y;

  switch (ModeConstr) {

  case mcSelect:

    //--------------------------------------------------
    // Mode sélection (et déplacement/redimensionnement)
    //--------------------------------------------------

    Action = acNull;
    IdClick = -1;
    for (i = 1; i < ControlSizers->Count; i++) {
      ControlSizers->Item[i]->TraitLButtonDown(X, Y, XRound, YRound, &Action);
      if (IdClick == -1 && Action != acNull) {
				IdClick = i;
				AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[IdClick]->Data);
				FFrame_SaisieDialog->ObjSelected = AElemBase;
			}
    }
    ControlSizers->Action = Action;

    if (Action != acAnchorsLeftTop && Action != acAnchorsRightTop &&
				Action != acAnchorsRightBottom && Action != acAnchorsLeftBottom &&
				Action != acAnchorsLeft && Action != acAnchorsTop &&
        Action != acAnchorsRight && Action != acAnchorsBottom) {

      if (Action != acNull) {

        // Action de déplacement ou de dimensionnement
        for (i = 1; i < ControlSizers->Count; i++) {
          if (ControlSizers->Item[i]->Visible) ControlSizers->Item[i]->DrawRect();
        }

        // Capture de la souris
		    Mouse->Capture = FHandle;

      }
      else {

        // Sélection d'un composant
				NewSelected = ControlSizers->GetItemAt(X, Y);

        if (NewSelected == -1) {

          // Activation sélection dans un rectangle
          rcDrawRect.left = X;
          rcDrawRect.top = Y;
          rcDrawRect.right = X;
          rcDrawRect.bottom = Y;
          bDrawRect = true;
          DrawRect();

          // Capture de la souris
			    Mouse->Capture = FHandle;

        }

      }

    }

    break;

  case mcCreat:

    //--------------------------------------------------
    // Mode création
    //--------------------------------------------------

    // Recherche de l'objet parent
		IdClick = ControlSizers->GetItemAt(X, Y);
    if (IdClick != -1) {
			AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[IdClick]->Data);
			FFrame_SaisieDialog->ObjSelected = AElemBase;
    }

    rcDrawRect.left = XRound;
    rcDrawRect.top = YRound;
    rcDrawRect.right = XRound;
    rcDrawRect.bottom = YRound;
    bDrawRect = true;
    DrawRect();

    // Capture de la souris
    Mouse->Capture = FHandle;

    break;

  }

  return true;
}

//---------------------------------------------------------------------------
bool TForm_SaisieDialog::TraitLButtonUp(int X, int Y) {
  int i;
	int Index;
	int IdClick2;
	TElemBase *AElemBase;
  TElemBase *NewElement;
  TElemBase *ElementParent;
  int Left, Top, Width, Height;
  TAnchors Anchors, BaseAnchors;
  ACTION Action;
	bool bAlignGrid = false;
	int GridSizeX = 0;
	int GridSizeY = 0;
	int XRound, YRound;
  int NewSelected;
	MODECONSTR ModeConstr = mcSelect;


	TControlSizer_Collection *ControlSizers;
	ControlSizers = shared_ptr_ControlSizer_Collection.get();

	if (FFrame_SaisieDialog) {
		bAlignGrid = FFrame_SaisieDialog->AlignGrid;
		GridSizeX = FFrame_SaisieDialog->GridSizeX;
		GridSizeY = FFrame_SaisieDialog->GridSizeY;
		ModeConstr = FFrame_SaisieDialog->ModeConstr;
	}

	if (ControlSizers->Count > 1) {
		Action = ControlSizers->Action;
	}
	else {
		Action = acNull;
	}

	TShiftState ShiftState;
	ShiftState = GetShiftState();

  switch (ModeConstr) {

  case mcSelect:

    if (bDrawRect) {

      //--------------------------------------------------
      // Fin sélection dans un rectangle
      //--------------------------------------------------

      DrawRect();
      bDrawRect = false;
      ClientToPlanRect(&rcDrawRect);

			// Le dialogue ne doit jamais être sélectionné
			AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[0]->Data);
			AElemBase->Selected = false;

      if ((ShiftState.Contains(ssCtrl)) == 0) {
        // Désélection de tous les composants
        for (i = 1; i < ControlSizers->Count; i++) {
					AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
					AElemBase->Selected = false;
        }
      }

			if (ShiftState.Contains(ssShift)) {

				for (i = 1; i < ControlSizers->Count; i++) {
					if (rcDrawRect.left < ControlSizers->Item[i]->X + ControlSizers->Item[i]->Width &&
							rcDrawRect.right > ControlSizers->Item[i]->X &&
				      rcDrawRect.top < ControlSizers->Item[i]->Y + ControlSizers->Item[i]->Height &&
							rcDrawRect.bottom > ControlSizers->Item[i]->Y) {
								AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
								AElemBase->Selected = true;
					}
				}

			}

			else {

				for (i = 1; i < ControlSizers->Count; i++) {
					if (rcDrawRect.left < ControlSizers->Item[i]->X &&
							rcDrawRect.top < ControlSizers->Item[i]->Y &&
							ControlSizers->Item[i]->X + ControlSizers->Item[i]->Width < rcDrawRect.right &&
							ControlSizers->Item[i]->Y + ControlSizers->Item[i]->Height < rcDrawRect.bottom) {
						AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
						AElemBase->Selected = true;
					}
				}

			}

			OnChangeSelection(this);

    }

    else if (Action == acNull) {

			//--------------------------------------------------
      // Sélection d'un composant
      //--------------------------------------------------

			NewSelected = ControlSizers->GetItemAt(X, Y);

      if (NewSelected != -1) {

        if (ShiftState.Contains(ssCtrl)) {
          // Sélection du composant cliqué en plus des autres
					AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[NewSelected]->Data);
					AElemBase->Selected = true;
        }
        else {
          // Sélection du composant cliqué uniquement
					// On commence à zéro parce que le dialogue lui-même peut être sélectionné
          for (i = 0; i < ControlSizers->Count; i++) {
						AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
						AElemBase->Selected = (NewSelected == i);
          }
        }

      }

      else {

        if ((ShiftState.Contains(ssCtrl)) == 0) {
          // Désélection de tous les composants
          for (i = 1; i < ControlSizers->Count; i++) {
						AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
						AElemBase->Selected = false;
          }
        }

			}

			OnChangeSelection(this);

		}

    else if (Action != acAnchorsLeftTop &&
						 Action != acAnchorsRightTop &&
						 Action != acAnchorsRightBottom &&
						 Action != acAnchorsLeftBottom &&
						 Action != acAnchorsLeft &&
						 Action != acAnchorsTop &&
						 Action != acAnchorsRight &&
						 Action != acAnchorsBottom) {

      //--------------------------------------------------
      // Mode sélection (et déplacement/redimensionnement)
      //--------------------------------------------------

      if (bAlignGrid && GridSizeX) XRound = GridSizeX * (int) ((double) X / GridSizeX + .5); else XRound = X;
      if (bAlignGrid && GridSizeY) YRound = GridSizeY * (int) ((double) Y / GridSizeY + .5); else YRound = Y;

			if (FFrame_SaisieDialog) {

				// Nombre et nom de l'objet redimensionné
				AnsiString asNameResized;
				int NbResize = 0;
				for (i = 1; i < ControlSizers->Count; i++) {
					if (ControlSizers->Item[i]->Visible) {
						NbResize++;
						AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
						asNameResized = AElemBase->Name;
					}
				}

				UNDODIALCHANGEVALUE UndoChangeValue;

				if (Action == acMove) {
					if (NbResize == 1) {
						FFrame_SaisieDialog->UndoRedo->BeginOperation(0, _TT("Déplacement objet ") + asNameResized);
					}
					else {
						FFrame_SaisieDialog->UndoRedo->BeginOperation(0, _TT("Déplacement multiple"));
					}
				}
				else {
					if (NbResize == 1) {
						FFrame_SaisieDialog->UndoRedo->BeginOperation(0, _TT("Dimensionnement objet ") + asNameResized);
					}
					else {
						FFrame_SaisieDialog->UndoRedo->BeginOperation(0, _TT("Dimensionnement multiple"));
					}
				}

				for (i = 1; i < ControlSizers->Count; i++) {
					if (ControlSizers->Item[i]->Visible) {

						AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
						Index = FFrame_SaisieDialog->GetIdTreeview(AElemBase);

						// Mémorisation du Undo
						UndoChangeValue.ActionUndoDial = auChangeLeft;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Left;
						FFrame_SaisieDialog->UndoRedo->MemoUndo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));
						UndoChangeValue.ActionUndoDial = auChangeTop;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Top;
						FFrame_SaisieDialog->UndoRedo->MemoUndo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));
						UndoChangeValue.ActionUndoDial = auChangeWidth;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Width;
						FFrame_SaisieDialog->UndoRedo->MemoUndo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));
						UndoChangeValue.ActionUndoDial = auChangeHeight;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Height;
						FFrame_SaisieDialog->UndoRedo->MemoUndo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));

						ControlSizers->Item[i]->TraitLButtonUp(X, Y, XRound, YRound, &Action);

						// Mémorisation du Redo
						UndoChangeValue.ActionUndoDial = auChangeLeft;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Left;
						FFrame_SaisieDialog->UndoRedo->MemoRedo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));
						UndoChangeValue.ActionUndoDial = auChangeTop;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Top;
						FFrame_SaisieDialog->UndoRedo->MemoRedo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));
						UndoChangeValue.ActionUndoDial = auChangeWidth;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Width;
						FFrame_SaisieDialog->UndoRedo->MemoRedo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));
						UndoChangeValue.ActionUndoDial = auChangeHeight;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = AElemBase->Height;
						FFrame_SaisieDialog->UndoRedo->MemoRedo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));

					}
				}

				FFrame_SaisieDialog->UndoRedo->EndOperation();

				// Le dimensionnement d'un objet parent peut modifier les coordonnées absolues des enfants
				// => Appel AfterResize sinon les ControlSizers sont décalés
				AfterResize();

			}

    }

    else {

      //--------------------------------------------------
      // Ancrages
      //--------------------------------------------------

	    if (IdClick != -1) BaseAnchors = ControlSizers->Item[IdClick]->Anchors;

			if (Action == acAnchorsLeftTop) {
				if (BaseAnchors.Contains(akLeft) && BaseAnchors.Contains(akTop)) BaseAnchors >> akLeft >> akTop;
				else BaseAnchors << akLeft << akTop;
			}
			else if (Action == acAnchorsRightTop) {
				if (BaseAnchors.Contains(akRight) && BaseAnchors.Contains(akTop)) BaseAnchors >> akRight >> akTop;
				else BaseAnchors << akRight << akTop;
			}
			else if (Action == acAnchorsRightBottom) {
				if (BaseAnchors.Contains(akRight) && BaseAnchors.Contains(akBottom)) BaseAnchors >> akRight >> akBottom;
				else BaseAnchors << akRight << akBottom;
			}
			else if (Action == acAnchorsLeftBottom) {
				if (BaseAnchors.Contains(akLeft) && BaseAnchors.Contains(akBottom)) BaseAnchors >> akLeft >> akBottom;
				else BaseAnchors << akLeft << akBottom;
			}
			else if (Action == acAnchorsLeft) {
				if (BaseAnchors.Contains(akLeft)) BaseAnchors >> akLeft;
				else BaseAnchors << akLeft;
			}
			else if (Action == acAnchorsTop) {
				if (BaseAnchors.Contains(akTop)) BaseAnchors >> akTop;
				else BaseAnchors << akTop;
			}
			else if (Action == acAnchorsRight) {
				if (BaseAnchors.Contains(akRight)) BaseAnchors >> akRight;
				else BaseAnchors << akRight;
			}
			else if (Action == acAnchorsBottom) {
				if (BaseAnchors.Contains(akBottom)) BaseAnchors >> akBottom;
				else BaseAnchors << akBottom;
			}

			if (FFrame_SaisieDialog) {

				UNDODIALCHANGEVALUE UndoChangeValue;

				FFrame_SaisieDialog->UndoRedo->BeginOperation(0, _TT("Modifications ancrages"));

				for (i = 1; i < ControlSizers->Count; i++) {
					if (ControlSizers->Item[i]->Visible) {

						AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
						Index = FFrame_SaisieDialog->GetIdTreeview(AElemBase);
						Anchors = ControlSizers->Item[i]->Anchors;

						// Mémorisation du Undo
						UndoChangeValue.ActionUndoDial = auChangeAnchors;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = 0;
						if (Anchors.Contains(akLeft)) UndoChangeValue.Value |= 1;
						if (Anchors.Contains(akTop)) UndoChangeValue.Value |= 2;
						if (Anchors.Contains(akRight)) UndoChangeValue.Value |= 4;
						if (Anchors.Contains(akBottom)) UndoChangeValue.Value |= 8;
						FFrame_SaisieDialog->UndoRedo->MemoUndo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));

						if (Action == acAnchorsLeftTop) {
							if (BaseAnchors.Contains(akLeft) && BaseAnchors.Contains(akTop)) Anchors << akLeft << akTop;
							else Anchors >> akLeft >> akTop;
						}
						else if (Action == acAnchorsRightTop) {
							if (BaseAnchors.Contains(akRight) && BaseAnchors.Contains(akTop)) Anchors << akRight << akTop;
							else Anchors >> akRight >> akTop;
						}
						else if (Action == acAnchorsRightBottom) {
							if (BaseAnchors.Contains(akRight) && BaseAnchors.Contains(akBottom)) Anchors << akRight << akBottom;
							else Anchors >> akRight >> akBottom;
						}
						else if (Action == acAnchorsLeftBottom) {
							if (BaseAnchors.Contains(akLeft) && BaseAnchors.Contains(akBottom)) Anchors << akLeft << akBottom;
							else Anchors >> akLeft >> akBottom;
						}
						else if (Action == acAnchorsLeft) {
							if (BaseAnchors.Contains(akLeft)) Anchors << akLeft;
							else Anchors >> akLeft;
						}
						else if (Action == acAnchorsTop) {
							if (BaseAnchors.Contains(akTop)) Anchors << akTop;
							else Anchors >> akTop;
						}
						else if (Action == acAnchorsRight) {
							if (BaseAnchors.Contains(akRight)) Anchors << akRight;
							else Anchors >> akRight;
						}
						else if (Action == acAnchorsBottom) {
							if (BaseAnchors.Contains(akBottom)) Anchors << akBottom;
							else Anchors >> akBottom;
						}

						ControlSizers->Item[i]->Anchors = Anchors;

						// Mémorisation du Redo
						UndoChangeValue.ActionUndoDial = auChangeAnchors;
						UndoChangeValue.Index = Index;
						UndoChangeValue.Value = 0;
						if (Anchors.Contains(akLeft)) UndoChangeValue.Value |= 1;
						if (Anchors.Contains(akTop)) UndoChangeValue.Value |= 2;
						if (Anchors.Contains(akRight)) UndoChangeValue.Value |= 4;
						if (Anchors.Contains(akBottom)) UndoChangeValue.Value |= 8;
						FFrame_SaisieDialog->UndoRedo->MemoRedo(FFrame_SaisieDialog, &UndoChangeValue, sizeof(UndoChangeValue));

					}

				}

				FFrame_SaisieDialog->UndoRedo->EndOperation();

			}

    }

		Mouse->Capture = NULL;

		ControlSizers->Action = acNull;
		Invalidate();

    break;

  case mcCreat:

    //--------------------------------------------------
    // Mode création
    //--------------------------------------------------

    DrawRect();
    bDrawRect = false;

    // Vérification de l'objet parent pour le second point
		IdClick2 = ControlSizers->GetItemAt(X, Y);

		if (IdClick2 != IdClick) {
			IdClick = -1;
			FFrame_SaisieDialog->ObjSelected = NULL;
		}

		Anchors.Clear() << akLeft << akTop << akRight << akBottom;
    ClientToPlanRect(&rcDrawRect);
		if (rcDrawRect.left < rcDrawRect.right) {
			X = rcDrawRect.left;
	    Width = rcDrawRect.right - rcDrawRect.left;
		}
		else {
			X = rcDrawRect.right;
	    Width = rcDrawRect.left - rcDrawRect.right;
		}
		if (rcDrawRect.top < rcDrawRect.bottom) {
			Y = rcDrawRect.top;
			Height = rcDrawRect.bottom - rcDrawRect.top;
		}
		else {
			Y = rcDrawRect.bottom;
			Height = rcDrawRect.top - rcDrawRect.bottom;
		}

    // Objet parent
    if (IdClick != -1) {
			ElementParent = static_cast<TElemBase *>((void *) ControlSizers->Item[IdClick]->Data);
      while (!ElementParent->ParentPotentiel && ElementParent->Parent != NULL) {
        ElementParent = ElementParent->Parent;
      }
    }
    else {
			ElementParent = static_cast<TElemBase *>((void *) ControlSizers->Item[0]->Data);
		}

		// Libération de la souris AVANT la création de l'objet
		// (sinon le curseur reste dans la boîte de dialogue des propriétés)
		Mouse->Capture = NULL;

    // Création et mémorisation de l'objet
		Left = X - ElementParent->X;
		Top = Y - ElementParent->Y;
    NewElement = FFrame_SaisieDialog->CreationObjet(ElementParent,
                           Left, Top, Width, Height, Anchors);
		IdClick = ControlSizers->Count - 1;
    break;

  }

  return true;
}

//---------------------------------------------------------------------------
bool TForm_SaisieDialog::TraitRButtonDown(int X, int Y) {
  TElemBase *AElemBase;
	int i;
  int NewSelected;
  bool IsSelected;


	TControlSizer_Collection *ControlSizers;
	ControlSizers = shared_ptr_ControlSizer_Collection.get();

  // Est-ce qu'on a cliqué dans un composant déjà sélectionné ?
  IsSelected = false;
  for (i = 1; i < ControlSizers->Count; i++) {
    if (ControlSizers->Item[i]->Visible) {
      if (ControlSizers->Item[i]->IsInFrame(X, Y)) {
        IdClick = i;
				AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[IdClick]->Data);
				FFrame_SaisieDialog->ObjSelected = AElemBase;
        IsSelected = true;
      }
    }
  }

  if (!IsSelected) {

    // Sélection d'un composant
		NewSelected = ControlSizers->GetItemAt(X, Y);
		// Dialogue (Index 0) sélectionné en l'absence d'un autre
		if (NewSelected == -1) NewSelected = 0;
    IdClick = NewSelected;
		AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[IdClick]->Data);
		FFrame_SaisieDialog->ObjSelected = AElemBase;

		TShiftState ShiftState;
		ShiftState = GetShiftState();

    if (ShiftState.Contains(ssCtrl)) {
      // Sélection du composant cliqué en plus des autres
			AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[NewSelected]->Data);
			AElemBase->Selected = true;
    }
    else {
      // Sélection du composant cliqué uniquement
			// On commence à zéro parce que le dialogue lui-même peut être sélectionné
      for (i = 0; i < ControlSizers->Count; i++) {
				AElemBase = static_cast<TElemBase *>((void *) ControlSizers->Item[i]->Data);
				AElemBase->Selected = (NewSelected == i);
      }
    }

		OnChangeSelection(this);
		Invalidate();

  }

  return Frame_SaisieDialog->AffPopupMenu();
}

//---------------------------------------------------------------------------
