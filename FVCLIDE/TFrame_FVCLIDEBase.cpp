//-----------------------------------------------------------------------------
//! @file TFrame_FVCLIDEBase.cpp
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


//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------
#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------
#include "TFrame_FVCLIDEBase.h"


//---------------------------------------------------------------------------
TFrame_FVCLIDEBase::TFrame_FVCLIDEBase(TWinControl *AOwner)
  : TFrame(AOwner) {

  // Initialisations
	FChanged = false;
}

//---------------------------------------------------------------------------
TFrame_FVCLIDEBase::~TFrame_FVCLIDEBase(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FileType
//---------------------------------------------------------------------------

TFVCLIDEFileType TFrame_FVCLIDEBase::Get_FileType(void) {
  return FFileType;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DisplayedFileName
//---------------------------------------------------------------------------

AnsiString TFrame_FVCLIDEBase::Get_DisplayedFileName(void) {
	if (FFileName.IsEmpty()) return _TT("Nouveau fichier");
  return ExtractFileName(FFileName);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FileName
//---------------------------------------------------------------------------

AnsiString TFrame_FVCLIDEBase::Get_FileName(void) {
  return FFileName;
}

bool TFrame_FVCLIDEBase::Set_FileName(AnsiString NewFileName) {
  if (FFileName != NewFileName) {
    FFileName = NewFileName;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Changed
//---------------------------------------------------------------------------

bool TFrame_FVCLIDEBase::Get_Changed(void) {
  return FChanged;
}

bool TFrame_FVCLIDEBase::Set_Changed(bool NewChanged) {
  if (FChanged != NewChanged) {
    FChanged = NewChanged;
		if (FChanged) OnChange(this);
		OnFlagChanged(this, FChanged);
  }
  return true;
}

//---------------------------------------------------------------------------
// Ouverture d'un fichier
//---------------------------------------------------------------------------
bool TFrame_FVCLIDEBase::Open(void) {
	bool bRet;

	bRet = OpenFile(FFileName);
	if (bRet) {
		// Pas de MemoDateFile (il a déjà été fait dans le open)
		Changed = false;
	}

  return bRet;
}

//---------------------------------------------------------------------------
// Enregistrement d'un fichier
//---------------------------------------------------------------------------
bool TFrame_FVCLIDEBase::Save(void) {
	bool bRet;

	bRet = SaveToFile(FFileName);
	if (bRet) {
		// Pas de MemoDateFile (il a déjà été fait dans le SaveToFile)
		Changed = false;
	}

  return bRet;
}

//---------------------------------------------------------------------------
int TFrame_FVCLIDEBase::TestExternalChange(void) {
	WIN32_FILE_ATTRIBUTE_DATA FileAttributeData;

	if (FFileName.IsEmpty()) return 3;

	if (GetFileAttributesEx(FFileName, GetFileExInfoStandard, &FileAttributeData) == FALSE) {
		if (ftLastWriteTime.dwHighDateTime == 0 && ftLastWriteTime.dwLowDateTime == 0) return 0;
		return 2;
	}

	if (FileAttributeData.ftLastWriteTime.dwHighDateTime != ftLastWriteTime.dwHighDateTime ||
			FileAttributeData.ftLastWriteTime.dwLowDateTime != ftLastWriteTime.dwLowDateTime) return 1;

	return 0;
}

//---------------------------------------------------------------------------
bool TFrame_FVCLIDEBase::MemoDateFile(void) {
	WIN32_FILE_ATTRIBUTE_DATA FileAttributeData;

	if (FFileName.IsEmpty()) return false;

	if (GetFileAttributesEx(FFileName, GetFileExInfoStandard, &FileAttributeData) == TRUE) {
		ftLastWriteTime = FileAttributeData.ftLastWriteTime;
	}
	else {
		ftLastWriteTime.dwHighDateTime = 0;
		ftLastWriteTime.dwLowDateTime = 0;
	}

	return true;
}

//---------------------------------------------------------------------------
