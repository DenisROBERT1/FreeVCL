//-----------------------------------------------------------------------------
//! @file TGIFFile.cpp
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
// TGIFFile
//---------------------------------------------------------------------------

TGIFFile::TGIFFile(void) {
	cr = NULL;
	PixelBits = NULL;
}

TGIFFile::~TGIFFile(void) {
	delete[] cr;
	delete[] PixelBits;
}

//---------------------------------------------------------------------------
BOOL TGIFFile::LoadFromStream(TStream *Stream,
															std::vector<TBitmap *> &ListBitmaps,
															TCHAR *szMsgErreur) {
	return false;
}

//---------------------------------------------------------------------------
BOOL TGIFFile::LoadFromFile(const TCHAR *szFileName,
														std::vector<TBitmap *> &ListBitmaps,
														TCHAR *szMsgErreur) {
  HANDLE hFile;
  DWORD dw;
  HEADERGIFFILE HeaderGifFile;

  BOOL Ok;
	int i;


  hFile = CreateFile(szFileName, GENERIC_READ, 0,
                     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  Ok = (hFile != INVALID_HANDLE_VALUE);
  if (!Ok) wsprintf(szMsgErreur, _T("Impossible d'ouvrir le fichier\n%s"), szFileName);

  if (Ok) {

		// Header
    ReadFile(hFile, &HeaderGifFile, sizeof(HEADERGIFFILE), &dw, NULL);
    if (dw != sizeof(HEADERGIFFILE)) {
      Ok = false;
      wsprintf(szMsgErreur, _T("Impossible de lire l'en-tête HEADERGIFFILE"));
    }

	}

  if (Ok) {

    // Colors
    if ((Header.Flags & 0x80) != 0) {
      NbColors = 2 << (Header.Flags & 0x07);
			delete[] cr;
      cr = new COLORREF[NbColors];
      for (i = 0; i < NbColors; i++) {
				cr[i] = 0;
			  ReadFile(hFile, &cr[i], 3, &dw, NULL);
      }
    }

	}

  if (Ok) {
		BYTE Separator;
		BYTE BlockType;
		BYTE LZWMinSize;

		// Blocs
    do {

			ReadFile(hFile, &Separator, 1, &dw, NULL);
			if (Separator == 0x2C) {
        CharString Lzw;
				ReadFile(hFile, &ImageBlock, sizeof(GIFIMAGEBLOCK), &dw, NULL);
        // Colors
        if ((ImageBlock.Flags & 0x80) != 0) {
					COLORREF cr;
          NbColors = 2 << (ImageBlock.Flags & 0x07);
					for (i = 0; i < NbColors; i++) {
						ReadFile(hFile, &cr, 3, &dw, NULL);
					}
        }

				ReadFile(hFile, &LZWMinSize, 1, &dw, NULL);
        Lzw = ReadBlock(hFile);
        if (NbBitmap++ == 0) {
					delete[] PixelBits;
					int LenPixelBits;
					SizePixelBits = (ImageBlock.Width - ImageBlock.Left) * (ImageBlock.Height - ImageBlock.Top);
					PixelBits = new BYTE[SizePixelBits];
          LZWDecode((const BYTE *) (LPCSTR) Lzw, Lzw.Length(), LZWMinSize + 1, PixelBits, SizePixelBits, &LenPixelBits);
        }
			}
			else if (Separator == 0x21) {
				ReadFile(hFile, &BlockType, 1, &dw, NULL);
        if (BlockType == 0xF9) {
          GIFEXTENSIONBLOCK ExtensionBlock;
					ReadFile(hFile, &ExtensionBlock, sizeof(GIFEXTENSIONBLOCK), &dw, NULL);
				}
        else if (BlockType == 0xFF) {
          GIFAPPLICATIONBLOCK ApplicationBlock;
          CharString csAppData;
			    ReadFile(hFile, &ApplicationBlock, sizeof(GIFAPPLICATIONBLOCK), &dw, NULL);
					csAppData = ReadBlock(hFile);
				}
			}

    } while (Separator != 0x3B);

  }

  if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

  return Ok;
}

//---------------------------------------------------------------------------
BOOL TGIFFile::SaveToStream(TStream *Stream,
														std::vector<TBitmap *> &ListBitmaps,
														TCHAR *szMsgErreur) {
	return false;
}


//---------------------------------------------------------------------------
BOOL TGIFFile::SaveToFile(const TCHAR *szFileName,
													std::vector<TBitmap *> &ListBitmaps,
													TCHAR *szMsgErreur) {
  HANDLE hFile;
  DWORD dw;
  HEADERGIFFILE HeaderGifFile;
	GIFEXTENSIONBLOCK ExtensionBlock;
	BYTE Separator;
	BYTE BlockType;
	BYTE LZWMinSize;
	BYTE *Lzw;
	int LenLzw;

  BOOL Ok;
	int i;


  hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0,
                     NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  Ok = (hFile != INVALID_HANDLE_VALUE);
  if (!Ok) wsprintf(szMsgErreur, _T("Impossible de créer le fichier\n%s"), szFileName);

  if (Ok) {

		// Header
    WriteFile(hFile, &HeaderGifFile, sizeof(HEADERGIFFILE), &dw, NULL);
    if (dw != sizeof(HEADERGIFFILE)) {
      Ok = false;
      wsprintf(szMsgErreur, _TT("Impossible d'enregistrer l'en-tête HEADERGIFFILE"));
    }

	}

  if (Ok) {

    // Colors
    if ((Header.Flags & 0x01) != 0) {
      for (i = 0; i < NbColors; i++) {
		    WriteFile(hFile, &cr[i], 3, &dw, NULL);
      }
    }

	}

  if (Ok) {

    // Blocs

    Separator = 0x21;
    WriteFile(hFile, &Separator, 1, &dw, NULL);
    BlockType = 0xF9;
    WriteFile(hFile, &BlockType, 1, &dw, NULL);
    ExtensionBlock.BlockSize = 0x04;
    ExtensionBlock.PackedFields = 0x01;
    ExtensionBlock.DelayTime = 0;
    ExtensionBlock.TranspColorIndex = 16;
    ExtensionBlock.BlockTerminator = 0;
    WriteFile(hFile, &ExtensionBlock, sizeof(GIFEXTENSIONBLOCK), &dw, NULL);

    Separator = 0x2C;
    WriteFile(hFile, &Separator, 1, &dw, NULL);
    ImageBlock.Left = 0;
    ImageBlock.Top = 0;
    ImageBlock.Width = Header.Width;
    ImageBlock.Height = Header.Height;
    ImageBlock.Flags = 0x00;
    WriteFile(hFile, &ImageBlock, sizeof(GIFIMAGEBLOCK), &dw, NULL);
    LZWMinSize = 8;
    WriteFile(hFile, &LZWMinSize, 1, &dw, NULL);

    Lzw = new BYTE[SizePixelBits];
		LZWEncode(PixelBits, SizePixelBits, LZWMinSize + 1, Lzw, SizePixelBits, &LenLzw);
    WriteFile(hFile, Lzw, LenLzw, &dw, NULL);
		delete[] Lzw;

    Separator = 0x3B;
    WriteFile(hFile, &Separator, 1, &dw, NULL);

  }

  if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

  return true;
}

bool TGIFFile::LZWEncode(const BYTE *Bits, int LenBits, int NbBits, BYTE *CompBits, int MaxLenCompBits, int *LenCompBits) {
	int i, j;
	CharString Dictionnaire[4096];
	int DicLength;
	int IndexDic;
	int Code_w;
	BYTE b;
	CharString w, wc;
	BYTE ByteWrite;
	int NbBitsWrite;
	int CodeInit, CodeEnd;
	int NbBitsInit;

	CodeInit = 1 << (NbBits - 1);
	CodeEnd = CodeInit + 1;
	NbBitsInit = NbBits;

	// Initialisation du dictionnaire
	DicLength = CodeEnd + 1;
	for (i = 0; i < DicLength; i++) {
		Dictionnaire[i] = (char) i;
	}

	/*
	w = Nul;
	tant que (lecture d'un caractère b) faire
			si (wc existe dans le dictionnaire) alors
					w = wc;
			sinon
					ajouter wc au dictionnaire;
					écrire le code de w;
					w = b;
			fin si
	fin tant que
	écrire le code de w;
	*/

	w = "";
	*LenCompBits = 0;
	Code_w = -1;
	NbBitsWrite = 0;
	ByteWrite = '\0';
	WriteCode(CompBits, MaxLenCompBits, LenCompBits, CodeInit, &ByteWrite, &NbBitsWrite, NbBits);
	for (i = 0; i < LenBits; i++) {
		b = Bits[i];
		wc = w + (char) b;
		IndexDic = -1;
		for (j = 0; j < DicLength; j++) {
			if (Dictionnaire[j] == wc) {
				IndexDic = j;
				break;
			}
		}

		if (IndexDic != -1) {
			w = wc;
			Code_w = IndexDic;
		}
		else {
			WriteCode(CompBits, MaxLenCompBits, LenCompBits, Code_w, &ByteWrite, &NbBitsWrite, NbBits);
			if (DicLength == (1 << NbBits)) NbBits++;
			Dictionnaire[DicLength++] = wc;
			w = (char) b;
			Code_w = b;
			if (DicLength >= 4096) {
				WriteCode(CompBits, MaxLenCompBits, LenCompBits, CodeInit, &ByteWrite, &NbBitsWrite, NbBits);
				NbBits = NbBitsInit;
				DicLength = (1 << (NbBits - 1)) + 2;
			}
		}
	}
	if (Code_w != -1) WriteCode(CompBits, MaxLenCompBits, LenCompBits, Code_w, &ByteWrite, &NbBitsWrite, NbBits);
	WriteCode(CompBits, MaxLenCompBits, LenCompBits, CodeEnd, &ByteWrite, &NbBitsWrite, NbBits);

	// Dernier octet
	if (NbBitsWrite != 0) {
		NbBits = 8;
		WriteCode(CompBits, MaxLenCompBits, LenCompBits, 0, &ByteWrite, &NbBitsWrite, NbBits);
	}


	return true;

}

//---------------------------------------------------------------------------
bool TGIFFile::LZWDecode(const BYTE *CompBits, int LenCompBits, int NbBits, BYTE *Bits, int MaxLenBits, int *LenBits) {
	int i;
	CharString Dictionnaire[4096];
	int DicLength;
	int IndexDic;
	int Code_c;
	CharString w, wc, entree;
	int NbBitsRead;
	int CodeInit, CodeEnd;
	int NbBitsInit;
	bool bFirst;

	CodeInit = 1 << (NbBits - 1);
	CodeEnd = CodeInit + 1;
	NbBitsInit = NbBits;

	// Initialisation du dictionnaire
	DicLength = CodeEnd + 1;
	for (i = 0; i < DicLength; i++) {
		Dictionnaire[i] = (char) i;
	}

	/*
	lecture d'un caractère b;
	écrire b; // ajout suite à un oubli
	w = b;
	tant que (lecture d'un caractère b) faire
			si (b > 255 && l'index b existe dans le dictionnaire) faire
					entrée = l'entrée du dictionnaire de b;
			sinon si (b > 255 && l'index b n'existe pas dans le dictionnaire) faire
					entrée = w + w[0];
			sinon
					entrée = b;
			fin si;
			écrire entrée;
			ajouter w+entrée[0] au dictionnaire;
			w = entrée;
	fin tant que;
	*/

	IndexDic = 0;
	NbBitsRead = 0;
	*LenBits = 0;
	bFirst = true;
	if (!ReadCode(CompBits, LenCompBits, &IndexDic, &Code_c, &NbBitsRead, NbBits)) return false;
	if (Code_c != CodeInit) return false;

	while (ReadCode(CompBits, LenCompBits, &IndexDic, &Code_c, &NbBitsRead, NbBits)) {
		if (Code_c == CodeInit) {
			NbBits = NbBitsInit;
			DicLength = (1 << (NbBits - 1)) + 2;
			bFirst = true;
			continue;
		}
		if (Code_c == CodeEnd) break;
		if (bFirst) {
			if (*LenBits >= MaxLenBits) return false;
			w = Bits[(*LenBits)++] = (char) Code_c;
			bFirst = false;
			continue;
		}
		if (Code_c > CodeEnd) {
			if (Code_c < DicLength) {
				entree = Dictionnaire[Code_c];
			}
			else {
				entree = w + w[1];
			}
		}
		else {
			entree = (char) Code_c;
		}
		if (*LenBits + (int) entree.Length() > MaxLenBits) return false;
		memcpy(&Bits[*LenBits], entree, entree.Length());
		(*LenBits) += entree.Length();
		wc = w + entree[1];
		w = entree;
		Dictionnaire[DicLength++] = wc;
		if (DicLength == (1 << NbBits)) NbBits++;
	}

	return true;
}

//---------------------------------------------------------------------------
bool TGIFFile::WriteCode(BYTE *Buf, int MaxLenBuf, int *LenBuf, int Code, BYTE *ByteWrite, int *NbBitsWrite, int NbBits) {
	int NbBitsToWrite;

	Code = (Code << (*NbBitsWrite)) + (*ByteWrite);
	NbBitsToWrite = NbBits + (*NbBitsWrite);

	while (NbBitsToWrite >= 8) {
		if ((*LenBuf) >= MaxLenBuf) return false;
		Buf[(*LenBuf)++] = (char) (Code & 0xFF);
		Code >>= 8;
		NbBitsToWrite -= 8;
	}

	(*ByteWrite) = (char) Code;
	(*NbBitsWrite) = NbBitsToWrite;

	return true;
}

//---------------------------------------------------------------------------
bool TGIFFile::ReadCode(const BYTE *Buf, int MaxLenBuf, int *LenBuf, int *Code, int *NbBitsRead, int NbBits) {
	int NbBitsToRead;
	int NbBitsDecal;
	BYTE b;

	NbBitsToRead = NbBits;
	NbBitsDecal = 0;
	*Code = 0;

	if (*LenBuf >= MaxLenBuf) {
		Code = 0;
		return false;
	}

	while (NbBitsToRead != 0) {

		if (*LenBuf >= MaxLenBuf) {
			Code = 0;
			return false;
		}

		if (*LenBuf < MaxLenBuf) b = Buf[*LenBuf];
		else b = 0;

		if (NbBitsToRead + (*NbBitsRead) < 8) {
			b &= (BYTE) (0xFF >> (8 - NbBitsToRead - (*NbBitsRead)));
			(*Code) += ((b >> (*NbBitsRead)) << NbBitsDecal);
			(*NbBitsRead) += NbBitsToRead;
			NbBitsToRead = 0;
		}
		else {
			(*LenBuf)++;
			(*Code) += ((b >> (*NbBitsRead)) << NbBitsDecal);
			NbBitsToRead -= 8 - (*NbBitsRead);
			NbBitsDecal += 8 - (*NbBitsRead);
			(*NbBitsRead) = 0;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
CharString TGIFFile::ReadBlock(HANDLE hFile) {
	DWORD dw;
  CharString csRet;
  BYTE BlockSize;
	BYTE b;
  int i;


	ReadFile(hFile, &BlockSize, 1, &dw, NULL);
  while (BlockSize != 0) {
    for (i = 0; i < BlockSize; i++) {
			ReadFile(hFile, &b, 1, &dw, NULL);
      csRet += b;
    }
		ReadFile(hFile, &BlockSize, 1, &dw, NULL);
  }

  return csRet;
}

//---------------------------------------------------------------------------
bool TGIFFile::WriteBlock(HANDLE hFile, CharString str) {
	DWORD dw;
  BYTE BlockSize;
  int BytesToWrite;
  int LenStr;
  int i;


  LenStr = str.Length();
  for (i = 0; i < LenStr; i += 255) {
    BytesToWrite = LenStr - i;
    if (BytesToWrite > 255) BytesToWrite = 255;
		BlockSize = (BYTE) BytesToWrite;
		WriteFile(hFile, &BlockSize, 1, &dw, NULL);
		WriteFile(hFile, &str.c_str()[i], BytesToWrite, &dw, NULL);
  }
  BlockSize = 0;
	WriteFile(hFile, &BlockSize, 1, &dw, NULL);

  return true;
}

//---------------------------------------------------------------------------
COLORREF TGIFFile::GetPixel(int x, int y) {

  if (x >= Header.Width || y >= Header.Height) {
    return RGB(0, 0, 0);
  }
  else {
    int crIndex = 0;
    int Index = y * Header.Width + x;
    if (Index < SizePixelBits) crIndex = PixelBits[Index];
    if (crIndex >= NbColors) crIndex = 0;
    return cr[crIndex];
  }
}

//---------------------------------------------------------------------------

