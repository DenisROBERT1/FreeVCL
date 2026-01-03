//-----------------------------------------------------------------------------
//! @file TCURICOFile.h
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

#ifdef In_TCURICOFileH
#error "Inclusion circulaire de TCURICOFile"
#endif // In_TCURICOFileH

#define In_TCURICOFileH

#ifndef TCURICOFileH
#define TCURICOFileH


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
  short int Data1;
  short int FileType;
  short int NbBlocks;        // Nombre d'en-têtes
} HEADERCURICOFILE, *LPHEADERCURICOFILE;

typedef struct {
  BYTE  bWidth;              // Largeur de l'icône
  BYTE  bHeight;             // Hauteur de l'icône
  BYTE  bColorCount;
  BYTE  bReserved;
	union {
	  WORD  wPlanes;             // Nombre de plans (icône)
	  WORD  wXHotSpot;           // X hotspot (curseur)
	};
	union {
	  WORD  wBitCount;           // Bits par pixel (icône)
	  WORD  wYHotSpot;           // Y hotspot (curseur)
	};
  DWORD dwBytesInRes;        // Taille
  DWORD dwImageOffset;       // Adresse de début
} HEADERCURICO, *LPHEADERCURICO;

#pragma pack(pop)

enum TCURICOType {
	ciNull,                    //!< Type inconnu
	ciICO = 1,                 //!< Icône
	ciCUR = 2                  //!< Curseur
};


//-----------------------------------------------------------------------------
//! @class TCURICOFile
//!
//! @brief Définition de la classe TCURICOFile
//!
//! Cette classe encapsule un fichier d'image curseur ou icône. Elle permet la
//! lecture et l'écriture dans un fichier. Cette classe est dérivée pour chaque
//! type de fichier, icône ou curseur, permettant l'implémentation de
//! propriétés spécifiques à chaque format (hotspot pour les curseurs, nombre
//! de plans pour les icônes).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCURICOFile {
private:
protected:
	TCURICOType FileType;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCURICOFile(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCURICOFile(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un curseur ou d'une icône à partir d'un flux
  //!
  //! Cette méthode permet de lire une image (curseur ou icône) à partir d'un
	//! flux
  //!
  //! @param[in]        Stream Adresse du flux à lire
  //! @param[in]        ListBitmaps Liste des bitmaps incluses dans le fichier
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL LoadFromStream(TStream *Stream,
										std::vector<TBitmap *> &ListBitmaps,
										TCHAR *szMsgErreur);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un fichier CUR ou d'un fichier ICO.
  //!
  //! Cette méthode permet de lire un fichier curseur (.CUR) ou un fichier
	//! icône (.ICO).
  //!
  //! @param[in]        szFileName Nom du fichier à lire
  //! @param[in]        ListBitmaps Liste des bitmaps de le fichier
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL LoadFromFile(const TCHAR * szFileName,
										std::vector<TBitmap *> &ListBitmaps,
										TCHAR *szMsgErreur);


  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un curseur ou d'une icône dans un flux
  //!
  //! Cette méthode permet d'enregistrer une image (curseur ou icône) dans un
	//! flux
  //!
  //! @param[in]        Stream Adresse du flux à enregistrer
  //! @param[in]        ListBitmaps Liste des bitmaps incluses dans le fichier
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	BOOL SaveToStream(TStream *Stream,
										std::vector<TBitmap *> &ListBitmaps,
										TCHAR *szMsgErreur);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'un fichier CUR ou d'un fichier ICO
  //!
  //! Cette méthode permet d'enregistrer un curseur (.CUR) ou une icône (.ICO)
	//! dans un fichier.
  //!
  //! @param[in]        szFileName Nom du fichier à enregistrer
  //! @param[in]        ListBitmaps Liste des bitmaps incluses dans le fichier
  //! @param[out]       szMsgErreur Message d'erreur si retour = false
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  BOOL SaveToFile(const TCHAR * szFileName,
                  std::vector<TBitmap *> &ListBitmaps,
									TCHAR *szMsgErreur);


  //! @name Propriétés
  //@{

  //@}

};



#else  // TCURICOFileH

class TCURICOFile;

#endif  // TCURICOFileH


#undef In_TCURICOFileH
