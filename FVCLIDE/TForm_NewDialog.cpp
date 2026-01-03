//-----------------------------------------------------------------------------
//! @file TForm_NewDialog.cpp
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


//---------------------------------------------------------------------------
// Fichiers inclus standard
//---------------------------------------------------------------------------

#include <FreeVCL.h>

//---------------------------------------------------------------------------
// Fichiers inclus spécifiques
//---------------------------------------------------------------------------

#include "TConfig.h"
#include "TFVCLIDEAutomation.h"
#include "TForm_NewDialog.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_NewDialog::TForm_NewDialog(TComponent *Owner, LPCTSTR szName)
		: TForm(Owner, szName, FVCL_ARG_SUPP) {
	Choice = 1;	// Option 1 sélectionnée à l'initialisation
	FDescription = new TStringList();
	FHandleNewWindow = NULL;
	bListBoxUpdated = false;
}

//---------------------------------------------------------------------------
TForm_NewDialog::~TForm_NewDialog(void) {
	delete FDescription;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NbControls
//---------------------------------------------------------------------------

TStrings *TForm_NewDialog::Get_Description(void) {
	AnsiString asLigne;
	AnsiString asAnchors;
	int i;

	FDescription->Clear();

	if (ConstructionApercu()) {

		asLigne.sprintf(_T("object %s: TForm"), asFormName.c_str()); FDescription->Add(asLigne);
		asLigne.sprintf(_T("Caption = '%s'"), asFormCaption.c_str()); FDescription->Add(asLigne);
		asLigne.sprintf(_T("Width = %i"), FormWidth); FDescription->Add(asLigne);
		asLigne.sprintf(_T("Height = %i"), FormHeight); FDescription->Add(asLigne);
		asLigne.sprintf(_T("Position = poOwnerFormCenter")); FDescription->Add(asLigne);
		for (i = 0; i < (int) Controls.size(); i++) {
			asLigne.sprintf(_T("object %s: %s"), Controls[i].asName.c_str(), Controls[i].asClass.c_str()); FDescription->Add(asLigne);
			asLigne.sprintf(_T("Caption = '%s'"), Controls[i].asCaption.c_str()); FDescription->Add(asLigne);
			asLigne.sprintf(_T("Left = %i"), Controls[i].Left); FDescription->Add(asLigne);
			asLigne.sprintf(_T("Top = %i"), Controls[i].Top); FDescription->Add(asLigne);
			asLigne.sprintf(_T("Width = %i"), Controls[i].Width); FDescription->Add(asLigne);
			asLigne.sprintf(_T("Height = %i"), Controls[i].Height); FDescription->Add(asLigne);
			asLigne.sprintf(_T("TabOrder = %i"), Controls[i].TabOrder); FDescription->Add(asLigne);
			asAnchors = _T("");
			if (Controls[i].AnchorLeft) asAnchors += _T("akLeft, ");
			if (Controls[i].AnchorTop) asAnchors += _T("akTop, ");
			if (Controls[i].AnchorRight) asAnchors += _T("akRight, ");
			if (Controls[i].AnchorBottom) asAnchors += _T("akBottom, ");
			if (!asAnchors.IsEmpty()) asAnchors = asAnchors.SubString(1, asAnchors.Length() - 2);
			asLigne.sprintf(_T("Anchor = [%s]"), asAnchors.c_str()); FDescription->Add(asLigne);
			if (Controls[i].OnClick) {
				asLigne.sprintf(_T("OnClick = %sClick"), Controls[i].asName.c_str()); FDescription->Add(asLigne);
			}
			asLigne.sprintf(_T("end")); FDescription->Add(asLigne);
		}

		asLigne.sprintf(_T("end")); FDescription->Add(asLigne);

	}

  return FDescription;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété HandleNewWindow
//---------------------------------------------------------------------------

HWND TForm_NewDialog::Get_HandleNewWindow(void) {
  return FHandleNewWindow;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_NewDialog::RadioButton_EmptyClick(TObject *Sender) {
	Choice = 1;
	Label_Struct->Visible = false;
	Memo_Struct->Visible = false;
  Label_Windows->Visible = false;
  ListBox_Windows->Visible = false;
	Button_Apercu->Enabled = true;
	Invalidate();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_NewDialog::RadioButton_OkCancelHelpClick(TObject *Sender) {
	Choice = 2;
	Label_Struct->Visible = false;
	Memo_Struct->Visible = false;
  Label_Windows->Visible = false;
  ListBox_Windows->Visible = false;
	Button_Apercu->Enabled = true;
	Invalidate();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_NewDialog::RadioButton_StructureClick(TObject *Sender) {
	Choice = 3;
	Label_Struct->Visible = true;
	Memo_Struct->Visible = true;
  Label_Windows->Visible = false;
  ListBox_Windows->Visible = false;
	Button_Apercu->Enabled = true;
	Invalidate();
}

//---------------------------------------------------------------------------
void FASTCALL TForm_NewDialog::RadioButton_FromWindowClick(TObject *Sender) {
	Choice = 4;
	Label_Struct->Visible = false;
	Memo_Struct->Visible = false;
  Label_Windows->Visible = true;
  ListBox_Windows->Visible = true;
	Button_Apercu->Enabled = false;
	Invalidate();

	if (!bListBoxUpdated) {
		EnumWindows(EnumWnd, (LPARAM) this);
		bListBoxUpdated = true;
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_NewDialog::Button_OkClick(TObject *Sender) {
	if (Choice != 4) {
		if (!ConstructionApercu()) return;
	}
	else {
		int ItemIndex = ListBox_Windows->ItemIndex;
		if (ItemIndex != -1) {
			FHandleNewWindow = (HWND) ListBox_Windows->Items->Objects[ItemIndex];
		}
		else {
			FVCLIDEAutomation->DisplayError(
				_TT("Veuillez choisir la fenêtre dans la liste"),
				_TT("Erreur saisie"),
				_T(""),
				MB_OK | MB_ICONWARNING);
			return;
		}
	}
	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_NewDialog::Button_ApercuClick(TObject *Sender) {
  TForm *Form;
  TWinControl *WinControl;
  TLabel *Label = NULL;
  TButton *Button = NULL;
  TEdit *Edit = NULL;
  TUpDown *UpDown = NULL;
  TCheckBox *CheckBox = NULL;
  TAnchors Anchors;
  int i;


	if (!ConstructionApercu()) return;

  Form = new TForm(this);
  Form->Name = asFormName;
  Form->Caption = asFormCaption;
  Form->Width = FormWidth + 2 * GetSystemMetrics(SM_CXSIZEFRAME) - 8;
  Form->Height = FormHeight + 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) - 27;
  Form->Position = poOwnerFormCenter;

  for (i = 0; i < (int) Controls.size(); i++) {

    WinControl = NULL;

    if (Controls[i].asClass == _T("TLabel")) {
      Label = new TLabel(Form);
      WinControl = Label;
    }
    else if (Controls[i].asClass == _T("TButton")) {
      Button = new TButton(Form);
      WinControl = Button;
    }
    else if (Controls[i].asClass == _T("TEdit")) {
      Edit = new TEdit(Form);
      WinControl = Edit;
    }
    else if (Controls[i].asClass == _T("TUpDown")) {
      UpDown = new TUpDown(Form);
      if (Edit) UpDown->Associate = Edit;
      WinControl = UpDown;
    }
    else if (Controls[i].asClass == _T("TCheckBox")) {
      CheckBox = new TCheckBox(Form);
      WinControl = CheckBox;
    }

    if (WinControl) {
      WinControl->Left = Controls[i].Left;
      WinControl->Top = Controls[i].Top;
      WinControl->Width = Controls[i].Width;
      WinControl->Height = Controls[i].Height;
      WinControl->Name = Controls[i].asName;
      WinControl->Caption = Controls[i].asCaption;
      WinControl->TabOrder = Controls[i].TabOrder;
      Anchors.Clear();
      if (Controls[i].AnchorLeft) Anchors << akLeft;
      if (Controls[i].AnchorTop) Anchors << akTop;
      if (Controls[i].AnchorRight) Anchors << akRight;
      if (Controls[i].AnchorBottom) Anchors << akBottom;
      WinControl->Anchors = Anchors;
    }

  }

  Form->ShowModal();

  delete Form;
}

//---------------------------------------------------------------------------
void FASTCALL TForm_NewDialog::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_NEWDIALOG);
}

//---------------------------------------------------------------------------
bool TForm_NewDialog::ConstructionApercu(void) {

	switch (Choice) {
	case 1:
		return DialogueStandard(false);
	case 2:
		return DialogueStandard(true);
	case 3:
		return AnalyzeMemo();
	}

	return false;
}

//---------------------------------------------------------------------------
bool TForm_NewDialog::DialogueStandard(bool bButtons) {
  CONTROL NewControl;
  int TabOrder;


  TabOrder = 0;
	Controls.clear();

  asFormName = Edit_Unite->Text;
  asFormCaption = Edit_Caption->Text;
  FormWidth = 292 + 8;  // 8 = 2 * CXSIZEFRAME (nombre en dur aussi dans FreeVCL)
  FormHeight = 173 + 27;   // 27 = 2 * CYMARGEFRAME + CYCAPTION (idem)

	if (bButtons) {

		// Bouton "Ok"
		NewControl.asClass = _T("TButton");
		NewControl.asName = _T("Button_Ok");
		NewControl.asCaption = _T("Ok");
		NewControl.Left = 32;
		NewControl.Top = 134;
		NewControl.Width = 60;
		NewControl.Height = 23;
		NewControl.TabOrder = TabOrder++;
		NewControl.AnchorLeft = true;
		NewControl.AnchorTop = false;
		NewControl.AnchorRight = false;
		NewControl.AnchorBottom = true;
		NewControl.OnClick = true;
		Controls.push_back(NewControl);

		// Bouton "Annuler"
		NewControl.asClass = _T("TButton");
		NewControl.asName = _T("Button_Annuler");
		NewControl.asCaption = _T("Annuler");
		NewControl.Left = 112;
		NewControl.Top = 134;
		NewControl.Width = 60;
		NewControl.Height = 23;
		NewControl.TabOrder = TabOrder++;
		NewControl.AnchorLeft = true;
		NewControl.AnchorTop = false;
		NewControl.AnchorRight = false;
		NewControl.AnchorBottom = true;
		NewControl.OnClick = true;
		Controls.push_back(NewControl);

		// Bouton "Aide"
		NewControl.asClass = _T("TButton");
		NewControl.asName = _T("Button_Aide");
		NewControl.asCaption = _T("Aide");
		NewControl.Left = 192;
		NewControl.Top = 134;
		NewControl.Width = 60;
		NewControl.Height = 23;
		NewControl.TabOrder = TabOrder++;
		NewControl.AnchorLeft = true;
		NewControl.AnchorTop = false;
		NewControl.AnchorRight = false;
		NewControl.AnchorBottom = true;
		NewControl.OnClick = true;
		Controls.push_back(NewControl);

	}

	return true;
}

//---------------------------------------------------------------------------
bool TForm_NewDialog::AnalyzeMemo(void) {
  AnsiString asLigne;
  CharString csType, csVar, csCaption;
  TStringList * ListeMots;
  CONTROL NewControl;
  int TabOrder;
  bool bOpenDialog;
  bool bParcourir;
  int i, x, y;
  int MaxLength;


  ListeMots = new TStringList();
  x = 8;
  y = 6;
  TabOrder = 0;
  MaxLength = 0;
  bOpenDialog = false;
  Controls.clear();

  for (i = 0; i < (int) Memo_Struct->Lines->Count; i++) {
    asLigne = Memo_Struct->Lines->Strings[i];
    AnalyzeLine(asLigne, ListeMots);

    if (ListeMots->Count >= 2) {

      csType = ListeMots->Strings[0];
      csVar = ListeMots->Strings[1];
      if (ListeMots->Count >= 3) csCaption = ListeMots->Strings[2];
      else csCaption = csVar;

      bParcourir = (csCaption.LowerCase().Pos("chemin") != 0 ||
                    csCaption.LowerCase().Pos("fichier") != 0);

      if (csType == "int") {

        if (MaxLength < csCaption.Length()) MaxLength = csCaption.Length();

        NewControl.asClass = "TLabel";
        NewControl.asName = CharString("Label_") + csVar.c_str();
        NewControl.asCaption = csCaption.c_str();
        NewControl.Left = 8;
        NewControl.Top = y + 4;
        NewControl.Width = 100;
        NewControl.Height = 13;
        NewControl.TabOrder = -1;
        NewControl.AnchorLeft = true;
        NewControl.AnchorTop = true;
        NewControl.AnchorRight = false;
        NewControl.AnchorBottom = false;
        NewControl.OnClick = false;
        Controls.push_back(NewControl);

        NewControl.asClass = _T("TEdit");
        NewControl.asName = CharString("Edit_") + csVar.c_str();
        NewControl.asCaption = _T("");
        NewControl.Left = 108;
        NewControl.Top = y;
        NewControl.Width = 80;
        NewControl.Height = 21;
        NewControl.TabOrder = TabOrder++;
        NewControl.AnchorLeft = true;
        NewControl.AnchorTop = true;
        NewControl.AnchorRight = false;
        NewControl.AnchorBottom = false;
        NewControl.OnClick = false;
        Controls.push_back(NewControl);

        NewControl.asClass = _T("TUpDown");
        NewControl.asName = CharString("UpDown_") + csVar.c_str();
        NewControl.asCaption = _T("");
        NewControl.Left = 188;
        NewControl.Top = y;
        NewControl.Width = 12;
        NewControl.Height = 21;
        NewControl.TabOrder = -1;
        NewControl.AnchorLeft = true;
        NewControl.AnchorTop = true;
        NewControl.AnchorRight = false;
        NewControl.AnchorBottom = false;
        NewControl.OnClick = false;
        Controls.push_back(NewControl);

        if (x < NewControl.Left + NewControl.Width) x = NewControl.Left + NewControl.Width;
        y += 26;

      }

      if (csType == "char" || csType == "wchar_t" ||
          csType == "TCHAR" || csType == "AnsiString") {

        if (MaxLength < csCaption.Length()) MaxLength = csCaption.Length();

        NewControl.asClass = _T("TLabel");
        NewControl.asName = CharString("Label_") + csVar.c_str();
        NewControl.asCaption = csCaption.c_str();
        NewControl.Left = 8;
        NewControl.Top = y + 4;
        NewControl.Width = 100;
        NewControl.Height = 13;
        NewControl.TabOrder = -1;
        NewControl.AnchorLeft = true;
        NewControl.AnchorTop = true;
        NewControl.AnchorRight = false;
        NewControl.AnchorBottom = false;
        NewControl.OnClick = false;
        Controls.push_back(NewControl);

        NewControl.asClass = _T("TEdit");
        NewControl.asName = CharString("Edit_") + csVar.c_str();
        NewControl.asCaption = _T("");
        NewControl.Left = 108;
        NewControl.Top = y;
        if (bParcourir) NewControl.Width = 200;
        else NewControl.Width = 276;
        NewControl.Height = 21;
        NewControl.TabOrder = TabOrder++;
        NewControl.AnchorLeft = true;
        NewControl.AnchorTop = true;
        NewControl.AnchorRight = true;
        NewControl.AnchorBottom = false;
        NewControl.OnClick = false;
        Controls.push_back(NewControl);

        if (bParcourir) {

          if (!bOpenDialog) {
            NewControl.asClass = _T("TOpenDialog");
            NewControl.asName = CharString("OpenDialog_") + csVar.c_str();
            NewControl.asCaption = csCaption.c_str();
            NewControl.Left = 356;
            NewControl.Top = y + 21;
            NewControl.Width = -1;
            NewControl.Height = -1;
            NewControl.TabOrder = -1;
            NewControl.AnchorLeft = true;
            NewControl.AnchorTop = true;
            NewControl.AnchorRight = false;
            NewControl.AnchorBottom = false;
            NewControl.OnClick = false;
            Controls.push_back(NewControl);

            bOpenDialog = true;
          }

          NewControl.asClass = _T("TButton");
          NewControl.asName = CharString("Button_") + csVar.c_str();
          NewControl.asCaption = _T("Parcourir");
          NewControl.Left = 316;
          NewControl.Top = y;
          NewControl.Width = 68;
          NewControl.Height = 21;
          NewControl.TabOrder = TabOrder++;
          NewControl.AnchorLeft = false;
          NewControl.AnchorTop = true;
          NewControl.AnchorRight = true;
          NewControl.AnchorBottom = false;
          NewControl.OnClick = true;
          Controls.push_back(NewControl);
        }

        if (x < NewControl.Left + NewControl.Width) x = NewControl.Left + NewControl.Width;
        y += 26;

      }

      else if (csType == "bool") {

        NewControl.asClass = _T("TCheckBox");
        NewControl.asName = CharString("CheckBox_") + csVar.c_str();
        NewControl.asCaption = csCaption.c_str();
        NewControl.Left = 8;
        NewControl.Top = y + 2;
        NewControl.Width = 168;
        NewControl.Height = 17;
        NewControl.TabOrder = -1;
        NewControl.AnchorLeft = true;
        NewControl.AnchorTop = true;
        NewControl.AnchorRight = false;
        NewControl.AnchorBottom = false;
        NewControl.OnClick = false;
        Controls.push_back(NewControl);

        if (x < NewControl.Left + NewControl.Width) x = NewControl.Left + NewControl.Width;
        y += 24;

      }

    }

  }

  MaxLength *= 6;
  if (MaxLength > 100) {
    for (i = 0; i < (int) Controls.size(); i++) {
      if (Controls[i].Left < 108) Controls[i].Width += MaxLength - 100;
      else Controls[i].Left += MaxLength - 100;
    }
    x  += MaxLength - 100;
  }

  y += 14;

  NewControl.asClass = _T("TButton");
  NewControl.asName = _T("Button_Ok");
  NewControl.asCaption = _T("Ok");
  NewControl.Left = x / 2 - 78;
  NewControl.Top = y;
  NewControl.Width = 68;
  NewControl.Height = 23;
  NewControl.TabOrder = TabOrder++;
  NewControl.AnchorLeft = true;
  NewControl.AnchorTop = false;
  NewControl.AnchorRight = false;
  NewControl.AnchorBottom = true;
  NewControl.OnClick = true;
  Controls.push_back(NewControl);

  NewControl.asClass = _T("TButton");
  NewControl.asName = _T("Button_Annuler");
  NewControl.asCaption = _T("Annuler");
  NewControl.Left = x / 2 + 10;
  NewControl.Top = y;
  NewControl.Width = 68;
  NewControl.Height = 23;
  NewControl.TabOrder = TabOrder++;
  NewControl.AnchorLeft = true;
  NewControl.AnchorTop = false;
  NewControl.AnchorRight = false;
  NewControl.AnchorBottom = true;
  NewControl.OnClick = true;
  Controls.push_back(NewControl);

  y += 40;

  asFormName = Edit_Unite->Text;
  asFormCaption = Edit_Caption->Text;
  FormWidth = x + 8 + 8;  // 8 = Marge, 8 = 2 * CXSIZEFRAME (nombre en dur aussi dans FreeVCL)
  FormHeight = y + 27;   // 27 = 2 * CYMARGEFRAME + CYCAPTION (idem)

  delete ListeMots;

  return true;
}

//---------------------------------------------------------------------------
bool TForm_NewDialog::AnalyzeLine(AnsiString asLigne, TStrings * ListeMots) {
  int i;
  TCHAR c;
  bool bPointeur, bSize, bDebComm, bCommentaire;
  AnsiString asMot;


  bPointeur = false;
  bSize = false;
  bDebComm = false;
  bCommentaire = false;
  ListeMots->Clear();

  for (i = 1; i <= asLigne.Length(); i++) {
    c = asLigne[i];

    switch (c) {
    case _T('/'):
      bDebComm = true;
      break;
    case _T('*'):
    case _T('['):
      bPointeur = true;
      bSize = true;
      break;
    case _T(']'):
      bPointeur = true;
      bSize = false;
      break;
    case _T(' '):
    case _T('\t'):
      if (!bCommentaire) {
        if (asMot != _T("")) {
          ListeMots->Add(asMot);
          asMot = _T("");
        }
      }
      else asMot += c;
      break;
    default:
      if (bDebComm) bCommentaire = true;
      if (!bSize) asMot += c;
      break;
    }

  }

  if (asMot != _T("")) {
    ListeMots->Add(asMot);
    asMot = _T("");
  }

  return true;
}

//---------------------------------------------------------------------------
BOOL TForm_NewDialog::EnumWnd(HWND hWnd) {
  DWORD WindowStyle;


  WindowStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (WindowStyle & WS_CAPTION) {
		TCHAR szClassName[512];
		TCHAR szCaption[512];
		GetClassName(hWnd, szClassName, _countof(szClassName));
		GetWindowText(hWnd, szCaption, _countof(szCaption));
		ListBox_Windows->Items->AddObject(AnsiString(szClassName) + _T(" - ") + szCaption, (TObject *) hWnd);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CALLBACK TForm_NewDialog::EnumWnd(HWND hWnd, LPARAM lParam) {
	TForm_NewDialog *Form_NewDialog = (TForm_NewDialog *) lParam;
	return Form_NewDialog->EnumWnd(hWnd);
}
//---------------------------------------------------------------------------
