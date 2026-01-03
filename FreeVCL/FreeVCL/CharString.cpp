//-----------------------------------------------------------------------------
//! @file CharString.cpp
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
char * _itoa(int i, char * a, int b) {
  char c;
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
    if (n <= 9) c = '0' + n;
    else c = 'A' + n - 10;
    a[j++] = c;
  } while(i);

  // Signe
  if (bNeg) a[j++] = '-';

  // Zéro terminal
  a[j++] = '\0';

  // Inversion de la chaîne
  n = j;
  for (j = 0; j < n / 2; j++) {
    c = a[j];
    a[j] = a[n - j - 1];
    a[n - j - 1] = c;
  }

  return a;
}

__int64 _atoi64(const char * a) {
  char c;
  __int64 i;
  int j;
  bool bNeg = false;


  // Initialisation
  i = 0;
  j = 0;

  // Parcours de la chaîne
  c = a[j++];
  while (c) {
    if (c == '-') bNeg = true;
    else if (c == '+') bNeg = false;
    else {
      i *= 10;
      i += (int) (c - '0');
    }

    c = a[j++];
  }

  // Signe
  if (bNeg) i = -i;

  return i;
}

*/

#endif	// __GNUG__

CharString FASTCALL IntToStrA(int Value) {

  char szBuf[20];
  _itoa_s(Value, szBuf, 20, 10);

  return szBuf;
}

int FASTCALL StrToIntA(const CharString &src) {
  return atoi(src);
}

int FASTCALL StrToIntDefA(const CharString &src, int Default) {
  return src.ToIntDef(Default);
}

bool FASTCALL HexToByteA(char c, BYTE * q) {
	if ('0' <= c && c <= '9') {
		*q = c - '0';
		return true;
	}
	if ('A' <= c && c <= 'F') {
		*q = c - 'A' + 10;
		return true;
	}
  if ('a' <= c && c <= 'f') {
    *q = c - 'a' + 10;
		return true;
	}

	return false;
}

