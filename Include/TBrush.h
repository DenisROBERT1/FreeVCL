//-----------------------------------------------------------------------------
//! @file TBrush.h
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

#ifdef In_TBrushH
#error "Inclusion circulaire de TBrush"
#endif // In_TBrushH

#define In_TBrushH

#ifndef TBrushH
#define TBrushH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TCustomBrush.h"
#include "TColor.h"


//-----------------------------------------------------------------------------
//! @class TBrush
//!
//! @brief Définition de la classe TBrush
//!
//! Cette classe représente un pinceau système. Elle contient les attributs
//! de couleur et de style permettant de remplir une zone de dessin.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TBrush: public TCustomBrush {
protected:
  HBRUSH FHandle;
  LOGBRUSH LogBrush;
  bool bChanged;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TBrush(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TBrush(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Style
  //!
  //! Cette méthode permet de lire la propriété Style
  //!
  //! @return @c TBrushStyle Propriété Style
  //---------------------------------------------------------------------------

  virtual TBrushStyle Get_Style(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Style
  //!
  //! Cette méthode permet d'écrire la propriété Style
  //!
  //! @param[in]        NewStyle Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Style(TBrushStyle NewStyle);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Color
  //!
  //! Cette méthode permet de lire la propriété Color
  //!
  //! @return @c TColor Propriété Color
  //---------------------------------------------------------------------------

  virtual TColor Get_Color(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Color
  //!
  //! Cette méthode permet d'écrire la propriété Color
  //!
  //! @param[in]        NewColor Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Color(TColor NewColor);


  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système
	//!
	//! Cette propriété permet de lire et de définir le handle système du
	//! pinceau. Si vous affectez cette propriété, l'objet @b TBrush devient
	//! propriétaire de l'objet défini par ce handle, il sera détruit en même
	//! temps que l'objet @b TBrush.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBrush, HBRUSH, Handle);

  //@}

};


#else  // TBrushH

class TBrush;

#endif  // TBrushH


#undef In_TBrushH
