//-----------------------------------------------------------------------------
//! @file TDirectoryListBox.h
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

#ifdef In_TDirectoryListBoxH
#error "Inclusion circulaire de TDirectoryListBox"
#endif // In_TDirectoryListBoxH

#define In_TDirectoryListBoxH

#ifndef TDirectoryListBoxH
#define TDirectoryListBoxH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TTreeView.h"


//---------------------------------------------------------------------------
// Pré-déclarations
//---------------------------------------------------------------------------

class TLabel;
class TListBox;


//-----------------------------------------------------------------------------
//! @class TDirectoryListBox
//!
//! @brief Contrôle de sélection d'un répertoire.
//!
//! Cette classe permet d'afficher l'arborescence des répertoires d'un disque
//! ou du réseau. Malgré son nom, ce contrôle n'est pas une list-box mais
//! dérive de l'objet TreeView.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDirectoryListBox: public TTreeView {
private:
	TImageList *FilesImages;
	bool SetRootDirectory(AnsiString Directory);
	bool BuildTreeView(TTreeNode *TreeNodeParent, TTreeNode *TreeNode, AnsiString asPath, AnsiString asName, int State);
protected:
  bool FCaseSensitive;
  AnsiString FDirectory;
  TLabel *FDirLabel;
  AnsiString FDrive;
  TListBox *FFileList;
  bool FPreserveCase;
  bool FShowFiles;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages système
  //---------------------------------------------------------------------------

