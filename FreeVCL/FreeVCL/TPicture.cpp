//-----------------------------------------------------------------------------
//! @file TPicture.cpp
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

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// TPicture
//---------------------------------------------------------------------------

TPicture::TPicture(void): TPersistent() {
  FGraphic = NULL;
}

//---------------------------------------------------------------------------
TPicture::~TPicture(void) {
  delete FGraphic;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TPicture::Get_Width(void) {
  return FGraphic->Width;
}

bool TPicture::Set_Width(int NewWidth) {
  FGraphic->Width = NewWidth;
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TPicture::Get_Height(void) {
  return FGraphic->Height;
}

bool TPicture::Set_Height(int NewHeight) {
  FGraphic->Height = NewHeight;
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Graphic
//---------------------------------------------------------------------------

TGraphic *TPicture::Get_Graphic(void) {
  return FGraphic;
}

bool TPicture::Set_Graphic(TGraphic *NewGraphic) {

	if (NewGraphic) {
		if (!FGraphic) {

			TBitmap *NewBitmap = dynamic_cast<TBitmap *>(NewGraphic);
			TIcon *NewIcon = dynamic_cast<TIcon *>(NewGraphic);
			TGIFImage *NewGIFImage = dynamic_cast<TGIFImage *>(NewGraphic);

			if (NewBitmap) FGraphic = (TBitmap *) new TBitmap();
			else if (NewIcon) FGraphic = (TIcon *) new TIcon();
			else if (NewGIFImage) FGraphic = (TGIFImage *) new TGIFImage();

		}
		if (FGraphic) FGraphic->Assign(NewGraphic);
	}
	else {
		delete FGraphic;
		FGraphic = NULL;
	}
  OnChange(this);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Bitmap
//---------------------------------------------------------------------------

TBitmap *TPicture::Get_Bitmap(void) {
  return dynamic_cast<TBitmap *>(FGraphic);
}

bool TPicture::Set_Bitmap(TBitmap *NewBitmap) {

	if (NewBitmap) {
		if (!FGraphic) FGraphic = (TBitmap *) new TBitmap();
		FGraphic->Assign(NewBitmap);
	}
	else {
		delete FGraphic;
		FGraphic = NULL;
	}
  OnChange(this);

  return true;
}


//---------------------------------------------------------------------------
// Chargement à partir des ressources
//---------------------------------------------------------------------------

bool FASTCALL TPicture::LoadFromResourceName(HINSTANCE hInstance, const AnsiString ResName) {
  if (FGraphic->LoadFromResourceName(hInstance, ResName)) return true;

	FLastError = FGraphic->LastError;

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TPicture::LoadFromFile(const AnsiString asFileName) {
  if (FGraphic->LoadFromFile(asFileName)) return true;

	FLastError = FGraphic->LastError;

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TPicture::SaveToFile(const AnsiString asFileName) {
  if (FGraphic->SaveToFile(asFileName)) return true;

	FLastError = FGraphic->LastError;

	return false;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPicture::GetListProperties(TStrings *ListProperties) {

  TPersistent::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPicture::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TPersistent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TPicture::GetDefaultProperty(AnsiString asProperty) {
  return TPersistent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TPicture::GetProperty(AnsiString asProperty) {
  return TPersistent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPicture::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

