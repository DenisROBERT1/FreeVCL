//-----------------------------------------------------------------------------
//! @file TForm_Proprietes.h
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

#ifndef TForm_ProprietesH
#define TForm_ProprietesH

//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------

#include <vector>


//---------------------------------------------------------------------------
//! @class TForm_Proprietes
//!
//! @brief Fenêtre de saisie de la position et de l'ancrage des objets
//!
//! Cette classe (@c TForm_Proprietes) permet de saisir les caractéristiques
//! communes d'un objet (nom, position, ancrages, propriétés).
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TForm_Proprietes : public TForm {
private:
protected:
  TAnchors FAnchorsElement;
  TAnchors FAnchorsElements;
  int FLeftElement;
  int FTopElement;
  int FWidthElement;
  int FHeightElement;
  int FLeftParent;
  int FTopParent;
  int FWidthParent;
  int FHeightParent;
  static int MemoLeft;
  static int MemoTop;
  static int MemoWidth;
  static int MemoHeight;
  static int MemoActivePageIndex;
  static bool bCoordRelatives;

  void GetDecalageCoord(int *X1, int *Y1, int *X2, int *Y2);

public:
  FVCL_BEGIN_COMPONENTS
  TLabel *Label_Nom;
  TEdit *Edit_Nom;
  TRadioButton *RadioButton_Relatives;
  TRadioButton *RadioButton_Absolues;
  TLabel *Label_PositionX1;
  TEdit *Edit_PositionX1;
  TUpDown *UpDown_PositionX1;
  TLabel *Label_UnitPositionX1;
  TLabel *Label_PositionY1;
  TEdit *Edit_PositionY1;
  TUpDown *UpDown_PositionY1;
  TLabel *Label_UnitPositionY1;
  TGroupBox *GroupBox_SupGauche;
  TLabel *Label_PositionX2;
  TEdit *Edit_PositionX2;
  TUpDown *UpDown_PositionX2;
  TLabel *Label_UnitPositionX2;
  TLabel *Label_PositionY2;
  TEdit *Edit_PositionY2;
  TUpDown *UpDown_PositionY2;
  TLabel *Label_UnitPositionY2;
  TGroupBox *GroupBox_InfDroit;
  TLabel *Label_Largeur;
  TEdit *Edit_Largeur;
  TUpDown *UpDown_Largeur;
  TLabel *Label_UnitLargeur;
  TLabel *Label_Hauteur;
  TEdit *Edit_Hauteur;
  TUpDown *UpDown_Hauteur;
  TLabel *Label_UnitHauteur;
  TGroupBox *GroupBox_Dimensions;
  TButton *Button_Aligner;
  TTabSheet *TabSheet_Position;
  TLabel *LabelAncrage;
  TBevel *BevelAncrage;
  TShape *Shape_Left;
  TShape *Shape_LeftTop;
  TShape *Shape_Top;
  TShape *Shape_RightTop;
  TShape *Shape_Right;
  TShape *Shape_RightBottom;
  TShape *Shape_Bottom;
  TShape *Shape_LeftBottom;
  TBitBtn *BitBtn_Predef1;
  TBitBtn *BitBtn_Predef2;
  TBitBtn *BitBtn_Predef3;
  TBitBtn *BitBtn_Predef4;
  TBitBtn *BitBtn_Predef5;
  TBitBtn *BitBtn_Predef6;
  TBitBtn *BitBtn_Predef7;
  TBitBtn *BitBtn_Predef8;
  TBitBtn *BitBtn_Predef9;
  TBitBtn *BitBtn_Predef10;
  TBitBtn *BitBtn_Predef11;
  TBitBtn *BitBtn_Predef12;
  TBitBtn *BitBtn_Predef13;
  TBitBtn *BitBtn_Predef14;
  TGroupBox *GroupBox_Predefinis;
  TTabSheet *TabSheet_Ancrages;
  TScrollBox *ScrollBox_Proprietes;
  TTabSheet *TabSheet_Proprietes;
  TPageControl *PageControl_Proprietes;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
	TOpenDialog *OpenDialog_Graphic;
	FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL RadioButton_AbsoluesClick(TObject *Sender);
  void FASTCALL RadioButton_RelativesClick(TObject *Sender);
  void FASTCALL Edit_PositionX1Change(TObject *Sender);
  void FASTCALL Edit_PositionY1Change(TObject *Sender);
  void FASTCALL Edit_PositionX2Change(TObject *Sender);
  void FASTCALL Edit_PositionY2Change(TObject *Sender);
  void FASTCALL Edit_LargeurChange(TObject *Sender);
  void FASTCALL Edit_HauteurChange(TObject *Sender);
  void FASTCALL Button_AlignerClick(TObject *Sender);
  void FASTCALL Shape_LeftMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL Shape_LeftTopMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL Shape_TopMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL Shape_RightTopMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL Shape_RightMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL Shape_RightBottomMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL Shape_BottomMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL Shape_LeftBottomMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL BitBtn_Predef7Click(TObject *Sender);
  void FASTCALL BitBtn_Predef1Click(TObject *Sender);
  void FASTCALL BitBtn_Predef2Click(TObject *Sender);
  void FASTCALL BitBtn_Predef3Click(TObject *Sender);
  void FASTCALL BitBtn_Predef4Click(TObject *Sender);
  void FASTCALL BitBtn_Predef5Click(TObject *Sender);
  void FASTCALL BitBtn_Predef6Click(TObject *Sender);
  void FASTCALL BitBtn_Predef8Click(TObject *Sender);
  void FASTCALL BitBtn_Predef9Click(TObject *Sender);
  void FASTCALL BitBtn_Predef10Click(TObject *Sender);
  void FASTCALL BitBtn_Predef11Click(TObject *Sender);
  void FASTCALL BitBtn_Predef12Click(TObject *Sender);
  void FASTCALL BitBtn_Predef13Click(TObject *Sender);
  void FASTCALL BitBtn_Predef14Click(TObject *Sender);
	void FASTCALL Button_ParcourirClick(TObject *Sender);
	void FASTCALL Button_CodeClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
  void FASTCALL FormClose(TObject *Sender, TCloseAction &Action);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, RadioButton_AbsoluesClick, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, RadioButton_RelativesClick, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnChange, Edit_PositionX1Change, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnChange, Edit_PositionY1Change, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnChange, Edit_PositionX2Change, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnChange, Edit_PositionY2Change, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnChange, Edit_LargeurChange, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnChange, Edit_HauteurChange, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, Button_AlignerClick, TObject*);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_LeftMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_LeftTopMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_TopMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_RightTopMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_RightMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_RightBottomMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_BottomMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_5(TForm_Proprietes, TOnMouseDown, OnMouseDown, Shape_LeftBottomMouseDown, TObject*, TMouseButton, TShiftState, int, int);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef7Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef1Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef2Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef3Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef4Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef5Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef6Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef8Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef9Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef10Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef11Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef12Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef13Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, BitBtn_Predef14Click, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, Button_ParcourirClick, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, Button_CodeClick, TObject*);
    SET_EVENT_STR_1(TForm_Proprietes, TNotifyEvent, OnClick, Button_AideClick, TObject*);
    SET_EVENT_STR_2(TForm_Proprietes, TOnClose, OnClose, FormClose, TObject *, TCloseAction &);
  }

  FVCL_END_EVENTS


  TForm_Proprietes(TComponent *Owner, LPCTSTR szName);
  virtual ~TForm_Proprietes(void);

  //! @name Méthodes
  //@{

  void MajCoordonnees();

  void MajAncrages();

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété AnchorsElement
	//!
	//! Cette propriété représente les ancrages définis dans la fenêtre des
	//! propriétés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, TAnchors, AnchorsElement);


  //---------------------------------------------------------------------------
  //! @brief Propriété AnchorsElements
	//!
	//! Cette propriété représente les ancrages communs à tous les éléments (cas
	//! d'une sélection multiple).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, TAnchors, AnchorsElements);


  //---------------------------------------------------------------------------
  //! @brief Propriété LeftElement
	//!
	//! Cette propriété représente la propriété @b Left définie dans la fenêtre
	//! des propriétés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, LeftElement);


  //---------------------------------------------------------------------------
  //! @brief Propriété TopElement
	//!
	//! Cette propriété représente la propriété @b Top définie dans la fenêtre
	//! des propriétés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, TopElement);


  //---------------------------------------------------------------------------
  //! @brief Propriété WidthElement
	//!
	//! Cette propriété représente la propriété @b Width définie dans la fenêtre
	//! des propriétés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, WidthElement);


  //---------------------------------------------------------------------------
  //! @brief Propriété HeightElement
	//!
	//! Cette propriété représente la propriété @b Height définie dans la fenêtre
	//! des propriétés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, HeightElement);


  //---------------------------------------------------------------------------
  //! @brief Propriété LeftParent
	//!
	//! Cette propriété représente la propriété @b Left de la fenêtre parente
	//! (pour coordonnées relatives).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, LeftParent);


  //---------------------------------------------------------------------------
  //! @brief Propriété TopParent
	//!
	//! Cette propriété représente la propriété @b Top de la fenêtre parente
	//! (pour coordonnées relatives).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, TopParent);


  //---------------------------------------------------------------------------
  //! @brief Propriété WidthParent
	//!
	//! Cette propriété représente la propriété @b Width de la fenêtre parente
	//! (pour coordonnées relatives).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, WidthParent);


  //---------------------------------------------------------------------------
  //! @brief Propriété HeightParent
	//!
	//! Cette propriété représente la propriété @b Height de la fenêtre parente
	//! (pour coordonnées relatives).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, int, HeightParent);


  //---------------------------------------------------------------------------
  //! @brief Propriété NomObj
	//!
	//! Cette propriété représente la propriété @b Name définie dans la fenêtre
	//! des propriétés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TForm_Proprietes, AnsiString, NomObj);


  //@}

};
//---------------------------------------------------------------------------
#endif