int FASTCALL HexToBinA(const char *Text, void *Buffer, int BufSize) {
  char c;
  BYTE b = 0;
  BYTE q;
  bool bHighByte = true;
  int i = 0;
  int j = 0;
	int NoHexChar = 0;
  while (j < BufSize) {
    c = Text[i++];
    if (c == '\0') break;
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

void FASTCALL BinToHexA(const void *Buffer, char *Text, int BufSize) {
  int i, j;

	for (i = 0, j = 0; i < BufSize; i++) {
    wsprintfA(&Text[j], "%02X", ((BYTE *) Buffer)[i]);
    j += 2;
  }
}

int FASTCALL HexToIntA(const char *Text, void *Buffer, int BufSize) {
  char c;
  BYTE b = 0;
  BYTE q;
  bool bHighByte = true;
  int i = lstrlenA(Text) - 1;
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

void FASTCALL IntToHexA(const void *Buffer, char *Text, int BufSize) {
  int i, j;

	for (i = BufSize - 1, j = 0; i >= 0 ; i--) {
		if (j || ((char *) Buffer)[i] != 0) {
			wsprintfA(&Text[j], "%02X", ((BYTE *) Buffer)[i]);
			j += 2;
		}
  }
	if (j == 0) lstrcpyA(Text, "00");
}

// ----------------------------------------------------------------------------
CharString FASTCALL Utf8ToAnsi(const char *szSrc) {
	CharString sRet;
	unsigned int b, bh = 0;
	int NbBytes = 0;
	int i;

	for (i = 0; i < (int) lstrlenA(szSrc); i++) {
		b = szSrc[i];
		if (NbBytes > 0) {
			bh = (bh << 6) | (b & 0x3F);
			if (--NbBytes == 0) sRet += (char) bh;
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
			sRet += (char) b;
		}
	}

	return sRet;
}

// ----------------------------------------------------------------------------
CharString FASTCALL AnsiToUtf8(const CharString &csSrc) {
  CharString sRet;
  unsigned int b;
  int i;

  for (i = 1; i <= (int) csSrc.length(); i++) {
		b = (BYTE) csSrc[i];

		if (b < 0x80) {
			sRet += (char) b;
		}
		else if (b < 0x800) {
			sRet += (char) (((b >> 6) & 0x1F) | 0xC0);
			sRet += (char) ((b & 0x3F) | 0x80);
		}
		else if (b < 0x10000) {
			sRet += (char) (((b >> 12) & 0x0F) | 0xE0);
			sRet += (char) (((b >> 6) & 0x3F) | 0x80);
			sRet += (char) ((b & 0x3F) | 0x80);
		}
		else {
			sRet += (char) (((b >> 18) & 0x07) | 0xF0);
			sRet += (char) (((b >> 12) & 0x3F) | 0x80);
			sRet += (char) (((b >> 6) & 0x3F) | 0x80);
			sRet += (char) ((b & 0x3F) | 0x80);
		}
  }

  return sRet;
}

//---------------------------------------------------------------------------
// CharString
//---------------------------------------------------------------------------

CharString::CharString(void): string() {
}

CharString::CharString(const char* src): string(src) {
}

CharString::CharString(const wchar_t* src): string() {
	*this = src;
}

CharString::CharString(const CharString& src): string(src) {
}

CharString::CharString(const WideString& src): string() {
	*this = src;
}

CharString::CharString(const char c, unsigned int len): string() {
  for (unsigned int i = 0; i < len; i++) *this += c;
}

CharString::CharString(const char* src, unsigned int len): string() {
  string::operator= (string(src).substr(0, len));
}

CharString::CharString(int i): string() {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;
}

CharString::CharString(double src): string() {
}

CharString::CharString(char src): string() {
  string::operator= (src);  // Attention : il faut pouvoir faire CharString('\0')
}

CharString::CharString(short i): string() {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;
}

CharString::CharString(unsigned short i): string() {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;
}

CharString::CharString(unsigned int i): string() {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;
}

CharString::CharString(long i): string() {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;
}

CharString::CharString(unsigned long i): string() {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;
}

CharString::CharString(__int64 i): string() {
  char szBuf[20];
  string::operator= (_i64toa_s(i, szBuf, 20, 10));
}

CharString::CharString(unsigned __int64 i): string() {
  char szBuf[20];
  string::operator= (_i64toa_s(i, szBuf, 20, 10));
}

CharString::CharString(const string s): string(s) {
}

//---------------------------------------------------------------------------

CharString& FASTCALL CharString::operator = (int i) {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;

  return *this;
}

CharString& FASTCALL CharString::operator = (unsigned int i) {
  char szBuf[20];
	_itoa_s(i, szBuf, 20, 10);
	*this = szBuf;

  return *this;
}

CharString& FASTCALL CharString::operator = (const char c) {
  string::operator= (c);

  return *this;
}

CharString& FASTCALL CharString::operator = (const char* rhs) {
  string::operator= (rhs);

  return *this;
}

CharString& FASTCALL CharString::operator = (const string& rhs) {
  string::operator= (rhs);

  return *this;
}

CharString& FASTCALL CharString::operator = (const CharString& rhs) {
  string::operator= (rhs);

  return *this;
}

CharString& FASTCALL CharString::operator = (const wchar_t c) {
	*this = (char) c;

  return *this;
}

CharString& FASTCALL CharString::operator = (const wchar_t* rhs) {
  int i = 0;
  char c;


#if (0 < _MSC_VER && _MSC_VER <= 1200)
  string::operator = ("");
#else  // _MSC_VER <= 1200
  clear();
#endif  // _MSC_VER <= 1200

	c = (char) (BYTE) rhs[i++];
	while (c) {
		*this += c;
		c = (char) (BYTE) rhs[i++];
	}

  return *this;
}

CharString& FASTCALL CharString::operator = (const wstring& rhs) {
	*this = rhs.c_str();

  return *this;
}

CharString& FASTCALL CharString::operator = (const WideString& rhs) {
	*this = rhs.c_str();

  return *this;
}

char& FASTCALL CharString::operator [](int idx) {
  FVCL_ASSERT(1 <= idx && idx <= Length() && "CharString::operator[]: Index out of string");
  return string::operator[] (idx - 1);
}

const char& FASTCALL CharString::operator [](int idx) const {
  FVCL_ASSERT(1 <= idx && idx <= Length() && "CharString::operator[]: Index out of string");
  return string::operator[] (idx - 1);
}

CharString::operator const char *() const {
  return c_str();
}

int __cdecl CharString::printf(const char* format, ...) {
  string::operator= (format);
  // $$$

  return lstrlenA(format);
}

CharString& __cdecl CharString::sprintf(const char* format, ...) {
  char *szBuf;
	int SizeBuf;
  char szFmt[10];
  BYTE * pArg;
  int i, j, k;
  char c;


	SizeBuf = lstrlenA(format) + 128;
	szBuf = new char[SizeBuf];
  pArg = (BYTE *) &format + sizeof(char *);
  i = j = 0;
  do {
    c = format[i++];
    if (c == '%') {
      k = 0;
      szFmt[k++] = c;
      do {
        c = format[i++];
        szFmt[k++] = c;
        switch (c) {
        case '%':
          szBuf[j++] = c;
          c = '\0';
          break;
        case 'c':
          c = '\0';
          szFmt[k] = c;
					szBuf = ReAllocBuf(szBuf, &SizeBuf, j + 20);
          j += wsprintfA(&szBuf[j], szFmt, * ((char *) pArg));
					pArg += ARG_SIZE(sizeof(int));
          break;
        case 'i':
        case 'd':
        case 'x':
        case 'X':
          c = '\0';
          szFmt[k] = c;
					szBuf = ReAllocBuf(szBuf, &SizeBuf, j + 20);
          if (!lstrcmpA(szFmt, "%I64i") || !lstrcmpA(szFmt, "%I64d")) {
            j += wsprintfA(&szBuf[j], szFmt, * ((LONGLONG *) pArg));
						pArg += ARG_SIZE(sizeof(LONGLONG));
          }
          else {
            j += wsprintfA(&szBuf[j], szFmt, * ((int *) pArg));
						pArg += ARG_SIZE(sizeof(int));
          }
          break;
        case 's':
          c = '\0';
          szFmt[k] = c;
          char * lpStr;
          lpStr = * ((char * *) pArg);
          // Bon, je sais, IsBadPtr c'est pas joli,
          // mais comment éviter que ça plante si pointeur incorrect ?
          if (!IsBadReadPtr(lpStr, sizeof(char))) {
						szBuf = ReAllocBuf(szBuf, &SizeBuf, j + lstrlenA(lpStr) + 1);
	          j += wsprintfA(&szBuf[j], szFmt, lpStr);
          }
					pArg += ARG_SIZE(sizeof(char *));
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ' ':
        case '+':
        case '-':
        case 'l':
        case 'h':
        case 'I':
          break;
        default:
          szBuf[j++] = c;
          c = '\0';
          break;
        }
      } while (c && k < 10);
      c = '%';
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

char *CharString::ReAllocBuf(char *szBuf, int *SizeBuf, int NewSizeBuf) {

	if (*SizeBuf >= NewSizeBuf) return szBuf;

	char *szNewBuf = new char[NewSizeBuf + 128];
	memcpy(szNewBuf, szBuf, *SizeBuf * sizeof(char));
	*SizeBuf = NewSizeBuf + 128;
	delete[] szBuf;

	return szNewBuf;
}

bool FASTCALL CharString::IsEmpty(void) const {
  return (length() == 0);
}

int FASTCALL CharString::Length(void) const {
  return (int) length();
}


CharString FASTCALL CharString::SubString(int index, int count) const {
  if (count <= 0) return "";
  FVCL_ASSERT(1 <= index && index <= Length() && "SubString: Index out of string");
  size_type pos = index - 1;
  return substr(pos, count);
}

CharString& FASTCALL CharString::Insert(const CharString& str, int index) {
  if (index < 1) index = 1;
  if (index > Length() + 1) index = Length() + 1;
  insert(index - 1, str);

  return *this;
}

CharString& FASTCALL CharString::Delete(int index, int count) {
  if (1 <= index && index <= Length() && count > 0) erase(index - 1, count);

  return *this;
}

CharString FASTCALL CharString::LowerCase(void) const {
  CharString csRet;
  char *szRet;
  int Len;


  Len = Length() + 1;
  szRet = new char[Len];
  lstrcpyA(szRet, c_str());
  CharLowerA(szRet);
  csRet = CharString(szRet);
  delete[] szRet;

  return csRet;
}

CharString FASTCALL CharString::UpperCase(void) const {
  CharString csRet;
  char *szRet;
  int Len;


  Len = Length() + 1;
  szRet = new char[Len];
  lstrcpyA(szRet, c_str());
  CharUpperA(szRet);
  csRet = CharString(szRet);
  delete[] szRet;

  return csRet;
}

CharString FASTCALL CharString::ToOEM(void) const {
  CharString csRet;
  char *szRet;
  int Len;


  Len = Length() + 1;
  szRet = new char[Len];
  CharToOemA(c_str(), szRet);
  csRet = CharString(szRet);
  delete[] szRet;

  return csRet;
}

CharString FASTCALL CharString::ToAnsi(void) const {
  CharString csRet;
  char *szRet;
  int Len;


  Len = Length() + 1;
  szRet = new char[Len];
  OemToCharA(c_str(), szRet);
  csRet = CharString(szRet);
  delete[] szRet;

  return csRet;
}

CharString FASTCALL CharString::TrimRight(void) const {
  size_type Fin;


  Fin = find_last_not_of(" \t\r\n") + 1;
	if (Fin == string::npos) return *this;
  return substr(0, Fin);
}


CharString FASTCALL CharString::TrimLeft(void) const {
  size_type Deb;
  size_type Count;


  Deb = find_first_not_of(" \t\r\n");
  if (Deb == string::npos) return *this;
  Count = length() - Deb;
  return substr(Deb, Count);
}


CharString FASTCALL CharString::Trim(void) const {
  return TrimRight().TrimLeft();
}

int FASTCALL CharString::LastDelimiter(const CharString& delimiters) const {
  CharString csRet;
  return (int) find_last_of(delimiters) + 1;
}

int FASTCALL CharString::AnsiCompareIC(const CharString& rhs) const {
  return lstrcmpiA(c_str(), rhs);
}

int FASTCALL CharString::AnsiPos(const CharString& subStr) {
  return (int) find(subStr) + 1;
}

int FASTCALL CharString::Pos(const CharString& subStr) const {
  return (int) find(subStr) + 1;
}

CharString FASTCALL CharString::StringOfChar(char ch, int count) {
  CharString csRetour;
  for (int i = 0; i < count; i++) csRetour += ch;

  return csRetour;
}

int FASTCALL CharString::ToInt(void) const {
  return atoi(c_str());
}

int FASTCALL CharString::ToIntDef(int Default) const {
  return atoi(c_str());
}

LongLong FASTCALL CharString::ToLongLong(void) const {
  return _atoi64(c_str());
}

double FASTCALL CharString::ToDouble(void) const {
  return atof(c_str());
}

//---------------------------------------------------------------------------
int FASTCALL CharString::CharCharBufSize(void) const {
  return (int) (length() + 1) * sizeof(char);
}

char* FASTCALL CharString::CharChar(char* dest, int DestSize) const {
  lstrcpynA(dest, c_str(), DestSize);

	return dest;
}

//---------------------------------------------------------------------------
int FASTCALL CharString::WideCharBufSize(void) const {
  return (int) (length() + 1) * sizeof(wchar_t);
}

wchar_t* FASTCALL CharString::WideChar(wchar_t* dest, int DestSize) const {
  int i;
  for (i = 0; i < (int) length(); i++) {
    if (i >= DestSize - 1) break;
    dest[i] = (wchar_t) (BYTE) string::operator[] (i);
  }
  dest[i] = '\0';

  return dest;
}

//---------------------------------------------------------------------------
CharString FASTCALL CharString::AsCharString(void) const {
	return *this;
}

//---------------------------------------------------------------------------
WideString FASTCALL CharString::AsWideString(void) const {
	return WideString(*this);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL CharString::AsAnsiString(void) const {
#ifdef UNICODE
	return WideString(*this);
#else
	return *this;
#endif  // UNICODE
}

//---------------------------------------------------------------------------

