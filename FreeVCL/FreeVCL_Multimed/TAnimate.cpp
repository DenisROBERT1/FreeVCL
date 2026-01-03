//-----------------------------------------------------------------------------
//! @file TAnimate.cpp
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
#include "TAnimate.h"


//---------------------------------------------------------------------------
// TAnimate
//---------------------------------------------------------------------------

TAnimate::TAnimate(TComponent* AOwner): TWinControl(AOwner) {
  HFONT hFont;
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TAnimate");
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
  FWindowStyle |= ACS_CENTER | ACS_TRANSPARENT;
  FActive = false;
  FCommonAVI = aviNone;
  FFrameCount = 0;
  FFrameHeight = 0;
  FFrameWidth = 0;
  FOpen = false;
  FRepetitions = 0;
  FResHandle = HInstance;
  FResId = 0;
  FStartFrame = 1;
  FStopFrame = 0;
  hDllShell32 = NULL;

  // Enregistrement de la classe Animate
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCtrls.dwICC = ICC_ANIMATE_CLASS;
  InitCommonControlsEx(&InitCtrls);

  // Création de la fenêtre
  FHandle = CreateWindowEx(FExWindowStyle,
                         ANIMATE_CLASS, FCaption,
                         FWindowStyle,
                         FLeft, FTop,
                         FWidth, FHeight,
                         hWndParent,
                         (HMENU) (size_t) (int) FCommand, ::HInstance, this);

  if (FHandle) {
    hFont = FFont->Handle;
    SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

    InitStdControl();
  }

}

