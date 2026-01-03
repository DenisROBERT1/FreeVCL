//-----------------------------------------------------------------------------
//! @file Unit1.h
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

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TUnit1 : public TForm
{
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
	FVCL_BEGIN_COMPONENTS
	TTreeView *TreeView1;
	TEdit *Edit1;
	TButton *Button_AddRoot;
	TButton *Button_AddChild;
	TButton *Button_Add;
	TButton *Button_Delete;
	FVCL_END_COMPONENTS

	FVCL_BEGIN_EVENTS
        void FASTCALL Button_AddRootClick(TObject *Sender);
  void FASTCALL Button_AddChildClick(TObject *Sender);
  void FASTCALL Button_AddClick(TObject *Sender);
  void FASTCALL Button_DeleteClick(TObject *Sender);
	FVCL_END_EVENTS

	TUnit1(TComponent* Owner, LPTSTR szName);
};
//---------------------------------------------------------------------------
extern PACKAGE TUnit1 *Unit1;
//---------------------------------------------------------------------------
#endif
