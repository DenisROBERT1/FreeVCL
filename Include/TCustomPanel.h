//-----------------------------------------------------------------------------
//! @file TCustomPanel.h
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

#ifdef In_TCustomPanelH
#error "Inclusion circulaire de TCustomPanel"
#endif // In_TCustomPanelH

#define In_TCustomPanelH

#ifndef TCustomPanelH
#define TCustomPanelH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TCustomControl.h"


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TBevelCut {
	bvNone,												//!< Aucune biseau
	bvLowered,										//!< Biseau en retrait
	bvRaised,											//!< Biseau en relief
	bvSpace												//!< Biseau en forme d'espace
};

typedef int TBevelWidth;        //!< typedef totalement inutile, utilisez un int


//-----------------------------------------------------------------------------
//! @class TCustomPanel
//!
//! @brief Définition de la classe TCustomPanel
//!
//! Cette classe est la classe de base des objets de type Panel, c'est à dire
//! des cadres permettant de contenir d'autres composants. Vous pouvez définir
//! différents rebords.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomPanel: public TCustomControl {
private:

protected:

	TAlignment FAlignment;
	TAlignmentV FAlignmentV;
  TBorderStyle FBorderStyle;
  TBevelCut FBevelInner;
  TBevelCut FBevelOuter;
  int FBorderWidth;
  int FBevelWidth;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessPaint(HDC hdc, RECT rcPaint);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomPanel(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomPanel(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ajustement du rectangle client
  //!
  //! @param[in, out]   Rect Rectangle de la zone client
  //---------------------------------------------------------------------------
  virtual void FASTCALL AdjustClientRect(TRect &Rect);

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

  DECLARE_PROPERTY(TCustomPanel, TAlignment, Alignment);


  //---------------------------------------------------------------------------
  //! @brief Propriété AlignmentV
	//!
	//! Cette propriété détermine l'alignement vertical du texte dans le
	//! rectangle client.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPanel, TAlignmentV, AlignmentV);


  //---------------------------------------------------------------------------
  //! @brief Propriété BorderStyle
	//!
	//! Cette propriété définit le style de bordure du contrôle.
	//!
	//! @sa BevelInner
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPanel, TBorderStyle, BorderStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété BevelInner
	//!
	//! Cette propriété détermine le biseau intérieur du panel.
	//! @image html TPanel_Bevel.jpg "Bordures et biseaux"
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPanel, TBevelCut, BevelInner);


  //---------------------------------------------------------------------------
  //! @brief Propriété BevelOuter
	//!
	//! Cette propriété détermine le biseau extérieur du panel.
	//!
	//! @sa BevelInner
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPanel, TBevelCut, BevelOuter);


  //---------------------------------------------------------------------------
  //! @brief Propriété BorderWidth
	//!
	//! Cette propriété détermine la largeur du bord, c'est à dire entre les
	//! biseaux intérieur et extérieur du panel.
	//!
	//! @sa BevelInner
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPanel, int, BorderWidth);


  //---------------------------------------------------------------------------
  //! @brief Propriété BevelWidth
	//!
	//! Cette propriété détermine la largeur des biseaux.
	//!
	//! @sa BevelInner
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomPanel, int, BevelWidth);


  //@}


};


#else  // TCustomPanelH

class TCustomPanel;

#endif  // TCustomPanelH


#undef In_TCustomPanelH