//---------------------------------------------------------------------------
TAnimate::~TAnimate(void) {
  if (hDllShell32) FreeLibrary(hDllShell32);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Active
//---------------------------------------------------------------------------

bool TAnimate::Get_Active() {
  return FActive;
}

bool TAnimate::Set_Active(bool NewActive) {
  if (FActive != NewActive) {
    FActive = NewActive;
    if (FHandle) {
      if (FActive) {
        if (SendMessage(FHandle, ACM_PLAY,
						(WPARAM) (UINT) ((FRepetitions != 0)? FRepetitions: -1),
						(LPARAM) MAKELONG(FStartFrame - 1, FStopFrame - 1))) {
					OnStart(this);
				}
      }
      else {
        SendMessage(FHandle, ACM_STOP, 0, 0);
				OnStop(this);
      }
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Center
//---------------------------------------------------------------------------

bool TAnimate::Get_Center() {
  return ((FWindowStyle & ACS_CENTER) != 0);
}

bool TAnimate::Set_Center(bool NewCenter) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewCenter) NewWindowStyle |= ACS_CENTER;
  else NewWindowStyle &= (DWORD) ~ACS_CENTER;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété CommonAVI
//---------------------------------------------------------------------------

TCommonAVI TAnimate::Get_CommonAVI() {
  return FCommonAVI;
}

bool TAnimate::Set_CommonAVI(TCommonAVI NewCommonAVI) {
  if (FCommonAVI != NewCommonAVI) {
    FCommonAVI = NewCommonAVI;
    FActive = false;
    if (FHandle) {
      if (FCommonAVI != aviNone) {
				OSVERSIONINFO VersionInformation;
				LPCTSTR szDllName = _T("Shell32.dll");
				VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				if (GetVersionEx(&VersionInformation) == TRUE) {
					if (VersionInformation.dwMajorVersion > 4) {
						// Windows VISTA ou SEVEN: la DLL Shell32.dll ne contient plus
						// le même format d'image => il faut chercher les images ailleurs
						szDllName = _T("ShellXP32.dll");
					}
				}
				hDllShell32 = LoadLibrary(szDllName);
        if (hDllShell32) {
          int ResIdShell32 = 0;
          switch (FCommonAVI) {
            case aviFindFolder:
              ResIdShell32 = 150;
              break;
            case aviFindFile:
              ResIdShell32 = 151;
              break;
            case aviFindComputer:
              ResIdShell32 = 152;
              break;
            case aviCopyFiles:
              ResIdShell32 = 160;
              break;
            case aviCopyFile:
              ResIdShell32 = 161;
              break;
            case aviRecycleFile:
              ResIdShell32 = 162;
              break;
            case aviEmptyRecycle:
              ResIdShell32 = 163;
              break;
            case aviDeleteFile:
              ResIdShell32 = 164;
              break;
          }
          if (SendMessage(FHandle, ACM_OPEN, (WPARAM) hDllShell32,
						(LPARAM) MAKEINTRESOURCE(ResIdShell32))) {
						FOpen = true;
						OnOpen(this);
					}
        }
				FFileName = "";
				FResName = "";
				FResId = 0;
      }
      else {
        SendMessage(FHandle, ACM_OPEN, 0, 0);
				FOpen = false;
				OnClose(this);
        if (hDllShell32) {
          FreeLibrary(hDllShell32);
          hDllShell32 = NULL;
        }
      }

    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FileName
//---------------------------------------------------------------------------

AnsiString TAnimate::Get_FileName(void) {
  return FFileName;
}

bool TAnimate::Set_FileName(AnsiString NewFileName) {
  if (FFileName != NewFileName) {
    FFileName = NewFileName;
    FActive = false;
    if (FHandle) {
      if (SendMessage(FHandle, ACM_OPEN, (WPARAM) HInstance,
				(LPARAM) (LPCTSTR) FFileName)) {
				FOpen = true;
				OnOpen(this);
			}
			FCommonAVI = aviNone;
			FResName = "";
			FResId = 0;
    }
  }

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FrameCount
//---------------------------------------------------------------------------

int TAnimate::Get_FrameCount() {
	// todo Je ne sais pas comment lire le nombre d'images dans l'animation
  return FFrameCount;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FrameHeight
//---------------------------------------------------------------------------

int TAnimate::Get_FrameHeight() {
  return FFrameHeight;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FrameWidth
//---------------------------------------------------------------------------

int TAnimate::Get_FrameWidth() {
  return FFrameWidth;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Open
//---------------------------------------------------------------------------

bool TAnimate::Get_Open() {
  return FOpen;
}

bool TAnimate::Set_Open(bool NewOpen) {
  if (FOpen != NewOpen) {
    FOpen = NewOpen;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Repetitions
//---------------------------------------------------------------------------

int TAnimate::Get_Repetitions() {
  return FRepetitions;
}

bool TAnimate::Set_Repetitions(int NewRepetitions) {
  if (FRepetitions != NewRepetitions) {
    FRepetitions = NewRepetitions;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ResHandle
//---------------------------------------------------------------------------

HINSTANCE TAnimate::Get_ResHandle() {
  return FResHandle;
}

bool TAnimate::Set_ResHandle(HINSTANCE NewResHandle) {
  if (FResHandle != NewResHandle) {
    FResHandle = NewResHandle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ResId
//---------------------------------------------------------------------------

int TAnimate::Get_ResId() {
  return FResId;
}

bool TAnimate::Set_ResId(int NewResId) {
  if (FResId != NewResId) {
    FResId = NewResId;
    FActive = false;
    if (FHandle) {
      if (SendMessage(FHandle, ACM_OPEN, (WPARAM) FResHandle,
				(LPARAM) MAKEINTRESOURCE(FResId))) {
				FOpen = true;
				OnOpen(this);
			}
			FCommonAVI = aviNone;
			FFileName = "";
			FResName = "";
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété ResName
//---------------------------------------------------------------------------

AnsiString TAnimate::Get_ResName() {
  return FResName;
}

bool TAnimate::Set_ResName(AnsiString NewResName) {
  if (FResName != NewResName) {
    FResName = NewResName;
    if (FHandle) {
      if (SendMessage(FHandle, ACM_OPEN, (WPARAM) FResHandle,
				(LPARAM) (LPCTSTR) FResName)) {
				FOpen = true;
				OnOpen(this);
			}
			FCommonAVI = aviNone;
			FFileName = "";
			FResId = 0;
    }
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété StartFrame
//---------------------------------------------------------------------------

short TAnimate::Get_StartFrame() {
  return FStartFrame;
}

bool TAnimate::Set_StartFrame(short NewStartFrame) {
  if (FStartFrame != NewStartFrame) {
    FStartFrame = NewStartFrame;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété StopFrame
//---------------------------------------------------------------------------

short TAnimate::Get_StopFrame() {
  return FStopFrame;
}

bool TAnimate::Set_StopFrame(short NewStopFrame) {
  if (FStopFrame != NewStopFrame) {
    FStopFrame = NewStopFrame;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Timers
//---------------------------------------------------------------------------

bool TAnimate::Get_Timers() {
  return ((FWindowStyle & ACS_TIMER) != 0);
}

bool TAnimate::Set_Timers(bool NewTimers) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewTimers) NewWindowStyle |= ACS_TIMER;
  else NewWindowStyle &= (DWORD) ~ACS_TIMER;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Transparent
//---------------------------------------------------------------------------

bool TAnimate::Get_Transparent() {
  return ((FWindowStyle & ACS_TRANSPARENT) != 0);
}

bool TAnimate::Set_Transparent(bool NewTransparent) {
  DWORD NewWindowStyle;


  NewWindowStyle = FWindowStyle;
  if (NewTransparent) NewWindowStyle |= ACS_TRANSPARENT;
  else NewWindowStyle &= (DWORD) ~ACS_TRANSPARENT;
  ChangeWindowStyle(NewWindowStyle);

  return true;
}


//---------------------------------------------------------------------------
// Joue un intervalle de l'animation AVI.
//---------------------------------------------------------------------------
void FASTCALL TAnimate::Play(Word FromFrame, Word ToFrame, int Count) {
  FStartFrame = FromFrame;
  FStopFrame = ToFrame;
  FRepetitions = Count;
  Set_Active(true);
}


//---------------------------------------------------------------------------
// Réinitialise l'animation
//---------------------------------------------------------------------------
void FASTCALL TAnimate::Reset(void) {
	WPARAM wParam = 0;
	LPARAM lParam = 0;

	if (FOpen) {
		SendMessage(FHandle, ACM_OPEN, 0, 0);
		FOpen = false;
		OnClose(this);
	}

	if (FCommonAVI != aviNone && hDllShell32 != NULL) {
    int ResIdShell32 = 0;
    switch (FCommonAVI) {
      case aviFindFolder:
        ResIdShell32 = 150;
        break;
      case aviFindFile:
        ResIdShell32 = 151;
        break;
      case aviFindComputer:
        ResIdShell32 = 152;
        break;
      case aviCopyFiles:
        ResIdShell32 = 160;
        break;
      case aviCopyFile:
        ResIdShell32 = 161;
        break;
      case aviRecycleFile:
        ResIdShell32 = 162;
        break;
      case aviEmptyRecycle:
        ResIdShell32 = 163;
        break;
      case aviDeleteFile:
        ResIdShell32 = 164;
        break;
    }
		wParam = (WPARAM) hDllShell32;
		lParam = (LPARAM) MAKEINTRESOURCE(ResIdShell32);
	}
	else if (!FFileName.IsEmpty()) {
		wParam = (WPARAM) HInstance;
		lParam = (LPARAM) (LPCTSTR) FFileName;
	}
	else if (FResId != 0) {
		wParam = (WPARAM) FResHandle;
		lParam = (LPARAM) MAKEINTRESOURCE(FResId);
	}
	else if (!FResName.IsEmpty()) {
		wParam = (WPARAM) FResHandle;
		lParam = (LPARAM) (LPCTSTR) FResName;
	}

	if (wParam != 0) {
		if (SendMessage(FHandle, ACM_OPEN, wParam, lParam)) {
			FOpen = true;
			OnOpen(this);
		}
	}

}


//---------------------------------------------------------------------------
// Affiche une image de l'animation
//---------------------------------------------------------------------------
void FASTCALL TAnimate::Seek(short Frame) {
    if (FHandle) {
      if (SendMessage(FHandle, ACM_PLAY, (WPARAM) 1,
				(LPARAM) MAKELONG(Frame - 1, Frame - 1))) {
				OnStart(this);
			}
    }
}


//---------------------------------------------------------------------------
// Stoppe l'animation
//---------------------------------------------------------------------------
void FASTCALL TAnimate::Stop(void) {
  Set_Active(false);
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TAnimate::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Active"));
  ListProperties->Add(_T("Center"));
  ListProperties->Add(_T("CommonAVI"));
  ListProperties->Add(_T("FileName"));
  ListProperties->Add(_T("FrameCount"));
  ListProperties->Add(_T("FrameHeight"));
  ListProperties->Add(_T("FrameWidth"));
  ListProperties->Add(_T("Open"));
  ListProperties->Add(_T("Repetitions"));
  ListProperties->Add(_T("ResHandle"));
  ListProperties->Add(_T("ResId"));
  ListProperties->Add(_T("ResName"));
  ListProperties->Add(_T("StartFrame"));
  ListProperties->Add(_T("StopFrame"));
  ListProperties->Add(_T("Timers"));
  ListProperties->Add(_T("Transparent"));
  ListProperties->Add(_T("OnOpen"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TAnimate::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Active")) {
    return tpBool;
  }
  if (asProperty == _T("Center")) {
    return tpBool;
  }
  if (asProperty == _T("CommonAVI")) {
    *asInfos = _T("aviNone\naviFindFolder\naviFindFile\naviFindComputer\naviCopyFiles\naviCopyFile\naviRecycleFile\naviEmptyRecycle\naviDeleteFile");
    return tpChoice;
  }
  if (asProperty == _T("FileName")) {
    return tpText;
  }
  if (asProperty == _T("FrameCount")) {
    return tpNumber;
  }
  if (asProperty == _T("FrameHeight")) {
    return tpNumber;
  }
  if (asProperty == _T("FrameWidth")) {
    return tpNumber;
  }
  if (asProperty == _T("Open")) {
    return tpBool;
  }
  if (asProperty == _T("Repetitions")) {
    return tpNumber;
  }
  if (asProperty == _T("ResHandle")) {
    return tpNumber;
  }
  if (asProperty == _T("ResId")) {
    return tpNumber;
  }
  if (asProperty == _T("ResName")) {
    return tpText;
  }
  if (asProperty == _T("StartFrame")) {
    return tpNumber;
  }
  if (asProperty == _T("StopFrame")) {
    return tpNumber;
  }
  if (asProperty == _T("Timers")) {
    return tpBool;
  }
  if (asProperty == _T("Transparent")) {
    return tpBool;
  }
  if (asProperty == _T("OnOpen")) {
    *asInfos = _T("(TObject *Sender)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TAnimate::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Active")) {
    return _T("False");
  }
  if (asProperty == _T("Center")) {
    return _T("True");
  }
  if (asProperty == _T("CommonAVI")) {
    return _T("aviNone");
  }
  if (asProperty == _T("FileName")) {
    return _T("");
  }
  if (asProperty == _T("FrameCount")) {
    return _T("");
  }
  if (asProperty == _T("FrameHeight")) {
    return _T("");
  }
  if (asProperty == _T("FrameWidth")) {
    return _T("");
  }
  if (asProperty == _T("Open")) {
    return _T("False");
  }
  if (asProperty == _T("Repetitions")) {
    return _T("0");
  }
  if (asProperty == _T("ResHandle")) {
    return _T("0");
  }
  if (asProperty == _T("ResId")) {
    return _T("");
  }
  if (asProperty == _T("ResName")) {
    return _T("");
  }
  if (asProperty == _T("StartFrame")) {
    return _T("1");
  }
  if (asProperty == _T("StopFrame")) {
    return _T("0");
  }
  if (asProperty == _T("Timers")) {
    return _T("False");
  }
  if (asProperty == _T("Transparent")) {
    return _T("True");
  }
  if (asProperty == _T("OnOpen")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TAnimate::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Active")) {
    if (Get_Active()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Center")) {
    if (Get_Center()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("CommonAVI")) {
    TCommonAVI CommonAVI = Get_CommonAVI();
    if (CommonAVI == aviNone) return _T("aviNone");
    if (CommonAVI == aviFindFolder) return _T("aviFindFolder");
    if (CommonAVI == aviFindFile) return _T("aviFindFile");
    if (CommonAVI == aviFindComputer) return _T("aviFindComputer");
    if (CommonAVI == aviCopyFiles) return _T("aviCopyFiles");
    if (CommonAVI == aviCopyFile) return _T("aviCopyFile");
    if (CommonAVI == aviRecycleFile) return _T("aviRecycleFile");
    if (CommonAVI == aviEmptyRecycle) return _T("aviEmptyRecycle");
    if (CommonAVI == aviDeleteFile) return _T("aviDeleteFile");
    return _T("");
  }
  if (asProperty == _T("FileName")) {
    return Get_FileName();
  }
  if (asProperty == _T("FrameCount")) {
    return IntToStr(Get_FrameCount());
  }
  if (asProperty == _T("FrameHeight")) {
    return IntToStr(Get_FrameHeight());
  }
  if (asProperty == _T("FrameWidth")) {
    return IntToStr(Get_FrameWidth());
  }
  if (asProperty == _T("Open")) {
    if (Get_Open()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Repetitions")) {
    return IntToStr(Get_Repetitions());
  }
  if (asProperty == _T("ResHandle")) {
    return IntToStr((int) ((size_t) (Get_ResHandle())));
  }
  if (asProperty == _T("ResId")) {
    return IntToStr(Get_ResId());
  }
  if (asProperty == _T("ResName")) {
    return Get_ResName();
  }
  if (asProperty == _T("StartFrame")) {
    return IntToStr(Get_StartFrame());
  }
  if (asProperty == _T("StopFrame")) {
    return IntToStr(Get_StopFrame());
  }
  if (asProperty == _T("Timers")) {
    if (Get_Timers()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("Transparent")) {
    if (Get_Transparent()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("OnOpen")) {
    return OnOpen.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TAnimate::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Active")) {
    Set_Active(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Center")) {
    Set_Center(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("CommonAVI")) {
    if (asValue == _T("aviNone")) Set_CommonAVI(aviNone);
    else if (asValue == _T("aviFindFolder")) Set_CommonAVI(aviFindFolder);
    else if (asValue == _T("aviFindFile")) Set_CommonAVI(aviFindFile);
    else if (asValue == _T("aviFindComputer")) Set_CommonAVI(aviFindComputer);
    else if (asValue == _T("aviCopyFiles")) Set_CommonAVI(aviCopyFiles);
    else if (asValue == _T("aviCopyFile")) Set_CommonAVI(aviCopyFile);
    else if (asValue == _T("aviRecycleFile")) Set_CommonAVI(aviRecycleFile);
    else if (asValue == _T("aviEmptyRecycle")) Set_CommonAVI(aviEmptyRecycle);
    else if (asValue == _T("aviDeleteFile")) Set_CommonAVI(aviDeleteFile);
    return true;
  }
  if (asProperty == _T("FileName")) {
    Set_FileName(asValue);
    return true;
  }
  if (asProperty == _T("Open")) {
    Set_Open(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Repetitions")) {
    Set_Repetitions(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("ResHandle")) {
    Set_ResHandle((HINSTANCE) (size_t) asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("ResId")) {
    Set_ResId(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("ResName")) {
    Set_ResName(asValue);
    return true;
  }
  if (asProperty == _T("StartFrame")) {
    Set_StartFrame(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("StopFrame")) {
    Set_StopFrame(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("Timers")) {
    Set_Timers(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Transparent")) {
    Set_Transparent(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("OnOpen")) {
		OnOpen.ObjCall = Sender;
    OnOpen = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

