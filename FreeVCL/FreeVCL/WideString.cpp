//-----------------------------------------------------------------------------
//! @file WideString.cpp
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


// "using namespace std" pour compatibilité Visual Studio 6
// sinon erreur "illegal member initialization: 'string' is not a base or member"
using namespace std;


//---------------------------------------------------------------------------
//
//                         fonctions globales
//
//---------------------------------------------------------------------------

#ifdef __GNUG__

/* 05/2020 redeclared without dllimport attribute

// itoa n'est pas du C standard
wchar_t * _itow(int i, wchar_t * w, int b) {
  wchar_t c;
  int j, n;
  bool bNeg = false;


  if (i < 0) {
    i = -i;
    bNeg = true;
  }
  j = 0;
  do {
    n = i % b;
    i = i / b;
    if (n <= 9) c = L'0' + n;
    else c = L'A' + n - 10;
    w[j++] = c;
  } while(i);

  // Signe
  if (bNeg) w[j++] = L'-';

  // Zéro terminal
  w[j++] = L'\0';

  // Inversion de la chaîne
  n = j;
  for (j = 0; j < n / 2; j++) {
    c = w[j];
    w[j] = w[n - j - 1];
    w[n - j - 1] = c;
  }

  return w;
}

int _wtoi(const wchar_t * w) {
  wchar_t c;
  int i, j;
  bool bNeg = false;


  // Initialisation
  i = 0;
  j = 0;

  // Parcours de la chaîne
  c = w[j++];
  while (c) {
    if (c == '-') bNeg = true;
    else if (c == '+') bNeg = false;
    else {
      i *= 10;
      i += (int) (c - '0');
    }

    c = w[j++];
  }

  // Signe
  if (bNeg) i = -i;

  return i;
}

__int64 _wtoi64(const wchar_t * w) {
  wchar_t c;
  __int64 i;
  int j;
  bool bNeg = false;


  // Initialisation
  i = 0;
  j = 0;

  // Parcours de la chaîne
  c = w[j++];
  while (c) {
    if (c == '-') bNeg = true;
    else if (c == '+') bNeg = false;
    else {
      i *= 10;
      i += (int) (c - '0');
    }

    c = w[j++];
  }

  // Signe
  if (bNeg) i = -i;

  return i;
}

*/

#endif	// __GNUG__

WideString IntToStrW(int Value) {

  wchar_t szBuf[20];
  _itow_s(Value, szBuf, 20, 10);

  return szBuf;
}

WideString FASTCALL WideString::StringOfChar(wchar_t ch, int count) {
  WideString asRetour;
  for (int i = 0; i < count; i++) asRetour += ch;

  return asRetour;
}

int StrToIntW(const WideString &src) {
  return _wtoi(src);
}

int StrToIntDefW(const WideString &src, int Default) {
  return src.ToIntDef(Default);
}

bool FASTCALL HexToByteW(wchar_t c, BYTE * q) {
	if (L'0' <= c && c <= L'9') {
		*q = c - L'0';
		return true;
	}
	if (L'A' <= c && c <= L'F') {
		*q = c - L'A' + 10;
		return true;
	}
  if (L'a' <= c && c <= L'f') {
    *q = c - L'a' + 10;
		return true;
	}

	return false;
}

int FASTCALL HexToBinW(const wchar_t *Text, void *Buffer, int BufSize) {
  wchar_t c;
  BYTE b = 0;
  BYTE q;
  bool bHighByte = true;
  int i = 0;
  int j = 0;
	int NoHexChar = 0;
  while (j < BufSize) {
    c = Text[i++];
    if (c == L'\0') break;
		if (HexToByteW(c, &q)) {
      b = (b << 4) + q;
      bHighByte = !bHighByte;
      if (bHighByte) ((char *) Buffer)[j++] = (char) b;
    }
		else NoHexChar++;
  }
  if (!bHighByte && j < BufSize) ((char *) Buffer)[j] = (char) b;

  return NoHexChar;
}

void FASTCALL BinToHexW(const void *Buffer, wchar_t *Text, int BufSize) {
  int i, j;

	for (i = 0, j = 0; i < BufSize; i++) {
    wsprintfW(&Text[j], L"%02X", ((BYTE *) Buffer)[i]);
    j += 2;
  }
}

