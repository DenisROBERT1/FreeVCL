//-----------------------------------------------------------------------------
//! @file TPaletteVCL.h
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

#ifdef In_TPaletteVCLH
#error "Inclusion circulaire de TPaletteVCL"
#endif // In_TPaletteVCLH

#define In_TPaletteVCLH

#ifndef TPaletteVCLH
#define TPaletteVCLH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH

#define PALVERSION   0x300                   // LOGPALETTE's palVersion field


//-----------------------------------------------------------------------------
//! @class TPaletteVCL
//!
//! @brief Définition de la classe TPaletteVCL
//!
//! Cette classe encapsule une palette Windows (HPALETTE).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPaletteVCL {
private:
protected:
  RGBQUAD *bmiColors;
  int FNbColors;
  bool FIsWin30;
  BYTE Byte;
  int NbBits;
  HPALETTE FHandle;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPaletteVCL(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPaletteVCL(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture de la place sur disque de la palette
  //!
  //! Cette méthode permet de lire la place nécessaire sur disque pour mémoriser
  //! la palette.
  //!
  //! @return @c int Nombre d'octets
  //---------------------------------------------------------------------------

  int SizeInFile();


  //---------------------------------------------------------------------------
  //! @brief Lecture d'une palette à partir d'un flux
  //!
  //! Cette méthode permet de lire une palette à partir d'un flux
  //!
  //! @param[in]        Stream Flux à lire
  //!
  //! @return @c int Nombre d'octets lus
  //---------------------------------------------------------------------------

  int ReadFromStream(TStream *Stream);


  //---------------------------------------------------------------------------
  //! @brief Lecture d'une palette
  //!
  //! Cette méthode permet de lire une palette à partir d'un fichier BMP ou ICO
  //!
  //! @param[in]        hFile Handle du fichier à lire
  //!
  //! @return @c int Nombre d'octets lus
  //---------------------------------------------------------------------------

  int ReadFromFile(HANDLE hFile);


  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une palette dans un flux
  //!
  //! Cette méthode permet d'enregistrer la palette dans un flux
  //!
  //! @return @c LongLong Nombre d'octets écrits
  //---------------------------------------------------------------------------

  LongLong WriteToStream(TStream *Stream);


  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une palette dans un fichier
  //!
  //! Cette méthode permet d'enregistrer la palette dans un fichier BMP ou ICO
  //!
  //! @return @c int Nombre d'octets écrits
  //---------------------------------------------------------------------------

  int WriteToFile(HANDLE hFile);


  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un pixel
  //!
  //! Cette méthode permet d'enregistrer un pixel en fonction de la palette
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  int WritePixelToFile(HANDLE hFile, COLORREF cr);


  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un pixel dans un stream
  //!
  //! Cette méthode permet d'enregistrer un pixel en fonction de la palette
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  int WritePixelToStream(TStream *Stream, COLORREF cr);


  //---------------------------------------------------------------------------
  //! @brief Création d'un handle de palette
  //!
  //! Cette méthode permet de créer une palette à partir du tableau de couleurs
  //!
  //! @return @c HPALETTE Handle de palette
  //---------------------------------------------------------------------------

  HPALETTE ColorToHandle();

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un handle de palette
  //!
  //! Cette méthode permet de créer un tableau de couleurs à partir d'une
  //! palette
  //!
  //! @param[in]        hPalette Handle de palette
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool HandleToColor(HPALETTE hPalette);

  //---------------------------------------------------------------------------
  //! @brief Adresse d'une structure BITMAPINFO
  //!
  //! Cette méthode permet d'allouer et de renseigner une structure BITMAPINFO
  //! à partir des informations et couleurs de la palette.
  //!
  //! @param[in]        BitCount Nombre de bits par pixels
  //! @param[in]        Width Nombre de pixels en X
  //! @param[in]        Height Nombre de pixels en Y
  //!
  //! @return @c BITMAPINFO * Adresse de la structure BITMAPINFO
  //!
  //! @warning La fonction appelante est responsable de la libération de la
  //! structure retournée lorsque celle-ci n'est plus utilisée (via un delete)
  //---------------------------------------------------------------------------

  BITMAPINFO *GetBitmapInfo(int BitCount, int Width, int Height);

  //---------------------------------------------------------------------------
  //! @brief Affectation d'une palette à une autre
  //!
  //! Cette méthode permet de dupliquer les données d'une palette vers une autre
  //! palette
  //!
  //! @param[in]        PaletteVCL Palette source
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  const TPaletteVCL & operator =(const TPaletteVCL &PaletteVCL);

  //---------------------------------------------------------------------------
  //! @brief Calcul de la taille d'une ligne
  //!
  //! Cette méthode permet de calculer la taille d'une ligne.
  //!
  //! @param[in]        BitCount Nombre de bits par pixels
  //! @param[in]        Width Largeur d'une ligne en pixels
  //!
  //! @return @c int taille d'une ligne en octets
  //---------------------------------------------------------------------------

  int SizeLine(int BitCount, int Width);

  //---------------------------------------------------------------------------
  //! @brief Calcul de la taille d'une image
  //!
  //! Cette méthode permet de calculer la taille d'une image.
  //!
  //! @param[in]        BitCount Nombre de bits par pixels
  //! @param[in]        Width Largeur de l'image en pixels
  //! @param[in]        Height Hauteur de l'image en pixels
  //!
  //! @return @c DWORD taille de l'image en octets
  //---------------------------------------------------------------------------

  DWORD SizeBits(int BitCount, int Width, int Height);

  //---------------------------------------------------------------------------
  //! @brief Calcul du nombre maxi de couleurs de la palette
  //!
  //! Cette méthode permet de calculer le nombre maxi de couleurs de la palette
  //! en fonction du nombre de bits par pixels.
  //!
  //! @param[in]        BitCount Nombre de bits par pixels
  //!
  //! @return @c int Nombre maxi de couleurs
  //---------------------------------------------------------------------------

  int NbColorsMax(int BitCount);

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: handle Windows (HPALETTE) de la palette
  //!
  //! Utilisez cette propriété pour lire ou pour définir la palette windows
  //! encapsulée dans l'objet. Affecter une nouvelle valeur à cette propriété
  //! peut modifier les autres propriétés. L'objet @b TPalette devient ensuite
  //! propriétaire de l'objet (c'est à dire que l'utilisateur n'a pas et ne
  //! doit pas faire un @b DeleteObject du handle).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPaletteVCL, HPALETTE, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété NbColors: nombre de couleurs
  //!
  //! Utilisez cette propriété pour lire ou pour définir le nombre de couleurs
  //! dans la palette. Attention à ne pas définir un nombre supérieur au nombre
  //! maximum de couleurs de votre image dépendant du nombre de bits par pixel:
  //! les couleurs de la palette seront bien enregistrées et relues, mais elles
  //! ne seront pas écrites dans le fichier de sortie.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPaletteVCL, int, NbColors);

  //---------------------------------------------------------------------------
  //! @brief Propriété IsWin30: Format 3.0
  //!
  //! Cette propriété indique que la palette est au format Windows 3.0 ou
  //! supérieur. La valeur par défaut est true. il est déconseillé de mettre
  //! cette propriété à false, l'image générée risque de ne pas être relue par
  //! tous les logiciels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPaletteVCL, bool, IsWin30);

  //---------------------------------------------------------------------------
  //! @brief Propriété Color: Couleur d'indice i
  //!
  //! Utilisez cette propriété pour lire ou écrire une couleur dans la palette.
  //! Veillez à ne pas définir un indice inférieur à zéro ou supérieur au
  //! nombre de couleurs (l'opération n'aurait aucun effet, ou renverrait la
  //! couleur noir en lecture).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TPaletteVCL, RGBQUAD, Color);

  //@}

};



#else  // TPaletteVCLH

class TPaletteVCL;

#endif  // TPaletteVCLH


#undef In_TPaletteVCLH
