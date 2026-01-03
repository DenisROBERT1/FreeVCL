//-----------------------------------------------------------------------------
//! @file TFileList.h
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

#ifdef In_TFileListH
#error "Inclusion circulaire de TFileList"
#endif // In_TFileListH

#define In_TFileListH

#ifndef TFileListH
#define TFileListH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TComponentH
#include "TComponent.h"
#endif  // TComponentH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TStringList;
class TMask;


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TTypeList {
	tlFiles,                      //!< Affichage des fichiers
	tlDirectories                 //!< Affichage des répertoires
};

typedef Set<TTypeList, tlFiles, tlDirectories> TTypeLists;  //!< Combinaison de plusieurs TTypeList

//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDirectory.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDirectory)(TObject *Sender, AnsiString asDirectory);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnFileFound.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnFileFound)(TObject *Sender, AnsiString asFileName);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnFileFiltered.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnFileFiltered)(TObject *Sender, AnsiString asFileName);


//-----------------------------------------------------------------------------
//! @class TFileList
//!
//! @brief Définition de la classe TFileList
//!
//! Cette classe permet de parcourir les fichiers d'un répertoire, de les
//! lister ou de faire une action dessus.@n
//! Renseignez les propriétés en fonction de vos critères de recherche, puis
//! lancez la méthode GetFileList pour avoir la liste des fichiers d'un
//! répertoire, ou bien utilisez les évènements pour traiter chaque fichier
//! individuellement.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TFileList: public TComponent {
protected:
  AnsiString FFilter;
  bool FRecursive;
	TTypeLists FTypes;
  TMask *Mask;
  bool bAbort;

  bool ParseFiles(AnsiString asPathSrc);

public:

  //! @name Constructeur et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFileList(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TFileList(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Liste de fichiers d'un répertoire
  //!
  //! Cette méthode liste les fichiers d'un répertoire. Le résultat peut être
  //! mis dans une liste de strings, ou sinon utilisez les évènements pour
  //! traiter chaque fichier séparément.
  //!
  //! @param[in]        Directory Non du répertoire à lister
  //! @param[in]        FullName Si true: nom du fichier avec le chemin. Sinon, nom et extension seulement.
  //! @param[out]       FileList Liste des fichiers (optionnel)
  //!
  //! @return @c int Nombre de fichiers listés
  //---------------------------------------------------------------------------

  int GetFileList(AnsiString Directory, bool FullName = true, TStrings *FileList = NULL);

  //---------------------------------------------------------------------------
  //! @brief Interruption du traitement de fichiers
  //!
  //! Cette méthode interromp le déroulement de la méthode GetListFiles.
  //---------------------------------------------------------------------------

  void Abort(void);

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
  //! @brief Propriété Filter
	//!
	//! Cette propriété permet de filtrer les fichier à lister. Cette méthode
	//! utilise un objet TMask. La syntaxe est donc la même.
	//!
	//! @sa TMask
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFileList, AnsiString, Filter);


  //---------------------------------------------------------------------------
  //! @brief Propriété Recursive
	//!
	//! Cette propriété permet de parcourir les sous-répertoires des répertoires
	//! explorés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFileList, bool, Recursive);

  //---------------------------------------------------------------------------
  //! @brief Propriété Type
	//!
	//! Cette propriété permet de sélectionner le type des données à lister
	//! (fichiers, répertoires ou les deux).
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY(TFileList, TTypeLists, Types);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDirectory: traitement d'un répertoire.
	//!
	//! Cet évènement est déclenché lorsque la méthode @b GetFileList commence le
	//! traitement d'un répertoire. Vous pouver intercepter cet évènement pour
	//! effectuer un traitement initial sur chaque répertoire.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nom du répertoire à traiter
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnDirectory, OnDirectory, TObject *, AnsiString);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnFileFound: fichier trouvé.
	//!
	//! Cet évènement est déclenché lorsque la méthode @b GetFileList a trouvé un
	//! fichier. Vous pouver intercepter cet évènement pour effectuer un
	//! traitement sur chaque fichier, notamment lorsque le paramètre @b FileList
	//! est à NULL.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nom du fichier à traiter
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnFileFound, OnFileFound, TObject *, AnsiString);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnFileFiltered: fichier filtré.
	//!
	//! Cet évènement est déclenché lorsque la méthode @b GetFileList a trouvé un
	//! fichier, mais que celui-ci n'a pas les critères défini par le filtre.
	//! Vous pouver intercepter cet évènement pour effectuer un traitement sur
	//! les fichiers non listés.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nom du fichier non traité
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnFileFiltered, OnFileFiltered, TObject *, AnsiString);

  //@}

};


#else  // TFileListH

class TFileList;

#endif  // TFileListH

#undef In_TFileListH
