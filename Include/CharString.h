//-----------------------------------------------------------------------------
//! @file CharString.h
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


//-----------------------------------------------------------------------------
// Dispositif anti-redéfinition
//-----------------------------------------------------------------------------

#ifdef In_CharStringH
#error "Inclusion circulaire de CharString"
#endif // In_CharStringH

#define In_CharStringH

#ifndef CharStringH
#define CharStringH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>

#ifndef PropertyH
#include "Property.h"
#endif  // PropertyH


//---------------------------------------------------------------------------
//
//                         déclarations anticipées
//
//---------------------------------------------------------------------------

class TColor;
class CharString;
class WideString;


//---------------------------------------------------------------------------
//
//                         fonctions globales
//
//---------------------------------------------------------------------------

#ifdef __GNUG__

// Fonctions non définies dans Code::blocks

/* 05/2020 Maintenant défini dans Code::blocks ?
char * _itoa(int i, char * a, int b);
__int64 _atoi64(const char *);
*/

#endif	// __GNUG__


extern CharString FASTCALL IntToStrA(int Value);
extern int FASTCALL StrToIntA(const CharString &src);
extern int FASTCALL StrToIntDefA(const CharString &src, int Default);

extern bool FASTCALL HexToByteA(char c, BYTE * q);

extern CharString FASTCALL Utf8ToAnsi(const char *szSrc);
extern CharString FASTCALL AnsiToUtf8(const CharString &csSrc);


//---------------------------------------------------------------------------
//! @brief Conversion d'une chaîne hexa en binaire
//!
//! Cette fonction convertit une chaîne hexadécimale en binaire. La chaîne
//! peut être en majuscules ou en minuscules. Chaque groupe de deux chiffres
//! valides est converti en un octet. S'ils sont en nombre impair, le chiffre
//! restant est ignoré.
//!
//! @param[out]       Text chaîne héxadécimale
//! @param[out]       Buffer Chaîne convertie
//! @param[in]        BufSize Taille du paramètre Text en octets
//!
//! @return @c int Nombre de caractères non traduits
//---------------------------------------------------------------------------

extern int FASTCALL HexToBinA(const char *Text, void *Buffer, int BufSize);

//---------------------------------------------------------------------------
//! @brief Conversion d'une zone binaire en chaîne hexa.
//!
//! Cette fonction convertit une zone binaire en hexadécimal. Chaque octet est
//! traduit par deux chiffres sans espaces. La chaîne Text doit donc être de
//! longueur minimum 2 * BufSize caractères + 1 caractère pour le zéro terminal.
//!
//! @param[out]       Buffer Zone mémoire à convertir
//! @param[out]       Text Chaîne hexa convertie
//! @param[in]        BufSize Taille de la zone mémoire à convertir.
//---------------------------------------------------------------------------

extern void FASTCALL BinToHexA(const void *Buffer, char *Text, int BufSize);

//---------------------------------------------------------------------------
//! @brief Conversion d'une chaîne hexa en entier
//!
//! Cette fonction convertit une chaîne hexadécimale en entier. La chaîne
//! peut être en majuscules ou en minuscules. Chaque groupe de deux chiffres
//! valides est converti en un octet. S'ils sont en nombre impair, le chiffre
//! restant est ignoré.
//!
//! @param[out]       Text chaîne héxadécimale
//! @param[out]       Buffer Adresse de l'entier converti
//! @param[in]        BufSize Taille de l'entier en octets (en général 2, 4 ou 8)
//!
//! @return @c int Nombre de caractères non traduits
//---------------------------------------------------------------------------

extern int FASTCALL HexToIntA(const char *Text, void *Buffer, int BufSize);

//---------------------------------------------------------------------------
//! @brief Conversion d'un entier en chaîne hexa.
//!
//! Cette fonction convertit un entier en hexadécimal. Chaque octet est
//! traduit par deux chiffres sans espaces. La chaîne Text doit donc être de
//! longueur minimum 2 * BufSize caractères + caractère pour le zéro terminal.
//! Les zéros non significatifs ne sont pas convertis.
//!
//! @param[out]       Buffer Adresse de l'entier à convertir
//! @param[out]       Text Chaîne hexa convertie
//! @param[in]        BufSize Taille de la zone mémoire à convertir.
//---------------------------------------------------------------------------

extern void FASTCALL IntToHexA(const void *Buffer, char *Text, int BufSize);

//-----------------------------------------------------------------------------
//! @class CharString
//!
//! @brief Définition de la classe CharString
//!
//! Cette classe représente les chaînes de caractères sur un octet. C'est en
//! fait la classe AnsiString lorsque le flag UNICODE n'est pas activé, et elle
//! a les mêmes méthodes et propriétés.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class CharString: public std::string {
private:
	char *ReAllocBuf(char *szBuf, int *SizeBuf, int NewSizeBuf);
