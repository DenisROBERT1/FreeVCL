//-----------------------------------------------------------------------------
//! @file TShape.cpp
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
// TShape
//---------------------------------------------------------------------------

TShape::TShape(TComponent* AOwner): TWinControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TShape");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FColor = clWindow;
	FPen = new TPen();
	FBrush = new TBrush();
  FRoundWidth = 10;
  FRoundHeight = 10;
  FShape = stRectangle;

  // Enregistrement de la classe "TShape":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TShape");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TShape"), FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    bWantPaint = true;
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TShape::~TShape(void) {
	delete FBrush;
	delete FPen;
}

//---------------------------------------------------------------------------
bool TShape::ProcessPaint(HDC hdc, RECT rcPaint) {
	RECT Rect;
	HPEN hOldPen;
	HBRUSH hOldBrush;


	// Rectangle de dessin
	::GetClientRect(FHandle, &Rect);
	if (FShape == stSquare || FShape == stRoundSquare || FShape == stCircle) {
		int l = Rect.right - Rect.left;
		int h = Rect.bottom - Rect.top;
		if (l < h) {
			Rect.top = (Rect.top + Rect.bottom) / 2 - l / 2;
			Rect.bottom = Rect.top + l;
		}
		else if (l > h) {
			Rect.left = (Rect.left + Rect.right) / 2 - h / 2;
			Rect.right = Rect.left + h;
		}
	}

	// Dessin
	hOldPen = (HPEN) SelectObject(hdc, FPen->Handle);
	hOldBrush = (HBRUSH) SelectObject(hdc, FBrush->Handle);
	switch (FShape) {
		case stRectangle:
		case stSquare:
			Rectangle(hdc, Rect.left, Rect.top, Rect.right, Rect.bottom);
			break;
		case stRoundRect:
		case stRoundSquare:
			RoundRect(hdc, Rect.left, Rect.top, Rect.right, Rect.bottom, FRoundWidth, FRoundHeight);
			break;
		case stEllipse:
		case stCircle:
			Ellipse(hdc, Rect.left, Rect.top, Rect.right, Rect.bottom);
			break;
	}
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

  return false;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Pen
//---------------------------------------------------------------------------

TPen *TShape::Get_Pen() {
  return FPen;
}

bool TShape::Set_Pen(TPen *NewPen) {
  if (FPen != NewPen) {
    FPen->Assign(NewPen);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Brush
//---------------------------------------------------------------------------

TBrush *TShape::Get_Brush() {
  return FBrush;
}

bool TShape::Set_Brush(TBrush *NewBrush) {
  if (FBrush != NewBrush) {
    FBrush->Assign(NewBrush);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Shape
//---------------------------------------------------------------------------

TShapeType TShape::Get_Shape() {
  return FShape;
}

bool TShape::Set_Shape(TShapeType NewShape) {
  if (FShape != NewShape) {
    FShape = NewShape;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété RoundWidth
//---------------------------------------------------------------------------

int TShape::Get_RoundWidth() {
  return FRoundWidth;
}

bool TShape::Set_RoundWidth(int NewRoundWidth) {
  if (FRoundWidth != NewRoundWidth) {
    FRoundWidth = NewRoundWidth;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété RoundHeight
//---------------------------------------------------------------------------

int TShape::Get_RoundHeight() {
  return FRoundHeight;
}

bool TShape::Set_RoundHeight(int NewRoundHeight) {
  if (FRoundHeight != NewRoundHeight) {
    FRoundHeight = NewRoundHeight;
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TShape::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Pen.Style"));
  ListProperties->Add(_T("Pen.Width"));
  ListProperties->Add(_T("Pen.Color"));
  ListProperties->Add(_T("Brush"));
  ListProperties->Add(_T("RoundWidth"));
  ListProperties->Add(_T("RoundHeight"));
  ListProperties->Add(_T("Shape"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TShape::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Pen.Style")) {
    return FPen->GetTypeProperty(_T("Style"), asInfos);
  }
  if (asProperty == _T("Pen.Width")) {
    return FPen->GetTypeProperty(_T("Width"), asInfos);
  }
  if (asProperty == _T("Pen.Color")) {
    return FPen->GetTypeProperty(_T("Color"), asInfos);
  }
  if (asProperty == _T("Brush.Style")) {
    return FPen->GetTypeProperty(_T("Style"), asInfos);
  }
  if (asProperty == _T("Brush.Color")) {
    return FPen->GetTypeProperty(_T("Color"), asInfos);
  }
  if (asProperty == _T("RoundWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("RoundHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("Shape")) {
    *asInfos = _T("stRectangle\nstSquare\nstRoundRect\nstRoundSquare\nstEllipse\nstCircle");
    return tpChoice;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TShape::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Pen.Style")) {
    return FPen->GetDefaultProperty(_T("Style"));
  }
  if (asProperty == _T("Pen.Width")) {
    return FPen->GetDefaultProperty(_T("Width"));
  }
  if (asProperty == _T("Pen.Color")) {
    return FPen->GetDefaultProperty(_T("Color"));
  }
  if (asProperty == _T("Brush.Style")) {
    return FBrush->GetDefaultProperty(_T("Style"));
  }
  if (asProperty == _T("Brush.Color")) {
    return FBrush->GetDefaultProperty(_T("Color"));
  }
  if (asProperty == _T("RoundWidth")) {
    return _T("10");
  }
  if (asProperty == _T("RoundHeight")) {
    return _T("10");
  }
  if (asProperty == _T("Shape")) {
    return _T("stRectangle");
  }
  if (asProperty == _T("Color")) {
    return _T("clWindow");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TShape::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Pen.Style")) {
    return FPen->GetProperty(_T("Style"));
  }
  if (asProperty == _T("Pen.Width")) {
    return FPen->GetProperty(_T("Width"));
  }
  if (asProperty == _T("Pen.Color")) {
    return FPen->GetProperty(_T("Color"));
  }
  if (asProperty == _T("Brush.Style")) {
    return FBrush->GetProperty(_T("Style"));
  }
  if (asProperty == _T("Brush.Color")) {
    return FBrush->GetProperty(_T("Color"));
  }
  if (asProperty == _T("RoundWidth")) {
    return IntToStr(Get_RoundWidth());
  }
  if (asProperty == _T("RoundHeight")) {
    return IntToStr(Get_RoundHeight());
  }
  if (asProperty == _T("Shape")) {
    TShapeType Shape = Get_Shape();
    if (Shape == stRectangle) return _T("stRectangle");
    if (Shape == stSquare) return _T("stSquare");
    if (Shape == stRoundRect) return _T("stRoundRect");
    if (Shape == stRoundSquare) return _T("stRoundSquare");
    if (Shape == stEllipse) return _T("stEllipse");
    if (Shape == stCircle) return _T("stCircle");
    return _T("");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TShape::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Pen.Style")) {
    return FPen->SetProperty(_T("Style"), asValue, Sender);
  }
  if (asProperty == _T("Pen.Width")) {
    return FPen->SetProperty(_T("Width"), asValue, Sender);
  }
  if (asProperty == _T("Pen.Color")) {
    return FPen->SetProperty(_T("Color"), asValue, Sender);
  }
  if (asProperty == _T("Brush.Style")) {
    return FBrush->SetProperty(_T("Style"), asValue, Sender);
  }
  if (asProperty == _T("Brush.Color")) {
    return FBrush->SetProperty(_T("Color"), asValue, Sender);
  }
  if (asProperty == _T("RoundWidth")) {
    Set_RoundWidth(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("RoundHeight")) {
    Set_RoundHeight(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Shape")) {
    if      (asValue == _T("stRectangle")) Set_Shape(stRectangle);
    else if (asValue == _T("stSquare")) Set_Shape(stSquare);
    else if (asValue == _T("stRoundRect")) Set_Shape(stRoundRect);
    else if (asValue == _T("stRoundSquare")) Set_Shape(stRoundSquare);
    else if (asValue == _T("stEllipse")) Set_Shape(stEllipse);
    else if (asValue == _T("stCircle")) Set_Shape(stCircle);
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

