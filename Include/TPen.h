//-----------------------------------------------------------------------------
//! @file TPen.h
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

#ifdef In_TPenH
#error "Inclusion circulaire de TPen"
#endif // In_TPenH

#define In_TPenH

#ifndef TPenH
#define TPenH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TCustomPen.h"
#include "TColor.h"


//-----------------------------------------------------------------------------
//! @class TPen
//!
//! @brief Définition de la classe TPen
//!
//! Cette classe représente un stylo système dans lequel vous pouvez changer
//! les paramètres des traits dessinés. En général, le programmeur n'a pas à
//! créer d'instances de cette classe. Il utilise les objets déjà présents
//! dans d'autres objets graphiques (@b TCanvas le plus souvent).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPen: public TCustomPen {
protected:
  HPEN FHandle;
  LOGPEN LogPen;
  bool bChanged;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPen(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPen(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Style
  //!
  //! Cette méthode permet de lire la propriété Style
  //!
  //! @return @c TPenStyle Propriété Style
  //---------------------------------------------------------------------------

  virtual TPenStyle Get_Style(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Style
  //!
  //! Cette méthode permet d'écrire la propriété Style
  //!
  //! @param[in]        NewStyle Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Style(TPenStyle NewStyle);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Width
  //!
  //! Cette méthode permet de lire la propriété Width
  //!
  //! @return @c int Propriété Width
  //---------------------------------------------------------------------------

  virtual int Get_Width(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Width
  //!
  //! Cette méthode permet d'écrire la propriété Width
  //!
  //! @param[in]        NewWidth Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Width(int NewWidth);


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
	//! Cette propriété détermine le handle système du stylo encapsulé dans
	//! l'objet. Si vous affectez un nouveau handle, l'objet @b TPen devient
	//! propriétaire de ce handle. Ne le détruisez donc pas, il sera
	//! automatiquement détruit avec l'objet ou lors du prochain changement de
	//! ses caractéristiques.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPen, HPEN, Handle);

  //@}

};


#else  // TPenH

class TPen;

#endif  // TPenH


#undef In_TPenH
