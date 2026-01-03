//-----------------------------------------------------------------------------
//! @file TElemVCL.cpp
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
#include "TElemVCL.h"
#include "TFrame_SaisieDialog.h"
#include "TForm_SaisieDialog.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TElemVCL::TElemVCL(TElemBase *ElemParent, TComponent *Component):
                     TElemBase(ElemParent) {
  FVisible = true;
  FShowHint = false;
  FEnabled = true;
  FPosition = poDesigned;
	bDeleteComponent = false;

  FComponent = Component;
  if (FComponent) {
	  TControl *Control = dynamic_cast<TControl *>(FComponent);
    if (Control) Control->Visible = true;
		bDeleteComponent = true;
  }
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TElemVCL::~TElemVCL(void) {
  int i;


  // La destruction du composant entraîne automatiquement la destruction
	// de ses enfants => flag pour éviter une double destruction
	for (i = 0; i < (int) FComponents.size(); i++) {
		TElemVCL *ElemVCLChild = dynamic_cast<TElemVCL *>(FComponents[i]);
		if (ElemVCLChild) ElemVCLChild->bDeleteComponent = false;
  }

	if (bDeleteComponent) delete FComponent;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ClassName
//---------------------------------------------------------------------------

AnsiString TElemVCL::Get_ClassName(void) {
  if (FComponent) return FComponent->ClassName;

	return _T("");
}


//---------------------------------------------------------------------------
// Propriétés
//---------------------------------------------------------------------------

AnsiString TElemVCL::Get_Type(void) {
  if (FComponent) return FComponent->ClassName;

	return _T("");
}

//---------------------------------------------------------------------------
AnsiString TElemVCL::Get_Name(void) {
  if (FComponent) return FComponent->Name;

	return FName;
}

//---------------------------------------------------------------------------
bool TElemVCL::Set_Name(AnsiString NewName) {
	FName = NewName;
	if (FComponent) {
		FComponent->Name = NewName;
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
int TElemVCL::Get_Left(void) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) return Control->Left;
  else return 0;
}

//---------------------------------------------------------------------------
bool TElemVCL::Set_Left(int NewLeft) {
  TControl *Control;

	TElemBase::Set_Left(NewLeft);

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) {
    Control->Left = NewLeft;
  }

  return true;
}

//---------------------------------------------------------------------------
int TElemVCL::Get_Top(void) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) return Control->Top;
  else return 0;
}

//---------------------------------------------------------------------------
bool TElemVCL::Set_Top(int NewTop) {
  TControl *Control;

	TElemBase::Set_Top(NewTop);

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) {
    Control->Top = NewTop;
  }

  return true;
}

//---------------------------------------------------------------------------
int TElemVCL::Get_Width(void) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) return Control->Width;
  else return 0;
}

//---------------------------------------------------------------------------
bool TElemVCL::Set_Width(int NewWidth) {
  TControl *Control;

	TElemBase::Set_Width(NewWidth);

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) {
    Control->Width = NewWidth;
  }

  return true;
}

//---------------------------------------------------------------------------
int TElemVCL::Get_Height(void) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) return Control->Height;
  else return 0;
}

//---------------------------------------------------------------------------
bool TElemVCL::Set_Height(int NewHeight) {
  TControl *Control;

	TElemBase::Set_Height(NewHeight);

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) {
    Control->Height = NewHeight;
  }

  return true;
}

//---------------------------------------------------------------------------
TRect TElemVCL::Get_ClientRect(void) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) return Control->ClientRect;
	else return TElemBase::ClientRect;
}


//---------------------------------------------------------------------------
TAnchors TElemVCL::Get_Anchors(void) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) return Control->Anchors;
  else {
    TAnchors Anchors;
    Anchors.Clear();
    return Anchors;
  }
}

//---------------------------------------------------------------------------
bool TElemVCL::Set_Anchors(TAnchors NewAnchors) {
  TControl *Control;

	TElemBase::Set_Anchors(NewAnchors);

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) {
    Control->Anchors = NewAnchors;
  }

  return true;
}

