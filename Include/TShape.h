//-----------------------------------------------------------------------------
//! @file TShape.h
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

#ifdef In_TShapeH
#error "Inclusion circulaire de TShape"
#endif // In_TShapeH

#define In_TShapeH

#ifndef TShapeH
#define TShapeH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TShapeType {
	stRectangle,							 //!< Rectangle
	stSquare,									 //!< Carré centré sur le rectangle client
	stRoundRect,							 //!< Rectangle avec coins arrondis
	stRoundSquare,						 //!< Carré centré avec coins arrondis
	stEllipse,								 //!< Ellipse circonscrite sur le rectangle client
	stCircle									 //!< Cercle centré sur le rectangle client
};


//-----------------------------------------------------------------------------
//! @class TShape
//!
//! @brief Zone colorée
//! @image html TShape.jpg "TShape"
//!
//! Cette classe permet de faire une simple zone colorée (rectangle, carré,
//! ellipse ou cercle). Vous pouvez paramétrer la forme, la couleur du contour
//! et du remplissage, et la dimension des arrondis (dans le cas d'un carré ou
//! d'un rectangle avec coins arrondis).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TShape: public TWinControl {
private:

protected:

  TPen *FPen;
  TBrush *FBrush;
  int FRoundWidth;
  int FRoundHeight;
  TShapeType FShape;

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

  TShape(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TShape(void);

  //@}


  //! @name Méthodes
  //@{

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
  //! @brief Propriété Pen
	//!
	//! Définition du contour de la zone.
	//!
	//! @note Le stylo est affecté par copie, ce qui signifie que la classe ne
	//! devient pas propriétaire du stylo affecté par cette propriété.
	//! Donc l'utilisateur peut (et doit s'il en est propriétaire) le détruire
	//! juste après affectation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TShape, TPen *, Pen);


  //---------------------------------------------------------------------------
  //! @brief Propriété Brush
	//!
	//! Définition du remplissage de la zone.
	//!
	//! @note Le pinceau est affecté par copie, ce qui signifie que la classe ne
	//! devient pas propriétaire du pinceau affecté par cette propriété.
	//! Donc l'utilisateur peut (et doit s'il en est propriétaire) le détruire
	//! juste après affectation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TShape, TBrush *, Brush);


  //---------------------------------------------------------------------------
  //! @brief Propriété RoundWidth: dimension horizontale de l'arrondi
	//!
	//! Lorsque la propriété @b Shape est égale à @b stRoundRect ou bien
	//! @b stRoundSquare, affectez cette propriété pour modifier l'aspect de
	//! l'arrondi des coins.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TShape, int, RoundWidth);


  //---------------------------------------------------------------------------
  //! @brief Propriété RoundHeight: dimension verticale de l'arrondi
	//!
	//! Lorsque la propriété @b Shape est égale à @b stRoundRect ou bien
	//! @b stRoundSquare, affectez cette propriété pour modifier l'aspect de
	//! l'arrondi des coins.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TShape, int, RoundHeight);


  //---------------------------------------------------------------------------
  //! @brief Propriété Shape: type de forme
	//!
	//! Cette propriété définit le type de forme à dessiner (carré, rectangle ou
	//! Ellipse).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TShape, TShapeType, Shape);


  //@}


  //! @name Evènements
  //@{

  //@}


};


#else  // TShapeH

class TShape;

#endif  // TShapeH


#undef In_TShapeH

