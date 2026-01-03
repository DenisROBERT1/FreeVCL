//-----------------------------------------------------------------------------
//! @file TCustomGrid.h
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

#ifdef In_TCustomGridH
#error "Inclusion circulaire de TCustomGrid"
#endif // In_TCustomGridH

#define In_TCustomGridH

#ifndef TCustomGridH
#define TCustomGridH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <vector>

#ifndef TCustomControlH
#include "TCustomControl.h"
#endif  // TCustomControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TGridOption {
	goFixedVertLine,							//!< Lignes de séparation verticales des cases fixes 
	goFixedHorzLine,							//!< Lignes de séparation horizontales des cases fixes
	goVertLine,										//!< Lignes de séparation verticales des cases
	goHorzLine,										//!< Lignes de séparation horizontales des cases
	goRangeSelect,								//!< Possibilité de sélectionner une plage de cases (ignoré si @b goEditing est activé)
	goDrawFocusSelected,					//!< Dessin du focus sur la case sélectionnée
	goRowSizing,									//!< Redimensionnement possible des rangées
	goColSizing,									//!< Redimensionnement possible des colonnes
	goRowMoving,									//!< Déplacement possible des rangées
	goColMoving,									//!< Déplacement possible des colonnes
	goEditing,										//!< Possibilité d'édition du contenu (incompatible avec @b goRangeSelect)
	goTabs,												//!< Touches TAB et MAJ + TAB actives pour se déplacer de cases en cases
	goRowSelect,									//!< Sélection de la ligne entière au lieu de cases isolées (incompatible avec @b goAlwaysShowEditor)
	goAlwaysShowEditor,						//!< Mode édition automatiquement activé lorsque l'utilisateur se positionne dans une case (ignoré si @b goRowSelect est activé)
	goThumbTracking								//!< Rafraichissement de la grille pendant le déplacement de la scroll-bar
};

typedef Set<TGridOption, goFixedVertLine, goThumbTracking> TGridOptions;  //!< Combinaison de plusieurs TGridOption

enum TGridDrawState {
  gdSelected,										//!< La cellule est sélectionnée
  gdFocused,										//!< La cellule a le focus
  gdFixed,											//!< La cellule dans la partie fixe (voir @b FixedCols et @b FixedRows)
  gdRowSelected,								//!< La ligne entière est sélectionnée.
  gdHotTrack,										//!< La cellule est suivie
  gdPressed											//!< La cellule est dans l'état "enfoncée" (mouse down)
};

typedef Set<TGridDrawState, gdSelected, gdPressed> TGridDrawStates;  //!< Combinaison de plusieurs TGridDrawState


//-----------------------------------------------------------------------------
//! @class TCustomGrid
//!
//! @brief Définition de la classe TCustomGrid
//!
//! Cette classe est la classe de base des composants représentés sous forme
//! de tableau.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomGrid: public TCustomControl {
private:
  int DecalageX;
  int DecalageY;

  int FixedColsWidth;
  int FixedRowsHeight;
  int ColsWidth;
  int RowsHeight;
  int FGridWidth;
  int FGridHeight;
  int FUpdateCount;

  int ColSize;
  int RowSize;
  int ValInit;
  int CoordInit;
  bool bSizing;
  bool bLButtonDown;

  void CalculGridWidth(void);
  void CalculGridHeight(void);
  bool CanSize(int X, int Y, int * AColSize, int * ARowSize);
protected:
  TBorderStyle FBorderStyle;
  int FFixedCols;
  int FFixedRows;
  TColor FFixedColor;
  int FDefaultColWidth;
  int FDefaultRowHeight;
  TColor FGridLineColor;
  int FGridLineWidth;
  std::vector<int>FColWidths;
  std::vector<int>FRowHeights;
  int FRow;
  int FCol;
  TGridOptions FOptions;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessMouseMove(int X, int Y);
  virtual bool ProcessLButtonDown(int X, int Y);
  virtual bool ProcessLButtonUp(int X, int Y);
  virtual bool ProcessSize(int SizeType, int L, int H);

  //---------------------------------------------------------------------------
  //! @brief Dessin de la fenêtre
  //---------------------------------------------------------------------------

  virtual void FASTCALL Paint(void);
  virtual void DrawCell(HDC hdc, int ACol, int ARow, TRect Rect);

