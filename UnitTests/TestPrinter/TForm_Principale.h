//-----------------------------------------------------------------------------
//! @file TForm_Principale.h
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

#ifndef TFormPrincipaleH
#define TFormPrincipaleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormPrincipale : public TForm
{
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
	FVCL_BEGIN_COMPONENTS
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TPrintDialog *PrintDialog1;
	TPrinterSetupDialog *PrinterSetupDialog1;
	FVCL_END_COMPONENTS

	FVCL_BEGIN_EVENTS
  void FASTCALL Button1Click(TObject *Sender);
  void FASTCALL Button3Click(TObject *Sender);
  void FASTCALL Button2Click(TObject *Sender);
  void FASTCALL Button4Click(TObject *Sender);
	FVCL_END_EVENTS

	TFormPrincipale(TComponent* Owner, LPTSTR szName);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPrincipale *FormPrincipale;
//---------------------------------------------------------------------------
#endif
