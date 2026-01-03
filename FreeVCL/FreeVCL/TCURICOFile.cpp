//-----------------------------------------------------------------------------
//! @file TCURICOFile.cpp
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
// TCURICOFile
//---------------------------------------------------------------------------

TCURICOFile::TCURICOFile(void) {
  // Initialisations
	FileType = ciNull;
}

TCURICOFile::~TCURICOFile(void) {
}

//---------------------------------------------------------------------------
BOOL TCURICOFile::LoadFromStream(TStream *Stream,
															std::vector<TBitmap *> &ListBitmaps,
															TCHAR *szMsgErreur) {
	DWORD dw;
  HEADERCURICOFILE HeaderIcoFile;
  HEADERCURICO HeaderIco;
  BITMAPINFO BitmapInfo;
  int i, k, x, y;
  BYTE Mask = 0, Octet;
  TCanvas *Canvas;
  TBitmap *Bitmap;
  int WidthIco, HeightIco, SizeIco;
	int BitCount;
  DWORD dwSizeBits;
  char *lpDIBBits;
  LPBITMAPINFO lpBmi;
  bool bMasque;

  bool Ok = true;


  dw = (DWORD) Stream->Read(&HeaderIcoFile, sizeof(HEADERCURICOFILE));
  if (dw != sizeof(HEADERCURICOFILE)) {
    Ok = false;
    wsprintf(szMsgErreur, _TT("Impossible de lire l'en-tête du fichier"));
  }

  if (Ok) {
		if (HeaderIcoFile.FileType != FileType) {
			Ok = false;
			wsprintf(szMsgErreur, _TT("Type de fichier incorrect"));
		}
	}

  if (Ok) {

    for (i = 0; i < HeaderIcoFile.NbBlocks && Ok; i++) {

      Bitmap = new TBitmap();

      Stream->Seek((int) (sizeof(HEADERCURICOFILE) + i * sizeof(HEADERCURICO)), soBeginning);
      dw = (DWORD) Stream->Read(&HeaderIco, sizeof(HEADERCURICO));
      if (dw != sizeof(HEADERCURICO)) {
        Ok = false;
        wsprintf(szMsgErreur, _TT("Impossible de lire l'en-tête du bloc"));
      }

      if (Ok) {
        WidthIco = HeaderIco.bWidth;
        HeightIco = HeaderIco.bHeight;
        bMasque = true;
        if (WidthIco < HeightIco) SizeIco = WidthIco;
        else SizeIco = HeightIco;
        Bitmap->Width = SizeIco;
        Bitmap->Height = SizeIco * 2;

        Stream->Seek((int) HeaderIco.dwImageOffset, soBeginning);
        dw = (DWORD) Stream->Read((LPSTR) &BitmapInfo, sizeof(BITMAPINFOHEADER));
        if (dw != sizeof(BITMAPINFOHEADER)) {
          Ok = false;
          wsprintf(szMsgErreur, _TT("Fin de fichier atteinte"));
        }
      }

      if (Ok) {
        BitCount = BitmapInfo.bmiHeader.biBitCount;
				if (FileType == ciCUR) {
					Bitmap->XHotSpot = HeaderIco.wXHotSpot;
					Bitmap->YHotSpot = HeaderIco.wYHotSpot;
	        BitCount = BitmapInfo.bmiHeader.biBitCount;
				}
				else {
	        BitCount = HeaderIco.wBitCount;
				}
        Bitmap->BitCount = BitCount;
        Bitmap->PaletteVCL->NbColors = Bitmap->PaletteVCL->NbColorsMax(BitCount);
        Bitmap->PaletteVCL->ReadFromStream(Stream);

        // Lecture des bits
        dwSizeBits = Bitmap->PaletteVCL->SizeBits(BitCount, WidthIco, HeightIco);
        lpDIBBits = new char[dwSizeBits];
        dw = (DWORD) Stream->Read(lpDIBBits, dwSizeBits);
        lpBmi = Bitmap->PaletteVCL->GetBitmapInfo(BitCount, WidthIco, HeightIco);
        Canvas = Bitmap->Canvas;
        SetDIBits(
          Canvas->Handle,						// handle to device context
          Bitmap->Handle,           // handle to bitmap
          0,                        // starting scan line
          HeightIco,                // number of scan lines
          lpDIBBits,                // array of bitmap bits
          lpBmi,                    // address of structure with bitmap data
          DIB_RGB_COLORS            // type of color indexes to use
        );
        delete[] lpDIBBits;
        delete lpBmi;

        // Lecture du masque AND
        if (bMasque) {
          for (y = 0; y < SizeIco; y++) {
            k = 0;
            for (x = 0; x < SizeIco; x++) {
              if (k == 0) {
                dw = (DWORD) Stream->Read(&Octet, sizeof(Octet));
                Mask = 0x80;
              }
              if ((Octet & Mask) != 0) {
                Canvas->Set_Pixels(x, 2 * SizeIco - y - 1, RGB(255, 255, 255));
              }
              Mask >>= 1;
              if (++k == 8) k = 0;
            }
            for (k = SizeIco / 8; (k % 4) != 0; k++) {
              dw = (DWORD) Stream->Read(&Octet, sizeof(Octet));
            }
          }
        }

      }

      if (Ok) ListBitmaps.push_back(Bitmap);
      else delete Bitmap;
    }

  }

  return Ok? TRUE: FALSE;
}

