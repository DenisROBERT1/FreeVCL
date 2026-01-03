//-----------------------------------------------------------------------------
//! @file TBitmap.h
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

#ifdef In_TBitmapH
#error "Inclusion circulaire de TBitmap"
#endif // In_TBitmapH

#define In_TBitmapH

#ifndef TBitmapH
#define TBitmapH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <string>

#ifndef TGraphicH
#include "TGraphic.h"
#endif  // TGraphicH

#ifndef TBMPFileH
#include "TBMPFile.h"
#endif  // TBMPFileH

#ifndef TPNGFileH
#include "TPNGFile.h"
#endif  // TPNGFileH

#ifndef TCURFileH
#include "TCURFile.h"
#endif  // TCURFileH

#ifndef TCanvasH
#include "TCanvas.h"
#endif  // TCanvasH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TStretchBltMode {
	smBlackOnWhite = 1,	       //!< Fait une opération AND avec le fond. Le noir reste noir, les autres couleurs sont mélangées.
	smWhiteOnBlack = 2,				 //!< Fait une opération OR avec le fond. Le blanc reste blanc, les autres couleurs sont mélangées.
	smColorOnColor = 3,				 //!< Copie les pixels en éliminant les lignes surnuméraires. Il y a donc perte d'information.
	smHalfTone = 4						 //!< Copie les pixels en fusionnant les lignes surnuméraires. Le rendu final est meilleur.
};

//-----------------------------------------------------------------------------
//! @class TBitmap
//!
//! @brief Définition de la classe TBitmap
//!
//! Cette classe permet de gérer une image pour l'afficher dans une boîte de
//! dialogue ou bien pour y effectuer un traitement. Elle encapsule une bitmap
//! Windows et sa palette.@n
//! Elle hérite de TGraphic, donc en contient les méthodes permettant de charger
//! l'image depuis un fichier ou les ressources, de la dessiner dans un hdc, de
//! l'enregistrer dans un fichier.@n
//! L'image est systématiquement mémorisée avec sa résolution optimum, ce qui
//! signifie que vous pouvez lire une image de 16 millions de couleurs, y
//! effectuer des changements, et l'enregistrer ensuite sans perte
//! d'informations quel que soit la résolution de votre écran.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TBitmap: public TGraphic {
private:
protected:
  HDC hdcMem;
  HBITMAP hOldHandle;
  HBITMAP FHandle;
  int FWidth;
  int FHeight;
  int FBitCount;
  int FXHotSpot;
  int FYHotSpot;
  TPaletteVCL *FPaletteVCL;
  TPixelFormat FPixelFormat;
	TStretchBltMode FStretchBltMode;
  DWORD Compression;
  void *lpvBits;

  TCanvas *FCanvas;

	LongLong FASTCALL GetFileSize(void);
	bool FASTCALL SetHandleCanvas(void);
	bool FASTCALL ReleaseHandleCanvas(void);

