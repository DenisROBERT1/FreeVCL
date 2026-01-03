//-----------------------------------------------------------------------------
//! @file TFileForm.cpp
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

#include <windows.h>
#include <FreeVcl.h>

#include "TForm_DisplayError.h"
#include "TFrame_SaisieDialog.h"
#include "TElemVcl.h"
#include "TFileForm.h"

//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TFileForm::TFileForm(void) {
  NbStatics = 1000;
  NewFormElemVCL = NULL;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TFileForm::~TFileForm(void) {
}


//---------------------------------------------------------------------------
AnsiString TFileForm::UnMot(AnsiString asLigne, AnsiString asDelim1, AnsiString asDelim2, int * Cur) {
  int Deb, Fin, Len;


  Len = asLigne.Length();
  if (*Cur >= (int) Len) {
    *Cur = Len + 1;
    return _T("");
  }

  Deb = (int) asLigne.find_first_not_of(asDelim1, *Cur);
  if (Deb == -1) Deb = Len;
  Fin = (int) asLigne.find_first_of(asDelim2, Deb);
  if (Fin == -1) Fin = Len;
  *Cur = Fin + 1;

  return AnsiString(asLigne.substr(Deb, Fin - Deb));
}

//---------------------------------------------------------------------------
AnsiString TFileForm::UnArg(AnsiString asLigne, int * Cur) {
  return UnTruc(asLigne, Cur, _T(','));
}

//---------------------------------------------------------------------------
AnsiString TFileForm::UneOption(AnsiString asLigne, int * Cur) {
  return UnTruc(asLigne, Cur, _T('|'));
}

//---------------------------------------------------------------------------
AnsiString TFileForm::UnTexte(AnsiString asLigne, int * Cur) {
  AnsiString asMot1, asMot2;
  int Deb, Fin, Len;
  bool bGuill;


  // Chaîne + espaces
  Len = asLigne.Length();
  Deb = *Cur + 1;
  if (Deb > Len) return _T("");
  Fin = Deb;
  bGuill = false;
  while (Fin <= Len && (asLigne[Fin] != _T(',') || bGuill)) {
    if (asLigne[Fin] == _T('\"')) bGuill = !bGuill;
    Fin++;
  }
  asMot1 = asLigne.SubString(Deb, Fin - Deb);
  *Cur = Fin;

  // Suppression des espaces
  Len = asMot1.Length();
  Deb = 1;
  while (Deb <= Len && asMot1[Deb] == _T(' ')) Deb++;
  Fin = Len;
  while (Fin >= Deb && asMot1[Fin] == _T(' ')) Fin--;
  asMot1 = asMot1.SubString(Deb, Fin - Deb + 1);

  // Suppression des guillemets
  Len = asMot1.Length();
  Deb = 1;
  if (Deb <= Len && asMot1[Deb] == _T('\"')) Deb++;
  Fin = Len;
  if (Fin >= Deb && asMot1[Fin] == _T('\"')) Fin--;
  asMot2 = asMot1.SubString(Deb, Fin - Deb + 1);

  return asMot2;
}

//---------------------------------------------------------------------------
AnsiString TFileForm::UnTruc(AnsiString asLigne, int * Cur, char Delim) {
  AnsiString asMot1, asMot2;
  int Deb, Fin, Len;


  // Mot + espaces
  Len = asLigne.Length();
  Deb = *Cur + 1;
  if (Deb > Len) return _T("");
  Fin = Deb;
  while (Fin <= Len && asLigne[Fin] != Delim) Fin++;
  asMot1 = asLigne.SubString(Deb, Fin - Deb);
  *Cur = Fin;

  // Suppression des espaces
  Len = asMot1.Length();
  Deb = 1;
  while (Deb <= Len && asMot1[Deb] == _T(' ')) Deb++;
  Fin = Len;
  while (Fin >= Deb && asMot1[Fin] == _T(' ')) Fin--;
  asMot2 = asMot1.SubString(Deb, Fin - Deb + 1);

  return asMot2;
}

//---------------------------------------------------------------------------
void TFileForm::OrdreDesComposants(TForm * Form) {
  int i;
  int Pos = 0;


  for (i = 0; i < Form->ComponentCount; i++) {
    if (Form->Components[i]->ClassNameIs(_T("TPageControl")) ||
        Form->Components[i]->ClassNameIs(_T("TGroupBox")) ||
        Form->Components[i]->ClassNameIs(_T("TPanel"))) {
        Form->Components[i]->ComponentIndex = Pos++;
    }
  }
}

//---------------------------------------------------------------------------
AnsiString TFileForm::AddGuillVCL(AnsiString asTexte) {
  AnsiString asBuf, asRetour;
  int i;
  TCHAR c;
  bool bGuillemets;


  bGuillemets = false;

  for (i = 1; i <= asTexte.Length(); i++) {
    c = asTexte[i];

    if (c == _T('\'') || c < 0) {
      if (bGuillemets) {
        asRetour += _T('\'');
        bGuillemets = false;
      }
      asBuf.sprintf(_T("#%i"), (unsigned int) c);
      asRetour += asBuf;
    }
    else {
      if (!bGuillemets) {
        asRetour += _T('\'');
        bGuillemets = true;
      }
      asRetour += c;
    }

  }

  if (bGuillemets) {
    asRetour += _T('\'');
  }

  if (asRetour.IsEmpty()) asRetour = _T("\'\'");

  return asRetour;
}

//---------------------------------------------------------------------------
AnsiString TFileForm::SuppGuillVCL(AnsiString asTexte) {
  int i;
  TCHAR c;
  bool bGuill;
  AnsiString asRetour;
  AnsiString asCarac;


  asRetour = _T("");
  asCarac = _T("");
  bGuill = false;

  // Il n'y a pas nécessairement un guillemet ni au début ni à la fin
  // Exemple: Caption = #39
  for (i = 1; i <= (int) asTexte.Length(); i++) {
    c = asTexte[i];
    if (c == _T('\'')) {
      if (!bGuill && asCarac.Length() >= 2 && asCarac[1] == _T('#')) {
        asCarac = asCarac.SubString(2, 3);
        c = StrToInt(asCarac);
        asRetour += c;
      }
      bGuill = !bGuill;
    }
    else if (!bGuill && c == _T('#')) {
      if (asCarac.Length() >= 2 && asCarac[1] == _T('#')) {
        asCarac = asCarac.SubString(2, 3);
        c = StrToInt(asCarac);
        asRetour += c;
      }
      asCarac = _T("#");
    }
    else {
      if (bGuill) {
        asCarac = _T("");
        asRetour += c;
      }
      else if (c != _T(' ')) {
        asCarac += c;
      }
    }
  }
  if (!bGuill && asCarac.Length() >= 2 && asCarac[1] == _T('#')) {
    asCarac = asCarac.SubString(2, 3);
    c = StrToInt(asCarac);
    asRetour += c;
  }

  return asRetour;
}

//---------------------------------------------------------------------------
AnsiString TFileForm::SuppGuillFRM(AnsiString asTexte) {
  int i;
  TCHAR c;
	bool bEscape;
  AnsiString asRetour;


  asRetour = _T("");
	bEscape = false;

  for (i = 1; i <= (int) asTexte.Length(); i++) {
    c = asTexte[i];
    if (c != _T('"') || bEscape) {
      asRetour += c;
			bEscape = false;
    }
		else {
			bEscape = true;
		}
  }

  return asRetour;
}

//---------------------------------------------------------------------------
bool TFileForm::GetBaseUnit(HFONT hFont, int *baseunitX, int *baseunitY) {
  HDC hdc;
  HFONT hOldFont;
  TEXTMETRIC tm;
  SIZE Size;


  hdc = GetDC(NULL);
  hOldFont = (HFONT) SelectObject(hdc, hFont);
  GetTextMetrics(hdc, &tm);
  GetTextExtentPoint32(hdc,
                       _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"),
                       52, &Size);
  *baseunitX = (Size.cx / 26 + 1) / 2;
  *baseunitY = (WORD) tm.tmHeight;
  SelectObject(hdc, hOldFont);
  DeleteObject(hFont);
  ReleaseDC(NULL, hdc);

  return true;
}

//---------------------------------------------------------------------------
bool TFileForm::SetProperty(TElemVCL *ElemVCL,
                                       AnsiString asProperty,
                                       AnsiString asValue) {

	if (!asValue.IsEmpty() &&	(asValue[1] == _T('\'') || asValue[1] == _T('#'))) asValue = SuppGuillVCL(asValue);

	return ElemVCL->SetProperty(asProperty, asValue);
}

