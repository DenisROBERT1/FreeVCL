//-----------------------------------------------------------------------------
//! @file TForm_FileFormat.h
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

#ifndef TForm_FileFormatH
#define TForm_FileFormatH

//---------------------------------------------------------------------------
class TForm_FileFormat: public TForm {
private:

protected:

public:

  FVCL_BEGIN_COMPONENTS
	// FVCL automatic code ! Don't modify this section.
	TCheckBox *CheckBox_ByteOrderMark;;
	TCheckBox *CheckBox_BigEndian;;
	TRadioButton *RadioButton_UTF8;
	TRadioButton *RadioButton_ASCII;
	TRadioButton *RadioButton_UNICODE;
	TGroupBox *GroupBox_FormatFichier;
	TRadioButton *RadioButton_CRLF;
	TRadioButton *RadioButton_LF;
	TGroupBox *GroupBox_FinLignes;
	TButton *Button_Ok;
	TButton *Button_Annuler;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
	// FVCL automatic code ! Don't modify this section.
  FVCL_END_EVENTS

  TForm_FileFormat(TComponent* Owner, LPCTSTR szName);

};

//---------------------------------------------------------------------------
#endif  // TForm_FileFormatH
