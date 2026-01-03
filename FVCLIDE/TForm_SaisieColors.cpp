//-----------------------------------------------------------------------------
//! @file TForm_SaisieColors.cpp
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

#include <FreeVCL.h>
#include <TDictionnary.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------

#include "TConfig.h"
#include "TForm_DisplayError.h"
#include "TFrame_EditFichierText.h"
#include "TForm_SaisieColors.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
TForm_SaisieColors::TForm_SaisieColors(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {
	int i;


	bNotChange = false;
  ListKeywords = NULL;

	TTextEditor *TextEditor = Frame_EditFichierText->TextEditor;

	TextEditor->LinesNumbers = true;
	TextEditor->IconsColumn = true;
	TextEditor->Autocompletion = false;

  ListKeywords = NULL;
	bModifs = false;

	for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
		FColorParams[i] = TextEditor->ColorParams[i];
	}
  TextEditor->Font = Config->Font;

	ComboBox_LangageChange(this);
	ListBox_CategoriesChange(this);
}

//---------------------------------------------------------------------------
TForm_SaisieColors::~TForm_SaisieColors(void) {
	delete ListKeywords;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColorParams
//---------------------------------------------------------------------------

COLORPARAM TForm_SaisieColors::Get_ColorParams(int Index) {
	if ((TYPESYNTAX) Index < tsNormal || MAX_TYPESYNTAX < (TYPESYNTAX) Index) Index = 0;
  return FColorParams[Index];
}

bool TForm_SaisieColors::Set_ColorParams(int Index, COLORPARAM NewColorParams) {
	if ((TYPESYNTAX) Index < tsNormal || MAX_TYPESYNTAX < (TYPESYNTAX) Index) return false;
  FColorParams[Index] = NewColorParams;
	Frame_EditFichierText->TextEditor->ColorParams[Index] = NewColorParams;
	ListBox_CategoriesChange(this);

	return true;
}

//---------------------------------------------------------------------------
//
//                           CONTROLES ET BOUTONS
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::ListBox_CategoriesChange(TObject *Sender) {
	int Index = ListBox_Categories->ItemIndex;
	bNotChange = true;
	ColorBox_ColorBack->Selected = FColorParams[Index].Background;
	ColorBox_ColorText->Selected = FColorParams[Index].Foreground;
	CheckBox_Bold->Checked = FColorParams[Index].bBold;
	CheckBox_Italic->Checked = FColorParams[Index].bItalic;
	bNotChange = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::ColorBox_ColorBackChange(TObject *Sender) {
	if (!bNotChange) {
		int Index = ListBox_Categories->ItemIndex;
		FColorParams[Index].Background = ColorBox_ColorBack->Selected;
		Frame_EditFichierText->TextEditor->ColorParams[Index] = FColorParams[Index];
		ComboBox_Standard->ItemIndex = -1;
	  bModifs = true;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::ColorBox_ColorTextChange(TObject *Sender) {
	if (!bNotChange) {
		int Index = ListBox_Categories->ItemIndex;
		FColorParams[Index].Foreground = ColorBox_ColorText->Selected;
		Frame_EditFichierText->TextEditor->ColorParams[Index] = FColorParams[Index];
		ComboBox_Standard->ItemIndex = -1;
	  bModifs = true;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::CheckBox_BoldClick(TObject *Sender) {
	if (!bNotChange) {
		int Index = ListBox_Categories->ItemIndex;
		FColorParams[Index].bBold = CheckBox_Bold->Checked;
		Frame_EditFichierText->TextEditor->ColorParams[Index] = FColorParams[Index];
		ComboBox_Standard->ItemIndex = -1;
	  bModifs = true;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::CheckBox_ItalicClick(TObject *Sender) {
	if (!bNotChange) {
		int Index = ListBox_Categories->ItemIndex;
		FColorParams[Index].bItalic = CheckBox_Italic->Checked;
		Frame_EditFichierText->TextEditor->ColorParams[Index] = FColorParams[Index];
		ComboBox_Standard->ItemIndex = -1;
	  bModifs = true;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::ComboBox_StandardChange(TObject *Sender) {
	int i;
	static COLORPARAM StandardColorParams[5] [MAX_TYPESYNTAX + 1] = {  // tsNormal = 0 à tsIconsColumn = 8
		{
			// FVCLIDE Défaut
			{clWhite, clBlack, false, false},
			{RGB(10, 36, 106), clWhite, true, false},
			{clWhite, clBlue, true, false},
			{clWhite, clMaroon, true, false},
			{clWhite, clPurple, true, false},
			{clWhite, clNavy, true, false},
			{clWhite, clGreen, true, false},
			{clAqua, clGray, false, false},
			{clLtGray, clBlack, false, false},
			{clMoneyGreen, clBlack, false, false}
		},
		{
			// FVCLIDE Fond noir
			{clBlack, clLtGray, true, false},
			{clWhite, RGB(10, 36, 106), true, false},
			{clBlack, clSkyBlue, true, false},
			{clBlack, clRed, true, false},
			{clBlack, clFuchsia, true, false},
			{clBlack, clTeal, true, false},
			{clBlack, clLime, true, false},
			{clTeal, clLtGray, true, false},
			{clDkGray, clLtGray, true, false},
			{clGreen, clLtGray, true, false}
		},
		{
			// Builder
			{clWhite, clBlack, false, false},
			{RGB(10, 36, 106), clWhite, false, false},
			{clWhite, clBlack, true, false},
			{clWhite, clGreen, false, false},
			{clWhite, clNavy, false, false},
			{clWhite, clNavy, false, false},
			{clWhite, clNavy, false, true},
			{clAqua, clGray, false, false},
			{clLtGray, clBlack, false, false},
			{clMoneyGreen, clBlack, false, false}
		},
		{
			// Visual
			{clWhite, clBlack, false, false},
			{RGB(10, 36, 106), clWhite, false, false},
			{clWhite, clBlue, false, false},
			{clWhite, clBlue, false, false},
			{clWhite, clBlack, false, false},
			{clWhite, RGB(163, 21, 21), false, false},
			{clWhite, clGreen, false, false},
			{clAqua, clGray, false, false},
			{clLtGray, clBlack, false, false},
			{clMoneyGreen, clBlack, false, false}
		},
		{
			// Code::Blocks
			{clWhite, clBlack, false, false},
			{RGB(10, 36, 106), clWhite, true, false},
			{clWhite, clBlue, true, false},
			{clWhite, clBlue, true, false},
			{clWhite, RGB(240, 0, 240), true, false},
			{clWhite, clNavy, true, false},
			{clWhite, clGreen, false, false},
			{clAqua, clGray, false, false},
			{clLtGray, clBlack, false, false},
			{clMoneyGreen, clBlack, false, false}
		}
	};

	int Index = ComboBox_Standard->ItemIndex;
	if (Index >= 0) {

		for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
			FColorParams[i] = StandardColorParams[Index] [i];
			Frame_EditFichierText->TextEditor->ColorParams[i] = FColorParams[i];
		}

		ListBox_CategoriesChange(this);
		bModifs = true;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::ComboBox_LangageChange(TObject *Sender) {

	AnsiString asSamplePath;
  AnsiString asSampleFile;
	AnsiString asExt;
	asSamplePath = (AnsiString) Config->InstallPath + _T("\\Samples");
	asExt = ComboBox_Langage->Text;
	asSampleFile.sprintf(_T("%s\\Sample.%s"), (LPCTSTR) asSamplePath, (LPCTSTR) asExt);
	Frame_EditFichierText->FileName = asSampleFile;
	Frame_EditFichierText->Open();

	TTextEditor *TextEditor = Frame_EditFichierText->TextEditor;

	TextEditor->SetFlag(3, lfCurStatement, true);
  TextEditor->SetFlag(5, lfBookmark, true);
  TextEditor->SetFlag(6, lfBreakpoint, true);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIECOLORS);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieColors::FormClose(TObject *Sender,
      TCloseAction &Action) {
  int Rep;


  if (FModalResult != mrOk && FModalResult != mrCancel) {

    if (bModifs) {
			Rep = DisplayError(_TT("Voulez-vous enregistrer les modifications ?"),
		                     _TT("Coloration syntaxique"),
												 _T("SAVE_CHANGES"),
												 MB_YESNOCANCEL | MB_ICONQUESTION);
      if (Rep == IDYES) {
				FModalResult = mrOk;
      }
      else if (Rep == IDNO) {
				FModalResult = mrCancel;
      }
      else if (Rep == IDCANCEL) {
				Action = caNone;
      }
    }

  }

}
