//-----------------------------------------------------------------------------
//! @file TThread.cpp
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
// TThread
//---------------------------------------------------------------------------

TThread::TThread(bool CreateSuspended): TObject() {
  // Initialisations
  FClassName = _T("TThread");
	FFatalException = NULL;
  FFreeOnTerminate = true;
  FHandle = NULL;
  FPriority = tpNormal;
  FReturnValue = 0;
  FSuspended = true;
  FTerminated = false;
  FThreadID = 0;

  FHandle = CreateThread(
      NULL,                                   // Choose default security
      0,                                      // Default stack size
      (LPTHREAD_START_ROUTINE) &ThreadProc,   // Routine to execute
      (LPVOID) this,                          // Thread parameter
      CreateSuspended? CREATE_SUSPENDED: 0,   // Immediately run the thread or create suspended
      NULL);                                  // Thread Id

}

TThread::~TThread(void) {
}

//---------------------------------------------------------------------------
TObject *TThread::Get_FatalException(void) {
  return FFatalException;
}

//---------------------------------------------------------------------------
bool TThread::Get_FreeOnTerminate(void) {
  return FFreeOnTerminate;
}

bool TThread::Set_FreeOnTerminate(bool NewFreeOnTerminate) {
  if (FFreeOnTerminate != NewFreeOnTerminate) {
    FFreeOnTerminate = NewFreeOnTerminate;
  }

  return true;
}

//---------------------------------------------------------------------------
HANDLE TThread::Get_Handle(void) {
  return FHandle;
}

//---------------------------------------------------------------------------
TThreadPriority TThread::Get_Priority(void) {
  return FPriority;
}

bool TThread::Set_Priority(TThreadPriority NewPriority) {
  if (FPriority != NewPriority) {
    FPriority = NewPriority;
  }

  return true;
}

//---------------------------------------------------------------------------
int TThread::Get_ReturnValue(void) {
  return FReturnValue;
}

bool TThread::Set_ReturnValue(int NewReturnValue) {
  if (FReturnValue != NewReturnValue) {
    FReturnValue = NewReturnValue;
  }

  return true;
}

//---------------------------------------------------------------------------
bool TThread::Get_Suspended(void) {
  return FSuspended;
}

bool TThread::Set_Suspended(bool NewSuspended) {
  if (FSuspended != NewSuspended) {
    FSuspended = NewSuspended;
  }

  return true;
}

//---------------------------------------------------------------------------
bool TThread::Get_Terminated(void) {
  return FTerminated;
}

//---------------------------------------------------------------------------
int TThread::Get_ThreadID(void) {
  return FThreadID;
}

//---------------------------------------------------------------------------
// Lancement automatique d'un thread
//---------------------------------------------------------------------------
void FASTCALL TThread::AfterConstruction(void) {
}

//---------------------------------------------------------------------------
// Génère un évènement OnTerminate
//---------------------------------------------------------------------------

void FASTCALL TThread::DoTerminate(void) {
}

//---------------------------------------------------------------------------
// Redémarre un thread suspendu ou pas encore démarré
//---------------------------------------------------------------------------

void FASTCALL TThread::Resume(void) {
}

//---------------------------------------------------------------------------
// Arrête temporairement un thread.
//---------------------------------------------------------------------------

void FASTCALL TThread::Suspend(void) {
}

//---------------------------------------------------------------------------
// Appel d'une fonction du thread principal.
//---------------------------------------------------------------------------

void FASTCALL TThread::Synchronize(TThreadMethod &Method) {
}

//---------------------------------------------------------------------------
// Met la propriété Terminated à true.
//---------------------------------------------------------------------------

void FASTCALL TThread::Terminate(void) {
}

//---------------------------------------------------------------------------
// Attend l'arrêt du thread et retourne la valeur @b ReturnValue.
//-----------------------------------------------------------------------------

int FASTCALL TThread::WaitFor(void) {
	return 0;
}

DWORD WINAPI TThread::ThreadProc(LPVOID lpParam) {
  TThread *Thread;
  DWORD dwExitCode;


  if (lpParam == NULL) return (DWORD) -1;

  Thread = reinterpret_cast<TThread *>(lpParam);
  Thread->Execute();
  WaitForSingleObject(Thread->FHandle, INFINITE);
  GetExitCodeThread(Thread->FHandle, &dwExitCode);
  Thread->FHandle = NULL;

  return dwExitCode;
}

//---------------------------------------------------------------------------

