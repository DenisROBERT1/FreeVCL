//-----------------------------------------------------------------------------
//! @file TControlBar.cpp
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
// TControlBar
//---------------------------------------------------------------------------

TControlBar::TControlBar(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TControlBar");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= RBS_AUTOSIZE | RBS_BANDBORDERS | RBS_VARHEIGHT;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         REBARCLASSNAME, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

    InitStdControl();
  }

}

//---------------------------------------------------------------------------
TControlBar::~TControlBar(void) {
}

//---------------------------------------------------------------------------
// Méthode appelée en fin de construction de la fiche
//---------------------------------------------------------------------------

void FASTCALL TControlBar::AfterConstruction(void) {
  REBARBANDINFO rbbi;
  TWinControl *WinControl;
  int i;


  for (i = 0; i < (int) FComponents.size(); i++) {

    WinControl = dynamic_cast<TWinControl *>(FComponents[i]);
    if (WinControl) {
      rbbi.cbSize = sizeof(REBARBANDINFO);
      rbbi.fMask = RBBIM_SIZE | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD;
      rbbi.fStyle = RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS | RBBS_CHILDEDGE;
      rbbi.cx = WinControl->Width;
      rbbi.cxMinChild = WinControl->Width;
      rbbi.cyMinChild = WinControl->Height;
      rbbi.cyMaxChild = WinControl->Height;
      rbbi.cyChild = 30;
      rbbi.cyIntegral = 30;
      rbbi.hwndChild = WinControl->Handle;
      SendMessage(FHandle, RB_INSERTBAND, (WPARAM) -1, (LPARAM) &rbbi);
    }

  }

}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TControlBar::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TControlBar::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TControlBar::GetDefaultProperty(AnsiString asProperty) {
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TControlBar::GetProperty(AnsiString asProperty) {
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TControlBar::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

