//-----------------------------------------------------------------------------
//! @file TWinControl.cpp
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
// TWinControl
//---------------------------------------------------------------------------

TWinControl::TWinControl(TComponent* AOwner): TControl(AOwner) {

	// Initialisations
	FCaption = _T("");
	FClassName = _T("");
	FName = _T("");
	FExWindowStyle = 0;
	FWindowStyle = WS_CHILD | WS_VISIBLE;  // | WS_CLIPCHILDREN provoque des bugs d'affichage avec les TabSheets et les GroupBox
	FTabStop = false;
	FTabOrder = -1;
	FWantReturns = false;
	FWantTabs = false;
	FDragAcceptFiles = false;
	FDragKind = dkDrag;
	CurrentFocus = -1;
	FCanvas = NULL;
	hBrushColorBack = NULL;
	DialogProc = NULL;
	bWantPaint = false;
	bProcessCreateToDo = false;
	bProcessShowWindowToDo = true;

	// Link avec commctl32.dll
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_STANDARD_CLASSES;
  InitCommonControlsEx(&InitCtrls);

}

TWinControl::~TWinControl(void) {
	if (FCanvas) {
		ReleaseDC(FHandle, FCanvas->Handle);
		delete FCanvas;
	}
	if (hBrushColorBack) {
		DeleteObject(hBrushColorBack);
	}
	if (Mouse->Capture == FHandle) Mouse->Capture = NULL;
	Destroying();
	if (FHandle) DestroyWindow(FHandle);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

bool TWinControl::Set_Caption(AnsiString NewCaption) {
	if (FCaption != NewCaption) {
		FCaption = NewCaption;
		SetWindowText(FHandle, FCaption);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Left
//---------------------------------------------------------------------------

bool TWinControl::Set_Left(int NewLeft) {

	if (FLeft != NewLeft) {
		FLeft = NewLeft;
		int FLeftWnd = FLeft;
		int FTopWnd = FTop;
		if (FParent) {
			FLeftWnd += FParent->LeftWnd;
			FTopWnd += FParent->TopWnd;
		}
		if (FHandle && FAlignDisabled == 0) {
			MoveWindow(FHandle, FLeftWnd, FTopWnd, FWidth, FHeight, TRUE);
		}

	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Top
//---------------------------------------------------------------------------

bool TWinControl::Set_Top(int NewTop) {

	if (FTop != NewTop) {
		FTop = NewTop;
		int FLeftWnd = FLeft;
		int FTopWnd = FTop;
		if (FParent) {
			FLeftWnd += FParent->LeftWnd;
			FTopWnd += FParent->TopWnd;
		}
		if (FHandle && FAlignDisabled == 0) {
			MoveWindow(FHandle, FLeftWnd, FTopWnd, FWidth, FHeight, TRUE);
		}
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

bool TWinControl::Set_Width(int NewWidth) {
	int OldWidth;


	OldWidth = FWidth;
	TControl::Set_Width(NewWidth);

	if (FWidth != OldWidth && FAlignDisabled == 0) {
		int FLeftWnd = FLeft;
		int FTopWnd = FTop;
		if (FParent) {
			FLeftWnd += FParent->LeftWnd;
			FTopWnd += FParent->TopWnd;
		}
		if (FHandle && FWidth >= 0 && FHeight >= 0) {
			MoveWindow(FHandle, FLeftWnd, FTopWnd, FWidth, FHeight, TRUE);
		}
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

bool TWinControl::Set_Height(int NewHeight) {
	int OldHeight;


	OldHeight = FHeight;
	TControl::Set_Height(NewHeight);

	if (FHeight != OldHeight && FAlignDisabled == 0) {
		int FLeftWnd = FLeft;
		int FTopWnd = FTop;
		if (FParent) {
			FLeftWnd += FParent->LeftWnd;
			FTopWnd += FParent->TopWnd;
		}
		if (FHandle && FWidth >= 0 && FHeight >= 0) {
			MoveWindow(FHandle, FLeftWnd, FTopWnd, FWidth, FHeight, TRUE);
		}
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

bool TWinControl::Set_Font(TFont *NewFont) {
	if (FFont != NewFont) {
		TControl::Set_Font(NewFont);
		if (FHandle) {
			HFONT hFont = FFont->Handle;
			SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
		}
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientWidth
//---------------------------------------------------------------------------

int TWinControl::Get_ClientWidth(void) {
	RECT ClientRect;

	::GetClientRect(FHandle, &ClientRect);

	return ClientRect.right - ClientRect.left;
}

bool TWinControl::Set_ClientWidth(int NewClientWidth) {
	RECT WindowRect;
	RECT ClientRect;

	::GetWindowRect(FHandle, &WindowRect);
	::GetClientRect(FHandle, &ClientRect);

	return Set_Width(NewClientWidth +
									 WindowRect.right - WindowRect.left -
									 ClientRect.right + ClientRect.left);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientHeight
//---------------------------------------------------------------------------

int TWinControl::Get_ClientHeight(void) {
	RECT ClientRect;

	::GetClientRect(FHandle, &ClientRect);

	return ClientRect.bottom - ClientRect.top;
}

bool TWinControl::Set_ClientHeight(int NewClientHeight) {
	RECT WindowRect;
	RECT ClientRect;

	::GetWindowRect(FHandle, &WindowRect);
	::GetClientRect(FHandle, &ClientRect);

	return Set_Height(NewClientHeight +
										WindowRect.bottom - WindowRect.top -
										ClientRect.bottom + ClientRect.top);
}


//---------------------------------------------------------------------------
// Origine de la fenêtre Windows
//---------------------------------------------------------------------------

int TWinControl::Get_LeftWnd(void) {
	TRect Rect;
	Rect = ClientRect;
	return Rect.Left;
}


//---------------------------------------------------------------------------
// Origine de la fenêtre Windows
//---------------------------------------------------------------------------

int TWinControl::Get_TopWnd(void) {
	TRect Rect;
	Rect = ClientRect;
	return Rect.Top;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WindowStyle
//---------------------------------------------------------------------------

DWORD TWinControl::Get_WindowStyle(void) {
	return FWindowStyle;
}

bool TWinControl::Set_WindowStyle(DWORD NewWindowStyle) {
	if (FWindowStyle != NewWindowStyle) {
		FWindowStyle = NewWindowStyle;
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ExWindowStyle
//---------------------------------------------------------------------------

DWORD TWinControl::Get_ExWindowStyle(void) {
	return FExWindowStyle;
}

bool TWinControl::Set_ExWindowStyle(DWORD NewExWindowStyle) {
	if (FExWindowStyle != NewExWindowStyle) {
		FExWindowStyle = NewExWindowStyle;
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool TWinControl::Set_Visible(bool NewVisible) {

	if (FVisible != NewVisible) {
		FVisible = NewVisible;
		if (FVisible && (FWindowStyle & WS_VISIBLE) == 0) {
			ShowWindow(FHandle, SW_SHOW);
			FWindowStyle |= WS_VISIBLE;
		}
		else if (!FVisible && (FWindowStyle & WS_VISIBLE) != 0) {
			ShowWindow(FHandle, SW_HIDE);
			FWindowStyle &= ~WS_VISIBLE;
		}
		if (!FVisible) KillCurrentFocus();
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Enabled
//---------------------------------------------------------------------------

bool TWinControl::Set_Enabled(bool NewEnabled) {

	if (FEnabled != NewEnabled) {
		FEnabled = NewEnabled;
		if (FEnabled) FWindowStyle &= ~WS_DISABLED;
		else FWindowStyle |= WS_DISABLED;
		if (FHandle) EnableWindow(FHandle, FEnabled);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

bool TWinControl::Set_Color(TColor NewColor) {
	if (NewColor != FColor) {
		FColor = NewColor;
		if (hBrushColorBack) {
			DeleteObject(hBrushColorBack);
		}
		hBrushColorBack = CreateSolidBrush(FColor.cr);
		Invalidate();
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TabStop
//---------------------------------------------------------------------------

bool TWinControl::Get_TabStop(void) {
	return FTabStop;
}

bool TWinControl::Set_TabStop(bool NewTabStop) {

	if (FTabStop != NewTabStop) {
		FTabStop = NewTabStop;
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TabOrder
//---------------------------------------------------------------------------

int TWinControl::Get_TabOrder(void) {
	return FTabOrder;
}

bool TWinControl::Set_TabOrder(int NewTabOrder) {

	if (FTabOrder != NewTabOrder) {
		FTabOrder = NewTabOrder;
		if (FTabOrder == -1) FTabStop = false;
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WantReturns
//---------------------------------------------------------------------------

bool TWinControl::Get_WantReturns(void) {
	return FWantReturns;
}

bool TWinControl::Set_WantReturns(bool NewWantReturns) {

	if (FWantReturns != NewWantReturns) {
		FWantReturns = NewWantReturns;
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WantTabs
//---------------------------------------------------------------------------

bool TWinControl::Get_WantTabs(void) {
	return FWantTabs;
}

bool TWinControl::Set_WantTabs(bool NewWantTabs) {

	if (FWantTabs != NewWantTabs) {
		FWantTabs = NewWantTabs;
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DragAcceptFiles
//---------------------------------------------------------------------------

bool TWinControl::Get_DragAcceptFiles(void) {
	return FDragAcceptFiles;
}

bool TWinControl::Set_DragAcceptFiles(bool NewDragAcceptFiles) {

	if (FDragAcceptFiles != NewDragAcceptFiles) {
		FDragAcceptFiles = NewDragAcceptFiles;
		::DragAcceptFiles(FHandle, (BOOL) FDragAcceptFiles);
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DragKind
//---------------------------------------------------------------------------

TDragKind TWinControl::Get_DragKind(void) {
	return FDragKind;
}

bool TWinControl::Set_DragKind(TDragKind NewDragKind) {

	if (FDragKind != NewDragKind) {
		FDragKind = NewDragKind;
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ClientRect
//---------------------------------------------------------------------------

TRect TWinControl::Get_ClientRect(void) {
	RECT ClientRect;
	TRect Retour;

	::GetClientRect(FHandle, &ClientRect);
	Retour.Left = ClientRect.left;
	Retour.Top = ClientRect.top;
	Retour.Right = ClientRect.right;
	Retour.Bottom = ClientRect.bottom;

	AdjustClientRect(Retour);

	return Retour;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Canvas
//---------------------------------------------------------------------------

TCanvas *TWinControl::Get_Canvas(void) {
	if (FCanvas == NULL) {
		FCanvas = new TControlCanvas();
		FCanvas->Control = this;
	}
	return FCanvas;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Parent
//---------------------------------------------------------------------------

bool TWinControl::Set_Parent(TControl *NewParent) {
	TWinControl *WinControlParent;

  if (FParent != NewParent) {
    FParent = NewParent;
		WinControlParent = dynamic_cast<TWinControl *>(NewParent);
		if (WinControlParent) ::SetParent(FHandle, WinControlParent->Handle);
  }
  return true;
}


//---------------------------------------------------------------------------
bool TWinControl::ProcessPrintClient(HDC hdc, DWORD Flags) {
	RECT rcPaint;

	if (FVisible || (Flags & PRF_CHECKVISIBLE) == 0) {
		GetClientRect(FHandle, &rcPaint);
		if (Flags & PRF_ERASEBKGND) {
			if (!hBrushColorBack) {
				hBrushColorBack = CreateSolidBrush(FColor.cr);
			}
			FillRect(hdc, &rcPaint, hBrushColorBack);
		}
		ProcessPaint(hdc, rcPaint);
	}

	return true;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessShowWindow(bool bShow) {
	int i;
	TWinControl *WinControl;


	if (!FComponentState.Contains(csLoading) &&
	    !FComponentState.Contains(csDestroying)) {

		if (bShow) {
			OnShow(this);
			bProcessShowWindowToDo = false;
		}
		else {
			OnHide(this);
			bProcessShowWindowToDo = true;
		}

		for (i = 0; i < (int) FComponents.size(); i++) {
			WinControl = dynamic_cast<TWinControl *>(FComponents[i]);
			if (WinControl && WinControl->FVisible == true) {
				WinControl->ProcessShowWindow(bShow);
			}
		}

	}

	return true;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessSetFocus(HWND hWndLoseFocus) {

	if (!FComponentState.Contains(csLoading) &&
	    !FComponentState.Contains(csDestroying)) {

		if (!IsChild(FHandle, hWndLoseFocus)) {
			OnEnter(this);
		}
		// SetCurrentFocus et SetFocusOnCurrentControl sortis du "if (!IsChild(..."
		// sinon deux SetFocus successifs ne marchent plus
		// (ce qui arrive apparemment lorsqu'on restaure une fenêtre en icône) :
		// Le premier transfère le focus au contrôle enfant
		// Le second non car la fenêtre perdant le focus est le contrôle enfant
		SetCurrentFocus();
		SetFocusOnCurrentControl();

	}

	return false;  // Pour exécuter le traitement par défaut (edit par exemple)
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessKillFocus(HWND hWndGetFocus) {
	TWinControl *WinControlParent;


	if (!FComponentState.Contains(csLoading) &&
	    !FComponentState.Contains(csDestroying)) {

		if (FHandle != hWndGetFocus && !IsChild(FHandle, hWndGetFocus)) {
			OnExit(this);
			WinControlParent = dynamic_cast<TWinControl *> (FParent);
			if (WinControlParent) WinControlParent->ProcessKillFocus(hWndGetFocus);
		}

	}

	return false;  // Pour exécuter le traitement par défaut (edit par exemple)
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessSizing(int fwSide, LPRECT lprc) {
	int MinWidth;
	int MinHeight;
	int MaxWidth;
	int MaxHeight;


	MinWidth = FConstraints->MinWidth;
	if (MinWidth != 0 && lprc->right < lprc->left + MinWidth) {
		if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT) {
			lprc->right = lprc->left + MinWidth;
		}
		else {
			lprc->left = lprc->right - MinWidth;
		}
	}

	MinHeight = FConstraints->MinHeight;
	if (MinHeight != 0 && lprc->bottom < lprc->top + MinHeight) {
		if (fwSide == WMSZ_BOTTOM || fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_BOTTOMRIGHT) {
			lprc->bottom = lprc->top + MinHeight;
		}
		else {
			lprc->top = lprc->bottom - MinHeight;
		}
	}

	MaxWidth = FConstraints->MaxWidth;
	if (MaxWidth != 0 && lprc->right > lprc->left + MaxWidth) {
		if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT) {
			lprc->right = lprc->left + MaxWidth;
		}
		else {
			lprc->left = lprc->right - MaxWidth;
		}
	}

	MaxHeight = FConstraints->MaxHeight;
	if (MaxHeight != 0 && lprc->bottom > lprc->top + MaxHeight) {
		if (fwSide == WMSZ_BOTTOM || fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_BOTTOMRIGHT) {
			lprc->bottom = lprc->top + MinHeight;
		}
		else {
			lprc->top = lprc->bottom - MinHeight;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessMove(int X, int Y) {

	if (FAlignDisabled == 0) {
		if (FHandle) { // Parce que Size peut être appelé dans CreateWindow (donc avant que FHandle soit défini)
			RECT Rect;

			GetWindowRect(FHandle, &Rect);	// Car Move donne les coordonnées du rectangle CLIENT
			if (FParent) {
				HWND hWndParent = FParent->Handle;
				::ScreenToClient(hWndParent, (LPPOINT) &Rect);
			}
			FLeft = Rect.left;
			FTop = Rect.top;
		}
		else {
			FLeft = X;
			FTop = Y;
		}

		if (FParent) {
			FLeft -= FParent->LeftWnd;
			FTop -= FParent->TopWnd;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessSize(int SizeType, int L, int H) {

	if (FAlignDisabled == 0) {
		RECT Rect;
		if (FHandle) { // Parce que Size peut être appelé dans CreateWindow (donc avant que FHandle soit défini)
			GetWindowRect(FHandle, &Rect);
			L = Rect.right - Rect.left;
			H = Rect.bottom - Rect.top;
		}
		if (L != FWidth || H != FHeight) {
			SizeSubComponents(L - FWidth, H - FHeight, false);
			FWidth = L;
			FHeight = H;
			OnResize(this);
			if (FCanvas != NULL) FCanvas->ClipRect = ClientRect;
			if (FHandle) InvalidateRect(FHandle, NULL, TRUE);
		}
	}

	return false;
}

//---------------------------------------------------------------------------
void TWinControl::Size(int NewLeft, int NewTop, int NewWidth, int NewHeight, bool bForceResize) {
	TAnchors SubAnchors;
	TRect ClientRect;


	if (FAutoSize) {
		InternalCanAutoSize(NewLeft, NewTop, NewWidth, NewHeight);
	}

	if (NewLeft != FLeft || NewTop != FTop || NewWidth != FWidth || NewHeight != FHeight ||
			bForceResize) {
		if (FHandle && FAlignDisabled == 0) {
			int FLeftWnd = NewLeft;
			int FTopWnd = NewTop;
			if (FParent) {
				FLeftWnd += FParent->LeftWnd;
				FTopWnd += FParent->TopWnd;
			}
			if (NewWidth >= 0 && NewHeight >= 0) {
				MoveWindow(FHandle, FLeftWnd, FTopWnd, NewWidth, NewHeight, TRUE);
			}
		}

		TControl::Size(NewLeft, NewTop, NewWidth, NewHeight, bForceResize);
		if (FCanvas != NULL) FCanvas->ClipRect = ClientRect;

	}

}

//---------------------------------------------------------------------------
bool TWinControl::DistributionHScroll(HWND hWnd, int Command, int Info) {
	TWinControl *WinControl;


	WinControl = FindComponentFromHWnd(hWnd);
	if (WinControl) {

		// Verrue pour avoir la valeur sur 32 bits (et non sur 16 comme dans le message WM_HSCROLL)
		SCROLLINFO ScrollInfo;
		memset(&ScrollInfo, 0, sizeof(SCROLLINFO));
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		if (Command == SB_THUMBTRACK) {
			ScrollInfo.fMask = SIF_TRACKPOS;
			if (GetScrollInfo(hWnd, SB_HORZ, &ScrollInfo)) Info = ScrollInfo.nTrackPos;
		}
		else {
			ScrollInfo.fMask = SIF_POS;
			if (GetScrollInfo(hWnd, SB_HORZ, &ScrollInfo)) Info = ScrollInfo.nPos;
		}

		return WinControl->ProcessHScroll(Command, Info);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::DistributionVScroll(HWND hWnd, int Command, int Info) {
	TWinControl *WinControl;


	WinControl = FindComponentFromHWnd(hWnd);
	if (WinControl) {

		// Verrue pour avoir la valeur sur 32 bits (et non sur 16 comme dans le message WM_VSCROLL)
		SCROLLINFO ScrollInfo;
		memset(&ScrollInfo, 0, sizeof(SCROLLINFO));
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		if (Command == SB_THUMBTRACK) {
			ScrollInfo.fMask = SIF_TRACKPOS;
			if (GetScrollInfo(hWnd, SB_VERT, &ScrollInfo)) Info = ScrollInfo.nTrackPos;
		}
		else {
			ScrollInfo.fMask = SIF_POS;
			if (GetScrollInfo(hWnd, SB_VERT, &ScrollInfo)) Info = ScrollInfo.nPos;
		}

		return WinControl->ProcessVScroll(Command, Info);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::DistributionKeyDown(HWND hWnd, int nVirtKey) {
	TWinControl *WinControl;


	WinControl = FindComponentFromHWnd(hWnd);
	if (WinControl) {
		return WinControl->ProcessKeyDown(nVirtKey);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessKeyDown(int nVirtKey) {

	if (!FComponentState.Contains(csLoading) && OnKeyDown) {
		TShiftState Shift = GetShiftState();
		OnKeyDown(this, nVirtKey, Shift);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::DistributionKeyUp(HWND hWnd, int nVirtKey) {
	TWinControl *WinControl;


	WinControl = FindComponentFromHWnd(hWnd);
	if (WinControl) {
		return WinControl->ProcessKeyUp(nVirtKey);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessKeyUp(int nVirtKey) {

	if (!FComponentState.Contains(csLoading) && OnKeyUp) {
		TShiftState Shift = GetShiftState();
		OnKeyUp(this, nVirtKey, Shift);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::DistributionChar(HWND hWnd, TCHAR chCharCode) {
	TWinControl *WinControl;


	WinControl = FindComponentFromHWnd(hWnd);
	if (WinControl) {
		return WinControl->ProcessChar(chCharCode);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessChar(TCHAR chCharCode) {

	if (!FComponentState.Contains(csLoading)) {
		OnKeyPress(this, chCharCode);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessMouseMove(int X, int Y) {
	TPoint Pt(X, Y);

	if (!FComponentState.Contains(csLoading)) {
		if (bDragAndDrop || bDragNotImmediate) {
			Pt = ClientToScreen(Pt);
			// Le ButtonUp n'est pas toujours intercepté (capture souris ?)
			// donc il faut refaire le test du bouton souris ici
			if ((GetKeyState(VK_LBUTTON) & 0x8000) == 0) {
				bDragNotImmediate = false;
				bDragAndDrop = false;
				if (Mouse->Capture == FHandle) Mouse->Capture = NULL;
			}
		}
		TShiftState Shift = GetShiftState();
		OnMouseMove(this, Shift, X, Y);
		if (FDragKind == dkDrag) {
			if (bDragNotImmediate) {
				if (Pt.x <= XDragBegin1 || XDragBegin2 <= Pt.x ||
						Pt.y <= YDragBegin1 || YDragBegin2 <= Pt.y) {
					bDragNotImmediate = false;
					bDragAndDrop = true;
					OnStartDrag(this, DragObject);
					Mouse->Capture = FHandle;
				}
			}
		}
		if (bDragAndDrop) {
			HWND hWnd;
			TControl *Control;
			bool Accept = false;

			hWnd = WindowFromPoint(Pt);
			if (hWnd != NULL) {
				Control = reinterpret_cast<TControl *>((SIZE_T) GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if (Control) Control->OnDragOver(this, this, X, Y, dsDragMove, Accept);
			}
			if (Accept) {
				::SetCursor(Screen->Cursors[crDrag]);
			}
			else {
				::SetCursor(Screen->Cursors[crNoDrop]);
			}
		}
		else {
			if (FCursor != crDefault) ::SetCursor(Screen->Cursors[FCursor]);
			/*
			// Supprimé (sinon les edit-box ont le curseur par défaut au lieu du caret)
			else ::SetCursor(Screen->Cursors[crArrow]);
			*/
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessLButtonDown(int X, int Y) {
	if (!FComponentState.Contains(csLoading)) {
		TShiftState Shift = GetShiftState();
		OnMouseDown(this, mbLeft, Shift, X, Y);
		if (FDragMode == dmAutomatic) {
			BeginDrag(Mouse->DragImmediate, Mouse->DragThreshold);
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessLButtonUp(int X, int Y) {
	if (!FComponentState.Contains(csLoading)) {
		TShiftState Shift = GetShiftState();
		OnMouseUp(this, mbLeft, Shift, X, Y);
		if (bDragAndDrop || bDragNotImmediate) {
			if (bDragAndDrop) {
				TPoint Pt(X, Y);
				HWND hWnd;
				TControl *Control;
				bool Accept = false;

				Pt = ClientToScreen(Pt);
				hWnd = WindowFromPoint(Pt);
				if (hWnd != NULL) {
					Control = reinterpret_cast<TControl *>((SIZE_T) GetWindowLongPtr(hWnd, GWLP_USERDATA));
					if (Control) OnEndDrag(this, Control, X, Y);
				}
				bDragAndDrop = false;
				if (Mouse->Capture == FHandle) Mouse->Capture = NULL;
			}
			bDragNotImmediate = false;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessLButtonDblClk(int X, int Y) {
	if (!FComponentState.Contains(csLoading)) {
		OnDblClick(this);
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessMButtonDown(int X, int Y) {
	if (!FComponentState.Contains(csLoading)) {
		SetFocus();

		if (OnMouseDown) {
			TShiftState Shift = GetShiftState();
			OnMouseDown(this, mbMiddle, Shift, X, Y);
		}
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessMButtonUp(int X, int Y) {
	if (!FComponentState.Contains(csLoading) && OnMouseUp) {
		TShiftState Shift = GetShiftState();
		OnMouseUp(this, mbMiddle, Shift, X, Y);
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessMButtonDblClk(int X, int Y) {
	/* Pas de double clic sur ce bouton
	if (!FComponentState.Contains(csLoading)) {
		OnDblClick(this);
	}
	*/
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessRButtonDown(int X, int Y) {
	if (!FComponentState.Contains(csLoading)) {
		SetFocus();

		if (OnMouseDown) {
			TShiftState Shift = GetShiftState();
			OnMouseDown(this, mbRight, Shift, X, Y);
		}
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessRButtonUp(int X, int Y) {
	if (!FComponentState.Contains(csLoading) && OnMouseUp) {
		TShiftState Shift = GetShiftState();
		OnMouseUp(this, mbRight, Shift, X, Y);
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessRButtonDblClk(int X, int Y) {
	/* Pas de double clic sur ce bouton
	if (!FComponentState.Contains(csLoading)) {
		OnDblClick(this);
	}
	*/
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessMouseWheel(WORD fwKeys, short zDelta, int X, int Y) {
	if (!FComponentState.Contains(csLoading) && OnMouseWheel) {
		TShiftState Shift;
		TPoint MousePos;
		bool Handled;

		Shift.Clear();
		if (fwKeys & MK_CONTROL) Shift << ssCtrl;
		if (fwKeys & MK_SHIFT) Shift << ssShift;
		if (fwKeys & MK_LBUTTON) Shift << ssLeft;
		if (fwKeys & MK_MBUTTON) Shift << ssMiddle;
		if (fwKeys & MK_RBUTTON) Shift << ssRight;
		MousePos.x = X;
		MousePos.y = Y;
		Handled = false;
		OnMouseWheel(this, Shift, (int) zDelta, MousePos, Handled);
		if (Handled) return true;
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::DistributionColor(HWND hWnd, HDC hdc, HBRUSH *hBrush) {
	TWinControl *WinControl;


	WinControl = FindComponentFromHWnd(hWnd);
	if (WinControl) {
		return WinControl->ProcessColor(hdc, hBrush);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessColor(HDC hdc, HBRUSH *hBrush) {

	if (!FComponentState.Contains(csLoading)) {
		TColor ColorText;

		if (!hBrushColorBack) {
			hBrushColorBack = CreateSolidBrush(FColor.cr);
		}
		ColorText = FFont->Color;
		SetBkColor(hdc, FColor.cr);
		SetTextColor(hdc, ColorText.cr);
		SetBkMode(hdc, TRANSPARENT);
		*hBrush = hBrushColorBack;
	}

	return true;
}

//---------------------------------------------------------------------------
bool TWinControl::DistributionContextMenu(HWND hWnd, int X, int Y) {
	TWinControl *WinControl;


	WinControl = FindComponentFromHWnd(hWnd);
	if (WinControl) {
		return WinControl->ProcessContextMenu(X, Y);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessContextMenu(int X, int Y) {
	if (!FComponentState.Contains(csLoading)) {
		if (OnContextPopup) {
			bool Handled = false;
			TPoint MousePos(X, Y);
			OnContextPopup(this, MousePos, Handled);
			if (Handled) return true;
		}
		if (FPopupMenu && FPopupMenu->AutoPopup) {
			FPopupMenu->Popup(X, Y);
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::ProcessDropFiles(HDROP hDrop) {
	if (!FComponentState.Contains(csLoading) && OnDropFile) {
		TShiftState Shift = GetShiftState();
		POINT Pt;
		DragQueryPoint(hDrop, &Pt);
		int NbFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		int i, NbChar;
		AnsiString asFileName;
		TCHAR *szFileName;
		for (i = 0; i < NbFiles; i++) {
			NbChar = DragQueryFile(hDrop, i, NULL, 0) + 1;
			szFileName = new TCHAR[NbChar];
			if (DragQueryFile(hDrop, i, szFileName, NbChar)) {
				asFileName = szFileName;
				OnDropFile(this, asFileName, Shift, Pt.x, Pt.y);
			}
			delete[] szFileName;
		}
	}
	DragFinish(hDrop);

	return false;
}

//---------------------------------------------------------------------------
// Fermeture de la fenêtre
//---------------------------------------------------------------------------
bool TWinControl::ProcessClose(void) {
	Destroying();
	DestroyWindow(Handle);

	return true;
}

//---------------------------------------------------------------------------
// Suppression d'un composant enfant
//---------------------------------------------------------------------------

void TWinControl::RemoveComponent(TComponent* AComponent) {
	int i;


	for (i = 0; i < (int) FComponents.size(); i++) {
		if (FComponents[i] == AComponent) {
			FComponents.erase(FComponents.begin() + i);
			if (CurrentFocus == i) CurrentFocus = -1;
			else if (CurrentFocus > i) CurrentFocus--;
		}
	}
}

//---------------------------------------------------------------------------
// Demande de dessin du composant
//---------------------------------------------------------------------------

bool FASTCALL TWinControl::Invalidate(bool bErase) {
	return (InvalidateRect(FHandle, NULL, bErase? TRUE: FALSE) == TRUE);
}

//---------------------------------------------------------------------------
// Dessin immédiat du composant si besoin
//---------------------------------------------------------------------------

bool FASTCALL TWinControl::Update(void) {
	UpdateWindow(FHandle);

	return true;
}

//---------------------------------------------------------------------------
// Demande de dessin du composant
//---------------------------------------------------------------------------

bool FASTCALL TWinControl::Repaint(void) {
	InvalidateRect(FHandle, NULL, FALSE);
	UpdateWindow(FHandle);

	return true;
}

//---------------------------------------------------------------------------
// Transformation des coordonnées client en coordonnées écran
//---------------------------------------------------------------------------

TPoint TWinControl::ClientToScreen(const TPoint &Point) {
	POINT Pt;

	Pt.x = Point.x;
	Pt.y = Point.y;
	::ClientToScreen(FHandle, &Pt);

	return TPoint(Pt);
}

//---------------------------------------------------------------------------
// Transformation des coordonnées écran en coordonnées client
//---------------------------------------------------------------------------

TPoint TWinControl::ScreenToClient(const TPoint &Point) {
	POINT Pt;

	Pt.x = Point.x;
	Pt.y = Point.y;
	::ScreenToClient(FHandle, &Pt);

	return TPoint(Pt);
}

//---------------------------------------------------------------------------
// Détermine si le contrôle a le focus
//---------------------------------------------------------------------------

bool TWinControl::Focused(void) {
	return (::GetFocus() == FHandle);
}

//---------------------------------------------------------------------------
// Détermine si le contrôle ou un de ses enfants a le focus
//---------------------------------------------------------------------------

bool TWinControl::FocusedChild(void) {
	HWND hWndFocused = ::GetFocus();
	return (hWndFocused == FHandle || IsChild(FHandle, hWndFocused));
}

//---------------------------------------------------------------------------
// Détermine si le contrôle peut recevoir le focus
//---------------------------------------------------------------------------

bool FASTCALL TWinControl::CanFocus(void) {
	TControl *Control = this;

	do {
		if (!FVisible || !FEnabled || !FTabStop) return false;
		Control = Control->Parent;
	} while (Control);

	return true;
}

//---------------------------------------------------------------------------
// Lecture du contrôle ayant le focus
//---------------------------------------------------------------------------

TWinControl * FASTCALL TWinControl::GetFocus(void) {
	TWinControl *WinControl;


	if (CurrentFocus != -1) {
		WinControl = dynamic_cast<TWinControl *> (FComponents[CurrentFocus]);
		if (WinControl &&
				WinControl->Visible &&
				WinControl->Enabled) {
			return WinControl->GetFocus();
		}
	}

	return this;
}

//---------------------------------------------------------------------------
// Affectation du focus au contrôle
//---------------------------------------------------------------------------

void FASTCALL TWinControl::SetFocus(void) {
	if (FHandle) ::SetFocus(FHandle);  // Il arrive que FHandle soit NULL en phase de destruction
}

//---------------------------------------------------------------------------
// Affectation du focus au contrôle ou à un de ses enfants
//---------------------------------------------------------------------------

void FASTCALL TWinControl::SetFocusChild(void) {
	if (!SetFocusOnCurrentControl()) SetFocus();
}

//---------------------------------------------------------------------------
// Recherche des fenêtres enfants interceptant la touche entrée
//---------------------------------------------------------------------------

TComponent * TWinControl::ChildWantReturns(void) {
	int i;
	TWinControl *Component;
	TComponent *ComponentWantReturn;


	if (FWantReturns && ::GetFocus() == FHandle) return this;

	for (i = 0; i < (int) FComponents.size(); i++) {
		Component = dynamic_cast<TWinControl *>(FComponents[i]);
		if (Component) {
			ComponentWantReturn = Component->ChildWantReturns();
			if (ComponentWantReturn != NULL) return ComponentWantReturn;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------
// Recherche des fenêtres enfants interceptant la touche tabulation
//---------------------------------------------------------------------------

TComponent * TWinControl::ChildWantTabs(void) {
	int i;
	TWinControl *Component;
	TComponent *ComponentWantTabs;


	if (FWantTabs && ::GetFocus() == FHandle) return this;

	for (i = 0; i < (int) FComponents.size(); i++) {
		Component = dynamic_cast<TWinControl *>(FComponents[i]);
		if (Component) {
			ComponentWantTabs = Component->ChildWantTabs();
			if (ComponentWantTabs != NULL) return ComponentWantTabs;
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------
void TWinControl::InitFocus(void) {
	int i;
	TWinControl *WinControl;


	CurrentFocus = -1;
	for (i = 0; i < (int) FComponents.size(); i++) {
		WinControl = dynamic_cast<TWinControl *>(FComponents[i]);
		if (WinControl) WinControl->InitFocus();
	}

}

//---------------------------------------------------------------------------
bool TWinControl::SetCurrentFocus(void) {
	int i;
	TWinControl *WinControlParent;


	WinControlParent = dynamic_cast<TWinControl *> (FParent);
	if (WinControlParent) {
		WinControlParent->SetCurrentFocus();

		for (i = 0; i < (int) WinControlParent->FComponents.size(); i++) {
			if (WinControlParent->FComponents[i] == this) {
				WinControlParent->CurrentFocus = i;
				return true;
			}
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::KillCurrentFocus(void) {
	TWinControl *WinControlParent;


	WinControlParent = dynamic_cast<TWinControl *> (FParent);
	if (WinControlParent) {
		if (WinControlParent->CurrentFocus != -1 &&
				WinControlParent->FComponents[WinControlParent->CurrentFocus] == this) {
			WinControlParent->CurrentFocus = -1;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::SetFocusOnCurrentControl(void) {
	TWinControl *WinControl;


	if (CurrentFocus == -1) {
		if (SetFocusOnNextControl(true)) return true;
	}
	else {
		WinControl = dynamic_cast<TWinControl *>(FComponents[CurrentFocus]);
		if (WinControl &&
				WinControl->Visible &&
				WinControl->Enabled &&
				WinControl->TabStop) {
			if (!WinControl->SetFocusOnCurrentControl()) WinControl->SetFocus();
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TWinControl::SetFocusOnNextControl(bool bDown) {
	TWinControl *WinControl;
	int NextFocus;
	int NextTabOrder;
	int CurrentTabOrder;
	int TabOrder;
	int i;


	if (CurrentFocus == -1) {
		if (bDown) CurrentTabOrder = -1;
		else CurrentTabOrder = INT_MAX;
	}
	else {
		WinControl = dynamic_cast<TWinControl *> (FComponents[CurrentFocus]);
		if (WinControl) {
			if (WinControl->Visible &&
					WinControl->Enabled &&
					WinControl->TabStop) {
				if (WinControl->SetFocusOnNextControl(bDown)) return true;
			}
			CurrentTabOrder = WinControl->TabOrder;
		}
		else CurrentTabOrder = INT_MAX;
	}

	NextFocus = -1;
	if (bDown) NextTabOrder = INT_MAX;
	else NextTabOrder = -1;
	for (i = 0; i < (int) FComponents.size(); i++) {
		WinControl = dynamic_cast<TWinControl *>(FComponents[i]);
		if (WinControl) {
			if (WinControl->Visible &&
					WinControl->Enabled &&
					WinControl->TabStop) {
				TabOrder = WinControl->TabOrder;
				if (bDown) {
					if (TabOrder > CurrentTabOrder && TabOrder < NextTabOrder) {
						NextFocus = i;
						NextTabOrder = TabOrder;
					}
				}
				else {
					if (TabOrder < CurrentTabOrder && TabOrder > NextTabOrder) {
						NextFocus = i;
						NextTabOrder = TabOrder;
					}
				}
			}
		}
	}

	if (NextFocus != -1) {
		WinControl = dynamic_cast<TWinControl *> (FComponents[NextFocus]);
		if (WinControl && !WinControl->SetFocusOnNextControl(bDown)) WinControl->SetFocus();
		CurrentFocus = NextFocus;
		return true;
	}
	else {
		CurrentFocus = -1;
		return false;
	}
}

//---------------------------------------------------------------------------
bool TWinControl::ChangeWindowStyle(DWORD NewWindowStyle) {

	if (FHandle) {

		// Relecture du style de la fenêtre (qui a pu changer suite à des évènements
		// extérieurs: clic souris, ...)
		FWindowStyle = GetWindowLong(FHandle, GWL_STYLE);

		if (FWindowStyle != NewWindowStyle) {
			STYLESTRUCT StyleStruct;
			StyleStruct.styleOld = FWindowStyle;
			StyleStruct.styleNew = NewWindowStyle;
			SendMessage(FHandle, WM_STYLECHANGING, (WPARAM) GWL_STYLE, (LPARAM) &StyleStruct);
			SetWindowLongPtr(FHandle, GWL_STYLE, NewWindowStyle);
			SendMessage(FHandle, WM_STYLECHANGED, (WPARAM) GWL_STYLE, (LPARAM) &StyleStruct);
		}

	}

	FWindowStyle = NewWindowStyle;

	return true;
}

//---------------------------------------------------------------------------
bool TWinControl::ChangeExWindowStyle(DWORD NewExWindowStyle) {

	if (FHandle) {

		// Relecture du style de la fenêtre (qui a pu changer suite à des évènements
		// extérieurs: clic souris, ...)
		FExWindowStyle = GetWindowLong(FHandle, GWL_EXSTYLE);

		if (FExWindowStyle != NewExWindowStyle) {
			STYLESTRUCT StyleStruct;
			StyleStruct.styleOld = FExWindowStyle;
			StyleStruct.styleNew = NewExWindowStyle;
			SendMessage(FHandle, WM_STYLECHANGING, (WPARAM) GWL_EXSTYLE, (LPARAM) &StyleStruct);
			SetWindowLongPtr(FHandle, GWL_EXSTYLE, NewExWindowStyle);
			SendMessage(FHandle, WM_STYLECHANGED, (WPARAM) GWL_EXSTYLE, (LPARAM) &StyleStruct);
		}

	}

	FExWindowStyle = NewExWindowStyle;

	return true;
}

//---------------------------------------------------------------------------
// Renvoie la longueur du texte du contrôle.
//---------------------------------------------------------------------------
int FASTCALL TWinControl::GetTextLen(void) {
	if (FHandle) return GetWindowTextLength(FHandle);
	return TControl::GetTextLen();
}

//---------------------------------------------------------------------------
// Récupère le texte du contrôle, le copie dans un tampon et renvoie
// le nombre de caractères copiés.
//---------------------------------------------------------------------------
int FASTCALL TWinControl::GetTextBuf(TCHAR * Buffer, int BufSize) {
	if (FHandle) return GetWindowText(FHandle, Buffer, BufSize);
	return TControl::GetTextBuf(Buffer, BufSize);
}

//---------------------------------------------------------------------------
// Définit le texte du contrôle.
//---------------------------------------------------------------------------
void FASTCALL TWinControl::SetTextBuf(const TCHAR * Buffer) {
	if (FHandle) SetWindowText(FHandle, Buffer);
	FCaption = Buffer;
}

//---------------------------------------------------------------------------
// Initialistion des contrôles Windows (Edit, Button, ...)
//---------------------------------------------------------------------------

void TWinControl::InitStdControl(void) {
	DialogProc = (DLGPROC) (SIZE_T) GetWindowLongPtr(FHandle, GWLP_WNDPROC);
	FVCL_ASSERT((DLGPROC) DialogProc != (DLGPROC) WndProc && "InitStdControl sur composant non standard");
	SetWindowLongPtr(FHandle, GWLP_WNDPROC, (LONG_PTR) (SIZE_T) WndProc);
	SetWindowLongPtr(FHandle, GWLP_USERDATA, (LONG_PTR) (SIZE_T) this);
}

//---------------------------------------------------------------------------
LRESULT FAR PASCAL TWinControl::WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	TWinControl *WinControl;
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	LRESULT Ret = 0;
	bool bReturnRet = false;


	WinControl = reinterpret_cast<TWinControl *>((SIZE_T) GetWindowLongPtr(Handle, GWLP_USERDATA));
	if (Message == WM_CREATE && WinControl == NULL) {
		LPCREATESTRUCT lpCreatStruct = (LPCREATESTRUCT) lParam;
		WinControl = (TWinControl *) lpCreatStruct->lpCreateParams;
		SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR) (SIZE_T) WinControl);
	}

	if (WinControl) {

		// Interception des messages par MESSAGE_MAP
		TMessage StructMessage;
		StructMessage.Msg = Message;
		StructMessage.WParam = (int) wParam;
		StructMessage.LParam = (int) lParam;
		StructMessage.Result = FALSE;
		WinControl->Dispatch(&StructMessage);
		if (StructMessage.Result != FALSE) return StructMessage.Result;

		if (WinControl->DialogProc) {
			// 17/03/2011
			// Mis l'appel de la fonction CallWindowProc en début car certains messages ne marchent pas avant
			// Exemple: le message TCM_ADJUSTRECT d'un TPageControl avant le traitement définitif d'un WM_SIZE
			Ret = CallWindowProc((WNDPROC) WinControl->DialogProc, Handle, Message, wParam, lParam);
			bReturnRet = true;
		}

		if (!((TComponentStates) WinControl->ComponentState).Contains(csLoading) &&
				WinControl->bProcessCreateToDo) {
			// La surcharge de ProcessCreate ne marche pas dans le constructeur
			// Il faut donc l'appeler explicitement dans le premier message reçu
			WinControl->bProcessCreateToDo = false;
			WinControl->ProcessCreate();
		}

		/*
		if (WinControl->HintWindow != NULL && WM_MOUSEFIRST <= Message && Message <= WM_MOUSELAST) {
			MSG Msg;
			Msg.hwnd = Handle;
			Msg.message = Message;
			Msg.wParam = wParam;
			Msg.lParam = lParam;
			GetCursorPos(&Msg.pt);
			Msg.time = GetTickCount();
			SendMessage(WinControl->HintWindow->FHandle, TTM_RELAYEVENT, GetMessageExtraInfo(), (LPARAM) (LPSTR) &Msg);
			// SendMessage(WinControl->HintWindow->FHandle, TTM_POP, 0, 0);
		}
		*/

		switch (Message) {

		case WM_PAINT:
			// Windows ne renvoie pas systématiquement le message WM_SHOWWINDOW,
			// donc on le fait sur le premier paint reçu
			if (WinControl->bProcessShowWindowToDo) WinControl->ProcessShowWindow(true);

			if (WinControl->bWantPaint) {
				hdc = BeginPaint(Handle, &ps);
				WinControl->ProcessPaint(hdc, ps.rcPaint);
				EndPaint(Handle, &ps);
				return 0;
			}
			break;

		case WM_PRINTCLIENT:
			if (WinControl->ProcessPrintClient((HDC) wParam, (DWORD) lParam)) return 0;
			break;

		case WM_MEASUREITEM:
			if (WinControl->DistributionMeasureItem(
					(UINT) wParam, (LPMEASUREITEMSTRUCT) lParam)) return 0;
			break;

		case WM_DRAWITEM:
			if (WinControl->DistributionDrawItem(
					(UINT) wParam, (LPDRAWITEMSTRUCT) lParam)) return 0;
			break;

		case WM_ACTIVATEAPP:
			if (WinControl->ProcessActivateApp(wParam != 0)) return 0;
			break;

		case WM_ACTIVATE:
			if (WinControl->ProcessActivate(LOWORD(wParam))) return 0;
			break;

		case WM_SHOWWINDOW:
			if (WinControl->ProcessShowWindow(wParam != 0)) return 0;
			break;

		case WM_SETFOCUS:
			if (WinControl->ProcessSetFocus((HWND) wParam)) return 0;
			break;

		case WM_KILLFOCUS:
			if (WinControl->ProcessKillFocus((HWND) wParam)) return 0;
			break;

		case WM_MOVING:
			if (WinControl->ProcessMoving((int) wParam, (LPRECT) lParam)) return 0;
			break;

		case WM_SIZING:
			if (WinControl->ProcessSizing((int) wParam, (LPRECT) lParam)) return 0;
			break;

		case WM_MOVE:
			if (WinControl->ProcessMove(
					(short) LOWORD(lParam), (short) HIWORD(lParam))) return 0;
			break;

		case WM_SIZE:
			if (WinControl->ProcessSize((int) wParam,
					(short) LOWORD(lParam), (short) HIWORD(lParam))) return 0;
			break;

		case WM_HSCROLL:
			if (lParam == 0) lParam = (LPARAM) Handle;
			if (WinControl->DistributionHScroll((HWND) lParam,
					(short) LOWORD(wParam), (short) HIWORD(wParam))) return 0;
			break;

		case WM_VSCROLL:
			if (lParam == 0) lParam = (LPARAM) Handle;
			if (WinControl->DistributionVScroll((HWND) lParam,
					(short) LOWORD(wParam), (short) HIWORD(wParam))) return 0;
			break;

		/* Ces messages sont traités par InterceptionMessage */
		/* Remis le 12/08/2011 car sinon SendMessage ne marche plus */

		case WM_KEYDOWN:
			if (WinControl->DistributionKeyDown(Handle, (int) wParam)) return 0;
			break;

		case WM_KEYUP:
			if (WinControl->DistributionKeyUp(Handle, (int) wParam)) return 0;
			break;

		case WM_CHAR:
			if (WinControl->DistributionChar(Handle, (int) wParam)) return 0;
			break;

		case WM_LBUTTONDOWN:
			if (WinControl->ProcessLButtonDown(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_LBUTTONUP:
			if (WinControl->ProcessLButtonUp(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_LBUTTONDBLCLK:
			if (WinControl->ProcessLButtonDblClk(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_MBUTTONDOWN:
			if (WinControl->ProcessMButtonDown(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_MBUTTONUP:
			if (WinControl->ProcessMButtonUp(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_MBUTTONDBLCLK:
			if (WinControl->ProcessMButtonDblClk(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_RBUTTONDOWN:
			if (WinControl->ProcessRButtonDown(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_RBUTTONUP:
			if (WinControl->ProcessRButtonUp(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_RBUTTONDBLCLK:
			if (WinControl->ProcessRButtonDblClk(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_MOUSEMOVE:
			if (WinControl->ProcessMouseMove(
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_MOUSEWHEEL:
			if (WinControl->ProcessMouseWheel(
					(WORD) LOWORD(wParam), (short) HIWORD(wParam),
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))) return 0;
			break;

		case WM_CAPTURECHANGED:
			if (WinControl->ProcessCaptureChanged((HWND) lParam)) return 0;
			break;

		case WM_CONTEXTMENU:
			// Handle car (HWND) wParam peut contenir le handle d'un contrôle enfant
			if (WinControl->DistributionContextMenu(Handle,
					(short) LOWORD(lParam), (short) HIWORD(lParam))) return 0;
			break;

		case WM_DROPFILES:
			if (WinControl->ProcessDropFiles((HDROP) wParam)) return 0;
			break;

		case WM_TIMER:
			if (WinControl->ProcessTimer(LOWORD(wParam))) return 0;
			break;

		case WM_COMMAND:
			if (WinControl->DistributionCommand(LOWORD(wParam), HIWORD(wParam))) return 0;
			break;

		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			hBrush = NULL;
			WinControl->DistributionColor((HWND) lParam, (HDC) wParam, &hBrush);
			return (LRESULT) hBrush;

		case WM_NOTIFY:
			if (WinControl->DistributionNotify((int) wParam, (LPNMHDR) lParam)) return 0;
			break;

		case WM_CLOSE:
			if (WinControl->ProcessClose()) return 0;
			break;

		case WM_DESTROY:
			WinControl->ProcessDestroy();
			WinControl->FHandle = NULL;
			SetWindowLongPtr(Handle, GWLP_USERDATA, 0);
			return 0;

		case WM_QUERYENDSESSION:
			if (!Application->ProcessQueryEndSession()) return 0;
			break;

		case WM_ENDSESSION:
			if (!Application->ProcessEndSession()) return 0;
			break;

		default:
			if (WM_USER <= Message && Message <= 0xBFFF) {
				if (WinControl->ProcessUser(Message, wParam, lParam)) return 0;
			}
			break;

		}

	}

	if (bReturnRet) return Ret;
	else return DefWindowProc(Handle, Message, wParam, lParam);

	/* 17/03/2011

	// Relecture du WinControl (la fenêtre a pu être détruite dans le traitement
	// d'un message, par exemple
	WinControl = reinterpret_cast<TWinControl *>((SIZE_T) GetWindowLongPtr(Handle, GWLP_USERDATA));
	if (WinControl) {

		if (WinControl->DialogProc) {
			return CallWindowProc((WNDPROC) WinControl->DialogProc, Handle, Message, wParam, lParam);
		}

	}

	return DefWindowProc(Handle, Message, wParam, lParam);
	*/

}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TWinControl::GetListProperties(TStrings *ListProperties) {

	TControl::GetListProperties(ListProperties);

	ListProperties->Add(_T("TabStop"));
	ListProperties->Add(_T("TabOrder"));
	ListProperties->Add(_T("WantReturns"));
	ListProperties->Add(_T("WantTabs"));
	ListProperties->Add(_T("DragAcceptFiles"));
	ListProperties->Add(_T("DragKind"));
	ListProperties->Add(_T("OnShow"));
	ListProperties->Add(_T("OnHide"));
	ListProperties->Add(_T("OnKeyDown"));
	ListProperties->Add(_T("OnKeyUp"));
	ListProperties->Add(_T("OnKeyPress"));
	ListProperties->Add(_T("OnMouseMove"));
	ListProperties->Add(_T("OnMouseDown"));
	ListProperties->Add(_T("OnMouseUp"));
	ListProperties->Add(_T("OnMouseWheel"));
	ListProperties->Add(_T("OnEnter"));
	ListProperties->Add(_T("OnExit"));
	ListProperties->Add(_T("OnDropFile"));
	ListProperties->Add(_T("OnDragOver"));
	ListProperties->Add(_T("OnStartDrag"));
	ListProperties->Add(_T("OnEndDrag"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TWinControl::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("TabStop")) {
		return tpBool;
	}
	if (asProperty == _T("TabOrder")) {
		return tpNumber;
	}
	if (asProperty == _T("WantReturns")) {
		return tpBool;
	}
	if (asProperty == _T("WantTabs")) {
		return tpBool;
	}
	if (asProperty == _T("DragAcceptFiles")) {
		return tpBool;
	}
	if (asProperty == _T("DragKind")) {
		*asInfos = _T("dkDrag\ndkDock");
		return tpChoice;
	}
	if (asProperty == _T("OnShow")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnHide")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnKeyDown")) {
    *asInfos = _T("(TObject *Sender, Word Key, TShiftState Shift)");
		return tpEvent;
	}
	if (asProperty == _T("OnKeyUp")) {
    *asInfos = _T("(TObject *Sender, Word Key, TShiftState Shift))");
		return tpEvent;
	}
	if (asProperty == _T("OnKeyPress")) {
    *asInfos = _T("(TObject *Sender, TCHAR Char)");
		return tpEvent;
	}
	if (asProperty == _T("OnMouseMove")) {
    *asInfos = _T("(TObject *Sender, TShiftState Shift, int X, int Y)");
		return tpEvent;
	}
	if (asProperty == _T("OnMouseDown")) {
    *asInfos = _T("(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)");
		return tpEvent;
	}
	if (asProperty == _T("OnMouseUp")) {
    *asInfos = _T("(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)");
		return tpEvent;
	}
	if (asProperty == _T("OnMouseWheel")) {
    *asInfos = _T("(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled)");
		return tpEvent;
	}
	if (asProperty == _T("OnEnter")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnExit")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnDropFile")) {
    *asInfos = _T("(TObject *Sender, AnsiString FileName, TShiftState Shift, int X, int Y)");
		return tpEvent;
	}
	if (asProperty == _T("OnDragOver")) {
    *asInfos = _T("(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept)");
		return tpEvent;
	}
	if (asProperty == _T("OnStartDrag")) {
    *asInfos = _T("(TObject *Sender, TDragObject *DragObject)");
		return tpEvent;
	}
	if (asProperty == _T("OnEndDrag")) {
    *asInfos = _T("(TObject *Sender, TObject *Target, int X, int Y)");
		return tpEvent;
	}
	return TControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TWinControl::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("TabStop")) {
		return _T("False");
	}
	if (asProperty == _T("TabOrder")) {
		return _T("-1");
	}
	if (asProperty == _T("WantReturns")) {
		return _T("False");
	}
	if (asProperty == _T("WantTabs")) {
		return _T("False");
	}
	if (asProperty == _T("DragAcceptFiles")) {
		return _T("False");
	}
	if (asProperty == _T("DragKind")) {
		return _T("dkDrag");
	}
	if (asProperty == _T("OnShow")) {
		return _T("");
	}
	if (asProperty == _T("OnHide")) {
		return _T("");
	}
	if (asProperty == _T("OnKeyDown")) {
		return _T("");
	}
	if (asProperty == _T("OnKeyUp")) {
		return _T("");
	}
	if (asProperty == _T("OnKeyPress")) {
		return _T("");
	}
	if (asProperty == _T("OnMouseMove")) {
		return _T("");
	}
	if (asProperty == _T("OnMouseDown")) {
		return _T("");
	}
	if (asProperty == _T("OnMouseUp")) {
		return _T("");
	}
	if (asProperty == _T("OnMouseWheel")) {
		return _T("");
	}
	if (asProperty == _T("OnEnter")) {
		return _T("");
	}
	if (asProperty == _T("OnExit")) {
		return _T("");
	}
	if (asProperty == _T("OnDropFile")) {
		return _T("");
	}
	if (asProperty == _T("OnDragOver")) {
		return _T("");
	}
	if (asProperty == _T("OnStartDrag")) {
		return _T("");
	}
	if (asProperty == _T("OnEndDrag")) {
		return _T("");
	}
	return TControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TWinControl::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("TabStop")) {
		if (Get_TabStop()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("TabOrder")) {
		return IntToStr(Get_TabOrder());
	}
	if (asProperty == _T("WantReturns")) {
		if (Get_WantReturns()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("WantTabs")) {
		if (Get_WantTabs()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("DragAcceptFiles")) {
		if (Get_DragAcceptFiles()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("DragKind")) {
		if (FDragKind == dkDrag) return _T("dkDrag");
		if (FDragKind == dkDock) return _T("dkDock");
		return _T("");
	}
	if (asProperty == _T("OnShow")) {
		return OnShow.AsString();
	}
	if (asProperty == _T("OnHide")) {
		return OnHide.AsString();
	}
	if (asProperty == _T("OnKeyDown")) {
		return OnKeyDown.AsString();
	}
	if (asProperty == _T("OnKeyUp")) {
		return OnKeyUp.AsString();
	}
	if (asProperty == _T("OnKeyPress")) {
		return OnKeyPress.AsString();
	}
	if (asProperty == _T("OnMouseMove")) {
		return OnMouseMove.AsString();
	}
	if (asProperty == _T("OnMouseDown")) {
		return OnMouseDown.AsString();
	}
	if (asProperty == _T("OnMouseUp")) {
		return OnMouseUp.AsString();
	}
	if (asProperty == _T("OnMouseWheel")) {
		return OnMouseWheel.AsString();
	}
	if (asProperty == _T("OnEnter")) {
		return OnEnter.AsString();
	}
	if (asProperty == _T("OnExit")) {
		return OnExit.AsString();
	}
	if (asProperty == _T("OnDropFile")) {
		return OnDropFile.AsString();
	}
	if (asProperty == _T("OnDragOver")) {
		return OnDragOver.AsString();
	}
	if (asProperty == _T("OnStartDrag")) {
		return OnStartDrag.AsString();
	}
	if (asProperty == _T("OnEndDrag")) {
		return OnEndDrag.AsString();
	}
	return TControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TWinControl::SetProperty(AnsiString asProperty, AnsiString asValue,
															TPersistent *Sender) {
	HFONT hFont;


	if (asProperty == _T("TabStop")) {
		Set_TabStop(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("TabOrder")) {
		Set_TabOrder(StrToInt(asValue));
		return true;
	}
	if (asProperty == _T("WantReturns")) {
		Set_WantReturns(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("WantTabs")) {
		Set_WantTabs(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("DragAcceptFiles")) {
		Set_DragAcceptFiles(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("DragKind")) {
		if (asValue == _T("dkDrag")) Set_DragKind(dkDrag);
		else if (asValue == _T("dkDock")) Set_DragKind(dkDock);
		return true;
	}
	if (asProperty == _T("OnShow")) {
		OnShow.ObjCall = Sender;
		OnShow = asValue.c_str();
		bProcessShowWindowToDo = true;
		return true;
	}
	if (asProperty == _T("OnHide")) {
		OnHide.ObjCall = Sender;
		OnHide = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnKeyDown")) {
		OnKeyDown.ObjCall = Sender;
		OnKeyDown = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnKeyUp")) {
		OnKeyUp.ObjCall = Sender;
		OnKeyUp = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnKeyPress")) {
		OnKeyPress.ObjCall = Sender;
		OnKeyPress = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnMouseMove")) {
		OnMouseMove.ObjCall = Sender;
		OnMouseMove = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnMouseDown")) {
		OnMouseDown.ObjCall = Sender;
		OnMouseDown = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnMouseUp")) {
		OnMouseUp.ObjCall = Sender;
		OnMouseUp = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnMouseWheel")) {
		OnMouseWheel.ObjCall = Sender;
		OnMouseWheel = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnEnter")) {
		OnEnter.ObjCall = Sender;
		OnEnter = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnExit")) {
		OnExit.ObjCall = Sender;
		OnExit = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnDropFile")) {
		OnDropFile.ObjCall = Sender;
		OnDropFile = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnDragOver")) {
		OnDragOver.ObjCall = Sender;
		OnDragOver = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnStartDrag")) {
		OnStartDrag.ObjCall = Sender;
		OnStartDrag = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnEndDrag")) {
		OnEndDrag.ObjCall = Sender;
		OnEndDrag = asValue.c_str();
		return true;
	}
	if (asProperty.Length() >= 5 && asProperty.SubString(1, 5) == _T("Font.")) {
		asProperty = asProperty.SubString(6, asProperty.Length() - 5);
		if (FFont->SetProperty(asProperty, asValue, Sender)) {
			hFont = FFont->Handle;
			SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
			return true;
		}
		return false;
	}
	return TControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
