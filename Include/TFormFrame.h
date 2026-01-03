//-----------------------------------------------------------------------------
//! @file TFormFrame.h
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

#ifdef In_TFormFrameH
#error "Inclusion circulaire de TFormFrame"
#endif // In_TFormFrameH

#define In_TFormFrameH

#ifndef TFormFrameH
#define TFormFrameH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// MACROS
//---------------------------------------------------------------------------

// Taille des contrôles dans les boîtes de dialogues
#define DLGX(x) (((2 *(x) + 1) *tm.tmAveCharWidth) / 8)
#define DLGY(y) (((2 *(y) + 1) *tm.tmAscent) / 16)

#ifndef DOXYGEN

#define FVCL_BEGIN_COMPONENTS \
union {\
  TComponent *ListComponents[1];\
  struct {

#define FVCL_END_COMPONENTS \
}; };\
virtual TComponent **GetListComponents(void) {return ListComponents;}

#define FVCL_ARG_SUPP ListComponents

#else // DOXYGEN

#define FVCL_BEGIN_COMPONENTS
#define FVCL_END_COMPONENTS
#define FVCL_ARG_SUPP

#endif // DOXYGEN


//-----------------------------------------------------------------------------
//! @class TFormFrame
//!
//! @brief Définition de la classe TFormFrame
//!
//! Cette classe est la classe de base des boîtes de dialogue et des
//! parties communes de boîte de dialogue. Elle permet la création des
//! contrôles à partir des ressources de type DFM ou RC.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFormFrame: public TWinControl {
private:
protected:

  //---------------------------------------------------------------------------
  //! @brief Construction du dialogue
  //---------------------------------------------------------------------------

  bool LoadDialog(const TCHAR *szName, TComponent *ListComponents[]);

  //---------------------------------------------------------------------------
  //! @brief Chargement d'un dialogue RC
  //---------------------------------------------------------------------------

  bool LoadDialogRC(HGLOBAL hgRes, int SizeRes);

  //---------------------------------------------------------------------------
  //! @brief Chargement d'un dialogue VCL
  //---------------------------------------------------------------------------

  bool LoadDialogVCL(HGLOBAL hgRes, int SizeRes, TComponent *ListComponents[]);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFormFrame(TComponent *AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFormFrame(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Liste des composants
  //---------------------------------------------------------------------------
  virtual TComponent **GetListComponents(void) {return NULL;};

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}

  //! @name Propriétés
  //@{

  //@}

  //! @name Evènements
  //@{

  //@}


};


#else  // TFormFrameH

class TFormFrame;

#endif  // TFormFrameH


#undef In_TFormFrameH
