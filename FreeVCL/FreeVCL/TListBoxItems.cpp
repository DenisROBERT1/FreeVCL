//-----------------------------------------------------------------------------
//! @file TListBoxItems.cpp
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

// Handle d'instance
extern HINSTANCE HInstance;


//---------------------------------------------------------------------------
// TListBoxItems
//---------------------------------------------------------------------------

TListBoxItems::TListBoxItems(void): TStrings() {
  FHandle = NULL;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Handle
//---------------------------------------------------------------------------

HWND TListBoxItems::Get_Handle(void) {
  return FHandle;
}

bool TListBoxItems::Set_Handle(HWND NewHandle) {

  if (FHandle != NewHandle) {
    FHandle = NewHandle;
  }
  return true;
}

//---------------------------------------------------------------------------
int FASTCALL TListBoxItems::GetCount(void) const {
  return (int) SendMessage(FHandle, LB_GETCOUNT, 0, 0);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Strings
//---------------------------------------------------------------------------

AnsiString TListBoxItems::Get_Strings(int i) {
  int Len;
  LPTSTR szString;
  AnsiString Retour;


  Len = (int) SendMessage(FHandle, LB_GETTEXTLEN, i, 0);
  if (Len != CB_ERR) {
    szString = new TCHAR[Len + 1];
    SendMessage(FHandle, LB_GETTEXT, i, (LPARAM) (LPSTR) szString);
    Retour = AnsiString(szString);
    delete[] szString;
  }

  return Retour;
}

//---------------------------------------------------------------------------
bool TListBoxItems::Set_Strings(int i, AnsiString NewStrings) {
  if (FHandle) {
    SendMessage(FHandle, LB_DELETESTRING, i, 0);
    SendMessage(FHandle, LB_INSERTSTRING, i, (LPARAM) (LPCTSTR) NewStrings);
		OnChange(this);
  }

  return true;
}

//---------------------------------------------------------------------------
void TListBoxItems::Clear(void) {
  // int Command;


  if (FHandle) {
    SendMessage(FHandle, LB_RESETCONTENT, 0, 0);
		OnChange(this);
		/* Mis en commentaires (remplacé par l'évènement OnChange)
    // Envoi du message LBN_SELCHANGE qui n'est pas envoyé automatiquement
    // (pour évènement OnChange et mise à jour de ItemIndex)
    Command = (int) (SIZE_T) GetWindowLongPtr(FHandle, GWL_ID);
    SendMessage(GetParent(FHandle), WM_COMMAND, MAKEWPARAM(Command, LBN_SELCHANGE), (LPARAM) FHandle);
		*/
  }
}

//---------------------------------------------------------------------------
int TListBoxItems::Add(const AnsiString S) {
  int Retour = -1;

  if (FHandle) {
		Retour = (int) SendMessage(FHandle, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) S);
		OnChange(this);
	}

  return Retour;
}

//---------------------------------------------------------------------------
void TListBoxItems::Insert(int Index, const AnsiString S) {
  // int Command;


  if (FHandle) {
    SendMessage(FHandle, LB_INSERTSTRING, Index, (LPARAM) (LPCTSTR) S);
		OnChange(this);
		/* Mis en commentaires (remplacé par l'évènement OnChange)
    // Envoi du message LBN_SELCHANGE qui n'est pas envoyé automatiquement
    // (pour évènement OnChange et mise à jour de ItemIndex)
    Command = (int) (SIZE_T) GetWindowLongPtr(FHandle, GWL_ID);
    SendMessage(GetParent(FHandle), WM_COMMAND, MAKEWPARAM(Command, LBN_SELCHANGE), (LPARAM) FHandle);
		*/
  }
}

//---------------------------------------------------------------------------
void TListBoxItems::Delete(int Index) {
  // int Command;


  if (FHandle) {
    SendMessage(FHandle, LB_DELETESTRING, Index, 0);
		OnChange(this);
		/* Mis en commentaires (remplacé par l'évènement OnChange)
    // Envoi du message LBN_SELCHANGE qui n'est pas envoyé automatiquement
    // (pour évènement OnChange et mise à jour de ItemIndex)
    Command = (int) (SIZE_T) GetWindowLongPtr(FHandle, GWL_ID);
    SendMessage(GetParent(FHandle), WM_COMMAND, MAKEWPARAM(Command, LBN_SELCHANGE), (LPARAM) FHandle);
		*/
  }
}

//---------------------------------------------------------------------------
TObject *FASTCALL TListBoxItems::GetObject(int Index) {
  if (FHandle) {
    return (TObject *) SendMessage(FHandle, LB_GETITEMDATA, (WPARAM) Index, 0);
  }

	return NULL;
}

//---------------------------------------------------------------------------
void FASTCALL TListBoxItems::PutObject(int Index, TObject *Object) {
  if (FHandle) {
    SendMessage(FHandle, LB_SETITEMDATA, (WPARAM) Index, (LPARAM) Object);
  }
}

//---------------------------------------------------------------------------

