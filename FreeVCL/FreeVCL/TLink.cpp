//-----------------------------------------------------------------------------
//! @file TLink.cpp
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


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TLink
//---------------------------------------------------------------------------

TLink::TLink(TComponent* AOwner): TWinControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TLink");
	FAlignment = taLeftJustify;
  FCursor = crHandPoint;
	FAutoSize = true;
	FLinkColor = 0x00FF0000;
	FALinkColor = 0x00003FFF;
	FVLinkColor = 0x007F007F;
	FVisited = false;
	PopupMenuLink = NULL;
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;

  // Enregistrement de la classe "TLink":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TLink");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
												 _T("TLink"), FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
		FFont->Style = (TFontStyles) FFont->Style << fsBold << fsUnderline;
		FFont->Color = FLinkColor;
		TMenuItem *MenuItem;
		PopupMenuLink = new TPopupMenu(this);
		MenuItem = new TMenuItem(PopupMenuLink);
		MenuItem->Caption = _TT("Ouvrir");
		SET_EVENT(MenuItem, TNotifyEvent, OnClick, TLink, this, OuvrirClick);
		PopupMenuLink->Add(MenuItem);
		MenuItem = new TMenuItem(PopupMenuLink);
		MenuItem->Caption = _TT("Copier le raccourci");
		SET_EVENT(MenuItem, TNotifyEvent, OnClick, TLink, this, CopierClick);
		PopupMenuLink->Add(MenuItem);
		MenuItem = new TMenuItem(PopupMenuLink);
		MenuItem->Caption = _T("-");
		PopupMenuLink->Add(MenuItem);
		MenuItem = new TMenuItem(PopupMenuLink);
		MenuItem->Caption = _TT("Propriétés");
		SET_EVENT(MenuItem, TNotifyEvent, OnClick, TLink, this, ProprietesClick);
		PopupMenuLink->Add(MenuItem);
		PopupMenu = PopupMenuLink;

    bWantPaint = true;
		bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TLink::~TLink(void) {
	delete PopupMenuLink;
}

