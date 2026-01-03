//-----------------------------------------------------------------------------
//! @file TFrame_FVCLIDEBase.h
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

#ifndef TFrame_FVCLIDEBaseH
#define TFrame_FVCLIDEBaseH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TFVCLIDEAutomation.h"

//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnFlagChanged.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnFlagChanged)(TObject *Sender, bool Changed);


//---------------------------------------------------------------------------
class TFrame_FVCLIDEBase : public TFrame {
private:
protected:
  AnsiString FFileName;
	FILETIME ftLastWriteTime;
  bool FChanged;
	TFVCLIDEFileType FFileType;

public:

  TFrame_FVCLIDEBase(TWinControl *AOwner);
  virtual ~TFrame_FVCLIDEBase(void);

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ouverture d'un fichier
  //!
  //! Cette méthode ouvre le fichier correspondant à la propriété FileName.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool Open(void);

  //---------------------------------------------------------------------------
  //! @brief Ouverture d'un fichier
  //!
  //! Cette méthode ouvre un fichier.
  //!
  //! @param[in]        asFileName Nom du fichier à ouvrir.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool OpenFile(AnsiString asFileName) = 0;

  //---------------------------------------------------------------------------
  //! @brief Enregistrement.
  //!
  //! Cette méthode enregistre les données dans le fichier correspondant à la
  //! propriété FileName.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool Save(void);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un fichier
  //!
  //! Cette méthode enregistre les données dans le fichier asFileName. Si le
  //! fichier existe déjà, il est écrasé sans confirmation de l'utilisateur.
  //!
  //! @param[in]        asFileName Nom du fichier à enregistrer.
  //!
  //! @return @c bool true si Ok.
  //!
  //! @note Attention à ne pas affecter la variable FFileName avant d'avoir
	//! réellement créé le fichier, sinon, si l'utilisateur passe à une autre
	//! application à ce moment, il aura un message "Une application extérieure
	//! a supprimé le fichier, voulez-vous... ".
  //---------------------------------------------------------------------------
	virtual bool SaveToFile(AnsiString asFileName) = 0;

  //---------------------------------------------------------------------------
  //! @brief Teste si un fichier a été modifié par un programme externe
  //!
  //! Cette méthode est appelée lorsque l'application reçoit la focalisation.
	//! Par défaut, elle compare la date du fichier avec celle mémorisée lors du
	//! chargement. Si elles sont différentes, c'est qu'un programme externe a
	//! modifié le fichier. Ne pas afficher de message dans cette fonction. La
	//! gestion du rechargement (question "voulez-vous recharger le fichier") est
	//! gérée par le programme principal.
  //!
  //! @return @c int 1 si le fichier a été modifié, 2 s'il a été supprimé. O sinon.
  //---------------------------------------------------------------------------
	virtual int TestExternalChange(void);

  //---------------------------------------------------------------------------
  //! @brief Mémorise la date du fichier
  //!
  //! Cette méthode mémorise la date de modification du fichier utilisée par la
	//! méthode TestExternalChange. Appelez cette méthode après une modification
	//! volontaire du fichier pour éviter la question "Le fichier a été modifié
	//! par un programme externe, voulez-vous le recharger ?".
  //!
  //! @return @c bool true si la date a été enregistrée.
  //---------------------------------------------------------------------------
	virtual bool MemoDateFile(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération undo
  //!
  //! Cette fonction effectue une opération undo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Undo(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette fonction effectue une opération redo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Redo(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Liste des trois premières modifications
  //!
  //! Cette méthode renvoie le libellé des premières modifications
  //!
  //! @return @c AnsiString Liste des modifications (séparées par un retour à
	//! la ligne)
  //---------------------------------------------------------------------------
  virtual AnsiString FASTCALL GetModificationsList(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Couper et copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CutToClipboard(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CopyToClipboard(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Coller depuis le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL PasteFromClipboard(void) = 0;

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété FileType
	//!
	//! Type du fichier fichier affiché dans l'onglet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TFrame_FVCLIDEBase, TFVCLIDEFileType, FileType);

  //---------------------------------------------------------------------------
  //! @brief Propriété DisplayedFileName
	//!
	//! Nom du fichier affiché dans l'onglet.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TFrame_FVCLIDEBase, AnsiString, DisplayedFileName);

  //---------------------------------------------------------------------------
  //! @brief Propriété FileName
	//!
	//! Nom du fichier.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_FVCLIDEBase, AnsiString, FileName);

  //---------------------------------------------------------------------------
  //! @brief Propriété Changed
	//!
	//! Cette propriété indique si des modifications ont été faites.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_FVCLIDEBase, bool, Changed);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque l'objet est modifié, soit par une
	//! action de l'utilisateur, soit par programme.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnFlagChanged
	//!
	//! Cet évènement est déclenché lorsque la propriété Changed change d'état.
	//! Ceci permet de mettre en évidence les fichiers modifiés.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nouvelle valeur du flag
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnFlagChanged, OnFlagChanged, TObject *, bool);

  //@}

};
//---------------------------------------------------------------------------
#endif
