//-----------------------------------------------------------------------------
//! @file TFrame_EditFichierText.cpp
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
#include <FreeVcl_ToolsEDI.h>


//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------
#include "Utilit.h"
#include "TConfig.h"
#include "TFrame_EditFichierText.h"
#include "TFVCLIDEAutomation.h"


//---------------------------------------------------------------------------
// Variables externes
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

int TFrame_EditFichierText::NbFrame_EditFichierText = 0;
std::vector<LIST_KEYWORDS> TFrame_EditFichierText::ListsKeywords;


//---------------------------------------------------------------------------
TFrame_EditFichierText::TFrame_EditFichierText(TWinControl *AOwner)
  : TFrame_FVCLIDEBase(AOwner) {

  // Initialisations
	FFileType = ftf_TextFile;
	FCodeFormatType = cfNone;
	NbFrame_EditFichierText++;

}

//---------------------------------------------------------------------------
TFrame_EditFichierText::~TFrame_EditFichierText(void) {

	if (--NbFrame_EditFichierText == 0) {
		int i;

		for (i = 0; i < (int) ListsKeywords.size(); i++) {
			delete ListsKeywords[i].Keywords;
		}
		ListsKeywords.clear();
	}

}

//---------------------------------------------------------------------------
void TFrame_EditFichierText::AfterConstruction(void) {
	int i;

  TextEditor->NbSpacesTab = (int) Config->NbSpacesTab;
  TextEditor->SpacesToTabs = (int) Config->SpacesToTabs;
  TextEditor->TabsToSpaces = (int) Config->TabsToSpaces;
  TextEditor->LinesNumbers = (int) Config->LinesNumbers;
  TextEditor->IconsColumn = (int) Config->IconsColumn;
  TextEditor->TypeAuto = (int) Config->TypeAuto;
  TextEditor->IndentAuto = (int) Config->IndentAuto;
  TextEditor->CurAfterEndLine = (int) Config->CurAfterEndLine;
  TextEditor->UndoRedoMoveCur = (int) Config->UndoRedoMoveCur;
  TextEditor->Autocompletion = (int) Config->Autocompletion;
	for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
		TextEditor->ColorParams[i] = Config->ColorParams[i];
	}
  TextEditor->Font = Config->Font;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CodeFormatType
//---------------------------------------------------------------------------

TCodeFormatType TFrame_EditFichierText::Get_CodeFormatType(void) {
	return FCodeFormatType;
}

