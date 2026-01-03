//-----------------------------------------------------------------------------
//! @file TIcon.h
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

#ifdef In_TIconH
#error "Inclusion circulaire de TIcon"
#endif // In_TIconH

#define In_TIconH

#ifndef TIconH
#define TIconH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#include "TGraphic.h"
#include "TBitmap.h"
#include "TICOFile.h"


//-----------------------------------------------------------------------------
//! @class TIcon
//!
//! @brief Définition de la classe TIcon
//!
//! Cette classe permet de gérer une image de type ICO pour l'afficher dans
//! une boîte de dialogue ou bien pour y effectuer un traitement.@n
//! Elle hérite de TGraphic, donc en contient les méthodes permettant de charger
//! l'image depuis un fichier ou les ressources, de la dessiner dans un hdc, de
//! l'enregistrer dans un fichier.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TIcon: public TGraphic {
private:
protected:

  std::vector<TBitmap *> ListBitmaps;
  HICON FHandle;
  int FWidth;
  int FHeight;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TIcon();

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TIcon(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Width
  //!
  //! Cette méthode permet de lire la propriété Width
  //!
  //! @return @c int Propriété Width
  //---------------------------------------------------------------------------

  virtual int Get_Width(void);


  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Width
  //!
  //! Cette méthode permet d'écrire la propriété Width
  //!
  //! @param[in]        NewWidth Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Width(int NewWidth);


  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Height
  //!
  //! Cette méthode permet de lire la propriété Height
  //!
  //! @return @c int Propriété Height
  //---------------------------------------------------------------------------

  virtual int Get_Height(void);


  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Height
  //!
  //! Cette méthode permet d'écrire la propriété Height
  //!
  //! @param[in]        NewHeight Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Height(int NewHeight);


  //---------------------------------------------------------------------------
  //! @brief Copie d'une icône sur une autre
  //!
  //! Cette méthode permet d'initialiser l'icône à partir d'une autre.
  //!
  //! @param[in]        Source Icône d'origine.
  //---------------------------------------------------------------------------

  virtual void FASTCALL Assign(TPersistent* Source);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'une bitmap dans l'icône
  //!
  //! Cette méthode permet d'ajouter une image à l'icône
  //!
  //! @param[in]        NewBitmap Nouvelle bitmap à ajouter
  //!
  //! @return @c bool true si l'opération a réussi.
  //!
  //! @warning L'objet TIcon fait une copie de la bitmap, mais n'en devient pas
	//! propriétaire. La fonction appelante est donc responsable de la
	//! destruction de l'objet TBitmap après utilisation.
  //---------------------------------------------------------------------------

  bool FASTCALL AddBitmap(TBitmap *NewBitmap);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une bitmap de l'icône
  //!
  //! Cette méthode permet d'enlever une image à l'icône
  //!
  //! @param[in]        IndexBitmap Index de l'image à enlever
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  bool FASTCALL RemoveBitmap(int IndexBitmap);

	//---------------------------------------------------------------------------
	//! @brief Chargement à partir d'un flux
  //!
  //! Cette méthode permet de lire une icône à partir d'un flux
  //!
  //! @param[in]        Stream Flux source
  //!
  //! @return @c bool true si l'opération a réussi.
	//---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromStream(TStream *Stream);

  //---------------------------------------------------------------------------
  //! @brief Chargement à partir des ressources
  //!
  //! Cette méthode permet de lire une icône dans les ressources
  //!
  //! @param[in]        hInstance Handle d'instance
  //! @param[in]        szResName Nom de la ressource
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un flux.
  //!
  //! Cette méthode permet d'enregistrer l'icône dans un flux. Elle est
  //! redéfinie depuis TGraphic::SaveToFile (voir cette méthode pour plus de
  //! détails).
  //!
  //! @param[in]        Stream Adresse du flux à enregistrer.
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SaveToStream(TStream *Stream);

  //---------------------------------------------------------------------------
  //! @brief Lecture des données
	//!
	//! Cette méthode permet de lire les données brutes de l'image sous forme
	//! de chaîne hexadécimale (pour enregistrement dans le DFM, par exemple).
  //!
  //! @return @c AnsiString Chaîne hexa.
  //---------------------------------------------------------------------------

  virtual AnsiString FASTCALL GetData(void);

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

  virtual void FASTCALL Draw(HDC hdc, int X, int Y, int L, int H);

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
  //! @brief Propriété Handle: Identifiant système
	//!
	//! Cette propriété permet de lire et de définir le handle système de l'icône
	//! encapsulée dans l'objet @b TIcon. L'objet TIcon ne devient pas
	//! propriétaire de ce handle. Il revient donc à l'utilisateur de détruire
	//! l'objet attaché au handle après la destruction de l'objet @b TIcon.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TIcon, HICON, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété BitmapCount
	//!
	//! Cette propriété permet de lire le nombre d'images que contient l'icône.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TIcon, int, BitmapCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété Bitmap
	//!
	//! Cette propriété permet de lire une image que contient l'icône.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TIcon, TBitmap *, Bitmap);

  //@}


};


#else  // TIconH

class TIcon;

#endif  // TIconH


#undef In_TIconH
