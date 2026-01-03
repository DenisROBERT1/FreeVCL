//-----------------------------------------------------------------------------
//! @file TLabel.h
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

#ifdef In_TLabelH
#error "Inclusion circulaire de TLabel"
#endif // In_TLabelH

#define In_TLabelH

#ifndef TLabelH
#define TLabelH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TLabel
//!
//! @brief Définition de la classe TLabel
//! @image html TLabel.jpg "TLabel"
//!
//! Cette classe définit le contrôle permettant d'afficher du texte statique
//! dans une boîte de dialogue.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TLabel: public TWinControl {
private:
protected:
  bool FWordWrap;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TLabel(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TLabel(void);

  //@}


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
  //! @brief Setter de la propriété Font
  //!
  //! Cette méthode permet d'écrire la propriété Font
  //!
  //! @param[in]        NewFont Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Font(TFont *NewFont);


  //---------------------------------------------------------------------------
  //! @brief Indique si le contrôle peut se redimensionner automatiquement.
  //---------------------------------------------------------------------------
	virtual bool FASTCALL CanAutoSize(int &NewWidth, int &NewHeight);

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
  //! @brief Propriété Alignment
	//!
	//! Cette propriété détermine l'alignement du texte dans le rectangle client.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLabel, TAlignment, Alignment);


  //---------------------------------------------------------------------------
  //! @brief Propriété WordWrap
	//!
	//! Cette propriété permet un retour à la ligne si le texte est plus grand
	//! que la taille du contrôle. Dans le cas contraire, le libellé est tronqué.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TLabel, bool, WordWrap);

  //@}


};


#else  // TLabelH

class TLabel;

#endif  // TLabelH


#undef In_TLabelH
