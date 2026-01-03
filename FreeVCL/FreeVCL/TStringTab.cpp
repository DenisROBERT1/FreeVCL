//-----------------------------------------------------------------------------
//! @file TStringTab.cpp
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
// TStringTab
//---------------------------------------------------------------------------

TStringTab::TStringTab(void): TPersistent() {
  // Initialisations
  Updating = false;
  FColCount = 0;
}

TStringTab::~TStringTab(void) {
	int Index;

	for (Index = 0; Index < (int) FItems.size(); Index++) {
		delete FItems[Index];
	}

	FItems.clear();
}

//---------------------------------------------------------------------------
// Début ou fin d'une série de modifications
//---------------------------------------------------------------------------

void FASTCALL TStringTab::SetUpdateState(bool Updating) {
  if (!Updating) OnChange(this);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColCount
//---------------------------------------------------------------------------

int TStringTab::Get_ColCount(void) {
  return FColCount;
}

bool TStringTab::Set_ColCount(int NewColCount) {

  if (FColCount != NewColCount) {
    if (FColCount < NewColCount) {
      while (FColCount < NewColCount) AddCol();
    }
    else {
      while (FColCount > NewColCount) DeleteCol(FColCount - 1);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété RowCount
//---------------------------------------------------------------------------

int TStringTab::Get_RowCount(void) {
  return (int) FItems.size();
}

bool TStringTab::Set_RowCount(int NewRowCount) {

  if ((int) FItems.size() != NewRowCount) {
    if ((int) FItems.size() < NewRowCount) {
      while ((int) FItems.size() < NewRowCount) AddRow();
    }
    else {
      while ((int) FItems.size() > NewRowCount) DeleteRow((int) FItems.size() - 1);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TStringTab::Get_Strings(int ACol, int ARow) {
  FVCL_ASSERT(0 <= ARow && ARow < (int) FItems.size() && "TStringTab::operator[]: Index out of string");
  return FItems[ARow]->operator[](ACol);
}

//---------------------------------------------------------------------------
bool TStringTab::Set_Strings(int ACol, int ARow, AnsiString NewStrings) {
  FVCL_ASSERT(0 <= ARow && ARow < (int) FItems.size() && "TStringTab::operator[]: Index out of string");
  FItems[ARow]->Set_Strings(ACol, NewStrings);
  if (!Updating) OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TStringTab::Clear(void) {
	int j;

	for (j = 0; j < (int) FItems.size(); j++) delete FItems[j];
  FItems.clear();
  if (!Updating) OnChange(this);
}

//---------------------------------------------------------------------------
int TStringTab::AddCol(void) {
	int j;

	for (j = 0; j < (int) FItems.size(); j++) {
		FItems[j]->Add(_T(""));
	}
  FColCount++;
  if (!Updating) OnChange(this);

  return true;
}
//---------------------------------------------------------------------------
int TStringTab::AddRow(void) {
	TStringList * NewRow = new TStringList();
  for (int i = 0; i < FColCount; i++) NewRow->Add(_T(""));
  FItems.push_back(NewRow);
  if (!Updating) OnChange(this);

  return true;
}
//---------------------------------------------------------------------------
void TStringTab::InsertCol(int Index) {
	int j;

	for (j = 0; j < (int) FItems.size(); j++) {
		FItems[j]->Insert(Index, _T(""));
	}
  FColCount++;
  if (!Updating) OnChange(this);
}
//---------------------------------------------------------------------------
void TStringTab::InsertRow(int Index) {
	TStringList * NewRow = new TStringList();
  for (int i = 0; i < FColCount; i++) NewRow->Add(_T(""));
  FItems.insert(FItems.begin() + Index, NewRow);
  if (!Updating) OnChange(this);
}
//---------------------------------------------------------------------------
void TStringTab::DeleteCol(int Index) {
	int j;

	for (j = 0; j < (int) FItems.size(); j++) {
		FItems[j]->Delete(Index);
	}
  FColCount--;
  if (!Updating) OnChange(this);
}

//---------------------------------------------------------------------------
void TStringTab::DeleteRow(int Index) {
	delete FItems[Index];
  FItems.erase(FItems.begin() + Index);
  if (!Updating) OnChange(this);
}

//---------------------------------------------------------------------------

