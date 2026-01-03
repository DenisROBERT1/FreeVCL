//-----------------------------------------------------------------------------
//! @file TControlSizer.cpp
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
#include <windows.h>
#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------
#include "TControlSizer.h"


//---------------------------------------------------------------------------
TControlSizer::TControlSizer(int X, int Y, int Width, int Height, TAnchors Anchors) {
	FVisible = false;
	FX = X;
	FY = Y;
	FWidth = Width;
	FHeight = Height;
	FAnchors = Anchors;
	FData = NULL;
}

//---------------------------------------------------------------------------
TControlSizer::~TControlSizer(void) {
}

//---------------------------------------------------------------------------
void TControlSizer::DrawRect(void) {
	HDC hdc;
	int OldROP2;
	POINT Pt1, Pt2;


	hdc = GetDC(NULL);
	OldROP2 = SetROP2(hdc, R2_NOT);
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
	SetROP2(hdc, OldROP2);
	ReleaseDC(NULL, hdc);
}

//---------------------------------------------------------------------------
bool TControlSizer::SelectAction(int x, int y, ACTION *lpAction) {
	RECT RectObj;
	int X, Y;
	TShiftState ShiftState;
	bool bShift;


	SetRect(&RectObj, FX, FY, FX + FWidth, FY + FHeight);
	PlanToClientRect(&RectObj);

	if (RectObj.left - 4 <= x && x <= RectObj.right + 4 &&
			RectObj.top - 4 <= y && y <= RectObj.bottom + 4) {

		ShiftState = GetShiftState();
		bShift = ShiftState.Contains(ssShift);

		if (!bShift) *lpAction = acMove;

		if (x <= RectObj.left || RectObj.right <= x ||
				y <= RectObj.top || RectObj.bottom <= y) {

			X = RectObj.left - 3;
			Y = (RectObj.top + RectObj.bottom) / 2;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeW;
				else *lpAction = acAnchorsLeft;
				return true;
			}

			X = (RectObj.left + RectObj.right) / 2;
			Y = RectObj.top - 3;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeN;
				else *lpAction = acAnchorsTop;
				return true;
			}

			X = RectObj.right + 3;
			Y = (RectObj.top + RectObj.bottom) / 2;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeE;
				else *lpAction = acAnchorsRight;
				return true;
			}

			X = (RectObj.left + RectObj.right) / 2;
			Y = RectObj.bottom + 3;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeS;
				else *lpAction = acAnchorsBottom;
				return true;
			}

			X = RectObj.left - 3;
			Y = RectObj.top - 3;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeNW;
				else *lpAction = acAnchorsLeftTop;
				return true;
			}

			X = RectObj.right + 3;
			Y = RectObj.top - 3;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeNE;
				else *lpAction = acAnchorsRightTop;
				return true;
			}

			X = RectObj.right + 3;
			Y = RectObj.bottom + 3;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeSE;
				else *lpAction = acAnchorsRightBottom;
				return true;
			}

			X = RectObj.left - 3;
			Y = RectObj.bottom + 3;
			if (X - 3 <= x && x <=  X + 3 && Y - 3 <= y && y <= Y + 3) {
				if (!bShift) *lpAction = acSizeSW;
				else *lpAction = acAnchorsLeftBottom;
				return true;
			}

		}

		return true;

	}

	return false;
}

