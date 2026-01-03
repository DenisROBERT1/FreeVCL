//-----------------------------------------------------------------------------
//! @file TFont.h
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

#ifdef In_TFontH
#error "Inclusion circulaire de TFont"
#endif // In_TFontH

#define In_TFontH

#ifndef TFontH
#define TFontH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TCustomFont.h"
#include "TColor.h"


//-----------------------------------------------------------------------------
//! @class TFont
//!
//! @brief Définition de la classe TFont
//!
//! Cette classe encapsule une fonte système (définie par son handle). Elle est
//! une implémentation de la classe @b TCustomFont.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFont: public TCustomFont {
protected:
  HFONT FHandle;
  TColor FColor;
  LOGFONT LogFont;
  bool bChanged;
  int FPixelsPerInch;
  int FCharacterExtra;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFont(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFont(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Width
  //!
  //! Cette méthode permet de lire la propriété Width
  //!
  //! @return @c int Propriété Width
  //---------------------------------------------------------------------------

  virtual int Get_Width(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Width
  //!
  //! Cette méthode permet d'écrire la propriété Width
  //!
  //! @param[in]        NewWidth Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Width(int NewWidth);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Height
  //!
  //! Cette méthode permet de lire la propriété Height
  //!
  //! @return @c int Propriété Height
  //---------------------------------------------------------------------------

  virtual int Get_Height(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Height
  //!
  //! Cette méthode permet d'écrire la propriété Height
  //!
  //! @param[in]        NewHeight Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Height(int NewHeight);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Size
  //!
  //! Cette méthode permet de lire la propriété Size
  //!
  //! @return @c int Propriété Size
  //---------------------------------------------------------------------------

  virtual int Get_Size(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Size
  //!
  //! Cette méthode permet d'écrire la propriété Size
  //!
  //! @param[in]        NewSize Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Size(int NewSize);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Name
  //!
  //! Cette méthode permet de lire la propriété Name
  //!
  //! @return @c AnsiString Propriété Name
  //---------------------------------------------------------------------------

  virtual AnsiString Get_Name(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Name
  //!
  //! Cette méthode permet d'écrire la propriété Name
  //!
  //! @param[in]        NewName Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Name(AnsiString NewName);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Style
  //!
  //! Cette méthode permet de lire la propriété Style
  //!
  //! @return @c TFontStyles Propriété Style
  //---------------------------------------------------------------------------

  virtual TFontStyles Get_Style(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Style
  //!
  //! Cette méthode permet d'écrire la propriété Style
  //!
  //! @param[in]        NewStyle Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Style(TFontStyles NewStyle);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Charset
  //!
  //! Cette méthode permet de lire la propriété Charset
  //!
  //! @return @c TFontCharset Propriété Charset
  //---------------------------------------------------------------------------

  virtual TFontCharset Get_Charset(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Charset
  //!
  //! Cette méthode permet d'écrire la propriété Charset
  //!
  //! @param[in]        NewCharset Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Charset(TFontCharset NewCharset);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Pitch
  //!
  //! Cette méthode permet de lire la propriété Pitch
  //!
  //! @return @c TFontPitch Propriété Pitch
  //---------------------------------------------------------------------------

  virtual TFontPitch Get_Pitch(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Pitch
  //!
  //! Cette méthode permet d'écrire la propriété Pitch
  //!
  //! @param[in]        NewPitch Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Pitch(TFontPitch NewPitch);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Color
  //!
  //! Cette méthode permet de lire la propriété Color
  //!
  //! @return @c TColor Propriété Color
  //---------------------------------------------------------------------------

  virtual TColor Get_Color(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Color
  //!
  //! Cette méthode permet d'écrire la propriété Color
  //!
  //! @param[in]        NewColor Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Color(TColor NewColor);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété PixelsPerInch
  //!
  //! Cette méthode permet de lire la propriété PixelsPerInch
  //!
  //! @return @c int Propriété PixelsPerInch
  //---------------------------------------------------------------------------

  virtual int Get_PixelsPerInch(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété PixelsPerInch
  //!
  //! Cette méthode permet d'écrire la propriété PixelsPerInch
  //!
  //! @param[in]        NewPixelsPerInch Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_PixelsPerInch(int NewPixelsPerInch);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété CharacterExtra
  //!
  //! Cette méthode permet de lire la propriété CharacterExtra
  //!
  //! @return @c int Propriété CharacterExtra
  //---------------------------------------------------------------------------

  virtual int Get_CharacterExtra(void);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété CharacterExtra
  //!
  //! Cette méthode permet d'écrire la propriété CharacterExtra
  //!
  //! @param[in]        NewCharacterExtra Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_CharacterExtra(int NewCharacterExtra);


  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système
	//!
	//! Cette propriété permet de définir un handle système de fonte. L'objet
	//! TFont fait une copie de l'objet passé en paramètres, mais n'en devient
	//! pas propriétaire. C'est à l'utilisateur de détruire l'objet après avoir
	//! fait l'affectation (à moins qu'il ne s'agisse d'une fonte système).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFont, HFONT, Handle);

  //@}

};


#else  // TFontH

class TFont;

#endif  // TFontH


#undef In_TFontH
