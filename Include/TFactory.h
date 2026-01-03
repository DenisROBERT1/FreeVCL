//-----------------------------------------------------------------------------
//! @file TFactory.h
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

#ifdef In_TFactoryH
#error "Inclusion circulaire de TFactory"
#endif // In_TFactoryH

#define In_TFactoryH

#ifndef TFactoryH
#define TFactoryH


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define FVCL_RegisterComponent(TClass) \
class TFactory_##TClass: public TFactory {\
protected:\
  virtual bool GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs) {\
    ListObjects->Add(_T(#TClass));ListTabs->Add(_T("Frames"));\
    return true;\
  }\
  virtual TComponent *CreateObjectVirt(TComponent* AOwner, AnsiString asNom) {\
    if (asNom == _T(#TClass)) return new TClass((TWinControl *) AOwner);\
    return NULL;\
  }\
} Factory_##TClass\


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TComponent;
class TStrings;


//-----------------------------------------------------------------------------
//! @class TFactory
//!
//! @brief Définition de la classe TFactory
//!
//! Cette classe est la classe de base permettant de créer des composants à
//! partir de leur nom (nom lu dans le DFM, par exemple). C'est une classe
//! vituelle. Créez des descendants de cette classe pour permettre à l'EDI
//! de créer vos composants. Il est pour convention d'appeler Les descendants
//! "TFactory_" + Nom du conteneur (par exemple TFactory_FVCL).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFactory {
private:
  TFactory *Precedent;
  TFactory *Suivant;
  static TFactory *Premier;
protected:

  //! @name Méthodes protégées
  //@{

  //---------------------------------------------------------------------------
  //! @brief Liste des objets pouvant être créés (fonction virtuelle à
  //! surcharger par les descendants)
  //!
  //! @param[in, out]   ListObjects Liste des objets pouvant être créés
  //! @param[in, out]   ListTabs Onglet par défaut des objets (en création)
  //!
  //! @return @c bool true si Ok
  //!
  /*! @verbatim
      En général, cette méthode se contente d'ajouter les composants qu'elle
      peut créer aux deux listes, par exemple:

      ListObjects->Add(_T("TForm")); ListTabs->Add(_T("Contrôles courants"));
      ListObjects->Add(_T("TFrame")); ListTabs->Add(_T("Contrôles courants"));
      ListObjects->Add(_T("TLabel")); ListTabs->Add(_T("Contrôles courants"));
      return true; @endverbatim */
  //!
  //! @warning Ne pas mettre à zéro les listes ListObjects et ListOnglets avant
	//! de les compléter, plusieurs factories pouvant être chaînées à la suite.
  //! @warning Le nom du composant doit être unique. Si plusieurs composants de
  //! même nom peuvent être créés par plusieurs containers, les résultats
  //! seront imprévisibles.
  //---------------------------------------------------------------------------

  virtual bool GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs) = 0;

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //!
  //! Cette méthode virtuelle est à surcharger par les descendants. Elle permet
  //! de créer un composant à partir de son nom.
  //!
  //! @param[in]        AOwner Propriétaire du composant à créer
  //! @param[in]        asNom Nom du composant à créer
  //!
  //! @return @c TComponent * Composant créé, ou NULL si erreur
  //---------------------------------------------------------------------------

  virtual TComponent *CreateObjectVirt(TComponent* AOwner, AnsiString asNom) = 0;

  //@}

public:
 
	//! @name Constructeurs et destructeur
  //@{

 //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFactory(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFactory(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Fonction vide à appeler pour dire au link d'utiliser l'objet
  //!
  //! Cette méthode ne fait rien. Mais elle doit obligatoirement être appelée
  //! par le module utilisant le container pour indiquer au link d'intégrer la
  //! librairie.
  //---------------------------------------------------------------------------

  void Init(void);

  //---------------------------------------------------------------------------
  //! @brief Liste des objets pouvant être créés
  //!
  //! @param[in, out]   ListObjects Liste des objets pouvant être créés
  //! @param[in, out]   ListTabs Onglet par défaut des objets (en création)
  //!
  //! @return @c bool true si Ok
  //!
  /*! @verbatim
      Le créateur d'un EDI peut utiliser cette méthode pour connaître tous les
      objets que peut créer le système. Cette méthode met à zéro la liste, puis
      interroge tous les objets dérivés avec un système de chaînage, chaque
      objet dérivé complète la liste avec les objets qu'il sait créer. @endverbatim */
  //---------------------------------------------------------------------------

  bool GetListObjects(TStrings *ListObjects, TStrings *ListTabs);

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //!
  //! @param[in]        AOwner Propriétaire du composant à créer
  //! @param[in]        asNom Nom du composant à créer
  //!
  //! @return @c TComponent * Composant créé, ou NULL si erreur (objet inconnu,
  //! par exemple).
  //---------------------------------------------------------------------------

  TComponent *CreateObject(TComponent* AOwner, AnsiString asNom);

  //@}

};


#else  // TFactoryH

class TFactory;

#endif  // TFactoryH


#undef In_TFactoryH
