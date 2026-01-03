//-----------------------------------------------------------------------------
//! @file TFVCLIDEAutomation.h
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

#ifndef TFVCLIDEAutomationH
#define TFVCLIDEAutomationH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------
enum TTypeLog {
	tl_Search,                    //!< Onglet "Recherche"
	tl_Bookmarks,                 //!< Onglet "Marque-pages"
	tl_Build,                     //!< Onglet "Construction"
	tl_Process,                   //!< Onglet "Exécution"
	tl_Breakpoints,               //!< Onglet "Points d'arrêt"
	tl_CallStack                  //!< Onglet "Pile d'appel"
};

enum TFVCLIDEFileType {
	ftf_Other,                     //!< Type autre
	ftf_TextFile,                  //!< Fichier texte
	ftf_HexaFile,                  //!< Fichier binaire
	ftf_Dialog,                    //!< Boîte de dialogue
	ftf_Bmp,                       //!< Fichier image (BMP ou autre)
	ftf_Icon,                      //!< Fichier icône
	ftf_Cursor                     //!< Fichier curseur
};

enum TTypeDisplay {
	td_Position,                   //!< Display de type position
	td_Message 	                   //!< Display de type message
};


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TTextEditor;
class THexaEditor;
class TTypeProject;
class TFVCLIDEAutomation;


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------
extern TFVCLIDEAutomation *FVCLIDEAutomation;


//---------------------------------------------------------------------------
//! @class TFVCLIDEAutomation
//!
//! @brief Classe de base permettant de contrôler le programme FVCLIDE. Cette
//! classe est abstraite, les classes (en fait LA classe) dérivée(s) doivent
//! implémenter les méthodes virtuelles.
//!
//! @author Denis ROBERT
//!
//---------------------------------------------------------------------------

class TFVCLIDEAutomation {
private:
protected:
public:


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
  TFVCLIDEAutomation(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
  virtual ~TFVCLIDEAutomation(void);

  //@}


  //! @name Méthodes
  //@{

  virtual bool FASTCALL NewWorkSpace(void) = 0;
  virtual bool FASTCALL OpenWorkSpace(const AnsiString &asFileName) = 0;
  virtual bool FASTCALL SaveWorkSpace(void) = 0;
  virtual bool FASTCALL CloseWorkSpace(void) = 0;

	virtual bool FASTCALL AddExistingFile(const AnsiString &asNodeName, const AnsiString &asFileName) = 0;
	virtual bool FASTCALL AddExistingProject(const AnsiString &asNodeName, const AnsiString &asProjectFileName, const AnsiString &asProjectName, const AnsiString &asProjectType) = 0;

	virtual bool FASTCALL OpenFile(const AnsiString &asFileName) = 0;
	virtual bool FASTCALL OpenTextFile(const AnsiString &asFileName) = 0;
	virtual bool FASTCALL OpenBinFile(const AnsiString &asFileName) = 0;
  virtual bool FASTCALL OpenDialog(const AnsiString &asFileName) = 0;
  virtual bool FASTCALL OpenIcon(const AnsiString &asFileName) = 0;
  virtual bool FASTCALL OpenCursor(const AnsiString &asFileName) = 0;
  virtual bool FASTCALL SaveFile(int Index) = 0;

	virtual int FASTCALL GetNbFiles(void) = 0;
  virtual void FASTCALL GetListFiles(TStrings *ListFiles) = 0;
  virtual TFVCLIDEFileType FASTCALL GetFileType(int Index) = 0;
  virtual AnsiString FASTCALL GetFileName(int Index) = 0;
  virtual int FASTCALL GetFileIndex(const AnsiString asFileName, TFVCLIDEFileType FileType) = 0;
  virtual TTextEditor * FASTCALL GetTextFile(int Index) = 0;
  virtual THexaEditor * FASTCALL GetHexaFile(int Index) = 0;
  virtual TTextEditor * FASTCALL GetCurTextFile(void) = 0;
  virtual THexaEditor * FASTCALL GetCurHexaFile(void) = 0;
  virtual int FASTCALL GetCurFileIndex(void) = 0;
  virtual bool FASTCALL ActivateFile(int Index) = 0;
  virtual bool FASTCALL SetFocusOnEditor(void) = 0;

	virtual AnsiString FASTCALL GetSelText(void) = 0;
	virtual bool FASTCALL SetSelText(AnsiString asText) = 0;

	virtual bool FASTCALL AddBookmark(void) = 0;
	virtual bool FASTCALL DelBookmark(void) = 0;
	virtual bool FASTCALL AddBreakpoint(void) = 0;
	virtual bool FASTCALL DelBreakpoint(void) = 0;

	virtual void FASTCALL DisplayMessage(TTypeDisplay TypeDisplay, const AnsiString &asMessage) = 0;
	virtual int FASTCALL DisplayError(const AnsiString &asMessage, const AnsiString &asTitle, const AnsiString &asKey, int Flags) = 0;
	virtual int FASTCALL DisplaySystemError(const AnsiString &asMessage, int SystemError, const AnsiString &asTitle, const AnsiString &asKey, int Flags) = 0;

	virtual void FASTCALL ClearLogMessage(TTypeLog TypeLog) = 0;
	virtual void FASTCALL AddLogMessage(TTypeLog TypeLog, const AnsiString &asMessage) = 0;
	virtual void FASTCALL DeleteLogMessage(TTypeLog TypeLog, int i) = 0;
	virtual AnsiString FASTCALL GetLogMessage(TTypeLog TypeLog, int i) = 0;
	virtual int FASTCALL GetCountLogMessage(TTypeLog TypeLog) = 0;

	virtual bool FASTCALL AddVariableDefinition(AnsiString asVarName, int TypeDefinition, AnsiString asDescription, AnsiString asTypeContenant, AnsiString asTypeVariable, AnsiString asFileName, int NumLine, int NumCol) = 0;
	virtual bool FASTCALL FindDeclaration(AnsiString asVarName, AnsiString *asFileName, int *NumLine, int *NumCol) = 0;
	virtual bool FASTCALL FindImplementation(AnsiString asVarName, AnsiString *asFileName, int *NumLine, int *NumCol) = 0;
	virtual bool FASTCALL GetDescription(AnsiString asVarName, AnsiString *asDescription) = 0;
	virtual bool FASTCALL GetTypeVariable(AnsiString asVarName, AnsiString *asTypeVariable) = 0;
	virtual bool FASTCALL GetListAutocompletion(AnsiString asTypeContenant, TStrings *slList) = 0;

	virtual bool FASTCALL ReplaceDeclarationsFVCL(AnsiString asFileNameH, AnsiString asClassName, AnsiString asComponents, AnsiString asEvents) = 0;
	virtual bool FASTCALL SearchOrCreateCodeEvent(AnsiString asFileNameCpp, AnsiString asClassName, AnsiString asFunctionEvent, AnsiString asArgs) = 0;

  //@}


  //! @name Propriétés
  //@{

  //@}
};


//---------------------------------------------------------------------------
#endif
