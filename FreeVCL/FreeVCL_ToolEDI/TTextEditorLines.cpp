//-----------------------------------------------------------------------------
//! @file TTextEditorLines.cpp
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
// Includes
//---------------------------------------------------------------------------

#include <FreeVcl.h>
#include <FreeVcl_ToolsEDI.h>


//---------------------------------------------------------------------------
// TTextEditorLines
//---------------------------------------------------------------------------

TTextEditorLines::TTextEditorLines(void): TStrings() {
}

TTextEditorLines::~TTextEditorLines(void) {
}

//---------------------------------------------------------------------------
// Début ou fin d'une série de modifications
//---------------------------------------------------------------------------

void FASTCALL TTextEditorLines::SetUpdateState(bool Updating) {
  if (!Updating) OnChange(this);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int FASTCALL TTextEditorLines::GetCount(void) const {
  return (int) FItems.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TTextEditorLines::Get_Strings(int i) {
  FVCL_ASSERT(0 <= i && i < (int) FItems.size() && "TTextEditorLines::operator[]: Index out of string");
  return FItems[i].asLine;
}

bool TTextEditorLines::Set_Strings(int i, AnsiString NewStrings) {
  FVCL_ASSERT(0 <= i && i < (int) FItems.size() && "TTextEditorLines::operator[]: Index out of string");
  FItems[i].asLine = NewStrings;
  if (FUpdateCount == 0) OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagCommentBegin
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagCommentBegin(int i) {
  return FItems[i].FFlagCommentBegin;
}

bool TTextEditorLines::Set_FlagCommentBegin(int i, bool NewFlagCommentBegin) {
  FItems[i].FFlagCommentBegin = NewFlagCommentBegin;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagCommentEnd
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagCommentEnd(int i) {
  return FItems[i].FFlagCommentEnd;
}

bool TTextEditorLines::Set_FlagCommentEnd(int i, bool NewFlagCommentEnd) {
  FItems[i].FFlagCommentEnd = NewFlagCommentEnd;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagCommentBegin2
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagCommentBegin2(int i) {
  return FItems[i].FFlagCommentBegin2;
}

bool TTextEditorLines::Set_FlagCommentBegin2(int i, bool NewFlagCommentBegin2) {
  FItems[i].FFlagCommentBegin2 = NewFlagCommentBegin2;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagCommentEnd2
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagCommentEnd2(int i) {
  return FItems[i].FFlagCommentEnd2;
}

bool TTextEditorLines::Set_FlagCommentEnd2(int i, bool NewFlagCommentEnd2) {
  FItems[i].FFlagCommentEnd2 = NewFlagCommentEnd2;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagQuote1Begin
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagQuote1Begin(int i) {
  return FItems[i].FFlagQuote1Begin;
}

bool TTextEditorLines::Set_FlagQuote1Begin(int i, bool NewFlagQuote1Begin) {
  FItems[i].FFlagQuote1Begin = NewFlagQuote1Begin;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagQuote1End
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagQuote1End(int i) {
  return FItems[i].FFlagQuote1End;
}

bool TTextEditorLines::Set_FlagQuote1End(int i, bool NewFlagQuote1End) {
  FItems[i].FFlagQuote1End = NewFlagQuote1End;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagQuote2Begin
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagQuote2Begin(int i) {
  return FItems[i].FFlagQuote2Begin;
}

bool TTextEditorLines::Set_FlagQuote2Begin(int i, bool NewFlagQuote2Begin) {
  FItems[i].FFlagQuote2Begin = NewFlagQuote2Begin;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FlagQuote2End
//---------------------------------------------------------------------------

bool TTextEditorLines::Get_FlagQuote2End(int i) {
  return FItems[i].FFlagQuote2End;
}

bool TTextEditorLines::Set_FlagQuote2End(int i, bool NewFlagQuote2End) {
  FItems[i].FFlagQuote2End = NewFlagQuote2End;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Flags
//---------------------------------------------------------------------------

TEditorLinesFlags TTextEditorLines::Get_Flags(int i) {
  return FItems[i].FFlags;
}

bool TTextEditorLines::Set_Flags(int i, TEditorLinesFlags NewFlags) {
  FItems[i].FFlags = NewFlags;

  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditorLines::Clear(void) {
  FItems.clear();
  if (FUpdateCount == 0) OnChange(this);
}

//---------------------------------------------------------------------------
int FASTCALL TTextEditorLines::Add(const AnsiString S) {
  TEXTEDITORLINE TextEditorLine;

  TextEditorLine.asLine = S;
  unsigned int Index = (unsigned int) FItems.size();
  if (Index == 0) {
    TextEditorLine.FFlagCommentBegin = false;
    TextEditorLine.FFlagCommentEnd = false;
    TextEditorLine.FFlagCommentBegin2 = false;
    TextEditorLine.FFlagCommentEnd2 = false;
    TextEditorLine.FFlagQuote1Begin = false;
    TextEditorLine.FFlagQuote1End = false;
    TextEditorLine.FFlagQuote2Begin = false;
    TextEditorLine.FFlagQuote2End = false;
  }
  else {
    TextEditorLine.FFlagCommentBegin = FItems[Index - 1].FFlagCommentEnd;
    TextEditorLine.FFlagCommentEnd = TextEditorLine.FFlagCommentBegin;
    TextEditorLine.FFlagCommentBegin2 = FItems[Index - 1].FFlagCommentEnd2;
    TextEditorLine.FFlagCommentEnd2 = TextEditorLine.FFlagCommentBegin2;
    TextEditorLine.FFlagQuote1Begin = FItems[Index - 1].FFlagQuote1End;
    TextEditorLine.FFlagQuote1End = TextEditorLine.FFlagQuote1Begin;
    TextEditorLine.FFlagQuote2Begin = FItems[Index - 1].FFlagQuote2End;
    TextEditorLine.FFlagQuote2End = TextEditorLine.FFlagQuote2Begin;
  }
  TextEditorLine.FFlagCommentEnd = false;
  TextEditorLine.FFlags.Clear();
  FItems.push_back(TextEditorLine);
  if (FUpdateCount == 0) OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditorLines::Insert(int Index, const AnsiString S) {
  TEXTEDITORLINE TextEditorLine;

  TextEditorLine.asLine = S;
  if (Index == 0) {
    TextEditorLine.FFlagCommentBegin = false;
    TextEditorLine.FFlagCommentEnd = false;
    TextEditorLine.FFlagCommentBegin2 = false;
    TextEditorLine.FFlagCommentEnd2 = false;
    TextEditorLine.FFlagQuote1Begin = false;
    TextEditorLine.FFlagQuote1End = false;
    TextEditorLine.FFlagQuote2Begin = false;
    TextEditorLine.FFlagQuote2End = false;
  }
  else {
		TextEditorLine.FFlagCommentBegin = FItems[Index - 1].FFlagCommentEnd;
    TextEditorLine.FFlagCommentEnd = TextEditorLine.FFlagCommentBegin;
		TextEditorLine.FFlagCommentBegin2 = FItems[Index - 1].FFlagCommentEnd2;
    TextEditorLine.FFlagCommentEnd2 = TextEditorLine.FFlagCommentBegin2;
    TextEditorLine.FFlagQuote1Begin = FItems[Index - 1].FFlagQuote1End;
    TextEditorLine.FFlagQuote1End = TextEditorLine.FFlagQuote1Begin;
    TextEditorLine.FFlagQuote2Begin = FItems[Index - 1].FFlagQuote2End;
    TextEditorLine.FFlagQuote2End = TextEditorLine.FFlagQuote2Begin;
  }
  TextEditorLine.FFlagCommentEnd = false;
  TextEditorLine.FFlags.Clear();
  FItems.insert(FItems.begin() + Index, TextEditorLine);
  if (FUpdateCount == 0) OnChange(this);
}

//---------------------------------------------------------------------------
void FASTCALL TTextEditorLines::Delete(int Index) {
  FItems.erase(FItems.begin() + Index);
  if (FUpdateCount == 0) OnChange(this);
}

//---------------------------------------------------------------------------
