//-----------------------------------------------------------------------------
//! @file TCustomGrid.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TCustomGrid
//---------------------------------------------------------------------------

TCustomGrid::TCustomGrid(TComponent* AOwner): TCustomControl(AOwner) {

  // Initialisations
  FClassName = _T("TCustomGrid");
  FTabStop = true;
  FBorderStyle = bsNone;
  Set_BorderStyle(bsSingle);
  DecalageX = 0;
  DecalageY = 0;
  FFixedCols = 1;
  FFixedRows = 1;
  FFixedColor = clBtnFace;
  FixedRowsHeight = 24;
  FixedColsWidth = 50;
  FDefaultRowHeight = 24;
  FDefaultColWidth = 50;
  FGridLineColor = clBtnFace;
  FGridLineWidth = 1;
  FRow = -1;
  FCol = -1;
  ColSize = -1;
  RowSize = -1;
  bSizing = false;
  bLButtonDown = false;
  FOptions.Clear() << goFixedVertLine << goFixedHorzLine << goVertLine << goHorzLine;
  FUpdateCount = 0;

  CalculGridWidth();
  CalculGridHeight();

  SET_EVENT(this, TOnMouseWheel, OnMouseWheel, TCustomGrid, this, GridMouseWheel);
  SET_EVENT(FHorzScrollBar, TOnScroll, OnScroll, TCustomGrid, this, HorzScrollBarScroll);
  SET_EVENT(FVertScrollBar, TOnScroll, OnScroll, TCustomGrid, this, VertScrollBarScroll);

  bProcessCreateToDo = true;

}