//---------------------------------------------------------------------------
void TControlSizer::Paint(HDC hdc) {
	HBRUSH hBrush;
	HPEN hOldPen;
	RECT RectObj, Rect;
	int i, X, Y;


	if (FVisible) {

		// Encadrement
		SetRect(&RectObj, FX, FY, FX + FWidth, FY + FHeight);
		PlanToClientRect(&RectObj);

    hBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 255));
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SetBkMode(hdc, TRANSPARENT);
    SetBkColor(hdc, RGB(255, 255, 255));
    Rect = RectObj;
    for (i = 0; i < 3; i++) {
      InflateRect(&Rect, 1, 1);
      FrameRect(hdc, &Rect, hBrush);
    }
    DeleteObject(hBrush);

		// Poignées d'ancrage
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		hOldPen = (HPEN) SelectObject(hdc, GetStockObject(BLACK_PEN));

		if (FAnchors.Contains(akLeft)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = RectObj.left - 3;
		Y = (RectObj.top + RectObj.bottom) / 2;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		if (FAnchors.Contains(akTop)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = (RectObj.left + RectObj.right) / 2;
		Y = RectObj.top - 3;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		if (FAnchors.Contains(akRight)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = RectObj.right + 3;
		Y = (RectObj.top + RectObj.bottom) / 2;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		if (FAnchors.Contains(akBottom)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = (RectObj.left + RectObj.right) / 2;
		Y = RectObj.bottom + 3;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		if (FAnchors.Contains(akLeft) && FAnchors.Contains(akTop)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = RectObj.left - 3;
		Y = RectObj.top - 3;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		if (FAnchors.Contains(akRight) && FAnchors.Contains(akTop)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = RectObj.right + 3;
		Y = RectObj.top - 3;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		if (FAnchors.Contains(akRight) && FAnchors.Contains(akBottom)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = RectObj.right + 3;
		Y = RectObj.bottom + 3;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		if (FAnchors.Contains(akLeft) && FAnchors.Contains(akBottom)) SelectObject(hdc, hBrush);
		else SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		X = RectObj.left - 3;
		Y = RectObj.bottom + 3;
		Rectangle(hdc, X - 3, Y - 3, X + 3, Y + 3);

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		DeleteObject(hBrush);

	}
}

//---------------------------------------------------------------------------
bool TControlSizer::TraitMouseMove(int x, int y, int XRound, int YRound, ACTION *lpAction) {
	RECT RectObj;


	if (FVisible) {

		SetRect(&RectObj, FX, FY, FX + FWidth, FY + FHeight);
		PlanToClientRect(&RectObj);

		if (*lpAction == acNull) {
			return SelectAction(x, y, lpAction);
		}
		else {
			DrawRect();
			switch (*lpAction) {
			case (acSizeNW):
				rcDrawRect.left = XRound;
				rcDrawRect.top = YRound;
				break;
			case (acSizeN):
				rcDrawRect.top = YRound;
				break;
			case (acSizeNE):
				rcDrawRect.right = XRound;
				rcDrawRect.top = YRound;
				break;
			case (acSizeE):
				rcDrawRect.right = XRound;
				break;
			case (acSizeSE):
				rcDrawRect.right = XRound;
				rcDrawRect.bottom = YRound;
				break;
			case (acSizeS):
				rcDrawRect.bottom = YRound;
				break;
			case (acSizeSW):
				rcDrawRect.left = XRound;
				rcDrawRect.bottom = YRound;
				break;
			case (acSizeW):
				rcDrawRect.left = XRound;
				break;
			case (acMove):
				rcDrawRect.left = RectObj.left + XRound - xInit;
				rcDrawRect.top = RectObj.top + YRound - yInit;
				rcDrawRect.right = RectObj.right + XRound - xInit;
				rcDrawRect.bottom = RectObj.bottom + YRound - yInit;
				break;
			}
			DrawRect();
		}

	}

	return false;
}

//---------------------------------------------------------------------------
bool TControlSizer::TraitLButtonDown(int x, int y, int XRound, int YRound, ACTION *lpAction) {
	RECT RectObj;


	if (FVisible) {

		SelectAction(x, y, lpAction);

		SetRect(&RectObj, FX, FY, FX + FWidth, FY + FHeight);
		PlanToClientRect(&RectObj);

		rcDrawRect = RectObj;
		xInit = XRound;
		yInit = YRound;

	}

	return true;
}

//---------------------------------------------------------------------------
bool TControlSizer::TraitLButtonUp(int x, int y, int XRound, int YRound, ACTION *lpAction) {

	if (FVisible) {

		if (*lpAction != acNull) {
			DrawRect();

			ClientToPlanRect(&rcDrawRect);

			if (rcDrawRect.right - rcDrawRect.left > 0) {
				Set_X(rcDrawRect.left);
				Set_Width(rcDrawRect.right - rcDrawRect.left);
			}
			else {
				Set_X(rcDrawRect.right);
				Set_Width(rcDrawRect.left - rcDrawRect.right);
			}

			if (rcDrawRect.bottom - rcDrawRect.top > 0) {
				Set_Y(rcDrawRect.top);
				Set_Height(rcDrawRect.bottom - rcDrawRect.top);
			}
			else {
				Set_Y(rcDrawRect.bottom);
				Set_Height(rcDrawRect.top - rcDrawRect.bottom);
			}

		}

	}

	return true;
}

//---------------------------------------------------------------------------
bool TControlSizer::IsInFrame(int x, int y) {
	RECT RectObj;


	SetRect(&RectObj, FX, FY, FX + FWidth, FY + FHeight);
	PlanToClientRect(&RectObj);

	return (RectObj.left - 4 <= x && x <= RectObj.right + 4 &&
					RectObj.top - 4 <= y && y <= RectObj.bottom + 4);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété X
//---------------------------------------------------------------------------

int TControlSizer::Get_X(void) {
	return FX;
}

bool TControlSizer::Set_X(int NewX) {
	if (FX != NewX) {
		FX = NewX;
		OnXChange(this, NewX);
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Y
//---------------------------------------------------------------------------

int TControlSizer::Get_Y(void) {
	return FY;
}

bool TControlSizer::Set_Y(int NewY) {
	if (FY != NewY) {
		FY = NewY;
		OnYChange(this, NewY);
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TControlSizer::Get_Width(void) {
	return FWidth;
}

bool TControlSizer::Set_Width(int NewWidth) {
	if (FWidth != NewWidth) {
		FWidth = NewWidth;
		OnWidthChange(this, NewWidth);
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TControlSizer::Get_Height(void) {
	return FHeight;
}

bool TControlSizer::Set_Height(int NewHeight) {
	if (FHeight != NewHeight) {
		FHeight = NewHeight;
		OnHeightChange(this, NewHeight);
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété XDrawRect
//---------------------------------------------------------------------------

int TControlSizer::Get_XDrawRect(void) {
	return rcDrawRect.left;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété YDrawRect
//---------------------------------------------------------------------------

int TControlSizer::Get_YDrawRect(void) {
	return rcDrawRect.top;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WidthDrawRect
//---------------------------------------------------------------------------

int TControlSizer::Get_WidthDrawRect(void) {
	return rcDrawRect.right - rcDrawRect.left;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété HeightDrawRect
//---------------------------------------------------------------------------

int TControlSizer::Get_HeightDrawRect(void) {
	return rcDrawRect.bottom - rcDrawRect.top;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Anchors
//---------------------------------------------------------------------------

TAnchors TControlSizer::Get_Anchors(void) {
	return FAnchors;
}

//---------------------------------------------------------------------------
bool TControlSizer::Set_Anchors(TAnchors NewAnchors) {
	if (FAnchors != NewAnchors) {
		FAnchors = NewAnchors;
		OnAnchorsChange(this, NewAnchors);
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool TControlSizer::Get_Visible(void) {
	return FVisible;
}

//---------------------------------------------------------------------------
bool TControlSizer::Set_Visible(bool NewVisible) {
	if (FVisible != NewVisible) {
		FVisible = NewVisible;
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Data
//---------------------------------------------------------------------------

void *TControlSizer::Get_Data(void) {
	return FData;
}

//---------------------------------------------------------------------------
bool TControlSizer::Set_Data(void *NewData) {
	if (FData != NewData) {
		FData = NewData;
	}

	return true;
}

//---------------------------------------------------------------------------
void TControlSizer::PlanToClientRect(LPRECT lpRect) {
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
void TControlSizer::ClientToPlanRect(LPRECT lpRect) {
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
