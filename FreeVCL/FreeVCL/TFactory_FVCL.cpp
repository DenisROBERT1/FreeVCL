//-----------------------------------------------------------------------------
//! @file TFactory_FVCL.cpp
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

TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// TFactory_FVCL
//---------------------------------------------------------------------------

TFactory_FVCL::TFactory_FVCL(void) {
  // Initialisations
}

TFactory_FVCL::~TFactory_FVCL(void) {
}

bool TFactory_FVCL::GetListObjectsVirt(TStrings *ListObjects, TStrings *ListTabs) {
	// Ne pas traduire le contenu de ListTabs (ce sera fait au moment de l'affichage)
  ListObjects->Add(_T("TAction")); ListTabs->Add(_T("Système"));
  ListObjects->Add(_T("TBevel")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TBitBtn")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TButton")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TCheckBox")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TColorDialog")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TColorBox")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TComboBox")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TControlBar")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TDateTimePicker")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TDirectoryListBox")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TDrawGrid")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TEdit")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TFrame")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TFontDialog")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TForm")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TGroupBox")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("THotKey")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TImage")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TImageList")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TLabel")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TLink")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TListBox")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TListView")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TMainMenu")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TMemo")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TMenuItem")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TMonthCalendar")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TOpenDialog")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TPaintBox")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TPageControl")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TPageScroller")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TPanel")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TPopupMenu")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TPrintDialog")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TPrinterSetupDialog")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TProgressBar")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TRadioButton")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TRadioGroup")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TSaveDialog")); ListTabs->Add(_T("Dialogues"));
  ListObjects->Add(_T("TScrollBar")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TScrollBox")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TShape")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TSpeedButton")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TSplitter")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TStaticText")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TStatusBar")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TStringGrid")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TTabControl")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TTabSheet")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TTimer")); ListTabs->Add(_T("Système"));
  ListObjects->Add(_T("TToolBar")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TToolButton")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TTrackBar")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TTrayIcon")); ListTabs->Add(_T("Contrôles étendus"));
  ListObjects->Add(_T("TTreeView")); ListTabs->Add(_T("Contrôles courants"));
  ListObjects->Add(_T("TUpDown")); ListTabs->Add(_T("Contrôles courants"));

  return true;
}

TComponent *TFactory_FVCL::CreateObjectVirt(TComponent* AOwner, AnsiString asNom) {
  if (asNom == _T("TAction")) return new TAction(AOwner);
  if (asNom == _T("TBevel")) return new TBevel(AOwner);
  if (asNom == _T("TBitBtn")) return new TBitBtn(AOwner);
  if (asNom == _T("TButton")) return new TButton(AOwner);
  if (asNom == _T("TCheckBox")) return new TCheckBox(AOwner);
  if (asNom == _T("TColorDialog")) return new TColorDialog(AOwner);
  if (asNom == _T("TColorBox")) return new TColorBox(AOwner);
  if (asNom == _T("TComboBox")) return new TComboBox(AOwner);
  if (asNom == _T("TControlBar")) return new TControlBar(AOwner);
  if (asNom == _T("TDateTimePicker")) return new TDateTimePicker(AOwner);
  if (asNom == _T("TDirectoryListBox")) return new TDirectoryListBox(AOwner);
  if (asNom == _T("TDrawGrid")) return new TDrawGrid(AOwner);
  if (asNom == _T("TEdit")) return new TEdit(AOwner);
  if (asNom == _T("TFontDialog")) return new TFontDialog(AOwner);
  if (asNom == _T("TForm")) return new TForm(AOwner);
  if (asNom == _T("TFrame")) return new TFrame(AOwner);
  if (asNom == _T("TGroupBox")) return new TGroupBox(AOwner);
  if (asNom == _T("THotKey")) return new THotKey(AOwner);
  if (asNom == _T("TImage")) return new TImage(AOwner);
  if (asNom == _T("TImageList")) return new TImageList(AOwner);
  if (asNom == _T("TLabel")) return new TLabel(AOwner);
  if (asNom == _T("TLink")) return new TLink(AOwner);
  if (asNom == _T("TListBox")) return new TListBox(AOwner);
  if (asNom == _T("TListView")) return new TListView(AOwner);
  if (asNom == _T("TMainMenu")) return new TMainMenu(AOwner);
  if (asNom == _T("TMemo")) return new TMemo(AOwner);
  if (asNom == _T("TMenuItem")) return new TMenuItem(AOwner);
  if (asNom == _T("TMonthCalendar")) return new TMonthCalendar(AOwner);
  if (asNom == _T("TOpenDialog")) return new TOpenDialog(AOwner);
  if (asNom == _T("TPageControl")) return new TPageControl(AOwner);
  if (asNom == _T("TPaintBox")) return new TPaintBox(AOwner);
  if (asNom == _T("TPageScroller")) return new TPageScroller(AOwner);
  if (asNom == _T("TPanel")) return new TPanel(AOwner);
  if (asNom == _T("TPopupMenu")) return new TPopupMenu(AOwner);
  if (asNom == _T("TPrintDialog")) return new TPrintDialog(AOwner);
  if (asNom == _T("TPrinterSetupDialog")) return new TPrinterSetupDialog(AOwner);
  if (asNom == _T("TProgressBar")) return new TProgressBar(AOwner);
  if (asNom == _T("TRadioButton")) return new TRadioButton(AOwner);
  if (asNom == _T("TRadioGroup")) return new TRadioGroup(AOwner);
  if (asNom == _T("TSaveDialog")) return new TSaveDialog(AOwner);
  if (asNom == _T("TScrollBar")) return new TScrollBar(AOwner);
  if (asNom == _T("TScrollBox")) return new TScrollBox(AOwner);
  if (asNom == _T("TShape")) return new TShape(AOwner);
  if (asNom == _T("TSpeedButton")) return new TSpeedButton(AOwner);
  if (asNom == _T("TSplitter")) return new TSplitter(AOwner);
  if (asNom == _T("TStaticText")) return new TStaticText(AOwner);
  if (asNom == _T("TStringGrid")) return new TStringGrid(AOwner);
  if (asNom == _T("TStatusBar")) return new TStatusBar(AOwner);
  if (asNom == _T("TTabControl")) return new TTabControl(AOwner);
  if (asNom == _T("TTabSheet")) return new TTabSheet(AOwner);
  if (asNom == _T("TTimer")) return new TTimer(AOwner);
  if (asNom == _T("TToolBar")) return new TToolBar(AOwner);
  if (asNom == _T("TToolButton")) return new TToolButton(AOwner);
  if (asNom == _T("TTrackBar")) return new TTrackBar(AOwner);
  if (asNom == _T("TTrayIcon")) return new TTrayIcon(AOwner);
  if (asNom == _T("TTreeView")) return new TTreeView(AOwner);
  if (asNom == _T("TUpDown")) return new TUpDown(AOwner);

  return NULL;
}

//---------------------------------------------------------------------------

