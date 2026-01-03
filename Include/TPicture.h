//-----------------------------------------------------------------------------
//! @file TPicture.h
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


#ifdef In_TPictureH
#error "Inclusion circulaire de TPicture"
#endif // In_TPictureH

#define In_TPictureH

#ifndef TPictureH
#define TPictureH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TGraphic;
class TBitmap;


//-----------------------------------------------------------------------------
//! @class TPicture
//!
//! @brief Définition de la classe TPicture
//!
//! Cette classe encapsule un graphisme, qui peut être une image classique (BMP
//! par exemple) ou vectorielle (WMF ou EMF). Utilisez la propriété @b Graphic
//! pour accéder à l'objet graphique sous-jacent.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPicture: public TPersistent {
private:
protected:

  TGraphic *FGraphic;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPicture();

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPicture(void);

  //@}


  //! @name Méthodes
  //@{

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
  //! @brief Chargement à partir des ressources
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromResourceName(HINSTANCE hInstance, const AnsiString ResName);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un fichier
  //!
  //! @param[in]        asFileName Fichier à lire
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromFile(const AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Sauvegarde dans un fichier
  //!
  //! @param[in]        asFileName Fichier destination
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SaveToFile(const AnsiString asFileName);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Width: largeur de l'image
	//!
	//! Cette propriété définit la largeur de l'image.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPicture, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height: hauteur de l'image
	//!
	//! Cette propriété définit la hauteur de l'image.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPicture, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Propriété Graphic: image utilisée
	//!
	//! Cette propriété définit l'image contenue dans l'objet picture. L'image
	//! est recopiée, c'est à dire qu'il revient à l'utilisateur de détruire
	//! l'objet passé en paramètres après utilisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPicture, TGraphic *, Graphic);

  //---------------------------------------------------------------------------
  //! @brief Propriété Bitmap
	//!
	//! Cette propriété permet de définir l'image (ou les images) qui apparaît
	//! dans le composant.
	//!
	//! @note Le composant fait une copie de l'image, mais n'en devient pas
	//! propriétaire. La fonction appelante est donc chargée de sa destruction
	//! après utilisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPicture, TBitmap *, Bitmap);

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


#else  // TPictureH

class TPicture;

#endif  // TPictureH

#undef In_TPictureH
