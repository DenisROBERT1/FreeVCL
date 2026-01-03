//-----------------------------------------------------------------------------
//! @file TWebCam.h
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

#ifdef In_TWebCamH
#error "Inclusion circulaire de TWebCam"
#endif // In_TWebCamH

#define In_TWebCamH

#ifndef TWebCamH
#define TWebCamH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifdef __GNUG__
#include <webcam.h>
#else  // __GNUG__
#include <vfw.h>
#endif  // __GNUG__


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnFrame.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnFrame)(TObject *Sender, TBitmap *Frame);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnError2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnError2)(TObject *Sender, AnsiString Message, AnsiString Title);


//---------------------------------------------------------------------------
//! @class TWebCam
//!
//! @brief Fenêtre d'affichage de la WebCam
//!
//! Cette classe (@c TWebCam) permet de récupérer, d'afficher et de traiter
//! les images venant de la webcam.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TWebCam: public TWinControl {
private:
  int BitmapWidth, BitmapHeight;
  TPixelFormat BitmapPixelFormat;
	int BitmapBitCount;

protected:
  bool FConnected;
  int FPreviewRate;
  bool FPreview;
  bool FPreviewScale;

	HWND FHandleWebCam;
	HWND FHandleCache;
	DWORD biCompression;

  //---------------------------------------------------------------------------
  //! @brief Changement du format vidéo
  //!
  //! Cette méthode est appelée après un changement du format vidéo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool ChangeFormatVideo();

  bool ConvertFormatVideo(DWORD biCompression, TPixelFormat PixelFormat, int Width, int Height, BYTE* lpBits1, BYTE* lpBits2);
	COLORREF YVUtoRGB(int Y, int V, int U);

public:


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TWebCam(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  ~TWebCam();

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Connexion à une web-cam
  //!
  //! Cette méthode permet de se connecter à une web-cam. Le numéro de web-cam
	//! permet d'en choisir une si plusieurs sont connectées. La première
	//! connectée a le numéro zéro.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool ConnectWebCam(int NumWebCam);

  //---------------------------------------------------------------------------
  //! @brief Connexion à une web-cam
  //!
  //! Cette méthode permet de se déconnecter d'une web-cam.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool DisconnectWebCam(void);

  //---------------------------------------------------------------------------
  //! @brief Nom d'un numéro de driver
  //!
  //! Cette méthode permet de lire le nom du driver d'indice i
  //!
  //! @param[in]        i Indice du driver
  //!
  //! @return @c AnsiString Nom du driver
  //---------------------------------------------------------------------------

  AnsiString GetDriverName(int i);


  //---------------------------------------------------------------------------
  //! @brief Dialogue de paramétrage du format de compression
  //!
  //! Cette méthode permet d'afficher une boîte de dialogue permettant de régler
  //! les paramètres de compression de l'image.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool DlgVideoCompression(void);


  //---------------------------------------------------------------------------
  //! @brief Dialogue de paramétrage du format d'affichage
  //!
  //! Cette méthode permet d'afficher une boîte de dialogue permettant de régler
  //! les paramètres d'affichage de l'image.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool DlgVideoDisplay(void);


  //---------------------------------------------------------------------------
  //! @brief Dialogue de paramétrage du format d'affichage
  //!
  //! Cette méthode permet d'afficher une boîte de dialogue permettant de régler
  //! les paramètres d'affichage de l'image.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool DlgVideoFormat(void);


  //---------------------------------------------------------------------------
  //! @brief Déclenchement d'une capture d'image
  //!
  //! Cette méthode permet de déclencher la réception d'une image
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool GetFrame();

  //---------------------------------------------------------------------------
  //! @brief Traitement d'une capture d'image
  //!
  //! Cette méthode permet de traiter la réception d'une image capturée
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool ProcessFrame(LPVIDEOHDR lpVHdr);

  //---------------------------------------------------------------------------
  //! @brief Fonction CALLBACK de traitement d'images
  //!
  //! N'appelez pas cette méthode. Cette méthode est appelée par le système
	//! lorsqu'une image a été capturée.
  //!
  //! @return @c LRESULT 0 si évènement traité
  //---------------------------------------------------------------------------
  static LRESULT CALLBACK CallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr);

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
  //! @brief Propriété Connected
	//!
	//! Cette propriété permet de se connecter ou de se déconecter à une web-cam.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TWebCam, bool, Connected);

  //---------------------------------------------------------------------------
  //! @brief Propriété PreviewRate
	//!
	//! Cette propriété définit le delai entre deux captures d'écran en mode
	//! prévisualisation (en millisecondes). Un délai trop court peut surcharger
	//! inutilement le CPU. La valeur par défaut dépend de la webcam utilisée (la
	//! propriété est initialisée à zéro).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TWebCam, int, PreviewRate);

  //---------------------------------------------------------------------------
  //! @brief Propriété Preview
	//!
	//! Cette propriété permet d'activer ou de désactiver la prévisualisation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TWebCam, bool, Preview);

  //---------------------------------------------------------------------------
  //! @brief Propriété PreviewScale
	//!
	//! Cette propriété permet d'adapter automatiquement la sortie vidéo à la
	//! taille de la fenêtre de visualisation. Si cette propriété est à @b false,
	//! l'image est coupée ou remplit partiellement la fenêtre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TWebCam, bool, PreviewScale);

  //---------------------------------------------------------------------------
  //! @brief Propriété DriverName
	//!
	//! Cette propriété permet de lire le nom du pilote de périphérique.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TWebCam, AnsiString, DriverName);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnFrame
	//!
	//! Cet évènement est déclenché lorsqu'une image est envoyée par la web-cam.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Image reçue
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnFrame, OnFrame, TObject *, TBitmap *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnError
	//!
	//! Cet évènement est déclenché lorsqu'une erreur survient.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Message d'erreur
  //! @param[in]        Arg3 Titre du message d'erreur
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnError2, OnError, TObject *, AnsiString, AnsiString);

  //@}

};


//---------------------------------------------------------------------------


#else  // TWebCamH

class TWebCam;

#endif  // TWebCamH


#undef In_TWebCamH