int FASTCALL HexToIntW(const wchar_t *Text, void *Buffer, int BufSize) {
  wchar_t c;
  BYTE b = 0;
  BYTE q;
  bool bHighByte = true;
  int i = lstrlenW(Text) - 1;
  int j = 0;
	int NoHexChar = 0;
	memset(Buffer, 0, BufSize);
  while (j < BufSize) {
    if (i < 0) break;
    c = Text[i--];
		if (HexToByteW(c, &q)) {
      b = (b >> 4) + (q << 4);
      bHighByte = !bHighByte;
			if (bHighByte) ((char *) Buffer)[j++] = (char) b;
    }
		else NoHexChar++;
  }
	if (!bHighByte && j < BufSize) ((char *) Buffer)[j] = (char) (b >> 4);

  return NoHexChar;
}

void FASTCALL IntToHexW(const void *Buffer, wchar_t *Text, int BufSize) {
  int i, j;

	for (i = BufSize - 1, j = 0; i >= 0 ; i--) {
		if (j || ((char *) Buffer)[i] != 0) {
			wsprintfW(&Text[j], L"%02X", ((BYTE *) Buffer)[i]);
			j += 2;
		}
  }
	if (j == 0) lstrcpyW(Text, L"00");
}

// ----------------------------------------------------------------------------
WideString FASTCALL Utf8ToUnicode(const char *szSrc) {
	WideString sRet;
	unsigned int b, bh;
	int NbBytes = 0;
	int i;

	for (i = 0; i < (int) lstrlenA(szSrc); i++) {
		b = szSrc[i];
		if (NbBytes > 0) {
			bh = (bh << 6) | (b & 0x3F);
			if (--NbBytes == 0) sRet += (wchar_t) bh;
		}
		else if ((b & 0xE0) == 0xC0) {
			bh = b & 0x1F;
			NbBytes = 1;
		}
		else if ((b & 0xF0) == 0xE0) {
			bh = b & 0x0F;
			NbBytes = 2;
		}
		else if ((b & 0xF8) == 0xF0) {
			bh = b & 0x07;
			NbBytes = 3;
		}
		else {
			sRet += (wchar_t) b;
		}
	}

	return sRet;
}

// ----------------------------------------------------------------------------
CharString FASTCALL UnicodeToUtf8(const WideString &wsSrc) {
  CharString sRet;
  unsigned int b;
  int i;

  for (i = 1; i <= (int) wsSrc.length(); i++) {
		b = (unsigned int) wsSrc[i];

		if (b < 0x80) {
			sRet += (BYTE) b;
		}
		else if (b < 0x800) {
			sRet += (BYTE) (((b >> 6) & 0x1F) | 0xC0);
			sRet += (BYTE) ((b & 0x3F) | 0x80);
		}
		else if (b < 0x10000) {
			sRet += (BYTE) (((b >> 12) & 0x0F) | 0xE0);
			sRet += (BYTE) (((b >> 6) & 0x3F) | 0x80);
			sRet += (BYTE) ((b & 0x3F) | 0x80);
		}
		else {
			sRet += (BYTE) (((b >> 18) & 0x07) | 0xF0);
			sRet += (BYTE) (((b >> 12) & 0x3F) | 0x80);
			sRet += (BYTE) (((b >> 6) & 0x3F) | 0x80);
			sRet += (BYTE) ((b & 0x3F) | 0x80);
		}
  }

  return sRet;
}

// ----------------------------------------------------------------------------
WideString FASTCALL MultiByteToUnicode(const char *szSrc) {
	WideString sRet;
  int i = 0;
  wchar_t c;
	LPWSTR lpWideCharStr;
	int cchWideChar;


  cchWideChar = MultiByteToWideChar(1252, MB_PRECOMPOSED, szSrc, -1, NULL, 0);
	if (cchWideChar > 0) {
		lpWideCharStr = new wchar_t[cchWideChar];
		MultiByteToWideChar(1252, MB_PRECOMPOSED, szSrc, -1, lpWideCharStr, cchWideChar);
		sRet = lpWideCharStr;
		delete[] lpWideCharStr;
	}
	else {
		c = (wchar_t) (BYTE) szSrc[i++];
		while (c) {
			sRet += c;
			c = (wchar_t) (BYTE) szSrc[i++];
		}
	}

  return sRet;
}

