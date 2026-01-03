//-----------------------------------------------------------------------------
//! @file TCustomImageList.cpp
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
// TCustomImageList
//---------------------------------------------------------------------------

TCustomImageList::TCustomImageList(TComponent* AOwner): TComponent(AOwner) {
  // Initialisations
  FClassName = _T("TCustomImageList");
  FWidth = 0;
  FHeight = 0;
  FAllocBy = 4;
  FBkColor = clWindow;
  FMasked = false;
  FImageBitmap = NULL;
  FResourceId = 0;
  FResourceName = _T("");
  FHandle = ImageList_Create(1, 1, ILC_COLOR24 | ILC_MASK, 1, FAllocBy);
	ImageList_SetBkColor(FHandle, FBkColor.cr);
}

TCustomImageList::~TCustomImageList(void) {
  if (FImageBitmap) DeleteObject(FImageBitmap);
  ImageList_Destroy(FHandle);
}

//---------------------------------------------------------------------------
HIMAGELIST TCustomImageList::Get_Handle(void) {
  return FHandle;
}

//---------------------------------------------------------------------------
bool TCustomImageList::Set_Handle(HIMAGELIST NewHandle) {
  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
int TCustomImageList::Get_Width(void) {
  return FWidth;
}

//---------------------------------------------------------------------------
bool TCustomImageList::Set_Width(int NewWidth) {
  if (FWidth != NewWidth) {
    FWidth = NewWidth;
    if (FWidth && FHeight) ImageList_SetIconSize(FHandle, FWidth, FHeight);
  }
  return true;
}

//---------------------------------------------------------------------------
int TCustomImageList::Get_Height(void) {
  return FHeight;
}

//---------------------------------------------------------------------------
bool TCustomImageList::Set_Height(int NewHeight) {
  if (FHeight != NewHeight) {
    FHeight = NewHeight;
    if (FWidth && FHeight) ImageList_SetIconSize(FHandle, FWidth, FHeight);
  }
  return true;
}

//---------------------------------------------------------------------------
int TCustomImageList::Get_AllocBy(void) {
  return FAllocBy;
}

//---------------------------------------------------------------------------
bool TCustomImageList::Set_AllocBy(int NewAllocBy) {
  if (FAllocBy != NewAllocBy) {
    FAllocBy = NewAllocBy;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété BkColor
//---------------------------------------------------------------------------
TColor TCustomImageList::Get_BkColor(void) {
  return FBkColor;
}

bool TCustomImageList::Set_BkColor(TColor NewBkColor) {
  if (FBkColor != NewBkColor) {
    FBkColor = NewBkColor;
		ImageList_SetBkColor(FHandle, FBkColor.cr);
  }
  return true;
}

//---------------------------------------------------------------------------
int TCustomImageList::Get_Count(void) {
  return ImageList_GetImageCount(FHandle);
}

//---------------------------------------------------------------------------
bool TCustomImageList::Get_Masked(void) {
  return FMasked;
}

//---------------------------------------------------------------------------
bool TCustomImageList::Set_Masked(bool NewMasked) {
  if (FMasked != NewMasked) {
    FMasked = NewMasked;
  }
  return true;
}

//---------------------------------------------------------------------------
int TCustomImageList::Get_ResourceId(void) {
  return FResourceId;
}

//---------------------------------------------------------------------------
bool TCustomImageList::Set_ResourceId(int NewResourceId) {
  if (FResourceId != NewResourceId) {
    FResourceId = NewResourceId;
    FImageBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(NewResourceId));
  }
  return true;
}

//---------------------------------------------------------------------------
AnsiString TCustomImageList::Get_ResourceName(void) {
  return FResourceName;
}

//---------------------------------------------------------------------------
bool TCustomImageList::Set_ResourceName(AnsiString NewResourceName) {
  if (FResourceName != NewResourceName) {
    FResourceName = NewResourceName;
		ResourceLoad(rtBitmap, NewResourceName, RGB(0, 0, 0));
  }
  return true;
}

//---------------------------------------------------------------------------
void TCustomImageList::Clear(void) {
  ImageList_RemoveAll(FHandle);
}

//---------------------------------------------------------------------------
int TCustomImageList::Add(TBitmap *Image, TBitmap *Mask) {
  HBITMAP hBmpImage, hBmpMasked;

  hBmpImage = Image->Handle;
  if (FMasked && Mask) hBmpMasked = Mask->Handle;
  else hBmpMasked = NULL;
  return ImageList_Add(FHandle, hBmpImage, hBmpMasked);
}

//---------------------------------------------------------------------------
void TCustomImageList::Delete(int Index) {
  ImageList_Remove(FHandle, Index);
}

//---------------------------------------------------------------------------
HBITMAP TCustomImageList::GetImageBitmap(void) {
  return FImageBitmap;
}

//---------------------------------------------------------------------------
void TCustomImageList::GetBitmap(int Index, TBitmap *Image) {
  IMAGEINFO ImageInfo;


  ImageList_GetImageInfo(FHandle, Index, &ImageInfo);
  Image->Handle = ImageInfo.hbmImage;

}

//---------------------------------------------------------------------------
bool TCustomImageList::ResourceLoad(TResType ResType, const AnsiString Name, TColor MaskColor) {

	if (FImageBitmap) {
		DeleteObject(FImageBitmap);
		FImageBitmap = NULL;
	}

	if (FHandle) {
		ImageList_Destroy(FHandle);
		FHandle = NULL;
	}

  switch (ResType) {
  case rtBitmap:
    FImageBitmap = LoadBitmap(HInstance, Name);
		if (FWidth == 0) {
			BITMAP Bitmap;
			if (GetObject(FImageBitmap, sizeof(Bitmap), &Bitmap)) {
				FWidth = Bitmap.bmWidth;
				FHeight = Bitmap.bmHeight;
				if (FWidth > FHeight) FWidth = FHeight;
				else FHeight = FWidth;
			}
		}
    FHandle = ImageList_LoadBitmap(HInstance, Name, FWidth,
                 FAllocBy, MaskColor.cr);
    break;
  case rtCursor:
    break;
  case rtIcon:
    break;
  }

  return (FHandle != NULL);
}

//---------------------------------------------------------------------------
bool TCustomImageList::Draw(HDC hdc, int x, int y, int Index) {
  return (ImageList_Draw(FHandle, Index, hdc, x, y, ILD_NORMAL) != 0);
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCustomImageList::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Width"));
  ListProperties->Add(_T("Height"));
  ListProperties->Add(_T("AllocBy"));
  ListProperties->Add(_T("BkColor"));
  ListProperties->Add(_T("Masked"));
  ListProperties->Add(_T("ResourceId"));
  ListProperties->Add(_T("ResourceName"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCustomImageList::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Width")) {
    return tpNumber;
  }
  if (asProperty == _T("Height")) {
    return tpNumber;
  }
  if (asProperty == _T("AllocBy")) {
    return tpNumber;
  }
  if (asProperty == _T("BkColor")) {
    return tpColor;
  }
  if (asProperty == _T("Masked")) {
    return tpBool;
  }
  if (asProperty == _T("ResourceId")) {
    return tpNumber;
  }
  if (asProperty == _T("ResourceName")) {
    return tpText;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCustomImageList::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Width")) {
    return _T("-1");
  }
  if (asProperty == _T("Height")) {
    return _T("-1");
  }
  if (asProperty == _T("AllocBy")) {
    return _T("4");
  }
  if (asProperty == _T("BkColor")) {
    return _T("clWindow");
  }
  if (asProperty == _T("Masked")) {
    return _T("True");
  }
  if (asProperty == _T("ResourceId")) {
    return _T("0");
  }
  if (asProperty == _T("ResourceName")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCustomImageList::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Width")) {
    return IntToStr(Get_Width());
  }
  if (asProperty == _T("Height")) {
    return IntToStr(Get_Height());
  }
  if (asProperty == _T("AllocBy")) {
    return IntToStr(Get_AllocBy());
  }
  if (asProperty == _T("BkColor")) {
    return ColorToString(Get_BkColor());
  }
  if (asProperty == _T("Masked")) {
    if (Get_Masked()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ResourceId")) {
    return IntToStr(Get_ResourceId());
  }
  if (asProperty == _T("ResourceName")) {
    return Get_ResourceName();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCustomImageList::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  if (asProperty == _T("Width")) {
    Set_Width(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("Height")) {
    Set_Height(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("AllocBy")) {
    Set_AllocBy(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("BkColor")) {
    TColor BkColor;
    BkColor = asValue;
    Set_BkColor(BkColor);
    return true;
  }
  if (asProperty == _T("Masked")) {
    Set_Masked(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ResourceId")) {
    Set_ResourceId(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("ResourceName")) {
    Set_ResourceName(asValue);
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