public:
  FVCL_BEGIN_EVENTS
  void FASTCALL NodeExpanded(TObject *Sender, TTreeNode *TreeNode);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_2(TDirectoryListBox, TOnExpanded, OnExpanded, NodeExpanded, TObject*, TTreeNode *);
  }

  FVCL_END_EVENTS


	//---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDirectoryListBox(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDirectoryListBox(void);

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Mise en forme d'un nom de répertoire en vue de l'affichage
  //!
  //! Cette méthode met en forme un nom de répertoire (minuscules et
	//! majuscules) en fonction de la la propriété CaseSensitive.
  //!
  //! @param[in]        S Nom du répertoire
  //!
  //! @return @c AnsiString Chaîne mise en forme
	//!
	//! @sa CaseSensitive
  //---------------------------------------------------------------------------
	AnsiString FASTCALL DisplayCase(const AnsiString S);

  //---------------------------------------------------------------------------
  //! @brief Comparaison de deux chaînes
  //!
  //! Cette méthode compare deux chaînes en fonction de la la propriété
	//! CaseSensitive.
  //!
  //! @param[in]        A Première chaîne à comparer
  //! @param[in]        B Seconde chaîne à comparer
  //!
  //! @return @c int Comme AnsiCompareIC
  //---------------------------------------------------------------------------
	int FASTCALL FileCompareText(const AnsiString A, const AnsiString B);

  //---------------------------------------------------------------------------
  //! @brief Nom complet du répertoire d'un noeud
  //!
  //! Cette méthode renvoie le nom complet (disque + chemin + nom) d'un
	//! répertoire d'un noeud en fonction de son index.
  //!
  //! @param[in]        Index Index de l'élément à lire
  //!
  //! @return @c AnsiString Nom du répetoire
  //---------------------------------------------------------------------------
	AnsiString FASTCALL GetItemPath(int Index);

  //---------------------------------------------------------------------------
  //! @brief Nom complet du répertoire d'un noeud
  //!
  //! Cette méthode renvoie le nom complet (disque + chemin + nom) d'un
	//! répertoire d'un noeud.
  //!
  //! @param[in]        TreeNode Élément à lire
  //!
  //! @return @c AnsiString Nom du répetoire
  //---------------------------------------------------------------------------
	AnsiString FASTCALL GetItemPath(TTreeNode *TreeNode);

  //---------------------------------------------------------------------------
  //! @brief Renvoie @b true si le noeud sélectionné est un fichier
  //!
  //! Cette méthode renvoie @b true si le noeud sélectionné est un fichier, et
	//! @b false si c'est un répertoire.
  //!
  //! @param[in]        TreeNode Élément à lire
  //!
  //! @return @c bool @b true si fichier
  //---------------------------------------------------------------------------
	bool FASTCALL IsFile(TTreeNode *TreeNode);

  //---------------------------------------------------------------------------
  //! @brief Déroule l'élément courant 
  //!
  //! Cette méthode ouvre (déroule) l'élément courant comme si l'utilisateur
	//! avait cliqué sur l'icône d'ouverture (signe "+").
  //---------------------------------------------------------------------------
	void FASTCALL OpenCurrent(void);

  //---------------------------------------------------------------------------
  //! @brief Met à jour la liste des répertoires
  //!
  //! Cette méthode rafraichit la liste des répertoires, par exemple après une
	//! création ou une suppression.
  //!
  //! @return @c bool True si Ok
  //---------------------------------------------------------------------------
	bool virtual FASTCALL Update(void);

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
  //! @param[in]        Sender Boîte de dialogue (utilisé par les événement)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété CaseSensitive
	//!
	//! Cette propriété permet de savoir si le système de fichiers est sensible
	//! à la casse ou pas. En général, les systèmes Windows ne font pas de
	//! différence entre le fichier "TOTO" et "Toto". Cette propriété renvoie
	//! donc @b false dans ce cas. Cependant, même s'il ne fait pas de
	//! différence, il se peut que le nom soit mémorisé sous la forme avec
	//! laquelle il a été enregistré. Consultez la propriété @b PreserveCase pour
	//! savoir si le système est dans ce cas.
	//!
	//! @sa PreserveCase
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TDirectoryListBox, bool, CaseSensitive);


  //---------------------------------------------------------------------------
  //! @brief Propriété Directory
	//!
	//! Cette propriété permet de lire ou d'affecter le nom de répertoire racine
	//! du tree-view. Les répertoires (et éventuellement les fichiers) affichés
	//! représentent le contenu de ce répertoire.
	//! @note Le caractère séparateur de fin (slash ou antislash) est
	//! automatiquement supprimé s'il est présent lors de l'affectation de cette
	//! propriété.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDirectoryListBox, AnsiString, Directory);


  //---------------------------------------------------------------------------
  //! @brief Propriété DirLabel
	//!
	//! Cette propriété permet de déterminer le contrôle @b TLabel contenant le
	//! nom du répertoire sélectionné par l'utilisateur. Lorsque cette propriété
	//! est définie, le contrôle correspondant est automatiquement mis à jour
	//! lorsque l'utilisateur clique un répertoire dans le tree-view, et
	//! inversement le répertoire sélectioné est automatiquement mis à jour
	//! lorsque l'utilisateur change le contenu du label.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDirectoryListBox, TLabel *, DirLabel);


  //---------------------------------------------------------------------------
  //! @brief Propriété Drive
	//!
	//! Cette propriété permet de lire le nom du disque du répertoire choisi par
	//! l'utilisateur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDirectoryListBox, AnsiString, Drive);


  //---------------------------------------------------------------------------
  //! @brief Propriété FileList
	//!
	//! Cette propriété permet de déterminer le contrôle @b TListBox contenant
	//! les noms de fichiers contenus dans le répertoire sélectioné. Lorsque
	//! cette propriété est définie, le contrôle correspondant est
	//! automatiquement mis à jour lorsque l'utilisateur clique un répertoire
	//! dans le tree-view.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDirectoryListBox, TListBox *, FileList);


  //---------------------------------------------------------------------------
  //! @brief Propriété PreserveCase
	//!
	//! Cette propriété permet de savoir si le système de fichiers mémorise la
	//! casse (minuscules / majuscules) des fichiers et des répertoires. En
	//! général, actuellement, tous les systèmes de fichiers le permettent. Cette
	//! propriété renvoie donc toujours @b true;
	//!
	//! @sa CaseSensitive
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TDirectoryListBox, bool, PreserveCase);

  //---------------------------------------------------------------------------
  //! @brief Propriété ShowFiles
	//!
	//! Cette propriété détermine si les fichiers sont affichés dans le tree-view
	//! (si la propriété est à true). Dans le cas contraire, seuls les
	//! répertoires sont affichés.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDirectoryListBox, bool, ShowFiles);


  //@}


  //! @name Evènements
  //@{


  //@}


};


#else  // TDirectoryListBoxH

class TDirectoryListBox;

#endif  // TDirectoryListBoxH


#undef In_TDirectoryListBoxH