//---------------------------------------------------------------------------
bool TFileForm::OpenRC(AnsiString asFileName) {
  TComponent *Component;
  TElemVCL * NewElemVCL;
  TElemVCL * TabSheet;
  TLabel *NewLabel;
  TGroupBox *NewGroupBox;
  TMemo *NewMemo;
  TEdit *NewEdit;
  TListBox *NewListBox;
  TComboBox *NewComboBox;
  TButton *NewButton;
  TCheckBox *NewCheckBox;
  TRadioButton *NewRadioButton;
  TPanel *NewPanel;
	TPaintBox *NewPaintBox;
  HANDLE hFile;
  DWORD dw;
  char c;
	AnsiString asFontHeight, asFontName;
  AnsiString asLigne, asLigne2, asMot1, asMot2, asMot3, asMot4, asMot5, asMot;
  AnsiString asName, asCaption;
  bool bDialog;
	int NbBegin;
  int Cur, Cur2;
  int X = 0, Y = 0, H = 0, L = 0;
  DWORD WindowStyle;
  bool bEof;
  HFONT hFont;
  int tmHeight;
  int baseunitX, baseunitY;


  hFile = CreateFile(asFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		NewFormElemVCL = NULL;
    NbBegin = 0;
    bDialog = false;
    bEof = false;
    do {

      asLigne = _T("");
      do {
        asLigne2 = _T("");
        do {
          ReadFile(hFile, &c, 1, &dw, NULL);
          if (dw == 0) bEof = true;
          else if (c != '\r' && c != '\n') asLigne2 += (unsigned char) c;
        } while (!bEof && c != '\n');

        asLigne += asLigne2;

      } while (!bEof && !asLigne2.IsEmpty() && (asLigne2[asLigne2.Length()] == _T(',') || asLigne2[asLigne2.Length()] == _T('|')));


      Cur = 0;

      // CONTROL         "Tree1",IDD_TREEVIEW,"SysTreeView32",TVS_HASBUTTONS |
      //                 TVS_HASLINES | TVS_LINESATROOT | WS_BORDER | WS_TABSTOP,
      //                 4,4,140,300,WS_EX_CLIENTEDGE

      // SAISWMFBOX DIALOG DISCARDABLE  25, 18, 305, 212
      // CAPTION "Fichier metafile"
      // FONT 8, "MS Sans Serif"
      // BEGIN
      //     LTEXT           "Nom de l'élément:",IDC_STATIC,5,6,65,8
      //     GROUPBOX        "Point inférieur gauche",IDC_STATIC,5,16,145,58
      //     EDITTEXT        IDD_NOM,70,4,80,12,ES_AUTOHSCROLL
      //     LTEXT           "X (cm) =",IDC_STATIC,10,32,30,8
      //     EDITTEXT        IDD_X1,40,30,35,12,ES_AUTOHSCROLL
      //     LTEXT           "Y (cm) =",IDC_STATIC,10,46,30,8
      //     EDITTEXT        IDD_Y1,40,44,35,12,ES_AUTOHSCROLL
      //     LTEXT           "Par rapport au point:",IDC_STATIC,10,60,70,10
      //     CONTROL         "",IDC_STATIC,"Static",SS_BLACKFRAME,95,30,45,38
      //     CONTROL         "",IDD_SG1,"Button",BS_AUTORADIOBUTTON | BS_LEFTTEXT,90,
      //                     26,10,10
      //     LISTBOX         IDLISTE,8,8,272,136,LBS_SORT | LBS_OWNERDRAWFIXED |
      //                     LBS_NOINTEGRALHEIGHT | LBS_MULTICOLUMN |
      //                     LBS_WANTKEYBOARDINPUT | WS_HSCROLL | WS_TABSTOP
      //     COMBOBOX        IDD111_TYPE,48,88,112,104,CBS_DROPDOWNLIST | WS_TABSTOP
      //     DEFPUSHBUTTON   "Ok",IDOK,114,192,40,14,WS_DISABLED
      //     PUSHBUTTON      "Annuler",IDCANCEL,170,192,40,14
      //     PUSHBUTTON      "Aide",IDHELP,226,192,40,14
      // END

      if (!bDialog) {

        asMot1 = UnMot(asLigne, _T("\t "), _T("\t "), &Cur);
        asMot2 = UnMot(asLigne, _T("\t "), _T("\t "), &Cur);
        if (asMot1 == _T("BEGIN")) {
          NbBegin++;
        }
        else if (asMot1 == _T("END")) {
          if (NbBegin > 0) NbBegin--;
        }
        else if (asMot2 == _T("DIALOG") && NbBegin == 0) {
          UnMot(asLigne, _T("\t "), _T("\t "), &Cur);  // Mot "DISCARDABLE"

          asMot = UnArg(asLigne, &Cur);
          X = asMot.ToInt();
          asMot = UnArg(asLigne, &Cur);
          Y = asMot.ToInt();
          asMot = UnArg(asLigne, &Cur);
          L = asMot.ToInt();
          asMot = UnArg(asLigne, &Cur);
          H = asMot.ToInt();

          asName = AnsiString(_T("Form_")) + asMot1;
          asCaption = "";
          bDialog = true;
          NbBegin = 0;
        }
        else if (asMot2 == _T("DIALOGEX") && NbBegin == 0) {

          asMot = UnArg(asLigne, &Cur);
          X = asMot.ToInt();
          asMot = UnArg(asLigne, &Cur);
          Y = asMot.ToInt();
          asMot = UnArg(asLigne, &Cur);
          L = asMot.ToInt();
          asMot = UnArg(asLigne, &Cur);
          H = asMot.ToInt();

          asName = AnsiString(_T("Form_")) + asMot1;
          asCaption = "";
          bDialog = true;
          NbBegin = 0;
        }
        else if (asMot2 == _T("MENU") && NbBegin == 0) {

          X = 0;
          Y = 0;
          L = 1;
          H = 1;

          asName = AnsiString(_T("Menu_")) + asMot1;
          asCaption = "";
          bDialog = true;
          NbBegin = 0;

          // Unités du menu
          baseunitX = 4;
					baseunitY = 2;
				}

      }
      else {

        if (NbBegin == 0) {

          asMot1 = UnMot(asLigne, _T("\t "), _T("\t "), &Cur);
          if (asMot1 == _T("CAPTION")) {
            asCaption = UnTexte(asLigne, &Cur);
          }
          else if (asMot1 == _T("FONT")) {
            asFontHeight = UnArg(asLigne, &Cur);
            tmHeight = asFontHeight.ToInt();
            asFontName = UnTexte(asLigne, &Cur);

            // Unités des boîtes de dialogue
            hFont = CreateFont(tmHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                               DEFAULT_PITCH | FF_DONTCARE,
                               asFontName);
            GetBaseUnit(hFont, &baseunitX, &baseunitY);
          }
          else if (asMot1 == _T("BEGIN")) {
            NewFormElemVCL = new TElemVCL(NULL, NULL);
            if (NewFormElemVCL->Create(_T("TForm"))) {
              // NewFormElemVCL = new TForm(this);
							if (!asFontHeight.IsEmpty() && !asFontName.IsEmpty()) {
								NewFormElemVCL->SetProperty(_T("Font.Height"), asFontHeight);
								NewFormElemVCL->SetProperty(_T("Font.Name"), asFontName);
							}
              NewFormElemVCL->Left = X_PIXEL(X) - GetSystemMetrics(SM_CXFRAME);
              if (NewFormElemVCL->Left < 0) NewFormElemVCL->Left = 0;
              NewFormElemVCL->Top = Y_PIXEL(Y) - GetSystemMetrics(SM_CYCAPTION)
                                     - GetSystemMetrics(SM_CYFRAME);
              if (NewFormElemVCL->Top < 0) NewFormElemVCL->Top = 0;
              NewFormElemVCL->Width = X_PIXEL(L) + 2 * GetSystemMetrics(SM_CXFRAME);
              NewFormElemVCL->Height = Y_PIXEL(H) + GetSystemMetrics(SM_CYCAPTION)
                                        + 2 * GetSystemMetrics(SM_CYFRAME);
              NewFormElemVCL->Name = asName;
              NewFormElemVCL->SetProperty(_T("Caption"), asCaption);
              NewFormElemVCL->SetProperty(_T("AutoScroll"), _T("False"));

	            OnNewFormInit(this, NewFormElemVCL);

              NbBegin++;
            }
          }

        }
        else {

          asMot1 = UnMot(asLigne, _T("\t "), _T("\t "), &Cur);

          if (asMot1 == _T("LTEXT") || asMot1 == _T("CTEXT") || asMot1 == _T("RTEXT")) {
            asMot2 = UnTexte(asLigne, &Cur);
            asMot3 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TLabel"))) {
              NewElemVCL->SetProperty(_T("AutoSize"), _T("False"));
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
              Component = NewElemVCL->Component;
              NewLabel = (TLabel *) Component;
              NewLabel->Caption = asMot2;
              if (asMot1 == _T("LTEXT")) NewLabel->Alignment = taLeftJustify;
              else if (asMot1 == _T("CTEXT")) NewLabel->Alignment = taCenter;
              else if (asMot1 == _T("RTEXT")) NewLabel->Alignment = taRightJustify;
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("GROUPBOX")) {
            asMot2 = UnTexte(asLigne, &Cur);
            asMot3 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TGroupBox"))) {
              // NewElemVCL = new TGroupBox(NewFormElemVCL);
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
              Component = NewElemVCL->Component;
              NewGroupBox = (TGroupBox *) Component;
              NewGroupBox->Caption = asMot2;
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("EDITTEXT")) {
            asMot2 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

            WindowStyle = 0;
            do {
              asMot = UneOption(asLigne, &Cur);
              if (asMot.IsEmpty()) break;
              else if (asMot == _T("WS_TABSTOP")) WindowStyle |= WS_TABSTOP;
              else if (asMot == _T("WS_VSCROLL")) WindowStyle |= WS_VSCROLL;
              else if (asMot == _T("ES_MULTILINE")) WindowStyle |= ES_MULTILINE;
              else if (asMot == _T("ES_AUTOHSCROLL")) WindowStyle |= ES_AUTOHSCROLL;
              else if (asMot == _T("ES_AUTOVSCROLL")) WindowStyle |= ES_AUTOVSCROLL;
              else if (asMot == _T("ES_WANTRETURN")) WindowStyle |= ES_WANTRETURN;
              else if (asMot == _T("ES_PASSWORD")) WindowStyle |= ES_PASSWORD;
            } while (true);

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (WindowStyle & ES_MULTILINE) NewElemVCL->Create(_T("TMemo"));
            else NewElemVCL->Create(_T("TEdit"));
            if (NewElemVCL->Component) {
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot2 != _T("IDC_STATIC") && asMot2 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot2;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
              Component = NewElemVCL->Component;
	            if (WindowStyle & ES_MULTILINE) {
								NewMemo = (TMemo *) Component;
								NewMemo->Text = _T("");
								if ((WindowStyle & WS_TABSTOP) == 0) {
									NewMemo->TabStop = false;
								}
								if ((WindowStyle & ES_AUTOHSCROLL) != 0 && (WindowStyle & ES_AUTOVSCROLL) != 0) {
									NewMemo->ScrollBars = ssBoth;
								}
								else if ((WindowStyle & ES_AUTOHSCROLL) != 0) {
									NewMemo->ScrollBars = ssHorizontal;
								}
								else if ((WindowStyle & ES_AUTOVSCROLL) != 0) {
									NewMemo->ScrollBars = ssVertical;
								}
								if ((WindowStyle & ES_WANTRETURN) != 0) {
									NewMemo->WantReturns = true;
								}
							}
							else {
								NewEdit = (TEdit *) Component;
								NewEdit->Text = _T("");
								if ((WindowStyle & WS_TABSTOP) == 0) {
									NewEdit->TabStop = false;
								}
								if ((WindowStyle & ES_PASSWORD) != 0) {
									NewEdit->PasswordChar = '*';
								}
							}
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("LISTBOX")) {
            asMot2 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

						WindowStyle = 0;

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TListBox"))) {
              // NewElemVCL = new TListBox(NewFormElemVCL);
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot2 != _T("IDC_STATIC") && asMot2 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot2;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
							do {
								asMot = UneOption(asMot4, &Cur2);
								if (asMot.IsEmpty()) break;
								else if (asMot == _T("WS_TABSTOP")) WindowStyle |= WS_TABSTOP;
								else if (asMot == _T("LBS_SORT")) WindowStyle |= LBS_SORT;
							} while (true);

							Component = NewElemVCL->Component;
              NewListBox = (TListBox *) Component;
              if ((WindowStyle & WS_TABSTOP) == 0) {
                NewListBox->TabStop = false;
							}
							if ((WindowStyle & LBS_SORT) == 0) {
								NewListBox->Sorted = false;
							}
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("COMBOBOX")) {
            asMot2 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

						WindowStyle = 0;

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TComboBox"))) {
              // NewElemVCL = new TComboBox(NewFormElemVCL);
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot2 != _T("IDC_STATIC") && asMot2 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot2;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
              Component = NewElemVCL->Component;
              NewComboBox = (TComboBox *) Component;
              NewComboBox->Text = _T("");
              if ((WindowStyle & WS_TABSTOP) == 0) {
                NewComboBox->TabStop = false;
							}
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("PUSHBUTTON") || asMot1 == _T("DEFPUSHBUTTON")) {
            asMot2 = UnTexte(asLigne, &Cur);
            asMot3 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

						WindowStyle = 0;

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TButton"))) {
              // NewElemVCL = new TButton(NewFormElemVCL);
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
              Component = NewElemVCL->Component;
              NewButton = (TButton *) Component;
              NewButton->Caption = asMot2;
							if (asMot1 == _T("DEFPUSHBUTTON")) NewButton->Default = true;
              if ((WindowStyle & WS_TABSTOP) == 0) {
                NewButton->TabStop = false;
							}
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("CHECKBOX")) {
            asMot2 = UnTexte(asLigne, &Cur);
            asMot3 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

            asMot4 = UnArg(asLigne, &Cur);
            WindowStyle = 0;
            Cur2 = 0;
            do {
              asMot = UneOption(asMot4, &Cur2);
              if (asMot.IsEmpty()) break;
              else if (asMot == _T("WS_TABSTOP")) WindowStyle |= WS_TABSTOP;
              else if (asMot == _T("BS_LEFTTEXT")) WindowStyle |= BS_LEFTTEXT;
            } while (true);

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TCheckBox"))) {
              // NewElemVCL = new TCheckBox(NewFormElemVCL);
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
              Component = NewElemVCL->Component;
              NewCheckBox = (TCheckBox *) Component;
              NewCheckBox->Caption = asMot2;
              if ((WindowStyle & WS_TABSTOP) == 0) {
                NewCheckBox->TabStop = false;
							}
              if ((WindowStyle & 0x0000000F) == BS_LEFTTEXT) {
                NewCheckBox->Alignment = taLeftJustify;
              }
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("RADIOBUTTON") || asMot1 == _T("AUTORADIOBUTTON")) {
            asMot2 = UnTexte(asLigne, &Cur);
            asMot3 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

            asMot4 = UnArg(asLigne, &Cur);
            WindowStyle = 0;
            Cur2 = 0;
            do {
              asMot = UneOption(asMot4, &Cur2);
              if (asMot.IsEmpty()) break;
              else if (asMot == _T("WS_TABSTOP")) WindowStyle |= WS_TABSTOP;
              else if (asMot == _T("BS_LEFTTEXT")) WindowStyle |= BS_LEFTTEXT;
              else if (asMot == _T("BS_AUTORADIOBUTTON")) WindowStyle |= BS_AUTORADIOBUTTON;
            } while (true);

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TRadioButton"))) {
              // NewElemVCL = new TCheckBox(NewFormElemVCL);
              NewElemVCL->Left = X_PIXEL(X);
              NewElemVCL->Top = Y_PIXEL(Y);
              NewElemVCL->Width = X_PIXEL(L);
              NewElemVCL->Height = Y_PIXEL(H);
              if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
              else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
              Component = NewElemVCL->Component;
              NewRadioButton = (TRadioButton *) Component;
              NewRadioButton->Caption = asMot2;
              if ((WindowStyle & WS_TABSTOP) == 0) {
                NewRadioButton->TabStop = false;
							}
              if ((WindowStyle & 0x0000000F) == BS_LEFTTEXT) {
                NewRadioButton->Alignment = taLeftJustify;
              }
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("POPUP")) {
            asMot2 = UnTexte(asLigne, &Cur);

            NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
            NewElemVCL->Parent = NewFormElemVCL;
            if (NewElemVCL->Create(_T("TMenuItem"))) {
              // NewElemVCL = new TCheckBox(NewFormElemVCL);
              NewElemVCL->Name = AnsiString(_T("MenuItem_")) + AnsiString(++NbStatics);
              NewElemVCL->SetProperty(_T("Caption"), asMot2);
              OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
            }
          }
          else if (asMot1 == _T("MENUITEM")) {
            asMot2 = UnTexte(asLigne, &Cur);

						if (asMot2 == _T("SEPARATOR")) {
							NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
							NewElemVCL->Parent = NewFormElemVCL;
							if (NewElemVCL->Create(_T("TMenuItem"))) {
								// NewElemVCL = new TCheckBox(NewFormElemVCL);
								NewElemVCL->Name = AnsiString(_T("Separator_")) + AnsiString(++NbStatics);
	              NewElemVCL->SetProperty(_T("Caption"), _T("-"));
								OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
							}
						}
						else {
							NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
							NewElemVCL->Parent = NewFormElemVCL;
							if (NewElemVCL->Create(_T("TMenuItem"))) {
								// NewElemVCL = new TCheckBox(NewFormElemVCL);
								NewElemVCL->Name = AnsiString(_T("MenuItem_")) + AnsiString(++NbStatics);
	              NewElemVCL->SetProperty(_T("Caption"), asMot2);
								OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
							}
						}
          }
          else if (asMot1 == _T("CONTROL")) {
            asMot2 = UnTexte(asLigne, &Cur);
            asMot3 = UnArg(asLigne, &Cur);
            asMot4 = UnTexte(asLigne, &Cur);
            asMot5 = UnArg(asLigne, &Cur);

            asMot = UnArg(asLigne, &Cur);
            X = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            Y = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            L = asMot.ToInt();
            asMot = UnArg(asLigne, &Cur);
            H = asMot.ToInt();

            if (asMot4 == _T("Static")) {
              WindowStyle = 0;
              Cur2 = 0;
              do {
                asMot = UneOption(asMot5, &Cur2);
                if (asMot.IsEmpty()) break;
	              else if (asMot == _T("SS_BLACKFRAME")) WindowStyle |= SS_BLACKFRAME;
                else if (asMot == _T("WS_TABSTOP")) WindowStyle |= WS_TABSTOP;
                else if (asMot == _T("WS_BORDER")) WindowStyle |= WS_BORDER;
              } while (true);

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if (WindowStyle & SS_BLACKFRAME) {
								NewElemVCL->Create(_T("TPanel"));
								if (NewElemVCL->Component) {
									NewElemVCL->Left = X_PIXEL(X);
									NewElemVCL->Top = Y_PIXEL(Y);
									NewElemVCL->Width = X_PIXEL(L);
									NewElemVCL->Height = Y_PIXEL(H);
									if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
									else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
									Component = NewElemVCL->Component;
									NewPanel = (TPanel *) Component;
									NewPanel->BorderStyle = bsSingle;
									NewPanel->Caption = _T("");
									OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
								}
							}
              else {
								NewElemVCL->Create(_T("TPaintBox"));
								if (NewElemVCL->Component) {
									NewElemVCL->Left = X_PIXEL(X);
									NewElemVCL->Top = Y_PIXEL(Y);
									NewElemVCL->Width = X_PIXEL(L);
									NewElemVCL->Height = Y_PIXEL(H);
									if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
									else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
									Component = NewElemVCL->Component;
									NewPaintBox = (TPaintBox *) Component;
									NewPaintBox->Caption = _T("");
									OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
								}
							}
            }
            else if (asMot4 == _T("Button")) {
              WindowStyle = 0;
              Cur2 = 0;
              do {
                asMot = UneOption(asMot5, &Cur2);
                if (asMot.IsEmpty()) break;
                else if (asMot == _T("WS_TABSTOP")) WindowStyle |= WS_TABSTOP;
                else if (asMot == _T("BS_AUTOCHECKBOX")) WindowStyle |= BS_AUTOCHECKBOX;
                else if (asMot == _T("BS_AUTO3STATE")) WindowStyle |= BS_AUTO3STATE;
                else if (asMot == _T("BS_AUTORADIOBUTTON")) WindowStyle |= BS_AUTORADIOBUTTON;
                else if (asMot == _T("BS_OWNERDRAW")) WindowStyle |= BS_OWNERDRAW;
              } while (true);

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if ((WindowStyle & 0x0000000F) == BS_AUTOCHECKBOX) NewElemVCL->Create(_T("TCheckBox"));
              else if ((WindowStyle & 0x0000000F) == BS_AUTO3STATE) NewElemVCL->Create(_T("TCheckBox"));
              else if ((WindowStyle & 0x0000000F) == BS_AUTORADIOBUTTON) NewElemVCL->Create(_T("TRadioButton"));
              else if ((WindowStyle & 0x0000000F) == BS_OWNERDRAW) NewElemVCL->Create(_T("TPaintBox"));
              if (NewElemVCL->Component) {
                NewElemVCL->Left = X_PIXEL(X);
                NewElemVCL->Top = Y_PIXEL(Y);
                NewElemVCL->Width = X_PIXEL(L);
                NewElemVCL->Height = Y_PIXEL(H);
                if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
                else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                Component = NewElemVCL->Component;
                NewCheckBox = (TCheckBox *) Component;
                NewCheckBox->Caption = asMot2;
                if ((WindowStyle & 0x0000000F) == BS_AUTO3STATE) {
                  NewCheckBox->AllowGrayed = true;
                }
                OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
              }
            }
            else if (asMot4 == _T("SysTreeView32")) {
              WindowStyle = 0;
              Cur2 = 0;
              do {
                asMot = UneOption(asMot5, &Cur2);
                if (asMot.IsEmpty()) break;
                else if (asMot == _T("WS_TABSTOP")) WindowStyle |= WS_TABSTOP;
                else if (asMot == _T("TVS_HASBUTTONS")) WindowStyle |= TVS_HASBUTTONS;
                else if (asMot == _T("TVS_HASLINES")) WindowStyle |= TVS_HASLINES;
                else if (asMot == _T("TVS_LINESATROOT")) WindowStyle |= TVS_LINESATROOT;
              } while (true);

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if (NewElemVCL->Create(_T("TTreeView"))) {
                // NewElemVCL = new TTreeView(NewFormElemVCL);
                NewElemVCL->Left = X_PIXEL(X);
                NewElemVCL->Top = Y_PIXEL(Y);
                NewElemVCL->Width = X_PIXEL(L);
                NewElemVCL->Height = Y_PIXEL(H);
                if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
                else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
              }
            }
            else if (asMot4 == _T("SysTabControl32")) {
              WindowStyle = 0;
              Cur2 = 0;
              do {
                asMot = UneOption(asMot5, &Cur2);
                if (asMot.IsEmpty()) break;
              } while (true);

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if (NewElemVCL->Create(_T("TPageControl"))) {
                // NewElemVCL = new TTreeView(NewFormElemVCL);
                NewElemVCL->Left = X_PIXEL(X);
                NewElemVCL->Top = Y_PIXEL(Y);
                NewElemVCL->Width = X_PIXEL(L);
                NewElemVCL->Height = Y_PIXEL(H);
                if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
                else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
              }
            }
            else if (asMot4 == _T("SplitterClass")) {
              WindowStyle = 0;
              Cur2 = 0;
              do {
                asMot = UneOption(asMot5, &Cur2);
                if (asMot.IsEmpty()) break;
              } while (true);

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if (NewElemVCL->Create(_T("TSplitter"))) {
                // NewElemVCL = new TTreeView(NewFormElemVCL);
                NewElemVCL->Left = X_PIXEL(X);
                NewElemVCL->Top = Y_PIXEL(Y);
                NewElemVCL->Width = X_PIXEL(L);
                NewElemVCL->Height = Y_PIXEL(H);
                if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
                else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
              }
            }
            else if (asMot4 == _T("ONGLETS")) {

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if (NewElemVCL->Create(_T("TPageControl"))) {
                // NewElemVCL = new TPageControl(NewFormElemVCL);
                NewElemVCL->Left = X_PIXEL(X);
                NewElemVCL->Top = Y_PIXEL(Y);
                NewElemVCL->Width = X_PIXEL(L);
                NewElemVCL->Height = Y_PIXEL(H);
                if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
                else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);

              }

              Cur2 = 0;
              do {
                asMot = UneOption(asMot2, &Cur2);
                if (asMot.IsEmpty()) break;
                TabSheet = new TElemVCL(NewFormElemVCL, NULL);
                TabSheet->Parent = NewElemVCL;
                if (TabSheet->Create(_T("TTabSheet"))) {
                  // TabSheet = new TTabSheet(NewElemVCL);
                  Component = NewElemVCL->Component;
                  TabSheet->SetProperty(_T("PageControl"), AnsiString((int) (size_t) Component));
                  TabSheet->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                  TabSheet->SetProperty(_T("Caption"), asMot);
                }
              } while (true);

            }
            else if (asMot4 == _T("PLUSMOINS")) {

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if (NewElemVCL->Create(_T("TUpDown"))) {
                // NewElemVCL = new TUpDown(NewFormElemVCL);
                NewElemVCL->Left = X_PIXEL(X);
                NewElemVCL->Top = Y_PIXEL(Y);
                NewElemVCL->Width = X_PIXEL(L);
                NewElemVCL->Height = Y_PIXEL(H);
                if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
                else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);

                Cur2 = 0;
                asMot = UneOption(asMot2, &Cur2);
                if (!asMot.IsEmpty()) NewElemVCL->SetProperty(_T("Min"), asMot);
                asMot = UneOption(asMot2, &Cur2);
                if (!asMot.IsEmpty()) NewElemVCL->SetProperty(_T("Max"), asMot);
              }

            }

            else {

              NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
              NewElemVCL->Parent = NewFormElemVCL;
              if (NewElemVCL->Create(_T("TPaintBox"))) {
                // NewElemVCL = new TPaintBox(NewFormElemVCL);
                NewElemVCL->Left = X_PIXEL(X);
                NewElemVCL->Top = Y_PIXEL(Y);
                NewElemVCL->Width = X_PIXEL(L);
                NewElemVCL->Height = Y_PIXEL(H);
                if (asMot3 != _T("IDC_STATIC") && asMot3 != _T("-1")) NewElemVCL->Name = AnsiString(_T("Ctrl_")) + asMot3;
                else NewElemVCL->Name = AnsiString(_T("Ctrl_")) + AnsiString(++NbStatics);
                OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
              }

            }
          }
          else if (asMot1 == _T("END")) {
            if (--NbBegin == 0) {
							Component = NewFormElemVCL->Component;
							OrdreDesComposants((TForm *) Component);
							// $$$ ((TForm *) Component)->FormStyle = fsStayOnTop;
							bDialog = false;
						}
          }

        }

      }

    } while (!bEof);

    CloseHandle(hFile);

    return true;
  }
  else {
    DisplaySystemError(asFileName,
											 GetLastError(),
											 _TT("Erreur ouverture"),
											 _T(""),
											 MB_OK | MB_ICONSTOP);
    return false;
  }

}

