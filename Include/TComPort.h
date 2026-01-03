//-----------------------------------------------------------------------------
//! @file TComPort.h
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

#ifdef In_TComPortH
#error "Inclusion circulaire de TComPort"
#endif // In_TComPortH

#define In_TComPortH

#ifndef TComPortH
#define TComPortH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//---------------------------------------------------------------------------
// Macros et enums
//---------------------------------------------------------------------------

#define WaitInfinite            -1
#define CError_OpenFailed       0x01
#define CError_WriteFailed      0x02
#define CError_ReadFailed       0x03
#define CError_InvalidAsync     0x04
#define CError_PurgeFailed      0x05
#define CError_AsyncCheck       0x06
#define CError_SetStateFailed   0x07
#define CError_TimeoutsFailed   0x08
#define CError_SetupComFailed   0x09
#define CError_ClearComFailed   0x0A
#define CError_ModemStatFailed  0x0B
#define CError_EscapeComFailed  0x0C
#define CError_TransmitFailed   0x0D
#define CError_ConnChangeProp   0x0E
#define CError_EnumPortsFailed  0x0F
#define CError_StoreFailed      0x10
#define CError_LoadFailed       0x11
#define CError_RegFailed        0x12
#define CError_LedStateFailed   0x13
#define CError_ThreadCreated    0x14
#define CError_WaitFailed       0x15
#define CError_HasLink          0x16
#define CError_RegError         0x17
#define CError_UNDEFINED        0x00


enum TParityBits {
	prNone,                        //!< Pas de parité
	prOdd,                         //!< Parité impaire
	prEven,                        //!< Parité paire
	prMark,                        //!< Bit de parité à 1
	prSpace                        //!< Bit de parité à 0
};

enum TStopBits {
	sbOneStopBit,                  //!< Un bit de stop
	sbOne5StopBits,                //!< 1.5 bit de stop
	sbTwoStopBits                  //!< Deux bits de stop
};

/*
enum TDTRFlowControl {dtrDisable, dtrEnable, dtrHandshake};
enum TRTSFlowControl {rtsDisable, rtsEnable, rtsHandshake, rtsToggle};
enum TFlowControl {fcHardware, fcSoftware, fcNone, fcCustom};
enum TComEvent {evRxChar, evTxEmpty, evRxFlag, evRing, evBreak, evCTS, evDSR, evError, evRLSD, evRx80Full};
typedef Set<TComEvent, evRxChar, evRx80Full>  TComEvents;
enum TComSignal {csCTS, csDSR, csRing, csRLSD};
typedef Set<TComSignal, csCTS, csRLSD>  TComSignals;
enum TComError {ceFrame, ceRxParity, ceOverrun, ceBreak, ceIO, ceMode, ceRxOver, ceTxFull};
typedef Set<TComError, ceFrame, ceTxFull>  TComErrors;
enum TSyncMethod {smThreadSync, smWindowSync, smNone, smDisableEvents};
enum TStoreType {stRegistry, stIniFile};
enum TStoredProp {spBasic, spFlowControl, spBuffer, spTimeouts, spParity, spOthers};
typedef Set<TStoredProp, spBasic, spOthers>  TStoredProps;
enum TComLinkEvent {leConn, leCTS, leDSR, leRLSD, leRing, leRx, leTx, leTxEmpty, leRxFlag};
enum TOperationKind {okWrite, okRead};
*/


//-----------------------------------------------------------------------------
//! @class TComPort
//!
//! @brief Définition de la classe TComPort
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur le
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TComPort: public TComponent {
private:
protected:
  int FBaudRate;
  TParityBits FParity;
  int FDataBits;
  TStopBits FStopBits;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool TraitmntCommand(WORD Notify);
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TComPort(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TComPort(void);

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
  //! @brief Propriété BaudRate
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComPort, int, BaudRate);


  //---------------------------------------------------------------------------
  //! @brief Propriété Parity
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComPort, TParityBits, Parity);


  //---------------------------------------------------------------------------
  //! @brief Propriété DataBits
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComPort, int, DataBits);


  //---------------------------------------------------------------------------
  //! @brief Propriété StopBits
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TComPort, TStopBits, StopBits);


  //@}


  //! @name Evènements
  //@{

  //@}


};


#else  // TComPortH

class TComPort;

#endif  // TComPortH


#undef In_TComPortH