//---------------------------------------------------------------------------
bool TElemVCL::Set_Parent(TElemBase * NewParent) {
  TControl *Control;
  TComponent *ComponentParent;
  TWinControl *WinControlParent;


  TElemBase::Set_Parent(NewParent);
  if (FComponent) {
		if (NewParent) {
			ComponentParent = ((TElemVCL *) NewParent)->Component;
			WinControlParent = dynamic_cast<TWinControl *>(ComponentParent);
		}
		else {
			WinControlParent = NULL;
		}
		if (WinControlParent) {
			FComponent->Owner = WinControlParent;
		}
		Control = dynamic_cast<TWinControl *>(FComponent);
    if (Control && WinControlParent) {
      Control->Parent = WinControlParent;
    }
  }
	UpdateControlSizer();
	OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
TComponent * TElemVCL::Get_Component(void) {
  return FComponent;
}

//---------------------------------------------------------------------------
// Méthodes
//---------------------------------------------------------------------------

void TElemVCL::Invalidate(void) {
  TWinControl *WinControl;

  WinControl = dynamic_cast<TWinControl *>(FComponent);
  if (WinControl) {
    WinControl->Invalidate();
  }
}

//---------------------------------------------------------------------------
bool TElemVCL::UpdateControlSizer(void) {
  TControl *Control;

  // Mise à jour des dimensions à partir du contrôle
	// Parce que en fonction des ancrages ça peut changer suite à
	// un redimensionnement de l'objet parent
	Control = dynamic_cast<TControl *>(FComponent);
	if (Control) {
		FLeft = Control->Left;
		FTop = Control->Top;
		FWidth = Control->Width;
		FHeight = Control->Height;
	}

	return TElemBase::UpdateControlSizer();
}

//---------------------------------------------------------------------------
bool TElemVCL::UpdateInfos(AnsiString asInfos) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
	if (Control) {
		Control->ShowHint = true;
		Control->Hint = asInfos;
	  return true;
	}

  return false;
}

//---------------------------------------------------------------------------
bool TElemVCL::Create(AnsiString asTypeACreer) {
  TWinControl *WinControlParent;
  TComponent *ComponentOwner;
  TControl *Control;


  delete FComponent;
  FComponent = NULL;

  if (FParent) {
    ComponentOwner = ((TElemVCL *) FParent)->Component;
    WinControlParent = dynamic_cast<TWinControl *>(ComponentOwner);
  }
  else {
    ComponentOwner = NULL;
    WinControlParent = NULL;
  }

	if (asTypeACreer == _T("TForm")) {
		TForm_SaisieDialog *Form_SaisieDialog;
		Form_SaisieDialog = new TForm_SaisieDialog(NULL, _T("TForm_SaisieDialog"));
		shared_ptr_ControlSizer_Collection = Form_SaisieDialog->shared_ptr_ControlSizer_Collection;
		FComponent = Form_SaisieDialog;
		Form_SaisieDialog->ElemBase = this;
		TControlSizer_Collection *ControlSizers;
		ControlSizers = shared_ptr_ControlSizer_Collection.get();
		ControlSizers->Add(FControlSizer);
	}
	else {
		FComponent = (TComponent *) Factory_FVCL.CreateObject(ComponentOwner, asTypeACreer);
	}
	if (FComponent) bDeleteComponent = true;

  Control = dynamic_cast<TControl *>(FComponent);
  if (Control) {
    if (FParent) {
      Control->Parent = WinControlParent;
      // Control->Enabled = false;
    }
    Control->Visible = true;
  }

  return FComponent != NULL;
}

//---------------------------------------------------------------------------
TElemBase *TElemVCL::NewChild(void) {
	TElemVCL *NewElemVCL = new TElemVCL(this, NULL);
	NewElemVCL->Parent = this;

	return NewElemVCL;
}

