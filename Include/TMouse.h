//-----------------------------------------------------------------------------
//! @file TMouse.h
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

#ifdef In_TMouseH
#error "Inclusion circulaire de TMouse"
#endif // In_TMouseH

#define In_TMouseH

#ifndef TMouseH
#define TMouseH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH

#ifndef TPointH
#include "TPoint.h"
#endif  // TPointH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TMouseButton {
	mbLeft,												//!< Bouton gauche de la souris
	mbMiddle,											//!< Bouton central de la souris
	mbRight												//!< Bouton droit de la souris
};

enum TShiftState_1 {
	ssShift,											//!< Touche majuscules
	ssAlt,												//!< Touche ALT
	ssCtrl,												//!< Touche contrôle
	ssLeft,												//!< Bouton gauche de la souris
	ssMiddle,											//!< Bouton central de la souris
	ssRight,											//!< Bouton droit de la souris
	ssDouble											//!< Double-clic
};

typedef Set<TShiftState_1, ssShift, ssDouble> TShiftState; //!< Etat des touches et des boutons


//-----------------------------------------------------------------------------
//! @class TMouse
//!
//! @brief Définition de la classe TMouse
//!
//! Cette classe représente la souris. N'implémentez pas d'instances de cette
//! classe, utilisez l'instance globale définie automatiquement dans tout
//! programme VCL, le pointeur sur cette instance s'appelle @b Mouse. Vous
//! pouvez donc y accéder en tapant "Mouse->..." dans n'importe quel contexte.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMouse: public TObject {
protected:
  bool FDragImmediate;
  int FDragThreshold;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TMouse(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Capture
	//!
	//! Cette propriété permet de lire ou de définir la fenêtre qui possède la
	//! capture souris, c'est à dire qui reçoit tous les évènements souris
	//! (clics, déplacement, utilisation de la molette de la souris), y compris
	//! si le curseur est en dehors de cette fenêtre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMouse, HWND, Capture);

  //---------------------------------------------------------------------------
  //! @brief Propriété CursorPos
	//!
	//! Cette propriété permet de lire la position du curseur de la souris, ou
	//! bien de positionner le curseur à un endroit précis. Les coordonnées sont
	//! en pixels à partir du coin supérieur gauche de l'écran.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMouse, TPoint, CursorPos);

  //---------------------------------------------------------------------------
  //! @brief Propriété WheelScrollLines
	//!
	//! Cette propriété détermine le nombre de lignes qui défilent lorsqu'on
	//! tourne la molette de la souris d'un cran.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TMouse, int, WheelScrollLines);

  //---------------------------------------------------------------------------
  //! @brief Propriété DragImmediate
	//!
	//! Cette propriété détermine si l'opération de drag and drop démarre
	//! immédiatement après le clic souris (si @b DragImmediate = @b true), ou
	//! bien après un petit déplacement dépendant de @b DragThreshold.
	//!
	//! @sa BeginDrag, DragMode
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMouse, bool, DragImmediate);

  //---------------------------------------------------------------------------
  //! @brief Propriété DragThreshold
	//!
	//! Cette propriété détermine le déplacement minimum en pixels avant que le
	//! programme entame une opération de drag and drop. La valeur par défaut est
	//! de 5 pixels, ce qui permet de faire d'autres opérations qui ne nécessitent
	//! pas de déplacement (clic pour sélectionner un élément).
	//!
	//! @sa BeginDrag, DragMode
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMouse, int, DragThreshold);


  //@}

};


#else  // TMouseH

class TMouse;

#endif  // TMouseH

#undef In_TMouseH