public:

  FVCL_BEGIN_EVENTS
  void FASTCALL GridMouseWheel(TObject *Sender, TShiftState Shift,
        int WheelDelta, const TPoint &MousePos, bool &Handled);
  void FASTCALL HorzScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);
  void FASTCALL VertScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_5(TCustomGrid, TOnMouseWheel, OnMouseWheel, GridMouseWheel, TObject*, TShiftState, int, const TPoint &, bool &);
		SET_EVENT_STR_3(TCustomGrid, TOnScroll, OnScroll, HorzScrollBarScroll, TObject*, TScrollCode, int &);
		SET_EVENT_STR_3(TCustomGrid, TOnScroll, OnScroll, VertScrollBarScroll, TObject*, TScrollCode, int &);
  }

  FVCL_END_EVENTS

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomGrid(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomGrid(void);

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
  //! @brief Renvoie les coordonnées écran d'une cellule de la grille.
  //---------------------------------------------------------------------------

  TRect FASTCALL CellRect(int ACol, int ARow);

  //---------------------------------------------------------------------------
  //! @brief Indices de colonne et de ligne aux coordonnées écran (X,Y).
  //---------------------------------------------------------------------------

  void FASTCALL MouseToCell(int X, int Y, int &ACol, int &ARow);

  //---------------------------------------------------------------------------
  //! @brief Début d'une série de modifications
  //!
  //! Cette méthode permet de faire une série de modifications sans que ça
	//! déclenche un raffraichissement de la fenêtre. L'utilisateur doit ensuite
	//! appeler la méthode TTreeNodes::EndUpdate autant de fois qu'il a appelé
	//! @b BeginUpdate (si ce n'est pas fait, les modifications risquent de ne
	//! plus être visibles).
	//!
	//! @sa TTreeNodes::EndUpdate
  //---------------------------------------------------------------------------

	void FASTCALL BeginUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Fin d'une série de modifications
  //!
  //! Cette méthode termine une série de modification après l'appel de
	//! TTreeNodes::BeginUpdate.
	//!
	//! @sa TTreeNodes::BeginUpdate
  //---------------------------------------------------------------------------

	void FASTCALL EndUpdate(void);

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
  //! @brief Propriété BorderStyle
	//!
	//! Cette propriété définit le style de bordure du contrôle.
	//! Seuls les styles @b bsNone et @b bsSingle peuvent être utilisés pour ce
	//! composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, TBorderStyle, BorderStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété ColCount
	//!
	//! Cette propriété définit le nombre de colonnes du contrôle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, ColCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété RowCount
	//!
	//! Cette propriété définit le nombre de lignes du contrôle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, RowCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété FixedRows
	//!
	//! Cette propriété définit le nombre de lignes fixes du contrôle. Les lignes
	//! fixes sont dessinées avec un fond d'une autre couleur (définie par la
	//! propriété @b FixedColor).
	//!
	//! @sa FixedColor
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, FixedRows);

  //---------------------------------------------------------------------------
  //! @brief Propriété FixedCols
	//!
	//! Cette propriété définit le nombre de colonnes fixes du contrôle. Les
	//! colonnes fixes sont dessinées avec un fond d'une autre couleur (définie
	//! par la propriété @b FixedColor).
	//!
	//! @sa FixedColor
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, FixedCols);

  //---------------------------------------------------------------------------
  //! @brief Propriété FixedColor
	//!
	//! Cette propriété détermine la couleur des lignes et des colonnes fixes.
	//! Par défaut c'est la même couleur que les boutons.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, TColor, FixedColor);

  //---------------------------------------------------------------------------
  //! @brief Propriété DefaultColWidth
	//!
	//! Cette propriété définit la largeur par défaut des colonnes en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, DefaultColWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété DefaultRowHeight
	//!
	//! Cette propriété définit la hauteur par défaut des lignes en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, DefaultRowHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété GridLineColor
	//!
	//! Cette propriété définit la couleur de la grille.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, TColor, GridLineColor);

  //---------------------------------------------------------------------------
  //! @brief Propriété GridLineWidth
	//!
	//! Cette propriété définit la largeur des traits de la grille.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, GridLineWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété RowHeights
	//!
	//! Cette propriété définit la hauteur d'une ligne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TCustomGrid, int, RowHeights);

  //---------------------------------------------------------------------------
  //! @brief Propriété ColWidths
	//!
	//! Cette propriété définit la largeur d'une colonne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TCustomGrid, int, ColWidths);

  //---------------------------------------------------------------------------
  //! @brief Propriété Row
	//!
	//! Cette propriété définit la ligne courante.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, Row);

  //---------------------------------------------------------------------------
  //! @brief Propriété Col
	//!
	//! Cette propriété définit la colonne courante.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, int, Col);

  //---------------------------------------------------------------------------
  //! @brief Propriété GridWidth
	//!
	//! Cette propriété permet de lire la largeur de la grille en pixels. La
	//! grille apparaitra avec une barre de défilement ou un zone vide si elle
	//! est plus grande ou plus petite que le composant lui-même.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCustomGrid, int, GridWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété GridHeight
	//!
	//! Cette propriété permet de lire la hauteur de la grille en pixels. La
	//! grille apparaitra avec une barre de défilement ou un zone vide si elle
	//! est plus grande ou plus petite que le composant lui-même.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCustomGrid, int, GridHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété Options
	//!
	//! Cette propriété permet de définir différentes options visuelles de la
	//! grille.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomGrid, TGridOptions, Options);

   //---------------------------------------------------------------------------
  //! @brief Propriété UpdateCount
	//!
	//! Cette propriété indique le nombre de fois où @b BeginUpdate a été appelé
	//! sans appel à @b EndUpdate.
	//!
	//! @sa BeginUpdate, EndUpdate
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCustomGrid, int, UpdateCount);

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


#else  // TCustomGridH

class TCustomGrid;

#endif  // TCustomGridH


#undef In_TCustomGridH