// ----------------------------------------------------------------------------
CharString FASTCALL UnicodeToMultiByte(const WideString &wsSrc) {
  CharString sRet;
  int i = 0;
  char c;
	LPSTR lpMultiByteStr;
	int cbMultiByte;


  cbMultiByte = WideCharToMultiByte(1252, WC_NO_BEST_FIT_CHARS, wsSrc, -1, NULL, 0, NULL, NULL);
	if (cbMultiByte > 0) {
		lpMultiByteStr = new char[cbMultiByte];
		WideCharToMultiByte(1252, WC_NO_BEST_FIT_CHARS, wsSrc, -1, lpMultiByteStr, cbMultiByte, NULL, NULL);
		sRet = lpMultiByteStr;
		delete[] lpMultiByteStr;
	}
	else {
		c = (char) (BYTE) wsSrc[i++];
		while (c) {
			sRet += c;
			c = (char) (BYTE) wsSrc[i++];
		}
	}

	return sRet;
}


//---------------------------------------------------------------------------
// WideString
//---------------------------------------------------------------------------

using namespace std;
WideString::WideString(void): wstring() {
}

WideString::WideString(const char* src): wstring() {
	*this = src;
}

WideString::WideString(const wchar_t* src): wstring(src) {
}

WideString::WideString(const CharString& src): wstring() {
	*this = src;
}

WideString::WideString(const WideString& src): wstring(src) {
}

WideString::WideString(const wchar_t c, unsigned int len): wstring() {
  for (unsigned int i = 0; i < len; i++) *this += c;
}

WideString::WideString(const wchar_t* src, unsigned int len): wstring() {
  *this = wstring(src).substr(0, len);
}

WideString::WideString(int i): wstring() {
  wchar_t szBuf[20];
	_itow_s(i, szBuf, 20, 10);
  *this = szBuf;
}

WideString::WideString(double src): wstring() {
}

WideString::WideString(wchar_t src): wstring() {
  wchar_t szBuf[2];

  szBuf[0] = src;
  szBuf[1] = L'\0';
  *this = wstring(szBuf);
}

WideString::WideString(short i): wstring() {
  wchar_t szBuf[20];
	_itow_s(i, szBuf, 20, 10);
  *this = szBuf;
}

WideString::WideString(unsigned int i): wstring() {
  wchar_t szBuf[20];
	_itow_s(i, szBuf, 20, 10);
  *this = szBuf;
}

WideString::WideString(long i): wstring() {
  wchar_t szBuf[20];
	_itow_s(i, szBuf, 20, 10);
  *this = szBuf;
}

WideString::WideString(unsigned long i): wstring() {
  wchar_t szBuf[20];
	_itow_s(i, szBuf, 20, 10);
  *this = szBuf;
}

WideString::WideString(__int64 i): wstring() {
  wchar_t szBuf[20];
  *this = _i64tow_s(i, szBuf, 20, 10);
}

WideString::WideString(unsigned __int64 i): wstring() {
  wchar_t szBuf[20];
  *this = _i64tow_s(i, szBuf, 20, 10);
}

WideString::WideString(const wstring s): wstring(s) {
}

//---------------------------------------------------------------------------

WideString& FASTCALL WideString::operator = (int i) {
  wchar_t szBuf[20];
	_itow_s(i, szBuf, 20, 10);
  *this = szBuf;

  return *this;
}

WideString& FASTCALL WideString::operator = (unsigned int i) {
  wchar_t szBuf[20];
	_itow_s(i, szBuf, 20, 10);
  *this = szBuf;

  return *this;
}

WideString& FASTCALL WideString::operator = (const wchar_t c) {
  wstring::operator= (c);

  return *this;
}

WideString& FASTCALL WideString::operator = (const wchar_t* rhs) {
  wstring::operator= (rhs);

  return *this;
}

WideString& FASTCALL WideString::operator = (const wstring& rhs) {
  wstring::operator= (rhs);

  return *this;
}

WideString& FASTCALL WideString::operator = (const WideString& rhs) {
  wstring::operator= (rhs);

  return *this;
}

WideString& FASTCALL WideString::operator = (const char c) {
	*this = (wchar_t) (BYTE) c;

  return *this;
}

WideString& FASTCALL WideString::operator = (const char* rhs) {
  int i = 0;
  wchar_t c;


#if (0 < _MSC_VER && _MSC_VER <= 1200)
  wstring::operator = (L"");
#else  // _MSC_VER <= 1200
  clear();
#endif  // _MSC_VER <= 1200

	c = (wchar_t) (BYTE) rhs[i++];
	while (c) {
		*this += c;
		c = (wchar_t) (BYTE) rhs[i++];
	}

  return *this;
}

WideString& FASTCALL WideString::operator = (const string& rhs) {
	*this = rhs.c_str();

  return *this;
}

WideString& FASTCALL WideString::operator = (const CharString& rhs) {
	*this = rhs.c_str();

  return *this;
}


