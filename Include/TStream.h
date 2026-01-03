//-----------------------------------------------------------------------------
//! @file TStream.h
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

#ifdef In_TStreamH
#error "Inclusion circulaire de TStream"
#endif // In_TStreamH

#define In_TStreamH

#ifndef TStreamH
#define TStreamH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TObject.h"


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TSeekOrigin {
	soBeginning,                  //!< Début de fichier
	soCurrent,                    //!< Position courante
	soEnd                         //!< Fin de fichier
};


//-----------------------------------------------------------------------------
//! @class TStream
//!
//! @brief Définition de la classe TStream
//!
//! Cette classe est la classe de base des objets Flux qui permettent de
//! transférer des informations entre divers objets de types différents sans
//! avoir à se soucier du type de ces objets. Utilisez les classes dérivées
//! pour transférer des données depuis un fichier vers la mémoire, pour
//! transférer des données par socket, par exemple.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TStream: public TObject {
protected:
  LongLong FPosition;
  LongLong FSize;
	int FLastError;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TStream(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TStream(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Copie directe d'un flux vers un autre.
  //!
  //! Cette méthode copie directement Count octets d'un flux vers un autre
  //! flux. Elle met automatiquement à jour la propriété Position.
  //!
  //! @param[in]        Source Flux source
  //! @param[in]        Count Nombre d'octets à copier
  //!
  //! @return @c LongLong Nombre d'octets copiés.
  //---------------------------------------------------------------------------

  LongLong FASTCALL CopyFrom(TStream* Source, LongLong Count);

  //---------------------------------------------------------------------------
  //! @brief Méthode virtuelle de lecture du flux.
  //!
  //! Cette méthode lit Count octets dans le flux. La méthode ReadBuffer
  //! utilise cette méthode.
  //!
  //! Si vous créez une classe dérivée de TStream, il vous faut implémenter
  //! cette méthode virtuelle pure.
  //!
  //! @param[out]       Buffer Buffer de destination
  //! @param[in]        Count Nombre d'octets à lire
  //!
  //! @return @c LongLong Nombre d'octets réellement lus
  //---------------------------------------------------------------------------

  virtual LongLong FASTCALL Read(void *Buffer, LongLong Count) = 0;

  //---------------------------------------------------------------------------
  //! @brief Lit les octets du flux et les place dans Buffer.
  //!
  //! Cette méthode appelle la fonction Read pour remplir un buffer.
  //!
  //! @param[out]       Buffer Buffer de destination
  //! @param[in]        Count Nombre d'octets à lire
  //---------------------------------------------------------------------------

  void FASTCALL ReadBuffer(void *Buffer, LongLong Count);

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
  //! @brief Méthode virtuelle pour l'écriture dans le flux.
  //!
  //! Cette méthode écrit Count octets dans le flux. La méthode WriteBuffer
  //! utilise cette méthode.
  //!
  //! Si vous créez une classe dérivée de TStream, il vous faut implémenter
  //! cette méthode virtuelle pure.
  //!
  //! @param[in]        Buffer Buffer des données à copier.
  //! @param[in]        Count Nombre d'octets à écrire.
  //!
  //! @return @c LongLong Nombre d'octets réellement copiés
  //---------------------------------------------------------------------------

  virtual LongLong FASTCALL Write(const void *Buffer, LongLong Count) = 0;

  //---------------------------------------------------------------------------
  //! @brief Ecriture de données dans le flux.
  //!
  //! Cette méthode écrit Count octets dans le flux.
  //!
  //! @param[in]        Buffer Buffer des données à copier.
  //! @param[in]        Count Nombre d'octets à écrire.
  //!
  //---------------------------------------------------------------------------

  void FASTCALL WriteBuffer(const void *Buffer, LongLong Count);

  /* C'est n'importe nawak, ça: Ces méthodes devraient se trouver dans
     TComponent ou bien dans une classe TStreamComponent héritée de celle-ci,
     mais pas ici.

  //---------------------------------------------------------------------------
  //! @brief Manipule les composants et leurs propriétés avec des flux.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[out]       Instance _INSERER_UN_COMMENTAIRE___
  //!
  //! @return @c TComponent _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  TComponent* FASTCALL ReadComponent(TComponent* Instance);

  //---------------------------------------------------------------------------
  //! @brief Lit des composants et leurs propriétés depuis un flux au format de ressource Windows.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[out]       Instance _INSERER_UN_COMMENTAIRE___
  //!
  //! @return @c TComponent _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  TComponent* FASTCALL ReadComponentRes(TComponent* Instance);

  //---------------------------------------------------------------------------
  //! @brief Lit dans le flux un en-tête de fichier ressource Windows.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[in]        void _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  void FASTCALL ReadResHeader(void);

  //---------------------------------------------------------------------------
  //! @brief Manipule l'écriture des composants et leurs propriétés dans des flux.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[out]       Instance _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  void FASTCALL WriteComponent(TComponent* Instance);

  //---------------------------------------------------------------------------
  //! @brief Ecrit des composants et leurs propriétés dans un flux en utilisant le format de ressource Windows.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[in]        ResName _INSERER_UN_COMMENTAIRE___
  //! @param[out]       Instance _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  void FASTCALL WriteComponentRes(const AnsiString ResName, TComponent* Instance);

  //---------------------------------------------------------------------------
  //! @brief Ecrit des composants et leurs propriétés dans des fiches dérivées.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[out]       Instance _INSERER_UN_COMMENTAIRE___
  //! @param[out]       Ancestor _INSERER_UN_COMMENTAIRE___
  //!
  //! @return @c virtual _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  virtual void FASTCALL WriteDescendent(TComponent* Instance, TComponent* Ancestor);

  //---------------------------------------------------------------------------
  //! @brief Ecrit des composants et leurs propriétés dans des fiches dérivées.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[in]        ResName _INSERER_UN_COMMENTAIRE___
  //! @param[out]       Instance _INSERER_UN_COMMENTAIRE___
  //! @param[out]       Ancestor _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  void FASTCALL WriteDescendentRes(const AnsiString ResName, TComponent* Instance, TComponent* Ancestor);

  //---------------------------------------------------------------------------
  //! @brief Ecrit un en-tête de fichier ressource Windows dans le flux.
  //!
  //! Cette méthode _INSERER_LA_DESCRIPTION___
  //!
  //! @param[in]        ResName _INSERER_UN_COMMENTAIRE___
  //! @param[in]        FixupInfo _INSERER_UN_COMMENTAIRE___
  //---------------------------------------------------------------------------

  void FASTCALL WriteResourceHeader(const AnsiString ResName, int &FixupInfo);

  */

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Position
  //!
  //! Position en cours dans le flux utilisée pour la lecture et l'écriture.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStream, LongLong, Position);


  //---------------------------------------------------------------------------
  //! @brief Propriété Size
  //!
  //! Taille du flux en octets.
	//!
	//! @note Le fait de forcer la valeur de size permet de tronquer un fichier
	//! à une longueur donnée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStream, LongLong, Size);


  //---------------------------------------------------------------------------
  //! @brief Propriété LastError: Dernière erreur
	//!
	//! Cette propriété indique la dernière erreur système survenue dans un
	//! objet. Les valeurs retournées sont les mêmes que la fonction GetLastError
	//! de Windows.
	//!
	//! @note Cette propriété n'est pas "thread safe", c'est à dire qu'il ne faut
	//! pas l'utiliser dans un objet lorsque plusieurs processus peuvent y
	//! accéder simultanément. Mais ce cas est relativement rare pour une
	//! utilisation normale de la VCL.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TStream, int, LastError);

  //@}

};


#else  // TStreamH

class TStream;

#endif  // TStreamH


#undef In_TStreamH
