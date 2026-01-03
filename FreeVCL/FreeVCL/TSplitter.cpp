//-----------------------------------------------------------------------------
//! @file TSplitter.cpp
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
// TSplitter
//---------------------------------------------------------------------------

TSplitter::TSplitter(TComponent* AOwner): TWinControl(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TSplitter");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	else hWndParent = NULL;
  bMove = false;
  FAlign = alLeft;

  // Enregistrement de la classe "TSplitter":
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TSplitter");

  RegisterClass(&wc);

  // Création de la boîte
  FHandle = CreateWindowEx(FExWindowStyle,
                         _T("TSplitter"), FCaption,
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
TSplitter::~TSplitter(void) {
}

//---------------------------------------------------------------------------
bool TSplitter::ProcessPaint(HDC hdc, RECT rcPaint) {
  RECT ClientRect;


  ::GetClientRect(FHandle, &ClientRect);
  if (FAlign == alLeft || FAlign == alRight) {
    DrawEdge(hdc, &ClientRect, EDGE_RAISED, BF_LEFT | BF_RIGHT | BF_SOFT | BF_MIDDLE);
  }
  else {
    DrawEdge(hdc, &ClientRect, EDGE_RAISED, BF_TOP | BF_BOTTOM | BF_SOFT | BF_MIDDLE);
  }

  return false;
}

//---------------------------------------------------------------------------
bool TSplitter::ProcessMouseMove(int X, int Y) {


  if (bMove) {
    POINT Pt;
    Pt.x = X;
    Pt.y = Y;
    ::ClientToScreen(FHandle, &Pt);
    if (FAlign == alLeft || FAlign == alRight) {
      if (XMove != Pt.x) {
        XMove = Pt.x;
        MoveWindow(FHandle, XWnd + XMove - XInit, YWnd, FWidth, FHeight, FALSE);
      }
    }
    else {
      if (YMove != Pt.y) {
        YMove = Pt.y;
        MoveWindow(FHandle, XWnd, YWnd + YMove - YInit, FWidth, FHeight, FALSE);
      }
    }
  }
  FCursor = (FAlign == alLeft || FAlign == alRight)? crSizeWE: crSizeNS;
  TWinControl::ProcessMouseMove(X, Y);

  return true;
}

//---------------------------------------------------------------------------
bool TSplitter::ProcessLButtonDown(int X, int Y) {

  if (!bMove) {
    POINT Pt;
    XWnd = FLeft;
    YWnd = FTop;
    Pt.x = X;
    Pt.y = Y;
    ::ClientToScreen(FHandle, &Pt);
    XInit = Pt.x;
    YInit = Pt.y;
    SetCapture(FHandle);
    bMove = true;
    TWinControl::ProcessLButtonDown(X, Y);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TSplitter::ProcessLButtonUp(int X, int Y) {

  if (bMove) {
    ReleaseCapture();
    bMove = false;
    InvalidateRect(FParent->Handle, NULL, TRUE);
    UpdateWindow(FParent->Handle);
    TWinControl::ProcessLButtonUp(X, Y);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TSplitter::ProcessMove(int X, int Y) {
  HWND hWndParent;
  RECT ClientRect;
  int SizeSplit;
  int i;
  TControl *Brother, *DerBrother;
  bool bAugmentation;


  if (bMove && FAlignDisabled == 0) {

    hWndParent = ::GetParent(FHandle);
    ::GetClientRect(hWndParent, &ClientRect);
    switch (FAlign) {
    case alLeft:
      SizeSplit = X;
      break;
    case alTop:
      SizeSplit = Y;
      break;
    case alRight:
      SizeSplit = ClientRect.right - ClientRect.left - (X + FWidth);
      break;
    case alBottom:
      SizeSplit = ClientRect.bottom - ClientRect.top - (Y + FHeight);
      break;
    default:
      return false;
    }

    DerBrother = NULL;
    bAugmentation = true;
    for (i = 0; i < FParent->ComponentCount; i++) {
      Brother = dynamic_cast<TControl *>(Parent->Components[i]);
      if (Brother == this) break;
      if (Brother) {
        if (Brother->Align == FAlign) {
          DerBrother = Brother;
          if (FAlign == alLeft || FAlign == alRight) {
            if (Brother->Width > SizeSplit) {
              Brother->Width = SizeSplit;
              bAugmentation = false;
            }
            SizeSplit -= Brother->Width;
          }
          else {
            if (Brother->Height > SizeSplit) {
              Brother->Height = SizeSplit;
              bAugmentation = false;
            }
            SizeSplit -= Brother->Height;
          }
        }
      }
    }
    if (DerBrother && bAugmentation) {
      if (FAlign == alLeft || FAlign == alRight) {
        DerBrother->Width = SizeSplit + DerBrother->Width;
      }
      else {
        DerBrother->Height = SizeSplit + DerBrother->Height;
      }
    }
    FParent->SizeSubComponents(0, 0, false);

  }

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TSplitter::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TSplitter::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TSplitter::GetDefaultProperty(AnsiString asProperty) {
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TSplitter::GetProperty(AnsiString asProperty) {
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TSplitter::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

