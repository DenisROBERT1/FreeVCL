//-----------------------------------------------------------------------------
//! @file TRichEdit.cpp
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
#include <TFactory_FVCL_RichEdit.h>
#include <TRichEdit.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

extern TFactory_FVCL_RichEdit Factory_FVCL_RichEdit;
LONG TRichEdit::IndexBuf;
LONG TRichEdit::SizeBuf;


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TRichEdit::TRichEdit(TComponent* AOwner): TWinControl(AOwner) {
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = "TRichEdit";
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
  FExWindowStyle = WS_EX_CLIENTEDGE;
  FWindowStyle |= WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL;
  FTabStop = true;
  FColor = clWindow;
	FBorderStyle = bsSingle;
  FHideScrollBars = true;
  NbBeginUpdate = 0;


  hDllRichEdit = LoadLibrary(_T("RICHED20.DLL"));

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                           RICHEDIT_CLASS /*"RichEdit20A"*/,
													 _T(""),
                           FWindowStyle,
                           FLeft, FTop, FWidth, FHeight,
                           hWndParent,
                           (HMENU) (UINT_PTR) FCommand, ::HInstance, NULL);

  if (FHandle) {
    FDefAttributes = new TTextAttributes(FHandle, FALSE);
    FSelAttributes = new TTextAttributes(FHandle, TRUE);
    FParaAttributes = new TParaAttributes(FHandle);
    FLines = new TStringList();
    SET_EVENT(FLines, TNotifyEvent, OnChange, TRichEdit, this, StringListChange);
    SendMessage(FHandle, EM_SETEVENTMASK, 0, (LPARAM) (DWORD) ENM_CHANGE);
    m_pIRichEditOleCallback = new IExRichEditOleCallback;
    SendMessage(FHandle, EM_SETOLECALLBACK, 0, (LPARAM) (size_t) m_pIRichEditOleCallback);
    bNoChange = false;

    InitStdControl();
    bProcessCreateToDo = true;

  }

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TRichEdit::~TRichEdit() {
  delete FLines;
  delete FDefAttributes;
  delete FSelAttributes;
  delete FParaAttributes;
  FreeLibrary(hDllRichEdit);
}

//---------------------------------------------------------------------------
void TRichEdit::ReCreateWindow(void) {
  HWND hWndParent = NULL;
  HWND NewHandle;


  if (FOwner) hWndParent = ((TControl *) FOwner)->Handle;

  // Création de la boîte
  NewHandle = CreateWindowEx(FExWindowStyle,
                             RICHEDIT_CLASS /*"RichEdit20A"*/,
														 _T(""),
                             FWindowStyle,
                             FLeft, FTop, FWidth, FHeight,
                             hWndParent,
                             (HMENU) (UINT_PTR) FCommand, ::HInstance, NULL);

  if (NewHandle) {
    DestroyWindow(FHandle);
    FHandle = NewHandle;
    FDefAttributes->HandleParent = FHandle;
    FSelAttributes->HandleParent = FHandle;
    FParaAttributes->HandleParent = FHandle;
    SendMessage(FHandle, EM_SETEVENTMASK, 0, (LPARAM) (DWORD) ENM_CHANGE);
    m_pIRichEditOleCallback = new IExRichEditOleCallback;
    SendMessage(FHandle, EM_SETOLECALLBACK, 0, (LPARAM) (size_t) m_pIRichEditOleCallback);

    InitStdControl();
  }

}

