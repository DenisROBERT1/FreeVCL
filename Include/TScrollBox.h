//-----------------------------------------------------------------------------
//! @file TScrollBox.h
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

#ifdef In_TScrollBoxH
#error "Inclusion circulaire de TScrollBox"
#endif // In_TScrollBoxH

#define In_TScrollBoxH

#ifndef TScrollBoxH
#define TScrollBoxH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TScrollBox
//!
//! @brief Définition de la classe TScrollBox
//!
//! Cette classe représente une fenêtre comportant des barres de défilement.
//! Utilisez ce contrôle pour pouvoir accéder à un grand nombre de composants
//! lorsque la taille de l'écran ou de la fenêtre principale ne le permet pas
//! directement.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TScrollBox: public TWinControl {
private:

  bool bAutoRange;
  bool ScrollControls();

protected:

  int XMaxScroll;
	int YMaxScroll;
  int XContainer;
  int YContainer;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessShowWindow(bool bShow);
  virtual bool ProcessSize(int SizeType, int L, int H);
  virtual bool ProcessHScroll(int Command, int Info);
  virtual bool ProcessVScroll(int Command, int Info);
	virtual bool ProcessMouseWheel(WORD fwKeys, short zDelta, int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Calcul des paramètres des scroll-bars
  //---------------------------------------------------------------------------

  void CalculRangeScrollBars(void);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TScrollBox(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TScrollBox(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un composant enfant
  //---------------------------------------------------------------------------

  virtual void InsertComponent(TComponent* AComponent, int Index = -1);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un composant enfant de la liste des composants
  //---------------------------------------------------------------------------

  virtual void RemoveComponent(TComponent* AComponent);


  //---------------------------------------------------------------------------
  //! @brief Désactive le défilement automatique.
  //---------------------------------------------------------------------------

  void FASTCALL DisableAutoRange(void);

  //---------------------------------------------------------------------------
  //! @brief Réactive le défilement automatique.
  //---------------------------------------------------------------------------

  void FASTCALL EnableAutoRange(void);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété LeftWnd
  //!
  //! Cette méthode permet de lire la propriété LeftWnd
  //!
  //! @return @c int Propriété LeftWnd
  //---------------------------------------------------------------------------

  virtual int Get_LeftWnd(void);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété TopWnd
  //!
  //! Cette méthode permet de lire la propriété TopWnd
  //!
  //! @return @c int Propriété TopWnd
  //---------------------------------------------------------------------------

  virtual int Get_TopWnd(void);

  //---------------------------------------------------------------------------
  //! @brief Amène un contrôle dans la partie visible
  //---------------------------------------------------------------------------

  void ScrollInView(TControl* AControl);


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


#else  // TScrollBoxH

class TScrollBox;

#endif  // TScrollBoxH


#undef In_TScrollBoxH