public:

  //! @name Constructeur et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TBitmap(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TBitmap(void);

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
  //! @brief Copie d'une bitmap sur une autre
  //!
  //! Cette méthode permet d'initialiser la bitmap à partir d'une autre.
  //!
  //! @param[in]        Source Bitmap d'origine.
  //---------------------------------------------------------------------------

  virtual void FASTCALL Assign(TPersistent* Source);

  //---------------------------------------------------------------------------
  //! @brief Libère le handle de contexte associé à l'image.
  //!
  //! Cette méthode permet de libérer le handle de contexte (HDC) lié
  //! automatiquement à la bitmap lorsqu'on la charge ou qu'on dessine dedans.
	//! Cette méthode libére également le lien entre la bitmap et le @b Canvas.
  //! Appelez cette méthode pour libérer de la place mémoire ou lorsque vous
  //! devez passez le handle à une fonction système. Certaines fonctions ne
	//! marchent pas si vous passez un handle encore lié à un HDC, c'est
	//! notamment le cas avec les @b ImageList.
	//!
	//! @note Cette méthode ne libère pas le handle de bitmap lui-même, à la
	//! différence de la méthode @b ReleaseHandle.
	//!
	//! @sa ReleaseHandle
  //---------------------------------------------------------------------------

  void FASTCALL FreeImage(void);

  //---------------------------------------------------------------------------
  //! @brief Indique si le handle de l'objet sous-jacent existe.
  //!
  //! Cette méthode permet de déterminer si la propriété @b Handle a une valeur
  //! HBITMAP, ce qui indique que l'objet Windows a été créé. Il est préférable
  //! d'utiliser cette méthode plutôt que de lire la propriété @b Handle, car
  //! cette dernière crée automatiquement un handle s'il n'existe pas.
  //!
  //! @return @c bool true si handle existe.
  //---------------------------------------------------------------------------

  bool FASTCALL HandleAllocated(void);

  //---------------------------------------------------------------------------
  //! @brief Chargement à partir d'un flux.
  //!
  //! Cette méthode permet de lire l'image à partir d'un flux. Elle est
  //! redéfinie depuis TGraphic::LoadFromStream (voir cette méthode pour plus
  //! de détails).
  //!
  //! @param[in]        Stream Flux source.
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromStream(TStream *Stream);

  //---------------------------------------------------------------------------
  //! @brief Retourne et libère le handle de bitmap.
  //!
  //! Cette méthode permet de lire le handle Windows de l'image (HBITMAP) et de
  //! le mettre à zéro en interne. Ce qui signifie que la fonction appelante
  //! aura à sa charge la libération de ce handle (par un @b DeleteObject) pour
  //! éviter une fuite de mémoire. Utilisez cette méthode lorsque vous devez
	//! passer un handle à une fonction ou une méthode qui détruit le handle de
	//! bitmap en fin de traitement.
  //!
  //! @return @c HBITMAP Handle de bitmap.
	//!
	//! @sa FreeImage
  //---------------------------------------------------------------------------

  HBITMAP FASTCALL ReleaseHandle(void);
  
  //---------------------------------------------------------------------------
  //! @brief Lecture à partir des ressources
  //!
  //! Cette méthode permet de lire une bitmap dans les ressources.
  //!
  //! @param[in]        hInstance Handle d'instance
  //! @param[in]        szResName Nom ou ID de la ressource
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un flux.
  //!
  //! Cette méthode permet d'enregistrer l'image dans un flux. Elle est
  //! redéfinie depuis TGraphic::SaveToFile (voir cette méthode pour plus de
  //! détails).
  //!
  //! @param[in]        Stream Adresse du flux à enregistrer.
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL SaveToStream(TStream *Stream);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un fichier.
  //!
  //! Cette méthode permet d'enregistrer l'image dans un fichier. Elle est
  //! redéfinie depuis TGraphic::SaveToFile (voir cette méthode pour plus de
  //! détails).
  //!
  //! @param[in]        asFileName Nom du fichier bmp à créer.
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

  virtual AnsiString FASTCALL GetData(void);

  //---------------------------------------------------------------------------
  //! @brief Dessin de l'image graphique dans un HDC.
  //!
  //! Cette méthode permet de dessiner le graphique dans un contexte de
  //! dispositif (HDC).
  //!
  //! @param[in]        hdc HDC où dessiner l'image.
  //! @param[in]        X Position en X.
  //! @param[in]        Y Position en Y.
  //! @param[in]        L Largeur à dessiner.
  //! @param[in]        H Hauteur à dessiner.
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
  //! @brief Propriété Canvas: Canvas de l'image
  //!
  //! Utilisez cette méthode pour obtenir un objet TCanvas permettant de
  //! dessiner directement dans l'image.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TBitmap, TCanvas *, Canvas);

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: handle HBITMAP de l'image.
  //!
  //! Utiliser cette propriété pour obtenir un handle Windows de Bitmap
  //! (HBITMAP). Si le handle n'a pas encore été créé, le fait de le lire en
  //! crée automatiquement un. Pour cette raison, il est préférable d'utiliser
  //! la méthode @b HandleAllocated si le but est simplement de savoir s'il
  //! existe ou pas.@n
  //!
	//! Si vous affectez cette propriété, le handle doit être un handle de bitmap
	//! indépendant du périphérique (DIB = Device Independant Bitmap). Dans le cas
	//! contraire, l'image risque de ne pas s'afficher correctement.@n
  //!
	//! Lorsque vous affectez cette propriété, les propriétés Width, Height, 
	//! BitCount et PixelFormat sont automatiquement ajustées en fonction des
	//! caractéristiques du handle de départ.
	//!
  //! @note L'objet TBitmap reste propriétaire du handle, ce qui signifie que
  //! l'utilisateur ne doit pas détruire l'objet ni le passer à une fonction
  //! qui le détruit. Si ça doit être le cas, utilisez la méthode
  //! @b ReleaseHandle
  //!
  //! @sa HandleAllocated ReleaseHandle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, HBITMAP, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété PixelFormat: format de pixel
  //!
  //! Utilisez cette propriété pour lire ou pour définir le format d'un pixel
  //! (nombre de bits pour mémoriser un pixel).@n
  //! Si vous désirez avoir directement le nombre de bits d'un pixel (pour
  //! calculer la taille totale de l'image, par exemple), utilisez la méthode
  //! @b BitCount.
  //!
  //! @sa BitCount
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, TPixelFormat, PixelFormat);

  //---------------------------------------------------------------------------
  //! @brief Propriété BitCount: Nombre de bits d'un pixel
  //!
  //! Cette propriété est similaire à la propriété @b PixelFormat, mais sous
  //! forme d'un entier. Changer l'une change également l'autre.
  //!
  //! @sa PixelFormat
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, int, BitCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété Palette: Palette Windows
  //!
  //! Cette propriété permet de lire ou de définir la palette Windows utilisée
  //! par l'image.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, HPALETTE, Palette);

  //---------------------------------------------------------------------------
  //! @brief Propriété PaletteVCL: Objet définissant la palette
  //!
  //! Cette propriété représente l'objet VCL encapsulant la palette Windows.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, TPaletteVCL *, PaletteVCL);

  //---------------------------------------------------------------------------
  //! @brief Propriété StretchBltMode: Mode de mise à l'échelle du dessin. 
  //!
  //! Cette propriété définit le mode de mise à l'échelle et l'opération
	//! booléenne avec le fond lors de l'appel de la méthode Draw.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, TStretchBltMode, StretchBltMode);

  //---------------------------------------------------------------------------
  //! @brief Propriété XHotSpot: coordonnée X du hotspot
  //!
  //! Utilisez cette propriété pour lire ou pour définir le X du hotspot.
  //! Cette propriété n'a une signification que si l'objet est de type curseur.
	//! dans le cas contraire, lire ou enregistrer cette propriété n'a aucun
	//! effet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, int, XHotSpot);

  //---------------------------------------------------------------------------
  //! @brief Propriété XHotSpot: coordonnée Y du hotspot
  //!
  //! Utilisez cette propriété pour lire ou pour définir le Y du hotspot
  //! Cette propriété n'a une signification que si l'objet est de type curseur.
	//! dans le cas contraire, lire ou enregistrer cette propriété n'a aucun
	//! effet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBitmap, int, YHotSpot);

  //@}

};

#else  // TBitmapH

class TBitmap;

#endif  // TBitmapH

#undef In_TBitmapH