//---------------------------------------------------------------------------
bool TRichEdit::ProcessCommand(WORD Notify) {
  if (Notify == EN_CHANGE) {
    if (!FComponentState.Contains(csLoading)) OnChange(this);
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TBorderStyle TRichEdit::Get_BorderStyle(void) {
	return FBorderStyle;
}

bool TRichEdit::Set_BorderStyle(TBorderStyle NewBorderStyle) {

	if (FBorderStyle != NewBorderStyle) {
		FBorderStyle = NewBorderStyle;
		if (FHandle) {
			DWORD NewWindowStyle;
			DWORD NewExWindowStyle;
			NewWindowStyle = (FWindowStyle & ~WS_BORDER);
			NewExWindowStyle = (FExWindowStyle & ~WS_EX_CLIENTEDGE);
			if (FBorderStyle == bsSingle) {
				NewWindowStyle |= WS_BORDER;
				NewExWindowStyle |= WS_EX_CLIENTEDGE;
			}
			ChangeWindowStyle(NewWindowStyle);
			ChangeExWindowStyle(NewExWindowStyle);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété WordWrap
//---------------------------------------------------------------------------

bool TRichEdit::Get_WordWrap() {
  return ((FWindowStyle & ES_AUTOHSCROLL) == 0);
}

bool TRichEdit::Set_WordWrap(bool NewWordWrap) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewWordWrap) NewWindowStyle &= (DWORD) ~(WS_HSCROLL | ES_AUTOHSCROLL);
  else NewWindowStyle |= WS_HSCROLL | ES_AUTOHSCROLL;
  /*
  // Ca marche pô
  ChangeWindowStyle(NewWindowStyle);
  ChangeExWindowStyle(NewExWindowStyle);
  */
  if (FWindowStyle != NewWindowStyle) {
    FWindowStyle = NewWindowStyle;
    if (FHandle) ReCreateWindow();
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HideScrollBars
//---------------------------------------------------------------------------

bool TRichEdit::Get_HideScrollBars() {
  return FHideScrollBars;
}

bool TRichEdit::Set_HideScrollBars(bool NewHideScrollBars) {
  if (FHideScrollBars != NewHideScrollBars) {
    FHideScrollBars = NewHideScrollBars;
		if (FHandle) {
			DWORD NewWindowStyle;
			NewWindowStyle = FWindowStyle & ~(WS_HSCROLL | WS_VSCROLL);
			if (!FHideScrollBars) {
				NewWindowStyle |= (WS_HSCROLL | WS_VSCROLL);
			}
			ChangeWindowStyle(NewWindowStyle);
		}
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelStart
//---------------------------------------------------------------------------

int TRichEdit::Get_SelStart(void) {
  CHARRANGE CharRange;


  if (FHandle) {
    SendMessage(FHandle, EM_EXGETSEL, 0, (LPARAM) &CharRange);
  }

  return CharRange.cpMin;
}

bool TRichEdit::Set_SelStart(int NewSelStart) {
  CHARRANGE CharRange;


  if (FHandle) {
    CharRange.cpMin = NewSelStart;
    CharRange.cpMax = NewSelStart;
    SendMessage(FHandle, EM_EXSETSEL, 0, (LPARAM) &CharRange);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelLength
//---------------------------------------------------------------------------

int TRichEdit::Get_SelLength(void) {
  CHARRANGE CharRange;


  SendMessage(FHandle, EM_EXGETSEL, 0, (LPARAM) &CharRange);

  return CharRange.cpMax - CharRange.cpMin;
}

bool TRichEdit::Set_SelLength(int NewSelLength) {
  CHARRANGE CharRange;


  if (FHandle) {
    SendMessage(FHandle, EM_EXGETSEL, 0, (LPARAM) &CharRange);
    CharRange.cpMax = CharRange.cpMin + NewSelLength;
    SendMessage(FHandle, EM_EXSETSEL, 0, (LPARAM) &CharRange);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DefAttributes
//---------------------------------------------------------------------------

TTextAttributes * TRichEdit::Get_DefAttributes() {
  return FDefAttributes;
}

bool TRichEdit::Set_DefAttributes(TTextAttributes * NewDefAttributes) {

  FDefAttributes->Assign(NewDefAttributes);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SelAttributes
//---------------------------------------------------------------------------

TTextAttributes * TRichEdit::Get_SelAttributes() {
  return FSelAttributes;
}

bool TRichEdit::Set_SelAttributes(TTextAttributes * NewSelAttributes) {

  FSelAttributes->Assign(NewSelAttributes);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ParaAttributes
//---------------------------------------------------------------------------

TParaAttributes * TRichEdit::Get_ParaAttributes() {
  return FParaAttributes;
}

bool TRichEdit::Set_ParaAttributes(TParaAttributes * NewParaAttributes) {

  FParaAttributes->Assign(NewParaAttributes);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Lines
//---------------------------------------------------------------------------

TStringList * TRichEdit::Get_Lines() {
  int BufLength;
  char *szBuf;
  EDITSTREAM EditStream;


  BufLength = (int) SendMessage(FHandle, WM_GETTEXTLENGTH, (WPARAM) 0, (LPARAM) 0) + 20000;
  szBuf = new char[BufLength];
  memset(szBuf, 0, BufLength);

  EditStream.dwCookie = (DWORD) (size_t) szBuf;
  EditStream.dwError = 0;
  EditStream.pfnCallback = ReadStreamCallback;
  SendMessage(FHandle, EM_STREAMOUT, (WPARAM) SF_RTF, (LPARAM) (EDITSTREAM FAR *) &EditStream);

  bNoChange = true;
  FLines->SetText(szBuf);
  bNoChange = false;

  delete[] szBuf;

  return FLines;
}

bool TRichEdit::Set_Lines(TStringList * NewLines) {
  if (FLines != NewLines) {
    FLines = NewLines;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ScrollBars
//---------------------------------------------------------------------------

TScrollStyle TRichEdit::Get_ScrollBars(void) {
  switch (FWindowStyle & (WS_HSCROLL | WS_VSCROLL)) {
    case WS_HSCROLL: return ssHorizontal;
    case WS_VSCROLL: return ssVertical;
    case (WS_HSCROLL | WS_VSCROLL): return ssBoth;
  }

  return ssNone;
}

bool TRichEdit::Set_ScrollBars(TScrollStyle NewScrollBars) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle & (DWORD) ~(WS_HSCROLL | WS_VSCROLL);
  if (NewScrollBars == ssHorizontal) NewWindowStyle |= (DWORD) WS_HSCROLL;
  else if (NewScrollBars == ssVertical) NewWindowStyle |= (DWORD) WS_VSCROLL;
  else if (NewScrollBars == ssBoth) NewWindowStyle |= (DWORD) (WS_HSCROLL | WS_VSCROLL);
  if (FHandle) {
    SCROLLINFO ScrollInfo;
    memset(&ScrollInfo, 0, sizeof(SCROLLINFO));
    ScrollInfo.cbSize = sizeof(SCROLLINFO);

    // Horizontale
    ScrollInfo.fMask = SIF_ALL;
    GetScrollInfo(FHandle, SB_HORZ, &ScrollInfo);
    if (NewWindowStyle & WS_HSCROLL) {
      ScrollInfo.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
    }
    else {
      ScrollInfo.fMask = SIF_ALL;
      ScrollInfo.nMin = 0;
      ScrollInfo.nMax = 0;
      ScrollInfo.nPage = 0;
      ScrollInfo.nPos = 0;
      ScrollInfo.nTrackPos = 0;
    }
    SetScrollInfo(FHandle, SB_HORZ, &ScrollInfo, TRUE);

    // Verticale
    ScrollInfo.fMask = SIF_ALL;
    GetScrollInfo(FHandle, SB_VERT, &ScrollInfo);
    if (NewWindowStyle & WS_VSCROLL) {
      ScrollInfo.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
    }
    else {
      ScrollInfo.fMask = SIF_ALL;
      ScrollInfo.nMin = 0;
      ScrollInfo.nMax = 0;
      ScrollInfo.nPage = 0;
      ScrollInfo.nPos = 0;
      ScrollInfo.nTrackPos = 0;
    }
    SetScrollInfo(FHandle, SB_VERT, &ScrollInfo, TRUE);

  }

  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
void TRichEdit::StringListChange(TObject* Sender) {
  AnsiString asText;


  if (!bNoChange) {
    asText = FLines->GetTextStr();
    SendMessage(FHandle, WM_SETTEXT, (WPARAM) 0, (LPARAM) (LPCTSTR) asText.c_str());
  }

}

//---------------------------------------------------------------------------
void FASTCALL TRichEdit::BeginUpdate(void) {
  NbBeginUpdate++;
}

//---------------------------------------------------------------------------
void FASTCALL TRichEdit::EndUpdate(void) {
  FVCL_ASSERT(NbBeginUpdate > 0 && "EndUpdate sans BeginUpdate");
  if (NbBeginUpdate > 0) {
    NbBeginUpdate--;
  }
}

//---------------------------------------------------------------------------
void FASTCALL TRichEdit::Clear(void) {
	Lines->SetText(_T(""));
}

//---------------------------------------------------------------------------
// Chargement depuis un fichier
//---------------------------------------------------------------------------

bool FASTCALL TRichEdit::LoadFromFile(const AnsiString FileName) {
	HANDLE hFile;
	DWORD dw;
	char *BufFile;
	bool Ok;


	BeginUpdate();
	Clear();
	hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD SizeLow, SizeHigh;
		DWORD FileSize;

		SizeLow = GetFileSize(hFile, &SizeHigh);
		FileSize = ((LongLong) SizeHigh << 32) + SizeLow;

		BufFile = new char[FileSize];

		ReadFile(hFile, BufFile, FileSize, &dw, NULL);
		LoadRTF(BufFile, FileSize);

		CloseHandle(hFile);

		delete[] BufFile;

		Ok = true;
  }
  else {
    // Erreur ouverture FileName
		FLastError = GetLastError();
		Ok = false;
  }
  EndUpdate();

	return Ok;
}

//---------------------------------------------------------------------------
bool TRichEdit::LoadRTF(const char *szBuf, DWORD Size) {
  EDITSTREAM EditStream;


	IndexBuf = 0;
	SizeBuf = Size;
  EditStream.dwCookie = (DWORD) (size_t) szBuf;
  EditStream.dwError = 0;
  EditStream.pfnCallback = WriteStreamCallback;
  SendMessage(FHandle, EM_STREAMIN, (WPARAM) SF_RTF, (LPARAM) (EDITSTREAM FAR *) &EditStream);

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TRichEdit::AddBitmap(TBitmap *NewBitmap) {
	IRichEditOle* pRichEditOle;


	SendMessage(FHandle, EM_GETOLEINTERFACE, 0, (LPARAM) &pRichEditOle);
	CImageDataObject::InsertBitmap(pRichEditOle, NewBitmap->Handle);

	return true;
}

//---------------------------------------------------------------------------
int FASTCALL TRichEdit::Print(const AnsiString Caption) {
  TPrinter *Pr;
  FORMATRANGE Fmt;
  TPrintDialog *PrintDialog;
  TPrintDialogOptions PrintDialogOptions;
  HDC hdcPrinter, hdcRTF;
	int Error;
	int cxPhysOffset;
	int cyPhysOffset;
	int cxPhys;
	int cyPhys;
	int cpMin;


  PrintDialog = new TPrintDialog(this);
  PrintDialogOptions = PrintDialog->Options;
  PrintDialog->Options = PrintDialogOptions << poPrintToFile;
  if (PrintDialog->Execute()) {

    Pr = Printer();
    Pr->Title = Caption;

    hdcPrinter = Pr->Handle;
    hdcRTF = GetDC(FHandle);

		cxPhysOffset = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETX);
		cyPhysOffset = GetDeviceCaps(hdcPrinter, PHYSICALOFFSETY);
    
		cxPhys = GetDeviceCaps(hdcPrinter, PHYSICALWIDTH);
		cyPhys = GetDeviceCaps(hdcPrinter, PHYSICALHEIGHT);

		SendMessage(FHandle, EM_SETTARGETDEVICE, (WPARAM) hdcPrinter, cxPhys / 2);

    Pr->BeginDoc();
    memset(&Fmt, 0, sizeof(FORMATRANGE));
    Fmt.hdc = hdcPrinter;
    Fmt.hdcTarget = hdcPrinter;
    Fmt.rc.left = CONVERT_TWIPS_X(hdcPrinter, cxPhysOffset);
    Fmt.rc.top = CONVERT_TWIPS_Y(hdcPrinter, cyPhysOffset);
    Fmt.rc.right = CONVERT_TWIPS_X(hdcPrinter, cxPhys - cxPhysOffset);
    Fmt.rc.bottom = CONVERT_TWIPS_Y(hdcPrinter, cyPhys - cyPhysOffset);
    Fmt.rcPage.left = 0;
    Fmt.rcPage.top = 0;
    Fmt.rcPage.right = CONVERT_TWIPS_X(hdcPrinter, cxPhys);
    Fmt.rcPage.bottom = CONVERT_TWIPS_Y(hdcPrinter, cyPhys);

		SendMessage(FHandle, EM_SETSEL, 0, (LPARAM) -1);          // Select the entire contents.
		SendMessage(FHandle, EM_EXGETSEL, 0, (LPARAM) &Fmt.chrg);  // Get the selection into a CHARRANGE.

		Error = 0;

		while (Fmt.chrg.cpMin < Fmt.chrg.cpMax && Error == 0) {
			if (StartPage(hdcPrinter) > 0) {
				Error = ERROR_SPL_NO_STARTDOC;
				break;
			}
        
			cpMin = (int) SendMessage(FHandle, EM_FORMATRANGE, TRUE, (LPARAM) &Fmt);
        
			if (cpMin <= Fmt.chrg.cpMin) {
				Error = ERROR_OUT_OF_PAPER;
				break;
			}
        
			Fmt.chrg.cpMin = cpMin;
			if (EndPage(hdcPrinter) > 0) {
				Error = ERROR_GEN_FAILURE;
				break;
			}
		}

		if (Error == 0) EndDoc(hdcPrinter);
		else AbortDoc(hdcPrinter);

    SendMessage(FHandle, EM_FORMATRANGE, (WPARAM) FALSE, (LPARAM) NULL);
    ReleaseDC(FHandle, hdcRTF);

  }
  delete PrintDialog;

	return Error;
}

//---------------------------------------------------------------------------
DWORD CALLBACK TRichEdit::ReadStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb) {
  memcpy((void *) dwCookie, pbBuff, cb);
  *pcb = cb;

  return 0;
}

//---------------------------------------------------------------------------
DWORD CALLBACK TRichEdit::WriteStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb) {
	DWORD SizeToRead;


	if (cb < SizeBuf - IndexBuf) SizeToRead = cb;
	else SizeToRead = SizeBuf - IndexBuf;
  memcpy(pbBuff, (void *) &((char *) dwCookie)[IndexBuf], SizeToRead);
  *pcb = SizeToRead;
	IndexBuf += SizeToRead;

  return 0;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TRichEdit::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

	ListProperties->Add(_T("BorderStyle"));
  ListProperties->Add(_T("WordWrap"));
  ListProperties->Add(_T("HideScrollBars"));
  ListProperties->Add(_T("SelStart"));
  ListProperties->Add(_T("SelLength"));
  ListProperties->Add(_T("OnChange"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TRichEdit::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("BorderStyle")) {
		*asInfos = _T("bsNone\nbsSingle");
		return tpChoice;
	}
  if (asProperty == _T("WordWrap")) {
    return tpBool;
  }
  if (asProperty == _T("HideScrollBars")) {
    return tpBool;
  }
  if (asProperty == _T("SelStart")) {
    return tpNumber;
  }
  if (asProperty == _T("SelLength")) {
    return tpNumber;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TRichEdit::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("BorderStyle")) {
		return _T("bsSingle");
	}
  if (asProperty == _T("WordWrap")) {
    return _T("True");
  }
  if (asProperty == _T("HideScrollBars")) {
    return _T("True");
  }
  if (asProperty == _T("SelStart")) {
    return _T("0");
  }
  if (asProperty == _T("SelLength")) {
    return _T("0");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TRichEdit::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("BorderStyle")) {
		TBorderStyle BorderStyle = Get_BorderStyle();
		if (BorderStyle == bsNone) return _T("bsNone");
		if (BorderStyle == bsSingle) return _T("bsSingle");
		return _T("");
	}
  if (asProperty == _T("WordWrap")) {
    if (Get_WordWrap()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("HideScrollBars")) {
    if (Get_HideScrollBars()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("SelStart")) {
    return IntToStr(Get_SelStart());
  }
  if (asProperty == _T("SelLength")) {
    return IntToStr(Get_SelLength());
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
bool TRichEdit::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
	if (asProperty == _T("BorderStyle")) {
		if (asValue == _T("bsNone")) Set_BorderStyle(bsNone);
		else if (asValue == _T("bsSingle")) Set_BorderStyle(bsSingle);
		return true;
	}
  if (asProperty == _T("WordWrap")) {
    if (asValue == _T("True")) Set_WordWrap(true);
    else Set_WordWrap(false);
    return true;
  }
  if (asProperty == _T("HideScrollBars")) {
    if (asValue == _T("True")) Set_HideScrollBars(true);
    else Set_HideScrollBars(false);
    return true;
  }
  if (asProperty == _T("SelStart")) {
    Set_SelStart(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("SelLength")) {
    Set_SelLength(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
TRichEdit::IExRichEditOleCallback::IExRichEditOleCallback()
{
    pStorage = NULL;
    m_iNumStorages = 0;
    m_dwRef = 0;
    // set up OLE storage
    HRESULT hResult = ::StgCreateDocfile(NULL,
        STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE /*| STGM_DELETEONRELEASE */|STGM_CREATE ,
        0, &pStorage );

}

//---------------------------------------------------------------------------
TRichEdit::IExRichEditOleCallback::~IExRichEditOleCallback()
{
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::GetNewStorage(LPSTORAGE* lplpstg)
{
  m_iNumStorages++;
  WCHAR tName[50];
  swprintf_s(tName, 50, L"REOLEStorage%d", m_iNumStorages);

  HRESULT hResult = pStorage->CreateStorage(tName,
      STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE ,
      0, 0, lplpstg );

	if (pStorage == NULL || hResult != S_OK)
	{
		// $$$ AfxThrowOleException( hResult );
	}

	return hResult;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::QueryInterface(REFIID iid, void ** ppvObject)
{
	HRESULT hr = S_OK;
	*ppvObject = NULL;

	if ( iid == IID_IUnknown || iid == IID_IRichEditOleCallback)
	{
		*ppvObject = this;
		AddRef();
		hr = NOERROR;
	}
	else
	{
		hr = E_NOINTERFACE;
	}

	return hr;
}

//---------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::AddRef()
{
  return ++m_dwRef;
}

//---------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::Release()
{
  if ( --m_dwRef == 0 )
  {
      delete this;
      return 0;
  }
  return m_dwRef;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
        LPOLEINPLACEUIWINDOW FAR *lplpDoc,
        LPOLEINPLACEFRAMEINFO lpFrameInfo) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::ShowContainerUI(BOOL fShow) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::DeleteObject(LPOLEOBJECT lpoleobj) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
        DWORD reco, BOOL fReally, HGLOBAL hMetaPict) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::ContextSensitiveHelp(BOOL fEnterMode) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
	TRichEdit::IExRichEditOleCallback::GetClipboardData(CHARRANGE FAR *lpchrg,
        DWORD reco, LPDATAOBJECT FAR *lplpdataobj) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
  TRichEdit::IExRichEditOleCallback::GetDragDropEffect(BOOL fDrag,
        DWORD grfKeyState, LPDWORD pdwEffect) {
  return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
  TRichEdit::IExRichEditOleCallback::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj,
        CHARRANGE FAR *lpchrg, HMENU FAR *lphmenu) {
  return S_OK;
}

//////////////////////////////////////////////////////////////////////
// ImageDataObject.h: Impementation for IDataObject Interface to be used 
//					     in inserting bitmap to the RichEdit Control.
//
// Author : Hani Atassi  (atassi@arabteam2000.com)
//
// How to use : Just call the static member InsertBitmap with 
//				the appropriate parrameters. 
//
// Known bugs :
//
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Static member functions
//////////////////////////////////////////////////////////////////////

void CImageDataObject::InsertBitmap(IRichEditOle* pRichEditOle, HBITMAP hBitmap)
{
	SCODE sc;

	// Get the image data object
	//
	CImageDataObject *pods = new CImageDataObject;
	LPDATAOBJECT lpDataObject;
	pods->QueryInterface(IID_IDataObject, (void **)&lpDataObject);

	pods->SetBitmap(hBitmap);

	// Get the RichEdit container site
	//
	IOleClientSite *pOleClientSite;	
	pRichEditOle->GetClientSite(&pOleClientSite);

	// Initialize a Storage Object
	//
	IStorage *pStorage;	

	LPLOCKBYTES lpLockBytes = NULL;
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	// $$$ if (sc != S_OK) AfxThrowOleException(sc);
	assert(lpLockBytes != NULL);
	
	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		assert(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		// $$$ AfxThrowOleException(sc);
	}
	assert(pStorage != NULL);

	// The final ole object which will be inserted in the richedit control
	//
	IOleObject *pOleObject; 
	pOleObject = pods->GetOleObject(pOleClientSite, pStorage);

	// all items are "contained" -- this makes our reference to this object
	//  weak -- which is needed for links to embedding silent update.
	OleSetContainedObject(pOleObject, TRUE);

	// Now Add the object to the RichEdit 
	//
	REOBJECT reobject;
	ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);
	
	CLSID clsid;
	sc = pOleObject->GetUserClassID(&clsid);
	// $$$ if (sc != S_OK) AfxThrowOleException(sc);

	reobject.clsid = clsid;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.poleobj = pOleObject;
	reobject.polesite = pOleClientSite;
	reobject.pstg = pStorage;

	// Insert the bitmap at the current location in the richedit control
	//
	pRichEditOle->InsertObject(&reobject);

	// Release all unnecessary interfaces
	//
	pOleObject->Release();
	pOleClientSite->Release();
	pStorage->Release();
	lpDataObject->Release();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CImageDataObject::SetBitmap(HBITMAP hBitmap)
{
	assert(hBitmap);

	STGMEDIUM stgm;
	stgm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle		
	stgm.hBitmap = hBitmap;
	stgm.pUnkForRelease = NULL;				// Use ReleaseStgMedium

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;				// Clipboard format = CF_BITMAP
	fm.ptd = NULL;							// Target Device = Screen
	fm.dwAspect = DVASPECT_CONTENT;			// Level of detail = Full content
	fm.lindex = -1;							// Index = Not applicaple
	fm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle

	this->SetData(&fm, &stgm, TRUE);		
}

IOleObject *CImageDataObject::GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage)
{
	assert(m_stgmed.hBitmap);

	SCODE sc;
	IOleObject *pOleObject;
	sc = ::OleCreateStaticFromData(this, IID_IOleObject, OLERENDER_FORMAT, 
			&m_fromat, pOleClientSite, pStorage, (void **)&pOleObject);
	// $$$ if (sc != S_OK) AfxThrowOleException(sc);
	return pOleObject;
}

//---------------------------------------------------------------------------
