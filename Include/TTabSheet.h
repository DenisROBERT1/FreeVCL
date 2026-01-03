//-----------------------------------------------------------------------------
//! @file TTabSheet.h
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

#ifdef In_TTabSheetH
#error "Inclusion circulaire de TTabSheet"
#endif // In_TTabSheetH

#define In_TTabSheetH

#ifndef TTabSheetH
#define TTabSheetH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TPageControl;


//-----------------------------------------------------------------------------
//! @class TTabSheet
//!
//! @brief Définition de la classe TTabSheet
//!
//! Cette classe représente une page (un onglet) d'un contrôle @b TPageControl.
//! En général vous n'avez pas à créer d'instances de cet objet : elles sont
//! créées automatiquement lorsque vous ajoutez un onglet à un @b TPageControl.
//! Utilisez la propriété @b Page de celui-ci pour accéder aux différentes
//! pages.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTabSheet: public TWinControl {
private:
  int FPageIndex;
  TPageControl *FPageControl;
protected:
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTabSheet(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTabSheet(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Left
  //!
  //! Cette méthode permet d'écrire la propriété Left
  //!
  //! @param[in]        NewLeft Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Left(int NewLeft);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Top
  //!
  //! Cette méthode permet d'écrire la propriété Top
  //!
  //! @param[in]        NewTop Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Top(int NewTop);

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
  //! @brief Setter de la propriété Height
  //!
  //! Cette méthode permet d'écrire la propriété Height
  //!
  //! @param[in]        NewHeight Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Height(int NewHeight);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Caption
  //!
  //! Cette méthode permet d'écrire la propriété Caption
  //!
  //! @param[in]        NewCaption Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Caption(AnsiString NewCaption);

	//---------------------------------------------------------------------------
  //! @brief Rend visible la fenêtre.
  //!
  //! Cette méthode permet de rendre visible la fenêtre, et de l'amener au
	//! premier plan (au dessus des autres).
  //---------------------------------------------------------------------------

	virtual void FASTCALL Show(void);

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété PageIndex
	//!
	//! Cette propriété permet de lire l'index de la page dans le @b PageControl.
	//! Ne modifiez pas cette propriété, seul le système étant autorisé à le
	//! faire lors de la création d'un objet @b TPageControl et de ses objets
	//! @b TTabSheet associés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTabSheet, int, PageIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété PageControl
	//!
	//! Cette propriété permet de retrouver l'objet @b TPageControl auquel
	//! appartient cet objet. Le fait d'affecter une valeur à cette propriété
	//! supprime la page de l'ancien @b TPageControl (s'il y en avait un), et
	//! l'insère automatiquement dans le nouvel objet @b TPageControl.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTabSheet, TPageControl*, PageControl);

  //@}

};


#else  // TTabSheetH

class TTabSheet;

#endif  // TTabSheetH


#undef In_TTabSheetH
