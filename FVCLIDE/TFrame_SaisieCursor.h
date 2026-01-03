//-----------------------------------------------------------------------------
//! @file TFrame_SaisieCursor.h
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

#ifndef TFrame_SaisieCursorH
#define TFrame_SaisieCursorH

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

enum ACTIONUNDOCUR {
	auAddCursor,
	auDelCursor,
	auModifCursor,
	auModifHotSpot
};

typedef struct {
  ACTIONUNDOCUR ActionUndoCursor;
	int Index;
	unsigned int SizeBuffer;
	BYTE Buffer[1];
} UNDOCURADDIMAGE, *LPUNDOCURADDIMAGE;

typedef struct {
  ACTIONUNDOCUR ActionUndoCursor;
	int Index;
} UNDOCURDELIMAGE, *LPUNDOCURDELIMAGE;

typedef struct {
  ACTIONUNDOCUR ActionUndoCursor;
	int Index;
	unsigned int SizeBuffer;
	BYTE Buffer[1];
} UNDOCURMODIFIMAGE, *LPUNDOCURMODIFIMAGE;

typedef struct {
  ACTIONUNDOCUR ActionUndoCursor;
	int Index;
	int X;
	int Y;
} UNDOCURMODIFHOTSPOT, *LPUNDOCURMODIFHOTSPOT;

typedef struct {
  AnsiString asFileName;
  TBitmap *Bitmap;
  int Size;
} ELEMCUR, *LPELEMCUR;


//---------------------------------------------------------------------------
class TFrame_SaisieCursor: public TFrame_FVCLIDEBase, public TUndoRedoUser {
private:
	bool bNotChangeHotSpot;
protected:
  TUndoRedo *FUndoRedo;

	std::vector<ELEMCUR> ListElems;

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
	TLabel *Label_XHotSpot;
	TEdit *Edit_XHotSpot;
	TUpDown *UpDown_XHotSpot;
	TLabel *Label_YHotSpot;
	TEdit *Edit_YHotSpot;
	TUpDown *UpDown_YHotSpot;
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
  void FASTCALL Edit_XHotSpotChange(TObject *Sender);
  void FASTCALL Edit_YHotSpotChange(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnClick, Button_AjouterImageClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnClick, Button_EnregistrerImageSousClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnClick, Button_EditerImageClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnClick, Button_EnleverImageClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnClick, Button_MiseAZeroClick, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnClick, Button_AideClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnChange, TabControlChange, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnPaint, PaintBoxPaint, TObject*);
		SET_EVENT_STR_4(TFrame_SaisieCursor, TOnMouseMove, OnMouseMove, PaintBoxMouseMove, TObject *, TShiftState, int, int);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnPaint, PaintBoxMaskPaint, TObject*);
		SET_EVENT_STR_4(TFrame_SaisieCursor, TOnMouseMove, OnMouseMove, PaintBoxMaskMouseMove, TObject *, TShiftState, int, int);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnChange, Edit_XHotSpotChange, TObject*);
		SET_EVENT_STR_1(TFrame_SaisieCursor, TNotifyEvent, OnChange, Edit_YHotSpotChange, TObject*);
  }

  FVCL_END_EVENTS

  //! @name Constructeur, destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFrame_SaisieCursor(TWinControl *AOwner);

	//---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFrame_SaisieCursor(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Création d'un fichier
  //!
  //! Cette méthode crée un nouveau fichier Curseur.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool CreateNewCursor(void);

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

  DECLARE_PROPERTY_GET(TFrame_SaisieCursor, TUndoRedo *, UndoRedo);

  //@}

  //! @name Evènements
  //@{

  //@}

};
//---------------------------------------------------------------------------
#endif
