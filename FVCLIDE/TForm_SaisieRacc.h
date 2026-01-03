//-----------------------------------------------------------------------------
//! @file TForm_SaisieRacc.h
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

#ifndef TForm_SaisieRaccH
#define TForm_SaisieRaccH

//---------------------------------------------------------------------------
#include <FreeVCL.h>
#include <vector>


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define NBCORRESPKEY (sizeof(CorrespKey) / sizeof(CORRESPKEY))


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

#pragma pack(push, 1)

typedef struct {
  TCHAR szNom[7];
  WORD Key;
} CORRESPKEY;

typedef struct {
  int Commande;                        // Commande
	TShortCut ShortCut;									 // Raccourci
  int Categorie;                       // Catégorie de la commande
  TAction *Action;										 // Adresse de l'action
  AnsiString asLib;                    // Libellé de la commande
} COMMANDE, * LPCOMMANDE;

typedef struct {
  int Categorie;                       // Catégorie
  AnsiString asLib;                    // Libellé de la catégorie
} CATEGORIE, * LPCATEGORIE;

#pragma pack(pop)


//---------------------------------------------------------------------------
class TForm_SaisieRacc : public TForm {
private: // Déclarations de l'utilisateur

  std::vector<CATEGORIE> Categories;
  std::vector<COMMANDE> Commandes;

  bool bModifs;
  bool bNotChangeTreeView;
  bool bNotChangeListView;
  bool bNotChangeGroupBox;
  bool bNotChangeHotKey;

