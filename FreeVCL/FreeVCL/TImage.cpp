//-----------------------------------------------------------------------------
//! @file TImage.cpp
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
// Déclarations anticipées
//---------------------------------------------------------------------------

class TPicture;


//---------------------------------------------------------------------------
// TImage
//---------------------------------------------------------------------------

TImage::TImage(TComponent* AOwner): TWinControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TImage");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
	FCenter = false;
	FProportional = false;
	FStretch = false;
  FPicture = new TPicture();
	SET_EVENT(FPicture, TNotifyEvent, OnChange, TImage, this, PictureChange);

  // Enregistrement de la classe "TImage":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TImage");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TImage"), FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

  if (FHandle) {
    bWantPaint = true;
    bProcessCreateToDo = true;
  }

}

//---------------------------------------------------------------------------
TImage::~TImage(void) {
  delete FPicture;
}

//---------------------------------------------------------------------------
bool TImage::ProcessPaint(HDC hdc, RECT rcPaint) {
	int LeftDraw, TopDraw, WidthDraw, HeightDraw;

	TGraphic *Graphic = FPicture->Graphic;
	if (!Graphic) return false;

  TRect DrawRect = ClientRect;

	if (FStretch) {
		if (FProportional) {
			if ((DrawRect.Right - DrawRect.Left) * Graphic->Height > (DrawRect.Bottom - DrawRect.Top) * Graphic->Width) {
				WidthDraw = (DrawRect.Bottom - DrawRect.Top) * Graphic->Width / Graphic->Height;
				HeightDraw = DrawRect.Bottom - DrawRect.Top;
			}
			else {
				WidthDraw = DrawRect.Right - DrawRect.Left;
				HeightDraw = (DrawRect.Right - DrawRect.Left) * Graphic->Height / Graphic->Width;
			}
		}
		else {
			WidthDraw = DrawRect.Right - DrawRect.Left;
			HeightDraw = DrawRect.Bottom - DrawRect.Top;
		}
	}
	else {
		WidthDraw = Graphic->Width;
		HeightDraw = Graphic->Height;
	}
	if (FCenter) {
		LeftDraw = (DrawRect.Right - DrawRect.Left - WidthDraw) / 2;
		TopDraw = (DrawRect.Bottom - DrawRect.Top - HeightDraw) / 2;
	}
	else {
		LeftDraw = DrawRect.Left;
		TopDraw = DrawRect.Top;
	}

	Graphic->Draw(hdc, LeftDraw, TopDraw, WidthDraw, HeightDraw);

  return true;

}

//---------------------------------------------------------------------------
void TImage::PictureChange(TObject *Sender) {
  Invalidate();
}

//---------------------------------------------------------------------------
bool TImage::Get_Center(void) {
  return FCenter;
}

//---------------------------------------------------------------------------
bool TImage::Set_Center(bool NewCenter) {
  if (FCenter != NewCenter) {
    FCenter = NewCenter;
		Invalidate();
  }

  return true;
}

//---------------------------------------------------------------------------
bool TImage::Get_Proportional(void) {
  return FProportional;
}

//---------------------------------------------------------------------------
bool TImage::Set_Proportional(bool NewProportional) {
  if (FProportional != NewProportional) {
    FProportional = NewProportional;
		Invalidate();
  }

  return true;
}

//---------------------------------------------------------------------------
bool TImage::Get_Stretch(void) {
  return FStretch;
}

//---------------------------------------------------------------------------
bool TImage::Set_Stretch(bool NewStretch) {
  if (FStretch != NewStretch) {
    FStretch = NewStretch;
		Invalidate();
  }

  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Picture
//---------------------------------------------------------------------------

TPicture *TImage::Get_Picture(void) {
  return FPicture;
}

bool TImage::Set_Picture(TPicture *NewPicture) {
	TGraphic *NewGraphic;

	if (NewPicture) {
		NewGraphic = NewPicture->Graphic;
	}
	else {
		NewGraphic = NULL;
	}
	if (FPicture->Graphic != NewGraphic) {
		FPicture->Graphic = NewGraphic;
	}

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TImage::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Center"));
  ListProperties->Add(_T("Proportional"));
  ListProperties->Add(_T("Stretch"));
  ListProperties->Add(_T("Picture"));
  ListProperties->Add(_T("Picture.Data"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TImage::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Center")) {
    return tpBool;
  }
  if (asProperty == _T("Proportional")) {
    return tpBool;
  }
  if (asProperty == _T("Stretch")) {
    return tpBool;
  }
  if (asProperty == _T("Picture")) {
    return tpObject;
  }
  if (asProperty == _T("Picture.Data")) {
    return tpData;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TImage::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Center")) {
    return _T("False");
  }
  if (asProperty == _T("Proportional")) {
    return _T("False");
  }
  if (asProperty == _T("Stretch")) {
    return _T("False");
  }
  if (asProperty == _T("Picture")) {
    return _T("");
  }
  if (asProperty == _T("Picture.Data")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TImage::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Center")) {
    if (Get_Center()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Proportional")) {
    if (Get_Proportional()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Stretch")) {
    if (Get_Stretch()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Picture")) {
    return FPicture->Name;
  }
  if (asProperty == _T("Picture.Data")) {
    if (FPicture->Graphic != NULL) return FPicture->Graphic->GetData();
		return _T("");
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TImage::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  if (asProperty == _T("Center")) {
    Set_Center(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Proportional")) {
    Set_Proportional(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Stretch")) {
    Set_Stretch(asValue == _T("True"));
    return true;
  }
	if (asProperty == _T("Picture")) {
    TPicture *Picture;
		if (asValue.IsEmpty()) Picture = NULL;
		else Picture = dynamic_cast<TPicture *>(FindRootComponent()->FindComponent(asValue));
    Set_Picture(Picture);
    return true;
  }
  if (asProperty == _T("Picture.Data")) {
    int SizeValeur = asValue.Length() / 2;
    char *szValeur = new char[SizeValeur];
    HexToBin(asValue, szValeur, SizeValeur);
		TMemoryStream PictureData(&szValeur[4], SizeValeur - 4);
    TBitmap *Bitmap = new TBitmap;
		Bitmap->LoadFromStream(&PictureData);
    FPicture->Graphic = Bitmap;
		delete Bitmap;
    delete[] szValeur;
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}


//---------------------------------------------------------------------------