//---------------------------------------------------------------------------
bool TFileForm::OpenDFM(AnsiString asFileName) {
  TElemVCL * NewElemVCL;
  TElemVCL *Components[100];  // 100 niveaux d'arborescence
  HANDLE hFile;
  DWORD dw;
  char c;
  AnsiString asLigne, asLigne2, asMot1, asMot2, asMot3, asMot;
  AnsiString asName;
  AnsiString asClassName;
  AnsiString asValue;
  bool bDialog;
  int Cur;
  bool bEof;
  int Degre;


  hFile = CreateFile(asFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		NewFormElemVCL = NULL;
    Degre = 0;
    Components[0] = NULL;
    bDialog = false;
    bEof = false;
    do {

      asLigne = _T("");
      do {
        ReadFile(hFile, &c, 1, &dw, NULL);
        if (dw == 0) bEof = true;
        else if (c != '\r' && c != '\n') asLigne += (unsigned char) c;
      } while (!bEof && c != '\n');

      Cur = 0;

      asMot1 = UnMot(asLigne, _T("\t "), _T("\t ="), &Cur);

      if (asMot1 == _T("object") || asMot == _T("inline")) {
				asName = UnMot(asLigne, _T("\t "), _T("\t :"), &Cur);
				asClassName = UnMot(asLigne, _T("\t :"), _T("\t "), &Cur);

				NewElemVCL = NULL;
        if (!bDialog) {
          // object Form_Principale: TFileForm
          NewFormElemVCL = new TElemVCL(NULL, NULL);
          if (NewFormElemVCL->Create(_T("TForm"))) {
            TWinControl *WinControl;
            WinControl = dynamic_cast<TWinControl *> ((TComponent *) NewFormElemVCL->Component);
            NewElemVCL = NewFormElemVCL;
            NewElemVCL->Name = asName;
            bDialog = true;
				    OnNewFormInit(this, NewFormElemVCL);
          }
        }
        else {
          NewElemVCL = new TElemVCL(Components[Degre], NULL);
          NewElemVCL->Parent = Components[Degre];
          if (!NewElemVCL->Create(asClassName)) {
						// Type d'élément inconnu dans un DFM
						// Message d'erreur mais on continue quand même
						AnsiString asErrMessage;
						asErrMessage.sprintf(_TT("Type de composant \"%s\" inconnu").c_str(), asClassName.c_str());
						DisplayError(asErrMessage.c_str(),
												 _TT("Erreur création"),
												 _T("CREATE_COMPONENT"),
												 MB_OK | MB_ICONWARNING);
					}
          NewElemVCL->Name = asName;
          OnNewComponentInit(this, NewFormElemVCL, Components[Degre], NewElemVCL);
        }
        Components[++Degre] = NewElemVCL;
      }
      else if (asMot1 == _T("end")) {
        if (Components[Degre] && Components[Degre]->Component) {
          Components[Degre]->Component->AfterConstruction();
        }
        if (--Degre == 0) bDialog = false;
      }
      else {
        if (bDialog && Components[Degre]) {
				  bool bSortie;

          asValue = UnMot(asLigne, _T("\t ="), _T(""), &Cur);
					if (asValue == _T("(")) {
						asValue = _T("");
						bSortie = false;
						do {
							asLigne = _T("");
							ReadFile(hFile, &c, 1, &dw, NULL);
							if (dw == 0) break;
							while (c != '\n') {
								if (c != '\r') asLigne += (BYTE) c;
								ReadFile(hFile, &c, 1, &dw, NULL);
								if (dw == 0) break;
							}
							if (dw == 0) break;
							if (asLigne.Length() > 0 && asLigne[asLigne.Length()] == _T(')')) {
								asLigne = asLigne.SubString(1, asLigne.Length() - 1);
								bSortie = true;
							}
							asValue += SuppGuillVCL(asLigne) + _T('\n');
						} while (!bSortie);
						SetProperty(Components[Degre], asMot1, asValue);
					}
					else if (asValue == _T("<") || asValue == _T("{")) {
						TCHAR EndCarac = _T('\0');
						if (asValue == _T("<")) EndCarac = _T('>');
						else if (asValue == _T("{")) EndCarac = _T('}');
						asValue = _T("");
						bSortie = false;
						do {
							asLigne = _T("");
							ReadFile(hFile, &c, 1, &dw, NULL);
							if (dw == 0) break;
							while (c != '\n') {
								if (c != '\r') asLigne += (BYTE) c;
								ReadFile(hFile, &c, 1, &dw, NULL);
								if (dw == 0) break;
							}
							if (dw == 0) break;
							if (asLigne.Length() > 0 && asLigne[asLigne.Length()] == EndCarac) {
								asLigne = asLigne.SubString(1, asLigne.Length() - 1);
								bSortie = true;
							}
							asValue += asLigne + _T('\n');
						} while (!bSortie);
						SetProperty(Components[Degre], asMot1, asValue);
					}
					else {
						if (!asValue.IsEmpty() && (asValue[1] == _T('\'') || asValue[1] == _T('#'))) {
							asValue = SuppGuillVCL(asValue);
						}
						SetProperty(Components[Degre], asMot1, asValue);
					}
        }
      }

    } while (!bEof);

    CloseHandle(hFile);

		// $$$ ((TForm *) NewFormElemVCL->Component)->FormStyle = fsStayOnTop;

    return true;
  }
  else {
    DisplaySystemError(asFileName,
											 GetLastError(),
											 _TT("Erreur ouverture"),
											 _T(""),
											 MB_OK | MB_ICONSTOP);
    return false;
  }

}