//---------------------------------------------------------------------------
void TElemVCL::GetListProperties(TStrings *ListProperties) {
  ListProperties->Clear();
  if (FComponent) FComponent->GetListProperties(ListProperties);
}

//---------------------------------------------------------------------------
TYPEPROPERTY TElemVCL::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (FComponent) return FComponent->GetTypeProperty(asProperty, asInfos);
	return tpNull;
}

//---------------------------------------------------------------------------
AnsiString TElemVCL::GetDefaultProperty(AnsiString asProperty) {
  if (FComponent) return FComponent->GetDefaultProperty(asProperty);
	return _T("");
}

//---------------------------------------------------------------------------
AnsiString TElemVCL::GetProperty(AnsiString asProperty) {
  TForm *Form;


  if (asProperty == _T("Visible")) {
    if (FVisible) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ShowHint")) {
    if (FShowHint) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Hint")) {
    return FHint;
  }
  if (asProperty == _T("Enabled")) {
    if (FEnabled) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Position")) {
    Form = dynamic_cast<TForm *>(FComponent);
    if (Form) {
			switch (FPosition) {
				case poDesigned: return _T("poDesigned");
				case poDefault: return _T("poDefault");
				case poDefaultPosOnly: return _T("poDefaultPosOnly");
				case poDefaultSizeOnly: return _T("poDefaultSizeOnly");
				case poScreenCenter: return _T("poScreenCenter");
				case poDesktopCenter: return _T("poDesktopCenter");
				case poMainFormCenter: return _T("poMainFormCenter");
				case poOwnerFormCenter: return _T("poOwnerFormCenter");
			}
		}
  }
  if (asProperty == _T("ActiveControl")) {
    return asActiveControl;
  }
  if (asProperty == _T("OnMouseDown")) {
    return asOnMouseDown;
  }
  if (asProperty == _T("OnMouseUp")) {
    return asOnMouseUp;
  }
  if (asProperty == _T("OnMouseMove")) {
    return asOnMouseMove;
  }
  if (asProperty == _T("OnClick")) {
    return asOnClick;
  }
  if (asProperty == _T("OnPaint")) {
    return asOnPaint;
  }
  if (asProperty == _T("OnResize")) {
    return asOnResize;
  }
  if (asProperty == _T("OnClose")) {
    return asOnClose;
  }

  if (FComponent) return FComponent->GetProperty(asProperty);
	return _T("");
}

