//-----------------------------------------------------------------------------
//! @file TUndoRedo.cpp
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
#include <FreeVcl.h>

#include "TUndoRedo.h"

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

//---------------------------------------------------------------------------
TUndoRedo::TUndoRedo(void) {
  TCHAR szLibTemp[MAX_PATH];          // Répertoire temporaire
  int i;


  // Initialisations
  DebutOp = 0;
  bOperationName = false;

  // Nom du fichier UNDO
  GetTempPath(MAX_PATH, szLibTemp);
  for (i = 0; i < 10000; i++) {
    asFileUndo.sprintf(_T("%sUNDO%04i.TMP"), (LPSTR) szLibTemp, i);
    if (!FileExists(asFileUndo)) break;
  }

  if (i < 10000) {

    // Création du fichier UNDO
    PosFileUndo = 0;
    hFileUndo = CreateFile(asFileUndo, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFileUndo == INVALID_HANDLE_VALUE) {
      MessageBox(NULL, _T("Impossible d'ouvrir le fichier"), asFileUndo, MB_OK | MB_ICONWARNING);
      asFileUndo = _T("");
    }

    // Allocation buffer UNDO
    hgBufferUndo = GlobalAlloc(GHND, 1024);

  }
  else asFileUndo = _T("");

}


//---------------------------------------------------------------------------
TUndoRedo::~TUndoRedo(void) {
  CloseHandle(hFileUndo);
  if (!asFileUndo.IsEmpty()) DeleteFile(asFileUndo);

  GlobalFree(hgBufferUndo);
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::Clear(void) {

  // Test opération en cours
  FVCL_ASSERT(DebutOp == 0 && _T("Une opération est en cours d'exécution"));

  SetFilePointer(hFileUndo, 0, NULL, FILE_BEGIN);
  SetEndOfFile(hFileUndo);
  PosFileUndo = 0;

  return true;
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::BeginOperation(int Categorie, AnsiString asOperationName) {
  LPBUFFERUNDO lpBufferUndo;
	LPBEGINOP lpBeginOp;
	unsigned int LenUndoRedo;
  GLOBALHANDLE hgTmp;


  // Test opération en cours
  if (!DebutOp) {

		LenUndoRedo = sizeof(BEGINOP) + (asOperationName.Length() + 1) * sizeof(TCHAR);

		// Si buffer trop petit: réallocation
		if (GlobalSize(hgBufferUndo) < sizeof(BUFFERUNDO) + (unsigned) LenUndoRedo) {
			hgTmp = GlobalAlloc(GHND, sizeof(BUFFERUNDO) + LenUndoRedo);
			if (hgTmp == NULL) return false;
			GlobalFree(hgBufferUndo);
			hgBufferUndo = hgTmp;
		}

    lpBufferUndo = (LPBUFFERUNDO) GlobalLock(hgBufferUndo);

    lpBufferUndo->Sender = NULL;
    lpBufferUndo->bUndo = false;
    lpBufferUndo->LngData = LenUndoRedo;

		lpBeginOp = (LPBEGINOP) lpBufferUndo->Data;
		lpBeginOp->Categorie = Categorie;
    lstrcpy((LPTSTR) lpBeginOp->Info, asOperationName);
    bOperationName = true;

    GlobalUnlock(hgBufferUndo);

  }

  DebutOp++;

  return true;
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::EndOperation(void) {
  bool Ok = false;


  // Test opération en cours
  FVCL_ASSERT(DebutOp != 0 && _T("Aucune opération en cours"));

  // Décrémentation du nombre d'opérations
  DebutOp--;

  if (DebutOp == 0) {

    // Si aucune modif n'a été faite: on renvoie faux
    Ok = !bOperationName;

    // Mise à zéro des flags
    bOperationName = false;

  }

  return Ok;
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::AbortOperation(AnsiString *asOperationName) {
  bool Ok;


  Ok = EndOperation();
  if (Ok) Undo(asOperationName);

  return Ok;
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::MemoUndo(TUndoRedoUser * Sender,
    void * Undo, unsigned int LenUndo) {
  return MemoUndoRedo(Sender, Undo, LenUndo, true);
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::MemoRedo(TUndoRedoUser * Sender,
    void * Redo, unsigned int LenRedo) {
  return MemoUndoRedo(Sender, Redo, LenRedo, false);
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::MemoUndoRedo(TUndoRedoUser * Sender,
    void * UndoRedo, unsigned int LenUndoRedo, bool bUndo) {
  LPBUFFERUNDO lpBufferUndo;
  GLOBALHANDLE hgTmp;


  // Test opération en cours
  FVCL_ASSERT(DebutOp != 0 && _T("Aucune opération en cours"));

  // Si c'est la première opération: enregistrement du nom de l'opération
  if (bOperationName) {
    lpBufferUndo = (LPBUFFERUNDO) GlobalLock(hgBufferUndo);
    WriteBuffer(lpBufferUndo);
    GlobalUnlock(hgBufferUndo);
    bOperationName = false;
  }

  // Si buffer trop petit: réallocation
  if (GlobalSize(hgBufferUndo) < sizeof(BUFFERUNDO) + LenUndoRedo) {
    hgTmp = GlobalAlloc(GHND, sizeof(BUFFERUNDO) + LenUndoRedo);
    if (hgTmp == NULL) return false;
		GlobalFree(hgBufferUndo);
    hgBufferUndo = hgTmp;
  }

  lpBufferUndo = (LPBUFFERUNDO) GlobalLock(hgBufferUndo);

  // Enregistrement du undo
  lpBufferUndo->Sender = Sender;
  lpBufferUndo->bUndo = bUndo;
  lpBufferUndo->LngData = LenUndoRedo;
  memcpy(lpBufferUndo->Data, UndoRedo, LenUndoRedo);
  WriteBuffer(lpBufferUndo);

  GlobalUnlock(hgBufferUndo);

  // Fichier tronqué (si des undos ont déjà été faits précédement)
  SetEndOfFile(hFileUndo);

  return true;
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::Undo(AnsiString *asOperationName) {
  LPBUFFERUNDO lpBufferUndo;
  bool Ok;


  // Test opération en cours
  FVCL_ASSERT(DebutOp == 0 && _T("Une opération est en cours d'exécution"));

  lpBufferUndo = (LPBUFFERUNDO) GlobalLock(hgBufferUndo);

  for (;;) {

    // Si début de fichier atteinte -> Retour = faux
    if (!PrevBuffer(lpBufferUndo)) {
      (*asOperationName) = _T("Toutes les opérations ont été défaites");
      Ok = false;
      break;
    }

    // Si flag de début d'opération -> Undo terminé correctement
    if (lpBufferUndo->Sender == NULL) {
			LPBEGINOP lpBeginOp = (LPBEGINOP) lpBufferUndo->Data;
      (*asOperationName) = (LPTSTR) lpBeginOp->Info;
      Ok = true;
      break;
    }

    // Exécution du undo (si c'est un enregistrement undo)
    if (lpBufferUndo->bUndo) lpBufferUndo->Sender->ProcessUndo(lpBufferUndo->Data, lpBufferUndo->LngData);

  }

  GlobalUnlock(hgBufferUndo);

  return Ok;
}


//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::Redo(AnsiString * asOperationName) {
  LPBUFFERUNDO lpBufferUndo;
  bool Ok;
  bool bPremier;


  // Test opération en cours
  FVCL_ASSERT(DebutOp == 0 && _T("Une opération est en cours d'exécution"));

  lpBufferUndo = (LPBUFFERUNDO) GlobalLock(hgBufferUndo);

  for (bPremier = true;;) {

    // Si fin de fichier atteinte
    // Soit le premier a déjà été lu -> Redo terminé correctement
    // Soit le premier n'a pas encore été lu -> Tout a déjà été refait
    if (!NextBuffer(lpBufferUndo)) {
      if (!bPremier) {
        Ok = true;
      }
      else {
        (*asOperationName) = _T("Toutes les opérations ont été refaites");
        Ok = false;
      }
      break;
    }

    // Si flag de début d'opération
    // Soit c'est le premier -> on continue
    // Soit c'est le suivant -> Undo terminé correctement (mais un enr lu en trop => PrevBuffer)
    if (lpBufferUndo->Sender == NULL) {
      if (bPremier) {
				LPBEGINOP lpBeginOp = (LPBEGINOP) lpBufferUndo->Data;
        (*asOperationName) = (LPTSTR) lpBeginOp->Info;
        bPremier = false;
      }
      else {
        PrevBuffer(lpBufferUndo);
        Ok = true;
        break;
      }
    }

    else {
      // Exécution du redo (si c'est un enregistrement redo)
      if (!lpBufferUndo->bUndo) lpBufferUndo->Sender->ProcessRedo(lpBufferUndo->Data, lpBufferUndo->LngData);
    }

  }

  GlobalUnlock(hgBufferUndo);

  return Ok;
}

//---------------------------------------------------------------------------
AnsiString TUndoRedo::GetModificationsList(int Categorie) {
  LPBUFFERUNDO lpBufferUndo;
  AnsiString asListe;
  unsigned int MemoPosFileUndo;
  int NbLignes;


  MemoPosFileUndo = PosFileUndo;
  PosFileUndo = 0;
  NbLignes = 0;
  lpBufferUndo = (LPBUFFERUNDO) GlobalLock(hgBufferUndo);

  while (NextBuffer(lpBufferUndo) && PosFileUndo < MemoPosFileUndo) {
    if (lpBufferUndo->Sender == NULL) {
			LPBEGINOP lpBeginOp = (LPBEGINOP) lpBufferUndo->Data;
			if (Categorie == -1 || Categorie == lpBeginOp->Categorie) {
				if (NbLignes != 0) asListe += _T("\n");
				if (++NbLignes <= 3) asListe += (LPTSTR) lpBeginOp->Info;
				else {
					asListe += _T("etc...");
					break;
				}
			}
    }
  }

  GlobalUnlock(hgBufferUndo);
  PosFileUndo = MemoPosFileUndo;

  return asListe;
}

//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::IsEmpty(int Categorie) {
  LPBUFFERUNDO lpBufferUndo;
  unsigned int MemoPosFileUndo;
	bool bFound;


	if (PosFileUndo == 0) return true;

  MemoPosFileUndo = PosFileUndo;
  lpBufferUndo = (LPBUFFERUNDO) GlobalLock(hgBufferUndo);

	bFound = false;
  while (PrevBuffer(lpBufferUndo)) {
    if (lpBufferUndo->Sender == NULL) {
			LPBEGINOP lpBeginOp = (LPBEGINOP) lpBufferUndo->Data;
			if (Categorie == -1 || Categorie == lpBeginOp->Categorie) {
				bFound = true;
				break;
			}
    }
	}

  GlobalUnlock(hgBufferUndo);
  PosFileUndo = MemoPosFileUndo;

	return !bFound;
}

//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::WriteBuffer(LPBUFFERUNDO lpBufferUndo) {
  unsigned int SizeBuffer;
  DWORD NbOctets;


  SetFilePointer(hFileUndo, PosFileUndo, NULL, FILE_BEGIN);
  SizeBuffer = sizeof(BUFFERUNDO) + lpBufferUndo->LngData;
  WriteFile(hFileUndo, lpBufferUndo, SizeBuffer, &NbOctets, NULL);
  if (NbOctets != SizeBuffer) return false;
  WriteFile(hFileUndo, &SizeBuffer, sizeof(SizeBuffer), &NbOctets, NULL);
  if (NbOctets != sizeof(SizeBuffer)) return false;
  PosFileUndo += SizeBuffer + sizeof(SizeBuffer);

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::PrevBuffer(LPBUFFERUNDO lpBufferUndo) {
  unsigned int SizeBuffer;
  DWORD NbOctets;


	if (PosFileUndo < sizeof(SizeBuffer)) return false;
  SetFilePointer(hFileUndo, PosFileUndo - sizeof(SizeBuffer), NULL, FILE_BEGIN);
  ReadFile(hFileUndo, &SizeBuffer, sizeof(SizeBuffer), &NbOctets, NULL);
  if (NbOctets != sizeof(SizeBuffer)) return false;
  SetFilePointer(hFileUndo, PosFileUndo - SizeBuffer - sizeof(SizeBuffer), NULL, FILE_BEGIN);
  ReadFile(hFileUndo, lpBufferUndo, SizeBuffer, &NbOctets, NULL);
  if (NbOctets != SizeBuffer) return false;
  if (SizeBuffer != sizeof(BUFFERUNDO) + lpBufferUndo->LngData) return false;

  PosFileUndo -= SizeBuffer + sizeof(SizeBuffer);

  return true;
}

//---------------------------------------------------------------------------
bool FASTCALL TUndoRedo::NextBuffer(LPBUFFERUNDO lpBufferUndo) {
  unsigned int SizeBuffer;
  DWORD NbOctets;



  SetFilePointer(hFileUndo, PosFileUndo, NULL, FILE_BEGIN);
  ReadFile(hFileUndo, lpBufferUndo, sizeof(BUFFERUNDO), &NbOctets, NULL);
  if (NbOctets != sizeof(BUFFERUNDO)) return false;
  ReadFile(hFileUndo, lpBufferUndo->Data, lpBufferUndo->LngData, &NbOctets, NULL);
  if (NbOctets != lpBufferUndo->LngData) return false;
  ReadFile(hFileUndo, &SizeBuffer, sizeof(SizeBuffer), &NbOctets, NULL);
  if (NbOctets != sizeof(SizeBuffer)) return false;
  if (SizeBuffer != sizeof(BUFFERUNDO) + lpBufferUndo->LngData) return false;

  PosFileUndo += SizeBuffer + sizeof(SizeBuffer);

  return true;
}
//---------------------------------------------------------------------------
