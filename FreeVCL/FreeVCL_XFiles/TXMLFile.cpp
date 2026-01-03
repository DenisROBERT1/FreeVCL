//-----------------------------------------------------------------------------
//! @file TXMLFile.cpp
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

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

#include "TXMLFile.h"

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TXMLFile::TXMLFile(TComponent *AOwner):
	TComponent(AOwner) {
  FType = tn_Tag;
  FNoComment = false;
	FParent = NULL;
}

//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TXMLFile::~TXMLFile(void) {
	Clear();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TXMLFile::Get_Name(void) {
  return FName;
}

bool TXMLFile::Set_Name(AnsiString NewName) {
  if (FName != NewName) {
		if (NewName.Pos(_T(" ")) != 0) return false;
    FName = NewName;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TXMLFile::Get_Text(void) {
  return FText;
}

bool TXMLFile::Set_Text(AnsiString NewText) {
  if (FText != NewText) {
    FText = NewText;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Type
//---------------------------------------------------------------------------

TYPENODE TXMLFile::Get_Type(void) {
  return FType;
}

bool TXMLFile::Set_Type(TYPENODE NewType) {
  if (FType != NewType) {
    FType = NewType;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NoComment
//---------------------------------------------------------------------------

bool TXMLFile::Get_NoComment(void) {
  return FNoComment;
}

bool TXMLFile::Set_NoComment(bool NewNoComment) {
  if (FNoComment != NewNoComment) {
    FNoComment = NewNoComment;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TXMLFile::Get_Count(void) {
  return (int) FNodes.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AttributesCount
//---------------------------------------------------------------------------

int TXMLFile::Get_AttributesCount(void) {
  return FAttributesNames.Count;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AttributeName
//---------------------------------------------------------------------------

AnsiString TXMLFile::Get_AttributeName(int Index) {
  return FAttributesNames[Index];
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AttributeValue
//---------------------------------------------------------------------------

AnsiString TXMLFile::Get_AttributeValue(int Index) {
  return FAttributesValues[Index];
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Node
//---------------------------------------------------------------------------

TXMLFile * TXMLFile::Get_Node(int Index) {
  return FNodes[Index];
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Parent
//---------------------------------------------------------------------------

TXMLFile *TXMLFile::Get_Parent(void) {
  return FParent;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Index
//---------------------------------------------------------------------------

int TXMLFile::Get_Index(void) {
  return FIndex;
}

//---------------------------------------------------------------------------
// Met à zéro la structure arborescente
//---------------------------------------------------------------------------
bool TXMLFile::Clear(void) {
  int i;


  for (i = 0; i < (int) FNodes.size(); i++) {
    FNodes[i]->Clear();
    delete FNodes[i];
  }
  FNodes.clear();
  FAttributesNames.Clear();
  FAttributesValues.Clear();

	if (!FParent) {
		FName = _T("xml");
		FType = tn_Header;
	}

  return true;

}

//---------------------------------------------------------------------------
// Insère un noeud dans une structure arborescente
//---------------------------------------------------------------------------
TXMLFile * TXMLFile::InsertNode(int Index, TXMLFile &NewXMLNode) {

	if (Index == -1) {
		NewXMLNode.FIndex = (int) FNodes.size();
		FNodes.push_back(&NewXMLNode);
	}
	else {
		int i;

		FNodes.insert(FNodes.begin() + Index, &NewXMLNode);

		// Décalage des index
		for (i = Index; i < (int) FNodes.size(); i++) {
			FNodes[i]->FIndex = i;
		}

	}
	NewXMLNode.FParent = this;

  return &NewXMLNode;
}

//---------------------------------------------------------------------------
// Insère un noeud dans une structure arborescente
//---------------------------------------------------------------------------
TXMLFile * TXMLFile::InsertNode(int Index, AnsiString asName) {
  TXMLFile * NewXMLNode = NULL;


	NewXMLNode = new TXMLFile(this);
  NewXMLNode->FName = asName;
  if (!InsertNode(Index, *NewXMLNode)) {
    delete NewXMLNode;
    NewXMLNode = NULL;
  }

  return NewXMLNode;
}

//---------------------------------------------------------------------------
// Ajout d'un noeud
//---------------------------------------------------------------------------
TXMLFile * TXMLFile::AddNode(TXMLFile &NewXMLNode) {
  return InsertNode(-1, NewXMLNode);
}

//---------------------------------------------------------------------------
// Ajout d'un noeud
//---------------------------------------------------------------------------
TXMLFile * TXMLFile::AddNode(AnsiString asName) {
  return InsertNode(-1, asName);
}

//---------------------------------------------------------------------------
// Supprime un noeud dans une structure arborescente
//---------------------------------------------------------------------------
bool TXMLFile::DeleteNode(int Index) {
	if (Index < 0 || Index >= (int) FNodes.size()) return false;

	delete FNodes[Index];
  FNodes.erase(FNodes.begin() + Index);

  return true;
}

//---------------------------------------------------------------------------
// Lit un attribut d'un noeud
//---------------------------------------------------------------------------
AnsiString TXMLFile::GetAttribute(AnsiString asAttributeName) {
	int Index;


	Index = FAttributesNames.IndexOf(asAttributeName);
	if (Index != -1) return FAttributesValues[Index];

	return _T("");
}

//---------------------------------------------------------------------------
// Ecrit un attribut dans un noeud
//---------------------------------------------------------------------------
bool TXMLFile::SetAttribute(AnsiString asAttributeName, AnsiString asAttributeValue) {
	int Index;


	Index = FAttributesNames.IndexOf(asAttributeName);
	if (Index != -1) {
		FAttributesValues.Set_Strings(Index, asAttributeValue);
		return false;
	}
	else {
		FAttributesNames.Add(asAttributeName);
		FAttributesValues.Add(asAttributeValue);
		return true;
	}

}

//---------------------------------------------------------------------------
// Détruit un attribut dans un noeud
//---------------------------------------------------------------------------
bool TXMLFile::DeleteAttribute(AnsiString asAttributeName) {
	int Index;


	Index = FAttributesNames.IndexOf(asAttributeName);
	if (Index == -1) {
		return false;
	}
	else {
		FAttributesNames.Delete(Index);
		FAttributesValues.Delete(Index);
		return true;
	}

}

//---------------------------------------------------------------------------
// Lecture d'un fichier XML
//---------------------------------------------------------------------------
bool TXMLFile::LoadFromFile(AnsiString asXMLFile) {
  TXMLFile *NewXMLNode;
  AnsiString asLine;
  AnsiString asName;
  AnsiString asAttributeName;
  AnsiString asAttributeValue;
  AnsiString asText;
	TStringList AttributesNames;
	TStringList AttributesValues;
  bool Ok;
  int i;
  int Level;
  TCHAR c;
  bool bInTag;
  bool bInName;
  bool bInAttributeName;
  bool bInAttributeValue;
	bool bSimpleQuote;
  bool bInText;
  bool bTraite;
  TYPEBALISE TypeBalise;
  std::vector<TXMLFile *> Pile;
	HANDLE hFile;


  hFile = CreateFile(asXMLFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

    // Initialisations
    Clear();
    bInTag = false;
    bInName = false;
    bInAttributeName = false;
    bInAttributeValue = false;
    bInText = false;
    FType = tn_Header;
	  Level = 0;

    Pile.push_back(this);
    Level = 1;
    TypeBalise = tb_OpenTag;
    while (FileLineInput(hFile, &asLine)) {
      for (i = 1; i <= asLine.Length(); i++) {
        c = asLine[i];
        bTraite = false;
        switch (c) {
        case _T('<'):
          if (!bInTag) {
            if (!asText.IsEmpty()) {
              Pile[Level - 1]->Text = (AnsiString) Pile[Level - 1]->Text + asText.TrimRight();
              asText = _T("");
            }
            bInTag = true;
            bInName = true;
            bInText = false;
            bTraite = true;
          }
          break;
        case _T(' '):
          if (bInTag && !bInAttributeValue) {
						bInName = false;
						bInAttributeName = false;
          }
          break;
        case _T('>'):
          if (bInTag && !bInAttributeValue) {
            if (TypeBalise == tb_Header) {
              FName = asName;
							FAttributesNames.Assign(&AttributesNames);
							FAttributesValues.Assign(&AttributesValues);
							AttributesNames.Clear();
							AttributesValues.Clear();
            }
            else if (TypeBalise == tb_Comment) {
              if (!FNoComment) {
                NewXMLNode = new TXMLFile(this);
                NewXMLNode->Name = asName;
                NewXMLNode->Type = tn_Comment;
								NewXMLNode->FAttributesNames.Assign(&AttributesNames);
								NewXMLNode->FAttributesValues.Assign(&AttributesValues);
								AttributesNames.Clear();
								AttributesValues.Clear();
                Pile[Level - 1]->AddNode(*NewXMLNode);
              }
            }
            else if (TypeBalise == tb_OpenTag) {
              NewXMLNode = new TXMLFile(this);
              NewXMLNode->Name = asName;
              NewXMLNode->Type = tn_Tag;
              NewXMLNode->FAttributesNames.Assign(&AttributesNames);
              NewXMLNode->FAttributesValues.Assign(&AttributesValues);
							AttributesNames.Clear();
							AttributesValues.Clear();
              Pile[Level - 1]->AddNode(*NewXMLNode);
              Pile.push_back(NewXMLNode);
              Level++;
            }
            else if (TypeBalise == tb_CloseTag) {
              if (Level > 1) {
                Level--;
                if (Level > 1) {
                  Pile[Level - 1]->Text = (AnsiString) Pile[Level - 1]->Text + asText.TrimRight();
                  asText = _T("");
                }
                Pile.pop_back();
              }
            }
            else if (TypeBalise == tb_SingleTag) {
              NewXMLNode = new TXMLFile(this);
              NewXMLNode->Name = asName;
              NewXMLNode->Type = tn_Tag;
              NewXMLNode->FAttributesNames.Assign(&AttributesNames);
              NewXMLNode->FAttributesValues.Assign(&AttributesValues);
							AttributesNames.Clear();
							AttributesValues.Clear();
              Pile[Level - 1]->AddNode(*NewXMLNode);
            }
            asName = _T("");
            bInTag = false;
            bInName = false;
            bInText = true;
            bTraite = true;
            TypeBalise = tb_OpenTag;
          }
          break;
        case _T('/'):
          if (bInTag && !bInAttributeValue) {
            if (asName.IsEmpty()) TypeBalise = tb_CloseTag;
            else TypeBalise = tb_SingleTag;
            bTraite = true;
          }
          break;
        case _T('?'):
          if (bInTag) {
            if (asName.IsEmpty()) TypeBalise = tb_Header;
            bTraite = true;
          }
          break;
        case _T('!'):
          if (bInTag) {
            if (asName.IsEmpty()) TypeBalise = tb_Comment;
            bTraite = true;
          }
          break;
        case _T('='):
          if (bInTag) {
						bInAttributeName = false;
          }
          break;
        case _T('\''):
					if (!bInAttributeValue) {
						if (bInTag && !bInAttributeName && !asAttributeName.IsEmpty()) {
							bInAttributeValue = true;
							bSimpleQuote = true;
	            bTraite = true;
						}
					}
					else if (bSimpleQuote) {
						bInAttributeValue = false;
						// Enregistrement de l'attribut
						AttributesNames.Add(asAttributeName);
						AttributesValues.Add(asAttributeValue);
						// Mise à zéro pour l'attribut suivant
						asAttributeName = _T("");
						asAttributeValue = _T("");
					}
          break;
        case _T('"'):
					if (!bInAttributeValue) {
						if (bInTag && !bInAttributeName && !asAttributeName.IsEmpty()) {
							bInAttributeValue = true;
							bSimpleQuote = false;
	            bTraite = true;
						}
					}
					else if (!bSimpleQuote)  {
						bInAttributeValue = false;
						// Enregistrement de l'attribut
						AttributesNames.Add(asAttributeName);
						AttributesValues.Add(asAttributeValue);
						// Mise à zéro pour l'attribut suivant
						asAttributeName = _T("");
						asAttributeValue = _T("");
					}
          break;
        default:
          if (bInTag && !bInName && !bInAttributeValue) {
						bInAttributeName = true;
          }
          break;
        }
        if (!bTraite) {
          if (bInName) asName += c;
          else if (bInAttributeName) asAttributeName += c;
          else if (bInAttributeValue) asAttributeValue += c;
          else if (bInText && (!asText.IsEmpty() || (c != _T(' ') && c != _T('\t')))) asText += c;
        }

      }
    }
		Ok = true;
	}
	else {
    // Erreur ouverture asXMLFile
		FLastError = GetLastError();
		Ok = false;
	}

  return Ok;
}

//---------------------------------------------------------------------------
// Enregistrement d'un fichier XML
//---------------------------------------------------------------------------
bool TXMLFile::SaveToFile(AnsiString asXMLFile) {
	HANDLE hFile;
  bool Ok;


  hFile = CreateFile(asXMLFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
		Ok = EnrNode(hFile, 0);
		CloseHandle(hFile);
	}
	else {
    // Erreur ouverture asXMLFile
		FLastError = GetLastError();
		Ok = false;
	}

  return Ok;
}

//---------------------------------------------------------------------------
// Enregistrement d'un noeud du fichier XML et de ses sous-noeuds.
//---------------------------------------------------------------------------
bool TXMLFile::EnrNode(HANDLE hFile, int Indent) {
  AnsiString asLine;
  AnsiString asIndent;
  AnsiString asAttribute;
  AnsiString asAttributes;
	DWORD dw;
  int i;


  for (i = 0; i < Indent; i++) asIndent += _T(" ");

  switch (FType) {

  case tn_Header:
		asAttributes = _T("");
		for (i = 0; i < FAttributesNames.Count; i++) {
      asAttribute.sprintf(_T(" %s=\"%s\""), (LPCTSTR) FAttributesNames[i], (LPCTSTR) FAttributesValues[i]);
			asAttributes += asAttribute;
		}
    if (!FName.IsEmpty()) {
      asLine.sprintf(_T("<?%s%s?>\r\n"), (LPCTSTR) FName, (LPCTSTR) asAttributes);
      WriteFile(hFile, (LPCSTR) asLine.AsCharString(), asLine.Length(), &dw, NULL);
    }
    for (i = 0; i < (int) FNodes.size(); i++) {
      FNodes[i]->EnrNode(hFile, 0);
    }
    break;

  case tn_Tag:
		asAttributes = _T("");
		for (i = 0; i < FAttributesNames.Count; i++) {
      asAttribute.sprintf(_T(" %s=\"%s\""), (LPCTSTR) FAttributesNames[i], (LPCTSTR) FAttributesValues[i]);
			asAttributes += asAttribute;
		}
    if (FNodes.size() != 0) {
      asLine.sprintf(_T("%s<%s%s>\r\n"), (LPCTSTR) asIndent, (LPCTSTR) FName, (LPCTSTR) asAttributes);
      WriteFile(hFile, (LPCSTR) asLine.AsCharString(), asLine.Length(), &dw, NULL);
      if (!FText.IsEmpty()) {
        asLine.sprintf(_T("%s  %s\r\n"), (LPCTSTR) asIndent, (LPCTSTR) FText);
	      WriteFile(hFile, (LPCSTR) asLine.AsCharString(), asLine.Length(), &dw, NULL);
      }
      for (i = 0; i < (int) FNodes.size(); i++) {
        FNodes[i]->EnrNode(hFile, Indent + 2);
      }
      asLine.sprintf(_T("%s</%s>\r\n"), (LPCTSTR) asIndent, (LPCTSTR) FName);
      WriteFile(hFile, (LPCSTR) asLine.AsCharString(), asLine.Length(), &dw, NULL);
    }
    else if (!FText.IsEmpty()) {
      asLine.sprintf(_T("%s<%s%s>%s</%s>\r\n"), (LPCTSTR) asIndent, (LPCTSTR) FName, (LPCTSTR) asAttributes, FText.c_str(), FName.c_str());
      WriteFile(hFile, (LPCSTR) asLine.AsCharString(), asLine.Length(), &dw, NULL);
    }
    else {
      asLine.sprintf(_T("%s<%s%s/>\r\n"), (LPCTSTR) asIndent, (LPCTSTR) FName, (LPCTSTR) asAttributes);
      WriteFile(hFile, (LPCSTR) asLine.AsCharString(), asLine.Length(), &dw, NULL);
    }
    break;

  case tn_Comment:
    asLine.sprintf(_T("%s<!%s>\r\n"), (LPCTSTR) asIndent, (LPCTSTR) FName);
    WriteFile(hFile, (LPCSTR) asLine.AsCharString(), asLine.Length(), &dw, NULL);
    break;

  }

  return true;
}

//---------------------------------------------------------------------------
//   bool FileLineInput(HFILE hFile, BYTE * szBuffer, DWORD cbMaxLenght)
// Description:
//   Lecture d'une donnée dans un fichier texte (séparateur = CR)
// Paramètres:
//   [in]  HFILE hFile = handle du fichier
//   [out] szBuffer = ligne lue
//   [in]  DWORD cbMaxBuffer = longueur du buffer
// Sortie:
//   FALSE si fin de fichier atteinte ou erreur de lecture
//---------------------------------------------------------------------------

bool TXMLFile::FileLineInput(HANDLE hFile, AnsiString *asBuffer) {
  char c;
  DWORD dwBytesRead;


  *asBuffer = _T("");
  while (true) {
    ReadFile(hFile, &c, 1, &dwBytesRead, NULL);
    if (dwBytesRead != 1) {
      if (!asBuffer->empty()) break;
      else return false;
    }
    if (c == _T('\n')) break;
    if (c != _T('\r')) *asBuffer += (unsigned char) c;
  }

  return true;
}

//---------------------------------------------------------------------------

