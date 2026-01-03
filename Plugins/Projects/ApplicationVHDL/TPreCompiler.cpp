//-----------------------------------------------------------------------------
//! @file TPreCompiler.cpp
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
#include "../../Include/Plugin.h"
#include "TPreCompiler.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
extern CALLBACK_ERROR CallbackError;


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
TPreCompiler::TPreCompiler(CALLBACK_DEFVAR ACallbackDefVar) {

  ListPathInclude.push_back(_T("C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\VC\\include"));
	ListPathInclude.push_back(_T("C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\Include"));
  ListPathInclude.push_back(_T("C:\\Sources\\FreeVCL\\Include"));

	CallbackDefVar = ACallbackDefVar;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::PreCompile(AnsiString asFileName) {
  IncludesStack.clear();
  IncludesList.clear();
  DefinePreProc.clear();
  IfStack.clear();
  IfLevel = 0;
	NbErrors = 0;
	bool Ok;


  DEFINE_PREPROC VarTmp;

  VarTmp.IncludeFile = _T("");
  VarTmp.NumLine = 0;
  VarTmp.Name = _T("__cplusplus");
  VarTmp.Args = _T("");
  VarTmp.Value = _T("");
  DefinePreProc.push_back(VarTmp);

  VarTmp.IncludeFile = _T("");
  VarTmp.NumLine = 0;
  VarTmp.Name = _T("_WIN32");
  VarTmp.Args = _T("");
  VarTmp.Value = _T("");
  DefinePreProc.push_back(VarTmp);

  VarTmp.IncludeFile = _T("");
  VarTmp.NumLine = 0;
  VarTmp.Name = _T("_M_IX86");
  VarTmp.Args = _T("");
  VarTmp.Value = _T("");
  DefinePreProc.push_back(VarTmp);

	// $$$$$$$ TEST $$$$$$$$$
	/*
	CallbackDefVar(_T("Button_Test"), 1, _T("TButton Button_Test"), _T("TForm"), _T("TButton"), _T(""), 0, 0);
	CallbackDefVar(_T("Default"), 1, _T("bool Default"), _T("TButton"), _T("bool"), _T(""), 0, 0);
	CallbackDefVar(_T("Cancel"), 1, _T("bool Cancel"), _T("TButton"), _T("bool"), _T(""), 0, 0);
	CallbackDefVar(_T("ModalResult"), 1, _T("TModalResult ModalResult"), _T("TButton"), _T("TModalResult"), _T(""), 0, 0);
	CallbackDefVar(_T("Edit_Test"), 1, _T("TEdit Edit_Test"), _T("TForm"), _T("TEdit"), _T(""), 0, 0);
	CallbackDefVar(_T("Text"), 1, _T("AnsiString Test"), _T("TEdit"), _T("AnsiString"), _T(""), 0, 0);
	CallbackDefVar(_T("MaxLength"), 1, _T("int MaxLength"), _T("TEdit"), _T("int"), _T(""), 0, 0);
	*/
	// $$$$$$$$$$$$$$$$$$$$$$

  if (!CtrlInclude(asFileName, -1, 0, false, false)) {
		AnsiString asErrMsg;
		asErrMsg.sprintf(_TT("%i erreurs détectées").c_str(), NbErrors);
		CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONINFORMATION);
		Ok = false;
  }
	else {
	  CallbackError(_TT("Traitement terminé").c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONINFORMATION);
		Ok = true;
	}

	return Ok;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::CtrlInclude(AnsiString asFileName, int IndexMemoPrec, int NumLinePrec, bool bIncludeTwoTimes, bool bSysFile) {
  bool Ok = true;
  AnsiString asLineAntiSlash;
  AnsiString asLine;
  AnsiString asPath;
  int Result;
  int Len;
  int i;
  unsigned int IfLevelPrev;
	TStringList Lines;
  int NumLine;
	bool bCommentMultiLines;
	int IndexMemo;


  bIfPrev = true;
  bIf = true;
  bElif = true;
  IfLevelPrev = IfLevel;
  IfLevel = (unsigned int) IfStack.size();

  asPath = ExtractFileDir(asFileName);

	if (!Lines.LoadFromFile(asFileName)) {
    AnsiString asErrMsg;
		wchar_t szBuf[128];

		i = (int) IncludesStack.size() - 1;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Lines.LastError, 0, szBuf, 128, NULL);
		asErrMsg.sprintf(_TT("  %s (%i, 1): Impossible d'ouvrir le fichier %s %s").c_str(),
										 IncludesStack[i].IncludeFile.c_str(),
										 NumLinePrec + 1,
										 asFileName.c_str(),
										 szBuf);
    CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
		NbErrors++;
    Ok = false;
  }
  else {

    MEMO_INCLUDE MemoInclude;
    MemoInclude.IncludeFile = asFileName;
    MemoInclude.NumLinePrec = NumLinePrec;
    MemoInclude.IndexMemoPrec = IndexMemoPrec;
		MemoInclude.bIncludeTwoTimes = bIncludeTwoTimes;
    IncludesStack.push_back(MemoInclude);
    IncludesList.push_back(MemoInclude);
		IndexMemo = (int) IncludesList.size() - 1;

		bCommentMultiLines = false;
    asLineAntiSlash = _T("");
    for (NumLine = 0; NumLine < Lines.Count && NbErrors < 100; NumLine++) {
			asLine = Lines[NumLine];
			Len = asLine.Length();
			if (Len > 0 && asLine[Len] == _T('\\')) {
				asLineAntiSlash += asLine.SubString(1, Len - 1);
			}
			else {
				asLine = asLineAntiSlash + asLine;
		    asLineAntiSlash = _T("");
				Result = Interpret(asPath, asFileName.c_str(), IndexMemo, NumLine, bIncludeTwoTimes, bSysFile, asLine, &bCommentMultiLines);
				if (Result < 0) {
					Ok = false;
				}
				else if (Result == 9999) {
				  while (IfStack.size() > IfLevel) PopIfStatus();
					Ok = true;
					break;
				}
				else if (Result > 0) {
					// Pas une commande pré-processor
				}
			}

    }

    IncludesStack.pop_back();

  }

  if (Ok && IfStack.size() != IfLevel) {
		AnsiString asErrMsg;
		asErrMsg.sprintf(_TT("  %s (%i, 1): #if sans #endif").c_str(), asFileName.c_str(), NumLine + 1);
		CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
	  while (IfStack.size() > IfLevel) PopIfStatus();
		NbErrors++;
    Ok = false;
  }

  IfLevel = IfLevelPrev;

  return Ok;
}

