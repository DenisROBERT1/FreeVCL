//-----------------------------------------------------------------------------
//! @file THintWindow.cpp
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

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// THintWindow
//---------------------------------------------------------------------------

THintWindow::THintWindow(TComponent* AOwner): TWinControl(AOwner) {
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("THintWindow");
	hWndParent = NULL;
  FExWindowStyle = 0;
  FWindowStyle = TTS_ALWAYSTIP;
  FVisible = false;

  // Enregistrement de la classe ToolTips
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_BAR_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         TOOLTIPS_CLASS, _T(""),
                         FWindowStyle,
                         0, 0, 0, 0,
                         hWndParent,
                         (HMENU) 0, ::HInstance, NULL);

  if (FHandle) {
    SendMessage(FHandle, TTM_ACTIVATE, (WPARAM) TRUE, 0);
    SendMessage(FHandle, TTM_SETDELAYTIME, (WPARAM) TTDT_INITIAL,
                (LPARAM) (INT) MAKELONG(500,0));
    SendMessage(FHandle, TTM_SETDELAYTIME, (WPARAM) TTDT_AUTOPOP,
                (LPARAM) (INT) MAKELONG(5000,0));
    SendMessage(FHandle, TTM_SETDELAYTIME, (WPARAM) TTDT_RESHOW,
                (LPARAM) (INT) MAKELONG(50,0));
		// Le TTM_SETMAXTIPWIDTH est indispensable pour avoir des infos-bulles multilignes
    SendMessage(FHandle, TTM_SETMAXTIPWIDTH, (WPARAM) 0,
                (LPARAM) (INT) MAKELONG(GetSystemMetrics(SM_CXSCREEN),0));
		// $$$ faire une propertie balloon : ChangeWindowStyle(TTS_ALWAYSTIP | TTS_BALLOON);
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
THintWindow::~THintWindow(void) {
	FHandle = NULL;  // Il semble que Windows détruise automatiquement le HInt avec le contrôle parent
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool THintWindow::Set_Visible(bool NewVisible) {
  // On ne fait rien (cette fenêtre n'est jamais rendue visible par programme)

  return true;
}


//---------------------------------------------------------------------------
// Affichage de l'info-bulle
//---------------------------------------------------------------------------
void FASTCALL THintWindow::ActivateHint(const TRect &Rect, const AnsiString AHint) {
  // $$$ Non implémenté
}

//---------------------------------------------------------------------------
// Calcule la taille de l'info-bulle pour un texte donné
//---------------------------------------------------------------------------
TRect FASTCALL THintWindow::CalcHintRect(int MaxWidth, const AnsiString AHint, void * AData) {
	TRect HintRect;


  // $$$ Non implémenté
	HintRect.Left = 0;
	HintRect.Top = 0;
	HintRect.Right = 0;
	HintRect.Bottom = 0;

	return HintRect;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void THintWindow::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY THintWindow::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString THintWindow::GetDefaultProperty(AnsiString asProperty) {
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString THintWindow::GetProperty(AnsiString asProperty) {
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool THintWindow::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

