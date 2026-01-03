//-----------------------------------------------------------------------------
//! @file TCustomPanel.cpp
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
// TCustomPanel
//---------------------------------------------------------------------------

TCustomPanel::TCustomPanel(TComponent* AOwner): TCustomControl(AOwner) {
  // Initialisations
  FClassName = _T("TCustomPanel");
	FAlignment = taLeftJustify;
	FAlignmentV = taBaseLine;
  FBorderStyle = bsNone;
  FBevelInner = bvNone;
  FBevelOuter = bvRaised;
  FBorderWidth = 0;
  FBevelWidth = 1;
  FColor = clBtnFace;
	FCursor = crArrow;
}

//---------------------------------------------------------------------------
TCustomPanel::~TCustomPanel(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Alignment
//---------------------------------------------------------------------------

TAlignment TCustomPanel::Get_Alignment(void) {
  return FAlignment;
}

bool TCustomPanel::Set_Alignment(TAlignment NewAlignment) {
	if (FAlignment != NewAlignment) {
		FAlignment = NewAlignment;
		Invalidate();
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AlignmentV
//---------------------------------------------------------------------------

TAlignmentV TCustomPanel::Get_AlignmentV(void) {
  return FAlignmentV;
}

bool TCustomPanel::Set_AlignmentV(TAlignmentV NewAlignmentV) {
	if (FAlignmentV != NewAlignmentV) {
		FAlignmentV = NewAlignmentV;
		Invalidate();
	}

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TBorderStyle TCustomPanel::Get_BorderStyle(void) {
  return FBorderStyle;
}

bool TCustomPanel::Set_BorderStyle(TBorderStyle NewBorderStyle) {

  if (FBorderStyle != NewBorderStyle) {
    FBorderStyle = NewBorderStyle;
    if (FHandle) InvalidateRect(FHandle, NULL, TRUE);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BevelInner
//---------------------------------------------------------------------------

TBevelCut TCustomPanel::Get_BevelInner(void) {
  return FBevelInner;
}

bool TCustomPanel::Set_BevelInner(TBevelCut NewBevelInner) {
  if (FBevelInner != NewBevelInner) {
    FBevelInner = NewBevelInner;
    if (FHandle) InvalidateRect(FHandle, NULL, TRUE);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BevelOuter
//---------------------------------------------------------------------------

TBevelCut TCustomPanel::Get_BevelOuter(void) {
  return FBevelOuter;
}

bool TCustomPanel::Set_BevelOuter(TBevelCut NewBevelOuter) {
  if (FBevelOuter != NewBevelOuter) {
    FBevelOuter = NewBevelOuter;
    if (FHandle) InvalidateRect(FHandle, NULL, TRUE);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderWidth
//---------------------------------------------------------------------------

int TCustomPanel::Get_BorderWidth(void) {
  return FBorderWidth;
}

bool TCustomPanel::Set_BorderWidth(int NewBorderWidth) {
  if (FBorderWidth != NewBorderWidth) {
    FBorderWidth = NewBorderWidth;
    if (FHandle) InvalidateRect(FHandle, NULL, TRUE);
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BevelWidth
//---------------------------------------------------------------------------

int TCustomPanel::Get_BevelWidth(void) {
  return FBevelWidth;
}

bool TCustomPanel::Set_BevelWidth(int NewBevelWidth) {
  if (FBevelWidth != NewBevelWidth) {
    FBevelWidth = NewBevelWidth;
    if (FHandle) InvalidateRect(FHandle, NULL, TRUE);
  }

  return true;
}

//---------------------------------------------------------------------------
// Ajustement du rectangle client
//---------------------------------------------------------------------------
void FASTCALL TCustomPanel::AdjustClientRect(TRect &Rect) {

  if (FBorderStyle == bsSingle) {
    InflateRect(&Rect, -1, -1);
  }
  if (FBevelInner != bvNone) {
    InflateRect(&Rect, -FBevelWidth, -FBevelWidth);
  }
  if (FBorderWidth != 0) {
    InflateRect(&Rect, -FBorderWidth, -FBorderWidth);
  }
  if (FBevelOuter != bvNone) {
    InflateRect(&Rect, -FBevelWidth, -FBevelWidth);
  }

}

//---------------------------------------------------------------------------
// Dessin du contrôle
//---------------------------------------------------------------------------

bool TCustomPanel::ProcessPaint(HDC hdc, RECT rcPaint) {
  RECT Rect;
	int i;


  // Rectangle de la totalité du contrôle (et non rcPaint qui peut être plus petit)
  GetWindowRect(FHandle, &Rect);

  // Transformation en coordonnées relatives au contrôle
  Rect.right -= Rect.left;
  Rect.bottom -= Rect.top;
  Rect.left = 0;
  Rect.top = 0;

  // Fond
	if (!hBrushColorBack) {
	  hBrushColorBack = CreateSolidBrush(FColor.cr);
	}
  FillRect(hdc, &Rect, hBrushColorBack);

  // Bordure
  if (FBorderStyle == bsSingle) {
    DrawEdge(hdc, &Rect, BDR_RAISEDOUTER, BF_RECT);
    InflateRect(&Rect, -1, -1);
  }
  if (FBevelOuter != bvNone) {
    if (FBevelOuter != bvSpace) {
      UINT edge = 0;
      if (FBevelOuter == bvRaised) edge = BDR_RAISEDOUTER;
      else edge = BDR_SUNKENOUTER;
			for (i = 0; i < FBevelWidth; i++) {
	      DrawEdge(hdc, &Rect, edge, BF_RECT);
				InflateRect(&Rect, -1, -1);
			}
    }
		else {
			InflateRect(&Rect, -FBevelWidth, -FBevelWidth);
		}
  }
  if (FBorderWidth != 0) {
    InflateRect(&Rect, -FBorderWidth, -FBorderWidth);
  }
  if (FBevelInner != bvNone) {
    if (FBevelInner != bvSpace) {
      UINT edge = 0;
      if (FBevelInner == bvRaised) edge = BDR_RAISEDINNER;
      else edge = BDR_SUNKENINNER;
			for (i = 0; i < FBevelWidth; i++) {
	      DrawEdge(hdc, &Rect, edge, BF_RECT);
				InflateRect(&Rect, -1, -1);
			}
    }
		else {
			InflateRect(&Rect, -FBevelWidth, -FBevelWidth);
		}
  }

  // Texte
  if (!FCaption.IsEmpty()) {
		int Flags;

		if (FAlignment == taRightJustify) Flags = DT_RIGHT;
		else if (FAlignment == taCenter) Flags = DT_CENTER;
		else Flags = DT_LEFT;

		if (FAlignmentV == taTop) Flags |= DT_TOP | DT_WORDBREAK;
		else if (FAlignmentV == taBaseLine) Flags |= DT_VCENTER | DT_SINGLELINE;
		else Flags |= DT_BOTTOM | DT_SINGLELINE;

		HFONT hOldFont = (HFONT) SelectObject(hdc, FFont->Handle);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, FCaption, FCaption.Length(), &Rect, Flags);
    SelectObject(hdc, hOldFont);
  }

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomPanel::GetListProperties(TStrings *ListProperties) {

  TCustomControl::GetListProperties(ListProperties);

	ListProperties->Add(_T("Alignment"));
	ListProperties->Add(_T("AlignmentV"));
  ListProperties->Add(_T("BorderStyle"));
  ListProperties->Add(_T("BevelInner"));
  ListProperties->Add(_T("BevelOuter"));
  ListProperties->Add(_T("BorderWidth"));
  ListProperties->Add(_T("BevelWidth"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomPanel::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Alignment")) {
    *asInfos = _T("taLeftJustify\ntaRightJustify\ntaCenter");
    return tpChoice;
  }
  if (asProperty == _T("AlignmentV")) {
    *asInfos = _T("taTop\ntaBaseLine\ntaBottom");
    return tpChoice;
  }
  if (asProperty == _T("BorderStyle")) {
    *asInfos = _T("bsNone\nbsSingle");
    return tpChoice;
  }
  if (asProperty == _T("BevelInner")) {
    *asInfos = _T("bvNone\nbvLowered\nbvRaised\nbvSpace");
    return tpChoice;
  }
  if (asProperty == _T("BevelOuter")) {
    *asInfos = _T("bvNone\nbvLowered\nbvRaised\nbvSpace");
    return tpChoice;
  }
  if (asProperty == _T("BorderWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("BevelWidth")) {
    return tpNumber;
  }
  return TCustomControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomPanel::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    return _T("taLeftJustify");
  }
  if (asProperty == _T("AlignmentV")) {
    return _T("taBaseline");
  }
  if (asProperty == _T("BorderStyle")) {
    return _T("bsNone");
  }
  if (asProperty == _T("BevelInner")) {
    return _T("bvNone");
  }
  if (asProperty == _T("BevelOuter")) {
    return _T("bvRaised");
  }
  if (asProperty == _T("BorderWidth")) {
    return _T("0");
  }
  if (asProperty == _T("BevelWidth")) {
    return _T("1");
  }
  if (asProperty == _T("Color")) {
    return _T("clBtnFace");
  }
  return TCustomControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomPanel::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Alignment")) {
    TAlignment Alignment = Get_Alignment();
    if (Alignment == taLeftJustify) return _T("taLeftJustify");
    if (Alignment == taRightJustify) return _T("taRightJustify");
    if (Alignment == taCenter) return _T("taCenter");
    return _T("taLeftJustify");
  }
  if (asProperty == _T("AlignmentV")) {
    TAlignmentV AlignmentV = Get_AlignmentV();
    if (AlignmentV == taTop) return _T("taTop");
    if (AlignmentV == taBaseLine) return _T("taBaseLine");
    if (AlignmentV == taBottom) return _T("taBottom");
    return _T("taBaseLine");
  }
  if (asProperty == _T("BorderStyle")) {
    TBorderStyle BorderStyle = Get_BorderStyle();
    if (BorderStyle == bsNone) return _T("bsNone");
    if (BorderStyle == bsSingle) return _T("bsSingle");
    return _T("bsNone");
  }
  if (asProperty == _T("BevelInner")) {
    TBevelCut BevelInner = Get_BevelInner();
    if (BevelInner == bvNone) return _T("bvNone");
    if (BevelInner == bvLowered) return _T("bvLowered");
    if (BevelInner == bvRaised) return _T("bvRaised");
    if (BevelInner == bvSpace) return _T("bvSpace");
    return _T("bvRaised");
  }
  if (asProperty == _T("BevelOuter")) {
    TBevelCut BevelOuter = Get_BevelOuter();
    if (BevelOuter == bvNone) return _T("bvNone");
    if (BevelOuter == bvLowered) return _T("bvLowered");
    if (BevelOuter == bvRaised) return _T("bvRaised");
    if (BevelOuter == bvSpace) return _T("bvSpace");
    return _T("bvRaised");
  }
  if (asProperty == _T("BorderWidth")) {
    return IntToStr(Get_BorderWidth());
  }
  if (asProperty == _T("BevelWidth")) {
    return IntToStr(Get_BevelWidth());
  }
  return TCustomControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomPanel::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Alignment")) {
    if (asValue == _T("taLeftJustify")) Set_Alignment(taLeftJustify);
    else if (asValue == _T("taRightJustify")) Set_Alignment(taRightJustify);
    else if (asValue == _T("taCenter")) Set_Alignment(taCenter);
    return true;
  }
  if (asProperty == _T("AlignmentV")) {
    if (asValue == _T("taTop")) Set_AlignmentV(taTop);
    else if (asValue == _T("taBaseLine")) Set_AlignmentV(taBaseLine);
    else if (asValue == _T("taBottom")) Set_AlignmentV(taBottom);
    return true;
  }
  if (asProperty == _T("BorderStyle")) {
    if (asValue == _T("bsNone")) Set_BorderStyle(bsNone);
    else if (asValue == _T("bsSingle")) Set_BorderStyle(bsSingle);
    return true;
  }
  if (asProperty == _T("BevelInner")) {
    if (asValue == _T("bvNone")) Set_BevelInner(bvNone);
    else if (asValue == _T("bvLowered")) Set_BevelInner(bvLowered);
    else if (asValue == _T("bvRaised")) Set_BevelInner(bvRaised);
    else if (asValue == _T("bvSpace")) Set_BevelInner(bvSpace);
    return true;
  }
  if (asProperty == _T("BevelOuter")) {
    if (asValue == _T("bvNone")) Set_BevelOuter(bvNone);
    else if (asValue == _T("bvLowered")) Set_BevelOuter(bvLowered);
    else if (asValue == _T("bvRaised")) Set_BevelOuter(bvRaised);
    else if (asValue == _T("bvSpace")) Set_BevelOuter(bvSpace);
    return true;
  }
  if (asProperty == _T("BorderWidth")) {
    Set_BorderWidth(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("BevelWidth")) {
    Set_BevelWidth(asValue.ToIntDef(0));
    return true;
  }
  return TCustomControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

