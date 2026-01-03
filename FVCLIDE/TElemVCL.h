//-----------------------------------------------------------------------------
//! @file TElemVCL.h
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



//---------------------------------------------------------------------------

#ifndef TElemVCLH
#define TElemVCLH

//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------

#include "TElemBase.h"


//---------------------------------------------------------------------------
//! @class TElemVCL
//!
//! @brief Classe des composants VCL
//!
//! @author Denis ROBERT
//!
//---------------------------------------------------------------------------

class TElemVCL: public TElemBase {
private:
	bool bDeleteComponent;
protected:
  TComponent *FComponent;
  bool FVisible;
  bool FShowHint;
  AnsiString FHint;
  bool FEnabled;
  TPosition FPosition;
  TBorderStyle FBorderStyle;
  AnsiString asActiveControl;
  AnsiString asOnMouseDown;
  AnsiString asOnMouseUp;
  AnsiString asOnMouseMove;
  AnsiString asOnClick;
  AnsiString asOnPaint;
  AnsiString asOnResize;
  AnsiString asOnClose;

  AnsiString UnTruc(AnsiString asLigne, int * Cur, char Delim);
public:


  //! @name Constructeurs / destructeurs
  //@{


  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
  TElemVCL(TElemBase *ElemParent, TComponent *Component);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
  virtual ~TElemVCL(void);

  //@}


  //! @name Méthodes
  //@{

	//---------------------------------------------------------------------------
  //! @brief Getter de la propriété ClassName
  //!
  //! Cette méthode permet de lire la propriété ClassName
  //!
  //! @return @c AnsiString Propriété ClassName
  //---------------------------------------------------------------------------

	AnsiString Get_ClassName(void);

	//---------------------------------------------------------------------------
  //! @brief Getter de la propriété Type
  //!
  //! Cette méthode permet de lire la propriété Type
  //!
  //! @return @c AnsiString Propriété Type
  //---------------------------------------------------------------------------

  virtual AnsiString Get_Type();

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Name
  //!
  //! Cette méthode permet de lire la propriété Name
  //!
  //! @return @c AnsiString Propriété Name
  //---------------------------------------------------------------------------

  virtual AnsiString Get_Name();

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Name
  //!
  //! @param[in]        NewName Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Name(AnsiString NewName);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Left
  //!
  //! Cette méthode permet de lire la propriété Left
  //!
  //! @return @c int Propriété Left
  //---------------------------------------------------------------------------

  virtual int Get_Left();

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Left
  //!
  //! @param[in]        NewLeft Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Left(int NewLeft);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Top
  //!
  //! Cette méthode permet de lire la propriété Top
  //!
  //! @return @c int Propriété Top
  //---------------------------------------------------------------------------

  virtual int Get_Top();

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Top
  //!
  //! @param[in]        NewTop Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Top(int NewTop);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Width
  //!
  //! Cette méthode permet de lire la propriété Width
  //!
  //! @return @c int Propriété Width
  //---------------------------------------------------------------------------

  virtual int Get_Width();

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Width
  //!
  //! @param[in]        NewWidth Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Width(int NewWidth);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Height
  //!
  //! Cette méthode permet de lire la propriété Height
  //!
  //! @return @c int Propriété Height
  //---------------------------------------------------------------------------

  virtual int Get_Height();

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Height
  //!
  //! @param[in]        NewHeight Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Height(int NewHeight);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété ClientRect
  //!
  //! Cette méthode permet de lire la propriété ClientRect
  //!
  //! @return @c TRect Propriété ClientRect
  //---------------------------------------------------------------------------

  virtual TRect Get_ClientRect();

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Anchors
  //!
  //! Cette méthode permet de lire la propriété Anchors
  //!
  //! @return @c TAnchors Propriété Anchors
  //---------------------------------------------------------------------------

  virtual TAnchors Get_Anchors();

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Anchors
  //!
  //! @param[in]        NewAnchors Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Anchors(TAnchors NewAnchors);

  //---------------------------------------------------------------------------
  //! @brief Accesseur de la propriété Parent
  //!
  //! @param[in]        NewParent Nouvelle valeur de la propriété
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Set_Parent(TElemBase * NewParent);

  //---------------------------------------------------------------------------
  //! @brief Régénération d'un objet
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual void Invalidate();

  //---------------------------------------------------------------------------
  //! @brief Met à jour la taille du TControlSizer
  //!
  //! Cette fonction met à jout les propriétés Left, Top, Width et Height de
	//! l'objet TControlSizer. Elle doit être appelée suite à un
	//! redimensionnement de l'objet parent qui peut entrainer une modification
	//! de ces données en fonction des ancrages et des alignements.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool UpdateControlSizer(void);

  //---------------------------------------------------------------------------
  //! @brief Mise à jour des informations
	//!
	//! Cette méthode met à jour les informations de l'objet suite à une
	//! modification des propriétés de base (nom, position, dimensions). Ces
	//! informations sont généralement affichées dans une info-bulle.
  //!
  //! @param[in]        asInfos Informations sous forme de chaîne.
  //---------------------------------------------------------------------------
	virtual bool UpdateInfos(AnsiString asInfos);

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //!
  //! @param[in]        asTypeACreer Type de l'objet à créer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Create(AnsiString asTypeACreer);

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet enfant
  //!
  //! @return @c bool Adresse du nouvel élément, ou NULL si échec
  //---------------------------------------------------------------------------
  virtual TElemBase *NewChild(void);

  //---------------------------------------------------------------------------
  //! @brief liste des propriétés
  //!
  //! @param[in]        ListeProperties Liste à remplir
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListeProperties);

  //---------------------------------------------------------------------------
  //! @brief lecture du type d'une propriété
  //!
  //! @param[in]        asProperty Propriété à lire
  //! @param[out]       asInfos Choix si type == tcChoix ou tcChoixMultiple
  //!
  //! @return @c TYPEPROPERTY Type de la propriété
  //---------------------------------------------------------------------------
  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Propriété
  //!
  //! @return @c std::string Valeur par défaut de la propriété
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief lecture d'une propriété
  //!
  //! @param[in]        asProperty Propriété à lire
  //!
  //! @return @c AnsiString Propriété lue
  //---------------------------------------------------------------------------
  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une propriété
  //!
  //! @param[in]        asProperty Propriété à écrire
  //! @param[out]       asValue Valeur de la propriété
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Component
  //!
  //! Cette méthode permet de lire la propriété Component
  //!
  //! @return @c TComponent *Propriété Component
  //---------------------------------------------------------------------------

  TComponent *GetComponent();

  //---------------------------------------------------------------------------
  //! @brief Visibilité du contrôle
  //!
  //! @param[in]        bVisible true si visible
  //!
  //! @return @c bool true si Ok (et en général, c'est Ok)
  //---------------------------------------------------------------------------
  virtual bool Show(bool bVisible);

  //---------------------------------------------------------------------------
  //! @brief Lecture du nom de la classe du contrôle
  //!
  //! @return @c AnsiString Nom de la classe
  //---------------------------------------------------------------------------
  AnsiString ClassName(void);

  //---------------------------------------------------------------------------
  //! @brief Test du nom de la classe du contrôle
  //!
  //! @param[in]        asClasse Nom de la classe
  //!
  //! @return @c true si égal
  //---------------------------------------------------------------------------
  bool ClassNameIs(AnsiString asClasse);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Component
	//!
	//! Cette propriété indique le composant VCL associé.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TElemVCL, TComponent *, Component);

  //@}
};


//---------------------------------------------------------------------------
#endif
