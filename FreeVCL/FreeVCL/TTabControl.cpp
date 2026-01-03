//-----------------------------------------------------------------------------
//! @file TTabControl.cpp
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
// TTabControl
//---------------------------------------------------------------------------

TTabControl::TTabControl(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TTabControl");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;

  FTabs = new TTabControlTabs();
  FTabIndex = -1;
  FTabStop = true;
	FTabOrder = 0;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_TABCONTROL, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    FTabs->Handle = FHandle;

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TTabControl::~TTabControl(void) {
  delete FTabs;
}

//---------------------------------------------------------------------------
int TTabControl::ProcessNotify(LPNMHDR pnmh) {
  int i;


  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
	case TCN_SELCHANGE:
    FTabIndex = (int) SendMessage(FHandle, TCM_GETCURSEL, 0, 0);
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    for (i = 0; i < (int) FComponents.size(); i++) {
      ((TControl *) FComponents[i])->Invalidate();
    }
    return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété TabIndex
//---------------------------------------------------------------------------

int TTabControl::Get_TabIndex(void) {
  return FTabIndex;
}

bool TTabControl::Set_TabIndex(int NewPageIndex) {

  if (FTabIndex != NewPageIndex) {
    FTabIndex = NewPageIndex;
    SendMessage(FHandle, TCM_SETCURSEL, FTabIndex, 0);
  }

  // Evènement
  if (!FComponentState.Contains(csLoading)) OnChange(this);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Tabs
//---------------------------------------------------------------------------

TStrings *TTabControl::Get_Tabs(void) {
  return FTabs;
}

bool TTabControl::Set_Tabs(TStrings *NewTabs) {

  if (FTabs != NewTabs) {
    FTabs->Assign(NewTabs);
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TTabControl::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Tabs.Strings"));
  ListProperties->Add(_T("TabIndex"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TTabControl::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Tabs.Strings")) {
    return tpStrings;
  }
  if (asProperty == _T("TabIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TTabControl::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Tabs.Strings")) {
    return _T("");
  }
  if (asProperty == _T("TabIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("OnChange")) {
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

AnsiString TTabControl::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Tabs.Strings")) {
    return FTabs->GetTextStr();
  }
  if (asProperty == _T("TabIndex")) {
    return IntToStr(Get_TabIndex());
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TTabControl::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Tabs.Strings")) {
    FTabs->SetTextStr(asValue);
    return true;
  }
  if (asProperty == _T("TabIndex")) {
    Set_TabIndex(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
// Méthode appelée en fin de construction de la fiche
//---------------------------------------------------------------------------

void FASTCALL TTabControl::AfterConstruction(void) {

  SendMessage(FHandle, TCM_SETCURSEL, FTabIndex, 0);

}


//---------------------------------------------------------------------------

