//-----------------------------------------------------------------------------
//! @file TForm_Principale.cpp
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

#include "TForm_Principale.h"

//---------------------------------------------------------------------------
TForm_Principale::TForm_Principale(HWND hWndParent, LPCTSTR szName)
  : TForm(hWndParent, szName, FVCL_ARG_SUPP) {
  Factory_FVCL_Multimed.Init();

  bCapture = false;
	WebCam->Connected = true;
}

//---------------------------------------------------------------------------
TForm_Principale::~TForm_Principale(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Button_ParamClick(TObject *Sender) {
  AnsiString asTitle;
  AnsiString asMsg;


  if (!WebCam->DlgVideoFormat()) {
    asTitle = Application->Translate("Erreur");
    asMsg = Application->Translate("Pas de paramètres de format pour cette webcam");
    MessageBox(FHandle, asMsg.c_str(), asTitle.c_str(), MB_OK | MB_ICONEXCLAMATION);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::Button_CaptureClick(TObject *Sender) {
  bCapture = true;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_Principale::WebCamFrame(TObject *Sender, TBitmap *Bitmap) {
  if (bCapture) {
    bCapture = false;
    if (SaveDialog->Execute()) {
      Bitmap->SaveToFile(SaveDialog->FileName);
    }
  }
}

//---------------------------------------------------------------------------

