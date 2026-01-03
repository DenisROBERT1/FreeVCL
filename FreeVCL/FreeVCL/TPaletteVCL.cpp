//-----------------------------------------------------------------------------
//! @file TPaletteVCL.cpp
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
// TPaletteVCL
//---------------------------------------------------------------------------

TPaletteVCL::TPaletteVCL(void) {
  // Initialisations
  FNbColors = 0;
  bmiColors = NULL;
  FIsWin30 = true;
  NbBits = 0;
  Byte = 0;
  FHandle = NULL;
}

TPaletteVCL::~TPaletteVCL(void) {
  delete[] bmiColors;
	if (FHandle != NULL) DeleteObject(FHandle);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HPALETTE TPaletteVCL::Get_Handle(void) {

	if (FHandle == NULL) FHandle = ColorToHandle();

	return FHandle;	
}

bool TPaletteVCL::Set_Handle(HPALETTE NewHandle) {

	if (FHandle != NewHandle) {
		if (FHandle != NULL) DeleteObject(FHandle);
		FHandle = NewHandle;
	  HandleToColor(FHandle);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NbColors
//---------------------------------------------------------------------------

int TPaletteVCL::Get_NbColors(void) {
  return FNbColors;
}

bool TPaletteVCL::Set_NbColors(int NewNbColors) {
  RGBQUAD *NewbmiColors;

  if (FNbColors != NewNbColors) {
    NewbmiColors = new RGBQUAD[NewNbColors];
		memcpy(NewbmiColors, bmiColors, ((FNbColors < NewNbColors)? FNbColors: NewNbColors) * sizeof(RGBQUAD));
    FNbColors = NewNbColors;
    delete[] bmiColors;
    bmiColors = NewbmiColors;
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété IsWin30
//---------------------------------------------------------------------------

bool TPaletteVCL::Get_IsWin30(void) {
  return FIsWin30;
}

bool TPaletteVCL::Set_IsWin30(bool NewIsWin30) {

  if (FIsWin30 != NewIsWin30) {
    FIsWin30 = NewIsWin30;
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Color
//---------------------------------------------------------------------------

RGBQUAD TPaletteVCL::Get_Color(int i) {
  if (0 <= i && i < NbColors) return bmiColors[i];
	else {
		RGBQUAD rgbQuad = {0, 0, 0, 0};
		return rgbQuad;
	}
}

bool TPaletteVCL::Set_Color(int i, RGBQUAD NewColor) {

  if (0 <= i && i < NbColors) bmiColors[i] = NewColor;

  return true;
}

//---------------------------------------------------------------------------
int TPaletteVCL::ReadFromStream(TStream *Stream) {
  DWORD dw;
  int Retour = 0;

  int i;


  if (FNbColors) {

    // Lecture des couleurs de la palette
    if (FIsWin30) {
			dw = (DWORD) Stream->Read(bmiColors, FNbColors * sizeof(RGBQUAD));
      Retour = dw;
    }
    else {
      Retour = 0;
      for (i = 0; i < FNbColors; i++) {
				dw = (DWORD) Stream->Read(&bmiColors[i], sizeof(RGBTRIPLE));
        Retour += dw;
      }
    }

  }

  return Retour;
}

//---------------------------------------------------------------------------
int TPaletteVCL::ReadFromFile(HANDLE hFile) {
  DWORD dw;
  int Retour = 0;

  int i;


  if (FNbColors) {

    // Lecture des couleurs de la palette
    if (FIsWin30) {
      ReadFile(hFile, bmiColors, FNbColors * sizeof(RGBQUAD), &dw, NULL);
      Retour = dw;
    }
    else {
      Retour = 0;
      for (i = 0; i < FNbColors; i++) {
        ReadFile(hFile, &bmiColors[i], sizeof(RGBTRIPLE), &dw, NULL);
        Retour += dw;
      }
    }

  }

  return Retour;
}

//---------------------------------------------------------------------------
int TPaletteVCL::SizeInFile(void) {

  if (FNbColors) {
    if (FIsWin30) return FNbColors * sizeof(RGBQUAD);
    else return FNbColors * sizeof(RGBTRIPLE);
  }

  return 0;
}

//---------------------------------------------------------------------------
LongLong TPaletteVCL::WriteToStream(TStream *Stream) {
  LongLong dw;
  LongLong Retour = 0;

  int i;


  if (FNbColors) {

    // Ecriture des couleurs de la palette
    if (FIsWin30) {
      dw = Stream->Write(bmiColors, FNbColors * sizeof(RGBQUAD));
      Retour = dw;
    }
    else {
      Retour = 0;
      for (i = 0; i < FNbColors; i++) {
        dw = Stream->Write(&bmiColors[i], sizeof(RGBTRIPLE));
        Retour += dw;
      }
    }

  }

  return Retour;
}

//---------------------------------------------------------------------------
int TPaletteVCL::WriteToFile(HANDLE hFile) {
  DWORD dw;
  int Retour = 0;

  int i;


  if (FNbColors) {

    // Ecriture des couleurs de la palette
    if (FIsWin30) {
      WriteFile(hFile, bmiColors, FNbColors * sizeof(RGBQUAD), &dw, NULL);
      Retour = dw;
    }
    else {
      Retour = 0;
      for (i = 0; i < FNbColors; i++) {
        WriteFile(hFile, &bmiColors[i], sizeof(RGBTRIPLE), &dw, NULL);
        Retour += dw;
      }
    }

  }

  return Retour;
}

//---------------------------------------------------------------------------
int TPaletteVCL::WritePixelToFile(HANDLE hFile, COLORREF cr) {
  DWORD dw;
  int i;


  // Ecriture d'un pixel
  if (FNbColors) {
    for (i = 0; i < FNbColors; i++) {
      if (bmiColors[i].rgbRed == GetRValue(cr) &&
          bmiColors[i].rgbGreen == GetGValue(cr) &&
          bmiColors[i].rgbBlue == GetBValue(cr)) break;
    }
    if (i == FNbColors) i = 0;

    if (FNbColors == 2) {
      Byte <<= 1;
      Byte |= i;
      NbBits++;
    }
    else if (FNbColors <= 16) {
      Byte <<= 4;
      Byte |= i;
      NbBits += 4;
    }
    else if (FNbColors <= 256) {
      Byte = i;
      NbBits = 8;
    }
    if (NbBits == 8) {
      WriteFile(hFile, &Byte, 1, &dw, NULL);
      NbBits = 0;
      Byte = 0;
    }
    else dw = 0;
  }
  else {
		// Attention: l'ordre est inversé par rapport à cr (BGR
		// au lieu de RGB) parce qu'on est en little endian
		unsigned long BGR;
		BGR = ((cr & 0xFF) << 16) | (cr & 0xFF00) | ((cr & 0xFF0000) >> 16);
    WriteFile(hFile, &BGR, sizeof(RGBTRIPLE), &dw, NULL);
  }

  return dw;
}

//---------------------------------------------------------------------------
int TPaletteVCL::WritePixelToStream(TStream *Stream, COLORREF cr) {
  DWORD dw;
  int i;


  // Ecriture d'un pixel
  if (FNbColors) {
    for (i = 0; i < FNbColors; i++) {
      if (bmiColors[i].rgbRed == GetRValue(cr) &&
          bmiColors[i].rgbGreen == GetGValue(cr) &&
          bmiColors[i].rgbBlue == GetBValue(cr)) break;
    }
    if (i == FNbColors) i = 0;

    if (FNbColors == 2) {
      Byte <<= 1;
      Byte |= i;
      NbBits++;
    }
    else if (FNbColors <= 16) {
      Byte <<= 4;
      Byte |= i;
      NbBits += 4;
    }
    else if (FNbColors <= 256) {
      Byte = i;
      NbBits = 8;
    }
    if (NbBits == 8) {
      dw = (DWORD) Stream->Write(&Byte, 1);
      NbBits = 0;
      Byte = 0;
    }
    else dw = 0;
  }
  else {
		// Attention: l'ordre est inversé par rapport à cr (BGR
		// au lieu de RGB) parce qu'on est en little endian
		unsigned long BGR;
		BGR = ((cr & 0xFF) << 16) | (cr & 0xFF00) | ((cr & 0xFF0000) >> 16);
    dw = (DWORD) Stream->Write(&BGR, sizeof(RGBTRIPLE));
  }

  return dw;
}

//---------------------------------------------------------------------------
HPALETTE TPaletteVCL::ColorToHandle(void) {
  LPLOGPALETTE lpLogPal;
  HPALETTE hPalette = NULL;
  int i;


  if (FNbColors) {

    lpLogPal = (LPLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * FNbColors];

    lpLogPal->palVersion = PALVERSION;
    lpLogPal->palNumEntries = FNbColors;

    for (i = 0; i < FNbColors; i++) {
      lpLogPal->palPalEntry[i].peRed   = bmiColors[i].rgbRed;
      lpLogPal->palPalEntry[i].peGreen = bmiColors[i].rgbGreen;
      lpLogPal->palPalEntry[i].peBlue  = bmiColors[i].rgbBlue;
      lpLogPal->palPalEntry[i].peFlags = 0;
    }

    hPalette = CreatePalette(lpLogPal);

    delete lpLogPal;
  }

  return hPalette;
}

//---------------------------------------------------------------------------
bool TPaletteVCL::HandleToColor(HPALETTE hPalette) {
  LPPALETTEENTRY lpPalEntry;
  int i;


  FNbColors = 0;
  GetObject(hPalette, sizeof(FNbColors), &FNbColors);

  lpPalEntry = (LPPALETTEENTRY) new BYTE[sizeof(PALETTEENTRY) * FNbColors];

  GetPaletteEntries(hPalette, 0, FNbColors, lpPalEntry);

  delete[] bmiColors;
  bmiColors = new RGBQUAD[FNbColors];

  for (i = 0; i < FNbColors; i++) {
    bmiColors[i].rgbRed   = lpPalEntry[i].peRed;
    bmiColors[i].rgbGreen = lpPalEntry[i].peGreen;
    bmiColors[i].rgbBlue  = lpPalEntry[i].peBlue;
    bmiColors[i].rgbReserved = 0;
  }

  delete lpPalEntry;

  return true;
}

//---------------------------------------------------------------------------
BITMAPINFO *TPaletteVCL::GetBitmapInfo(int BitCount, int Width, int Height) {
  LPBITMAPINFO lpBmi;
  int i;


  lpBmi = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFO) + FNbColors * sizeof(RGBQUAD)];
  memset(lpBmi, 0, sizeof(BITMAPINFO));
  lpBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  lpBmi->bmiHeader.biWidth = Width;
  lpBmi->bmiHeader.biHeight = Height;
  lpBmi->bmiHeader.biPlanes = 1;
  lpBmi->bmiHeader.biBitCount = BitCount;
  lpBmi->bmiHeader.biCompression = BI_RGB;
  lpBmi->bmiHeader.biSizeImage = 0;
  lpBmi->bmiHeader.biXPelsPerMeter = 0;
  lpBmi->bmiHeader.biYPelsPerMeter = 0;
  lpBmi->bmiHeader.biClrUsed = FNbColors;
  lpBmi->bmiHeader.biClrImportant = 0;
  for (i = 0; i < FNbColors; i++) {
    lpBmi->bmiColors[i] = bmiColors[i];
  }

  return lpBmi;
}

//---------------------------------------------------------------------------
const TPaletteVCL & TPaletteVCL::operator =(const TPaletteVCL &PaletteVCL) {
  FNbColors = PaletteVCL.FNbColors;
  FIsWin30 = PaletteVCL.FIsWin30;

  delete[] bmiColors;
  bmiColors = new RGBQUAD[FNbColors];
  memcpy(bmiColors, PaletteVCL.bmiColors, FNbColors * sizeof(RGBQUAD));

  return *this;
}

//---------------------------------------------------------------------------
int TPaletteVCL::SizeLine(int BitCount, int Width) {
  int nbPixelsParOctet;
  long SizeLine;


  // Nombre de bits par pixels
  switch (BitCount) {
    case 1: nbPixelsParOctet = 8; break;
    case 4: nbPixelsParOctet = 2; break;
    case 8: nbPixelsParOctet = 1; break;
    default: nbPixelsParOctet = 0; break;
  }

  // Calcul de la taille d'une ligne
  if (BitCount == 32) SizeLine = 4 * Width;
  else if (BitCount == 24) SizeLine = 3 * Width;
  else if (BitCount == 16) SizeLine = 2 * Width;
  else if (nbPixelsParOctet) SizeLine = Width / nbPixelsParOctet;
	else SizeLine = Width;
  while ((SizeLine % 4) != 0) SizeLine++;

  return SizeLine;
}

//---------------------------------------------------------------------------
DWORD TPaletteVCL::SizeBits(int BitCount, int Width, int Height) {
  return (DWORD) Height * SizeLine(BitCount, Width);
}

//---------------------------------------------------------------------------
int TPaletteVCL::NbColorsMax(int BitCount) {
  int NbColorsMax;

  // Nombre de couleurs maxi
  switch (BitCount) {
    case 1: NbColorsMax = 2; break;
    case 4: NbColorsMax = 16; break;
    case 8: NbColorsMax = 256; break;
    default: NbColorsMax = 0; break;
  }

  return NbColorsMax;
}

//---------------------------------------------------------------------------

