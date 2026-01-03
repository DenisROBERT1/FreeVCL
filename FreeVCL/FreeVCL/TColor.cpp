//-----------------------------------------------------------------------------
//! @file TColor.cpp
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
extern TApplication *Application;
extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// Fonctions globales
//---------------------------------------------------------------------------

COLORREF FASTCALL ColorToRGB(TColor Color) {
	return Color.cr;
}


//---------------------------------------------------------------------------
// TColor
//---------------------------------------------------------------------------

TColor::TColor(void) {
  // Initialisations
  cr = 0;
}

TColor::TColor(DWORD dwRGB) {
  // Initialisations
  cr = dwRGB;
}

TColor::TColor(BYTE R, BYTE G, BYTE B) {
  // Initialisations
  cr = RGB(R, G, B);
}

//---------------------------------------------------------------------------
bool TColor::operator == (const TColor Color) {
  return cr == Color.cr;
}

//---------------------------------------------------------------------------
bool TColor::operator != (const TColor Color) {
  return cr != Color.cr;
}

//---------------------------------------------------------------------------
bool TColor::operator = (COLORREF crSrc) {
  cr = crSrc;
  return true;
}

//---------------------------------------------------------------------------
bool TColor::operator = (AnsiString asSrc) {
	*this = StringToColor(asSrc);

  return true;
}

//---------------------------------------------------------------------------
BYTE TColor::RValue(void) {
  return GetRValue(cr);
}

//---------------------------------------------------------------------------
BYTE TColor::GValue(void) {
  return GetGValue(cr);
}

//---------------------------------------------------------------------------
BYTE TColor::BValue(void) {
  return GetBValue(cr);
}

//---------------------------------------------------------------------------
bool FASTCALL ColorToIdent(COLORREF Color, AnsiString &asIdent) {

	if (Color == clNone) asIdent = _T("clNone");
  else if (Color == clAqua) asIdent = _T("clAqua");
  else if (Color == clBlack) asIdent = _T("clBlack");
  else if (Color == clBlue) asIdent = _T("clBlue");
  else if (Color == clCream) asIdent = _T("clCream");
  else if (Color == clDkGray) asIdent = _T("clDkGray");
  else if (Color == clFuchsia) asIdent = _T("clFuchsia");
  else if (Color == clGray) asIdent = _T("clGray");
  else if (Color == clGreen) asIdent = _T("clGreen");
  else if (Color == clLime) asIdent = _T("clLime");
  else if (Color == clLtGray) asIdent = _T("clLtGray");
  else if (Color == clMaroon) asIdent = _T("clMaroon");
  else if (Color == clMedGray) asIdent = _T("clMedGray");
  else if (Color == clMoneyGreen) asIdent = _T("clMoneyGreen");
  else if (Color == clNavy) asIdent = _T("clNavy");
  else if (Color == clOlive) asIdent = _T("clOlive");
  else if (Color == clPurple) asIdent = _T("clPurple");
  else if (Color == clRed) asIdent = _T("clRed");
  else if (Color == clSilver) asIdent = _T("clSilver");
  else if (Color == clSkyBlue) asIdent = _T("clSkyBlue");
  else if (Color == clTeal) asIdent = _T("clTeal");
  else if (Color == clWhite) asIdent = _T("clWhite");
  else if (Color == clYellow) asIdent = _T("clYellow");

  // Couleurs système
  else if (Color == GetSysColor(COLOR_BTNFACE)) asIdent = _T("clBtnFace");
  else if (Color == GetSysColor(COLOR_SCROLLBAR)) asIdent = _T("clSColorollBar");
  else if (Color == GetSysColor(COLOR_BACKGROUND)) asIdent = _T("clBackground");
  else if (Color == GetSysColor(COLOR_ACTIVECAPTION)) asIdent = _T("clActiveCaption");
  else if (Color == GetSysColor(COLOR_INACTIVECAPTION)) asIdent = _T("clInactiveCaption");
  else if (Color == GetSysColor(COLOR_MENU)) asIdent = _T("clMenu");
  else if (Color == GetSysColor(COLOR_WINDOW)) asIdent = _T("clWindow");
  else if (Color == GetSysColor(COLOR_WINDOWFRAME)) asIdent = _T("clWindowFrame");
  else if (Color == GetSysColor(COLOR_MENUTEXT)) asIdent = _T("clMenuText");
  else if (Color == GetSysColor(COLOR_WINDOWTEXT)) asIdent = _T("clWindowText");
  else if (Color == GetSysColor(COLOR_CAPTIONTEXT)) asIdent = _T("clCaptionText");
  else if (Color == GetSysColor(COLOR_ACTIVEBORDER)) asIdent = _T("clActiveBorder");
  else if (Color == GetSysColor(COLOR_INACTIVEBORDER)) asIdent = _T("clInactiveBorder");
  else if (Color == GetSysColor(COLOR_APPWORKSPACE)) asIdent = _T("clAppWorkSpace");
  else if (Color == GetSysColor(COLOR_HIGHLIGHT)) asIdent = _T("clHighlight");
  else if (Color == GetSysColor(COLOR_HIGHLIGHTTEXT)) asIdent = _T("clHightlightText");
  else if (Color == GetSysColor(COLOR_BTNSHADOW)) asIdent = _T("clBtnShadow");
  else if (Color == GetSysColor(COLOR_GRAYTEXT)) asIdent = _T("clGrayText");
  else if (Color == GetSysColor(COLOR_BTNTEXT)) asIdent = _T("clBtnText");
  else if (Color == GetSysColor(COLOR_INACTIVECAPTIONTEXT)) asIdent = _T("clInactiveCaptionText");
  else if (Color == GetSysColor(COLOR_BTNHILIGHT)) asIdent = _T("clBtnHighlight");
  else if (Color == GetSysColor(COLOR_3DFACE)) asIdent = _T("cl3DFace");
  else if (Color == GetSysColor(COLOR_3DSHADOW)) asIdent = _T("cl3DShadow");
  else if (Color == GetSysColor(COLOR_3DDKSHADOW)) asIdent = _T("cl3DDkShadow");
  else if (Color == GetSysColor(COLOR_3DLIGHT)) asIdent = _T("cl3DLight");
  else if (Color == GetSysColor(COLOR_3DHILIGHT)) asIdent = _T("cl3DHilight");
  else if (Color == GetSysColor(COLOR_3DHIGHLIGHT)) asIdent = _T("cl3DHighlight");
  else if (Color == GetSysColor(COLOR_INFOTEXT)) asIdent = _T("clInfoText");
  else if (Color == GetSysColor(COLOR_INFOBK)) asIdent = _T("clInfoBk");
  else if (Color == GetSysColor(COLOR_ACTIVECAPTION)) asIdent = _T("clGradientActiveCaption");
  else if (Color == GetSysColor(COLOR_INACTIVECAPTION)) asIdent = _T("clGradientInactiveCaption");
  else if (Color == GetSysColor(COLOR_DESKTOP)) asIdent = _T("clDefault");
	else return false;

	return true;
}

