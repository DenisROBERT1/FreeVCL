//-----------------------------------------------------------------------------
//! @file TClipboard.h
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

#ifdef In_TClipboardH
#error "Inclusion circulaire de TClipboard"
#endif // In_TClipboardH

#define In_TClipboardH

#ifndef TClipboardH
#define TClipboardH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//-----------------------------------------------------------------------------
//! @class TClipboard
//!
//! @brief Définition de la classe TClipboard
//!
//! Cette classe encapsule le presse-papiers dans lequel l'utilisateur peut
//! temporairement mémoriser des informations sous différents formats. L'objet
//! @b Clipboard est global et n'a pas besoin d'être instancié.
//!
//! @author Denis ROBERT
//!
/*! @verbatim
	Exemple d'utilisation :

	Clipboard->Open();
	Clipboard->Clear();
	Clipboard->SetTextBuf("Exemple");
	Clipboard->Close(); @endverbatim */
//-----------------------------------------------------------------------------

class TClipboard: public TPersistent {
private:
  int NbOpen;
protected:
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TClipboard(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ouverture du presse-papiers
  //---------------------------------------------------------------------------

  bool FASTCALL Open(void);

  //---------------------------------------------------------------------------
  //! @brief Fermeture du presse-papiers
  //---------------------------------------------------------------------------

  bool FASTCALL Close(void);

  //---------------------------------------------------------------------------
  //! @brief Mise à zéro du presse-papiers
  //---------------------------------------------------------------------------

  void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Handle du presse-papiers
  //---------------------------------------------------------------------------

  HANDLE FASTCALL GetAsHandle(UINT Format);

  //---------------------------------------------------------------------------
  //! @brief Copie dans le presse-papiers
  //---------------------------------------------------------------------------

  void FASTCALL SetAsHandle(UINT Format, HANDLE hValue);

  //---------------------------------------------------------------------------
  //! @brief Copie dans le presse-papiers en format texte
  //---------------------------------------------------------------------------

  void FASTCALL SetTextBuf(const TCHAR * Buffer);


  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété AsText
	//!
	//! Cette propriété définit le contenu (format texte) du presse-papiers.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TClipboard, AnsiString, AsText);


  //@}

};


#else  // TClipboardH

class TClipboard;

#endif  // TClipboardH

#undef In_TClipboardH
