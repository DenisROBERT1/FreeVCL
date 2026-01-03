//-----------------------------------------------------------------------------
//! @file TForm_SaisieStructure.h
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

#ifndef TForm_SaisieStructureH
#define TForm_SaisieStructureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <vector>
//---------------------------------------------------------------------------
typedef struct {
  int Left;
  int Top;
  int Width;
  int Height;
  int TabOrder;
  bool AnchorLeft;
  bool AnchorTop;
  bool AnchorRight;
  bool AnchorBottom;
  bool OnClick;
  AnsiString asClass;
  AnsiString asName;
  AnsiString asCaption;
} CONTROL;

//---------------------------------------------------------------------------
class TForm_SaisieStructure : public TForm {
private:	// Déclarations de l'utilisateur
  std::vector<CONTROL> Controls;
  int FormWidth, FormHeight;
  AnsiString asFormCaption;
  bool AnalyzeMemo(void);
  bool AnalyzeLine(AnsiString asLigne, TStrings * asMots);
public:		// Déclarations de l'utilisateur
	FVCL_BEGIN_COMPONENTS
	TLabel *Label_Repertoire;
	TEdit *Edit_Repertoire;
	TButton *Button_Parcourir;
	TLabel *Label_Unite;
	TLabel *Label_T;
	TEdit *Edit_Unite;
	TLabel *Label_Commentaire;
	TLabel *Label_Caption;
	TEdit *Edit_Caption;
	TLabel *Label_Struct;
	TMemo *Memo_Struct;
	TButton *Button_Apercu;
	TButton *Button_Creer;
	TButton *Button_Annuler;
	FVCL_END_COMPONENTS

	FVCL_BEGIN_EVENTS
  void FASTCALL Button_ApercuClick(TObject *Sender);
  void FASTCALL Button_CreerClick(TObject *Sender);
  void FASTCALL Button_ParcourirClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_SaisieStructure, TNotifyEvent, OnClick, Button_ApercuClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieStructure, TNotifyEvent, OnClick, Button_CreerClick, TObject*);
    SET_EVENT_STR_1(TForm_SaisieStructure, TNotifyEvent, OnClick, Button_ParcourirClick, TObject*);
  }

	FVCL_END_EVENTS

	TForm_SaisieStructure(TComponent* Owner, const TCHAR *szName);
};
//---------------------------------------------------------------------------
#endif
