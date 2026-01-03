//-----------------------------------------------------------------------------
//! @file TRect.h
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

#ifdef In_TRectH
#error "Inclusion circulaire de TRect"
#endif // In_TRectH

#define In_TRectH

#ifndef TRectH
#define TRectH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//! @class TRect
//!
//! @brief Définition de la classe TRect
//!
//! Cette classe encapsule un rectangle défini par son point supérieur gauche
//! et par une largeur et une hauteur.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TRect: public RECT {
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TRect(void);

  TRect(int l, int t, int r, int b);

  TRect(const TPoint& Pt1, const TPoint& Pt2);

  TRect(RECT& r);

  //@}


  //---------------------------------------------------------------------------
  //! @brief Opérateurs
  //---------------------------------------------------------------------------

  bool operator == (const TRect &Rect);

  bool operator != (const TRect &Rect);

  const TRect& operator = (const RECT &Rect);

  //! @name Méthodes
  //@{

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Left
	//!
	//! Cette propriété détermine le côté gauche du rectangle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRect, int, Left);


  //---------------------------------------------------------------------------
  //! @brief Propriété Top
	//!
	//! Cette propriété détermine le côté supérieur du rectangle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRect, int, Top);


  //---------------------------------------------------------------------------
  //! @brief Propriété Right
	//!
	//! Cette propriété détermine le côté droit du rectangle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRect, int, Right);


  //---------------------------------------------------------------------------
  //! @brief Propriété Bottom
	//!
	//! Cette propriété détermine le côté inférieur du rectangle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRect, int, Bottom);


  //---------------------------------------------------------------------------
  //! @brief Propriété Width
	//!
	//! Cette propriété calcule la largeur du rectangle. Le fait de modifier
	//! cette propriété modifie la propriété @b Right.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRect, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété calcule la hauteur du rectangle. Le fait de modifier
	//! cette propriété modifie la propriété @b Bottom.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRect, int, Height);

  //@}

};


#else  // TRectH

class TRect;

#endif  // TRectH


#undef In_TRectH
