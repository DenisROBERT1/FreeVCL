//-----------------------------------------------------------------------------
//! @file TSpeedButton.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// TSpeedButton
//---------------------------------------------------------------------------

TSpeedButton::TSpeedButton(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TSpeedButton");
  FAllowAllUp = false;
  FGroupIndex = 0;
  FNumGlyph = 0;
  FGlyph = NULL;
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= BS_AUTOCHECKBOX | BS_PUSHLIKE;
  FTabStop = true;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_BUTTON, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TSpeedButton::~TSpeedButton(void) {
  delete FGlyph;
}

//---------------------------------------------------------------------------
bool TSpeedButton::ProcessCommand(WORD Notify) {
  int i;
  TSpeedButton *SpeedButton;


  if (Notify == BN_CLICKED) {
    if (GroupIndex != 0) {
      if (!FAllowAllUp && Down == false) {
        Down = true;
        return true;
      }
      for (i = 0; i < FParent->ComponentCount; i++) {
        if (FParent->Components[i]->ClassNameIs("TSpeedButton")) {
          SpeedButton = (TSpeedButton *) FParent->Components[i];
          if (SpeedButton != this && SpeedButton->FGroupIndex == FGroupIndex) {
            SpeedButton->Down = false;
          }
        }
      }
    }
		if (!FComponentState.Contains(csLoading)) {
			if (FAction) FAction->Execute();
			OnClick(this);
		}
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AllowAllUp
//---------------------------------------------------------------------------

bool TSpeedButton::Get_AllowAllUp(void) {
  return (FAllowAllUp);
}

bool TSpeedButton::Set_AllowAllUp(bool NewAllowAllUp) {

  if (FAllowAllUp != NewAllowAllUp) {
    FAllowAllUp = NewAllowAllUp;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Down
//---------------------------------------------------------------------------

bool TSpeedButton::Get_Down(void) {
  return (SendMessage(FHandle, BM_GETCHECK, 0, 0) == BST_CHECKED);
}

bool TSpeedButton::Set_Down(bool NewDown) {
  if (FHandle) {
    if (NewDown) SendMessage(FHandle, BM_SETCHECK, BST_CHECKED, 0);
    else SendMessage(FHandle, BM_SETCHECK, BST_UNCHECKED, 0);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Flat
//---------------------------------------------------------------------------

bool TSpeedButton::Get_Flat(void) {
  return ((FWindowStyle & BS_FLAT) != 0);
}

bool TSpeedButton::Set_Flat(bool NewFlat) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewFlat) NewWindowStyle |= BS_FLAT;
  else NewWindowStyle &= ~BS_FLAT;
  if (FWindowStyle != NewWindowStyle) {
    FWindowStyle = NewWindowStyle;
    SendMessage(FHandle, BM_SETSTYLE, (WPARAM) FWindowStyle, MAKELPARAM(TRUE, 0));
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété GroupIndex
//---------------------------------------------------------------------------

int TSpeedButton::Get_GroupIndex(void) {
  return FGroupIndex;
}

bool TSpeedButton::Set_GroupIndex(int NewGroupIndex) {

  if (FGroupIndex != NewGroupIndex) {
    FGroupIndex = NewGroupIndex;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NumGlyph
//---------------------------------------------------------------------------

int TSpeedButton::Get_NumGlyph(void) {
  return FNumGlyph;
}

bool TSpeedButton::Set_NumGlyph(int NewNumGlyph) {

	if (FNumGlyph != NewNumGlyph) {
		FNumGlyph = NewNumGlyph;
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Glyph
//---------------------------------------------------------------------------

TBitmap * TSpeedButton::Get_Glyph(void) {
  return FGlyph;
}

bool TSpeedButton::Set_Glyph(TBitmap *NewGlyph) {
  HBITMAP hBitmap;


	if (NewGlyph) {
		if (!FGlyph) FGlyph = (TBitmap *) new TBitmap();
		FGlyph->Assign(NewGlyph);
	  ChangeWindowStyle(FWindowStyle | BS_BITMAP);
		if (FHandle) {
			hBitmap = FGlyph->Handle;
			SendMessage(FHandle, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hBitmap);
		}
	}
	else {
		delete FGlyph;
		FGlyph = NULL;
	  ChangeWindowStyle(FWindowStyle & ~BS_BITMAP);
	}

	return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TSpeedButton::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("AllowAllUp"));
  ListProperties->Add(_T("Down"));
  ListProperties->Add(_T("Flat"));
  ListProperties->Add(_T("GroupIndex"));
  ListProperties->Add(_T("NumGlyph"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TSpeedButton::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("AllowAllUp")) {
    return tpBool;
  }
  if (asProperty == _T("Down")) {
    return tpBool;
  }
  if (asProperty == _T("Flat")) {
    return tpBool;
  }
  if (asProperty == _T("GroupIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("NumGlyph")) {
    return tpNumber;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TSpeedButton::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("AllowAllUp")) {
    return _T("False");
  }
  if (asProperty == _T("Down")) {
    return _T("False");
  }
  if (asProperty == _T("Flat")) {
    return _T("False");
  }
  if (asProperty == _T("GroupIndex")) {
    return _T("0");
  }
  if (asProperty == _T("NumGlyph")) {
    return _T("");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TSpeedButton::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("AllowAllUp")) {
    if (Get_AllowAllUp()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Down")) {
    if (Get_Down()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Flat")) {
    if (Get_Flat()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("GroupIndex")) {
    return IntToStr(Get_GroupIndex());
  }
  if (asProperty == _T("NumGlyph")) {
    return IntToStr(Get_NumGlyph());
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TSpeedButton::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("AllowAllUp")) {
    Set_AllowAllUp(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Down")) {
    Set_Down(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Flat")) {
    Set_Flat(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("GroupIndex")) {
    FGroupIndex = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("NumGlyph")) {
    FNumGlyph = StrToInt(asValue);
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

