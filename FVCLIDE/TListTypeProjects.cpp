//-----------------------------------------------------------------------------
//! @file TListTypeProjects.cpp
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


//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------
#include <FreeVcl.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------
#include "TListTypeProjects.h"
#include "TTypeProject.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
TListTypeProjects::TListTypeProjects(void) {
}

//---------------------------------------------------------------------------
TListTypeProjects::~TListTypeProjects(void) {
	int i;


	for (i = 0; i < (int) Items.size(); i++) {
		delete Items[i];
	}
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Count
//---------------------------------------------------------------------------

int TListTypeProjects::Get_Count(void) {
	return (int) Items.size();
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Associate
//---------------------------------------------------------------------------

TTypeProject *TListTypeProjects::Get_TypeProject(int Index) {
	return Items[Index];
}

//---------------------------------------------------------------------------
int TListTypeProjects::LoadTypeProject(AnsiString asProjectType) {
	int i;


	for (i = 0; i < (int) Items.size(); i++) {
		if (Items[i]->Name == asProjectType) return i;
	}

	TTypeProject *TypeProject = new TTypeProject(asProjectType);
	Items.push_back(TypeProject);
	TypeProject->Install();

	return i;
}

//---------------------------------------------------------------------------
