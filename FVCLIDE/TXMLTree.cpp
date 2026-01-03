//-----------------------------------------------------------------------------
//! @file TXMLTree.cpp
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

#include "TXMLTree.h"

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TXMLTree::TXMLTree(void) {
  FType = tn_Balise;
  FNoComment = false;
	FParent = NULL;
}

//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TXMLTree::~TXMLTree(void) {
	Clear();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Name
//---------------------------------------------------------------------------

AnsiString TXMLTree::Get_Name(void) {
  return FName;
}

bool TXMLTree::Set_Name(AnsiString NewName) {
  if (FName != NewName) {
		if (NewName.Pos(_T(" ")) != 0) return false;
    FName = NewName;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Text
//---------------------------------------------------------------------------

AnsiString TXMLTree::Get_Text(void) {
  return FText;
}

bool TXMLTree::Set_Text(AnsiString NewText) {
  if (FText != NewText) {
    FText = NewText;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Type
//---------------------------------------------------------------------------

TYPENODE TXMLTree::Get_Type(void) {
  return FType;
}

bool TXMLTree::Set_Type(TYPENODE NewType) {
  if (FType != NewType) {
    FType = NewType;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NoComment
//---------------------------------------------------------------------------

bool TXMLTree::Get_NoComment(void) {
  return FNoComment;
}

bool TXMLTree::Set_NoComment(bool NewNoComment) {
  if (FNoComment != NewNoComment) {
    FNoComment = NewNoComment;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TXMLTree::Get_Count(void) {
  return (int) FNodes.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AttributesCount
//---------------------------------------------------------------------------

int TXMLTree::Get_AttributesCount(void) {
  return FAttributesNames.Count;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AttributeName
//---------------------------------------------------------------------------

AnsiString TXMLTree::Get_AttributeName(int Index) {
  return FAttributesNames[Index];
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AttributeValue
//---------------------------------------------------------------------------

AnsiString TXMLTree::Get_AttributeValue(int Index) {
  return FAttributesValues[Index];
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Node
//---------------------------------------------------------------------------

TXMLTree * TXMLTree::Get_Node(int Index) {
  return FNodes[Index];
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Parent
//---------------------------------------------------------------------------

TXMLTree *TXMLTree::Get_Parent(void) {
  return FParent;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Index
//---------------------------------------------------------------------------

int TXMLTree::Get_Index(void) {
  return FIndex;
}

//---------------------------------------------------------------------------
// Met à zéro la structure arborescente
//---------------------------------------------------------------------------
bool TXMLTree::Clear(void) {
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
TXMLTree * TXMLTree::InsertNode(int Index, TXMLTree &NewXMLNode) {

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
TXMLTree * TXMLTree::InsertNode(int Index, AnsiString asName) {
  TXMLTree * NewXMLNode = NULL;


	NewXMLNode = new TXMLTree();
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
TXMLTree * TXMLTree::AddNode(TXMLTree &NewXMLNode) {
  return InsertNode(-1, NewXMLNode);
}

//---------------------------------------------------------------------------
// Ajout d'un noeud
//---------------------------------------------------------------------------
TXMLTree * TXMLTree::AddNode(AnsiString asName) {
  return InsertNode(-1, asName);
}

//---------------------------------------------------------------------------
// Supprime un noeud dans une structure arborescente
//---------------------------------------------------------------------------
bool TXMLTree::DeleteNode(int Index) {
	if (Index < 0 || Index >= (int) FNodes.size()) return false;

	delete FNodes[Index];
  FNodes.erase(FNodes.begin() + Index);

  return true;
}

//---------------------------------------------------------------------------
// Lit un attibut d'un noeud
//---------------------------------------------------------------------------
AnsiString TXMLTree::GetAttribute(AnsiString asAttributeName) {
	int Index;


	Index = FAttributesNames.IndexOf(asAttributeName);
	if (Index != -1) return FAttributesValues[Index];

	return _T("");
}

//---------------------------------------------------------------------------
// Ecrit un attibut dans un noeud
//---------------------------------------------------------------------------
bool TXMLTree::SetAttribute(AnsiString asAttributeName, AnsiString asAttributeValue) {
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
// Détruit un attibut dans un noeud
//---------------------------------------------------------------------------
bool TXMLTree::DeleteAttribute(AnsiString asAttributeName) {
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
bool TXMLTree::LoadFromFile(AnsiString asXMLFile) {
  TFileASCII *FileASCII;
  TXMLTree *NewXMLNode;
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
  std::vector<TXMLTree *> Pile;


  FileASCII = new TFileASCII();
  FileASCII->ReadOnly = true;
  Ok = FileASCII->OpenFile(asXMLFile);
  Level = 0;
  if (Ok) {

    // Initialisations
    Clear();
    bInTag = false;
    bInName = false;
    bInAttributeName = false;
    bInAttributeValue = false;
    bInText = false;
    FType = tn_Header;

    Pile.push_back(this);
    Level = 1;
    TypeBalise = tb_Ouvrante;
    while (FileASCII->ReadLine(&asLine)) {
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
                NewXMLNode = new TXMLTree();
                NewXMLNode->Name = asName;
                NewXMLNode->Type = tn_Comment;
								NewXMLNode->FAttributesNames.Assign(&AttributesNames);
								NewXMLNode->FAttributesValues.Assign(&AttributesValues);
								AttributesNames.Clear();
								AttributesValues.Clear();
                Pile[Level - 1]->AddNode(*NewXMLNode);
              }
            }
            else if (TypeBalise == tb_Ouvrante) {
              NewXMLNode = new TXMLTree();
              NewXMLNode->Name = asName;
              NewXMLNode->Type = tn_Balise;
              NewXMLNode->FAttributesNames.Assign(&AttributesNames);
              NewXMLNode->FAttributesValues.Assign(&AttributesValues);
							AttributesNames.Clear();
							AttributesValues.Clear();
              Pile[Level - 1]->AddNode(*NewXMLNode);
              Pile.push_back(NewXMLNode);
              Level++;
            }
            else if (TypeBalise == tb_Fermante) {
              if (Level > 1) {
                Level--;
                if (Level > 1) {
                  Pile[Level - 1]->Text = (AnsiString) Pile[Level - 1]->Text + asText.TrimRight();
                  asText = _T("");
                }
                Pile.pop_back();
              }
            }
            else if (TypeBalise == tb_Isolee) {
              NewXMLNode = new TXMLTree();
              NewXMLNode->Name = asName;
              NewXMLNode->Type = tn_Balise;
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
            TypeBalise = tb_Ouvrante;
          }
          break;
        case _T('/'):
          if (bInTag && !bInAttributeValue) {
            if (asName.IsEmpty()) TypeBalise = tb_Fermante;
            else TypeBalise = tb_Isolee;
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
  }
  FileASCII->CloseFile();
  delete FileASCII;

  return Ok;
}

//---------------------------------------------------------------------------
// Enregistrement d'un fichier XML
//---------------------------------------------------------------------------
bool TXMLTree::SaveToFile(AnsiString asXMLFile) {
  TFileASCII *FileASCII;
  bool Ok;


  FileASCII = new TFileASCII();
  FileASCII->ReadOnly = false;
  Ok = FileASCII->OpenFile(asXMLFile);
  if (Ok) EnrNode(*FileASCII, 0);
  FileASCII->CloseFile();
  delete FileASCII;

  return Ok;
}

//---------------------------------------------------------------------------
// Enregistrement d'un noeud du fichier XML et de ses sous-noeuds.
//---------------------------------------------------------------------------
bool TXMLTree::EnrNode(TFileASCII &FileASCII, int Indent) {
  AnsiString asLine;
  AnsiString asIndent;
  AnsiString asAttribute;
  AnsiString asAttributes;
  int i;


  for (i = 0; i < Indent; i++) asIndent += _T(" ");

  switch (FType) {

  case tn_Header:
		asAttributes = _T("");
		for (i = 0; i < FAttributesNames.Count; i++) {
      asAttribute.sprintf(_T(" %s=\"%s\""), FAttributesNames[i].c_str(), FAttributesValues[i].c_str());
			asAttributes += asAttribute;
		}
    if (!FName.IsEmpty()) {
      asLine.sprintf(_T("<?%s%s?>"), FName.c_str(), asAttributes.c_str());
      FileASCII.WriteLine(asLine);
    }
    for (i = 0; i < (int) FNodes.size(); i++) {
      FNodes[i]->EnrNode(FileASCII, 0);
    }
    break;

  case tn_Balise:
		asAttributes = _T("");
		for (i = 0; i < FAttributesNames.Count; i++) {
      asAttribute.sprintf(_T(" %s=\"%s\""), FAttributesNames[i].c_str(), FAttributesValues[i].c_str());
			asAttributes += asAttribute;
		}
    if (FNodes.size() != 0) {
      asLine.sprintf(_T("%s<%s%s>"), asIndent.c_str(), FName.c_str(), asAttributes.c_str());
      FileASCII.WriteLine(asLine);
      if (!FText.IsEmpty()) {
        asLine.sprintf(_T("%s  %s"), asIndent.c_str(), FText.c_str());
        FileASCII.WriteLine(asLine);
      }
      for (i = 0; i < (int) FNodes.size(); i++) {
        FNodes[i]->EnrNode(FileASCII, Indent + 2);
      }
      asLine.sprintf(_T("%s</%s>"), asIndent.c_str(), FName.c_str());
      FileASCII.WriteLine(asLine);
    }
    else if (!FText.IsEmpty()) {
      asLine.sprintf(_T("%s<%s%s>%s</%s>"), asIndent.c_str(), FName.c_str(), asAttributes.c_str(), FText.c_str(), FName.c_str());
      FileASCII.WriteLine(asLine);
    }
    else {
      asLine.sprintf(_T("%s<%s%s/>"), asIndent.c_str(), FName.c_str(), asAttributes.c_str());
      FileASCII.WriteLine(asLine);
    }
    break;

  case tn_Comment:
    asLine.sprintf(_T("%s<!%s>"), asIndent.c_str(), FName.c_str());
    FileASCII.WriteLine(asLine);
    break;

  }

  return true;
}
//---------------------------------------------------------------------------

