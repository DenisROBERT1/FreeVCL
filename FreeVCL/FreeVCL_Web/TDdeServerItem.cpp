//-----------------------------------------------------------------------------
//! @file TDdeServerItem.cpp
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
#include <FreeVcl_Web.h>


//---------------------------------------------------------------------------
// TDdeServerItem
//---------------------------------------------------------------------------

TDdeServerItem::TDdeServerItem(TComponent* AOwner): TComponent(AOwner) {

  // Initialisations
  FClassName = _T("TDdeServerItem");
  FLines = new TStringList();

}

TDdeServerItem::~TDdeServerItem(void) {

  if (FServerConv) FServerConv->RemoveDdeServerItem(this);
  delete FLines;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ServerConv
//---------------------------------------------------------------------------

TDdeServerConv *TDdeServerItem::Get_ServerConv() {
  return FServerConv;
}

bool TDdeServerItem::Set_ServerConv(TDdeServerConv *NewServerConv) {
  if (FServerConv != NewServerConv) {
    if (FServerConv) FServerConv->RemoveDdeServerItem(this);
    FServerConv = NewServerConv;
    if (FServerConv) FServerConv->AddDdeServerItem(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Lines
//---------------------------------------------------------------------------

TStrings *TDdeServerItem::Get_Lines() {
  return FLines;
}

bool TDdeServerItem::Set_Lines(TStrings *NewLines) {
  if (FLines != NewLines) {
    FLines->Assign(NewLines);
    FText = NewLines->GetTextStr();
    OnChange(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TDdeServerItem::Get_Text() {
  return FText;
}

bool TDdeServerItem::Set_Text(AnsiString NewText) {
  if (FText != NewText) {
    FLines->SetTextStr(NewText);
    FText = NewText;
    OnChange(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDdeServerItem::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("ServerConv"));
  ListProperties->Add(_T("Lines.Strings"));
  ListProperties->Add(_T("Text"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDdeServerItem::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("ServerConv")) {
    return tpObject;
  }
  if (asProperty == _T("Lines.Strings")) {
    return tpObject;
  }
  if (asProperty == _T("Text")) {
    return tpText;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDdeServerItem::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("ServerConv")) {
    return _T("");
  }
  if (asProperty == _T("Lines.Strings")) {
    return _T("");
  }
  if (asProperty == _T("Text")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDdeServerItem::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("ServerConv")) {
    TDdeServerConv *ServerConv = Get_ServerConv();
    if (ServerConv) return ServerConv->Name;
    return _T("");
  }
  if (asProperty == _T("Lines.Strings")) {
    TStrings *Lines = Get_Lines();
    return FLines->GetTextStr();
  }
  if (asProperty == _T("Text")) {
    return Get_Text();
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDdeServerItem::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("ServerConv")) {
    TDdeServerConv *ServerConv;
    ServerConv = (TDdeServerConv *) FindRootComponent()->FindComponent(asValue);
    Set_ServerConv(ServerConv);
    return true;
  }
  if (asProperty == _T("Lines.Strings")) {
    FLines->SetTextStr(asValue);
    return true;
  }
  if (asProperty == _T("Text")) {
    Set_Text(asValue);
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}
//---------------------------------------------------------------------------
