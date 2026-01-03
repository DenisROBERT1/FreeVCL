//-----------------------------------------------------------------------------
//! @file TCustomCanvas.h
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

#ifdef In_TCustomCanvasH
#error "Inclusion circulaire de TCustomCanvas"
#endif // In_TCustomCanvasH

#define In_TCustomCanvasH

#ifndef TCustomCanvasH
#define TCustomCanvasH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TPen.h"
#include "TBrush.h"
#include "TFont.h"
#include "TRect.h"


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TAlignment {
	taLeftJustify,								//!< Texte calé à gauche
	taCenter,											//!< Texte centré
	taRightJustify								//!< Texte calé à droite
};

enum TAlignmentV {
	taTop,												//!< Texte calé en haut
	taBaseLine,										//!< Texte calé sur la ligne de base
	taBottom											//!< Texte calé en bas
};


//-----------------------------------------------------------------------------
//! @class TCustomCanvas
//!
//! @brief Définition de la classe TCustomCanvas
//!
//! Cette classe est la classe de base représentant une fenêtre de dessin.
//! C'est une classe abstraite: ne pas implémenter cette classe, mais ses
//! descendants.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomCanvas: public TPersistent {
protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomCanvas(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomCanvas(void);

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
  //! @brief Lecture de l'origine des coordonnées de la fenêtre
  //!
  //! Cette méthode permet de lire la valeur les coordonnées dispositif
  //!
  //! @param[in]        X Origine des coordonnées en x
  //! @param[in]        Y Origine des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL GetWindowOrg(int *X, int *Y) = 0;


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

  virtual bool FASTCALL SetWindowOrg(int X, int Y) = 0;


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

  virtual bool FASTCALL GetViewportOrg(int *X, int *Y) = 0;


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

  virtual bool FASTCALL SetViewportOrg(int X, int Y) = 0;


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

  virtual bool FASTCALL GetWindowExt(int *XExt, int *YExt) = 0;


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

  virtual bool FASTCALL SetWindowExt(int XExt, int YExt) = 0;


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

  virtual bool FASTCALL GetViewportExt(int *XExt, int *YExt) = 0;


  //---------------------------------------------------------------------------
  //! @brief Modification de l'échelle des coordonnées du viewport
  //!
  //! Cette méthode permet de modifier l'échelle des coordonnées logiques
  //!
  //! @param[in]        XExt Nouvelle échelle des coordonnées en x
  //! @param[in]        YExt Nouvelle échelle des coordonnées en y
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SetViewportExt(int XExt, int YExt) = 0;


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

  virtual bool FASTCALL DPtoLP(TPoint * lpPoints, int nCount);


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

  virtual bool FASTCALL DPtoLP(int XD, int YD, int *XL, int *YL) = 0;


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

  virtual bool FASTCALL LPtoDP(TPoint * lpPoints, int nCount);


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

  virtual bool FASTCALL LPtoDP(int XL, int YL, int *XD, int *YD) = 0;


  //---------------------------------------------------------------------------
  //! @brief Déplacement de la position courante
  //---------------------------------------------------------------------------

  virtual void FASTCALL MoveTo(int X, int Y) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un segment
  //---------------------------------------------------------------------------

  virtual void FASTCALL LineTo(int X, int Y) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une polyligne
  //---------------------------------------------------------------------------

  virtual void FASTCALL Polyline(TPoint *ptPolyline, int NbPts) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un polygone
  //---------------------------------------------------------------------------

  virtual void FASTCALL Polygon(TPoint *ptPolygon, int NbPts) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un rectangle
  //---------------------------------------------------------------------------

  virtual void FASTCALL Rectangle(const TRect &Rect);
  virtual void FASTCALL Rectangle(int X1, int Y1, int X2, int Y2) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un rectangle avec coins arrondis
  //---------------------------------------------------------------------------

  virtual void FASTCALL RoundRect(const TRect &Rect, int X3, int Y3);
  virtual void FASTCALL RoundRect(int X1, int Y1, int X2, int Y2, int X3, int Y3) = 0;

  //---------------------------------------------------------------------------
  //! @brief Remplissage d'un rectangle
  //---------------------------------------------------------------------------

  virtual void FASTCALL FillRect(const TRect &Rect);
  virtual void FASTCALL FillRect(int X1, int Y1, int X2, int Y2) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une ellipse (ou d'un cercle)
  //---------------------------------------------------------------------------

  virtual void FASTCALL Ellipse(const TRect &Rect);
  virtual void FASTCALL Ellipse(int X1, int Y1, int X2, int Y2) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un texte
  //---------------------------------------------------------------------------

  virtual void FASTCALL TextOut(int X, int Y, const AnsiString asText) = 0;

  //---------------------------------------------------------------------------
  //! @brief Retourne les caractéristique de la fonte
  //---------------------------------------------------------------------------

  virtual bool FASTCALL GetTextMetrics(LPTEXTMETRIC lptm) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une bitmap
  //---------------------------------------------------------------------------

  virtual void FASTCALL Draw(int X, int Y, TBitmap* Bitmap) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une bitmap
  //---------------------------------------------------------------------------

  virtual void FASTCALL StretchDraw(const TRect &Rect, TBitmap *Bitmap);
  virtual void FASTCALL StretchDraw(int X1, int Y1, int X2, int Y2, TBitmap *Bitmap) = 0;

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Pen
	//!
	//! Cette propriété permet de définir le stylo utilisée par l'objet
	//! TCustomCanvas. L'objet TFont passé en paramètres est copié, ce qui
	//! signifie que c'est à l'utilisateur de détruire l'objet après utilisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomCanvas, TCustomPen *, Pen);

  //---------------------------------------------------------------------------
  //! @brief Propriété Brush
	//!
	//! Cette propriété permet de définir le pinceau utilisée par l'objet
	//! TCustomCanvas. L'objet TFont passé en paramètres est copié, ce qui
	//! signifie que c'est à l'utilisateur de détruire l'objet après utilisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomCanvas, TCustomBrush *, Brush);

  //---------------------------------------------------------------------------
  //! @brief Propriété Font
	//!
	//! Cette propriété permet de définir la fonte utilisée par l'objet
	//! TCustomCanvas. L'objet TFont passé en paramètres est copié, ce qui
	//! signifie que c'est à l'utilisateur de détruire l'objet après utilisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomCanvas, TCustomFont *, Font);

  //---------------------------------------------------------------------------
  //! @brief Propriété TextAlignH
	//!
	//! Cette propriété permet de définir l'alignement horizontal du texte qui
	//! peut être calé à gauche, au centre ou à droite du point d'origine passé
	//! à la fonction TextOut.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomCanvas, TAlignment, TextAlignH);

  //---------------------------------------------------------------------------
  //! @brief Propriété TextAlignV
	//!
	//! Cette propriété permet de définir l'alignement vertical du texte qui
	//! peut être calé en haut, sur la ligne de base ou en bas du point d'origine
	//! passé à la fonction TextOut.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomCanvas, TAlignmentV, TextAlignV);

  //---------------------------------------------------------------------------
  //! @brief Propriété TextFlags
	//!
	//! Cette propriété permet de définir les options de dessin du texte (mode
	//! opaque ou transparent)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomCanvas, TTextFlags, TextFlags);

  //---------------------------------------------------------------------------
  //! @brief Propriété ClipRect
	//!
	//! Cette propriété permet de définir le rectangle de clipping du dessin.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomCanvas, TRect, ClipRect);

  //@}

};


#else  // TCustomCanvasH

class TCustomCanvas;

#endif  // TCustomCanvasH


#undef In_TCustomCanvasH
