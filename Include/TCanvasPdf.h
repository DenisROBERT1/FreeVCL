//-----------------------------------------------------------------------------
//! @file TCanvasPdf.h
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

#ifdef In_TCanvasPdfH
#error "Inclusion circulaire de TCanvasPdf"
#endif // In_TCanvasPdfH

#define In_TCanvasPdfH

#ifndef TCanvasPdfH
#define TCanvasPdfH

#include <TCustomCanvas.h>


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TColorPdf;
class TPenPdf;
class TBrushPdf;
class TFontPdf;


//---------------------------------------------------------------------------
//! @class TCanvasPdf
//!
//! @brief Canvas PDF
//!
//! Cette classe (@c TCanvasPdf) fonctionne comme la classe TCanvas. Comme
//! elle, elle comporte des propriétés Pen, Brush et Font, sauf qu'il s'agit
//! ici d'objets prévus pour les fichiers PDF.
//!
//! @author Denis ROBERT
//!
//! @sa TCustomCanvas
//---------------------------------------------------------------------------

class TCanvasPdf: public TCustomCanvas {
private:

protected:
  CharString FStream;
  bool bLine;
  TColorPdf *ColorPdf;
  TPenPdf * FPen;
  TBrushPdf * FBrush;
  TFontPdf * FFont;
  UINT FTextAlign;

  int WidthPage;
  int HeightPage;

  int WindowOrgX;
  int WindowOrgY;
  int WindowExtX;
  int WindowExtY;
  int ViewportOrgX;
  int ViewportOrgY;
  int ViewportExtX;
  int ViewportExtY;