wchar_t& FASTCALL WideString::operator [](int idx) {
  FVCL_ASSERT(1 <= idx && idx <= Length() && "WideString::operator[]: Index out of string");
  return wstring::operator[] (idx - 1);
}

const wchar_t& FASTCALL WideString::operator [](int idx) const {
  FVCL_ASSERT(1 <= idx && idx <= Length() && "WideString::operator[]: Index out of string");
  return wstring::operator[] (idx - 1);
}

WideString::operator const wchar_t *() const {
  return c_str();
}

int __cdecl WideString::printf(const wchar_t* format, ...) {
  *this = format;

  return lstrlenW(format);
}

WideString& __cdecl WideString::sprintf(const wchar_t* format, ...) {
  wchar_t *szBuf;
	int SizeBuf;
  wchar_t szFmt[10];
  BYTE * pArg;
  int i, j, k;
  wchar_t c;


	SizeBuf = lstrlenW(format) + 128;
	szBuf = new wchar_t[SizeBuf];
  pArg = (BYTE *) &format + sizeof(wchar_t *);
  i = j = 0;
  do {
    c = format[i++];
    if (c == L'%') {
      k = 0;
      szFmt[k++] = c;
      do {
        c = format[i++];
        szFmt[k++] = c;
        switch (c) {
        case L'%':
          szBuf[j++] = c;
          c = L'\0';
          break;
        case L'c':
          c = L'\0';
          szFmt[k] = c;
					szBuf = ReAllocBuf(szBuf, &SizeBuf, j + 20);
          j += wsprintfW(&szBuf[j], szFmt, * ((wchar_t *) pArg));
          pArg += ARG_SIZE(sizeof(int));
          break;
        case L'i':
        case L'd':
        case L'x':
        case L'X':
          c = L'\0';
          szFmt[k] = c;
					szBuf = ReAllocBuf(szBuf, &SizeBuf, j + 20);
          if (!lstrcmpW(szFmt, L"%I64i") || !lstrcmpW(szFmt, L"%I64d")) {
            j += wsprintfW(&szBuf[j], szFmt, * ((LONGLONG *) pArg));
						pArg += ARG_SIZE(sizeof(LONGLONG));
          }
          else {
            j += wsprintfW(&szBuf[j], szFmt, * ((int *) pArg));
            pArg += ARG_SIZE(sizeof(int));
          }
          break;
        case L's':
          c = L'\0';
          szFmt[k] = c;
          wchar_t * lpStr;
          lpStr = * ((wchar_t * *) pArg);
          // Bon, je sais, IsBadPtr c'est pas joli,
          // mais comment éviter que ça plante si pointeur incorrect ?
          if (!IsBadReadPtr(lpStr, sizeof(wchar_t))) {
						szBuf = ReAllocBuf(szBuf, &SizeBuf, j + lstrlenW(lpStr) + 1);
						j += wsprintfW(&szBuf[j], szFmt, lpStr);
          }
					pArg += ARG_SIZE(sizeof(wchar_t *));
          break;
        case L'0':
        case L'1':
        case L'2':
        case L'3':
        case L'4':
        case L'5':
        case L'6':
        case L'7':
        case L'8':
        case L'9':
        case L'.':
        case L' ':
        case L'+':
        case L'-':
        case L'l':
        case L'h':
        case L'I':
          break;
        default:
          szBuf[j++] = c;
          c = L'\0';
          break;
        }
      } while (c && k < 10);
      c = L'%';
    }
    else {
			szBuf = ReAllocBuf(szBuf, &SizeBuf, j + 1);
			szBuf[j++] = c;
		}
  } while (c);

  *this = szBuf;
	delete[] szBuf;

  return *this;
}

wchar_t *WideString::ReAllocBuf(wchar_t *szBuf, int *SizeBuf, int NewSizeBuf) {

	if (*SizeBuf >= NewSizeBuf) return szBuf;

	wchar_t *szNewBuf = new wchar_t[NewSizeBuf + 128];
	memcpy(szNewBuf, szBuf, *SizeBuf * sizeof(wchar_t));
	*SizeBuf = NewSizeBuf + 128;
	delete[] szBuf;

	return szNewBuf;
}

bool FASTCALL WideString::IsEmpty(void) const {
  return (length() == 0);
}

int FASTCALL WideString::Length(void) const {
  return (int) length();
}


WideString FASTCALL WideString::SubString(int index, int count) const {
  if (count <= 0) return L"";
  FVCL_ASSERT(1 <= index && index <= Length() && "SubString: Index out of string");
  size_type pos = index - 1;
  return substr(pos, count);
}

