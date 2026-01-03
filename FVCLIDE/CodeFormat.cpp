//-----------------------------------------------------------------------------
//! @file CodeFormat.cpp
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

//***********
/* Comment */
//***********

#include _T("stdafx.h")
#include _T("CodeFormat.h")
#include <commctrl.h>
#include <shlwapi.h>
#include <StrSafe.h>

#define MAX_LOADSTRING 100

// Global Variables:

HINSTANCE hInst; // current instance

bool bAccALaLigne = true;
bool bSpaceAfterParenthesis = false;

char * OneLetterOp[] =
{
	_T("&"),
	_T("|"),
	_T("="),
	_T(">"),
	_T("<"),
	_T("+"),
	_T("-"),
	_T("*"),
	_T("/"),
	_T("|"),
	_T("&"),
	_T("^")
};

char * TwoLettersOp[] =
{
	_T("&&"),
	_T("||"),
	_T("=="),
	_T("!="),
	_T(">="),
	_T("<="),
	_T("+="),
	_T("-="),
	_T("*="),
	_T("/="),
	_T("|="),
	_T("&="),
	_T("^="),
	_T("//"),
	_T("/*"),
	_T("*/"),
	_T("++"),
	_T("--"),
	_T("->")
} ;

char * TwoLettersOpNS[] =
{
	_T("++"),
	_T("--"),
	_T("->")
};

char * TwoLettersComment[] =
{
	_T("//"),
	_T("/*"),
	_T("*/")
};

// Forward declarations of functions included in this code module:


bool CodeFormat(TCHAR * szFileName);
bool CodeFormat(HANDLE hFileIn, HANDLE hFileOut);
bool IsOneLetterOp(char c);
bool IsTwoLettersOp(char c1, char c2);
bool IsTwoLettersOpNS(char c1, char c2);
bool IsTwoLettersComment(char c1, char c2);

int APIENTRY _tWinMain(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR lpCmdLine,
		int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	TCHAR szFileName[_MAX_FNAME];
	int Len;

	StringCchCopy(szFileName, _MAX_FNAME, lpCmdLine);

	Len = lstrlen(szFileName); /* Comment */
	if (Len == 0)
	{
		MessageBox(NULL, L_T("Usage: CodeFormat [\"]File[\"]"), L_T("Error"), MB_OK | MB_ICONSTOP);
		return 0;
	}


	if (szFileName[0] == _T('\"') && szFileName[Len - 1] == _T('\"'))
	{
		StringCchCopy(szFileName, Len - 1, & lpCmdLine[1]);
	}

	if (CodeFormat(szFileName))
	{
		MessageBox(NULL, L_T("Process succesfully terminated"), lpCmdLine, MB_OK);
	}

	return 0;
}


bool CodeFormat(TCHAR * szFileName)
{

	HANDLE hFileIn, hFileOut;
	TCHAR szFileNameOld[_MAX_FNAME];
	bool Ok;


	StringCchCopy(szFileNameOld, _MAX_FNAME, szFileName);
	StringCchCopy(PathFindExtension(szFileNameOld), 1, L_T(""));
	StringCchCat(szFileNameOld, _MAX_FNAME, L_T(".old"));

	if (!CopyFile(szFileName, szFileNameOld, FALSE))
	{
		MessageBox(NULL, L_T("Impossible de copier le fichier"), szFileName, MB_OK);
		return false;
	}

	hFileIn = CreateFileW(szFileNameOld, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileIn == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L_T("Impossible d'ouvrir le fichier"), szFileName, MB_OK);
		return false;
	}

	hFileOut = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileOut == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFileIn);
		MessageBox(NULL, L_T("Impossible d'ouvrir le fichier"), szFileNameOld, MB_OK);
		return false;
	}

	Ok = CodeFormat(hFileIn, hFileOut);

	CloseHandle(hFileIn);
	CloseHandle(hFileOut);

	return Ok;
}

