//-----------------------------------------------------------------------------
//! @file TControl.cpp
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


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TPopup;


//---------------------------------------------------------------------------
// TControl
//---------------------------------------------------------------------------

TControl::TControl(TComponent* AOwner): TComponent(AOwner) {

  // Initialisations
  FCaption = _T("");
  FClassName = _T("");
  FName = _T("");
  FParent = NULL;
  FHandle = NULL;
  FVisible = true;
  FEnabled = true;
  if (AOwner) {
    FParent = dynamic_cast<TControl *>(AOwner);
    if (FParent) FVisible = FParent->Visible;
  }
  FLeft = 0;
  FTop = 0;
  FWidth = 0;
  FHeight = 0;
  FConstraints = new TSizeConstraints(this);
  FAnchors.Clear() << akLeft << akTop;
  FAlign = alNone;
  FAlignDisabled = 0;
  FColor = clBtnFace;
  FCursor = crDefault;
  FAutoSize = false;
  FPopupMenu = NULL;
  FShowHint = false;
  FParentShowHint = false;
  HintWindow = NULL;
	FAction = NULL;
	FDragMode = dmManual;
	bDragAndDrop = false;
	bDragNotImmediate = false;
	DragObject = NULL;

  FFont = new TFont();
  FFont->Handle = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
}

TControl::~TControl(void) {
  delete FConstraints;
  delete FFont;
  delete HintWindow;
}

