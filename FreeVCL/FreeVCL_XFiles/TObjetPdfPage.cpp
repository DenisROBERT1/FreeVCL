//-----------------------------------------------------------------------------
//! @file TObjetPdfPage.cpp
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
//                            TObjetPdfPage
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TObjetPdfPage::TObjetPdfPage(void *Owner): TObjetPdf(Owner) {
  int i;


  for (i = 0; i < 14; i++) FObjetPdfFont[i] = NULL;
	FWidth = 0;
	FHeight = 0;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TObjetPdfPage::~TObjetPdfPage(void) {
	// TObjetPdfPage n'est pas propriétaire des FObjetPdfFont
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Width
//---------------------------------------------------------------------------

int TObjetPdfPage::Get_Width(void) {
  return FWidth;
}

bool TObjetPdfPage::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Height
//---------------------------------------------------------------------------

int TObjetPdfPage::Get_Height(void) {
  return FHeight;
}

bool TObjetPdfPage::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ObjetPdfParent
//---------------------------------------------------------------------------

TObjetPdfPages * TObjetPdfPage::Get_ObjetPdfParent(void) {
  return FObjetPdfParent;
}

bool TObjetPdfPage::Set_ObjetPdfParent(TObjetPdfPages *NewObjetPdfParent) {
  FObjetPdfParent = NewObjetPdfParent;

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ObjetPdfProcSet
//---------------------------------------------------------------------------

TObjetPdfProcSet * TObjetPdfPage::Get_ObjetPdfProcSet(void) {
  return FObjetPdfProcSet;
}

bool TObjetPdfPage::Set_ObjetPdfProcSet(TObjetPdfProcSet *NewObjetPdfProcSet) {
  FObjetPdfProcSet = NewObjetPdfProcSet;

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ObjetPdfFont
//---------------------------------------------------------------------------

TObjetPdfFont * TObjetPdfPage::Get_ObjetPdfFont(int i) {
  return FObjetPdfFont[i];
}

bool TObjetPdfPage::Set_ObjetPdfFont(int i, TObjetPdfFont *NewObjetPdfFont) {
	FObjetPdfFont[i] = NewObjetPdfFont;

  return true;
}

//---------------------------------------------------------------------------
// Ajout d'un contenu
//---------------------------------------------------------------------------

void TObjetPdfPage::AddContent(TObjetPdfContent *ObjetPdfContent) {
  Contents.push_back(ObjetPdfContent);
}


//---------------------------------------------------------------------------
// Chaîne de description
//---------------------------------------------------------------------------

CharString TObjetPdfPage::GetDescription(void) {
  CharString csDescription, csBuf;
  int NumObj;
  int i;
  bool bFonts;


  NumObj = FObjetPdfParent->NumObj;
  csDescription.sprintf("<<\n/Type /Page\n/Parent %i 0 R\n", NumObj);

  csBuf.sprintf("/MediaBox [0 0 %i %i]\n", FWidth, FHeight);
  csDescription += csBuf;

  if (Contents.size() > 0) {
    csDescription += "/Contents ";
    if (Contents.size() > 1) csDescription += "[";
    for (i = 0; i < (int) Contents.size(); i++) {
      if (i) csDescription += " ";
      NumObj = Contents[i]->NumObj;
      csBuf.sprintf("%i 0 R", NumObj);
      csDescription += csBuf;
    }
    if (Contents.size() > 1) csDescription += "]";
    csDescription += "\n";
  }

  csDescription += "/Resources << ";

  bFonts = false;
  for (i = 0; i < 14; i++) {
    if (FObjetPdfFont[i]) {
      bFonts = true;
      break;
    }
  }

  if (bFonts) {
    csDescription += "/Font << ";
    for (i = 0; i < 14; i++) {
      if (FObjetPdfFont[i]) {
        NumObj = FObjetPdfFont[i]->NumObj;
        csBuf.sprintf("/F%i %i 0 R ", i + 1, NumObj);
        csDescription += csBuf;
      }
    }
    csDescription += ">> ";
  }

  csBuf.sprintf("/ProcSet %i 0 R >>\n",
                (int) FObjetPdfProcSet->NumObj);
  csDescription += csBuf;

  csDescription += ">>\n";

  return csDescription;
}

//---------------------------------------------------------------------------
bool TObjetPdfPage::SetDescription(CharString csDescription) {
	CharString csValue;
	int Pos = 0;
	CharString csWord;

	csWord = ExtractFirstWord(csDescription, " \t\n", &Pos);
	while (!csWord.IsEmpty()) {
		if (csWord == "/MediaBox") break;
		csWord = ExtractFirstWord(csDescription, " \t\n", &Pos);
	}
	if (csWord.IsEmpty()) return false;

	csValue = ExtractFirstWord(csDescription, "[] \t\n", &Pos);
	csValue = ExtractFirstWord(csDescription, "[] \t\n", &Pos);
	csValue = ExtractFirstWord(csDescription, "[] \t\n", &Pos);
	FWidth = csValue.ToIntDef(0);
	csValue = ExtractFirstWord(csDescription, "[] \t\n", &Pos);
	FHeight = csValue.ToIntDef(0);

	return true;
}

//---------------------------------------------------------------------------

