//-----------------------------------------------------------------------------
//! @file TApplication.cpp
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
#include <limits.h>
#include <shlobj.h>
#include <Shlwapi.h>
#include <time.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

TApplication StaticApplication;
TApplication *Application = &StaticApplication;
std::vector<int> DebFinParam;
bool bCommandLine;

int CALLBACK SelectDirectoryCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);


//---------------------------------------------------------------------------
// Fonctions globales
//---------------------------------------------------------------------------

int FASTCALL ParamCount(void) {

	if (DebFinParam.size() == 0) SplitCommandLine();
	return (int) (DebFinParam.size() / 2);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ParamStr(int i) {
	TCHAR *szRetour;
	AnsiString asRetour;
	TCHAR *lp;
	int Deb, Fin;


	if (DebFinParam.size() == 0) SplitCommandLine();
	if (i < 0 || i >= (int) DebFinParam.size() / 2) return _T("");

	lp = GetCommandLine();
	Deb = DebFinParam[2 * i];
	Fin = DebFinParam[2 * i + 1];
	szRetour = new TCHAR[Fin - Deb + 1];
	lstrcpyn(szRetour, &lp[Deb], Fin - Deb + 1);
	asRetour = szRetour;
	delete[] szRetour;

	return asRetour;
}

//---------------------------------------------------------------------------
void FASTCALL SplitCommandLine(void) {
	TCHAR *lp;
	TCHAR c;
	int i;
	bool bDebArg;
	bool bQuotes;


	if (!bCommandLine) {
		lp = GetCommandLine();
		i = 0;
		bQuotes = false;
		bDebArg = false;
		do {
			c = lp[i];
			switch (c) {
			case L'\"':
				if (bQuotes) {
					DebFinParam.push_back(i);
					bDebArg = false;
					bQuotes = false;
				}
				else {
					DebFinParam.push_back(i + 1);
					bDebArg = true;
					bQuotes = true;
				}
				break;
			case L' ':
			case L'\t':
				if (bDebArg && !bQuotes) {
					DebFinParam.push_back(i);
					bDebArg = false;
				}
				break;
			case L'\0':
				if (bDebArg) {
					DebFinParam.push_back(i);
					bDebArg = false;
				}
				break;
			default:
				if (!bDebArg) {
					DebFinParam.push_back(i);
					bDebArg = true;
				}
				break;
			}
			i++;
		} while (c);

		bCommandLine = true;
	}
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ShortCutToText(TShortCut ShortCut) {
	AnsiString sRetour;
	Word VirtualKey;


	if (ShortCut & 0x4000) sRetour += _T("Ctrl + ");
	if (ShortCut & 0x2000) sRetour += _T("Maj + ");
	if (ShortCut & 0x8000) sRetour += _T("Alt + ");
	TCHAR szKeyNameText[32];	 // 32 au pif
	VirtualKey = ShortCut & 0xFF;
	if (48 <= VirtualKey && VirtualKey < 58) {
		// Chiffres de 0 à 9 (parce que GetKeyNameText renvoie "&", "é", ...)
		szKeyNameText[0] = VirtualKey + _T('0') - 48;
		szKeyNameText[1] = _T('\0');
		sRetour += szKeyNameText;
	}
	else if (VirtualKey == VK_INSERT) sRetour += _T("INS"); 	 // Parce que GetKeyNameText renvoie "0 (PAVE NUM.)"
	else if (VirtualKey == VK_DELETE) sRetour += _T("SUP"); 	 // Idem
	else if (VirtualKey == VK_HOME) sRetour += _T("ORIGINE");
	else if (VirtualKey == VK_END) sRetour += _T("FIN");
	else if (VirtualKey == VK_UP) sRetour += _T("PG.PREC");
	else if (VirtualKey == VK_DOWN) sRetour += _T("PG.SUIV");
	else {
		LONG lParam = (MapVirtualKey(VirtualKey, 0) /*Scan code*/ << 16) + 1;
		if (GetKeyNameText(lParam, szKeyNameText, 32)) {
			sRetour += szKeyNameText;
		}
	}

	return sRetour;
}

//---------------------------------------------------------------------------
TShortCut FASTCALL TextToShortCut(AnsiString asText) {
	TShortCut ShortCut = 0;

	int i;
	AnsiString asMot;


	ShortCut = 0;
	while (!asText.IsEmpty()) {
		i = asText.Pos(_T(" "));
		if (i != 0) {
			asMot = asText.SubString(1, i - 1).UpperCase();
			asText = asText.SubString(i + 1, asText.Length() - i);
		}
		else {
			asMot = asText.UpperCase();
			asText = _T("");
		}
		if (!asMot.IsEmpty()) {
			if (asMot == _T("ALT")) ShortCut |= 0x8000;
			else if (asMot == _T("CTRL")) ShortCut |= 0x4000;
			else if (asMot == _T("MAJ") || asMot == _T("SHIFT")) ShortCut |= 0x2000;
			else if (asMot == _T("INS")) ShortCut |= VK_INSERT;  // Parce que GetKeyNameText renvoie "0 (PAVE NUM.)"
			else if (asMot == _T("SUP") || asMot == _T("DEL")) ShortCut |= VK_DELETE;
			else if (asMot == _T("ORIGINE") || asMot == _T("HOME")) ShortCut |= VK_HOME;
			else if (asMot == _T("FIN") || asMot == _T("END")) ShortCut |= VK_END;
			else if (asMot == _T("PG.PREC") || asMot == _T("PG.UP")) ShortCut |= VK_UP;
			else if (asMot == _T("PG.SUIV") || asMot == _T("PG.DOWN")) ShortCut |= VK_DOWN;
			else if (asMot != _T("+") || asText.IsEmpty()) {
				TCHAR szKeyNameText[32];	 // 32 au pif
				int Key;
				for (Key = 0; Key < 0xFF; Key++) {
					LONG lParam = (MapVirtualKey(Key, 0)	/*Scan code*/ << 16) + 1;
					if (GetKeyNameText(lParam, szKeyNameText, 32)) {
						if (asMot == AnsiString(szKeyNameText).UpperCase()) {
							ShortCut |= Key;
							break;
						}
					}
					lParam = (MapVirtualKey(VK_INSERT, 0) /*Scan code*/ << 16) + 1;
					GetKeyNameText(lParam, szKeyNameText, 32);
				}
			}
		}
	}

	return ShortCut;
}

//---------------------------------------------------------------------------
void FASTCALL ShortCutToKey(TShortCut ShortCut, WORD &Key, TShiftState &Shift) {
	Shift.Clear();
	if (ShortCut & 0x8000) Shift << ssAlt;
	if (ShortCut & 0x4000) Shift << ssCtrl;
	if (ShortCut & 0x2000) Shift << ssShift;
	Key = ShortCut & 0xFF;
}

//---------------------------------------------------------------------------
bool FASTCALL SelectDirectory(const AnsiString Caption, const AnsiString Root,
		AnsiString &Directory, TSelectDirOpts SelectDirOpts) {
	BROWSEINFO BrowseInfo;
	TCHAR szPath[MAX_PATH];
	bool Ok = false;


	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	memset(&BrowseInfo, 0, sizeof(BROWSEINFO));
	BrowseInfo.hwndOwner = Application->Handle;
	BrowseInfo.pidlRoot = NULL;
	lstrcpyn(szPath, (LPCTSTR) Root, MAX_PATH);
	BrowseInfo.pszDisplayName = szPath;
	BrowseInfo.lpszTitle = Caption;
	BrowseInfo.ulFlags = 0;
	if (SelectDirOpts.Contains(sdAllowCreate)) BrowseInfo.ulFlags |= BIF_NEWDIALOGSTYLE;
	else BrowseInfo.ulFlags |= BIF_NONEWFOLDERBUTTON;
	if (SelectDirOpts.Contains(sdShowEdit)) BrowseInfo.ulFlags |= BIF_EDITBOX;
	if (SelectDirOpts.Contains(sdShowShares)) BrowseInfo.ulFlags |= BIF_SHAREABLE;
	if (SelectDirOpts.Contains(sdNewUI)) BrowseInfo.ulFlags |= BIF_NEWDIALOGSTYLE;
	if (SelectDirOpts.Contains(sdShowFiles)) BrowseInfo.ulFlags |= BIF_BROWSEINCLUDEFILES;
	if (SelectDirOpts.Contains(sdValidateDir)) BrowseInfo.ulFlags |= BIF_VALIDATE;
	if (SelectDirOpts.Contains(sdBrowseForComputer)) BrowseInfo.ulFlags |= BIF_BROWSEFORCOMPUTER;
	if (SelectDirOpts.Contains(sdBrowseForPrinters)) BrowseInfo.ulFlags |= BIF_BROWSEFORPRINTER;
	if (SelectDirOpts.Contains(sdDontGoBelowDomain)) BrowseInfo.ulFlags |= BIF_DONTGOBELOWDOMAIN;
	if (SelectDirOpts.Contains(sdReturnFSAncestors)) BrowseInfo.ulFlags |= BIF_RETURNFSANCESTORS;
	if (SelectDirOpts.Contains(sdReturnOnlyFSDirs)) BrowseInfo.ulFlags |= BIF_RETURNONLYFSDIRS;
	if (SelectDirOpts.Contains(sdBrowseFileJunctions)) BrowseInfo.ulFlags |= BIF_BROWSEFILEJUNCTIONS;
	if (Directory.IsEmpty()) {
		BrowseInfo.lpfn = NULL;
		BrowseInfo.lParam = 0;
	}
	else {
		BrowseInfo.lpfn = SelectDirectoryCallbackProc;
		BrowseInfo.lParam = (LPARAM) (LPCTSTR) Directory;
	}
	BrowseInfo.iImage = 0;

	LPITEMIDLIST lpItemIDList = SHBrowseForFolder(&BrowseInfo);

	if (lpItemIDList != NULL) {
		SHGetPathFromIDList(lpItemIDList, szPath);
		Directory = szPath;
		CoTaskMemFree(lpItemIDList);
		Ok = true;
	}

	CoUninitialize();

	return Ok;
}

//---------------------------------------------------------------------------
int CALLBACK SelectDirectoryCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
	if (uMsg == BFFM_INITIALIZED) {
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

//---------------------------------------------------------------------------
bool FASTCALL FileExists(const AnsiString asFileName) {
	return (GetFileAttributes(asFileName) != (DWORD) -1);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL FileNameStandardFormat(const AnsiString asFileName) {
	return asFileName.LowerCase();
}

//---------------------------------------------------------------------------
bool FASTCALL DeleteFile(const AnsiString asFileName, bool bToRecycleBin) {

	if (bToRecycleBin) {
		SHFILEOPSTRUCT fos;
		AnsiString asListFileNames;


		// Liste des fichiers
		asListFileNames = asFileName + _T('\0') + _T('\0');

		memset(&fos, 0, sizeof(fos));
		fos.hwnd = Application->Handle;
		fos.wFunc = FO_DELETE;
		fos.pFrom = const_cast<TCHAR *>((LPCTSTR) asListFileNames);
		fos.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;

		return (SHFileOperation(&fos) == 0);
	}
	else {
		return (::DeleteFile((LPCTSTR) asFileName) == TRUE);
	}

}

//---------------------------------------------------------------------------
bool FASTCALL ChDir(const AnsiString asDir) {
	return (::SetCurrentDirectory(asDir) == TRUE);
}

//---------------------------------------------------------------------------
bool FASTCALL CreateDir(const AnsiString asDir) {
	return (::CreateDirectory(asDir, NULL) == TRUE);
}

//---------------------------------------------------------------------------
bool FASTCALL RenameFile(const AnsiString OldName, const AnsiString NewName) {
	AnsiString asNewPathName;

	asNewPathName = ExtractFilePath(OldName) + NewName;
	return (::MoveFile(OldName, asNewPathName) == TRUE);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ExtractFileDir(const AnsiString FileName) {
	int PosAntiSlash;


	PosAntiSlash = FileName.LastDelimiter(_T(":\\/"));
	if (PosAntiSlash == 0) return _T("");
	return FileName.SubString(1, PosAntiSlash - 1);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ExtractFileDrive(const AnsiString FileName) {
	int PosDoubleDot;


	PosDoubleDot = FileName.LastDelimiter(_T(":"));
	if (PosDoubleDot == 0) return _T("");
	return FileName.SubString(1, PosDoubleDot - 1);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ExtractFileExt(const AnsiString FileName) {
	int PosDot;


	PosDot = FileName.LastDelimiter(_T("."));
	if (PosDot == 0) return _T("");
	return FileName.SubString(PosDot, FileName.Length() - PosDot + 1);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ExtractFileName(const AnsiString FileName) {
	int PosAntiSlash;


	PosAntiSlash = FileName.LastDelimiter(_T(":\\/"));
	return FileName.SubString(PosAntiSlash + 1, FileName.Length() - PosAntiSlash);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ExtractFilePath(const AnsiString FileName) {
	int PosAntiSlash;


	PosAntiSlash = FileName.LastDelimiter(_T(":\\/"));
	if (PosAntiSlash == 0) return _T("");
	// Le path comprend le '\' à la fin (contrairement à ExtractFileDir) :
	return FileName.SubString(1, PosAntiSlash);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ChangeFileExt(const AnsiString FileName,
																	const AnsiString Extension) {
	int PosDot;
	AnsiString asNewFileName;
	AnsiString asExtension;


	if (!Extension.IsEmpty() && Extension[1] != '.') {
		asExtension = AnsiString(".") + Extension;
	}
	else {
		asExtension = Extension;
	}
	PosDot = FileName.LastDelimiter(_T("."));
	if (PosDot == 0) PosDot = FileName.Length() + 1;
	asNewFileName = FileName.SubString(1, PosDot - 1) + asExtension;

	return asNewFileName;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL StringReplace(const AnsiString S,
																	const AnsiString OldPattern,
																	const AnsiString NewPattern,
																	TReplaceFlags Flags) {
	AnsiString asRet;
	AnsiString OldPatternUpper;
	size_t PosPattern;

	asRet = S;
	PosPattern = 0;

	if (Flags.Contains(rfIgnoreCase)) {
		OldPatternUpper = OldPattern.UpperCase();
		PosPattern = asRet.UpperCase().find(OldPatternUpper, PosPattern);
	}
	else {
		PosPattern = asRet.find(OldPattern, PosPattern);
	}

	while (PosPattern != std::string::npos) {

		asRet = asRet.replace(PosPattern, OldPattern.Length(), NewPattern);
		PosPattern += NewPattern.Length();

		if (!Flags.Contains(rfReplaceAll)) break;

		if (Flags.Contains(rfIgnoreCase)) {
			PosPattern = asRet.UpperCase().find(OldPatternUpper, PosPattern);
		}
		else {
			PosPattern = asRet.find(OldPattern, PosPattern);
		}

	}

	return asRet;
}

//---------------------------------------------------------------------------
TShiftState FASTCALL GetShiftState(void) {
	TShiftState Shift;

	Shift.Clear();
	if (GetKeyState(VK_MENU) & 0x8000) Shift << ssAlt;
	if (GetKeyState(VK_SHIFT) & 0x8000) Shift << ssShift;
	if (GetKeyState(VK_CONTROL) & 0x8000) Shift << ssCtrl;

	return Shift;
}

//---------------------------------------------------------------------------
// Entrée d'une valeur au clavier
//---------------------------------------------------------------------------

AnsiString FASTCALL InputBox(const AnsiString ACaption, const AnsiString APrompt,
														 const AnsiString ADefault, int WidthEditControl) {
	TForm *Form;
	TLabel *Label;
	TEdit *Edit;
	TButton *ButtonOk;
	TButton *ButtonCancel;
	TAnchors Anchors;
	AnsiString asRet;
	int WidthLabel;
	HDC hdc;
	SIZE SizeText;


	hdc = GetDC(NULL);
	SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	GetTextExtentPoint32(hdc, APrompt, APrompt.Length(), &SizeText);
	WidthLabel = SizeText.cx;
	ReleaseDC(NULL, hdc);

	Form = new TForm(Application->Handle);
	Form->ClientWidth = WidthLabel + WidthEditControl + 20;  // 8 + 8 + 4
	Form->ClientHeight = 80;
	Form->Constraints->MinWidth = Form->Width;
	Form->Constraints->MinHeight = Form->Height;
	Form->Constraints->MaxHeight = Form->Height;
	Form->Caption = ACaption;
	Form->Position = poMainFormCenter;

	Label = new TLabel(Form);
	Label->Left = 8;
	Label->Top = 6;
	Label->Width = WidthLabel;
	Label->Height = 17;
	Label->Caption = APrompt;

	Edit = new TEdit(Form);
	Edit->Left = WidthLabel + 12;
	Edit->Top = 4;
	Edit->Width = WidthEditControl;
	Edit->Height = 21;
	Anchors.Clear() << akLeft << akTop << akRight;
	Edit->Anchors = Anchors;
	Edit->Text = ADefault;
	Edit->TabOrder = 0;

	ButtonOk = new TButton(Form);
	ButtonOk->Left = (Form->ClientWidth - 164) / 2;  // 72 * 2 + 20
	ButtonOk->Top = 40;
	ButtonOk->Width = 72;
	ButtonOk->Height = 24;
	Anchors.Clear() << akLeft << akBottom;
	ButtonOk->Anchors = Anchors;
	ButtonOk->Caption = _TT("Ok");
	ButtonOk->ModalResult = mrOk;
	ButtonOk->Default = true;
	ButtonOk->TabOrder = 1;

	ButtonCancel = new TButton(Form);
	ButtonCancel->Left = ButtonOk->Left + ButtonOk->Width + 20;
	ButtonCancel->Top = 40;
	ButtonCancel->Width = 72;
	ButtonCancel->Height = 24;
	ButtonCancel->Anchors = Anchors;
	ButtonCancel->Caption = _TT("Annuler");
	ButtonCancel->ModalResult = mrCancel;
	ButtonCancel->Cancel = true;
	ButtonCancel->TabOrder = 2;

	if (Form->ShowModal() == mrOk) {
		asRet = Edit->Text;
	}
	else {
		asRet = ADefault;
	}

	delete Form;

	return asRet;
}


//---------------------------------------------------------------------------
// TApplication
//---------------------------------------------------------------------------

TApplication::TApplication(void): TPersistent() {
	// Initialisations
	FMainForm = NULL;
	FHandle = NULL;
	FIcon = NULL;
	FHintPause = 500;
	FHintHidePause = 2500;
	hTranslate = (HINSTANCE) -1;
}

TApplication::~TApplication(void) {
	if (hTranslate) FreeLibrary(hTranslate);
	delete FIcon;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété MainForm
//---------------------------------------------------------------------------

TForm * TApplication::Get_MainForm(void) {
	return FMainForm;
}

//---------------------------------------------------------------------------
bool TApplication::Set_MainForm(TForm *NewMainForm) {
	if (FMainForm == NULL || NewMainForm == NULL) {
		FMainForm = NewMainForm;
		if (NewMainForm) FHandle = NewMainForm->Handle;
		else FHandle = NULL;
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TApplication::Get_Handle(void) {
	return FHandle;
}

//---------------------------------------------------------------------------
bool TApplication::Set_Handle(HWND NewHandle) {
	if (FHandle == NULL) {
		FHandle = NewHandle;
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Icon
//---------------------------------------------------------------------------

TIcon * TApplication::Get_Icon(void) {
	return FIcon;
}

//---------------------------------------------------------------------------
bool TApplication::Set_Icon(TIcon *NewIcon) {
	HICON hIcon;

	if (NewIcon != NULL) {
		if (FIcon == NULL) FIcon = new TIcon();

		FIcon->Assign(NewIcon);
		if (FMainForm) {
			hIcon = FIcon->Handle;
			SetClassLongPtr(FMainForm->Handle, GCLP_HICON, (LONG_PTR) (SIZE_T) hIcon);
			DrawMenuBar(FMainForm->Handle);
		}
	}
	else {
		delete FIcon;
		FIcon = NULL;
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ExeName
//---------------------------------------------------------------------------

AnsiString TApplication::Get_ExeName(void) {
	TCHAR szModuleFileName[MAX_PATH];

	GetModuleFileName(NULL, szModuleFileName, MAX_PATH);

	return AnsiString(szModuleFileName);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ExePath
//---------------------------------------------------------------------------

AnsiString TApplication::Get_ExePath(void) {
	TCHAR szModuleFilePath[MAX_PATH];


	GetModuleFileName(NULL, szModuleFilePath, MAX_PATH);

	return ExtractFileDir(szModuleFilePath);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété HelpFile
//---------------------------------------------------------------------------

AnsiString TApplication::Get_HelpFile(void) {
	return FHelpFile;
}

//---------------------------------------------------------------------------
bool TApplication::Set_HelpFile(AnsiString NewHelpFile) {

	if (FHelpFile != NewHelpFile) {
		FHelpFile = NewHelpFile;
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété HintPause
//---------------------------------------------------------------------------

int TApplication::Get_HintPause() {
	return FHintPause;
}

bool TApplication::Set_HintPause(int NewHintPause) {
	if (FHintPause != NewHintPause) {
		FHintPause = NewHintPause;
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HintHidePause
//---------------------------------------------------------------------------

int TApplication::Get_HintHidePause() {
	return FHintHidePause;
}

bool TApplication::Set_HintHidePause(int NewHintHidePause) {
	if (FHintHidePause != NewHintHidePause) {
		FHintHidePause = NewHintHidePause;
	}
	return true;
}


//---------------------------------------------------------------------------
void FASTCALL TApplication::Initialize(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TApplication::CreateForm(TForm *Instance, void *Reference) {
	FVCL_ASSERT(Instance != NULL && "Instance == NULL dans CreateForm");
	FVCL_ASSERT(Reference != NULL && "Reference == NULL dans CreateForm");
	*((void **) Reference) = (void *) Instance;
}

//---------------------------------------------------------------------------
int FASTCALL TApplication::Run(void) {
	MSG msg;


	if (!FMainForm || !FMainForm->Handle) return -1;

	// Boucle de messages:
	while (GetMessage(&msg, 0, 0, 0)) {
		if (!FMainForm->InterceptionMessage(msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//---------------------------------------------------------------------------
void FASTCALL TApplication::ProcessMessages(void) {
	MSG msg;


	if (FMainForm && FMainForm->Handle) {

		// Boucle de messages:
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (!FMainForm->InterceptionMessage(msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

	}

}

//---------------------------------------------------------------------------
void FASTCALL TApplication::Minimize(void) {
	if (FMainForm) ShowWindow(FMainForm->Handle, SW_MINIMIZE);
}

//---------------------------------------------------------------------------
void FASTCALL TApplication::Restore(void) {
	if (FMainForm) ShowWindow(FMainForm->Handle, SW_RESTORE);
}

//---------------------------------------------------------------------------
void FASTCALL TApplication::ShowException(Exception* E) {
	int Ret;
	Ret = ::MessageBoxA(FHandle, E->what(), "Error exception", MB_ABORTRETRYIGNORE | MB_ICONWARNING);
	if (Ret == IDABORT) Terminate();
}

//---------------------------------------------------------------------------
void FASTCALL TApplication::Terminate(void) {
	if (FMainForm) DestroyWindow(FMainForm->Handle);
}

//---------------------------------------------------------------------------
bool FASTCALL TApplication::HelpCommand(int Command, THelpContext Data) {
	AnsiString asHelpFile;
	AnsiString asHelpFileExt;

	asHelpFile = Translate(FHelpFile);
	if (asHelpFile.LastDelimiter(_T(":\\/")) == 0) {
		asHelpFile = Get_ExePath() + DIR_DELIMITER + asHelpFile;
	}
	asHelpFileExt = ExtractFileExt(asHelpFile).LowerCase();
	if (asHelpFileExt == _T(".hlp")) {
		return (WinHelp(FHandle, asHelpFile, Command, (ULONG_PTR) Data) != FALSE);
	}
	else if (asHelpFileExt == _T(".chm")) {
		HINSTANCE Ret;
		AnsiString asArgs;
		if (Command == HELP_CONTEXT) {
			asArgs.sprintf(_T("%s::/%s"), (LPCTSTR) asHelpFile, (LPTSTR) (SIZE_T) Data);
			Ret = ShellExecute(FHandle, _T("open"), _T("hh.exe"), asArgs, NULL, SW_NORMAL);
		}
		else {
			Ret = ShellExecute(FHandle, _T("open"), asHelpFile, NULL, NULL, SW_NORMAL);
		}
		return (Ret > (HINSTANCE) 32);
	}
	else {
		HINSTANCE Ret;
		Ret = ShellExecute(FHandle, _T("open"), asHelpFile, NULL, NULL, SW_NORMAL);
		return (Ret > (HINSTANCE) 32);
	}
}

//---------------------------------------------------------------------------
// Ouvre une info-bulle.
//---------------------------------------------------------------------------
void FASTCALL TApplication::ActivateHint(TPoint CursorPos) {
	// $$$
}

//---------------------------------------------------------------------------
// Ferme l'info-bulle.
//---------------------------------------------------------------------------
void FASTCALL TApplication::HideHint(void) {
	// $$$
}

//---------------------------------------------------------------------------
// Empêche l'ouverture d'une info-bulle.
//---------------------------------------------------------------------------
void FASTCALL TApplication::CancelHint(void) {
	// $$$
}

//---------------------------------------------------------------------------
// Affiche à l'utilisateur le message spécifié.
//---------------------------------------------------------------------------
int FASTCALL TApplication::MessageBox(const TCHAR * Text,
		const TCHAR * Caption, int Flags) {
	return ::MessageBox(Get_Handle(), Text, Caption, Flags);
}

//---------------------------------------------------------------------------
bool FASTCALL TApplication::ChooseLanguage(void) {
	OpenDllTranslate();
	if (!hTranslate) return false;
	if (!FMainForm) return false;

	return (Translate_ChooseLanguage(FMainForm->Handle) == TRUE);
}

//---------------------------------------------------------------------------
bool FASTCALL TApplication::SetLanguage(AnsiString asNewLanguage) {
	OpenDllTranslate();
	if (!hTranslate) return false;

	return (Translate_SetLanguage(asNewLanguage.AsCharString()) == TRUE);
}

//---------------------------------------------------------------------------
AnsiString FASTCALL TApplication::Translate(AnsiString asInput) {
	OpenDllTranslate();

	if (hTranslate) return Translate_Translate(asInput.AsCharString());
	else return asInput;
}

//---------------------------------------------------------------------------
void FASTCALL TApplication::OpenDllTranslate(void) {
	if (hTranslate == (HINSTANCE) -1) {
		hTranslate = LoadLibrary(_T("Translate.dll"));
		if (hTranslate) {
			Translate_ChooseLanguage = (TRANSLATE_CHOOSELANGUAGE) (void *) GetProcAddress(hTranslate, "Translate_ChooseLanguage@4");  // GetProcAdress(HMODULE, LPCSTR) => donc pas de _T()
			Translate_GetLanguage = (TRANSLATE_GETLANGUAGE) (void *) GetProcAddress(hTranslate, "Translate_GetLanguage@0");
			Translate_SetLanguage = (TRANSLATE_SETLANGUAGE) (void *) GetProcAddress(hTranslate, "Translate_SetLanguage@4");
			Translate_Translate = (TRANSLATE_TRANSLATE) (void *) GetProcAddress(hTranslate, "Translate_Translate@4");
			Translate_Add = (TRANSLATE_ADD) (void *) GetProcAddress(hTranslate, "Translate_Add@8");
			if (Translate_ChooseLanguage == NULL &&
					Translate_GetLanguage == NULL &&
					Translate_SetLanguage == NULL &&
					Translate_Translate == NULL &&
					Translate_Add == NULL) {
				// Pas de décorations en 64 bits (car "@4" se transformerait en "@8")
				Translate_ChooseLanguage = (TRANSLATE_CHOOSELANGUAGE) (void *) GetProcAddress(hTranslate, "Translate_ChooseLanguage");  // GetProcAdress(HMODULE, LPCSTR) => donc pas de _T()
				Translate_GetLanguage = (TRANSLATE_GETLANGUAGE) (void *) GetProcAddress(hTranslate, "Translate_GetLanguage");
				Translate_SetLanguage = (TRANSLATE_SETLANGUAGE) (void *) GetProcAddress(hTranslate, "Translate_SetLanguage");
				Translate_Translate = (TRANSLATE_TRANSLATE) (void *) GetProcAddress(hTranslate, "Translate_Translate");
				Translate_Add = (TRANSLATE_ADD) (void *) GetProcAddress(hTranslate, "Translate_Add");
			}
			if (Translate_ChooseLanguage == NULL ||
					Translate_GetLanguage == NULL ||
					Translate_SetLanguage == NULL ||
					Translate_Translate == NULL ||
					Translate_Add == NULL) hTranslate = NULL;
		}
	}

}

//---------------------------------------------------------------------------
bool FASTCALL TApplication::ProcessQueryEndSession(void) {
	bool Accept;


	Accept = true;
	OnQueryEndSession(this, &Accept);

	return Accept;
}

//---------------------------------------------------------------------------
bool FASTCALL TApplication::ProcessEndSession(void) {

	OnEndSession(this);

	return true;
}

//---------------------------------------------------------------------------

