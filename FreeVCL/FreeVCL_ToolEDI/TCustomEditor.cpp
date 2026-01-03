//-----------------------------------------------------------------------------
//! @file TCustomEditor.cpp
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

#include <TCustomEditor.h>

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TCustomEditor::TCustomEditor(TComponent* AOwner):
	TCustomControl(AOwner),
	TUndoRedoUser() {

	UndoRedo = new TUndoRedo();

	FChanged = false;

	Cursor = crNone;
	TabStop = true;
	NbBeginUpdate = 0;
	bMustRefresh = false;

	// Couleurs par défaut
#if (1 <= _MSC_VER && _MSC_VER <= 1200)
	// Visual 6: l'initialisation ne marche pas ?
	static COLORPARAM DefaultColorParams[MAX_TYPESYNTAX + 1];
	DefaultColorParams[0].Background = clWhite;
	DefaultColorParams[0].Foreground = clBlack;
	DefaultColorParams[0].bBold = false;
	DefaultColorParams[0].bItalic = false;
	DefaultColorParams[1].Background = RGB(10, 36, 106);
	DefaultColorParams[1].Foreground = clWhite;
	DefaultColorParams[1].bBold = true;
	DefaultColorParams[1].bItalic = false;
	DefaultColorParams[2].Background = clWhite;
	DefaultColorParams[2].Foreground = clBlue;
	DefaultColorParams[2].bBold = true;
	DefaultColorParams[2].bItalic = false;
	DefaultColorParams[3].Background = clWhite;
	DefaultColorParams[3].Foreground = clMaroon;
	DefaultColorParams[3].bBold = true;
	DefaultColorParams[3].bItalic = false;
	DefaultColorParams[4].Background = clWhite;
	DefaultColorParams[4].Foreground = clPurple;
	DefaultColorParams[4].bBold = true;
	DefaultColorParams[4].bItalic = false;
	DefaultColorParams[5].Background = clWhite;
	DefaultColorParams[5].Foreground = clNavy;
	DefaultColorParams[5].bBold = true;
	DefaultColorParams[5].bItalic = false;
	DefaultColorParams[6].Background = clWhite;
	DefaultColorParams[6].Foreground = clGreen;
	DefaultColorParams[6].bBold = true;
	DefaultColorParams[6].bItalic = false;
	DefaultColorParams[7].Background = clAqua;
	DefaultColorParams[7].Foreground = clGray;
	DefaultColorParams[7].bBold = false;
	DefaultColorParams[7].bItalic = false;
	DefaultColorParams[8].Background = clLtGray;
	DefaultColorParams[8].Foreground = clBlack;
	DefaultColorParams[8].bBold = false;
	DefaultColorParams[8].bItalic = false;
	DefaultColorParams[9].Background = clMoneyGreen;
	DefaultColorParams[9].Foreground = clBlack;
	DefaultColorParams[9].bBold = false;
	DefaultColorParams[9].bItalic = false;
#else // 1 <= _MSC_VER && _MSC_VER <= 1200
	static const COLORPARAM DefaultColorParams[MAX_TYPESYNTAX + 1] = {
		{clWhite, clBlack, false, false},
		{RGB(10, 36, 106), clWhite, true, false},
		{clWhite, clBlue, true, false},
		{clWhite, clMaroon, true, false},
		{clWhite, clPurple, true, false},
		{clWhite, clNavy, true, false},
		{clWhite, clGreen, true, false},
		{clAqua, clGray, false, false},
		{clLtGray, clBlack, false, false},
		{clMoneyGreen, clBlack, false, false}
	};
#endif // 1 <= _MSC_VER && _MSC_VER <= 1200

	int i;
	for (i = tsNormal; i <= MAX_TYPESYNTAX; i++) {
		FColorParams[i] = DefaultColorParams[i];
	}

	// Font
	TCanvas *ACanvas = Canvas;
	ACanvas->Font->Name = _T("Courier New");
	ACanvas->Font->Charset = ANSI_CHARSET;
	ACanvas->Font->Size = 8;
	ACanvas->Font->Pitch = fpFixed;
	ACanvas->Brush->Style = bsSolid;

	TEXTMETRIC tm;
	Canvas->GetTextMetrics(&tm);
	tmAveCharWidth = tm.tmAveCharWidth;
	tmHeight = tm.tmHeight;
	tmInternalLeading = 0; // Pour gagner de la place en hauteur (et non tm.tmInternalLeading)

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TCustomEditor::~TCustomEditor(void) {
	delete UndoRedo;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Changed
//---------------------------------------------------------------------------

bool TCustomEditor::Get_Changed(void) {
	return FChanged;
}

bool TCustomEditor::Set_Changed(bool NewChanged) {
	if (FChanged != NewChanged) {
		FChanged = NewChanged;
		OnFlagChanged(this, FChanged);
	}
	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColorParams
//---------------------------------------------------------------------------

COLORPARAM TCustomEditor::Get_ColorParams(int Index) {
	if ((TYPESYNTAX) Index < tsNormal || MAX_TYPESYNTAX < (TYPESYNTAX) Index) Index = 0;
	return FColorParams[Index];
}

bool TCustomEditor::Set_ColorParams(int Index, COLORPARAM NewColorParams) {
	if ((TYPESYNTAX) Index < tsNormal || MAX_TYPESYNTAX < (TYPESYNTAX) Index) return false;
	FColorParams[Index] = NewColorParams;
	InvalidateOrSetFlag();

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Font
//---------------------------------------------------------------------------

TCustomFont *TCustomEditor::Get_Font(void) {
	return FCanvas->Font;
}

bool TCustomEditor::Set_Font(TCustomFont *NewFont) {
	FCanvas->Font->Assign(NewFont);

	// Calcul de la largeur sur le style des caractères normaux
	// (en principe gras ou pas c'est pareil, mais en fait il semble que non)
	TFontStyles FontStyles;
	if (FColorParams[0].bBold) FontStyles << fsBold;
	FCanvas->Font->Style = FontStyles;

	TEXTMETRIC tm;
	Canvas->GetTextMetrics(&tm);
	tmAveCharWidth = tm.tmAveCharWidth;
	tmHeight = tm.tmHeight;
	tmInternalLeading = 0; // Pour gagner de la place en hauteur (et non tm.tmInternalLeading)

	ProcessResize();

	return true;
}

//---------------------------------------------------------------------------
// Demande de dessin du composant, ou bien mise à jour du flag bMustRefresh
//---------------------------------------------------------------------------

bool FASTCALL TCustomEditor::InvalidateOrSetFlag(void) {
	if (NbBeginUpdate != 0) bMustRefresh = true;
	else Invalidate();

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TCustomEditor::SetColorSyntax(TCanvas *ACanvas, TYPESYNTAX Type) {
	TFontStyles FontStyles;

	ACanvas->Brush->Style = bsSolid;
	ACanvas->Brush->Color = FColorParams[Type].Background;
	ACanvas->Font->Color = FColorParams[Type].Foreground;
	FontStyles.Clear();
	if (FColorParams[Type].bBold) FontStyles << fsBold;
	if (FColorParams[Type].bItalic) FontStyles << fsItalic;
	ACanvas->Font->Style = FontStyles;

	return true;
}

//---------------------------------------------------------------------------
// Efface le tampon d'annulation afin qu'aucune modification du texte ne
// puisse être restituée.
//---------------------------------------------------------------------------
void FASTCALL TCustomEditor::ClearUndo(void) {
	UndoRedo->Clear();
}

//---------------------------------------------------------------------------
bool FASTCALL TCustomEditor::BeginOperation(AnsiString asOperationName) {

	UndoRedo->BeginOperation(UR_MODIF, asOperationName);

	return true;
}


//---------------------------------------------------------------------------
bool FASTCALL TCustomEditor::EndOperation(void) {

	UndoRedo->EndOperation();

	return true;
}

//---------------------------------------------------------------------------
// Liste des modifications
//---------------------------------------------------------------------------
AnsiString TCustomEditor::GetModificationsList(void) {
	return UndoRedo->GetModificationsList(UR_MODIF);
}

//---------------------------------------------------------------------------
// Début d'une série de modifications (pas de raffraichissement).
//---------------------------------------------------------------------------
void FASTCALL TCustomEditor::BeginUpdate(void) {
	if (NbBeginUpdate++ == 0) bMustRefresh = false;
}

//---------------------------------------------------------------------------
// Fin d'une série de modifications (raffraichissement éventuel).
//---------------------------------------------------------------------------
void FASTCALL TCustomEditor::EndUpdate(void) {
	if (--NbBeginUpdate == 0) {
		if (bMustRefresh) {
			Invalidate();  // Pas InvalidateOrSetFlag puisque NbBeginUpdate == 0
		}
	}
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomEditor::GetListProperties(TStrings *ListProperties) {

	TCustomControl::GetListProperties(ListProperties);

	ListProperties->Add(_T("OnMessage"));
	ListProperties->Add(_T("OnChange"));
	ListProperties->Add(_T("OnFlagChanged"));
	ListProperties->Add(_T("OnPassBegin"));
	ListProperties->Add(_T("OnPassEnd"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomEditor::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("OnMessage")) {
    *asInfos = _T("(TObject *Sender, const AnsiString &Message)");
		return tpEvent;
	}
	if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnFlagChanged")) {
    *asInfos = _T("(TObject *Sender, bool Changed)");
		return tpEvent;
	}
	if (asProperty == _T("OnPassBegin")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnPassEnd")) {
    *asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	return TCustomControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomEditor::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("OnMessage")) {
		return _T("");
	}
	if (asProperty == _T("OnChange")) {
		return _T("");
	}
	if (asProperty == _T("OnFlagChanged")) {
		return _T("");
	}
	if (asProperty == _T("OnPassBegin")) {
		return _T("");
	}
	if (asProperty == _T("OnPassEnd")) {
		return _T("");
	}
	return TCustomControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomEditor::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("OnMessage")) {
		return OnMessage.AsString();
	}
	if (asProperty == _T("OnChange")) {
		return OnChange.AsString();
	}
	if (asProperty == _T("OnFlagChanged")) {
		return OnFlagChanged.AsString();
	}
	if (asProperty == _T("OnPassBegin")) {
		return OnPassBegin.AsString();
	}
	if (asProperty == _T("OnPassEnd")) {
		return OnPassEnd.AsString();
	}
	return TCustomControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomEditor::SetProperty(AnsiString asProperty, AnsiString asValue,
															TPersistent *Sender) {
	if (asProperty == _T("OnMessage")) {
		OnMessage.ObjCall = Sender;
		OnMessage = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
		OnChange = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnFlagChanged")) {
		OnFlagChanged.ObjCall = Sender;
		OnFlagChanged = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnPassBegin")) {
		OnPassBegin.ObjCall = Sender;
		OnPassBegin = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnPassEnd")) {
		OnPassEnd.ObjCall = Sender;
		OnPassEnd = asValue.c_str();
		return true;
	}
	return TCustomControl::SetProperty(asProperty, asValue, Sender);
}
//---------------------------------------------------------------------------
