//-----------------------------------------------------------------------------
//! @file TFactory.cpp
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

#include <FreeVcl.h>

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

TFactory *TFactory::Premier;


//---------------------------------------------------------------------------
// TFactory
//---------------------------------------------------------------------------

TFactory::TFactory(void) {

  if (Premier == NULL) {
    // Premier créé => initialisation de "Premier"
    Premier = this;
    Precedent = this;
    Suivant = this;
  }
  else {
    // Insertion dans la chaîne
    Precedent = Premier->Precedent;
    Suivant = Premier;
    Precedent->Suivant = this;
    Suivant->Precedent = this;
  }
}

TFactory::~TFactory(void) {

  if (Precedent == this && Suivant == this) {

    // C'est le dernier détruit => on remet tout à zéro
    Premier = NULL;

  }
  else {

    // Premier créé, mais pas le dernier détruit => on passe le témoin au suivant
    if (Premier == this) Premier = Suivant;

    // On referme la chaîne avant de partir sur la pointe des pieds
    Precedent->Suivant = Suivant;
    Suivant->Precedent = Precedent;
  }

}

//---------------------------------------------------------------------------
void TFactory::Init(void) {
}

//---------------------------------------------------------------------------
bool TFactory::GetListObjects(TStrings *ListObjects, TStrings *ListTabs) {

  ListObjects->Clear();
  ListTabs->Clear();
  TFactory * Current = Premier;
  do {
    Current->GetListObjectsVirt(ListObjects, ListTabs);

    Current = Current->Suivant;
  } while (Current != Premier);

  return true;
}

//---------------------------------------------------------------------------
TComponent *TFactory::CreateObject(TComponent* AOwner, AnsiString asNom) {
  TComponent *Retour = NULL;


  TFactory * Current = Premier;
  do {
    Retour = Current->CreateObjectVirt(AOwner, asNom);
    if (Retour) break;

    Current = Current->Suivant;
  } while (Current != Premier);

  return Retour;
}

//---------------------------------------------------------------------------

