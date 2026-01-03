//-----------------------------------------------------------------------------
//! @file TForm_DisplayError.cpp
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

#define OEMRESOURCE

#include <windows.h>
#include <commdlg.h>

#include <FreeVcl.h>

#include "TForm_DisplayError.h"
#include "TConfig.h"
#include "HLP_FVCLIDE_Fr.h"

//---------------------------------------------------------------------------
TForm_DisplayError::TForm_DisplayError(HWND hWndParent, LPCTSTR szName)
  : TForm(hWndParent, szName, FVCL_ARG_SUPP) {
	FNePlusAfficher = false;
}

//---------------------------------------------------------------------------
// Positionnement des boutons visibles
//---------------------------------------------------------------------------
void TForm_DisplayError::ReArrangeBoutons(void) {

	int NbButtons = 0;
	if (Button_Oui->Visible) NbButtons++;
	if (Button_Non->Visible) NbButtons++;
	if (Button_Ok->Visible) NbButtons++;
	if (Button_Annuler->Visible) NbButtons++;
	if (Button_Aide->Visible) NbButtons++;

	TRect Rect = ClientRect;
	int x = Rect.Left + (Rect.Width - (NbButtons - 1) * 20 - NbButtons * 68) / 2;
	if (Button_Oui->Visible) {
		Button_Oui->Left = x;
		x += 88;  // Largeur du bouton (68) + Espace entre boutons (20)
	}
	if (Button_Non->Visible) {
		Button_Non->Left = x;
		x += 88;
	}
	if (Button_Ok->Visible) {
		Button_Ok->Left = x;
		x += 88;
	}
	if (Button_Annuler->Visible) {
		Button_Annuler->Left = x;
		x += 88;
	}
	if (Button_Aide->Visible) {
		Button_Aide->Left = x;
		x += 88;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DisplayError::Button_OkClick(TObject *Sender) {
	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DisplayError::Button_AnnulerClick(TObject *Sender) {
	ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DisplayError::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_DISPLAYERROR);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DisplayError::Button_OuiClick(TObject *Sender) {
	ModalResult = mrYes;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DisplayError::Button_NonClick(TObject *Sender) {
	ModalResult = mrNo;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DisplayError::FormClose(TObject *Sender, TCloseAction &Action) {
	FNePlusAfficher = CheckBox_NePlusAfficher->Checked;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NePlusAfficher
//---------------------------------------------------------------------------

bool TForm_DisplayError::Get_NePlusAfficher() {
  return FNePlusAfficher;
}

int DisplayError(const AnsiString &asMessage, const AnsiString &asTitle, const AnsiString &asKey, int Flags) {
  TForm_DisplayError *Form_DisplayError;
	int FlagIcon;
	int FlagButtons;
	TIcon *Icon = NULL;
	DWORD dwSize;
	DWORD Ret = 0;
	bool NePlusAfficher;
	LONG rc = ERROR_SUCCESS;
	HKEY hKey;
	int SystemError = 0;
	const TCHAR * szErrorMessage;


	if (!asKey.IsEmpty()) {
		rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
												0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
		if (rc == ERROR_ACCESS_DENIED) {
			rc = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
													0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
		}

		if (rc == ERROR_SUCCESS) {
			dwSize = sizeof(DWORD);
			rc = RegQueryValueEx(hKey, asKey, NULL, NULL, (BYTE*) &Ret, &dwSize);
			if (rc == ERROR_SUCCESS) {
				if (Ret != 0) {
					RegCloseKey(hKey);
					return Ret;
				}
			}
			else if (rc != ERROR_FILE_NOT_FOUND) {
				szErrorMessage = _TT("Impossible de lire la clé \"FVCLIDE\\AutoResp\".");
				SystemError = rc;
			}
			else rc = ERROR_SUCCESS;  // Tout va bien mais il n'y a pas de réponse auto enregistrée
		}
		else {
			szErrorMessage = _TT("Impossible d'ouvrir ou de créer la clé \"FVCLIDE\\AutoResp\".");
			SystemError = rc;
		}
	}

  Form_DisplayError = new TForm_DisplayError(Application->Handle, _T("TForm_DisplayError"));

	FlagIcon = Flags & MB_ICONMASK;
  if (FlagIcon) {
    Icon = new TIcon();
		Icon->Width = Form_DisplayError->Image->Width;
		Icon->Height = Form_DisplayError->Image->Height;
		if (FlagIcon == MB_ICONINFORMATION) {
			Icon->LoadFromResourceName(HInstance, _T("ICON_INFORMATION"));
		}
		else if (FlagIcon == MB_ICONQUESTION) {
			Icon->LoadFromResourceName(HInstance, _T("ICON_QUESTION"));
		}
		else if (FlagIcon == MB_ICONWARNING) {
			Icon->LoadFromResourceName(HInstance, _T("ICON_WARNING"));
		}
		else if (FlagIcon == MB_ICONERROR) {
			Icon->LoadFromResourceName(HInstance, _T("ICON_ERROR"));
		}
	}

	FlagButtons = Flags & MB_TYPEMASK;
	if (FlagButtons == MB_OK) {
		Form_DisplayError->Button_Ok->Visible = true;
		Form_DisplayError->Button_Annuler->Visible = false;
		Form_DisplayError->Button_Oui->Visible = false;
		Form_DisplayError->Button_Non->Visible = false;
	}
	else if (FlagButtons == MB_OKCANCEL) {
		Form_DisplayError->Button_Ok->Visible = true;
		Form_DisplayError->Button_Annuler->Visible = true;
		Form_DisplayError->Button_Oui->Visible = false;
		Form_DisplayError->Button_Non->Visible = false;
	}
	else if (FlagButtons == MB_YESNO) {
		Form_DisplayError->Button_Ok->Visible = false;
		Form_DisplayError->Button_Annuler->Visible = false;
		Form_DisplayError->Button_Oui->Visible = true;
		Form_DisplayError->Button_Non->Visible = true;
	}
	else if (FlagButtons == MB_YESNOCANCEL) {
		Form_DisplayError->Button_Ok->Visible = false;
		Form_DisplayError->Button_Annuler->Visible = true;
		Form_DisplayError->Button_Oui->Visible = true;
		Form_DisplayError->Button_Non->Visible = true;
	}
	Form_DisplayError->ReArrangeBoutons();

	if (asKey.IsEmpty()) Form_DisplayError->CheckBox_NePlusAfficher->Visible = false;

  Form_DisplayError->Image->Picture->Graphic = Icon;
  Form_DisplayError->Caption = asTitle;
  Form_DisplayError->Label_Message->Caption = asMessage;
  Ret = Form_DisplayError->ShowModal();
	NePlusAfficher = Form_DisplayError->NePlusAfficher;

  delete Form_DisplayError;
	delete Icon;

	if (!asKey.IsEmpty()) {
		if (rc == ERROR_SUCCESS) {
			if (NePlusAfficher) {
				rc = RegSetValueEx(hKey, asKey, 0, REG_DWORD, (BYTE*) &Ret, sizeof(DWORD));
				DisplayError(_TT("Vous pouvez paramétrer les réponses automatiques en allant dans \"Options\", puis \"Réponses par défaut\"."), _TT("Réponse automatique"), _T("AUTO_RESP"), MB_OK | MB_ICONINFORMATION);
			}
			RegCloseKey(hKey);
			if (rc != ERROR_SUCCESS) {
				szErrorMessage = _TT("Impossible d'écrire dans la clé \"FVCLIDE\\AutoResp\".");
				SystemError = rc;
			}
		}
	}

	if (SystemError) DisplaySystemError(szErrorMessage, SystemError, _TT("Erreur système"), _T(""), MB_OK | MB_ICONSTOP);

	return Ret;
}

//---------------------------------------------------------------------------
int DisplaySystemError(const AnsiString &asMessage, int SystemError, const AnsiString &asTitle, const AnsiString &asKey, int Flags) {
	return DisplaySystemErrorArgs(asMessage, SystemError, asTitle, asKey, Flags, _T(""), _T(""), _T(""));
}

//---------------------------------------------------------------------------
int DisplaySystemErrorArgs(const AnsiString &asMessage, int SystemError, const AnsiString &asTitle, const AnsiString &asKey, int Flags, ...) {
	TCHAR szBuf[128];
	AnsiString asMessage2;
	va_list argptr;

	wsprintf(szBuf, _TT("Erreur système %i"), SystemError);  // Au cas où FormatMessage ne marcherait pas
	va_start(argptr, Flags);
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, SystemError, 0, szBuf, 128, &argptr);
	va_end(argptr);
	asMessage2 = asMessage + _T('\n') + szBuf;

	return DisplayError(asMessage2, asTitle, asKey, Flags);
}

//---------------------------------------------------------------------------
