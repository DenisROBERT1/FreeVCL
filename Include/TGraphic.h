//-----------------------------------------------------------------------------
//! @file TGraphic.h
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

#ifdef In_TGraphicH
#error "Inclusion circulaire de TGraphic"
#endif // In_TGraphicH

#define In_TGraphicH

#ifndef TGraphicH
#define TGraphicH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TPixelFormat {
	pfDevice,									//!< Dépendant de l'affichage
	pf1bit,										//!< 1 bit/pixel (monochrome)
	pf4bit,										//!< 4 bits/pixel (16 couleurs)
	pf8bit,										//!< 8 bits/pixel (256 couleurs)
	pf15bit,									//!< 15 bits/pixel (32768 couleurs)
	pf16bit,									//!< 16 bits/pixel (65536 couleurs)
	pf24bit,									//!< 24 bits/pixel (16 millions de couleurs)
	pf32bit,									//!< 32 bits/pixel (16 millions de couleurs + transparence)
	pfCustom									//!< Autre
};

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TStream;


//-----------------------------------------------------------------------------
//! @class TGraphic
//!
//! @brief Définition de la classe TGraphic
//!
//! Cette classe est la classe de base des composants de type image. Elle
//! définit des méthodes de lecture et d'écriture dans un fichier, un flux ou
//! la lecture depuis les ressources, et une méthode de dessin dans un HDC.
//! Cette classe est une classe abstraite, vous ne pouvez donc pas
//! l'implémenter, utilisez ses descendants (@b TBitmap par exemple).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TGraphic: public TPersistent {
private:
protected:

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TGraphic(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TGraphic(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Chargement à partir d'un flux.
  //!
  //! Cette méthode permet de lire l'image à partir d'un flux. C'est une
	//! méthode virtuelle pure, il faut donc l'implémenter dans toutes les classes
	//! qui dérivent de celle-ci.
  //!
  //! @param[in]        Stream Flux source.
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromStream(TStream* Stream) = 0;

  //---------------------------------------------------------------------------
  //! @brief Lecture à partir d'un fichier
  //!
  //! Cette méthode permet de lire le graphique à partir d'un fichier
  //!
  //! @param[in]        asFileName Fichier à lire
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromFile(const AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Chargement à partir des ressources.
  //!
  //! Cette méthode permet de lire un objet graphique depuis les ressources
  //! (fichier RC). 
  //!
  //! @param[in]        hInstance Handle de l'instance du programme (généralement HInstance)
  //! @param[in]        ResID Identifiant de la ressource.
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromResourceID(HINSTANCE hInstance, int ResID);

  //---------------------------------------------------------------------------
  //! @brief Lecture à partir des ressources
  //!
  //! Cette méthode permet de lire un objet graphique dans les ressources.
  //!
  //! @param[in]        hInstance Handle d'instance
  //! @param[in]        ResName Nom de la ressource
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromResourceName(HINSTANCE hInstance, const AnsiString ResName);

  //---------------------------------------------------------------------------
  //! @brief Lecture à partir des ressources
  //!
  //! Cette méthode permet de lire un objet graphique dans les ressources. C'est
	//! une méthode virtuelle pure, il faut donc l'implémenter dans toutes les
	//! classes qui dérivent de celle-ci.
  //!
  //! @param[in]        hInstance Handle d'instance
  //! @param[in]        szResName Nom ou ID de la ressource
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName) = 0;

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un flux.
  //!
  //! Cette méthode permet d'enregistrer le graphique dans un flux.
  //!
  //! @param[in]        Stream Adresse du flux à enregistrer.
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SaveToStream(TStream *Stream) = 0;

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un fichier
  //!
  //! Cette méthode permet d'enregistrer le graphique dans un fichier
  //!
  //! @param[in]        asFileName Fichier destination
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SaveToFile(const AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Lecture des données
	//!
	//! Cette méthode permet de lire les données brutes de l'image sous forme
	//! de chaîne hexadécimale (pour enregistrement dans le DFM, par exemple).
  //!
  //! @return @c AnsiString Chaîne hexa.
  //---------------------------------------------------------------------------

  virtual AnsiString FASTCALL GetData(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'un graphique dans un HDC
  //!
  //! Cette méthode permet de dessiner le graphique dans un HDC
  //!
  //! @param[in]        hdc HDC où dessiner
  //! @param[in]        X Position en X
  //! @param[in]        Y Position en Y
  //! @param[in]        L Largeur à dessiner
  //! @param[in]        H Hauteur à dessiner
  //---------------------------------------------------------------------------

  virtual void FASTCALL Draw(HDC hdc, int X, int Y, int L, int H) = 0;

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
  //! @brief Propriété Width
	//!
	//! Cette propriété permet de lire la largeur de l'image associée au
	//! graphique.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TGraphic, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété permet de lire la hauteur de l'image associée au
	//! graphique.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TGraphic, int, Height);

  //@}


};

#else  // TGraphicH

class TGraphic;

#endif  // TGraphicH

#undef In_TGraphicH
