//-----------------------------------------------------------------------------
//! @file TForm.cpp
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

//---------------------------------------------------------------------------
// TForm
//---------------------------------------------------------------------------

TForm::TForm(HWND HandleParent, const TCHAR * szName, TComponent* ListComponents[]):
					 TFormFrame(NULL) {
	FHandleParent = HandleParent;
	CreateForm(szName, ListComponents);
}

TForm::TForm(TComponent* AOwner):
					 TFormFrame(AOwner) {
	// Initialisation
	TWinControl *Parent = dynamic_cast<TWinControl *>(AOwner);
	if (Parent) FHandleParent = Parent->Handle;
	else FHandleParent = NULL;

	CreateForm(NULL, NULL);
}

TForm::TForm(HWND HandleParent):
					 TFormFrame(NULL) {
	FHandleParent = HandleParent;
	CreateForm(NULL, NULL);
}

TForm::TForm(TComponent* AOwner, const TCHAR * szName,
						 TComponent* ListComponents[]):
					 TFormFrame(AOwner) {
	// Initialisation
	TWinControl *Parent = dynamic_cast<TWinControl *>(AOwner);
	if (Parent) FHandleParent = Parent->Handle;
	else FHandleParent = NULL;

	CreateForm(szName, ListComponents);
}

//---------------------------------------------------------------------------
TForm::~TForm(void) {
}

//---------------------------------------------------------------------------
bool TForm::CreateForm(const TCHAR * szName, TComponent *ListComponents[]) {
	WNDCLASS wc;
	bool Ok = false;
	HFONT hFont;
	RECT WindowRect;


	// Initialisations
	FClassName = _T("TForm");
	FWindowStyle = WS_OVERLAPPEDWINDOW;
	FLeft = CW_USEDEFAULT;
	FTop = CW_USEDEFAULT;
	FWidth = CW_USEDEFAULT;
	FHeight = CW_USEDEFAULT;
	FAlphaBlendValue = 255;
	FModalResult = mrNone;
	FPosition = poDefault;
	FWindowState = wsNormal;
	FFormActivation = true;
	FFormState = 0;
	FFormStyle = fsNormal;
	FBorderStyle = bsSizeable;
	FBorderIcons.Clear() << biSystemMenu << biMinimize << biMaximize;
	FVisible = true; // Pour que les descendants aient la propriété Visible
	FDefaultCloseAction = caFree;
	FMenu = NULL;

	// Enregistrement de la classe "TForm":
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("TForm");

	RegisterClass(&wc);

	// Création de la fenêtre
	FHandle = CreateWindowEx(FExWindowStyle,
													 _T("TForm"), FCaption,
													 FWindowStyle,
													 FLeft, FTop,
													 FWidth, FHeight,
													 FHandleParent,
													 NULL, HInstance, this);

	// Relecture de la vraie position de la fenêtre
	// (CW_USEDEFAULT a pour valeur -1)
	if (FLeft == (int) CW_USEDEFAULT ||
			FTop == (int) CW_USEDEFAULT ||
			FWidth == (int) CW_USEDEFAULT ||
			FHeight == (int) CW_USEDEFAULT) {
		GetWindowRect(FHandle, &WindowRect);
		FLeft = WindowRect.left;
		FTop = WindowRect.top;
		FWidth = WindowRect.right - WindowRect.left;
		FHeight = WindowRect.bottom - WindowRect.top;
	}

	if (FHandle) {
		Ok = true;

		hFont = FFont->Handle;
		SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
	}

	if (Ok && szName) {
		Ok = LoadDialog(szName, ListComponents);
	}

	if (Ok) {
		bWantPaint = true;
		Application->MainForm = this;
		// La fenêtre principale est toujours modale
		if (Application->MainForm == this) FFormState << fsModal;
		bProcessCreateToDo = true;
	}

	return true;
}

//---------------------------------------------------------------------------
// Recherche des fenêtres enfants interceptant la touche entrée
//---------------------------------------------------------------------------

TComponent * TForm::ChildWantReturns(void) {

	// Si c'est une fenêtre non modale => on a besoin d'intercepter la touche entrée
	if (!	FFormState.Contains(fsModal)) return this;

  return TWinControl::ChildWantReturns();
}

//---------------------------------------------------------------------------
// Recherche des fenêtres enfants interceptant la touche tabulation
//---------------------------------------------------------------------------

TComponent * TForm::ChildWantTabs(void) {

	// Si c'est une fenêtre non modale => on a besoin d'intercepter la touche tabulation
	if (!	FFormState.Contains(fsModal)) return this;

	return TWinControl::ChildWantTabs();
}

//---------------------------------------------------------------------------
// Création effective de la fenêtre
//---------------------------------------------------------------------------

