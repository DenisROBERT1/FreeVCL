//-----------------------------------------------------------------------------
//! @file TElemBase.cpp
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
#include <windows.h>
#include <assert.h>

#include "TControlSizer.h"
#include "TControlSizer_Collection.h"
#include "TElemBase.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TElemBase::TElemBase(TElemBase *ElemParent) {
  TAnchors Anchors;


	FClassName = _T("TElemBase");
	FLeft = 0;
  FTop = 0;
  FWidth = 0;
  FHeight = 0;
	Anchors.Clear() << akLeft << akTop;
  FAnchors = Anchors;
	shared_ptr_ControlSizer_Collection = NULL;

  FParent = ElemParent;
  if (FParent) {
    FParent->FComponents.push_back(this);
		shared_ptr_ControlSizer_Collection = FParent->shared_ptr_ControlSizer_Collection;
  }

  FControlSizer = new TControlSizer(0, 0, 0, 0, Anchors);
	FControlSizer->Data = this;
  FControlSizer->Visible = true;
  SET_EVENT(FControlSizer, TOnXChange, OnXChange, TElemBase, this, ControlSizerXChange);
  SET_EVENT(FControlSizer, TOnYChange, OnYChange, TElemBase, this, ControlSizerYChange);
  SET_EVENT(FControlSizer, TOnWidthChange, OnWidthChange, TElemBase, this, ControlSizerWidthChange);
  SET_EVENT(FControlSizer, TOnHeightChange, OnHeightChange, TElemBase, this, ControlSizerHeightChange);
  SET_EVENT(FControlSizer, TOnAnchorsChange, OnAnchorsChange, TElemBase, this, ControlSizerAnchorsChange);
	if (shared_ptr_ControlSizer_Collection) {
		TControlSizer_Collection *ControlSizers;
		ControlSizers = shared_ptr_ControlSizer_Collection.get();
		ControlSizers->Add(FControlSizer);
	}

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TElemBase::~TElemBase(void) {
  int i;


  // Destruction du dernier au premier (car le delete provoque un erase
	// dans le destructeur du FComponents du parent)
	for (i = (int) FComponents.size() - 1; i >= 0; i--) {
    delete FComponents[i];
  }
  FComponents.clear();

  if (FParent) {
    for (i = 0; i < (int) FParent->FComponents.size(); i++) {
      if (FParent->FComponents[i] == this) {
        FParent->FComponents.erase(FParent->FComponents.begin() + i);
        break;
      }
    }
  }

	if (shared_ptr_ControlSizer_Collection) {
		TControlSizer_Collection *ControlSizers;
		ControlSizers = shared_ptr_ControlSizer_Collection.get();
		ControlSizers->Remove(FControlSizer);
	}
  delete FControlSizer;

	shared_ptr_ControlSizer_Collection.reset();
}

//---------------------------------------------------------------------------
void FASTCALL TElemBase::ControlSizerXChange(TObject *Sender, int NewX) {
	int ALeft;

	ALeft = FControlSizer->X;
	if (FParent) ALeft -= FParent->ControlSizer->X;
	Set_Left(ALeft);
}

//---------------------------------------------------------------------------
void FASTCALL TElemBase::ControlSizerYChange(TObject *Sender, int NewX) {
	int ATop;

	ATop = FControlSizer->Y;
	if (FParent) ATop -= FParent->ControlSizer->Y;
	Set_Top(ATop);
}

//---------------------------------------------------------------------------
void FASTCALL TElemBase::ControlSizerWidthChange(TObject *Sender, int NewWidth) {
	Set_Width(NewWidth);
}

//---------------------------------------------------------------------------
void FASTCALL TElemBase::ControlSizerHeightChange(TObject *Sender, int NewHeight) {
	Set_Height(NewHeight);
}

//---------------------------------------------------------------------------
void FASTCALL TElemBase::ControlSizerAnchorsChange(TObject *Sender, TAnchors NewAnchors) {
	Set_Anchors(NewAnchors);
}

//---------------------------------------------------------------------------
AnsiString TElemBase::Get_Type(void) {
  return _T("");
}

//---------------------------------------------------------------------------
AnsiString TElemBase::Get_Name(void) {
  return _T("");
}

bool TElemBase::Set_Name(AnsiString NewName) {
	UpdateInfos();
  OnChange(this);

	return true;
}

//---------------------------------------------------------------------------
int TElemBase::Get_X(void) {
	int X;


	if (FParent) X = FParent->Get_X() + FLeft;
	else X = 0;

	return X;
}

//---------------------------------------------------------------------------
int TElemBase::Get_Y(void) {
	int Y;


	if (FParent) Y = FParent->Get_Y() + FTop;
	else Y = 0;

	return Y;
}

//---------------------------------------------------------------------------
int TElemBase::Get_Left(void) {
  return FLeft;
}

bool TElemBase::Set_Left(int NewLeft) {

	if (FLeft != NewLeft) {
		FLeft = NewLeft;
		UpdateInfos();
		FControlSizer->X = Get_X();
		OnChange(this);
	}

  return true;
}

