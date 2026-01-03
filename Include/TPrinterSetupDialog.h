//-----------------------------------------------------------------------------
//! @file TPrinterSetupDialog.h
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

//-----------------------------------------------------------------------------
// Dispositif anti-redéfinition
//-----------------------------------------------------------------------------

#ifdef In_TPrinterSetupDialogH
#error "Inclusion circulaire de TPrinterSetupDialog"
#endif // In_TPrinterSetupDialogH

#define In_TPrinterSetupDialogH

#ifndef TPrinterSetupDialogH
#define TPrinterSetupDialogH


//-----------------------------------------------------------------------------
//! @class TPrinterSetupDialog
//!
//! @brief Définition de la classe TPrinterSetupDialog
//!
//! Cette classe représente une boîte de dialogue permettant à l'utilisateur de
//! saisir les paramètres de l'imprimante.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPrinterSetupDialog: public TComponent {
private:

protected:
  PAGESETUPDLG PrintDlgStruct;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPrinterSetupDialog(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPrinterSetupDialog(void);

  //@}


  //! @name Méthodes
  //@{

  virtual bool Execute(void);

  //@}


  //! @name Propriétés
  //@{

  //@}
};


//---------------------------------------------------------------------------
#endif


#undef In_TPrinterSetupDialogH
