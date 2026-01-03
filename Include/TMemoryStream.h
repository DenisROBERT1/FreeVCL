//-----------------------------------------------------------------------------
//! @file TMemoryStream.h
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

#ifdef In_TMemoryStreamH
#error "Inclusion circulaire de TMemoryStream"
#endif // In_TMemoryStreamH

#define In_TMemoryStreamH

#ifndef TMemoryStreamH
#define TMemoryStreamH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TStream.h"


//-----------------------------------------------------------------------------
//! @class TMemoryStream
//!
//! @brief Définition de la classe TMemoryStream
//!
//! Cette classe permet de lire et d'écrire dans un flux à partir ou vers une
//! zone mémoire.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMemoryStream: public TStream {
protected:
	Byte *Data;
	bool bAllocated;
	bool bReadOnly;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TMemoryStream(void);
  TMemoryStream(const void *Buffer, LongLong Count);
  TMemoryStream(void *Buffer, LongLong Count);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TMemoryStream(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Met à zéro le tampon mémoire.
  //!
  //! Cette méthode efface totalement les données du flux mémoire. Les
	//! propriétés Size et Position sont remises à zéro.
  //---------------------------------------------------------------------------

	void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Définit une nouvelle taille du tampon mémoire.
  //!
  //! Cette méthode redimensionne le tampon des données du flux mémoire. Les
	//! données existantes sont effacées, la propriété Position est remise à
	//! zéro.
  //---------------------------------------------------------------------------

	virtual void FASTCALL SetSize(LongLong NewSize);

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


#else  // TMemoryStreamH

class TMemoryStream;

#endif  // TMemoryStreamH


#undef In_TMemoryStreamH
