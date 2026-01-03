//-----------------------------------------------------------------------------
//! @file TControlScrollBar.h
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

#ifdef In_TControlScrollBarH
#error "Inclusion circulaire de TControlScrollBar"
#endif // In_TControlScrollBarH

#define In_TControlScrollBarH

#ifndef TControlScrollBarH
#define TControlScrollBarH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnScroll.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//! @param[in]        ScrollCode Type du scroll effectué par l'utilisateur.
//! @param[in]        ScrollPos Position du scroll.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnScroll)(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);


//-----------------------------------------------------------------------------
//! @class TControlScrollBar
//!
//! @brief Définition de la classe TControlScrollBar
//!
//! Cette classe représente une barre de défilement associée à un contrôle, en
//! général un objet @b TForm ou @b TScrollBox.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TControlScrollBar: public TPersistent {
private:
  SCROLLINFO ScrollInfo;
  long FSmallChange;
  long FLargeChange;
  HWND FHandleParent;
  TScrollBarKind FKind;

protected:

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TControlScrollBar(TWinControl* AControl, TScrollBarKind AKind);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TControlScrollBar(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages de scroll
  //!
  //! Traitement des messages windows de scroll. Cette méthode est publique
  //! pour pouvoir être appelée par la fenêtre qui contient les scroll-bars.
  //---------------------------------------------------------------------------

  virtual bool ProcessScroll(int Command, int Info);

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

  DECLARE_PROPERTY_GET(TControlScrollBar, TScrollBarKind, Kind);

  //---------------------------------------------------------------------------
  //! @brief Propriété Min
	//!
	//! Cette propriété définit la valeur minimum que peut prendre la barre de
	//! défilement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlScrollBar, long, Min);

  //---------------------------------------------------------------------------
  //! @brief Propriété Max
	//!
	//! Cette propriété définit la valeur maximum que peut prendre la barre de
	//! défilement. Si cette valeur est inférieure à la valeur minimum, la barre
	//! est inversée (de bas en haut ou de droite à gauche)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlScrollBar, long, Max);

  //---------------------------------------------------------------------------
  //! @brief Propriété Increment
	//!
	//! Cette propriété définit la valeur du défilement lorsqu'on se déplace
	//! ligne à ligne (avec les touches de déplacement Haut et bas, ou Droite et
	//! gauche). Cette propriété est strictement égale à @b SmallChange. Utilisez
	//! de préférence cette dernière.
	//!
	//! @sa SmallChange
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlScrollBar, long, Increment);

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

  DECLARE_PROPERTY(TControlScrollBar, long, PageSize);

  //---------------------------------------------------------------------------
  //! @brief Propriété SmallChange
	//!
	//! Cette propriété définit la valeur du défilement lorsqu'on se déplace
	//! ligne à ligne (avec les touches de déplacement Haut et bas, ou Droite et
	//! gauche). Cette propriété est strictement égale à @b Increment.
	//!
	//! @sa Increment
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlScrollBar, long, SmallChange);

  //---------------------------------------------------------------------------
  //! @brief Propriété LargeChange
	//!
	//! Cette propriété définit la valeur du défilement lorsqu'on se déplace page
	//! par page (avec les touches Page-up ou Page-down). Cette propriété est
	//! par défaut égale à @b PageSize.
	//!
	//! @sa PageSize
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlScrollBar, long, LargeChange);

  //---------------------------------------------------------------------------
  //! @brief Propriété Position
	//!
	//! Cette propriété définit la position supérieure de la barre de défilement.
	//! La portion visible va donc de @b Position à @b Position + @b PageSize, et
	//! la valeur maximum de @b Position est de @b Max - @b PageSize.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlScrollBar, long, Position);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque la position de la scroll-bar est
	//! modifiée, soit par une action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnScroll
	//!
	//! Cet évènement est déclenché avant que la position de la scroll-bar soit
	//! modifiée, soit par une action de l'utilisateur, soit par programme. Vous
	//! pouvez intercepter cet évènement pour ajuster la position de l'ascenseur,
	//! ou pour interdire certaines valeurs.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Action effectuée
  //! @param[in, out]   Arg3 Position de la scroll-bar.
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnScroll, OnScroll, TObject *, TScrollCode, int &);

  //@}


};


#else  // TControlScrollBarH

class TControlScrollBar;

#endif  // TControlScrollBarH


#undef In_TControlScrollBarH
