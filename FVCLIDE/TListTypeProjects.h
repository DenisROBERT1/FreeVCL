//-----------------------------------------------------------------------------
//! @file TListTypeProjects.h
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


#ifndef TListTypeProjectsH
#define TListTypeProjectsH


//---------------------------------------------------------------------------
// Pré-déclarations
//---------------------------------------------------------------------------

class TTypeProject;

//---------------------------------------------------------------------------
// Macros, enums
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @class TListTypeProjects
//!
//! @brief Liste des projets chargés en mémoire
//!
//! Cette classe (@c TListTypeProjects) permet de mémoriser les projets en
//! mémoire.
//---------------------------------------------------------------------------

class TListTypeProjects: public TObject {
private:
protected:
	std::vector<TTypeProject *> Items;

public:

  //! @name Constructeur / destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
  TListTypeProjects(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
  ~TListTypeProjects(void);

  //@}


  //! @name Méthodes
  //@{

  //@}

	int LoadTypeProject(AnsiString asProjectType);

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Count
 	//!
	//! Cette propriété permet de lire le nombre de projets chargés
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TListTypeProjects, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété TypeProject
	//!
	//! Cette propriété permet de lire un des types de projets de la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TListTypeProjects, TTypeProject *, TypeProject);

  //@}


  //! @name Evènements
  //@{


  //@}

};

#endif