  void FASTCALL DrawTouche(
      TCanvas * ACanvas, int x, int y, AnsiString asTexte, AnsiString asBitmap,
      bool bKeyDown, bool bGrayed);
  void FASTCALL DrawTouche(
      int x, int y, AnsiString asTexte, AnsiString asBitmap, bool bKeyDown);
  bool FASTCALL GetListeCommandes();
  void FASTCALL InitRaccourcis();
	void FASTCALL AddRaccourci(TTreeNode *TreeNodeSource);
  bool FASTCALL RegenTreeView();
  AnsiString FASTCALL TexteRaccourci();
  void FASTCALL UpdateGroupBox(TShortCut ShortCut);
  void FASTCALL UpdateHotKey(TShortCut ShortCut);

public:  // Déclarations de l'utilisateur
  FVCL_BEGIN_COMPONENTS
  TLabel *LabelFonctions;
  TTreeView *TreeView_Fnct;
  TLabel *LabelListeRacc;
  TListView *ListView_Raccourcis;
  TButton *Button_Supprimer;
  TButton *Button_MAZ;
  TButton *Button_Default;
  TPaintBox *PaintBox_Alt;
  TPaintBox *PaintBox_Shift;
  TPaintBox *PaintBox_Ctrl;
  TComboBox *ComboBox_ToucheRacc;
  THotKey *HotKey_SaisieDirecte;
  TGroupBox *GroupBox;
  TButton *Button_Ok;
  TButton *Button_Annuler;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_OkClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
  void FASTCALL Button_SupprimerClick(TObject *Sender);
  void FASTCALL Button_MAZClick(TObject *Sender);
  void FASTCALL Button_DefaultClick(TObject *Sender);
  void FASTCALL RaccourciChange(TObject *Sender);
  void FASTCALL HotKey_SaisieDirecteChange(TObject *Sender);
  void FASTCALL TreeView_FnctDblClick(TObject *Sender);
  void FASTCALL TreeView_FnctSelect(TObject *Sender);
  void FASTCALL ListView_RaccourcisChange(TObject *Sender);
  void FASTCALL PaintBox_AltPaint(TObject *Sender);
  void FASTCALL PaintBox_ShiftPaint(TObject *Sender);
  void FASTCALL PaintBox_CtrlPaint(TObject *Sender);
  void FASTCALL PaintBox_AltClick(TObject *Sender);
  void FASTCALL PaintBox_ShiftClick(TObject *Sender);
  void FASTCALL PaintBox_CtrlClick(TObject *Sender);
  void FASTCALL FormClose(TObject *Sender, TCloseAction &Action);
  void FASTCALL ListView_RaccourcisKeyDown(TObject *Sender, WORD Key,
          TShiftState Shift);
  void FASTCALL ComboBox_ToucheRaccDrawItem(TObject *Sender,
          int Index, const TRect &Rect, TOwnerDrawStates States);
  void FASTCALL ListView_RaccourcisCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
  void FASTCALL FormDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
  void FASTCALL TreeView_FnctStartDrag(TObject *Sender,
          TDragObject *DragObject);
  void FASTCALL TreeView_FnctEndDrag(TObject *Sender, TObject *Target,
          int X, int Y);
  void FASTCALL ListView_RaccourcisDragOver(TObject *Sender,
          TObject *Source, int X, int Y, TDragState State, bool &Accept);
  void FASTCALL ListView_RaccourcisEndDrag(TObject *Sender,
          TObject *Target, int X, int Y);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, Button_OkClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, Button_AideClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, Button_SupprimerClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, Button_MAZClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, Button_DefaultClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnChange, RaccourciChange, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnChange, HotKey_SaisieDirecteChange, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnDblClick, TreeView_FnctDblClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnSelect, TreeView_FnctSelect, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnChange, ListView_RaccourcisChange, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnPaint, PaintBox_AltPaint, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnPaint, PaintBox_ShiftPaint, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnPaint, PaintBox_CtrlPaint, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, PaintBox_AltClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, PaintBox_ShiftClick, TObject *);
    SET_EVENT_STR_1(TForm_SaisieRacc, TNotifyEvent, OnClick, PaintBox_CtrlClick, TObject *);
    SET_EVENT_STR_2(TForm_SaisieRacc, TOnClose, OnClose, FormClose, TObject *, TCloseAction &);
    SET_EVENT_STR_3(TForm_SaisieRacc, TOnKeyDown, OnKeyDown, ListView_RaccourcisKeyDown, TObject *, WORD, TShiftState);
    SET_EVENT_STR_4(TForm_SaisieRacc, TOnDrawItem, OnDrawItem, ComboBox_ToucheRaccDrawItem, TObject *, int, const TRect &, TOwnerDrawStates);
    // $$$---$$$ SET_EVENT_STR_5(TForm_SaisieRacc, TOnCompare, OnCompare, ListView_RaccourcisCompare, TObject *, TListItem *, TListItem *, int, int &);
    SET_EVENT_STR_6(TForm_SaisieRacc, TOnDragOver, OnDragOver, FormDragOver, TObject *, TObject *, int, int, TDragState, bool &);
    SET_EVENT_STR_2(TForm_SaisieRacc, TOnStartDrag, OnStartDrag, TreeView_FnctStartDrag, TObject *, TDragObject *);
    SET_EVENT_STR_4(TForm_SaisieRacc, TOnEndDrag, OnEndDrag, TreeView_FnctEndDrag, TObject *, TObject *, int, int);
    SET_EVENT_STR_6(TForm_SaisieRacc, TOnDragOver, OnDragOver, ListView_RaccourcisDragOver, TObject *, TObject *, int, int, TDragState, bool &);
    SET_EVENT_STR_4(TForm_SaisieRacc, TOnEndDrag, OnEndDrag, ListView_RaccourcisEndDrag, TObject *, TObject *, int, int);
  }

  FVCL_END_EVENTS

  TForm_SaisieRacc(TComponent *Owner, LPCTSTR szName);
  virtual ~TForm_SaisieRacc(void);

  //! @name Méthodes
  //@{

  //@}


  //! @name Propriétés
  //@{

  //@}


  //! @name Evènements
  //@{

  //@}

};
//---------------------------------------------------------------------------
#endif
