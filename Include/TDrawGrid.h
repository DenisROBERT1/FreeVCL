//-----------------------------------------------------------------------------
//! @file TDrawGrid.h
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

#ifdef In_TDrawGridH
#error "Inclusion circulaire de TDrawGrid"
#endif // In_TDrawGridH

#define In_TDrawGridH

#ifndef TDrawGridH
#define TDrawGridH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <vector>
#include "TCustomGrid.h"
#include "TStringTab.h"


//-----------------------------------------------------------------------------
//! @class TDrawGrid
//!
//! @brief Définition de la classe TDrawGrid : grille d'éléments texte.
//! @image html TDrawGrid.jpg "TDrawGrid"
//!
//! Cette classe est un contrôle ayant la forme d'un tableur, c'est à dire
//! d'une grille contenant des textes.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDrawGrid: public TCustomGrid {
private:
protected:
  TStringTab *FCells;

  //---------------------------------------------------------------------------
  //! @brief Dessin de la fenêtre
  //---------------------------------------------------------------------------

  virtual void DrawCell(HDC hdc, int ACol, int ARow, TRect Rect);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDrawGrid(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDrawGrid(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Cells
  //!
  //! Cette méthode permet de lire la propriété Cells
  //!
  //! @return @c AnsiString Propriété Cells
  //---------------------------------------------------------------------------

  virtual AnsiString Get_Cells(int Col, int Row);


  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Cells
  //!
  //! Cette méthode permet d'écrire la propriété Cells
  //!
  //! @param[in]        Col Colonne
  //! @param[in]        Row Ligne
  //! @param[in]        NewCells Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Cells(int Col, int Row, AnsiString NewCells);


  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété ColCount
  //!
  //! Cette méthode permet d'écrire la propriété ColCount
  //!
  //! @param[in]        NewColCount Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_ColCount(int NewColCount);


  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété RowCount
  //!
  //! Cette méthode permet d'écrire la propriété RowCount
  //!
  //! @param[in]        NewRowCount Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_RowCount(int NewRowCount);


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

  //@}


  //! @name Evènements
  //@{

  //@}


};


#else  // TDrawGridH

class TDrawGrid;

#endif  // TDrawGridH


#undef In_TDrawGridH
