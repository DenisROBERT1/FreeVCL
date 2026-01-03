//-----------------------------------------------------------------------------
//! @file TProject.h
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


#ifndef TProjectH
#define TProjectH


//---------------------------------------------------------------------------
// Pré-déclarations
//---------------------------------------------------------------------------

enum TYPESTEPBYSTEP;
class TListTypeProjects;


//---------------------------------------------------------------------------
// Structures, macros, enums
//---------------------------------------------------------------------------

typedef struct {
	AnsiString asVarName;								 //!< Nom de la variable ou de la fonction
	int TypeDefinition;								   //!< Type de définition (1 = déclaration, 2 = implémentation)
	AnsiString asDescription;						 //!< Description de la variable ou de la fonction (qui apparaitra dans l'info-bulle)
	AnsiString asTypeContenant;					 //!< Type de la classe ou de la structure contenant la variable
	AnsiString asTypeVariable;					 //!< Type de la variable ou de la fonction
	int IndexProgram;					           //!< Index dans la liste des programmes
  int NumLine;                         //!< Ligne du programme
  int NumCol;                          //!< Colonne du programme
} DEF_VARIABLE;

//---------------------------------------------------------------------------
//! @class TProject
//!
//! @brief Projet FreeVCL
//!
//! Cette classe (@c TProject) encapsule un projet FreeVCL.
//---------------------------------------------------------------------------

class TProject: public TObject {
private:
	TListTypeProjects *ListTypeProjects;
	bool bIsPreCompiled;
protected:
	TDateTime FBuilt;
	AnsiString FContent;
	AnsiString FFileName;
	int FIndexTypeProject;
	TDateTime FLastChange;
	AnsiString FName;
	AnsiString FOutput;
	AnsiString FTypeProject;

	// Recherche de définitions
	TStringList ListPrograms;
	std::vector<DEF_VARIABLE> ListVariables;

public:

  //! @name Constructeur / destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
  TProject(TListTypeProjects *AListTypeProjects);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
  ~TProject(void);

  //@}


  //! @name Méthodes
  //@{

	TBitmap *GetBitmap20(void);
	TBitmap *GetBitmap32(void);
	TBitmap *GetBitmap48(void);
	AnsiString GetDescription(void);
	bool Create(void);
	bool Open(void);
	bool GetInputs(TStringList *asInputFileNames);
	bool GetOutput(AnsiString *asOutputFileName);
	bool Can_AddFile(void);
	bool AddFile(AnsiString asFileName);
	bool Can_RemoveFile(void);
	bool RemoveFile(AnsiString asFileName);
	bool Can_RenameFile(void);
	bool RenameFile(AnsiString asOldFileName, AnsiString asNewFileName);
	bool Build(void);
	bool Can_Run(void);
	bool Run(void);
	bool Can_Debug(void);
	bool StartDebug(void);
	bool StopDebug(void);
	bool StepByStep(TYPESTEPBYSTEP TypeStepByStep, AnsiString *asFileName, int *NumLine1, int *NumCol1, int *NumLine2, int *NumCol2);
	bool AddBreakpoint(AnsiString asFileName, int NumLine, int NumCol);
	bool DelBreakpoint(AnsiString asFileName, int NumLine, int NumCol);
	bool ClearBreakpoints(void);
	bool GetCallStack(AnsiString *asCallStack);
	bool GetVariableValue(AnsiString asVarName, AnsiString *asVarValue);

	bool Can_PreCompil(void);
	bool IsPreCompiled(void);
	bool PreCompilation(void);
	bool AddVariableDefinition(AnsiString asVarName, int TypeDefinition, AnsiString asDescription, AnsiString asTypeContenant, AnsiString asTypeVariable, AnsiString asFileName, int NumLine, int NumCol);
	bool GetVariableDefinition(AnsiString asVarName, int TypeDefinition, AnsiString *asFileName, int *NumLine, int *NumCol);
	bool GetVariableDescription(AnsiString asVarName, AnsiString *asDescription);
	bool FASTCALL GetTypeVariable(AnsiString asVarName, AnsiString *asTypeVariable);
	bool GetListAutocompletion(AnsiString asTypeContenant, TStrings *slList);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Built
 	//!
	//! Cette propriété renvoie la date de dernière construction du projet. Si le
	//! projet est un fichier, cette date est égale à la date de dernière
	//! modification.
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProject, TDateTime, Built);

  //---------------------------------------------------------------------------
  //! @brief Propriété Content
 	//!
	//! Cette propriété permet de mémoriser le contenu du projet. Si cette
	//! propriété est vide, alors ça signifie que le contenu est mémorisé dans un
	//! fichier (et la propriété FileName ne devrait pas être vide).
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProject, AnsiString, Content);

  //---------------------------------------------------------------------------
  //! @brief Propriété FileName
 	//!
	//! Cette propriété permet de mémoriser et de définir le nom du fichier
	//! contenant le projet. Si la valeur retournée est vide, alors le projet
	//! est encapsulé dans le fichier workspace. Elle peut être lue et modifiée
	//! avec la propriété Content.
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProject, AnsiString, FileName);

  //---------------------------------------------------------------------------
  //! @brief Propriété IndexTypeProject
 	//!
	//! Cette propriété permet de lire le numéro de type de projet. Les simples
	//! fichiers ont le numéro de type -1.
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TProject, int, IndexTypeProject);

  //---------------------------------------------------------------------------
  //! @brief Propriété LastChange
 	//!
	//! Cette propriété renvoie la date de dernière modification du projet (et
	//! du projet uniquement, le fichier output n'est pas concerné).
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProject, TDateTime, LastChange);

  //---------------------------------------------------------------------------
  //! @brief Propriété Name
 	//!
	//! Cette propriété permet de mémoriser le nom du projet.
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProject, AnsiString, Name);

  //---------------------------------------------------------------------------
  //! @brief Propriété Output
 	//!
	//! Cette propriété permet de mémoriser le nom du fichier de sortie. Si
	//! le projet est un simple fichier, le nom du fichier de sortie est le
	//! nom du fichier lui-même. Sinon, tout projet doit avoir un fichier de
	//! sortie pour détecter s'il doit être recompilé ou pas.
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProject, AnsiString, Output);

  //---------------------------------------------------------------------------
  //! @brief Propriété IndexTypeProject
 	//!
	//! Cette propriété permet de lire et de définir le type de projet. Les
	//! simples fichiers ont le type "File".
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TProject, AnsiString, TypeProject);

  //@}


  //! @name Evènements
  //@{


  //@}

};

#endif