//-----------------------------------------------------------------------------
int TPreCompiler::Interpret(AnsiString asPath, AnsiString asFileName, int IndexMemo, int NumLine,
														bool bIncludeTwoTimes, bool bSysFile,
														AnsiString &asLine, bool *bCommentMultiLines) {
  AnsiString asCommande;
  AnsiString asLine2;
  AnsiString asLine3;
  AnsiString asLine4;
  AnsiString asParenthesis;
  AnsiString asVarName;
  std::vector<AnsiString> sMots;
	int i;


	if (!SplitCommande(SuppSpaces(SuppComment(asLine, bCommentMultiLines)), &asCommande, &asLine2)) return 0;

  // ------------------------------ #include ------------------------------
  if (asCommande == _T("include")) {

    if (bIfPrev && bIf) {

      AnsiString sFileNameInclude = SuppSpaces(asLine2);
      int Len = (int) sFileNameInclude.Length();
      if (Len < 2) {
				AnsiString asErrMsg;
				asErrMsg.sprintf(_TT("  %s (%i, 1): Erreur de syntaxe %s").c_str(), asFileName.c_str(), NumLine + 1, asLine2.c_str());
				CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
				NbErrors++;
        return -1;
      }

      if (sFileNameInclude[1] == L'"' && sFileNameInclude[Len] == L'"') {
        sFileNameInclude = sFileNameInclude.SubString(2, Len - 2);
        AnsiString asFullFileName = GetFullName(asPath, sFileNameInclude);
        if (TestCoherenceInclude(asFullFileName, NumLine, &bIncludeTwoTimes, bSysFile)) {
          if (!CtrlInclude(asFullFileName.c_str(), IndexMemo, NumLine, bIncludeTwoTimes, bSysFile)) return -1;
        }
      }
      else if (sFileNameInclude[1] == L'<' && sFileNameInclude[Len] == L'>') {
        sFileNameInclude = sFileNameInclude.SubString(2, Len - 2);
        AnsiString asFullFileName = RechercheInclude(sFileNameInclude);
        if (TestCoherenceInclude(asFullFileName, NumLine, &bIncludeTwoTimes, true)) {
          if (!CtrlInclude(asFullFileName.c_str(), IndexMemo, NumLine, bIncludeTwoTimes, true)) return -1;
        }
      }
      else {
				AnsiString asErrMsg;
				asErrMsg.sprintf(_TT("  %s (%i, 1): Erreur de syntaxe %s").c_str(), asFileName.c_str(), NumLine + 1, asLine2.c_str());
				CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
				NbErrors++;
        return -1;
      }


    }

  }

  // ------------------------------ #define ------------------------------
  else if (asCommande == _T("define")) {

    if (bIfPrev && bIf) {

      if (!SplitVarName(asLine2, &asVarName, &asLine3)) {
				AnsiString asErrMsg;
				asErrMsg.sprintf(_TT("  %s (%i, 1): Erreur de syntaxe").c_str(), asFileName.c_str(), NumLine + 1);
				CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
				NbErrors++;
        return -1;
      }

      // "#define TOTO (a)" : (a) est une valeur, pas un paramètre.
      // "#define TOTO(a)" : ici (a) est un paramètre.
			if (asLine3.Length() >= 2 && asLine3[1] == _T('(')) {
				if (!SplitParenthesis(asLine3, &asParenthesis, &asLine4)) {
	        return -1;
				}
				asParenthesis = SuppSpaces(asParenthesis);
			}
			else {
				asParenthesis = _T("");
        asLine4 = asLine3;
			}
      asLine4 = SuppSpaces(asLine4);

			int Index = IndexOfVar(asVarName);
      if (Index != -1) {
        if (DefinePreProc[Index].Value != asLine4) {
					AnsiString asErrMsg;
					asErrMsg.sprintf(_TT("  %s (%i, 1): Variable déjà définie %s").c_str(), asFileName.c_str(), NumLine + 1, asLine.c_str());
					CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);

					AnsiString asPrevDef;
          asPrevDef += _T("#define ");
          asPrevDef += DefinePreProc[Index].Name;
          if (!DefinePreProc[Index].Args.IsEmpty()) {
            asPrevDef += _T("(");
            asPrevDef += DefinePreProc[Index].Args;
            asPrevDef += _T(")");
          }
          asPrevDef += _T(" ");
          asPrevDef += DefinePreProc[Index].Value;
					asErrMsg.sprintf(_TT("  %s (%i, 1): Précédente définition : %s").c_str(), DefinePreProc[Index].IncludeFile.c_str(), DefinePreProc[Index].NumLine + 1, asPrevDef.c_str());
					CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONINFORMATION);

					NbErrors++;
          return -1;
        }
      }
      else {
        DEFINE_PREPROC VarTmp;
				AnsiString asFullFileName;
				AnsiString asDescription;

				if (asParenthesis.IsEmpty()) {
					asDescription.sprintf(_T("#define %s %s"), asVarName.c_str(), asLine4.c_str());
				}
				else {
					asDescription.sprintf(_T("#define %s(%s) %s"), asVarName.c_str(), asParenthesis.c_str(), asLine4.c_str());
				}

				asFullFileName = GetFullName(asPath, asFileName);
        VarTmp.IncludeFile = asFullFileName;
        VarTmp.NumLine = NumLine;
        VarTmp.Name = asVarName;
        VarTmp.Args = asParenthesis;
        VarTmp.Value = asLine4;
        DefinePreProc.push_back(VarTmp);

				CallbackDefVar(asVarName.c_str(), 1, asDescription.c_str(), _T(""), _T(""), asFullFileName.c_str(), NumLine, 0);

			}

    }

  }

  // ------------------------------ #undef ------------------------------
  else if (asCommande == _T("undef")) {

    if (bIfPrev && bIf) {

      SplitWords(asLine2, &sMots);

      if (sMots.size() != 1) {
				AnsiString asErrMsg;
				asErrMsg.sprintf(_TT("  %s (%i, 1): Erreur de syntaxe").c_str(), asFileName.c_str(), NumLine + 1);
				CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
				NbErrors++;
        return -1;
      }

      int Index = IndexOfVar(sMots[0]);
      if (Index != -1) {
        DefinePreProc.erase(DefinePreProc.begin() + Index);
      }

    }

  }

  // ------------------------------ #if ------------------------------
  else if (asCommande == _T("if")) {

    PushIfStatus();

    bIf = EvalBool(asFileName, NumLine, asLine2);

  }

  // ------------------------------ #ifdef ------------------------------
  else if (asCommande == _T("ifdef")) {

    PushIfStatus();

    SplitWords(asLine2, &sMots);

    if (sMots.size() != 1) {
			AnsiString asErrMsg;
			asErrMsg.sprintf(_TT("  %s (%i, 1): Erreur de syntaxe").c_str(), asFileName.c_str(), NumLine + 1);
			CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			NbErrors++;
      return -1;
    }

    bIf = (IndexOfVar(sMots[0]) != -1);

  }

  // ------------------------------ #ifndef ------------------------------
  else if (asCommande == _T("ifndef")) {

    PushIfStatus();

    SplitWords(asLine2, &sMots);

    if (sMots.size() != 1) {
			AnsiString asErrMsg;
			asErrMsg.sprintf(_TT("  %s (%i, 0): Erreur de syntaxe").c_str(), asFileName.c_str(), NumLine);
			CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			NbErrors++;
      return -1;
    }

    bIf = (IndexOfVar(sMots[0]) == -1);

  }

  // ------------------------------ #elif ------------------------------
  else if (asCommande == _T("elif")) {

    if (IfStack.size() <= IfLevel) {
			AnsiString asErrMsg;
			asErrMsg.sprintf(_TT("  %s (%i, 1): #elif sans #if %s").c_str(), asFileName.c_str(), NumLine + 1, asLine.c_str());
			CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			NbErrors++;
      return -1;
    }

    if (bIf) {
      bIf = false;
      bElif = false;
    }
    else if (!bElif) {
      bIf = false;
    }
    else {
      bIf = EvalBool(asFileName, NumLine, asLine2);
    }

  }

  // ------------------------------ #else ------------------------------
  else if (asCommande == _T("else")) {

    if (IfStack.size() <= IfLevel) {
			AnsiString asErrMsg;
			asErrMsg.sprintf(_TT("  %s (%i, 1): #else sans #if %s").c_str(), asFileName.c_str(), NumLine + 1, asLine.c_str());
			CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			NbErrors++;
      return -1;
    }

    bIf = (!bIf && bElif);

  }

  // ------------------------------ #endif ------------------------------
  else if (asCommande == _T("endif")) {

    if (!asLine2.IsEmpty()) {
			AnsiString asErrMsg;
			asErrMsg.sprintf(_TT("  %s (%i, 1): Erreur de syntaxe").c_str(), asFileName.c_str(), NumLine + 1);
			CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			NbErrors++;
      return -1;
    }

    if (IfStack.size() <= IfLevel) {
			AnsiString asErrMsg;
			asErrMsg.sprintf(_TT("  %s (%i, 1): #endif sans #if").c_str(), asFileName.c_str(), NumLine + 1);
			CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			NbErrors++;
      return -1;
    }

    PopIfStatus();

  }

  // ------------------------------ #error ------------------------------
  else if (asCommande == _T("error")) {

    if (bIfPrev && bIf) {
      AnsiString asErrMsg;
			asErrMsg.sprintf(_T("  %s (%i, 1): %s"), asFileName.c_str(), NumLine + 1, asLine.c_str());
      CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			NbErrors++;
      return -1;
    }

  }

  // ------------------------------ #pragma ------------------------------
  else if (asCommande == _T("pragma")) {

    asLine2 = SuppSpaces(asLine2);
		if (asLine2 == _T("once")) {
			for (i = 0; i < (int) IncludesList.size() - 1; i++) {
				if (IncludesList[i].IncludeFile == asFileName) {
					return 9999;
				}
			}
		}

  }

  // ------------------------------ Commande inconnue ------------------------------
  else {
    AnsiString asErrMsg;
		asErrMsg.sprintf(_TT("  %s (%i, 1): Commande inconnue %s").c_str(), asFileName.c_str(), NumLine + 1, asLine.c_str());
    CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
		NbErrors++;
  }

  return 1;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::SplitCommande(AnsiString &asLine, AnsiString *asCommande, AnsiString *asLine2) {
  int i = 0;
  int DebMot;
  int Len = (int) asLine.Length();


  DebMot = -1;

  // Espaces de début de ligne
  i = 1;
  while (i <= Len && (asLine[i] == L' ' || asLine[i] == L'\t')) i++;

  // #
  if (i > Len || asLine[i] != L'#') return false;
  i++;
  while (i <= Len && (asLine[i] == L' ' || asLine[i] == L'\t')) i++;

  // Commande pré-processeur
  DebMot = i;
  while (i <= Len && asLine[i] != L' ' && asLine[i] != L'\t' && asLine[i] != L'(') i++;
  *asCommande = asLine.SubString(DebMot, i - DebMot);

  // Arguments
  while (i <= Len && (asLine[i] == L' ' || asLine[i] == L'\t')) i++;
  *asLine2 = asLine.SubString(i, Len - i + 1);

  return true;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::SplitVarName(AnsiString &asLine, AnsiString *asVarName, AnsiString *asLine2) {
  int i = 0;
  int DebMot;
  int Len = (int) asLine.Length();


  DebMot = -1;

  // Espaces de début de ligne
  i = 1;
  while (i <= Len && (asLine[i] == L' ' || asLine[i] == L'\t')) i++;

  // Nom de variable ou de fonction
  DebMot = i;
  while (i <= Len && asLine[i] != L' ' && asLine[i] != L'\t' && asLine[i] != L'(') i++;
  *asVarName = asLine.SubString(DebMot, i - DebMot);

  // Pas de suppression des espaces après le nom de variable
	// Pour pouvoir différentier "#define TOTO(a)" et "#define TOTO (a)"

  // Reste de l'instruction
  *asLine2 = asLine.SubString(i, Len - i + 1);

  return true;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::SplitParenthesis(AnsiString &asLine, AnsiString *asParenthesis, AnsiString *asLine2) {
  int i = 0;
  int Len = (int) asLine.Length();
  int LevelPar, DebPar;
  wchar_t c;


  // Espaces de début de ligne
  i = 1;
  while (i <= Len && (asLine[i] == L' ' || asLine[i] == L'\t')) i++;

  if (i > Len || asLine[i] != L'(') return false;

  DebPar = i;
  LevelPar = 0;
  for (i; i <= Len; i++) {
    c = asLine[i];
    if (c == L'(') {
      LevelPar++;
    }
    else if (c == L')') {
      if (--LevelPar == 0) {
        *asParenthesis = asLine.SubString(DebPar + 1, i - DebPar - 1);
        *asLine2 = asLine.SubString(i + 1, Len - i);
        return true;
      }
    }
  }

  return false;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::SplitWords(AnsiString &asLine, std::vector<AnsiString> *sMots) {
  int i = 0;
  int DebMot;
  int Len = (int) asLine.Length();


  DebMot = -1;

  // Premier mot
  for (i = 1; i <= Len + 1; i++) {
    // $$$ Traiter les guillemets
    if (i > Len || asLine[i] == L' ' || asLine[i] == L'\t' || asLine[i] == L'(' || asLine[i] == L')') {
      if (DebMot != -1) {
        sMots->push_back(asLine.SubString(DebMot, i - DebMot));
        DebMot = -1;
      }
    }
    else {
      if (DebMot == -1) DebMot = i;
    }
  }

  return true;
}

//-----------------------------------------------------------------------------
AnsiString TPreCompiler::SuppComment(AnsiString asLine, bool *bCommentMultiLines) {
  int Len = (int) asLine.Length();
  int i;
  wchar_t c, dc;
  int DebComment;


	if (*bCommentMultiLines) DebComment = 1;
	else DebComment = 0;

	dc = '\0';
  for (i = 1; i <= Len; i++) {
    // $$$ Traiter les guillemets
    c = asLine[i];
    if (DebComment == 0 && dc == '/' && c == '/') {
			asLine = asLine.SubString(1, i - 2);
			break;
		}
    if (DebComment == 0 && dc == '/' && c == '*') DebComment = i - 1;
    else if (DebComment >= 1 && dc == '*' && c == '/') {
      asLine = asLine.SubString(1, DebComment - 1) + asLine.SubString(i + 1, Len - i);
      dc = '\0';
      i = DebComment;
			DebComment = 0;
      Len = (int) asLine.Length();
    }
    dc = c;
  }

	if (DebComment > 0) {
    asLine = asLine.SubString(1, DebComment - 1);
		*bCommentMultiLines = true;
	}
	else {
		*bCommentMultiLines = false;
	}

  return asLine;
}

//-----------------------------------------------------------------------------
AnsiString TPreCompiler::SuppSpaces(AnsiString asLine) {
  int Len = (int) asLine.Length();
  int i, j;


  // Espaces de début de ligne
  i = 1;
  while (i <= Len && (asLine[i] == L' ' || asLine[i] == L'\t')) i++;
  if (i > Len) return _T("");

  // Espaces de fin de ligne
  j = Len;
  while (j > i && (asLine[j] == L' ' || asLine[j] == L'\t')) j--;

  return asLine.SubString(i, j - i + 1);
}

//-----------------------------------------------------------------------------
AnsiString TPreCompiler::GetFullName(AnsiString asPath, AnsiString asFileName) {
  int i = 0;
  int Len = (int) asFileName.Length();


  if (Len > 2 && asFileName[2] == ':') return asFileName;

  return asPath + DIR_DELIMITER + asFileName;
}

//-----------------------------------------------------------------------------
AnsiString TPreCompiler::RechercheInclude(AnsiString asFileName) {
  AnsiString asFullFileName;
  int i;


  for (i = 0; i < (int) ListPathInclude.size(); i++) {
    asFullFileName = GetFullName(ListPathInclude[i], asFileName);
    if (GetFileAttributes(asFullFileName.c_str()) != -1) return asFullFileName;
  }

  return asFileName;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::TestCoherenceInclude(AnsiString asFileName, int NumLine, bool *bIncludeTwoTimes, bool bSysFile) {
	AnsiString asNextFile;
  AnsiString asErrMsg;
	int NumLinePrec;
	bool bOk = true;
  int i, j;


  if (!(*bIncludeTwoTimes) && !bSysFile) {

    for (i = 0; i < (int) IncludesList.size(); i++) {
      if (!IncludesList[i].bIncludeTwoTimes && IncludesList[i].IncludeFile == asFileName) {

				asErrMsg.sprintf(_TT("Inclusion en double : %s").c_str(), asFileName.c_str());
        CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONWARNING);
				IncludesList[i].bIncludeTwoTimes = true;
				*bIncludeTwoTimes = true;

        CallbackError(_TT("Première inclusion :").c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONWARNING);
				NumLinePrec = IncludesList[i].NumLinePrec;
				asNextFile = IncludesList[i].IncludeFile;
				j = IncludesList[i].IndexMemoPrec;
				while (j != -1) {
					asErrMsg.sprintf(_TT("    %s (%i, 1): Inclusion de \"%s\"").c_str(),
													 IncludesList[j].IncludeFile.c_str(),
													 NumLinePrec + 1,
													 ExtractFileName(asNextFile).c_str());
					CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONWARNING);
					NumLinePrec = IncludesList[j].NumLinePrec;
					asNextFile = IncludesList[j].IncludeFile;
					j = IncludesList[j].IndexMemoPrec;
				}

        CallbackError(_TT("Seconde inclusion :").c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONWARNING);
				NumLinePrec = NumLine;
				asNextFile = asFileName;
        for (j = (int) IncludesStack.size() - 1; j >= 0; j--) {
					asErrMsg.sprintf(_TT("    %s (%i, 1): Inclusion de \"%s\"").c_str(),
													 IncludesStack[j].IncludeFile.c_str(),
													 NumLinePrec + 1,
													 ExtractFileName(asNextFile).c_str());
					CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONWARNING);
					NumLinePrec = IncludesStack[j].NumLinePrec;
					asNextFile = IncludesStack[j].IncludeFile;
        }

      }
    }

  }

  for (i = 0; i < (int) IncludesStack.size(); i++) {
    if (IncludesStack[i].IncludeFile == asFileName) {

			asErrMsg.sprintf(_TT("  %s (1, 1): Inclusion circulaire :").c_str(), asFileName.c_str());
      CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONWARNING);
			NumLinePrec = NumLine;
			asNextFile = asFileName;
      for (j = (int) IncludesStack.size() - 1; j >= i ; j--) {
				asErrMsg.sprintf(_TT("    %s (%i, 1): Inclusion de \"%s\"").c_str(),
													IncludesStack[j].IncludeFile.c_str(),
													NumLinePrec + 1,
													ExtractFileName(asNextFile).c_str());
				CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
				NumLinePrec = IncludesStack[j].NumLinePrec;
				asNextFile = IncludesStack[j].IncludeFile;
      }
			NbErrors++;
      bOk = false;
      break;
    }
  }

  if (GetFileAttributes(asFileName.c_str()) == -1) {

		AnsiString asErrMsg;
    for (i = (int) IncludesStack.size() - 1; i >= 0; i--) {
			if (i == (int) IncludesStack.size() - 1) {
				asErrMsg.sprintf(_TT("  %s (%i, 1): Fichier inexistant \"%s\"").c_str(),
												 IncludesStack[i].IncludeFile.c_str(),
												 NumLinePrec + 1,
												 asFileName.c_str());
				CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
			}
			else {
				asErrMsg.sprintf(_TT("  %s (%i, 1): Inclusion de \"%s\"").c_str(),
												 IncludesStack[i].IncludeFile.c_str(),
												 IncludesStack[i + 1].NumLinePrec + 1,
												 ExtractFileName(IncludesStack[i + 1].IncludeFile).c_str());
				// $$$ CallbackError(asErrMsg.c_str(), _TT("Inclusion").c_str(), MB_OK | MB_ICONINFORMATION);
			}
    }

		NbErrors++;
    bOk = false;
  }

  return bOk;
}