//---------------------------------------------------------------------------
bool FASTCALL IdentToColor(const AnsiString asIdent, TColor &Color) {

  // Couleurs standard
  if (asIdent == _T("clNone")) Color = clNone;
  else if (asIdent == _T("clAqua")) Color = clAqua;
  else if (asIdent == _T("clBlack")) Color = clBlack;
  else if (asIdent == _T("clBlue")) Color = clBlue;
  else if (asIdent == _T("clCream")) Color = clCream;
  else if (asIdent == _T("clDkGray")) Color = clDkGray;
  else if (asIdent == _T("clFuchsia")) Color = clFuchsia;
  else if (asIdent == _T("clGray")) Color = clGray;
  else if (asIdent == _T("clGreen")) Color = clGreen;
  else if (asIdent == _T("clLime")) Color = clLime;
  else if (asIdent == _T("clLtGray")) Color = clLtGray;
  else if (asIdent == _T("clMaroon")) Color = clMaroon;
  else if (asIdent == _T("clMedGray")) Color = clMedGray;
  else if (asIdent == _T("clMoneyGreen")) Color = clMoneyGreen;
  else if (asIdent == _T("clNavy")) Color = clNavy;
  else if (asIdent == _T("clOlive")) Color = clOlive;
  else if (asIdent == _T("clPurple")) Color = clPurple;
  else if (asIdent == _T("clRed")) Color = clRed;
  else if (asIdent == _T("clSilver")) Color = clSilver;
  else if (asIdent == _T("clSkyBlue")) Color = clSkyBlue;
  else if (asIdent == _T("clTeal")) Color = clTeal;
  else if (asIdent == _T("clWhite")) Color = clWhite;
  else if (asIdent == _T("clYellow")) Color = clYellow;

  // Couleurs système
  else if (asIdent == _T("clScrollBar")) Color = GetSysColor(COLOR_SCROLLBAR);
  else if (asIdent == _T("clBackground")) Color = GetSysColor(COLOR_BACKGROUND);
  else if (asIdent == _T("clActiveCaption")) Color = GetSysColor(COLOR_ACTIVECAPTION);
  else if (asIdent == _T("clInactiveCaption")) Color = GetSysColor(COLOR_INACTIVECAPTION);
  else if (asIdent == _T("clMenu")) Color = GetSysColor(COLOR_MENU);
  else if (asIdent == _T("clWindow")) Color = GetSysColor(COLOR_WINDOW);
  else if (asIdent == _T("clWindowFrame")) Color = GetSysColor(COLOR_WINDOWFRAME);
  else if (asIdent == _T("clMenuText")) Color = GetSysColor(COLOR_MENUTEXT);
  else if (asIdent == _T("clWindowText")) Color = GetSysColor(COLOR_WINDOWTEXT);
  else if (asIdent == _T("clCaptionText")) Color = GetSysColor(COLOR_CAPTIONTEXT);
  else if (asIdent == _T("clActiveBorder")) Color = GetSysColor(COLOR_ACTIVEBORDER);
  else if (asIdent == _T("clInactiveBorder")) Color = GetSysColor(COLOR_INACTIVEBORDER);
  else if (asIdent == _T("clAppWorkSpace")) Color = GetSysColor(COLOR_APPWORKSPACE);
  else if (asIdent == _T("clHighlight")) Color = GetSysColor(COLOR_HIGHLIGHT);
  else if (asIdent == _T("clHightlightText")) Color = GetSysColor(COLOR_HIGHLIGHTTEXT);
  else if (asIdent == _T("clBtnFace")) Color = GetSysColor(COLOR_BTNFACE);
  else if (asIdent == _T("clBtnShadow")) Color = GetSysColor(COLOR_BTNSHADOW);
  else if (asIdent == _T("clGrayText")) Color = GetSysColor(COLOR_GRAYTEXT);
  else if (asIdent == _T("clBtnText")) Color = GetSysColor(COLOR_BTNTEXT);
  else if (asIdent == _T("clInactiveCaptionText")) Color = GetSysColor(COLOR_INACTIVECAPTIONTEXT);
  else if (asIdent == _T("clBtnHighlight")) Color = GetSysColor(COLOR_BTNHILIGHT);
  else if (asIdent == _T("cl3DFace")) Color = GetSysColor(COLOR_3DFACE);
  else if (asIdent == _T("cl3DShadow")) Color = GetSysColor(COLOR_3DSHADOW);
  else if (asIdent == _T("cl3DDkShadow")) Color = GetSysColor(COLOR_3DDKSHADOW);
  else if (asIdent == _T("cl3DLight")) Color = GetSysColor(COLOR_3DLIGHT);
  else if (asIdent == _T("cl3DHilight")) Color = GetSysColor(COLOR_3DHILIGHT);
  else if (asIdent == _T("cl3DHighlight")) Color = GetSysColor(COLOR_3DHIGHLIGHT);
  else if (asIdent == _T("clInfoText")) Color = GetSysColor(COLOR_INFOTEXT);
  else if (asIdent == _T("clInfoBk")) Color = GetSysColor(COLOR_INFOBK);
  else if (asIdent == _T("clGradientActiveCaption")) Color = GetSysColor(COLOR_ACTIVECAPTION);
  else if (asIdent == _T("clGradientInactiveCaption")) Color = GetSysColor(COLOR_INACTIVECAPTION);
  else if (asIdent == _T("clDefault")) Color = GetSysColor(COLOR_DESKTOP);
	else return false;

	return true;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL ColorToString(TColor Color) {
	return Color.ColorToString();
}

//---------------------------------------------------------------------------
TColor FASTCALL StringToColor(const AnsiString asString) {
	TColor Color;


	if (!IdentToColor(asString, Color)) {
		// Couleur RGB
		if (asString.Length() > 2 && asString[1] == _T('0') && asString[2] == _T('x')) {
			int crValue;
			TCHAR c;
			crValue = 0;
			for (int i = 3; i <= (int) asString.Length(); i++) {
				crValue <<= 4;
				c = asString[i];
				if (_T('0') <= c && c <= _T('9')) crValue += c - _T('0');
				else if (_T('a') <= c && c <= _T('f')) crValue += c - _T('a') + 10;
				else if (_T('A') <= c && c <= _T('F')) crValue += c - _T('A') + 10;
			}
			Color = crValue;
		}
		else Color = StrToInt(asString);
	}

	return Color;
}

//---------------------------------------------------------------------------
AnsiString TColor::ColorToString(void) {
	AnsiString asIdent;


	if (ColorToIdent(cr, asIdent)) {
		return asIdent;
	}
	else {
		TCHAR szBuf[20];
		wsprintf(szBuf, _T("0x%08X"), cr);
		return szBuf;
	}
}

//---------------------------------------------------------------------------

