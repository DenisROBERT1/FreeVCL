//-----------------------------------------------------------------------------
//! @file TPDFFile.cpp
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
//                            TPDFFile
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TPDFFile::TPDFFile(TComponent *AOwner): TComponent(AOwner) {
  FCanvas = new TCanvasPdf();

  ObjetPdfCatalog = new TObjetPdfCatalog(this);
  ObjetsPdf.push_back(ObjetPdfCatalog);

  ObjetPdfPages = new TObjetPdfPages(this);
  ObjetsPdf.push_back(ObjetPdfPages);
  ObjetPdfCatalog->Pages = ObjetPdfPages;

  ObjetPdfOutlines = new TObjetPdfOutlines(this);
  ObjetsPdf.push_back(ObjetPdfOutlines);
  ObjetPdfCatalog->Outlines = ObjetPdfOutlines;

  ObjetPdfProcSet = new TObjetPdfProcSet(this);
  ObjetsPdf.push_back(ObjetPdfProcSet);

  ObjetPdfContent = NULL;
  ObjetPdfPage = NULL;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TPDFFile::~TPDFFile(void) {
  int i;


	for (i = 0; i < (int) ObjetsPdf.size(); i++) {
		if (ObjetsPdf[i]->Owner == this) delete ObjetsPdf[i];
	}
  delete FCanvas;

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Canvas
//---------------------------------------------------------------------------

TCanvasPdf *TPDFFile::Get_Canvas(void) {
  return FCanvas;
}


//---------------------------------------------------------------------------
void TPDFFile::AddPage(int Width, int Height) {
  int i;


  if (ObjetPdfPage) {
		TFontPdf * FontPdf = dynamic_cast<TFontPdf *>((TCustomFont *) FCanvas->Font);
		if (FontPdf) {
			for (i = 0; i < 14; i++) {
				if (FontPdf->ObjetPdfFont[i]) {
					if (ObjetPdfPage->ObjetPdfFont[i] == NULL) {
						ObjetPdfPage->Set_ObjetPdfFont(i, FontPdf->ObjetPdfFont[i]);
					}
				}
			}
		}
	}

  if (ObjetPdfContent) {
    FCanvas->EndDraw();
    ObjetPdfContent->Stream =
        (CharString) ObjetPdfContent->Stream +
        (CharString) FCanvas->Stream;
    FCanvas->Clear();
  }

  ObjetPdfPage = new TObjetPdfPage(this);
  ObjetsPdf.push_back(ObjetPdfPage);
  ObjetPdfPage->Width = Width;
  ObjetPdfPage->Height = Height;
  ObjetPdfPage->ObjetPdfProcSet = ObjetPdfProcSet;
  ObjetPdfPages->AddPage(ObjetPdfPage);

  ObjetPdfContent = new TObjetPdfContent(this);
  ObjetsPdf.push_back(ObjetPdfContent);
  ObjetPdfPage->AddContent(ObjetPdfContent);

  FCanvas->SetPageSize(Width, Height);
}

//---------------------------------------------------------------------------
TObjetPdf * TPDFFile::CreatePdfObject(CharString csType) {

	if (csType == "Catalog") return new TObjetPdfCatalog(this);
	else if (csType == "Content") return new TObjetPdfContent(this);
	else if (csType == "Font") return new TObjetPdfFont(this);
	else if (csType == "Outlines") return new TObjetPdfOutlines(this);
	else if (csType == "Page") return new TObjetPdfPage(this);
	else if (csType == "Pages") return new TObjetPdfPages(this);
	else if (csType == "Procset") return new TObjetPdfProcSet(this);
	else return new TObjetPdfUnknown(this);

}

//---------------------------------------------------------------------------
bool FASTCALL TPDFFile::LoadFromFile(const AnsiString asFileName) {
  HANDLE hFile;
  DWORD dw;
  std::vector<unsigned int> Adresses;
  std::vector<TObjetPdf *> ObjetsPdfTmp;
  unsigned int Adresse, AdresseEnd;
	unsigned int FileSize;
	int NbObjects = 0;
	char c;
	int i;
	bool Ok = true;


  hFile = CreateFile(asFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		char szBuf[20];

		// Lecture entête
		memset(szBuf, 0, 9);
		ReadFile(hFile, szBuf, 9, &dw, NULL);
		if (memcmp(szBuf, "%PDF-1.0\n", 9)) Ok = false;  // Header non reconnu

		if (Ok) {
			FileSize = GetFileSize(hFile, NULL);
			if (FileSize < 27 /* sizeof("%PDF-1.0\nstartxref\n0\n%%EOF\n") */) Ok = false;  // Taille incorrecte
		}

		if (Ok) {
			// Lecture "\n%%EOF\n"
			memset(szBuf, 0, 7);
			SetFilePointer(hFile, FileSize - 7, NULL, FILE_BEGIN);
			ReadFile(hFile, szBuf, 7, &dw, NULL);
			if (memcmp(szBuf, "\n%%EOF\n", 7)) Ok = false;  // Marque de fin non trouvée
		}

		if (Ok) {
			// Lecture adresse xref (on lit le fichier à partir de la fin jusqu'au '\n')

			AdresseEnd = FileSize - 8;
			szBuf[19] = '\0';
			for (i = 0; i < 19; i++) {
				SetFilePointer(hFile, AdresseEnd, NULL, FILE_BEGIN);
				ReadFile(hFile, &c, 1, &dw, NULL);
				if (c == '\n') {
					Adresse = (unsigned int) atoi(&szBuf[19 - i]);
					AdresseEnd = Adresse;
					break;
				}
				szBuf[18 - i] = c;
				AdresseEnd--;
			}
			if (i >= 19) Ok = false;  // '\n' non trouvé
			else if (Adresse >= FileSize - 5) Ok = false;  // Adresse incohérente

		}

		if (Ok) {
			// Lecture "xref 0 "
			memset(szBuf, 0, 7);
			SetFilePointer(hFile, Adresse, NULL, FILE_BEGIN);
			ReadFile(hFile, szBuf, 7, &dw, NULL);
			if (memcmp(szBuf, "xref 0 ", 7)) Ok = false;
		}

		if (Ok) {
			// Lecture nombre d'objets
			for (i = 0; i < 19; i++) {
				ReadFile(hFile, &c, 1, &dw, NULL);
				if (c == '\n') {
					szBuf[i] = '\0';
					NbObjects = atoi(szBuf);
					break;
				}
				szBuf[i] = c;
			}
		}

		if (Ok) {
			// Lecture des adresses des objets
			for (i = 0; i < NbObjects; i++) {
				ReadFile(hFile, szBuf, 19, &dw, NULL);
				szBuf[10] = '\0';
				Adresse = atoi(szBuf);
				Adresses.push_back(Adresse);
			}
			Adresses.push_back(AdresseEnd);
		}

		if (Ok) {
			TObjetPdf *ObjetPdf;
			char *szDescription;
			unsigned int ObjectSize;

			// Lecture des objets
			for (i = 0; i < (int) Adresses.size() - 1; i++) {

				ObjectSize = Adresses[i + 1] - Adresses[i];
				szDescription = new char[ObjectSize + 1];
				memset(szDescription, 0, ObjectSize + 1);

				// Lecture du contenu de l'objet
				SetFilePointer(hFile, Adresses[i], NULL, FILE_BEGIN);
				ReadFile(hFile, szDescription, ObjectSize, &dw, NULL);

				// Mémorisation de l'objet
				ObjetPdf = new TObjetPdfUnknown(this);
				ObjetPdf->SetDescription(szDescription);
				ObjetsPdfTmp.push_back(ObjetPdf);

				delete[] szDescription;

			}

		}

		if (Ok) {
			TObjetPdf *ObjetPdf;
			CharString csType, csIndex;
			int NumObj, Index;

			// Transfert des objets temporaires
			for (i = 0; i < (int) ObjetsPdfTmp.size(); i++) {
				ObjetPdf = ObjetsPdfTmp[i];
				csType = ObjetPdf->ExtractValue(ObjetPdf->GetDescription(), "/Type");
				if (csType == "/Catalog") {
				}
				else if (csType == "/Outlines") {
				}
				else if (csType == "/Pages") {
				}
				else if (csType == "/Page") {
					ObjetPdfPage = new TObjetPdfPage(this);
				  ObjetsPdf.push_back(ObjetPdfPage);
					ObjetPdfPage->SetDescription(ObjetPdf->GetDescription());
				  ObjetPdfPage->ObjetPdfProcSet = ObjetPdfProcSet;
					ObjetPdfPages->AddPage(ObjetPdfPage);
					// Contenu de la page
					ObjetPdfContent = new TObjetPdfContent(this);
				  ObjetsPdf.push_back(ObjetPdfContent);
					csIndex = ObjetPdf->ExtractValue(ObjetPdf->GetDescription(), "/Contents");
					NumObj = csIndex.ToIntDef(0);
					for (Index = 0; Index < (int) ObjetsPdfTmp.size(); Index++) {
						if (ObjetsPdfTmp[Index]->NumObj == NumObj) {
							ObjetPdfContent->SetDescription(ObjetsPdfTmp[Index]->GetDescription());
							break;
						}
					}
					ObjetPdfPage->AddContent(ObjetPdfContent);
				}
				else if (csType == "/Font") {
					ObjetPdfFont = new TObjetPdfFont(this);
				  ObjetsPdf.push_back(ObjetPdfFont);
					ObjetPdfFont->SetDescription(ObjetPdf->GetDescription());
					ObjetPdfPage->Set_ObjetPdfFont(ObjetPdfFont->TypeStandard - 1, ObjetPdfFont);
				}
				else if (ObjetPdf->GetDescription() == "[/PDF]") {
				}

			}

			// Nettoyage
			for (i = 0; i < (int) ObjetsPdfTmp.size(); i++) {
				delete ObjetsPdfTmp[i];
			}

		}

		CloseHandle(hFile);

  }
  else {
    // Message d'erreur ouverture asFileName
		FLastError = GetLastError();
		Ok = false;
  }

	return Ok;
}

//---------------------------------------------------------------------------
bool FASTCALL TPDFFile::SaveToFile(const AnsiString asFileName) {
  HANDLE hFile;
  DWORD dw;
  int NumObj;
  int i;
  unsigned int Adresse;
  CharString csBuf;
  std::vector<unsigned int> Adresses;
	bool Ok;


  if (ObjetPdfContent) {
    FCanvas->EndDraw();
    ObjetPdfContent->Stream =
        (CharString) ObjetPdfContent->Stream +
        (CharString) FCanvas->Stream;
    FCanvas->Clear();
  }

  TFontPdf * FontPdf = dynamic_cast<TFontPdf *>((TCustomFont *) FCanvas->Font);
  if (FontPdf) {
		for (i = 0; i < 14; i++) {
			if (FontPdf->ObjetPdfFont[i]) {
				TObjetPdfFont *ObjetPdfFont = FontPdf->ObjetPdfFont[i];
				ObjetsPdf.push_back(ObjetPdfFont);
				if (ObjetPdfPage && ObjetPdfPage->ObjetPdfFont[i] == NULL) {
					ObjetPdfPage->Set_ObjetPdfFont(i, ObjetPdfFont);
				}
			}
		}
  }

  hFile = CreateFile(asFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

    WriteFile(hFile, "%PDF-1.0\n", 9, &dw, NULL);

    for (i = 0; i < (int) ObjetsPdf.size(); i++) {
      ObjetsPdf[i]->NumObj = i + 1;
    }

    for (i = 0; i < (int) ObjetsPdf.size(); i++) {
      Adresse = SetFilePointer(hFile, 0, 0, FILE_CURRENT);
      Adresses.push_back(Adresse);
      NumObj = ObjetsPdf[i]->NumObj;
      csBuf.sprintf("%i 0 obj\n", NumObj);
      WriteFile(hFile, (LPCSTR) csBuf, csBuf.Length(), &dw, NULL);

      csBuf = ObjetsPdf[i]->GetDescription();
      WriteFile(hFile, (LPCSTR) csBuf, csBuf.Length(), &dw, NULL);

      WriteFile(hFile, "endobj\n", 7, &dw, NULL);
    }

    Adresse = SetFilePointer(hFile, 0, 0, FILE_CURRENT);
    csBuf.sprintf("xref 0 %i\n", (int) Adresses.size());
    WriteFile(hFile, (LPCSTR) csBuf, csBuf.Length(), &dw, NULL);
    for (i = 0; i < (int) Adresses.size(); i++) {
      csBuf.sprintf("%010i 00000 n\n", Adresses[i]);
      WriteFile(hFile, (LPCSTR) csBuf, csBuf.Length(), &dw, NULL);
    }

    NumObj = ObjetPdfCatalog->NumObj;
    csBuf.sprintf("trailer\n<<\n/Size %i\n/Root %i 0 R\n>>\n",
                  (int) Adresses.size(), NumObj);
    WriteFile(hFile, (LPCSTR) csBuf, csBuf.Length(), &dw, NULL);

    csBuf.sprintf("startxref\n%i\n%%%%EOF\n", (int) Adresse);
    WriteFile(hFile, (LPCSTR) csBuf, csBuf.Length(), &dw, NULL);

    CloseHandle(hFile);

		Ok = true;
  }
  else {
    // Message d'erreur ouverture asFileName
		FLastError = GetLastError();
		Ok = false;
  }

	return Ok;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPDFFile::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPDFFile::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TPDFFile::GetDefaultProperty(AnsiString asProperty) {
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TPDFFile::GetProperty(AnsiString asProperty) {
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPDFFile::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

