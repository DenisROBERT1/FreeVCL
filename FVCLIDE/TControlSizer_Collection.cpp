//-----------------------------------------------------------------------------
//! @file TControlSizer_Collection.cpp
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


//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------
#include <windows.h>
#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------
#include "TControlSizer_Collection.h"


//---------------------------------------------------------------------------
TControlSizer_Collection::TControlSizer_Collection(TComponent *AOwner) {
	Owner = AOwner;
	FAction = acNull;
}

//---------------------------------------------------------------------------
TControlSizer_Collection::~TControlSizer_Collection(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Action
//---------------------------------------------------------------------------

ACTION TControlSizer_Collection::Get_Action(void) {
	return FAction;
}

bool TControlSizer_Collection::Set_Action(ACTION NewAction) {
	if (FAction != NewAction) {
		FAction = NewAction;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TControlSizer_Collection::Get_Count(void) {
  return (int) ControlSizers.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Item
//---------------------------------------------------------------------------

TControlSizer *TControlSizer_Collection::Get_Item(int i) {
  return ControlSizers[i];
}


//---------------------------------------------------------------------------
bool TControlSizer_Collection::Add(TControlSizer *ControlSizer) {
	ControlSizers.push_back(ControlSizer);

  SET_EVENT(ControlSizer, TClientToScreen, ClientToScreen, TControlSizer_Collection, this, ClientToScreen);
  SET_EVENT(ControlSizer, TPlanToClient, PlanToClient, TControlSizer_Collection, this, PlanToClient);
  SET_EVENT(ControlSizer, TClientToPlan, ClientToPlan, TControlSizer_Collection, this, ClientToPlan);

	return true;
}

//---------------------------------------------------------------------------
bool TControlSizer_Collection::Remove(TControlSizer *ControlSizer) {
	int i;


	for (i = 0; i < (int) ControlSizers.size(); i++) {
		if (ControlSizers[i] == ControlSizer) {
			ControlSizers.erase(ControlSizers.begin() + i);
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
int TControlSizer_Collection::GetItemAt(int X, int Y) {
	int Id, WidthMax, HeightMax;
	int i;


	Id = -1;
	WidthMax = INT_MAX;
	HeightMax = INT_MAX;
  for (i = 1; i < (int) ControlSizers.size(); i++) {
		if (WidthMax > ControlSizers[i]->Width &&
				HeightMax > ControlSizers[i]->Height &&
				ControlSizers[i]->IsInFrame(X, Y)) {
			Id = i;
			WidthMax = ControlSizers[i]->Width;
			HeightMax = ControlSizers[i]->Height;
		}
  }

	return Id;
}

//---------------------------------------------------------------------------
void TControlSizer_Collection::ClientToScreen(TObject *Sender, LPPOINT lpPoint) {

	ClientToScreen_Collection(Sender, lpPoint);
}

//---------------------------------------------------------------------------
void TControlSizer_Collection::PlanToClient(TObject *Sender, LPPOINT lpPoint) {

	PlanToClient_Collection(Sender, lpPoint);
}

//---------------------------------------------------------------------------
void TControlSizer_Collection::ClientToPlan(TObject *Sender, LPPOINT lpPoint) {

	ClientToPlan_Collection(Sender, lpPoint);
}

//---------------------------------------------------------------------------
