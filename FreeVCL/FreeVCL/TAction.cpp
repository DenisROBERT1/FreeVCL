//-----------------------------------------------------------------------------
//! @file TAction.cpp
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


//---------------------------------------------------------------------------
// TAction
//---------------------------------------------------------------------------

TAction::TAction(TComponent* AOwner): TComponent(AOwner) {

  // Initialisations
  FClassName = _T("TAction");
	FAutoCheck = false;
	FChecked = false;
	FDisableIfNoHandler = false;
	FEnabled = true;
	FGroupIndex = 0;
	FHelpContext = 0;
	FHelpType = htKeyword;
	FImageIndex = -1;
  FShortCut = 0;
	FVisible = true;
}

TAction::~TAction(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

bool TAction::Set_Caption(AnsiString NewCaption) {
  if (FCaption != NewCaption) {
    FCaption = NewCaption;
		OnUpdate(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AutoCheck
//---------------------------------------------------------------------------

bool TAction::Get_AutoCheck(void) {
  return FAutoCheck;
}

bool TAction::Set_AutoCheck(bool NewAutoCheck) {
  if (FAutoCheck != NewAutoCheck) {
    FAutoCheck = NewAutoCheck;
		OnUpdate(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Checked
//---------------------------------------------------------------------------

bool TAction::Get_Checked(void) {
  return FChecked;
}

bool TAction::Set_Checked(bool NewChecked) {
  if (FChecked != NewChecked) {
    FChecked = NewChecked;
		OnUpdate(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DisableIfNoHandler
//---------------------------------------------------------------------------

bool TAction::Get_DisableIfNoHandler(void) {
  return FDisableIfNoHandler;
}

bool TAction::Set_DisableIfNoHandler(bool NewDisableIfNoHandler) {
  if (FDisableIfNoHandler != NewDisableIfNoHandler) {
    FDisableIfNoHandler = NewDisableIfNoHandler;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Enabled
//---------------------------------------------------------------------------

bool TAction::Get_Enabled(void) {
  return FEnabled;
}

bool TAction::Set_Enabled(bool NewEnabled) {
  if (FEnabled != NewEnabled) {
    FEnabled = NewEnabled;
		OnUpdate(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété GroupIndex
//---------------------------------------------------------------------------

int TAction::Get_GroupIndex(void) {
  return FGroupIndex;
}

bool TAction::Set_GroupIndex(int NewGroupIndex) {
  if (FGroupIndex != NewGroupIndex) {
    FGroupIndex = NewGroupIndex;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HelpContext
//---------------------------------------------------------------------------

THelpContext TAction::Get_HelpContext(void) {
  return FHelpContext;
}

bool TAction::Set_HelpContext(THelpContext NewHelpContext) {
  if (FHelpContext != NewHelpContext) {
    FHelpContext = NewHelpContext;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HelpKeyword
//---------------------------------------------------------------------------

AnsiString TAction::Get_HelpKeyword(void) {
  return FHelpKeyword;
}

bool TAction::Set_HelpKeyword(AnsiString NewHelpKeyword) {
  if (FHelpKeyword != NewHelpKeyword) {
    FHelpKeyword = NewHelpKeyword;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HelpType
//---------------------------------------------------------------------------

THelpType TAction::Get_HelpType(void) {
  return FHelpType;
}

bool TAction::Set_HelpType(THelpType NewHelpType) {
  if (FHelpType != NewHelpType) {
    FHelpType = NewHelpType;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Hint
//---------------------------------------------------------------------------

AnsiString TAction::Get_Hint(void) {
  return FHint;
}

bool TAction::Set_Hint(AnsiString NewHint) {
  if (FHint != NewHint) {
    FHint = NewHint;
		OnUpdate(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ImageIndex
//---------------------------------------------------------------------------

TImageIndex TAction::Get_ImageIndex(void) {
  return FImageIndex;
}

bool TAction::Set_ImageIndex(TImageIndex NewImageIndex) {
  if (FImageIndex != NewImageIndex) {
    FImageIndex = NewImageIndex;
		OnUpdate(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TAction::Get_Name(void) {
  return FName;
}

bool TAction::Set_Name(AnsiString NewName) {
  if (FName != NewName) {
    FName = NewName;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShortCut
//---------------------------------------------------------------------------

TShortCut TAction::Get_ShortCut(void) {
  return FShortCut;
}

bool TAction::Set_ShortCut(TShortCut NewShortCut) {
  if (FShortCut != NewShortCut) {
    FShortCut = NewShortCut;
		OnUpdate(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool TAction::Get_Visible(void) {
  return FVisible;
}

bool TAction::Set_Visible(bool NewVisible) {
  if (FVisible != NewVisible) {
    FVisible = NewVisible;
  }
  return true;
}

//---------------------------------------------------------------------------
bool TAction::Execute(void) {

	if (!FComponentState.Contains(csLoading)) {
		if (FAutoCheck) Set_Checked(!FChecked);
		if (OnExecute) { 
			OnExecute(this);
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TAction::ProcessShortCut(int nVirtKey, TShiftState Shift) {
  WORD ShortCutKey;
  TShiftState ShortCutShift;


  if (!FComponentState.Contains(csLoading) && FEnabled) {
    ShortCutToKey(FShortCut, ShortCutKey, ShortCutShift);
    if (ShortCutKey == (WORD) nVirtKey && ShortCutShift == Shift) {
			Execute();
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TAction::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Caption"));
  ListProperties->Add(_T("AutoCheck"));
  ListProperties->Add(_T("Checked"));
  ListProperties->Add(_T("DisableIfNoHandler"));
  ListProperties->Add(_T("Enabled"));
  ListProperties->Add(_T("GroupIndex"));
  ListProperties->Add(_T("HelpContext"));
  ListProperties->Add(_T("HelpKeyword"));
  ListProperties->Add(_T("HelpType"));
  ListProperties->Add(_T("Hint"));
  ListProperties->Add(_T("ImageIndex"));
  ListProperties->Add(_T("Name"));
  ListProperties->Add(_T("ShortCut"));
  ListProperties->Add(_T("Visible"));
  ListProperties->Add(_T("OnHint"));
  ListProperties->Add(_T("OnExecute"));
  ListProperties->Add(_T("OnUpdate"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TAction::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Caption")) {
    return tpText;
  }
  if (asProperty == _T("AutoCheck")) {
    return tpBool;
  }
  if (asProperty == _T("Checked")) {
    return tpBool;
  }
  if (asProperty == _T("DisableIfNoHandler")) {
    return tpBool;
  }
  if (asProperty == _T("Enabled")) {
    return tpBool;
  }
  if (asProperty == _T("GroupIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("HelpContext")) {
    return tpNumber;
  }
  if (asProperty == _T("HelpKeyword")) {
    return tpText;
  }
  if (asProperty == _T("HelpType")) {
    *asInfos = _T("htKeyword\nhtContext");
    return tpChoice;
  }
  if (asProperty == _T("Hint")) {
    return tpText;
  }
  if (asProperty == _T("ImageIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("Name")) {
    return tpText;
  }
  if (asProperty == _T("ShortCut")) {
    return tpNumber;
  }
  if (asProperty == _T("Visible")) {
    return tpBool;
  }
  if (asProperty == _T("OnHint")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnExecute")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnUpdate")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TAction::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Caption")) {
    return _T("");
  }
  if (asProperty == _T("AutoCheck")) {
    return _T("False");
  }
  if (asProperty == _T("Checked")) {
    return _T("False");
  }
  if (asProperty == _T("DisableIfNoHandler")) {
    return _T("False");
  }
  if (asProperty == _T("Enabled")) {
    return _T("False");
  }
  if (asProperty == _T("GroupIndex")) {
    return _T("0");
  }
  if (asProperty == _T("HelpContext")) {
    return _T("");
  }
  if (asProperty == _T("HelpKeyword")) {
    return _T("");
  }
  if (asProperty == _T("HelpType")) {
    return _T("");
  }
  if (asProperty == _T("Hint")) {
    return _T("");
  }
  if (asProperty == _T("ImageIndex")) {
    return _T("");
  }
  if (asProperty == _T("Name")) {
    return _T("");
  }
  if (asProperty == _T("ShortCut")) {
    return _T("");
  }
  if (asProperty == _T("Visible")) {
    return _T("False");
  }
  if (asProperty == _T("OnHint")) {
    return _T("");
  }
  if (asProperty == _T("OnExecute")) {
    return _T("");
  }
  if (asProperty == _T("OnUpdate")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TAction::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Caption")) {
    return Get_Caption();
  }
  if (asProperty == _T("AutoCheck")) {
    if (Get_AutoCheck()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Checked")) {
    if (Get_Checked()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("DisableIfNoHandler")) {
    if (Get_DisableIfNoHandler()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Enabled")) {
    if (Get_Enabled()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("GroupIndex")) {
    return IntToStr(Get_GroupIndex());
  }
  if (asProperty == _T("HelpContext")) {
    return Get_HelpContext();
  }
  if (asProperty == _T("HelpKeyword")) {
    return Get_HelpKeyword();
  }
  if (asProperty == _T("HelpType")) {
    THelpType HelpType = Get_HelpType();
    if (HelpType == htKeyword) return _T("htKeyword");
    if (HelpType == htContext) return _T("htContext");
    return _T("");
  }
  if (asProperty == _T("Hint")) {
    return Get_Hint();
  }
  if (asProperty == _T("ImageIndex")) {
    return Get_ImageIndex();
  }
  if (asProperty == _T("Name")) {
    return Get_Name();
  }
  if (asProperty == _T("ShortCut")) {
    return AnsiString((int) Get_ShortCut());
  }
  if (asProperty == _T("Visible")) {
    if (Get_Visible()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("OnHint")) {
    return OnHint.AsString();
  }
  if (asProperty == _T("OnExecute")) {
    return OnExecute.AsString();
  }
  if (asProperty == _T("OnUpdate")) {
    return OnUpdate.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TAction::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Caption")) {
    Set_Caption(asValue);
    return true;
  }
  if (asProperty == _T("AutoCheck")) {
    Set_AutoCheck(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Checked")) {
    Set_Checked(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("DisableIfNoHandler")) {
    Set_DisableIfNoHandler(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Enabled")) {
    Set_Enabled(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("GroupIndex")) {
    Set_GroupIndex(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("HelpContext")) {
    Set_HelpContext(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("HelpKeyword")) {
    Set_HelpKeyword(asValue);
    return true;
  }
  if (asProperty == _T("HelpType")) {
    Set_HelpType((THelpType) asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Hint")) {
    Set_Hint(asValue);
    return true;
  }
  if (asProperty == _T("ImageIndex")) {
    Set_ImageIndex(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Name")) {
    Set_Name(asValue);
    return true;
  }
  if (asProperty == _T("ShortCut")) {
    Set_ShortCut((TShortCut) asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Visible")) {
    Set_Visible(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("OnHint")) {
		OnHint.ObjCall = Sender;
    OnHint = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnExecute")) {
		OnExecute.ObjCall = Sender;
    OnExecute = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnUpdate")) {
		OnUpdate.ObjCall = Sender;
    OnUpdate = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

