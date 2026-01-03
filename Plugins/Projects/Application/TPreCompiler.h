//-----------------------------------------------------------------------------
//! @file TPreCompiler.h
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
#ifndef PreCompilation_H
#define PreCompilation_H

#include "targetver.h"

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

typedef struct {
  AnsiString IncludeFile;
  int IndexMemoPrec;
  int NumLinePrec;
	bool bIncludeTwoTimes;
} MEMO_INCLUDE;

typedef struct {
  AnsiString IncludeFile;
  int NumLine;
  AnsiString Name;
  AnsiString Args;
  AnsiString Value;
} DEFINE_PREPROC;

typedef struct {
  bool bIf;
  bool bElif;
} MEMO_IF;

//---------------------------------------------------------------------------
class TPreCompiler {
private:
	std::vector<AnsiString> ListPathInclude;
	std::vector<MEMO_INCLUDE> IncludesStack;
	std::vector<MEMO_INCLUDE> IncludesList;
	std::vector<DEFINE_PREPROC> DefinePreProc;
	std::vector<MEMO_IF> IfStack;
	
	bool bIfPrev;
	bool bIf;
	bool bElif;
	unsigned int IfLevel;
	int NbErrors;	

	CALLBACK_DEFVAR CallbackDefVar;

	bool CtrlInclude(AnsiString asFileName, int IndexMemoPrec, int NumLinePrec,
									 bool bIncludeTwoTimes, bool bSysFile);
	int Interpret(AnsiString asPath, AnsiString asFileName, int IndexMemo, int NumLine,
								bool bIncludeTwoTimes, bool bSysFile,
								AnsiString &asLine, bool *bCommentMultiLines);
	bool SplitCommande(AnsiString &asLine, AnsiString *asCommande, AnsiString *asLine2);
	bool SplitVarName(AnsiString &asLine, AnsiString *asVarName, AnsiString *asLine2);
	bool SplitParenthesis(AnsiString &asLine, AnsiString *asParenthesis, AnsiString *asLine2);
	bool SplitWords(AnsiString &asLine, std::vector<AnsiString> *sMots);
	AnsiString SuppComment(AnsiString asLine, bool *bCommentMultiLines);
	AnsiString SuppSpaces(AnsiString asLine);
	AnsiString GetFullName(AnsiString asPath, AnsiString asFileName);
	AnsiString RechercheInclude(AnsiString asFileName);
	bool TestCoherenceInclude(AnsiString asFileName, int NumLine, bool *bIncludeTwoTimes, bool bSysFile);
	int IndexOfVar(AnsiString asVarName);
	bool EvalBool(AnsiString asFileName, int NumLine, AnsiString asLine);
	int EvalInt(AnsiString asFileName, int NumLine, AnsiString asLine);
	AnsiString EvalString(AnsiString asFileName, int NumLine, AnsiString asLine);
	void PushIfStatus(void);
	void PopIfStatus(void);
public:
  TPreCompiler(CALLBACK_DEFVAR ACallbackDefVar);

  //! @name Méthodes
  //@{

  bool PreCompile(AnsiString asFileName);

  //@}


  //! @name Propriétés
  //@{

  //@}

};

//---------------------------------------------------------------------------

#endif /* PreCompilation_H */
