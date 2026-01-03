//-----------------------------------------------------------------------------
//! @file TCodePanel.cpp
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

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

#include _T("TCodePanel.h")

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TCodePanel::TCodePanel(TComponent* AOwner):
  TCustomPanel(AOwner) {
  BevelOuter = bvLowered;
  FCodeEditor = new TCodeEditor(this);  // Donc détruit automatiquement
  FCodeEditor->Parent = this;
  TRect Rect = ClientRect;
  FCodeEditor->Anchors = ((TAnchors) FCodeEditor->Anchors) << akLeft << akTop << akRight << akBottom;
  FCodeEditor->Left = 0;  // En coordonnées client
  FCodeEditor->Top = 0;
  FCodeEditor->Width = (int) Rect.Width;
  FCodeEditor->Height = (int) Rect.Height;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TCodePanel::~TCodePanel() {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété CodeEditor
//---------------------------------------------------------------------------

TCodeEditor * TCodePanel::Get_CodeEditor(void) {
  return FCodeEditor;
}

bool TCodePanel::Set_CodeEditor(TCodeEditor *NewCodeEditor) {
  if (FCodeEditor != NewCodeEditor) {
    delete FCodeEditor;
    FCodeEditor = NewCodeEditor;
  }
  return true;
}

//---------------------------------------------------------------------------

