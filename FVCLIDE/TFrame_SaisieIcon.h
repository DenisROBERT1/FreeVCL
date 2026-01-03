//-----------------------------------------------------------------------------
//! @file TFrame_SaisieIcon.h
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

#ifndef TFrame_SaisieIconH
#define TFrame_SaisieIconH

//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------
#include <FreeVCL.h>
#include <FreeVCL_ToolsEDI.h>
#include <TUndoRedo.h>
#include "TFrame_FVCLIDEBase.h"


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum ACTIONUNDOICON {
	auAddImage,
	auDelImage,
	auModifImage
};

typedef struct {
  ACTIONUNDOICON ActionUndoIcon;
	int Index;
	unsigned int SizeBuffer;
	BYTE Buffer[1];
} UNDOICONADDIMAGE, *LPUNDOICONADDIMAGE;

typedef struct {
  ACTIONUNDOICON ActionUndoIcon;
	int Index;
} UNDOICONDELIMAGE, *LPUNDOICONDELIMAGE;

typedef struct {
  ACTIONUNDOICON ActionUndoIcon;
	int Index;
	unsigned int SizeBuffer;
	BYTE Buffer[1];
} UNDOICONMODIFIMAGE, *LPUNDOICONMODIFIMAGE;

typedef struct {
  AnsiString asFileName;
  TBitmap *Bitmap;
  int Size;
} ELEMICO, *LPELEMICO;

//---------------------------------------------------------------------------
class TFrame_SaisieIcon: public TFrame_FVCLIDEBase, public TUndoRedoUser {
private:
protected:
  TUndoRedo *FUndoRedo;

	std::vector<ELEMICO> ListElems;

  int AjouterBitmap(TBitmap *Bitmap);

  virtual bool ProcessUndo(void * Undo, int LenUndo);
  virtual bool ProcessRedo(void * Redo, int LenRedo);

public:
  FVCL_BEGIN_COMPONENTS
	TButton *Button_AjouterImage;
	TButton *Button_EnregistrerImageSous;
	TButton *Button_EditerImage;
	TButton *Button_EnleverImage;
	TButton *Button_MiseAZero;
	TButton *Button_Aide;
  TPaintBox *PaintBox;
  TLabel *Label_Masque;
  TPaintBox *PaintBoxMask;
  TLabel *Label_TexteCouleurs;
  TLabel *Label_Couleurs;
  TTabControl *TabControl;
  TPanel *Panel;
  TOpenDialog *OpenDialogBmp;
  TSaveDialog *SaveDialogBmp;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_AjouterImageClick(TObject *Sender);
  void FASTCALL Button_EnregistrerImageSousClick(TObject *Sender);
  void FASTCALL Button_EditerImageClick(TObject *Sender);
  void FASTCALL Button_EnleverImageClick(TObject *Sender);
  void FASTCALL Button_MiseAZeroClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
  void FASTCALL TabControlChange(TObject *Sender);
  void FASTCALL PaintBoxPaint(TObject *Sender);
  void FASTCALL PaintBoxMouseMove(
        TObject *Sender, TShiftState Shift, int X, int Y);
  void FASTCALL PaintBoxMaskPaint(TObject *Sender);
  void FASTCALL PaintBoxMaskMouseMove(
        TObject *Sender, TShiftState Shift, int X, int Y);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnClick, Button_AjouterImageClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnClick, Button_EnregistrerImageSousClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnClick, Button_EditerImageClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnClick, Button_EnleverImageClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnClick, Button_MiseAZeroClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnClick, Button_AideClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnChange, TabControlChange, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnPaint, PaintBoxPaint, TObject*);
		SET_EVENT_STR_4(TFrame_SaisieIcon, TOnMouseMove, OnMouseMove, PaintBoxMouseMove, TObject *, TShiftState, int, int);
		SET_EVENT_STR_1(TFrame_SaisieIcon, TNotifyEvent, OnPaint, PaintBoxMaskPaint, TObject*);
		SET_EVENT_STR_4(TFrame_SaisieIcon, TOnMouseMove, OnMouseMove, PaintBoxMaskMouseMove, TObject *, TShiftState, int, int);
  }

  FVCL_END_EVENTS

  //! @name Constructeur, destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFrame_SaisieIcon(TWinControl *AOwner);

	//---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFrame_SaisieIcon(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Création d'un fichier
  //!
  //! Cette méthode crée un nouveau fichier icône.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool CreateNewIcon(void);

  //---------------------------------------------------------------------------
  //! @brief Ouverture d'un fichier
  //!
  //! Cette méthode ouvre un fichier.
  //!
  //! @param[in]        asFileName Nom du fichier à ouvrir.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool OpenFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un fichier
  //!
  //! Cette méthode enregistre les données dans le fichier asFileName. Si le
  //! fichier existe déjà, il est écrasé sans confirmation de l'utilisateur.
  //!
  //! @param[in]        asFileName Nom du fichier à enregistrer.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool SaveToFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération undo
  //!
  //! Cette fonction effectue une opération undo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Undo(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette fonction effectue une opération redo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Redo(void);

  //---------------------------------------------------------------------------
  //! @brief Liste des trois premières modifications
  //!
  //! Cette méthode renvoie le libellé des premières modifications
  //!
  //! @return @c AnsiString Liste des modifications (séparées par un retour à
	//! la ligne)
  //---------------------------------------------------------------------------
  virtual AnsiString FASTCALL GetModificationsList(void);

  //---------------------------------------------------------------------------
  //! @brief Couper et copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CutToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CopyToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Coller depuis le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL PasteFromClipboard(void);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété UndoRedo
	//!
	//! Cette propriété permet d'obtenir l'objet permettant d'enregistrer les
	//! actions pouvant être défaites.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TFrame_SaisieIcon, TUndoRedo *, UndoRedo);

  //@}

  //! @name Evènements
  //@{

  //@}

};
//---------------------------------------------------------------------------
#endif
