//-----------------------------------------------------------------------------
//! @file TMonthCalColors.h
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

#ifdef In_TMonthCalColorsH
#error "Inclusion circulaire de TMonthCalColors"
#endif // In_TMonthCalColorsH

#define In_TMonthCalColorsH

#ifndef TMonthCalColorsH
#define TMonthCalColorsH

//-----------------------------------------------------------------------------
//! @class TMonthCalColors
//!
//! @brief Définition de la classe TMonthCalColors
//!
//! Cette classe permet de paramétrer la couleur d'un objet TMonthCalendar ou
//! TDateTimePicker.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMonthCalColors: public TPersistent {
private:

protected:
  TColor FBackColor;
  TColor FMonthBackColor;
  TColor FTextColor;
  TColor FTitleBackColor;
  TColor FTitleTextColor;
  TColor FTrailingTextColor;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TMonthCalColors(HWND HandleParent);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TMonthCalColors(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Transfert des propriétés
  //!
  //! Cette méthode permet de recopier un TMonthCalColors externe
  //!
  //! @param[in]        src Objet TMonthCalColors à copier
  //---------------------------------------------------------------------------

  void FASTCALL Assign(TPersistent* src);


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


  //---------------------------------------------------------------------------
  //! @brief Propriété BackColor: Couleur de fond entre les mois
  //! 
  //! Cette propriété détermine la couleur de fond du calendrier, hors des
	//! cadres des mois. Utilisez la propriété TMonthCalColors::MonthBackColor
	//! pour déterminer la couleur de l'intérieur des cadres.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMonthCalColors, TColor, BackColor);


  //---------------------------------------------------------------------------
  //! @brief Propriété MonthBackColor: Couleur de fond des mois
  //! 
  //! Cette propriété détermine la couleur de fond du cadre représentant un
	//! mois du calendrier. Utilisez la propriété TMonthCalColors::BackColor
	//! pour déterminer la couleur à l'extérieur des cadres.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMonthCalColors, TColor, MonthBackColor);


  //---------------------------------------------------------------------------
  //! @brief Propriété TextColor: Couleur du texte des dates
  //! 
  //! Cette propriété détermine la couleur des textes de date dans le mois,
	//! sauf la date sélectionnée qui prend la couleur système de sélection. Les
	//! jours avant le premier ou après la fin de mois prennent la couleur
	//! TMonthCalColors::TrailingTextColor.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMonthCalColors, TColor, TextColor);


  //---------------------------------------------------------------------------
  //! @brief Propriété TitleBackColor: couleur de fond des titres
  //! 
  //! Cette propriété détermine la couleur de fond du titre des cadres
	//! représentant un mois (titre qui contient le mois et l'année).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMonthCalColors, TColor, TitleBackColor);


  //---------------------------------------------------------------------------
  //! @brief Propriété TitleTextColor: Couleur du texte des titres
  //! 
  //! Cette propriété détermine la couleur des textes du titre (titre qui
	//! contient le mois et l'année).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMonthCalColors, TColor, TitleTextColor);


  //---------------------------------------------------------------------------
  //! @brief Propriété TrailingTextColor: Couleur des dates hors mois courant.
  //! 
  //! Cette propriété détermine la couleur des textes de date du mois précédent
	//! (avant le premier) et du mois suivant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMonthCalColors, TColor, TrailingTextColor);

  //@}
};


//---------------------------------------------------------------------------
#endif

#undef In_TMonthCalColorsH
