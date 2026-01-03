//-----------------------------------------------------------------------------
//! @file TForm_DefaultResponses.cpp
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

#include "TConfig.h"
#include "TForm_SaisieTools.h"
#include "TForm_DefaultResponses.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_DefaultResponses::TForm_DefaultResponses(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {

  MAJListBox();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DefaultResponses::Button_MAZClick(TObject *Sender) {
	DWORD dwSize;
	LONG rc;
	HKEY hKey;
	TCHAR szKeyName[111];  // 100 + sizeof(" = IDIGNORE")


	rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
											0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
	if (rc == ERROR_ACCESS_DENIED) {
		rc = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
												0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
	}

	if (rc == ERROR_SUCCESS) {
		while (rc == ERROR_SUCCESS) {
			dwSize = 100;
			rc = RegEnumValue(hKey, 0, szKeyName, &dwSize, NULL, NULL, NULL, NULL);
			if (rc == ERROR_NO_MORE_ITEMS) break;
			rc = RegDeleteValue(hKey, szKeyName);
		}
	}

	RegCloseKey(hKey);

	MAJListBox();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_DefaultResponses::Button_SupprClick(TObject *Sender) {
	DWORD dwSize;
	LONG rc;
	HKEY hKey;
	AnsiString asKeyName;
	int Pos;
	int i;


	i = ListBox_DefaultResponses->ItemIndex;
	if (i >= 0) {
		asKeyName = ListBox_DefaultResponses->Items->Strings[i];

		Pos = asKeyName.Pos(_T(" = "));
		if (Pos != 0) asKeyName = asKeyName.SubString(1, Pos - 1);

		rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
												0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
		if (rc == ERROR_ACCESS_DENIED) {
			rc = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
													0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
		}

		if (rc == ERROR_SUCCESS) {
			RegDeleteValue(hKey, asKeyName);
		}

		RegCloseKey(hKey);

	  MAJListBox();

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_DefaultResponses::Button_SortieClick(TObject *Sender) {
  Close();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_DefaultResponses::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_DEFAULTRESPONSES);
}

//---------------------------------------------------------------------------
void TForm_DefaultResponses::MAJListBox(void) {
	DWORD dwSize;
	DWORD dwData;
	DWORD dwSizeData;
	LONG rc;
	HKEY hKey;
	TCHAR szKeyName[111];  // 100 + sizeof(" = IDIGNORE")
	int i;


	ListBox_DefaultResponses->Clear();

	rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
											0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
	if (rc == ERROR_ACCESS_DENIED) {
		rc = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Denis ROBERT\\FVCLIDE\\AutoResp"),
												0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwSize);
	}

	if (rc == ERROR_SUCCESS) {
		for (i = 0; i < 1000; i++) {
			dwSize = 100;
			dwSizeData = sizeof(DWORD);
			rc = RegEnumValue(hKey, i, szKeyName, &dwSize, NULL, NULL, (BYTE*) &dwData, &dwSizeData);
			if (rc == ERROR_NO_MORE_ITEMS) break;
			if (dwData == IDOK) lstrcat(szKeyName, _T(" = OK"));
			if (dwData == IDCANCEL) lstrcat(szKeyName, _T(" = CANCEL"));
			if (dwData == IDABORT) lstrcat(szKeyName, _T(" = IDABORT"));
			if (dwData == IDRETRY) lstrcat(szKeyName, _T(" = IDRETRY"));
			if (dwData == IDIGNORE) lstrcat(szKeyName, _T(" = IDIGNORE"));
			if (dwData == IDYES) lstrcat(szKeyName, _T(" = YES"));
			if (dwData == IDNO) lstrcat(szKeyName, _T(" = NO"));
	    ListBox_DefaultResponses->Items->Add(szKeyName);
		}
	}

	RegCloseKey(hKey);
}
//---------------------------------------------------------------------------
