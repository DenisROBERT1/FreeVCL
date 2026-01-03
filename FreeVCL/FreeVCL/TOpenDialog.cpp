//-----------------------------------------------------------------------------
//! @file TOpenDialog.cpp
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
// TOpenDialog
//---------------------------------------------------------------------------

TOpenDialog::TOpenDialog(TComponent* AOwner): TOpenSaveDialog(AOwner) {

  // Initialisations
  FClassName = _T("TOpenDialog");
  Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_SHAREAWARE;
	FFiles = new TStringList();
}

TOpenDialog::~TOpenDialog(void) {
	delete FFiles;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Files
//---------------------------------------------------------------------------

TStrings *TOpenDialog::Get_Files(void) {
  return FFiles;
}

//---------------------------------------------------------------------------
bool TOpenDialog::Execute(void) {
  OPENFILENAME ofn;
  TCHAR *szFileName;
  TCHAR *szFilter;
	int nMaxFile;
  int i;
  bool Ok = false;


	if (Flags & OFN_ALLOWMULTISELECT) nMaxFile = 65536;  // Au pif
	else nMaxFile = 32767;
	szFileName = new TCHAR[nMaxFile];
	szFilter = new TCHAR[FFilter.Length() + 2];  // + 2 zéros terminaux

  lstrcpyn(szFileName, FFileName, nMaxFile);
  memset(&ofn, 0, sizeof(OPENFILENAME));
  if ((DWORD) (LOBYTE(LOWORD(GetVersion()))) <= 4) {
    // COMPATIBILITE Windows 98
    ofn.lStructSize = sizeof(OPENFILENAME) - 12;
  }
  else {
    ofn.lStructSize = sizeof(OPENFILENAME);
  }
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter = 0;
  ofn.nFilterIndex = FFilterIndex;
  ofn.nMaxFile = nMaxFile;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrDefExt = FDefaultExt;
  ofn.hwndOwner = Handle;
  for (i = 0; i < (int) FFilter.Length(); i++) {
    if (FFilter[i + 1] == _T('|')) szFilter[i] = _T('\0');
    else szFilter[i] = FFilter[i + 1];
  }
  szFilter[i++] = _T('\0');
  szFilter[i] = _T('\0');
  ofn.lpstrFilter = szFilter;
  ofn.lpstrInitialDir = FInitialDir;
  ofn.lpstrFile = szFileName;
  ofn.lpstrTitle = FTitle;
  ofn.Flags = Flags;
  if (GetOpenFileName((LPOPENFILENAME) &ofn)) {
    FFileName = szFileName;
    FFilterIndex = ofn.nFilterIndex;
		FFiles->Clear();
		if (ofn.nFileOffset > lstrlen(szFileName)) {
			AnsiString asPath;
			TCHAR * lp;
			lp = szFileName;
			asPath = lp;
			lp += lstrlen(lp) + 1;
			while (*lp) {
				FFiles->Add(asPath + DIR_DELIMITER + lp);
				lp += lstrlen(lp) + 1;
			}
		}
		else {
			FFiles->Add(szFileName);
		}
    Ok = true;
  }

  delete[] szFilter;
  delete[] szFileName;

  return Ok;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TOpenDialog::GetListProperties(TStrings *ListProperties) {

  TOpenSaveDialog::GetListProperties(ListProperties);

  ListProperties->Add(_T("Files"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TOpenDialog::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Files")) {
    return tpStrings;
  }
  return TOpenSaveDialog::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TOpenDialog::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Files")) {
    return _T("");
  }
  return TOpenSaveDialog::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TOpenDialog::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Files")) {
    return FFiles->GetTextStr();
  }
  return TOpenSaveDialog::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TOpenDialog::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TOpenSaveDialog::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

