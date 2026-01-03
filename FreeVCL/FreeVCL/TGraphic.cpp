//-----------------------------------------------------------------------------
//! @file TGraphic.cpp
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
// TGraphic
//---------------------------------------------------------------------------

TGraphic::TGraphic(void): TPersistent() {
  // Initialisations
}

TGraphic::~TGraphic(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TGraphic::Get_Width(void) {
  return 0;
}

bool TGraphic::Set_Width(int NewWidth) {
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TGraphic::Get_Height(void) {
  return 0;
}

bool TGraphic::Set_Height(int NewHeight) {
  return true;
}

//---------------------------------------------------------------------------
// Chargement à partir des ressources
//---------------------------------------------------------------------------

bool FASTCALL TGraphic::LoadFromResourceID(HINSTANCE hInstance, int ResID) {
  return LoadFromResource(hInstance, MAKEINTRESOURCE(ResID));
}

//---------------------------------------------------------------------------
bool FASTCALL TGraphic::LoadFromResourceName(HINSTANCE hInstance, const AnsiString ResName) {
#ifdef __GNUG__
  // Erreur incompréhensible "error: unable to find a register to spill in class 'CLOBBERED_REGS'"
  LoadFromResource(hInstance, ResName); return true;
#else // __GNUG__
  return LoadFromResource(hInstance, ResName);
#endif // __GNUG__
}


//---------------------------------------------------------------------------
// Chargement à partir d'un fichier
//---------------------------------------------------------------------------

bool FASTCALL TGraphic::LoadFromFile(const AnsiString asFileName) {
  AnsiString asMsgErreur;


	TFileStream Stream(asFileName, fmOpenRead | fmShareDenyNone);
	if (Stream.Size == (LongLong) 0) {
		FLastError = Stream.LastError;
		return false;
	}

	return LoadFromStream(&Stream);
}


//---------------------------------------------------------------------------
// Enregistrement dans un fichier
//---------------------------------------------------------------------------

bool FASTCALL TGraphic::SaveToFile(const AnsiString asFileName) {

	TFileStream Stream(asFileName, fmOpenReadWrite | fmShareExclusive);

	return SaveToStream(&Stream);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TGraphic::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Width"));
  ListProperties->Add(_T("Height"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TGraphic::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Width")) {
    return tpNumber;
  }
  if (asProperty == _T("Height")) {
    return tpNumber;
  }
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TGraphic::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Width")) {
    return _T("-1");
  }
  if (asProperty == _T("Height")) {
    return _T("-1");
  }
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TGraphic::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Width")) {
    return IntToStr(Get_Width());
  }
  if (asProperty == _T("Height")) {
    return IntToStr(Get_Height());
  }
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TGraphic::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  if (asProperty == _T("Width")) {
    Set_Width(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Height")) {
    Set_Height(StrToInt(asValue));
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

