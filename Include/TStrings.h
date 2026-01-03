//-----------------------------------------------------------------------------
//! @file TStrings.h
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

#ifdef In_TStringsH
#error "Inclusion circulaire de TStrings"
#endif // In_TStringsH

#define In_TStringsH

#ifndef TStringsH
#define TStringsH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TStream;


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define SIZE_BUF 1024


//-----------------------------------------------------------------------------
//! @class TStrings
//!
//! @brief Définition de la classe TStrings
//!
//! Cette classe est la classe de base de tous les objets pouvant gérer une
//! liste de chaînes de caractères. Elle prend en charge l'ajout, la
//! suppression des chaînes de caractères, la recherche, la lecture et
//! l'enregistrement dans un fichier ou un flux.@n
//! Cette classe est une classe virtuelle pure. Ne pas l'implémenter, utiliser
//! uniquement ses descendants.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TStrings: public TPersistent {
private:
	DWORD SizeBuf;
	DWORD PosBuf;
	char *BufFile;
	bool ReadBuf(HANDLE hFile, void * Buf, int Size);
	bool ReadChar(HANDLE hFile, char * c);

protected:
  int FUpdateCount;
  bool FCaseSensitive;
	bool FUnicode;
	bool FUtf8;
	bool FBigEndian;
	bool FByteOrderMark;
	bool FLinuxEndLine;
	bool FWantLastLine;
	bool bCrLfEndFile;

  //---------------------------------------------------------------------------
  //! @brief Méthode protégée de lecture de l'objet associé
  //!
  //! Cette méthode est la méthode virtuelle appelée lorsqu'on lit la propriété
	//! Objects. Dans TStrings, elle renvoie toujours NULL. Elle est redéfinie
	//! dans les objets dérivés qui supportent l'affectation d'objets.
  //!
  //! @param[in]        Index Index de la chaîne associée à l'objet
  //!
  //! @return @c TObject * Objet associé
  //---------------------------------------------------------------------------
	virtual TObject *FASTCALL GetObject(int Index) {return NULL;}

	//---------------------------------------------------------------------------
  //! @brief Méthode protégée d'affectation d'un objet
  //!
  //! Cette méthode est la méthode virtuelle appelée lorsqu'on affecte la
	//! propriété Objects. Dans TStrings, elle ne fait rien. Elle est redéfinie
	//! dans les objets dérivés qui supportent l'affectation d'objets.
  //!
  //! @param[in]        Index Index de la chaîne
  //! @param[out]       Object Objet à associer à la chaîne
  //!
  //! @warning L'objet TStrings ou ses descendants ne deviennent pas
	//! propriétaires des objets associés. L'objet appelant est donc responsable
	//! de leur destruction après utilisation pour éviter une fuite mémoire.
  //---------------------------------------------------------------------------
	virtual void FASTCALL PutObject(int Index, TObject *Object) {}

