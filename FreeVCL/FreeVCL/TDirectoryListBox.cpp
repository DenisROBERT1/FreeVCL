//-----------------------------------------------------------------------------
//! @file TDirectoryListBox.cpp
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
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TDirectoryListBox
//---------------------------------------------------------------------------

TDirectoryListBox::TDirectoryListBox(TComponent* AOwner): TTreeView(AOwner) {

  // Initialisations
  FClassName = _T("TDirectoryListBox");
  FCaseSensitive = true;
  FDirLabel = NULL;
  FFileList = NULL;
  FShowFiles = false;
  FPreserveCase = true;
  ReadOnly = false;

	FilesImages = new TImageList(NULL);
	FilesImages->Width = GetSystemMetrics(SM_CXSMICON);
	FilesImages->Height = GetSystemMetrics(SM_CYSMICON);
	FilesImages->Masked = true;
	FilesImages->BkColor = FColor;
	Images = FilesImages;

	SET_EVENT(this, TOnExpanded, OnExpanded, TDirectoryListBox, this, NodeExpanded);

}

//---------------------------------------------------------------------------
TDirectoryListBox::~TDirectoryListBox(void) {
	delete FilesImages;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CaseSensitive
//---------------------------------------------------------------------------

bool TDirectoryListBox::Get_CaseSensitive() {
  return FCaseSensitive;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Directory
//---------------------------------------------------------------------------

AnsiString TDirectoryListBox::Get_Directory() {
  return FDirectory;
}

bool TDirectoryListBox::Set_Directory(AnsiString NewDirectory) {

	// Suppression éventuelle du slash de fin (sinon bug d'affichage)
	int Len = NewDirectory.Length();
	if (Len > 0 && (NewDirectory[Len] == _T('\\') || NewDirectory[Len] == _T('/'))) {
		NewDirectory = NewDirectory.SubString(1, Len - 1);
	}

  if (FDirectory != NewDirectory) {
    FDirectory = NewDirectory;
		SetRootDirectory(FDirectory);
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DirLabel
//---------------------------------------------------------------------------

TLabel *TDirectoryListBox::Get_DirLabel() {
  return FDirLabel;
}

bool TDirectoryListBox::Set_DirLabel(TLabel *NewDirLabel) {
  if (FDirLabel != NewDirLabel) {
    FDirLabel = NewDirLabel;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Drive
//---------------------------------------------------------------------------

AnsiString TDirectoryListBox::Get_Drive() {
  return FDrive;
}

bool TDirectoryListBox::Set_Drive(AnsiString NewDrive) {
  if (FDrive != NewDrive) {
    FDrive = NewDrive;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FileList
//---------------------------------------------------------------------------

TListBox *TDirectoryListBox::Get_FileList() {
  return FFileList;
}

bool TDirectoryListBox::Set_FileList(TListBox *NewFileList) {
  if (FFileList != NewFileList) {
    FFileList = NewFileList;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PreserveCase
//---------------------------------------------------------------------------

bool TDirectoryListBox::Get_PreserveCase() {
  return FPreserveCase;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowFiles
//---------------------------------------------------------------------------

bool TDirectoryListBox::Get_ShowFiles() {
  return FShowFiles;
}

bool TDirectoryListBox::Set_ShowFiles(bool NewShowFiles) {
  if (FShowFiles != NewShowFiles) {
    FShowFiles = NewShowFiles;
		Update();
  }
  return true;
}


//---------------------------------------------------------------------------
// Mise en forme d'un nom de répertoire en vue de l'affichage
//---------------------------------------------------------------------------
AnsiString FASTCALL TDirectoryListBox::DisplayCase(const AnsiString S) {
	if (FCaseSensitive) return S.LowerCase();
	return S;
}

//---------------------------------------------------------------------------
// Comparaison de deux chaînes
//---------------------------------------------------------------------------
int FASTCALL TDirectoryListBox::FileCompareText(const AnsiString A, const AnsiString B) {
	if (FCaseSensitive) {
		return A.AnsiCompareIC(B);
	}
	else {
		return A.AnsiCompareIC(B);
	}
}

//---------------------------------------------------------------------------
// Déroule l'élément courant
//---------------------------------------------------------------------------
void FASTCALL TDirectoryListBox::OpenCurrent(void) {
}

//---------------------------------------------------------------------------
// Met à jour la liste des répertoires
//---------------------------------------------------------------------------
bool FASTCALL TDirectoryListBox::Update(void) {
	TStringList *slMemoExpanded;
	AnsiString asMemoSelected;
	int i, j;


	slMemoExpanded = new TStringList();

	// Mémorisation de l'état du tree-view (noeuds développés, sélection)
	for (i = 0; i < FItems->Count; i++) {
		if (FItems->Item[i]->Expanded) {
			slMemoExpanded->Add(GetItemPath(FItems->Item[i]));
		}
		if (FItems->Item[i]->Selected) {
			asMemoSelected = GetItemPath(FItems->Item[i]);
		}
	}

	FItems->Clear();
	FilesImages->Clear();
	SetRootDirectory(FDirectory);

	// Etat du tree-view
	AnsiString asItemPath;
	for (i = 0; i < FItems->Count; i++) {
		asItemPath = GetItemPath(FItems->Item[i]);
		for (j = 0; j < slMemoExpanded->Count; j++) {
			if (asItemPath == slMemoExpanded->Strings[j]) {
				FItems->Item[i]->Expand(true);
			}
		}
		if (asItemPath == asMemoSelected) {
			FItems->Item[i]->Selected = true;
		}
	}

	delete slMemoExpanded;

	return true;
}

//---------------------------------------------------------------------------
bool TDirectoryListBox::SetRootDirectory(AnsiString ADirectory) {
	bool Ok;

	FItems->BeginUpdate();
	FItems->Clear();
  Ok = BuildTreeView(FItems->FirstNode, NULL, _T(""), ADirectory, 2);
	FItems->EndUpdate();

	return Ok;
}

//---------------------------------------------------------------------------
bool TDirectoryListBox::BuildTreeView(TTreeNode *TreeNodeParent, TTreeNode *TreeNode, AnsiString asPath, AnsiString asName, int State) {

  if (TreeNode == NULL) {

		// Nom complet du fichier
		AnsiString asPathSub;
		if (!asPath.IsEmpty()) asPathSub = asPath + DIR_DELIMITER + asName;
		else asPathSub = asName;

		// Icône associée
		SHFILEINFO shFileInfo;
		ICONINFO stIconInfo;
		TBitmap *BitmapColor, *BitmapMasked;
		int NumImage = -1;

		SHGetFileInfo(asPathSub.c_str(), 0, &shFileInfo, sizeof(shFileInfo),
									SHGFI_ICON | SHGFI_SMALLICON | SHGFI_DISPLAYNAME | SHGFI_ADDOVERLAYS);
		if (GetIconInfo(shFileInfo.hIcon, &stIconInfo) == TRUE) {
			BitmapColor = new TBitmap();
			BitmapColor->Handle = stIconInfo.hbmColor;
			BitmapMasked = new TBitmap();
			BitmapMasked->Handle = stIconInfo.hbmMask;
			NumImage = FilesImages->Add(BitmapColor, BitmapMasked);
			// Pas de DeleteObject de hbmColor et hbmMask, l'objet TBitmap s'en chargera
			delete BitmapColor;
			delete BitmapMasked;
		}
		DestroyIcon(shFileInfo.hIcon);

		// Ajout du noeud au tree-view
		TreeNode = FItems->AddChild(TreeNodeParent, shFileInfo.szDisplayName);
		TreeNode->Data = (void *) (size_t) State;
		TreeNode->Name = asName;
		TreeNode->ImageIndex = NumImage;
		TreeNode->SelectedIndex = NumImage;

	}

  if (State > 0) {
		int i;

		TStringList *StringList = new TStringList();
		TFileList *FileList = new TFileList(NULL);
		FileList->Recursive = false;
		TTypeLists Types;
		Types.Clear() << tlDirectories;
		FileList->Types = Types;
		FileList->Filter = _T("*.*");
		AnsiString asPathSub;
		if (!asPath.IsEmpty()) asPathSub = asPath + DIR_DELIMITER + asName;
		else asPathSub = asName;
		FileList->GetFileList(asPathSub, false, StringList);

		for (i = 0; i < StringList->Count; i++) {
			BuildTreeView(TreeNode, NULL, asPathSub, StringList->Strings[i], State - 1);
		}

		if (FShowFiles) {
			Types.Clear() << tlFiles;
			FileList->Types = Types;
			FileList->Filter = _T("*.*");
			if (!asPath.IsEmpty()) asPathSub = asPath + DIR_DELIMITER + asName;
			else asPathSub = asName;
			StringList->Clear();
			FileList->GetFileList(asPathSub, false, StringList);

			for (i = 0; i < StringList->Count; i++) {
				BuildTreeView(TreeNode, NULL, asPathSub, StringList->Strings[i], -1);
			}

		}

		delete FileList;
		delete StringList;

		if (State == 2) TreeNode->Expand(false);

	}

  return TRUE;
}

//---------------------------------------------------------------------------
void FASTCALL TDirectoryListBox::NodeExpanded(TObject *Sender, TTreeNode *TreeNode) {
	TTreeNode *TreeNodeChild;
	AnsiString asPath;

	if (TreeNode->Data >= (void *) 2) return;

	TreeNodeChild = TreeNode->GetFirstChild();
  asPath = GetItemPath(TreeNode);
	while (TreeNodeChild) {
    BuildTreeView(TreeNode, TreeNodeChild, asPath, TreeNode->Text, 1);
		TreeNodeChild = TreeNode->GetNextChild(TreeNodeChild);
	}

	TreeNode->Data = (void *) 2;
}

//---------------------------------------------------------------------------
// Nom complet du répertoire d'un noeud en fonction de son index
//---------------------------------------------------------------------------
AnsiString FASTCALL TDirectoryListBox::GetItemPath(int Index) {
	return GetItemPath(FItems->Item[Index]);
}

//---------------------------------------------------------------------------
// Nom complet du répertoire d'un noeud
//---------------------------------------------------------------------------
AnsiString TDirectoryListBox::GetItemPath(TTreeNode *TreeNode) {
	TTreeNode *TreeNodeParent;

	TreeNodeParent = TreeNode->Parent;
	if (TreeNodeParent == NULL || TreeNodeParent->IsFirstNode) return TreeNode->Name;
	return GetItemPath(TreeNodeParent) + AnsiString(DIR_DELIMITER) + (AnsiString) (TreeNode->Name);
}

//---------------------------------------------------------------------------
// Test fichier ou répertoire
//---------------------------------------------------------------------------
bool FASTCALL TDirectoryListBox::IsFile(TTreeNode *TreeNode) {
	if (TreeNode == NULL) return false;
	return (int) (size_t) (void *) TreeNode->Data == -1;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDirectoryListBox::GetListProperties(TStrings *ListProperties) {

	TTreeView::GetListProperties(ListProperties);

  ListProperties->Add(_T("CaseSensitive"));
  ListProperties->Add(_T("Directory"));
  ListProperties->Add(_T("DirLabel"));
  ListProperties->Add(_T("ShowFiles"));
  ListProperties->Add(_T("Drive"));
  ListProperties->Add(_T("FileList"));
  ListProperties->Add(_T("PreserveCase"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDirectoryListBox::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("CaseSensitive")) {
    return tpBool;
  }
  if (asProperty == _T("Directory")) {
    return tpText;
  }
  if (asProperty == _T("DirLabel")) {
    return tpObject;
  }
  if (asProperty == _T("ShowFiles")) {
    return tpBool;
  }
  if (asProperty == _T("Drive")) {
    return tpText;
  }
  if (asProperty == _T("FileList")) {
    return tpObject;
  }
  if (asProperty == _T("PreserveCase")) {
    return tpBool;
  }
  return TTreeView::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDirectoryListBox::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("CaseSensitive")) {
    return _T("False");
  }
  if (asProperty == _T("Directory")) {
    return _T("");
  }
  if (asProperty == _T("DirLabel")) {
    return _T("");
  }
  if (asProperty == _T("ShowFiles")) {
    return _T("False");
  }
  if (asProperty == _T("Drive")) {
    return _T("");
  }
  if (asProperty == _T("FileList")) {
    return _T("");
  }
  if (asProperty == _T("PreserveCase")) {
    return _T("False");
  }

  return TTreeView::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDirectoryListBox::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("CaseSensitive")) {
    if (Get_CaseSensitive()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Directory")) {
    return Get_Directory();
  }
  if (asProperty == _T("DirLabel")) {
    TLabel *DirLabel = Get_DirLabel();
    if (DirLabel) return DirLabel->Name;
    return _T("");
  }
  if (asProperty == _T("ShowFiles")) {
    if (Get_ShowFiles()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Drive")) {
    return Get_Drive();
  }
  if (asProperty == _T("FileList")) {
    TListBox *FileList = Get_FileList();
    if (FileList) return FileList->Name;
    return _T("");
  }
  if (asProperty == _T("PreserveCase")) {
    if (Get_PreserveCase()) return _T("True");
    else return _T("False");
  }

  return TTreeView::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDirectoryListBox::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Directory")) {
    Set_Directory(asValue);
    return true;
  }
  if (asProperty == _T("DirLabel")) {
    TLabel *DirLabel;
    DirLabel = (TLabel *) FindRootComponent()->FindComponent(asValue);
    Set_DirLabel(DirLabel);
    return true;
  }
  if (asProperty == _T("ShowFiles")) {
    Set_ShowFiles(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Drive")) {
    Set_Drive(asValue);
    return true;
  }
  if (asProperty == _T("FileList")) {
    TListBox *FileList;
    FileList = (TListBox *) FindRootComponent()->FindComponent(asValue);
    Set_FileList(FileList);
    return true;
  }
  return TTreeView::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------