//---------------------------------------------------------------------------
TCustomGrid::~TCustomGrid(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TCustomGrid::GridMouseWheel(TObject *Sender, TShiftState Shift,
      int WheelDelta, const TPoint &MousePos, bool &Handled) {
  TRect Rect;


  Rect = ClientRect;
  int NewDecalageY = DecalageY - WheelDelta / 3;
  if (NewDecalageY > RowsHeight - Rect.Height) NewDecalageY = RowsHeight - Rect.Height;
  if (NewDecalageY < 0) NewDecalageY = 0;
  if (DecalageY != NewDecalageY) {
    DecalageY = NewDecalageY;
		if (FUpdateCount == 0) {
			CalculGridHeight();
			Invalidate();
		}
  }
  Handled = true;
}


//---------------------------------------------------------------------------
bool TCustomGrid::ProcessSize(int SizeType, int L, int H) {
  CalculGridWidth();
  CalculGridHeight();

  return TWinControl::ProcessSize(SizeType, L, H);
}

//---------------------------------------------------------------------------
void FASTCALL TCustomGrid::HorzScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos) {
  DecalageX = ScrollPos;
	if (FUpdateCount == 0) {
		CalculGridWidth();
		Invalidate();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TCustomGrid::VertScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos) {
  DecalageY = ScrollPos;
	if (FUpdateCount == 0) {
		CalculGridHeight();
		Invalidate();
	}
}

//---------------------------------------------------------------------------
bool TCustomGrid::ProcessMouseMove(int X, int Y) {
  int AColSize, ARowSize;
  int NewVal;


  if (bSizing) {
    if (ColSize != -1) {
      NewVal = ValInit + X - CoordInit;
      if (NewVal < 4) NewVal = 4;
      if (FColWidths[ColSize] != NewVal) {
        FColWidths[ColSize] = NewVal;
				if (FUpdateCount == 0) {
					CalculGridWidth();
					Invalidate();
				}
      }
    }
    else {
      NewVal = ValInit + Y - CoordInit;
      if (NewVal < 4) NewVal = 4;
      if (FRowHeights[RowSize] != NewVal) {
        FRowHeights[RowSize] = NewVal;
				if (FUpdateCount == 0) {
					CalculGridHeight();
					Invalidate();
				}
      }
    }
  }
  else {
    if (CanSize(X, Y, &AColSize, &ARowSize)) {
      if (AColSize != -1) FCursor = crHSplit;
      else FCursor = crVSplit;
    }
    else FCursor = crArrow;
  }

  return TWinControl::ProcessMouseMove(X, Y);
}

//---------------------------------------------------------------------------
bool TCustomGrid::ProcessLButtonDown(int X, int Y) {

  if (CanSize(X, Y, &ColSize, &RowSize)) {
    bSizing = true;
    if (ColSize != -1) {
      ValInit = FColWidths[ColSize];
      CoordInit = X;
    }
    else {
      ValInit = FRowHeights[RowSize];
      CoordInit = Y;
    }
    Mouse->Capture = FHandle;
  }
  else if (FixedColsWidth - DecalageX <= X && X < ColsWidth - DecalageX &&
           FixedRowsHeight - DecalageY <= Y && Y < RowsHeight - DecalageY) {
    bLButtonDown = true;
    Mouse->Capture = FHandle;
  }


  return TWinControl::ProcessLButtonDown(X, Y);
}

//---------------------------------------------------------------------------
bool TCustomGrid::ProcessLButtonUp(int X, int Y) {
	int NewCol, NewRow;


  if (bSizing) {
    bSizing = false;
    Mouse->Capture = NULL;
  }
  else if (bLButtonDown) {
    bLButtonDown = false;
    Mouse->Capture = NULL;
    if (FixedColsWidth - DecalageX <= X && X < ColsWidth - DecalageX &&
        FixedRowsHeight - DecalageY <= Y && Y < RowsHeight - DecalageY) {
      MouseToCell(X, Y, NewCol, NewRow);
			if (FCol != NewCol || FRow != NewRow) {
				FCol = NewCol;
				FRow = NewRow;
				if (FUpdateCount == 0) {
					Invalidate();
				}
			}
			if (!FComponentState.Contains(csLoading)) {
				if (FAction) FAction->Execute();
				OnClick(this);
			}
    }
  }

  return TWinControl::ProcessLButtonUp(X, Y);
}

//---------------------------------------------------------------------------
// Dessin de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL TCustomGrid::Paint(void) {
  int i, j;
  HDC hdc;
  TRect Rect;
  HBRUSH hBrush;


  hdc = FCanvas->Handle;

	// Fond
	GetClientRect(FHandle, &Rect);
  hBrush = CreateSolidBrush(clBtnFace);
  FillRect(hdc, &Rect, hBrush);
  DeleteObject(hBrush);

  // Cellules
  Rect.Left = -DecalageX;
  for (i = 0; i < (int) FColWidths.size(); i++) {
    Rect.Right = Rect.Left + FColWidths[i];

    Rect.Top = -DecalageY;
    for (j = 0; j < (int) FRowHeights.size(); j++) {
      Rect.Bottom = Rect.Top + FRowHeights[j];
      DrawCell(hdc, i, j, Rect);
      Rect.Top = Rect.Bottom + FGridLineWidth;
    }

    Rect.Left = Rect.Right + FGridLineWidth;
  }
}