bool CodeFormat(HANDLE hFileIn, HANDLE hFileOut)
{
	char dc, c, pc;
	DWORD dw;
	int CRLF = 0;
	int NbAcc = 0;
	bool bSpace = false;
	bool bGuill1 = false;
	bool bGuill2 = false;
	bool bComment1 = false;
	bool bComment2 = false;
	bool bPrecomp = false;
	int Indent = 0;
	int Indent2 = 0;
	int Indent3 = 0;
	int Parenth = 0;
	bool bWrite;
	int LevelCase = 0;
	int i;


	dc = _T('\0');
	c = _T('\0');
	pc = _T('\0');
	ReadFile(hFileIn, & pc, 1, & dw, NULL);
	while (pc != _T('\0'))
	{
		dc = c;
		c = pc;
		pc = _T('\0');
		ReadFile(hFileIn, & pc, 1, & dw, NULL);

		if (bComment1)
		{
			if (c == _T('\r'))
			{
				bComment1 = false;
				CRLF = 1;
			}
			else
			{
				WriteFile(hFileOut, & c, 1, & dw, NULL);
			}
		}

		else if (bComment2)
		{
			WriteFile(hFileOut, & c, 1, & dw, NULL);
			if (c == _T('/') && dc == _T('*')) bComment2 = false;
		}

		else if (bGuill1)
		{
			WriteFile(hFileOut, & c, 1, & dw, NULL);
			if (dc == _T('\\') c = ')\0_T(';
			if (c == _T('\"') && dc != _T('\\') bGuill1 = false;
		}

		else if (bGuill2)
		{
			WriteFile(hFileOut, & c, 1, & dw, NULL);
			if (dc == _T('\\') c = ')\0_T(';
			if (c == _T('\'') && dc != _T('\\') bGuill2 = false;
		}

		else
		{

			bWrite = true;

			switch (c)
			{
				case _T(' '):
				case _T('\t'):
					bSpace = true;
					if (dc == _T('(') && !bSpaceAfterParenthesis) bSpace = false;
					bWrite = false;
					break;
				case _T('#'):
					bPrecomp = true;
					break;
				case _T('\"'):
					bGuill1 = true;
					break;
				case _T('\''):
					bGuill2 = true;
					break;
				case _T('('):
          if (!bPrecomp)
          {
            if (Parenth == 0) Parenth = 2;
            else Parenth++ ;
            Indent2 = 2;
          }
          else
          {
            else Parenth++;
            Indent2 = 1;
          }
					break;
				case _T(')'):
          if (!bPrecomp)
          {
            if (Parenth > 0) Parenth-- ;
            if (Parenth == 1) Indent2 = 1;
            else if (Parenth == 0) Indent2 = 0;
          }
          else
          {
            if (Parenth > 0) Parenth-- ;
            if (Parenth == 0) Indent2 = 0;
          }
					break;
				case _T(';'):
					if (Parenth == 1)
					{
						Parenth = 0;
						Indent2 = 0;
					}
					break;
				case _T('{'):
					NbAcc++;
					bWrite = false;
					Indent2 = 0;
					if (CRLF) CRLF-- ;
					break;
				case _T('}'):
					while (NbAcc)
					{
						WriteFile(hFileOut, _T("{"), 1, & dw, NULL);
						NbAcc--;
						Indent++;
					}
					if (Indent > 0) Indent-- ;
					if (CRLF)
					{
						while (CRLF--) WriteFile(hFileOut, _T("\r\n"), 2, & dw, NULL);
						CRLF = 0;
						if (Indent <= LevelCase)
						{
							Indent3 = 0;
							LevelCase = 0;
						}
						if (!bPrecomp)
						{
							for (i = 0; i < Indent + Indent2 + Indent3; i++)
							{
								WriteFile(hFileOut, _T("\t"), 1, & dw, NULL);
							}
						}
					}
					NbAcc = 0;
					WriteFile(hFileOut, & c, 1, & dw, NULL);
					bWrite = false;
					break;
				case _T('\r'):
					CRLF++ ;
					if (dc == _T(':'))
					{
						Indent3 = 1;
						LevelCase = Indent - 1;
						if (Parenth == 1)
						{
							Parenth = 0;
							Indent2 = 0;
						}
					}
					bPrecomp = false;
					bWrite = false;
					break;
				case _T('\n'):
					bWrite = false;
					break;
				default:
					if (CRLF == 0 && NbAcc == 0 && !bPrecomp && _T('a') <= c && c <= _T('z'))
					{
						if (Parenth == 0)
						{
							Parenth = 1;
							Indent2 = 1;
						}
					}
					break;
			}

			if (bWrite)
			{

				if (!IsTwoLettersComment(c, pc))
				{
					if (IsTwoLettersOpNS(c, pc)) bSpace = false;
					else if (IsTwoLettersOp(c, pc)) bSpace = true;
					else if (IsOneLetterOp(c) && !IsTwoLettersOp(dc, c)) bSpace = true;
					if (c == _T(',')) bSpace = false;
					if (c == _T(')')) bSpace = bSpaceAfterParenthesis;
					if (bPrecomp && c == _T('>')) bSpace = false;
				}

				if (CRLF)
				{
					while (NbAcc)
					{
						if (bAccALaLigne)
						{
							WriteFile(hFileOut, _T("\r\n"), 2, & dw, NULL);
							if (!bPrecomp)
							{
								for (i = 0; i < Indent + Indent2 + Indent3; i++)
								{
									WriteFile(hFileOut, _T("\t"), 1, & dw, NULL);
								}
							}
						}
						else
						{
							WriteFile(hFileOut, _T(" "), 1, & dw, NULL);
						}
						WriteFile(hFileOut, _T("{"), 1, & dw, NULL);
						NbAcc--;
						Indent++ ;
					}
					while (CRLF--) WriteFile(hFileOut, _T("\r\n"), 2, & dw, NULL);
					CRLF = 0;
					if (!bPrecomp)
					{
						for (i = 0; i < Indent + Indent2; i++)
						{
							WriteFile(hFileOut, _T("\t"), 1, & dw, NULL);
						}
						if (Indent3 != 0 &&
              (c != _T('c') || pc != _T('a')) &&  // case
              (c != _T('d') || pc != _T('e')) &&  // default
              (c != _T('p') || pc != _T('r')) &&  // private, protected
              (c != _T('p') || pc != _T('u')))    // public
						{
							WriteFile(hFileOut, _T("\t"), 1, & dw, NULL);
						}
					}
					bSpace = false;
				}
				if (bSpace)
				{
					WriteFile(hFileOut, _T(" "), 1, & dw, NULL);
					bSpace = false;
				}
				while (NbAcc)
				{
					WriteFile(hFileOut, _T("{"), 1, & dw, NULL);
					NbAcc--;
					Indent++ ;
				}

				WriteFile(hFileOut, & c, 1, & dw, NULL);

			}

			if (dc == _T('/') && c == _T('/')) bComment1 = true;
			if (dc == _T('/') && c == _T('*')) bComment2 = true;
			if (bComment1 || bComment2)
			{
				if (bSpace)
				{
					WriteFile(hFileOut, _T(" "), 1, & dw, NULL);
					bSpace = false;
				}
				while (NbAcc)
				{
					WriteFile(hFileOut, _T("{"), 1, & dw, NULL);
					NbAcc--;
					Indent++;
				}
			}
			else
			{
				if (IsTwoLettersOpNS(dc, c)) bSpace = false;
				else if (IsTwoLettersOp(dc, c)) bSpace = true;
				else if (IsOneLetterOp(c) && !IsTwoLettersOp(c, pc)) bSpace = true;
				if (c == _T(',')) bSpace = true;
				if (c == _T('(')) bSpace = bSpaceAfterParenthesis;
				if (bPrecomp && c == _T('<')) bSpace = false;
				if (bPrecomp && c == _T('>')) bSpace = false;
			}
		}

	}

	if (CRLF) WriteFile(hFileOut, _T("\r\n"), 2, & dw, NULL);

	return true;
}

bool IsOneLetterOp(char c)
{
	int i;

	for (i = 0; i < sizeof(OneLetterOp) / sizeof(char * *); i++)
	{
		if (c == OneLetterOp[i] [0]) return true;
	}

	return false;
}

bool IsTwoLettersOp(char c1, char c2)
{
	int i;

	for (i = 0; i < sizeof(TwoLettersOp) / sizeof(char * *); i++)
	{
		if (c1 == TwoLettersOp[i] [0] && c2 == TwoLettersOp[i] [1]) return true;
	}

	return false;
}

bool IsTwoLettersOpNS(char c1, char c2)
{
	int i;

	for (i = 0; i < sizeof(TwoLettersOpNS) / sizeof(char * *); i++)
	{
		if (c1 == TwoLettersOpNS[i] [0] && c2 == TwoLettersOpNS[i] [1]) return true;
	}

	return false;
}

bool IsTwoLettersComment(char c1, char c2)
{
	int i;

	for (i = 0; i < sizeof(TwoLettersComment) / sizeof(char * *); i++)
	{
		if (c1 == TwoLettersComment[i] [0] && c2 == TwoLettersComment[i] [1]) return true;
	}

	return false;
}
