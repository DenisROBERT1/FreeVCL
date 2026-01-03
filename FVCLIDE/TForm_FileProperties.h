//-----------------------------------------------------------------------------
//! @file TForm_FileProperties.h
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

#ifndef TForm_FilePropertiesH
#define TForm_FilePropertiesH
//---------------------------------------------------------------------------
#include <vector>

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TIndexTypeProject;


//---------------------------------------------------------------------------
class TForm_FileProperties : public TForm {
private:
public:
  FVCL_BEGIN_COMPONENTS
	TImage *Image_Icon;
  TLabel *Label_TypeProjet;
  TLabel *Label_TextNom;
  TLabel *Label_Nom;
  TLabel *Label_TextFile;
  TLabel *Label_File;
  TLabel *Label_TextLastModif;
  TLabel *Label_LastModif;
  TLabel *Label_TextBuilt;
  TLabel *Label_Built;
	TImage *Image_State;
  TLabel *Label_State;
  TButton *Button_Ok;
  TButton *Button_Aide;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Button_AideClick(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
    SET_EVENT_STR_1(TForm_FileProperties, TNotifyEvent, OnClick, Button_AideClick, TObject*);
  }

  FVCL_END_EVENTS

  TForm_FileProperties(TComponent *Owner, LPCTSTR szName);
  ~TForm_FileProperties(void);

};
//---------------------------------------------------------------------------
#endif
