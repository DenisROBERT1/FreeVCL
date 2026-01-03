//-----------------------------------------------------------------------------
//! @file TForm_Autocompletion.cpp
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
#include "TFrame_EditFichierText.h"
#include "TForm_Autocompletion.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_Autocompletion::TForm_Autocompletion(TComponent *Owner, LPCTSTR szName)
    : TForm(Owner, szName, FVCL_ARG_SUPP) {
	FItems = new TStringList;

	TopIndex = 0;
	ItemIndex = 0;
	ItemHeight = 10;
	MinTickCount = 0;

	if (Config->AlphaBlend == false) {
		AlphaBlend = false;
		Timer_AlphaBlend->Enabled = false;
	}

}

//---------------------------------------------------------------------------
TForm_Autocompletion::~TForm_Autocompletion(void) {
	delete FItems;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FItems
//---------------------------------------------------------------------------

TStrings *TForm_Autocompletion::Get_Items(void) {
  return FItems;
}

bool TForm_Autocompletion::Set_Items(TStrings *NewItems) {

  if (FItems != NewItems) {
    FItems->Assign(NewItems);
  }
  return true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Autocompletion::PaintBox_AutocompletionPaint(TObject *Sender) {
  TCanvas *ACanvas;
  TRect Rect;
	TFont *GuiFont;
	int i, y;
	TEXTMETRIC tm;


  ACanvas = PaintBox_Autocompletion->Canvas;
  Rect = PaintBox_Autocompletion->ClientRect;

	// Fond
	ACanvas->Brush->Style = bsSolid;
	ACanvas->Brush->Color = clWindow;
	ACanvas->FillRect(Rect);

	// Texte
	GuiFont = new TFont();
	GuiFont->Handle = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	ACanvas->Font = GuiFont;
	delete GuiFont;
	ACanvas->TextFlags = tfOpaque;
	ACanvas->GetTextMetrics(&tm);
	ItemHeight = tm.tmHeight + tm.tmDescent;
	for (y = 0, i = TopIndex;
			 i < (int) FItems->Count && y <= PaintBox_Autocompletion->Height;
			 i++, y += ItemHeight) {
		if (i == ItemIndex) {
			ACanvas->Font->Color = clHighlightText;
			ACanvas->Brush->Color = clHighlight;
		}
		else {
			ACanvas->Font->Color = clWindowText;
			ACanvas->Brush->Color = clWindow;
		}
		ACanvas->TextOut(Rect.Left + 2, Rect.Top + y, FItems->Strings[i]);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Autocompletion::Timer_AlphaBlendTimer(TObject *Sender) {
	RECT Rect;
	TPoint ptCursorPos;
	int NewAlphaBlendValue;
	int Distance, MaxDistance = 0;


	GetWindowRect(Handle, &Rect);
	ptCursorPos = Mouse->CursorPos;
	if (ptCursorPos.x < Rect.left) {
		Distance = Rect.left - ptCursorPos.x;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	if (Rect.right < ptCursorPos.x) {
		Distance = ptCursorPos.x - Rect.right;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	if (ptCursorPos.y < Rect.top) {
		Distance = Rect.top - ptCursorPos.y;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	if (Rect.bottom < ptCursorPos.y) {
		Distance = ptCursorPos.y - Rect.bottom;
		if (MaxDistance < Distance) MaxDistance = Distance;
	}
	NewAlphaBlendValue = 255 - MaxDistance * 2;

	if (NewAlphaBlendValue < 31) NewAlphaBlendValue = 31;
	AlphaBlendValue = NewAlphaBlendValue;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Autocompletion::PaintBox_AutocompletionMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	int NbLinesVisibles = PaintBox_Autocompletion->ClientHeight / ItemHeight;
	int MaxTopIndex = FItems->Count - NbLinesVisibles;
	int NewTopIndex = TopIndex;
	int NewItemIndex;


	if (GetTickCount() >= MinTickCount) {  // Pour éviter un défilement trop rapide

		if (MaxTopIndex < 0) MaxTopIndex = 0;

		if (Y < 10) {
			NewTopIndex--;
			if (NewTopIndex < 0) NewTopIndex = 0;
			MinTickCount = GetTickCount() + 100;
		}
		else if (Y > PaintBox_Autocompletion->ClientHeight - 10) {
			NewTopIndex++;
			if (NewTopIndex > MaxTopIndex) NewTopIndex = MaxTopIndex;
			MinTickCount = GetTickCount() + 100;
		}

	}

	NewItemIndex = NewTopIndex + Y / ItemHeight;
	if (NewItemIndex >= FItems->Count) {
		NewItemIndex = FItems->Count - 1;
	}

	if (TopIndex != NewTopIndex || ItemIndex != NewItemIndex) {
		TopIndex = NewTopIndex;
		ItemIndex = NewItemIndex;
		PaintBox_Autocompletion->Invalidate();
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Autocompletion::PaintBox_AutocompletionMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled) {
	int NbLinesVisibles = PaintBox_Autocompletion->ClientHeight / ItemHeight;
	int MaxTopIndex = FItems->Count - NbLinesVisibles;
	int NewTopIndex = TopIndex;
	int NewItemIndex;


	if (MaxTopIndex < 0) MaxTopIndex = 0;

	NewTopIndex -= WheelDelta / 30;
	if (NewTopIndex > MaxTopIndex) NewTopIndex = MaxTopIndex;
	if (NewTopIndex < 0) NewTopIndex = 0;

	NewItemIndex = NewTopIndex + MousePos.y / ItemHeight;
	if (NewItemIndex >= FItems->Count) {
		NewItemIndex = FItems->Count - 1;
	}

	if (TopIndex != NewTopIndex || ItemIndex != NewItemIndex) {
		TopIndex = NewTopIndex;
		ItemIndex = NewItemIndex;
		PaintBox_Autocompletion->Invalidate();
	}

	Handled = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Autocompletion::PaintBox_AutocompletionMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	if (Button == mbLeft) {
		FVCLIDEAutomation->SetSelText(FItems->Strings[ItemIndex]);
		Close();
	}
}

//---------------------------------------------------------------------------
