//-----------------------------------------------------------------------------
//! @file TIcon.cpp
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
// TIcon
//---------------------------------------------------------------------------

TIcon::TIcon(void): TGraphic() {
  // Initialisations
  FClassName = _T("TIcon");
  FHandle = NULL;
  FWidth = 0;
  FHeight = 0;
}

//---------------------------------------------------------------------------
TIcon::~TIcon(void) {
  int i;


  for (i = 0; i < (int) ListBitmaps.size(); i++) {
    delete ListBitmaps[i];
  }

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HICON TIcon::Get_Handle(void) {
  return FHandle;
}

bool TIcon::Set_Handle(HICON NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TIcon::Get_Width(void) {
  return FWidth;
}

bool TIcon::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TIcon::Get_Height(void) {
  return FHeight;
}

bool TIcon::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BitmapCount
//---------------------------------------------------------------------------

int TIcon::Get_BitmapCount(void) {
  return (int) ListBitmaps.size();
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Bitmap
//---------------------------------------------------------------------------

TBitmap *TIcon::Get_Bitmap(int i) {
  return ListBitmaps[i];
}


//---------------------------------------------------------------------------
// Copie d'une bitmap sur une autre
//---------------------------------------------------------------------------

void FASTCALL TIcon::Assign(TPersistent* Source) {
	TIcon *IconSource;
	int i;
	int BitmapCount;

	IconSource = dynamic_cast<TIcon *>(Source);
	if (IconSource == NULL) return;

	// Handle
	FHandle = IconSource->FHandle;

  // Dimensions
  FWidth = IconSource->FWidth;
  FHeight = IconSource->FHeight;

	// Bitmaps
	BitmapCount = IconSource->BitmapCount;
	for (i = 0; i < BitmapCount; i++) {
		AddBitmap(IconSource->Bitmap[i]);
	}
}

//---------------------------------------------------------------------------
bool FASTCALL TIcon::AddBitmap(TBitmap *NewBitmap) {
	TBitmap *Bitmap = new TBitmap();

	Bitmap->Assign(NewBitmap);
  ListBitmaps.push_back(Bitmap);

	return true;
}

//---------------------------------------------------------------------------
// Suppression d'une bitmap de l'icône
//---------------------------------------------------------------------------

bool FASTCALL TIcon::RemoveBitmap(int IndexBitmap) {
	if (IndexBitmap >= (int) ListBitmaps.size()) return false;

	delete ListBitmaps[IndexBitmap];
	ListBitmaps.erase(ListBitmaps.begin() + IndexBitmap);

	return true;
}

//---------------------------------------------------------------------------
// Chargement à partir d'un flux
//---------------------------------------------------------------------------

bool FASTCALL TIcon::LoadFromStream(TStream *Stream) {
	TCHAR szMsgErreur[256];
  AnsiString asMsgErreur;
  TICOFile *ICOFile;
	bool Ok = true;


  ICOFile = new TICOFile();

  if (!ICOFile->LoadFromStream(Stream, ListBitmaps, szMsgErreur)) {
		Ok = false;
  }

  delete ICOFile;

	return Ok;
}


//---------------------------------------------------------------------------
// Chargement à partir des ressources
//---------------------------------------------------------------------------

bool FASTCALL TIcon::LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName) {
	if (FWidth == 0 || FHeight == 0) {
		FWidth = GetSystemMetrics(SM_CXICON);
		FHeight = GetSystemMetrics(SM_CYICON);
	}
	FHandle = (HICON) LoadImage(hInstance, szResName, IMAGE_ICON, FWidth, FHeight, LR_DEFAULTCOLOR);

	return FHandle != NULL;
}

//---------------------------------------------------------------------------
bool FASTCALL TIcon::SaveToStream(TStream *Stream) {
	TCHAR szMsgErreur[256];
  AnsiString asMsgErreur;
  TICOFile *ICOFile;
	bool Ok = true;


  ICOFile = new TICOFile();

  if (!ICOFile->SaveToStream(Stream, ListBitmaps, szMsgErreur)) {
    asMsgErreur.sprintf(_TT("Impossible d'écrire dans le stream :\n%s"),
                        szMsgErreur);
    MessageBox(NULL, asMsgErreur, _TT("Erreur enregistrement fichier ICO"),
               MB_OK | MB_ICONSTOP);
		Ok = false;
  }

  delete ICOFile;

	return Ok;
}

//---------------------------------------------------------------------------
// Lecture des données
//---------------------------------------------------------------------------
AnsiString FASTCALL TIcon::GetData(void) {
	return _T("");
}

//---------------------------------------------------------------------------
// Dessin d'un graphique dans un HDC
//---------------------------------------------------------------------------
void FASTCALL TIcon::Draw(HDC hdc, int X, int Y, int L, int H) {
  DrawIconEx(hdc, X, Y, FHandle, L, H, 0, NULL, DI_NORMAL);
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TIcon::GetListProperties(TStrings *ListProperties) {

  TGraphic::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TIcon::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TGraphic::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TIcon::GetDefaultProperty(AnsiString asProperty) {
  return TGraphic::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TIcon::GetProperty(AnsiString asProperty) {
  return TGraphic::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TIcon::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TGraphic::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

