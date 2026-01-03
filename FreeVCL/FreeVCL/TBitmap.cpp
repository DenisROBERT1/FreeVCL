//-----------------------------------------------------------------------------
//! @file TBitmap.cpp
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
// TBitmap
//---------------------------------------------------------------------------

TBitmap::TBitmap(void): TGraphic() {
  // Initialisations
  FClassName = _T("TBitmap");
  FWidth = 1;
  FHeight = 1;
  FBitCount = 24;
  FXHotSpot = 0;
  FYHotSpot = 0;
	FPixelFormat = pf24bit;
  FPaletteVCL = NULL;
	FStretchBltMode = smColorOnColor;
  Compression = BI_RGB;
  FCanvas = NULL;
	hdcMem = NULL;
	hOldHandle = NULL;
  FHandle = NULL;
  lpvBits = NULL;
}

TBitmap::~TBitmap(void) {
	ReleaseHandleCanvas();
  if (FHandle) DeleteObject(FHandle);
  if (FCanvas) delete FCanvas;
  delete FPaletteVCL;
}

//---------------------------------------------------------------------------
bool FASTCALL TBitmap::SetHandleCanvas(void) {

	if (!hdcMem) {
		HDC hdc = GetDC(NULL);
		hdcMem = CreateCompatibleDC(hdc);
		ReleaseDC(NULL, hdc);
	}
  if (FCanvas) {
		FCanvas->Handle = hdcMem;
	}
  if (FHandle) {
    hOldHandle = (HBITMAP) SelectObject(hdcMem, FHandle);
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TBitmap::ReleaseHandleCanvas(void) {

	if (hdcMem) {
		if (hOldHandle) {
			SelectObject(hdcMem, hOldHandle);
			hOldHandle = NULL;
		}
	  if (FCanvas) {
			FCanvas->Handle = NULL;
		}
		DeleteDC(hdcMem);
		hdcMem = NULL;
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HBITMAP TBitmap::Get_Handle(void) {
  if (FHandle == NULL) {
		HDC hdc = GetDC(NULL);
		if (FPaletteVCL == NULL) FPaletteVCL = new TPaletteVCL();
		LPBITMAPINFO lpBmi = FPaletteVCL->GetBitmapInfo(FBitCount, FWidth, FHeight);
    FHandle = CreateDIBSection(NULL, lpBmi, DIB_RGB_COLORS, &lpvBits, NULL, 0);
		delete lpBmi;
    ReleaseDC(NULL, hdc);
  }
  return FHandle;
}

bool TBitmap::Set_Handle(HBITMAP NewHandle) {
	BITMAP Bitmap; 

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
	  memset(&Bitmap, 0, sizeof(BITMAP));
		if (GetObject(FHandle, sizeof(BITMAP), &Bitmap) != 0) {
			FWidth = Bitmap.bmWidth;
			FHeight = Bitmap.bmHeight;
		  BitCount = Bitmap.bmBitsPixel;
		}
		if (FCanvas) SetHandleCanvas();
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TBitmap::Get_Width(void) {
  return FWidth;
}

bool TBitmap::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TBitmap::Get_Height(void) {
  return FHeight;
}

bool TBitmap::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Canvas
//---------------------------------------------------------------------------

TCanvas *TBitmap::Get_Canvas(void) {
	bool bMAZ = false;

  if (FCanvas == NULL) {

    FCanvas = new TCanvas();
    if (!FHandle) {
			if (FPaletteVCL == NULL) FPaletteVCL = new TPaletteVCL();
			LPBITMAPINFO lpBmi = FPaletteVCL->GetBitmapInfo(FBitCount, FWidth, FHeight);
			FHandle = CreateDIBSection(NULL, lpBmi, DIB_RGB_COLORS, &lpvBits, NULL, 0);
			delete lpBmi;
			bMAZ = true;
    }
		SetHandleCanvas();
    if (bMAZ) PatBlt(hdcMem, 0, 0, FWidth, FHeight, BLACKNESS);
  }
  return FCanvas;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PixelFormat
//---------------------------------------------------------------------------

TPixelFormat TBitmap::Get_PixelFormat(void) {
  return FPixelFormat;
}

bool TBitmap::Set_PixelFormat(TPixelFormat NewPixelFormat) {
  if (FPixelFormat != NewPixelFormat) {
    switch (NewPixelFormat) {
      case pfDevice: FBitCount = 24; break;
      case pf1bit: FBitCount = 1; break;
      case pf4bit: FBitCount = 4; break;
      case pf8bit: FBitCount = 8; break;
      case pf15bit: FBitCount = 16; break;
      case pf16bit: FBitCount = 16; break;
      case pf24bit: FBitCount = 24; break;
      case pf32bit: FBitCount = 32; break;
      default: FBitCount = 24; break;
    }
    FPixelFormat = NewPixelFormat;
  }

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BitCount
//---------------------------------------------------------------------------

int TBitmap::Get_BitCount(void) {
  return FBitCount;
}

bool TBitmap::Set_BitCount(int NewBitCount) {
  if (FBitCount != NewBitCount) {
    switch (NewBitCount) {
			case 1: FPixelFormat = pf1bit; break;
			case 4: FPixelFormat = pf4bit; break;
			case 8: FPixelFormat = pf8bit; break;
			case 16: FPixelFormat = pf16bit; break;
			case 24: FPixelFormat = pf24bit; break;
			case 32: FPixelFormat = pf32bit; break;
      default: FPixelFormat = pfDevice; break;
    }
    FBitCount = NewBitCount;
  }

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Palette
//---------------------------------------------------------------------------

HPALETTE TBitmap::Get_Palette(void) {
	if (FPaletteVCL == NULL) FPaletteVCL = new TPaletteVCL();
  return FPaletteVCL->Handle;
}

bool TBitmap::Set_Palette(HPALETTE NewPalette) {

	if (FPaletteVCL == NULL) FPaletteVCL = new TPaletteVCL();
	FPaletteVCL->Handle = NewPalette;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PaletteVCL
//---------------------------------------------------------------------------

TPaletteVCL *TBitmap::Get_PaletteVCL(void) {
	if (FPaletteVCL == NULL) FPaletteVCL = new TPaletteVCL();
  return FPaletteVCL;
}

bool TBitmap::Set_PaletteVCL(TPaletteVCL *NewPaletteVCL) {

  if (FPaletteVCL != NewPaletteVCL) {
		delete FPaletteVCL;
		FPaletteVCL = NewPaletteVCL;
	}

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété StretchBltMode
//---------------------------------------------------------------------------

TStretchBltMode TBitmap::Get_StretchBltMode(void) {
  return FStretchBltMode;
}

bool TBitmap::Set_StretchBltMode(TStretchBltMode NewStretchBltMode) {

  if (FStretchBltMode != NewStretchBltMode) {
		FStretchBltMode = NewStretchBltMode;
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété XHotSpot
//---------------------------------------------------------------------------

int TBitmap::Get_XHotSpot(void) {
  return FXHotSpot;
}

bool TBitmap::Set_XHotSpot(int NewXHotSpot) {
  if (FXHotSpot != NewXHotSpot) {
    switch (NewXHotSpot) {
			case 1: FPixelFormat = pf1bit; break;
			case 4: FPixelFormat = pf4bit; break;
			case 8: FPixelFormat = pf8bit; break;
			case 16: FPixelFormat = pf16bit; break;
			case 24: FPixelFormat = pf24bit; break;
			case 32: FPixelFormat = pf32bit; break;
      default: FPixelFormat = pfDevice; break;
    }
    FXHotSpot = NewXHotSpot;
  }

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété YHotSpot
//---------------------------------------------------------------------------

int TBitmap::Get_YHotSpot(void) {
  return FYHotSpot;
}

bool TBitmap::Set_YHotSpot(int NewYHotSpot) {
  if (FYHotSpot != NewYHotSpot) {
    switch (NewYHotSpot) {
			case 1: FPixelFormat = pf1bit; break;
			case 4: FPixelFormat = pf4bit; break;
			case 8: FPixelFormat = pf8bit; break;
			case 16: FPixelFormat = pf16bit; break;
			case 24: FPixelFormat = pf24bit; break;
			case 32: FPixelFormat = pf32bit; break;
      default: FPixelFormat = pfDevice; break;
    }
    FYHotSpot = NewYHotSpot;
  }

	return true;
}


//---------------------------------------------------------------------------
// Copie d'une bitmap sur une autre
//---------------------------------------------------------------------------

void FASTCALL TBitmap::Assign(TPersistent* Source) {
	TBitmap *BitmapSource;

	BitmapSource = dynamic_cast<TBitmap *>(Source);
	if (BitmapSource == NULL) return;

  // Dimensions
  FWidth = BitmapSource->FWidth;
  FHeight = BitmapSource->FHeight;
  FBitCount = BitmapSource->FBitCount;
  FXHotSpot = BitmapSource->FXHotSpot;
  FYHotSpot = BitmapSource->FYHotSpot;
  FPixelFormat = BitmapSource->FPixelFormat;

  // Palette
	if (BitmapSource->PaletteVCL != NULL) {
		if (FPaletteVCL == NULL) FPaletteVCL = new TPaletteVCL();
		*FPaletteVCL = *BitmapSource->PaletteVCL;
	}

  // Destruction du Canvas pour forcer sa création avec les nouvelles dimensions
	if (FCanvas) {
		delete FCanvas;
		FCanvas = NULL;
	}
  if (FHandle) {
		DeleteObject(FHandle);
		FHandle = NULL;
	}

  Canvas->Draw(0, 0, BitmapSource);
}

//---------------------------------------------------------------------------
// Libère hdcmem et la référence du handle dans le Canvas.
//---------------------------------------------------------------------------

void FASTCALL TBitmap::FreeImage(void) {
	ReleaseHandleCanvas();
}

//---------------------------------------------------------------------------
// Indique si l'objet a acquis un handle pour l'objet GDI sous-jacent.
//---------------------------------------------------------------------------

bool FASTCALL TBitmap::HandleAllocated(void) {
  return (FHandle != NULL);
}


//---------------------------------------------------------------------------
// Chargement à partir d'un flux
//---------------------------------------------------------------------------

bool FASTCALL TBitmap::LoadFromStream(TStream *Stream) {
	TCHAR szMsgErreur[256];
  AnsiString asMsgErreur;
  TBMPFile *BMPFile;
	bool Ok;


  BMPFile = new TBMPFile();

  if (BMPFile->LoadFromStream(Stream, szMsgErreur)) {

    // Initialisation dimensions
    FWidth = BMPFile->Width;
    FHeight = BMPFile->Height;
    BitCount = BMPFile->BitCount;  // Pas FBitCount = ...
		FXHotSpot = 0;
		FYHotSpot = 0;

    // Initialisation palette
		// Supprimé, c'est toujours vrai : if (BMPFile->PaletteVCL) {
		if (FPaletteVCL == NULL) FPaletteVCL = new TPaletteVCL();
		*FPaletteVCL = *BMPFile->PaletteVCL;

	  LPBITMAPINFO lpBmi = FPaletteVCL->GetBitmapInfo(FBitCount, FWidth, FHeight);
	  if (FHandle) DeleteObject(FHandle);
		FHandle = CreateDIBSection(NULL, lpBmi, DIB_RGB_COLORS, &lpvBits, NULL, 0);
		delete lpBmi;

    // Dessin
		DWORD dwSizeBits = FPaletteVCL->SizeBits(FBitCount, FWidth, FHeight);
    BMPFile->GetBits(lpvBits, dwSizeBits);

    // Initialisation du canvas
		if (FCanvas) SetHandleCanvas();

		Ok = true;
  }
  else {
    asMsgErreur.sprintf(_T("Impossible de lire le fichier BMP\n%s"), szMsgErreur);
    MessageBox(NULL, asMsgErreur, _T("Erreur lecture fichier BMP"),
               MB_OK | MB_ICONSTOP);
		Ok = false;
  }

  delete BMPFile;

	return Ok;
}

//---------------------------------------------------------------------------
// Chargement à partir des ressources
//---------------------------------------------------------------------------

bool FASTCALL TBitmap::LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName) {
  BITMAP Bitmap;

  if (FHandle) DeleteObject(FHandle);

  FHandle = (HBITMAP) LoadImage(hInstance, szResName, IMAGE_BITMAP, 0, 0, 0);
	if (FHandle == NULL) return false;

  memset(&Bitmap, 0, sizeof(BITMAP));
	if (GetObject(FHandle, sizeof(BITMAP), &Bitmap) != 0) {
		FWidth = Bitmap.bmWidth;
		FHeight = Bitmap.bmHeight;
		BitCount = Bitmap.bmBitsPixel;
	}
	if (FCanvas) SetHandleCanvas();

	return true;
}

//---------------------------------------------------------------------------
// Retourne et libère le handle de bitmap.
//---------------------------------------------------------------------------

HBITMAP FASTCALL TBitmap::ReleaseHandle(void) {
	HBITMAP MemoHandle = FHandle;
	ReleaseHandleCanvas();
	if (FHandle) {
		DeleteObject(FHandle);
		FHandle = NULL;
	}

	return MemoHandle;
}

//---------------------------------------------------------------------------
// Enregistrement dans un fichier
//---------------------------------------------------------------------------

bool FASTCALL TBitmap::SaveToStream(TStream *Stream) {
	TCHAR szMsgErreur[256];
  AnsiString asMsgErreur;
  TBMPFile *BMPFile;
  TCanvas *ACanvas;
	bool Ok = true;


  BMPFile = new TBMPFile();

  // Dimensions
  BMPFile->Width = FWidth;
  BMPFile->Height = FHeight;
  BMPFile->BitCount = FBitCount;

  // Palette
  if (FPaletteVCL) *BMPFile->PaletteVCL = *FPaletteVCL;

  ACanvas = Get_Canvas();
  BMPFile->SetBits(ACanvas->Handle, FHandle);

  if (!BMPFile->SaveToStream(Stream, szMsgErreur)) {
    asMsgErreur.sprintf(_T("Impossible d'enregistrer le fichier\n%s"),
                        szMsgErreur);
    MessageBox(NULL, asMsgErreur, _T("Erreur enregistrement fichier BMP"),
               MB_OK | MB_ICONSTOP);
		Ok = false;
  }

  delete BMPFile;

	return Ok;
}

//---------------------------------------------------------------------------
// Enregistrement dans un fichier
//---------------------------------------------------------------------------

bool FASTCALL TBitmap::SaveToFile(const AnsiString asFileName) {
	TCHAR szMsgErreur[256];
  AnsiString asMsgErreur;
  TBMPFile *BMPFile;
  TCanvas *ACanvas;
	bool Ok = true;


  BMPFile = new TBMPFile();

  // Dimensions
  BMPFile->Width = FWidth;
  BMPFile->Height = FHeight;
  BMPFile->BitCount = FBitCount;

  // Palette
  if (FPaletteVCL) *BMPFile->PaletteVCL = *FPaletteVCL;

  ACanvas = Get_Canvas();
  BMPFile->SetBits(ACanvas->Handle, FHandle);

  if (!BMPFile->SaveToFile(asFileName, szMsgErreur)) {
    asMsgErreur.sprintf(_T("Impossible d'enregistrer le fichier %s\n%s"),
                        (LPCTSTR) asFileName, szMsgErreur);
    MessageBox(NULL, asMsgErreur, _T("Erreur enregistrement fichier BMP"),
               MB_OK | MB_ICONSTOP);
		Ok = false;
  }

  delete BMPFile;

	return Ok;
}

//---------------------------------------------------------------------------
// Lecture de la taille des données
//---------------------------------------------------------------------------
LongLong FASTCALL TBitmap::GetFileSize(void) {
	LongLong FileSize;


	if (FPaletteVCL == NULL) return 0;  // La palette doit obligatoirement être définie

	FileSize = sizeof(BITMAPFILEHEADER) +
						 sizeof(BITMAPINFOHEADER) +
             FPaletteVCL->SizeInFile() +
						 FPaletteVCL->SizeBits(FBitCount, FWidth, FHeight);

	return FileSize;
}

//---------------------------------------------------------------------------
// Lecture des données
//---------------------------------------------------------------------------
AnsiString FASTCALL TBitmap::GetData(void) {
	BYTE *Content;
	TCHAR *ContentHex;
	DWORD FileSize;
	AnsiString asData;


	FileSize = (DWORD) GetFileSize();
	Content = new BYTE[sizeof(DWORD) + FileSize];  // Longueur + contenu
	ContentHex = new TCHAR[2 * (sizeof(DWORD) + FileSize) + 1];

	*((DWORD *) Content) = FileSize;
	TMemoryStream MemoryStream(&Content[sizeof(DWORD)], FileSize);
	SaveToStream(&MemoryStream);
	BinToHex((const char *) Content, ContentHex, sizeof(DWORD) + FileSize);
	asData = AnsiString(ContentHex);

	delete[] ContentHex;
	delete[] Content;

	return asData;
}

//---------------------------------------------------------------------------
// Dessin d'un graphique dans un HDC
//---------------------------------------------------------------------------
void FASTCALL TBitmap::Draw(HDC hdc, int X, int Y, int L, int H) {

	SetStretchBltMode(hdc, FStretchBltMode);
	StretchBlt(hdc, X, Y, L, H,
						 Canvas->Handle, 0, 0, FWidth, FHeight, SRCCOPY);  // Pas FCanvas pour provoquer éventuellement sa création
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TBitmap::GetListProperties(TStrings *ListProperties) {

  TGraphic::GetListProperties(ListProperties);

  ListProperties->Add(_T("Handle"));
  ListProperties->Add(_T("PixelFormat"));
  ListProperties->Add(_T("BitCount"));
  ListProperties->Add(_T("StretchBltMode"));
  ListProperties->Add(_T("XHotSpot"));
  ListProperties->Add(_T("YHotSpot"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TBitmap::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Handle")) {
    return tpNumber;
  }
  if (asProperty == _T("PixelFormat")) {
    *asInfos = _T("pfDevice\npf1bit\npf4bit\npf8bit\npf15bit\npf16bit\npf24bit\npf32bit\npfCustom");
    return tpChoice;
  }
  if (asProperty == _T("BitCount")) {
    return tpNumber;
  }
  if (asProperty == _T("StretchBltMode")) {
    *asInfos = _T("smBlackOnWhite\nsmWhiteOnBlack\nsmColorOnColor\nsmHalfTone");
    return tpChoice;
  }
  if (asProperty == _T("XHotSpot")) {
    return tpNumber;
  }
  if (asProperty == _T("YHotSpot")) {
    return tpNumber;
  }
  return TGraphic::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TBitmap::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Handle")) {
    return _T("0");
  }
  if (asProperty == _T("PixelFormat")) {
    return _T("pf24bit");
  }
  if (asProperty == _T("BitCount")) {
    return _T("0");
  }
  if (asProperty == _T("StretchBltMode")) {
    return _T("smColorOnColor");
  }
  if (asProperty == _T("XHotSpot")) {
    return _T("0");
  }
  if (asProperty == _T("YHotSpot")) {
    return _T("0");
  }
  return TGraphic::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TBitmap::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Handle")) {
    return IntToStr((int) (size_t) Get_Handle());
  }
  if (asProperty == _T("PixelFormat")) {
    TPixelFormat PixelFormat = Get_PixelFormat();
    if (PixelFormat == pfDevice) return _T("pfDevice");
    if (PixelFormat == pf1bit) return _T("pf1bit");
    if (PixelFormat == pf4bit) return _T("pf4bit");
    if (PixelFormat == pf8bit) return _T("pf8bit");
    if (PixelFormat == pf15bit) return _T("pf15bit");
    if (PixelFormat == pf16bit) return _T("pf16bit");
    if (PixelFormat == pf24bit) return _T("pf24bit");
    if (PixelFormat == pf32bit) return _T("pf32bit");
    if (PixelFormat == pfCustom) return _T("pfCustom");
    return _T("");
  }
  if (asProperty == _T("BitCount")) {
    return IntToStr(Get_BitCount());
  }
  if (asProperty == _T("StretchBltMode")) {
    TStretchBltMode StretchBltMode = Get_StretchBltMode();
    if (StretchBltMode == smBlackOnWhite) return _T("smBlackOnWhite");
    if (StretchBltMode == smWhiteOnBlack) return _T("smWhiteOnBlack");
    if (StretchBltMode == smColorOnColor) return _T("smColorOnColor");
    if (StretchBltMode == smHalfTone) return _T("smHalfTone");
    return _T("");
  }
  if (asProperty == _T("XHotSpot")) {
    return IntToStr(Get_XHotSpot());
  }
  if (asProperty == _T("YHotSpot")) {
    return IntToStr(Get_YHotSpot());
  }
  return TGraphic::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TBitmap::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  if (asProperty == _T("Handle")) {
    Set_Handle((HBITMAP) (size_t) asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("PixelFormat")) {
    if      (asValue == _T("pfDevice")) Set_PixelFormat(pfDevice);
    else if (asValue == _T("pf1bit")) Set_PixelFormat(pf1bit);
    else if (asValue == _T("pf4bit")) Set_PixelFormat(pf4bit);
    else if (asValue == _T("pf8bit")) Set_PixelFormat(pf8bit);
    else if (asValue == _T("pf15bit")) Set_PixelFormat(pf15bit);
    else if (asValue == _T("pf16bit")) Set_PixelFormat(pf16bit);
    else if (asValue == _T("pf24bit")) Set_PixelFormat(pf24bit);
    else if (asValue == _T("pf32bit")) Set_PixelFormat(pf32bit);
    else if (asValue == _T("pfCustom")) Set_PixelFormat(pfCustom);
    return true;
  }
  if (asProperty == _T("BitCount")) {
    Set_BitCount(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("StretchBltMode")) {
    if      (asValue == _T("smBlackOnWhite")) Set_StretchBltMode(smBlackOnWhite);
    else if (asValue == _T("smWhiteOnBlack")) Set_StretchBltMode(smWhiteOnBlack);
    else if (asValue == _T("smColorOnColor")) Set_StretchBltMode(smColorOnColor);
    else if (asValue == _T("smHalfTone")) Set_StretchBltMode(smHalfTone);
    return true;
  }
  if (asProperty == _T("XHotSpot")) {
    Set_XHotSpot(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("YHotSpot")) {
    Set_YHotSpot(asValue.ToIntDef(0));
    return true;
  }
  return TPersistent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
