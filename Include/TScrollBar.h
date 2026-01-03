//-----------------------------------------------------------------------------
//! @file TScrollBar.h
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

#ifdef In_TScrollBarH
#error "Inclusion circulaire de TScrollBar"
#endif // In_TScrollBarH

#define In_TScrollBarH

#ifndef TScrollBarH
#define TScrollBarH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//-----------------------------------------------------------------------------
//! @class TScrollBar
//!
//! @brief Définition de la classe TScrollBar
//! @image html TScrollBar.jpg "TScrollBar"
//!
//! Cette classe représente un ascenseur horizontal ou vertical. En général
//! vous n'avez pas à créer un objet de ce type: des ascenseurs apparaissent
//! automatiquement lorsque c'est utile dans la plupart des contrôles (c'est
//! parfois optionnel).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TScrollBar: public TWinControl {
private:
  SCROLLINFO ScrollInfo;
  long FSmallChange;
  long FLargeChange;

protected:

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessHScroll(int Command, int Info);
  virtual bool ProcessVScroll(int Command, int Info);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TScrollBar(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TScrollBar(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Destruction du contrôle Windows associé au composant
  //!
  //! Cette méthode détruit la fenêtre Windows associée au composant, ceci
	//! pouvant être nécessaire au changement de certaines propriétés. Appeler
	//! ensuite CreateWnd pour recréer une fenêtre avec les mêmes propriétés.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
  //---------------------------------------------------------------------------
  void FASTCALL DestroyWnd(void);

  //---------------------------------------------------------------------------
  //! @brief Création du contrôle Windows associé au composant
  //!
  //! Cette méthode crée la fenêtre Windows associée au composant, ceci pouvant
	//! être nécessaire suite au changement de certaines propriétés. Appeler
	//! DestroyWnd, puis CreateWnd pour recréer la fenêtre. Les propriétés sont
	//! conservées entre la destruction et la création.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
  //---------------------------------------------------------------------------
	void FASTCALL CreateWnd(void);

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

  //---------------------------------------------------------------------------
  //! @brief Propriété Kind
	//!
	//! Cette propriété définit l'orientation horizontale ou verticale de la
	//! barre de défilement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScrollBar, TScrollBarKind, Kind);

  //---------------------------------------------------------------------------
  //! @brief Propriété Min
	//!
	//! Cette propriété définit la valeur minimum que peut prendre la barre de
	//! défilement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScrollBar, long, Min);

  //---------------------------------------------------------------------------
  //! @brief Propriété Max
	//!
	//! Cette propriété définit la valeur maximum que peut prendre la barre de
	//! défilement. Si cette valeur est inférieure à la valeur minimum, la barre
	//! est inversée (de bas en haut ou de droite à gauche)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScrollBar, long, Max);

  //---------------------------------------------------------------------------
  //! @brief Propriété PageSize
	//!
	//! Cette propriété définit la taille d'une page. Cette taille est symbolisée
	//! par une taille du bouton de défilement proportionelle. Ce n'est pas une
	//! taille en pixels, mais en unités de la barre de défilement (même unité
	//! que @b Min, @b Max, @b Position, @b SmallChange et @b LargeChange).
	//!
	//! @sa LargeChange
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScrollBar, long, PageSize);

  //---------------------------------------------------------------------------
  //! @brief Propriété SmallChange
	//!
	//! Cette propriété définit la valeur du défilement lorsqu'on se déplace
	//! ligne à ligne (avec les touches de déplacement Haut et bas, ou Droite et
	//! gauche).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScrollBar, long, SmallChange);

  //---------------------------------------------------------------------------
  //! @brief Propriété LargeChange
	//!
	//! Cette propriété définit la valeur du défilement lorsqu'on se déplace page
	//! par page (avec les touches Page-up ou Page-down). Cette propriété est
	//! par défaut égale à @b PageSize.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScrollBar, long, LargeChange);

  //---------------------------------------------------------------------------
  //! @brief Propriété Position
	//!
	//! Cette propriété définit la position supérieure de la barre de défilement.
	//! La portion visible va donc de @b Position à @b Position + @b PageSize, et
	//! la valeur maximum de @b Position est de @b Max - @b PageSize.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TScrollBar, long, Position);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque l'objet est modifié, soit par une
	//! action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnScroll
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnScroll, TObject *);

  //@}


};


#else  // TScrollBarH

class TScrollBar;

#endif  // TScrollBarH


#undef In_TScrollBarH
