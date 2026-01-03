//-----------------------------------------------------------------------------
//! @file TToolBar.cpp
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
// Déclarations anticipées
//---------------------------------------------------------------------------

class TImageList;


//---------------------------------------------------------------------------
// TToolBar
//---------------------------------------------------------------------------

TToolBar::TToolBar(TComponent* AOwner): TWinControl(AOwner) {

  // Initialisations
  FClassName = _T("TToolBar");
  FWindowStyle |= TBSTYLE_TRANSPARENT | TBSTYLE_FLAT | TBSTYLE_LIST |
		              CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER;
  FAlign = alTop;
  FTabStop = false;
  FFlat = false;
  FButtonWidth = 16;
  FButtonHeight = 16;
  FHotImages = NULL;
  FImages = NULL;

  // Enregistrement de la classe ToolBar
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_COOL_CLASSES;
  InitCommonControlsEx(&InitCtrls);


  bProcessCreateToDo = true;

}

//---------------------------------------------------------------------------
TToolBar::~TToolBar(void) {
}

//---------------------------------------------------------------------------
// Création effective de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL TToolBar::AfterConstruction(void) {
  TBBUTTON *TbButtons;
  TToolButton *ToolButton;
  int i, j;
  int NbComponents;
  HWND hWndParent = NULL;
  HBITMAP hBitmap = NULL;
  int iString;
  AnsiString asButtonHint;
  AnsiString asStrings;


  NbComponents = ComponentCount;
  TbButtons = new TBBUTTON[NbComponents];
  memset(TbButtons, 0, NbComponents * sizeof(TBBUTTON));

  iString = 0;
  for (i = 0, j = 0; i < NbComponents; i++) {
    if (Components[i]->ClassNameIs("TToolButton")) {
      ToolButton = (TToolButton *) Components[i];
      TbButtons[j].iBitmap = ToolButton->ImageIndex;
      TbButtons[j].idCommand = ToolButton->Command;
      if (ToolButton->Enabled) TbButtons[j].fsState = TBSTATE_ENABLED;
      else TbButtons[j].fsState = TBSTATE_INDETERMINATE;
      switch ((int) ToolButton->Style) {
        case tbsButton: TbButtons[j].fsStyle = TBSTYLE_BUTTON; break;
        case tbsCheck: TbButtons[j].fsStyle = TBSTYLE_CHECK; break;
        case tbsDropDown: TbButtons[j].fsStyle = TBSTYLE_DROPDOWN; break;
        case tbsSeparator: TbButtons[j].fsStyle = TBSTYLE_SEP; break;
        case tbsDivider: TbButtons[j].fsStyle = TBSTYLE_SEP; break;
      }
      TbButtons[j].dwData = 0;
      asButtonHint = ToolButton->Hint;
      if (ToolButton->ShowHint && !asButtonHint.IsEmpty()) {
				FWindowStyle |= TBSTYLE_TOOLTIPS;
        TbButtons[j].iString = iString++;
        asStrings += asButtonHint;
        asStrings += _T('\0');
      }
      else TbButtons[j].iString = -1;
      j++;
    }
  }
  asStrings += _T('\0');

  // Création de la barre d'outils
  if (FOwner) hWndParent = ((TControl *) FOwner)->Handle;
  if (FImages) hBitmap = FImages->GetImageBitmap();
  FHandle = CreateToolbarEx(hWndParent,
                            FWindowStyle,
                            FCommand, j,
                            NULL, (UINT_PTR) hBitmap,
                            TbButtons, j,
                            FButtonWidth, FButtonHeight,
                            FButtonWidth, FButtonHeight,
                            sizeof(TBBUTTON));
  delete[] TbButtons;

	if (FHandle) {

		// Infos bulles
		if (asStrings.Length() != 0) {
			SendMessage(FHandle, TB_SETEXTENDEDSTYLE, (WPARAM) 0, (LPARAM) TBSTYLE_EX_MIXEDBUTTONS);
			SendMessage(FHandle, TB_ADDSTRING, (WPARAM) 0, (LPARAM) (LPCTSTR) asStrings);
		}

	}

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Flat
//---------------------------------------------------------------------------

