//-----------------------------------------------------------------------------
//! @file TFileList.cpp
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
// TFileList
//---------------------------------------------------------------------------

TFileList::TFileList(TComponent* AOwner): TComponent(AOwner) {
	// Initialisations
	Mask = new TMask(_T("*.*"));
	FRecursive = true;
	FTypes.Clear() << tlFiles;
}

//---------------------------------------------------------------------------
TFileList::~TFileList(void) {
	delete Mask;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Filter
//---------------------------------------------------------------------------

AnsiString TFileList::Get_Filter() {
	return FFilter;
}

bool TFileList::Set_Filter(AnsiString NewFilter) {
	if (FFilter != NewFilter) {
		FFilter = NewFilter;
		delete Mask;
		Mask = new TMask(FFilter);
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Recursive
//---------------------------------------------------------------------------

bool TFileList::Get_Recursive() {
	return FRecursive;
}

bool TFileList::Set_Recursive(bool NewRecursive) {
	if (FRecursive != NewRecursive) {
		FRecursive = NewRecursive;
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Types
//---------------------------------------------------------------------------

TTypeLists TFileList::Get_Types() {
	return FTypes;
}

bool TFileList::Set_Types(TTypeLists NewTypes) {
	if (FTypes != NewTypes) {
		FTypes = NewTypes;
	}
	return true;
}


//---------------------------------------------------------------------------
int TFileList::GetFileList(AnsiString Directory, bool FullName, TStrings *FileList) {
	HANDLE hFindFile;
	WIN32_FIND_DATA FindFileData;
	AnsiString asFileName;
	AnsiString asPath;


	bAbort = false;

	OnDirectory(this, Directory);

	asPath = Directory + _T("\\*.*");
	hFindFile = FindFirstFile(asPath, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE) {
		FLastError = GetLastError();
		return 0;
	}

	do {
		asFileName = FindFileData.cFileName;
		if (asFileName != _T(".") && asFileName != _T("..")) {
			bool bIsDirectory = ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
			if (bIsDirectory) {
				if (FRecursive) {
					// Traitement directory
					GetFileList(Directory + DIR_DELIMITER + asFileName, FullName, FileList);
				}
			}
			if (bIsDirectory && FTypes.Contains(tlDirectories) ||
					!bIsDirectory && FTypes.Contains(tlFiles)) {
				AnsiString asFullFileName;
				if (FullName) {
					asFullFileName = Directory + DIR_DELIMITER + asFileName;
				}
				if (Mask->Matches(FindFileData.cFileName)) {
					// Traitement fichier asFileName
					if (FullName) {
						if (FileList) FileList->Add(asFullFileName);
						OnFileFound(this, asFullFileName);
					}
					else {
						if (FileList) FileList->Add(asFileName);
						OnFileFound(this, asFileName);
					}
				}
				else {
					// Non traitement
					if (FullName) {
						OnFileFiltered(this, asFullFileName);
					}
					else {
						OnFileFiltered(this, asFileName);
					}
				}
			}
		}

	} while (!bAbort && FindNextFile(hFindFile, &FindFileData));

	FindClose(hFindFile);

	return true;

}


//---------------------------------------------------------------------------
// Interruption du traitement de fichiers
//---------------------------------------------------------------------------

void TFileList::Abort(void) {
	bAbort = true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TFileList::GetListProperties(TStrings *ListProperties) {

	TComponent::GetListProperties(ListProperties);

	ListProperties->Add(_T("Filter"));
	ListProperties->Add(_T("Recursive"));
	ListProperties->Add(_T("Type"));
	ListProperties->Add(_T("OnDirectory"));
	ListProperties->Add(_T("OnFileFound"));
	ListProperties->Add(_T("OnFileFiltered"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TFileList::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("Filter")) {
		return tpText;
	}
	if (asProperty == _T("Recursive")) {
		return tpBool;
	}
	if (asProperty == _T("Type")) {
		*asInfos = _T("tlFiles\ntlDirectories");
		return tpMultipleChoice;
	}
	if (asProperty == _T("OnDirectory")) {
    *asInfos = _T("(TObject *Sender, AnsiString *asDirectory)");
		return tpEvent;
	}
	if (asProperty == _T("OnFileFound")) {
    *asInfos = _T("(TObject *Sender, AnsiString *asFileName)");
		return tpEvent;
	}
	if (asProperty == _T("OnFileFiltered")) {
    *asInfos = _T("(TObject *Sender, AnsiString *asFileName)");
		return tpEvent;
	}
	return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TFileList::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("Directory")) {
		return _T("");
	}
	if (asProperty == _T("Filter")) {
		return _T("");
	}
	if (asProperty == _T("Recursive")) {
		return _T("False");
	}
  if (asProperty == _T("Types")) {
    return _T("[tlFiles]");
  }
  if (asProperty == _T("OnDirectory")) {
    return _T("");
  }
  if (asProperty == _T("OnFileFound")) {
    return _T("");
  }
  if (asProperty == _T("OnFileFiltered")) {
    return _T("");
  }
	return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TFileList::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("Filter")) {
		return Get_Filter();
	}
	if (asProperty == _T("Recursive")) {
		if (Get_Recursive()) return _T("True");
		else return _T("False");
	}
  if (asProperty == _T("Types")) {
    AnsiString asTypes;
    TTypeLists Types = Get_Types();
    if (Types.Contains(tlFiles)) asTypes += _T("tlFiles\n");
    if (Types.Contains(tlDirectories)) asTypes += _T("tlDirectories\n");
    return FormatProperty(asTypes, _T("["), _T("]"), _T(", "));
  }
	if (asProperty == _T("OnDirectory")) {
		return OnDirectory.AsString();
	}
	if (asProperty == _T("OnFileFound")) {
		return OnFileFound.AsString();
	}
	if (asProperty == _T("OnFileFiltered")) {
		return OnFileFiltered.AsString();
	}
	return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TFileList::SetProperty(AnsiString asProperty, AnsiString asValue,
															TPersistent *Sender) {
	if (asProperty == _T("Filter")) {
		Set_Filter(asValue);
		return true;
	}
	if (asProperty == _T("Recursive")) {
		Set_Recursive(asValue == _T("True"));
		return true;
	}
  if (asProperty == _T("Types")) {
    int Cur;
    AnsiString asMot;

    FTypes.Clear();
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("tlFiles")) FTypes << tlFiles;
      else if (asMot == _T("tlDirectories")) FTypes << tlDirectories;
    } while (asMot != _T(""));

    return true;
  }
	if (asProperty == _T("OnDirectory")) {
		OnDirectory.ObjCall = Sender;
		OnDirectory = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnFileFound")) {
		OnFileFound.ObjCall = Sender;
		OnFileFound = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnFileFiltered")) {
		OnFileFiltered.ObjCall = Sender;
		OnFileFiltered = asValue.c_str();
		return true;
	}
	return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
