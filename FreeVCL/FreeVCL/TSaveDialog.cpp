//-----------------------------------------------------------------------------
//! @file TSaveDialog.cpp
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
// TSaveDialog
//---------------------------------------------------------------------------

TSaveDialog::TSaveDialog(TComponent* AOwner): TOpenSaveDialog(AOwner) {

  // Initialisations
  FClassName = _T("TSaveDialog");
  Flags = OFN_HIDEREADONLY;
}

TSaveDialog::~TSaveDialog(void) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TSaveDialog::GetListProperties(TStrings *ListProperties) {

  TOpenSaveDialog::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TSaveDialog::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TOpenSaveDialog::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TSaveDialog::GetDefaultProperty(AnsiString asProperty) {
  return TOpenSaveDialog::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TSaveDialog::GetProperty(AnsiString asProperty) {
  return TOpenSaveDialog::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TSaveDialog::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TOpenSaveDialog::SetProperty(asProperty, asValue, Sender);
}

bool TSaveDialog::Execute(void) {
  OPENFILENAME ofn;
  TCHAR *szFileName;
  TCHAR *szFilter;
	int nMaxFile;
  int i;
  bool Ok = false;


	nMaxFile = 32767;
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
  if (GetSaveFileName((LPOPENFILENAME) &ofn)) {
    FFileName = szFileName;
    FFilterIndex = ofn.nFilterIndex;
    Ok = true;
  }
  /* $$$ (pour tests)
  else {
    i = CommDlgExtendedError();
    if (i == CDERR_DIALOGFAILURE) MessageBox(NULL, _T("Erreur CDERR_DIALOGFAILURE"), _T("$$$"), MB_OK);
    else if (i == CDERR_FINDRESFAILURE) MessageBox(NULL, _T("Erreur CDERR_FINDRESFAILURE"), _T("$$$"), MB_OK);
    else if (i == CDERR_INITIALIZATION) MessageBox(NULL, _T("Erreur CDERR_INITIALIZATION"), _T("$$$"), MB_OK);
    else if (i == CDERR_LOADRESFAILURE) MessageBox(NULL, _T("Erreur CDERR_LOADRESFAILURE"), _T("$$$"), MB_OK);
    else if (i == CDERR_LOADSTRFAILURE) MessageBox(NULL, _T("Erreur CDERR_LOADSTRFAILURE"), _T("$$$"), MB_OK);
    else if (i == CDERR_LOCKRESFAILURE) MessageBox(NULL, _T("Erreur CDERR_LOCKRESFAILURE"), _T("$$$"), MB_OK);
    else if (i == CDERR_MEMALLOCFAILURE) MessageBox(NULL, _T("Erreur CDERR_MEMALLOCFAILURE"), _T("$$$"), MB_OK);
    else if (i == CDERR_MEMLOCKFAILURE) MessageBox(NULL, _T("Erreur CDERR_MEMLOCKFAILURE"), _T("$$$"), MB_OK);
    else if (i == CDERR_NOHINSTANCE) MessageBox(NULL, _T("Erreur CDERR_NOHINSTANCE"), _T("$$$"), MB_OK);
    else if (i == CDERR_NOHOOK) MessageBox(NULL, _T("Erreur CDERR_NOHOOK"), _T("$$$"), MB_OK);
    else if (i == CDERR_NOTEMPLATE) MessageBox(NULL, _T("Erreur CDERR_NOTEMPLATE"), _T("$$$"), MB_OK);
    else if (i == CDERR_REGISTERMSGFAIL) MessageBox(NULL, _T("Erreur CDERR_REGISTERMSGFAIL"), _T("$$$"), MB_OK);
    else if (i == CDERR_STRUCTSIZE) MessageBox(NULL, _T("Erreur CDERR_STRUCTSIZE"), _T("$$$"), MB_OK);
    else MessageBox(NULL, _T("Erreur inconnue"), _T("$$$"), MB_OK);
  } */

  delete[] szFilter;
  delete[] szFileName;

  return Ok;
}

//---------------------------------------------------------------------------