//---------------------------------------------------------------------------
bool TFileForm::OpenFRM(AnsiString asFileName) {
  TElemVCL * NewFormElemVCL = NULL;
  TElemVCL * NewElemVCL;
  TElemVCL *Components[100];
  HANDLE hFile;
  DWORD dw;
  char c;
  AnsiString asLigne, asLigne2, asMot1, asMot2, asMot3, asMot;
  AnsiString asName;
  AnsiString asVBClassName;
  AnsiString asClassName;
  AnsiString asProperty;
  AnsiString asVBValue;
  AnsiString asValue;
  bool bDialog;
  int Cur;
  bool bEof;
  int Degre;


  hFile = CreateFile(asFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

		NewFormElemVCL = NULL;
    Degre = 0;
    Components[0] = NULL;
    bDialog = false;
    bEof = false;
    do {

      asLigne = _T("");
      do {
        ReadFile(hFile, &c, 1, &dw, NULL);
        if (dw == 0) bEof = true;
        else if (c != '\r' && c != '\n') asLigne += (unsigned char) c;
      } while (!bEof && c != '\n');

      Cur = 0;

      asMot1 = UnMot(asLigne, _T("\t "), _T("\t ="), &Cur);

      if (asMot1 == _T("Begin")) {
				asVBClassName = UnMot(asLigne, _T("\t "), _T("\t "), &Cur);
				asName = UnMot(asLigne, _T("\t "), _T("\t "), &Cur);

        NewElemVCL = new TElemVCL(Components[Degre], NULL);
        if (Degre >= 1) NewElemVCL->Parent = Components[Degre];
				if (asVBClassName == _T("VB.Form")) asClassName = _T("TForm");
				else if (asVBClassName == _T("VB.Label")) asClassName = _T("TLabel");
				else if (asVBClassName == _T("VB.TextBox")) asClassName = _T("TEdit");
				else if (asVBClassName == _T("VB.OptionButton")) asClassName = _T("TRadioButton");
				else if (asVBClassName == _T("VB.CheckBox")) asClassName = _T("TCheckBox");
				else if (asVBClassName == _T("VB.CommandButton")) asClassName = _T("TButton");
				else if (asVBClassName == _T("TabDlg.SSTab")) asClassName = _T("TPageControl");
				else if (asVBClassName == _T("VB.Frame")) asClassName = _T("TPanel");
				else if (asVBClassName == _T("MSComctlLib.StatusBar")) asClassName = _T("TStatusBar");
				else asClassName = _T("TPanel");
        if (NewElemVCL->Create(asClassName)) {
          NewElemVCL->Name = asName;
					if (asClassName == _T("TForm")) {
						NewFormElemVCL = NewElemVCL;
					  OnNewFormInit(this, NewFormElemVCL);
            bDialog = true;
					}
					else {
						OnNewComponentInit(this, NewFormElemVCL, Components[Degre], NewElemVCL);
					}
        }
        Components[++Degre] = NewElemVCL;
      }
      else if (asMot1 == _T("End")) {
        if (Components[Degre]) {
          Components[Degre]->Component->AfterConstruction();
        }
        if (--Degre == 0) {
					bDialog = false;
					break;
				}
      }
      else {
        if (bDialog && Components[Degre]) {
          asVBValue = UnMot(asLigne, _T("\t ="), _T(""), &Cur);
					if (asMot1 == _T("ForeGroundColor")) asProperty = _T("Color");
					if (asMot1 == _T("ForeColor")) asProperty = _T("Font.Color");
					else asProperty = asMot1;
					if (asProperty == _T("Left")) {
						asValue.sprintf(_T("%i"), asVBValue.ToIntDef(0) / 15);
					}
					else if (asProperty == _T("Top")) {
						asValue.sprintf(_T("%i"), asVBValue.ToIntDef(0) / 15);
					}
					else if (asProperty == _T("Width")) {
						asValue.sprintf(_T("%i"), asVBValue.ToIntDef(0) / 15);
					}
					else if (asProperty == _T("Height")) {
						asValue.sprintf(_T("%i"), asVBValue.ToIntDef(0) / 15);
					}
					else if (asProperty == _T("ClientWidth")) {
						asValue.sprintf(_T("%i"), asVBValue.ToIntDef(0) / 15);
					}
					else if (asProperty == _T("ClientHeight")) {
						asValue.sprintf(_T("%i"), asVBValue.ToIntDef(0) / 15);
					}
					else if (asProperty == _T("WordWrap")) {
						int Cur2 = 0;
	          asVBValue = UnMot(asVBValue, _T("\t "), _T(" "), &Cur2);
						if (asVBValue == _T("-1")) asValue = _T("True");
						else asValue = _T("False");
					}
					else asValue = SuppGuillFRM(asVBValue);
					SetProperty(Components[Degre], asProperty, asValue);
        }
      }

    } while (!bEof);

    CloseHandle(hFile);

		// $$$ ((TForm *) NewFormElemVCL->Component)->FormStyle = fsStayOnTop;

    return true;
  }
  else {
    DisplaySystemError(asFileName,
											 GetLastError(),
											 _TT("Erreur ouverture"),
											 _T(""),
											 MB_OK | MB_ICONSTOP);
    return false;
  }

}

