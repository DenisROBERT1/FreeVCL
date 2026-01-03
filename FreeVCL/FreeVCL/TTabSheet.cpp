//-----------------------------------------------------------------------------
//! @file TTabSheet.cpp
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
// TTabSheet
//---------------------------------------------------------------------------

TTabSheet::TTabSheet(TComponent* AOwner): TWinControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TTabSheet");
  FLeft = 0;
  FTop = 0;
  FWidth = 100;
  FHeight = 100;
  if (AOwner) {
    TWinControl *WinControl;
    WinControl = dynamic_cast<TWinControl *> (AOwner);
    if (WinControl) hWndParent = WinControl->Handle;
  }
  FAlign = alCustom;
  FTabStop = true;
  FTabOrder = 0;
  FPageIndex = -1;
  FPageControl = NULL;
	FCursor = crArrow;
	
	// Le TabSheet est invisible par défaut: il sera rendu visible par le premier Set_ActivePageIndex
	FVisible = false;
	FWindowStyle &= ~WS_VISIBLE;

  // Enregistrement de la classe "TTabSheet":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TTabSheet");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TTabSheet"), FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    if (AOwner) {
      TPageControl *PageControl;
      PageControl = dynamic_cast<TPageControl *> (AOwner);
      if (PageControl) Set_PageControl(PageControl);
    }
    bProcessCreateToDo = true;
  }

}

TTabSheet::~TTabSheet(void) {
  // Pour supprimer l'onglet du contrôle parent:
  Set_PageControl(NULL);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Left
//---------------------------------------------------------------------------

bool TTabSheet::Set_Left(int NewLeft) {
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Top
//---------------------------------------------------------------------------

bool TTabSheet::Set_Top(int NewTop) {
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

bool TTabSheet::Set_Width(int NewWidth) {
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

bool TTabSheet::Set_Height(int NewHeight) {
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

bool TTabSheet::Set_Caption(AnsiString NewCaption) {
  if (FCaption != NewCaption) {
    FCaption = NewCaption;
		SetWindowText(FHandle, FCaption);
    PageControl = dynamic_cast<TPageControl *> (FOwner);
    if (PageControl && FPageIndex != -1) {
      TCITEM TcItem;
      TcItem.mask = TCIF_TEXT;
      TcItem.pszText = (LPTSTR) (LPCTSTR) FCaption;
      HWND hWndParent = PageControl->Handle;
      SendMessage(hWndParent, TCM_SETITEM, (WPARAM) FPageIndex, (LPARAM) (LPTCITEM) &TcItem);
			PageControl->UpdateSizeTabSheets();
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PageIndex
//---------------------------------------------------------------------------

int TTabSheet::Get_PageIndex(void) {
  return FPageIndex;
}

bool TTabSheet::Set_PageIndex(int NewPageIndex) {

  if (NewPageIndex != FPageIndex) {
    FPageIndex = NewPageIndex;
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PageControl
//---------------------------------------------------------------------------

TPageControl* TTabSheet::Get_PageControl(void) {
  return (TPageControl*) FOwner;
}

bool TTabSheet::Set_PageControl(TPageControl* NewPageControl) {

  if (FPageControl != NewPageControl) {

    // Suppression de l'onglet de l'ancien PageControl
    if (FPageControl && FPageIndex != -1) {
      FPageControl->RemoveTab(FPageIndex);
    }

    FPageControl = NewPageControl;

    // Ajout de l'onglet dans le nouveau PageControl
    if (FPageControl) {
      if (FPageIndex == -1 || FPageIndex > FPageControl->PageCount) {
        FPageIndex = FPageControl->PageCount;
      }
      HWND hWndParent = FPageControl->Handle;
      if (hWndParent) SetParent(FHandle, hWndParent);
      FPageControl->InsertTab(FPageIndex, this);
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Rend visible la fenêtre.
//---------------------------------------------------------------------------

void FASTCALL TTabSheet::Show(void) {
	if (FPageControl) {
		FPageControl->ActivePageIndex = FPageIndex;
	}
	TWinControl::Show();
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TTabSheet::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("PageIndex"));
  ListProperties->Add(_T("PageControl"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TTabSheet::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("PageIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("PageControl")) {
    return tpObject;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TTabSheet::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("PageIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("PageControl")) {
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

AnsiString TTabSheet::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("PageIndex")) {
    return IntToStr(Get_PageIndex());
  }
  if (asProperty == _T("PageControl")) {
    TPageControl *PageControl = Get_PageControl();
    if (PageControl) return PageControl->Name;
    return _T("");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TTabSheet::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("PageIndex")) {
    Set_PageIndex(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("PageControl")) {
    TPageControl *PageControl;
		if (asValue.IsEmpty()) PageControl = NULL;
		else PageControl = dynamic_cast<TPageControl *>(FindRootComponent()->FindComponent(asValue));
    Set_PageControl(PageControl);
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

