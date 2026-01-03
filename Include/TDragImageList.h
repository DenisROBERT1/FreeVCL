//-----------------------------------------------------------------------------
//! @file TDragImageList.h
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

#ifdef In_TDragImageListH
#error "Inclusion circulaire de TDragImageList"
#endif // In_TDragImageListH

#define In_TDragImageListH

#ifndef TDragImageListH
#define TDragImageListH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TCustomImageList.h"


//-----------------------------------------------------------------------------
//! @class TDragImageList
//!
//! @brief Définition de la classe TDragImageList
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDragImageList: public TCustomImageList {
protected:
  TCursor FDragCursor;
  bool FDragging;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TDragImageList(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDragImageList(void);

  //@}


  //! @name Méthodes
  //@{

  bool FASTCALL BeginDrag(HWND Window, int X, int Y);
  bool FASTCALL DragLock(HWND Window, int XPos, int YPos);
  bool FASTCALL DragMove(int X, int Y);
  void FASTCALL DragUnlock(void);
  bool FASTCALL EndDrag(void);
  virtual TPoint FASTCALL GetHotSpot();
  void FASTCALL HideDragImage(void);
  virtual void FASTCALL Initialize(void);
  bool FASTCALL SetDragImage(int Index, int HotSpotX, int HotSpotY);
  void FASTCALL ShowDragImage(void);

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

protected:


public:
  //---------------------------------------------------------------------------
  //! @brief Propriété DragCursor
	//!
	//! Cette propriété définit le curseur qui s'affiche lorsqu'on fait un drag
	//! and drop.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDragImageList, TCursor, DragCursor);


  //---------------------------------------------------------------------------
  //! @brief Propriété Dragging
	//!
	//! Cette propriété est à @b true pendant le drag and drop.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDragImageList, bool, Dragging);


  //@}

};


#else  // TDragImageListH

class TDragImageList;

#endif  // TDragImageListH


#undef In_TDragImageListH