//-----------------------------------------------------------------------------
int TPreCompiler::IndexOfVar(AnsiString asVarName) {
  int i;


  for (i = 0; i < (int) DefinePreProc.size(); i++) {
    if (DefinePreProc[i].Name == asVarName) return i;
  }

  return -1;
}

//-----------------------------------------------------------------------------
bool TPreCompiler::EvalBool(AnsiString asFileName, int NumLine, AnsiString asLine) {
  int i;
  int Len;
  int LevelPar;
  wchar_t c, dc;


  asLine = SuppSpaces(asLine);
  Len = (int) asLine.Length();

  // Operateurs && et ||
  LevelPar = 0;
  dc = L'\0';
  for (i = 1; i <= Len; i++) {
    c = asLine[i];
    if (c == L'(') {
      LevelPar++;
    }
    else if (c == L')') {
      LevelPar--;
    }
    else if (dc == L'&' && c == L'&' && LevelPar == 0) {
      bool bEval1 = EvalBool(asFileName, NumLine, asLine.SubString(1, i - 2));
      bool bEval2 = EvalBool(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return bEval1 && bEval2;
    }
    else if (dc == L'|' && c == L'|' && LevelPar == 0) {
      bool bEval1 = EvalBool(asFileName, NumLine, asLine.SubString(1, i - 2));
      bool bEval2 = EvalBool(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return bEval1 && bEval2;
    }
    dc = c;
  }

  // Operateurs ==, !=, <, <=, > et >=
  LevelPar = 0;
  dc = L'\0';
  for (i = 1; i <= Len; i++) {
    c = asLine[i];
    if (c == L'(') {
      LevelPar++;
    }
    else if (c == L')') {
      LevelPar--;
    }
    else if (dc == L'=' && c == L'=' && LevelPar == 0) {
			AnsiString asEval1 = EvalString(asFileName, NumLine, asLine.SubString(1, i - 2));
			AnsiString asEval2 = EvalString(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (asEval1 == asEval2);
    }
    else if (dc == L'!' && c == L'=' && LevelPar == 0) {
			AnsiString asEval1 = EvalString(asFileName, NumLine, asLine.SubString(1, i - 2));
			AnsiString asEval2 = EvalString(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (asEval1 != asEval2);
    }
    else if (c == L'<' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 1));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 < Eval2);
    }
    else if (dc == L'<' && c == L'=' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 2));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 <= Eval2);
    }
    else if (c == L'>' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 1));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 > Eval2);
    }
    else if (dc == L'>' && c == L'=' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 2));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 >= Eval2);
    }
    dc = c;
  }

  // Operateurs !  
  LevelPar = 0;
  dc = L'\0';
  for (i = 1; i <= Len; i++) {
    c = asLine[i];
    if (c == L'(') {
      LevelPar++;
    }
    else if (c == L')') {
      LevelPar--;
    }
    else if (c == L'!' && LevelPar == 0) {
      bool bEval = EvalBool(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return !bEval;
    }
    dc = c;
  }

  // Fonctions defined, ...
  // Parenthèses englobant la totalité
	LevelPar = 0;
  AnsiString sKeyWord;
  for (i = 1; i <= Len; i++) {
    c = asLine[i];
    if (!((L'a' <= c && c <= L'z') || (L'A' <= c && c <= L'Z') || c == L'_')) break;
    sKeyWord += c;
  }
  while (i <= Len && (asLine[i] == L' ' || asLine[i] == L'\t')) i++;

  AnsiString sArg;
  for (i; i <= Len; i++) {
    c = asLine[i];
    if (c == L'(') {
      if (LevelPar++ > 0) sArg += c;
    }
    else if (c == L')') {
      if (--LevelPar > 0) sArg += c;
      else break;
    }
    else if (LevelPar >= 1) {
      sArg += c;
    }
  }
  sArg = SuppSpaces(sArg);

	if (LevelPar != 0) {
    AnsiString asErrMsg;
		asErrMsg.sprintf(_TT("  %s (%i, 1): Parenthèses dépareillées %s").c_str(), asFileName.c_str(), NumLine + 1, asLine.c_str());
    CallbackError(asErrMsg.c_str(), _TT("Erreur").c_str(), MB_OK | MB_ICONSTOP);
	}

  if (sKeyWord == _T("defined")) {
    return (IndexOfVar(sArg) != -1);
  }
  else if (sKeyWord.IsEmpty() && !sArg.IsEmpty()) {
    return EvalBool(asFileName, NumLine, sArg);
  }
  else {
    return (EvalInt(asFileName, NumLine, asLine) != 0);
  }

  return false;
}