//---------------------------------------------------------------------------
bool TFileForm::SaveRC(AnsiString asFileName, std::vector<DIALOGUE> &Dialogues) {
  int i;
  HANDLE hFile;
  CharString csLigne;
  DWORD dw;
  HFONT hFont;
  int baseunitX, baseunitY;
  TComponent *Component;
  TForm *Form;
	int FontHeight;
  AnsiString asFontName;
	CharString csName;


  hFile = CreateFile(asFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {

    for (i = 0; i < (int) Dialogues.size(); i++) {

      // Unités de la boîte de dialogue
      Component = Dialogues[i].Form->Component;
      Form = (TForm *) Component;
			csName = ((AnsiString) Form->Name).AsCharString();
			if (csName.Length() > 5 && csName.SubString(1, 5) == "Form_") {
				csName = csName.SubString(6, csName.Length() - 5);
			}
      FontHeight = Form->Font->Height;
      asFontName = Form->Font->Name;
      hFont = CreateFont(FontHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                         DEFAULT_PITCH | FF_DONTCARE,
                         asFontName);
      GetBaseUnit(hFont, &baseunitX, &baseunitY);

      csLigne.sprintf("%s DIALOG DISCARDABLE %i, %i, %i, %i\r\n",
                      (LPCSTR) csName,
                      X_DLG(Form->Left + GetSystemMetrics(SM_CXFRAME)),
                      Y_DLG(Form->Top + GetSystemMetrics(SM_CYCAPTION)
                                              + GetSystemMetrics(SM_CYFRAME)),
                      X_DLG(Form->Width - 2 * GetSystemMetrics(SM_CXFRAME)),
                      Y_DLG(Form->Height - GetSystemMetrics(SM_CYCAPTION)
                                                 - 2 * GetSystemMetrics(SM_CYFRAME)));
      WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);

      csLigne.sprintf("CAPTION \"%s\"\r\n", (LPCSTR) ((AnsiString) Form->Caption).AsCharString());
      WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);

      csLigne.sprintf("FONT %i, \"%s\"\r\n", FontHeight, (LPCSTR) asFontName.AsCharString());
      WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);

      csLigne.sprintf("BEGIN\r\n");
      WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);

      SaveComponentsRC(hFile, Dialogues[i].Form, 0, 0, baseunitX, baseunitY);

      csLigne.sprintf("END\r\n\r\n");
      WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);

    }

  }

  CloseHandle(hFile);

  return true;
}

