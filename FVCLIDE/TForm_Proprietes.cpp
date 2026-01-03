//-----------------------------------------------------------------------------
//! @file TForm_Proprietes.cpp
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
#include <windows.h>
#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------
#include <windows.h>
#include <assert.h>
#include <math.h>

#include "TForm_Proprietes.h"
#include "TConfig.h"
#include "HLP_FVCLIDE_Fr.h"
#include "TFVCLIDEAutomation.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

int TForm_Proprietes::MemoLeft;
int TForm_Proprietes::MemoTop;
int TForm_Proprietes::MemoWidth;
int TForm_Proprietes::MemoHeight;
int TForm_Proprietes::MemoActivePageIndex;
bool TForm_Proprietes::bCoordRelatives;


//---------------------------------------------------------------------------
TForm_Proprietes::TForm_Proprietes(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {

  FAnchorsElement.Clear();
  FAnchorsElements.Clear();

  FLeftElement = 0;
  FTopElement = 0;
  FWidthElement = 0;
  FHeightElement = 0;

  FLeftParent = 0;
  FTopParent = 0;
  FWidthParent = 0;
  FHeightParent = 0;

	if (bCoordRelatives) RadioButton_Relatives->Checked = true;
  else RadioButton_Absolues->Checked = true;

  if (MemoWidth != 0 && MemoHeight != 0) {
    Set_Left(MemoLeft);
    Set_Top(MemoTop);
    Set_Width(MemoWidth);
    Set_Height(MemoHeight);
  }
  PageControl_Proprietes->ActivePageIndex = MemoActivePageIndex;
}

//---------------------------------------------------------------------------
TForm_Proprietes::~TForm_Proprietes(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété asNomObj
//---------------------------------------------------------------------------

AnsiString TForm_Proprietes::Get_NomObj(void) {
  AnsiString asRetour;

  asRetour = Edit_Nom->Text;
  return asRetour;
}

bool TForm_Proprietes::Set_NomObj(AnsiString asNewNomObj) {
  if (Edit_Nom->Text != asNewNomObj) {
    Edit_Nom->Text = asNewNomObj;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AnchorsElement
//---------------------------------------------------------------------------

TAnchors TForm_Proprietes::Get_AnchorsElement(void) {
  return FAnchorsElement;
}

bool TForm_Proprietes::Set_AnchorsElement(TAnchors NewAnchorsElement) {
  if (FAnchorsElement != NewAnchorsElement) {
    FAnchorsElement = NewAnchorsElement;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété AnchorsElements
//---------------------------------------------------------------------------

TAnchors TForm_Proprietes::Get_AnchorsElements(void) {
  return FAnchorsElements;
}

bool TForm_Proprietes::Set_AnchorsElements(TAnchors NewAnchorsElements) {
  if (FAnchorsElements != NewAnchorsElements) {
    FAnchorsElements = NewAnchorsElements;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LeftElement
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_LeftElement(void) {
  return FLeftElement;
}

bool TForm_Proprietes::Set_LeftElement(int NewLeftElement) {
  if (FLeftElement != NewLeftElement) {
    FLeftElement = NewLeftElement;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TopElement
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_TopElement(void) {
  return FTopElement;
}

bool TForm_Proprietes::Set_TopElement(int NewTopElement) {
  if (FTopElement != NewTopElement) {
    FTopElement = NewTopElement;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WidthElement
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_WidthElement(void) {
  return FWidthElement;
}

bool TForm_Proprietes::Set_WidthElement(int NewWidthElement) {
  if (FWidthElement != NewWidthElement) {
    FWidthElement = NewWidthElement;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HeightElement
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_HeightElement(void) {
  return FHeightElement;
}

bool TForm_Proprietes::Set_HeightElement(int NewHeightElement) {
  if (FHeightElement != NewHeightElement) {
    FHeightElement = NewHeightElement;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LeftParent
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_LeftParent(void) {
  return FLeftParent;
}

bool TForm_Proprietes::Set_LeftParent(int NewLeftParent) {
  if (FLeftParent != NewLeftParent) {
    FLeftParent = NewLeftParent;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TopParent
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_TopParent(void) {
  return FTopParent;
}

bool TForm_Proprietes::Set_TopParent(int NewTopParent) {
  if (FTopParent != NewTopParent) {
    FTopParent = NewTopParent;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WidthParent
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_WidthParent(void) {
  return FWidthParent;
}

bool TForm_Proprietes::Set_WidthParent(int NewWidthParent) {
  if (FWidthParent != NewWidthParent) {
    FWidthParent = NewWidthParent;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HeightParent
//---------------------------------------------------------------------------

int TForm_Proprietes::Get_HeightParent(void) {
  return FHeightParent;
}

bool TForm_Proprietes::Set_HeightParent(int NewHeightParent) {
  if (FHeightParent != NewHeightParent) {
    FHeightParent = NewHeightParent;
  }
  return true;
}

//---------------------------------------------------------------------------
void TForm_Proprietes::RadioButton_RelativesClick(TObject *Sender) {
  bCoordRelatives = true;
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void TForm_Proprietes::RadioButton_AbsoluesClick(TObject *Sender) {
  bCoordRelatives = false;
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void TForm_Proprietes::GetDecalageCoord(int *X1, int *Y1, int *X2, int *Y2) {

  if (bCoordRelatives) {
    if (FAnchorsElement.Contains(akLeft)) *X1 = FLeftParent;
    else *X1 = LeftParent + WidthParent;

    if (FAnchorsElement.Contains(akTop)) *Y1 = FTopParent;
    else *Y1 = TopParent + HeightParent;

    if (FAnchorsElement.Contains(akRight)) *X2 = FLeftParent + FWidthParent;
    else *X2 = LeftParent;

    if (FAnchorsElement.Contains(akBottom)) *Y2 = FTopParent + FHeightParent;
    else *Y2 = TopParent;
  }
  else {
    *X1 = 0;
    *Y1 = 0;
    *X2 = 0;
    *Y2 = 0;
  }
}


//---------------------------------------------------------------------------
void TForm_Proprietes::MajCoordonnees(void) {
  int X1, Y1, X2, Y2;


	if (PageControl_Proprietes->PageCount < 3) return;  // Onglet "Nom et position" supprimé (propriétés sur objets multiples)

  GetDecalageCoord(&X1, &Y1, &X2, &Y2);

  Edit_PositionX1->Text = AnsiString(FLeftElement - X1);
  Edit_PositionY1->Text = AnsiString(FTopElement - Y1);
  Edit_PositionX2->Text = AnsiString(FLeftElement + FWidthElement - X2);
  Edit_PositionY2->Text = AnsiString(FTopElement + FHeightElement - Y2);
  Edit_Largeur->Text = AnsiString(FWidthElement);
  Edit_Hauteur->Text = AnsiString(FHeightElement);

}

//---------------------------------------------------------------------------
void TForm_Proprietes::MajAncrages(void) {

	// FAnchorsElements avec un 's' : ancrages communs à tous les éléments
	if (FAnchorsElements.Contains(akLeft)) {
		if (FAnchorsElement.Contains(akLeft)) Shape_Left->Brush->Color = clRed;
		else Shape_Left->Brush->Color = clWhite;
	}
	else {
		Shape_Left->Brush->Color = clLtGray;
	}
	Shape_Left->Invalidate();

	if (FAnchorsElements.Contains(akLeft) && FAnchorsElements.Contains(akTop)) {
		if (FAnchorsElement.Contains(akLeft) && FAnchorsElement.Contains(akTop)) Shape_LeftTop->Brush->Color = clRed;
		else Shape_LeftTop->Brush->Color = clWhite;
	}
	else {
		Shape_LeftTop->Brush->Color = clLtGray;
	}
	Shape_LeftTop->Invalidate();

	if (FAnchorsElements.Contains(akTop)) {
		if (FAnchorsElement.Contains(akTop)) Shape_Top->Brush->Color = clRed;
		else Shape_Top->Brush->Color = clWhite;
	}
	else {
		Shape_Top->Brush->Color = clLtGray;
	}
	Shape_Top->Invalidate();

	if (FAnchorsElements.Contains(akRight) && FAnchorsElements.Contains(akTop)) {
		if (FAnchorsElement.Contains(akRight) && FAnchorsElement.Contains(akTop)) Shape_RightTop->Brush->Color = clRed;
		else Shape_RightTop->Brush->Color = clWhite;
	}
	else {
		Shape_RightTop->Brush->Color = clLtGray;
	}
	Shape_RightTop->Invalidate();

	if (FAnchorsElements.Contains(akRight)) {
		if (FAnchorsElement.Contains(akRight)) Shape_Right->Brush->Color = clRed;
		else Shape_Right->Brush->Color = clWhite;
	}
	else {
		Shape_Right->Brush->Color = clLtGray;
	}
	Shape_Right->Invalidate();

	if (FAnchorsElements.Contains(akRight) && FAnchorsElements.Contains(akBottom)) {
		if (FAnchorsElement.Contains(akRight) && FAnchorsElement.Contains(akBottom)) Shape_RightBottom->Brush->Color = clRed;
		else Shape_RightBottom->Brush->Color = clWhite;
	}
	else {
		Shape_RightBottom->Brush->Color = clLtGray;
	}
	Shape_RightBottom->Invalidate();

	if (FAnchorsElements.Contains(akBottom)) {
		if (FAnchorsElement.Contains(akBottom)) Shape_Bottom->Brush->Color = clRed;
		else Shape_Bottom->Brush->Color = clWhite;
	}
	else {
		Shape_Bottom->Brush->Color = clLtGray;
	}
	Shape_Bottom->Invalidate();

	if (FAnchorsElements.Contains(akLeft) && FAnchorsElements.Contains(akBottom)) {
		if (FAnchorsElement.Contains(akLeft) && FAnchorsElement.Contains(akBottom)) Shape_LeftBottom->Brush->Color = clRed;
		else Shape_LeftBottom->Brush->Color = clWhite;
	}
	else {
		Shape_LeftBottom->Brush->Color = clLtGray;
	}
	Shape_LeftBottom->Invalidate();

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Edit_PositionX1Change(TObject *Sender) {
  int X1, Y1, X2, Y2;
  AnsiString asText;


  GetDecalageCoord(&X1, &Y1, &X2, &Y2);
  asText = Edit_PositionX1->Text;
  FLeftElement = asText.ToIntDef(0) + X1;
}


//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Edit_PositionY1Change(TObject *Sender) {
  int X1, Y1, X2, Y2;
  AnsiString asText;


  GetDecalageCoord(&X1, &Y1, &X2, &Y2);
  asText = Edit_PositionY1->Text;
  FTopElement = asText.ToIntDef(0) + Y1;
}


//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Edit_PositionX2Change(TObject *Sender) {
  int X1, Y1, X2, Y2;
  AnsiString asText;


  GetDecalageCoord(&X1, &Y1, &X2, &Y2);
  asText = Edit_PositionX2->Text;
  FWidthElement = asText.ToIntDef(0) + X2 - FLeftElement;
}


//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Edit_PositionY2Change(TObject *Sender) {
  int X1, Y1, X2, Y2;
  AnsiString asText;


  GetDecalageCoord(&X1, &Y1, &X2, &Y2);
  asText = Edit_PositionY2->Text;
  FHeightElement = asText.ToIntDef(0) + Y2 - FTopElement;
}


//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Edit_LargeurChange(TObject *Sender) {
  AnsiString asText;


  asText = Edit_Largeur->Text;
  FWidthElement = asText.ToIntDef(0);
}


//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Edit_HauteurChange(TObject *Sender) {
  AnsiString asText;


  asText = Edit_Hauteur->Text;
  FHeightElement = asText.ToIntDef(0);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Button_AlignerClick(TObject *Sender) {
  FLeftElement = FLeftParent;
  FTopElement = FTopParent;
  FWidthElement = FWidthParent;
  FHeightElement = FHeightParent;

  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_LeftMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akLeft)) FAnchorsElement >> akLeft;
  else FAnchorsElement << akLeft;
	FAnchorsElements << akLeft;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_LeftTopMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akLeft) && FAnchorsElement.Contains(akTop)) FAnchorsElement >> akLeft >> akTop;
  else FAnchorsElement << akLeft << akTop;
	FAnchorsElements << akLeft << akTop;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_TopMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akTop)) FAnchorsElement >> akTop;
  else FAnchorsElement << akTop;
	FAnchorsElements << akTop;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_RightTopMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akRight) && FAnchorsElement.Contains(akTop)) FAnchorsElement >> akRight >> akTop;
  else FAnchorsElement << akRight << akTop;
	FAnchorsElements << akRight << akTop;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_RightMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akRight)) FAnchorsElement >> akRight;
  else FAnchorsElement << akRight;
	FAnchorsElements << akRight;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_RightBottomMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akRight) && FAnchorsElement.Contains(akBottom)) FAnchorsElement >> akRight >> akBottom;
  else FAnchorsElement << akRight << akBottom;
	FAnchorsElements << akRight << akBottom;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_BottomMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akBottom)) FAnchorsElement >> akBottom;
  else FAnchorsElement << akBottom;
	FAnchorsElements << akBottom;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Shape_LeftBottomMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
  if (FAnchorsElement.Contains(akLeft) && FAnchorsElement.Contains(akBottom)) FAnchorsElement >> akLeft >> akBottom;
  else FAnchorsElement << akLeft << akBottom;
	FAnchorsElements << akLeft << akBottom;

  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef1Click(TObject *Sender) {
  FAnchorsElement.Clear() << akLeft << akTop << akRight << akBottom;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef2Click(TObject *Sender) {
  FAnchorsElement.Clear() << akLeft << akTop << akRight;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef3Click(TObject *Sender) {
  FAnchorsElement.Clear() << akLeft << akRight << akBottom;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef4Click(TObject *Sender) {
  FAnchorsElement.Clear() << akLeft << akTop << akBottom;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef5Click(TObject *Sender) {
  FAnchorsElement.Clear() << akTop << akRight << akBottom;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef6Click(TObject *Sender) {
  FAnchorsElement.Clear();
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef7Click(TObject *Sender) {
  FAnchorsElement.Clear() << akLeft << akTop;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef8Click(TObject *Sender) {
  FAnchorsElement.Clear() << akTop;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef9Click(TObject *Sender) {
  FAnchorsElement.Clear() << akTop << akRight;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef10Click(TObject *Sender) {
  FAnchorsElement.Clear() << akLeft;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef11Click(TObject *Sender) {
  FAnchorsElement.Clear() << akRight;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef12Click(TObject *Sender) {
  FAnchorsElement.Clear() << akLeft << akBottom;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef13Click(TObject *Sender) {
  FAnchorsElement.Clear() << akBottom;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::BitBtn_Predef14Click(TObject *Sender) {
  FAnchorsElement.Clear() << akRight << akBottom;
  FAnchorsElements.Clear() << akLeft << akTop << akRight << akBottom;
  MajAncrages();
  MajCoordonnees();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Button_ParcourirClick(TObject *Sender) {
	HANDLE hFile;
	DWORD dw;
	BYTE *Content;
	TCHAR *ContentHex;
	DWORD FileSize;
	AnsiString asButtonName;
	AnsiString asEditEventName;
	AnsiString asProperty;
	TEdit *EditProperty = NULL;


	asButtonName = ((TPersistent *) Sender)->Name;
	if (asButtonName.Length() < 7) return;  // 7 = "Button_" + au moins un caractère
	asProperty = asButtonName.SubString(8, asButtonName.Length() - 7);
	EditProperty = (TEdit *) ScrollBox_Proprietes->FindComponent(AnsiString(_T("Edit_")) + asProperty);

	if (OpenDialog_Graphic->Execute()) {
    hFile = CreateFile((AnsiString) OpenDialog_Graphic->FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	  if (hFile != INVALID_HANDLE_VALUE) {

			FileSize = GetFileSize(hFile, NULL);
			Content = new BYTE[sizeof(DWORD) + FileSize];  // Longueur + contenu
			ContentHex = new TCHAR[2 * (sizeof(DWORD) + FileSize) + 1];

			*((DWORD *) Content) = FileSize;
			ReadFile(hFile, &Content[sizeof(DWORD)], FileSize, &dw, NULL);
			if (dw == FileSize && EditProperty) {
				BinToHex(Content, ContentHex, sizeof(DWORD) + FileSize);
				EditProperty->Text = ContentHex;
			}

			delete[] ContentHex;
			delete[] Content;
			CloseHandle(hFile);

		}
		else {
			// Message d'erreur ouverture asFileName
		}


	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Button_CodeClick(TObject *Sender) {
	AnsiString asFileName, asFileNameCpp;
	AnsiString asClassName;
	AnsiString asButtonName;
	AnsiString asEditEventName;
	AnsiString asProperty;
	AnsiString asEvent;
	TEdit *EditEvent = NULL;
	int Index;
	int i;


	Index = FVCLIDEAutomation->GetCurFileIndex();;
	if (Index == -1) {
		FVCLIDEAutomation->DisplayError(_TT("Sélectionnez l'onglet du dialogue."),
																		_TT("Fichier courant non défini"),
																		_T(""),
																		MB_OK | MB_ICONWARNING);
		return;
	}
	asFileName = FVCLIDEAutomation->GetFileName(Index);
	if (asFileName.IsEmpty()) {
		FVCLIDEAutomation->DisplayError(_TT("Le dialogue n'a pas de nom. Enregistrez-le avant de compléter le code."),
																		_TT("Fichier cpp non défini"),
																		_T(""),
																		MB_OK | MB_ICONWARNING);
		return;
	}
	asFileNameCpp = ChangeFileExt(asFileName, _T(".cpp"));
  i = asFileName.LastDelimiter(DIR_DELIMITER);
  asClassName = asFileName.SubString(i + 1, asFileName.Length() - i - 4);
	asButtonName = ((TPersistent *) Sender)->Name;
	if (asButtonName.Length() < 10) return;  // 10 = "Button_On" + au moins un caractère
	asProperty = asButtonName.SubString(8, asButtonName.Length() - 7);
	asEvent = asProperty;
	if (asEvent.SubString(1, 2) == _T("On")) {
		asEvent = asEvent.SubString(3, asProperty.Length() - 2);
	}
	EditEvent = (TEdit *) ScrollBox_Proprietes->FindComponent(AnsiString(_T("Edit_")) + asProperty);
	if (EditEvent) {
		if (((AnsiString) EditEvent->Text).IsEmpty()) EditEvent->Text = (AnsiString) Edit_Nom->Text + asEvent;
		FVCLIDEAutomation->SearchOrCreateCodeEvent(asFileNameCpp, asClassName, EditEvent->Text, EditEvent->Hint);
		ModalResult = mrOk;
	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_PROPRIETES);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Proprietes::FormClose(TObject *Sender,
      TCloseAction &Action) {
  MemoLeft = Get_Left();  // Pas FLeft parce qu'il y a un décalage
  MemoTop = Get_Top();
  MemoWidth = Get_Width();
  MemoHeight = Get_Height();
  MemoActivePageIndex = PageControl_Proprietes->ActivePageIndex;
	// bCoordRelatives = RadioButton_Relatives->Checked;  // Déjà mis à jour dans le OnClick
  Action = caHide;  // Sinon, les contrôles sont détruits et on ne peut plus les lire

}
//---------------------------------------------------------------------------

