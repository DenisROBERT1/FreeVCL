//-----------------------------------------------------------------------------
//! @file TImage.h
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

#ifdef In_TImageH
#error "Inclusion circulaire de TImage"
#endif // In_TImageH

#define In_TImageH

#ifndef TImageH
#define TImageH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TPicture;


//-----------------------------------------------------------------------------
//! @class TImage
//!
//! @brief Définition de la classe TImage
//!
//! Cette classe est un contrôle permettant l'affichage d'une image. L'image
//! affichée est définie par la propriété @b Picture, qui peut contenir une
//! icône, une image de type @b bmp, etc...
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TImage: public TWinControl {
private:
protected:
	bool FCenter;
	bool FProportional;
	bool FStretch;
  TPicture * FPicture;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessPaint(HDC hdc, RECT rcPaint);

  //---------------------------------------------------------------------------
  //! @brief Changement de l'image
  //---------------------------------------------------------------------------

  void PictureChange(TObject *Sender);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TImage(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TImage(void);

  //@}


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
  //! @brief Propriété Center: Image centrée
	//!
	//! @image html TImage_cf_pf_sf.jpg "Center = False, Proportional = True ou False, Stretch = False"
	//! @image html TImage_ct_pf_sf.jpg "Center = True, Proportional = True ou False, Stretch = False"
	//! @image html TImage_cf_pf_st.jpg "Center = True ou False, Proportional = False, Stretch = True"
	//! @image html TImage_cf_pt_st.jpg "Center = False, Proportional = True, Stretch = True"
	//! @image html TImage_ct_pt_st.jpg "Center = True, Proportional = True, Stretch = True"
	//!
	//! Cette propriété permet de centrer l'image sur le composant. Sinon,
	//! l'image est calée sur le coin inférieur gauche. Si la propriété
	//! @b Proportional est à false et que la propriété @b Stretch est à true,
	//! cette propriété n'a aucun effet.
	//!
	//! @sa Proportional, Stretch
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TImage, bool, Center);

  //---------------------------------------------------------------------------
  //! @brief Propriété Proportional: Image proportionnelle
	//!
	//! Cette propriété permet de déformer l'image pour qu'elle remplisse la
	//! totalité du contrôle (si la propriété est à true). Si elle est égale à
	//! false, le rapport hauteur sur largeur de l'image est conservé, et une ou
	//! deux bandes (selon le centrage) sont laissées libres. Si la propriété
	//! @b Stretch est à false, cette propriété n'a aucun effet.
	//!
	//! @sa Center, Stretch
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY(TImage, bool, Proportional);

  //---------------------------------------------------------------------------
  //! @brief Propriété Stretch: Image redimensionée
	//!
	//! Cette propriété permet de redimensionner l'image sur le composant. Si
	//! cette propriété est à false, l'image sera coupée si elle est plus grande,
	//! ou un espace sera laissé libre si elle est plus petite que le composant.
	//!
	//! @sa Center, Proportional
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY(TImage, bool, Stretch);

  //---------------------------------------------------------------------------
  //! @brief Propriété Picture: Image apparaissant dans le contrôle
	//!
	//! Cette propriété permet de définir l'image (bitmap, icône, ...) qui
	//! apparaît dans le composant.@n
	//! Exemple:@n
	//! MyImage->Picture->Graphic = MyBitmap;@n
	//!
	//! @note Le composant fait une copie de l'objet Picture, mais n'en devient
	//! pas propriétaire. La fonction appelante est donc chargée de sa
	//! destruction après utilisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TImage, TPicture *, Picture);

  //@}


};


#else  // TImageH

class TImage;

#endif  // TImageH


#undef In_TImageH
