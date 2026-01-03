//-----------------------------------------------------------------------------
//! @file TComPort.cpp
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

#include <FreeVcl.h>
#include <TComPort.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TComPort
//---------------------------------------------------------------------------

TComPort::TComPort(TComponent* AOwner): TComponent(AOwner) {
}

//---------------------------------------------------------------------------
TComPort::~TComPort(void) {
}

//---------------------------------------------------------------------------
bool TComPort::TraitmntCommand(WORD Notify) {

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BaudRate
//---------------------------------------------------------------------------

int TComPort::Get_BaudRate() {
  return FBaudRate;
}

bool TComPort::Set_BaudRate(int NewBaudRate) {
  if (FBaudRate != NewBaudRate) {
    FBaudRate = NewBaudRate;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Parity
//---------------------------------------------------------------------------

TParityBits TComPort::Get_Parity() {
  return FParity;
}

bool TComPort::Set_Parity(TParityBits NewParity) {
  if (FParity != NewParity) {
    FParity = NewParity;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DataBits
//---------------------------------------------------------------------------

int TComPort::Get_DataBits() {
  return FDataBits;
}

bool TComPort::Set_DataBits(int NewDataBits) {
  if (FDataBits != NewDataBits) {
    FDataBits = NewDataBits;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété StopBits
//---------------------------------------------------------------------------

TStopBits TComPort::Get_StopBits() {
  return FStopBits;
}

bool TComPort::Set_StopBits(TStopBits NewStopBits) {
  if (FStopBits != NewStopBits) {
    FStopBits = NewStopBits;
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TComPort::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("BaudRate"));
  ListProperties->Add(_T("Parity"));
  ListProperties->Add(_T("DataBits"));
  ListProperties->Add(_T("StopBits"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TComPort::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("BaudRate")) {
    return tpNumber;
  }
  if (asProperty == _T("Parity")) {
    *asInfos = _T("prNone, prOdd, prEven, prMark, prSpace");
    return tpChoice;
  }
  if (asProperty == _T("DataBits")) {
    return tpNumber;
  }
  if (asProperty == _T("StopBits")) {
    *asInfos = _T("sbOneStopBit\nsbOne5StopBits\nsbTwoStopBits");
    return tpChoice;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TComPort::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("BaudRate")) {
    return _T("9600");
  }
  if (asProperty == _T("Parity")) {
    return _T("prNone");
  }
  if (asProperty == _T("DataBits")) {
    return _T("7");
  }
  if (asProperty == _T("StopBits")) {
    return _T("2");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TComPort::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("BaudRate")) {
    return IntToStr(Get_BaudRate());
  }
  if (asProperty == _T("Parity")) {
    TParityBits Parity = Get_Parity();
    if (Parity == prNone) return _T("prNone");
    if (Parity == prOdd) return _T("prOdd");
    if (Parity == prEven) return _T("prEven");
    if (Parity == prMark) return _T("prMark");
    if (Parity == prSpace) return _T("prSpace");
    return _T("");
  }
  if (asProperty == _T("DataBits")) {
    return IntToStr(Get_DataBits());
  }
  if (asProperty == _T("StopBits")) {
    TStopBits StopBits = Get_StopBits();
    if (StopBits == sbOneStopBit) return _T("sbOneStopBit");
    if (StopBits == sbOne5StopBits) return _T("sbOne5StopBits");
    if (StopBits == sbTwoStopBits) return _T("sbTwoStopBits");
    return _T("");
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TComPort::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("BaudRate")) {
    Set_BaudRate(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Parity")) {
    if      (asValue == _T("prNone")) Set_Parity(prNone);
    else if (asValue == _T("prOdd")) Set_Parity(prOdd);
    else if (asValue == _T("prEven")) Set_Parity(prEven);
    else if (asValue == _T("prMark")) Set_Parity(prMark);
    else if (asValue == _T("prSpace")) Set_Parity(prSpace);
    return true;
  }
  if (asProperty == _T("DataBits")) {
    Set_DataBits(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("StopBits")) {
    if      (asValue == _T("sbOneStopBit")) Set_StopBits(sbOneStopBit);
    else if (asValue == _T("sbOne5StopBits")) Set_StopBits(sbOne5StopBits);
    else if (asValue == _T("sbTwoStopBits")) Set_StopBits(sbTwoStopBits);
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

