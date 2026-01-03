//-----------------------------------------------------------------------------
//! @file TThread.h
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

#ifdef In_TThreadH
#error "Inclusion circulaire de TThread"
#endif // In_TThreadH

#define In_TThreadH

#ifndef TThreadH
#define TThreadH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TThreadPriority {
	tpIdle,        //!< Priorité minimum (le thread est exécuté uniquement lorsque le système est inactif)
	tpLowest,      //!< Priorité très basse
	tpLower,       //!< Priorité basse
	tpNormal,      //!< Priorité normale
	tpHigher,      //!< Priorité haute
	tpHighest,     //!< Priorité très haute
	tpTimeCritical //!< Priorité maximale (peut empêcher les autres applications de tourner, voire bloquer le système)
};


//-----------------------------------------------------------------------------
//! @class TThread
//!
//! @brief Définition de la classe TThread
//!
//! Cette classe définit une temporisation. Elle permet de recevoir à
//! intervalle régulier un évènement que vous pourrez traiter pour effectuer un
//! traitement particulier.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TThread: public TObject {
private:

protected:
	TObject *FFatalException;
	bool FFreeOnTerminate;
	HANDLE FHandle;
  TThreadPriority FPriority;
  int FReturnValue;
  bool FSuspended;
  bool FTerminated;
  int FThreadID;

  static DWORD WINAPI ThreadProc(LPVOID lpParam);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TThread(bool CreateSuspended);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TThread(void);

  //@}


  //! @name Méthodes
  //@{

	//---------------------------------------------------------------------------
	//! @brief Lancement automatique d'un thread
	//!
	//! Cette méthode lance la méthode @b Execute lorsque le thread n'a pas été
	//! créé avec le flag CreateSuspended. Cela évite qu'un thread se lance et se
	//! termine avant la sortie du constructeur de l'objet @b TThread.
	//---------------------------------------------------------------------------

	virtual void FASTCALL AfterConstruction(void);

	//---------------------------------------------------------------------------
	//! @brief Génère un évènement OnTerminate
	//!
	//! Cette méthode déclenche un évènement @b OnTerminate, mais n'interromp pas
	//! le thread lui-même.
	//---------------------------------------------------------------------------

	virtual void FASTCALL DoTerminate(void);

	//---------------------------------------------------------------------------
	//! @brief Exécution d'un thread
	//!
	//! Cette méthode virtuelle pure doit être redéfinie dans les objets dérivés.
	//! Elle contient le code à exécuter en parallèle. Ce code est lancé juste
	//! après la création si le flag @b CreateSuspended est à false, ou après
	//! l'appel de la méthode @b Resume dans le cas contraire.
	//!
	//! @note N'utilisez pas les propriétés et méthodes des autres objets dans
	//! cette fonction, le fonctionnement en multi-thread pourrait provoquer des
	//! comportements aléatoires. Créez une méthode séparée et utilisez cette
	//! méthode au travers de la méthode @b Synchronize.
	//!
	//! @sa Synchronize
	//---------------------------------------------------------------------------

	virtual void FASTCALL Execute(void) = 0;

	//---------------------------------------------------------------------------
	//! @brief Redémarre un thread suspendu ou pas encore démarré
	//!
	//! Cette méthode redémarre un thread suspendu avec la méthode @b Suspend ou
	//! bien créé avec le flag @b CreateSuspended à @b true. Si plusieurs appels
	//! à @b Suspend ont été faits, il faut appeler autant de fois cette méthode
	//! pour redémarrer le thread.
	//!
	//! @sa Suspend
	//---------------------------------------------------------------------------

	void FASTCALL Resume(void);

	//---------------------------------------------------------------------------
	//! @brief Arrête temporairement un thread.
	//!
	//! Cette méthode arrête temporairement un thread. Si vous appelez plusieurs
	//! fois cette méthode, vous devrez appeler autant de fois la méthode
	//! @b Resume pour redémarrer le thread.
	//!
	//! @sa Resume
	//---------------------------------------------------------------------------

	void FASTCALL Suspend(void);

	//---------------------------------------------------------------------------
	//! @brief Appel d'une fonction du thread principal.
	//!
	//! Cette méthode permet d'appeler une méthode du thread principal, sans
	//! risquer les conflits dûs au traitement multi-thread.
	//!
	//! @param[in]      Method Méthode à appeler
	//!
	//! @warning Ne pas appeler cette méthode depuis le thread principal, ceci
	//! pourrait créer un "dead lock" et donc bloquer l'application.
	//---------------------------------------------------------------------------

	typedef void FASTCALL (*TThreadMethod)(void);

	void FASTCALL Synchronize(TThreadMethod &Method);

	//---------------------------------------------------------------------------
	//! @brief Met la propriété @b Terminated à @b true.
	//!
	//! Cette méthode positionne la propriété @b Terminated à @b true, mais ne
	//! termine pas le thread lui-même. Il est de la responsabilité du
	//! programmeur de la fonction @b Execute de tester aussi souvent que
	//! possible l'état de la propriété @b Terminated, et de sortir proprement de
	//! la fonction si cette propriété est à true.
	//!
	//! @sa Terminated
	//---------------------------------------------------------------------------

	void FASTCALL Terminate(void);

	//---------------------------------------------------------------------------
	//! @brief Attend l'arrêt du thread et retourne la valeur @b ReturnValue.
	//!
	//! Cette méthode attend l'arrêt réel du thread (c'est à dire pas seulement
	//! le positionnement de la propriété @b Terminated), et retourne la valeur
	//! de retour définie dans la propriété @b ReturnValue.
	//!
	//! @sa ReturnValue
	//---------------------------------------------------------------------------

	int FASTCALL WaitFor(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
	//! @brief Propriété FatalException : Exception fatale déclenchée dans le thread
	//!
	//! Cette propriété retourne l'exception déclenchée dans le thread exécuté,
	//! lorsqu'une une erreur fatale apparait.
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TThread, TObject *, FatalException);

  //---------------------------------------------------------------------------
	//! @brief Propriété FreeOnTerminate : Libération automatique de la mémoire
	//!
	//! Cette propriété, lorsqu'elle est à true, permet de libérer
	//! automatiquement la mémoire quand le thread se termine. Sinon, vous devez
	//! explicitement libérer le thread avec l'instruction @b CloseHandle.
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY(TThread, bool, FreeOnTerminate);

  //---------------------------------------------------------------------------
	//! @brief Propriété Handle : Handle du thread
	//!
	//! Cette propriété retourne le handle système du thread. Vous pouvez
	//! l'utiliser pour appeler les fonctions système qui nécessitent un handle
	//! de thread.
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TThread, HANDLE, Handle);

  //---------------------------------------------------------------------------
	//! @brief Propriété Priority : Priorité du thread
	//!
	//! Cette propriété permet de définir la priorité du thread. Evitez
	//! d'utiliser les priorités maximum pendant un temps prolongé, ceci aurait
	//! pour effet de bloquer les autres applications, et même dans certains cas
	//! de bloquer le système lui-même.
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY(TThread, TThreadPriority, Priority);

  //---------------------------------------------------------------------------
	//! @brief Propriété ReturnValue : Valeur de retour du thread
	//!
	//! Cette propriété permet de lire ou de définir la valeur de retour du
	//! Thread. Cette valeur est celle retournée par la méthode @b WaitFor.
	//!
	//! @sa WaitFor
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY(TThread, int, ReturnValue);

	//---------------------------------------------------------------------------
	//! @brief Propriété Suspended : Etat suspendu d'un thread
	//!
	//! Cette propriété permet de lire ou de définir l'état d'un thread. Mettez
	//! cette propriété à @b true pour arrêter temporairement un thread,
	//! remettez-la à @b false pour relancer l'exécution.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TThread, bool, Suspended);

	//---------------------------------------------------------------------------
	//! @brief Propriété Terminated : Etat terminé d'un thread
	//!
	//! Cette propriété permet de tester si un thread est terminé.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TThread, bool, Terminated);

	//---------------------------------------------------------------------------
	//! @brief Propriété ThreadID : Identifiant d'un thread
	//!
	//! Cette propriété permet de lire l'identifiant système d'un thread. Chaque
	//! thread a un identifiant unique dans le système. Cet identifiant peut être
	//! utilisé dans l'appel de certaines fonctions système.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TThread, int, ThreadID);

  //@}

  //! @name Evènements
  //@{

	//---------------------------------------------------------------------------
	//! @brief Evènement fin de thread.
	//!
	//! Cet évènement est déclenché juste avant la destruction du thread. Il est
	//! exécuté dans le thread principal, ce qui signifie que les méthodes de la
	//! VCL peuvent être appelées sans risquer de conflit dû au multi-thread.
	//!
	//! @sa ReturnValue
	//---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnTerminate, TObject *);

  //@}


};


#else  // TThreadH

class TThread;

#endif  // TThreadH

#undef In_TThreadH
