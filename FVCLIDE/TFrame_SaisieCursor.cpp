//-----------------------------------------------------------------------------
//! @file TFrame_SaisieCursor.cpp
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
#include <FreeVcl_ToolsEDI.h>


//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------
#include "TConfig.h"
#include "TFrame_SaisieCursor.h"
#include "TFVCLIDEAutomation.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
// Variables externes
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
TFrame_SaisieCursor::TFrame_SaisieCursor(TWinControl *AOwner)
  : TFrame_FVCLIDEBase(AOwner) {

  // Initialisations
	FFileType = ftf_Cursor;
  bNotChangeHotSpot = false;

  FUndoRedo = new TUndoRedo();

}

//---------------------------------------------------------------------------
TFrame_SaisieCursor::~TFrame_SaisieCursor(void) {
	Button_MiseAZeroClick(this);
	delete FUndoRedo;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété UndoRedo
//---------------------------------------------------------------------------

TUndoRedo *TFrame_SaisieCursor::Get_UndoRedo(void) {
  return FUndoRedo;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieCursor::CreateNewCursor(void) {

	Changed = true;

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieCursor::OpenFile(AnsiString asFileName) {
  TCURFile *Cursor;
  std::vector<TBitmap *> ListBitmaps;
	TCHAR szMsgErreur[256];
	bool Ok = false;
  int i;


	// Mise à zéro
  for (i = (int) ListElems.size() - 1; i >= 0; i--) {
    TabControl->Tabs->Delete(i);
    delete ListElems[i].Bitmap;
  }
  ListElems.clear();

  Cursor = new TCURFile();
  Ok = (Cursor->LoadFromFile(asFileName, ListBitmaps, szMsgErreur) == TRUE);
	if (Ok) {
		FFileName = asFileName;
		for (i = 0; i < (int) ListBitmaps.size(); i++) {
			// AjouterBitmap prend le contrôle de la Bitmap
			// Donc pas besoin de la détruire ensuite
			AjouterBitmap(ListBitmaps[i]);
		}
	}
  delete Cursor;

	return Ok;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieCursor::SaveToFile(AnsiString asFileName) {
  TCURFile *Cursor;
  std::vector<TBitmap *> ListBitmaps;
	TCHAR szMsgErreur[256];
	bool Ok = false;
  int i;


  Cursor = new TCURFile();
  for (i = 0; i < (int) ListElems.size(); i++) {
    ListBitmaps.push_back(ListElems[i].Bitmap);
  }
  Ok = (Cursor->SaveToFile(asFileName, ListBitmaps, szMsgErreur) == TRUE);
  delete Cursor;

	if (Ok) {
		FFileName = asFileName;

		// Nom long
		AnsiString asLongFileName;
		TCHAR * szLongFileName = new TCHAR[32767];
		if (GetLongPathName(asFileName, szLongFileName, 32767)) {
			asLongFileName = szLongFileName;
		}
		delete[] szLongFileName;
		if (asLongFileName.IsEmpty()) asLongFileName = asFileName;

		// Enregistrement dans la liste des fichiers récents
		Config->PushLastGraphic(asLongFileName);

	}


	return Ok;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieCursor::Undo(void) {
	AnsiString asOperation;
	bool bRet;

  bRet = FUndoRedo->Undo(&asOperation);
  FVCLIDEAutomation->DisplayMessage(td_Message, asOperation);
	if (FUndoRedo->IsEmpty(-1)) {
		if (FChanged) {
			FChanged = false;
			OnFlagChanged(this, FChanged);
		}
	}

	return bRet;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieCursor::Redo(void) {
	AnsiString asOperation;
	bool bRet;

  bRet = FUndoRedo->Redo(&asOperation);
  FVCLIDEAutomation->DisplayMessage(td_Message, asOperation);
	if (!FChanged) {
		FChanged = true;
		OnFlagChanged(this, FChanged);
	}

	return bRet;
}

//---------------------------------------------------------------------------
AnsiString TFrame_SaisieCursor::GetModificationsList(void) {
	return FUndoRedo->GetModificationsList(0);
}

//---------------------------------------------------------------------------
bool TFrame_SaisieCursor::ProcessUndo(void * Undo, int LenUndo) {
	LPUNDOCURADDIMAGE lpUndoAddCursor;
	LPUNDOCURDELIMAGE lpUndoDelCursor;
	LPUNDOCURMODIFIMAGE lpUndoModifCursor;
	LPUNDOCURMODIFHOTSPOT lpUndoModifHotSpot;
	TMemoryStream *BitmapStream;
	TBitmap *Bitmap;
	AnsiString asCaption;
	ELEMCUR NewElemCur;
	int SizeCur;
	int Index;


	ACTIONUNDOCUR ActionUndoCursor = *(ACTIONUNDOCUR *) Undo;

  switch (ActionUndoCursor) {

		case auAddCursor:
			lpUndoAddCursor = (LPUNDOCURADDIMAGE) Undo;
			Bitmap = new TBitmap();
			BitmapStream = new TMemoryStream(lpUndoAddCursor->Buffer, lpUndoAddCursor->SizeBuffer);
			Bitmap->LoadFromStream(BitmapStream);
			if (Bitmap->Width < Bitmap->Height) SizeCur = Bitmap->Width;
			else SizeCur = Bitmap->Height;
			NewElemCur.Bitmap = Bitmap;
			NewElemCur.Size = SizeCur;
			Index = lpUndoAddCursor->Index;
			asCaption.sprintf(_T("%i x %i"), SizeCur, SizeCur);
			ListElems.insert(ListElems.begin() + Index, NewElemCur);
			TabControl->Tabs->Insert(Index, asCaption);
			TabControl->TabIndex = Index;
      return true;

		case auDelCursor:
			lpUndoDelCursor = (LPUNDOCURDELIMAGE) Undo;
			Index = lpUndoDelCursor->Index;
			delete ListElems[Index].Bitmap;
			ListElems.erase(ListElems.begin() + Index);
			TabControl->Tabs->Delete(Index);
      return true;

    case auModifCursor:
			lpUndoModifCursor = (LPUNDOCURMODIFIMAGE) Undo;
			Index = lpUndoModifCursor->Index;
			BitmapStream = new TMemoryStream(lpUndoModifCursor->Buffer, lpUndoModifCursor->SizeBuffer);
			ListElems[Index].Bitmap->LoadFromStream(BitmapStream);
			delete BitmapStream;
			Bitmap = ListElems[Index].Bitmap;
      return true;

    case auModifHotSpot:
			lpUndoModifHotSpot = (LPUNDOCURMODIFHOTSPOT) Undo;
			Index = lpUndoModifHotSpot->Index;
			ListElems[Index].Bitmap->XHotSpot = lpUndoModifHotSpot->X;
			ListElems[Index].Bitmap->YHotSpot = lpUndoModifHotSpot->Y;
			PaintBox->Repaint();
			bNotChangeHotSpot = true;
			Edit_XHotSpot->Text = (AnsiString) ListElems[Index].Bitmap->XHotSpot;
			Edit_YHotSpot->Text = (AnsiString) ListElems[Index].Bitmap->YHotSpot;
			bNotChangeHotSpot = false;
      return true;

  }

  return false;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieCursor::ProcessRedo(void * Redo, int LenRedo) {
  // Les commandes undo et redo sont les mêmes
	return ProcessUndo(Redo, LenRedo);
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieCursor::CutToClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieCursor::CopyToClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieCursor::PasteFromClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::TabControlChange(TObject *Sender) {
  TBitmap *Bitmap;
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
    Bitmap = ListElems[Index].Bitmap;
		bNotChangeHotSpot = true;
		Edit_XHotSpot->Text = (AnsiString) ListElems[Index].Bitmap->XHotSpot;
		Edit_YHotSpot->Text = (AnsiString) ListElems[Index].Bitmap->YHotSpot;
		bNotChangeHotSpot = false;
  }

	Invalidate();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::Button_AjouterImageClick(TObject *Sender) {

  if (OpenDialogBmp->Execute()) {

		TBitmap *NewBitmap;
		AnsiString asCaption;
		ELEMCUR NewElemCur;
		LPUNDOCURADDIMAGE lpUndoAddCursor;
		LPUNDOCURDELIMAGE lpUndoDelCursor;
		TMemoryStream BitmapStream;
		unsigned int SizeBuffer;
		int Index;

		FUndoRedo->BeginOperation(0, _TT("Ajout d'une image"));

		NewBitmap = new TBitmap();
    NewBitmap->LoadFromFile(OpenDialogBmp->FileName);
    Index = AjouterBitmap(NewBitmap);
		Changed = true;  // Et non FChanged pour mettre à jour l'onglet

		// Mémorisation du Undo
		lpUndoDelCursor = (LPUNDOCURDELIMAGE) new BYTE[sizeof(UNDOCURDELIMAGE)];
		lpUndoDelCursor->ActionUndoCursor = auDelCursor;
		lpUndoDelCursor->Index = Index;
		FUndoRedo->MemoRedo(this, lpUndoDelCursor, sizeof(UNDOCURMODIFIMAGE));
		delete lpUndoDelCursor;

		// Mémorisation du Redo
		ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
		SizeBuffer = (unsigned int) BitmapStream.Size;
		lpUndoAddCursor = (LPUNDOCURADDIMAGE) new BYTE[sizeof(UNDOCURADDIMAGE) + SizeBuffer - 1];
		lpUndoAddCursor->ActionUndoCursor = auAddCursor;
		lpUndoAddCursor->Index = Index;
		lpUndoAddCursor->SizeBuffer = SizeBuffer;
		BitmapStream.Position = 0;
		BitmapStream.Read(lpUndoAddCursor->Buffer, SizeBuffer);
		FUndoRedo->MemoUndo(this, lpUndoAddCursor, sizeof(UNDOCURADDIMAGE) + SizeBuffer - 1);
		delete lpUndoAddCursor;

	}

}
//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::Button_EnleverImageClick(TObject *Sender) {
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
		LPUNDOCURMODIFIMAGE lpUndoModifCursor;
		TMemoryStream BitmapStream;
		unsigned int SizeBuffer;

		FUndoRedo->BeginOperation(0, _TT("Suppression d'une image"));

		// Mémorisation du Undo
		ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
		SizeBuffer = (unsigned int) BitmapStream.Size;
		lpUndoModifCursor = (LPUNDOCURMODIFIMAGE) new BYTE[sizeof(UNDOCURMODIFIMAGE) + SizeBuffer - 1];
		lpUndoModifCursor->ActionUndoCursor = auAddCursor;
		lpUndoModifCursor->Index = Index;
		lpUndoModifCursor->SizeBuffer = SizeBuffer;
		BitmapStream.Position = 0;
		BitmapStream.Read(lpUndoModifCursor->Buffer, SizeBuffer);
		FUndoRedo->MemoUndo(this, lpUndoModifCursor, sizeof(UNDOCURMODIFIMAGE) + SizeBuffer - 1);
		delete lpUndoModifCursor;

    delete ListElems[Index].Bitmap;
    ListElems.erase(ListElems.begin() + Index);
    TabControl->Tabs->Delete(Index);
		Changed = true;  // Et non FChanged pour mettre à jour l'onglet

		// Mémorisation du Redo
		lpUndoModifCursor = (LPUNDOCURMODIFIMAGE) new BYTE[sizeof(UNDOCURMODIFIMAGE)];
		lpUndoModifCursor->ActionUndoCursor = auDelCursor;
		lpUndoModifCursor->Index = Index;
		FUndoRedo->MemoRedo(this, lpUndoModifCursor, sizeof(UNDOCURMODIFIMAGE));
		delete lpUndoModifCursor;

		FUndoRedo->EndOperation();

  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::Button_EnregistrerImageSousClick(TObject *Sender) {
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
    if (SaveDialogBmp->Execute()) {
      ListElems[Index].Bitmap->SaveToFile(SaveDialogBmp->FileName);
    }
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::Button_EditerImageClick(TObject *Sender) {
  AnsiString asFileName;
  int Index;
	WIN32_FILE_ATTRIBUTE_DATA FileAttributeData;
	FILETIME ftFileNameTime1, ftFileNameTime2;



  Index = TabControl->TabIndex;
  if (Index != -1) {
    asFileName = ListElems[Index].asFileName;
		if (asFileName == _T("")) {
			TCHAR szTempPath[MAX_PATH];
			GetTempPath(MAX_PATH, szTempPath);
			asFileName = AnsiString(szTempPath) + _T("Temp.bmp");
      ListElems[Index].Bitmap->SaveToFile(asFileName);
		}

		// Date time du fichier avant modif
		if (GetFileAttributesEx(asFileName, GetFileExInfoStandard, &FileAttributeData) == TRUE) {
			ftFileNameTime1 = FileAttributeData.ftLastWriteTime;
		}
		else {
			ftFileNameTime1.dwHighDateTime = 0;
			ftFileNameTime1.dwLowDateTime = 0;
		}

		HINSTANCE hInstBmp = ShellExecute(Handle, _T("open"), asFileName.c_str(), NULL, _T("."), SW_SHOWDEFAULT);
		MessageBox(FHandle, _T("Modifiez l'image et validez"), _T("Modification"), MB_OK);

		// Date time du fichier après modif
		if (GetFileAttributesEx(asFileName, GetFileExInfoStandard, &FileAttributeData) == TRUE) {
			ftFileNameTime2 = FileAttributeData.ftLastWriteTime;
		}
		else {
			ftFileNameTime2.dwHighDateTime = 0;
			ftFileNameTime2.dwLowDateTime = 0;
		}

		if (ftFileNameTime1.dwHighDateTime != ftFileNameTime2.dwHighDateTime ||
				ftFileNameTime1.dwLowDateTime != ftFileNameTime2.dwLowDateTime) {
			LPUNDOCURMODIFIMAGE lpUndoModifCursor;
			TMemoryStream BitmapStream;
			unsigned int SizeBuffer;

			FUndoRedo->BeginOperation(0, _TT("Modification d'une image"));

			// Mémorisation du Undo
			ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
			SizeBuffer = (unsigned int) BitmapStream.Size;
			lpUndoModifCursor = (LPUNDOCURMODIFIMAGE) new BYTE[sizeof(UNDOCURMODIFIMAGE) + SizeBuffer - 1];
			lpUndoModifCursor->ActionUndoCursor = auModifCursor;
			lpUndoModifCursor->Index = Index;
			lpUndoModifCursor->SizeBuffer = SizeBuffer;
			BitmapStream.Position = 0;
			BitmapStream.Read(lpUndoModifCursor->Buffer, SizeBuffer);
			FUndoRedo->MemoUndo(this, lpUndoModifCursor, sizeof(UNDOCURMODIFIMAGE) + SizeBuffer - 1);
			delete lpUndoModifCursor;

      ListElems[Index].Bitmap->LoadFromFile(asFileName);
			Changed = true;  // Et non FChanged pour mettre à jour l'onglet

			// Mémorisation du Redo
			BitmapStream.Clear();
			ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
			SizeBuffer = (unsigned int) BitmapStream.Size;
			lpUndoModifCursor = (LPUNDOCURMODIFIMAGE) new BYTE[sizeof(UNDOCURMODIFIMAGE) + SizeBuffer - 1];
			lpUndoModifCursor->ActionUndoCursor = auModifCursor;
			lpUndoModifCursor->Index = Index;
			lpUndoModifCursor->SizeBuffer = SizeBuffer;
			BitmapStream.Position = 0;
			BitmapStream.Read(lpUndoModifCursor->Buffer, SizeBuffer);
			FUndoRedo->MemoRedo(this, lpUndoModifCursor, sizeof(UNDOCURMODIFIMAGE) + SizeBuffer - 1);
			delete lpUndoModifCursor;

			FUndoRedo->EndOperation();

		}

		DeleteFile(asFileName);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::Button_MiseAZeroClick(TObject *Sender) {
  int i;


  for (i = (int) ListElems.size() - 1; i >= 0; i--) {
    TabControl->Tabs->Delete(i);
    delete ListElems[i].Bitmap;
  }
  ListElems.clear();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::PaintBoxPaint(TObject *Sender) {
  TBitmap *Bitmap;
  TRect Rect1;
  TRect Rect2;
  int SizeCur;
  int Index;


  Rect2 = PaintBox->ClientRect;
  Index = TabControl->TabIndex;
  if (Index != -1) {
    Bitmap = ListElems[Index].Bitmap;
    if (Bitmap->Width < Bitmap->Height) SizeCur = Bitmap->Width;
    else SizeCur = Bitmap->Height;
    Rect1.Left = 0;
    Rect1.Top = 0;
    Rect1.Right = SizeCur;
    Rect1.Bottom = SizeCur;
    if (Rect2.Width < Rect2.Height) {
      Rect2.Bottom = Rect2.Top + Rect2.Width;
    }
    else {
      Rect2.Right = Rect2.Left + Rect2.Height;
    }

    PaintBox->Canvas->CopyRect(Rect2, Bitmap->Canvas, Rect1);

		// Hotspot
		PaintBox->Canvas->Pen->Color = clRed;
		PaintBox->Canvas->Brush->Style = bsSolid;
		int X1 = Rect2.Left + ListElems[Index].Bitmap->XHotSpot * (Rect2.Right - Rect2.Left) / (Rect1.Right - Rect1.Left);
		int Y1 = Rect2.Top + ListElems[Index].Bitmap->YHotSpot * (Rect2.Bottom - Rect2.Top) / (Rect1.Bottom - Rect1.Top);
		int X2 = X1 + (Rect2.Right - Rect2.Left) / (Rect1.Right - Rect1.Left);
		int Y2 = Y1 + (Rect2.Bottom - Rect2.Top) / (Rect1.Bottom - Rect1.Top);
		PaintBox->Canvas->Brush->Color = clYellow;
		PaintBox->Canvas->Ellipse(X1, Y1, X2, Y2);
  }
  else {
    PaintBox->Canvas->Pen->Style = psSolid;
    PaintBox->Canvas->Pen->Color = clBlack;
    PaintBox->Canvas->Rectangle(PaintBox->ClientRect);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::PaintBoxMouseMove(
        TObject *Sender, TShiftState Shift, int X, int Y) {
  TBitmap *Bitmap;
	AnsiString asMessage;
	int XImage, YImage;
  int SizeCur;
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
		Bitmap = ListElems[Index].Bitmap;
    if (Bitmap->Width < Bitmap->Height) SizeCur = Bitmap->Width;
    else SizeCur = Bitmap->Height;
		XImage = (int) (X * SizeCur / PaintBox->Width);
		YImage = (int) (Y * SizeCur / PaintBox->Height);
		asMessage.sprintf(_T("%i, %i"), YImage, XImage);
		FVCLIDEAutomation->DisplayMessage(td_Position, asMessage);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::PaintBoxMaskPaint(TObject *Sender) {
  TBitmap *Bitmap;
  TRect Rect1;
  TRect Rect2;
  int SizeCur;
  int Index;


  Rect2 = PaintBoxMask->ClientRect;
  Index = TabControl->TabIndex;
  if (Index != -1) {
    Bitmap = ListElems[Index].Bitmap;
    if (Bitmap->Width < Bitmap->Height) SizeCur = Bitmap->Width;
    else SizeCur = Bitmap->Height;
    if (Bitmap->Height >= 2 * SizeCur) {
      Rect1.Left = 0;
      Rect1.Top = SizeCur;
      Rect1.Right = SizeCur;
      Rect1.Bottom = 2 * SizeCur;
      PaintBoxMask->Canvas->CopyRect(Rect2, Bitmap->Canvas, Rect1);
    }
    else {
      PaintBoxMask->Canvas->Brush->Style = bsSolid;
      PaintBoxMask->Canvas->Brush->Color = clBlack;
      PaintBoxMask->Canvas->FillRect(Rect2);
    }

  }
  else {
    PaintBoxMask->Canvas->Brush->Style = bsSolid;
    PaintBoxMask->Canvas->Brush->Color = clBlack;
    PaintBoxMask->Canvas->FillRect(Rect2);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::PaintBoxMaskMouseMove(
        TObject *Sender, TShiftState Shift, int X, int Y) {
  TBitmap *Bitmap;
	AnsiString asMessage;
	int XImage, YImage;
  int SizeCur;
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
		Bitmap = ListElems[Index].Bitmap;
    if (Bitmap->Width < Bitmap->Height) SizeCur = Bitmap->Width;
    else SizeCur = Bitmap->Height;
		XImage = (int) (X * SizeCur / PaintBoxMask->Width);
		YImage = (int) (Y * SizeCur / PaintBoxMask->Height);
		asMessage.sprintf(_T("%i, %i"), YImage, XImage);
		FVCLIDEAutomation->DisplayMessage(td_Position, asMessage);
	}
}

//---------------------------------------------------------------------------
void TFrame_SaisieCursor::Edit_XHotSpotChange(TObject *Sender) {
  int Index;
	UNDOCURMODIFHOTSPOT UndoModifHotSpot;


	if (!bNotChangeHotSpot) {

		Index = TabControl->TabIndex;
		if (Index != -1) {

			FUndoRedo->BeginOperation(0, _TT("Modification hotspot"));

			// Mémorisation du Undo
			UndoModifHotSpot.ActionUndoCursor = auModifHotSpot;
			UndoModifHotSpot.Index = Index;
			UndoModifHotSpot.X = ListElems[Index].Bitmap->XHotSpot;
			UndoModifHotSpot.Y = ListElems[Index].Bitmap->YHotSpot;
			FUndoRedo->MemoUndo(this, &UndoModifHotSpot, sizeof(UNDOCURMODIFHOTSPOT));

			ListElems[Index].Bitmap->XHotSpot = ((AnsiString) Edit_XHotSpot->Text).ToIntDef(0);
			Changed = true;  // Et non FChanged pour mettre à jour l'onglet

			// Mémorisation du Redo
			UndoModifHotSpot.ActionUndoCursor = auModifHotSpot;
			UndoModifHotSpot.Index = Index;
			UndoModifHotSpot.X = ListElems[Index].Bitmap->XHotSpot;
			UndoModifHotSpot.Y = ListElems[Index].Bitmap->YHotSpot;
			FUndoRedo->MemoRedo(this, &UndoModifHotSpot, sizeof(UNDOCURMODIFHOTSPOT));

			FUndoRedo->EndOperation();

			PaintBox->Repaint();
		}
	}
}

//---------------------------------------------------------------------------
void TFrame_SaisieCursor::Edit_YHotSpotChange(TObject *Sender) {
  int Index;
	UNDOCURMODIFHOTSPOT UndoModifHotSpot;


	if (!bNotChangeHotSpot) {

		Index = TabControl->TabIndex;
		if (Index != -1) {

			FUndoRedo->BeginOperation(0, _TT("Modification hotspot"));

			// Mémorisation du Undo
			UndoModifHotSpot.ActionUndoCursor = auModifHotSpot;
			UndoModifHotSpot.Index = Index;
			UndoModifHotSpot.X = ListElems[Index].Bitmap->XHotSpot;
			UndoModifHotSpot.Y = ListElems[Index].Bitmap->YHotSpot;
			FUndoRedo->MemoUndo(this, &UndoModifHotSpot, sizeof(UNDOCURMODIFHOTSPOT));

			ListElems[Index].Bitmap->YHotSpot = ((AnsiString) Edit_YHotSpot->Text).ToIntDef(0);
			Changed = true;  // Et non FChanged pour mettre à jour l'onglet

			// Mémorisation du Redo
			UndoModifHotSpot.ActionUndoCursor = auModifHotSpot;
			UndoModifHotSpot.Index = Index;
			UndoModifHotSpot.X = ListElems[Index].Bitmap->XHotSpot;
			UndoModifHotSpot.Y = ListElems[Index].Bitmap->YHotSpot;
			FUndoRedo->MemoRedo(this, &UndoModifHotSpot, sizeof(UNDOCURMODIFHOTSPOT));

			FUndoRedo->EndOperation();

			PaintBox->Repaint();
		}
	}
}

//---------------------------------------------------------------------------
int TFrame_SaisieCursor::AjouterBitmap(TBitmap *Bitmap) {
  AnsiString asCaption;
  ELEMCUR NewElemCur;
  int SizeCur;
  int i;
  int Index;


  if (Bitmap->Width < Bitmap->Height) SizeCur = Bitmap->Width;
  else SizeCur = Bitmap->Height;
  NewElemCur.asFileName = OpenDialogBmp->FileName;
  NewElemCur.Bitmap = Bitmap;
  NewElemCur.Size = SizeCur;
  Index = -1;
  for (i = 0; i < (int) ListElems.size(); i++) {
    if (ListElems[i].Size >= SizeCur) {
      Index = i;
      break;
    }
  }

  asCaption.sprintf(_T("%i x %i"), SizeCur, SizeCur);
  if (Index != -1) {
    ListElems.insert(ListElems.begin() + Index, NewElemCur);
    TabControl->Tabs->Insert(Index, asCaption);
    TabControl->TabIndex = Index;
  }
  else {
    ListElems.push_back(NewElemCur);
    TabControl->Tabs->Add(asCaption);
    TabControl->TabIndex = TabControl->Tabs->Count - 1;
    Index = (int) ListElems.size() - 1;
  }

	return Index;
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieCursor::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIEDIALOG);
}

//---------------------------------------------------------------------------
FVCL_RegisterComponent(TFrame_SaisieCursor);

//---------------------------------------------------------------------------
