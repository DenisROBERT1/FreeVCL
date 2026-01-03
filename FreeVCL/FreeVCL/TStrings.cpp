//-----------------------------------------------------------------------------
//! @file TStrings.cpp
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
// TStrings
//---------------------------------------------------------------------------

TStrings::TStrings(void): TPersistent() {
  // Initialisations
  FUpdateCount = 0;
  FCaseSensitive = false;
	FUnicode = false;
	FUtf8 = false;
	FBigEndian = false;
	FByteOrderMark = false;
	FLinuxEndLine = false;
	FWantLastLine = false;
	bCrLfEndFile = true;
}

//---------------------------------------------------------------------------
TStrings::~TStrings(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TStrings::Assign(TPersistent* Source) {
  int i;
  int SourceCount;
	TStrings *StringsSource;


	StringsSource = dynamic_cast<TStrings *>(Source);
	if (StringsSource == NULL) return;

  BeginUpdate();
  Clear();
  SourceCount = StringsSource->Count;
  for (i = 0; i < SourceCount; i++) {
    Add(StringsSource->Strings[i]);
  }
	FWantLastLine = StringsSource->FWantLastLine;
	bCrLfEndFile = StringsSource->bCrLfEndFile;
  EndUpdate();
}

//---------------------------------------------------------------------------
int FASTCALL TStrings::AddObject(const AnsiString S, TObject* Object) {
	int Index;


	Index = Add(S);
	if (Object) PutObject(Index, Object);

	return Index;
}

//---------------------------------------------------------------------------
int FASTCALL TStrings::IndexOf(const AnsiString S) {
  int i;
  int StringCount;


  StringCount = Get_Count();
  for (i = 0; i < StringCount; i++) {
    if (FCaseSensitive) {
      if (!lstrcmp(Strings[i], S)) return i;
    }
    else {
      if (!lstrcmpi(Strings[i], S)) return i;
    }
  }

  return -1;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CaseSensitive
//---------------------------------------------------------------------------

bool TStrings::Get_CaseSensitive(void) {
  return FCaseSensitive;
}

bool TStrings::Set_CaseSensitive(bool NewCaseSensitive) {
  if (FCaseSensitive != NewCaseSensitive) {
    FCaseSensitive = NewCaseSensitive;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Unicode
//---------------------------------------------------------------------------

bool TStrings::Get_Unicode(void) {
  return FUnicode;
}

bool TStrings::Set_Unicode(bool NewUnicode) {
  if (FUnicode != NewUnicode) {
    FUnicode = NewUnicode;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Utf8
//---------------------------------------------------------------------------

bool TStrings::Get_Utf8(void) {
  return FUtf8;
}

bool TStrings::Set_Utf8(bool NewUtf8) {
  if (FUtf8 != NewUtf8) {
    FUtf8 = NewUtf8;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BigEndian
//---------------------------------------------------------------------------

bool TStrings::Get_BigEndian(void) {
  return FBigEndian;
}

bool TStrings::Set_BigEndian(bool NewBigEndian) {
  if (FBigEndian != NewBigEndian) {
    FBigEndian = NewBigEndian;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ByteOrderMark
//---------------------------------------------------------------------------

bool TStrings::Get_ByteOrderMark(void) {
  return FByteOrderMark;
}

bool TStrings::Set_ByteOrderMark(bool NewByteOrderMark) {
  if (FByteOrderMark != NewByteOrderMark) {
    FByteOrderMark = NewByteOrderMark;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LinuxEndLine
//---------------------------------------------------------------------------

bool TStrings::Get_LinuxEndLine(void) {
  return FLinuxEndLine;
}

bool TStrings::Set_LinuxEndLine(bool NewLinuxEndLine) {
  if (FLinuxEndLine != NewLinuxEndLine) {
    FLinuxEndLine = NewLinuxEndLine;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WantLastLine
//---------------------------------------------------------------------------

bool TStrings::Get_WantLastLine(void) {
  return FWantLastLine;
}

bool TStrings::Set_WantLastLine(bool NewWantLastLine) {
  if (FWantLastLine != NewWantLastLine) {
    FWantLastLine = NewWantLastLine;
		bCrLfEndFile = !FWantLastLine;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TStrings::Get_Count(void) {
  return GetCount();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété UpdateCount
//---------------------------------------------------------------------------

int TStrings::Get_UpdateCount(void) {
  return FUpdateCount;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TStrings::Get_Strings(int i) {
  FVCL_ASSERT(false && "TStrings::Get_Strings must be inherited");
	return _T("");
}

bool TStrings::Set_Strings(int i, AnsiString NewStrings) {
  FVCL_ASSERT(false && "TStrings::Set_Strings must be inherited");
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Objects
//---------------------------------------------------------------------------

TObject *TStrings::Get_Objects(int i) {
  return GetObject(i);
}

bool TStrings::Set_Objects(int i, TObject * NewObject) {
  PutObject(i, NewObject);

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TStrings::Get_Text(void) {
  return GetTextStr();
}

bool TStrings::Set_Text(AnsiString NewText) {
  SetTextStr(NewText);
  return true;
}

//---------------------------------------------------------------------------
// Opérateur =
//---------------------------------------------------------------------------

const TStrings & TStrings::operator = (TStrings &Str) {
	Assign(&Str);

	return *this;
}

//---------------------------------------------------------------------------
// Opérateur ==
//---------------------------------------------------------------------------

bool TStrings::operator == (TStrings &Str) {
	int i;
  int Count1, Count2;

	Count1 = GetCount();
	Count2 = Str.Count;
	if (Count1 != Count2) return false;

  for (i = 0; i < Count1; i++) {
		if (Get_Strings(i) != Str.Strings[i]) return false;
  }

	return true;
}

//---------------------------------------------------------------------------
// Opérateur !=
//---------------------------------------------------------------------------

bool TStrings::operator != (TStrings &Str) {
  return !operator == (Str);
}

//---------------------------------------------------------------------------
// Opérateur []
//---------------------------------------------------------------------------

AnsiString TStrings::operator[](int i) {
  return Get_Strings(i);
}

//---------------------------------------------------------------------------
// Lecture des lignes
//---------------------------------------------------------------------------

char * FASTCALL TStrings::GetText(void) {
  static AnsiString asRetour;  // static pour pouvoir retourner un pointeur sur la chaîne

  asRetour = GetTextStr();
  return (char *) (LPCTSTR) asRetour;
}


//---------------------------------------------------------------------------
// Lecture des lignes
//---------------------------------------------------------------------------

AnsiString FASTCALL TStrings::GetTextStr(void) {
  AnsiString asRet;
  int i, Count;


  Count = GetCount();
  for (i = 0; i < Count; i++) {
    if (i) asRet += _T("\n");  // C'est bien "\n" tout court (voir déclaration)
    asRet += operator[] (i);
  }

  return asRet;
}

//---------------------------------------------------------------------------
// Ecriture des lignes
//---------------------------------------------------------------------------

void FASTCALL TStrings::SetText(const char * Text) {
  SetTextStr(Text);
}


void FASTCALL TStrings::SetText(const wchar_t * Text) {
  SetTextStr(Text);
}


//---------------------------------------------------------------------------
// Ecriture des lignes
//---------------------------------------------------------------------------

void FASTCALL TStrings::SetTextStr(const AnsiString asValue) {
  int Deb, Fin, Len;


  BeginUpdate();
  Clear();
  Deb = 1;
  Len = asValue.Length();
  while (Deb <= Len) {
    Fin = Deb;
    while (Fin <= Len && asValue[Fin] != _T('\r') && asValue[Fin] != _T('\n')) Fin++;
    Add(asValue.SubString(Deb, Fin - Deb));
    Deb = Fin + 1;
    if (Deb <= Len && asValue[Fin] == _T('\r') && asValue[Deb] == _T('\n')) Deb++;
  }
  EndUpdate();

}

//---------------------------------------------------------------------------
// Série de modifications
//---------------------------------------------------------------------------

void FASTCALL TStrings::BeginUpdate(void) {
  FUpdateCount++;
  SetUpdateState(true);
}

void FASTCALL TStrings::EndUpdate(void) {
  FVCL_ASSERT(FUpdateCount > 0 && "EndUpdate sans BeginUpdate");
  if (FUpdateCount > 0) {
    FUpdateCount--;
    if (FUpdateCount == 0) SetUpdateState(false);
  }
}

//---------------------------------------------------------------------------
// Chargement depuis un fichier
//---------------------------------------------------------------------------

bool FASTCALL TStrings::LoadFromFile(const AnsiString FileName) {
	HANDLE hFile;
	DWORD dw;
	wchar_t c;
	char Bom[4];
	int Begin;
	CharString csLigne;
	WideString wsLigne;
	bool Ok;


	BeginUpdate();
	Clear();
	hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {

		// Test unicode
		FUnicode = false;
		FUtf8 = false;
		FBigEndian = false;
		FByteOrderMark = false;
		FLinuxEndLine = true;
		Begin = 0;
		memset(Bom, 0, sizeof(Bom));
    if (ReadFile(hFile, &Bom, 2 * sizeof(char), &dw, NULL)) {
			if (Bom[1] == '\0') {
				FUnicode = true;
				Begin = 0;
			}
			else if (Bom[0] == '\xFF' && Bom[1] == '\xFE') {
				FUnicode = true;
				FBigEndian = false;
				FByteOrderMark = true;
				Begin = 2;
			}
			else if (Bom[0] == '\xFE' && Bom[1] == '\xFF') {
				FUnicode = true;
				FBigEndian = true;
				FByteOrderMark = true;
				Begin = 2;
			}
			else if (Bom[0] == '\xEF' && Bom[1] == '\xBB') {
				// $$$ Données non vérifiées (fichier vcproj == EF BB BF)
				FUnicode = false;
				FBigEndian = false;
				FByteOrderMark = true;
				Begin = 3;
			}
		}

		BufFile = new char[SIZE_BUF];
		PosBuf = SIZE_BUF;
		SizeBuf = 0;

		SetFilePointer(hFile, Begin, NULL, FILE_BEGIN);

		for (;;) {
			if (FUnicode) {
				if (!ReadBuf(hFile, &c, sizeof(wchar_t))) break;
				if (FBigEndian) c = (c & 0x00FF) << 8 | (c & 0xFF00) >> 8;
			}
			else {
				c = L'\0';
				if (!ReadChar(hFile, (char *) &c)) break;
				if (c == L'Ã') {
					FUtf8 = true;  // Moyen un peu bourrin de détecter de l'Utf8 => à améliorer ?
				}
			}
      if (c == L'\r') {
				FLinuxEndLine = false;
      }
      else if (c == L'\n') {
				if (FUnicode) {
					Add(wsLigne.AsAnsiString());
	        wsLigne = L"";
				}
				else if (FUtf8) {
					Add(Utf8ToUnicode(csLigne.AsCharString().c_str()));
	        csLigne = "";
				}
				else {
					Add(csLigne.AsAnsiString());
	        csLigne = "";
				}
      }
      else {
				if (FUnicode) {
	        wsLigne += c;
				}
				else {
	        csLigne += (char) (BYTE) c;
				}
      }
    }
    if (!WantLastLine && csLigne.IsEmpty() && wsLigne.IsEmpty()) {
			bCrLfEndFile = true;
		}
		else {
			if (FUnicode) {
				Add(wsLigne.AsAnsiString());
			}
			else {
				Add(csLigne.AsAnsiString());
			}
			bCrLfEndFile = false;
		}
    CloseHandle(hFile);

		delete[] BufFile;

		Ok = true;
  }
  else {
    // Erreur ouverture FileName
		FLastError = GetLastError();
		Ok = false;
  }
  EndUpdate();

	return Ok;
}

//---------------------------------------------------------------------------
// Lecture d'un buffer
//---------------------------------------------------------------------------

bool TStrings::ReadBuf(HANDLE hFile, void * Buf, int Size) {
	int i;

	for (i = 0; i < Size; i++) {
		if (!ReadChar(hFile, &((char *) Buf)[i])) return false;
	}

	return true;
}

//---------------------------------------------------------------------------
// Lecture d'un caractère
//---------------------------------------------------------------------------

bool TStrings::ReadChar(HANDLE hFile, char * c) {

	if (PosBuf >= SIZE_BUF) {
    ReadFile(hFile, BufFile, SIZE_BUF, &SizeBuf, NULL);
		PosBuf = 0;
	}

	if (PosBuf >= SizeBuf) return false;

	*c = BufFile[PosBuf++];

  return true;
}

//---------------------------------------------------------------------------
// Chargement depuis un flux
//---------------------------------------------------------------------------

bool FASTCALL TStrings::LoadFromStream(TStream* Stream) {
  char c;
	AnsiString asLigne;


	while (Stream->Read(&c, sizeof(char))) {
    if (c == '\n') {
      Add(asLigne);
      asLigne = _T("");
    }
    else if (c != '\r') {
      asLigne += (TCHAR) (BYTE) c;
    }
  }
  if (!WantLastLine && asLigne.IsEmpty()) {
		bCrLfEndFile = true;
	}
	else {
		Add(asLigne);
		bCrLfEndFile = false;
	}

	return true;
}

//---------------------------------------------------------------------------
// Sauvegarde dans un fichier
//---------------------------------------------------------------------------

bool FASTCALL TStrings::SaveToFile(const AnsiString FileName) {
  HANDLE hFile;
  DWORD dw;
	wchar_t c;
  int i, j, Count;
  CharString csLigne;
  WideString wsLigne;
	bool Ok = false;


  hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
		if (FByteOrderMark) {
			if (FUnicode) {
				if (FBigEndian) WriteFile(hFile, "\xFE\xFF", 2, &dw, NULL);
				else WriteFile(hFile, "\xFF\xFE", 2, &dw, NULL);
			}
			else {
				WriteFile(hFile, "\xEF\xBB\xBF", 3, &dw, NULL);
			}
		}
    Count = Get_Count();
    for (i = 0; i < Count; i++) {
			if (FUnicode) {
				wsLigne = WideString(operator[] (i));
				if (bCrLfEndFile || i < Count - 1) {
					if (FLinuxEndLine) wsLigne += L"\n";
					else wsLigne += L"\r\n";
				}
				if (FBigEndian) {
			    for (j = 1; j <= wsLigne.Length(); j++) {
						c = wsLigne[j];
						c = (c & 0x00FF) << 8 | (c & 0xFF00) >> 8;
						WriteFile(hFile, &c, sizeof(wchar_t), &dw, NULL);
					}
				}
				else {
					WriteFile(hFile, (LPCWSTR) wsLigne, wsLigne.Length() * sizeof(wchar_t), &dw, NULL);
				}
			}
			else if (FUtf8) {
				csLigne = UnicodeToUtf8(WideString(operator[] (i)));
				if (bCrLfEndFile || i < Count - 1)  {
					if (FLinuxEndLine) csLigne += "\n";
					else csLigne += "\r\n";
				}
				WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length() * sizeof(char), &dw, NULL);
			}
			else {
				csLigne = operator[] (i);
				if (bCrLfEndFile || i < Count - 1)  {
					if (FLinuxEndLine) csLigne += "\n";
					else csLigne += "\r\n";
				}
				WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length() * sizeof(char), &dw, NULL);
			}
    }
    CloseHandle(hFile);

		Ok = true;
  }
  else {
    // Erreur ouverture FileName
		FLastError = GetLastError();
		if (FLastError == ERROR_ACCESS_DENIED) {
			// Copie par l'intermédiaire d'un fichier temporaire (à cause des droits administrateur)
			TCHAR szTempPath[MAX_PATH];
			TCHAR szTempFileName[MAX_PATH + 1];  // + 1 parce c'est une liste terminée par deux zéros
			TCHAR *szFileName;

			szFileName = new TCHAR[FileName.length() + 2];  // Idem que plus haut
			if (szFileName) {

				memset(szTempFileName, 0, sizeof(szTempFileName));
				memset(szFileName, 0, sizeof(szFileName));
				GetTempPath(MAX_PATH, szTempPath);
				GetTempFileName(szTempPath, _T("FVC"), 0, szTempFileName);
				lstrcpy(szFileName, FileName);

				if (SaveToFile(szTempFileName)) {

					SHFILEOPSTRUCT fos;
					memset(&fos, 0, sizeof(fos));
					fos.hwnd = Application->Handle;
					fos.wFunc = FO_COPY;
					fos.pFrom = szTempFileName;
					fos.pTo = szFileName;
					fos.fFlags = FOF_FILESONLY | FOF_NOCONFIRMATION;

					FLastError = SHFileOperation(&fos);
					if (FLastError == 0) Ok = true;
					DeleteFile(szTempFileName);
				}

				delete[] szFileName;

			}

		}
  }

	return Ok;
}

//---------------------------------------------------------------------------
// Sauvegarde dans un flux
//---------------------------------------------------------------------------

bool FASTCALL TStrings::SaveToStream(TStream* Stream) {
  AnsiString asLine;
  int i, Count;


  Count = Get_Count();
  for (i = 0; i < Count; i++) {
    asLine = operator[] (i);
    Stream->Write(asLine, asLine.Length() * sizeof(TCHAR));
    if (bCrLfEndFile || i < Count - 1) Stream->Write(_T("\r\n"), 2 * sizeof(TCHAR));
  }

	return true;
}

//---------------------------------------------------------------------------
