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

#include <FreeVCL.h>

#include "TFormPrincipale.h"
#include "TFVPrintDialog.h"
#include "TFVPrinterSetupDialog.h"
#include "TFVPrinter.h"
//---------------------------------------------------------------------------
TFormPrincipale *FormPrincipale;
//---------------------------------------------------------------------------
FASTCALL TFormPrincipale::TFormPrincipale(TComponent* Owner)
  : TForm(Owner) {
}

//---------------------------------------------------------------------------
void TFormPrincipale::CallEvent(AnsiString asEvent, TObject *Sender, ...) {
	BYTE *pArgs;


	pArgs = (BYTE *) &Sender;
	pArgs += sizeof(TObject *);

	if (asEvent == _T("Button1Clic")) Button1Clic(Sender);
	else if (asEvent == _T("Button3Clic")) Button3Clic(Sender);
	else if (asEvent == _T("Button2Clic")) Button2Clic(Sender);
	else if (asEvent == _T("Button4Clic")) Button4Clic(Sender);

}

//---------------------------------------------------------------------------
void FASTCALL TFormPrincipale::Button1Click(TObject *Sender) {
  TPrinter *Pr;


  PrintDialog1->Options = PrintDialog1->Options << poPrintToFile;
  if (PrintDialog1->Execute()) {
    Pr = Printer();
    Pr->BeginDoc();
    TextOut(Pr->Handle, 100, 100, _T("Titi fait du ski"), 16);
    Pr->EndDoc();
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFormPrincipale::Button3Click(TObject *Sender) {
  TFVPrintDialog * FVPrintDialog;
  TFVPrinter *Pr;


  FVPrintDialog = new TFVPrintDialog();
  FVPrintDialog->Options = FVPrintDialog->Options << poPrintToFile;
  if (FVPrintDialog->Execute()) {
    Pr = FVPrinter();
    Pr->BeginDoc();
    TextOut(Pr->Handle, 100, 100, _T("Titi fait du ski"), 16); 
    Pr->EndDoc();
  }
  delete FVPrintDialog;

}

//---------------------------------------------------------------------------
void FASTCALL TFormPrincipale::Button2Click(TObject *Sender) {
  if (PrinterSetupDialog1->Execute()) {
    //
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFormPrincipale::Button4Click(TObject *Sender) {
  TFVPrinterSetupDialog * FVPrinterSetupDialog;


  FVPrinterSetupDialog = new TFVPrinterSetupDialog();
  if (FVPrinterSetupDialog->Execute()) {
    //
  }
  delete FVPrinterSetupDialog;

}

//---------------------------------------------------------------------------