public:

  //! @name Constructeur et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TStrings(void);

	//---------------------------------------------------------------------------
  //! @brief Destructeurs
  //---------------------------------------------------------------------------

	virtual ~TStrings(void);

  //@}


  //! @name Opérateurs
  //@{

  const TStrings & operator =(TStrings &Str);
	bool operator ==(TStrings &Str);
	bool operator !=(TStrings &Str);
  virtual AnsiString operator [](int Index);

  //@}

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture du nombre de chaînes
  //!
  //! Cette méthode permet de lire le nombre d'éléments chaîne dans la liste.
  //!
  //! @return @c int Nombre de chaînes
  //!
  //! @note Par défaut, la méthode Get_Count appelle cette méthode (GetCount).
  //! Il est donc préférable de dériver cette méthode plutôt que Get_Count.
  //---------------------------------------------------------------------------

  virtual int FASTCALL GetCount(void) const = 0;

  //---------------------------------------------------------------------------
  //! @brief Lecture de l'ensemble des chaînes
  //!
  //! Cette fonction retourne l'ensemble des chaînes de la liste dans un buffer
	//! unique. Les différentes chaînes sont séparées par un retour à la ligne
	//! (caractère ASCII 0x10). Le pointeur de chaîne renvoyé en retour est
	//! unique sur l'ensemble du programme, ce qui signifie que cette méthode ne
	//! doit pas être utilisée dans un programme multi-thread, et que même en
	//! mono-thread il est conseillé d'utiliser ou de sauvegarder immédiatement
	//! le résultat pour éviter les effets de bord. Dans tous les cas utilisez
	//! plutôt la méthode @b GetTextStr.
  //!
  //! @return @c char * Pointeur sur la chaîne résultante
  //---------------------------------------------------------------------------
  virtual char * FASTCALL GetText(void);

  //---------------------------------------------------------------------------
  //! @brief Lecture de l'ensemble des chaînes dans une @b AnsiString
  //!
  //! Cette fonction retourne l'ensemble des chaînes de la liste dans une
	//! unique @b AnsiString dans laquelle les différentes chaînes sont séparées
	//! par un retour à la ligne (caractère ASCII 0x10).
  //!
  //! @return @c AnsiString Chaîne résultante
  //---------------------------------------------------------------------------
  virtual AnsiString FASTCALL GetTextStr(void);

  //---------------------------------------------------------------------------
  //! @brief Affectation de la liste des chaînes à partir d'un buffer de @b char
  //!
  //! Cette fonction remplit la liste de chaînes à partir d'un buffer unique
	//! dans lequel les différentes chaînes sont séparées par un retour à la
	//! ligne (caractère ASCII 0x10). La liste de chaînes est mise à zéro avant
	//! le remplissage par le buffer.
  //!
  //! @param[in]        Text Buffer de caractères contenant les chaînes 
  //---------------------------------------------------------------------------
  virtual void FASTCALL SetText(const char * Text);

  //---------------------------------------------------------------------------
  //! @brief Affectation de la liste des chaînes à partir d'un buffer UNICODE
  //!
  //! Cette fonction remplit la liste de chaînes à partir d'un buffer unique
	//! dans lequel les différentes chaînes sont séparées par un retour à la
	//! ligne (caractère ASCII 0x10). La liste de chaînes est mise à zéro avant
	//! le remplissage par le buffer.
  //!
  //! @param[in]        Text Buffer UNICODE contenant les chaînes 
  //---------------------------------------------------------------------------
  virtual void FASTCALL SetText(const wchar_t * Text);

  //---------------------------------------------------------------------------
  //! @brief Affectation de la liste des chaînes à partir d'une @b AnsiString
  //!
  //! Cette fonction remplit la liste de chaînes à partir d'une chaîne unique
	//! dans laquelle les différentes chaînes sont séparées par un retour à la
	//! ligne (caractère ASCII 0x10). La liste de chaînes est mise à zéro avant
	//! le remplissage.
  //!
  //! @param[in]        Value AnsiString contenant les chaînes
  //---------------------------------------------------------------------------
  virtual void FASTCALL SetTextStr(const AnsiString Value);
	
  //---------------------------------------------------------------------------
  //! @brief Début de mise à jour
  //!
  //! Cette méthode permet de modifier la liste sans provoquer une mise à jour
	//! de l'affichage (lorsqu'il y en a un). Ceci permet une meilleure rapidité
	//! lorsqu'il y a un grand nombre de modifications à effectuer. Appelez la
	//! méthode @b EndUpdate en fin de modifications. L'objet comptabilise le
	//! nombre d'appels à @b BeginUpdate, ce qui signifie que vous pouvez appeler
	//! plusieurs fois de suite cette méthode, l'important étant d'appeler la
	//! méthode @b EndUpdate autant de fois. Utilisez la propriété @b UpdateCount
	//! pour connaître le nombre de fois où @b BeginUpdate a été appelé sans
	//! @b EndUpdate.
  //!
	//! @sa EndUpdate, UpdateCount
  //---------------------------------------------------------------------------
  void FASTCALL BeginUpdate(void);
	
  //---------------------------------------------------------------------------
  //! @brief Fin de mise à jour
  //!
  //! Appelez cette méthode après un @b BeginUpdate pour indiquer que les mises
	//! à jour sont terminées. Lorsque la méthode @b EndUpdate est appelée, le
	//! système raffraichit l'affichage (lorsqu'il y en a un). S'il y a plusieurs
	//! @b BeginUpdate imbriqués, l'affichage n'est mis à jour que sur le dernier
	//! appel à @b EndUpdate (c'est à dire lorsque la propriété @b UpdateCount
	//! redevient égale à zéro). C'est pour cela qu'il faut appeler @b EndUpdate
	//! exactement autant de fois que @b BeginUpdate.
  //!
	//! @sa EndUpdate, UpdateCount
  //---------------------------------------------------------------------------
  void FASTCALL EndUpdate(void);

	//---------------------------------------------------------------------------
  //! @brief Définition du flag de mise à jour
  //!
  //! Cette fonction est appelée lorsque le flag de mise à jour change de
	//! valeur (c'est à dire lors du premier appel à BeginUpdate et du dernier
	//! appel à EndUpdate). Dans l'objet @b TStrings, cette fonction ne fait
	//! rien. Les classes dérivées surchargent cette méthode pour effectuer des
	//! actions qui améliorent la rapidité de mise à jour (désactivation et
	//! réactivation de l'affichage en général).
  //!
  //! @param[in]        Updating @b true lorsqu'on passe à l'état "mise à jour". @b false sinon.
  //!
	//! @sa BeginUpdate, EndUpdate
  //---------------------------------------------------------------------------
  virtual void FASTCALL SetUpdateState(bool Updating) {}

  //---------------------------------------------------------------------------
  //! @brief Lecture depuis un fichier
  //!
  //! Cette méthode charge en mémoire le contenu d'un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à lire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  virtual bool FASTCALL LoadFromFile(const AnsiString FileName);

  //---------------------------------------------------------------------------
  //! @brief Lecture depuis un flux
  //!
  //! Cette méthode charge en mémoire le contenu d'un flux
  //!
  //! @param[out]       Stream Flux à lire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool FASTCALL LoadFromStream(TStream* Stream);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un fichier
  //!
  //! Cette méthode enregistre les données dans un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à écrire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool FASTCALL SaveToFile(const AnsiString FileName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un flux
  //!
  //! Cette méthode enregistre les données dans un flux.
  //!
  //! @param[out]       Stream Flux dans lequel écrire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool FASTCALL SaveToStream(TStream* Stream);

  //---------------------------------------------------------------------------
  //! @brief Effacement complet de la liste
  //!
  //! Cette méthode met à zéro la liste.
  //---------------------------------------------------------------------------
  virtual void FASTCALL Clear(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Ajout d'une chaîne à la fin 
  //!
  //! Cette méthode ajoute une chaîne de caractères en fin de liste.
  //!
  //! @param[in]        S Chaîne de caractères à ajouter
  //!
  //! @return @c int Position de la nouvelle chaîne (premier = 0)
  //---------------------------------------------------------------------------
	virtual int FASTCALL Add(const AnsiString S) = 0;

  //---------------------------------------------------------------------------
  //! @brief Insertion d'une chaîne dans la liste
  //!
  //! Cette méthode insère une chaîne dans la liste de chaînes.
  //!
  //! @param[in]        Index Position d'insertion
  //! @param[in]        S Chaîne de caractères à insérer
  //---------------------------------------------------------------------------
	virtual void FASTCALL Insert(int Index, const AnsiString S) = 0;

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une chaîne de la liste
  //!
  //! Cette méthode détruit une chaîne de la liste
  //!
  //! @param[in]        Index Position de la chaîne à détruire
  //---------------------------------------------------------------------------
	virtual void FASTCALL Delete(int Index) = 0;
  
  //---------------------------------------------------------------------------
  //! @brief Copie d'une liste de chaînes
  //!
  //! Cette méthode copie une liste de chaînes de caractères sur une autre, qui
	//! peut être de nature différente à partir du moment où elle hérite de la
	//! classe TStrings.
  //!
  //! @param[out]       Source Liste à copier
  //---------------------------------------------------------------------------
	virtual void FASTCALL Assign(TPersistent* Source);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'une chaîne avec un objet associé 
  //!
  //! Cette méthode ajoute une chaîne à la fin de la liste, avec un objet
	//! associé (a condition que l'objet hérité de TString le supporte). Par
	//! défaut dans TString, le paramètre Object est ignoré.
  //!
  //! @param[in]        S Chaîne à ajouter
  //! @param[out]       Object Objet associé
  //!
  //! @return @c int Position de la nouvelle chaîne (premier = 0)
	//!
	//! @sa Add, PutObject
  //---------------------------------------------------------------------------
	virtual int FASTCALL AddObject(const AnsiString S, TObject* Object);

  //---------------------------------------------------------------------------
  //! @brief Recherche d'une chaîne de caractères dans la liste
  //!
  //! Cette méthode recherche une chaîne de caractères dans la liste. La
	//! propriété CaseSensitive permet de différencier les majuscules des
	//! minuscules.
  //!
  //! @param[in]        S Chaîne à rechercher
  //!
  //! @return @c int Position de la chaîne trouvée, ou -1 si non trouvé.
  //!
  //! @sa CaseSensitive
  //---------------------------------------------------------------------------
	virtual int FASTCALL IndexOf(const AnsiString S);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Strings
	//!
	//! Cette propriété permet de lire la chaîne d'un index donné.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TStrings, AnsiString, Strings);

  //---------------------------------------------------------------------------
  //! @brief Propriété CaseSensitive
	//!
	//! Cette propriété détermine si les recherches sont sensible à la casse
	//! (différencient les majuscules et les minuscules) ou pas. Par défaut elle
	//! est à Faux.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, bool, CaseSensitive);

  //---------------------------------------------------------------------------
  //! @brief Propriété Unicode
	//!
	//! Cette propriété détermine si les chaînes sont en unicode. Cette propriété
	//! n'influe que sur la méthode SaveToFile, car en interne les chaînes ont
	//! le format déterminé par l'option de compilation UNICODE.@n
	//! Lorsque cette propriété est à @b true, la propriété @b Utf8 n'a pas
	//! d'effet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, bool, Unicode);

  //---------------------------------------------------------------------------
  //! @brief Propriété Utf8
	//!
	//! Cette propriété détermine si les chaînes sont en Utf8. Cette propriété
	//! n'influe que sur la méthode SaveToFile, car en interne les chaînes ont
	//! le format ASCII ou UNICODE déterminé par l'option de compilation
	//! UNICODE.@n
	//! Cette propriété n'a d'effet que lorsque la propriété @b Unicode est à
	//! @b false.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, bool, Utf8);

  //---------------------------------------------------------------------------
  //! @brief Propriété BigEndian
	//!
	//! Cette propriété détermine si les caractères sont codés avec la valeur
	//! haute en premier.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, bool, BigEndian);

  //---------------------------------------------------------------------------
  //! @brief Propriété ByteOrderMark
	//!
	//! Cette propriété détermine si le fichier en lecture et en enregistrement
	//! contient un en-tête Byte Order Mark (BOM). 
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, bool, ByteOrderMark);

  //---------------------------------------------------------------------------
  //! @brief Propriété Count: nombre de chaînes
	//!
	//! Cette propriété permet de connaître le nombre de chaînes présentes dans
	//! la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TStrings, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété LinuxEndLine
	//!
	//! Cette propriété détermine si le fichier texte contient des fins de lignes
	//! de type Linux/Unix (\\n soit 0A) ou bien des fins de lignes de type
	//! Windows (\\r\\n soit 0D 0A). Si un même fichier contient les deux types
	//! de fin de lignes, la valeur de LinuxEndLine sera @b false (c'est à dire
	//! que le type Windows sera choisi). 
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, bool, LinuxEndLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété WantLastLine: lecture de la dernière ligne
	//!
	//! Cette propriété détermine si la dernière ligne (située après le dernier
	//! retour charriot) doit être considérée comme une ligne vide ou pas. Par
	//! exemple, si un fichier contient les octets suivants: "54 6F 74 6F 0D 0A",
	//! ça peut être considéré comme une ligne contenant "Toto" (si la propriété
	//! @b WantLastLine est à @b false), ou bien deux ligne, la seconde étant
	//! vide (cas où la propriété est à @b true). La valeur par défaut est
	//! @b false.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, bool, WantLastLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété Text: contenu de la liste
	//!
	//! Cette propriété renvoie l'ensemble des chaînes de la liste en une seule
	//! chaîne globale. Les différentes chaînes sont séparées par le caractère
	//! de retour à la ligne (caractère ASCII 0x10).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStrings, AnsiString, Text);

  //---------------------------------------------------------------------------
  //! @brief Propriété UpdateCount
	//!
	//! Cette propriété indique le nombre de fois où @b BeginUpdate a été appelé
	//! sans appel à @b EndUpdate.
	//!
	//! @sa BeginUpdate, EndUpdate
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TStrings, int, UpdateCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété Objects: objet associé
	//!
	//! Dans l'objet @b TStrings, cette propriété n'a aucun effet, ce qui
	//! signifie que la lecture renvoie NULL, et que l'affectation d'une valeur
	//! ne fait rien. Dans les classes dérivées, cette propriété permet
	//! d'associer un objet quelconque à chaque élément de la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TStrings, TObject *, Objects);

  //@}


};


#else  // TStringsH

class TStrings;

#endif  // TStringsH


#undef In_TStringsH
