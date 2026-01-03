//-----------------------------------------------------------------------------
//! @file TStringList.h
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

#ifdef In_TStringListH
#error "Inclusion circulaire de TStringList"
#endif // In_TStringListH

#define In_TStringListH

#ifndef TStringListH
#define TStringListH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TStringsH
#include "TStrings.h"
#endif // TStringsH


//---------------------------------------------------------------------------
// Enum
//---------------------------------------------------------------------------

enum TDuplicates {
	dupIgnore,                    //!< Ignore les doublons
	dupAccept,                    //!< Accepte les doublons
	dupError                      //!< Déclenche une exception en cas de doublons
};


//-----------------------------------------------------------------------------
//! @class TStringList
//!
//! @brief Définition de la classe TStringList
//!
//! Cette classe représente une liste de textes. Elle peut gérer le tri, les
//! doublons, la recherche. Elle hérite de la classe TStrings et donc contient
//! également toutes les propriétés et méthodes de cette dernière.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TStringList: public TStrings {
protected:
  TDuplicates FDuplicates;
  bool FSorted;
  std::vector<AnsiString> FItems;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TStringList(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TStringList(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture du nombre de chaînes
  //!
  //! Cette méthode permet de lire le nombre d'éléments chaîne dans la liste.
  //!
  //! @return @c int Nombre de chaînes
  //---------------------------------------------------------------------------

  virtual int FASTCALL GetCount(void) const;

	//---------------------------------------------------------------------------
  //! @brief Définition du flag de mise à jour
  //!
  //! Cette fonction est une surcharge de la méthode définie dans @b TStrings.
	//! Voir cette méthode pour plus de détails.
  //!
  //! @param[in]        Updating @b true lorsqu'on passe à l'état "mise à jour". @b false sinon.
  //!
	//! @sa TStrings::SetUpdateState
  //---------------------------------------------------------------------------
  virtual void FASTCALL SetUpdateState(bool Updating);

	//---------------------------------------------------------------------------
  //! @brief Effacement complet de la liste
  //!
  //! Cette méthode met à zéro la liste.
  //---------------------------------------------------------------------------
  virtual void FASTCALL Clear(void);

	//---------------------------------------------------------------------------
  //! @brief Ajout d'une chaîne à la fin 
  //!
  //! Cette méthode ajoute une chaîne de caractères en fin de liste.
  //!
  //! @param[in]        S Chaîne de caractères à ajouter
  //!
  //! @return @c int Position de la nouvelle chaîne (premier = 0)
  //---------------------------------------------------------------------------
	virtual int FASTCALL Add(const AnsiString S);

	//---------------------------------------------------------------------------
  //! @brief Insertion d'une chaîne dans la liste
  //!
  //! Cette méthode insère une chaîne dans la liste de chaînes.
  //!
  //! @param[in]        Index Position d'insertion
  //! @param[in]        S Chaîne de caractères à insérer
  //---------------------------------------------------------------------------
	virtual void FASTCALL Insert(int Index, const AnsiString S);

	//---------------------------------------------------------------------------
  //! @brief Suppression d'une chaîne de la liste
  //!
  //! Cette méthode détruit une chaîne de la liste
  //!
  //! @param[in]        Index Position de la chaîne à détruire
  //---------------------------------------------------------------------------
	virtual void FASTCALL Delete(int Index);

	//---------------------------------------------------------------------------
  //! @brief Recherche d'une chaîne dans la liste
  //!
  //! Cette méthode recherche une chaîne dans la liste. Lorsque la liste est
	//! triée, utilisez de préférence cette méthode qui est plus rapide que la
	//! méthode @b IndexOf. Sinon, les deux méthodes sont strictement
	//! équivalentes.
  //!
  //! @param[in]        S Chaîne à rechercher
  //! @param[out]       Index Position de la chaîne trouvée (ou -1, ou chaîne
	//!                   supérieure si liste triée).
  //!
	//! @return @c bool true si chaîne trouvée.
  //---------------------------------------------------------------------------
	virtual bool FASTCALL Find(const AnsiString S, int &Index);

	//@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Strings
	//!
	//! Cette propriété permet de lire la chaîne d'un index donné.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TStringList, AnsiString, Strings);

  //---------------------------------------------------------------------------
  //! @brief Propriété Capacity
	//!
	//! Cette propriété détermine le nombre de chaînes alloués en mémoire. Ce
	//! nombre est toujours supérieur ou égal au nombre réel de chaînes présentes
	//! dans la liste car, lorsque c'est nécessaire, l'objet @b TStringList
	//! alloue un certain nombre d'éléments en une seule fois.
	//!
	//! @sa TStrings::Count
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStringList, int, Capacity);


  //---------------------------------------------------------------------------
  //! @brief Propriété Duplicates
	//!
	//! Cette propriété détermine si les chaînes identiques sont autorisées. La
	//! valeur par défaut est @b dupAccept (c'est à dire que les doublons sont
	//! autorisés).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStringList, TDuplicates, Duplicates);


  //---------------------------------------------------------------------------
  //! @brief Propriété Sorted: liste triée
	//!
	//! Si cette propriété est à @b true, la liste est automatiquement triée.
	//! Dans ce cas, les méthodes Add et Insert ont le même comportement, qui est
	//! d'insérer la nouvelle chaîne au bon endroit pour conserver l'ordre.@n
	//! La sensibilité à la casse (majuscules et minuscules) pour le tri dépend
	//! de la propriété @b CaseSensitive.
	//!
	//! @sa CaseSensitive
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TStringList, bool, Sorted);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque l'objet est modifié, soit par une
	//! action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);


  //@}


};


#else  // TStringListH

class TStringList;

#endif  // TStringListH


#undef In_TStringListH