  CharString AsciiHexa(CharString csIn);
  CharString Ascii85(CharString csIn);
  void ReOrdonne(int *X1, int *X2);
  bool LPtoPdf(int XL, int YL, int *XPdf, int *YPdf);


public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCanvasPdf(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCanvasPdf(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Pen
  //!
  //! Cette méthode permet de lire la propriété Pen
  //!
  //! @return @c TCustomPen* Propriété Pen
  //---------------------------------------------------------------------------

  virtual TCustomPen* Get_Pen(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Pen
  //!
  //! Cette méthode permet d'écrire la propriété Pen
  //!
  //! @param[in]        NewPen Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Pen(TCustomPen* NewPen);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Brush
  //!
  //! Cette méthode permet de lire la propriété Brush
  //!
  //! @return @c TCustomBrush* Propriété Brush
  //---------------------------------------------------------------------------

  virtual TCustomBrush* Get_Brush(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Brush
  //!
  //! Cette méthode permet d'écrire la propriété Brush
  //!
  //! @param[in]        NewBrush Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Brush(TCustomBrush* NewBrush);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Font
  //!
  //! Cette méthode permet de lire la propriété Font
  //!
  //! @return @c TCustomFont* Propriété Font
  //---------------------------------------------------------------------------

  virtual TCustomFont* Get_Font(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Font
  //!
  //! Cette méthode permet d'écrire la propriété Font
  //!
  //! @param[in]        NewFont Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Font(TCustomFont* NewFont);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété TextAlignH
  //!
  //! Cette méthode permet de lire la propriété TextAlignH
  //!
  //! @return @c TAlignment Propriété TextAlignH
  //---------------------------------------------------------------------------

  virtual TAlignment Get_TextAlignH(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété TextAlignH
  //!
  //! Cette méthode permet d'écrire la propriété TextAlignH
  //!
  //! @param[in]        NewTextAlignH Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_TextAlignH(TAlignment NewTextAlignH);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété TextAlignV
  //!
  //! Cette méthode permet de lire la propriété TextAlignV
  //!
  //! @return @c TAlignmentV Propriété TextAlignV
  //---------------------------------------------------------------------------

  virtual TAlignmentV Get_TextAlignV(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété TextAlignV
  //!
  //! Cette méthode permet d'écrire la propriété TextAlignV
  //!
  //! @param[in]        NewTextAlignV Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_TextAlignV(TAlignmentV NewTextAlignV);


  //---------------------------------------------------------------------------
  //! @brief Lecture de l'origine des coordonnées de la fenêtre
  //!
  //! Cette méthode permet de lire la valeur les coordonnées dispositif
  //!
  //! @param[in]        X Origine des coordonnées en x
  //! @param[in]        Y Origine des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL GetWindowOrg(int *X, int *Y);


  //---------------------------------------------------------------------------
  //! @brief Modification de l'origine des coordonnées de la fenêtre
  //!
  //! Cette méthode permet de translater les coordonnées dispositif
  //!
  //! @param[in]        X Nouvelle origine des coordonnées en x
  //! @param[in]        Y Nouvelle origine des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SetWindowOrg(int X, int Y);


  //---------------------------------------------------------------------------
  //! @brief Lecture de l'origine des coordonnées du viewport
  //!
  //! Cette méthode permet de lire la valeur les coordonnées logiques
  //!
  //! @param[in]        X Origine des coordonnées en x
  //! @param[in]        Y Origine des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL GetViewportOrg(int *X, int *Y);


  //---------------------------------------------------------------------------
  //! @brief Modification de l'origine des coordonnées du viewport
  //!
  //! Cette méthode permet de translater les coordonnées logiques
  //!
  //! @param[in]        X Nouvelle origine des coordonnées en x
  //! @param[in]        Y Nouvelle origine des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SetViewportOrg(int X, int Y);


  //---------------------------------------------------------------------------
  //! @brief Lecture de l'échelle des coordonnées de la fenêtre
  //!
  //! Cette méthode permet de lire l'échelle des coordonnées dispositif
  //!
  //! @param[in]        XExt Echelle des coordonnées en x
  //! @param[in]        YExt Echelle des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL GetWindowExt(int *XExt, int *YExt);


  //---------------------------------------------------------------------------
  //! @brief Modification de l'échelle des coordonnées de la fenêtre
  //!
  //! Cette méthode permet de modifier l'échelle des coordonnées dispositif
  //!
  //! @param[in]        XExt Nouvelle échelle des coordonnées en x
  //! @param[in]        YExt Nouvelle échelle des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SetWindowExt(int XExt, int YExt);


  //---------------------------------------------------------------------------
  //! @brief Lecture de l'échelle des coordonnées du viewport
  //!
  //! Cette méthode permet de lire l'échelle des coordonnées logiques
  //!
  //! @param[in]        XExt Echelle des coordonnées en x
  //! @param[in]        YExt Echelle des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL GetViewportExt(int *XExt, int *YExt);


  //---------------------------------------------------------------------------
  //! @brief Modification de l'échelle des coordonnées du viewport
  //!
  //! Cette méthode permet modifier l'échelle des coordonnées logiques
  //!
  //! @param[in]        XExt Nouvelle échelle des coordonnées en x
  //! @param[in]        YExt Nouvelle échelle des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SetViewportExt(int XExt, int YExt);


  //---------------------------------------------------------------------------
  //! @brief Conversion de coordonnées dispositif vers coordonnées logiques.
  //!
  //! Cette méthode permet de calculer les coordonnées logiques à partir de
  //! coordonnées dispositif.
  //!
  //! @param[in]        XD Coordonnée X à convertir
  //! @param[in]        YD Coordonnée Y à convertir
  //! @param[out]       XL Coordonnée X convertie
  //! @param[out]       YL Coordonnée Y convertie
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL DPtoLP(int XD, int YD, int *XL, int *YL);
  virtual bool FASTCALL DPtoLP(TPoint * lpPoints, int nCount) {return TCustomCanvas::DPtoLP(lpPoints, nCount);}


  //---------------------------------------------------------------------------
  //! @brief Conversion de coordonnées logiques vers coordonnées dispositif.
  //!
  //! Cette méthode permet de calculer les coordonnées dispositif à partir de
  //! coordonnées logiques.
  //!
  //! @param[in]        XL Coordonnée X à convertir
  //! @param[in]        YL Coordonnée Y à convertir
  //! @param[out]       XD Coordonnée X convertie
  //! @param[out]       YD Coordonnée Y convertie
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LPtoDP(int XL, int YL, int *XD, int *YD);
  virtual bool FASTCALL LPtoDP(TPoint * lpPoints, int nCount) {return TCustomCanvas::LPtoDP(lpPoints, nCount);}


  //---------------------------------------------------------------------------
  virtual void FASTCALL MoveTo(int X, int Y);

  virtual void FASTCALL LineTo(int X, int Y);
  virtual void FASTCALL Polyline(TPoint *ptPolyline, int NbPts);
  virtual void FASTCALL Polygon(TPoint *ptPolygon, int NbPts);

  virtual void FASTCALL Rectangle(TRect &Rect) {TCustomCanvas::Rectangle(Rect);}
  virtual void FASTCALL Rectangle(int X1, int Y1, int X2, int Y2);
  virtual void FASTCALL RoundRect(TRect &Rect, int X3, int Y3) {TCustomCanvas::RoundRect(Rect, X3, Y3);}
  virtual void FASTCALL RoundRect(int X1, int Y1, int X2, int Y2, int X3, int Y3);
  virtual void FASTCALL FillRect(TRect &Rect) {TCustomCanvas::FillRect(Rect);}
  virtual void FASTCALL FillRect(int X1, int Y1, int X2, int Y2);
  virtual void FASTCALL Ellipse(TRect &Rect) {TCustomCanvas::Ellipse(Rect);}
  virtual void FASTCALL Ellipse(int X1, int Y1, int X2, int Y2);

  virtual void FASTCALL TextOut(int X, int Y, const AnsiString asText);
  virtual bool FASTCALL GetTextMetrics(LPTEXTMETRIC lptm);

  virtual void FASTCALL Draw(int X, int Y, TBitmap *Graphic);
  virtual void FASTCALL StretchDraw(TRect &Rect, TBitmap *Bitmap) {TCustomCanvas::StretchDraw(Rect, Bitmap);}
  virtual void FASTCALL StretchDraw(int X1, int Y1, int X2, int Y2, TBitmap *Bitmap);

  void Clear(void);
  void EndDraw(void);

  //---------------------------------------------------------------------------
  //! @brief Détermine la taille de la page.
  //!
  //! Cette fonction définit la taille de la page Pdf (nécessaire au calcul des
  //! coordonnées).
  //!
  //! @param[in]        Width Largeur de la page en points
  //! @param[in]        Height Hauteur de la page en points
  //---------------------------------------------------------------------------

  void SetPageSize(int Width, int Height);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Stream
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCanvasPdf, CharString, Stream);

  //@}

};


#endif

#undef In_TCanvasPdfH
