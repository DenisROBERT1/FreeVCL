//-----------------------------------------------------------------------------
//! @file TComponent.cpp
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
// TComponent
//---------------------------------------------------------------------------

WORD TComponent::NumCommand = 100;

TComponent::TComponent(TComponent* AOwner): TPersistent() {
  // Initialisations
  FComponentState.Clear();
  FCommand = NumCommand++;
  FTag = 0;
  FOwner = AOwner;
  if (FOwner) {
    FComponentState = FOwner->ComponentState;
    FOwner->InsertComponent(this);
  }
}

TComponent::~TComponent(void) {
  int i;


  Destroying();

  // Destruction en commencant par la fin, car les composants détruits se
  // suppriment de la liste de composants du parent (donc ça décalerait
  // la liste si on commencait par le premier)
  for (i = (int) FComponents.size() - 1; i >= 0; i--) {
    FVCL_ASSERT(i < (int) FComponents.size() && "Double insertion ?");
    delete FComponents[i];
  }
  FComponents.clear();
  if (FOwner) FOwner->RemoveComponent(this);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

AnsiString TComponent::Get_Caption(void) {
  return FCaption;
}

bool TComponent::Set_Caption(AnsiString NewCaption) {
  if (FCaption != NewCaption) {
    FCaption = NewCaption;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Owner
//---------------------------------------------------------------------------

TComponent * TComponent::Get_Owner(void) {
  return FOwner;
}

bool TComponent::Set_Owner(TComponent *NewOwner) {
  if (FOwner != NewOwner) {
	  if (FOwner) FOwner->RemoveComponent(this);
    FOwner = NewOwner;
    if (FOwner) FOwner->InsertComponent(this);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Components
//---------------------------------------------------------------------------

TComponent *TComponent::Get_Components(int i) {
  return FComponents[i];
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ComponentCount
//---------------------------------------------------------------------------

int TComponent::Get_ComponentCount(void) {
  return (int) FComponents.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ComponentIndex
//---------------------------------------------------------------------------

int TComponent::Get_ComponentIndex(void) {

  if (FOwner) {
    int i, ComponentsSize;
    ComponentsSize = (int) FOwner->FComponents.size();
    for (i = 0; i < ComponentsSize; i++) {
      if (FOwner->FComponents[i] == this) return i;
    }
  }

  return -1;
}

bool TComponent::Set_ComponentIndex(int NewComponentIndex) {
  if (!FOwner) return false;
  if (NewComponentIndex < 0 ||
      NewComponentIndex >= (int) FOwner->FComponents.size()) return false;


  FOwner->RemoveComponent(this);
  FOwner->InsertComponent(this, NewComponentIndex);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Command
//---------------------------------------------------------------------------

WORD TComponent::Get_Command(void) {
  return FCommand;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ComponentState
//---------------------------------------------------------------------------

TComponentStates TComponent::Get_ComponentState(void) {
  return FComponentState;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Tag
//---------------------------------------------------------------------------

int TComponent::Get_Tag(void) {
  return FTag;
}

bool TComponent::Set_Tag(int NewTag) {
  if (FTag != NewTag) {
    FTag = NewTag;
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TComponent::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Caption"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TComponent::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Caption")) {
    return tpText;
  }
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TComponent::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Caption")) {
    return _T("");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TComponent::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Caption")) {
    return Get_Caption();
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TComponent::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Caption")) {
    Set_Caption(asValue);
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
// Insertion d'un composant enfant
//---------------------------------------------------------------------------

void TComponent::InsertComponent(TComponent* AComponent, int Index) {
  if (Index == -1) FComponents.push_back(AComponent);
  else FComponents.insert(FComponents.begin() + Index, AComponent);
}

//---------------------------------------------------------------------------
// Suppression d'un composant enfant de la liste des composants
//---------------------------------------------------------------------------

void TComponent::RemoveComponent(TComponent* AComponent) {
  int i;


  for (i = 0; i < (int) FComponents.size(); i++) {
    if (FComponents[i] == AComponent) FComponents.erase(FComponents.begin() + i);
  }

}


//---------------------------------------------------------------------------
// Initialise le composant après la lecture complète du fichier DFM.
//---------------------------------------------------------------------------

void FASTCALL TComponent::Loaded(void) {
  int i;


  if (FComponentState.Contains(csLoading)) {
    FComponentState >> csLoading;
    for (i = 0; i < (int) FComponents.size(); i++) FComponents[i]->Loaded();
  }

}


//---------------------------------------------------------------------------
// Indique que le composant et ses composants possédés vont être détruits.
//---------------------------------------------------------------------------

void FASTCALL TComponent::Destroying(void) {
  int i;


  if (!FComponentState.Contains(csDestroying)) {
    FComponentState << csDestroying;
    for (i = 0; i < (int) FComponents.size(); i++) FComponents[i]->Destroying();
  }

}


//---------------------------------------------------------------------------
TComponent * TComponent::FindRootComponent(void) {
  if (FOwner == NULL) return this;
  else return FOwner->FindRootComponent();
}

//---------------------------------------------------------------------------
TComponent * TComponent::FindComponent(const AnsiString AName) {
  int i;
  TComponent *Component;
  TComponent *ComponentFound;


  if (FName == AName) return this;
  for (i = 0; i < (int) FComponents.size(); i++) {
    Component = FComponents[i];
    ComponentFound = Component->FindComponent(AName);
    if (ComponentFound) return ComponentFound;
  }

  return NULL;
}

//---------------------------------------------------------------------------
TComponent * TComponent::FindComponentFromId(WORD Id, bool bRecursif) {
  int i;
  TComponent *Component;
  TComponent *ComponentFound;


  for (i = 0; i < (int) FComponents.size(); i++) {
    Component = FComponents[i];
    if (Component->Command == Id) return Component;
    if (bRecursif) {
      ComponentFound = Component->FindComponentFromId(Id, true);
      if (ComponentFound) return ComponentFound;
    }
  }

  return NULL;
}

//---------------------------------------------------------------------------
TWinControl * TComponent::FindComponentFromHWnd(HWND hWnd) {
  int i;
  TWinControl *Component;
  TWinControl *ComponentFound;


  Component = dynamic_cast<TWinControl *>(this);
  if (Component) {
    if (Component->Handle == hWnd) return Component;
  }
  for (i = 0; i < (int) FComponents.size(); i++) {
    Component = dynamic_cast<TWinControl *>(FComponents[i]);
    if (Component) {
      ComponentFound = Component->FindComponentFromHWnd(hWnd);
      if (ComponentFound) return ComponentFound;
    }
  }

  return NULL;
}

//---------------------------------------------------------------------------
bool TComponent::DistributionCommand(WORD Command, WORD Notify) {
  int i;


  if (Command == FCommand) {
    ProcessCommand(Notify);
    return true;
  }

  for (i = 0; i < (int) FComponents.size(); i++) {
    if (FComponents[i]->DistributionCommand(Command, Notify)) return true;
  }

  return false;
}

//---------------------------------------------------------------------------
int TComponent::DistributionNotify(int Command, LPNMHDR pnmh) {
  int i;
  int Retour;


	// Dans le cas des infos-bulles, le wParam (Command) contient une valeur aléatoire.
  // Mais comme les infos-bulles sont toujours renvoyées à la fenêtre parente on peut
	// forcer Command = FCommand.
	if (pnmh->code == TTN_SHOW) Command = FCommand;

	if (Command == FCommand) {
    return ProcessNotify(pnmh);
  }

  for (i = 0; i < (int) FComponents.size(); i++) {
    Retour = FComponents[i]->DistributionNotify(Command, pnmh);
    if (Retour) return Retour;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
bool TComponent::DistributionShortCut(int nVirtKey, TShiftState Shift) {
  int i;


  if (ProcessShortCut(nVirtKey, Shift)) return true;

  for (i = 0; i < (int) FComponents.size(); i++) {
    if (FComponents[i]->DistributionShortCut(nVirtKey, Shift)) return true;
  }

  return false;
}

//---------------------------------------------------------------------------
bool TComponent::DistributionMeasureItem(UINT idCtrl, LPMEASUREITEMSTRUCT lpMIS) {
  TComponent *Component;


  if (lpMIS->CtlType == ODT_MENU) idCtrl = lpMIS->itemID;
  Component = FindComponentFromId(idCtrl, true);
  if (Component) {
    return Component->ProcessMeasureItem(lpMIS);
  }

  return false;
}

//---------------------------------------------------------------------------
bool TComponent::DistributionDrawItem(UINT idCtrl, LPDRAWITEMSTRUCT lpDIS) {
  TComponent *Component;


  if (lpDIS->CtlType == ODT_MENU) idCtrl = lpDIS->itemID;
  Component = FindComponentFromId(idCtrl, true);
  if (Component) {
    return Component->ProcessDrawItem(lpDIS);
  }

  return false;
}

//---------------------------------------------------------------------------

