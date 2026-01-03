//-----------------------------------------------------------------------------
//! @file TStatusBar.cpp
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
// TStatusBar
//---------------------------------------------------------------------------

TStatusBar::TStatusBar(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TStatusBar");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= SBARS_SIZEGRIP;
  FSimplePanel = true;
  FAlign = alBottom;
	NbPanels = 0;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         STATUSCLASSNAME, FCaption,
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
TStatusBar::~TStatusBar(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Panel
//---------------------------------------------------------------------------

int TStatusBar::Get_Panels(int Index) {
  return FPanels[Index];
}

bool TStatusBar::Set_Panels(int Index, int NewPanels) {
  if (FPanels[Index] != NewPanels) {
	  FPanels[Index] = NewPanels;
		if (NbPanels < Index + 1) NbPanels = Index + 1;
    SendMessage(FHandle, SB_SIMPLE, (WPARAM) FSimplePanel, 0L);
		if (!FSimplePanel) {
			SendMessage(FHandle, SB_SETPARTS, (WPARAM) NbPanels, (LPARAM) FPanels);
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SimplePanel
//---------------------------------------------------------------------------

bool TStatusBar::Get_SimplePanel(void) {
  return FSimplePanel;
}

bool TStatusBar::Set_SimplePanel(bool NewSimplePanel) {
  if (FSimplePanel != NewSimplePanel) {
    FSimplePanel = NewSimplePanel;
    SendMessage(FHandle, SB_SIMPLE, (WPARAM) FSimplePanel, 0L);
		if (!FSimplePanel) {
			SendMessage(FHandle, SB_SETPARTS, (WPARAM) NbPanels, (LPARAM) FPanels);
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SimpleText
//---------------------------------------------------------------------------

AnsiString TStatusBar::Get_SimpleText(void) {
  return FSimpleText;
}

bool TStatusBar::Set_SimpleText(AnsiString NewSimpleText) {
  if (FSimpleText != NewSimpleText) {
    FSimpleText = NewSimpleText;
		SendMessage(FHandle, SB_SETTEXT, (WPARAM) SB_SIMPLEID, (LPARAM) (LPCTSTR) FSimpleText);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PanelText
//---------------------------------------------------------------------------

AnsiString TStatusBar::Get_PanelText(int Index) {
  return FSimpleText;
}

bool TStatusBar::Set_PanelText(int Index, AnsiString NewPanelText) {
	if (FSimplePanel) {
		SendMessage(FHandle, SB_SETTEXT, (WPARAM) SB_SIMPLEID, (LPARAM) (LPCTSTR) NewPanelText);
	}
	else {
		SendMessage(FHandle, SB_SETTEXT, (WPARAM) Index, (LPARAM) (LPCTSTR) NewPanelText);
  }
  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TStatusBar::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Panels"));
  ListProperties->Add(_T("SimplePanel"));
  ListProperties->Add(_T("SimpleText"));
  ListProperties->Add(_T("PanelText"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TStatusBar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Panels")) {
    return tpNumber;
  }
  if (asProperty == _T("SimplePanel")) {
    return tpBool;
  }
  if (asProperty == _T("SimpleText")) {
    return tpText;
  }
  if (asProperty == _T("PanelText")) {
    return tpStrings;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TStatusBar::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Panels")) {
    return _T("");
  }
  if (asProperty == _T("SimplePanel")) {
    return _T("True");
  }
  if (asProperty == _T("SimpleText")) {
    return _T("");
  }
  if (asProperty == _T("PanelText")) {
    return _T("<>");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TStatusBar::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Panels")) {
  }
  if (asProperty == _T("SimplePanel")) {
    if (Get_SimplePanel()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("SimpleText")) {
    return Get_SimpleText();
  }
  if (asProperty == _T("PanelText")) {
    return Get_PanelText(0);
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TStatusBar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Panels")) {
    int Cur;
    AnsiString asMot;

		NbPanels = 0;
    Cur = 0;
		do {
      asMot = UnMot(asValue, _T("< "), _T(", >"), &Cur);
			if (asMot.IsEmpty()) break;
      FPanels[NbPanels++] = asMot.ToIntDef(0);
    } while (NbPanels < 256);
    return true;
	}
  if (asProperty == _T("SimplePanel")) {
    Set_SimplePanel(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("SimpleText")) {
    Set_SimpleText(asValue);
    return true;
  }
  if (asProperty == _T("PanelText")) {
    int Cur;
    AnsiString asMot;

		NbPanels = 0;
    Cur = 0;
		do {
      asMot = UnMot(asValue, _T("< "), _T(", >"), &Cur);
			if (asMot.IsEmpty()) break;
	    Set_PanelText(NbPanels, asMot);
    } while (NbPanels < 256);
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

