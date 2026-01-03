//-----------------------------------------------------------------------------
//! @file TForm_SaisieStructure.cpp
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
#include "TFVCLIDEAutomation.h"
#include "TForm_SaisieStructure.h"

//---------------------------------------------------------------------------
TForm_SaisieStructure::TForm_SaisieStructure(TComponent* Owner, const TCHAR *szName)
  : TForm(Owner, szName, FVCL_ARG_SUPP) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieStructure::Button_ParcourirClick(TObject *Sender) {
  AnsiString asRep;


  if (SelectDirectory(_TT("Répertoire du projet Builder"), _T(""), asRep,
      TSelectDirOpts() << sdAllowCreate << sdBrowseForComputer << sdShowEdit)) {
    Edit_Repertoire->Text = asRep;
    Edit_Repertoire->SetFocus();
  }
}

//---------------------------------------------------------------------------
void FASTCALL TForm_SaisieStructure::Button_ApercuClick(TObject *Sender) {
  TForm *Form;
  TWinControl *WinControl;
  TLabel *Label = NULL;
  TButton *Button = NULL;
  TEdit *Edit = NULL;
  TUpDown *UpDown = NULL;
  TCheckBox *CheckBox = NULL;
  TAnchors Anchors;
  int i;


  if (!AnalyzeMemo()) return;

  Form = new TForm(this);
  Form->Width = FormWidth + 2 * GetSystemMetrics(SM_CXSIZEFRAME) - 8;
  Form->Height = FormHeight + 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) - 27;
  Form->Position = poOwnerFormCenter;
  Form->Caption = asFormCaption;

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
void FASTCALL TForm_SaisieStructure::Button_CreerClick(TObject *Sender) {
  AnsiString asRep;
  AnsiString asNomFichier;
	AnsiString asUnite;
	AnsiString asMsg;
  CharString csLigne, csUnite, asBuf;
  HANDLE hFile;
  DWORD dw;
	int Ret;
  int i;
  bool Ok = true;


  /*
  int Left           // Gauche
  int Top            // Haut
  bool bTruc         // Machin chose
  int Width          // Largeur
  int Height         // Hauteur
  char Path          // Chemin
  */

  /*
  char Nom[128];       // Nom
  char Prenom[128];    // Prénom
  char Adresse[128];   // Adresse
  int CodePostal;      // Code postal
  char Ville[64];      // Ville
  char PathPhoto[128]; // Fichier photo
  bool bAdherent;      // Adhérent
  */

  if (!AnalyzeMemo()) return;

  //--------------------------------------------------------------------------
  //                               CPP
  //--------------------------------------------------------------------------

  asRep = Edit_Repertoire->Text;
  asUnite = Edit_Unite->Text;
	csUnite = asUnite.AsCharString();
  asNomFichier = asRep + _T("\\T") + asUnite + _T(".cpp");

  if (FileExists(asNomFichier)) {
		asMsg = _TT("Ce fichier existe déjà, voulez-vous l'écraser ?\n") + asNomFichier;
		Ret = FVCLIDEAutomation->DisplayError(asMsg,
																					_TT("Enregistrement fichier .cpp"),
																					_T(""),
																					MB_YESNOCANCEL | MB_ICONQUESTION);
		if (Ret != IDYES) return;
  }

  hFile = CreateFile(asNomFichier.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
                     NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //! @file TForm_SaisieStructure.cpp
    csLigne.sprintf("//! @file T%s.cpp\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // #include <FreeVCL.h>
    csLigne = "#include <FreeVCL.h>\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // #include "TForm_SaisieStructure.h"
    csLigne.sprintf("#include \"T%s.h\"\r\n\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // TForm_SaisieStructure::TForm_SaisieStructure(TComponent* Owner, const TCHAR *szName)
    csLigne.sprintf("T%s::T%s(TComponent* Owner, LPTSTR szName)\r\n",
                    csUnite.c_str(), csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    //   : TForm(Owner, szName, FVCL_ARG_SUPP) {
    csLigne.sprintf("    : TForm(Owner, szName, FVCL_ARG_SUPP) {\r\n");
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // }
    csLigne = "}\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // void TForm_SaisieStructure::CallEvent(AnsiString asEvent, TObject *Sender, ...) {
    csLigne.sprintf("void T%s::CallEvent(AnsiString asEvent, TObject *Sender, ...) {\r\n",
                    csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // BYTE *pArgs;
    csLigne.sprintf("  BYTE *pArgs;\r\n\r\n\r\n");
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // pArgs = (BYTE *) &Sender;
    csLigne.sprintf("  pArgs = (BYTE *) &Sender;\r\n");
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // pArgs += sizeof(TObject *);
    csLigne.sprintf("  pArgs += sizeof(TObject *);\r\n\r\n");
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    asBuf = "";
    for (i = 0; i < (int) Controls.size(); i++) {

      if (Controls[i].OnClick) {
        // if (asEvent == _T("Button_OkClick")) Button_OkClick(Sender);
        csLigne.sprintf("  %sif (asEvent == _T(\"%sClick\")) %sClick(Sender);\r\n",
                        asBuf.c_str(), Controls[i].asName.c_str(), Controls[i].asName.c_str());
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
        asBuf = "else ";
      }

    }

    // }
    csLigne = "\r\n}\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    for (i = 0; i < (int) Controls.size(); i++) {

      if (Controls[i].OnClick) {
        // //------------
        csLigne = "//---------------------------------------------------------------------------\r\n";
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

        // void FASTCALL TForm_SaisieStructure::Button_OkClick(TObject *Sender) {
        csLigne.sprintf("void FASTCALL T%s::%sClick(TObject *Sender) {\r\n",
                        csUnite.c_str(), Controls[i].asName.c_str());
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

        // //
        csLigne = " //\r\n";
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

        // }
        csLigne = "}\r\n\r\n";
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

      }

    }

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    CloseHandle(hFile);
  }
  else {
    // Message d'erreur ouverture asNomFichier
    Application->MessageBox(_T("Impossible de créer le fichier"), asNomFichier.c_str(), MB_OK | MB_ICONSTOP);
    Ok = false;
  }

  //--------------------------------------------------------------------------
  //                                H
  //--------------------------------------------------------------------------

  asNomFichier = asRep + _T("\\T") + asUnite.AsAnsiString() + _T(".h");

  if (FileExists(asNomFichier)) {
		asMsg = _TT("Ce fichier existe déjà, voulez-vous l'écraser ?\n") + asNomFichier;
		Ret = FVCLIDEAutomation->DisplayError(asMsg,
																					_TT("Enregistrement fichier .h"),
																					_T(""),
																					MB_YESNOCANCEL | MB_ICONQUESTION);
		if (Ret != IDYES) return;
  }

  hFile = CreateFile(asNomFichier.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
                     NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //! @file TForm_SaisieStructure.h
    csLigne.sprintf("//! @file T%s.h\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // #ifndef TForm_SaisieStructureH
    csLigne.sprintf("#ifndef T%sH\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // #define TForm_SaisieStructureH
    csLigne.sprintf("#define T%sH\r\n\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // class TForm_SaisieStructure : public TForm {
    csLigne.sprintf("class T%s: public TForm {\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // private:
    csLigne = "private:\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // protected:
    csLigne = "protected:\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // public:
    csLigne = "public:\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // FVCL_BEGIN_COMPONENTS
    csLigne = "  FVCL_BEGIN_COMPONENTS\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    for (i = 0; i < (int) Controls.size(); i++) {

      // TButton *Button1;
      csLigne.sprintf("  %s *%s;\r\n", Controls[i].asClass.c_str(), Controls[i].asName.c_str());
      WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    }

    // FVCL_END_COMPONENTS
    csLigne = "  FVCL_END_COMPONENTS\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // FVCL_BEGIN_EVENTS
    csLigne = "  FVCL_BEGIN_EVENTS\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    for (i = 0; i < (int) Controls.size(); i++) {

      if (Controls[i].OnClick) {
        // void FASTCALL Button_OkClick(TObject *Sender);
        csLigne.sprintf("  void FASTCALL %sClick(TObject *Sender);\r\n", Controls[i].asName.c_str());
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

    }

    // FVCL_END_EVENTS
    csLigne = "  FVCL_END_EVENTS\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // FASTCALL TForm_SaisieStructure(TComponent* Owner);
    csLigne.sprintf("  T%s(TComponent* Owner, LPTSTR szName);\r\n\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // };
    csLigne = "};\r\n\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // //------------
    csLigne = "//---------------------------------------------------------------------------\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // #endif  // TForm_SaisieStructureH
    csLigne.sprintf("#endif  // T%sH\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    CloseHandle(hFile);
  }
  else {
    // Message d'erreur ouverture asNomFichier
    Application->MessageBox(_TT("Impossible de créer le fichier"), asNomFichier.c_str(), MB_OK | MB_ICONSTOP);
    Ok = false;
  }

  //--------------------------------------------------------------------------
  //                               DFM
  //--------------------------------------------------------------------------

  asNomFichier = asRep + AnsiString(_T("\\T")) + asUnite.AsAnsiString() + _T(".dfm");

  if (FileExists(asNomFichier)) {
		asMsg = _TT("Ce fichier existe déjà, voulez-vous l'écraser ?\n") + asNomFichier;
		Ret = FVCLIDEAutomation->DisplayError(asMsg,
																					_TT("Enregistrement fichier .dfm"),
																					_T(""),
																					MB_YESNOCANCEL | MB_ICONQUESTION);
		if (Ret != IDYES) return;
  }

  hFile = CreateFile(asNomFichier.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
                     NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

    // object Form_SaisieStructure: TForm_SaisieStructure
    csLigne.sprintf("object %s: T%s\r\n", csUnite.c_str(), csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // Left = 100
    csLigne.sprintf("  Left = %i\r\n", 100);
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // Top = 100
    csLigne.sprintf("  Top = %i\r\n", 100);
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // Width = 358
    csLigne.sprintf("  Width = %i\r\n", FormWidth);
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // Height = 275
    csLigne.sprintf("  Height = %i\r\n", FormHeight);
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    // Caption = 'Form_SaisieStructure'
    csLigne.sprintf("  Caption = \'%s\'\r\n", csUnite.c_str());
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    for (i = 0; i < (int) Controls.size(); i++) {

      // object Button1: TButton
      csLigne.sprintf("  object %s: %s\r\n", Controls[i].asName.AsCharString().c_str(), Controls[i].asClass.AsCharString().c_str());
      WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

      // Left = 66
      csLigne.sprintf("    Left = %i\r\n", Controls[i].Left);
      WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

      // Top = 69
      csLigne.sprintf("    Top = %i\r\n", Controls[i].Top);
      WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

      // Width = 229
      if (Controls[i].Width != -1) {
        csLigne.sprintf("    Width = %i\r\n", Controls[i].Width);
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

      // Height = 115
      if (Controls[i].Height != -1) {
        csLigne.sprintf("    Height = %i\r\n", Controls[i].Height);
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

      // Anchors = [akLeft, akTop, akRight, akBottom]
      if (!Controls[i].AnchorLeft ||
          !Controls[i].AnchorTop ||
          Controls[i].AnchorRight ||
          Controls[i].AnchorBottom) {
        csLigne = "    Anchors = [";
        if (Controls[i].AnchorLeft) csLigne += "akLeft, ";
        if (Controls[i].AnchorTop) csLigne += "akTop, ";
        if (Controls[i].AnchorRight) csLigne += "akRight, ";
        if (Controls[i].AnchorBottom) csLigne += "akBottom, ";
        if (csLigne.Length() > 15) {
          csLigne = csLigne.SubString(1, csLigne.Length() - 2);
        }
        csLigne += "]\r\n";
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

      // Caption = 'Button1'
      if (Controls[i].asCaption != _T("")) {
        csLigne.sprintf("    Caption = \'%s\'\r\n", Controls[i].asCaption.AsCharString().c_str());
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

      // TabOrder = 0
      if (Controls[i].TabOrder != -1) {
        csLigne.sprintf("    TabOrder = %i\r\n", Controls[i].TabOrder);
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

      // Filter = 'Tous (*.*)|*.*'
      if (Controls[i].asClass == _T("TOpenDialog")) {
        csLigne = "    Filter = \'Tous (*.*)|*.*\'\r\n";
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

      if (Controls[i].OnClick) {
        // OnClick = Button1Click
        csLigne.sprintf("    OnClick = %sClick\r\n", Controls[i].asName.c_str());
        WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);
      }

      // end
      csLigne = "  end\r\n";
      WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    }

    // end
    csLigne = "end\r\n";
    WriteFile(hFile, csLigne.c_str(), csLigne.Length(), &dw, NULL);

    CloseHandle(hFile);
  }
  else {
    // Message d'erreur ouverture asNomFichier
		asMsg.sprintf(_TT("Impossible de créer le fichier %s"), asNomFichier.c_str());
		FVCLIDEAutomation->DisplaySystemError(asMsg, GetLastError(), _TT("Création DFM"), _T(""), MB_OK | MB_ICONSTOP);
    Ok = false;
  }

  if (Ok) ModalResult = mrOk;
	
}

//---------------------------------------------------------------------------
bool TForm_SaisieStructure::AnalyzeMemo(void) {
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

  asFormCaption = Edit_Caption->Text;
  FormWidth = x + 8 + 8;  // 8 = Marge, 8 = 2 * CXSIZEFRAME (nombre en dur aussi dans FreeVCL)
  FormHeight = y + 23;   // 23 = 2 * CYMARGEFRAME + CYCAPTION (idem)

  delete ListeMots;

  return true;
}

//---------------------------------------------------------------------------
bool TForm_SaisieStructure::AnalyzeLine(AnsiString asLigne, TStrings * ListeMots) {
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
