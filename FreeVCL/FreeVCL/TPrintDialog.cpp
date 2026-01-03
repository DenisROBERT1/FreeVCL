//-----------------------------------------------------------------------------
//! @file TPrintDialog.cpp
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
// Constructeur
//---------------------------------------------------------------------------

TPrintDialog::TPrintDialog(TComponent* AOwner): TComponent(AOwner) {

	// Initialisations
	FClassName = _T("TPrintDialog");
  memset(&PrintDlgStruct, 0, sizeof(PRINTDLG));
  PrintDlgStruct.lStructSize = sizeof(PRINTDLG);
  PrintDlgStruct.Flags = PD_RETURNDC | PD_COLLATE | PD_HIDEPRINTTOFILE;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TPrintDialog::~TPrintDialog(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Copies
//---------------------------------------------------------------------------

int TPrintDialog::Get_Copies(void) {
  return PrintDlgStruct.nCopies;
}

bool TPrintDialog::Set_Copies(int NewCopies) {
  if (PrintDlgStruct.nCopies != NewCopies) {
    PrintDlgStruct.nCopies = NewCopies;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MinPage
//---------------------------------------------------------------------------

int TPrintDialog::Get_MinPage(void) {
  return PrintDlgStruct.nMinPage;
}

bool TPrintDialog::Set_MinPage(int NewMinPage) {
  if (PrintDlgStruct.nMinPage != NewMinPage) {
    PrintDlgStruct.nMinPage = NewMinPage;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété MaxPage
//---------------------------------------------------------------------------

int TPrintDialog::Get_MaxPage(void) {
  return PrintDlgStruct.nMaxPage;
}

bool TPrintDialog::Set_MaxPage(int NewMaxPage) {
  if (PrintDlgStruct.nMaxPage != NewMaxPage) {
    PrintDlgStruct.nMaxPage = NewMaxPage;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FromPage
//---------------------------------------------------------------------------

int TPrintDialog::Get_FromPage(void) {
  return PrintDlgStruct.nFromPage;
}

bool TPrintDialog::Set_FromPage(int NewFromPage) {
  if (PrintDlgStruct.nFromPage != NewFromPage) {
    PrintDlgStruct.nFromPage = NewFromPage;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ToPage
//---------------------------------------------------------------------------

int TPrintDialog::Get_ToPage(void) {
  return PrintDlgStruct.nToPage;
}

bool TPrintDialog::Set_ToPage(int NewToPage) {
  if (PrintDlgStruct.nToPage != NewToPage) {
    PrintDlgStruct.nToPage = NewToPage;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété bCollate
//---------------------------------------------------------------------------

bool TPrintDialog::Get_Collate(void) {
  return ((PrintDlgStruct.Flags & PD_COLLATE) != 0);
}

bool TPrintDialog::Set_Collate(bool bNewCollate) {
  if ((PrintDlgStruct.Flags & PD_COLLATE) == 0) PrintDlgStruct.Flags |= PD_COLLATE;
  else PrintDlgStruct.Flags &= ~PD_COLLATE;

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PrintToFile
//---------------------------------------------------------------------------

bool TPrintDialog::Get_PrintToFile(void) {
  return ((PrintDlgStruct.Flags & PD_PRINTTOFILE) != 0);
}

bool TPrintDialog::Set_PrintToFile(bool NewPrintToFile) {
  if ((PrintDlgStruct.Flags & PD_PRINTTOFILE) == 0) PrintDlgStruct.Flags |= PD_PRINTTOFILE;
  else PrintDlgStruct.Flags &= ~PD_PRINTTOFILE;

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Options
//---------------------------------------------------------------------------

TPrintDialogOptions TPrintDialog::Get_Options(void) {
  TPrintDialogOptions Retour;


  Retour.Clear();
  if (!(PrintDlgStruct.Flags & PD_HIDEPRINTTOFILE)) Retour << poPrintToFile;
  if (!(PrintDlgStruct.Flags & PD_NOPAGENUMS)) Retour << poPageNums;
  if (!(PrintDlgStruct.Flags & PD_NOSELECTION)) Retour << poSelection;
  if (!(PrintDlgStruct.Flags & PD_NOWARNING)) Retour << poWarning;
  if (PrintDlgStruct.Flags & PD_SHOWHELP) Retour << poHelp;
  if (PrintDlgStruct.Flags & PD_DISABLEPRINTTOFILE) Retour << poDisablePrintToFile;

  return Retour;
}

bool TPrintDialog::Set_Options(TPrintDialogOptions NewOptions) {

  PrintDlgStruct.Flags &= ~(PD_HIDEPRINTTOFILE | PD_NOPAGENUMS | PD_NOSELECTION | PD_NOWARNING | PD_SHOWHELP | PD_DISABLEPRINTTOFILE);

  if (!NewOptions.Contains(poPrintToFile)) PrintDlgStruct.Flags |= PD_HIDEPRINTTOFILE;
  if (!NewOptions.Contains(poPageNums)) PrintDlgStruct.Flags |= PD_PAGENUMS;
  if (!NewOptions.Contains(poSelection)) PrintDlgStruct.Flags |= PD_NOSELECTION;
  if (!NewOptions.Contains(poWarning)) PrintDlgStruct.Flags |= PD_NOWARNING;
  if (NewOptions.Contains(poHelp)) PrintDlgStruct.Flags |= PD_SHOWHELP;
  if (NewOptions.Contains(poDisablePrintToFile)) PrintDlgStruct.Flags |= PD_DISABLEPRINTTOFILE;

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété PrintRange
//---------------------------------------------------------------------------

TPrintRange TPrintDialog::Get_PrintRange(void) {
  if (PrintDlgStruct.Flags & PD_SELECTION) return prSelection;
  else if (PrintDlgStruct.Flags & PD_PAGENUMS) return prPageNums;
  else return prAllPages;
}

bool TPrintDialog::Set_PrintRange(TPrintRange NewPrintRange) {
  PrintDlgStruct.Flags &= ~(PD_ALLPAGES | PD_SELECTION | PD_PAGENUMS);

  if (NewPrintRange == prSelection) PrintDlgStruct.Flags |= PD_SELECTION;
  else if (NewPrintRange == prPageNums) PrintDlgStruct.Flags |= PD_PAGENUMS;
  else PrintDlgStruct.Flags |= PD_ALLPAGES;

  return true;
}


//---------------------------------------------------------------------------
// Execute
//---------------------------------------------------------------------------

bool TPrintDialog::Execute(void) {
  TPrinter * PrinterThis;


  PrintDlgStruct.hwndOwner = Application->MainForm->Handle;
  if (!PrintDlg(&PrintDlgStruct)) return false;
  PrinterThis = Printer();
  PrinterThis->Handle = PrintDlgStruct.hDC;

  return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TPrintDialog::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Collate"));
  ListProperties->Add(_T("Copies"));
  ListProperties->Add(_T("FromPage"));
  ListProperties->Add(_T("MaxPage"));
  ListProperties->Add(_T("MinPage"));
  ListProperties->Add(_T("Options"));
  ListProperties->Add(_T("PrintRange"));
  ListProperties->Add(_T("PrintToFile"));
  ListProperties->Add(_T("ToPage"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TPrintDialog::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Collate")) {
    return tpBool;
  }
  if (asProperty == _T("Copies")) {
    return tpNumber;
  }
  if (asProperty == _T("FromPage")) {
    return tpNumber;
  }
  if (asProperty == _T("MaxPage")) {
    return tpNumber;
  }
  if (asProperty == _T("MinPage")) {
    return tpNumber;
  }
  if (asProperty == _T("Options")) {
    *asInfos = _T("poPrintToFile\npoPageNums\npoSelection\npoWarning\npoHelp\npoDisablePrintToFile");
    return tpMultipleChoice;
  }
  if (asProperty == _T("PrintRange")) {
    *asInfos = _T("prAllPages\nprSelection\nprPageNums");
    return tpChoice;
  }
  if (asProperty == _T("PrintToFile")) {
    return tpBool;
  }
  if (asProperty == _T("ToPage")) {
    return tpNumber;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TPrintDialog::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Collate")) {
    return _T("True");
  }
  if (asProperty == _T("Copies")) {
    return _T("0");
  }
  if (asProperty == _T("FromPage")) {
    return _T("0");
  }
  if (asProperty == _T("MaxPage")) {
    return _T("0");
  }
  if (asProperty == _T("MinPage")) {
    return _T("0");
  }
  if (asProperty == _T("Options")) {
    return _T("[poPageNums, poSelection, poWarning]");
  }
  if (asProperty == _T("PrintRange")) {
    return _T("prAllPages");
  }
  if (asProperty == _T("PrintToFile")) {
    return _T("False");
  }
  if (asProperty == _T("ToPage")) {
    return _T("0");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TPrintDialog::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Collate")) {
		if (Get_Collate()) return _T("True");
		else return _T("False");
  }
  if (asProperty == _T("Copies")) {
    return IntToStr(Get_Copies());
  }
  if (asProperty == _T("FromPage")) {
    return IntToStr(Get_FromPage());
  }
  if (asProperty == _T("MaxPage")) {
    return IntToStr(Get_MaxPage());
  }
  if (asProperty == _T("MinPage")) {
    return IntToStr(Get_MinPage());
  }
  if (asProperty == _T("Options")) {
    AnsiString asOptions;
    TPrintDialogOptions Options = Get_Options();
    if (Options.Contains(poPrintToFile)) asOptions += _T("poPrintToFile\n");
    if (Options.Contains(poPageNums)) asOptions += _T("poPageNums\n");
    if (Options.Contains(poSelection)) asOptions += _T("poSelection\n");
    if (Options.Contains(poWarning)) asOptions += _T("poWarning\n");
    if (Options.Contains(poHelp)) asOptions += _T("poHelp\n");
    if (Options.Contains(poDisablePrintToFile)) asOptions += _T("poDisablePrintToFile\n");
    return FormatProperty(asOptions, _T("["), _T("]"), _T(", "));
  }
  if (asProperty == _T("PrintRange")) {
    TPrintRange PrintRange = Get_PrintRange();
		if (PrintRange == prSelection) return _T("prSelection");
		else if (PrintRange == prPageNums) return _T("prPageNums");
		else return _T("prAllPages");
  }
  if (asProperty == _T("PrintToFile")) {
		if (Get_PrintToFile()) return _T("True");
		else return _T("False");
  }
  if (asProperty == _T("ToPage")) {
    return IntToStr(Get_ToPage());
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TPrintDialog::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {

  if (asProperty == _T("Collate")) {
		Set_Collate(asValue == _T("True"));
		return true;
  }
  if (asProperty == _T("Copies")) {
		Set_Copies(StrToInt(asValue));
		return true;
  }
  if (asProperty == _T("FromPage")) {
		Set_FromPage(StrToInt(asValue));
		return true;
  }
  if (asProperty == _T("MaxPage")) {
		Set_MaxPage(StrToInt(asValue));
		return true;
  }
  if (asProperty == _T("MinPage")) {
		Set_MinPage(StrToInt(asValue));
		return true;
  }
  if (asProperty == _T("Options")) {
		int Cur;
		AnsiString asMot;
		TPrintDialogOptions NewOptions;

		NewOptions.Clear();
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("poPrintToFile")) NewOptions << poPrintToFile;
      else if (asMot == _T("poPageNums")) NewOptions << poPageNums;
      else if (asMot == _T("poSelection")) NewOptions << poSelection;
      else if (asMot == _T("poWarning")) NewOptions << poWarning;
      else if (asMot == _T("poHelp")) NewOptions << poHelp;
      else if (asMot == _T("poDisablePrintToFile")) NewOptions << poDisablePrintToFile;
    } while (!asMot.IsEmpty());
		Set_Options(NewOptions);

		return true;
  }
  if (asProperty == _T("PrintRange")) {
    TPrintRange NewPrintRange = prAllPages;

    if (asValue == _T("prSelection")) NewPrintRange = prSelection;
    else if (asValue == _T("prPageNums")) NewPrintRange = prPageNums;
    else if (asValue == _T("prAllPages")) NewPrintRange = prAllPages;
		Set_PrintRange(NewPrintRange);

		return true;
  }
  if (asProperty == _T("PrintToFile")) {
		Set_PrintToFile(asValue == _T("True"));
		return true;
  }
  if (asProperty == _T("ToPage")) {
		Set_ToPage(StrToInt(asValue));
		return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