public:

  //! @name Constructeurs
  //@{

  CharString(void);
  CharString(const char* src);
  CharString(const wchar_t* src);
  CharString(const CharString& src);
  CharString(const WideString& src);
  CharString(const char c, unsigned int len);
  CharString(const char* src, unsigned int len);
  CharString(int src);
  CharString(double src);
  CharString(char src);
  CharString(short i);
  CharString(unsigned short i);
  CharString(unsigned int i);
  CharString(long i);
  CharString(unsigned long i);
  CharString(__int64 i);
  CharString(unsigned __int64 i);
  CharString(const std::string s);

  //@}

  //! @name Opérateurs
  //@{

  CharString& FASTCALL operator = (int i);
  CharString& FASTCALL operator = (unsigned int i);
  CharString& FASTCALL operator = (const char c);
  CharString& FASTCALL operator = (const char* rhs);
  CharString& FASTCALL operator = (const std::string& rhs);
  CharString& FASTCALL operator = (const CharString& rhs);

	CharString& FASTCALL operator = (const wchar_t c);
  CharString& FASTCALL operator = (const wchar_t* rhs);
  CharString& FASTCALL operator = (const std::wstring& rhs);
  CharString& FASTCALL operator = (const WideString& rhs);

  char& FASTCALL operator [](int idx);
  const char& FASTCALL operator [](int idx) const;
	operator const char *() const;

  //@}

  //! @name Méthodes
  //@{

  int __cdecl printf(const char* format, ...);
  CharString& __cdecl sprintf(const char* format, ...);

  bool FASTCALL IsEmpty(void) const;
  int FASTCALL Length(void) const;
  CharString FASTCALL SubString(int index, int count) const;
  CharString& FASTCALL Insert(const CharString& str, int index);
  CharString& FASTCALL Delete(int index, int count);
  CharString FASTCALL LowerCase(void) const;
  CharString FASTCALL UpperCase(void) const;
  CharString FASTCALL ToOEM(void) const;
  CharString FASTCALL ToAnsi(void) const;
  CharString FASTCALL TrimRight(void) const;
  CharString FASTCALL TrimLeft(void) const;
  CharString FASTCALL Trim(void) const;
  int FASTCALL LastDelimiter(const CharString& delimiters) const;
  int FASTCALL AnsiCompareIC(const CharString& rhs) const;
  int FASTCALL AnsiPos(const CharString& subStr);
  int FASTCALL Pos(const CharString& subStr) const;
  static CharString FASTCALL StringOfChar(char ch, int count);
  int FASTCALL ToInt(void) const;
  int FASTCALL ToIntDef(int Default) const;
  LongLong FASTCALL ToLongLong(void) const;
  double FASTCALL ToDouble(void) const;

	//---------------------------------------------------------------------------
  //! @brief Renvoie la taille du tampon nécessaire à la fonction CharChar.
  //!
  //! Cette méthode renvoie la taille du tampon requis par la méthode CharChar
	//! pour convertir cette chaîne.
  //!
  //! @return @c int Taille du tampon (zéro terminal compris).
  //---------------------------------------------------------------------------

	int FASTCALL CharCharBufSize(void) const;

	//---------------------------------------------------------------------------
  //! @brief Convertit l'objet CharString en tableau de caractères Ansi.
  //!
  //! Cette méthode convertit la chaîne en tableau de caractères sur un octet
	//! et renvoie le tableau de caractères résultant. Utilisez la méthode
	//! CharCharBufSize pour déterminer la taille nécessaire.
	//!
  //! @param[out]       dest Tampon alloué par l'appelant pour stocker la valeur de retour.
  //! @param[in]        DestSize Taille du tampon.
  //!
  //! @return @c char* Renvoie un pointeur sur dest.
  //---------------------------------------------------------------------------
  char* FASTCALL CharChar(char* dest, int DestSize) const;

  //---------------------------------------------------------------------------
  //! @brief Renvoie la taille du tampon nécessaire à la fonction WideChar.
  //!
  //! Cette méthode renvoie la taille du tampon requis par la méthode WideChar
	//! pour convertir cette chaîne.
  //!
  //! @return @c int Taille du tampon (zéro terminal compris).
  //---------------------------------------------------------------------------
	int FASTCALL WideCharBufSize(void) const;

  //---------------------------------------------------------------------------
  //! @brief Convertit l'objet CharString en tableau de caractères UNICODE.
  //!
  //! Cette méthode convertit la chaîne en tableau de caractères larges et
	//! renvoie le tableau de caractères larges résultant. Utilisez la méthode
	//! WideCharBufSize pour déterminer la taille nécessaire.
  //!
  //! @param[out]       dest Tampon alloué par l'appelant pour stocker la valeur de retour.
  //! @param[in]        DestSize Taille du tampon.
  //!
  //! @return @c wchar_t* Renvoie un pointeur sur dest.
  //---------------------------------------------------------------------------
	wchar_t* FASTCALL WideChar(wchar_t* dest, int DestSize) const;

  //---------------------------------------------------------------------------
  //! @brief Conversion de la chaîne en chaîne ANSI.
  //!
  //! Cette méthode convertit la chaîne en CharString (AnsiString ANSI). Si la
	//! chaîne de départ contient des caractères non transformables en octets,
	//! cette fonction les tronque ce qui entraîne dans ce cas une perte de
	//! données.
  //!
  //! @return @c CharString Chaîne ANSI.
  //---------------------------------------------------------------------------
	CharString FASTCALL AsCharString(void) const;

  //---------------------------------------------------------------------------
  //! @brief Conversion de la chaîne en chaine UNICODE.
  //!
  //! Cette méthode convertit la chaîne en WideString (AnsiString UNICODE).
  //!
  //! @return @c WideString Chaîne UNICODE.
  //---------------------------------------------------------------------------
	WideString FASTCALL AsWideString(void) const;

  //---------------------------------------------------------------------------
  //! @brief Conversion de la chaîne en AnsiString.
  //!
  //! Cette méthode convertit la chaîne en AnsiString (UNICODE ou ANSI suivant
	//! les options de compilation).
  //!
  //! @return @c AnsiString Chaîne AnsiString.
  //---------------------------------------------------------------------------
	AnsiString FASTCALL AsAnsiString(void) const;

  //@}

};


#else  // CharStringH

class CharString;

#endif  // CharStringH


#undef In_CharStringH
