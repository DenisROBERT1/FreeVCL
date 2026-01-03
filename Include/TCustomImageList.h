//-----------------------------------------------------------------------------
//! @file TCustomImageList.h
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

#ifdef In_TCustomImageListH
#error "Inclusion circulaire de TCustomImageList"
#endif // In_TCustomImageListH

#define In_TCustomImageListH

#ifndef TCustomImageListH
#define TCustomImageListH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TResType {
	rtBitmap,											//!< Ressource de type bitmap
	rtCursor,											//!< Ressource de type curseur
	rtIcon												//!< Ressource de type icône
};


//-----------------------------------------------------------------------------
//! @class TCustomImageList
//!
//! @brief Définition de la classe TCustomImageList
//!
//! Cette classe est la classe de base des listes d'images.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCustomImageList: public TComponent {
protected:
  HIMAGELIST FHandle;
  HBITMAP FImageBitmap;
  int FWidth;
  int FHeight;
  int FAllocBy;
  TColor FBkColor;
  bool FMasked;
  int FResourceId;
  AnsiString FResourceName;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TCustomImageList(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomImageList(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Mise à zéro de la liste d'images.
  //!
  //! Cette méthode met à zéro la liste d'image.
  //---------------------------------------------------------------------------
  void Clear(void);

	//---------------------------------------------------------------------------
  //! @brief Ajout d'une image à la liste d'images.
  //!
  //! Cette méthode ajoute une image et éventuellement un masque à la liste
	//! d'image. Le masque sert à rendre certaines parties de l'image
	//! transparente.
  //!
  //! @param[in]				Image Bitmap à insérer.
  //! @param[in]        Mask Masque de l'image, ou NULL.
  //!
  //! @return @c int Index de la nouvelle image, ou -1 en cas d'échec.
	//!
	//! @warning Cette méthode affecte les images par copie. C'est à dire que
	//! c'est à l'utilisateur de détruire les objets après le retour de la
	//! fonction pour éviter une fuite de mémoire.
  //---------------------------------------------------------------------------

	int Add(TBitmap *Image, TBitmap *Mask);

	//---------------------------------------------------------------------------
  //! @brief Destruction d'une image de la liste d'images.
  //!
  //! Cette méthode détruit l'image numéro Index de la liste d'images.
  //!
  //! @param[in]        Index Index de l'image à supprimer.
  //---------------------------------------------------------------------------

	void Delete(int Index);

  //---------------------------------------------------------------------------
  //! @brief Copie d'une image dans une TBitmap.
  //!
  //! Cette méthode copie une image de la liste d'image dans un objet TBitmap.
  //!
  //! @param[in]        Index Index de l'image à copier.
  //! @param[in]				Image Objet TBitmap destination.
  //---------------------------------------------------------------------------
	void GetBitmap(int Index, TBitmap *Image);

  //---------------------------------------------------------------------------
  //! @brief Retourne toutes les images de la liste d'image.
  //!
  //! Cette méthode retourne une image contenant l'ensemble des images de la
	//! liste d'images. Les images sont organisées de gauche à droite.
  //!
  //! @return @c HBITMAP Handle de bitmap, ou NULL si echec.
	//!
	//! @warning Ne pas détruire l'objet GDI HBITMAP, il reste la propriété de
	//! la liste d'images.
  //---------------------------------------------------------------------------
	HBITMAP GetImageBitmap(void);

	bool ResourceLoad(TResType ResType, const AnsiString Name, TColor MaskColor);
  bool Draw(HDC hdc, int x, int y, int Index);

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

  bool SetProperty(AnsiString asProperty, AnsiString asValue,
                                TPersistent *Sender);
  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Identifiant système du composant
	//!
	//! Cette propriété définit le handle système de la liste d'image. Utilisez
	//! ce handle pour effectuer des opération qui ne seraient pas gérées par la
	//! VCL.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, HIMAGELIST, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Width
	//!
	//! Cette propriété définit la largeur d'une image en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, int, Width);

  //---------------------------------------------------------------------------
  //! @brief Propriété Height
	//!
	//! Cette propriété définit la hauteur d'une image en pixels.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, int, Height);

  //---------------------------------------------------------------------------
  //! @brief Propriété AllocBy
	//!
	//! Cette propriété définit le nombre d'images allouées en même temps lorsque
	//! la place mémoire est insuffisante. Augmentez cette valeur si vous avez à
	//! créer un grand nombre d'images, pour éviter des réallocations trop
	//! nombreuses.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, int, AllocBy);

  //---------------------------------------------------------------------------
  //! @brief Propriété BkColor
  //!
  //! Cette propriété permet de définir la couleur de fond des images contenues
	//! dans la liste d'images. Cette couleur n'est visible que lorsqu'on utilise
	//! des images transparentes (paramètre @b Mask de la méthode @b Add)
	//!
	//! @sa Add
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, TColor, BkColor);

  //---------------------------------------------------------------------------
  //! @brief Propriété Count
	//!
	//! Cette propriété permet de lire le nombre d'images que contient la liste.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCustomImageList, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété Masked
	//!
	//! Cette propriété permet de définir de images de masquage (c'est à dire
	//! transparentes)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, bool, Masked);

  //---------------------------------------------------------------------------
  //! @brief Propriété ResourceId
	//!
	//! Cette propriété permet de lire directement une liste d'image depuis les
	//! ressources du programme. Affectez la propriété et la lecture se fait
	//! automatiquement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, int, ResourceId);

  //---------------------------------------------------------------------------
  //! @brief Propriété ResourceName
	//!
	//! Cette propriété permet de lire directement une liste d'image depuis les
	//! ressources du programme. Affectez la propriété et la lecture se fait
	//! automatiquement.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomImageList, AnsiString, ResourceName);

  //@}

};


#else  // TCustomImageListH

class TCustomImageList;

#endif  // TCustomImageListH


#undef In_TCustomImageListH
