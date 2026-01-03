//-----------------------------------------------------------------------------
//! @file TPersistent.cpp
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
// TPersistent
//---------------------------------------------------------------------------

TPersistent::TPersistent(void): TObject() {
  // Initialisations
	FLastError = 0;
}

TPersistent::~TPersistent(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TPersistent::Get_Name(void) {
  return FName;
}

bool TPersistent::Set_Name(AnsiString NewName) {
  if (FName != NewName) {
    FName = NewName;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LastError
//---------------------------------------------------------------------------

int TPersistent::Get_LastError(void) {
  return FLastError;
}

//---------------------------------------------------------------------------
// Message d'erreur correspondant à un numéro
//---------------------------------------------------------------------------

AnsiString FASTCALL TPersistent::GetErrorMessage(int ErrorNumber) {
	TCHAR szBuf[256];

	// Pour les codes d'erreur de type HRESULT (0x80000000 et des poussières)
	if (FACILITY_WINDOWS == HRESULT_FACILITY(ErrorNumber)) ErrorNumber = HRESULT_CODE(ErrorNumber); 

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ErrorNumber, 0, szBuf, 256, NULL);

	return szBuf;
}

//---------------------------------------------------------------------------
// Copie d'un composant sur un autre
//---------------------------------------------------------------------------

void FASTCALL TPersistent::Assign(TPersistent *Source) {
	EConvertError E;
	throw(E);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPersistent::GetListProperties(TStrings *ListProperties) {
  ListProperties->Add(_T("Name"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPersistent::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Name")) {
    return tpText;
  }
  return tpNull;
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TPersistent::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Name")) {
    return _T("");
  }
  return _T("");
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TPersistent::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Name")) {
    return Get_Name();
  }
  return false;
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPersistent::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Name")) {
    Set_Name(asValue);
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------
AnsiString TPersistent::UnMot(AnsiString asLigne, AnsiString asDelim1, AnsiString asDelim2, int *Cur) {
  int Deb, Fin, Len;


  Len = asLigne.Length();
  if (*Cur >= (int) Len) {
    *Cur = Len + 1;
    return _T("");
  }

  Deb = (int) asLigne.find_first_not_of(asDelim1, *Cur);
  if (Deb == -1) Deb = Len;
  Fin = (int) asLigne.find_first_of(asDelim2, Deb);
  if (Fin == -1) Fin = Len;
  *Cur = Fin + 1;

  return AnsiString(asLigne.substr(Deb, Fin - Deb));
}

//---------------------------------------------------------------------------
AnsiString TPersistent::SuppGuillVCL(AnsiString asTexte) {
  int i;
  TCHAR c;
  bool bGuill;
  AnsiString asRetour;
  AnsiString asCarac;


  asRetour = _T("");
  asCarac = _T("");
  bGuill = false;

  // Il n'y a pas nécessairement un guillemet ni au début ni à la fin
  // Exemple: Caption = #39
  for (i = 1; i <= (int) asTexte.Length(); i++) {
    c = asTexte[i];
    if (c == _T('\'')) {
      if (!bGuill && asCarac.Length() >= 2 && asCarac[1] == _T('#')) {
        asCarac = asCarac.SubString(2, 3);
        c = StrToInt(asCarac);
        asRetour += c;
      }
      bGuill = !bGuill;
    }
    else if (!bGuill && c == _T('#')) {
      if (asCarac.Length() >= 2 && asCarac[1] == _T('#')) {
        asCarac = asCarac.SubString(2, 3);
        c = StrToInt(asCarac);
        asRetour += c;
      }
      asCarac = _T("#");
    }
    else {
      if (bGuill) {
        asCarac = _T("");
        asRetour += c;
      }
      else if (c != _T(' ')) {
        asCarac += c;
      }
    }
  }
  if (!bGuill && asCarac.Length() >= 2 && asCarac[1] == _T('#')) {
    asCarac = asCarac.SubString(2, 3);
    c = StrToInt(asCarac);
    asRetour += c;
  }

  return asRetour;
}

//---------------------------------------------------------------------------
AnsiString TPersistent::FormatProperty(AnsiString asLigne,
      AnsiString asDeb, AnsiString asFin, AnsiString asDelim) {
  AnsiString asRetour;
  bool bDelim;
  int i;


  asRetour = asDeb;
  bDelim = false;
  for (i = 1; i <= (int) asLigne.Length(); i++) {
    if (asLigne[i] == _T('\n')) {
      bDelim = true;
    }
    else {
      if (bDelim) {
        asRetour += asDelim;
        bDelim = false;
      }
      asRetour += asLigne[i];
    }
  }
  asRetour += asFin;

  return asRetour;
}
//---------------------------------------------------------------------------

