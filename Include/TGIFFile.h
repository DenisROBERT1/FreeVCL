//-----------------------------------------------------------------------------
//! @file TGIFFile.h
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

#ifdef In_TGIFFileH
#error "Inclusion circulaire de TGIFFile"
#endif // In_TGIFFileH

#define In_TGIFFileH

#ifndef TGIFFileH
#define TGIFFileH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

#pragma pack(push, 1)

typedef struct {
	char Signature[6];
	unsigned short Width;
	unsigned short Height;
	BYTE Flags;
	BYTE BgColorIndex;
	BYTE PixelAspectRatio;
} HEADERGIFFILE, *LPHEADERGIFFILE;

typedef struct {
	unsigned short Left;
	unsigned short Top;
	unsigned short Width;
	unsigned short Height;
	BYTE Flags;
} GIFIMAGEBLOCK, *LPGIFIMAGEBLOCK;

typedef struct {
	BYTE BlockSize;
	BYTE PackedFields;
	unsigned short DelayTime;
	BYTE TranspColorIndex;
	BYTE BlockTerminator;
} GIFEXTENSIONBLOCK, *LPGIFEXTENSIONBLOCK;

typedef struct {
	BYTE BlockSize;
	BYTE AppIdentifier[8];
	BYTE AppAuthCode[3];
} GIFAPPLICATIONBLOCK, *LPAPPLICATIONBLOCK;

#pragma pack(pop)


//-----------------------------------------------------------------------------
//! @class TGIFFile
//!
//! @brief Définition de la classe TGIFFile
//!
//! Cette classe encapsule un fichier d'image GIF. Elle permet la lecture et
//! l'écriture d'un fichier.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TGIFFile {
private:
	HEADERGIFFILE Header;
	GIFIMAGEBLOCK ImageBlock;
	int NbColors;
	COLORREF *cr;
	BYTE *PixelBits;
	int SizePixelBits;
	int NbBitmap;

	bool LZWEncode(const BYTE *Bits, int LenBits, int NbBits, BYTE *CompBits, int MaxLenCompBits, int *LenCompBits);
	bool LZWDecode(const BYTE *CompBits, int LenCompBits, int NbBits, BYTE *Bits, int MaxLenBits, int *LenBits);
	bool WriteCode(BYTE *Buf, int MaxLenBuf, int *LenBuf, int Code, BYTE *ByteWrite, int *NbBitsWrite, int NbBits);
	bool ReadCode(const BYTE *Buf, int MaxLenBuf, int *LenBuf, int *Code, int *NbBitsRead, int NbBits);
	CharString ReadBlock(HANDLE hFile);
	bool WriteBlock(HANDLE hFile, CharString str);
	COLORREF GetPixel(int x, int y);

protected:
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TGIFFile(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TGIFFile(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un PNG à partir d'un flux
  //!
  //! Cette méthode permet de lire une image à partir d'un flux
  //!
  //! @param[in]        Stream Adresse du flux à lire
  //! @param[in]        ListBitmaps Liste des bitmaps du fichier GIF
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL LoadFromStream(TStream *Stream,
                 std::vector<TBitmap *> &ListBitmaps,
								 TCHAR *szMsgErreur);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un fichier GIF
  //!
  //! Cette méthode permet de lire un fichier GIF
  //!
  //! @param[in]        szFileName Nom du fichier à lire
  //! @param[in]        ListBitmaps Liste des bitmaps du fichier GIF
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL LoadFromFile(const TCHAR * szFileName,
                std::vector<TBitmap *> &ListBitmaps,
								TCHAR *szMsgErreur);


  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un PNG dans un flux
  //!
  //! Cette méthode permet d'enregistrer une image dans un flux
  //!
  //! @param[in]        Stream Adresse du flux à enregistrer
  //! @param[in]        ListBitmaps Liste des bitmaps du fichier GIF
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	BOOL SaveToStream(TStream *Stream,
                 std::vector<TBitmap *> &ListBitmaps,
								 TCHAR *szMsgErreur);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un fichier GIF
  //!
  //! Cette méthode permet d'enregistrer un fichier GIF
  //!
  //! @param[in]        szFileName Nom du fichier à enregistrer
  //! @param[in]        ListBitmaps Liste des bitmaps incluses dans le fichier GIF
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL SaveToFile(const TCHAR * szFileName,
                 std::vector<TBitmap *> &ListBitmaps,
                 TCHAR * szMsgErreur);


  //! @name Propriétés
  //@{

  //@}

};



#else  // TGIFFileH

class TGIFFile;

#endif  // TGIFFileH


#undef In_TGIFFileH
