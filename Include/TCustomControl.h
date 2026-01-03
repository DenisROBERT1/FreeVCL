//-----------------------------------------------------------------------------
//! @file TCustomControl.h
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

#ifdef In_TCustomControlH
#error "Inclusion circulaire de TCustomControl"
#endif // In_TCustomControlH

#define In_TCustomControlH

#ifndef TCustomControlH
#define TCustomControlH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TCustomControl
//!
//! @brief Définition de la classe TCustomControl
//!
//! Cette classe est la classe de base des objets gérant eux-même leur zone
//! de dessin et pouvant recevoir le focus.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomControl: public TWinControl {
protected:
  TControlScrollBar *FHorzScrollBar;
  TControlScrollBar *FVertScrollBar;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessPaint(HDC hdc, RECT rcPaint);
  virtual bool ProcessHScroll(int Command, int Info);
  virtual bool ProcessVScroll(int Command, int Info);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TCustomControl(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomControl(void);

  //@}


  //! @name Méthodes
  //@{

  virtual void FASTCALL Paint(void) {}

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

  bool SetProperty(AnsiString asProperty, AnsiString asValue,
                                TPersistent *Sender);
  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété HorzScrollBar
	//!
	//! Cette propriété définit les paramètres de la barre de défilement
	//! horizontale.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCustomControl, TControlScrollBar *, HorzScrollBar);


  //---------------------------------------------------------------------------
  //! @brief Propriété VertScrollBar
	//!
	//! Cette propriété définit les paramètres de la barre de défilement
	//! verticale.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCustomControl, TControlScrollBar *, VertScrollBar);


  //@}

};


#else  // TCustomControlH

class TCustomControl;

#endif  // TCustomControlH


#undef In_TCustomControlH
