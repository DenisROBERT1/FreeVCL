//-----------------------------------------------------------------------------
//! @file TBMPFile.cpp
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
// TBMPFile
//---------------------------------------------------------------------------

TBMPFile::TBMPFile(void) {
  // Initialisations
  FWidth = 0;
  FHeight = 0;
  FBitCount = 24;
  Compression = BI_RGB;
  lpDIBBits = NULL;
}

TBMPFile::~TBMPFile(void) {
  if (lpDIBBits) delete[] lpDIBBits;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TBMPFile::Get_Width(void) {
  return FWidth;
}

bool TBMPFile::Set_Width(int NewWidth) {

  if (FWidth != NewWidth) {
    FWidth = NewWidth;

    if (lpDIBBits) {
      delete[] lpDIBBits;
      lpDIBBits = NULL;
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TBMPFile::Get_Height(void) {
  return FHeight;
}

bool TBMPFile::Set_Height(int NewHeight) {

  if (FHeight != NewHeight) {
    FHeight = NewHeight;

    if (lpDIBBits) {
      delete[] lpDIBBits;
      lpDIBBits = NULL;
    }
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BitCount
//---------------------------------------------------------------------------

int TBMPFile::Get_BitCount(void) {
  return FBitCount;
}

bool TBMPFile::Set_BitCount(int NewBitCount) {

  if (FBitCount != NewBitCount) {
    FBitCount = NewBitCount;
    switch (FBitCount) {
      case 1: FPaletteVCL.NbColors = 2; break;
      case 4: FPaletteVCL.NbColors = 16; break;
      case 8: FPaletteVCL.NbColors = 256; break;
      default: FPaletteVCL.NbColors = 0; break;
    }
    if (lpDIBBits) {
      delete[] lpDIBBits;
      lpDIBBits = NULL;
    }
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Palette
//---------------------------------------------------------------------------

HPALETTE TBMPFile::Get_Palette(void) {
  return FPaletteVCL.Handle;
}

bool TBMPFile::Set_Palette(HPALETTE NewPalette) {

  FPaletteVCL.Handle = NewPalette;

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PaletteVCL
//---------------------------------------------------------------------------

TPaletteVCL *TBMPFile::Get_PaletteVCL(void) {
  return &FPaletteVCL;
}

bool TBMPFile::Set_PaletteVCL(TPaletteVCL *NewPaletteVCL) {

  FPaletteVCL = *NewPaletteVCL;

  return true;
}

//---------------------------------------------------------------------------
// Lecture d'un BMP à partir d'un flux
//---------------------------------------------------------------------------

BOOL TBMPFile::LoadFromStream(TStream *Stream, TCHAR *szMsgErreur) {
  BITMAPFILEHEADER bmfHeader;
  BITMAPINFO BitmapInfo;
  BITMAPCOREINFO BitmapCoreInfo;
  DWORD OffBits;
  int NbColors;
  DWORD dwSizeBits;
	DWORD dw;

  BOOL Ok = true;


  // Mise à zéro
  FWidth = 0;
  FHeight = 0;
  FBitCount = 0;
  Compression = 0;
  OffBits = 0;
  szMsgErreur[0] = _T('\0');

	// Lecture de l'entête
  dw = (DWORD) Stream->Read(&bmfHeader, sizeof(BITMAPFILEHEADER));
  if (dw != sizeof(BITMAPFILEHEADER)) {
    Ok = false;
    wsprintf(szMsgErreur, _T("Impossible de lire l'entête (fichier vide ?)"));
  }

  if (Ok) {
    if (bmfHeader.bfType != DIB_HEADER_MARKER) {
      Ok = false;
      wsprintf(szMsgErreur, _T("Format inconnu"));
    }
  }

  if (Ok) {
    // Lecture de la taille de la structure (BitmapInfo ou BitmapCoreInfo)
	  dw = (DWORD) Stream->Read(&BitmapInfo, sizeof(DWORD));
    if (dw != sizeof(DWORD)) {
      Ok = false;
      wsprintf(szMsgErreur, _T("Impossible de lire l'entête."));
    }
  }

  if (Ok) {
    // Lecture de BitmapInfo ou de BitmapCoreInfo
    if (BitmapInfo.bmiHeader.biSize == sizeof(BITMAPINFOHEADER)) {
		  dw = (DWORD) Stream->Read((LPSTR) &BitmapInfo + sizeof(DWORD), sizeof(BITMAPINFOHEADER) - sizeof(DWORD));
      if (dw != sizeof(BITMAPINFOHEADER) - sizeof(DWORD)) {
        Ok = false;
        wsprintf(szMsgErreur, _T("Impossible de lire l'entête."));
      }
      if (Ok) {
        FWidth = BitmapInfo.bmiHeader.biWidth;
        FHeight = BitmapInfo.bmiHeader.biHeight;
        FBitCount = BitmapInfo.bmiHeader.biBitCount;
        Compression = BitmapInfo.bmiHeader.biCompression;
        NbColors = BitmapInfo.bmiHeader.biClrUsed;
        FPaletteVCL.IsWin30 = true;
      }
    }
    else if (BitmapInfo.bmiHeader.biSize == sizeof(BITMAPCOREHEADER)) {
      BitmapCoreInfo.bmciHeader.bcSize = BitmapInfo.bmiHeader.biSize;
		  dw = (DWORD) Stream->Read((LPSTR) &BitmapCoreInfo + sizeof(DWORD), sizeof(BITMAPCOREHEADER) - sizeof(DWORD));
      if (dw != sizeof(BITMAPCOREHEADER) - sizeof(DWORD)) {
        Ok = false;
        wsprintf(szMsgErreur, _T("Impossible de lire l'entête."));
      }
      if (Ok) {
        FWidth = BitmapCoreInfo.bmciHeader.bcWidth;
        FHeight = BitmapCoreInfo.bmciHeader.bcHeight;
        FBitCount = BitmapCoreInfo.bmciHeader.bcBitCount;
        Compression = BI_RGB;
        NbColors = 0;
        FPaletteVCL.IsWin30 = false;
      }
    }
    else {
      Ok = false;
      wsprintf(szMsgErreur, _T("Format inconnu"));
    }
  }

  if (Ok) {
    if (NbColors == 0) NbColors = FPaletteVCL.NbColorsMax(FBitCount);
    FPaletteVCL.NbColors = NbColors;
    FPaletteVCL.ReadFromStream(Stream);

    if (lpDIBBits) delete[] lpDIBBits;
		dwSizeBits = FPaletteVCL.SizeBits(FBitCount, FWidth, FHeight);
    lpDIBBits = new char[dwSizeBits];

    // Lecture des bits
    OffBits = bmfHeader.bfOffBits;
		Stream->Position = OffBits;
	  Stream->Read(lpDIBBits, dwSizeBits);

  }

  return Ok;
}


//---------------------------------------------------------------------------
// Lecture d'un fichier BMP
//---------------------------------------------------------------------------

BOOL TBMPFile::LoadFromFile(const TCHAR * szFileName, TCHAR *szMsgErreur) {
	TFileStream BitmapStream(szFileName, fmOpenRead | fmShareDenyNone);
	return LoadFromStream(&BitmapStream, szMsgErreur);
}

//---------------------------------------------------------------------------
// Enregistrement d'un BMP dans un flux
//---------------------------------------------------------------------------

BOOL TBMPFile::SaveToStream(TStream *Stream, TCHAR *szMsgErreur) {
  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER BitmapInfoHeader;
  DWORD dwSizeBits;
	LongLong dw;

  BOOL Ok;


  // Mise à zéro
  szMsgErreur[0] = _T('\0');
	Ok = TRUE;

  // Calcul de la taille totale de l'image
  dwSizeBits = FPaletteVCL.SizeBits(FBitCount, FWidth, FHeight);

  // Enregistrement de l'entête
  memset(&bmfHeader, 0, sizeof(BITMAPFILEHEADER));
  bmfHeader.bfType = DIB_HEADER_MARKER;
  bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
                        sizeof(BITMAPINFOHEADER) +
                        FPaletteVCL.SizeInFile();
  bmfHeader.bfSize = bmfHeader.bfOffBits + dwSizeBits;
  dw = Stream->Write((LPSTR) &bmfHeader, sizeof(BITMAPFILEHEADER));
  if (dw != sizeof(BITMAPFILEHEADER)) {
    Ok = FALSE;
    wsprintf(szMsgErreur, _TT("Impossible d'enregistrer l'en-tête"));
  }

  if (Ok) {
    // Enregistrement de la structure BitmapInfoHeader
    memset(&BitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
    BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfoHeader.biWidth = FWidth;
    BitmapInfoHeader.biHeight = FHeight;
    BitmapInfoHeader.biPlanes = 1;
    BitmapInfoHeader.biBitCount = FBitCount;
    BitmapInfoHeader.biCompression = Compression;
    BitmapInfoHeader.biClrUsed = FPaletteVCL.NbColors;
    dw = Stream->Write((LPSTR) &BitmapInfoHeader, sizeof(BITMAPINFOHEADER));
    if (dw != sizeof(BITMAPINFOHEADER)) {
      Ok = FALSE;
      wsprintf(szMsgErreur, _TT("Impossible d'enregistrer les paramètres"));
    }
  }

  if (Ok) {

    // Ecriture des couleurs de la palette
    FPaletteVCL.WriteToStream(Stream);

  }

  if (Ok) {

    // Ecriture des bits
    dw = Stream->Write(lpDIBBits, dwSizeBits);
		if (dw != dwSizeBits) {
      Ok = FALSE;
      wsprintf(szMsgErreur, _TT("Impossible d'enregistrer l'image (écriture octet %u/%u)"), (long) dw, dwSizeBits);
    }

  }

  return Ok;
}

	//---------------------------------------------------------------------------
// Ecriture d'un fichier BMP
//---------------------------------------------------------------------------

BOOL TBMPFile::SaveToFile(const TCHAR * szFileName, TCHAR *szMsgErreur) {
  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER BitmapInfoHeader;
  DWORD dwSizeBits;

  HANDLE hFile;
  DWORD dw;

  BOOL Ok;


  // Mise à zéro
  szMsgErreur[0] = _T('\0');

  // Calcul de la taille totale de l'image
  dwSizeBits = FPaletteVCL.SizeBits(FBitCount, FWidth, FHeight);

  hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  Ok = (hFile != INVALID_HANDLE_VALUE);
  if (!Ok) wsprintf(szMsgErreur, _T("Impossible d'ouvrir le fichier\n%s"), szFileName);

  if (Ok) {
    // Enregistrement de l'entête
    memset(&bmfHeader, 0, sizeof(BITMAPFILEHEADER));
    bmfHeader.bfType = DIB_HEADER_MARKER;
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
                          sizeof(BITMAPINFOHEADER) +
                          FPaletteVCL.SizeInFile();
    bmfHeader.bfSize = bmfHeader.bfOffBits + dwSizeBits;
    WriteFile(hFile, (LPSTR) &bmfHeader, sizeof(BITMAPFILEHEADER), &dw, NULL);
    if (dw != sizeof(BITMAPFILEHEADER)) {
      Ok = false;
      wsprintf(szMsgErreur, _T("Impossible d'enregistrer l'en-tête"));
    }
  }

  if (Ok) {
    // Enregistrement de la structure BitmapInfoHeader
    memset(&BitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
    BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfoHeader.biWidth = FWidth;
    BitmapInfoHeader.biHeight = FHeight;
    BitmapInfoHeader.biPlanes = 1;
    BitmapInfoHeader.biBitCount = FBitCount;
    BitmapInfoHeader.biCompression = Compression;
    BitmapInfoHeader.biClrUsed = FPaletteVCL.NbColors;
    WriteFile(hFile, (LPSTR) &BitmapInfoHeader, sizeof(BITMAPINFOHEADER), &dw, NULL);
    if (dw != sizeof(BITMAPINFOHEADER)) {
      Ok = false;
      wsprintf(szMsgErreur, _T("Impossible d'enregistrer le fichier\n%s"), szFileName);
    }
  }

  if (Ok) {

    // Ecriture des couleurs de la palette
    FPaletteVCL.WriteToFile(hFile);

  }

  if (Ok) {

    // Ecriture des bits
    WriteFile(hFile, lpDIBBits, dwSizeBits, &dw, NULL);
		if (dw != dwSizeBits) Ok = FALSE;

  }

  if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

  return Ok;
}

//---------------------------------------------------------------------------
void TBMPFile::GetBits(HDC hdc, RECT &RectDC, RECT &RectImg) {
  LPBITMAPINFO lpBmi;


	if (!lpDIBBits) {
	  DWORD dwSizeBits = PaletteVCL->SizeBits(FBitCount, FWidth, FHeight);
    lpDIBBits = new char[dwSizeBits];
		memset(lpDIBBits, 0, dwSizeBits);
	}

  lpBmi = FPaletteVCL.GetBitmapInfo(FBitCount, FWidth, FHeight);

  // Make sure to use the stretching mode best for color pictures.
  SetStretchBltMode(hdc, COLORONCOLOR);

  StretchDIBits(hdc,                   // hdc
      RectDC.left,                     // DestX
      RectDC.top,                      // DestY
      RectDC.right - RectDC.left,      // nDestWidth
      RectDC.bottom - RectDC.top,      // nDestHeight
      RectImg.left,                    // SrcX
      RectImg.top,                     // SrcY
      RectImg.right - RectImg.left,    // wSrcWidth
      RectImg.bottom - RectImg.top,    // wSrcHeight
      lpDIBBits,                       // lpBits
      lpBmi,                           // lpBitsInfo
      DIB_RGB_COLORS,                  // wUsage
      SRCCOPY);                        // dwROP

  delete lpBmi;

}

//---------------------------------------------------------------------------
void TBMPFile::GetBits(HDC hdc, HBITMAP hBitmap) {
  LPBITMAPINFO lpBmi;


	if (!lpDIBBits) {
	  DWORD dwSizeBits = PaletteVCL->SizeBits(FBitCount, FWidth, FHeight);
    lpDIBBits = new char[dwSizeBits];
		memset(lpDIBBits, 0, dwSizeBits);
	}

  lpBmi = FPaletteVCL.GetBitmapInfo(FBitCount, FWidth, FHeight);

  SetDIBits(
    hdc,                      // handle to device context
    hBitmap,                  // handle to bitmap
    0,                        // starting scan line
    FHeight,                  // number of scan lines
    lpDIBBits,                // array of bitmap bits
    lpBmi,                    // address of structure with bitmap data
    DIB_RGB_COLORS            // type of color indexes to use
  );

  delete lpBmi;

}

//---------------------------------------------------------------------------
void TBMPFile::GetBits(void *lpBits, DWORD MaxSize) {
  DWORD dwSizeBits;

	dwSizeBits = FPaletteVCL.SizeBits(FBitCount, FWidth, FHeight);
	if (dwSizeBits > MaxSize) dwSizeBits = MaxSize;

	if (!lpDIBBits) {
    lpDIBBits = new char[dwSizeBits];
		memset(lpDIBBits, 0, dwSizeBits);
	}

	memcpy(lpBits, lpDIBBits, dwSizeBits);
}

//---------------------------------------------------------------------------
void TBMPFile::SetBits(HDC hdc, HBITMAP hBitmap) {
  LPBITMAPINFO lpBmi;


	if (!lpDIBBits) {
	  DWORD dwSizeBits = PaletteVCL->SizeBits(FBitCount, FWidth, FHeight);
    lpDIBBits = new char[dwSizeBits];
		memset(lpDIBBits, 0, dwSizeBits);
	}

  lpBmi = FPaletteVCL.GetBitmapInfo(FBitCount, FWidth, FHeight);

  GetDIBits(
      hdc,                             // handle to device context
      hBitmap,                         // handle to bitmap
      0,                               // first scan line to set in destination bitmap
      FHeight,                         // number of scan lines to copy
      lpDIBBits,                       // address of array for bitmap bits
      lpBmi,                           // address of structure with bitmap data
      DIB_RGB_COLORS                   // RGB or palette index
  );

  delete lpBmi;

}
//---------------------------------------------------------------------------

