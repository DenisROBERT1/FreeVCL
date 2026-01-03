//-----------------------------------------------------------------------------
//! @file TDrawGrid.cpp
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
// TDrawGrid
//---------------------------------------------------------------------------

TDrawGrid::TDrawGrid(TComponent* AOwner): TCustomGrid(AOwner) {

  // Initialisations
  FClassName = _T("TDrawGrid");
  FCells = new TStringTab();
  bProcessCreateToDo = true;

}

//---------------------------------------------------------------------------
TDrawGrid::~TDrawGrid(void) {
  delete FCells;
}

//---------------------------------------------------------------------------
void TDrawGrid::DrawCell(HDC hdc, int ACol, int ARow, TRect Rect) {

  TCustomGrid::DrawCell(hdc, ACol, ARow, Rect);

  InflateRect(&Rect, -4, 0);
  AnsiString asTextCell = Get_Cells(ACol, ARow);

  SelectObject(hdc, FFont->Handle);
  SetBkMode(hdc, TRANSPARENT);
  DrawText(hdc, asTextCell, asTextCell.Length(), &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Cells
//---------------------------------------------------------------------------

AnsiString TDrawGrid::Get_Cells(int Col, int Row) {
  return FCells->Get_Strings(Col, Row);
}

bool TDrawGrid::Set_Cells(int Col, int Row, AnsiString NewCells) {
  FCells->Set_Strings(Col, Row, NewCells);
  Invalidate();

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColCount
//---------------------------------------------------------------------------

bool TDrawGrid::Set_ColCount(int NewColCount) {

  FCells->ColCount = NewColCount;

  return TCustomGrid::Set_ColCount(NewColCount);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété RowCount
//---------------------------------------------------------------------------

bool TDrawGrid::Set_RowCount(int NewRowCount) {

  FCells->RowCount = NewRowCount;

  return TCustomGrid::Set_RowCount(NewRowCount);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDrawGrid::GetListProperties(TStrings *ListProperties) {

  TCustomGrid::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDrawGrid::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TCustomGrid::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDrawGrid::GetDefaultProperty(AnsiString asProperty) {
  return TCustomGrid::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDrawGrid::GetProperty(AnsiString asProperty) {
  return TCustomGrid::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDrawGrid::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TCustomGrid::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