void FASTCALL TForm::AfterConstruction(void) {
	bWantPaint = true;
	Application->MainForm = this;
	if (FVisible) {
		FVisible = false;  // A ce stade, la fiche est effectivement invisible
		Set_Visible(true);	// Provoque l'affichage de la fiche
	}
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Visible
//---------------------------------------------------------------------------

bool TForm::Set_Visible(bool NewVisible) {

	if (FVisible != NewVisible) {
    FVisible = NewVisible;
		TWindowState MemoWindowState = FWindowState;  // Mémorisation avant Set_Visible parce que
																									// celui-ci provoque un Size (donc modifie FWindowState)
		if (FHandle) {
			if (FVisible && (FWindowStyle & WS_VISIBLE) == 0) {
				if (MemoWindowState == wsMinimized) {
					ShowWindow(FHandle, SW_MINIMIZE);
				}
				else if (MemoWindowState == wsMaximized) {
					ShowWindow(FHandle, SW_MAXIMIZE);
				}
				else if (FFormActivation) {
					ShowWindow(FHandle, SW_SHOW);
				}
				else {
					ShowWindow(FHandle, SW_SHOWNOACTIVATE);
				}
				FWindowStyle |= WS_VISIBLE;
				FModalResult = mrNone;   // Pour les fenêtres qui ne sont pas détruites sur le Close (caHide),
				                         // et le Close ayant été provoqué par un ModalResult
			}
			else if (!FVisible && (FWindowStyle & WS_VISIBLE) != 0) {
				ShowWindow(FHandle, SW_HIDE);
				FWindowStyle &= ~WS_VISIBLE;
			}
			if (!FVisible) KillCurrentFocus();
		}
	}

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Left
// La position de la fenêtre est mémorisée pour une config standard, mais
// la position réelle dépend du thème Windows.
//---------------------------------------------------------------------------

int TForm::Get_Left(void) {
  return FLeft + GetSystemMetrics(SM_CXSIZEFRAME) - 4;
}

bool TForm::Set_Left(int NewLeft) {
  return TWinControl::Set_Left(NewLeft - GetSystemMetrics(SM_CXSIZEFRAME) + 4);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Top
//---------------------------------------------------------------------------

int TForm::Get_Top(void) {
  return FTop + GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYCAPTION) - 23;
}

bool TForm::Set_Top(int NewTop) {
  return TWinControl::Set_Top(NewTop - GetSystemMetrics(SM_CYBORDER) - GetSystemMetrics(SM_CYCAPTION) + 23);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TForm::Get_Width(void) {
  return FWidth - 2 * GetSystemMetrics(SM_CXSIZEFRAME) + 8;
}

bool TForm::Set_Width(int NewWidth) {
  return TWinControl::Set_Width(NewWidth + 2 * GetSystemMetrics(SM_CXSIZEFRAME) - 8);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TForm::Get_Height(void) {
  return FHeight - 2 * GetSystemMetrics(SM_CYSIZEFRAME) - GetSystemMetrics(SM_CYCAPTION) + 27;
}

bool TForm::Set_Height(int NewHeight) {
  return TWinControl::Set_Height(NewHeight + 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) - 27);
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Active
//---------------------------------------------------------------------------

bool TForm::Get_Active(void) {
	return GetActiveWindow() == FHandle;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ActiveControl
//---------------------------------------------------------------------------

TWinControl * TForm::Get_ActiveControl(void) {
	return GetFocus();
}

bool TForm::Set_ActiveControl(TWinControl * NewActiveControl) {

	if (NewActiveControl) NewActiveControl->SetFocus();
	else ::SetFocus(NULL);

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AlphaBlend
//---------------------------------------------------------------------------

bool TForm::Get_AlphaBlend(void) {
	return ((FExWindowStyle & WS_EX_LAYERED) != 0);
}

bool TForm::Set_AlphaBlend(bool NewAlphaBlend) {
  DWORD NewExWindowStyle;


  if (FHandle) {
    NewExWindowStyle = (FExWindowStyle & ~(WS_EX_LAYERED));
    if (NewAlphaBlend) {
      NewExWindowStyle |= WS_EX_LAYERED;
    }
    ChangeExWindowStyle(NewExWindowStyle);
		if (FExWindowStyle & WS_EX_LAYERED) Set_AlphaBlendValue(FAlphaBlendValue);
  }

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AlphaBlendValue
//---------------------------------------------------------------------------

Byte TForm::Get_AlphaBlendValue(void) {
	return FAlphaBlendValue;
}

bool TForm::Set_AlphaBlendValue(Byte NewAlphaBlendValue) {

	if (FAlphaBlendValue != NewAlphaBlendValue) {
		FAlphaBlendValue = NewAlphaBlendValue;
		if (FHandle && (FExWindowStyle & WS_EX_LAYERED)) {
			SetLayeredWindowAttributes(FHandle, 0, FAlphaBlendValue, ULW_ALPHA);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ModalResult
//---------------------------------------------------------------------------

TModalResult TForm::Get_ModalResult(void) {
	return FModalResult;
}

bool TForm::Set_ModalResult(TModalResult NewModalResult) {

	if (FModalResult != NewModalResult) {
		FModalResult = NewModalResult;
		Close();
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

TPosition TForm::Get_Position(void) {
	return FPosition;
}

bool TForm::Set_Position(TPosition NewPosition) {
	HWND HandleTop = NULL;
	RECT RectTop, RectWnd, RectScreen;
	int FLeftWnd;
	int FTopWnd;


	if (FPosition != NewPosition) {
		FPosition = NewPosition;
		FLeftWnd = FLeft;
		FTopWnd = FTop;
		switch (FPosition) {
		case poDesigned:
			return true;
		case poDefault:
			return true;
		case poDefaultPosOnly:
			return true;
		case poDefaultSizeOnly:
			return true;
		case poScreenCenter:
			HandleTop = ::GetDesktopWindow();
			break;
		case poDesktopCenter:
			HandleTop = ::GetDesktopWindow();
			break;
		case poMainFormCenter:
			HandleTop = Application->MainForm->Handle;
			break;
		case poOwnerFormCenter:
			HandleTop = FHandleParent;
			break;
		}
		if (HandleTop && FHandle) {
			GetWindowRect(HandleTop, &RectTop);
			GetWindowRect(FHandle, &RectWnd);
			FLeftWnd = (RectTop.right + RectTop.left) / 2 - (RectWnd.right - RectWnd.left) / 2;
			FTopWnd = (RectTop.bottom + RectTop.top) / 2 - (RectWnd.bottom - RectWnd.top) / 2;

			// Test pour que la fenêtre s'affiche dans l'écran
			// Ne marche pas en cas d'écrans multiples :
			// GetWindowRect(GetDesktopWindow(), &RectScreen);
			RectScreen.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
			RectScreen.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
			RectScreen.right = RectScreen.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
			RectScreen.bottom = RectScreen.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
			if (FLeftWnd < RectScreen.left) FLeftWnd = RectScreen.left;
			if (FTopWnd < RectScreen.top) FTopWnd = RectScreen.top;
			if (FLeftWnd + RectWnd.right - RectWnd.left > RectScreen.right)
					FLeftWnd = RectScreen.right - RectWnd.right + RectWnd.left;
			if (FTopWnd + RectWnd.bottom - RectWnd.top > RectScreen.bottom)
					FTopWnd = RectScreen.bottom - RectWnd.bottom + RectWnd.top;

			MoveWindow(FHandle, FLeftWnd, FTopWnd, FWidth, FHeight, FALSE);
		}
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WindowState
//---------------------------------------------------------------------------

TWindowState TForm::Get_WindowState(void) {
	return FWindowState;
}

bool TForm::Set_WindowState(TWindowState NewWindowState) {

	if (FWindowState != NewWindowState) {
		FWindowState = NewWindowState;
		if (FHandle && IsWindowVisible(FHandle)) {
			if (FWindowState == wsNormal) {
				ShowWindow(FHandle, SW_RESTORE);
			}
			else if (FWindowState == wsMinimized) {
				ShowWindow(FHandle, SW_MINIMIZE);
			}
			else if (FWindowState == wsMaximized) {
				ShowWindow(FHandle, SW_MAXIMIZE);
			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FormActivation
//---------------------------------------------------------------------------

bool TForm::Get_FormActivation(void) {
	return FFormActivation;
}

bool TForm::Set_FormActivation(bool NewFormActivation) {
	DWORD NewExWindowStyle;


	if (FFormActivation != NewFormActivation) {
		FFormActivation = NewFormActivation;
		if (FHandle) {
			if (FFormActivation) {
				NewExWindowStyle = FExWindowStyle & ~WS_EX_NOACTIVATE;
			}
			else {
				NewExWindowStyle = FExWindowStyle | WS_EX_NOACTIVATE;
			}
			ChangeExWindowStyle(NewExWindowStyle);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FormState
//---------------------------------------------------------------------------

TFormState TForm::Get_FormState(void) {
	return FFormState;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FormStyle
//---------------------------------------------------------------------------

TFormStyle TForm::Get_FormStyle(void) {
	return FFormStyle;
}

bool TForm::Set_FormStyle(TFormStyle NewFormStyle) {
	DWORD NewExWindowStyle;


	if (FFormStyle != NewFormStyle) {
		FFormStyle = NewFormStyle;
		if (FHandle) {
			NewExWindowStyle = (FExWindowStyle & ~(WS_EX_MDICHILD | WS_EX_TOPMOST));
			if (FFormStyle == fsMDIChild) {
				NewExWindowStyle |= WS_EX_MDICHILD;
			}
			else if (FFormStyle == fsMDIForm) {
			}
			else if (FFormStyle == fsStayOnTop) {
				NewExWindowStyle |= WS_EX_TOPMOST;
			}
			ChangeExWindowStyle(NewExWindowStyle);

			if (FFormStyle == fsStayOnTop) {
				SetWindowPos(FHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
			else {
				SetWindowPos(FHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}

		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderIcons
//---------------------------------------------------------------------------

TBorderIcons TForm::Get_BorderIcons(void) {
	return FBorderIcons;
}

bool TForm::Set_BorderIcons(TBorderIcons NewBorderIcons) {
	DWORD NewWindowStyle;
	DWORD NewExWindowStyle;


	if (FBorderIcons != NewBorderIcons) {
		FBorderIcons = NewBorderIcons;
		if (FHandle) {
			NewWindowStyle = FWindowStyle & ~(WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			NewExWindowStyle = (FExWindowStyle & ~WS_EX_CONTEXTHELP);
			if (FBorderIcons.Contains(biSystemMenu)) NewWindowStyle |= WS_SYSMENU;
			if (FBorderIcons.Contains(biMinimize)) NewWindowStyle |= WS_MINIMIZEBOX;
			if (FBorderIcons.Contains(biMaximize)) NewWindowStyle |= WS_MAXIMIZEBOX;
			if (FBorderIcons.Contains(biHelp)) NewExWindowStyle |= WS_EX_CONTEXTHELP;
			ChangeWindowStyle(NewWindowStyle);
			ChangeExWindowStyle(NewExWindowStyle);
			if (NewWindowStyle & WS_SYSMENU) {
				HMENU hSystemMenu = GetSystemMenu(FHandle, FALSE);
				if (hSystemMenu) {
					EnableMenuItem(hSystemMenu, SC_MINIMIZE,
							(NewWindowStyle & WS_MINIMIZEBOX)?
							(MF_BYCOMMAND | MF_ENABLED): (MF_BYCOMMAND | MF_GRAYED));
					EnableMenuItem(hSystemMenu, SC_MAXIMIZE,
							(NewWindowStyle & WS_MAXIMIZEBOX)?
							(MF_BYCOMMAND | MF_ENABLED): (MF_BYCOMMAND | MF_GRAYED));
				}
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TFormBorderStyle TForm::Get_BorderStyle(void) {
	return FBorderStyle;
}

bool TForm::Set_BorderStyle(TFormBorderStyle NewBorderStyle) {
	DWORD NewWindowStyle;
	DWORD NewExWindowStyle;


	if (FBorderStyle != NewBorderStyle) {
		FBorderStyle = NewBorderStyle;
		if (FHandle) {
			NewWindowStyle = (FWindowStyle & ~(WS_CAPTION | WS_BORDER | WS_SIZEBOX));
			NewExWindowStyle = (FExWindowStyle & ~(WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME));
			if (FBorderStyle == bsNone) {
				NewWindowStyle |= WS_POPUP;
			}
			else if (FBorderStyle == bsSingle) {
				NewWindowStyle |= WS_POPUP | WS_BORDER;
			}
			else if (FBorderStyle == bsSizeable) {
				NewWindowStyle |= WS_BORDER | WS_SIZEBOX;
			}
			else if (FBorderStyle == bsDialog) {
				NewWindowStyle |= WS_CAPTION | WS_BORDER;
				NewExWindowStyle |= WS_EX_DLGMODALFRAME;
			}
			else if (FBorderStyle == bsToolWindow) {
				NewWindowStyle |= WS_CAPTION | WS_BORDER;
				NewExWindowStyle |= WS_EX_DLGMODALFRAME | WS_EX_TOOLWINDOW;
			}
			else if (FBorderStyle == bsSizeToolWin) {
				NewWindowStyle |= WS_CAPTION | WS_BORDER | WS_SIZEBOX;
				NewExWindowStyle |= WS_EX_DLGMODALFRAME | WS_EX_TOOLWINDOW;
			}
			ChangeWindowStyle(NewWindowStyle);
			ChangeExWindowStyle(NewExWindowStyle);
			MoveWindow(FHandle, FLeft, FTop, FWidth - 1, FHeight - 1, FALSE);
			MoveWindow(FHandle, FLeft, FTop, FWidth, FHeight, TRUE);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DefaultCloseAction
//---------------------------------------------------------------------------

TCloseAction TForm::Get_DefaultCloseAction() {
	return FDefaultCloseAction;
}

bool TForm::Set_DefaultCloseAction(TCloseAction NewDefaultCloseAction) {
	if (FDefaultCloseAction != NewDefaultCloseAction) {
		FDefaultCloseAction = NewDefaultCloseAction;
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Menu
//---------------------------------------------------------------------------

TMainMenu *TForm::Get_Menu(void) {
	return FMenu;
}

bool TForm::Set_Menu(TMainMenu *NewMenu) {

	if (FMenu != NewMenu) {
		FMenu = NewMenu;
		if (FMenu) SetMenu(FHandle, FMenu->Handle);
		else SetMenu(FHandle, NULL);
	}
	return true;
}

//---------------------------------------------------------------------------
// Redimensionne la fenêtre en une seule opération
//---------------------------------------------------------------------------
void TForm::Size(int NewLeft, int NewTop, int NewWidth, int NewHeight, bool bForceResize) {

	NewLeft += - GetSystemMetrics(SM_CXSIZEFRAME) + 4;
  NewTop += - GetSystemMetrics(SM_CYBORDER) - GetSystemMetrics(SM_CYCAPTION) + 23;
  NewWidth += 2 * GetSystemMetrics(SM_CXSIZEFRAME) - 8;
  NewHeight += 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) - 27;

	TWinControl::Size(NewLeft, NewTop, NewWidth, NewHeight, bForceResize);
}

//---------------------------------------------------------------------------
// Rend le contrôle visible
//---------------------------------------------------------------------------
void FASTCALL TForm::Show(void) {
	Set_Visible(true);
	BringWindowToTop(FHandle);
}

//---------------------------------------------------------------------------
bool TForm::InterceptionMessage(MSG &msg) {
	TShiftState Shift;
	TComponent *ComponentInterception;
	TForm *FormInterception;


	// F10 est intercepté par le système et le WM_KEYDOWN n'est jamais reçu
	// => il faut tester WM_SYSKEYDOWN pour cette touche uniquement
	if (msg.message == WM_KEYDOWN || (msg.wParam == VK_F10 && msg.message == WM_SYSKEYDOWN)) {
		Shift.Clear();
		if (GetKeyState(VK_MENU) & 0x8000) Shift << ssAlt;
		if (GetKeyState(VK_SHIFT) & 0x8000) Shift << ssShift;
		if (GetKeyState(VK_CONTROL) & 0x8000) Shift << ssCtrl;
		if (DistributionShortCut((int) msg.wParam, Shift)) return true;
		if (msg.wParam == VK_TAB) {
			ComponentInterception = ChildWantTabs();
			if (ComponentInterception != NULL) {
				FormInterception = dynamic_cast<TForm *>(ComponentInterception);
				if (FormInterception != NULL) {
					// Fenêtre non modale enfant de this
					return FormInterception->TraitementTabulation();
				}
			}
			else {
				return TraitementTabulation();
			}
		}
		if (msg.wParam == VK_RETURN) {
			ComponentInterception = ChildWantReturns();
			if (ComponentInterception != NULL) {
				FormInterception = dynamic_cast<TForm *>(ComponentInterception);
				if (FormInterception != NULL) {
					// Fenêtre non modale enfant de this
					return FormInterception->TraitementEntree();
				}
			}
			else {
				// Si le composant est un TMemo multiligne (=> ClassName == "Edit" ET WindowStyle contient ES_WANTRETURN)
				// Alors on ne traite pas la touche entrée (elle sera gérée par le composant)
				// Dans tous les autres cas, elle est traitée
				TCHAR szClassName[30];
				DWORD WindowStyle;
				int Ret = ::GetClassName(msg.hwnd, szClassName, sizeof(szClassName));
				WindowStyle = (DWORD) (SIZE_T) GetWindowLongPtr(msg.hwnd, GWL_STYLE);
				if (Ret == 0 || 	// Si Ret == 0, szClassName est sûrement plus grand que 30 => ce n'est pas "Edit"
						lstrcmpi(szClassName, WC_EDIT) || 	// Edit ou Memo
						(WindowStyle & ES_WANTRETURN) == 0) {
					return TraitementEntree();
				}
			}
		}
		if (msg.wParam == VK_ESCAPE) {
			return TraitementEscape();
		}
		// 12/08/2001 Devrait être traité par le SendMessage
		// return DistributionKeyDown(msg.hwnd, (int) msg.wParam);
	}
	/*
	else if (msg.message == WM_KEYUP) {
		return DistributionKeyUp(msg.hwnd, (int) msg.wParam);
	}
	else if (msg.message == WM_CHAR) {
		return DistributionChar(msg.hwnd, (int) msg.wParam);
	} */

	return false;
}

//---------------------------------------------------------------------------
bool TForm::TraitementTabulation(void) {
	if ((GetKeyState(VK_SHIFT) & 0x8000) != 0) {
		if (!SetFocusOnNextControl(false)) {
			// On a atteint le premier contrôle => on boucle sur le dernier
			CurrentFocus = -1;
			SetFocusOnNextControl(false);
		}
	}
	else {
		if (!SetFocusOnNextControl(true)) {
			// On a atteint le dernier contrôle => on boucle sur le premier
			CurrentFocus = -1;
			SetFocusOnNextControl(true);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool TForm::TraitementEntree(void) {
	int i;


	for (i = 0; i < (int) FComponents.size(); i++) {
		if (FComponents[i]->ClassName == _T("TButton")) {
			if (((TButton *) FComponents[i])->Default) {
				DistributionCommand(FComponents[i]->Command, BN_CLICKED);
				return true;
			}
		}
	}

	return false;
}

//---------------------------------------------------------------------------
bool TForm::TraitementEscape(void) {
	int i;


	for (i = 0; i < (int) FComponents.size(); i++) {
		if (FComponents[i]->ClassName == _T("TButton")) {
			if (((TButton *) FComponents[i])->Cancel) {
				DistributionCommand(FComponents[i]->Command, BN_CLICKED);
				return true;
			}
		}
	}

	return false;
}

//---------------------------------------------------------------------------
TModalResult TForm::ShowModal(void) {
	MSG msg;


	// Fenêtre parente invalide
	EnableWindow(FHandleParent, FALSE);

	// Pour pouvoir lire les informations après le retour du ShowModal
	FDefaultCloseAction = caHide;

	// Etat de la fenêtre
	FFormState << fsModal;

	FVisible = false;  // A ce stade, la fiche est effectivement invisible
	Set_Visible(true);

	// Initialisation du focus courant
	InitFocus();
	SetFocusOnNextControl(true);

	// Boucle de messages:
	while (FHandle && IsWindowVisible(FHandle) && GetMessage(&msg, 0, 0, 0)) {
		if (!InterceptionMessage(msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	EnableWindow(FHandleParent, TRUE);
	SetForegroundWindow(FHandleParent);
	SetActiveWindow(FHandleParent);

	return FModalResult;
}

//---------------------------------------------------------------------------
bool TForm::Close(void) {
	if (FHandle) SendMessage(FHandle, WM_CLOSE, 0, 0);

	FFormState >> fsModal;

	return false;
}

//---------------------------------------------------------------------------
void TForm::ProcessCreate(void) {
	OnCreate(this);
}

//---------------------------------------------------------------------------
bool TForm::ProcessPaint(HDC hdc, RECT rcPaint) {
	HBRUSH hBrush;


	hBrush = (HBRUSH) CreateSolidBrush(FColor.cr);
	FillRect(hdc, &rcPaint, hBrush);
	DeleteObject(hBrush);

	if (!FComponentState.Contains(csLoading)) OnPaint(this);

	return false;
}

//---------------------------------------------------------------------------
bool TForm::ProcessMove(int X, int Y) {
	RECT Rect;


	GetWindowRect(FHandle, &Rect);	// Car Move donne les coordonnées du rectangle CLIENT
	FLeft = Rect.left;
	FTop = Rect.top;

	return true;
}


//---------------------------------------------------------------------------
bool TForm::ProcessSize(int SizeType, int L, int H) {

	if (SizeType == SIZE_RESTORED) {
		FWindowState = wsNormal;
		if (Application->MainForm == this) Application->OnRestore(this);
	}
	else if (SizeType == SIZE_MAXIMIZED) {
		FWindowState = wsMaximized;
		if (Application->MainForm == this) Application->OnRestore(this);
	}
	else if (SizeType == SIZE_MINIMIZED) {
		FWindowState = wsMinimized;
		if (Application->MainForm == this) Application->OnMinimize(this);
	}

	return TWinControl::ProcessSize(SizeType, L, H);
}

//---------------------------------------------------------------------------
bool TForm::ProcessActivateApp(bool bActive) {

	if (!FComponentState.Contains(csLoading)) {
		if (bActive) Application->OnActivate(this);
		else Application->OnDeactivate(this);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TForm::ProcessActivate(int fActive) {

	if (!FComponentState.Contains(csLoading)) {
		if (fActive != WA_INACTIVE) OnActivate(this);
		else OnDeactivate(this);
	}

	return false;
}

//---------------------------------------------------------------------------
bool TForm::ProcessShortCut(int nVirtKey, TShiftState Shift) {

	if (!FComponentState.Contains(csLoading) && OnShortCut) {
		bool Handled = false;
		OnShortCut(this, nVirtKey, Shift, Handled);
		if (Handled) return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool TForm::ProcessClose(void) {
	TCloseAction Action = FDefaultCloseAction;


	if (!FComponentState.Contains(csLoading)) OnClose(this, Action);

	switch (Action) {
		case caNone: FModalResult = mrNone; return true;
		case caHide: Set_Visible(false); return true;
		case caFree: DestroyWindow(FHandle); return true;
	}

	return false;
}

//---------------------------------------------------------------------------
void TForm::ProcessDestroy(void) {
	if (Application->MainForm == this) {
		Application->MainForm = NULL;
		PostQuitMessage(0);
	}
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TForm::GetListProperties(TStrings *ListProperties) {

	TFormFrame::GetListProperties(ListProperties);

	ListProperties->Add(_T("ActiveControl"));
	ListProperties->Add(_T("AlphaBlend"));
	ListProperties->Add(_T("AlphaBlendValue"));
	ListProperties->Add(_T("ModalResult"));
	ListProperties->Add(_T("Position"));
	ListProperties->Add(_T("WindowState"));
	ListProperties->Add(_T("FormActivation"));
	ListProperties->Add(_T("FormStyle"));
	ListProperties->Add(_T("FormState"));
	ListProperties->Add(_T("BorderIcons"));
	ListProperties->Add(_T("BorderStyle"));
	ListProperties->Add(_T("DefaultCloseAction"));
	ListProperties->Add(_T("Menu"));
	ListProperties->Add(_T("OnCreate"));
	ListProperties->Add(_T("OnActivate"));
	ListProperties->Add(_T("OnDeactivate"));
	ListProperties->Add(_T("OnPaint"));
	ListProperties->Add(_T("OnShortCut"));
	ListProperties->Add(_T("OnClose"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TForm::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("ActiveControl")) {
		return tpObject;
	}
	if (asProperty == _T("AlphaBlend")) {
		return tpBool;
	}
	if (asProperty == _T("AlphaBlendValue")) {
		return tpNumber;
	}
	if (asProperty == _T("ModalResult")) {
		*asInfos = _T("mrNone\nmrOk\nmrCancel\nmrAbort\nmrRetry\nmrIgnore\nmrYes\nmrNo\nmrAll\nmrNoToAll\nmrYesToAll");
		return tpChoice;
	}
	if (asProperty == _T("Position")) {
		*asInfos = _T("poDesigned\npoDefault\npoDefaultPosOnly\npoDefaultSizeOnly\npoScreenCenter\npoDesktopCenter\npoMainFormCenter\npoOwnerFormCenter");
		return tpChoice;
	}
	if (asProperty == _T("WindowState")) {
		*asInfos = _T("wsNormal\nwsMinimized\nwsMaximized");
		return tpChoice;
	}
	if (asProperty == _T("FormActivation")) {
		return tpBool;
	}
	if (asProperty == _T("FormState")) {
		*asInfos = _T("fsCreating\nfsVisible\nfsShowing\nfsModal\nfsCreatedMDIChild\nfsActivated");
		return tpMultipleChoice;
	}
	if (asProperty == _T("FormStyle")) {
		*asInfos = _T("fsNormal\nfsMDIChild\nfsMDIForm\nfsStayOnTop");
		return tpChoice;
	}
	if (asProperty == _T("BorderIcons")) {
		*asInfos = _T("biSystemMenu\nbiMinimize\nbiMaximize\nbiHelp");
		return tpMultipleChoice;
	}
	if (asProperty == _T("BorderStyle")) {
		*asInfos = _T("bsNone\nbsSingle\nbsSizeable\nbsDialog\nbsToolWindow\nbsSizeToolWin");
		return tpChoice;
	}
	if (asProperty == _T("DefaultCloseAction")) {
		*asInfos = _T("caNone\ncaHide\ncaFree");
		return tpChoice;
	}
	if (asProperty == _T("Menu")) {
		return tpObject;
	}
	if (asProperty == _T("OnCreate")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnActivate")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnDeactivate")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnPaint")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnShortCut")) {
    *asInfos = _T("(TObject *Sender, int VirtKey, TShiftState Shift, bool &Handled)");
		return tpEvent;
	}
	if (asProperty == _T("OnClose")) {
    *asInfos = _T("(TObject *Sender, TCloseAction &Action)");
		return tpEvent;
	}
	return TFormFrame::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TForm::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("ActiveControl")) {
		return _T("");
	}
	if (asProperty == _T("AlphaBlend")) {
		return _T("False");
	}
	if (asProperty == _T("AlphaBlendValue")) {
		return _T("255");
	}
	if (asProperty == _T("ModalResult")) {
		return _T("mrNone");
	}
	if (asProperty == _T("Position")) {
		return _T("poDesigned");
	}
	if (asProperty == _T("WindowState")) {
		return _T("wsNormal");
	}
	if (asProperty == _T("FormActivation")) {
		return _T("True");
	}
	if (asProperty == _T("FormState")) {
		return _T("[]");
	}
	if (asProperty == _T("FormStyle")) {
		return _T("fsNormal");
	}
	if (asProperty == _T("BorderIcons")) {
		return _T("[biSystemMenu, biMinimize, biMaximize]");
	}
	if (asProperty == _T("BorderStyle")) {
		return _T("bsSizeable");
	}
	if (asProperty == _T("DefaultCloseAction")) {
		return _T("caFree");
	}

	if (asProperty == _T("Menu")) {
		return _T("");
	}
	if (asProperty == _T("OnCreate")) {
		return _T("");
	}
	if (asProperty == _T("OnActivate")) {
		return _T("");
	}
	if (asProperty == _T("OnDeactivate")) {
		return _T("");
	}
	if (asProperty == _T("OnPaint")) {
		return _T("");
	}
	if (asProperty == _T("OnShortCut")) {
		return _T("");
	}
	if (asProperty == _T("OnClose")) {
		return _T("");
	}
	return TFormFrame::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TForm::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("Constraints.MinWidth")) {
		if (Constraints->MinWidth <= 0) return 0;
		return IntToStr(Constraints->MinWidth - 2 * GetSystemMetrics(SM_CXSIZEFRAME) + 8);
	}
	if (asProperty == _T("Constraints.MinHeight")) {
		if (Constraints->MinHeight <= 0) return 0;
		return IntToStr(Constraints->MinHeight - 2 * GetSystemMetrics(SM_CYSIZEFRAME) - GetSystemMetrics(SM_CYCAPTION) + 27);
	}
	if (asProperty == _T("Constraints.MaxWidth")) {
		if (Constraints->MaxWidth <= 0) return 0;
		return IntToStr(Constraints->MaxWidth - 2 * GetSystemMetrics(SM_CXSIZEFRAME) + 8);
	}
	if (asProperty == _T("Constraints.MaxHeight")) {
		if (Constraints->MaxHeight <= 0) return 0;
		return IntToStr(Constraints->MaxHeight - 2 * GetSystemMetrics(SM_CYSIZEFRAME) - GetSystemMetrics(SM_CYCAPTION) + 27);
	}
	if (asProperty == _T("ActiveControl")) {
		TWinControl *WinControl = Get_ActiveControl();
		if (WinControl) return WinControl->Name;
		return _T("");
	}
	if (asProperty == _T("AlphaBlend")) {
		if (Get_AlphaBlend()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("AlphaBlendValue")) {
		return IntToStr(Get_AlphaBlendValue());
	}
	if (asProperty == _T("ModalResult")) {
		TModalResult ModalResult = Get_ModalResult();
		if (ModalResult == mrNone) return _T("mrNone");
		if (ModalResult == mrOk) return _T("mrOk");
		if (ModalResult == mrCancel) return _T("mrCancel");
		if (ModalResult == mrAbort) return _T("mrAbort");
		if (ModalResult == mrRetry) return _T("mrRetry");
		if (ModalResult == mrIgnore) return _T("mrIgnore");
		if (ModalResult == mrYes) return _T("mrYes");
		if (ModalResult == mrNo) return _T("mrNo");
		if (ModalResult == mrAll) return _T("mrAll");
		if (ModalResult == mrNoToAll) return _T("mrNoToAll");
		if (ModalResult == mrYesToAll) return _T("mrYesToAll");
		return _T("mrNone");
	}
	if (asProperty == _T("Position")) {
		TPosition Position = Get_Position();
		if (Position == poDesigned) return _T("poDesigned");
		if (Position == poDefault) return _T("poDefault");
		if (Position == poDefaultPosOnly) return _T("poDefaultPosOnly");
		if (Position == poDefaultSizeOnly) return _T("poDefaultSizeOnly");
		if (Position == poScreenCenter) return _T("poScreenCenter");
		if (Position == poDesktopCenter) return _T("poDesktopCenter");
		if (Position == poMainFormCenter) return _T("poMainFormCenter");
		if (Position == poOwnerFormCenter) return _T("poOwnerFormCenter");
		return _T("poDesigned");
	}
	if (asProperty == _T("WindowState")) {
		TWindowState WindowState = Get_WindowState();
		if (WindowState == wsNormal) return _T("wsNormal");
		if (WindowState == wsMinimized) return _T("wsMinimized");
		if (WindowState == wsMaximized) return _T("wsMaximized");
		return _T("wsNormal");
	}
	if (asProperty == _T("FormActivation")) {
		if (Get_FormActivation()) return _T("True");
		else return _T("False");
	}
	if (asProperty == _T("FormState")) {
		AnsiString asFormState;
		TFormState FormState = Get_FormState();
		if (FormState.Contains(fsCreating)) asFormState += _T("fsCreating\n");
		if (FormState.Contains(fsVisible)) asFormState += _T("fsVisible\n");
		if (FormState.Contains(fsShowing)) asFormState += _T("fsShowing\n");
		if (FormState.Contains(fsModal)) asFormState += _T("fsModal\n");
		if (FormState.Contains(fsCreatedMDIChild)) asFormState += _T("fsCreatedMDIChild\n");
		if (FormState.Contains(fsActivated)) asFormState += _T("fsActivated\n");
		return FormatProperty(asFormState, _T("["), _T("]"), _T(", "));
	}
	if (asProperty == _T("FormStyle")) {
		TFormStyle FormStyle = Get_FormStyle();
		if (FormStyle == fsNormal) return _T("fsNormal");
		if (FormStyle == fsMDIChild) return _T("fsMDIChild");
		if (FormStyle == fsMDIForm) return _T("fsMDIForm");
		if (FormStyle == fsStayOnTop) return _T("fsStayOnTop");
		return _T("fsNormal");
	}
	if (asProperty == _T("BorderIcons")) {
		AnsiString asBorderIcons;
		TBorderIcons BorderIcons = Get_BorderIcons();
		if (BorderIcons.Contains(biSystemMenu)) asBorderIcons += _T("biSystemMenu\n");
		if (BorderIcons.Contains(biMinimize)) asBorderIcons += _T("biMinimize\n");
		if (BorderIcons.Contains(biMaximize)) asBorderIcons += _T("biMaximize\n");
		if (BorderIcons.Contains(biHelp)) asBorderIcons += _T("biHelp\n");
		return FormatProperty(asBorderIcons, _T("["), _T("]"), _T(", "));
	}
	if (asProperty == _T("BorderStyle")) {
		TFormBorderStyle BorderStyle = Get_BorderStyle();
		if (BorderStyle == bsNone) return _T("bsNone");
		if (BorderStyle == bsSingle) return _T("bsSingle");
		if (BorderStyle == bsSizeable) return _T("bsSizeable");
		if (BorderStyle == bsDialog) return _T("bsDialog");
		if (BorderStyle == bsToolWindow) return _T("bsToolWindow");
		if (BorderStyle == bsSizeToolWin) return _T("bsSizeToolWin");
		return _T("bsNone");
	}
	if (asProperty == _T("DefaultCloseAction")) {
		TCloseAction DefaultCloseAction = Get_DefaultCloseAction();
		if (DefaultCloseAction == caNone) return _T("caNone");
		if (DefaultCloseAction == caHide) return _T("caHide");
		if (DefaultCloseAction == caFree) return _T("caFree");
		return _T("");
	}

	if (asProperty == _T("Menu")) {
		TMainMenu *Menu = Get_Menu();
		if (Menu) return Menu->Name;
		return _T("");
	}
	if (asProperty == _T("OnCreate")) {
		return OnCreate.AsString();
	}
	if (asProperty == _T("OnActivate")) {
		return OnActivate.AsString();
	}
	if (asProperty == _T("OnDeactivate")) {
		return OnDeactivate.AsString();
	}
	if (asProperty == _T("OnPaint")) {
		return OnPaint.AsString();
	}
	if (asProperty == _T("OnShortCut")) {
		return OnShortCut.AsString();
	}
	if (asProperty == _T("OnClose")) {
		return OnClose.AsString();
	}
	return TFormFrame::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TForm::SetProperty(AnsiString asProperty, AnsiString asValue,
															TPersistent *Sender) {
	if (asProperty == _T("Constraints.MinWidth")) {
		int Value = StrToInt(asValue);
		if (Value > 0) Constraints->MinWidth = Value + 2 * GetSystemMetrics(SM_CXSIZEFRAME) - 8;
		else Constraints->MinWidth = Value;
		return true;
	}
	if (asProperty == _T("Constraints.MinHeight")) {
		int Value = StrToInt(asValue);
		if (Value > 0) Constraints->MinHeight = Value + 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) - 27;
		else Constraints->MinHeight = Value;
		return true;
	}
	if (asProperty == _T("Constraints.MaxWidth")) {
		int Value = StrToInt(asValue);
		if (Value > 0) Constraints->MaxWidth = Value + 2 * GetSystemMetrics(SM_CXSIZEFRAME) - 8;
		else Constraints->MaxWidth = Value;
		return true;
	}
	if (asProperty == _T("Constraints.MaxHeight")) {
		int Value = StrToInt(asValue);
		if (Value > 0) Constraints->MaxHeight = Value + 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) - 27;
		else Constraints->MaxHeight = Value;
		return true;
	}
	if (asProperty == _T("ActiveControl")) {
		TWinControl *WinControl;
		if (asValue.IsEmpty()) WinControl = NULL;
		else WinControl = dynamic_cast<TWinControl *>(FindRootComponent()->FindComponent(asValue));
		Set_ActiveControl(WinControl);
		return true;
	}
	if (asProperty == _T("AlphaBlend")) {
		Set_AlphaBlend(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("AlphaBlendValue")) {
		Set_AlphaBlendValue(asValue.ToInt());
		return true;
	}
	if (asProperty == _T("ModalResult")) {
		if (asValue == _T("mrNone")) Set_ModalResult(mrNone);
		if (asValue == _T("mrOk")) Set_ModalResult(mrOk);
		if (asValue == _T("mrCancel")) Set_ModalResult(mrCancel);
		if (asValue == _T("mrAbort")) Set_ModalResult(mrAbort);
		if (asValue == _T("mrRetry")) Set_ModalResult(mrRetry);
		if (asValue == _T("mrIgnore")) Set_ModalResult(mrIgnore);
		if (asValue == _T("mrYes")) Set_ModalResult(mrYes);
		if (asValue == _T("mrNo")) Set_ModalResult(mrNo);
		if (asValue == _T("mrAll")) Set_ModalResult(mrAll);
		if (asValue == _T("mrNoToAll")) Set_ModalResult(mrNoToAll);
		if (asValue == _T("mrYesToAll")) Set_ModalResult(mrYesToAll);
		return true;
	}
	if (asProperty == _T("Position")) {
		if (asValue == _T("poDesigned")) Set_Position(poDesigned);
		if (asValue == _T("poDefault")) Set_Position(poDefault);
		if (asValue == _T("poDefaultPosOnly")) Set_Position(poDefaultPosOnly);
		if (asValue == _T("poDefaultSizeOnly")) Set_Position(poDefaultSizeOnly);
		if (asValue == _T("poScreenCenter")) Set_Position(poScreenCenter);
		if (asValue == _T("poDesktopCenter")) Set_Position(poDesktopCenter);
		if (asValue == _T("poMainFormCenter")) Set_Position(poMainFormCenter);
		if (asValue == _T("poOwnerFormCenter")) Set_Position(poOwnerFormCenter);
		return true;
	}
	if (asProperty == _T("WindowState")) {
		if (asValue == _T("wsNormal")) Set_WindowState(wsNormal);
		else if (asValue == _T("wsMinimized")) Set_WindowState(wsMinimized);
		else if (asValue == _T("wsMaximized")) Set_WindowState(wsMaximized);
		return true;
	}
	if (asProperty == _T("FormActivation")) {
		Set_FormActivation(asValue == _T("True"));
		return true;
	}
	if (asProperty == _T("FormStyle")) {
		if (asValue == _T("fsNormal")) Set_FormStyle(fsNormal);
		else if (asValue == _T("fsMDIChild")) Set_FormStyle(fsMDIChild);
		else if (asValue == _T("fsMDIForm")) Set_FormStyle(fsMDIForm);
		else if (asValue == _T("fsStayOnTop")) Set_FormStyle(fsStayOnTop);
		return true;
	}
	if (asProperty == _T("BorderIcons")) {
		int Cur;
		AnsiString asMot;
		TBorderIcons NewBorderIcons;

		NewBorderIcons.Clear();
		Cur = 0;
		do {
			asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
			if (asMot == _T("biSystemMenu")) NewBorderIcons << biSystemMenu;
			else if (asMot == _T("biMinimize")) NewBorderIcons << biMinimize;
			else if (asMot == _T("biMaximize")) NewBorderIcons << biMaximize;
			else if (asMot == _T("biHelp")) NewBorderIcons << biHelp;
		} while (!asMot.IsEmpty());
		Set_BorderIcons(NewBorderIcons);

		return true;
	}
	if (asProperty == _T("BorderStyle")) {
		if (asValue == _T("bsNone")) Set_BorderStyle(bsNone);
		else if (asValue == _T("bsSingle")) Set_BorderStyle(bsSingle);
		else if (asValue == _T("bsSizeable")) Set_BorderStyle(bsSizeable);
		else if (asValue == _T("bsDialog")) Set_BorderStyle(bsDialog);
		else if (asValue == _T("bsToolWindow")) Set_BorderStyle(bsToolWindow);
		else if (asValue == _T("bsSizeToolWin")) Set_BorderStyle(bsSizeToolWin);
		return true;
	}
	if (asProperty == _T("DefaultCloseAction")) {
		if			(asValue == _T("caNone")) Set_DefaultCloseAction(caNone);
		else if (asValue == _T("caHide")) Set_DefaultCloseAction(caHide);
		else if (asValue == _T("caFree")) Set_DefaultCloseAction(caFree);
		return true;
	}
	if (asProperty == _T("Menu")) {
		TMainMenu *Menu;
		if (asValue.IsEmpty()) Menu = NULL;
		else Menu = dynamic_cast<TMainMenu *>(FindRootComponent()->FindComponent(asValue));
		Set_Menu(Menu);
		return true;
	}
	if (asProperty == _T("OnCreate")) {
		OnCreate.ObjCall = Sender;
		OnCreate = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnActivate")) {
		OnActivate.ObjCall = Sender;
		OnActivate = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnDeactivate")) {
		OnDeactivate.ObjCall = Sender;
		OnDeactivate = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnPaint")) {
		OnPaint.ObjCall = Sender;
		OnPaint = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnShortCut")) {
		OnShortCut.ObjCall = Sender;
		OnShortCut = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnClose")) {
		OnClose.ObjCall = Sender;
		OnClose = asValue.c_str();
		return true;
	}
	return TFormFrame::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

