//-----------------------------------------------------------------------------
//! @file TForm_SaisieRacc.cpp
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

#include <FreeVCL.h>


//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------

#include "TConfig.h"
#include "TForm_DisplayError.h"
#include "TForm_SaisieRacc.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
CORRESPKEY CorrespKey[] = {
  {_T("ESC"), VK_ESCAPE},
  {_T("F1"), VK_F1},
  {_T("F2"), VK_F2},
  {_T("F3"), VK_F3},
  {_T("F4"), VK_F4},
  {_T("F5"), VK_F5},
  {_T("F6"), VK_F6},
  {_T("F7"), VK_F7},
  {_T("F8"), VK_F8},
  {_T("F9"), VK_F9},
  {_T("F10"), VK_F10},
  {_T("F11"), VK_F11},
  {_T("F12"), VK_F12},
  {_T("A"), _T('A')},
  {_T("B"), _T('B')},
  {_T("C"), _T('C')},
  {_T("D"), _T('D')},
  {_T("E"), _T('E')},
  {_T("F"), _T('F')},
  {_T("G"), _T('G')},
  {_T("H"), _T('H')},
  {_T("I"), _T('I')},
  {_T("J"), _T('J')},
  {_T("K"), _T('K')},
  {_T("L"), _T('L')},
  {_T("M"), _T('M')},
  {_T("N"), _T('N')},
  {_T("O"), _T('O')},
  {_T("P"), _T('P')},
  {_T("Q"), _T('Q')},
  {_T("R"), _T('R')},
  {_T("S"), _T('S')},
  {_T("T"), _T('T')},
  {_T("U"), _T('U')},
  {_T("V"), _T('V')},
  {_T("W"), _T('W')},
  {_T("X"), _T('X')},
  {_T("Y"), _T('Y')},
  {_T("Z"), _T('Z')},
  {_T("0"), VK_NUMPAD0},
  {_T("1"), VK_NUMPAD1},
  {_T("2"), VK_NUMPAD2},
  {_T("3"), VK_NUMPAD3},
  {_T("4"), VK_NUMPAD4},
  {_T("5"), VK_NUMPAD5},
  {_T("6"), VK_NUMPAD6},
  {_T("7"), VK_NUMPAD7},
  {_T("8"), VK_NUMPAD8},
  {_T("9"), VK_NUMPAD9},
  {_T("/"), VK_DIVIDE},
  {_T("*"), VK_MULTIPLY},
  {_T("-"), VK_SUBTRACT},
  {_T("+"), VK_ADD},
  {_T("."), VK_DECIMAL},
  {_T("Tab"), VK_TAB},
  {_T("<--"), VK_BACK},
  {_T("ESPACE"), VK_SPACE},
  {_T("Entrée"), VK_RETURN},
  {_T("Ins"), VK_INSERT},
  {_T("Sup"), VK_DELETE},
  {_T("Déb"), VK_HOME},
  {_T("Fin"), VK_END},
  {_T("PG-UP"), VK_PRIOR},
  {_T("PG-DW"), VK_NEXT},
  {_T("<-"), VK_LEFT},
  {_T("->"), VK_RIGHT},
  {_T("^"), VK_UP},
  {_T("v"), VK_DOWN},
  {_T("CLIC-G"), VK_LBUTTON},
  {_T("CLIC-M"), VK_MBUTTON},
  {_T("CLIC-D"), VK_RBUTTON}
};

