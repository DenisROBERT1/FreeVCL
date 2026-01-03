//-----------------------------------------------------------------------------
//! @file TCanvas.h
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

#ifdef In_TCanvasH
#error "Inclusion circulaire de TCanvas"
#endif // In_TCanvasH

#define In_TCanvasH

#ifndef TCanvasH
#define TCanvasH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TCustomCanvas.h"
#include "TPen.h"
#include "TBrush.h"
#include "TFont.h"


//-----------------------------------------------------------------------------
//! @class TCanvas
//!
//! @brief Définition de la classe TCanvas
//!
//! Cette classe encapsule une zone de dessin système (basée sur un HDC),
//! permettant de dessiner sur un écran, une imprimante ou certains autres
//! périphériques conçus pour (PDF via imprimante virtuelle, ...). Elle
//! implémente les méthodes virtuelles de la classe @b TCustomCanvas.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCanvas: public TCustomCanvas {
protected:
  HDC FHandle;
  TPen *FPen;
  TBrush *FBrush;
  TFont *FFont;
  UINT FTextAlign;
  TTextFlags FTextFlags;
  TRect FClipRect;
  HRGN hClipRect;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCanvas(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCanvas(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Pixels
  //!
  //! Cette méthode permet de lire la propriété Pixels
  //!
  //! @param[in]        x Coordonnée en x du pixel à écrire
  //! @param[in]        y Coordonnée en y du pixel à écrire
  //!
  //! @return @c TColor Propriété Pixels
  //---------------------------------------------------------------------------

  virtual TColor Get_Pixels(int x, int y);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Pixels
  //!
  //! Cette méthode permet d'écrire la propriété Pixels
  //!
  //! @param[in]        x Coordonnée en x du pixel à écrire
  //! @param[in]        y Coordonnée en y du pixel à écrire
  //! @param[in]        NewPixels Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Pixels(int x, int y, TColor NewPixels);


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
  //! @brief Getter de la propriété TextFlags
  //!
  //! Cette méthode permet de lire la propriété TextFlags
  //!
  //! @return @c int Propriété TextFlags
  //---------------------------------------------------------------------------

  virtual TTextFlags Get_TextFlags(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété TextFlags
  //!
  //! Cette méthode permet d'écrire la propriété TextFlags
  //!
  //! @param[in]        NewTextFlags Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_TextFlags(TTextFlags NewTextFlags);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété ClipRect
  //!
  //! Cette méthode permet de lire la propriété ClipRect
  //!
  //! @return @c TRect Propriété ClipRect
  //---------------------------------------------------------------------------

  virtual TRect Get_ClipRect(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété ClipRect
  //!
  //! Cette méthode permet d'écrire la propriété ClipRect
  //!
  //! @param[in]        NewClipRect Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_ClipRect(TRect NewClipRect);


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
  //! @param[in ,out]   lpPoints Tableau de points à convertir
  //! @param[in]        nCount Nombre de points dans le tableau
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL DPtoLP(TPoint * lpPoints, int nCount) {return TCustomCanvas::DPtoLP(lpPoints, nCount);}


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


  //---------------------------------------------------------------------------
  //! @brief Conversion de coordonnées logiques vers coordonnées dispositif.
  //!
  //! Cette méthode permet de calculer les coordonnées dispositif à partir de
  //! coordonnées logiques.
  //!
  //! @param[in ,out]   lpPoints Tableau de points à convertir
  //! @param[in]        nCount Nombre de points dans le tableau
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LPtoDP(TPoint * lpPoints, int nCount) {return TCustomCanvas::LPtoDP(lpPoints, nCount);}


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


  //---------------------------------------------------------------------------
  //! @brief Déplacement de la position courante
  //---------------------------------------------------------------------------

  virtual void FASTCALL MoveTo(int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un segment
  //---------------------------------------------------------------------------

  virtual void FASTCALL LineTo(int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une polyligne
  //---------------------------------------------------------------------------

  virtual void FASTCALL Polyline(TPoint *Polyline, int NbPts);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un polygone
  //---------------------------------------------------------------------------

  virtual void FASTCALL Polygon(TPoint *Polyline, int NbPts);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un rectangle
  //---------------------------------------------------------------------------

  virtual void FASTCALL Rectangle(const TRect &Rect) {TCustomCanvas::Rectangle(Rect);}
  virtual void FASTCALL Rectangle(int X1, int Y1, int X2, int Y2);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un rectangle avec coins arrondis
  //---------------------------------------------------------------------------

  virtual void FASTCALL RoundRect(TRect &Rect, int X3, int Y3) {TCustomCanvas::RoundRect(Rect, X3, Y3);}
  virtual void FASTCALL RoundRect(int X1, int Y1, int X2, int Y2, int X3, int Y3);

  //---------------------------------------------------------------------------
  //! @brief Remplissage d'un rectangle
  //---------------------------------------------------------------------------

  virtual void FASTCALL FillRect(const TRect &Rect) {TCustomCanvas::FillRect(Rect);}
  virtual void FASTCALL FillRect(int X1, int Y1, int X2, int Y2);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une ellipse (ou d'un cercle)
  //---------------------------------------------------------------------------

  virtual void FASTCALL Ellipse(const TRect &Rect) {TCustomCanvas::Ellipse(Rect);}
  virtual void FASTCALL Ellipse(int X1, int Y1, int X2, int Y2);

  //---------------------------------------------------------------------------
  //! @brief Transfert d'un rectangle
  //---------------------------------------------------------------------------

  virtual bool FASTCALL CopyRect(TRect RectDest, TCanvas *CanvasSrc, TRect RectSrc);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un texte
  //---------------------------------------------------------------------------

  virtual void FASTCALL TextOut(int X, int Y, const AnsiString Text);

  //---------------------------------------------------------------------------
  //! @brief Retourne les caractéristique de la fonte
  //---------------------------------------------------------------------------

  virtual bool FASTCALL GetTextMetrics(LPTEXTMETRIC lptm);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une bitmap
  //---------------------------------------------------------------------------

  virtual void FASTCALL Draw(int X, int Y, TBitmap* Bitmap);

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une bitmap
  //---------------------------------------------------------------------------

  virtual void FASTCALL StretchDraw(TRect &Rect, TBitmap *Bitmap) {TCustomCanvas::StretchDraw(Rect, Bitmap);}
  virtual void FASTCALL StretchDraw(int X1, int Y1, int X2, int Y2, TBitmap *Bitmap);

  //---------------------------------------------------------------------------
  //! @brief Indique si l'objet TCanvas a acquis un HDC
  //---------------------------------------------------------------------------

  virtual bool FASTCALL HandleAllocated(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du Canvas
	//!
	//! Cette propriété permet de lire et de définir le handle système du
	//! canevas. L'objet @b TCanvas ne devient pas propriétaire de l'objet défini
	//! par ce handle, il revient à l'utilisateur de le détruire après
	//! la destruction de l'objet @b TCanvas (sauf si c'est un handle fourni par
	//! le système bien sûr).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCanvas, HDC, Handle);

  //@}

};


#else  // TCanvasH

class TCanvas;

#endif  // TCanvasH


#undef In_TCanvasH
