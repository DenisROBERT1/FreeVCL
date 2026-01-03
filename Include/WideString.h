//-----------------------------------------------------------------------------
//! @file WideString.h
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

#ifdef In_WideStringH
#error "Inclusion circulaire de WideString"
#endif // In_WideStringH

#define In_WideStringH

#ifndef WideStringH
#define WideStringH

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
wchar_t * _itow(int i, wchar_t * a, int b);
int _wtoi(const wchar_t *);
__int64 _wtoi64(const wchar_t *);
*/

#endif	// __GNUG__


extern WideString FASTCALL IntToStrW(int Value);
extern int FASTCALL StrToIntW(const WideString &src);
extern int FASTCALL StrToIntDefW(const WideString &src, int Default);

extern bool FASTCALL HexToByteW(wchar_t c, BYTE * q);

extern WideString FASTCALL Utf8ToUnicode(const char *szSrc);
extern CharString FASTCALL UnicodeToUtf8(const WideString &wsSrc);

extern WideString FASTCALL MultiByteToUnicode(const char *szSrc);
extern CharString FASTCALL UnicodeToMultiByte(const WideString &wsSrc);


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

extern int FASTCALL HexToBinW(const wchar_t *Text, void *Buffer, int BufSize);

//---------------------------------------------------------------------------
//! @brief Conversion d'une zone binaire en chaîne hexa.
//!
//! Cette fonction convertit une zone binaire en hexadécimal. Chaque octet est
//! traduit par deux chiffres sans espaces. La chaîne Text doit donc être de
//! longueur minimum 2 * BufSize caractères + caractère pour le zéro terminal.
//!
//! @param[out]       Buffer Zone mémoire à convertir
//! @param[out]       Text Chaîne hexa convertie
//! @param[in]        BufSize Taille de la zone mémoire à convertir.
//---------------------------------------------------------------------------

extern void FASTCALL BinToHexW(const void *Buffer, wchar_t *Text, int BufSize);


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

extern int FASTCALL HexToIntW(const wchar_t *Text, void *Buffer, int BufSize);

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

extern void FASTCALL IntToHexW(const void *Buffer, wchar_t *Text, int BufSize);

//-----------------------------------------------------------------------------
//! @class WideString
//!
//! @brief Définition de la classe WideString
//!
//! Cette classe représente les chaînes de caractères sur deux octets. C'est en
//! fait la classe AnsiString lorsque le flag UNICODE est activé, et elle a les
//! mêmes méthodes et propriétés.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class WideString: public std::wstring {
private:
	wchar_t *ReAllocBuf(wchar_t *szBuf, int *SizeBuf, int NewSizeBuf);