WideString& FASTCALL WideString::Insert(const WideString& str, int index) {
  if (index < 1) index = 1;
  if (index > Length() + 1) index = Length() + 1;
  insert(index - 1, str);

  return *this;
}

WideString& FASTCALL WideString::Delete(int index, int count) {
  if (1 <= index && index <= Length() && count > 0) erase(index - 1, count);

  return *this;
}

WideString FASTCALL WideString::LowerCase(void) const {
  WideString asRet;
  wchar_t *szRet;
  int Len;


  Len = Length() + 1;
  szRet = new wchar_t[Len];
  lstrcpyW(szRet, c_str());
  CharLowerW(szRet);
  asRet = WideString(szRet);
  delete[] szRet;

  return asRet;
}

WideString FASTCALL WideString::UpperCase(void) const {
  WideString asRet;
  wchar_t *szRet;
  int Len;


  Len = Length() + 1;
  szRet = new wchar_t[Len];
  lstrcpyW(szRet, c_str());
  CharUpperW(szRet);
  asRet = WideString(szRet);
  delete[] szRet;

  return asRet;
}

WideString FASTCALL WideString::ToOEM(void) const {
  WideString asRet;

  int Len = Length() + 1;
  char *szRet = new char[Len];
  CharToOemW(c_str(), szRet);
  asRet = WideString(szRet);
  delete[] szRet;

  return asRet;
}

WideString FASTCALL WideString::ToAnsi(void) const {
  WideString asRet;

  int Len = Length() + 1;
  wchar_t *szRet = new wchar_t[Len];
  OemToCharW(CharString(c_str()), szRet);
  asRet = WideString(szRet);
  delete[] szRet;

  return asRet;
}

WideString FASTCALL WideString::TrimRight(void) const {
  size_type Fin;


  Fin = find_last_not_of(L" \t\r\n") + 1;
  if (Fin == string::npos) return *this;
  return substr(0, Fin);
}


WideString FASTCALL WideString::TrimLeft(void) const {
  size_type Deb;
  size_type Count;


  Deb = find_first_not_of(L" \t\r\n");
  if (Deb == string::npos) return *this;
  Count = length() - Deb;
  return substr(Deb, Count);
}


WideString FASTCALL WideString::Trim(void) const {
  return TrimRight().TrimLeft();
}

int FASTCALL WideString::LastDelimiter(const WideString& delimiters) const {
  WideString asRet;
  return (int) find_last_of(delimiters) + 1;
}

int FASTCALL WideString::AnsiCompareIC(const WideString& rhs) const {
  return lstrcmpiW(c_str(), rhs);
}

int FASTCALL WideString::AnsiPos(const WideString& subStr) {
  return (int) find(subStr) + 1;
}

int FASTCALL WideString::Pos(const WideString& subStr) const {
  return (int) find(subStr) + 1;
}

int FASTCALL WideString::ToInt(void) const {
  return _wtoi(c_str());
}

int FASTCALL WideString::ToIntDef(int Default) const {
  return _wtoi(c_str());
}

LongLong FASTCALL WideString::ToLongLong(void) const {
  return _wtoi64(c_str());
}

double FASTCALL WideString::ToDouble(void) const {
  CharString csString;
  csString = CharString(c_str());
  return atof(csString);
}

//---------------------------------------------------------------------------
int FASTCALL WideString::CharCharBufSize(void) const {
  return (int) (length() + 1) * sizeof(char);
}

char* FASTCALL WideString::CharChar(char* dest, int DestSize) const {
  int i;
  for (i = 0; i < (int) length(); i++) {
    if (i >= DestSize - 1) break;
    dest[i] = (char) wstring::operator[] (i);
  }
  dest[i] = '\0';

  return dest;
}

//---------------------------------------------------------------------------
int FASTCALL WideString::WideCharBufSize(void) const {
  return (int) (length() + 1) * sizeof(wchar_t);
}

wchar_t* FASTCALL WideString::WideChar(wchar_t* dest, int DestSize) const {
  lstrcpynW(dest, c_str(), DestSize);
  return dest;
}

//---------------------------------------------------------------------------
CharString FASTCALL WideString::AsCharString(void) const {
	return CharString(*this);
}

//---------------------------------------------------------------------------
WideString FASTCALL WideString::AsWideString(void) const {
	return *this;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL WideString::AsAnsiString(void) const {
#ifdef UNICODE
	return *this;
#else
	return CharString(*this);
#endif  // UNICODE
}

//---------------------------------------------------------------------------