//---------------------------------------------------------------------------
bool TElemVCL::SetProperty(AnsiString asProperty, AnsiString asValue) {
  TForm *Form;


  // MessageBox(NULL, asValue, asProperty, MB_OK);
	OnChange(this);

  if (asProperty == _T("Visible")) {
    FVisible = (asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ShowHint")) {
    FShowHint = (asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Hint")) {
    FHint = asValue;
    return true;
  }
  if (asProperty == _T("Enabled")) {
    FEnabled = (asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Position")) {
    Form = dynamic_cast<TForm *>(FComponent);
    if (Form) {
      if (asValue == _T("poDesigned")) FPosition = poDesigned;
      if (asValue == _T("poDefault")) FPosition = poDefault;
      if (asValue == _T("poDefaultPosOnly")) FPosition = poDefaultPosOnly;
      if (asValue == _T("poDefaultSizeOnly")) FPosition = poDefaultSizeOnly;
      if (asValue == _T("poScreenCenter")) FPosition = poScreenCenter;
      if (asValue == _T("poDesktopCenter")) FPosition = poDesktopCenter;
      if (asValue == _T("poMainFormCenter")) FPosition = poMainFormCenter;
      if (asValue == _T("poOwnerFormCenter")) FPosition = poOwnerFormCenter;
	    return true;
    }
  }
	// Les propriétés "Name", "Left", "Top", "Width", "Height" doivent passer par
	// l'accesseur Set_XXXX (pour mise à jour de l'info-bulle).
  if (asProperty == _T("Name")) {
    return Set_Name(asValue);
  }
  if (asProperty == _T("Left")) {
    Form = dynamic_cast<TForm *>(FComponent);
		// $$$ Pas sûr que le test de la Form soit encore utile :
    if (!Form) {  // Les TForm doivent passer par SetProperty (à cause du décalage dû à la taille du bord)
	    return Set_Left(asValue.ToInt());
		}
  }
  if (asProperty == _T("Top")) {
    Form = dynamic_cast<TForm *>(FComponent);
    if (!Form) {
	    return Set_Top(asValue.ToInt());
		}
  }
  if (asProperty == _T("Width")) {
    Form = dynamic_cast<TForm *>(FComponent);
    if (!Form) {
	    return Set_Width(asValue.ToInt());
		}
  }
  if (asProperty == _T("Height")) {
    Form = dynamic_cast<TForm *>(FComponent);
    if (!Form) {
	    return Set_Height(asValue.ToInt());
		}
  }
  if (asProperty == _T("Anchors")) {
		if (FComponent) {
			TControl *Control;
			FComponent->SetProperty(asProperty, asValue, FComponent->Owner);
			Control = dynamic_cast<TControl *>(FComponent);
			if (Control) Set_Anchors(Control->Anchors);
		}
    return true;
  }
  if (asProperty == _T("ActiveControl")) {
    asActiveControl = asValue;
    return true;
  }
  if (asProperty == _T("OnMouseDown")) {
    asOnMouseDown = asValue;
    return true;
  }
  if (asProperty == _T("OnMouseUp")) {
    asOnMouseUp = asValue;
    return true;
  }
  if (asProperty == _T("OnMouseMove")) {
    asOnMouseMove = asValue;
    return true;
  }
  if (asProperty == _T("OnClick")) {
    asOnClick = asValue;
    return true;
  }
  if (asProperty == _T("OnPaint")) {
    asOnPaint = asValue;
    return true;
  }
  if (asProperty == _T("OnResize")) {
    asOnResize = asValue;
    return true;
  }
  if (asProperty == _T("OnClose")) {
    asOnClose = asValue;
    return true;
  }

	if (FComponent) {
		return FComponent->SetProperty(asProperty, asValue, FComponent->Owner);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TElemVCL::Show(bool bVisible) {
  TControl *Control;

  Control = dynamic_cast<TControl *>(FComponent);
	if (Control) {
		if (bVisible) Control->Show();
		else Control->Hide();
	}

  return true;
}

//---------------------------------------------------------------------------
AnsiString TElemVCL::ClassName(void) {
  AnsiString asRetour;


  if (FComponent) asRetour = FComponent->ClassName;
  return asRetour;
}

//---------------------------------------------------------------------------
bool TElemVCL::ClassNameIs(AnsiString asClasse) {
  if (FComponent) return FComponent->ClassNameIs(asClasse);

	return false;
}

//---------------------------------------------------------------------------
AnsiString TElemVCL::UnTruc(AnsiString asLigne, int * Cur, char Delim) {
  AnsiString asMot1, asMot2;
  int Deb, Fin, Len;


  // Mot + espaces
  Len = asLigne.Length();
  Deb = *Cur + 1;
  if (Deb > Len) return _T("");
  Fin = Deb;
  while (Fin <= Len && asLigne[Fin] != Delim) Fin++;
  asMot1 = asLigne.SubString(Deb, Fin - Deb);
  *Cur = Fin;

  // Suppression des espaces
  Len = asMot1.Length();
  Deb = 1;
  while (Deb <= Len && asMot1[Deb] == _T(' ')) Deb++;
  Fin = Len;
  while (Fin >= Deb && asMot1[Fin] == _T(' ')) Fin--;
  asMot2 = asMot1.SubString(Deb, Fin - Deb + 1);

  return asMot2;
}

//---------------------------------------------------------------------------