//---------------------------------------------------------------------------
void TCustomGrid::DrawCell(HDC hdc, int ACol, int ARow, TRect Rect) {
  TRect Rect2;
  HBRUSH hBrush;
	TGridDrawStates GridDrawStates;


	// Flags de dessin
	GridDrawStates.Clear();
  if (ACol < FFixedCols || ARow < FFixedRows) {
		GridDrawStates << gdFixed;
  }
	if ((ACol == FCol || FOptions.Contains(goRowSelect)) && ARow == FRow) {
		GridDrawStates << gdSelected;
	}

  // Grille
  Rect2.Left = (int) Rect.Left;
  Rect2.Top = (int) Rect.Top;
  Rect2.Right = Rect.Right + FGridLineWidth;
  Rect2.Bottom = Rect.Bottom + FGridLineWidth;
  Canvas->Brush->Style = bsSolid;
  if (GridDrawStates.Contains(gdFixed)) {
    hBrush = CreateSolidBrush(clBlack);
  }
  else {
    hBrush = CreateSolidBrush(FGridLineColor.cr);
  }
  FillRect(hdc, &Rect2, hBrush);
  DeleteObject(hBrush);

  // Cellule
  if (GridDrawStates.Contains(gdFixed)) {
    DrawFrameControl(hdc, &Rect, DFC_BUTTON, DFCS_BUTTONPUSH);
    if (FFixedColor != clBtnFace) {
      InflateRect(&Rect, -2, -2);
      HBRUSH hBrush = CreateSolidBrush(FFixedColor.cr);
      FillRect(hdc, &Rect, hBrush);
      DeleteObject(hBrush);
    }
  }
  else {
		HBRUSH hBrush;
		if (GridDrawStates.Contains(gdSelected)) {
			hBrush = CreateSolidBrush(clHighlight);
		}
		else {
			hBrush = CreateSolidBrush(FColor.cr);
		}
    FillRect(hdc, &Rect, hBrush);
    DeleteObject(hBrush);
  }

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TBorderStyle TCustomGrid::Get_BorderStyle(void) {
  return FBorderStyle;
}

bool TCustomGrid::Set_BorderStyle(TBorderStyle NewBorderStyle) {
  DWORD NewExWindowStyle;


  if (FBorderStyle != NewBorderStyle) {
    FBorderStyle = NewBorderStyle;
    if (FHandle) {
      NewExWindowStyle = (FExWindowStyle & ~(WS_EX_CLIENTEDGE));
      if (FBorderStyle == bsSingle) {
        NewExWindowStyle |= WS_EX_CLIENTEDGE;
      }
      ChangeExWindowStyle(NewExWindowStyle);
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ColCount
//---------------------------------------------------------------------------

int TCustomGrid::Get_ColCount(void) {
  return (int) FColWidths.size();
}

bool TCustomGrid::Set_ColCount(int NewColCount) {

  if ((int) FColWidths.size() != NewColCount) {
    if ((int) FColWidths.size() < NewColCount) {
      while ((int) FColWidths.size() < NewColCount) FColWidths.push_back(FDefaultColWidth);
    }
    else {
      while ((int) FColWidths.size() > NewColCount) FColWidths.erase(FColWidths.begin() + FColWidths.size() - 1);
    }
		if (FUpdateCount == 0) {
			CalculGridWidth();
			Invalidate();
		}
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété RowCount
//---------------------------------------------------------------------------

int TCustomGrid::Get_RowCount(void) {
  return (int) FRowHeights.size();
}

bool TCustomGrid::Set_RowCount(int NewRowCount) {

  if ((int) FRowHeights.size() != NewRowCount) {
    if ((int) FRowHeights.size() < NewRowCount) {
      while ((int) FRowHeights.size() < NewRowCount) FRowHeights.push_back(FDefaultRowHeight);
    }
    else {
      while ((int) FRowHeights.size() > NewRowCount) FRowHeights.erase(FRowHeights.begin() + FRowHeights.size() - 1);
    }
		if (FUpdateCount == 0) {
			CalculGridHeight();
			Invalidate();
		}
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FixedCols
//---------------------------------------------------------------------------

int TCustomGrid::Get_FixedCols(void) {
  return FFixedCols;
}

bool TCustomGrid::Set_FixedCols(int NewFixedCols) {

  if (FFixedCols != NewFixedCols) {
    FFixedCols = NewFixedCols;
		if (FUpdateCount == 0) {
			CalculGridWidth();
			Invalidate();
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FixedRows
//---------------------------------------------------------------------------

int TCustomGrid::Get_FixedRows(void) {
  return FFixedRows;
}

bool TCustomGrid::Set_FixedRows(int NewFixedRows) {

  if (FFixedRows != NewFixedRows) {
    FFixedRows = NewFixedRows;
		if (FUpdateCount == 0) {
			CalculGridHeight();
			Invalidate();
		}
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FixedColor
//---------------------------------------------------------------------------

TColor TCustomGrid::Get_FixedColor(void) {
  return FFixedColor;
}

bool TCustomGrid::Set_FixedColor(TColor NewFixedColor) {

  if (FFixedColor != NewFixedColor) {
    FFixedColor = NewFixedColor;
		if (FUpdateCount == 0) {
			CalculGridWidth();
			Invalidate();
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DefaultColWidth
//---------------------------------------------------------------------------

int TCustomGrid::Get_DefaultColWidth(void) {
  return FDefaultColWidth;
}

bool TCustomGrid::Set_DefaultColWidth(int NewDefaultColWidth) {

  if (FDefaultColWidth != NewDefaultColWidth) {
    FDefaultColWidth = NewDefaultColWidth;
		if (FUpdateCount == 0) {
			Invalidate();
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DefaultRowHeight
//---------------------------------------------------------------------------

int TCustomGrid::Get_DefaultRowHeight(void) {
  return FDefaultRowHeight;
}

bool TCustomGrid::Set_DefaultRowHeight(int NewDefaultRowHeight) {

  if (FDefaultRowHeight != NewDefaultRowHeight) {
    FDefaultRowHeight = NewDefaultRowHeight;
		if (FUpdateCount == 0) {
			Invalidate();
		}
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété GridLineColor
//---------------------------------------------------------------------------

TColor TCustomGrid::Get_GridLineColor(void) {
  return FGridLineColor;
}

bool TCustomGrid::Set_GridLineColor(TColor NewGridLineColor) {

  if (FGridLineColor != NewGridLineColor) {
    FGridLineColor = NewGridLineColor;
		if (FUpdateCount == 0) {
			Invalidate();
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété GridLineWidth
//---------------------------------------------------------------------------

int TCustomGrid::Get_GridLineWidth(void) {
  return FGridLineWidth;
}

bool TCustomGrid::Set_GridLineWidth(int NewGridLineWidth) {

  if (FGridLineWidth != NewGridLineWidth) {
    FGridLineWidth = NewGridLineWidth;
		if (FUpdateCount == 0) {
			CalculGridWidth();
			CalculGridHeight();
			Invalidate();
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColWidths
//---------------------------------------------------------------------------

int TCustomGrid::Get_ColWidths(int Index) {

  return FColWidths[Index];
}

bool TCustomGrid::Set_ColWidths(int Index, int NewColWidths) {

  if (FColWidths[Index] != NewColWidths) {
    FColWidths[Index] = NewColWidths;
		if (FUpdateCount == 0) {
			CalculGridWidth();
			Invalidate();
		}
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété RowHeights
//---------------------------------------------------------------------------

int TCustomGrid::Get_RowHeights(int Index) {

  return FRowHeights[Index];
}

bool TCustomGrid::Set_RowHeights(int Index, int NewRowHeights) {

  if (FRowHeights[Index] != NewRowHeights) {
    FRowHeights[Index] = NewRowHeights;
		if (FUpdateCount == 0) {
			CalculGridHeight();
			Invalidate();
		}
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Row
//---------------------------------------------------------------------------

int TCustomGrid::Get_Row(void) {
  return FRow;
}

bool TCustomGrid::Set_Row(int NewRow) {

  if (FRow != NewRow) {
    FRow = NewRow;
		if (FUpdateCount == 0) {
			Invalidate();
		}
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Col
//---------------------------------------------------------------------------

int TCustomGrid::Get_Col(void) {
  return FCol;
}

bool TCustomGrid::Set_Col(int NewCol) {

  if (FCol != NewCol) {
    FCol = NewCol;
		if (FUpdateCount == 0) {
			Invalidate();
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Cells
//---------------------------------------------------------------------------

AnsiString TCustomGrid::Get_Cells(int Col, int Row) {
  return _T("");
}

bool TCustomGrid::Set_Cells(int Col, int Row, AnsiString NewCells) {
  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété GridWidth
//---------------------------------------------------------------------------

int TCustomGrid::Get_GridWidth(void) {
  return FGridWidth;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété GridHeight
//---------------------------------------------------------------------------

int TCustomGrid::Get_GridHeight(void) {
  return FGridHeight;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Options
//---------------------------------------------------------------------------

TGridOptions TCustomGrid::Get_Options(void) {
  return FOptions;
}

bool TCustomGrid::Set_Options(TGridOptions NewOptions) {
  if (FOptions != NewOptions) {
    FOptions = NewOptions;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété UpdateCount
//---------------------------------------------------------------------------

int TCustomGrid::Get_UpdateCount(void) {
  return FUpdateCount;
}


//---------------------------------------------------------------------------
// Renvoie les coordonnées écran d'une cellule de la grille.
//---------------------------------------------------------------------------

TRect FASTCALL TCustomGrid::CellRect(int ACol, int ARow) {
  int i, j;
  TRect Rect;


  Rect.Right = -DecalageX;
  for (i = 0; i <= ACol; i++) {
    if (i == (int) FColWidths.size()) {
      Rect.Left = 0;
      Rect.Top = 0;
      Rect.Right = 0;
      Rect.Bottom = 0;
    }
    Rect.Left = Rect.Right;
    Rect.Right = Rect.Left + FColWidths[i] + FGridLineWidth;
  }

  Rect.Bottom = -DecalageY;
  for (j = 0; j <= ARow; j++) {
    if (j == (int) FRowHeights.size()) {
      Rect.Left = 0;
      Rect.Top = 0;
      Rect.Right = 0;
      Rect.Bottom = 0;
    }
    Rect.Top = Rect.Bottom;
    Rect.Bottom = Rect.Top + FRowHeights[j] + FGridLineWidth;
  }

  return Rect;
}

//---------------------------------------------------------------------------
// Indices de colonne et de ligne aux coordonnées écran (X,Y).
//---------------------------------------------------------------------------

void FASTCALL TCustomGrid::MouseToCell(int X, int Y, int &ACol, int &ARow) {
  int i, j;
  TRect Rect;

  ACol = -1;
  Rect.Right = -DecalageX;
  for (i = 0; i < (int) FColWidths.size(); i++) {
    Rect.Left = Rect.Right;
    Rect.Right = Rect.Left + FColWidths[i] + FGridLineWidth;
    if (Rect.Left <= X && X < Rect.Right) {
      ACol = i;
      break;
    }
  }

  ARow = -1;
  Rect.Bottom = -DecalageY;
  for (j = 0; j < (int) FRowHeights.size(); j++) {
    Rect.Top = Rect.Bottom;
    Rect.Bottom = Rect.Top + FRowHeights[j] + FGridLineWidth;
    if (Rect.Top <= Y && Y < Rect.Bottom) {
      ARow = j;
      break;
    }
  }

}


//---------------------------------------------------------------------------

void TCustomGrid::CalculGridWidth(void) {
  int i;
  TRect Rect;
  int ColWidth;


  Rect = ClientRect;
  FixedColsWidth = 0;
  ColsWidth = FGridLineWidth;
  for (i = 0; i < (int) FColWidths.size(); i++) {
    ColWidth = (int) FColWidths[i] + FGridLineWidth;
    if (i < FFixedCols) FixedColsWidth += ColWidth;
    ColsWidth += ColWidth;
  }

  // Ajustement du décalage pour éviter une zone vide
  if (DecalageX < 0 ) {
    DecalageX = 0;
    Invalidate();
  }
  else if (DecalageX > 0 && DecalageX > ColsWidth - Rect.Width) {
    DecalageX = ColsWidth - Rect.Width;
    if (DecalageX < 0) DecalageX = 0;
    Invalidate();
  }

  // Scroll-bar horizontale
  FHorzScrollBar->Max = ColsWidth;
  FHorzScrollBar->PageSize = Rect.Width + 1;
  FHorzScrollBar->Position = DecalageX;
  FHorzScrollBar->Increment = 10;

  // Ajustement à la zone client
  FGridWidth = ColsWidth;
  if (FGridWidth > Rect.Width) FGridWidth = Rect.Width;

}


//---------------------------------------------------------------------------

void TCustomGrid::CalculGridHeight(void) {
  int j;
  TRect Rect;
  int RowHeight;


  Rect = ClientRect;
  FixedRowsHeight = 0;
  RowsHeight = FGridLineWidth;
  for (j = 0; j < (int) FRowHeights.size(); j++) {
    RowHeight = (int) FRowHeights[j] + FGridLineWidth;
    if (j < FFixedRows) FixedRowsHeight += RowHeight;
    RowsHeight += RowHeight;
  }

  // Ajustement du décalage pour éviter une zone vide
  if (DecalageY < 0 ) {
    DecalageY = 0;
    Invalidate();
  }
  else if (DecalageY > 0 && DecalageY > RowsHeight - Rect.Height) {
    DecalageY = RowsHeight - Rect.Height;
    if (DecalageY < 0) DecalageY = 0;
    Invalidate();
  }

  // Scroll-bar verticale
  FVertScrollBar->Max = RowsHeight;
  FVertScrollBar->PageSize = Rect.Height + 1;
  FVertScrollBar->Position = DecalageY;
  FVertScrollBar->Increment = 10;

  // Ajustement à la zone client
  FGridHeight = RowsHeight;
  if (FGridHeight > Rect.Height) FGridHeight = Rect.Height;

}


//---------------------------------------------------------------------------
bool TCustomGrid::CanSize(int X, int Y, int * AColSize, int * ARowSize) {
  int i, j;
  int XSep, YSep;



  *AColSize = -1;
  *ARowSize = -1;

  if (Y < FixedRowsHeight - DecalageY && FOptions.Contains(goColSizing)) {
    XSep = -DecalageX;
    for (i = 0; i < (int) FColWidths.size(); i++) {
      XSep += FColWidths[i] + FGridLineWidth;
      if (XSep - FGridLineWidth - 2 <= X && X <= XSep + 2) {
        *AColSize = i;
        return true;
      }
      if (XSep - 2 > X) break;
    }
  }

  if (X < FixedColsWidth - DecalageX && FOptions.Contains(goRowSizing)) {
    YSep = -DecalageY;
    for (j = 0; j < (int) FRowHeights.size(); j++) {
      YSep += FRowHeights[j] + FGridLineWidth;
      if (YSep - FGridLineWidth - 2 <= Y && Y <= YSep + 2) {
        *ARowSize = j;
        return true;
      }
      if (YSep - 2 > Y) break;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TCustomGrid::BeginUpdate(void) {
  FUpdateCount++;
}

//---------------------------------------------------------------------------
void FASTCALL TCustomGrid::EndUpdate(void) {
  FVCL_ASSERT(FUpdateCount > 0 && "EndUpdate sans BeginUpdate");
  if (FUpdateCount > 0) FUpdateCount--;
  if (FUpdateCount == 0) {
		CalculGridWidth();
		CalculGridHeight();
		Invalidate();
	}
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomGrid::GetListProperties(TStrings *ListProperties) {

  TCustomControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("BorderStyle"));
  ListProperties->Add(_T("ColCount"));
  ListProperties->Add(_T("RowCount"));
  ListProperties->Add(_T("FixedRows"));
  ListProperties->Add(_T("FixedCols"));
  ListProperties->Add(_T("FixedColor"));
  ListProperties->Add(_T("DefaultColWidth"));
  ListProperties->Add(_T("DefaultRowHeight"));
  ListProperties->Add(_T("GridLineColor"));
  ListProperties->Add(_T("GridLineWidth"));
  ListProperties->Add(_T("Row"));
  ListProperties->Add(_T("Col"));
  ListProperties->Add(_T("Options"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomGrid::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("BorderStyle")) {
    *asInfos = _T("bsNone\nbsSingle");
    return tpChoice;
  }
  if (asProperty == _T("ColCount")) {
    return tpNumber;
  }
  if (asProperty == _T("RowCount")) {
    return tpNumber;
  }
  if (asProperty == _T("FixedRows")) {
    return tpNumber;
  }
  if (asProperty == _T("FixedCols")) {
    return tpNumber;
  }
  if (asProperty == _T("FixedColor")) {
    return tpColor;
  }
  if (asProperty == _T("DefaultColWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("DefaultRowHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("GridLineColor")) {
    return tpColor;
  }
  if (asProperty == _T("GridLineWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("Row")) {
    return tpNumber;
  }
  if (asProperty == _T("Col")) {
    return tpNumber;
  }
  if (asProperty == _T("Options")) {
    *asInfos = _T("goFixedVertLine\ngoFixedHorzLine\ngoVertLine\ngoHorzLine\ngoRangeSelect\ngoDrawFocusSelected\ngoRowSizing\ngoColSizing\ngoRowMoving\ngoColMoving\ngoEditing\ngoTabs\ngoRowSelect\ngoAlwaysShowEditor\ngoThumbTracking");
    return tpMultipleChoice;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TCustomControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomGrid::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("BorderStyle")) {
    return _T("bsSingle");
  }
  if (asProperty == _T("ColCount")) {
    return _T("0");
  }
  if (asProperty == _T("RowCount")) {
    return _T("0");
  }
  if (asProperty == _T("FixedRows")) {
    return _T("1");
  }
  if (asProperty == _T("FixedCols")) {
    return _T("1");
  }
  if (asProperty == _T("FixedColor")) {
    return _T("clBtnFace");
  }
  if (asProperty == _T("DefaultColWidth")) {
    return _T("50");
  }
  if (asProperty == _T("DefaultRowHeight")) {
    return _T("24");
  }
  if (asProperty == _T("GridLineColor")) {
    return _T("clBtnFace");
  }
  if (asProperty == _T("GridLineWidth")) {
    return _T("1");
  }
  if (asProperty == _T("Row")) {
    return _T("-1");
  }
  if (asProperty == _T("Col")) {
    return _T("-1");
  }
  if (asProperty == _T("Options")) {
    return _T("[goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TCustomControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomGrid::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("BorderStyle")) {
    TFormBorderStyle BorderStyle = Get_BorderStyle();
    if (BorderStyle == bsNone) return _T("bsNone");
    if (BorderStyle == bsSingle) return _T("bsSingle");
    return _T("bsNone");
  }
  if (asProperty == _T("ColCount")) {
    return IntToStr(Get_ColCount());
  }
  if (asProperty == _T("RowCount")) {
    return IntToStr(Get_RowCount());
  }
  if (asProperty == _T("FixedRows")) {
    return IntToStr(Get_FixedRows());
  }
  if (asProperty == _T("FixedCols")) {
    return IntToStr(Get_FixedCols());
  }
  if (asProperty == _T("FixedColor")) {
    return ColorToString(Get_FixedColor());
  }
  if (asProperty == _T("DefaultColWidth")) {
    return IntToStr(Get_DefaultColWidth());
  }
  if (asProperty == _T("DefaultRowHeight")) {
    return IntToStr(Get_DefaultRowHeight());
  }
  if (asProperty == _T("GridLineColor")) {
    return ColorToString(Get_GridLineColor());
  }
  if (asProperty == _T("GridLineWidth")) {
    return IntToStr(Get_GridLineWidth());
  }
  if (asProperty == _T("Row")) {
    return IntToStr(Get_Row());
  }
  if (asProperty == _T("Col")) {
    return IntToStr(Get_Col());
  }
  if (asProperty == _T("Options")) {
    AnsiString asOptions;
    TGridOptions Options = Get_Options();
    if (Options.Contains(goFixedVertLine)) asOptions += _T("goFixedVertLine\n");
    if (Options.Contains(goFixedHorzLine)) asOptions += _T("goFixedHorzLine\n");
    if (Options.Contains(goVertLine)) asOptions += _T("goVertLine\n");
    if (Options.Contains(goHorzLine)) asOptions += _T("goHorzLine\n");
    if (Options.Contains(goRangeSelect)) asOptions += _T("goRangeSelect\n");
    if (Options.Contains(goDrawFocusSelected)) asOptions += _T("goDrawFocusSelected\n");
    if (Options.Contains(goRowSizing)) asOptions += _T("goRowSizing\n");
    if (Options.Contains(goColSizing)) asOptions += _T("goColSizing\n");
    if (Options.Contains(goRowMoving)) asOptions += _T("goRowMoving\n");
    if (Options.Contains(goColMoving)) asOptions += _T("goColMoving\n");
    if (Options.Contains(goEditing)) asOptions += _T("goEditing\n");
    if (Options.Contains(goTabs)) asOptions += _T("goTabs\n");
    if (Options.Contains(goRowSelect)) asOptions += _T("goRowSelect\n");
    if (Options.Contains(goAlwaysShowEditor)) asOptions += _T("goAlwaysShowEditor\n");
    if (Options.Contains(goThumbTracking)) asOptions += _T("goThumbTracking\n");
    return FormatProperty(asOptions, _T("["), _T("]"), _T(", "));
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TCustomControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomGrid::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("BorderStyle")) {
    if (asValue == _T("bsNone")) Set_BorderStyle(bsNone);
    else if (asValue == _T("bsSingle")) Set_BorderStyle(bsSingle);
    return true;
  }
  if (asProperty == _T("RowCount")) {
    Set_RowCount(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("ColCount")) {
    Set_ColCount(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("FixedRows")) {
    Set_FixedRows(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("FixedCols")) {
    Set_FixedCols(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("FixedColor")) {
    TColor Color;
    Color = asValue;
    Set_FixedColor(Color);
    return true;
  }
  if (asProperty == _T("DefaultColWidth")) {
    Set_DefaultColWidth(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("DefaultRowHeight")) {
    Set_DefaultRowHeight(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("GridLineColor")) {
    TColor Color;
    Color = asValue;
    Set_GridLineColor(Color);
    return true;
  }
  if (asProperty == _T("GridLineWidth")) {
    Set_GridLineWidth(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Row")) {
    Set_Row(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Col")) {
    Set_Col(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Options")) {
    int Cur;
    AnsiString asMot;

    FOptions.Clear();
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("goFixedVertLine")) FOptions << goFixedVertLine;
      else if (asMot == _T("goFixedHorzLine")) FOptions << goFixedHorzLine;
      else if (asMot == _T("goVertLine")) FOptions << goVertLine;
      else if (asMot == _T("goHorzLine")) FOptions << goHorzLine;
      else if (asMot == _T("goRangeSelect")) FOptions << goRangeSelect;
      else if (asMot == _T("goDrawFocusSelected")) FOptions << goDrawFocusSelected;
      else if (asMot == _T("goRowSizing")) FOptions << goRowSizing;
      else if (asMot == _T("goColSizing")) FOptions << goColSizing;
      else if (asMot == _T("goRowMoving")) FOptions << goRowMoving;
      else if (asMot == _T("goColMoving")) FOptions << goColMoving;
      else if (asMot == _T("goEditing")) FOptions << goEditing;
      else if (asMot == _T("goTabs")) FOptions << goTabs;
      else if (asMot == _T("goRowSelect")) FOptions << goRowSelect;
      else if (asMot == _T("goAlwaysShowEditor")) FOptions << goAlwaysShowEditor;
      else if (asMot == _T("goThumbTracking")) FOptions << goThumbTracking;
    } while (!asMot.IsEmpty());

    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  return TCustomControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

