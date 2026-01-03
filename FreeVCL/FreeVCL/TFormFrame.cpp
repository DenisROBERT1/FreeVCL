//-----------------------------------------------------------------------------
//! @file TFormFrame.cpp
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

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
// TFormFrame
//---------------------------------------------------------------------------

TFormFrame::TFormFrame(TComponent *AOwner):
           TWinControl(AOwner) {
}

//---------------------------------------------------------------------------
TFormFrame::~TFormFrame(void) {
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TFormFrame::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TFormFrame::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TFormFrame::GetDefaultProperty(AnsiString asProperty) {
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TFormFrame::GetProperty(AnsiString asProperty) {
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TFormFrame::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
bool TFormFrame::LoadDialog(const TCHAR * szName, TComponent *ListComponents[]) {
  HRSRC hResInfo;
  HGLOBAL hgRes;
	int SizeRes;
  bool Ok = false;


  // DisableAlign(); // marche pas (ça perturbe les ScreenCenter, par exemple)
  FComponentState << csLoading;
	hResInfo = FindResource(HInstance, szName, RT_DIALOG);
	if (hResInfo) {
		hgRes = LoadResource(HInstance, hResInfo);
	  SizeRes = SizeofResource(HInstance, hResInfo);
		if (hgRes && SizeRes) Ok = LoadDialogRC(hgRes, SizeRes);
	}
  else {
		hResInfo = FindResource(HInstance, szName, _T("DFM"));
		if (hResInfo) {
			hgRes = LoadResource(HInstance, hResInfo);
		  SizeRes = SizeofResource(HInstance, hResInfo);
			if (hgRes && SizeRes) Ok = LoadDialogVCL(hgRes, SizeRes, ListComponents);
		}
  }

	if (Ok) {
		Loaded();
		// EnableAlign();
		// Ne pas utiliser FLeft, FTop, ... car dans le cas des forms il y a un décalage
		Size(Get_Left(), Get_Top(), Get_Width(), Get_Height(), true);
		ProcessSetFocus(NULL);
	}
	else {
		MessageBox(NULL,
				_TT("Dialogue non trouvé ou incorrect"),
				szName,
				MB_OK | MB_ICONSTOP);
	}

  return Ok;
}

//---------------------------------------------------------------------------
bool TFormFrame::LoadDialogRC(HGLOBAL hgRes, int SizeRes) {
  int i, Item;
  HDC hdc;
  TEXTMETRIC tm;
  LPBYTE lpRes;
  int PosRes;
  LPDLGTEMPLATE lpDlgTemplate;
  LPDLGITEMTEMPLATE lpDlgItemTemplate;
  WCHAR *szBuf;
  AnsiString asMenu;
  AnsiString asClass;
  AnsiString asFont;
  AnsiString asItemClass;
  AnsiString asItemCaption;
  AnsiString asItemCreationData;
  TWinControl *NewComponent;
  int lfHeight;
  LOGFONT LogFont;


  lpRes = (LPBYTE) LockResource(hgRes);
  PosRes = 0;

  hdc = GetDC(NULL);
  GetTextMetrics(hdc, &tm);
  ReleaseDC(NULL, hdc);

  lpDlgTemplate = (LPDLGTEMPLATE) &lpRes[PosRes];
  PosRes += sizeof(DLGTEMPLATE);
  FLeft = DLGX(lpDlgTemplate->x) - GetSystemMetrics(SM_CXFRAME);
  FTop = DLGX(lpDlgTemplate->y) - GetSystemMetrics(SM_CYCAPTION)
                                - GetSystemMetrics(SM_CYFRAME);
  FWidth = DLGX(lpDlgTemplate->cx) + 2 *GetSystemMetrics(SM_CXFRAME);
  FHeight = DLGX(lpDlgTemplate->cy) + GetSystemMetrics(SM_CYCAPTION)
                                    + 2 *GetSystemMetrics(SM_CYFRAME);
  if (FLeft < 0) FLeft = 0;
  if (FTop < 0) FTop = 0;
  FWindowStyle = lpDlgTemplate->style | WS_VISIBLE;
  FExWindowStyle = lpDlgTemplate->dwExtendedStyle;

  szBuf = (WCHAR *) &lpRes[PosRes];
  i = -1;
  asMenu = _T("");
  while (szBuf[++i]) asMenu += (char) szBuf[i];
  asClass = _T("");
  while (szBuf[++i]) asClass += (char) szBuf[i];
  while (szBuf[++i]) FCaption += (char) szBuf[i];
  lfHeight = szBuf[++i];
  if (lfHeight) {
    while (szBuf[++i]) asFont += (char) szBuf[i];
    if ((FWindowStyle & DS_SETFONT) && !asFont.IsEmpty()) {
      GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LogFont), &LogFont);
      LogFont.lfHeight = -lfHeight;
      lstrcpyn(LogFont.lfFaceName, asFont, 32);
      FFont->Handle = CreateFontIndirect(&LogFont);
    }
  }
  PosRes += ++i * sizeof(WCHAR);

  for (Item = 0; Item < lpDlgTemplate->cdit; Item++) {
    PosRes = 4 *((PosRes - 1) / 4 + 1);
    lpDlgItemTemplate = (LPDLGITEMTEMPLATE) &lpRes[PosRes];
    PosRes += sizeof(DLGITEMTEMPLATE);
    szBuf = (WCHAR *) &lpRes[PosRes];
    i = -1;
    if (szBuf[0] == 0xFFFF) {
      switch (szBuf[1]) {
        case 0x0080: asItemClass = _T("TButton"); break;
        case 0x0081: asItemClass = _T("TEdit"); break;
        case 0x0082: asItemClass = _T("TLabel"); break;
        case 0x0083: asItemClass = _T("TListBox"); break;
        case 0x0084: asItemClass = _T("TScrollBar"); break;
        case 0x0085: asItemClass = _T("TComboBox"); break;
      }
      i = 1;
    }
    else {
      asItemClass = _T("");
      while (szBuf[++i]) asItemClass += (char) szBuf[i];
    }
    asItemCaption = _T("");
    while (szBuf[++i]) asItemCaption += (char) szBuf[i];
    asItemCreationData = _T("");
    while (szBuf[++i]) asItemCreationData += (char) szBuf[i];
    PosRes += ++i *sizeof(WCHAR);

    NewComponent = (TWinControl *) Factory_FVCL.CreateObject(this, asItemClass);
    NewComponent->Caption = asItemCaption;
    NewComponent->ExWindowStyle = lpDlgItemTemplate->dwExtendedStyle;
    NewComponent->WindowStyle = lpDlgItemTemplate->style | WS_CHILD;
    NewComponent->Left = DLGX(lpDlgItemTemplate->x);
    NewComponent->Top = DLGX(lpDlgItemTemplate->y);
    NewComponent->Width = DLGX(lpDlgItemTemplate->cx);
    NewComponent->Height = DLGX(lpDlgItemTemplate->cy);
    NewComponent->Font = FFont;
  }

  return true;
}

//---------------------------------------------------------------------------
bool TFormFrame::LoadDialogVCL(HGLOBAL hgRes, int SizeRes, TComponent *ListComponents[]) {
  LPBYTE lpRes;
  AnsiString asLigne;
  AnsiString asMot;
  AnsiString asName;
  AnsiString asClassName;
  AnsiString asValue;
  int PosRes;
  int Cur;
  char c;
  TComponent *NewComponent[100];
  int Niveau;
  int NbComponents;
  int NbEvents;


  lpRes = (LPBYTE) LockResource(hgRes);
	PosRes = 0;
  Niveau = 0;
  NbComponents = 0;
  NbEvents = 0;

  do {

		if (PosRes >= SizeRes) {
      MessageBox(NULL,
					_TT("Erreur de syntaxe dans le DFM"),
					asClassName,
					MB_OK | MB_ICONSTOP);
			// The UnlockResource function is obsolete.
			return false;
		}

    asLigne = _T("");
		while ((c = lpRes[PosRes++]) != '\n') if (c != '\r') asLigne += (BYTE) c;

    Cur = 0;
    asMot = UnMot(asLigne, _T("\t "), _T("\t ="), &Cur);

    if (asMot == _T("object") || asMot == _T("inline")) {
      asName = UnMot(asLigne, _T("\t "), _T("\t :"), &Cur);
      asClassName = UnMot(asLigne, _T("\t :"), _T("\t "), &Cur);

      Niveau++;
      if (Niveau > 1) {
        NewComponent[Niveau] =
            Factory_FVCL.CreateObject(NewComponent[Niveau - 1], asClassName);
        if (NewComponent[Niveau] == NULL) {
          MessageBox(NULL,
						_TT("Impossible de créer le composant (peut-être avez-vous oublié le init() ?)"),
						asClassName,
						MB_OK | MB_ICONSTOP);
        }
      }
      else {
        NewComponent[Niveau] = this;
      }
      if (NewComponent[Niveau]) NewComponent[Niveau]->Name = asName;
    }
    else if (asMot == _T("end")) {
			if (Niveau > 1) ListComponents[NbComponents] = NULL;
      if (NewComponent[Niveau]) {
        NewComponent[Niveau]->AfterConstruction();
        if (Niveau > 1) {
          if (NewComponent[Niveau - 1]) {
            if (ListComponents) ListComponents[NbComponents] = NewComponent[Niveau];
          }
          else delete NewComponent[Niveau];
          NewComponent[Niveau] = NULL;
        }
      }
      NbComponents++;
      Niveau--;
    }
    else if (NewComponent[Niveau]) {
		  bool bSortie;

      asValue = UnMot(asLigne, _T("\t ="), _T(""), &Cur);
      if (asValue == _T("(")) {
        asValue = _T("");
        bSortie = false;
        do {
          asLigne = _T("");
          while ((c = lpRes[PosRes++]) != '\n') if (c != '\r') asLigne += (BYTE) c;
          if (asLigne[asLigne.Length()] == _T(')')) {
            asLigne = asLigne.SubString(1, asLigne.Length() - 1);
            bSortie = true;
          }
          asValue += Application->Translate(SuppGuillVCL(asLigne)) + _T('\n');
        } while (!bSortie);
        NewComponent[Niveau]->SetProperty(asMot, asValue, this);
      }
      else if (asValue == _T("<") || asValue == _T("{")) {
        TCHAR EndCarac = _T('\0');
        if (asValue == _T("<")) EndCarac = _T('>');
        else if (asValue == _T("{")) EndCarac = _T('}');
        asValue = _T("");
        bSortie = false;
        do {
          asLigne = _T("");
          while ((c = lpRes[PosRes++]) != '\n') if (c != '\r') asLigne += (BYTE) c;
          if (asLigne[asLigne.Length()] == EndCarac) {
            asLigne = asLigne.SubString(1, asLigne.Length() - 1);
            bSortie = true;
          }
          asValue += asLigne + _T('\n');
        } while (!bSortie);
        NewComponent[Niveau]->SetProperty(asMot, asValue, this);
      }
      else {
        if (!asValue.IsEmpty() && (asValue[1] == _T('\'') || asValue[1] == _T('#'))) {
          asValue = Application->Translate(SuppGuillVCL(asValue));
        }
        NewComponent[Niveau]->SetProperty(asMot, asValue, this);
      }
    }

  } while (Niveau);

	// The UnlockResource function is obsolete.

  return true;
}
//---------------------------------------------------------------------------

