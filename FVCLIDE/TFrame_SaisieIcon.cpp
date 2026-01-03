//-----------------------------------------------------------------------------
//! @file TFrame_SaisieIcon.cpp
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
#include "TFrame_SaisieIcon.h"
#include "TFVCLIDEAutomation.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
// Variables externes
//---------------------------------------------------------------------------

extern TFactory_FVCL Factory_FVCL;


//---------------------------------------------------------------------------
TFrame_SaisieIcon::TFrame_SaisieIcon(TWinControl *AOwner)
  : TFrame_FVCLIDEBase(AOwner) {

  // Initialisations
	FFileType = ftf_Icon;

  FUndoRedo = new TUndoRedo();

}

//---------------------------------------------------------------------------
TFrame_SaisieIcon::~TFrame_SaisieIcon(void) {
	Button_MiseAZeroClick(this);
	delete FUndoRedo;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété UndoRedo
//---------------------------------------------------------------------------

TUndoRedo *TFrame_SaisieIcon::Get_UndoRedo(void) {
  return FUndoRedo;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieIcon::CreateNewIcon(void) {

	Changed = true;

	return true;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieIcon::OpenFile(AnsiString asFileName) {
  TIcon *Icon;
  TBitmap *NewBitmap;
	bool Ok = false;
  int i;

	// Mise à zéro
  for (i = (int) ListElems.size() - 1; i >= 0; i--) {
    TabControl->Tabs->Delete(i);
    delete ListElems[i].Bitmap;
  }
  ListElems.clear();

  Icon = new TIcon();
  Ok = Icon->LoadFromFile(asFileName);
	if (Ok) {
		FFileName = asFileName;
		for (i = 0; i < Icon->BitmapCount; i++) {
			NewBitmap = new TBitmap();
			NewBitmap->Assign(Icon->Bitmap[i]);
			AjouterBitmap(NewBitmap);
		}
	}
  delete Icon;

	return Ok;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieIcon::SaveToFile(AnsiString asFileName) {
  TIcon *Icon;
	bool Ok = false;
  int i;


  Icon = new TIcon();
  for (i = 0; i < (int) ListElems.size(); i++) {
    Icon->AddBitmap(ListElems[i].Bitmap);
  }
  Ok = Icon->SaveToFile(asFileName);
  delete Icon;

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
bool TFrame_SaisieIcon::Undo(void) {
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
bool TFrame_SaisieIcon::Redo(void) {
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
AnsiString TFrame_SaisieIcon::GetModificationsList(void) {
	return FUndoRedo->GetModificationsList(0);
}

//---------------------------------------------------------------------------
bool TFrame_SaisieIcon::ProcessUndo(void * Undo, int LenUndo) {
	LPUNDOICONADDIMAGE lpUndoAddImage;
	LPUNDOICONDELIMAGE lpUndoDelImage;
	LPUNDOICONMODIFIMAGE lpUndoModifImage;
	TMemoryStream *BitmapStream;
	TBitmap *Bitmap;
  int BitCount;
  int NbColors;
	AnsiString asCaption;
	ELEMICO NewElemIco;
	int SizeIco;
	int Index;


	ACTIONUNDOICON ActionUndoIcon = *(ACTIONUNDOICON *) Undo;

  switch (ActionUndoIcon) {

		case auAddImage:
			lpUndoAddImage = (LPUNDOICONADDIMAGE) Undo;
			Bitmap = new TBitmap();
			BitmapStream = new TMemoryStream(lpUndoAddImage->Buffer, lpUndoAddImage->SizeBuffer);
			Bitmap->LoadFromStream(BitmapStream);
			if (Bitmap->Width < Bitmap->Height) SizeIco = Bitmap->Width;
			else SizeIco = Bitmap->Height;
			NewElemIco.Bitmap = Bitmap;
			NewElemIco.Size = SizeIco;
			Index = lpUndoAddImage->Index;
			asCaption.sprintf(_T("%i x %i"), SizeIco, SizeIco);
			ListElems.insert(ListElems.begin() + Index, NewElemIco);
			TabControl->Tabs->Insert(Index, asCaption);
			TabControl->TabIndex = Index;
			BitCount = Bitmap->BitCount;
			switch (BitCount) {
				case 1: NbColors = 2; break;
				case 4: NbColors = 16; break;
				case 8: NbColors = 256; break;
				case 16: NbColors = 65536; break;
				default: NbColors = 16777216; break;
			}
	    Label_Couleurs->Caption = AnsiString(NbColors);
      return true;

		case auDelImage:
			lpUndoDelImage = (LPUNDOICONDELIMAGE) Undo;
			delete ListElems[lpUndoDelImage->Index].Bitmap;
			ListElems.erase(ListElems.begin() + lpUndoDelImage->Index);
			TabControl->Tabs->Delete(lpUndoDelImage->Index);
      return true;

    case auModifImage:
			lpUndoModifImage = (LPUNDOICONMODIFIMAGE) Undo;
			Index = lpUndoModifImage->Index;
			BitmapStream = new TMemoryStream(lpUndoModifImage->Buffer, lpUndoModifImage->SizeBuffer);
			ListElems[Index].Bitmap->LoadFromStream(BitmapStream);
			delete BitmapStream;
			Bitmap = ListElems[Index].Bitmap;
			BitCount = Bitmap->BitCount;
			switch (BitCount) {
				case 1: NbColors = 2; break;
				case 4: NbColors = 16; break;
				case 8: NbColors = 256; break;
				case 16: NbColors = 65536; break;
				default: NbColors = 16777216; break;
			}
	    Label_Couleurs->Caption = AnsiString(NbColors);
      return true;

  }

  return false;
}

//---------------------------------------------------------------------------
bool TFrame_SaisieIcon::ProcessRedo(void * Redo, int LenRedo) {
  // Les commandes undo et redo sont les mêmes
	return ProcessUndo(Redo, LenRedo);
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieIcon::CutToClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieIcon::CopyToClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
bool FASTCALL TFrame_SaisieIcon::PasteFromClipboard(void) {

  return false;
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::TabControlChange(TObject *Sender) {
  TBitmap *Bitmap;
  int BitCount;
  int NbColors;
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
    Bitmap = ListElems[Index].Bitmap;
    BitCount = Bitmap->BitCount;
    switch (BitCount) {
      case 1: NbColors = 2; break;
      case 4: NbColors = 16; break;
      case 8: NbColors = 256; break;
      case 16: NbColors = 65536; break;
      default: NbColors = 16777216; break;
    }
    Label_Couleurs->Caption = AnsiString(NbColors);
  }

	Invalidate();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::Button_AjouterImageClick(TObject *Sender) {

  if (OpenDialogBmp->Execute()) {

		TBitmap *NewBitmap;
		AnsiString asCaption;
		ELEMICO NewElemIco;
		LPUNDOICONADDIMAGE lpUndoAddImage;
		LPUNDOICONDELIMAGE lpUndoDelImage;
		TMemoryStream BitmapStream;
		unsigned int SizeBuffer;
		int Index;

		FUndoRedo->BeginOperation(0, _TT("Ajout d'une image"));

		NewBitmap = new TBitmap();
    NewBitmap->LoadFromFile(OpenDialogBmp->FileName);
    Index = AjouterBitmap(NewBitmap);
		Changed = true;  // Et non FChanged pour mettre à jour l'onglet

		// Mémorisation du Undo
		lpUndoDelImage = (LPUNDOICONDELIMAGE) new BYTE[sizeof(UNDOICONDELIMAGE)];
		lpUndoDelImage->ActionUndoIcon = auDelImage;
		lpUndoDelImage->Index = Index;
		FUndoRedo->MemoRedo(this, lpUndoDelImage, sizeof(UNDOICONMODIFIMAGE));
		delete lpUndoDelImage;

		// Mémorisation du Redo
		ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
		SizeBuffer = (unsigned int) BitmapStream.Size;
		lpUndoAddImage = (LPUNDOICONADDIMAGE) new BYTE[sizeof(UNDOICONADDIMAGE) + SizeBuffer - 1];
		lpUndoAddImage->ActionUndoIcon = auAddImage;
		lpUndoAddImage->Index = Index;
		lpUndoAddImage->SizeBuffer = SizeBuffer;
		BitmapStream.Position = 0;
		BitmapStream.Read(lpUndoAddImage->Buffer, SizeBuffer);
		FUndoRedo->MemoUndo(this, lpUndoAddImage, sizeof(UNDOICONADDIMAGE) + SizeBuffer - 1);
		delete lpUndoAddImage;

	}

}
//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::Button_EnleverImageClick(TObject *Sender) {
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
		LPUNDOICONMODIFIMAGE lpUndoModifImage;
		TMemoryStream BitmapStream;
		unsigned int SizeBuffer;

		FUndoRedo->BeginOperation(0, _TT("Suppression d'une image"));

		// Mémorisation du Undo
		ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
		SizeBuffer = (unsigned int) BitmapStream.Size;
		lpUndoModifImage = (LPUNDOICONMODIFIMAGE) new BYTE[sizeof(UNDOICONMODIFIMAGE) + SizeBuffer - 1];
		lpUndoModifImage->ActionUndoIcon = auAddImage;
		lpUndoModifImage->Index = Index;
		lpUndoModifImage->SizeBuffer = SizeBuffer;
		BitmapStream.Position = 0;
		BitmapStream.Read(lpUndoModifImage->Buffer, SizeBuffer);
		FUndoRedo->MemoUndo(this, lpUndoModifImage, sizeof(UNDOICONMODIFIMAGE) + SizeBuffer - 1);
		delete lpUndoModifImage;

    delete ListElems[Index].Bitmap;
    ListElems.erase(ListElems.begin() + Index);
    TabControl->Tabs->Delete(Index);
		Changed = true;  // Et non FChanged pour mettre à jour l'onglet

		// Mémorisation du Redo
		lpUndoModifImage = (LPUNDOICONMODIFIMAGE) new BYTE[sizeof(UNDOICONMODIFIMAGE)];
		lpUndoModifImage->ActionUndoIcon = auDelImage;
		lpUndoModifImage->Index = Index;
		FUndoRedo->MemoRedo(this, lpUndoModifImage, sizeof(UNDOICONMODIFIMAGE));
		delete lpUndoModifImage;

		FUndoRedo->EndOperation();

  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::Button_EnregistrerImageSousClick(TObject *Sender) {
  int Index;


  Index = TabControl->TabIndex;
  if (Index != -1) {
    if (SaveDialogBmp->Execute()) {
      ListElems[Index].Bitmap->SaveToFile(SaveDialogBmp->FileName);
    }
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::Button_EditerImageClick(TObject *Sender) {
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
			LPUNDOICONMODIFIMAGE lpUndoModifImage;
			TMemoryStream BitmapStream;
			unsigned int SizeBuffer;

			FUndoRedo->BeginOperation(0, _TT("Modification d'une image"));

			// Mémorisation du Undo
			ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
			SizeBuffer = (unsigned int) BitmapStream.Size;
			lpUndoModifImage = (LPUNDOICONMODIFIMAGE) new BYTE[sizeof(UNDOICONMODIFIMAGE) + SizeBuffer - 1];
			lpUndoModifImage->ActionUndoIcon = auModifImage;
			lpUndoModifImage->Index = Index;
			lpUndoModifImage->SizeBuffer = SizeBuffer;
			BitmapStream.Position = 0;
			BitmapStream.Read(lpUndoModifImage->Buffer, SizeBuffer);
			FUndoRedo->MemoUndo(this, lpUndoModifImage, sizeof(UNDOICONMODIFIMAGE) + SizeBuffer - 1);
			delete lpUndoModifImage;

      ListElems[Index].Bitmap->LoadFromFile(asFileName);
			Changed = true;  // Et non FChanged pour mettre à jour l'onglet

			// Mémorisation du Redo
			BitmapStream.Clear();
			ListElems[Index].Bitmap->SaveToStream(&BitmapStream);
			SizeBuffer = (unsigned int) BitmapStream.Size;
			lpUndoModifImage = (LPUNDOICONMODIFIMAGE) new BYTE[sizeof(UNDOICONMODIFIMAGE) + SizeBuffer - 1];
			lpUndoModifImage->ActionUndoIcon = auModifImage;
			lpUndoModifImage->Index = Index;
			lpUndoModifImage->SizeBuffer = SizeBuffer;
			BitmapStream.Position = 0;
			BitmapStream.Read(lpUndoModifImage->Buffer, SizeBuffer);
			FUndoRedo->MemoRedo(this, lpUndoModifImage, sizeof(UNDOICONMODIFIMAGE) + SizeBuffer - 1);
			delete lpUndoModifImage;

			FUndoRedo->EndOperation();

		}

		DeleteFile(asFileName);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::Button_MiseAZeroClick(TObject *Sender) {
  int i;


  for (i = (int) ListElems.size() - 1; i >= 0; i--) {
    TabControl->Tabs->Delete(i);
    delete ListElems[i].Bitmap;
  }
  ListElems.clear();
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::PaintBoxPaint(TObject *Sender) {
  TBitmap *Bitmap;
  TRect Rect1;
  TRect Rect2;
  int SizeIco;
  int Index;


  Rect2 = PaintBox->ClientRect;
  Index = TabControl->TabIndex;
  if (Index != -1) {
    Bitmap = ListElems[Index].Bitmap;
    if (Bitmap->Width < Bitmap->Height) SizeIco = Bitmap->Width;
    else SizeIco = Bitmap->Height;
    Rect1.Left = 0;
    Rect1.Top = 0;
    Rect1.Right = SizeIco;
    Rect1.Bottom = SizeIco;
    if (Rect2.Width < Rect2.Height) {
      Rect2.Bottom = Rect2.Top + Rect2.Width;
    }
    else {
      Rect2.Right = Rect2.Left + Rect2.Height;
    }

    PaintBox->Canvas->CopyRect(Rect2, Bitmap->Canvas, Rect1);
  }
  else {
    PaintBox->Canvas->Pen->Style = psSolid;
    PaintBox->Canvas->Pen->Color = clBlack;
    PaintBox->Canvas->Rectangle(PaintBox->ClientRect);
  }
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::PaintBoxMouseMove(
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
void FASTCALL TFrame_SaisieIcon::PaintBoxMaskPaint(TObject *Sender) {
  TBitmap *Bitmap;
  TRect Rect1;
  TRect Rect2;
  int SizeIco;
  int Index;


  Rect2 = PaintBoxMask->ClientRect;
  Index = TabControl->TabIndex;
  if (Index != -1) {
    Bitmap = ListElems[Index].Bitmap;
    if (Bitmap->Width < Bitmap->Height) SizeIco = Bitmap->Width;
    else SizeIco = Bitmap->Height;
    if (Bitmap->Height >= 2 * SizeIco) {
      Rect1.Left = 0;
      Rect1.Top = SizeIco;
      Rect1.Right = SizeIco;
      Rect1.Bottom = 2 * SizeIco;
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
void FASTCALL TFrame_SaisieIcon::PaintBoxMaskMouseMove(
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
int TFrame_SaisieIcon::AjouterBitmap(TBitmap *Bitmap) {
  AnsiString asCaption;
  ELEMICO NewElemIco;
  int SizeIco;
  int i;
  int Index;


  if (Bitmap->Width < Bitmap->Height) SizeIco = Bitmap->Width;
  else SizeIco = Bitmap->Height;
  NewElemIco.asFileName = OpenDialogBmp->FileName;
  NewElemIco.Bitmap = Bitmap;
  NewElemIco.Size = SizeIco;
  Index = -1;
  for (i = 0; i < (int) ListElems.size(); i++) {
    if (ListElems[i].Size >= SizeIco) {
      Index = i;
      break;
    }
  }

  asCaption.sprintf(_T("%i x %i"), SizeIco, SizeIco);
  if (Index != -1) {
    ListElems.insert(ListElems.begin() + Index, NewElemIco);
    TabControl->Tabs->Insert(Index, asCaption);
    TabControl->TabIndex = Index;
  }
  else {
    ListElems.push_back(NewElemIco);
    TabControl->Tabs->Add(asCaption);
    TabControl->TabIndex = TabControl->Tabs->Count - 1;
    Index = (int) ListElems.size() - 1;
  }

	return Index;
}

//---------------------------------------------------------------------------
void FASTCALL TFrame_SaisieIcon::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_SAISIEDIALOG);
}

//---------------------------------------------------------------------------
FVCL_RegisterComponent(TFrame_SaisieIcon);

//---------------------------------------------------------------------------
