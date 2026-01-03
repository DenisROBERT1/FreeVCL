//-----------------------------------------------------------------------------
//! @file TDictionnary.cpp
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
#include "TDictionnary.h"

//---------------------------------------------------------------------------
// TDictionnary
//---------------------------------------------------------------------------

TDictionnary::TDictionnary(void) {
	FirstLetter = '\0';
	FCount = 0;
}

TDictionnary::~TDictionnary(void) {
	Clear();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TDictionnary::Get_Strings(int i) {
  return "";  // $$$
}

bool TDictionnary::Set_Strings(int i, AnsiString NewStrings) {
  return true;  // $$$
}

//---------------------------------------------------------------------------
int FASTCALL TDictionnary::GetCount(void) const {
	return FCount;
}

void FASTCALL TDictionnary::Clear(void) {
	int i;
	for (i = (int) Childs.size() - 1; i >= 0; i--) {
		delete Childs[i];
	}
	Childs.clear();
	FCount = 0;
}

int FASTCALL TDictionnary::Add(const AnsiString S) {
	Add((LPCTSTR) S);
	return FCount;
}

void FASTCALL TDictionnary::Insert(int Index, const AnsiString S) {
	Add((LPCTSTR) S);
}

void FASTCALL TDictionnary::Delete(int Index) {
	// $$$
}

bool FASTCALL TDictionnary::IsInDictionnary(const TCHAR *szWord) {
	TCHAR Letter = szWord[0];
	int i;
	if (!Seek(Letter, FCaseSensitive, &i)) return false;
	if (Letter == '\0') return true;
	return Childs[i]->IsInDictionnary(&szWord[1]);
}

int FASTCALL TDictionnary::IndexOf(const AnsiString S) {
	if (IsInDictionnary((LPCTSTR) S)) return 0;

	return -1;
}


bool FASTCALL TDictionnary::LoadFromFile(AnsiString asFileName) {
	char c;
	AnsiString asWord;
	DWORD dw;

	HANDLE hFile = CreateFile(asFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		while (true) {
			ReadFile(hFile, &c, 1, &dw, NULL);
			if (dw == 0) break;
			if (c == '\n') {
				Add((LPCTSTR) asWord);
				asWord = _T("");
			}
			else if (c != '\r') {
				asWord += c;
			}
		}

		CloseHandle(hFile);

		return true;
	}
  else {
    // Erreur ouverture asFileName
		FLastError = GetLastError();
		return false;
  }

}

bool FASTCALL TDictionnary::SaveToFile(AnsiString asFileName) {

	HANDLE hFile = CreateFile(asFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {

		WriteToFile(hFile, _T(""));

		CloseHandle(hFile);

		return true;
	}
  else {
    // Erreur ouverture asFileName
		FLastError = GetLastError();
		return false;
  }

}

bool TDictionnary::Add(const TCHAR *szWord) {
	TCHAR Letter = szWord[0];
	int i;
	if (!Seek(Letter, FCaseSensitive, &i)) {
		if (!FCaseSensitive) {
			CharLowerBuff(&Letter, 1);
		}
		TDictionnary * NewTDicNode = new TDictionnary();
		NewTDicNode->CaseSensitive = FCaseSensitive;
		NewTDicNode->FirstLetter = Letter;
		Childs.insert(Childs.begin() + i, NewTDicNode);
	}
	if (Letter == '\0') return true;

	if (Childs[i]->Add(&szWord[1])) {
		FCount++;
		return true;
	}

	return false;
}

bool TDictionnary::Seek(TCHAR Letter, bool bCaseSensitive, int *Index) {
	int Inf, Sup, i;
	TCHAR ChildFirstLetter;

	Inf = 0;
	Sup = (int) Childs.size();
	while (Inf < Sup) {
		i = (Inf + Sup) / 2;
		ChildFirstLetter = Childs[i]->FirstLetter;
		if (!FCaseSensitive) {
			CharLowerBuff(&ChildFirstLetter, 1);
			CharLowerBuff(&Letter, 1);
		}
		if (ChildFirstLetter == Letter) {
			*Index = i;
			return true;
		}
		else if (ChildFirstLetter < Letter) Inf = i + 1;
		else Sup = i;
	}

	*Index = Sup;

	return false;
}

bool TDictionnary::WriteToFile(HANDLE hFile, CharString csWord) {
	DWORD dw;
	int i;

	for (i = 0; i < (int) Childs.size(); i++) {
		char Letter = (char) Childs[i]->FirstLetter;
		if (Letter == '\0') {
			WriteFile(hFile, (LPCSTR) csWord, (DWORD) csWord.length(), &dw, NULL);
			WriteFile(hFile, "\r\n", 2, &dw, NULL);
		}
		else {
			Childs[i]->WriteToFile(hFile, csWord + Letter);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
