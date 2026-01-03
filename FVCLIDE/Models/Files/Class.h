//-----------------------------------------------------------------------------
//! @file Class.h
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

#ifdef In_{{{FILE_NAME}}}H
#error "Inclusion circulaire de {{{FILE_NAME}}}"
#endif // In_{{{FILE_NAME}}}H

#define In_{{{FILE_NAME}}}H

#ifndef {{{FILE_NAME}}}H
#define {{{FILE_NAME}}}H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <FreeVcl.h>

//-----------------------------------------------------------------------------
//! @class {{{FILE_NAME}}}
//!
//! @brief Définition de la classe {{{FILE_NAME}}}
//!
//! Cette classe permet de ...
//-----------------------------------------------------------------------------

class {{{FILE_NAME}}} {
private:
protected:
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  {{{FILE_NAME}}}(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~{{{FILE_NAME}}}(void);

  //@}


  //! @name Méthodes
  //@{

	// Ajouter ici vos méthodes publiques
	// ...
	  
  //@}

  //! @name Propriétés
  //@{

	// Exemple :
  // DECLARE_PROPERTY({{{FILE_NAME}}}, bool, Visible);

	//@}

  //! @name Evènements
  //@{

	// Exemple :
  // DECLARE_EVENT_1(OnExecute, TObject *);

  //@}

};

//---------------------------------------------------------------------------

#else  // {{{FILE_NAME}}}H

class {{{FILE_NAME}}};

#endif  // {{{FILE_NAME}}}H

#undef In_{{{FILE_NAME}}}H
