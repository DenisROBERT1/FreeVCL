//-----------------------------------------------------------------------------
//! @file TFileStream.h
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

#ifdef In_TFileStreamH
#error "Inclusion circulaire de TFileStream"
#endif // In_TFileStreamH

#define In_TFileStreamH

#ifndef TFileStreamH
#define TFileStreamH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TStream.h"


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

static const Word fmOpenRead =				0x0001;
static const Word fmOpenWrite =				0x0002;
static const Word fmOpenReadWrite =		(fmOpenRead | fmOpenWrite);

static const Word fmShareDenyNone =		0x0000;
static const Word fmShareDenyRead =		0x0010;
static const Word fmShareDenyWrite =	0x0020;
static const Word fmShareExclusive =	(fmShareDenyRead | fmShareDenyWrite);
static const Word fmShareCompat =			0x0040;

static const Word fmCreate =					0x0100;


//-----------------------------------------------------------------------------
//! @class TFileStream
//!
//! @brief Définition de la classe TFileStream
//!
//! Cette classe représente un flux venant ou à destination d'un fichier sur
//! disque dur.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFileStream: public TStream {
protected:
	HANDLE hFile;
	Word FMode;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFileStream(const AnsiString asFileName, Word Mode);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFileStream(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Charge la totalité du flux dans le tampon mémoire.
  //!
  //! Cette méthode charge la totalité des données depuis le flux passé en
  //! paramètres. La propriété Position est mise à zéro avant transfert, ce qui
  //! signifie que les données existantes avant l'appel à cette méthode sont
  //! effacées.
  //!
  //! @param[out]       Stream Flux source
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromStream(TStream* Stream);

  //---------------------------------------------------------------------------
  //! @brief Implémentation de la méthode virtuelle de lecture du flux.
  //!
  //! Cette méthode lit Count octets dans le flux. La méthode ReadBuffer
  //! utilise cette méthode.
  //!
  //! @param[out]       Buffer Buffer de destination
  //! @param[in]        Count Nombre d'octets à lire
  //!
  //! @return @c LongLong Nombre d'octets réellement lus
  //---------------------------------------------------------------------------

  virtual LongLong FASTCALL Read(void *Buffer, LongLong Count);

  //---------------------------------------------------------------------------
  //! @brief Se déplace vers une position spécifiée de la ressource de flux.
  //!
  //! Cette méthode permet de se positionner à un endroit précis du flux, en
  //! partant du début (si Origin = soFromBeginning), de la position courante
  //! (si Origin = soFromCurrent), ou de la fin (si Origin = soFromEnd).
  //!
  //! @param[in]        Offset Décalage positif ou négatif.
  //! @param[in]        Origin Origine du décalage.
  //!
  //! @return @c LongLong Nouvelle valeur de la propriété Position.
  //---------------------------------------------------------------------------

  virtual LongLong FASTCALL Seek(LongLong Offset, TSeekOrigin Origin);

  //---------------------------------------------------------------------------
  //! @brief Se déplace vers une position spécifiée de la ressource de flux.
  //!
  //! Cette méthode permet de se positionner à un endroit précis du flux, en
  //! partant du début (si Origin = soFromBeginning), de la position courante
  //! (si Origin = soFromCurrent), ou de la fin (si Origin = soFromEnd).
  //!
  //! @param[in]        Offset Décalage positif ou négatif.
  //! @param[in]        Origin Origine du décalage.
  //!
  //! @return @c int Nouvelle valeur de la propriété Position.
  //---------------------------------------------------------------------------

  virtual int FASTCALL Seek(int Offset, TSeekOrigin Origin);

  //---------------------------------------------------------------------------
  //! @brief Implémentation de la méthode virtuelle pour l'écriture dans le flux.
  //!
  //! Cette méthode écrit Count octets dans le flux. La méthode WriteBuffer
  //! utilise cette méthode.
  //!
  //! @param[in]        Buffer Buffer des données à copier.
  //! @param[in]        Count Nombre d'octets à écrire.
  //!
  //! @return @c LongLong Nombre d'octets réellement copiés
  //---------------------------------------------------------------------------

  virtual LongLong FASTCALL Write(const void *Buffer, LongLong Count);

  //@}

};


#else  // TFileStreamH

class TFileStream;

#endif  // TFileStreamH


#undef In_TFileStreamH