//---------------------------------------------------------------------------
TForm_SaisieRacc::TForm_SaisieRacc(TComponent *Owner, LPCTSTR szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {
  int i, j;
  TTreeNode * TreeNodeCategorie;
  TTreeNode * TreeNodeCommande;


  bNotChangeTreeView = false;
  bNotChangeListView = false;
  bNotChangeGroupBox = false;
  bNotChangeHotKey = false;

  GetListeCommandes();
  for (i = 0; i < (int) Categories.size(); i++) {
    TreeNodeCategorie = TreeView_Fnct->Items->Add(NULL, Categories[i].asLib);
    for (j = 0; j < (int) Commandes.size(); j++) {
      if (Commandes[j].Categorie == Categories[i].Categorie) {
        TreeNodeCommande = TreeView_Fnct->Items->AddChild(TreeNodeCategorie, Commandes[j].asLib);
        TreeNodeCommande->Data = (void *) (size_t) Commandes[j].Commande;
      }
    }
  }

  for (i = 0; i < (int) NBCORRESPKEY; i++) {
    ComboBox_ToucheRacc->Items->Add(CorrespKey[i].szNom);
  }

  InitRaccourcis();
}

//---------------------------------------------------------------------------
TForm_SaisieRacc::~TForm_SaisieRacc(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::InitRaccourcis(void) {
  TListItem * ListItem;
  int i;


  ListView_Raccourcis->Items->Clear();

	for (i = 0; i < (int) Commandes.size(); i++) {
		if (Commandes[i].ShortCut != 0) {
			ListItem = ListView_Raccourcis->Items->Add();
			ListItem->Data = (void *) (size_t) Commandes[i].Commande;
	    // Libellé
      ListItem->Caption = Commandes[i].asLib;
			// Texte du raccourci
			ListItem->SubItems->Add(ShortCutToText(Commandes[i].ShortCut));
		}
	}

  ListView_Raccourcis->AlphaSort();

  bModifs = false;
}

//---------------------------------------------------------------------------
bool FASTCALL TForm_SaisieRacc::GetListeCommandes(void) {
  CATEGORIE NewCategorie;
  COMMANDE NewCommande;
	TMenuItem *MenuItems[5];  // 5 niveaux de menus
	int Index[5];
	int Categorie[5];
	TMenuItem *MenuItem;
	int LastCat;
	int Level;


	MenuItems[0] = Application->MainForm->Menu;
	Categorie[0] = 0;
	Index[0] = 0;
	Level = 0;
	LastCat = 1;

	while (Level >= 0) {
		if (Index[Level] < MenuItems[Level]->Count) {
			MenuItem = dynamic_cast<TMenuItem *>(MenuItems[Level]->Components[Index[Level]]);
			if (MenuItem) {

				AnsiString asCaption;
				TReplaceFlags ReplaceFlags;
				ReplaceFlags.Clear() << rfReplaceAll;
				asCaption = StringReplace(MenuItem->Caption, _T("&"), _T(""), ReplaceFlags);

				if (MenuItem->Count) {

					NewCategorie.Categorie = LastCat++;
					NewCategorie.asLib = asCaption;
					Categories.push_back(NewCategorie);

					Level++;
					MenuItems[Level] = MenuItem;
					Categorie[Level] = NewCategorie.Categorie;
					Index[Level] = -1;
				}
				else {
					if (asCaption != _T("-") && asCaption != _T("|")) {
						if (MenuItem->Action != NULL) {
							NewCommande.Commande = MenuItem->Command;
							NewCommande.ShortCut = MenuItem->Action->ShortCut;
							NewCommande.Categorie = Categorie[Level];
							NewCommande.Action = MenuItem->Action;
							NewCommande.asLib = asCaption;
							Commandes.push_back(NewCommande);
						}
					}
				}
			}
		}
		else {
			Level--;
		}
		if (Level >= 0) Index[Level]++;
	}

  return true;
}


//---------------------------------------------------------------------------
//
//                                   DRAW
//
//---------------------------------------------------------------------------

void FASTCALL TForm_SaisieRacc::ComboBox_ToucheRaccDrawItem(
      TObject *Sender, int Index, const TRect &Rect, TOwnerDrawStates States) {

  AnsiString asBitmap;
  AnsiString asTexte;
  TRect RectDraw;
  bool bKeyDown;


  if (States.Contains(odSelected)) {
    ComboBox_ToucheRacc->Canvas->Brush->Color = clHighlight;
    bKeyDown = true;
  }
  else {
    ComboBox_ToucheRacc->Canvas->Brush->Color = clMenu;
    bKeyDown = false;
  }
  ComboBox_ToucheRacc->Canvas->FillRect(Rect);

  if (Index >= 0) {

		asTexte = CorrespKey[Index].szNom;

		if (CorrespKey[Index].Key == VK_TAB) asBitmap = _T("BMP_TAB");
		else if (CorrespKey[Index].Key == VK_HOME) asBitmap = _T("BMP_HOME");
		else if (CorrespKey[Index].Key == VK_PRIOR) asBitmap = _T("BMP_PGUP");
		else if (CorrespKey[Index].Key == VK_NEXT) asBitmap = _T("BMP_PGDOWN");
		else if (CorrespKey[Index].Key == VK_LEFT) asBitmap = _T("BMP_LEFT");
		else if (CorrespKey[Index].Key == VK_RIGHT) asBitmap = _T("BMP_RIGHT");
		else if (CorrespKey[Index].Key == VK_UP) asBitmap = _T("BMP_UP");
		else if (CorrespKey[Index].Key == VK_DOWN) asBitmap = _T("BMP_DOWN");
	}
	else asTexte = _T(" ");

	RectDraw = Rect; // Si on utilise Rect directement : passing 'const TRect::GetLeft' as 'this' argument discards qualifiers
	DrawTouche(ComboBox_ToucheRacc->Canvas, RectDraw.Left, RectDraw.Top, asTexte, asBitmap,
             bKeyDown, States.Contains(odGrayed));
}
//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::PaintBox_AltPaint(TObject *Sender) {
  TCanvas *ACanvas;
  TRect Rect;


  ACanvas = PaintBox_Alt->Canvas;
  Rect = PaintBox_Alt->ClientRect;
  DrawTouche(ACanvas, Rect.Left, Rect.Top, _T("Alt"), _T(""), PaintBox_Alt->Tag != 0, !PaintBox_Alt->Enabled);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::PaintBox_ShiftPaint(TObject *Sender) {
  TCanvas *ACanvas;
  TRect Rect;


  ACanvas = PaintBox_Shift->Canvas;
  Rect = PaintBox_Shift->ClientRect;
  DrawTouche(ACanvas, Rect.Left, Rect.Top, _T("Maj"), _T("BMP_TSHIFT"), PaintBox_Shift->Tag != 0, !PaintBox_Shift->Enabled);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::PaintBox_CtrlPaint(TObject *Sender) {
  TCanvas *ACanvas;
  TRect Rect;


  ACanvas = PaintBox_Ctrl->Canvas;
  Rect = PaintBox_Ctrl->ClientRect;
  DrawTouche(ACanvas, Rect.Left, Rect.Top, _T("Ctrl"), _T(""), PaintBox_Ctrl->Tag != 0, !PaintBox_Ctrl->Enabled);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::DrawTouche(
    TCanvas * ACanvas, int x, int y, AnsiString asTexte, AnsiString asBitmap,
    bool bKeyDown, bool bGrayed) {
  TBitmap * Bitmap;
  TRect Rect1, Rect2;
  int xTexte, yTexte, x2, y2;
  bool bXXL;


  Bitmap = new TBitmap();
  Bitmap->LoadFromResourceName(HInstance, AnsiString(bKeyDown? _T("BMP_KEYDOWN"): _T("BMP_KEYUP")));
  bXXL = false;
  if (asBitmap.IsEmpty()) {
    switch (asTexte.Length()) {
      case 1: xTexte = 10;  break;
      case 2: xTexte = 8; break;
      case 3: xTexte = 6; break;
      case 4: xTexte = 4; break;
      default: xTexte = 8; bXXL = true; break;
    }
    yTexte = 8;
  }
  else {
    xTexte = 8;
    yTexte = 7;
  }
  if (bKeyDown) {
    xTexte += 1;
    yTexte += 2;
  }
  if (bXXL) {
    SetRect(&Rect1, x, y, x + 24, y + 32);
    SetRect(&Rect2, 0, 0, 24, 32);
    ACanvas->CopyRect(Rect1, Bitmap->Canvas, Rect2);
    SetRect(&Rect1, x + 24, y, x + 40, y + 32);
    SetRect(&Rect2, 8, 0, 24, 32);
    ACanvas->CopyRect(Rect1, Bitmap->Canvas, Rect2);
    SetRect(&Rect1, x + 40, y, x + 64, y + 32);
    SetRect(&Rect2, 8, 0, 32, 32);
    ACanvas->CopyRect(Rect1, Bitmap->Canvas, Rect2);
  }
  else ACanvas->Draw(x, y, Bitmap);

  if (!asBitmap.IsEmpty()) {
    Bitmap->LoadFromResourceName(HInstance, asBitmap);
    if (bGrayed) {
      for (y2 = 0; y2 < Bitmap->Height; y2++) {
        for (x2 = 0; x2 < Bitmap->Width; x2++) {
          if (Bitmap->Canvas->Get_Pixels(x2, y2) == clBlack) Bitmap->Canvas->Set_Pixels(x2, y2, clGray);
        }
      }
    }
    ACanvas->Draw(x + xTexte, y + yTexte, Bitmap);
  }
  else {
    ACanvas->TextFlags = tfTransparent;
    if (bGrayed) ACanvas->Font->Color = clGrayText;
    else ACanvas->Font->Color = clBtnText;
    ACanvas->Brush->Color = clLtGray;
    ACanvas->TextOut(x + xTexte, y + yTexte, asTexte);
  }
  delete Bitmap;
}
//---------------------------------------------------------------------------
bool FASTCALL TForm_SaisieRacc::RegenTreeView(void) {
  return true;
}


//---------------------------------------------------------------------------
//
//                           DRAG AND DROP
//
//---------------------------------------------------------------------------

void FASTCALL TForm_SaisieRacc::TreeView_FnctStartDrag(TObject *Sender,
      TDragObject *DragObject) {
  TTreeNode * TreeNodeFonc;


  DragObject = NULL;
  TreeNodeFonc = TreeView_Fnct->Selected;
  if (TreeNodeFonc->Level != 1) {
    MessageBeep(0);
    TreeView_Fnct->EndDrag(false);
  }

}
//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::ListView_RaccourcisDragOver(
      TObject *Sender, TObject *Source, int X, int Y, TDragState State,
      bool &Accept) {
  TTreeNode * TreeNodeSource;


  TreeNodeSource = TreeView_Fnct->Selected;
  Accept = (TreeNodeSource->Level > 0);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::FormDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept) {
  Accept = (Source != TreeView_Fnct);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::TreeView_FnctEndDrag(TObject *Sender,
      TObject *Target, int X, int Y) {

  if (Target == ListView_Raccourcis) {
		AddRaccourci(TreeView_Fnct->Selected);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::ListView_RaccourcisEndDrag(
      TObject *Sender, TObject *Target, int X, int Y) {
  TListItem * ListItem;


  if (Target == this) {
    ListItem = ListView_Raccourcis->Selected;
    ListItem->Delete();
    bModifs = true;
  }
}


//---------------------------------------------------------------------------
//
//                           CONTROLES ET BOUTONS
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void FASTCALL TForm_SaisieRacc::TreeView_FnctSelect(TObject *Sender) {
  TTreeNode * TreeNode;
  int ListViewItemIndex;
  int i;


	if (!bNotChangeTreeView) {

		bNotChangeTreeView = true;

		if (!bNotChangeListView) {

			ListViewItemIndex = -1;

			TreeNode = TreeView_Fnct->Selected;

			if (TreeNode) {
				for (i = 0; i < ListView_Raccourcis->Items->Count; i++) {
					if (TreeNode->Data == ListView_Raccourcis->Items->Item[i]->Data) {
						ListViewItemIndex = i;
						break;
					}
				}
			}

			ListView_Raccourcis->ItemIndex = ListViewItemIndex;

		}

		bNotChangeTreeView = false;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::TreeView_FnctDblClick(TObject *Sender) {
	AddRaccourci(TreeView_Fnct->Selected);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::AddRaccourci(TTreeNode *TreeNodeSource) {
  TListItem * ListItem;
  int ListViewItemIndex;
  int i;


  if (TreeNodeSource && TreeNodeSource->Level > 0) {

		ListViewItemIndex = -1;

		for (i = 0; i < ListView_Raccourcis->Items->Count; i++) {
			if (TreeNodeSource->Data == ListView_Raccourcis->Items->Item[i]->Data) {
				ListViewItemIndex = i;
				break;
			}
		}

		if (ListViewItemIndex == -1) {
			ListItem = ListView_Raccourcis->Items->Add();
			ListItem->Caption = TreeNodeSource->Text;
			ListItem->Data = TreeNodeSource->Data;
			ListItem->SubItems->Add(_T(""));
			ListView_Raccourcis->Selected = ListItem;
			HotKey_SaisieDirecte->SetFocus();
			ListViewItemIndex = ListItem->NumItem;
			bModifs = true;
		}

		ListView_Raccourcis->ItemIndex = ListViewItemIndex;
		if (ListViewItemIndex < ListView_Raccourcis->TopItem->NumItem ||
			ListViewItemIndex > ListView_Raccourcis->TopItem->NumItem + ListView_Raccourcis->VisibleRowCount) {
			ListView_Raccourcis->TopIndex = ListViewItemIndex;
		}

  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::PaintBox_AltClick(TObject *Sender) {
  PaintBox_Alt->Tag = (BOOL) (PaintBox_Alt->Tag == 0);
  PaintBox_Alt->Invalidate();
  RaccourciChange(Sender);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::PaintBox_ShiftClick(TObject *Sender) {
  PaintBox_Shift->Tag = (BOOL) (PaintBox_Shift->Tag == 0);
  PaintBox_Shift->Invalidate();
  RaccourciChange(Sender);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::PaintBox_CtrlClick(TObject *Sender) {
  PaintBox_Ctrl->Tag = (BOOL) (PaintBox_Ctrl->Tag == 0);
  PaintBox_Ctrl->Invalidate();
  RaccourciChange(Sender);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::ListView_RaccourcisChange(TObject *Sender) {
  TListItem * ItemSelected;


  if (!bNotChangeListView) {

		bNotChangeListView = true;

		ItemSelected = ListView_Raccourcis->Selected;
		if (ItemSelected) {
			int i;

		  if (!bNotChangeTreeView) {

				FVCL_ASSERT(TreeView_Fnct->Items != NULL);
				int ItemsCount = TreeView_Fnct->Items->Count;
				for (i = 0; i < ItemsCount; i++) {
					FVCL_ASSERT(TreeView_Fnct->Items->Item[i] != NULL);
					if (TreeView_Fnct->Items->Item[i]->Data == ItemSelected->Data) {
						TreeView_Fnct->Items->Item[i]->Selected = true;
						break;
					}
				}

			}

			PaintBox_Alt->Enabled = true;
			PaintBox_Shift->Enabled = true;
			PaintBox_Ctrl->Enabled = true;
			ComboBox_ToucheRacc->Enabled = true;

			TShortCut ShortCut = TextToShortCut(ItemSelected->SubItems->Strings[1]);
			UpdateGroupBox(ShortCut);
			UpdateHotKey(ShortCut);

		}
		else {

		  if (!bNotChangeTreeView) {

				TreeView_Fnct->Selected = NULL;

			}

			PaintBox_Alt->Enabled = false;
			PaintBox_Shift->Enabled = false;
			PaintBox_Ctrl->Enabled = false;
			ComboBox_ToucheRacc->Enabled = false;

			PaintBox_Alt->Tag = FALSE;
			PaintBox_Shift->Tag = FALSE;
			PaintBox_Ctrl->Tag = FALSE;
			ComboBox_ToucheRacc->ItemIndex = -1;

			THKModifiers HKModifiers;
			HKModifiers.Clear();
			HotKey_SaisieDirecte->Modifiers = HKModifiers;
			HotKey_SaisieDirecte->HotKey = 0;
			HotKey_SaisieDirecte->Enabled = false;

			PaintBox_Alt->Invalidate();
			PaintBox_Shift->Invalidate();
			PaintBox_Ctrl->Invalidate();

		}

		bNotChangeListView = false;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::ListView_RaccourcisKeyDown(
      TObject *Sender, WORD Key, TShiftState Shift) {

  if (Key == VK_DELETE) Button_SupprimerClick(Sender);

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::Button_SupprimerClick(TObject *Sender) {
  int i;


  i = ListView_Raccourcis->ItemIndex;
  if (i != -1) {
    ListView_Raccourcis->Items->Delete(i);
    bModifs = true;
  }
  if (i >= ListView_Raccourcis->Items->Count) i--;
  ListView_Raccourcis->ItemIndex = i;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::Button_MAZClick(TObject *Sender) {
	int i;

  ListView_Raccourcis->Items->Clear();
	for (i = 0; i < (int) Commandes.size(); i++) {
	  Commandes[i].ShortCut = 0;
	}
  bModifs = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::Button_DefaultClick(TObject *Sender) {
	int i;

	for (i = 0; i < (int) Commandes.size(); i++) {

		if (Commandes[i].Action->Name == _T("Action_AddBookmark")) Commandes[i].ShortCut = 16450;
		else if (Commandes[i].Action->Name == _T("Action_AddBreakpoint")) Commandes[i].ShortCut = 120;
		else if (Commandes[i].Action->Name == _T("Action_Autocompletion")) Commandes[i].ShortCut = 113;
		else if (Commandes[i].Action->Name == _T("Action_BuildCurrent")) Commandes[i].ShortCut = 118;
		else if (Commandes[i].Action->Name == _T("Action_CopyToClipboard")) Commandes[i].ShortCut = 16451;
		else if (Commandes[i].Action->Name == _T("Action_CutToClipboard")) Commandes[i].ShortCut = 16472;
		else if (Commandes[i].Action->Name == _T("Action_Debug")) Commandes[i].ShortCut = 116;
		else if (Commandes[i].Action->Name == _T("Action_DelBookmark")) Commandes[i].ShortCut = 24642;
		else if (Commandes[i].Action->Name == _T("Action_DelBreakpoint")) Commandes[i].ShortCut = 8312;
		else if (Commandes[i].Action->Name == _T("Action_ClearBreakpoints")) Commandes[i].ShortCut = 24696;
		else if (Commandes[i].Action->Name == _T("Action_Find")) Commandes[i].ShortCut = 16454;
		else if (Commandes[i].Action->Name == _T("Action_FindInFiles")) Commandes[i].ShortCut = 24646;
		else if (Commandes[i].Action->Name == _T("Action_FindNext")) Commandes[i].ShortCut = 114;
		else if (Commandes[i].Action->Name == _T("Action_FindPrevious")) Commandes[i].ShortCut = 8306;
		else if (Commandes[i].Action->Name == _T("Action_FindSelNext")) Commandes[i].ShortCut = 16498;
		else if (Commandes[i].Action->Name == _T("Action_FindSelPrevious")) Commandes[i].ShortCut = 24690;
		else if (Commandes[i].Action->Name == _T("Action_Goto")) Commandes[i].ShortCut = 16455;
		else if (Commandes[i].Action->Name == _T("Action_LowerCase")) Commandes[i].ShortCut = 16469;
		else if (Commandes[i].Action->Name == _T("Action_NewFile")) Commandes[i].ShortCut = 16462;
		else if (Commandes[i].Action->Name == _T("Action_FreeVCLHelp")) Commandes[i].ShortCut = 112;
		else if (Commandes[i].Action->Name == _T("Action_OpenFile")) Commandes[i].ShortCut = 16463;
		else if (Commandes[i].Action->Name == _T("Action_PasteFromClipboard")) Commandes[i].ShortCut = 16470;
		else if (Commandes[i].Action->Name == _T("Action_PlayMacro")) Commandes[i].ShortCut = 24656;
		else if (Commandes[i].Action->Name == _T("Action_Quit")) Commandes[i].ShortCut = 16465;
		else if (Commandes[i].Action->Name == _T("Action_Rebuild")) Commandes[i].ShortCut = 8310;
		else if (Commandes[i].Action->Name == _T("Action_RecMacro")) Commandes[i].ShortCut = 16464;
		else if (Commandes[i].Action->Name == _T("Action_Redo")) Commandes[i].ShortCut = 16473;
		else if (Commandes[i].Action->Name == _T("Action_RefNext")) Commandes[i].ShortCut = 115;
		else if (Commandes[i].Action->Name == _T("Action_RefPrevious")) Commandes[i].ShortCut = 8307;
		else if (Commandes[i].Action->Name == _T("Action_Replace")) Commandes[i].ShortCut = 16466;
		else if (Commandes[i].Action->Name == _T("Action_ReplaceInFiles")) Commandes[i].ShortCut = 24658;
		else if (Commandes[i].Action->Name == _T("Action_RunCurrent")) Commandes[i].ShortCut = 16500;
		else if (Commandes[i].Action->Name == _T("Action_SelectAll")) Commandes[i].ShortCut = 16449;
		else if (Commandes[i].Action->Name == _T("Action_StepByStepNext")) Commandes[i].ShortCut = 121;
		else if (Commandes[i].Action->Name == _T("Action_StepByStepIn")) Commandes[i].ShortCut = 122;
		else if (Commandes[i].Action->Name == _T("Action_StepByStepOut")) Commandes[i].ShortCut = 8314;
		else if (Commandes[i].Action->Name == _T("Action_StopDebug")) Commandes[i].ShortCut = 8308;
		else if (Commandes[i].Action->Name == _T("Action_SaveAllFiles")) Commandes[i].ShortCut = 24659;
		else if (Commandes[i].Action->Name == _T("Action_SaveFile")) Commandes[i].ShortCut = 16467;
		else if (Commandes[i].Action->Name == _T("Action_Undo")) Commandes[i].ShortCut = 16474;
		else if (Commandes[i].Action->Name == _T("Action_UpperCase")) Commandes[i].ShortCut = 24661;
		else Commandes[i].ShortCut = 0;

	}

  InitRaccourcis();

	bModifs = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::RaccourciChange(TObject *Sender) {
  TListItem * ListItem;
  AnsiString asShortCut;


	if (!bNotChangeListView) {

		asShortCut = TexteRaccourci();
		ListItem = ListView_Raccourcis->Selected;
		if (ListItem) ListItem->SubItems->Set_Strings(1, asShortCut);
		ListView_Raccourcis->AlphaSort();
		UpdateHotKey(TextToShortCut(asShortCut));
		bModifs = true;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::HotKey_SaisieDirecteChange(TObject *Sender) {

  if (!bNotChangeHotKey) {

		bNotChangeHotKey = true;

    TShortCut ShortCut = HotKey_SaisieDirecte->HotKey;
    UpdateGroupBox(ShortCut);
    RaccourciChange(this);

		bNotChangeHotKey = false;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::Button_OkClick(TObject *Sender) {
  TListItem * ListItem;
  TListItem * ListItem2;

  int i, j;
  bool Ok = true;
  AnsiString asError;


  if (bModifs) {
		for (i = 0; i < (int) Commandes.size(); i++) {
			Commandes[i].ShortCut = 0;
		}
    for (i = 0; i < ListView_Raccourcis->Items->Count && Ok; i++) {
      ListItem = ListView_Raccourcis->Items->Item[i];
      if (!ListItem->SubItems->Strings[1].IsEmpty()) {
        // Test doublons
        for (j = 0; j < i; j++) {
          ListItem2 = ListView_Raccourcis->Items->Item[j];
          if (ListItem->SubItems->Strings[1] == ListItem2->SubItems->Strings[1]) {
						asError.sprintf(
								_TT("Attention !\nLes commandes \"%s\" et \"%s\" ont le même raccourci"),
								(LPCTSTR) ((AnsiString) ListItem2->Caption),
								(LPCTSTR) ((AnsiString) ListItem->Caption));
						::DisplayError(asError, _TT("Erreur"), _T(""), MB_OK | MB_ICONWARNING);
            ListView_Raccourcis->ItemIndex = j;
            Ok = false;
            break;
          }
        }
        // Mémorisation dans Commandes
				for (j = 0; j < (int) Commandes.size(); j++) {
					if (Commandes[j].Commande == (int) (size_t) (void *) ListItem->Data) {
						Commandes[j].ShortCut = TextToShortCut(ListItem->SubItems->Strings[1]);
						break;
					}
				}
      }
    }
    if (Ok) {
      // Mémorisation dans la config et dans le menu
			for (j = 0; j < (int) Commandes.size(); j++) {
				Commandes[j].Action->ShortCut = Commandes[j].ShortCut;
				Config->SetShortCut(Commandes[j].Action->Name, Commandes[j].ShortCut);
			}
      bModifs = false;
    }
  }
  if (Ok) ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIERACC);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::FormClose(TObject *Sender,
      TCloseAction &Action) {
  int Rep;


  if (FModalResult != mrCancel) {

    if (bModifs) {
			Rep = DisplayError(_TT("Voulez-vous enregistrer les modifications ?"),
												 _TT("Raccourcis clavier"),
												 _T("SAVE_CHANGES"),
												 MB_YESNOCANCEL | MB_ICONQUESTION);
      if (Rep == IDYES) {
        Button_OkClick(Sender);
        bModifs = false;
      }
      else if (Rep == IDNO) {
        bModifs = false;
      }
    }
    if (bModifs) Action = caNone;

  }

}


//---------------------------------------------------------------------------
//
//                              UTILITAIRES
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
AnsiString FASTCALL TForm_SaisieRacc::TexteRaccourci(void) {
  AnsiString asShortCut;
	TShortCut ShortCut;
	int ItemIndex;


	ItemIndex = ComboBox_ToucheRacc->ItemIndex;
	if (ItemIndex != -1) {
		ShortCut = 0;
    if (PaintBox_Alt->Tag) ShortCut |= 0x8000;
    if (PaintBox_Ctrl->Tag) ShortCut |= 0x4000;
    if (PaintBox_Shift->Tag) ShortCut |= 0x2000;
		ShortCut |= CorrespKey[ItemIndex].Key;
		asShortCut = ShortCutToText(ShortCut);
	}

  return asShortCut;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::UpdateGroupBox(TShortCut ShortCut) {

	if (!bNotChangeGroupBox) {

		bNotChangeGroupBox = true;

		WORD Key;
		TShiftState Shift;
		ShortCutToKey(ShortCut, Key, Shift);
		PaintBox_Alt->Tag = (BOOL) (Shift.Contains(ssAlt));
		PaintBox_Shift->Tag = (BOOL) (Shift.Contains(ssShift));
		PaintBox_Ctrl->Tag = (BOOL) (Shift.Contains(ssCtrl));

		int i;
		int Index = -1;
		for (i = 0; i < (int) NBCORRESPKEY; i++) {
			if (Key == CorrespKey[i].Key) {
				Index = i;
				break;
			}
		}
		ComboBox_ToucheRacc->ItemIndex = Index;

		PaintBox_Alt->Invalidate();
		PaintBox_Shift->Invalidate();
		PaintBox_Ctrl->Invalidate();

		bNotChangeGroupBox = false;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::UpdateHotKey(TShortCut ShortCut) {

	if (!bNotChangeHotKey) {

		bNotChangeHotKey = true;

		HotKey_SaisieDirecte->HotKey = ShortCut;
		HotKey_SaisieDirecte->Enabled = true;

		bNotChangeHotKey = false;

	}

}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieRacc::ListView_RaccourcisCompare(
      TObject *Sender, TListItem *Item1, TListItem *Item2, int Data,
      int &Compare) {
  TShortCut ShortCut1, ShortCut2;
  WORD key1, key2;


  ShortCut1 = 0;
  ShortCut2 = 0;

  if (Item1->SubItems->Count >= 1) ShortCut1 = TextToShortCut(Item1->SubItems->Strings[1]);
  if (Item2->SubItems->Count >= 1) ShortCut2 = TextToShortCut(Item2->SubItems->Strings[1]);
  key1 = ShortCut1 & 0xFF;
  key2 = ShortCut2 & 0xFF;

  if (key1 < key2) Compare = -1;
  else if (key1 > key2) Compare = 1;

  else if (ShortCut1 < ShortCut2) Compare = -1;
  else if (ShortCut1 > ShortCut2) Compare = 1;

  else Compare = 0;
}

//---------------------------------------------------------------------------
