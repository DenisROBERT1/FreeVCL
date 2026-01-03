//-----------------------------------------------------------------------------
//! @file TDdeClientConv.cpp
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
// TDdeClientConv
//---------------------------------------------------------------------------

TDdeClientConv::TDdeClientConv(TComponent* AOwner): TComponent(AOwner) {
  WNDCLASS wc;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TDdeClientConv");
  FConnectMode = ddeAutomatic;
  FConv = 0;
  FDataMode = ddeAutomatic;
  FDdeFmt = CF_TEXT;
  FFormatChars = false;
  FWaitStat = false;
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;

  // Enregistrement de la classe "TDdeClientConv":
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance;
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = _T("TDdeClientConv");

  RegisterClass(&wc);

  // Création de la fenêtre de communication
  HandleClient = CreateWindow(_T("TDdeClientConv"), _T(""),
                              0,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              hWndParent,
                              (HMENU) 0, ::HInstance, this);


}

TDdeClientConv::~TDdeClientConv(void) {
  if (HandleClient) DestroyWindow(HandleClient);
  CloseLink();
}

//---------------------------------------------------------------------------
// Fin de création
//---------------------------------------------------------------------------

void FASTCALL TDdeClientConv::AfterConstruction(void) {
  if (FConnectMode == ddeAutomatic) OpenLink();
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ConnectMode
//---------------------------------------------------------------------------

TDataMode TDdeClientConv::Get_ConnectMode() {
  return FConnectMode;
}

bool TDdeClientConv::Set_ConnectMode(TDataMode NewConnectMode) {
  if (FConnectMode != NewConnectMode) {
    FConnectMode = NewConnectMode;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Conv
//---------------------------------------------------------------------------

int TDdeClientConv::Get_Conv() {
  return FConv;
}

bool TDdeClientConv::Set_Conv(int NewConv) {
  if (FConv != NewConv) {
    FConv = NewConv;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DataMode
//---------------------------------------------------------------------------

TDataMode TDdeClientConv::Get_DataMode() {
  return FDataMode;
}

bool TDdeClientConv::Set_DataMode(TDataMode NewDataMode) {
  if (FDataMode != NewDataMode) {
    FDataMode = NewDataMode;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DdeFmt
//---------------------------------------------------------------------------

int TDdeClientConv::Get_DdeFmt() {
  return FDdeFmt;
}

bool TDdeClientConv::Set_DdeFmt(int NewDdeFmt) {
  if (FDdeFmt != NewDdeFmt) {
    FDdeFmt = NewDdeFmt;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DdeService
//---------------------------------------------------------------------------

AnsiString TDdeClientConv::Get_DdeService() {
  return FDdeService;
}

bool TDdeClientConv::Set_DdeService(AnsiString NewDdeService) {
  if (FDdeService != NewDdeService) {
    FDdeService = NewDdeService;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DdeTopic
//---------------------------------------------------------------------------

AnsiString TDdeClientConv::Get_DdeTopic() {
  return FDdeTopic;
}

bool TDdeClientConv::Set_DdeTopic(AnsiString NewDdeTopic) {
  if (FDdeTopic != NewDdeTopic) {
    FDdeTopic = NewDdeTopic;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FormatChars
//---------------------------------------------------------------------------

bool TDdeClientConv::Get_FormatChars() {
  return FFormatChars;
}

bool TDdeClientConv::Set_FormatChars(bool NewFormatChars) {
  if (FFormatChars != NewFormatChars) {
    FFormatChars = NewFormatChars;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ServiceApplication
//---------------------------------------------------------------------------

AnsiString TDdeClientConv::Get_ServiceApplication() {
  return FServiceApplication;
}

bool TDdeClientConv::Set_ServiceApplication(AnsiString NewServiceApplication) {
  if (FServiceApplication != NewServiceApplication) {
    FServiceApplication = NewServiceApplication;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété WaitStat
//---------------------------------------------------------------------------

bool TDdeClientConv::Get_WaitStat() {
  return FWaitStat;
}

bool TDdeClientConv::Set_WaitStat(bool NewWaitStat) {
  if (FWaitStat != NewWaitStat) {
    FWaitStat = NewWaitStat;
  }
  return true;
}


//---------------------------------------------------------------------------
// Met fin à une conversation DDE en cours.
//---------------------------------------------------------------------------

void FASTCALL TDdeClientConv::CloseLink(void) {
  if (HandleServer) {
    PostMessage(HandleServer, WM_DDE_TERMINATE, (WPARAM) HandleClient, 0);
    OnClose(this);
    HandleServer = NULL;
  }
}

//---------------------------------------------------------------------------
// Envoie une chaîne de macro-commande à l'application serveur DDE.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::ExecuteMacro(char * Cmd, bool WaitFlg) {
  return true;
}

//---------------------------------------------------------------------------
// Envoie une liste de macro-commandes à une application serveur DDE.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::ExecuteMacroLines(TStrings* Cmd, bool WaitFlg) {
  return true;
}

//---------------------------------------------------------------------------
// Commence une nouvelle conversation DDE.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::OpenLink(void) {
  ATOM atomApp = 0;
  ATOM atomTopic = 0;
  bool Retour = false;

  if (HandleServer) CloseLink();

  if (!FDdeService.IsEmpty()) atomApp = GlobalAddAtom(FDdeService);
  if (!FDdeTopic.IsEmpty()) atomTopic = GlobalAddAtom(FDdeTopic);

  MessageDde = 0;
  SendMessage((HWND) 0xFFFF, WM_DDE_INITIATE, (WPARAM) HandleClient, MAKELPARAM(atomApp, atomTopic));

  if (atomApp)   GlobalDeleteAtom(atomApp);
  if (atomTopic) GlobalDeleteAtom(atomTopic);

  if (MessageDde == WM_DDE_ACK) {  // HandleServer et lParamDde mis à jour à la réception du message WM_DDE_ACK
    OnOpen(this);
    atomApp   = LOWORD(lParamDde);
    atomTopic = HIWORD(lParamDde);
    Retour = true;
    StartAdvise();   // $$$ Faut-il le faire ici ?

    if (atomApp)   GlobalDeleteAtom(atomApp);
    if (atomTopic) GlobalDeleteAtom(atomTopic);
  }
  else {
    MessageBox(NULL, _T("OpenLink failed"), _T("$$$ client"), MB_OK);
  }

  return Retour;
}

//---------------------------------------------------------------------------
// Ouvre une conversation DDE avec l'objet Presse-papiers en cours.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::PasteLink(void) {
  return true;
}

//---------------------------------------------------------------------------
// Envoie des données à une application serveur DDE.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::PokeData(const AnsiString Item, char *Data) {
  return true;
}

//---------------------------------------------------------------------------
// Envoie plusieurs lignes de données à une application serveur DDE.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::PokeDataLines(const AnsiString Item, TStrings* Data) {
  return true;
}

//---------------------------------------------------------------------------
// Demande des données à un serveur DDE.
//---------------------------------------------------------------------------

AnsiString FASTCALL TDdeClientConv::RequestData(const AnsiString Item) {
  DDEDATA *DdeData;
  LPARAM lPackedVal;
  bool fAckReq = false;
  GLOBALHANDLE hData;
  ATOM atomItem;
  AnsiString asRetour;
  int i;

  if (!HandleServer) {
    MessageBox(NULL, _T("Server not connected"), _T("$$$ client"), MB_OK);
  }

  atomItem = GlobalAddAtom(Item);
  PostMessage(HandleServer, WM_DDE_REQUEST, (WPARAM) HandleClient, MAKELPARAM(FDdeFmt, atomItem));

  MessageDde = 0;
  for (i = 0; i < 100; i++) {
    Sleep(10);
    Application->ProcessMessages();
    if (MessageDde != 0) break;
  }
  if (MessageDde == WM_DDE_TERMINATE) return asRetour;

  if (MessageDde == WM_DDE_ACK) {
    union {
      UINT wStatus;
      DDEACK DdeAck;
    };
    wStatus = 0;
    UnpackDDElParam(WM_DDE_ACK, lParamDde, (PUINT_PTR) (PUINT) &wStatus, (PUINT_PTR) (PUINT) &atomItem);
    if (atomItem) GlobalDeleteAtom(atomItem);
  }

  else if (MessageDde == WM_DDE_DATA) {
    UnpackDDElParam(WM_DDE_DATA, lParamDde, (PUINT_PTR) (PUINT) &hData, (PUINT_PTR) (PUINT) &atomItem);
    if (hData) {
      DdeData = (DDEDATA *) GlobalLock(hData);
      fAckReq = DdeData->fAckReq;
      FDdeFmt = DdeData->cfFormat;
      if (FDdeFmt == CF_TEXT) {
        asRetour = (char *) DdeData->Value;
      }
      GlobalUnlock(hData);
      if (DdeData->fRelease) {
        fAckReq = false;
        GlobalFree(hData);
      }
    }

    if (fAckReq) {
      union {
        UINT wStatus;
        DDEACK DdeAck;
      };
      wStatus = 0;
      DdeAck.fAck = TRUE;
      lPackedVal = ReuseDDElParam(lParamDde, WM_DDE_DATA, WM_DDE_ACK, (UINT) wStatus, (UINT) atomItem);
      PostMessage(HandleServer, WM_DDE_ACK, (WPARAM) HandleClient, (LPARAM) lPackedVal);
    }
    else {
      GlobalDeleteAtom(atomItem);
      FreeDDElParam(WM_DDE_DATA, lParamDde);
    }

    if (!hData) RequestData("DdeServerItem1");  // $$$  // Provoque un nouveau DdeData avec hData

  }

  return asRetour;
}

//---------------------------------------------------------------------------
// Spécifie le service et la rubrique d'une conversation DDE et tente
// d'ouvrir le lien si ConnectMode vaut ddeAutomatic.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::SetLink(const AnsiString Service, const AnsiString Topic) {
  FDdeService = Service;
  FDdeTopic = Topic;

  if (FConnectMode == ddeAutomatic) return OpenLink();

  return true;
}

//---------------------------------------------------------------------------
// Démarre une boucle d'information pour tous les TDdeClientItems
// associés à la conversation DDE en cours.
//---------------------------------------------------------------------------

bool FASTCALL TDdeClientConv::StartAdvise(void) {
  GLOBALHANDLE hData;
  DDEADVISE *DdeAdvise;
  LPARAM lPackedVal;
  ATOM atomItem;
  int i;
  bool Retour = false;

  if (!HandleServer) {
    MessageBox(NULL, _T("Server not connected"), _T("$$$ client"), MB_OK);
  }

  atomItem = GlobalAddAtom(_T("DdeServerItem1"));  // $$$
  hData = GlobalAlloc(GHND | GMEM_DDESHARE, sizeof(DDEADVISE));
  DdeAdvise = (DDEADVISE *) GlobalLock(hData);
  DdeAdvise->cfFormat = FDdeFmt;
  DdeAdvise->fDeferUpd = TRUE;
  DdeAdvise->fAckReq = TRUE;
  GlobalUnlock(hData);
  lPackedVal = PackDDElParam(WM_DDE_ADVISE, (UINT_PTR) hData, (UINT_PTR) atomItem);
  PostMessage(HandleServer, WM_DDE_ADVISE, (WPARAM) HandleClient, (LPARAM) lPackedVal);

  MessageDde = 0;
  for (i = 0; i < 100; i++) {
    Sleep(10);
    Application->ProcessMessages();
    if (MessageDde != 0) break;
  }
  if (MessageDde == WM_DDE_TERMINATE) return false;

  if (MessageDde == WM_DDE_ACK) {
    UnpackDDElParam(WM_DDE_ACK, lParamDde, (PUINT_PTR) (PUINT) &hData, (PUINT_PTR) (PUINT) &atomItem);
    Retour = true;
  }
  else {
    MessageBox(NULL, _T("StartAdvise failed"), _T("$$$ client"), MB_OK);
  }

  if (hData) GlobalFree(hData);
  if (atomItem) GlobalDeleteAtom(atomItem);

  return Retour;
}


//---------------------------------------------------------------------------
// Traitement du message WM_DDE_ACK
//---------------------------------------------------------------------------
bool TDdeClientConv::TraitmntDdeAck(HWND hWnd, LPARAM lParam) {
  MessageDde = WM_DDE_ACK;
  HandleServer = hWnd;
  lParamDde = lParam;

  return true;
}

//---------------------------------------------------------------------------
// Traitement du message WM_DDE_TERMINATE
//---------------------------------------------------------------------------
bool TDdeClientConv::TraitmntDdeTerminate(HWND hWnd) {
  MessageDde = WM_DDE_TERMINATE;
  if (HandleServer && HandleServer == hWnd) {
    PostMessage(HandleServer, WM_DDE_TERMINATE, (WPARAM) HandleClient, 0);
    OnClose(this);
    HandleServer = NULL;
  }
  // Sinon, le message est la réponse à un TERMINATE déjà envoyé
  // => On ne fait rien

  return true;
}

//---------------------------------------------------------------------------
// Traitement du message WM_DDE_DATA
//---------------------------------------------------------------------------
bool TDdeClientConv::TraitmntDdeData(HWND hWnd, LPARAM lParam) {
  GLOBALHANDLE hData;
  ATOM atomItem;
  AnsiString asData;
  int i;


  MessageDde = WM_DDE_DATA;
  lParamDde = lParam;

  UnpackDDElParam(WM_DDE_DATA, lParam, (PUINT_PTR) (PUINT) &hData, (PUINT_PTR) (PUINT) &atomItem);
  if (!hData) {
    asData = RequestData("DdeServerItem1");  // $$$
    for (i = 0; i < (int) ListDdeClientItems.size(); i++) {
      ListDdeClientItems[i]->Text = asData;
    }
  }
  // Sinon, tout est fait dans le RequestData

  return true;
}

//---------------------------------------------------------------------------
// Ajout d'un client à la liste
//---------------------------------------------------------------------------
bool TDdeClientConv::AddDdeClientItem(TDdeClientItem *DdeClientItem) {
  int i;

  for (i = 0; i < (int) ListDdeClientItems.size(); i++) {
    if (ListDdeClientItems[i] == DdeClientItem) return false;
  }
  ListDdeClientItems.push_back(DdeClientItem);

  return true;
}

//---------------------------------------------------------------------------
// Suppression d'un client de la liste
//---------------------------------------------------------------------------
bool TDdeClientConv::RemoveDdeClientItem(TDdeClientItem *DdeClientItem) {
  int i;

  for (i = 0; i < (int) ListDdeClientItems.size(); i++) {
    if (ListDdeClientItems[i] == DdeClientItem) {
      ListDdeClientItems.erase(ListDdeClientItems.begin() + i);
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDdeClientConv::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("ConnectMode"));
  ListProperties->Add(_T("Conv"));
  ListProperties->Add(_T("DataMode"));
  ListProperties->Add(_T("DdeFmt"));
  ListProperties->Add(_T("DdeService"));
  ListProperties->Add(_T("DdeTopic"));
  ListProperties->Add(_T("FormatChars"));
  ListProperties->Add(_T("ServiceApplication"));
  ListProperties->Add(_T("WaitStat"));
  ListProperties->Add(_T("OnOpen"));
  ListProperties->Add(_T("OnClose"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDdeClientConv::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("ConnectMode")) {
    *asInfos = _T("ddeAutomatic\nddeManual");
    return tpChoice;
  }
  if (asProperty == _T("Conv")) {
    return tpNumber;
  }
  if (asProperty == _T("DataMode")) {
    *asInfos = _T("ddeAutomatic\nddeManual");
    return tpChoice;
  }
  if (asProperty == _T("DdeFmt")) {
    return tpNumber;
  }
  if (asProperty == _T("DdeService")) {
    return tpText;
  }
  if (asProperty == _T("DdeTopic")) {
    return tpText;
  }
  if (asProperty == _T("FormatChars")) {
    return tpBool;
  }
  if (asProperty == _T("ServiceApplication")) {
    return tpText;
  }
  if (asProperty == _T("WaitStat")) {
    return tpBool;
  }
  if (asProperty == _T("OnOpen")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  if (asProperty == _T("OnClose")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDdeClientConv::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("ConnectMode")) {
    return _T("ddeAutomatic");
  }
  if (asProperty == _T("Conv")) {
    return _T("0");
  }
  if (asProperty == _T("DataMode")) {
    return _T("");
  }
  if (asProperty == _T("DdeFmt")) {
    return _T("0");
  }
  if (asProperty == _T("DdeService")) {
    return _T("");
  }
  if (asProperty == _T("DdeTopic")) {
    return _T("");
  }
  if (asProperty == _T("FormatChars")) {
    return _T("False");
  }
  if (asProperty == _T("ServiceApplication")) {
    return _T("");
  }
  if (asProperty == _T("WaitStat")) {
    return _T("False");
  }
  if (asProperty == _T("OnOpen")) {
    return _T("");
  }
  if (asProperty == _T("OnClose")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDdeClientConv::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("ConnectMode")) {
    TDataMode ConnectMode = Get_ConnectMode();
    if (ConnectMode == ddeAutomatic) return _T("ddeAutomatic");
    if (ConnectMode == ddeManual) return _T("ddeManual");
    return _T("");
  }
  if (asProperty == _T("Conv")) {
    return IntToStr(Get_Conv());
  }
  if (asProperty == _T("DataMode")) {
    TDataMode DataMode = Get_DataMode();
    if (DataMode == ddeAutomatic) return _T("ddeAutomatic");
    if (DataMode == ddeManual) return _T("ddeManual");
    return _T("");
  }
  if (asProperty == _T("DdeFmt")) {
    return IntToStr(Get_DdeFmt());
  }
  if (asProperty == _T("DdeService")) {
    return Get_DdeService();
  }
  if (asProperty == _T("DdeTopic")) {
    return Get_DdeTopic();
  }
  if (asProperty == _T("FormatChars")) {
    if (Get_FormatChars()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("ServiceApplication")) {
    return Get_ServiceApplication();
  }
  if (asProperty == _T("WaitStat")) {
    if (Get_WaitStat()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("OnOpen")) {
    return OnOpen.AsString();
  }
  if (asProperty == _T("OnClose")) {
    return OnClose.AsString();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDdeClientConv::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("ConnectMode")) {
    if      (asValue == _T("ddeAutomatic")) Set_ConnectMode(ddeAutomatic);
    else if (asValue == _T("ddeManual")) Set_ConnectMode(ddeManual);
    return true;
  }
  if (asProperty == _T("Conv")) {
    Set_Conv(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("DataMode")) {
    if      (asValue == _T("ddeAutomatic")) Set_DataMode(ddeAutomatic);
    else if (asValue == _T("ddeManual")) Set_DataMode(ddeManual);
    return true;
  }
  if (asProperty == _T("DdeFmt")) {
    Set_DdeFmt(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("DdeService")) {
    Set_DdeService(asValue);
    return true;
  }
  if (asProperty == _T("DdeTopic")) {
    Set_DdeTopic(asValue);
    return true;
  }
  if (asProperty == _T("FormatChars")) {
    Set_FormatChars(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("ServiceApplication")) {
    Set_ServiceApplication(asValue);
    return true;
  }
  if (asProperty == _T("WaitStat")) {
    Set_WaitStat(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("OnOpen")) {
		OnOpen.ObjCall = Sender;
    OnOpen = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnClose")) {
		OnClose.ObjCall = Sender;
    OnClose = asValue.c_str();
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
LRESULT FAR PASCAL TDdeClientConv::WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
  TDdeClientConv *DdeClientConv;


  DdeClientConv = (TDdeClientConv *) (SIZE_T) GetWindowLongPtr(Handle, GWLP_USERDATA);
  if (Message == WM_CREATE && DdeClientConv == NULL) {
    LPCREATESTRUCT lpCreatStruct = (LPCREATESTRUCT) lParam;
    DdeClientConv = (TDdeClientConv *) lpCreatStruct->lpCreateParams;
    SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR) (SIZE_T) DdeClientConv);
  }

  if (DdeClientConv) {

    switch (Message) {

    case WM_DDE_ACK:
      DdeClientConv->TraitmntDdeAck((HWND) wParam, lParam);
      return 0;

    case WM_DDE_TERMINATE:
      DdeClientConv->TraitmntDdeTerminate((HWND) wParam);
      return 0;

    case WM_DDE_DATA:
      DdeClientConv->TraitmntDdeData((HWND) wParam, lParam);
      return 0;

    }

  }

  return DefWindowProc(Handle, Message, wParam, lParam);
}

//---------------------------------------------------------------------------

