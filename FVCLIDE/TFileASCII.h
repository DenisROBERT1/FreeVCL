//-----------------------------------------------------------------------------
//! @file TFileASCII.h
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

#ifndef TFileASCIIH
#define TFileASCIIH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <windows.h>


//---------------------------------------------------------------------------
//! @class TFileASCII
//!
//! @brief Lecture/écriture d'un fichier ASCII
//!
//! Cette classe (@c TFileASCII) Permet de lire et d'enregistrer un fichier
//! de type texte.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TFileASCII {
private:
  HANDLE hFile;
  AnsiString FName;

protected:
  bool FReadOnly;

public:


  //! @name Constructeurs / destructeurs
  //@{


  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFileASCII();


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFileASCII(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ouverture d'un fichier ASCII
  //!
  //! Cette fonction permet d'ouvrir un fichier ASCII
  //!
  //! @param[in]        asFileName Nom du fichier à ouvrir
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool OpenFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Fermeture d'un fichier.
  //!
  //! Cette fonction ferme un fichier précédemment ouvert
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool CloseFile(void);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'une donnée dans un fichier texte (séparateur = CR)
  //!
  //! Cette fonction permet la lecture d'une donnée dans un fichier texte
  //! (séparateur = CR).
  //!
  //! @param[out]       szBuffer Buffer destination
  //! @param[in]        cchMaxBuffer Taille du buffer destination en NOMBRE DE CARACTERES
  //!
  //! @return @c bool false si fin de fichier atteinte ou erreur de lecture
  //---------------------------------------------------------------------------

  bool ReadLine(char *szBuffer, DWORD cchMaxBuffer);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'une donnée dans un fichier texte (séparateur = CR)
  //!
  //! Cette fonction permet la lecture d'une donnée dans un fichier texte
  //! (séparateur = CR).
  //!
  //! @param[out]       szBuffer Buffer destination
  //! @param[in]        cchMaxBuffer Taille du buffer destination en NOMBRE DE CARACTERES
  //!
  //! @return @c bool false si fin de fichier atteinte ou erreur de lecture
  //---------------------------------------------------------------------------

  bool ReadLine(wchar_t *szBuffer, DWORD cchMaxBuffer);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'une donnée dans un fichier texte (séparateur = CR)
  //!
  //! Cette fonction permet la lecture d'une donnée dans un fichier texte
  //! (séparateur = CR).
  //!
  //! @param[out]       csBuffer Buffer destination
  //!
  //! @return @c bool false si fin de fichier atteinte ou erreur de lecture
  //---------------------------------------------------------------------------

  bool ReadLine(CharString *csBuffer);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'une donnée dans un fichier texte (séparateur = CR)
  //!
  //! Cette fonction permet la lecture d'une donnée dans un fichier texte
  //! (séparateur = CR).
  //!
  //! @param[out]       wsBuffer Buffer destination
  //!
  //! @return @c bool false si fin de fichier atteinte ou erreur de lecture
  //---------------------------------------------------------------------------

  bool ReadLine(WideString *wsBuffer);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une donnée dans un fichier texte (séparateur = CR LF)
  //!
  //! Cette fonction permet l'écriture d'une donnée dans un fichier texte
  //! (séparateur = CR LF)
  //!
  //! @param[in]        szBuffer ligne à enregistrer
  //! @param[in]        cbBuffer Longueur du buffer en BYTES
  //!
  //! @return @c bool false si erreur d'écriture
  //---------------------------------------------------------------------------

  bool WriteLine(const char * szBuffer, DWORD cbBuffer);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une donnée dans un fichier texte (séparateur = CR LF)
  //!
  //! Cette fonction permet l'écriture d'une donnée dans un fichier texte
  //! (séparateur = CR LF)
  //!
  //! @param[in]        asBuffer ligne à enregistrer
  //!
  //! @return @c bool false si erreur d'écriture
  //---------------------------------------------------------------------------

  bool WriteLine(AnsiString asBuffer);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ReadOnly
	//!
	//! Cette propriété indique si le fichier est en lecture seule.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFileASCII, bool, ReadOnly);

  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété indique le nom du fichier ouvert.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFileASCII, AnsiString, Name);

  //@}
};

//---------------------------------------------------------------------------
#endif