//---------------------------------------------------------------------------
bool FASTCALL TLink::CanAutoSize(int &NewWidth, int &NewHeight) {
	SIZE SizeLink;
	BOOL Ok;

	HDC hdc = GetDC(FHandle);
	HFONT hOldFont = (HFONT) SelectObject(hdc, FFont->Handle);
	Ok = GetTextExtentPoint32(hdc, FCaption, FCaption.Length(), &SizeLink);
	SelectObject(hdc, hOldFont);
	ReleaseDC(FHandle, hdc);

	if (!Ok ||
			(NewWidth == SizeLink.cx &&
			 NewHeight == SizeLink.cy)) return false;

	NewWidth = SizeLink.cx;
	NewHeight = SizeLink.cy;

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Caption
//---------------------------------------------------------------------------

bool TLink::Set_Caption(AnsiString NewCaption) {
	TWinControl::Set_Caption(NewCaption);
	if (FAutoSize) {
		int NewLeft = FLeft;
		int NewTop = FTop;
		int NewWidth = FWidth;
		int NewHeight = FHeight;
		if (InternalCanAutoSize(NewLeft, NewTop, NewWidth, NewHeight)) {
			Size(NewLeft, NewTop, NewWidth, NewHeight, false);
		}
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété HRef
//---------------------------------------------------------------------------

AnsiString TLink::Get_HRef(void) {
  return FHRef;
}

bool TLink::Set_HRef(AnsiString NewHRef) {
  if (FHRef != NewHRef) {
    FHRef = NewHRef;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

bool TLink::Set_Font(TFont *NewFont) {
	TWinControl::Set_Font(NewFont);
	if (FAutoSize) {
		int NewLeft = FLeft;
		int NewTop = FTop;
		int NewWidth = FWidth;
		int NewHeight = FHeight;
		if (InternalCanAutoSize(NewLeft, NewTop, NewWidth, NewHeight)) {
			Size(NewLeft, NewTop, NewWidth, NewHeight, false);
		}
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TLink::Get_Alignment(void) {
  return FAlignment;
}

bool TLink::Set_Alignment(TAlignment NewAlignment) {
	if (FAlignment != NewAlignment) {
		FAlignment = NewAlignment;
		Invalidate();
	}

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LinkColor
//---------------------------------------------------------------------------

TColor TLink::Get_LinkColor(void) {
  return FLinkColor;
}

bool TLink::Set_LinkColor(TColor NewLinkColor) {
  if (FLinkColor != NewLinkColor) {
    FLinkColor = NewLinkColor;
		FFont->Color = FLinkColor;
		Invalidate();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ALinkColor
//---------------------------------------------------------------------------

TColor TLink::Get_ALinkColor(void) {
  return FALinkColor;
}

bool TLink::Set_ALinkColor(TColor NewALinkColor) {
  if (FALinkColor != NewALinkColor) {
    FALinkColor = NewALinkColor;
		Invalidate();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété VLinkColor
//---------------------------------------------------------------------------

TColor TLink::Get_VLinkColor(void) {
  return FVLinkColor;
}

bool TLink::Set_VLinkColor(TColor NewVLinkColor) {
  if (FVLinkColor != NewVLinkColor) {
    FVLinkColor = NewVLinkColor;
		Invalidate();
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Visited
//---------------------------------------------------------------------------

bool TLink::Get_Visited(void) {
  return FVisited;
}

bool TLink::Set_Visited(bool NewVisited) {
  if (FVisited != NewVisited) {
    FVisited = NewVisited;
  }
  return true;
}


//---------------------------------------------------------------------------
bool TLink::ProcessPaint(HDC hdc, RECT rcPaint) {
  HFONT hOldFont;
  RECT ClientRect;
	UINT DrawFormat;
	TColor TextColor;

  GetClientRect(FHandle, &ClientRect);
  hOldFont = (HFONT) SelectObject(hdc, FFont->Handle);
  TextColor = FFont->Color;
  SetTextColor(hdc, TextColor.cr);
  SetBkMode(hdc, TRANSPARENT);
	switch (FAlignment) {
		case taRightJustify:
			DrawFormat = DT_RIGHT | DT_SINGLELINE | DT_VCENTER;
			break;
		case taCenter:
			DrawFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
			break;
		default:
			DrawFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
			break;
	}
  DrawText(hdc, FCaption, -1, &ClientRect, DrawFormat);

  return false;
}

//---------------------------------------------------------------------------
bool TLink::ProcessMouseMove(int X, int Y) {
  bool bRetour = TWinControl::ProcessMouseMove(X, Y);
	if (Mouse->Capture != FHandle) {
		if (FLinkColor != FALinkColor) {
			Mouse->Capture = FHandle;
			FFont->Color = FALinkColor;
			Invalidate();
		}
	}
	else {
	  RECT ClientRect;
	  GetClientRect(FHandle, &ClientRect);
		if (!(ClientRect.left <= X && X <= ClientRect.right &&
				  ClientRect.top <= Y && Y <= ClientRect.bottom)) {
			Mouse->Capture = NULL;
		}
	}

  return bRetour;
}

//---------------------------------------------------------------------------
bool TLink::ProcessLButtonUp(int X, int Y) {
  bool bRetour = TWinControl::ProcessLButtonUp(X, Y);
	Execute();

  return bRetour;
}

//---------------------------------------------------------------------------
void TLink::OuvrirClick(TObject *Sender) {
	Execute();
}

//---------------------------------------------------------------------------
void TLink::CopierClick(TObject *Sender) {
	Clipboard->Open();
	Clipboard->SetTextBuf(FHRef);
	Clipboard->Close();
}

//---------------------------------------------------------------------------
void TLink::ProprietesClick(TObject *Sender) {
	Application->MessageBox(FHRef, FCaption, MB_OK);
}

//---------------------------------------------------------------------------
bool TLink::ProcessCaptureChanged(HWND hWndNewCapture) {
	if (hWndNewCapture != FHandle) {
		if (FVisited) FFont->Color = FVLinkColor;
		else FFont->Color = FLinkColor;
		Invalidate();
	}

	return true;
}

//---------------------------------------------------------------------------
bool TLink::Execute(void) {
	HINSTANCE hInstExec;

  hInstExec = ShellExecute(NULL, _T("open"), FHRef, 0, 0, SW_NORMAL);
	if (hInstExec <= (HINSTANCE) 32) return false;

	FVisited = true;

	return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TLink::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Alignment"));
  ListProperties->Add(_T("HRef"));
  ListProperties->Add(_T("LinkColor"));
  ListProperties->Add(_T("ALinkColor"));
  ListProperties->Add(_T("VLinkColor"));
  ListProperties->Add(_T("Visited"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TLink::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Alignment")) {
    *asInfos = _T("taLeftJustify\ntaRightJustify\ntaCenter");
    return tpChoice;
  }
  if (asProperty == _T("HRef")) {
    return tpText;
  }
  if (asProperty == _T("LinkColor")) {
    return tpColor;
  }
  if (asProperty == _T("ALinkColor")) {
    return tpColor;
  }
  if (asProperty == _T("VLinkColor")) {
    return tpColor;
  }
  if (asProperty == _T("Visited")) {
    return tpBool;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TLink::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    return _T("taLeftJustify");
  }
  if (asProperty == _T("HRef")) {
    return _T("");
  }
  if (asProperty == _T("LinkColor")) {
    return _T("0x00FF0000");
  }
  if (asProperty == _T("ALinkColor")) {
    return _T("0x00007FFF");
  }
  if (asProperty == _T("VLinkColor")) {
    return _T("0x007F007F");
  }
  if (asProperty == _T("Visited")) {
    return _T("False");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TLink::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    TAlignment Alignment = Get_Alignment();
    if (Alignment == taLeftJustify) return _T("taLeftJustify");
    if (Alignment == taRightJustify) return _T("taRightJustify");
    if (Alignment == taCenter) return _T("taCenter");
    return _T("taLeftJustify");
  }
  if (asProperty == _T("HRef")) {
    return Get_HRef();
  }
  if (asProperty == _T("LinkColor")) {
    return ColorToString(Get_LinkColor());
  }
  if (asProperty == _T("ALinkColor")) {
    return ColorToString(Get_ALinkColor());
  }
  if (asProperty == _T("VLinkColor")) {
    return ColorToString(Get_VLinkColor());
  }
  if (asProperty == _T("Visited")) {
    if (Get_Visited()) return _T("True");
    else return _T("False");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TLink::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Alignment")) {
    if (asValue == _T("taLeftJustify")) Set_Alignment(taLeftJustify);
    else if (asValue == _T("taRightJustify")) Set_Alignment(taRightJustify);
    else if (asValue == _T("taCenter")) Set_Alignment(taCenter);
    return true;
  }
  if (asProperty == _T("HRef")) {
    Set_HRef(asValue);
    return true;
  }
  if (asProperty == _T("LinkColor")) {
    TColor Color;
    Color = asValue;
    Set_LinkColor(Color);
    return true;
  }
  if (asProperty == _T("ALinkColor")) {
    TColor Color;
    Color = asValue;
    Set_ALinkColor(Color);
    return true;
  }
  if (asProperty == _T("VLinkColor")) {
    TColor Color;
    Color = asValue;
    Set_VLinkColor(Color);
    return true;
  }
  if (asProperty == _T("Visited")) {
    Set_Visited(asValue == _T("True"));
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