//---------------------------------------------------------------------------
bool TFileForm::SaveComponentsRC(HANDLE hFile, TElemVCL * ElemVCL, int XInit, int YInit, int baseunitX, int baseunitY) {
  int i;
  int X, Y;
  CharString csLigne;
  CharString csNom;
  CharString csCaption;
  CharString csName;
  CharString csProperty;
  CharString csOptions;
  const char *szCompName;
  DWORD dw;


  csCaption = ElemVCL->GetProperty(_T("Caption"));
  csName = ElemVCL->Name;
	if (csName.Length() > 5 && csName.SubString(1, 5) == "Ctrl_") {
		csName = csName.SubString(6, csName.Length() - 5);
	}
  X = 0;
  Y = 0;
  if (ElemVCL->GetProperty(_T("TabStop")) == _T("True")) {
    if (!csOptions.IsEmpty()) csOptions += "|";
		csOptions += "WS_TABSTOP";
	}
  if (ElemVCL->GetProperty(_T("Enabled")) == _T("False")) {
    if (!csOptions.IsEmpty()) csOptions += "|";
		csOptions += "WS_DISABLED";
	}

	if (ElemVCL->ClassNameIs(_T("TStaticText"))) {
    //     LTEXT           "Nom de l'élément:",IDC_STATIC,5,6,65,8
		csProperty = ElemVCL->GetProperty(_T("Alignment"));
		if (csProperty == "taCenter") szCompName = "CTEXT";
		else if (csProperty == "taRightJustify") szCompName = "RTEXT";
		else szCompName = "LTEXT";
    csLigne.sprintf("    %s           \"%s\",%s,%i,%i,%i,%i",
										szCompName,
                    (LPCSTR) csCaption,
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TLabel"))) {
    //     LTEXT           "Nom de l'élément:",IDC_STATIC,5,6,65,8
		csProperty = ElemVCL->GetProperty(_T("Alignment"));
		if (csProperty == "taCenter") szCompName = "CTEXT";
		else if (csProperty == "taRightJustify") szCompName = "RTEXT";
		else szCompName = "LTEXT";
    csLigne.sprintf("    %s           \"%s\",%s,%i,%i,%i,%i",
										szCompName,
                    (LPCSTR) csCaption,
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TButton"))) {
    //     PUSHBUTTON      "Annuler",IDCANCEL,170,192,40,14
		csProperty = ElemVCL->GetProperty(_T("Default"));
		if (csProperty == "True") szCompName = "DEFPUSHBUTTON";
		else szCompName = "PUSHBUTTON   ";
    csLigne.sprintf("    %s   \"%s\",%s,%i,%i,%i,%i",
                    szCompName,
                    (LPCSTR) csCaption,
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TMemo"))) {
    //     EDITTEXT        IDD_NOM,70,4,80,12,ES_MULTILINE
    if (!csOptions.IsEmpty()) csOptions += "|";
    csOptions += "ES_MULTILINE";
		csProperty = ElemVCL->GetProperty(_T("WantReturns"));
		if (csProperty == "True") {
	    if (!csOptions.IsEmpty()) csOptions += "|";
			csOptions += "ES_WANTRETURN";
		}
		csLigne.sprintf("    EDITTEXT        %s,%i,%i,%i,%i",
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TEdit"))) {
    //     EDITTEXT        IDD_NOM,70,4,80,12,ES_AUTOHSCROLL
    if (!csOptions.IsEmpty()) csOptions += "|";
    csOptions += "ES_AUTOHSCROLL";
		csProperty = ElemVCL->GetProperty(_T("PasswordChar"));
		if (csProperty != "") {
	    if (!csOptions.IsEmpty()) csOptions += "|";
			csOptions += "ES_PASSWORD";
		}
    csLigne.sprintf("    EDITTEXT        %s,%i,%i,%i,%i",
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TListBox"))) {
    //     LISTBOX         IDLISTE,8,8,272,136,LBS_SORT | LBS_OWNERDRAWFIXED |
    //                     LBS_NOINTEGRALHEIGHT | LBS_MULTICOLUMN |
    //                     LBS_WANTKEYBOARDINPUT | WS_HSCROLL | WS_TABSTOP
    if (!csOptions.IsEmpty()) csOptions += "|";
    csOptions += "WS_HSCROLL|WS_TABSTOP";
    csLigne.sprintf("    LISTBOX         %s,%i,%i,%i,%i",
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TComboBox"))) {
    //     COMBOBOX        IDD111_TYPE,48,88,112,104,CBS_DROPDOWNLIST | WS_TABSTOP
    if (!csOptions.IsEmpty()) csOptions += "|";
    csOptions += "CBS_DROPDOWNLIST";
    csLigne.sprintf("    COMBOBOX        %s,%i,%i,%i,%i",
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TRadioButton"))) {
    if (!csOptions.IsEmpty()) csOptions += "|";
    csOptions += "BS_AUTORADIOBUTTON";
    csLigne.sprintf("    AUTORADIOBUTTON \"%s\",%s,%i,%i,%i,%i",
                    (LPCSTR) csCaption,
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TCheckBox"))) {
    if (!csOptions.IsEmpty()) csOptions += "|";
    csOptions += "BS_AUTOCHECKBOX";
    csLigne.sprintf("    CONTROL         \"%s\",%s,\"Button\",%s,%i,%i,%i,%i",
                    (LPCSTR) csCaption,
                    (LPCSTR) csName,
                    (LPCSTR) csOptions,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TGroupBox"))) {
    //     GROUPBOX        "Point inférieur gauche",IDC_STATIC,5,16,145,58
    csLigne.sprintf("    GROUPBOX        \"%s\",%s,%i,%i,%i,%i",
                    (LPCSTR) csCaption,
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (ElemVCL->ClassNameIs(_T("TPageControl"))) {
    //     CONTROL         "Installation|Identification|Répertoires",IDD100_ONGLETS,
    //                     "ONGLETS",WS_GROUP,8,8,280,166
    csNom = "";
    for (i = 0; i < ((TPageControl *) ElemVCL)->PageCount; i++) {
      if (i) csNom += "|";
      csNom += ((AnsiString) ((TPageControl *) ElemVCL)->Pages[i]->Caption).AsCharString();
    }
    if (!csOptions.IsEmpty()) csOptions += "|";
    csOptions += "WS_GROUP";
    csLigne.sprintf("    CONTROL         \"%s\",%s,\"ONGLETS\",%s,%i,%i,%i,%i",
                    (LPCSTR) csNom,
                    (LPCSTR) csName,
                    (LPCSTR) csOptions,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	else if (!ElemVCL->ClassNameIs(_T("TForm"))) {
    csLigne.sprintf("    CONTROL         \"%s\",%s,\"Static\",\"\",%i,%i,%i,%i",
                    (LPCSTR) csCaption,
                    (LPCSTR) csName,
                    X_DLG(XInit + ElemVCL->Left),
                    Y_DLG(YInit + ElemVCL->Top),
                    X_DLG(ElemVCL->Width),
                    Y_DLG(ElemVCL->Height));
    if (!csOptions.IsEmpty()) {
			csLigne += ",";
			csLigne += csOptions;
		}
		csLigne += "\r\n";
    WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
    X = ElemVCL->Left;
    Y = ElemVCL->Top;
  }

	for (i = 0; i < ElemVCL->ComponentCount; i++) {
    SaveComponentsRC(hFile, (TElemVCL *) ElemVCL->Components[i], X, Y, baseunitX, baseunitY);
  }

  return true;
}

//---------------------------------------------------------------------------
bool TFileForm::SaveDFM(AnsiString asFileName, std::vector<DIALOGUE> &Dialogues) {
  int i;
  bool Ok;
  int Rep;
  AnsiString asFileNameDFM;
  AnsiString asFileNameH;
  AnsiString asFileNameCpp;
  AnsiString asPathFile;
  AnsiString asClassName;
  AnsiString asClassName2;
	AnsiString asComponents, asEvents;
  AnsiString asName;
  HANDLE hFile;
  DWORD dw;


  i = asFileName.LastDelimiter(DIR_DELIMITER);
  asPathFile = asFileName.SubString(1, i - 1);
  asClassName = asFileName.SubString(i + 1, asFileName.Length() - i - 4);

	Rep = IDNO;
  if (Dialogues.size() > 1) {
    asClassName2 = AnsiString(_T("T")) + (AnsiString) Dialogues[0].Form->Name;

		if (asClassName != asClassName2) {
			Rep = DisplayError(_TT("Voulez-vous créer un fichier par dialogue ?"),
												 _TT("Enregistrement fichier .dfm"),
												 _T(""),
												 MB_YESNOCANCEL | MB_ICONQUESTION);
		}
  }

  if (Rep == IDYES) {

    for (i = 0; i < (int) Dialogues.size(); i++) {

      asName = Dialogues[i].Form->Name;
      asFileNameH.sprintf(_T("%s\\T%s.h"), (LPCTSTR) asPathFile, (LPCTSTR) asName);
      asFileNameCpp.sprintf(_T("%s\\T%s.cpp"), (LPCTSTR) asPathFile, (LPCTSTR) asName);
      asFileNameDFM.sprintf(_T("%s\\T%s.dfm"), (LPCTSTR) asPathFile, (LPCTSTR) asName);

      Ok = true;

      if (Ok && FileExists(asFileNameDFM)) {
				AnsiString asMsg;
				asMsg = _TT("Ce fichier existe déjà, voulez-vous l'écraser ?\n") + asFileNameDFM;
				if (DisplayError(asMsg,
												 _TT("Enregistrement fichier .dfm"),
												 _T(""),
												 MB_YESNOCANCEL | MB_ICONQUESTION) != IDYES) Ok = false;
      }

      if (Ok) {

				hFile = CreateFile(asFileNameDFM, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile != INVALID_HANDLE_VALUE) {

          asClassName = AnsiString(_T("T")) + asName;

					SaveComponentsDFM(hFile, Dialogues[i].Form, 0);

          WriteFile(hFile, "\r\n", 2, &dw, NULL);

        }
        else {
					DisplaySystemError(asFileName,
															GetLastError(),
															_TT("Impossible d'ouvrir ce fichier"),
															_T(""),
															MB_OK | MB_ICONSTOP);
        }

        CloseHandle(hFile);


				if (!FileExists(asFileNameH)) CreateFileH(asFileNameH, asClassName);
				if (!FileExists(asFileNameCpp)) CreateFileCpp(asFileNameCpp, asClassName);

				asClassName = _T("T") + asName;
				asComponents = _T("\t// FVCL automatic code ! Don't modify this section.\r\n");
				asComponents += FVCLDeclarations(Dialogues[i].Form);
				asEvents = _T("\t// FVCL automatic code ! Don't modify this section.\r\n");
				asEvents += FVCLEvents(Dialogues[i].Form);
				asEvents += _T("\r\n");
				asEvents += _T("\tvirtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {\r\n");
				asEvents += FVCLCallbacks(Dialogues[i].Form, asClassName);
				asEvents += _T("\t}\r\n");
				asEvents += _T("\r\n");
				FVCLIDEAutomation->ReplaceDeclarationsFVCL(asFileNameH, asClassName, asComponents, asEvents);

      }

    }

  }

  else {

		asFileNameDFM = asFileName;
    asFileNameH = ChangeFileExt(asFileName, _T(".h"));
    asFileNameCpp = ChangeFileExt(asFileName, _T(".cpp"));

    hFile = CreateFile(asFileNameDFM, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {

      for (i = 0; i < (int) Dialogues.size(); i++) {

				SaveComponentsDFM(hFile, Dialogues[i].Form, 0);

				WriteFile(hFile, "\r\n", 2, &dw, NULL);

      }

	    CloseHandle(hFile);


			if (!FileExists(asFileNameH)) CreateFileH(asFileNameH, asClassName);
			if (!FileExists(asFileNameCpp)) CreateFileCpp(asFileNameCpp, asClassName);

			for (i = 0; i < (int) Dialogues.size(); i++) {

				asName = Dialogues[i].Form->Name;
				asClassName = _T("T") + asName;
				asComponents = _T("\t// FVCL automatic code ! Don't modify this section.\r\n");
				asComponents += FVCLDeclarations(Dialogues[i].Form);
				asEvents = _T("\t// FVCL automatic code ! Don't modify this section.\r\n");
				asEvents += FVCLEvents(Dialogues[i].Form);
				asEvents += _T("\r\n");
				asEvents += _T("\tvirtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {\r\n");
				asEvents += FVCLCallbacks(Dialogues[i].Form, asClassName);
				asEvents += _T("\t}\r\n");
				asEvents += _T("\r\n");
				FVCLIDEAutomation->ReplaceDeclarationsFVCL(asFileNameH, asClassName, asComponents, asEvents);

			}

    }
    else {
			DisplaySystemError(asFileName,
													GetLastError(),
													_TT("Impossible d'ouvrir ce fichier"),
													_T(""),
													MB_OK | MB_ICONSTOP);
    }

	}

  return true;
}

//---------------------------------------------------------------------------
AnsiString TFileForm::FVCLDeclarations(TElemVCL * ElemVCL) {
  AnsiString asDeclarations;
  AnsiString asLigne;
  AnsiString asClassName;
  AnsiString asName;
  int i;


  for (i = 0; i < ElemVCL->ComponentCount; i++) {
    asDeclarations += FVCLDeclarations((TElemVCL *) ElemVCL->Components[i]);
  }

  asClassName = ElemVCL->ClassName();
  asName = ElemVCL->Name;
  if (asClassName != _T("TForm")) {
		asLigne.sprintf(_T("\t%s *%s;\r\n"),
										(LPCTSTR) asClassName,
										(LPCTSTR) asName);
		asDeclarations += asLigne;
	}

  return asDeclarations;
}


//---------------------------------------------------------------------------
AnsiString TFileForm::FVCLEvents(TElemVCL * ElemVCL) {
  AnsiString asEvents;
  AnsiString asLigne;
  AnsiString asValue;
  AnsiString asInfos;
  int i;
  TYPEPROPERTY TypeProperty;
  TStringList *ListeProperties;


  for (i = 0; i < ElemVCL->ComponentCount; i++) {
    asEvents += FVCLEvents((TElemVCL *) ElemVCL->Components[i]);
  }

  ListeProperties = new TStringList();
  ElemVCL->GetListProperties(ListeProperties);
  for (i = 0; i < ListeProperties->Count; i++) {
		asInfos = _T("");  // Pour le cas où GetTypeProperty ne renseigne pas asInfos (mais ça ne devrait pas arriver)
    TypeProperty = ElemVCL->GetTypeProperty(ListeProperties->Strings[i], &asInfos);
    if (TypeProperty == tpEvent) {
      asValue = ElemVCL->GetProperty(ListeProperties->Strings[i]);
			if (!asValue.IsEmpty()) {
				asLigne.sprintf(_T("\tvoid %s%s;\r\n"),
												(LPCTSTR) asValue,
												(LPCTSTR) asInfos);
				asEvents += asLigne;
			}
    }
	}
  delete ListeProperties;

  return asEvents;
}


//---------------------------------------------------------------------------
AnsiString TFileForm::FVCLCallbacks(TElemVCL * ElemVCL, AnsiString asClassName) {
  AnsiString asCallbacks;
  AnsiString asLigne;
  CharString csClassName;
  AnsiString asValue;
  AnsiString asInfos;
  AnsiString asArgsWithoutNames;
  AnsiString asEventType;
  AnsiString asEventName;
	int NbArgs;
  int i;
  TYPEPROPERTY TypeProperty;
  TStringList *ListeProperties;


  for (i = 0; i < ElemVCL->ComponentCount; i++) {
    asCallbacks += FVCLCallbacks((TElemVCL *) ElemVCL->Components[i], asClassName);
  }

  ListeProperties = new TStringList();
  ElemVCL->GetListProperties(ListeProperties);
  for (i = 0; i < ListeProperties->Count; i++) {
		asInfos = _T("");  // Pour le cas où GetTypeProperty ne renseigne pas asInfos (mais ça ne devrait pas arriver)
    TypeProperty = ElemVCL->GetTypeProperty(ListeProperties->Strings[i], &asInfos);
    if (TypeProperty == tpEvent) {
      asValue = ElemVCL->GetProperty(ListeProperties->Strings[i]);
			if (!asValue.IsEmpty()) {
				NbArgs = ArgsWithoutNames(asInfos, &asArgsWithoutNames);
				asEventName = ListeProperties->Strings[i];
				if (NbArgs == 1) {
					asEventType = _T("TNotifyEvent");
				}
				else {
					asEventType = _T('T') + asEventName;
				}
				asLigne.sprintf(_T("\t\tSET_EVENT_STR_%i(%s, %s, %s, %s, %s);\r\n"),
												NbArgs,
												(LPCTSTR) asClassName,
												(LPCTSTR) asEventType,
												(LPCTSTR) asEventName,
												(LPCTSTR) asValue,
												(LPCTSTR) asArgsWithoutNames);
				asCallbacks += asLigne;
			}
    }
	}
  delete ListeProperties;

  return asCallbacks;
}


//---------------------------------------------------------------------------
bool TFileForm::SaveComponentsDFM(HANDLE hFile, TElemVCL * ElemVCL, int Indent) {
  char *szIndent;
  CharString csLigne;
  CharString csClassName;
  CharString csName;
  AnsiString asAnchors;
  AnsiString asDefault;
  AnsiString asValue;
  AnsiString asValueSub;
  AnsiString asInfos;
  DWORD dw;
  int i, j;
  bool bIsForm = false;
  TYPEPROPERTY TypeProperty;
  TStringList *ListeProperties;
	TStringList *Items;
  TAnchors Anchors;


	szIndent = new char[Indent + 1];
	memset(szIndent, ' ', Indent);
  szIndent[Indent] = '\0';

  csClassName = ElemVCL->ClassName();
  if (csClassName == "TForm") {
    csClassName = ElemVCL->Name;
    csClassName = CharString("T") + csClassName;
    bIsForm = true;
  }

  csName = ElemVCL->Name;
  csLigne.sprintf("%sobject %s: %s\r\n",
                  szIndent,
                  (LPCSTR) csName,
                  (LPCSTR) csClassName);
  WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);

  ListeProperties = new TStringList();
  ElemVCL->GetListProperties(ListeProperties);
  for (i = 0; i < ListeProperties->Count; i++) {
    if (ListeProperties->Strings[i] != _T("Name") &&
        ListeProperties->Strings[i] != _T("ClientWidth") &&
        ListeProperties->Strings[i] != _T("ClientHeight")) {
      asDefault = ElemVCL->GetDefaultProperty(ListeProperties->Strings[i]);
      asValue = ElemVCL->GetProperty(ListeProperties->Strings[i]);
			if (ListeProperties->Strings[i] == _T("Color") &&
					asValue != asDefault) {
				// La couleur étant mémorisée sous forme de TColor, la propriété "Color" peut
				// renvoyer "clWhite" au lieu de "clWindow" puisque c'est la même couleur
				TColor ColorValue, ColorDefault;
				IdentToColor(asValue, ColorValue);
				IdentToColor(asDefault, ColorDefault);
				if (ColorValue == ColorDefault) asValue = asDefault;
			}
      if (asValue != asDefault) {
        TypeProperty = ElemVCL->GetTypeProperty(ListeProperties->Strings[i], &asInfos);
        if (TypeProperty == tpStrings) {
          csLigne.sprintf("%s  %s = (\r\n",
                          szIndent,
                          (LPCSTR) ListeProperties->Strings[i].AsCharString());
          WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
					
					Items = new TStringList();
					Items->SetTextStr(asValue);
					for (j = 0; j < Items->Count; j++) {
						csLigne.sprintf("%s    '%s'\r\n",
														szIndent,
														(LPCSTR) Items->Strings[j].AsCharString());
						WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
					}
					delete Items;

					csLigne.sprintf("%s  )\r\n",
                          szIndent);
          WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
        }
        else if (TypeProperty == tpData) {
          csLigne.sprintf("%s  %s = {\r\n",
                          szIndent,
                          (LPCSTR) ListeProperties->Strings[i].AsCharString());
          WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
					for (j = 0; j < asValue.Length(); j += 64) {
						if (j + 64 <= asValue.Length()) {
							asValueSub = asValue.SubString(j + 1, 64);
							csLigne.sprintf("%s    %s\r\n",
															szIndent,
															(LPCSTR) asValueSub.AsCharString());
						}
						else {
							asValueSub = asValue.SubString(j + 1, asValue.Length() - j);
							csLigne.sprintf("%s    %s}\r\n",
															szIndent,
															(LPCSTR) asValueSub.AsCharString());
						}
						WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
					}
        }
        else {
          if (TypeProperty == tpText) {
            asValue = AddGuillVCL(asValue);
          }
          csLigne.sprintf("%s  %s = %s\r\n",
                          szIndent,
                          (LPCSTR) ListeProperties->Strings[i].AsCharString(),
                          (LPCSTR) asValue.AsCharString());
          WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);
        }

			}
    }
  }
  delete ListeProperties;

  for (i = 0; i < ElemVCL->ComponentCount; i++) {
    SaveComponentsDFM(hFile, (TElemVCL *) ElemVCL->Components[i], Indent + 2);
  }

  csLigne.sprintf("%send\r\n", szIndent);
  WriteFile(hFile, (LPCSTR) csLigne, csLigne.Length(), &dw, NULL);

	delete[] szIndent;

  return true;
}

//---------------------------------------------------------------------------
bool TFileForm::CreateFileH(AnsiString asFileNameH, AnsiString asClassName) {
  CharString csClassName;
  CharString csLine;
  HANDLE hFile;
  DWORD dw;


	hFile = CreateFile(asFileNameH, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		csClassName = asClassName.AsCharString();

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "#ifndef ";
		csLine += csClassName;
		csLine += "H\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "#define ";
		csLine += csClassName;
		csLine += "H\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "#include <FreeVcl.h>\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "class ";
		csLine += csClassName;
		csLine += " : public TForm {\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "private:\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "protected:\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "public:\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "\tFVCL_BEGIN_COMPONENTS\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "\tFVCL_END_COMPONENTS\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "\tFVCL_BEGIN_EVENTS\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "\tFVCL_END_EVENTS\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "\t";
		csLine += csClassName;
		csLine += "(TComponent *Owner, LPCTSTR szName);\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "\t~";
		csLine += csClassName;
		csLine += "(void);\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		WriteFile(hFile, "};\r\n", 4, &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "#endif  // ";
		csLine += csClassName;
		csLine += "H\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		CloseHandle(hFile);

	}
	else {
		DisplaySystemError(asFileNameH,
												GetLastError(),
												_TT("Impossible d'ouvrir ce fichier"),
												_T(""),
												MB_OK | MB_ICONSTOP);

		return false;
	}

  return true;
}


//---------------------------------------------------------------------------
bool TFileForm::CreateFileCpp(AnsiString asFileNameCpp, AnsiString asClassName) {
  CharString csClassName;
  CharString csLine;
  HANDLE hFile;
  DWORD dw;


	hFile = CreateFile(asFileNameCpp, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		csClassName = asClassName.AsCharString();

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "// prog:          ";
		csLine += csClassName;
		csLine += ".cpp\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "// Fichiers inclus\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "#include \"";
		csLine += csClassName;
		csLine += ".h\"\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = csClassName;
		csLine += "::";
		csLine += csClassName;
		csLine += "(TComponent *Owner, LPCTSTR szName)\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "\t\t: TForm(Owner, szName, FVCL_ARG_SUPP) {\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "}\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		WriteFile(hFile, "\r\n", 2, &dw, NULL);

		csLine = "//---------------------------------------------------------------------------\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = csClassName;
		csLine += "::~";
		csLine += csClassName;
		csLine += "(void) {\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		csLine = "}\r\n";
		WriteFile(hFile, csLine.c_str(), csLine.Length(), &dw, NULL);

		CloseHandle(hFile);

	}
	else {
		DisplaySystemError(asFileNameCpp,
												GetLastError(),
												_TT("Impossible d'ouvrir ce fichier"),
												_T(""),
												MB_OK | MB_ICONSTOP);

		return false;
	}

  return true;
}

//---------------------------------------------------------------------------
int TFileForm::ArgsWithoutNames(AnsiString asListeArgs, AnsiString *asArgsWithoutNames) {
	AnsiString asMot;
	int NbArgs;
	TCHAR c;
	int i;


	NbArgs = 0;
	*asArgsWithoutNames = "";
	asMot = "";
	for (i = 1; i <= asListeArgs.Length(); i++) {
		c = asListeArgs[i];
		if (c == _T('(')) {
		}
		else if (c == _T(',')) {
			NbArgs++;
			(*asArgsWithoutNames) = asArgsWithoutNames->TrimRight();
			(*asArgsWithoutNames) += c;
			asMot = "";
		}
		else if (c == _T(')')) {
			NbArgs++;
			(*asArgsWithoutNames) = asArgsWithoutNames->TrimRight();
			break;
		}
		else if (IsAlphaNum(c)) {
			asMot += c;
		}
		else {
			(*asArgsWithoutNames) += asMot;
			(*asArgsWithoutNames) += c;
			asMot = "";
		}
	}


	return NbArgs;
}

//---------------------------------------------------------------------------
bool TFileForm::IsAlphaNum(TCHAR c) {
	return (_T('0') <= c && c <= _T('9') ||
					_T('a') <= c && c <= _T('z') ||
					_T('A') <= c && c <= _T('Z') ||
					c == _T('_'));
}

//---------------------------------------------------------------------------
bool TFileForm::LoadFromWindow(HWND hWnd) {
	TCHAR szCaption[256];
  AnsiString asName, asCaption;
	RECT Rect;
	int NumCommand;


  if (IsWindow(hWnd)) {

		NewFormElemVCL = NULL;
		hWndMain = hWnd;

		GetWindowText(hWnd, szCaption, _countof(szCaption));
		GetWindowRect(hWnd, &Rect);
		asCaption = szCaption;
	  NumCommand = GetWindowLong(hWnd, GWL_ID);
		asName.sprintf(_T("Form_%i"), NumCommand);

    NewFormElemVCL = new TElemVCL(NULL, NULL);
    if (NewFormElemVCL->Create(_T("TForm"))) {
      NewFormElemVCL->Name = asName;
			NewFormElemVCL->Left = Rect.left;
			NewFormElemVCL->Top = Rect.top;
			NewFormElemVCL->Width = Rect.right - Rect.left;
			NewFormElemVCL->Height = Rect.bottom - Rect.top;
      NewFormElemVCL->SetProperty(_T("Caption"), asCaption);

	    OnNewFormInit(this, NewFormElemVCL);

			EnumChildWindows(hWnd, EnumChildWnd, (LPARAM) this);

		}

    return true;
  }
  else {
		AnsiString asErrMsg;
		asErrMsg.sprintf(_TT("Fenêtre %08X non trouvée"), hWnd);
    DisplaySystemError(asErrMsg,
											 GetLastError(),
											 _TT("Erreur ouverture"),
											 _T(""),
											 MB_OK | MB_ICONSTOP);
    return false;
  }

}

//---------------------------------------------------------------------------
BOOL TFileForm::EnumChildWnd(HWND hWnd) {
	TElemVCL *NewElemVCL;
  DWORD WindowStyle;
	TCHAR szClassName[512];
	TCHAR szCaption[512];
  AnsiString asName, asClassName, asCaption;
	RECT Rect;
	int NumCommand;


  WindowStyle = GetWindowLong(hWnd, GWL_STYLE);
	GetClassName(hWnd, szClassName, _countof(szClassName));
	asClassName = AnsiString(szClassName).UpperCase();
	GetWindowText(hWnd, szCaption, _countof(szCaption));
	asCaption = szCaption;
	GetWindowRect(hWnd, &Rect);
	ScreenToClient(hWndMain, (LPPOINT) &Rect.left);
	ScreenToClient(hWndMain, (LPPOINT) &Rect.right);
	NumCommand = GetWindowLong(hWnd, GWL_ID);
	asName.sprintf(_T("Ctrl_%i"), NumCommand);
  if (asClassName == _T("STATIC") || asClassName == _T("TLABEL") || asClassName == _T("TSTATICTEXT")) {
		NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
		NewElemVCL->Parent = NewFormElemVCL;
		NewElemVCL->Create(_T("TLabel"));
		if (NewElemVCL->Component) {
			NewElemVCL->Name = asName;
			NewElemVCL->Left = Rect.left;
			NewElemVCL->Top = Rect.top;
			NewElemVCL->Width = Rect.right - Rect.left;
			NewElemVCL->Height = Rect.bottom - Rect.top;
			NewElemVCL->SetProperty(_T("Caption"), asCaption);
			OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
		}
	}
  else if (asClassName == _T("TSTATICTEXT")) {
		NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
		NewElemVCL->Parent = NewFormElemVCL;
		NewElemVCL->Create(_T("TStaticText"));
		if (NewElemVCL->Component) {
			NewElemVCL->Name = asName;
			NewElemVCL->Left = Rect.left;
			NewElemVCL->Top = Rect.top;
			NewElemVCL->Width = Rect.right - Rect.left;
			NewElemVCL->Height = Rect.bottom - Rect.top;
			NewElemVCL->SetProperty(_T("Caption"), asCaption);
			OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
		}
	}
  else if (asClassName == _T("EDIT")) {
		NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
		NewElemVCL->Parent = NewFormElemVCL;
    if (WindowStyle & ES_MULTILINE) NewElemVCL->Create(_T("TMemo"));
    else NewElemVCL->Create(_T("TEdit"));
		if (NewElemVCL->Component) {
			NewElemVCL->Name = asName;
			NewElemVCL->Left = Rect.left;
			NewElemVCL->Top = Rect.top;
			NewElemVCL->Width = Rect.right - Rect.left;
			NewElemVCL->Height = Rect.bottom - Rect.top;
			NewElemVCL->SetProperty(_T("Caption"), asCaption);
			OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
		}
	}
  else if (asClassName == _T("LISTBOX")) {
		NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
		NewElemVCL->Parent = NewFormElemVCL;
    NewElemVCL->Create(_T("TListBox"));
		if (NewElemVCL->Component) {
			NewElemVCL->Name = asName;
			NewElemVCL->Left = Rect.left;
			NewElemVCL->Top = Rect.top;
			NewElemVCL->Width = Rect.right - Rect.left;
			NewElemVCL->Height = Rect.bottom - Rect.top;
			NewElemVCL->SetProperty(_T("Caption"), asCaption);
			OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
		}
	}
  else if (asClassName == _T("BUTTON")) {
		NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
		NewElemVCL->Parent = NewFormElemVCL;
    if ((WindowStyle & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON) NewElemVCL->Create(_T("TRadioButton"));
    else if ((WindowStyle & BS_GROUPBOX) == BS_GROUPBOX) NewElemVCL->Create(_T("TGroupBox"));
    else NewElemVCL->Create(_T("TButton"));
		if (NewElemVCL->Component) {
			NewElemVCL->Name = asName;
			NewElemVCL->Left = Rect.left;
			NewElemVCL->Top = Rect.top;
			NewElemVCL->Width = Rect.right - Rect.left;
			NewElemVCL->Height = Rect.bottom - Rect.top;
			NewElemVCL->SetProperty(_T("Caption"), asCaption);
			OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
		}
	}
  else {
		NewElemVCL = new TElemVCL(NewFormElemVCL, NULL);
		NewElemVCL->Parent = NewFormElemVCL;
    NewElemVCL->Create(_T("TPanel"));
		if (NewElemVCL->Component) {
			NewElemVCL->Name = asName;
			NewElemVCL->Left = Rect.left;
			NewElemVCL->Top = Rect.top;
			NewElemVCL->Width = Rect.right - Rect.left;
			NewElemVCL->Height = Rect.bottom - Rect.top;
			NewElemVCL->SetProperty(_T("Caption"), asClassName);
			OnNewComponentInit(this, NewFormElemVCL, NewFormElemVCL, NewElemVCL);
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL CALLBACK TFileForm::EnumChildWnd(HWND hWnd, LPARAM lParam) {
	TFileForm *FileForm = (TFileForm *) lParam;
	return FileForm->EnumChildWnd(hWnd);
}

//---------------------------------------------------------------------------