public:

  //! @name Constructeurs
  //@{

  WideString(void);
  WideString(const wchar_t* src);
  WideString(const char* src);
  WideString(const CharString& src);
  WideString(const WideString& src);
  WideString(const wchar_t c, unsigned int len);
  WideString(const wchar_t* src, unsigned int len);
  WideString(int src);
  WideString(double src);
  WideString(wchar_t src);
  WideString(short i);
  WideString(unsigned int i);
  WideString(long i);
  WideString(unsigned long i);
  WideString(__int64 i);
  WideString(unsigned __int64 i);
  WideString(const std::wstring s);

  //@}

  //! @name Opérateurs
  //@{

  WideString& FASTCALL operator = (int i);
  WideString& FASTCALL operator = (unsigned int i);
  WideString& FASTCALL operator = (const wchar_t c);
  WideString& FASTCALL operator = (const wchar_t* rhs);
  WideString& FASTCALL operator = (const std::wstring& rhs);
  WideString& FASTCALL operator = (const WideString& rhs);

	WideString& FASTCALL operator = (const char c);
  WideString& FASTCALL operator = (const char* rhs);
  WideString& FASTCALL operator = (const std::string& rhs);
  WideString& FASTCALL operator = (const CharString& rhs);

  wchar_t& FASTCALL operator [](int idx);
  const wchar_t& FASTCALL operator [](int idx) const;
	operator const wchar_t *() const;

  //@}

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Formatage d'un texte
  //!
  //! Cette méthode formate une chaîne de caractères à partir d'un format et
	//! d'un nombre variable d'arguments. Elle fonctionne comme la fonction C
	//! printf (même syntaxe).
  //!
  //! @param[in]        format Format de chaîne
  //!
  //! @return @c int Nombre de caractère final de la chaîne
  //---------------------------------------------------------------------------
  int __cdecl printf(const wchar_t* format, ...);

	//---------------------------------------------------------------------------
  //! @brief Formatage d'un texte
  //!
  //! Cette méthode formate une chaîne de caractères à partir d'un format et
	//! d'un nombre variable d'arguments. Elle fonctionne comme la fonction C
	//! sprintf (même syntaxe).
  //!
  //! @param[in]        format Format de chaîne
  //!
  //! @return @c WideString Chaîne formatée
  //---------------------------------------------------------------------------
  WideString& __cdecl sprintf(const wchar_t* format, ...);

  //---------------------------------------------------------------------------
  //! @brief Teste si la chaîne est vide.
  //!
  //! Cette méthode teste si la chaîne est vide. Elle est plus rapide qu'une
	//! comparaison avec une chaîne vide avec l'opérateur "=", ou un test de
	//! la longueur.
  //!
  //! @return @c bool true si vide
  //---------------------------------------------------------------------------
  bool FASTCALL IsEmpty(void) const;

  //---------------------------------------------------------------------------
  //! @brief Longueur de la chaîne
  //!
  //! Cette méthode renvoie la longueur de la chaîne en nombre de caractères.
  //!
  //! @return @c int Longueur
  //---------------------------------------------------------------------------
	int FASTCALL Length(void) const;

  //---------------------------------------------------------------------------
  //! @brief Sous-chaîne
  //!
  //! Cette méthode retourne la sous-chaîne commençant à l'index @b index et de
	//! longueur @b count.
  //!
  //! @param[in]        index Début de la sous-chaîne (premier caractère = 1)
  //! @param[in]        count Nombre de caractères
  //!
  //! @return @c WideString Chaine résultante
  //---------------------------------------------------------------------------
	WideString FASTCALL SubString(int index, int count) const;

  //---------------------------------------------------------------------------
  //! @brief Insertion d'une chaîne
  //!
  //! Cette méthode insère une chaîne.
  //!
  //! @param[in]        str Chaîne à insérer
  //! @param[in]        index Position d'insertion (début = 1)
  //!
  //! @return @c WideString Chaine résultante
  //---------------------------------------------------------------------------
	WideString& FASTCALL Insert(const WideString& str, int index);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une partie de chaîne
  //!
  //! Cette méthode supprime une sous-partie de la chaîne
  //!
  //! @param[in]        index Premier caractère à supprimer (premier = 1)
  //! @param[in]        count Nombre de caractère à supprimer
  //!
  //! @return @c WideString Chaine résultante
  //---------------------------------------------------------------------------
	WideString& FASTCALL Delete(int index, int count);

  //---------------------------------------------------------------------------
  //! @brief Passage en minuscules
  //!
  //! Cette méthode convertit la chaîne en minuscules.
  //!
  //! @return @c WideString Chaine en minuscules
  //---------------------------------------------------------------------------
	WideString FASTCALL LowerCase(void) const;

  //---------------------------------------------------------------------------
  //! @brief Passage en majuscules
  //!
  //! Cette méthode convertit la chaîne en majuscules.
  //!
  //! @return @c WideString Chaine en majuscules
  //---------------------------------------------------------------------------
	WideString FASTCALL UpperCase(void) const;

  //---------------------------------------------------------------------------
  //! @brief Conversion au format OEM
  //!
  //! Cette méthode convertit la chaîne au format OEM.
  //!
  //! @return @c WideString Chaine en format OEM
  //---------------------------------------------------------------------------
	WideString FASTCALL ToOEM(void) const;

  //---------------------------------------------------------------------------
  //! @brief Conversion au format ANSI
  //!
  //! Cette méthode convertit la chaîne au format ANSI.
  //!
  //! @return @c WideString Chaine en format ANSI
  //---------------------------------------------------------------------------
	WideString FASTCALL ToAnsi(void) const;

  //---------------------------------------------------------------------------
  //! @brief Suppression des espaces de droite.
  //!
  //! Cette méthode supprime les espaces, les tabulation et les retours à la
	//! ligne à droite de la chaîne.
  //!
  //! @return @c WideString Chaine résultante
  //---------------------------------------------------------------------------
	WideString FASTCALL TrimRight(void) const;

  //---------------------------------------------------------------------------
  //! @brief Suppression des espaces de gauche.
  //!
  //! Cette méthode supprime les espaces, les tabulation et les retours à la
	//! ligne à gauche de la chaîne.
  //!
  //! @return @c WideString Chaine résultante
  //---------------------------------------------------------------------------
	WideString FASTCALL TrimLeft(void) const;

  //---------------------------------------------------------------------------
  //! @brief Suppression des espaces d'extrémité.
  //!
  //! Cette méthode supprime les espaces, les tabulation et les retours à la
	//! ligne à gauche et à droite de la chaîne.
  //!
  //! @return @c WideString Chaine résultante
  //---------------------------------------------------------------------------
	WideString FASTCALL Trim(void) const;

  //---------------------------------------------------------------------------
  //! @brief Recherche du dernier délimiteur
  //!
  //! Cette méthode recherche un délimiteur à partir de la fin de la chaîne. La
	//! liste des délimiteurs est passée en paramètres.
  //!
  //! @param[in]        delimiters Liste des délimiteurs à rechercher
  //!
  //! @return @c int Position, ou 0 si non trouvé
  //---------------------------------------------------------------------------
	int FASTCALL LastDelimiter(const WideString& delimiters) const;

  //---------------------------------------------------------------------------
  //! @brief Comparaison sans tenir compte des majuscules
  //!
  //! Cette méthode fait une comparaison en considérant comme égales les
	//! lettres majuscules et les lettres minuscules correspondantes.
  //!
  //! @param[in]        rhs Chaîne à comparer
  //!
  //! @return @c int 0 si chaine égales. -1 si la chaîne à comparer est plus
	//! grande (dans l'ordre alphabétique), 1 si elle est plus petite.
  //---------------------------------------------------------------------------
	int FASTCALL AnsiCompareIC(const WideString& rhs) const;

  //---------------------------------------------------------------------------
  //! @brief Recherche d'une sous-chaîne
  //!
  //! Cette méthode recherche la sous-chaîne @b subStr dans la chaîne. Elle
	//! fonctionne comme la méthode @b Pos, mais supporte les caractères multi-
	//! octets.
  //!
  //! @param[in]        subStr Chaîne à rechercher
  //!
  //! @return @c int Position trouvée, ou 0 si non trouvé
  //---------------------------------------------------------------------------
	int FASTCALL AnsiPos(const WideString& subStr);

  //---------------------------------------------------------------------------
  //! @brief Recherche d'une sous-chaîne
  //!
  //! Cette méthode recherche la sous-chaîne @b subStr dans la chaîne.
  //!
  //! @param[in]        subStr Chaîne à rechercher
  //!
  //! @return @c int Position trouvée, ou 0 si non trouvé
  //---------------------------------------------------------------------------
	int FASTCALL Pos(const WideString& subStr) const;

  //---------------------------------------------------------------------------
  //! @brief Initialisation avec une suite de caractères identiques
  //!
  //! Cette méthode initialise la chaîne avec @b count fois le caractère @b ch.
  //!
  //! @param[in]        ch Caractère à répéter
  //! @param[in]        count Nombre de fois
  //!
  //! @return @c static Chaîne résultante.
  //---------------------------------------------------------------------------
	static WideString FASTCALL StringOfChar(wchar_t ch, int count);

  //---------------------------------------------------------------------------
  //! @brief Transformation en entier
  //!
  //! Cette méthode transforme la chaîne en entier, et lève une exception si
	//! la conversion ne peut pas se faire.
  //!
  //! @return @c int Entier converti
  //---------------------------------------------------------------------------
	int FASTCALL ToInt(void) const;

  //---------------------------------------------------------------------------
  //! @brief Transformation en entier
  //!
  //! Cette méthode fonctionne comme la méthode @b ToInt, sauf que dans le cas
	//! où la transformation ne peut pas se faire, elle retourne la valeur par
	//! défaut.
  //!
  //! @param[in]        Default Valeur par défaut
  //!
  //! @return @c int Entier converti ou valeur par défaut
  //---------------------------------------------------------------------------
	int FASTCALL ToIntDef(int Default) const;

  //---------------------------------------------------------------------------
  //! @brief Transformation en entier 64 bits
  //!
  //! Cette méthode transforme la chaîne en entier 64 bits
  //!
  //! @return @c LongLong Entier converti, ou 0 si la conversion ne peut pas
	//! se faire.
  //---------------------------------------------------------------------------
	LongLong FASTCALL ToLongLong(void) const;

	//---------------------------------------------------------------------------
  //! @brief Transformation en réel Double
  //!
  //! Cette méthode transforme la chaîne en réel double précision.
  //!
  //! @return @c double Nombre réel converti, ou 0 si la conversion ne peut pas
	//! se faire.
  //---------------------------------------------------------------------------
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
  //! @brief Convertit l'objet WideString en tableau de caractères Ansi.
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
  //! @brief Convertit l'objet WideString en tableau de caractères UNICODE.
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


#else  // WideStringH

class WideString;

#endif  // WideStringH


#undef In_WideStringH