//---------------------------------------------------------------------------
BOOL TCURICOFile::LoadFromFile(const TCHAR * szFileName,
														std::vector<TBitmap *> &ListBitmaps,
														TCHAR *szMsgErreur) {
	TFileStream BitmapStream(szFileName, fmOpenRead | fmShareDenyNone);
	return LoadFromStream(&BitmapStream, ListBitmaps, szMsgErreur);
}

//---------------------------------------------------------------------------
BOOL TCURICOFile::SaveToStream(TStream *Stream,
														std::vector<TBitmap *> &ListBitmaps,
														TCHAR *szMsgErreur) {
  DWORD dw;
  HEADERCURICOFILE HeaderIcoFile;
  HEADERCURICO HeaderIco;
  BITMAPINFOHEADER BitmapInfoHeader;
  int i, j, k, x, y;
  int cr, crRef1;
  BYTE Mask, Octet;
  int PosCurIconFile;
  TCanvas *Canvas;
  int WidthIco, HeightIco, SizeIco;
  bool bMasque;

  BOOL Ok = true;

  int dwBytesInRes[256];


  HeaderIcoFile.Data1 = 0;
  HeaderIcoFile.FileType = FileType;
  HeaderIcoFile.NbBlocks = (int) ListBitmaps.size();
  dw = (DWORD) Stream->Write(&HeaderIcoFile, sizeof(HEADERCURICOFILE));
  if (dw != sizeof(HEADERCURICOFILE)) {
    Ok = false;
    wsprintf(szMsgErreur, _TT("Impossible d'enregistrer l'en-tête HEADERCURICOFILE"));
  }

  if (Ok) {
    memset(&HeaderIco, 0, sizeof(HEADERCURICO));
    for (i = 0; i < (int) ListBitmaps.size(); i++) {
      dw = (DWORD) Stream->Write(&HeaderIco, sizeof(HEADERCURICO));
      if (dw != sizeof(HEADERCURICO)) {
        Ok = false;
        wsprintf(szMsgErreur, _TT("Impossible d'enregistrer l'en-tête HEADERCURICO"));
        break;
      }
    }
  }

  if (Ok) {

		TPaletteVCL *PaletteVCL = new TPaletteVCL;

		for (i = 0; i < (int) ListBitmaps.size() && i < 256; i++) {
      WidthIco = ListBitmaps[i]->Width;
      HeightIco = ListBitmaps[i]->Height;
      if (WidthIco < HeightIco) SizeIco = WidthIco;
      else SizeIco = HeightIco;
      bMasque = (ListBitmaps[i]->Height >= 2 * SizeIco);
      memset(&BitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
      BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
      BitmapInfoHeader.biWidth = SizeIco;
      BitmapInfoHeader.biHeight = 2 * SizeIco;
      BitmapInfoHeader.biPlanes = 1;
      BitmapInfoHeader.biBitCount = ListBitmaps[i]->BitCount;
      dw = (DWORD) Stream->Write(&BitmapInfoHeader, sizeof(BITMAPINFOHEADER));
      dwBytesInRes[i] = sizeof(BITMAPINFOHEADER);
      if (dw != sizeof(BITMAPINFOHEADER)) {
        Ok = false;
        wsprintf(szMsgErreur, _TT("Impossible d'enregistrer l'en-tête BITMAPINFOHEADER"));
        break;
      }

      // Ecriture des couleurs de la palette
      if (ListBitmaps[i]->PaletteVCL) *PaletteVCL = *(ListBitmaps[i]->PaletteVCL);
      dwBytesInRes[i] += (DWORD) PaletteVCL->WriteToStream(Stream);

      // Ecriture des bits de l'image
      Canvas = ListBitmaps[i]->Canvas;
      for (y = 0; y < SizeIco; y++) {
        for (x = 0; x < SizeIco; x++) {
          cr = Canvas->Get_Pixels(x, SizeIco - y - 1).cr;
          if (bMasque) {
            crRef1 = Canvas->Get_Pixels(x, 2 * SizeIco - y - 1).cr;
            if (crRef1 == RGB(255, 255, 255)) cr = 0;
          }
          dwBytesInRes[i] += PaletteVCL->WritePixelToStream(Stream, cr);
        }
      }

      // Ecriture du masque AND
      if (bMasque) {
        for (y = 0; y < SizeIco; y++) {
          Mask = 0x80;
          Octet = 0;
          k = 0;
          for (x = 0; x < SizeIco; x++) {
            cr = Canvas->Get_Pixels(x, 2 * SizeIco - y - 1).cr;
            if (cr != RGB(0, 0, 0)) Octet |= Mask;
            Mask >>= 1;
            if (++k == 8) {
              dw = (DWORD) Stream->Write(&Octet, 1);
              dwBytesInRes[i] += 1;
              Mask = 0x80;
              Octet = 0;
              k = 0;
            }
          }
          if (k != 0) {
            dw = (DWORD) Stream->Write(&Octet, sizeof(Octet));
            dwBytesInRes[i] += 1;
          }
          Octet = 0;
          for (k = SizeIco / 8; (k % 4) != 0; k++) {
            dw = (DWORD) Stream->Write(&Octet, sizeof(Octet));
            dwBytesInRes[i] += 1;
          }
        }
      }
      else {
        Octet = 0;
        for (j = 0; j < ((SizeIco - 1) / 32 + 1) * 4 * SizeIco; j++) {
          dw = (DWORD) Stream->Write(&Octet, sizeof(Octet));
          dwBytesInRes[i] += 1;
        }
      }
    }
		// Pour tronquer le fichier (dans le cas d'un fichier)
		// Sinon, lorsqu'on enlève une bitmap d'une icône existante, le fichier reste à
		// la même taille avec des données inutiles
		Stream->Size = Stream->Position;

    Stream->Seek((int) sizeof(HEADERCURICOFILE), soBeginning);
    PosCurIconFile = sizeof(HEADERCURICOFILE) + (int) ListBitmaps.size() * sizeof(HEADERCURICO);
    for (i = 0; i < (int) ListBitmaps.size() && i < 256; i++) {
      WidthIco = ListBitmaps[i]->Width;
      HeightIco = ListBitmaps[i]->Height;
      if (WidthIco < HeightIco) SizeIco = WidthIco;
      else SizeIco = HeightIco;
      HeaderIco.bWidth = SizeIco;
      HeaderIco.bHeight = SizeIco;
      HeaderIco.bColorCount = ListBitmaps[i]->PaletteVCL->NbColors;
			if (FileType == ciCUR) {
				HeaderIco.wXHotSpot = ListBitmaps[i]->XHotSpot;
				HeaderIco.wYHotSpot = ListBitmaps[i]->YHotSpot;
			}
			else {
				HeaderIco.wPlanes = 1;
				HeaderIco.wBitCount = ListBitmaps[i]->BitCount;
			}
      HeaderIco.dwBytesInRes = dwBytesInRes[i];
      HeaderIco.dwImageOffset = PosCurIconFile;
      dw = (DWORD) Stream->Write(&HeaderIco, sizeof(HEADERCURICO));
      PosCurIconFile += dwBytesInRes[i];
    }

		delete PaletteVCL;

  }

  return Ok? TRUE: FALSE;
}

//---------------------------------------------------------------------------
BOOL TCURICOFile::SaveToFile(const TCHAR *szFileName,
													std::vector<TBitmap *> &ListBitmaps,
													TCHAR *szMsgErreur) {
	TFileStream BitmapStream(szFileName, fmCreate | fmOpenRead | fmOpenWrite | fmShareDenyWrite);
	return SaveToStream(&BitmapStream, ListBitmaps, szMsgErreur);
}

//---------------------------------------------------------------------------

