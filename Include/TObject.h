//-----------------------------------------------------------------------------
//! @file TObject.h
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

#ifdef In_TObjectH
#error "Inclusion circulaire de TObject"
#endif // In_TObjectH

#define In_TObjectH

#ifndef TObjectH
#define TObjectH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>

#ifndef FVCLTypesH
#include "FVCLTypes.h"
#endif  // FVCLTypesH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TEvent;


//-----------------------------------------------------------------------------
//! @class TObject
//!
//! @brief Définition de la classe TObject
//!
//! Cette classe est la classe de base de tous les objets VCL, composants,
//! contrôles, boîtes de dialogues. Cette classe contient la définition du nom
//! de la classe. Ne pas implémenter cette classe, mais uniquement ses
//! descendants.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TObject {
private:
protected:
  AnsiString FClassName;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TObject(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TObject(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Test de l'égalité de ClassName
  //!
  //! Cette méthode permet de tester la propriété ClassName
  //!
  //! @return @c bool true si égale
  //---------------------------------------------------------------------------

  virtual bool ClassNameIs(AnsiString ClassName);

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction de l'objet
  //!
  //! @warning Cette méthode peut être appelée depuis le constructeur (c'est
  //! à dire que les fonctions virtuelles, donc les évènements, ne marchent
  //! pas)
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void) {}

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un évènement
  //!
  //! Cette méthode permet de lire le nom d'une méthode d'évènement de type
  //! chaîne. Elle doit être redéfinie dans tous les objets possédant des
	//! méthodes de traitement des évènements
  //!
  //! @param[in]        Event Evènement à lire
  //!
  //! @return @c const wchar_t * Nom de la méthode.
  //---------------------------------------------------------------------------

  virtual const TCHAR *GetEvent(TEvent *Event) {return _T("");}

  //---------------------------------------------------------------------------
  //! @brief Affectation d'un évènement
  //!
  //! Cette méthode permet d'affecter une méthode d'évènement de type chaîne.
  //! Elle doit être redéfinie dans tous les objets possédant des méthodes de
	//! traitement des évènements
  //!
  //! @param[in]        Event Evènement à enregistrer
  //! @param[in]        EventCall Méthode de traitement de l'évènement
  //---------------------------------------------------------------------------

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {}

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ClassName
	//!
	//! Cette propriété permet de lire le nom de la classe d'un objet. Soyez
	//! prudents dans l'utilisation de cette propriété: gardez à l'esprit qu'une
	//! classe peut être dérivée, et donc que le nom renvoyé par cette propriété
	//! peut être trompeur. Si vous souhaitez savoir avec précision si un objet
	//! est une implémentation d'une classe donnée, utilisez plutôt la commande
	//! du c++ @b dynamic_cast.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TObject, AnsiString, ClassName);

  //@}

};


#else  // TObjectH

class TObject;

#endif  // TObjectH


#undef In_TObjectH
