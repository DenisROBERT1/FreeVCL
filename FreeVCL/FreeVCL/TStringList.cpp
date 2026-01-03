//-----------------------------------------------------------------------------
//! @file TStringList.cpp
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
// TStringList
//---------------------------------------------------------------------------

TStringList::TStringList(void): TStrings() {
	FDuplicates = dupAccept;
	FSorted = false;
}

TStringList::~TStringList(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Capacity
//---------------------------------------------------------------------------

int TStringList::Get_Capacity(void) {
  return GetCount();
}

bool TStringList::Set_Capacity(int NewCapacity) {
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Duplicates
//---------------------------------------------------------------------------

TDuplicates TStringList::Get_Duplicates(void) {
  return FDuplicates;
}

bool TStringList::Set_Duplicates(TDuplicates NewDuplicates) {
  if (FDuplicates != NewDuplicates) {
    FDuplicates = NewDuplicates;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Sorted
//---------------------------------------------------------------------------

bool TStringList::Get_Sorted(void) {
  return FSorted;
}

bool TStringList::Set_Sorted(bool NewSorted) {
  if (FSorted != NewSorted) {
    FSorted = NewSorted;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TStringList::Get_Strings(int i) {
  FVCL_ASSERT(0 <= i && i < (int) FItems.size() && "TStringList::operator[]: Index out of string");
  return FItems[i];
}

bool TStringList::Set_Strings(int i, AnsiString NewStrings) {
  FVCL_ASSERT(0 <= i && i < (int) FItems.size() && "TStringList::operator[]: Index out of string");
  FItems[i] = NewStrings;
  if (FUpdateCount == 0) OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int FASTCALL TStringList::GetCount(void) const {
  return (int) FItems.size();
}

//---------------------------------------------------------------------------
// Début ou fin d'une série de modifications
//---------------------------------------------------------------------------

void FASTCALL TStringList::SetUpdateState(bool Updating) {
  if (!Updating) OnChange(this);
}

//---------------------------------------------------------------------------
void FASTCALL TStringList::Clear(void) {
  FItems.clear();
  if (FUpdateCount == 0) OnChange(this);
}

//---------------------------------------------------------------------------
int FASTCALL TStringList::Add(const AnsiString S) {
	if (FSorted || FDuplicates != dupAccept) {
		int Index;
		bool bFound;

		bFound = Find(S, Index);
		if (bFound && FDuplicates != dupAccept) return Index;
		if (FSorted) {
		  FItems.insert(FItems.begin() + Index, S);
		  if (FUpdateCount == 0) OnChange(this);
			return Index;
		}
	}
  FItems.push_back(S);
  if (FUpdateCount == 0) OnChange(this);

  return (int) FItems.size() - 1;
}

//---------------------------------------------------------------------------
void FASTCALL TStringList::Insert(int Index, const AnsiString S) {
	if (FSorted || FDuplicates != dupAccept) {
		int Index;
		bool bFound;

		bFound = Find(S, Index);
		if (bFound && FDuplicates != dupAccept) return;
		if (FSorted) {
		  FItems.insert(FItems.begin() + Index, S);
		  if (FUpdateCount == 0) OnChange(this);
			return;
		}
	}
  FItems.insert(FItems.begin() + Index, S);
  if (FUpdateCount == 0) OnChange(this);
}

//---------------------------------------------------------------------------
void FASTCALL TStringList::Delete(int Index) {
  FItems.erase(FItems.begin() + Index);
  if (FUpdateCount == 0) OnChange(this);
}

//---------------------------------------------------------------------------
bool FASTCALL TStringList::Find(const AnsiString S, int &Index) {
	if (!FSorted) {
		Index = IndexOf(S);
		return (Index != -1);
	}
	else {
		int Inf, Sup, i;

		Inf = 0;
		Sup = (int) GetCount();
		while (Inf < Sup) {
			i = (Inf + Sup) / 2;
			if (Get_Strings(i) == S) {
				Index = i;
				return true;
			}
			else if (Get_Strings(i) < S) Inf = i + 1;
			else Sup = i;
		}

		Index = Sup;

		return false;
	}
}

//---------------------------------------------------------------------------