bool TToolBar::Get_Flat(void) {
  return FFlat;
}

bool TToolBar::Set_Flat(bool NewFlat) {

  if (FFlat != NewFlat) {
    FFlat = NewFlat;
    if (FFlat) FWindowStyle |= TBSTYLE_FLAT;
    else FWindowStyle &= ~TBSTYLE_FLAT;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ButtonWidth
//---------------------------------------------------------------------------

int TToolBar::Get_ButtonWidth(void) {
  return FButtonWidth;
}

bool TToolBar::Set_ButtonWidth(int NewButtonWidth) {

  if (FButtonWidth != NewButtonWidth) {
    FButtonWidth = NewButtonWidth;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ButtonHeight
//---------------------------------------------------------------------------

int TToolBar::Get_ButtonHeight(void) {
  return FButtonHeight;
}

bool TToolBar::Set_ButtonHeight(int NewButtonHeight) {

  if (FButtonHeight != NewButtonHeight) {
    FButtonHeight = NewButtonHeight;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HotImages
//---------------------------------------------------------------------------

TImageList *TToolBar::Get_HotImages(void) {
  return FHotImages;
}

bool TToolBar::Set_HotImages(TImageList * NewHotImages) {

  if (FHotImages != NewHotImages) {
    FHotImages = NewHotImages;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Images
//---------------------------------------------------------------------------

TImageList *TToolBar::Get_Images(void) {
  return FImages;
}

bool TToolBar::Set_Images(TImageList *NewImages) {

  if (FImages != NewImages) {
    FImages = NewImages;
  }
  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TToolBar::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Flat"));
  ListProperties->Add(_T("ButtonWidth"));
  ListProperties->Add(_T("ButtonHeight"));
  ListProperties->Add(_T("HotImages"));
  ListProperties->Add(_T("Images"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TToolBar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Flat")) {
    return tpBool;
  }
  if (asProperty == _T("ButtonWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("ButtonHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("HotImages")) {
    return tpObject;
  }
  if (asProperty == _T("Images")) {
    return tpObject;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TToolBar::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Flat")) {
    return _T("False");
  }
  if (asProperty == _T("ButtonWidth")) {
    return _T("16");
  }
  if (asProperty == _T("ButtonHeight")) {
    return _T("16");
  }
  if (asProperty == _T("HotImages")) {
    return _T("");
  }
  if (asProperty == _T("Images")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TToolBar::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Flat")) {
    if (Get_Flat()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ButtonWidth")) {
    return IntToStr(Get_ButtonWidth());
  }
  if (asProperty == _T("ButtonHeight")) {
    return IntToStr(Get_ButtonHeight());
  }
  if (asProperty == _T("HotImages")) {
    TImageList *HotImages = Get_HotImages();
    if (HotImages) return HotImages->Name;
    return _T("");
  }
  if (asProperty == _T("Images")) {
    TImageList *Images = Get_Images();
    if (Images) return Images->Name;
    return _T("");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TToolBar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Flat")) {
    Set_Flat(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ButtonWidth")) {
    FButtonWidth = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("ButtonHeight")) {
    FButtonHeight = StrToInt(asValue);
    return true;
  }
  if (asProperty == _T("HotImages")) {
    TImageList *ImageList;
		if (asValue.IsEmpty()) ImageList = NULL;
		else ImageList = dynamic_cast<TImageList *>(FindRootComponent()->FindComponent(asValue));
    Set_HotImages(ImageList);
    return true;
  }
  if (asProperty == _T("Images")) {
    TImageList *ImageList;
		if (asValue.IsEmpty()) ImageList = NULL;
		else ImageList = dynamic_cast<TImageList *>(FindRootComponent()->FindComponent(asValue));
    Set_Images(ImageList);
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

