//-----------------------------------------------------------------------------
//! @file TClientSocket.h
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

#ifdef In_TClientSocketH
#error "Inclusion circulaire de TClientSocket"
#endif // In_TClientSocketH

#define In_TClientSocketH

#ifndef TClientSocketH
#define TClientSocketH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <TCustomSocket.h>

enum TClientType {
	ctNonBlocking,                //!< Communication non bloquante
	ctBlocking                    //!< Communication bloquante
};

#define UM_MESSAGE   (WM_USER + 10)


//-----------------------------------------------------------------------------
//! @class TClientSocket
//!
//! @brief Définition de la classe TClientSocket
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur le
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TClientSocket: public TCustomSocket {
private:

protected:
  TClientType FClientType;
  AnsiString FAddress;
  AnsiString FHost;

  virtual bool Activate(void);
  virtual bool Desactivate(void);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TClientSocket(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TClientSocket();

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ClientType: mode bloquant ou pas.
	//!
	//! Utilisez cette propriété pour déterminer si la liaison avec le serveur
	//! doit être synchrone ou pas. Si le mode est ctNonBlocking (mode
	//! asynchrone), une opération de lecture ou d'écriture retournera avant que
	//! l'opération soit terminée. Sinon (mode synchrone), ces fonctions
	//! bloqueront le thread en cours jusqu'à ce que l'action soit finie.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TClientSocket, TClientType, ClientType);

  //---------------------------------------------------------------------------
  //! @brief Propriété Address: adresse IPv4 ou IPv6 du serveur
	//!
	//! Cette propriété contient l'adresse IP du serveur, sous la forme d'une
	//! chaîne: quatre nombres décimaux séparés par des points, par exemple
	//! "72.14.205.100" pour une adresse IP v4, ou bien huit nombres hexadécimaux
	//! séparés par des doubles points, par exemple
	//! "2a01:5d8:52eb:be1d:f053:2abf:ef7d:6c89" pour une adresse IP v6.
	//! Cette propriété n'est utilisée que si la propriété TClientSocket::Host
	//! est vide. L'utilisation de la propriété Address est plus rapide que celle
	//! de Host, car cela évite une résolution de nom. En contrepartie, cela
	//! nécessite que le serveur ait une IP fixe (ce qui n'est pas toujours le
	//! cas).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TClientSocket, AnsiString, Address);

  //---------------------------------------------------------------------------
  //! @brief Propriété Host
	//!
	//! Cette propriété contient le nom du serveur, par exemple
	//! "http://freevcl.com". Si cette propriété est renseignée, la propriété
	//! TClientSocket::Address est ignorée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TClientSocket, AnsiString, Host);

  //@}

  //! @name Evènements
  //@{

  //@}

};


//---------------------------------------------------------------------------


#else  // TClientSocketH

class TClientSocket;

#endif  // TClientSocketH


#undef In_TClientSocketH
