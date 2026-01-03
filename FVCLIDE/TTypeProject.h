//-----------------------------------------------------------------------------
//! @file TTypeProject.h
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

#ifndef TTypeProjectH
#define TTypeProjectH

//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "..\Plugins\Include\Plugin.h"


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TTypeProject {
private:
protected:
	HINSTANCE hDll;

	AnsiString FName;
	PLUGIN_INIT Plugin_Init;
	PLUGIN_INSTALL Plugin_Install;
	PLUGIN_GETBITMAP20 Plugin_GetBitmap20;
	PLUGIN_GETBITMAP32 Plugin_GetBitmap32;
	PLUGIN_GETBITMAP48 Plugin_GetBitmap48;
	PLUGIN_GETDESCRIPTION Plugin_GetDescription;
	PLUGIN_CREATE Plugin_Create;
	PLUGIN_OPEN Plugin_Open;
	PLUGIN_GETINPUTS Plugin_GetInputs;
	PLUGIN_FREE_GETINPUTS Plugin_Free_GetInputs;
	PLUGIN_GETOUTPUT Plugin_GetOutput;
	PLUGIN_FREE_GETOUTPUT Plugin_Free_GetOutput;
	PLUGIN_ADDFILE Plugin_AddFile;
	PLUGIN_REMOVEFILE Plugin_RemoveFile;
	PLUGIN_RENAMEFILE Plugin_RenameFile;
	PLUGIN_PRECOMPIL Plugin_PreCompil;
	PLUGIN_BUILD Plugin_Build;
	PLUGIN_RUN Plugin_Run;
	PLUGIN_STARTDEBUG Plugin_StartDebug;
	PLUGIN_STOPDEBUG Plugin_StopDebug;
	PLUGIN_STEPBYSTEP Plugin_StepByStep;
	PLUGIN_ADDBREAKPOINT Plugin_AddBreakpoint;
	PLUGIN_DELBREAKPOINT Plugin_DelBreakpoint;
	PLUGIN_CLEARBREAKPOINTS Plugin_ClearBreakpoints;
	PLUGIN_GETCALLSTACK Plugin_GetCallStack;
	PLUGIN_GETVARIABLEVALUE Plugin_GetVariableValue;

public:

	//! @name Constructeurs et destructeur
  //@{

	//---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTypeProject(AnsiString asName);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTypeProject(void);

  //@}


  //! @name Méthodes
  //@{

	bool Install(void);
	HBITMAP GetBitmap20(void);
	HBITMAP GetBitmap32(void);
	HBITMAP GetBitmap48(void);
	AnsiString GetDescription(void);
	bool Create(AnsiString *asProjectFileName, AnsiString *asProjectContent);
	bool Open(AnsiString *asProjectFileName, AnsiString *asProjectContent);
	bool GetInputs(AnsiString asProjectFileName, AnsiString asProjectContent, TStringList *asInputFileNames);
	bool GetOutput(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString *asOutputFileName);
	bool Can_AddFile(void);
	bool AddFile(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString asFileName);
	bool Can_RemoveFile(void);
	bool RemoveFile(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString asFileName);
	bool Can_RenameFile(void);
	bool RenameFile(AnsiString asProjectFileName, AnsiString asProjectContent, AnsiString asOldFileName, AnsiString asNewFileName);
	bool Can_PreCompil(void);
	bool PreCompil(AnsiString asProjectFileName, AnsiString asProjectContent);
	bool Build(AnsiString asProjectFileName, AnsiString asProjectContent);
	bool Can_Run(void);
	bool Run(AnsiString asProjectFileName, AnsiString asProjectContent);
	bool Can_Debug(void);
	bool StartDebug(AnsiString asProjectFileName, AnsiString asProjectContent);
	bool StopDebug(void);
	bool StepByStep(TYPESTEPBYSTEP TypeStepByStep, AnsiString *asFileName, int *NumLine1, int *NumCol1, int *NumLine2, int *NumCol2);
	bool AddBreakpoint(AnsiString asFileName, int NumLine, int NumCol);
	bool DelBreakpoint(AnsiString asFileName, int NumLine, int NumCol);
	bool ClearBreakpoints(void);
	bool GetCallStack(AnsiString *asCallStack);
	bool GetVariableValue(AnsiString asVarName, AnsiString *asVarValue);
	  
  //@}


  //! @name Propriétés
  //@{
	  
  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété permet de lire le type de projet choisi par
	//! l'utilisateur.
  //---------------------------------------------------------------------------

	DECLARE_PROPERTY_GET(TTypeProject, AnsiString, Name);


  //@}

};

//---------------------------------------------------------------------------
#endif
