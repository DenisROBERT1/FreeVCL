//-----------------------------------------------------------------------------
//! @file TBitBtn.cpp
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

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// TBitBtn
//---------------------------------------------------------------------------

TBitBtn::TBitBtn(TComponent* AOwner): TButton(AOwner) {

  // Initialisations
  FClassName = _T("TBitBtn");
	ChangeWindowStyle(FWindowStyle | BS_OWNERDRAW);
  FGlyph = NULL;
	FLayout = blGlyphLeft;
  FMargin = -1;
  FNumGlyphs = 1;
	FSpacing = 4;
  FStyle = bsAutoDetect;

}

//---------------------------------------------------------------------------
TBitBtn::~TBitBtn(void) {
  delete FGlyph;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Glyph
//---------------------------------------------------------------------------

TBitmap * TBitBtn::Get_Glyph(void) {
  return FGlyph;
}

bool TBitBtn::Set_Glyph(TBitmap * NewGlyph) {

	if (NewGlyph) {
		if (!FGlyph) FGlyph = (TBitmap *) new TBitmap();
		FGlyph->Assign(NewGlyph);
	}
	else {
		delete FGlyph;
		FGlyph = NULL;
	}
  if (FHandle) Invalidate();

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Layout
//---------------------------------------------------------------------------

TButtonLayout TBitBtn::Get_Layout(void) {
  return FLayout;
}

bool TBitBtn::Set_Layout(TButtonLayout NewLayout) {
  if (FLayout != NewLayout) {
    FLayout = NewLayout;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Margin
//---------------------------------------------------------------------------

int TBitBtn::Get_Margin(void) {
  return FMargin;
}

bool TBitBtn::Set_Margin(int NewMargin) {
  if (FMargin != NewMargin) {
    FMargin = NewMargin;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NumGlyphs
//---------------------------------------------------------------------------

int TBitBtn::Get_NumGlyphs(void) {
  return FNumGlyphs;
}

bool TBitBtn::Set_NumGlyphs(int NewNumGlyphs) {
  if (FNumGlyphs != NewNumGlyphs) {
    FNumGlyphs = NewNumGlyphs;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Spacing
//---------------------------------------------------------------------------

int TBitBtn::Get_Spacing(void) {
  return FSpacing;
}

bool TBitBtn::Set_Spacing(int NewSpacing) {
  if (FSpacing != NewSpacing) {
    FSpacing = NewSpacing;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Style
//---------------------------------------------------------------------------

TButtonStyle TBitBtn::Get_Style(void) {
  return FStyle;
}

bool TBitBtn::Set_Style(TButtonStyle NewStyle) {
  if (FStyle != NewStyle) {
    FStyle = NewStyle;
  }
  return true;
}

//---------------------------------------------------------------------------
bool TBitBtn::ProcessMeasureItem(LPMEASUREITEMSTRUCT lpMIS) {

  lpMIS->itemHeight = 40;
  lpMIS->itemWidth = 16;

  return true;
}

//---------------------------------------------------------------------------
bool TBitBtn::ProcessDrawItem(LPDRAWITEMSTRUCT lpDIS) {
	HDC hdc;
	RECT Rect, RectText;
	int XBitmap, YBitmap, WBitmap, HBitmap;
	int RealMargin, RealSpacing;
	HPEN hOldPen;
	UINT DrawFlags;

	hdc = lpDIS->hDC;
	Rect = lpDIS->rcItem;

	// Bouton par défaut
	if (lpDIS->itemState & ODS_DEFAULT) {
		hOldPen = (HPEN) SelectObject(hdc, GetStockObject(BLACK_PEN));
		Rectangle(hdc, Rect.left, Rect.top, Rect.right, Rect.bottom);
		SelectObject(hdc, hOldPen);
		InflateRect(&Rect, -1, -1);
	}

	// Encadrement
	DrawFlags = DFCS_BUTTONPUSH | DFCS_ADJUSTRECT;
	if (lpDIS->itemState & ODS_SELECTED) DrawFlags |= DFCS_PUSHED;
	if (lpDIS->itemState & ODS_GRAYED) DrawFlags |= DFCS_INACTIVE;
	DrawFrameControl(hdc, &Rect, DFC_BUTTON, DrawFlags);

	// Calcul des emplacements du texte et de l'image
	RectText = Rect;
	XBitmap = Rect.left;
	YBitmap = Rect.top;
	if (FGlyph) {
		WBitmap = FGlyph->Width;
		HBitmap = FGlyph->Height;
	}
	else {
		WBitmap = 0;
		HBitmap = 0;
	}

	if (FSpacing < 0) RealSpacing = 0;
	else RealSpacing = FSpacing;

	if (FMargin < 0) {
		SIZE SizeText;
		GetTextExtentPoint32(hdc, FCaption, FCaption.Length(), &SizeText);
		if (FLayout == blGlyphLeft || FLayout == blGlyphRight) {
			if (FSpacing < 0) {
				RealMargin = ((Rect.right - Rect.left) - SizeText.cx - WBitmap) / 3;
				RealSpacing = RealMargin;
			}
			else {
				RealMargin = ((Rect.right - Rect.left) - SizeText.cx - WBitmap - FSpacing) / 2;
			}
		}
		else {
			if (FSpacing < 0) {
				RealMargin = ((Rect.bottom - Rect.top) - SizeText.cy - HBitmap) / 3;
				RealSpacing = RealMargin;
			}
			else {
				RealMargin = ((Rect.bottom - Rect.top) - SizeText.cy - HBitmap - FSpacing) / 2;
			}
		}
		if (RealMargin < 0) RealMargin = 0;
		if (RealSpacing < 0) RealSpacing = 0;
	}
	else {
		RealMargin = FMargin;
	}

	DrawFlags = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
	switch (FLayout) {
		case blGlyphLeft:
			XBitmap = Rect.left + RealMargin;
			YBitmap = (Rect.top + Rect.bottom) / 2 - HBitmap / 2;
			RectText.left = XBitmap + WBitmap + RealSpacing;
			RectText.right -= RealMargin;
			if (FSpacing >= 0) DrawFlags = DT_SINGLELINE | DT_LEFT | DT_VCENTER;
			break;
		case blGlyphRight:
			XBitmap = Rect.right - RealMargin - WBitmap;
			YBitmap = (Rect.top + Rect.bottom) / 2 - HBitmap / 2;
			RectText.left += RealMargin;
			RectText.right = XBitmap - RealSpacing;
			if (FSpacing >= 0) DrawFlags = DT_SINGLELINE | DT_RIGHT | DT_VCENTER;
			break;
		case blGlyphTop:
			XBitmap = (Rect.left + Rect.right) / 2 - WBitmap / 2;
			YBitmap = Rect.top + RealMargin;
			RectText.top = YBitmap + HBitmap + RealSpacing;
			RectText.bottom -= RealMargin;
			if (FSpacing >= 0) DrawFlags = DT_SINGLELINE | DT_CENTER | DT_TOP;
			break;
		case blGlyphBottom:
			XBitmap = (Rect.left + Rect.right) / 2 - WBitmap / 2;
			YBitmap = Rect.bottom - RealMargin - HBitmap;
			RectText.top += RealMargin;
			RectText.bottom = YBitmap - RealSpacing;
			if (FSpacing >= 0) DrawFlags = DT_SINGLELINE | DT_CENTER | DT_BOTTOM;
			break;
	}
	if (lpDIS->itemState & ODS_SELECTED) {
		XBitmap++;
		YBitmap++;
		RectText.left++;
		RectText.top++;
		RectText.right++;
		RectText.bottom++;
	}

	// Image
	if (FGlyph) {
		FGlyph->Draw(hdc, XBitmap, YBitmap, WBitmap, HBitmap);
	}

	// Text
	if (RectText.left < RectText.right && RectText.top < RectText.bottom) {
		SetBkMode(hdc, TRANSPARENT);
		if (FEnabled) {
			SetTextColor(hdc, ((TColor) FFont->Color).cr);
			DrawText(hdc, FCaption, FCaption.Length(), &RectText, DrawFlags);
		}
		else {
			RectText.left++;
			RectText.top++;
			RectText.right++;
			RectText.bottom++;
			SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
			DrawText(hdc, FCaption, FCaption.Length(), &RectText, DrawFlags);
			RectText.left--;
			RectText.top--;
			RectText.right--;
			RectText.bottom--;
			SetTextColor(hdc, GetSysColor(COLOR_GRAYTEXT));
			DrawText(hdc, FCaption, FCaption.Length(), &RectText, DrawFlags);
		}
	}

	// Rectangle de focus
	InflateRect(&Rect, -1, -1);
	if (lpDIS->itemState & ODS_FOCUS) DrawFocusRect(hdc, &Rect);

	return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TBitBtn::GetListProperties(TStrings *ListProperties) {

  TButton::GetListProperties(ListProperties);

  ListProperties->Add(_T("Glyph"));
  ListProperties->Add(_T("Glyph.Data"));
  ListProperties->Add(_T("Layout"));
  ListProperties->Add(_T("Margin"));
  ListProperties->Add(_T("NumGlyphs"));
  ListProperties->Add(_T("Spacing"));
  ListProperties->Add(_T("Style"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TBitBtn::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Glyph")) {
    return tpObject;
  }
  if (asProperty == _T("Glyph.Data")) {
    return tpData;
  }
  if (asProperty == _T("Layout")) {
    *asInfos = _T("blGlyphLeft\nblGlyphRight\nblGlyphTop\nblGlyphBottom");
    return tpChoice;
  }
  if (asProperty == _T("Margin")) {
    return tpNumber;
  }
  if (asProperty == _T("NumGlyphs")) {
    return tpNumber;
  }
  if (asProperty == _T("Spacing")) {
    return tpNumber;
  }
  if (asProperty == _T("Style")) {
    *asInfos = _T("bsAutoDetect\nbsWin31\nbsNew");
    return tpChoice;
  }
  return TButton::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TBitBtn::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Glyph")) {
    return _T("");
  }
  if (asProperty == _T("Glyph.Data")) {
    return _T("");
  }
  if (asProperty == _T("Layout")) {
    return _T("blGlyphLeft");
  }
  if (asProperty == _T("Margin")) {
    return _T("-1");
  }
  if (asProperty == _T("NumGlyphs")) {
    return _T("1");
  }
  if (asProperty == _T("Spacing")) {
    return _T("4");
  }
  if (asProperty == _T("Style")) {
    return _T("bsAutoDetect");
  }
  return TButton::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TBitBtn::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Glyph")) {
    TBitmap *Glyph = Get_Glyph();
    if (Glyph) return Glyph->Name;
    return _T("");
  }
  if (asProperty == _T("Glyph.Data")) {
    return FGlyph->GetData();
  }
  if (asProperty == _T("Layout")) {
    TButtonLayout Layout = Get_Layout();
    if (Layout == blGlyphLeft) return _T("blGlyphLeft");
    if (Layout == blGlyphRight) return _T("blGlyphRight");
    if (Layout == blGlyphTop) return _T("blGlyphTop");
    if (Layout == blGlyphBottom) return _T("blGlyphBottom");
    return _T("");
  }
  if (asProperty == _T("Margin")) {
    return IntToStr(Get_Margin());
  }
  if (asProperty == _T("NumGlyphs")) {
    return IntToStr(Get_NumGlyphs());
  }
  if (asProperty == _T("Spacing")) {
    return IntToStr(Get_Spacing());
  }
  if (asProperty == _T("Style")) {
    TButtonStyle Style = Get_Style();
    if (Style == bsAutoDetect) return _T("bsAutoDetect");
    if (Style == bsWin31) return _T("bsWin31");
    if (Style == bsNew) return _T("bsNew");
    return _T("");
  }
  return TButton::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TBitBtn::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Glyph")) {
    TBitmap *Bitmap;
		if (asValue.IsEmpty()) Bitmap = NULL;
		else Bitmap = dynamic_cast<TBitmap *>(FindRootComponent()->FindComponent(asValue));
    Set_Glyph(Bitmap);
    return true;
  }
  if (asProperty == _T("Glyph.Data")) {
    ChangeWindowStyle((DWORD) (SIZE_T) GetWindowLongPtr(FHandle, GWL_STYLE) | BS_BITMAP);
    int SizeValeur = asValue.Length() / 2;
    char *szValeur = new char[SizeValeur];
    HexToBin(asValue, szValeur, SizeValeur);
		TMemoryStream GlyphData(&szValeur[4], SizeValeur - 4);
    TBitmap *Bitmap = new TBitmap;
		Bitmap->LoadFromStream(&GlyphData);
    Set_Glyph(Bitmap);
		delete Bitmap;
    delete[] szValeur;
    return true;
  }
  if (asProperty == _T("Layout")) {
    if      (asValue == _T("blGlyphLeft")) Set_Layout(blGlyphLeft);
    else if (asValue == _T("blGlyphRight")) Set_Layout(blGlyphRight);
    else if (asValue == _T("blGlyphTop")) Set_Layout(blGlyphTop);
    else if (asValue == _T("blGlyphBottom")) Set_Layout(blGlyphBottom);
    return true;
  }
  if (asProperty == _T("Margin")) {
    Set_Margin(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("NumGlyphs")) {
    Set_NumGlyphs(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Spacing")) {
    Set_Spacing(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Style")) {
    if      (asValue == _T("bsAutoDetect")) Set_Style(bsAutoDetect);
    else if (asValue == _T("bsWin31")) Set_Style(bsWin31);
    else if (asValue == _T("bsNew")) Set_Style(bsNew);
    return true;
  }
  return TButton::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------
