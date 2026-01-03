//-----------------------------------------------------------------------------
//! @file TDdeServerConv.cpp
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
#include <FreeVcl_Web.h>


//---------------------------------------------------------------------------
// TDdeServerConv
//---------------------------------------------------------------------------

TDdeServerConv::TDdeServerConv(TComponent* AOwner): TComponent(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TDdeServerConv");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;

  // Enregistrement de la classe "TDdeServerConv":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TDdeServerConv");

  RegisterClass(&wc);

  // Création de la fenêtre de communication
  HandleServer = CreateWindow(_T("TDdeServerConv"), _T(""),
                              0,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              hWndParent,
                              (HMENU) 0, ::HInstance, this);


}

TDdeServerConv::~TDdeServerConv(void) {
  if (HandleServer) DestroyWindow(HandleServer);
}

//---------------------------------------------------------------------------
// Répond aux macros envoyées par le client DDE.
//---------------------------------------------------------------------------

int FASTCALL TDdeServerConv::ExecuteMacro(int Data) {
  return 0;
}

//---------------------------------------------------------------------------
// Traitement du message WM_DDE_INITIATE
//---------------------------------------------------------------------------
bool TDdeServerConv::TraitmntDdeInitiate(HWND HandleClient, LPARAM lParam) {
  ATOM atomApp = 0;
  ATOM atomTopic = 0;
  bool Retour = false;
  bool bAppOk = false;
  bool bTopicOk = false;

  MessageDde = WM_DDE_INITIATE;

  atomApp   = LOWORD(lParam);
  atomTopic = HIWORD(lParam);

  // Test application
  if (atomApp != 0) {
    int LenMax = 100; // $$$ ModuleName.Length() + 1;
    TCHAR * szApp = new TCHAR[LenMax];
    if (GlobalGetAtomName(atomApp, szApp, LenMax)) {
      if (!lstrcmp(szApp, _T("Server"))) bAppOk = true;
    }
    // Sinon, l'atom est certainement plus long que LenMax
    delete[] szApp;
  }
  else bAppOk = true;

  // Test topic
  if (bAppOk) {
    if (atomTopic != 0) {
      int LenMax = FName.Length() + 1;
      TCHAR * szTopic = new TCHAR[LenMax];
      if (GlobalGetAtomName(atomTopic, szTopic, LenMax)) {
        if (FName == szTopic) bTopicOk = true;
      }
      // Sinon, l'atom est certainement plus long que LenMax
      delete[] szTopic;
    }
    else bTopicOk = true;
  }

  if (bAppOk && bTopicOk) {
    // When sending WM_DDE_ACK message, the server should create new aApp and aTopic atoms (MSDN)
    atomApp = GlobalAddAtom(_T("Server"));
    atomTopic = GlobalAddAtom(FName);
    SendMessage((HWND) HandleClient, WM_DDE_ACK, (WPARAM) HandleServer, MAKELPARAM(atomApp, atomTopic));
  }

  return true;
}

//---------------------------------------------------------------------------
// Traitement du message WM_DDE_ADVISE
//---------------------------------------------------------------------------
bool TDdeServerConv::TraitmntDdeAdvise(HWND HandleClient, LPARAM lParam) {
  DDEADVISE *DdeAdvise;
  LPARAM lPackedVal;
  bool fAckReq = false;


  MessageDde = WM_DDE_ADVISE;

  GLOBALHANDLE hData;
  ATOM atomItem;
  // MessageBox(NULL, _T("TraitmntDdeAdvise"), _T("$$$ server"), MB_OK);
  UnpackDDElParam(WM_DDE_DATA, lParam, (PUINT_PTR) (PUINT) &hData, (PUINT_PTR) (PUINT) &atomItem);
  if (hData) {
    DdeAdvise = (DDEADVISE *) GlobalLock(hData);
    fAckReq = DdeAdvise->fAckReq;
    // $$$$$$$$$$$$$$$$$$
    int LenMax = 100; // $$$ ModuleName.Length() + 1;
    TCHAR * szAtom = new TCHAR[LenMax];
    if (GlobalGetAtomName(atomItem, szAtom, LenMax)) {
      // MessageBox(NULL, szAtom, _T("$$$ server"), MB_OK);
    }
    delete[] szAtom;
    // $$$$$$$$$$$$$$$$$$
    GlobalUnlock(hData);
  }

  if (fAckReq) {
    union {
      UINT wStatus;
      DDEACK DdeAck;
    };
    wStatus = 0;
    DdeAck.fAck = TRUE;
    lPackedVal = ReuseDDElParam(lParam, WM_DDE_DATA, WM_DDE_ACK, (UINT_PTR) wStatus, (UINT_PTR) atomItem);
    PostMessage(HandleClient, WM_DDE_ACK, (WPARAM) HandleServer, (LPARAM) lPackedVal);
    // MessageBox(NULL, _T("ACK Advise"), _T("$$$ server"), MB_OK);
  }
  else {
    FreeDDElParam(WM_DDE_DATA, lParam);
  }

  return true;
}

//---------------------------------------------------------------------------
// Ajout d'un client à la liste
//---------------------------------------------------------------------------
bool TDdeServerConv::AddDdeServerItem(TDdeServerItem *DdeServerItem) {
  int i;

  for (i = 0; i < (int) ListDdeServerItems.size(); i++) {
    if (ListDdeServerItems[i] == DdeServerItem) return false;
  }
  ListDdeServerItems.push_back(DdeServerItem);

  return true;
}

//---------------------------------------------------------------------------
// Suppression d'un client de la liste
//---------------------------------------------------------------------------
bool TDdeServerConv::RemoveDdeServerItem(TDdeServerItem *DdeServerItem) {
  int i;

  for (i = 0; i < (int) ListDdeServerItems.size(); i++) {
    if (ListDdeServerItems[i] == DdeServerItem) {
      ListDdeServerItems.erase(ListDdeServerItems.begin() + i);
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDdeServerConv::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDdeServerConv::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDdeServerConv::GetDefaultProperty(AnsiString asProperty) {
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDdeServerConv::GetProperty(AnsiString asProperty) {
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDdeServerConv::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
LRESULT FAR PASCAL TDdeServerConv::WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
  TDdeServerConv *DdeServerConv;


  DdeServerConv = (TDdeServerConv *) (SIZE_T) GetWindowLongPtr(Handle, GWLP_USERDATA);
  if (Message == WM_CREATE && DdeServerConv == NULL) {
    LPCREATESTRUCT lpCreatStruct = (LPCREATESTRUCT) lParam;
    DdeServerConv = (TDdeServerConv *) lpCreatStruct->lpCreateParams;
    SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR) (SIZE_T) DdeServerConv);
  }

  if (DdeServerConv) {

    switch (Message) {

    case WM_DDE_INITIATE:
      DdeServerConv->TraitmntDdeInitiate((HWND) wParam, lParam);
      return 0;

    case WM_DDE_ADVISE:
      DdeServerConv->TraitmntDdeAdvise((HWND) wParam, lParam);
      return 0;

    }

  }

  return DefWindowProc(Handle, Message, wParam, lParam);
}

//---------------------------------------------------------------------------