bool TFrame_EditFichierText::Set_CodeFormatType(TCodeFormatType NewCodeFormatType) {
	if (FCodeFormatType != NewCodeFormatType) {
		FCodeFormatType = NewCodeFormatType;
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FileName
//---------------------------------------------------------------------------

bool TFrame_EditFichierText::Set_FileName(AnsiString NewFileName) {
  if (FFileName != NewFileName) {
		AnsiString asExt;

    FFileName = NewFileName;

		asExt = ExtractFileExt(FFileName).LowerCase();
		int Len = asExt.Length();
		if (Len > 0 && asExt[1] == _T('.')) {
			asExt = asExt.SubString(2, Len - 1);
		}

		SetLanguage(asExt);

  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Changed
//---------------------------------------------------------------------------

bool TFrame_EditFichierText::Get_Changed(void) {
  return TextEditor->Changed;
}

bool TFrame_EditFichierText::Set_Changed(bool NewChanged) {
  TextEditor->Changed = NewChanged;

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierText::OpenFile(AnsiString asFileName) {
	if (TextEditor->LoadFromFile(asFileName)) {
		FileName = asFileName;  // Et pas FFileName = ... (il y a un traitement dans le Set_FileName)
		TextEditor->MoveCur(TextEditor->NumLine, TextEditor->NumCol, false, false);
		return true;
	}
	else {
		AnsiString asError;
		asError.sprintf(_TT("Impossible d'ouvrir le fichier \"%s\""), (LPCTSTR) asFileName);
		FVCLIDEAutomation->DisplaySystemError(asError, TextEditor->LastError, _TT("Ouverture"), _T(""), MB_OK | MB_ICONSTOP);
		return false;
	}
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierText::SaveToFile(AnsiString asFileName) {
	if (TextEditor->SaveToFile(asFileName)) {
		FileName = asFileName;  // Et pas FFileName = ... (il y a un traitement dans le Set_FileName)

		// Nom long
		AnsiString asLongFileName;
		TCHAR * szLongFileName = new TCHAR[32767];
		if (GetLongPathName(asFileName, szLongFileName, 32767)) {
			asLongFileName = szLongFileName;
		}
		delete[] szLongFileName;
		if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

		// Enregistrement dans la liste des fichiers récents
		Config->PushLastFile(asLongFileName);

		return true;
	}
	else {
		AnsiString asError;
		asError.sprintf(_TT("Impossible d'enregistrer le fichier \"%s\""), (LPCTSTR) asFileName);
		FVCLIDEAutomation->DisplaySystemError(asError, TextEditor->LastError, _TT("Enregistrement"), _T(""), MB_OK | MB_ICONSTOP);
		return false;
	}
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierText::Undo(void) {
	TextEditor->Undo();

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierText::Redo(void) {
	TextEditor->Redo();

	return true;
}

//---------------------------------------------------------------------------
AnsiString TFrame_EditFichierText::GetModificationsList(void) {
	return TextEditor->GetModificationsList();
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierText::CutToClipboard(void) {
	TextEditor->CutToClipboard();

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierText::CopyToClipboard(void) {
	TextEditor->CopyToClipboard();

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierText::PasteFromClipboard(void) {
	TextEditor->PasteFromClipboard();

  return true;
}

//---------------------------------------------------------------------------
// Formatage du code
//---------------------------------------------------------------------------

bool FASTCALL TFrame_EditFichierText::CodeFormat(bool bEndLineAcc, bool bSpacesAfterPar) {
	TStringList *slNewLines;


	if (FCodeFormatType == cfNone) {
		AnsiString asExt = ExtractFileExt(FFileName);
		FVCLIDEAutomation->DisplayError(
				_TT("Aucun algorithme de formatage pour les fichiers de type ") + asExt,
				_TT("Formatage de code"),
				_T(""),
				MB_OK | MB_ICONWARNING);
		return false;
	}

	TextEditor->BeginUpdate();

	slNewLines = new TStringList();
	slNewLines->WantLastLine = true;
	switch (FCodeFormatType) {
		case cfCPP:
			CodeFormatCPP(TextEditor->Lines, slNewLines, bEndLineAcc, bSpacesAfterPar);
			break;
		case cfPAS:
			CodeFormatPAS(TextEditor->Lines, slNewLines, bSpacesAfterPar);
			break;
		case cfDFM:
			CodeFormatDFM(TextEditor->Lines, slNewLines, bSpacesAfterPar);
			break;
		case cfHTML:
			CodeFormatHTML(TextEditor->Lines, slNewLines);
			break;
	}
	TextEditor->Lines_SetLines(slNewLines);
	delete slNewLines;

	TextEditor->EndUpdate();
	TextEditor->Invalidate();

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierText::CodeFormatCPP(TStrings *sIn, TStrings *sOut, bool bEndLineAcc, bool bSpacesAfterPar) {
	TCHAR ddc, dc, c, pc;
	bool bCommentSingleLine = false;
	bool bComment = false;
	bool bSimpleQuote = false;
	bool bDoubleQuote = false;
	bool bEscape = false;
	bool bPrecomp = false;
	bool bOperator = false;
	bool bAddSpaceBefore = false;
	bool bAddSpaceAfter = false;
	AnsiString asIndent;
	AnsiString asOldLine;
	AnsiString asNewLine;
	AnsiString asKeyword;
	AnsiString asError;
	int x, y;
	int NumCol, NumLine, NbSpacesTab;
	int Indent, IndentLine, IndentPar;
	int NbErrors = 0;


	ddc = _T('\0');
	dc = _T('\0');
	c = _T('\0');
	pc = _T('\0');
	x = 0;
	y = 0;
	NumCol = 1;
	NumLine = 1;
	Indent = 0;
	IndentLine = 0;
	IndentPar = 0;
	NbSpacesTab = Config->NbSpacesTab;
	asKeyword = _T("");

	FVCLIDEAutomation->ClearLogMessage(tl_Process);
	FVCLIDEAutomation->AddLogMessage(tl_Process, _TT("Formatage de code"));

	if (y < sIn->Count) {
		asOldLine = sIn->Strings[y];
		if (x < asOldLine.Length() - 1) {
			c = asOldLine[x + 1];
			pc = asOldLine[x + 2];
		}
		else if (x < asOldLine.Length()) {
			c = asOldLine[x + 1];
			pc = _T('\n');
		}
		else if (x == asOldLine.Length()) {
			c = _T('\n');
			pc = _T('\0');
		}
	}
	else {
		c = _T('\0');
		pc = _T('\0');
	}

	while (c != _T('\0')) {

		// Traitement du caractère
		if (bCommentSingleLine) {
		}

		else if (bComment) {
			if (dc == _T('*') && c == _T('/')) bComment = false;
		}

		else if (bSimpleQuote) {
			if (!bEscape) {
				if (c == _T('\\')) {
					bEscape = true;
				}
				else {
					if (c == _T('\'')) bSimpleQuote = false;
					else if (dc != '\\' && c == _T('\n')) {
						asError.sprintf(_TT("  %s (%i, %i): Retour à la ligne dans chaîne constante"),
														(LPCTSTR) FFileName, NumLine, NumCol);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
				}
			}
			else {
				bEscape = false;
			}
		}

		else if (bDoubleQuote) {
			if (!bEscape) {
				if (c == _T('\\')) {
					bEscape = true;
				}
				else {
					if (c == _T('"')) bDoubleQuote = false;
					else if (dc != '\\' && c == _T('\n')) {
						asError.sprintf(_TT("  %s (%i, %i): Retour à la ligne dans chaîne constante"),
														(LPCTSTR) FFileName, NumLine, NumCol);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
				}
			}
			else {
				bEscape = false;
			}
		}

		else if (bPrecomp) {
		}

		else {

			// Espaces avant et après les opérateurs
			if (c == _T('=') ||
					c == _T('<') ||
					c == _T('/') ||
					c == _T('|')) {
				if (!bOperator) {
					bAddSpaceBefore = true;
					bOperator = true;
				}
			}
			else if (c == _T('!')) {
				// Traitement spécial pour le '!', pour différentier "j = !i" de "j != i"
				if (pc == _T('=')) {
					bAddSpaceBefore = true;
					bOperator = true;
				}
				else {
					if (bOperator) bAddSpaceBefore = true;
					bOperator = false;
				}
			}
			else if (c == _T('+')) {
				// Traitement spécial pour le '+', pour différentier "2 += 1" de "i++ == 1"
				if (dc == _T('+')) {
					bOperator = false;
				}
				else {
					if (pc != _T('+') || bOperator) bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
			}
			else if (c == _T('-')) {
				// Traitement spécial pour le '-', pour différentier "2 - 1", "2 == -1", "i--" et "i->m"
				if (dc == _T('-')) {
					bOperator = false;
				}
				else if (pc == _T('>')) {
					bAddSpaceBefore = false;
					bOperator = true;
				}
				else {
					if (pc != _T('-') || bOperator) bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
			}
			else if (c == _T('>')) {
				// Traitement spécial pour le '>', pour différentier "i > m" du "i->m"
				if (bOperator && dc == _T('-')) {
					bAddSpaceAfter = false;
					bOperator = false;
				}
				else {
					if (!bOperator) {
						bAddSpaceBefore = true;
						bOperator = true;
					}
				}
			}
			else if (c == _T('&')) {
				// Traitement spécial pour le '&', pour différentier "p = &i", "p &= i" et "p && i"
				if (dc != _T('&')) {
					bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
			}
			else if (c == _T('*')) {
				// Traitement spécial pour le '*', pour différentier "i = *p", "i *= p" et "/*"
				if (dc != _T('/')) {
					bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
				else {
					bAddSpaceBefore = false;
					bAddSpaceAfter = false;
					bOperator = false;
				}
			}
			else if (c != _T(' ')) {
				if (bOperator) {
					bAddSpaceBefore = true;
					bOperator = false;
				}
			}

			switch (c) {
				case _T('\''):
					if (bAddSpaceBefore) {
						asNewLine += _T(' ');
						NumCol++;
						bAddSpaceBefore = false;
					}
					bAddSpaceAfter = false;
					bSimpleQuote = true;
					bEscape = false;
					break;
				case _T('"'):
					if (bAddSpaceBefore) {
						asNewLine += _T(' ');
						NumCol++;
						bAddSpaceBefore = false;
					}
					bAddSpaceAfter = false;
					bDoubleQuote = true;
					bEscape = false;
					break;
				case _T('#'):
					if (asNewLine.Trim() == _T("")) bPrecomp = true;
					break;
				case _T('('):
					if (asKeyword == _T("if") || asKeyword == _T("while") || asKeyword == _T("for")) {
						asKeyword = _T("");
						bAddSpaceBefore = true;
					}
					if (bSpacesAfterPar) {
						bAddSpaceAfter = true;
					}
					else {
						bAddSpaceAfter = false;
					}
					IndentPar += 2;
					break;
				case _T(')'):
					if (bSpacesAfterPar) {
						bAddSpaceBefore = true;
					}
					else {
						bAddSpaceBefore = false;
					}
					IndentPar -= 2;
					break;
				case _T('{'):
					IndentLine++;
					break;
				case _T('}'):
					IndentLine--;
					if (Indent + IndentLine < 0) {
						asError.sprintf(_TT("  %s (%i, %i): \"}\" inattendu"),
														(LPCTSTR) FFileName, NumLine, NumCol);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
					break;
				case _T(';'):
				case _T(','):
					bAddSpaceBefore = false;
					bAddSpaceAfter = true;
					break;
				case _T('\t'):
				case _T(' '):
				case _T(':'):
					if ((c == _T('\t') || c == _T(' ')) && dc != _T('(')) bAddSpaceBefore = true;
					if (asKeyword == _T("case") || asKeyword == _T("default")) {
						asKeyword = _T("");
						Indent--;
						IndentLine++;
					}
					break;
				default:
					if (dc == _T('/') && c == _T('/')) bCommentSingleLine = true;
					if (dc == _T('/') && c == _T('*')) bComment = true;

					if (_T('a') <= c && c <= _T('z')) {
						if (dc < _T('a') || _T('z') < dc) asKeyword = _T("");
						asKeyword += c;
					}

					break;
			}

		}

		// Enregistrement du caractère

		if (bPrecomp || bSimpleQuote || bDoubleQuote || bCommentSingleLine || bComment) {
			asNewLine += c;
			NumCol++;
		}
		else if (c != _T('\t') && c != _T(' ') && c != _T('\n')) {
			if (bAddSpaceBefore) {
				asNewLine += _T(' ');
				NumCol++;
			}
			asNewLine += c;
			if (c != _T('\t')) NumCol++;
			else NumCol = ((NumCol - 1) / NbSpacesTab + 1) * NbSpacesTab + 1;
			bAddSpaceBefore = bAddSpaceAfter;
			bAddSpaceAfter = false;
		}

		// Traitement fin de ligne

		if (c != _T('\n')) {
			x++;
		}
		else {
			if (sOut) {
				if (IndentLine < 0) Indent += IndentLine;
				if (Indent < 0) Indent = 0;
				asNewLine = asNewLine.Trim();
				if (asNewLine.IsEmpty()) asIndent = _T("");
				else {
					if (!bPrecomp) {
						asIndent = AnsiString(_T('\t'), Indent);
						if (!bCommentSingleLine && !bComment && !bSimpleQuote && !bDoubleQuote) {
							int LengthLine = asNewLine.Length();
							if (!bEndLineAcc && LengthLine >= 2 && asNewLine[LengthLine] == _T('{')) {
								// Accolade à la ligne
								asNewLine.Delete(LengthLine, 1);
								asNewLine = asNewLine.Trim();
								if (!asNewLine.IsEmpty())	{
									sOut->Add(asIndent + asNewLine);
									NumLine++;
								}
								asNewLine = _T("{");
							}
							else if (bEndLineAcc && sOut->Count > 0 && asNewLine == _T("{")) {
								AnsiString asPrevLine;
								asPrevLine = sOut->Strings[sOut->Count - 1];
								// Il ne faut pas insérer d'accolade s'il y a des commentaires de fin de ligne
								if (asPrevLine.Pos(_T("//")) == 0) {
									asNewLine = asPrevLine.Trim() + _T(" {");
									sOut->Delete(sOut->Count - 1);
									NumLine--;
								}
							}
						}
					}
					else {
						asIndent = _T("");
					}
				}
				sOut->Add(asIndent + asNewLine);
				if (IndentLine > 0) Indent += IndentLine;
				Indent += IndentPar;
				IndentLine = 0;
				IndentPar = 0;
			}
			bCommentSingleLine = false;
			if (!bEscape) {
				bSimpleQuote = false;
				bDoubleQuote = false;
			}
			if (dc != '\\') {
				bPrecomp = false;
			}
			y++;
			NumLine++;
			x = 0;
			NumCol = Indent * NbSpacesTab + 1;
			if (y < sIn->Count) {
				asOldLine = sIn->Strings[y];
				asNewLine = _T("");
				bAddSpaceBefore = false;
				bAddSpaceAfter = false;
				bOperator = false;
			}
		}

		// Caractère suivant

		ddc = dc;
		dc = c;

		if (y < sIn->Count) {
			if (x < asOldLine.Length() - 1) {
				c = asOldLine[x + 1];
				pc = asOldLine[x + 2];
			}
			else if (x < asOldLine.Length()) {
				c = asOldLine[x + 1];
				pc = _T('\n');
			}
			else if (x == asOldLine.Length()) {
				c = _T('\n');
				pc = _T('\0');
			}
		}
		else {
			c = _T('\0');
			pc = _T('\0');
		}

	}

	if (Indent > 0) {
		asError.sprintf(_TT("  %s (%i, %i): \"}\" manquant"),
										(LPCTSTR) FFileName, NumLine, NumCol);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bComment) {
		asError.sprintf(_TT("  %s (%i, %i): Commentaire non terminé"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bSimpleQuote || bDoubleQuote) {
		asError.sprintf(_TT("  %s (%i, %i): Chaîne non terminée"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	asError.sprintf(_TT("%i Erreur(s)"), NbErrors);
	FVCLIDEAutomation->AddLogMessage(tl_Process, asError);

	return (NbErrors == 0);
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierText::CodeFormatPAS(TStrings *sIn, TStrings *sOut, bool bSpacesAfterPar) {
	TCHAR ddc, dc, c, pc;
	bool bCommentSingleLine = false;
	bool bComment = false;
	bool bSimpleQuote = false;
	bool bDoubleQuote = false;
	bool bEscape = false;
	bool bPrecomp = false;
	bool bOperator = false;
	bool bAddSpaceBefore = false;
	bool bAddSpaceAfter = false;
	AnsiString asIndent;
	AnsiString asOldLine;
	AnsiString asNewLine;
	AnsiString asKeyword;
	AnsiString asError;
	int x, y;
	int NumCol, NumLine, NbSpacesTab;
	int Indent, IndentLine, IndentPar;
	int NbErrors = 0;


	ddc = _T('\0');
	dc = _T('\0');
	c = _T('\0');
	pc = _T('\0');
	x = 0;
	y = 0;
	NumCol = 1;
	NumLine = 1;
	Indent = 0;
	IndentLine = 0;
	IndentPar = 0;
	NbSpacesTab = Config->NbSpacesTab;
	asKeyword = _T("");

	FVCLIDEAutomation->ClearLogMessage(tl_Process);
	FVCLIDEAutomation->AddLogMessage(tl_Process, _TT("Formatage de code"));

	if (y < sIn->Count) {
		asOldLine = sIn->Strings[y];
		if (x < asOldLine.Length() - 1) {
			c = asOldLine[x + 1];
			pc = asOldLine[x + 2];
		}
		else if (x < asOldLine.Length()) {
			c = asOldLine[x + 1];
			pc = _T('\n');
		}
		else if (x == asOldLine.Length()) {
			c = _T('\n');
			pc = _T('\0');
		}
	}
	else {
		c = _T('\0');
		pc = _T('\0');
	}

	while (c != _T('\0')) {

		// Traitement du caractère
		if (bCommentSingleLine) {
		}

		else if (bComment) {
			if (dc == _T('*') && c == _T('/')) bComment = false;
		}

		else if (bSimpleQuote) {
			if (!bEscape) {
				if (c == _T('\\')) {
					bEscape = true;
				}
				else {
					if (c == _T('\'')) bSimpleQuote = false;
					else if (dc != '\\' && c == _T('\n')) {
						asError.sprintf(_TT("  %s (%i, %i): Retour à la ligne dans chaîne constante"),
														(LPCTSTR) FFileName, NumLine, NumCol);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
				}
			}
			else {
				bEscape = false;
			}
		}

		else if (bDoubleQuote) {
			if (!bEscape) {
				if (c == _T('\\')) {
					bEscape = true;
				}
				else {
					if (c == _T('"')) bDoubleQuote = false;
					else if (dc != '\\' && c == _T('\n')) {
						asError.sprintf(_TT("  %s (%i, %i): Retour à la ligne dans chaîne constante"),
														(LPCTSTR) FFileName, NumLine, NumCol);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
				}
			}
			else {
				bEscape = false;
			}
		}

		else if (bPrecomp) {
		}

		else {

			// Espaces avant et après les opérateurs
			if (c == _T('=') ||
					c == _T('<') ||
					c == _T('/') ||
					c == _T('|')) {
				if (!bOperator) {
					bAddSpaceBefore = true;
					bOperator = true;
				}
			}
			else if (c == _T('!')) {
				// Traitement spécial pour le '!', pour différentier "j = !i" de "j != i"
				if (pc == _T('=')) {
					bAddSpaceBefore = true;
					bOperator = true;
				}
				else {
					if (bOperator) bAddSpaceBefore = true;
					bOperator = false;
				}
			}
			else if (c == _T('+')) {
				// Traitement spécial pour le '+', pour différentier "2 += 1" de "i++ == 1"
				if (dc == _T('+')) {
					bOperator = false;
				}
				else {
					if (pc != _T('+') || bOperator) bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
			}
			else if (c == _T('-')) {
				// Traitement spécial pour le '-', pour différentier "2 - 1", "2 == -1", "i--" et "i->m"
				if (dc == _T('-')) {
					bOperator = false;
				}
				else if (pc == _T('>')) {
					bAddSpaceBefore = false;
					bOperator = true;
				}
				else {
					if (pc != _T('-') || bOperator) bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
			}
			else if (c == _T('>')) {
				// Traitement spécial pour le '>', pour différentier "i > m" du "i->m"
				if (bOperator && dc == _T('-')) {
					bAddSpaceAfter = false;
					bOperator = false;
				}
				else {
					if (!bOperator) {
						bAddSpaceBefore = true;
						bOperator = true;
					}
				}
			}
			else if (c == _T('&')) {
				// Traitement spécial pour le '&', pour différentier "p = &i", "p &= i" et "p && i"
				if (dc != _T('&')) {
					bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
			}
			else if (c == _T('*')) {
				// Traitement spécial pour le '*', pour différentier "i = *p", "i *= p" et "/*"
				if (dc != _T('/')) {
					bAddSpaceBefore = true;
					bOperator = !bOperator;
				}
				else {
					bAddSpaceBefore = false;
					bAddSpaceAfter = false;
					bOperator = false;
				}
			}
			else if (c != _T(' ')) {
				if (bOperator) {
					bAddSpaceBefore = true;
					bOperator = false;
				}
			}

			switch (c) {
				case _T('\''):
					if (bAddSpaceBefore) {
						asNewLine += _T(' ');
						NumCol++;
						bAddSpaceBefore = false;
					}
					bAddSpaceAfter = false;
					bSimpleQuote = true;
					bEscape = false;
					break;
				case _T('"'):
					if (bAddSpaceBefore) {
						asNewLine += _T(' ');
						NumCol++;
						bAddSpaceBefore = false;
					}
					bAddSpaceAfter = false;
					bDoubleQuote = true;
					bEscape = false;
					break;
				case _T('#'):
					if (asNewLine.Trim() == _T("")) bPrecomp = true;
					break;
				case _T('('):
					if (asKeyword == _T("if") || asKeyword == _T("while") || asKeyword == _T("for")) {
						asKeyword = _T("");
						bAddSpaceBefore = true;
					}
					if (bSpacesAfterPar) {
						bAddSpaceAfter = true;
					}
					else {
						bAddSpaceAfter = false;
					}
					IndentPar += 2;
					break;
				case _T(')'):
					if (bSpacesAfterPar) {
						bAddSpaceBefore = true;
					}
					else {
						bAddSpaceBefore = false;
					}
					IndentPar -= 2;
					break;
				case _T(','):
					bAddSpaceBefore = false;
					bAddSpaceAfter = true;
					break;
				case _T('\t'):
				case _T(' '):
				case _T('\n'):
					if ((c == _T('\t') || c == _T(' ')) && dc != _T('(')) bAddSpaceBefore = true;
					if (asKeyword.LowerCase() == _T("begin")) {
						IndentLine++;
					}
					else if (asKeyword.LowerCase() == _T("end")) {
						IndentLine--;
					}
					asKeyword = _T("");
					break;
				default:
					if (dc == _T('/') && c == _T('/')) bCommentSingleLine = true;
					if (dc == _T('/') && c == _T('*')) bComment = true;

					if ((_T('a') <= c && c <= _T('z')) || (_T('A') <= c && c <= _T('Z'))) {
						if (!((_T('a') <= dc && dc <= _T('z')) || (_T('A') <= dc && dc <= _T('Z')))) asKeyword = _T("");
						asKeyword += c;
					}

					break;
			}

		}

		// Enregistrement du caractère

		if (bPrecomp || bSimpleQuote || bDoubleQuote || bCommentSingleLine || bComment) {
			asNewLine += c;
			NumCol++;
		}
		else if (c != _T('\t') && c != _T(' ') && c != _T('\n')) {
			if (bAddSpaceBefore) {
				asNewLine += _T(' ');
				NumCol++;
			}
			asNewLine += c;
			if (c != _T('\t')) NumCol++;
			else NumCol = ((NumCol - 1) / NbSpacesTab + 1) * NbSpacesTab + 1;
			bAddSpaceBefore = bAddSpaceAfter;
			bAddSpaceAfter = false;
		}

		// Traitement fin de ligne

		if (c != _T('\n')) {
			x++;
		}
		else {
			if (sOut) {
				if (IndentLine < 0) Indent += IndentLine;
				if (Indent < 0) Indent = 0;
				asNewLine = asNewLine.Trim();
				if (asNewLine.IsEmpty()) asIndent = _T("");
				else {
					if (!bPrecomp) {
						asIndent = AnsiString(_T('\t'), Indent);
						if (!bCommentSingleLine && !bComment && !bSimpleQuote && !bDoubleQuote) {
							int LengthLine = asNewLine.Length();
							if (sOut->Count > 0 && asNewLine == _T("{")) {
								AnsiString asPrevLine;
								asPrevLine = sOut->Strings[sOut->Count - 1];
								// Il ne faut pas insérer d'accolade s'il y a des commentaires de fin de ligne
								if (asPrevLine.Pos(_T("//")) == 0) {
									asNewLine = asPrevLine.Trim() + _T(" {");
									sOut->Delete(sOut->Count - 1);
									NumLine--;
								}
							}
						}
					}
					else {
						asIndent = _T("");
					}
				}
				sOut->Add(asIndent + asNewLine);
				if (IndentLine > 0) Indent += IndentLine;
				Indent += IndentPar;
				IndentLine = 0;
				IndentPar = 0;
			}
			bCommentSingleLine = false;
			if (!bEscape) {
				bSimpleQuote = false;
				bDoubleQuote = false;
			}
			if (dc != '\\') {
				bPrecomp = false;
			}
			y++;
			NumLine++;
			x = 0;
			NumCol = Indent * NbSpacesTab + 1;
			if (y < sIn->Count) {
				asOldLine = sIn->Strings[y];
				asNewLine = _T("");
				bAddSpaceBefore = false;
				bAddSpaceAfter = false;
				bOperator = false;
			}
		}

		// Caractère suivant

		ddc = dc;
		dc = c;

		if (y < sIn->Count) {
			if (x < asOldLine.Length() - 1) {
				c = asOldLine[x + 1];
				pc = asOldLine[x + 2];
			}
			else if (x < asOldLine.Length()) {
				c = asOldLine[x + 1];
				pc = _T('\n');
			}
			else if (x == asOldLine.Length()) {
				c = _T('\n');
				pc = _T('\0');
			}
		}
		else {
			c = _T('\0');
			pc = _T('\0');
		}

	}

	if (Indent > 0) {
		asError.sprintf(_TT("  %s (%i, %i): \"end\" manquant"),
										(LPCTSTR) FFileName, NumLine, NumCol);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bComment) {
		asError.sprintf(_TT("  %s (%i, %i): Commentaire non terminé"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bSimpleQuote || bDoubleQuote) {
		asError.sprintf(_TT("  %s (%i, %i): Chaîne non terminée"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	asError.sprintf(_TT("%i Erreur(s)"), NbErrors);
	FVCLIDEAutomation->AddLogMessage(tl_Process, asError);

	return (NbErrors == 0);
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierText::CodeFormatDFM(TStrings *sIn, TStrings *sOut, bool bSpacesAfterPar) {
	TCHAR ddc, dc, c, pc;
	bool bSimpleQuote = false;
	bool bOperator = false;
	bool bAddSpaceBefore = false;
	bool bAddSpaceAfter = false;
	AnsiString asIndent;
	AnsiString asOldLine;
	AnsiString asNewLine;
	AnsiString asKeyword;
	AnsiString asError;
	int x, y;
	int NumCol, NumLine, NbSpacesTab;
	int Indent, IndentLine, IndentPar;
	int NbErrors = 0;


	ddc = _T('\0');
	dc = _T('\0');
	c = _T('\0');
	x = 0;
	y = 0;
	NumCol = 1;
	NumLine = 1;
	Indent = 0;
	IndentLine = 0;
	IndentPar = 0;
	NbSpacesTab = Config->NbSpacesTab;
	asKeyword = _T("");

	FVCLIDEAutomation->ClearLogMessage(tl_Process);
	FVCLIDEAutomation->AddLogMessage(tl_Process, _TT("Formatage de code"));

	if (y < sIn->Count) {
		asOldLine = sIn->Strings[y];
		if (x < asOldLine.Length() - 1) {
			c = asOldLine[x + 1];
			pc = asOldLine[x + 2];
		}
		else if (x < asOldLine.Length()) {
			c = asOldLine[x + 1];
			pc = _T('\n');
		}
		else if (x == asOldLine.Length()) {
			c = _T('\n');
			pc = _T('\0');
		}
	}
	else {
		c = _T('\0');
		pc = _T('\0');
	}

	while (c != _T('\0')) {

		// Traitement du caractère
		if (bSimpleQuote) {
			if (c == _T('\'')) bSimpleQuote = false;
			else if (dc != '\\' && c == _T('\n')) {
				asError.sprintf(_TT("  %s (%i, %i): Retour à la ligne dans chaîne constante"),
												(LPCTSTR) FFileName, NumLine, NumCol);
				FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
				NbErrors++;
			}
		}

		else {

			// Espaces avant et après les opérateurs
			if (c == _T('=')) {
				if (!bOperator) {
					bAddSpaceBefore = true;
					bOperator = true;
				}
			}

			switch (c) {
				case _T('('):
				case _T('['):
					if (bSpacesAfterPar) {
						bAddSpaceAfter = true;
					}
					else {
						bAddSpaceAfter = false;
					}
					IndentPar += 2;
					break;
				case _T(')'):
				case _T(']'):
					if (bSpacesAfterPar) {
						bAddSpaceBefore = true;
					}
					else {
						bAddSpaceBefore = false;
					}
					IndentPar -= 2;
					break;
				case _T('\t'):
				case _T(' '):
				case _T('\n'):
					if ((c == _T('\t') || c == _T(' ')) && dc != _T('(') && dc != _T('[')) bAddSpaceBefore = true;
					if (asKeyword.LowerCase() == _T("object")) {
						IndentLine++;
					}
					else if (asKeyword.LowerCase() == _T("end")) {
						IndentLine--;
					}
					asKeyword = _T("");
					break;
				default:
					if ((_T('a') <= c && c <= _T('z')) || (_T('A') <= c && c <= _T('Z'))) {
						if (!((_T('a') <= dc && dc <= _T('z')) || (_T('A') <= dc && dc <= _T('Z')))) asKeyword = _T("");
						asKeyword += c;
					}

					break;
			}

		}

		// Enregistrement du caractère

		if (bSimpleQuote) {
			asNewLine += c;
			NumCol++;
		}
		else if (c != _T('\t') && c != _T(' ') && c != _T('\n')) {
			if (bAddSpaceBefore) {
				asNewLine += _T(' ');
				NumCol++;
			}
			asNewLine += c;
			if (c != _T('\t')) NumCol++;
			else NumCol = ((NumCol - 1) / NbSpacesTab + 1) * NbSpacesTab + 1;
			bAddSpaceBefore = bAddSpaceAfter;
			bAddSpaceAfter = false;
		}

		// Traitement fin de ligne

		if (c != _T('\n')) {
			x++;
		}
		else {
			if (sOut) {
				if (IndentLine < 0) Indent += IndentLine;
				if (Indent < 0) Indent = 0;
				asNewLine = asNewLine.Trim();
				if (asNewLine.IsEmpty()) asIndent = _T("");
				else {
					asIndent = AnsiString(_T('\t'), Indent);
					if (!bSimpleQuote) {
						int LengthLine = asNewLine.Length();
						if (sOut->Count > 0 && asNewLine == _T("{")) {
							AnsiString asPrevLine;
							asPrevLine = sOut->Strings[sOut->Count - 1];
							// Il ne faut pas insérer d'accolade s'il y a des commentaires de fin de ligne
							if (asPrevLine.Pos(_T("//")) == 0) {
								asNewLine = asPrevLine.Trim() + _T(" {");
								sOut->Delete(sOut->Count - 1);
								NumLine--;
							}
						}
					}
				}
				sOut->Add(asIndent + asNewLine);
				Indent += IndentPar;
				IndentPar = 0;
				if (IndentLine > 0) Indent += IndentLine;
				IndentLine = 0;
			}
			bSimpleQuote = false;
			y++;
			NumLine++;
			x = 0;
			NumCol = Indent * NbSpacesTab + 1;
			if (y < sIn->Count) {
				asOldLine = sIn->Strings[y];
				asNewLine = _T("");
				bAddSpaceBefore = false;
				bAddSpaceAfter = false;
				bOperator = false;
			}
		}

		// Caractère suivant

		ddc = dc;
		dc = c;

		if (y < sIn->Count) {
			if (x < asOldLine.Length() - 1) {
				c = asOldLine[x + 1];
				pc = asOldLine[x + 2];
			}
			else if (x < asOldLine.Length()) {
				c = asOldLine[x + 1];
				pc = _T('\n');
			}
			else if (x == asOldLine.Length()) {
				c = _T('\n');
				pc = _T('\0');
			}
		}
		else {
			c = _T('\0');
			pc = _T('\0');
		}

	}

	if (Indent > 0) {
		asError.sprintf(_TT("  %s (%i, %i): \"end\" manquant"),
										(LPCTSTR) FFileName, NumLine, NumCol);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bSimpleQuote) {
		asError.sprintf(_TT("  %s (%i, %i): Chaîne non terminée"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	asError.sprintf(_TT("%i Erreur(s)"), NbErrors);
	FVCLIDEAutomation->AddLogMessage(tl_Process, asError);

	return (NbErrors == 0);
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_EditFichierText::CodeFormatHTML(TStrings *sIn, TStrings *sOut) {
	TCHAR ddc, dc, c;
	bool bComment = false;
	bool bBalise = false;
	bool bBadName = false;
	bool bSpace = false;
	bool bQuote = false;
	bool bPHP = false;
	AnsiString asIndent;
	AnsiString asLine;
	AnsiString asBalise;
	AnsiString asBaliseAndAtt;
	AnsiString asError;
	TStringList slBalises;
	int x, y;
	int NumCol, NumLine, NbSpacesTab;
	int Indent, IndentLine;
	int NbErrors = 0;
	int i;


	ddc = _T('\0');
	dc = _T('\0');
	c = _T('\0');
	x = 0;
	y = 0;
	NumCol = 1;
	NumLine = 1;
	Indent = 0;
	IndentLine = 0;
	NbSpacesTab = Config->NbSpacesTab;

	FVCLIDEAutomation->ClearLogMessage(tl_Process);
	FVCLIDEAutomation->AddLogMessage(tl_Process, _TT("Formatage de code"));

	if (y < sIn->Count) {
		asLine = sIn->Strings[y];
		if (x < asLine.Length()) c = asLine[x + 1];
		else if (x == asLine.Length()) c = _T('\n');
	}
	else c = _T('\0');

	while (c != _T('\0')) {

		// Traitement du caractère
		if ((char) c < _T(' ') && c != _T('\n') && c != _T('\t')) {
			asError.sprintf(_TT("  %s (%i, %i): Caractère incorrect : %c"),
											(LPCTSTR) FFileName, NumLine, NumCol, c);
			FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
			NbErrors++;
		}

		if (bComment) {
			if (ddc == _T('-') && dc == _T('-') && c == _T('>')) bComment = false;
		}

		else if (bQuote) {
			if (dc != _T('\\') && c == _T('"')) bQuote = false;
			else if (c == _T('\n')) {
				asError.sprintf(_TT("  %s (%i, %i): Retour à la ligne dans chaîne constante"),
												(LPCTSTR) FFileName, NumLine, NumCol);
				FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
				NbErrors++;
				bQuote = false;
			}
		}

		else if (bPHP) {
			if (dc == _T('?') && c == _T('>')) bPHP = false;
		}

		else {

			switch (c) {
				case _T(' '):
				case _T('/'):
					if (bBalise) {
						asBaliseAndAtt += c;
						if (!asBalise.IsEmpty()) bSpace = true;
						else if (c == _T('/')) asBalise += c;
					}
					break;
				case _T('"'):
					if (bBalise) bQuote = true;
					break;
				case _T('?'):
					if (bBalise && dc == _T('<')) {
						bBalise = false;
						bPHP = true;
					}
					break;
				case _T('<'):
					if (bBalise) {
						asError.sprintf(_TT("  %s (%i, %i): \"<\" inattendu dans la balise <%s>"),
														(LPCTSTR) FFileName, NumLine, NumCol, (LPCTSTR) asBalise);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
					asBalise = _T("");
					asBaliseAndAtt = _T("");
					bBalise = true;
					bSpace = false;
					bBadName = false;
					break;
				case _T('>'):
					if (!bBalise) {
						asError.sprintf(_TT("  %s (%i, %i): \">\" inattendu"),
														(LPCTSTR) FFileName, x, y);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
					if (asBalise == _T("")) {
						asError.sprintf(_TT("  %s (%i, %i): Balise vide"),
														(LPCTSTR) FFileName, x, y);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
					if (bBadName && asBalise != _T("!DOCTYPE")) {
						asError.sprintf(_TT("  %s (%i, %i): Nom de balise incorrect <%s>"),
														(LPCTSTR) FFileName, NumLine, NumCol, (LPCTSTR) asBalise);
						FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
						NbErrors++;
					}
					if (asBalise.Length() >= 1 && asBalise[1] == '/') {
						int Index = (int) slBalises.Count - 1;
						if (Index < 0) {
							asError.sprintf(_TT("  %s (%i, %i): Balise inattendue <%s>"),
															(LPCTSTR) FFileName, NumLine, NumCol, (LPCTSTR) asBalise);
							FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
							NbErrors++;
						}
						else {
							if (asBalise != AnsiString(_T("/")) + slBalises[Index]) {
								asError.sprintf(_TT("  %s (%i, %i): Balise inattendue <%s> (attendue : </%s>)"),
																(LPCTSTR) FFileName, NumLine, NumCol, (LPCTSTR) asBalise, (LPCTSTR) slBalises[Index]);
								FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
								NbErrors++;

								for (i = Index; i >= 0; i--) {
									if (asBalise == AnsiString(_T("/")) + slBalises[i]) {
										while (Index >= i) {
											IndentLine--;
											slBalises.Delete(Index--);
										}
										break;
									}
								}

							}
							else {
								IndentLine--;
								slBalises.Delete(Index);
							}
						}
					}
					else if (asBaliseAndAtt[asBaliseAndAtt.Length()] != _T('/')) {
						if (asBalise != _T("!DOCTYPE")) {
							slBalises.Add(asBalise);
							IndentLine++;
						}
						else {
							if (slBalises.Count != 0) {
								asError.sprintf(_TT("  %s (%i, %i): \"!DOCTYPE\" inattendu"),
																(LPCTSTR) FFileName, NumLine, NumCol);
								FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
								NbErrors++;
							}
						}
					}
					bBalise = false;
					break;
				default:
					if (bBalise) {
						if (!bSpace) {
							if (!((c == _T('/') && asBalise == _T("")) || (_T('a') <= c && c <= _T('z')) || c == _T(':'))) bBadName = true;
							asBalise += c;
							if (asBalise == _T("!--")) {
								bComment = true;
								bBalise = false;
								bBadName = false;
							}
						}
						asBaliseAndAtt += c;
					}
					break;
			}

		}

		// Caractère suivant

		ddc = dc;
		dc = c;

		if (c != _T('\n')) {
			x++;
			if (c != _T('\t')) NumCol++;
			else NumCol = ((NumCol - 1) / NbSpacesTab + 1) * NbSpacesTab + 1;
		}
		else {
			if (sOut) {
				if (IndentLine < 0) Indent += IndentLine;
				if (Indent < 0) Indent = 0;
				if (!bPHP) {
					asLine = asLine.Trim();
					if (asLine.IsEmpty()) asIndent = _T("");
					else asIndent = AnsiString(_T('\t'), Indent);
					sOut->Add(asIndent + asLine);
				}
				else sOut->Add(asLine);
				if (IndentLine > 0) Indent += IndentLine;
				IndentLine = 0;
			}
			x = 0;
			NumCol = 1;
			y++;
			NumLine++;
			if (y < sIn->Count) {
				asLine = sIn->Strings[y];
			}
		}

		if (y < sIn->Count) {
			if (x < asLine.Length()) c = asLine[x + 1];
			else if (x == asLine.Length()) c = _T('\n');
		}
		else c = _T('\0');

	}

	if (bBalise) {
		asError.sprintf(_TT("  %s (%i, %i): \">\" manquant dans la balise %s"),
										(LPCTSTR) FFileName, NumLine, NumCol, (LPCTSTR) asBalise);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bComment) {
		asError.sprintf(_TT("  %s (%i, %i): Commentaire non terminé"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bQuote) {
		asError.sprintf(_TT("  %s (%i, %i): Chaîne non terminée"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	if (bPHP) {
		asError.sprintf(_TT("  %s (%i, %i): Code PHP non terminé"),
										(LPCTSTR) FFileName, x, y);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
	}

	while ((int) slBalises.Count) {
		asError.sprintf(_TT("  %s (%i, %i): Balise manquante </%s>"),
										(LPCTSTR) FFileName, NumLine, NumCol, (LPCTSTR) slBalises[(int) slBalises.Count - 1]);
		FVCLIDEAutomation->AddLogMessage(tl_Process, asError);
		NbErrors++;
		slBalises.Delete(slBalises.Count - 1);
	}

	asError.sprintf(_TT("%i Erreur(s)"), NbErrors);
	FVCLIDEAutomation->AddLogMessage(tl_Process, asError);

	return (NbErrors == 0);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::Frame_EditFichierTextShow(TObject *Sender) {
  SET_EVENT(TextEditor, TOnCaretMove, OnCaretMove, TFrame_EditFichierText, this, ObjectCaretMove);
	SET_EVENT(TextEditor, TOnMessage, OnMessage, TFrame_EditFichierText, this, ObjectDisplayMessage);
	SET_EVENT(TextEditor, TNotifyEvent, OnChange, TFrame_EditFichierText, this, ObjectChange);
	SET_EVENT(TextEditor, TOnFlagChanged, OnFlagChanged, TFrame_EditFichierText, this, ObjectFlagChanged);
	SET_EVENT(TextEditor, TNotifyEvent, OnPassBegin, TFrame_EditFichierText, this, ObjectPassBegin);
	SET_EVENT(TextEditor, TNotifyEvent, OnPassEnd, TFrame_EditFichierText, this, ObjectPassEnd);
	SET_EVENT(TextEditor, TOnAutocompletionOn, OnAutocompletionOn, TFrame_EditFichierText, this, ObjectAutocompletionOn);
	SET_EVENT(TextEditor, TOnAutocompletionOff, OnAutocompletionOff, TFrame_EditFichierText, this, ObjectAutocompletionOff);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::TextEditorDropFile(TObject *Sender,
		AnsiString &asFileName, TShiftState Shift, int X, int Y) {
  FVCLIDEAutomation->OpenFile(asFileName);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectCaretMove(TObject *Sender, int X, int Y) {
	AnsiString asMessage;
	asMessage.sprintf(_T("%i, %i"), Y, X);
	FVCLIDEAutomation->DisplayMessage(td_Position, asMessage);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectDisplayMessage(TObject *Sender, const AnsiString &asMessage) {
	FVCLIDEAutomation->DisplayMessage(td_Message, asMessage);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectChange(TObject *Sender) {
  OnChange(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectFlagChanged(TObject *Sender, bool bChanged) {
  OnFlagChanged(this, bChanged);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectPassBegin(TObject *Sender) {
  OnPassBegin(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectPassEnd(TObject *Sender) {
  OnPassEnd(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectAutocompletionOn(TObject *Sender, AnsiString asParent, int X, int Y, int NumCompletion) {
  OnAutocompletionOn(this, asParent, X, Y, NumCompletion);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::ObjectAutocompletionOff(TObject *Sender) {
  OnAutocompletionOff(this);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::TextEditorContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled) {
	TMenuItem *MenuItem;
	int NumCol;
	int NumLine;
	AnsiString asLine;
	AnsiString asSelText;


	NumCol = TextEditor->NumCol;
	NumLine = TextEditor->NumLine;
	asLine = TextEditor->Lines_Strings(NumLine);
	asSelText = TextEditor->SelText;

	PopupMenu_Context->Clear();

	// Commandes couper, copier, coller

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("Couper");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_CouperClick);
	if (asSelText.IsEmpty()) MenuItem->Enabled = false;
	PopupMenu_Context->Add(MenuItem);

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("Copier");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_CopierClick);
	if (asSelText.IsEmpty()) MenuItem->Enabled = false;
	PopupMenu_Context->Add(MenuItem);

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("Coller");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_CollerClick);
	PopupMenu_Context->Add(MenuItem);

	// Commandes positionner marque-pages et point d'arrêt

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _T("-");
	PopupMenu_Context->Add(MenuItem);

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _TT("Positionner un marque-page");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_AddBookmarkClick);
	PopupMenu_Context->Add(MenuItem);

	MenuItem = new TMenuItem(PopupMenu_Context);
	MenuItem->Caption = _TT("Positionner un point d'arrêt");
	SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_AddBreakpointClick);
	PopupMenu_Context->Add(MenuItem);

	if (asSelText.IsEmpty()) {

		// Commandes sélectionner le bloc

		MenuItem = new TMenuItem(PopupMenu_Context);
		MenuItem->Caption = _T("-");
		PopupMenu_Context->Add(MenuItem);

		MenuItem = new TMenuItem(PopupMenu_Context);
		MenuItem->Caption = _TT("Sélectionner le bloc");
		SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_SelectBlockClick);
		PopupMenu_Context->Add(MenuItem);

		MenuItem = new TMenuItem(PopupMenu_Context);
		MenuItem->Caption = _TT("Sélectionner la parenthèse");
		SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_SelectParenthesisClick);
		PopupMenu_Context->Add(MenuItem);

	}

	// Commandes "Chercher la déclaration" et "Chercher l'implémentation"

	if (TextEditor->WordFromPos(NumLine, NumCol, &asVarName)) {

		MenuItem = new TMenuItem(PopupMenu_Context);
		MenuItem->Caption = _T("-");
		PopupMenu_Context->Add(MenuItem);

		MenuItem = new TMenuItem(PopupMenu_Context);
		MenuItem->Caption = _TT("Chercher la déclaration de ") + asVarName;
		SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_SearchDeclarationClick);
		PopupMenu_Context->Add(MenuItem);

		MenuItem = new TMenuItem(PopupMenu_Context);
		MenuItem->Caption = _TT("Chercher l'implémentation de ") + asVarName;
		SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_SearchImplementationClick);
		PopupMenu_Context->Add(MenuItem);

	}

	// Commande "ouvrir le fichier sous le curseur"

	if (NumCol < asLine.Length()) {
		int Deb, Fin;
		AnsiString asFileName;
		AnsiString asCaption;

		// Guillemets
		Deb = NumCol;
		while (Deb >= 1 && asLine[Deb] != _T('\"')) Deb--;
		Fin = NumCol + 1;
		while (Fin <= asLine.Length() && asLine[Fin] != _T('\"')) Fin++;

		// Crochets
		if (Deb < 1 || asLine.Length() < Fin) {
			Deb = NumCol;
			while (Deb >= 1 && asLine[Deb] != _T('<')) Deb--;
			Fin = NumCol + 1;
			while (Fin <= asLine.Length() && asLine[Fin] != _T('>')) Fin++;
		}

		if (1 <= Deb && Fin <= asLine.Length()) {

			asFileName = asLine.SubString(Deb + 1, Fin - Deb - 1);

			if (IsCorrectFileName(asFileName)) {

				if (asFileName.Pos(_T("\\")) == 0 &&
						asFileName.Pos(_T("/")) == 0 &&
						asFileName.Pos(_T(":")) == 0) {
					asThisFile = ExtractFilePath(FFileName) + asFileName;
				}
				else {
					asThisFile = asFileName;
				}

				MenuItem = new TMenuItem(PopupMenu_Context);
				MenuItem->Caption = _T("-");
				PopupMenu_Context->Add(MenuItem);

				MenuItem = new TMenuItem(PopupMenu_Context);
				asCaption.sprintf(_TT("Ouvrir \"%s\""), (LPCTSTR) asFileName);
				MenuItem->Caption = asCaption;
				SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_OpenThisFileClick);
				PopupMenu_Context->Add(MenuItem);
			}

		}

	}

	// Commande "convertir en hexa" ou "convertir en décimal"
	if (!asSelText.IsEmpty()) {
		bool bIsHexa = false;
		bool bIsDec;
		unsigned long Number;

		if (IsDecimalOrHexa(asSelText, &Number, &bIsDec, &bIsHexa)) {
			AnsiString asCaption;

			MenuItem = new TMenuItem(PopupMenu_Context);
			MenuItem->Caption = _T("-");
			PopupMenu_Context->Add(MenuItem);

			MenuItem = new TMenuItem(PopupMenu_Context);
			if (bIsDec) {
				asConvert.sprintf(_T("0x%X"), Number);
				asCaption.sprintf(_TT("Convertir en hexa  %i -> %s"), Number, (LPCTSTR) asConvert);
			}
			else {
				asConvert.sprintf(_T("%i"), Number);
				asCaption.sprintf(_TT("Convertir en décimal  0x%X -> %s"), Number, (LPCTSTR) asConvert);
			}
			MenuItem->Caption = asCaption;
			SET_EVENT(MenuItem, TNotifyEvent, OnClick, TFrame_EditFichierText, this, MenuItem_ConvertClick);
			PopupMenu_Context->Add(MenuItem);
		}

	}

	PopupMenu_Context->Popup(MousePos.x, MousePos.y);
	Handled = true;

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_CouperClick(TObject *Sender) {
	TextEditor->CutToClipboard();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_CopierClick(TObject *Sender) {
	TextEditor->CopyToClipboard();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_CollerClick(TObject *Sender) {
	TextEditor->PasteFromClipboard();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_AddBookmarkClick(TObject *Sender) {
	int NumLine;

	NumLine = TextEditor->NumLine;
	if (TextEditor->GetFlag(NumLine, lfBookmark) == false) {
		FVCLIDEAutomation->AddBookmark();
	}
	else {
		FVCLIDEAutomation->DelBookmark();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_AddBreakpointClick(TObject *Sender) {
	int NumLine;

	NumLine = TextEditor->NumLine;
	if (TextEditor->GetFlag(NumLine, lfBreakpoint) == false) {
		FVCLIDEAutomation->AddBreakpoint();
	}
	else {
		FVCLIDEAutomation->DelBreakpoint();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_SelectBlockClick(TObject *Sender) {
	TextEditor->SelectBlock(TextEditor->NumLine, TextEditor->NumCol);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_SelectParenthesisClick(TObject *Sender) {
	TextEditor->SelectParenthesis(TextEditor->NumLine, TextEditor->NumCol);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_SearchDeclarationClick(TObject *Sender) {
	AnsiString asFileName;
	TTextEditor *NewTextEditor;
	int NumCol;
	int NumLine;


	if (FVCLIDEAutomation->FindDeclaration(asVarName, &asFileName, &NumLine, &NumCol)) {
		FVCLIDEAutomation->OpenTextFile(asFileName);
		NewTextEditor = FVCLIDEAutomation->GetCurTextFile();
		if (NewTextEditor) {
			NewTextEditor->MoveCur(NumLine, NumCol, false, false);
			NewTextEditor->SetFocus();
		}
	}
	else {
		FVCLIDEAutomation->DisplayError(
				_TT("Variable non trouvée : ") + asVarName,
				_TT("Recherche de la déclaration"),
				_T("FIND_VARIABLE"),
				MB_OK | MB_ICONWARNING);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_SearchImplementationClick(TObject *Sender) {
	AnsiString asFileName;
	TTextEditor *NewTextEditor;
	int NumCol;
	int NumLine;


	if (FVCLIDEAutomation->FindImplementation(asVarName, &asFileName, &NumLine, &NumCol)) {
		FVCLIDEAutomation->OpenTextFile(asFileName);
		NewTextEditor = FVCLIDEAutomation->GetCurTextFile();
		if (NewTextEditor) {
			NewTextEditor->MoveCur(NumLine, NumCol, false, false);
			NewTextEditor->SetFocus();
		}
	}
	else {
		FVCLIDEAutomation->DisplayError(
				_TT("Variable non trouvée : ") + asVarName,
				_TT("Recherche de l'implémentation"),
				_T("FIND_VARIABLE"),
				MB_OK | MB_ICONWARNING);
	}

}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_OpenThisFileClick(TObject *Sender) {
	FVCLIDEAutomation->OpenFile(asThisFile);
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_EditFichierText::MenuItem_ConvertClick(TObject *Sender) {
	TextEditor->SelText = asConvert;
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierText::SetKeyWords(AnsiString asExt) {
  TDictionnary *ListKeywords = NULL;
  bool bTrouve = false;
  int i;


	// Recherche si type inconnu
  for (i = 0; i < (int) ListsKeywords.size(); i++) {
    if (asExt == ListsKeywords[i].szExt) {
      ListKeywords = ListsKeywords[i].Keywords;
      bTrouve = true;
      break;
    }
  }

  // Si type inconnu: recherche si un fichier Keyword_xxxx existe
  if (!bTrouve) {
    AnsiString asKeywordsPath;
    AnsiString asKeywordsFile;
    asKeywordsPath = (AnsiString) Config->InstallPath + _T("\\Syntax");
    asKeywordsFile.sprintf(_T("%s\\Keywords_%s.txt"), (LPCTSTR) asKeywordsPath, (LPCTSTR) asExt);
    if (FileExists(asKeywordsFile)) {
      ListKeywords = new TDictionnary();
			ListKeywords->CaseSensitive = TextEditor->CaseSensitive;
			ListKeywords->LoadFromFile(asKeywordsFile);
      LIST_KEYWORDS NewListKeywords;
      lstrcpyn(NewListKeywords.szExt, asExt, 4);
      NewListKeywords.Keywords = ListKeywords;
      ListsKeywords.push_back(NewListKeywords);
    }
  }

	TextEditor->Keywords = ListKeywords;

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_EditFichierText::SetParameters(AnsiString asExt) {
  TDictionnary *ListKeywords = NULL;


  // Chargement des paramètres dépendants du langage
	// Ne pas réinitialiser les paramètres de la config (NbSpacesTab, Autocompletion, ...)
	TextEditor->AlphaChars = _T("abcdefghijklmnopqrstuvwxyzàâäéèêëîïôöùûüÿçABCDEFGHIJKLMNOPQRSTUVWXYZÀÂÄÉÈÊËÎÏÔÖÙÛÜŸÇ");
	TextEditor->Autocompletion1 = _T("");
	TextEditor->Autocompletion2 = _T("");
	TextEditor->Autocompletion3 = _T("");
	TextEditor->CaseSensitive = true;
	TextEditor->CommentSingle = _T("");
	TextEditor->CommentSingleBeg = _T("");
	TextEditor->CommentBegin = _T("");
	TextEditor->CommentEnd = _T("");
	TextEditor->CommentBegin2 = _T("");
	TextEditor->CommentEnd2 = _T("");
	TextEditor->BlockBegin = _T("");
	TextEditor->BlockEnd = _T("");
	TextEditor->PreProc = _T("");
	TextEditor->QuoteBegin = _T('\0');
	TextEditor->QuoteEnd = _T('\0');
	TextEditor->QuoteBegin2 = _T('\0');
	TextEditor->QuoteEnd2 = _T('\0');
	TextEditor->QuoteBegin3 = _T('\0');
	TextEditor->QuoteEnd3 = _T('\0');
	TextEditor->EscapeString = _T("");
	TextEditor->MultiLine = _T("");
	TextEditor->Associated1 = _T("");
	TextEditor->Associated2 = _T("");

	FCodeFormatType = cfNone;

  AnsiString asParametersPath;
  AnsiString asParametersFile;
  asParametersPath = (AnsiString) Config->InstallPath + _T("\\Syntax");
  asParametersFile.sprintf(_T("%s\\Parameters_%s.txt"), (LPCTSTR) asParametersPath, (LPCTSTR) asExt);
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
				if (asParam == _T("AlphaChars")) TextEditor->AlphaChars = asValue;
				else if (asParam == _T("Autocompletion1")) TextEditor->Autocompletion1 = asValue;
				else if (asParam == _T("Autocompletion2")) TextEditor->Autocompletion2 = asValue;
				else if (asParam == _T("Autocompletion3")) TextEditor->Autocompletion3 = asValue;
				else if (asParam == _T("CaseSensitive")) TextEditor->CaseSensitive = (asValue == _T("true"));
				else if (asParam == _T("CodeFormat")) {
					if (asValue == _T("CPP")) FCodeFormatType = cfCPP;
					else if (asValue == _T("PAS")) FCodeFormatType = cfPAS;
					else if (asValue == _T("DFM")) FCodeFormatType = cfDFM;
					else if (asValue == _T("HTML")) FCodeFormatType = cfHTML;
				}
				else if (asParam == _T("CommentSingle")) TextEditor->CommentSingle = asValue;
				else if (asParam == _T("CommentSingleBeg")) TextEditor->CommentSingleBeg = asValue;
				else if (asParam == _T("CommentBegin")) TextEditor->CommentBegin = asValue;
				else if (asParam == _T("CommentEnd")) TextEditor->CommentEnd = asValue;
				else if (asParam == _T("CommentBegin1")) TextEditor->CommentBegin = asValue;
				else if (asParam == _T("CommentEnd1")) TextEditor->CommentEnd = asValue;
				else if (asParam == _T("CommentBegin2")) TextEditor->CommentBegin2 = asValue;
				else if (asParam == _T("CommentEnd2")) TextEditor->CommentEnd2 = asValue;
				else if (asParam == _T("BlockBegin")) TextEditor->BlockBegin = asValue;
				else if (asParam == _T("BlockEnd")) TextEditor->BlockEnd = asValue;
				else if (asParam == _T("PreProc")) TextEditor->PreProc = asValue;
				else if (asParam == _T("QuoteBegin")) TextEditor->QuoteBegin = asValue[1];
				else if (asParam == _T("QuoteEnd")) TextEditor->QuoteEnd = asValue[1];
				else if (asParam == _T("QuoteBegin1")) TextEditor->QuoteBegin = asValue[1];
				else if (asParam == _T("QuoteEnd1")) TextEditor->QuoteEnd = asValue[1];
				else if (asParam == _T("QuoteBegin2")) TextEditor->QuoteBegin2 = asValue[1];
				else if (asParam == _T("QuoteEnd2")) TextEditor->QuoteEnd2 = asValue[1];
				else if (asParam == _T("QuoteBegin3")) TextEditor->QuoteBegin3 = asValue[1];
				else if (asParam == _T("QuoteEnd3")) TextEditor->QuoteEnd3 = asValue[1];
				else if (asParam == _T("EscapeString")) TextEditor->EscapeString = asValue;
				else if (asParam == _T("MultiLine")) TextEditor->MultiLine = asValue;
				else if (asParam == _T("Associated")) TextEditor->Associated1 = asValue;
				else if (asParam == _T("Associated1")) TextEditor->Associated1 = asValue;
				else if (asParam == _T("Associated2")) TextEditor->Associated2 = asValue;
			}
		}

		delete ListParameters;
  }

	return true;
}

bool FASTCALL TFrame_EditFichierText::SetLanguage(AnsiString asExt) {

	SetParameters(asExt);  // Paramètres avant les mot-clés (pour le paramètre CaseSensitive)
	SetKeyWords(asExt);

	return true;
}


//---------------------------------------------------------------------------
FVCL_RegisterComponent(TFrame_EditFichierText);

//---------------------------------------------------------------------------
