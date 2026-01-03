//-----------------------------------------------------------------------------
//! @file TForm_Statistics.cpp
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
#include "TForm_DisplayError.h"
#include "TForm_Statistics.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_Statistics::TForm_Statistics(TComponent *Owner, LPCTSTR szName)
    : TForm(Owner, szName, FVCL_ARG_SUPP) {
  NbFichiersTraites = 0;
  NbCharacters = 0;
  NbLines = 0;
  NbLinesCode = 0;
  NbLinesComment = 0;
  NbLinesCommentEmpty = 0;
  NbLinesCodeComment = 0;
  NbLinesEmpty = 0;

	bBusy = false;
}

//---------------------------------------------------------------------------
TForm_Statistics::~TForm_Statistics(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::RadioButton_FichiersOuvertsClick(
      TObject *Sender) {
  CheckBox_SousRep->Enabled = false;
  Label_Repertoire->Enabled = false;
  Edit_Repertoire->Enabled = false;
  Button_Repertoire->Enabled = false;
	Label_Extensions->Enabled = false;
	ComboBox_Extensions->Enabled = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::RadioButton_FichiersProjetClick(
      TObject *Sender) {
  CheckBox_SousRep->Enabled = false;
  Label_Repertoire->Enabled = false;
  Edit_Repertoire->Enabled = false;
  Button_Repertoire->Enabled = false;
	Label_Extensions->Enabled = false;
	ComboBox_Extensions->Enabled = false;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::RadioButton_RepertoireClick(
      TObject *Sender) {
  CheckBox_SousRep->Enabled = true;
  Label_Repertoire->Enabled = true;
  Edit_Repertoire->Enabled = true;
  Button_Repertoire->Enabled = true;
	Label_Extensions->Enabled = true;
	ComboBox_Extensions->Enabled = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::Button_RepertoireClick(TObject *Sender) {
  AnsiString asRep;

  if (SelectDirectory(_TT("Répertoire de recherche"), _T(""), asRep)) {
    Edit_Repertoire->Text = asRep;
    Edit_Repertoire->SetFocus();
  }

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::Button_AbandonClick(TObject *Sender) {
	bAbort = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_STATISTICS);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::FormClose(TObject *Sender,
			TCloseAction &Action) {

	if (bBusy) {
		int Rep;

		Rep = DisplayError(_TT("Voulez-vous abandonner le traitement en cours ?"),
											 _TT("Sortie"),
											 _T("ABORTSEARCH"),
											 MB_YESNO | MB_ICONQUESTION);

		if (Rep == IDYES) bAbort = true;
		Action = caNone;
		return;

	}

	Action = caFree;

}

//---------------------------------------------------------------------------
void FASTCALL TForm_Statistics::Button_StatisticsClick(TObject *Sender) {
  AnsiString asLigne;
  AnsiString asPath;


  Memo_Statistics->Clear();

	ProgressBar->Position = 0;
	Label_Progress->Caption = "";

  RadioButton_FichiersOuverts->Enabled = false;
  RadioButton_FichiersProjet->Enabled = false;
  RadioButton_Repertoire->Enabled = false;
  CheckBox_SousRep->Enabled = false;
  Label_Repertoire->Enabled = false;
  Edit_Repertoire->Enabled = false;
  Button_Repertoire->Enabled = false;
	Label_Extensions->Enabled = false;
	ComboBox_Extensions->Enabled = false;
  Memo_Statistics->Enabled = false;
  CheckBox_Cumul->Enabled = false;
  Button_Statistics->Enabled = false;
  Button_Sortie->Enabled = false;
  Button_Aide->Enabled = false;

  Button_Statistics->Visible = false;
  Button_Sortie->Visible = false;
  Button_Aide->Visible = false;
	ProgressBar->Visible = true;
  Button_Abandon->Visible = true;
	Label_Progress->Visible = true;

	Application->ProcessMessages();
	bAbort = false;
	bBusy = true;

	if (CheckBox_Cumul->Checked == false) {
    NbFichiersTraites = 0;
    NbCharacters = 0;
    NbLines = 0;
    NbLinesCode = 0;
    NbLinesComment = 0;
    NbLinesCommentEmpty = 0;
    NbLinesCodeComment = 0;
    NbLinesEmpty = 0;
  }

  if (RadioButton_FichiersOuverts->Checked) {
		ParseFichiersOuverts();
  }
  else if (RadioButton_FichiersProjet->Checked) {
  }
  else if (RadioButton_Repertoire->Checked) {
    bRecursif = CheckBox_SousRep->Checked;
    asPath = Edit_Repertoire->Text;
    ParseFichiersRep(asPath);
  }

  Memo_Statistics->Lines->Add(_T("---------------------------"));
  TDateTime *Now = new TDateTime();
  Now->CurrentDate();
  asLigne = _TT("Statistiques ") + Now->FormatString(_T("dd/mm/yyyy"));
  delete Now;
  Memo_Statistics->Lines->Add(asLigne);
  Memo_Statistics->Lines->Add(_T("---------------------------"));
  asLigne.sprintf(_TT("%i fichier(s) traité(s)"), NbFichiersTraites);
  Memo_Statistics->Lines->Add(asLigne);
  asLigne.sprintf(_TT("%i caractère(s)"), NbCharacters);
  Memo_Statistics->Lines->Add(asLigne);
  asLigne.sprintf(_TT("%i ligne(s)"), NbLines);
  Memo_Statistics->Lines->Add(asLigne);
  asLigne.sprintf(_TT("%i ligne(s) de code"), NbLinesCode);
  Memo_Statistics->Lines->Add(asLigne);
  asLigne.sprintf(_TT("%i ligne(s) de commentaire(s)"), NbLinesComment);
  Memo_Statistics->Lines->Add(asLigne);
  asLigne.sprintf(_TT("%i ligne(s) de code + commentaire(s)"), NbLinesCodeComment);
  Memo_Statistics->Lines->Add(asLigne);
  asLigne.sprintf(_TT("%i ligne(s) de commentaire(s) vide(s)"), NbLinesCommentEmpty);
  Memo_Statistics->Lines->Add(asLigne);
  asLigne.sprintf(_TT("%i ligne(s) vide(s)"), NbLinesEmpty);
  Memo_Statistics->Lines->Add(asLigne);
  Memo_Statistics->Lines->Add(_T("---------------------------"));

	bBusy = false;

	Button_Statistics->Visible = true;
  Button_Sortie->Visible = true;
  Button_Aide->Visible = true;
	ProgressBar->Visible = false;
  Button_Abandon->Visible = false;
	Label_Progress->Visible = false;

  RadioButton_FichiersOuverts->Enabled = true;
  RadioButton_FichiersProjet->Enabled = true;
  RadioButton_Repertoire->Enabled = true;
  if (RadioButton_Repertoire->Checked) {
		CheckBox_SousRep->Enabled = true;
		Label_Repertoire->Enabled = true;
		Edit_Repertoire->Enabled = true;
		Button_Repertoire->Enabled = true;
		Label_Extensions->Enabled = true;
		ComboBox_Extensions->Enabled = true;
	}
  Memo_Statistics->Enabled = true;
  CheckBox_Cumul->Enabled = true;
  Button_Statistics->Enabled = true;
  Button_Sortie->Enabled = true;
  Button_Aide->Enabled = true;

}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Statistics::ParseFichiersOuverts(void) {
	int i;
	TTextEditor *TextEditor;
	CharString asLines;
	TMemoryStream *MemoryStream;
	int NbFiles;
	AnsiString asCommentSingle, asCommentSingleBeg, asCommentBegin, asCommentEnd, asCommentBegin2, asCommentEnd2, asEscapeString;
	TCHAR QuoteBegin, QuoteEnd, QuoteBegin2, QuoteEnd2, QuoteBegin3, QuoteEnd3;


	NbFiles = FVCLIDEAutomation->GetNbFiles();
	ProgressBar->Max = NbFiles;
	for (i = 0; i < NbFiles && !bAbort; i++) {

		ProgressBar->Position = i;
		Label_Progress->Caption = FVCLIDEAutomation->GetFileName(i);
		Application->ProcessMessages();

		TextEditor = FVCLIDEAutomation->GetTextFile(i);
		if (TextEditor) {
			asCommentSingle = TextEditor->CommentSingle;
			asCommentSingleBeg = TextEditor->CommentSingleBeg;
			asCommentBegin = TextEditor->CommentBegin;
			asCommentEnd = TextEditor->CommentEnd;
			asCommentBegin2 = TextEditor->CommentBegin2;
			asCommentEnd2 = TextEditor->CommentEnd2;
			asEscapeString = TextEditor->EscapeString;
			QuoteBegin = TextEditor->QuoteBegin;
			QuoteEnd = TextEditor->QuoteEnd;
			QuoteBegin2 = TextEditor->QuoteBegin2;
			QuoteEnd2 = TextEditor->QuoteEnd2;
			QuoteBegin3 = TextEditor->QuoteBegin3;
			QuoteEnd3 = TextEditor->QuoteEnd3;
			asLines = TextEditor->Lines->GetTextStr();
			MemoryStream = new TMemoryStream(asLines, asLines.Length());
			if (TraitementFichier(MemoryStream, asCommentSingle, asCommentSingleBeg, asCommentBegin, asCommentEnd, asCommentBegin2, asCommentEnd2, asEscapeString,
														QuoteBegin, QuoteEnd, QuoteBegin2, QuoteEnd2, QuoteBegin3, QuoteEnd3)) {
				NbFichiersTraites++;
			}
			delete MemoryStream;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_Statistics::ParseFichiersRep(const TCHAR* szPath) {
	int i;


	TStringList *ListFiles = new TStringList();

	TFileList *FileList = new TFileList(this);
	FileList->Recursive = CheckBox_SousRep->Checked;
	FileList->Filter = ComboBox_Extensions->Text;
	FileList->GetFileList(Edit_Repertoire->Text, true, ListFiles);
	delete FileList;

	ProgressBar->Max = ListFiles->Count;
	for (i = 0; i < ListFiles->Count && !bAbort; i++) {

		ProgressBar->Position = i;
		Label_Progress->Caption = ListFiles->Strings[i];
		Application->ProcessMessages();

		if (TraitementFichier(ListFiles->Strings[i])) NbFichiersTraites++;

  }

	delete ListFiles;

  return true;
}

//-----------------------------------------------------------------------------
// Statistiques sur les programmes
//-----------------------------------------------------------------------------

bool FASTCALL TForm_Statistics::TraitementFichier(AnsiString asFileNameIn) {
	AnsiString asExt;
	TFileStream *FileStream;
	AnsiString asCommentSingle, asCommentSingleBeg, asCommentBegin, asCommentEnd, asCommentBegin2, asCommentEnd2, asEscapeString;
	TCHAR QuoteBegin, QuoteEnd, QuoteBegin2, QuoteEnd2, QuoteBegin3, QuoteEnd3;
	bool Ok;



	FileStream = new TFileStream(asFileNameIn, fmOpenRead);
	if (FileStream->LastError != 0) {
    // Message d'erreur ouverture
		DisplaySystemError(asFileNameIn,
											 FileStream->LastError,
											 _TT("Impossible d'ouvrir le fichier"),
											 _T("SAVE_CHANGES"),
											 MB_OK | MB_ICONSTOP);
    Ok = false;
  }
	else {

		asExt = ExtractFileExt(asFileNameIn).LowerCase();
		int Len = asExt.Length();
		if (Len > 0 && asExt[1] == _T('.')) {
			asExt = asExt.SubString(2, Len - 1);
		}

		AnsiString asParametersPath;
		AnsiString asParametersFile;
		asParametersPath = (AnsiString) Config->InstallPath + _T("\\Syntax");
		asParametersFile.sprintf(_T("%s\\Parameters_%s.txt"), (LPCTSTR) asParametersPath, (LPCTSTR) asExt);
		QuoteBegin = _T('\0');
		QuoteEnd = _T('\0');
		QuoteBegin2 = _T('\0');
		QuoteEnd2 = _T('\0');
		QuoteBegin3 = _T('\0');
		QuoteEnd3 = _T('\0');
		if (FileExists(asParametersFile)) {
			TStringList *ListParameters;
			ListParameters = new TStringList();
			ListParameters->LoadFromFile(asParametersFile);

			int i;
			int Pos;
			AnsiString asLine;
			AnsiString asParam;
			AnsiString asValue;
			for (i = 0; i < ListParameters->Count; i++) {
				asLine = ListParameters->Strings[i];
				Pos = asLine.Pos(_T("="));
				if (Pos != 0) {
					asParam = asLine.SubString(1, Pos - 1);
					asValue = asLine.SubString(Pos + 1, asLine.Length() - Pos);
					if (asParam == _T("CommentSingle")) asCommentSingle = asValue;
					else if (asParam == _T("CommentSingleBeg")) asCommentSingleBeg = asValue;
					else if (asParam == _T("CommentBegin")) asCommentBegin = asValue;
					else if (asParam == _T("CommentEnd")) asCommentEnd = asValue;
					else if (asParam == _T("CommentBegin2")) asCommentBegin2 = asValue;
					else if (asParam == _T("CommentEnd2")) asCommentEnd2 = asValue;
					else if (asParam == _T("EscapeString")) asEscapeString = asValue;
					else if (asParam == _T("QuoteBegin")) QuoteBegin = asValue[1];
					else if (asParam == _T("QuoteEnd")) QuoteEnd = asValue[1];
					else if (asParam == _T("QuoteBegin2")) QuoteBegin2 = asValue[1];
					else if (asParam == _T("QuoteEnd2")) QuoteEnd2 = asValue[1];
					else if (asParam == _T("QuoteBegin3")) QuoteBegin3 = asValue[1];
					else if (asParam == _T("QuoteEnd3")) QuoteEnd3 = asValue[1];
				}
			}

			delete ListParameters;
		}

		Ok = TraitementFichier(FileStream, asCommentSingle, asCommentSingleBeg, asCommentBegin, asCommentEnd, asCommentBegin2, asCommentEnd2, asEscapeString,
													 QuoteBegin, QuoteEnd, QuoteBegin2, QuoteEnd2, QuoteBegin3, QuoteEnd3);

	}

	delete FileStream;

  return Ok;
}

//-----------------------------------------------------------------------------
bool FASTCALL TForm_Statistics::TraitementFichier(TStream *Stream, AnsiString asCommentSingle, AnsiString asCommentSingleBeg, AnsiString asCommentBegin, AnsiString asCommentEnd, AnsiString asCommentBegin2, AnsiString asCommentEnd2, AnsiString asEscapeString,
																									TCHAR QuoteBegin, TCHAR QuoteEnd, TCHAR QuoteBegin2, TCHAR QuoteEnd2, TCHAR QuoteBegin3, TCHAR QuoteEnd3) {
	TCHAR Buf[51];  // On suppose que le mot-clé des commentaires fait moins de 50 caractères
	char c;
	int NbChar = 0;
	bool bComment = false;
	bool bComment1 = false;
	bool bComment2 = false;
	bool bQuote1 = false;
	bool bQuote2 = false;
	bool bQuote3 = false;
	int LenCommentSingle, LenCommentSingleBeg, LenCommentBegin, LenCommentEnd, LenCommentBegin2, LenCommentEnd2, LenEscapeString;


	LenCommentSingle = asCommentSingle.Length();
	LenCommentSingleBeg = asCommentSingleBeg.Length();
	LenCommentBegin = asCommentBegin.Length();
	LenCommentEnd = asCommentEnd.Length();
	LenCommentBegin2 = asCommentBegin2.Length();
	LenCommentEnd2 = asCommentEnd2.Length();
	LenEscapeString = asEscapeString.Length();

	memset(Buf, 0, sizeof(Buf));
	c = '\0';
	Stream->Read(&c, 1);
	Buf[49] = c;
	while (c != '\0') {

		if (c == '\r') {
      NbLines++;
      if (NbChar > 0 && bComment) NbLinesCodeComment++;
      else if (NbChar > 0) NbLinesCode++;
      else if (bComment) NbLinesComment++;
      else if (bComment1 || bComment2) NbLinesCommentEmpty++;
      else NbLinesEmpty++;
      NbChar = 0;
      bComment = false;
      bComment1 = false;
      bQuote1 = false;
      bQuote2 = false;
      bQuote3 = false;
		}

    if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			NbCharacters++;
      if (bComment1 || bComment2) bComment = true;
      else NbChar++;
		}

		if (!bComment1 && !bComment2 && !bQuote1 && !bQuote2 && !bQuote3 && c == QuoteBegin) {
			bQuote1 = true;
		}
		else if (bQuote1 && c == QuoteEnd) {
			bQuote1 = false;
		}
		else if (!bComment1 && !bComment2 && !bQuote1 && !bQuote2 && !bQuote3 && c == QuoteBegin2) {
			bQuote2 = true;
		}
		else if (bQuote2 && c == QuoteEnd2) {
			bQuote2 = false;
		}
		else if (!bComment1 && !bComment2 && !bQuote1 && !bQuote2 && !bQuote3 && c == QuoteBegin3) {
			bQuote3 = true;
		}
		else if (bQuote3 && c == QuoteEnd3) {
			bQuote3 = false;
		}
		else if (LenCommentSingle && !bComment1 && !bComment2 && !bQuote1 && !bQuote2 && !bQuote3 &&
						 !memcmp(&Buf[50 - LenCommentSingle], (LPCTSTR) asCommentSingle, LenCommentSingle * sizeof(TCHAR))) {
			NbChar -= LenCommentSingle;
			bComment1 = true;
		}
    else if (LenCommentBegin && !bComment1 && !bComment2 && !bQuote1 && !bQuote2 && !bQuote3 &&
						 !memcmp(&Buf[50 - LenCommentBegin], (LPCTSTR) asCommentBegin, LenCommentBegin * sizeof(TCHAR))) {
			NbChar -= LenCommentBegin;
			bComment2 = true;
		}
    else if (LenCommentEnd && bComment2 && !bQuote1 && !bQuote2 && !bQuote3 &&
						 !memcmp(&Buf[50 - LenCommentEnd], (LPCTSTR) asCommentEnd, LenCommentEnd * sizeof(TCHAR))) {
			NbChar -= LenCommentEnd;
			bComment2 = false;
		}
    else if (LenCommentBegin2 && !bComment1 && !bComment2 && !bQuote1 && !bQuote2 && !bQuote3 &&
						 !memcmp(&Buf[50 - LenCommentBegin2], (LPCTSTR) asCommentBegin2, LenCommentBegin2 * sizeof(TCHAR))) {
			NbChar -= LenCommentBegin2;
			bComment2 = true;
		}
    else if (LenCommentEnd2 && bComment2 && !bQuote1 && !bQuote2 && !bQuote3 &&
						 !memcmp(&Buf[50 - LenCommentEnd2], (LPCTSTR) asCommentEnd2, LenCommentEnd2 * sizeof(TCHAR))) {
			NbChar -= LenCommentEnd2;
			bComment2 = false;
		}

		memcpy(Buf, &Buf[1], 50 * sizeof(TCHAR));
		c = '\0';
		Stream->Read(&c, 1);
		Buf[49] = c;

	}

	c = (char) Buf[48];
	if (c != '\r' && c != '\n') {
    NbLines++;
    if (NbChar > 0 && bComment) NbLinesCodeComment++;
    else if (NbChar > 0) NbLinesCode++;
    else if (bComment) NbLinesComment++;
    else if (bComment1 || bComment2) NbLinesCommentEmpty++;
	}

	return true;
}

//---------------------------------------------------------------------------
