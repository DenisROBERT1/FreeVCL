//-----------------------------------------------------------------------------
//! @file TGIFImage.cpp
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
// TGIFImage
//---------------------------------------------------------------------------

TGIFImage::TGIFImage(void): TGraphic() {
  // Initialisations
  FClassName = _T("TGIFImage");
  FWidth = 0;
  FHeight = 0;
}

//---------------------------------------------------------------------------
TGIFImage::~TGIFImage(void) {
  int i;


  for (i = 0; i < (int) ListBitmaps.size(); i++) {
    delete ListBitmaps[i];
  }

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TGIFImage::Get_Width(void) {
  return FWidth;
}

bool TGIFImage::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TGIFImage::Get_Height(void) {
  return FHeight;
}

bool TGIFImage::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BitmapCount
//---------------------------------------------------------------------------

int TGIFImage::Get_BitmapCount(void) {
  return (int) ListBitmaps.size();
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Bitmap
//---------------------------------------------------------------------------

TBitmap *TGIFImage::Get_Bitmap(int i) {
  return ListBitmaps[i];
}


//---------------------------------------------------------------------------
// Copie d'une bitmap sur une autre
//---------------------------------------------------------------------------

void FASTCALL TGIFImage::Assign(TPersistent* Source) {
	TGIFImage *GIFImageSource;
	int i;
	int BitmapCount;

	GIFImageSource = dynamic_cast<TGIFImage *>(Source);
	if (GIFImageSource == NULL) return;

  // Dimensions
  FWidth = GIFImageSource->FWidth;
  FHeight = GIFImageSource->FHeight;

	// Bitmaps
	BitmapCount = GIFImageSource->BitmapCount;
	for (i = 0; i < BitmapCount; i++) {
		AddBitmap(GIFImageSource->Bitmap[i]);
	}
}

//---------------------------------------------------------------------------
bool FASTCALL TGIFImage::AddBitmap(TBitmap *NewBitmap) {
	TBitmap *Bitmap = new TBitmap();

	Bitmap->Assign(NewBitmap);
  ListBitmaps.push_back(Bitmap);

	return true;
}

//---------------------------------------------------------------------------
// Suppression d'une bitmap de l'image GIF
//---------------------------------------------------------------------------

bool FASTCALL TGIFImage::RemoveBitmap(int IndexBitmap) {
	if (IndexBitmap >= (int) ListBitmaps.size()) return false;

	delete ListBitmaps[IndexBitmap];
	ListBitmaps.erase(ListBitmaps.begin() + IndexBitmap);

	return true;
}

//---------------------------------------------------------------------------
// Chargement à partir d'un flux
//---------------------------------------------------------------------------

bool FASTCALL TGIFImage::LoadFromStream(TStream *Stream) {
	TCHAR szMsgErreur[256];
  AnsiString asMsgErreur;
  TGIFFile *GIFFile;
	bool Ok = true;


  GIFFile = new TGIFFile();

  if (!GIFFile->LoadFromStream(Stream, ListBitmaps, szMsgErreur)) {
    asMsgErreur.sprintf(_TT("Impossible de lire le stream :\n%s"),
                        szMsgErreur);
    MessageBox(NULL, asMsgErreur, _T("Erreur lecture fichier GIF"),
               MB_OK | MB_ICONSTOP);
		Ok = false;
  }

  delete GIFFile;

	return Ok;
}


//---------------------------------------------------------------------------
// Chargement à partir des ressources
//---------------------------------------------------------------------------

bool FASTCALL TGIFImage::LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName) {

	return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TGIFImage::SaveToStream(TStream *Stream) {
	TCHAR szMsgErreur[256];
  AnsiString asMsgErreur;
  TGIFFile *GIFFile;
	bool Ok = true;


  GIFFile = new TGIFFile();

  if (!GIFFile->SaveToStream(Stream, ListBitmaps, szMsgErreur)) {
    asMsgErreur.sprintf(_T("Impossible d'écrire dans le stream :\n%s"),
                        szMsgErreur);
    MessageBox(NULL, asMsgErreur, _T("Erreur enregistrement fichier GIF"),
               MB_OK | MB_ICONSTOP);
		Ok = false;
  }

  delete GIFFile;

	return Ok;
}

//---------------------------------------------------------------------------
// Lecture des données
//---------------------------------------------------------------------------
AnsiString FASTCALL TGIFImage::GetData(void) {
	return _T("");
}

//---------------------------------------------------------------------------
// Dessin d'un graphique dans un HDC
//---------------------------------------------------------------------------
void FASTCALL TGIFImage::Draw(HDC hdc, int X, int Y, int L, int H) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TGIFImage::GetListProperties(TStrings *ListProperties) {

  TGraphic::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TGIFImage::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TGraphic::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TGIFImage::GetDefaultProperty(AnsiString asProperty) {
  return TGraphic::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TGIFImage::GetProperty(AnsiString asProperty) {
  return TGraphic::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TGIFImage::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TGraphic::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

