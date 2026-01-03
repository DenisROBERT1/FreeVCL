//-----------------------------------------------------------------------------
//! @file TListView.cpp
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

#include <FreeVcl.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TListView
//---------------------------------------------------------------------------

TListView::TListView(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TListView");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  FWindowStyle |= LVS_REPORT | LVS_SINGLESEL | LVS_EDITLABELS;
  FExWindowStyle = WS_EX_CLIENTEDGE;
  FColumns = new TListColumns();
  FItems = new TListItems(this);
  FSelected = NULL;
  FItemIndex = -1;
  FTabStop = true;
	FAllocBy = 16;

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         WC_LISTVIEW, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);
    FColumns->Handle = FHandle;
    FItems->Handle = FHandle;

    InitStdControl();
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TListView::~TListView(void) {
  delete FColumns;
  delete FItems;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::ActionChange(TObject* Sender, bool CheckDefaults) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::AddItem(AnsiString Item, TObject* AObject) {
  TListItem* NewListItem;

  NewListItem = FItems->Add(Item);
  NewListItem->Data = AObject;
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::AlphaSort(void) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::Arrange(TListArrangement Code) {
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::CanChange(TListItem* Item, int Change) {
  bool AllowChange = true;
  if (!FComponentState.Contains(csLoading) && OnChanging) {
    if (Change & LVIF_TEXT) {
      OnChanging(this, Item, ctText, AllowChange);
    }
    if (AllowChange && (Change & LVIF_IMAGE)) {
      OnChanging(this, Item, ctImage, AllowChange);
    }
    if (AllowChange && (Change & LVIF_STATE)) {
      OnChanging(this, Item, ctState, AllowChange);
    }
  }
  return AllowChange;
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::CanEdit(TListItem* Item) {
  bool AllowEdit = true;
  OnEditing(this, Item, AllowEdit);
  return AllowEdit;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::Change(TListItem* ListItem, int Change) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::ChangeScale(int M, int D) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::Clear(void) {
  FItems->Clear();
}

//---------------------------------------------------------------------------
void FASTCALL TListView::ClearSelection(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::ColClick(TListColumn* Column) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::ColRightClick(TListColumn* Column, const TPoint &Point) {
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::ColumnsShowing(void) {
  return false;
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListView::CreateListItem(void) {
  return NULL;
}

//---------------------------------------------------------------------------
TListItems* FASTCALL TListView::CreateListItems(void) {
  return NULL;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::CreateParams(TCreateParams &Params) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::CreateWnd(void) {
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::CustomDraw(const TRect &ARect, TCustomDrawStage Stage) {
  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::CustomDrawItem(TListItem* Item, TCustomDrawStates State, TCustomDrawStage Stage) {
  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::CustomDrawSubItem(TListItem* Item, int SubItem, TCustomDrawStates State, TCustomDrawStage Stage) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::Delete(TListItem* Item) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::DeleteSelected(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::DestroyWnd(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::DoEndDrag(TObject* Target, int X, int Y) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::DoInfoTip(TListItem* Item, AnsiString &InfoTip) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::DoStartDrag(TDragObject* &DragObject) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::DrawItem(TListItem* Item, const TRect &Rect, TOwnerDrawStates States) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::Edit(const LVITEM &Item) {
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListView::FindCaption(int StartIndex, AnsiString Value, bool Partial, bool Inclusive, bool Wrap) {
  return NULL;
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListView::FindData(int StartIndex, void * Value, bool Inclusive, bool Wrap) {
  return NULL;
}

//---------------------------------------------------------------------------
int FASTCALL TListView::GetCount(void) const {
  return 0;
}

//---------------------------------------------------------------------------
THitTests FASTCALL TListView::GetHitTestInfoAt(int X, int Y) {
  THitTests HitTests;

  HitTests.Clear();

  return HitTests;
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListView::GetItemAt(int X, int Y) {
  return NULL;
}

//---------------------------------------------------------------------------
int FASTCALL TListView::GetItemIndex(TListItem* Value) {
  return 0;
}

//---------------------------------------------------------------------------
int FASTCALL TListView::GetItemIndex(void) {
  return 0;
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListView::GetNearestItem(const TPoint &Point, TSearchDirection Direction) {
  return NULL;
}

//---------------------------------------------------------------------------
TListItem* FASTCALL TListView::GetNextItem(TListItem* StartItem, TSearchDirection Direction, TItemStates States) {
  return NULL;
}

//---------------------------------------------------------------------------
AnsiString FASTCALL TListView::GetSearchString(void) {
  return _T("");
}

//---------------------------------------------------------------------------
int FASTCALL TListView::GetSelCount(void) {
  return 0;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::InsertItem(TListItem* Item) {
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::IsCustomDrawn(TCustomDrawTarget Target, TCustomDrawStage Stage) {
  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::IsEditing(void) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y) {
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::OwnerDataFetch(TListItem* Item, TItemRequest Request) {
  return false;
}

//---------------------------------------------------------------------------
int FASTCALL TListView::OwnerDataFind(TItemFind Find, const AnsiString FindString, const TPoint &FindPosition, void * FindData, int StartIndex, TSearchDirection Direction, bool Wrap) {
  return 0;
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::OwnerDataHint(int StartIndex, int EndIndex) {
  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TListView::OwnerDataStateChange(int StartIndex, int EndIndex, TItemStates OldState, TItemStates NewState) {
  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::Scroll(int DX, int DY) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::SelectAll(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::SetItemIndex(int Value) {
}

//---------------------------------------------------------------------------
int FASTCALL TListView::StringWidth(AnsiString S) {
  return 0;
}

//---------------------------------------------------------------------------
void FASTCALL TListView::UpdateColumn(int AnIndex) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::UpdateColumns(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TListView::UpdateItems(int FirstIndex, int LastIndex) {
}

//---------------------------------------------------------------------------
int TListView::ProcessNotify(LPNMHDR pnmh) {
  NMLISTVIEW *pnmv;
  NMLVDISPINFO *pdi;
  int OldIndex, NewIndex, NewItemIndex;


  switch (pnmh->code) {
	case TTN_SHOW:
		if (!FComponentState.Contains(csLoading)) OnHint(this);
		return TRUE;
  case NM_CLICK:
		if (!FComponentState.Contains(csLoading)) {
			if (FAction) FAction->Execute();
			OnClick(this);
		}
    return TRUE;
  case LVN_BEGINLABELEDIT:
    pdi = (NMLVDISPINFO *) pnmh;
    if (CanEdit((TListItem *) pdi->item.lParam)) return FALSE;
    return TRUE;
  case LVN_ENDLABELEDIT:
    pdi = (NMLVDISPINFO *) pnmh;
    if ((pdi->item.mask & LVIF_TEXT) != 0) {
      if (!FComponentState.Contains(csLoading) && OnEdited) {
        TListItem *ListItem;
        AnsiString asText;
        ListItem = FItems->Item[pdi->item.iItem];
        asText = pdi->item.pszText;
        if (ListItem) ListItem->Caption = asText;
        OnEdited(this, ListItem, asText);
      }
    }
    return TRUE;
  case LVN_ITEMCHANGING:
    pnmv = (NMLISTVIEW *) pnmh;
    NewIndex = pnmv->iItem;
    if (FItemIndex != NewIndex &&
        (pnmv->uOldState & LVIS_SELECTED) == 0 &&
        (pnmv->uNewState & LVIS_SELECTED) != 0) {
      if (CanChange((TListItem *) pnmv->lParam, pnmv->uChanged)) return FALSE;
    }
    return TRUE;
  case LVN_ITEMCHANGED:
    pnmv = (NMLISTVIEW *) pnmh;
    // Quel bordel !
    OldIndex = FItemIndex;
    NewIndex = pnmv->iItem;
    NewItemIndex = FItemIndex;
    if (OldIndex != NewIndex ||
        (pnmv->uNewState & LVIS_SELECTED) != (pnmv->uOldState & LVIS_SELECTED)) {
      if (OldIndex != -1) {
				TListItem *ItemSelected = FItems->Item[OldIndex];
				if (ItemSelected) {
					ItemSelected->Selected = false;
				}
      }
      if ((pnmv->uNewState & LVIS_SELECTED) != 0) {
        if (NewIndex != -1) {
          FSelected = FItems->Item[NewIndex];
          FSelected->Selected = true;
        }
        else {
          FSelected = NULL;
        }
        NewItemIndex = NewIndex;
      }
      else {
        FSelected = NULL;
        NewItemIndex = -1;
      }
    }
    if (FItemIndex != NewItemIndex) {
      if (!FComponentState.Contains(csLoading) && OnSelectItem) {
        if (FItemIndex != -1) {
          OnSelectItem(this, FItems->Item[FItemIndex], false);
        }
        if (NewItemIndex != -1) {
          OnSelectItem(this, FItems->Item[NewItemIndex], true);
        }
      }
      FItemIndex = NewItemIndex;
      if (!FComponentState.Contains(csLoading)) {
        OnChange(this);
      }
    }
    return TRUE;
  }

  return FALSE;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété AllocBy
//---------------------------------------------------------------------------

int TListView::Get_AllocBy(void) {
  return FAllocBy;
}

bool TListView::Set_AllocBy(int NewAllocBy) {
  if (FAllocBy != NewAllocBy) {
    FAllocBy = NewAllocBy;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BorderStyle
//---------------------------------------------------------------------------

TBorderStyle TListView::Get_BorderStyle(void) {
  return FBorderStyle;
}

bool TListView::Set_BorderStyle(TBorderStyle NewBorderStyle) {
  if (FBorderStyle != NewBorderStyle) {
    FBorderStyle = NewBorderStyle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété BoundingRect
//---------------------------------------------------------------------------

TRect TListView::Get_BoundingRect(void) {
  return FBoundingRect;
}

bool TListView::Set_BoundingRect(TRect NewBoundingRect) {
  if (FBoundingRect != NewBoundingRect) {
    FBoundingRect = NewBoundingRect;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CheckBoxes
//---------------------------------------------------------------------------

bool TListView::Get_CheckBoxes(void) {
  if (FExWindowStyle & LVS_EX_CHECKBOXES) return true;
  else return false;
}

bool TListView::Set_CheckBoxes(bool NewCheckBoxes) {

  if (NewCheckBoxes) FExWindowStyle |= LVS_EX_CHECKBOXES;
  else FExWindowStyle &= ~LVS_EX_CHECKBOXES;
  if (FHandle) {
    SendMessage(FHandle, LVM_SETEXTENDEDLISTVIEWSTYLE,
                (WPARAM) LVS_EX_CHECKBOXES,
                (LPARAM) FExWindowStyle);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Columns
//---------------------------------------------------------------------------

TListColumns *TListView::Get_Columns(void) {
  return FColumns;
}

bool TListView::Set_Columns(TListColumns *NewColumns) {

  if (FColumns != NewColumns) {
    delete FColumns;
    FColumns = NewColumns;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ColumnClick
//---------------------------------------------------------------------------

bool TListView::Get_ColumnClick(void) {
  return FColumnClick;
}

bool TListView::Set_ColumnClick(bool NewColumnClick) {
  if (FColumnClick != NewColumnClick) {
    FColumnClick = NewColumnClick;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DropTarget
//---------------------------------------------------------------------------

TListItem *TListView::Get_DropTarget(void) {
  return FDropTarget;
}

bool TListView::Set_DropTarget(TListItem *NewDropTarget) {
  if (FDropTarget != NewDropTarget) {
    FDropTarget = NewDropTarget;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FlatScrollBars
//---------------------------------------------------------------------------

bool TListView::Get_FlatScrollBars(void) {
  return FFlatScrollBars;
}

bool TListView::Set_FlatScrollBars(bool NewFlatScrollBars) {
  if (FFlatScrollBars != NewFlatScrollBars) {
    FFlatScrollBars = NewFlatScrollBars;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FullDrag
//---------------------------------------------------------------------------

bool TListView::Get_FullDrag(void) {
  return FFullDrag;
}

bool TListView::Set_FullDrag(bool NewFullDrag) {
  if (FFullDrag != NewFullDrag) {
    FFullDrag = NewFullDrag;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété GridLines
//---------------------------------------------------------------------------

bool TListView::Get_GridLines(void) {
  if (FExWindowStyle & LVS_EX_GRIDLINES) return true;
  else return false;
}

bool TListView::Set_GridLines(bool NewGridLines) {

  if (NewGridLines) FExWindowStyle |= LVS_EX_GRIDLINES;
  else FExWindowStyle &= ~LVS_EX_GRIDLINES;
  if (FHandle) {
    SendMessage(FHandle, LVM_SETEXTENDEDLISTVIEWSTYLE,
                (WPARAM) LVS_EX_GRIDLINES,
                (LPARAM) FExWindowStyle);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HideSelection
//---------------------------------------------------------------------------

bool TListView::Get_HideSelection(void) {
  if (FWindowStyle & LVS_SHOWSELALWAYS) return false;
  else return true;
}

bool TListView::Set_HideSelection(bool NewHideSelection) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewHideSelection) NewWindowStyle &= ~LVS_SHOWSELALWAYS;
  else NewWindowStyle |= LVS_SHOWSELALWAYS;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HotTrack
//---------------------------------------------------------------------------

bool TListView::Get_HotTrack(void) {
  return FHotTrack;
}

bool TListView::Set_HotTrack(bool NewHotTrack) {
  if (FHotTrack != NewHotTrack) {
    FHotTrack = NewHotTrack;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HotTrackStyles
//---------------------------------------------------------------------------

TListHotTrackStyles TListView::Get_HotTrackStyles(void) {
  return FHotTrackStyles;
}

bool TListView::Set_HotTrackStyles(TListHotTrackStyles NewHotTrackStyles) {
  if (FHotTrackStyles != NewHotTrackStyles) {
    FHotTrackStyles = NewHotTrackStyles;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété HoverTime
//---------------------------------------------------------------------------

int TListView::Get_HoverTime(void) {
  return FHoverTime;
}

bool TListView::Set_HoverTime(int NewHoverTime) {
  if (FHoverTime != NewHoverTime) {
    FHoverTime = NewHoverTime;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Items
//---------------------------------------------------------------------------

TListItems *TListView::Get_Items(void) {
  return FItems;
}

bool TListView::Set_Items(TListItems *NewItems) {

  if (FItems != NewItems) {
    delete FItems;
    FItems = NewItems;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemFocused
//---------------------------------------------------------------------------

TListItem *TListView::Get_ItemFocused(void) {
  return FItemFocused;
}

bool TListView::Set_ItemFocused(TListItem *NewItemFocused) {
  if (FItemFocused != NewItemFocused) {
    FItemFocused = NewItemFocused;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ItemIndex
//---------------------------------------------------------------------------

int TListView::Get_ItemIndex(void) {
  return FItemIndex;
}

bool TListView::Set_ItemIndex(int NewItemIndex) {

	if (NewItemIndex < -1) NewItemIndex = -1;
	if (NewItemIndex >= FItems->Count) NewItemIndex = FItems->Count - 1;

	if (FItemIndex != NewItemIndex) {
    if (FSelected) FSelected->Selected = false;
    FItemIndex = NewItemIndex;
    if (NewItemIndex != -1) {
      FSelected = FItems->Item[NewItemIndex];
      FSelected->Selected = true;
    }
    else {
      FSelected = NULL;
    }
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LargeImages
//---------------------------------------------------------------------------

TImageList *TListView::Get_LargeImages(void) {
  return FLargeImages;
}

bool TListView::Set_LargeImages(TImageList *NewLargeImages) {
  if (FLargeImages != NewLargeImages) {
    FLargeImages = NewLargeImages;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MultiSelect
//---------------------------------------------------------------------------

bool TListView::Get_MultiSelect(void) {
  if (FWindowStyle & LVS_SINGLESEL) return false;
  else return true;
}

bool TListView::Set_MultiSelect(bool NewMultiSelect) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewMultiSelect) NewWindowStyle &= ~LVS_SINGLESEL;
  else NewWindowStyle |= LVS_SINGLESEL;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété OwnerData
//---------------------------------------------------------------------------

bool TListView::Get_OwnerData(void) {
  return FOwnerData;
}

bool TListView::Set_OwnerData(bool NewOwnerData) {
  if (FOwnerData != NewOwnerData) {
    FOwnerData = NewOwnerData;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété OwnerDraw
//---------------------------------------------------------------------------

bool TListView::Get_OwnerDraw(void) {
  return FOwnerDraw;
}

bool TListView::Set_OwnerDraw(bool NewOwnerDraw) {
  if (FOwnerDraw != NewOwnerDraw) {
    FOwnerDraw = NewOwnerDraw;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ReadOnly
//---------------------------------------------------------------------------

bool TListView::Get_ReadOnly(void) {
  return ((FWindowStyle & LVS_EDITLABELS) == 0);
}

bool TListView::Set_ReadOnly(bool NewReadOnly) {
  DWORD NewWindowStyle;

  NewWindowStyle = FWindowStyle;
  if (NewReadOnly) NewWindowStyle &= (DWORD) ~(LVS_EDITLABELS);
  else NewWindowStyle |= LVS_EDITLABELS;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété RowSelect
//---------------------------------------------------------------------------

bool TListView::Get_RowSelect(void) {
  if (FExWindowStyle & LVS_EX_FULLROWSELECT) return true;
  else return false;
}

bool TListView::Set_RowSelect(bool NewRowSelect) {

  if (NewRowSelect) FExWindowStyle |= LVS_EX_FULLROWSELECT;
  else FExWindowStyle &= ~LVS_EX_FULLROWSELECT;
  if (FHandle) {
    SendMessage(FHandle, LVM_SETEXTENDEDLISTVIEWSTYLE,
                (WPARAM) LVS_EX_FULLROWSELECT,
                (LPARAM) FExWindowStyle);
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SelCount
//---------------------------------------------------------------------------

int TListView::Get_SelCount(void) {
  return FSelCount;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Selected
//---------------------------------------------------------------------------

TListItem *TListView::Get_Selected(void) {
  return FSelected;
}

bool TListView::Set_Selected(TListItem *NewSelected) {
  if (FSelected != NewSelected) {
    FSelected = NewSelected;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowColumnHeaders
//---------------------------------------------------------------------------

bool TListView::Get_ShowColumnHeaders(void) {
  return FShowColumnHeaders;
}

bool TListView::Set_ShowColumnHeaders(bool NewShowColumnHeaders) {
  if (FShowColumnHeaders != NewShowColumnHeaders) {
    FShowColumnHeaders = NewShowColumnHeaders;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ShowWorkAreas
//---------------------------------------------------------------------------

bool TListView::Get_ShowWorkAreas(void) {
  return FShowWorkAreas;
}

bool TListView::Set_ShowWorkAreas(bool NewShowWorkAreas) {
  if (FShowWorkAreas != NewShowWorkAreas) {
    FShowWorkAreas = NewShowWorkAreas;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SmallImages
//---------------------------------------------------------------------------

TImageList *TListView::Get_SmallImages(void) {
  return FSmallImages;
}

bool TListView::Set_SmallImages(TImageList *NewSmallImages) {
  if (FSmallImages != NewSmallImages) {
    FSmallImages = NewSmallImages;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété SortType
//---------------------------------------------------------------------------

TSortType TListView::Get_SortType(void) {
  return FSortType;
}

bool TListView::Set_SortType(TSortType NewSortType) {
  if (FSortType != NewSortType) {
    FSortType = NewSortType;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété StateImages
//---------------------------------------------------------------------------

TImageList *TListView::Get_StateImages(void) {
  return FStateImages;
}

bool TListView::Set_StateImages(TImageList *NewStateImages) {
  if (FStateImages != NewStateImages) {
    FStateImages = NewStateImages;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TopIndex
//---------------------------------------------------------------------------

int TListView::Get_TopIndex(void) {
	if (FHandle) {
		return ListView_GetTopIndex(FHandle);
	}

  return -1;
}

bool TListView::Set_TopIndex(int NewTopIndex) {
	POINT pt;

	ListView_GetItemPosition(FHandle, NewTopIndex, &pt);
	ListView_Scroll(FHandle, 0, pt.y - 16);

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété TopItem
//---------------------------------------------------------------------------

TListItem *TListView::Get_TopItem(void) {
	if (FHandle) {
		int TopIndex = ListView_GetTopIndex(FHandle);
		if (TopIndex < FItems->Count) return FItems->Item[TopIndex];
	}

  return NULL;
}

bool TListView::Set_TopItem(TListItem *NewTopItem) {
	Set_TopIndex(NewTopItem->NumItem);

	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ViewOrigin
//---------------------------------------------------------------------------

TPoint TListView::Get_ViewOrigin(void) {
  return FViewOrigin;
}

bool TListView::Set_ViewOrigin(TPoint NewViewOrigin) {
  if (FViewOrigin != NewViewOrigin) {
    FViewOrigin = NewViewOrigin;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ViewStyle
//---------------------------------------------------------------------------

TViewStyle TListView::Get_ViewStyle(void) {
  return FViewStyle;
}

bool TListView::Set_ViewStyle(TViewStyle NewViewStyle) {
  if (FViewStyle != NewViewStyle) {
    FViewStyle = NewViewStyle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété VisibleRowCount
//---------------------------------------------------------------------------

int TListView::Get_VisibleRowCount(void) {
	return ListView_GetCountPerPage(FHandle);
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TListView::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("BorderStyle"));
  ListProperties->Add(_T("CheckBoxes"));
  ListProperties->Add(_T("GridLines"));
  ListProperties->Add(_T("HideSelection"));
  ListProperties->Add(_T("RowSelect"));
  ListProperties->Add(_T("MultiSelect"));
  ListProperties->Add(_T("ReadOnly"));
  ListProperties->Add(_T("ItemIndex"));
  ListProperties->Add(_T("Columns"));
  ListProperties->Add(_T("OnChange"));
  ListProperties->Add(_T("OnChanging"));
  ListProperties->Add(_T("OnEdited"));
  ListProperties->Add(_T("OnEditing"));
  ListProperties->Add(_T("OnSelectItem"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TListView::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("BorderStyle")) {
    *asInfos = _T("bsNone\nbsSingle\nbsSizeable\nbsDialog");
    return tpChoice;
  }
  if (asProperty == _T("CheckBoxes")) {
    return tpBool;
  }
  if (asProperty == _T("GridLines")) {
    return tpBool;
  }
  if (asProperty == _T("HideSelection")) {
    return tpBool;
  }
  if (asProperty == _T("RowSelect")) {
    return tpBool;
  }
  if (asProperty == _T("MultiSelect")) {
    return tpBool;
  }
  if (asProperty == _T("ReadOnly")) {
    return tpBool;
  }
  if (asProperty == _T("ItemIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("Columns")) {
    return tpNull;
  }
  if (asProperty == _T("OnChange")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnChanging")) {
    *asInfos = _T("(TObject *Sender, TListItem *Item, TItemChange Type, bool &AllowChanging)");
    return tpEvent;
  }
  if (asProperty == _T("OnEdited")) {
    *asInfos = _T("(TObject *Sender, TListItem *Item, AnsiString Text)");
    return tpEvent;
  }
  if (asProperty == _T("OnEditing")) {
    *asInfos = _T("(TObject *Sender, TListItem *Item, bool &AllowEdit)");
    return tpEvent;
  }
  if (asProperty == _T("OnSelectItem")) {
    *asInfos = _T("(TObject *Sender, TListItem *Item, bool Selected)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TListView::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("BorderStyle")) {
    return _T("bsSingle");
  }
  if (asProperty == _T("CheckBoxes")) {
    return _T("False");
  }
  if (asProperty == _T("GridLines")) {
    return _T("False");
  }
  if (asProperty == _T("HideSelection")) {
    return _T("True");
  }
  if (asProperty == _T("RowSelect")) {
    return _T("False");
  }
  if (asProperty == _T("MultiSelect")) {
    return _T("False");
  }
  if (asProperty == _T("ReadOnly")) {
    return _T("False");
  }
  if (asProperty == _T("ItemIndex")) {
    return _T("-1");
  }
  if (asProperty == _T("Columns")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return _T("");
  }
  if (asProperty == _T("OnChanging")) {
    return _T("");
  }
  if (asProperty == _T("OnEdited")) {
    return _T("");
  }
  if (asProperty == _T("OnEditing")) {
    return _T("");
  }
  if (asProperty == _T("OnSelectItem")) {
    return _T("");
  }
  if (asProperty == _T("TabStop")) {
    return _T("True");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TListView::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("BorderStyle")) {
    TFormBorderStyle BorderStyle = Get_BorderStyle();
    if (BorderStyle == bsNone) return _T("bsNone");
    if (BorderStyle == bsSingle) return _T("bsSingle");
    if (BorderStyle == bsSizeable) return _T("bsSizeable");
    if (BorderStyle == bsDialog) return _T("bsDialog");
    return _T("bsNone");
  }
  if (asProperty == _T("CheckBoxes")) {
    if (Get_CheckBoxes()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("GridLines")) {
    if (Get_GridLines()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("HideSelection")) {
    if (Get_HideSelection()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("RowSelect")) {
    if (Get_RowSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("MultiSelect")) {
    if (Get_MultiSelect()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ReadOnly")) {
    if (Get_ReadOnly()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ItemIndex")) {
    return IntToStr(Get_ItemIndex());
  }
  if (asProperty == _T("Columns")) {
    return _T("");
  }
  if (asProperty == _T("OnChange")) {
    return OnChange.AsString();
  }
  if (asProperty == _T("OnChanging")) {
    return OnChanging.AsString();
  }
  if (asProperty == _T("OnEdited")) {
    return OnEdited.AsString();
  }
  if (asProperty == _T("OnEditing")) {
    return OnEditing.AsString();
  }
  if (asProperty == _T("OnSelectItem")) {
    return OnSelectItem.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TListView::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  AnsiString asLigne;
  AnsiString asMot;
  AnsiString asValue2;
  AnsiString asCaption;
  int WidthColumn = 50;
  int Cur, Cur2;


  if (asProperty == _T("BorderStyle")) {
    if (asValue == _T("bsNone")) Set_BorderStyle(bsNone);
    else if (asValue == _T("bsSingle")) Set_BorderStyle(bsSingle);
    else if (asValue == _T("bsSizeable")) Set_BorderStyle(bsSizeable);
    else if (asValue == _T("bsDialog")) Set_BorderStyle(bsDialog);
    return true;
  }
  if (asProperty == _T("CheckBoxes")) {
    Set_CheckBoxes(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("GridLines")) {
    Set_GridLines(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("HideSelection")) {
    Set_HideSelection(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("RowSelect")) {
    Set_RowSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("MultiSelect")) {
    Set_MultiSelect(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ReadOnly")) {
    Set_ReadOnly(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ItemIndex")) {
    Set_ItemIndex(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Columns")) {
    Cur = 0;
    while (Cur < (int) asValue.Length()) {
      asLigne = UnMot(asValue, _T(""), _T("\n"), &Cur);
      Cur2 = 0;
      asMot = UnMot(asLigne, _T(" "), _T(" ="), &Cur2);
      if (asMot == _T("item")) {
        asCaption = _T("");
        WidthColumn = 50;
      }
      else if (asMot == _T("Caption")) {
        asValue2 = UnMot(asLigne, _T(" ="), _T(""), &Cur2);
        asCaption = Application->Translate(SuppGuillVCL(asValue2));
      }
      else if (asMot == _T("Width")) {
        asValue2 = UnMot(asLigne, _T(" ="), _T(""), &Cur2);
        WidthColumn = StrToInt(asValue2);
      }
      else if (asMot == _T("end")) {
        FColumns->Add(asCaption, WidthColumn);
      }
    }
    return true;
  }
  if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
    OnChange = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnChanging")) {
		OnChanging.ObjCall = Sender;
    OnChanging = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnEdited")) {
		OnEdited.ObjCall = Sender;
    OnEdited = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnEditing")) {
		OnEditing.ObjCall = Sender;
    OnEditing = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnSelectItem")) {
		OnSelectItem.ObjCall = Sender;
    OnSelectItem = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

