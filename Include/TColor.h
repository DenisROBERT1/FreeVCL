//-----------------------------------------------------------------------------
//! @file TColor.h
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


//-----------------------------------------------------------------------------
// Dispositif anti-redéfinition
//-----------------------------------------------------------------------------

#ifdef In_TColorH
#error "Inclusion circulaire de TColor"
#endif // In_TColorH

#define In_TColorH

#ifndef TColorH
#define TColorH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// Constantes
//---------------------------------------------------------------------------

#define clNone                     RGB(  0,   0,   0)
#define clAqua                     RGB(  0, 255, 255)
#define clBlack                    RGB(  0,   0,   0)
#define clBlue                     RGB(  0,   0, 255)
#define clCream                    RGB(255, 251, 240)
#define clDkGray                   RGB(128, 128, 128)
#define clFuchsia                  RGB(255,   0, 255)
#define clGray                     RGB(128, 128, 128)
#define clGreen                    RGB(  0, 128,   0)
#define clLime                     RGB(  0, 255,   0)
#define clLtGray                   RGB(192, 192, 192)
#define clMaroon                   RGB(128,   0,   0)
#define clMedGray                  RGB(160, 160, 164)
#define clMoneyGreen               RGB(192, 220, 192)
#define clNavy                     RGB(  0,   0, 128)
#define clOlive                    RGB(128, 128,   0)
#define clPurple                   RGB(128,   0, 128)
#define clRed                      RGB(255,   0,   0)
#define clSilver                   RGB(192, 192, 192)
#define clSkyBlue                  RGB(166, 202, 240)
#define clTeal                     RGB(  0, 128, 128)
#define clWhite                    RGB(255, 255, 255)
#define clYellow                   RGB(255, 255,   0)

#define clScrollBar                GetSysColor(COLOR_SCROLLBAR)
#define clBackground               GetSysColor(COLOR_BACKGROUND)
#define clActiveCaption            GetSysColor(COLOR_ACTIVECAPTION)
#define clInactiveCaption          GetSysColor(COLOR_INACTIVECAPTION)
#define clMenu                     GetSysColor(COLOR_MENU)
#define clWindow                   GetSysColor(COLOR_WINDOW)
#define clWindowFrame              GetSysColor(COLOR_WINDOWFRAME)
#define clMenuText                 GetSysColor(COLOR_MENUTEXT)
#define clWindowText               GetSysColor(COLOR_WINDOWTEXT)
#define clCaptionText              GetSysColor(COLOR_CAPTIONTEXT)
#define clActiveBorder             GetSysColor(COLOR_ACTIVEBORDER)
#define clInactiveBorder           GetSysColor(COLOR_INACTIVEBORDER)
#define clAppWorkSpace             GetSysColor(COLOR_APPWORKSPACE)
#define clHighlight                GetSysColor(COLOR_HIGHLIGHT)
#define clHighlightText            GetSysColor(COLOR_HIGHLIGHTTEXT)
#define clBtnFace                  GetSysColor(COLOR_BTNFACE)
#define clBtnShadow                GetSysColor(COLOR_BTNSHADOW)
#define clGrayText                 GetSysColor(COLOR_GRAYTEXT)
#define clBtnText                  GetSysColor(COLOR_BTNTEXT)
#define clInactiveCaptionText      GetSysColor(COLOR_INACTIVECAPTIONTEXT)
#define clBtnHighlight             GetSysColor(COLOR_BTNHILIGHT)
#define cl3DFace                   GetSysColor(COLOR_3DFACE)
#define cl3DShadow                 GetSysColor(COLOR_3DSHADOW)
#define cl3DDkShadow               GetSysColor(COLOR_3DDKSHADOW)
#define cl3DLight                  GetSysColor(COLOR_3DLIGHT)
#define cl3DHilight                GetSysColor(COLOR_3DHILIGHT)
#define cl3DHighlight              GetSysColor(COLOR_3DHIGHLIGHT)
#define clInfoText                 GetSysColor(COLOR_INFOTEXT)
#define clInfoBk                   GetSysColor(COLOR_INFOBK)
#define clGradientActiveCaption    GetSysColor(COLOR_ACTIVECAPTION)
#define clGradientInactiveCaption  GetSysColor(COLOR_INACTIVECAPTION)
#define clDefault                  GetSysColor(COLOR_DESKTOP)


//---------------------------------------------------------------------------
//
//                         fonctions globales
//
//---------------------------------------------------------------------------

extern COLORREF FASTCALL ColorToRGB(TColor Color);
extern bool FASTCALL ColorToIdent(COLORREF Color, AnsiString &asIdent);
extern bool FASTCALL IdentToColor(const AnsiString asIdent, TColor &Color);
extern AnsiString FASTCALL ColorToString(TColor Color);
extern TColor FASTCALL StringToColor(const AnsiString asString);


//-----------------------------------------------------------------------------
//! @class TColor
//!
//! @brief Définition de la classe TColor
//!
//! Cette classe représente une couleur système définie par ses trois
//! composantes de couleurs fondamentales rouge vert et bleu.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TColor {
public:

  COLORREF cr;

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TColor(void);
  TColor(DWORD dwRGB);
  TColor(BYTE R, BYTE G, BYTE B);

  //@}


  //---------------------------------------------------------------------------
  //! @brief Opérateurs
  //---------------------------------------------------------------------------

  bool operator == (const TColor Color);

  bool operator != (const TColor Color);

  bool operator = (COLORREF crSrc);

  bool operator = (AnsiString sSrc);

  //---------------------------------------------------------------------------
  //! @brief Méthodes
  //---------------------------------------------------------------------------

  BYTE RValue(void);
  BYTE GValue(void);
  BYTE BValue(void);
  AnsiString ColorToString();

};

#else  // TColorH

class TColor;

#endif  // TColorH


#undef In_TColorH
