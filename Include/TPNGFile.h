//-----------------------------------------------------------------------------
//! @file TPNGFile.h
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

#ifdef In_TPNGFileH
#error "Inclusion circulaire de TPNGFile"
#endif // In_TPNGFileH

#define In_TPNGFileH

#ifndef TPNGFileH
#define TPNGFileH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH

#ifndef TPaletteVCLH
#include "TPaletteVCL.h"
#endif  // TPaletteVCLH


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

#pragma pack(push, 1)

#pragma pack(pop)


//-----------------------------------------------------------------------------
//! @class TPNGFile
//!
//! @brief Définition de la classe TPNGFile
//!
//! Cette classe encapsule un fichier d'image PNG. Elle permet la lecture,
//! l'écriture et la manipulation directe des bits.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPNGFile {
private:
protected:
  int FWidth;
  int FHeight;
  int FBitCount;
  DWORD Compression;
  LPSTR lpDIBBits;
  TPaletteVCL FPaletteVCL;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPNGFile(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPNGFile(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un PNG à partir d'un flux
  //!
  //! Cette méthode permet de lire une image à partir d'un flux
  //!
  //! @param[in]        Stream Adresse du flux à lire
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL LoadFromStream(TStream *Stream, TCHAR *szMsgErreur);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un PNG à partir d'un fichier
  //!
  //! Cette méthode permet de lire une image à partir d'un fichier
  //!
  //! @param[in]        szFileName Nom du fichier à lire
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL LoadFromFile(const TCHAR *szFileName, TCHAR *szMsgErreur);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un PNG dans un flux
  //!
  //! Cette méthode permet d'enregistrer une image dans un flux
  //!
  //! @param[in]        Stream Adresse du flux à enregistrer
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	BOOL SaveToStream(TStream *Stream,TCHAR *szMsgErreur);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un PNG dans un flux
  //!
  //! Cette méthode permet d'enregistrer une image dans un flux
  //!
  //! @param[in]        szFileName Nom du fichier à enregistrer
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	BOOL SaveToFile(const TCHAR *szFileName, TCHAR *szMsgErreur);

  void GetBits(HDC hdc, RECT &RectDC, RECT &RectImg);

  void GetBits(HDC hdc, HBITMAP hBitmap);

	void GetBits(void *lpBits, DWORD MaxSize);

  void SetBits(HDC hdc, HBITMAP hBitmap);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Width
	//!
	//! Cette propriété définit la largeur en pixels de l'image.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPNGFile, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété définit la hauteur en pixels de l'image. Cette valeur
	//! peut être négative lorsque l'image a été lue depuis un fichier provenant
	//! d'un logiciel ancien. En principe, les logiciels récents devraient créer
	//! des images avec une hauteur positive.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPNGFile, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Propriété BitCount
	//!
	//! Cette propriété définit le nombre de bits nécessaires pour coder un
	//! pixel.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPNGFile, int, BitCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété Palette
	//!
	//! Cette propriété définit la palette système utilisée par l'image. Il se
	//! peut que cette propriété renvoie la valeur NULL dans le cas d'une image
	//! en couleurs réelles.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPNGFile, HPALETTE, Palette);

  //---------------------------------------------------------------------------
  //! @brief Propriété PaletteVCL
	//!
	//! Cette propriété renvoie un objet VCL représentant la palette de l'image.
	//! L'objet est toujours défini, mais le handle de palette système qu'elle
	//! contient peut être NULL.
	//!
	//! @sa Palette
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPNGFile, TPaletteVCL *, PaletteVCL);

  //@}

};



#else  // TPNGFileH

class TPNGFile;

#endif  // TPNGFileH


#undef In_TPNGFileH
