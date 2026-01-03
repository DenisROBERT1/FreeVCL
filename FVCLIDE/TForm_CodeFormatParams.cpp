//-----------------------------------------------------------------------------
//! @file TForm_CodeFormatParams.cpp
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

#include "TTextEditor.h"
#include "TConfig.h"
#include "TFVCLIDEAutomation.h"
#include "TFrame_EditFichierText.h"
#include "TForm_CodeFormatParams.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_CodeFormatParams::TForm_CodeFormatParams(TComponent *Owner, LPCTSTR szName)
    : TForm(Owner, szName, FVCL_ARG_SUPP) {
	CheckBox_EndLineAcc->Checked = Config->EndLineAcc;
  CheckBox_SpacesAfterPar->Checked = Config->SpacesAfterPar;

  ListKeywords = NULL;

	ComboBox_LangageChange(this);
	TTextEditor *TextEditor = Frame_EditFichierText->TextEditor;
	TextEditor->LinesNumbers = false;
	TextEditor->IconsColumn = false;
	TextEditor->Autocompletion = false;

	TextEditor->Lines->SetTextStr(
		_T("#include <stdio.h>\r\n")
		_T("\r\n")
		_T("/* TEST */\r\n")
		_T("int main(int argc, char *argv[]) {\r\n")
		_T("\twhile (--argc > 0) {\r\n")
		_T("\t\tprintf(\"%s \", argv[argc]);\r\n")
		_T("\t}\r\n")
		_T("}\r\n")
	);

	CodeFormat();
}

//---------------------------------------------------------------------------
TForm_CodeFormatParams::~TForm_CodeFormatParams(void) {
	delete ListKeywords;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CodeFormatParams::CheckBox_EndLineAccClick(TObject *Sender) {
	CodeFormat();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CodeFormatParams::CheckBox_SpacesAfterParClick(TObject *Sender) {
	CodeFormat();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CodeFormatParams::ComboBox_LangageChange(TObject *Sender) {

	AnsiString asSamplePath;
  AnsiString asSampleFile;
	AnsiString asExt;
	asSamplePath = (AnsiString) Config->InstallPath + _T("\\Samples");
	asExt = ComboBox_Langage->Text;
	asSampleFile.sprintf(_T("%s\\Sample.%s"), (LPCTSTR) asSamplePath, (LPCTSTR) asExt);
	Frame_EditFichierText->FileName = asSampleFile;
	Frame_EditFichierText->Open();

	if (asExt == _T("cpp")) {
		CheckBox_EndLineAcc->Enabled = true;
	}
	else {
		CheckBox_EndLineAcc->Enabled = false;
	}

	CodeFormat();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CodeFormatParams::Button_OkClick(TObject *Sender) {
  Config->EndLineAcc = CheckBox_EndLineAcc->Checked;
  Config->SpacesAfterPar = CheckBox_SpacesAfterPar->Checked;

	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CodeFormatParams::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_CODEFORMATPARAMS);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_CodeFormatParams::CodeFormat(void) {
	bool bEndLineAcc, bSpacesAfterPar;

	bEndLineAcc = CheckBox_EndLineAcc->Checked;
	bSpacesAfterPar = CheckBox_SpacesAfterPar->Checked;
	Frame_EditFichierText->CodeFormat(bEndLineAcc, bSpacesAfterPar);
}

//---------------------------------------------------------------------------
