//-----------------------------------------------------------------------------
//! @file TPageControl.h
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

#ifdef In_TPageControlH
#error "Inclusion circulaire de TPageControl"
#endif // In_TPageControlH

#define In_TPageControlH

#ifndef TPageControlH
#define TPageControlH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TTabStyle {
	tsTabs,                       //!< Boutons ayant la forme d'onglets
	tsButtons,                    //!< Boutons standard
	tsFlatButtons                 //!< Boutons plats
};


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TTabSheet;


//-----------------------------------------------------------------------------
//! @class TPageControl
//!
//! @brief Définition de la classe TPageControl
//! @image html TPageControl.jpg "TPageControl"
//!
//! Cette classe représente une série d'onglets. Chaque onglet (et la page
//! associée) est défini par un élément de la propriété @b Pages (objets
//! @b TabSheet). Utilisez la méthode @b InsertTab et @b RemoveTab pour ajouter
//! ou supprimer des onglets par programme. Les onglets peuvent être disposés
//! sur une seule ligne ou sur plusieurs lignes (propriété @b MultiLine).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPageControl: public TWinControl {
  friend class TTabSheet;  // Pour l'accès à InsertTab et DeleteTab
  std::vector<TTabSheet *>FPages;
private:
protected:
  int FActivePageIndex;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual int ProcessNotify(LPNMHDR pnmh);
  virtual bool ProcessSize(int SizeType, int L, int H);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un onglet
  //---------------------------------------------------------------------------

  void FASTCALL InsertTab(int Index, TTabSheet *TabSheet);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un onglet de la liste (l'objet n'est pas détruit)
  //---------------------------------------------------------------------------

  void FASTCALL RemoveTab(int Index);

  //---------------------------------------------------------------------------
  //! @brief Mise à jour des onglets
  //---------------------------------------------------------------------------

  virtual void FASTCALL UpdateActivePage(void);

  //---------------------------------------------------------------------------
  //! @brief Mise à jour de la taille des pages
  //---------------------------------------------------------------------------

  virtual void FASTCALL UpdateSizeTabSheets(void);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPageControl(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPageControl(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Visible
  //!
  //! Cette méthode permet de lire la propriété Visible
  //!
  //! @return @c bool Propriété Visible
  //---------------------------------------------------------------------------

  virtual bool Set_Visible(bool NewVisible);

  //---------------------------------------------------------------------------
  //! @brief Ajustement du rectangle client
  //!
  //! @param[in, out]   Rect Rectangle de la zone client
  //---------------------------------------------------------------------------
  virtual void FASTCALL AdjustClientRect(TRect &Rect);

  //---------------------------------------------------------------------------
  //! @brief Renvoie la position du contrôle enfant à alignement personnalisé
  //! (Align = alCustom).
  //---------------------------------------------------------------------------

  virtual void FASTCALL CustomAlignPosition(TControl* Control,
      int &NewLeft, int &NewTop, int &NewWidth, int &NewHeight,
      TRect &AlignRect);

  //---------------------------------------------------------------------------
  //! @brief Indique l'index de l'onglet à un point spécifié.
  //---------------------------------------------------------------------------

  int FASTCALL IndexOfTabAt(int X, int Y);

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

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction de la fiche
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété ActivePage: onglet actif
	//!
	//! Cette propriété définit l'onglet actif dans le @b PageControl.
	//!
	//! @sa ActivePageIndex
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageControl, TTabSheet*, ActivePage);

  //---------------------------------------------------------------------------
  //! @brief Propriété ActivePageIndex: index de l'onglet actif
	//!
	//! Cette propriété définit l'index de l'onglet actif dans le @b PageControl.
	//!
	//! @sa ActivePage
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageControl, int, ActivePageIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété MultiLine: affichage sur plusieurs lignes
	//! @image html TPageControl_simple.jpg "MultiLine = False" @image html TPageControl_MultiLine.jpg "MultiLine = True"
	//!
	//! Cette propriété permet d'afficher les onglets sur plusieurs lignes. Sinon
	//! les onglets sont affichés sur une seule ligne avec un bouton de
	//! défilement (valeur par défaut).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageControl, bool, MultiLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété PageCount
	//!
	//! Cette propriété permet de connaitre le nombre de pages (d'onglets) que
	//! contient le @b PageControl.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TPageControl, int, PageCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété Pages
	//!
	//! Cette propriété permet d'accéder à chaque page pour lire ou définir leur
	//! propriétés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TPageControl, TTabSheet *, Pages);

  //---------------------------------------------------------------------------
  //! @brief Propriété Style
	//!
	//! Cette propriété permet de définir l'aspect des onglets. Les onglets
	//! peuvent être affichés soit de façon classique, soit sous forme de
	//! boutons.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPageControl, TTabStyle, Style);

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

  //@}


};


#else  // TPageControlH

class TPageControl;

#endif  // TPageControlH


#undef In_TPageControlH