//---------------------------------------------------------------------------
int TElemBase::Get_Top(void) {
  return FTop;
}

bool TElemBase::Set_Top(int NewTop) {

	if (FTop != NewTop) {
		FTop = NewTop;
		UpdateInfos();
		FControlSizer->Y = Get_Y();
		OnChange(this);
	}

  return true;
}

//---------------------------------------------------------------------------
int TElemBase::Get_Width(void) {
  return 0;
}

bool TElemBase::Set_Width(int NewWidth) {

	if (FWidth != NewWidth) {
		FWidth = NewWidth;
		UpdateInfos();
		FControlSizer->Width = NewWidth;
		OnChange(this);
	}

  return true;
}

//---------------------------------------------------------------------------
int TElemBase::Get_Height(void) {
  return FHeight;
}

bool TElemBase::Set_Height(int NewHeight) {

	if (FHeight != NewHeight) {
		FHeight = NewHeight;
		UpdateInfos();
		FControlSizer->Height = NewHeight;
		OnChange(this);
	}

  return true;
}


//---------------------------------------------------------------------------
TRect TElemBase::Get_ClientRect(void) {
  TRect Retour;

  Retour.Left = 0;
  Retour.Top = 0;
  Retour.Right = Get_Width();
  Retour.Bottom = Get_Height();

  return Retour;
}


//---------------------------------------------------------------------------
TAnchors TElemBase::Get_Anchors(void) {
  return FAnchors;
}

bool TElemBase::Set_Anchors(TAnchors NewAnchors) {

	if (FAnchors != NewAnchors) {
		FAnchors = NewAnchors;
		FControlSizer->Anchors = NewAnchors;
		OnChange(this);
	}

  return true;
}


//---------------------------------------------------------------------------
TElemBase * TElemBase::Get_Parent(void) {
  return FParent;
}

bool TElemBase::Set_Parent(TElemBase * NewParent) {

	if (FParent != NewParent) {
		if (FParent) {
			int i;
			for (i = 0; i < (int) FParent->FComponents.size(); i++) {
				if (FParent->FComponents[i] == this) {
					FParent->FComponents.erase(FParent->FComponents.begin() + i);
					break;
				}
			}
		}

		FParent = NewParent;

		if (FParent) {
	    FParent->FComponents.push_back(this);
		}

		OnChange(this);
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ParentPotentiel
//---------------------------------------------------------------------------

bool TElemBase::Get_ParentPotentiel(void) {
  return FParentPotentiel;
}

bool TElemBase::Set_ParentPotentiel(bool NewParentPotentiel) {

  if (FParentPotentiel != NewParentPotentiel) {
    FParentPotentiel = NewParentPotentiel;
  }

  return true;
}


//---------------------------------------------------------------------------
TElemBase * TElemBase::Get_Components(int i) {
  return FComponents[i];
}

//---------------------------------------------------------------------------
int TElemBase::Get_ComponentCount(void) {
  return (int) FComponents.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ControlSizer
//---------------------------------------------------------------------------

TControlSizer * TElemBase::Get_ControlSizer(void) {
  return FControlSizer;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété bSelected
//---------------------------------------------------------------------------

bool TElemBase::Get_Selected(void) {
  return FControlSizer->Visible;
}

//---------------------------------------------------------------------------
bool TElemBase::Set_Selected(bool NewSelected) {
  FControlSizer->Visible = NewSelected;

  return true;
}

//---------------------------------------------------------------------------
bool TElemBase::UpdateInfos(void) {
	AnsiString asInfo;
	asInfo.sprintf(_T("%s %s\n%i %i (%i x %i)"),
									(LPCTSTR) ((AnsiString) Type),
									(LPCTSTR) ((AnsiString) Name),
									Left,
									Top,
									Width,
									Height);

	return UpdateInfos(asInfo);
}

//---------------------------------------------------------------------------
TElemBase *TElemBase::NewChild(AnsiString asTypeACreer) {
	TElemBase *NewElemBase = NewChild();
	if (NewElemBase) {
		if (!NewElemBase->Create(asTypeACreer)) {
			delete NewElemBase;
			return NULL;
		}
	}

	return NewElemBase;
}

//---------------------------------------------------------------------------
bool TElemBase::SwapComponents(TElemBase *Element1, TElemBase *Element2) {
	int i;


	for (i = 0; i < (int) FComponents.size(); i++) {
		if (FComponents[i] == Element1) FComponents[i] = Element2;
		else if (FComponents[i] == Element2) FComponents[i] = Element1;
	}

	return true;
}

//---------------------------------------------------------------------------
bool TElemBase::UpdateControlSizer(void) {
	FControlSizer->X = Get_X();
	FControlSizer->Y = Get_Y();
	FControlSizer->Width = Get_Width();
	FControlSizer->Height = Get_Height();

	return true;
}

//---------------------------------------------------------------------------
