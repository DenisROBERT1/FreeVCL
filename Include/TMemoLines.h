//-----------------------------------------------------------------------------
//! @file TMemoLines.h
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

#ifdef In_TMemoLinesH
#error "Inclusion circulaire de TMemoLines"
#endif // In_TMemoLinesH

#define In_TMemoLinesH

#ifndef TMemoLinesH
#define TMemoLinesH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TStringsH
#include "TStrings.h"
#endif // TStringsH


//-----------------------------------------------------------------------------
//! @class TMemoLines
//!
//! @brief Définition de la classe TMemoLines
//!
//! Cette classe représente une ligne d'un objet @b TMemo. Ne créez pas
//! d'instances de cette classe, utilisez celles automatiquement créées lorsque
//! vous ajoutez des lignes dans un contrôle @b TMemo.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMemoLines: public TStrings {
protected:
	HWND FHandle;
public:

	//! @name Constructeurs et destructeur
  //@{

	//---------------------------------------------------------------------------
	//! @brief Constructeurs
	//---------------------------------------------------------------------------

	TMemoLines(void);

	//---------------------------------------------------------------------------
	//! @brief Destructeur
	//---------------------------------------------------------------------------

	virtual ~TMemoLines(void);

  //@}


	//! @name Méthodes
	//@{

	//---------------------------------------------------------------------------
	//! @brief Getter de la propriété Strings
	//!
	//! Cette méthode permet de lire la propriété Strings
	//!
	//! @param[in]				i Index de la propriété à lire
	//!
	//! @return @c AnsiString Propriété Strings
	//---------------------------------------------------------------------------

	virtual AnsiString Get_Strings(int i);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété Strings
	//!
	//! Cette méthode permet d'écrire la propriété Strings
	//!
	//! @param[in]				i Index de la propriété à enregistrer
	//! @param[in]				NewStrings Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_Strings(int i, AnsiString NewStrings);

	//---------------------------------------------------------------------------
	//! @brief Lecture du nombre de chaînes
	//!
	//! Cette méthode permet de lire le nombre d'éléments chaîne dans la liste.
	//!
	//! @return @c int Nombre de chaînes
	//---------------------------------------------------------------------------

	virtual int FASTCALL GetCount(void) const;


	//---------------------------------------------------------------------------
	virtual void FASTCALL Clear(void);
	virtual int FASTCALL Add(const AnsiString S);
	virtual void FASTCALL Insert(int Index, const AnsiString S);
	virtual void FASTCALL Delete(int Index);

	//@}

	//! @name Propriétés
	//@{

	//---------------------------------------------------------------------------
	//! @brief Propriété Handle: Identifiant système du composant
	//!
	//! Cette propriété permet de lire le Handle du TMemo auquel appartient
	//! l'objet TMemoLines.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemoLines, HWND, Handle);

	//@}

};


#else  // TMemoLinesH

class TMemoLines;

#endif	// TMemoLinesH


#undef In_TMemoLinesH
