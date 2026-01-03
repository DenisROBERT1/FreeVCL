//-----------------------------------------------------------------------------
//! @file TMetafile.cpp
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
#include <FreeVcl_XFiles.h>

//---------------------------------------------------------------------------
//
//                            TMetafile
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TMetafile::TMetafile(TComponent *AOwner): TGraphic() {
  FWidth = 0;
  FHeight = 0;
  FEmpty = true;
  FCanvas = NULL;
  FHandle = NULL;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TMetafile::~TMetafile(void) {
  delete FCanvas;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Canvas
//---------------------------------------------------------------------------

TCanvas *TMetafile::Get_Canvas(void) {
  if (FCanvas == NULL) {
    FCanvas = new TCanvas();
    Clear();
  }
  return FCanvas;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CreatedBy
//---------------------------------------------------------------------------

AnsiString TMetafile::Get_CreatedBy() {
  return FCreatedBy;
}

bool TMetafile::Set_CreatedBy(AnsiString NewCreatedBy) {
  if (FCreatedBy != NewCreatedBy) {
    FCreatedBy = NewCreatedBy;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Description
//---------------------------------------------------------------------------

AnsiString TMetafile::Get_Description() {
  return FDescription;
}

bool TMetafile::Set_Description(AnsiString NewDescription) {
  if (FDescription != NewDescription) {
    FDescription = NewDescription;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Empty
//---------------------------------------------------------------------------

bool TMetafile::Get_Empty() {
  return FEmpty;
}

bool TMetafile::Set_Empty(bool NewEmpty) {
  if (FEmpty != NewEmpty) {
    FEmpty = NewEmpty;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Enhanced
//---------------------------------------------------------------------------

bool TMetafile::Get_Enhanced() {
  return FEnhanced;
}

bool TMetafile::Set_Enhanced(bool NewEnhanced) {
  if (FEnhanced != NewEnhanced) {
    FEnhanced = NewEnhanced;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HENHMETAFILE TMetafile::Get_Handle() {
  if (FHandle == NULL) {
    FHandle = CloseEnhMetaFile(FCanvas->Handle);
    delete FCanvas;
    FCanvas = NULL;
  }
  return FHandle;
}

bool TMetafile::Set_Handle(HENHMETAFILE NewHandle) {
  if (FHandle != NewHandle) {
    if (FHandle) DeleteEnhMetaFile(FHandle);
    FHandle = NewHandle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TMetafile::Get_Height() {
  return FHeight;
}

bool TMetafile::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Inch
//---------------------------------------------------------------------------

Word TMetafile::Get_Inch() {
  return FInch;
}

bool TMetafile::Set_Inch(Word NewInch) {
  if (FInch != NewInch) {
    FInch = NewInch;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MMHeight
//---------------------------------------------------------------------------

int TMetafile::Get_MMHeight() {
  return FMMHeight;
}

bool TMetafile::Set_MMHeight(int NewMMHeight) {
  if (FMMHeight != NewMMHeight) {
    FMMHeight = NewMMHeight;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MMWidth
//---------------------------------------------------------------------------

int TMetafile::Get_MMWidth() {
  return FMMWidth;
}

bool TMetafile::Set_MMWidth(int NewMMWidth) {
  if (FMMWidth != NewMMWidth) {
    FMMWidth = NewMMWidth;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Transparent
//---------------------------------------------------------------------------

bool TMetafile::Get_Transparent() {
  return FTransparent;
}

bool TMetafile::Set_Transparent(bool NewTransparent) {
  if (FTransparent != NewTransparent) {
    FTransparent = NewTransparent;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TMetafile::Get_Width() {
  return FWidth;
}

bool TMetafile::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
  }
  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TMetafile::Clear(void) {
  if (FCanvas != NULL) {
    LPRECT lpRect = NULL;
    if (FMMWidth && FMMHeight) {
      TRect Rect(0, 0, FMMWidth, FMMHeight);
      lpRect = &Rect;
    }
    FCanvas->Handle = CreateEnhMetaFile(NULL, NULL, lpRect, FDescription);
    SetMapMode(FCanvas->Handle, MM_ANISOTROPIC);
  }
}

//---------------------------------------------------------------------------
bool FASTCALL TMetafile::HandleAllocated(void) {
  return FHandle != NULL;
}

//---------------------------------------------------------------------------

bool FASTCALL TMetafile::ReleaseHandle(void) {
  if (FHandle) {
    DeleteEnhMetaFile(FHandle);
    FHandle = NULL;
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------
// Chargement à partir d'un flux.
//---------------------------------------------------------------------------

bool FASTCALL TMetafile::LoadFromStream(TStream* Stream) {
	return false;
}

//---------------------------------------------------------------------------
// Lecture à partir des ressources.
//---------------------------------------------------------------------------
bool FASTCALL TMetafile::LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName) {
	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TMetafile::LoadFromFile(const AnsiString asFileName) {
  TCHAR szDescription[1024];
  FHandle = GetEnhMetaFile(asFileName);
  GetEnhMetaFileDescription(FHandle, 1024, szDescription);
  FDescription = szDescription;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TMetafile::SaveToStream(TStream *Stream) {
	// $$$$$$$$$

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TMetafile::SaveToFile(const AnsiString asFileName) {
  Get_Handle();
  CopyEnhMetaFile(FHandle, asFileName);

	return true;
}

//---------------------------------------------------------------------------
// Lecture des données
//---------------------------------------------------------------------------
AnsiString FASTCALL TMetafile::GetData(void) {
	// $$$$$$$$$
	return _T("");
}

//---------------------------------------------------------------------------
// Dessin d'un graphique dans un HDC
//---------------------------------------------------------------------------
void FASTCALL TMetafile::Draw(HDC hdc, int X, int Y, int L, int H) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TMetafile::GetListProperties(TStrings *ListProperties) {

  TGraphic::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TMetafile::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TGraphic::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TMetafile::GetDefaultProperty(AnsiString asProperty) {
  return TGraphic::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TMetafile::GetProperty(AnsiString asProperty) {
  return TGraphic::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TMetafile::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  return TGraphic::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