//---------------------------------------------------------------------------
int TControl::ProcessNotify(LPNMHDR pnmh) {

  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TControl::Get_Handle(void) {
  return FHandle;
}

bool TControl::Set_Handle(HWND NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Left
//---------------------------------------------------------------------------

int TControl::Get_Left(void) {
  return FLeft;
}

bool TControl::Set_Left(int NewLeft) {
  if (FLeft != NewLeft) {
    FLeft = NewLeft;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Top
//---------------------------------------------------------------------------

int TControl::Get_Top(void) {
  return FTop;
}

bool TControl::Set_Top(int NewTop) {
  if (FTop != NewTop) {
    FTop = NewTop;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TControl::Get_Width(void) {
  return FWidth;
}

bool TControl::Set_Width(int NewWidth) {
  int MinWidth;
  int MaxWidth;


  MinWidth = FConstraints->MinWidth;
  MaxWidth = FConstraints->MaxWidth;
  if (MinWidth != 0 && NewWidth < MinWidth) NewWidth = MinWidth;
  if (MaxWidth != 0 && NewWidth > MaxWidth) NewWidth = MaxWidth;

  if (FWidth != NewWidth) {
    if (FAlignDisabled == 0) {
      SizeSubComponents(NewWidth - FWidth, 0, false);
    }
    FWidth = NewWidth;
    if (FAlignDisabled == 0 && FParent && FAlign == alLeft || FAlign == alRight) {
			FParent->SizeSubComponents(FParent->Width, FParent->Height, false);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TControl::Get_Height(void) {
  return FHeight;
}

bool TControl::Set_Height(int NewHeight) {
  int MinHeight;
  int MaxHeight;


  MinHeight = FConstraints->MinHeight;
  MaxHeight = FConstraints->MaxHeight;
  if (MinHeight != 0 && NewHeight < MinHeight) NewHeight = MinHeight;
  if (MaxHeight != 0 && NewHeight > MaxHeight) NewHeight = MaxHeight;

  if (FHeight != NewHeight) {
    if (FAlignDisabled == 0) {
      SizeSubComponents(0, NewHeight - FHeight, false);
    }
    FHeight = NewHeight;
    if (FAlignDisabled == 0 && FParent && (FAlign == alTop || FAlign == alBottom)) {
			FParent->SizeSubComponents(FParent->Width, FParent->Height, false);
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientWidth
//---------------------------------------------------------------------------

int TControl::Get_ClientWidth(void) {
  TRect ClientRect = Get_ClientRect();
  return ClientRect.Width;
}

bool TControl::Set_ClientWidth(int NewClientWidth) {
  int OldClientWidth = Get_ClientWidth();
  if (OldClientWidth != NewClientWidth) {
    Set_Width(FWidth + NewClientWidth - OldClientWidth);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientHeight
//---------------------------------------------------------------------------

int TControl::Get_ClientHeight(void) {
  TRect ClientRect = Get_ClientRect();
  return ClientRect.Height;
}

bool TControl::Set_ClientHeight(int NewClientHeight) {
  int OldClientHeight = Get_ClientHeight();
  if (OldClientHeight != NewClientHeight) {
    Set_Height(FHeight + NewClientHeight - OldClientHeight);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Constraints
//---------------------------------------------------------------------------

TSizeConstraints * TControl::Get_Constraints(void) {
  return FConstraints;
}

bool TControl::Set_Constraints(TSizeConstraints * NewConstraints) {
  if (FConstraints != NewConstraints) {
    FConstraints = NewConstraints;
  }
  return true;
}


//---------------------------------------------------------------------------
// Origine de la fenêtre Windows
//---------------------------------------------------------------------------

int TControl::Get_LeftWnd(void) {
  int FLeftWnd = FLeft;
  if (FParent) FLeftWnd += FParent->Get_LeftWnd();
  return FLeftWnd;
}


//---------------------------------------------------------------------------
// Origine de la fenêtre Windows
//---------------------------------------------------------------------------

int TControl::Get_TopWnd(void) {
  int FTopWnd = FTop;
  if (FParent) FTopWnd += FParent->Get_TopWnd();
  return FTopWnd;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Anchors
//---------------------------------------------------------------------------

TAnchors TControl::Get_Anchors(void) {
  return FAnchors;
}

bool TControl::Set_Anchors(TAnchors NewAnchors) {
  if (FAnchors != NewAnchors) {
    FAnchors = NewAnchors;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Align
//---------------------------------------------------------------------------

TAlign TControl::Get_Align(void) {
  return FAlign;
}

bool TControl::Set_Align(TAlign NewAlign) {
  TComponent *ComponentParent;
  TControl *Control;

  if (FAlign != NewAlign) {
    FAlign = NewAlign;
    if (FAlignDisabled == 0) {
      ComponentParent = FParent;
      Control = dynamic_cast<TControl *>(ComponentParent);
      if (Control) Control->SizeSubComponents(0, 0, false);
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AlignDisabled
//---------------------------------------------------------------------------

bool TControl::Get_AlignDisabled(void) {
  return FAlignDisabled != 0;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool TControl::Get_Visible(void) {
  return FVisible;
}

bool TControl::Set_Visible(bool NewVisible) {

  if (FVisible != NewVisible) {
    FVisible = NewVisible;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Enabled
//---------------------------------------------------------------------------

bool TControl::Get_Enabled(void) {
  return FEnabled;
}

bool TControl::Set_Enabled(bool NewEnabled) {

  if (FEnabled != NewEnabled) {
    FEnabled = NewEnabled;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientRect
//---------------------------------------------------------------------------

TRect TControl::Get_ClientRect(void) {
  TRect Retour;


  Retour.Left = 0;
  Retour.Top = 0;
  Retour.Right = FWidth;
  Retour.Bottom = FHeight;

  return Retour;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AutoSize
//---------------------------------------------------------------------------

bool TControl::Get_AutoSize(void) {
  return FAutoSize;
}

bool TControl::Set_AutoSize(bool NewAutoSize) {
  if (FAutoSize != NewAutoSize) {
    FAutoSize = NewAutoSize;
		if (FAutoSize) {
			int NewLeft = FLeft;
			int NewTop = FTop;
			int NewWidth = FWidth;
			int NewHeight = FHeight;
			if (InternalCanAutoSize(NewLeft, NewTop, NewWidth, NewHeight)) {
				Size(NewLeft, NewTop, NewWidth, NewHeight, false);
			}
		}
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------
TColor TControl::Get_Color(void) {
  return FColor;
}

bool TControl::Set_Color(TColor NewColor) {
  if (FColor != NewColor) {
    FColor = NewColor;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Cursor
//---------------------------------------------------------------------------
TCursor TControl::Get_Cursor(void) {
  return FCursor;
}

bool TControl::Set_Cursor(TCursor NewCursor) {
  if (FCursor != NewCursor) {
    FCursor = NewCursor;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

TCustomFont *TControl::Get_Font(void) {
  return FFont;
}

bool TControl::Set_Font(TCustomFont *NewFont) {
  FFont->Assign(NewFont);

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Parent
//---------------------------------------------------------------------------

TControl *TControl::Get_Parent(void) {
  return FParent;
}

bool TControl::Set_Parent(TControl *NewParent) {
  if (FParent != NewParent) {
    FParent = NewParent;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowHint
//---------------------------------------------------------------------------

bool TControl::Get_ShowHint(void) {
  return FShowHint;
}

bool TControl::Set_ShowHint(bool NewShowHint) {

  if (FShowHint != NewShowHint) {
    FShowHint = NewShowHint;
    if (HintWindow) SendMessage(HintWindow->Handle, TTM_ACTIVATE, (WPARAM) (BOOL) InternalShowHint(), 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ParentShowHint
//---------------------------------------------------------------------------

bool TControl::Get_ParentShowHint(void) {
  return FParentShowHint;
}

bool TControl::Set_ParentShowHint(bool NewParentShowHint) {

  if (FParentShowHint != NewParentShowHint) {
    FParentShowHint = NewParentShowHint;
    if (HintWindow) SendMessage(HintWindow->Handle, TTM_ACTIVATE, (WPARAM) (BOOL) InternalShowHint(), 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Hint
//---------------------------------------------------------------------------

AnsiString TControl::Get_Hint(void) {
  return FHint;
}

bool TControl::Set_Hint(AnsiString NewHint) {
  TOOLINFO ToolInfo;


  if (FHint != NewHint) {
    FHint = NewHint;
    if (!HintWindow) {
      // Ne pas faire "new THintWindow(this)" ce qui
      // provoquerait l'enregistrement dans TComponents.
			// Ne pas faire "HintWindow->Parent = this", ça décale l'info-bulle
      HintWindow = new THintWindow(NULL);
      HintWindow->Name = AnsiString(_T("HInt_")) + FName;
    }

    memset(&ToolInfo, 0, sizeof(TOOLINFO));
#if (_WIN32_WINNT >= 0x0501)
    ToolInfo.cbSize = sizeof(TOOLINFO) - sizeof(void *);  // - sizeof(lpReserved) (ça marche pas sans ça)
#else
    ToolInfo.cbSize = sizeof(TOOLINFO);
#endif
    ToolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    ToolInfo.hwnd = FHandle;
    ToolInfo.uId = (UINT_PTR) FHandle;
    ToolInfo.hinst = HInstance;
    ToolInfo.lpszText = const_cast<TCHAR *>((LPCTSTR) FHint);
    SendMessage(HintWindow->Handle, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ToolInfo);

    SendMessage(HintWindow->Handle, TTM_ACTIVATE, (WPARAM) (BOOL) FShowHint, 0);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PopupMenu
//---------------------------------------------------------------------------

TPopupMenu * TControl::Get_PopupMenu(void) {
  return FPopupMenu;
}

bool TControl::Set_PopupMenu(TPopupMenu *NewPopupMenu) {
  if (FPopupMenu != NewPopupMenu) {
    FPopupMenu = NewPopupMenu;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Action
//---------------------------------------------------------------------------

TAction *TControl::Get_Action(void) {
  return FAction;
}

bool TControl::Set_Action(TAction *NewAction) {
  if (FAction != NewAction) {
    FAction = NewAction;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DragMode
//---------------------------------------------------------------------------

TDragMode TControl::Get_DragMode() {
  return FDragMode;
}

bool TControl::Set_DragMode(TDragMode NewDragMode) {
  if (FDragMode != NewDragMode) {
    FDragMode = NewDragMode;
  }
  return true;
}

//---------------------------------------------------------------------------
// Simule un clic sur le bouton
//---------------------------------------------------------------------------
void FASTCALL TControl::Click(void) {
	if (!FComponentState.Contains(csLoading)) {
		if (FAction) FAction->Execute();
		OnClick(this);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TControl::EndDrag(bool Drop) {
}

//---------------------------------------------------------------------------
// Rend le contrôle visible
//---------------------------------------------------------------------------
void FASTCALL TControl::Show(void) {
  Set_Visible(true);
	if (FParent) FParent->Show();
}

//---------------------------------------------------------------------------
// Rend le contrôle invisible
//---------------------------------------------------------------------------
void FASTCALL TControl::Hide(void) {
  Set_Visible(false);
}

//---------------------------------------------------------------------------
// Demande de dessin du composant
//---------------------------------------------------------------------------

bool FASTCALL TControl::Invalidate(void) {
  return true;
}

//---------------------------------------------------------------------------
// Dessin immédiat du composant si besoin
//---------------------------------------------------------------------------

bool FASTCALL TControl::Update(void) {
  return true;
}

//---------------------------------------------------------------------------
// Demande de dessin du composant
//---------------------------------------------------------------------------

bool FASTCALL TControl::Repaint(void) {
  return true;
}

bool FASTCALL TControl::Refresh(void) {
  return Repaint();
}

//---------------------------------------------------------------------------
// Renvoie la longueur du texte du contrôle.
//---------------------------------------------------------------------------
int FASTCALL TControl::GetTextLen(void) {
  return FCaption.Length();
}

//---------------------------------------------------------------------------
// Récupère le texte du contrôle, le copie dans un tampon et renvoie
// le nombre de caractères copiés.
//---------------------------------------------------------------------------
int FASTCALL TControl::GetTextBuf(TCHAR * Buffer, int BufSize) {
  lstrcpyn(Buffer, FCaption, BufSize);
  return FCaption.Length();
}

//---------------------------------------------------------------------------
// Définit le texte du contrôle.
//---------------------------------------------------------------------------
void FASTCALL TControl::SetTextBuf(const TCHAR * Buffer) {
  FCaption = Buffer;
}

//---------------------------------------------------------------------------
//                           METHODES PROTEGEES
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool TControl::InternalShowHint(void) {
	if (FShowHint) return true;
	if (FParent && FParentShowHint) FParent->InternalShowHint();

	return false;
}

//---------------------------------------------------------------------------
// Indique si le contrôle peut se redimensionner automatiquement.
//---------------------------------------------------------------------------
bool FASTCALL TControl::CanAutoSize(int &NewWidth, int &NewHeight) {
	return true;
}

bool TControl::InternalCanAutoSize(int &NewLeft, int &NewTop, int &NewWidth, int &NewHeight) {
	int MemoNewWidth = NewWidth;
	int MemoNewHeight = NewHeight;

	bool bRet = CanAutoSize(NewWidth, NewHeight);

	if (MemoNewWidth != NewWidth &&
		!FAnchors.Contains(akLeft) &&
		FAnchors.Contains(akRight)) {
			NewLeft += MemoNewWidth - NewWidth;
	}

	if (MemoNewHeight != NewHeight &&
		!FAnchors.Contains(akTop) &&
		FAnchors.Contains(akBottom)) {
			NewTop += MemoNewHeight - NewHeight;
	}

	return bRet;
}

//---------------------------------------------------------------------------
void TControl::Size(int NewLeft, int NewTop, int NewWidth, int NewHeight, bool bForceResize) {

	if (FAutoSize) {
		InternalCanAutoSize(NewLeft, NewTop, NewWidth, NewHeight);
	}

  FLeft = NewLeft;
  FTop = NewTop;
  if (NewWidth != FWidth || NewHeight != FHeight || bForceResize) {
    int DX, DY;

    DX = NewWidth - FWidth;
    DY = NewHeight - FHeight;
    FWidth = NewWidth;
    FHeight = NewHeight;
    if (FAlignDisabled == 0) {
      SizeSubComponents(DX, DY, bForceResize);
      OnResize(this);
    }
  }

}

//---------------------------------------------------------------------------
bool TControl::SizeSubComponents(int DX, int DY, bool bForceResize) {
  int i;
  int X1Component, Y1Component, X2Component, Y2Component;
  TControl *SubControl;
  TAnchors SubAnchors;
  TAlign SubAlign;


  TRect ClRect = ClientRect;
  TRect Rect(0, 0, ClRect.Width, ClRect.Height); // Rectangle client sans le décalage de AdjustClientRect (qui sera appliqué dans le SubControl->Size)
  for (i = 0; i < (int) FComponents.size(); i++) {
    SubControl = dynamic_cast<TControl *>(FComponents[i]);
    if (SubControl) {
			X1Component = SubControl->Left;
			Y1Component = SubControl->Top;
			X2Component = SubControl->Left + SubControl->Width;
			Y2Component = SubControl->Top + SubControl->Height;
			SubAlign = SubControl->Align;
			if (SubAlign == alNone) {
				SubAnchors = SubControl->Anchors;
				if (!(SubAnchors.Contains(akLeft))) X1Component += DX;
				if (!(SubAnchors.Contains(akTop))) Y1Component += DY;
				if (SubAnchors.Contains(akRight)) X2Component += DX;
				if (SubAnchors.Contains(akBottom)) Y2Component += DY;
			}
			else if (SubAlign == alTop) {
				X1Component = Rect.Left;
				Y1Component = Rect.Top;
				X2Component = Rect.Right;
				Y2Component = Rect.Top + SubControl->Height;
				if (SubControl->Visible) {
					Rect.Top = Rect.Top + SubControl->Height;
				}
			}
			else if (SubAlign == alBottom) {
				X1Component = Rect.Left;
				Y1Component = Rect.Bottom - SubControl->Height;
				X2Component = Rect.Right;
				Y2Component = Rect.Bottom;
				if (SubControl->Visible) {
					Rect.Bottom = Rect.Bottom - SubControl->Height;
				}
			}
			else if (SubAlign == alLeft) {
				X1Component = Rect.Left;
				Y1Component = Rect.Top;
				X2Component = Rect.Left + SubControl->Width;
				Y2Component = Rect.Bottom;
				if (SubControl->Visible) {
					Rect.Left = Rect.Left + SubControl->Width;
				}
			}
			else if (SubAlign == alRight) {
				X1Component = Rect.Right - SubControl->Width;
				Y1Component = Rect.Top;
				X2Component = Rect.Right;
				Y2Component = Rect.Bottom;
				if (SubControl->Visible) {
					Rect.Right = Rect.Right - SubControl->Width;
				}
			}
			else if (SubAlign == alClient) {
				X1Component = Rect.Left;
				Y1Component = Rect.Top;
				X2Component = Rect.Right;
				Y2Component = Rect.Bottom;
				Rect.Bottom = Rect.Top;
			}
			else if (SubAlign == alCustom) {
				int NewLeft = X1Component;
				int NewTop = Y1Component;
				int NewWidth = X2Component - X1Component;
				int NewHeight = Y2Component - Y1Component;
				CustomAlignPosition(SubControl,
						NewLeft, NewTop, NewWidth, NewHeight,
						Rect);
				X1Component = NewLeft;
				Y1Component = NewTop;
				X2Component = X1Component + NewWidth;
				Y2Component = Y1Component + NewHeight;
			}

			SubControl->Size(X1Component, Y1Component,
											 X2Component - X1Component,
											 Y2Component - Y1Component,
											 bForceResize);

		}
  }

  return true;
}

//---------------------------------------------------------------------------
// Désactive le réalignement des contrôles enfants.
//---------------------------------------------------------------------------

void FASTCALL TControl::DisableAlign(void) {
  FAlignDisabled++;
}

//---------------------------------------------------------------------------
// Réactive le réalignement des contrôles enfants.
//---------------------------------------------------------------------------

void FASTCALL TControl::EnableAlign(void) {
  TComponent *ComponentParent;
  TControl *Control;

  if (--FAlignDisabled == 0) {
    ComponentParent = FParent;
    Control = dynamic_cast<TControl *>(ComponentParent);
    if (Control) Control->SizeSubComponents(0, 0, false);
    Size(FLeft, FTop, FWidth, FHeight, true);
  }
}

//---------------------------------------------------------------------------
// Transformation des coordonnées client en coordonnées écran
//---------------------------------------------------------------------------

TPoint TControl::ClientToScreen(const TPoint &Point) {
  return Point;
}

//---------------------------------------------------------------------------
// Transformation des coordonnées écran en coordonnées client
//---------------------------------------------------------------------------

TPoint TControl::ScreenToClient(const TPoint &Point) {
  return Point;
}

//---------------------------------------------------------------------------
// Contrôle au premier plan
//---------------------------------------------------------------------------

void TControl::BringToFront(void) {
  if (FHandle) {
    SetWindowPos(FHandle, HWND_TOP, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
  }
}

//---------------------------------------------------------------------------
//! Amorce une opération de drag and drop
//---------------------------------------------------------------------------
void FASTCALL TControl::BeginDrag(bool Immediate, int Threshold) {
	if (Threshold == -1) Threshold = Mouse->DragThreshold;


	if (Immediate) {
		bDragAndDrop = true;
		OnStartDrag(this, DragObject);
		Mouse->Capture = FHandle;
	}
	else {
		bDragNotImmediate = true;
		TPoint Pt = Mouse->CursorPos;
		XDragBegin1 = Pt.x - Threshold;
		YDragBegin1 = Pt.y - Threshold;
		XDragBegin2 = Pt.x + Threshold;
		YDragBegin2 = Pt.y + Threshold;
	}
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TControl::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Left"));
  ListProperties->Add(_T("Top"));
  ListProperties->Add(_T("Width"));
  ListProperties->Add(_T("Height"));
  ListProperties->Add(_T("ClientWidth"));
  ListProperties->Add(_T("ClientHeight"));
  ListProperties->Add(_T("Constraints.MinWidth"));
  ListProperties->Add(_T("Constraints.MinHeight"));
  ListProperties->Add(_T("Constraints.MaxWidth"));
  ListProperties->Add(_T("Constraints.MaxHeight"));
  ListProperties->Add(_T("Font.Height"));
  ListProperties->Add(_T("Font.Name"));
  ListProperties->Add(_T("Anchors"));
  ListProperties->Add(_T("Align"));
  ListProperties->Add(_T("AlignDisabled"));
  ListProperties->Add(_T("Color"));
  ListProperties->Add(_T("Cursor"));
  ListProperties->Add(_T("Enabled"));
  ListProperties->Add(_T("Visible"));
  ListProperties->Add(_T("AutoSize"));
  ListProperties->Add(_T("PopupMenu"));
  ListProperties->Add(_T("ShowHint"));
  ListProperties->Add(_T("ParentShowHint"));
  ListProperties->Add(_T("Hint"));
  ListProperties->Add(_T("Action"));
  ListProperties->Add(_T("DragMode"));
  ListProperties->Add(_T("OnClick"));
  ListProperties->Add(_T("OnDblClick"));
  ListProperties->Add(_T("OnContextPopup"));
  ListProperties->Add(_T("OnHint"));
  ListProperties->Add(_T("OnResize"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TControl::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Left")) {
    return tpNumber;
  }
  if (asProperty == _T("Top")) {
    return tpNumber;
  }
  if (asProperty == _T("Width")) {
    return tpNumber;
  }
  if (asProperty == _T("Height")) {
    return tpNumber;
  }
  if (asProperty == _T("ClientWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("ClientHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("Constraints.MinWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("Constraints.MinHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("Constraints.MaxWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("Constraints.MaxHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("Font.Height")) {
    return tpNumber;
  }
  if (asProperty == _T("Font.Name")) {
    return tpText;
  }
  if (asProperty == _T("Anchors")) {
    *asInfos = _T("akLeft\nakTop\nakRight\nakBottom");
    return tpMultipleChoice;
  }
  if (asProperty == _T("Align")) {
    *asInfos = _T("alNone\nalTop\nalBottom\nalLeft\nalRight\nalClient\nalCustom");
    return tpChoice;
  }
  if (asProperty == _T("AlignDisabled")) {
    return tpBool;
  }
  if (asProperty == _T("Color")) {
    return tpColor;
  }
  if (asProperty == _T("Cursor")) {
    return tpNumber;
  }
  if (asProperty == _T("Enabled")) {
    return tpBool;
  }
  if (asProperty == _T("Visible")) {
    return tpBool;
  }
  if (asProperty == _T("AutoSize")) {
    return tpBool;
  }
  if (asProperty == _T("PopupMenu")) {
    return tpObject;
  }
  if (asProperty == _T("ShowHint")) {
    return tpBool;
  }
  if (asProperty == _T("ParentShowHint")) {
    return tpBool;
  }
  if (asProperty == _T("Hint")) {
    return tpText;
  }
  if (asProperty == _T("Action")) {
    return tpObject;
  }
  if (asProperty == _T("DragMode")) {
    *asInfos = _T("dmManual\ndmAutomatic");
    return tpChoice;
  }
  if (asProperty == _T("OnClick")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnDblClick")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnContextPopup")) {
    *asInfos = _T("(TObject *Sender, const TPoint &MousePos, bool &Handled)");
    return tpEvent;
  }
  if (asProperty == _T("OnHint")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnResize")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TControl::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Left")) {
    return _T("-1");
  }
  if (asProperty == _T("Top")) {
    return _T("-1");
  }
  if (asProperty == _T("Width")) {
    return _T("-1");
  }
  if (asProperty == _T("Height")) {
    return _T("-1");
  }
  if (asProperty == _T("ClientWidth")) {
    return _T("-1");
  }
  if (asProperty == _T("ClientHeight")) {
    return _T("-1");
  }
  if (asProperty == _T("Constraints.MinWidth")) {
    return _T("0");
  }
  if (asProperty == _T("Constraints.MinHeight")) {
    return _T("0");
  }
  if (asProperty == _T("Constraints.MaxWidth")) {
    return _T("0");
  }
  if (asProperty == _T("Constraints.MaxHeight")) {
    return _T("0");
  }
  if (asProperty == _T("Font.Height")) {
		LOGFONT LogFont;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LogFont), &LogFont);
    return IntToStr(LogFont.lfHeight);
  }
  if (asProperty == _T("Font.Name")) {
		LOGFONT LogFont;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LogFont), &LogFont);
    return AnsiString(LogFont.lfFaceName);
  }
  if (asProperty == _T("Anchors")) {
    return _T("[akLeft, akTop]");
  }
  if (asProperty == _T("Align")) {
    return _T("alNone");
  }
  if (asProperty == _T("AlignDisabled")) {
    return _T("False");
  }
  if (asProperty == _T("Color")) {
    return _T("clBtnFace");
  }
  if (asProperty == _T("Cursor")) {
    return _T("0");
  }
  if (asProperty == _T("Enabled")) {
    return _T("True");
  }
  if (asProperty == _T("Visible")) {
    return _T("True");
  }
  if (asProperty == _T("AutoSize")) {
    return _T("False");
  }
  if (asProperty == _T("PopupMenu")) {
    return _T("");
  }
  if (asProperty == _T("ShowHint")) {
    return _T("False");
  }
  if (asProperty == _T("ParentShowHint")) {
    return _T("False");
  }
  if (asProperty == _T("Hint")) {
    return _T("");
  }
  if (asProperty == _T("Action")) {
    return _T("");
  }
  if (asProperty == _T("DragMode")) {
    return _T("dmManual");
  }
  if (asProperty == _T("OnClick")) {
    return _T("");
  }
  if (asProperty == _T("OnDblClick")) {
    return _T("");
  }
  if (asProperty == _T("OnContextPopup")) {
    return _T("");
  }
  if (asProperty == _T("OnHint")) {
    return _T("");
  }
  if (asProperty == _T("OnResize")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TControl::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Left")) {
    return IntToStr(Get_Left());
  }
  if (asProperty == _T("Top")) {
    return IntToStr(Get_Top());
  }
  if (asProperty == _T("Width")) {
    return IntToStr(Get_Width());
  }
  if (asProperty == _T("Height")) {
    return IntToStr(Get_Height());
  }
  if (asProperty == _T("ClientWidth")) {
    return IntToStr(Get_ClientWidth());
  }
  if (asProperty == _T("ClientHeight")) {
    return IntToStr(Get_ClientHeight());
  }
  if (asProperty == _T("Constraints.MinWidth")) {
    int MinWidth = FConstraints->MinWidth;
    return IntToStr(MinWidth);
  }
  if (asProperty == _T("Constraints.MinHeight")) {
    int MinHeight = FConstraints->MinHeight;
    return IntToStr(MinHeight);
  }
  if (asProperty == _T("Constraints.MaxWidth")) {
    int MaxWidth = FConstraints->MaxWidth;
    return IntToStr(MaxWidth);
  }
  if (asProperty == _T("Constraints.MaxHeight")) {
    int MaxHeight = FConstraints->MaxHeight;
    return IntToStr(MaxHeight);
  }
  if (asProperty == _T("Font.Height")) {
    int Height = FFont->Height;
    return IntToStr(Height);
  }
  if (asProperty == _T("Font.Name")) {
    return FFont->Name;
  }
  if (asProperty == _T("Anchors")) {
    AnsiString asAnchors;
    TAnchors Anchors = Get_Anchors();
    if (Anchors.Contains(akLeft)) asAnchors += _T("akLeft\n");
    if (Anchors.Contains(akTop)) asAnchors += _T("akTop\n");
    if (Anchors.Contains(akRight)) asAnchors += _T("akRight\n");
    if (Anchors.Contains(akBottom)) asAnchors += _T("akBottom\n");
    return FormatProperty(asAnchors, _T("["), _T("]"), _T(", "));
  }
  if (asProperty == _T("Align")) {
    TAlign Align = Get_Align();
    if (Align == alNone) return _T("alNone");
    if (Align == alTop) return _T("alTop");
    if (Align == alBottom) return _T("alBottom");
    if (Align == alLeft) return _T("alLeft");
    if (Align == alRight) return _T("alRight");
    if (Align == alClient) return _T("alClient");
    if (Align == alCustom) return _T("alCustom");
    return _T("alNone");
  }
  if (asProperty == _T("AlignDisabled")) {
    if (Get_AlignDisabled()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Color")) {
    return ColorToString(Get_Color());
  }
  if (asProperty == _T("Cursor")) {
    return IntToStr(Get_Cursor());
  }
  if (asProperty == _T("Enabled")) {
    if (Get_Enabled()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Visible")) {
    if (Get_Visible()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("AutoSize")) {
    if (Get_AutoSize()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("PopupMenu")) {
    TPopupMenu *PopupMenu = Get_PopupMenu();
    if (PopupMenu) return PopupMenu->Name;
    return _T("");
  }
  if (asProperty == _T("ShowHint")) {
    if (Get_ShowHint()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ParentShowHint")) {
    if (Get_ParentShowHint()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Hint")) {
    return Get_Hint();
  }
  if (asProperty == _T("Action")) {
    TAction *Action = Get_Action();
    if (Action) return Action->Name;
    return _T("");
  }
  if (asProperty == _T("DragMode")) {
    TDragMode DragMode = Get_DragMode();
    if (DragMode == dmManual) return _T("dmManual");
    if (DragMode == dmAutomatic) return _T("dmAutomatic");
    return _T("");
  }
  if (asProperty == _T("OnClick")) {
    return OnClick.AsString();
  }
  if (asProperty == _T("OnDblClick")) {
    return OnDblClick.AsString();
  }
  if (asProperty == _T("OnContextPopup")) {
    return OnContextPopup.AsString();
  }
  if (asProperty == _T("OnHint")) {
    return OnHint.AsString();
  }
  if (asProperty == _T("OnResize")) {
    return OnResize.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TControl::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Left")) {
    Set_Left(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Top")) {
    Set_Top(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Width")) {
    Set_Width(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Height")) {
    Set_Height(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("ClientWidth")) {
    Set_ClientWidth(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("ClientHeight")) {
    Set_ClientHeight(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Constraints.MinWidth")) {
    Constraints->MinWidth = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("Constraints.MinHeight")) {
    Constraints->MinHeight = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("Constraints.MaxWidth")) {
    Constraints->MaxWidth = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("Constraints.MaxHeight")) {
    Constraints->MaxHeight = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("Font.Height")) {
    FFont->Height =  StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("Font.Name")) {
		FFont->Name =  asValue;
    return true;
  }
  if (asProperty == _T("Anchors")) {
    int Cur;
    AnsiString asMot;

    FAnchors.Clear();
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("akLeft")) FAnchors << akLeft;
      else if (asMot == _T("akTop")) FAnchors << akTop;
      else if (asMot == _T("akRight")) FAnchors << akRight;
      else if (asMot == _T("akBottom")) FAnchors << akBottom;
    } while (!asMot.IsEmpty());

    return true;
  }
  if (asProperty == _T("Align")) {
    if (asValue == _T("alNone")) Set_Align(alNone);
    else if (asValue == _T("alTop")) Set_Align(alTop);
    else if (asValue == _T("alBottom")) Set_Align(alBottom);
    else if (asValue == _T("alLeft")) Set_Align(alLeft);
    else if (asValue == _T("alRight")) Set_Align(alRight);
    else if (asValue == _T("alClient")) Set_Align(alClient);

    return true;
  }
  if (asProperty == _T("AlignDisabled")) {
    return false;
  }
  if (asProperty == _T("Color")) {
    TColor Color;
    Color = asValue;
    Set_Color(Color);
    return true;
  }
  if (asProperty == _T("Cursor")) {
		TCursor Cursor;
		if (asValue == _T("crDefault")) Cursor = crDefault;
		else if (asValue == _T("crNone")) Cursor = crNone;
		else if (asValue == _T("crArrow")) Cursor = crArrow;
		else if (asValue == _T("crCross")) Cursor = crCross;
		else if (asValue == _T("crIBeam")) Cursor = crIBeam;
		else if (asValue == _T("crSizeNESW")) Cursor = crSizeNESW;
		else if (asValue == _T("crSizeNS")) Cursor = crSizeNS;
		else if (asValue == _T("crSizeNWSE")) Cursor = crSizeNWSE;
		else if (asValue == _T("crSizeWE")) Cursor = crSizeWE;
		else if (asValue == _T("crUpArrow")) Cursor = crUpArrow;
		else if (asValue == _T("crHourGlass")) Cursor = crHourGlass;
		else if (asValue == _T("crDrag")) Cursor = crDrag;
		else if (asValue == _T("crNoDrop")) Cursor = crNoDrop;
		else if (asValue == _T("crHSplit")) Cursor = crHSplit;
		else if (asValue == _T("crVSplit")) Cursor = crVSplit;
		else if (asValue == _T("crMultiDrag")) Cursor = crMultiDrag;
		else if (asValue == _T("crSQLWait")) Cursor = crSQLWait;
		else if (asValue == _T("crNo")) Cursor = crNo;
		else if (asValue == _T("crAppStart")) Cursor = crAppStart;
		else if (asValue == _T("crHelp")) Cursor = crHelp;
		else if (asValue == _T("crHandPoint")) Cursor = crHandPoint;
		else if (asValue == _T("crSize")) Cursor = crSize;
		else if (asValue == _T("crSizeAll")) Cursor = crSizeAll;
		else Cursor = static_cast<TCursor>(StrToInt(asValue));
    Set_Cursor(Cursor);
    return true;
  }
  if (asProperty == _T("Enabled")) {
    Set_Enabled(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Visible")) {
    Set_Visible(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("AutoSize")) {
    Set_AutoSize(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("PopupMenu")) {
    TPopupMenu *PopupMenu;
		if (asValue.IsEmpty()) PopupMenu = NULL;
		else PopupMenu = dynamic_cast<TPopupMenu *>(FindRootComponent()->FindComponent(asValue));
    Set_PopupMenu(PopupMenu);
    return true;
  }
  if (asProperty == _T("ShowHint")) {
    Set_ShowHint(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ParentShowHint")) {
    Set_ParentShowHint(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Hint")) {
    Set_Hint(asValue);
    return true;
  }
  if (asProperty == _T("Action")) {
    TAction *Action;
		if (asValue.IsEmpty()) Action = NULL;
		else Action = dynamic_cast<TAction *>(FindRootComponent()->FindComponent(asValue));
    Set_Action(Action);
    return true;
  }
  if (asProperty == _T("DragMode")) {
    if (asValue == _T("dmManual")) Set_DragMode(dmManual);
    else if (asValue == _T("dmAutomatic")) Set_DragMode(dmAutomatic);
    return true;
  }
  if (asProperty == _T("OnClick")) {
		OnClick.ObjCall = Sender;
    OnClick = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnDblClick")) {
		OnDblClick.ObjCall = Sender;
    OnDblClick = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnContextPopup")) {
		OnContextPopup.ObjCall = Sender;
    OnContextPopup = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnHint")) {
		OnHint.ObjCall = Sender;
    OnHint = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnResize")) {
		OnResize.ObjCall = Sender;
    OnResize = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}
//---------------------------------------------------------------------------
