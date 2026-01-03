//-----------------------------------------------------------------------------
//! @file TMetafile.h
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

#ifdef In_TMetafileH
#error "Inclusion circulaire de TMetafile"
#endif // In_TMetafileH

#define In_TMetafileH

#ifndef TMetafileH
#define TMetafileH


//---------------------------------------------------------------------------
//! @class TMetafile
//!
//! @brief Fichier metafile
//!
//! Cette classe (@c TMetafile) représente un fichier WMF.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TMetafile: public TGraphic {
private:

protected:
  TCanvas *FCanvas;
  AnsiString FCreatedBy;
  AnsiString FDescription;
  bool FEmpty;
  bool FEnhanced;
  HENHMETAFILE FHandle;
  int FHeight;
  Word FInch;
  int FMMHeight;
  int FMMWidth;
  bool FTransparent;
  int FWidth;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TMetafile(TComponent *AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TMetafile(void);

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
  //! @brief Efface complètement l'image.
  //!
  //! Cette méthode permet d'effacer complètement l'image
  //---------------------------------------------------------------------------

  void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Indique si l'objet TMetafile a acquis un HDC.
  //!
  //! Cette méthode indique si l'objet TMetafile a acquis un handle pour
  //! l'objet GDI sous-jacent.
  //!
  //! @return @c bool True si handle créé.
  //---------------------------------------------------------------------------

  bool FASTCALL HandleAllocated(void);

  //---------------------------------------------------------------------------
  //! @brief Libère l'objet GDI Windows représenté par le métafichier.
  //!
  //! Cette méthode libère l'objet GDI Windows représenté par le métafichier.
  //!
  //! @return @c bool true si handle libéré.
  //---------------------------------------------------------------------------

  bool FASTCALL ReleaseHandle(void);

  //---------------------------------------------------------------------------
  //! @brief Renvoie les données brutes du métafichier.
  //!
  //! Cette méthode renvoie les données brutes comme lorsqu'on les enregistre
	//! dans un fichier.
  //!
  //! @return @c AnsiString Données brutes.
  //---------------------------------------------------------------------------

	AnsiString FASTCALL GetData(void);

  //---------------------------------------------------------------------------
  //! @brief Chargement à partir d'un flux.
  //!
  //! Cette méthode permet de lire l'image à partir d'un flux.
  //!
  //! @param[in]        Stream Flux source.
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromStream(TStream* Stream);

  //---------------------------------------------------------------------------
  //! @brief Lecture à partir des ressources
  //!
  //! Cette méthode permet de lire un objet graphique dans les ressources
  //!
  //! @param[in]        hInstance Handle d'instance
  //! @param[in]        szResName Nom ou ID de la ressource
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL LoadFromResource(HINSTANCE hInstance, LPCTSTR szResName);

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
  //! @brief Enregistrement d'un fichier WMF
  //!
  //! Cette fonction crée et enregistre un fichier WMF. Si un fichier de même
  //! nom existe déjà, il est détruit.
  //!
  //! @param[in]        asFileName Nom du fichier à créer
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------
  virtual bool FASTCALL SaveToFile(const AnsiString asFileName);

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
  //! @brief Propriété Canvas
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TMetafile, TCanvas *, Canvas);


  //---------------------------------------------------------------------------
  //! @brief Propriété CreatedBy
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, AnsiString, CreatedBy);


  //---------------------------------------------------------------------------
  //! @brief Propriété Description: description du fichier
  //! 
  //! Cette propriété permet de lire la description enregistrée dans l'en-tête
	//! du fichier.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, AnsiString, Description);


  //---------------------------------------------------------------------------
  //! @brief Propriété Empty: Fichier vide
  //! 
  //! Cette propriété permet de déterminer si le fichier contient des
	//! informations ou pas.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, bool, Empty);


  //---------------------------------------------------------------------------
  //! @brief Propriété Enhanced: Format du fichier.
  //! 
  //! Cette propriété détermine le format du fichier metafile. Si cette
	//! propriété est à false, le format WMF (compatible avec les versions
	//! Windows 16 bits) sera enregistré. Sinon, le format EMF sera utilisé
	//! (version améliorée qui permet entre autres d'avoir des coordonnées sur 32
	//! bits). Actuellement, il est presque toujours préférable d'utiliser la
	//! version EMF.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, bool, Enhanced);


  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Handle du metafile.
  //! 
  //! Cette propriété permet d'avoir accès au handle de fichier metafile, pour
	//! une utilisation directe dans les API Windows, par exemple.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, HENHMETAFILE, Handle);


  //---------------------------------------------------------------------------
  //! @brief Propriété Inch: Nombre d'unités par pouce.
  //! 
  //! Cette propriété permet de lire ou de modifier le nombre d'unités par
	//! pouces, qui permet une mise à l'échelle du fichier. Elle est utilisée
	//! uniquement par les fichiers WMF, il est donc inutile de la définir
	//! lorsque la propriété @b Enhanced est égale à @b true.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, Word, Inch);


  //---------------------------------------------------------------------------
  //! @brief Propriété MMHeight: Hauteur du fichier.
  //! 
  //! Cette propriété indique la hauteur du fichier exprimée en centièmes de
	//! millimètres. Le rapport entre MMHeight (en centièmes de millimètres) et
	//! Height (en pixels) permet une mise à l'échelle correcte.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, int, MMHeight);


  //---------------------------------------------------------------------------
  //! @brief Propriété MMWidth: Largeur du fichier.
  //! 
  //! Cette propriété indique la largeur du fichier exprimée en centièmes de
	//! millimètres. Le rapport entre MMWidth (en centièmes de millimètres) et
	//! Width (en pixels) permet une mise à l'échelle correcte.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, int, MMWidth);


  //---------------------------------------------------------------------------
  //! @brief Propriété Transparent: Transparence du dessin
  //! 
  //! Cette propriété détermine si le fichier metafile est dessiné en laissant
	//! apparaitre le fond (ce qui permet de dessiner plusieurs métafiles
	//! superposés, par exemple). Sinon, avant le dessin, le fond sera rempli
	//! avec la couleur correspondante à la propriété @b Color.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TMetafile, bool, Transparent);


  //@}
};


#endif

#undef In_TMetafileH