//-----------------------------------------------------------------------------
int TPreCompiler::EvalInt(AnsiString asFileName, int NumLine, AnsiString asLine) {
  int i;
  int Len;
  int LevelPar;
  wchar_t c, dc;


  asLine = EvalString(asFileName, NumLine, asLine);
  Len = (int) asLine.Length();

	// Parenthèses englobant la totalité
	while (asLine.Length() >= 2 && asLine[1] == _T('(')) {

	  LevelPar = 0;
		for (i = 1; i <= Len; i++) {
			c = asLine[i];
			if (c == L'(') {
				LevelPar++;
			}
			else if (c == L')') {
				if (--LevelPar == 0) break;
			}
		}

		if (i != Len) break;

		asLine = SuppSpaces(asLine.SubString(2, Len - 2));
		Len = (int) asLine.Length();

	}

  // Operateurs & et |
  LevelPar = 0;
  dc = L'\0';
  for (i = 1; i <= Len; i++) {
    c = asLine[i];
    if (c == L'(') {
      LevelPar++;
    }
    else if (c == L')') {
      LevelPar--;
    }
    if (c == L'&' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 1));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 & Eval2);
    }
    else if (c == L'|' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 1));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 | Eval2);
    }
    dc = c;
  }

  // Operateurs << et >>
  LevelPar = 0;
  dc = L'\0';
  for (i = 1; i <= Len; i++) {
    c = asLine[i];
    if (c == L'(') {
      LevelPar++;
    }
    else if (c == L')') {
      LevelPar--;
    }
    if (dc == L'<' && c == L'<' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 2));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 << Eval2);
    }
    else if (dc == L'>' && c == L'>' && LevelPar == 0) {
      int Eval1 = EvalInt(asFileName, NumLine, asLine.SubString(1, i - 2));
      int Eval2 = EvalInt(asFileName, NumLine, asLine.SubString(i + 1, Len - i));
      return (Eval1 >> Eval2);
    }
    dc = c;
  }

  if (asLine.Length() >= 2) {
    if (asLine[1] == L'0' && asLine[2] == L'x') {
      int Value = 0;
      Len = (int) asLine.Length();
      for (i = 3; i <= Len; i++) {
        c = asLine[i];
        if (L'0' <= c && c <= L'9') c = c - L'0';
        else if (L'a' <= c && c <= L'f') c = c - L'a' + 10;
        else if (L'A' <= c && c <= L'F') c = c - L'A' + 10;
        Value = Value * 16 + c;
      }
      return Value;
    }
  }

  return asLine.ToIntDef(0);
}

//-----------------------------------------------------------------------------
AnsiString TPreCompiler::EvalString(AnsiString asFileName, int NumLine, AnsiString asLine) {

  asLine = SuppSpaces(asLine);

	// $$$ remplacer la totalité des variables dans la chaîne
  int Index = IndexOfVar(asLine);
  while (Index != -1) {
    asLine = DefinePreProc[Index].Value;
    Index = IndexOfVar(asLine);
  }

  return asLine;
}

//-----------------------------------------------------------------------------
void TPreCompiler::PushIfStatus(void) {
  bIfPrev = (bIfPrev && bIf);

  MEMO_IF MemoIf;
  MemoIf.bIf = bIf;
  MemoIf.bElif = bElif;
  IfStack.push_back(MemoIf);
}

//-----------------------------------------------------------------------------
void TPreCompiler::PopIfStatus(void) {
  int i;


  bIf = IfStack[IfStack.size() - 1].bIf;
  bElif = IfStack[IfStack.size() - 1].bElif;
  IfStack.pop_back();

  bIfPrev = true;
  for (i = 0; i < (int) IfStack.size(); i++) {
    if (IfStack[i].bIf == false) {
      bIfPrev = false;
      break;
    }
  }
}
