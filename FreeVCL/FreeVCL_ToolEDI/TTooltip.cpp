//-----------------------------------------------------------------------------
//! @file TTooltip.cpp
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
#include <TTooltip.h>


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

TTooltip *TTooltip::This;

//---------------------------------------------------------------------------
// TTooltip
//---------------------------------------------------------------------------

TTooltip::TTooltip(void): TComponent(NULL) {
	WNDCLASS wc;

	This = this;
	TimeNoEvents = 0;
	Visible = false;
	NbUse = 0;

	// Enregistrement de la classe "TForm":
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH) (COLOR_INFOBK + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("TTooltip");

	RegisterClass(&wc);

	Handle = CreateWindowEx(WS_EX_TOPMOST | WS_EX_NOACTIVATE, _T("TTooltip"), _T(""), WS_BORDER | WS_POPUP, 100, 100, 60, 40, GetDesktopWindow(), NULL, HInstance, NULL);

	if (Handle) idTimer = SetTimer(Handle, 0, 100, NULL);
}

TTooltip::~TTooltip(void) {
	KillTimer(Handle, idTimer);
	DestroyWindow(Handle);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FText
//---------------------------------------------------------------------------

AnsiString TTooltip::Get_Text() {
	return FText;
}

bool TTooltip::Set_Text(AnsiString NewText) {
	if (FText != NewText) {
		FText = NewText;
		if (Visible) InvalidateRect(Handle, NULL, FALSE);
	}
	return true;
}

//---------------------------------------------------------------------------
void FASTCALL TTooltip::OnPaint(HDC hdc, RECT &Rect) {
	RECT rcClient;

	GetClientRect(Handle, &rcClient);
	InflateRect(&rcClient, -3, -3);
	SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	SetTextColor(hdc, GetSysColor(COLOR_INFOTEXT));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, FText, -1, &rcClient, DT_LEFT | DT_TOP | DT_WORDBREAK);
}

//---------------------------------------------------------------------------
void FASTCALL TTooltip::TimerOnTimer(void) {
	TRect Rect;


	if (!Visible) {
		if (++TimeNoEvents >= 5) {
			GetCursorPos(&PtTooltip);
			if (GetFocus() == HandleParent &&	WindowFromPoint(PtTooltip) == HandleParent) {
				OnHint(this);
				if (!FText.IsEmpty()) {
					Rect = GetTooltipRect();
					// Affichage de l'info bulle
					MoveWindow(Handle, PtTooltip.x + 5, PtTooltip.y + 10, Rect.Width, Rect.Height, FALSE);
					ShowWindow(Handle, SW_SHOWNOACTIVATE);
					Visible = true;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void FASTCALL TTooltip::Hide(void) {

	if (Visible) {
		ShowWindow(Handle, SW_HIDE);
		Visible = false;
	}
	TimeNoEvents = 0;
}

//---------------------------------------------------------------------------
void FASTCALL TTooltip::Event(HWND AHandleParent) {

	if (Visible) {
		POINT Pt;
		GetCursorPos(&Pt);
		if (Pt.x < PtTooltip.x - 1 || PtTooltip.x + 1 < Pt.x ||
				Pt.y < PtTooltip.y - 1 || PtTooltip.y + 1 < Pt.y) {
			ShowWindow(Handle, SW_HIDE);
			Visible = false;
		}
	}
	TimeNoEvents = 0;
	HandleParent = AHandleParent;
}

//---------------------------------------------------------------------------
int FASTCALL TTooltip::IncUse(void) {
	return ++NbUse;
}

//---------------------------------------------------------------------------
int FASTCALL TTooltip::DecUse(void) {
	return --NbUse;
}

//---------------------------------------------------------------------------
LRESULT FAR PASCAL TTooltip::WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;


	switch (Message) {

	case WM_PAINT:
		hdc = BeginPaint(Handle, &ps);
		This->OnPaint(hdc, ps.rcPaint);
		EndPaint(Handle, &ps);
		break;

	case WM_TIMER:
		This->TimerOnTimer();
		break;

	}

	return DefWindowProc(Handle, Message, wParam, lParam);
}

//-----------------------------------------------------------------------------
TRect FASTCALL TTooltip::GetTooltipRect(void) {
	TRect Rect;
	HWND hWnd;
	HDC hdc;
	SIZE Size;


	hWnd = GetDesktopWindow();
	hdc = GetDC(hWnd);
	SelectObject(hdc, (HFONT) GetStockObject(DEFAULT_GUI_FONT));
	Rect.Left = 0;
	Rect.Top = 0;
	Rect.Right = 500;
	Rect.Bottom = 30;
	if (GetTextExtentPoint32(hdc, FText, FText.Length(), &Size)) {
		if (Size.cx <= 500) {
			Rect.Right = Size.cx + 8;
			Rect.Bottom = Size.cy + 8;
		}
		else {
			Rect.Right = 506;
			Rect.Bottom = 2 * Size.cy + 8;
		}
	}
	ReleaseDC(hWnd, hdc);

	return Rect;
}

//-----------------------------------------------------------------------------
