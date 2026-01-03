//-----------------------------------------------------------------------------
//! @file TPDFFile.h
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

#ifdef In_TPDFFileH
#error "Inclusion circulaire de TPDFFile"
#endif // In_TPDFFileH

#define In_TPDFFileH

#ifndef TPDFFileH
#define TPDFFileH

#include <vector>

#include "TObjetPdf.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TObjetPdfCatalog;
class TObjetPdfOutlines;
class TObjetPdfPages;
class TObjetPdfPage;
class TObjetPdfContent;
class TObjetPdfProcSet;
class TObjetPdfFont;
class TCanvasPdf;


//---------------------------------------------------------------------------
//! @class TPDFFile
//!
//! @brief Fichier PDF
//!
//! Cette classe (@c TPDFFile) représente un fichier PDF. Elle encapsule tous
//! les objets PDF créés lors du dessin (ou lus dans un fichier).
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TPDFFile: public TComponent {
private:

protected:
  std::vector<TObjetPdf *> ObjetsPdf;
  TCanvasPdf *FCanvas;
  TObjetPdfCatalog *ObjetPdfCatalog;
  TObjetPdfOutlines *ObjetPdfOutlines;
  TObjetPdfPages *ObjetPdfPages;
  TObjetPdfPage *ObjetPdfPage;
  TObjetPdfFont *ObjetPdfFont;
  TObjetPdfProcSet *ObjetPdfProcSet;
  TObjetPdfContent *ObjetPdfContent;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TPDFFile(TComponent *AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPDFFile(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Crée une nouvelle page PDF
  //!
  //! Cette fonction crée une nouvelle page PDF
  //!
  //! @param[in]        Width Largeur de la page en points
  //! @param[in]        Height Hauteur de la page en points
  //---------------------------------------------------------------------------

  void AddPage(int Width, int Height);

  //---------------------------------------------------------------------------
  //! @brief Crée un nouvel objet PDF
  //!
  //! Cette fonction crée un objet PDF en fonction de son type.
  //!
  //! @param[in]        csType Type de l'objet à créer
  //!
  //! @return @c TObjetPdf * Adresse de l'objet créé. NULL si échec.
  //---------------------------------------------------------------------------

  TObjetPdf * CreatePdfObject(CharString csType);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un fichier PDF
  //!
  //! Cette fonction lit un fichier PDF. Si un fichier de même
  //! nom existe déjà, il est détruit.
  //!
  //! @param[in]        asFileName Nom du fichier à créer
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------
  bool FASTCALL LoadFromFile(const AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un fichier PDF
  //!
  //! Cette fonction crée et enregistre un fichier PDF. Si un fichier de même
  //! nom existe déjà, il est détruit.
  //!
  //! @param[in]        asFileName Nom du fichier à créer
  //!
  //! @return @c bool true si l'opération a réussi.
  //---------------------------------------------------------------------------
  bool FASTCALL SaveToFile(const AnsiString asFileName);

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

  DECLARE_PROPERTY_GET(TPDFFile, TCanvasPdf *, Canvas);

  //@}
};


#endif

#undef In_TPDFFileH
