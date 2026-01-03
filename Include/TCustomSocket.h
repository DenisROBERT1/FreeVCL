//-----------------------------------------------------------------------------
//! @file TCustomSocket.h
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

#ifdef In_TCustomSocketH
#error "Inclusion circulaire de TCustomSocket"
#endif // In_TCustomSocketH

#define In_TCustomSocketH

#ifndef TCustomSocketH
#define TCustomSocketH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <winsock2.h>

//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

enum TErrorEvent {
	eeGeneral,                     //!< Erreur de type général
	eeSend,                        //!< Erreur lors de l'envoi
	eeReceive,                     //!< Erreur lors de la réception
	eeConnect,                     //!< Erreur lors de la connexion
	eeDisconnect,                  //!< Erreur lors de la déconnexion
	eeAccept                       //!< Erreur lors de l'acceptation de la connexion
};

#define UM_MESSAGE   (WM_USER + 10)
#define TCustomWinSocket TCustomSocket


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TCustomSocket;


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnError.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnError)(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrNum);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnAccept.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnAccept)(TObject *Sender, TCustomWinSocket *Socket);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnConnecting.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnConnecting)(TObject *Sender, TCustomWinSocket *Socket);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnConnect.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnConnect)(TObject *Sender, TCustomWinSocket *Socket);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDisconnect.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDisconnect)(TObject *Sender, TCustomWinSocket *Socket);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnRead.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnRead)(TObject *Sender, TCustomWinSocket *Socket);


//-----------------------------------------------------------------------------
//! @class TCustomSocket
//!
//! @brief Définition de la classe TCustomSocket
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur le
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomSocket: public TComponent {
private:

protected:

  int FPort;
  bool FActive;

  // Socket
  WSADATA WSAData;
  SOCKET Socket;
  SOCKET SocketDistant;
  HWND hWndSocket;

  virtual bool Activate(void) = 0;
  virtual bool Desactivate(void) = 0;

  static LRESULT FAR PASCAL WndSocketProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int InetPton(int Family, const PTSTR pszAddrString, PVOID pAddrBuf);
  void DisplayError(AnsiString asTitle, AnsiString asFunction, int LastError);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomSocket(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomSocket();

  //@}


  //! @name Méthodes
  //@{

  int FASTCALL ReceiveBuf(void *Buf, int Count);
  int FASTCALL SendBuf(const void *Buf, int Count);

	bool FASTCALL Open(void);
	bool FASTCALL Close(void);

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
  //! @brief Propriété Port
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomSocket, int, Port);

  //---------------------------------------------------------------------------
  //! @brief Propriété Active
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomSocket, bool, Active);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnError
	//!
	//! Cet évènement est déclenché lorsqu'une erreur survient. Le composant
	//! étant asynchrone, une erreur peut arriver à n'importe quel moment, et pas
	//! seulement après l'appel d'une fonction (déconnexion, par exemple). Vous
	//! pouvez traiter cet évènement pour ignorer certaines erreurs. Dans ce cas
	//! mettez à zéro le numéro d'erreur. Sinon, le traitement normal sera
	//! poursuivi (par défaut affichage d'un message d'erreur).
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Socket à l'origine de l'erreur
  //! @param[in]        Arg3 Type général de l'erreur
  //! @param[in, out]   Arg4 Numéro d'erreur
  //---------------------------------------------------------------------------

  DECLARE_EVENT_4(TOnError, OnError, TObject*, TCustomWinSocket *, TErrorEvent, int &);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnAccept
	//!
	//! Cet évènement est déclenché lorsque un poste distant se connecte et que
	//! la connexion est acceptée.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Objet TCustomWinSocket ayant accepté la connexion
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnAccept, OnAccept, TObject*, TCustomWinSocket *);


  //---------------------------------------------------------------------------
  //! @brief Propriété OnConnecting
	//!
	//! Cet évènement est déclenché avant une connexion
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Objet TCustomWinSocket en train de se connecter
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnConnecting, OnConnecting, TObject*, TCustomWinSocket *);


  //---------------------------------------------------------------------------
  //! @brief Propriété OnConnect
	//!
	//! Cet évènement est déclenché juste après une connexion
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Objet TCustomWinSocket connecté
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnConnect, OnConnect, TObject*, TCustomWinSocket *);


  //---------------------------------------------------------------------------
  //! @brief Propriété OnDisconnect
	//!
	//! Cet évènement est déclenché juste après une déconnexion
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Objet TCustomWinSocket déconnecté
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnDisconnect, OnDisconnect, TObject*, TCustomWinSocket *);


  //---------------------------------------------------------------------------
  //! @brief Propriété OnRead
	//!
	//! Cet évènement est déclenché lorsque le poste distant envoie des
	//! informations.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Objet TCustomWinSocket ayant reçu les informations
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnRead, OnRead, TObject*, TCustomWinSocket *);


  //@}

};


//---------------------------------------------------------------------------


#else  // TCustomSocketH

class TCustomSocket;

#endif  // TCustomSocketH


#undef In_TCustomSocketH
