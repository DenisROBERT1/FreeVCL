//-----------------------------------------------------------------------------
//! @file TConfig.h
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

#ifndef TConfigH
#define TConfigH

//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------
enum TEndOfLine {
	eol_NULL,                     //!< Non défini
	eol_CR,                       //!< Fin de ligne de type UNIX
	eol_CRLF                      //!< Fin de ligne de type Windows
};


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <map>
#include <TTextEditor.h>


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

typedef struct {
	AnsiString Name;
	AnsiString Prog;
	AnsiString Args;
	AnsiString InitDir;
} USERTOOL;


//---------------------------------------------------------------------------
class TConfig {
private:
  bool bModifs;

protected:
	// Install
	AnsiString FInstallPath;

	// MainWindow
  int FEdiLeft;
  int FEdiTop;
  int FEdiWidth;
  int FEdiHeight;
  bool FEdiFullScreen;
  int FLeftPaneWidth;
  int FBottomPaneHeight;

	// Files\LastFiles
  TStringList *FLastWorkSpaces;
  TStringList *FLastProjects;
  TStringList *FLastFiles;
  TStringList *FLastDialogs;
  TStringList *FLastGraphics;

	// Files\OpenFiles
  AnsiString FOpenWorkSpace;
  int FActivePageIndex;

	// Files
  TEndOfLine FEndOfLine;

	// Editor
	int FNbSpacesTab;
  bool FSpacesToTabs;
  bool FTabsToSpaces;
  bool FTypeAuto;
  bool FIndentAuto;
  bool FCurAfterEndLine;
  bool FUndoRedoMoveCur;
  bool FLinesNumbers;
  bool FIconsColumn;
  bool FMultiLineTabs;
  bool FSaveBeforeBuild;
  bool FAutocompletion;
  bool FAlphaBlend;
  bool FEndLineAcc;
  bool FSpacesAfterPar;


  // Editor\Colors
	COLORPARAM FColorParams[MAX_TYPESYNTAX + 1];  // tsNormal = 0 à tsIconsColumn
	TFont * FFont;

  // Dialogs
  bool FAlignGrid;
	bool FShowGrid;
  int FGridX;
  int FGridY;

  // Search and replace
  TStringList *FSearchStrings;
  TStringList *FReplaceStrings;
  bool FWholeWord;
  bool FCaseSensitive;
  bool FBegLine;
  bool FEndLine;
  bool FOnlyInComments;
  bool FNotInComments;
  bool FOnlyInQuotes;
  bool FNotInQuotes;
  bool FRegExp;
  bool FKeepCase;
  bool FLoop;
	int FSearchIn;
  bool FRecurse;
	AnsiString FSearchDirectory;
	AnsiString FSearchExtensions;

	// ShortCuts
	std::map<AnsiString, TShortCut> ShortCuts;

  // Tools
	std::vector<USERTOOL> FUserTools;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TConfig(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TConfig(void);

  //@}


  //! @name Méthodes
  //@{

  bool PushLastWorkSpace(AnsiString asFileName);
  bool PushLastProject(AnsiString asFileName);
  bool PushLastFile(AnsiString asFileName);
  bool PushLastDialog(AnsiString asFileName);
  bool PushLastGraphic(AnsiString asFileName);
  bool PushSearchString(AnsiString asString);
  bool PushReplaceString(AnsiString asString);
	TShortCut GetShortCut(AnsiString asActionName);
	bool SetShortCut(AnsiString asActionName, TShortCut ShortCut);

	bool AddUserTool(AnsiString asToolName, AnsiString asToolProg, AnsiString asToolArgs, AnsiString asInitDir);
	bool DeleteUserTool(AnsiString asToolName);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété InstallPath
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TConfig, AnsiString, InstallPath);

  //---------------------------------------------------------------------------
  //! @brief Propriété EdiLeft
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, EdiLeft);

  //---------------------------------------------------------------------------
  //! @brief Propriété EdiTop
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, EdiTop);

  //---------------------------------------------------------------------------
  //! @brief Propriété EdiWidth
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, EdiWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété EdiHeight
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, EdiHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété EdiFullScreen
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, EdiFullScreen);

  //---------------------------------------------------------------------------
  //! @brief Propriété LeftPaneWidth
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, LeftPaneWidth);

  //---------------------------------------------------------------------------
  //! @brief Propriété BottomPaneHeight
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, BottomPaneHeight);

  //---------------------------------------------------------------------------
  //! @brief Propriété LastWorkSpaces
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TConfig, AnsiString, LastWorkSpaces);

  //---------------------------------------------------------------------------
  //! @brief Propriété LastProjects
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TConfig, AnsiString, LastProjects);

  //---------------------------------------------------------------------------
  //! @brief Propriété LastFiles
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TConfig, AnsiString, LastFiles);

  //---------------------------------------------------------------------------
  //! @brief Propriété LastDialogs
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TConfig, AnsiString, LastDialogs);

  //---------------------------------------------------------------------------
  //! @brief Propriété LastGraphics
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TConfig, AnsiString, LastGraphics);

  //---------------------------------------------------------------------------
  //! @brief Propriété OpenWorkSpace
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, AnsiString, OpenWorkSpace);

  //---------------------------------------------------------------------------
  //! @brief Propriété ActivePageIndex
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, ActivePageIndex);

  //---------------------------------------------------------------------------
  //! @brief Propriété EndOfLine
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, TEndOfLine, EndOfLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété NbSpacesTab
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, NbSpacesTab);

  //---------------------------------------------------------------------------
  //! @brief Propriété SpacesToTabs
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, SpacesToTabs);

  //---------------------------------------------------------------------------
  //! @brief Propriété TabsToSpaces
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, TabsToSpaces);

  //---------------------------------------------------------------------------
  //! @brief Propriété TypeAuto
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, TypeAuto);

  //---------------------------------------------------------------------------
  //! @brief Propriété IndentAuto
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, IndentAuto);

  //---------------------------------------------------------------------------
  //! @brief Propriété CurAfterEndLine
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, CurAfterEndLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété UndoRedoMoveCur
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, UndoRedoMoveCur);

  //---------------------------------------------------------------------------
  //! @brief Propriété LinesNumbers
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, LinesNumbers);

  //---------------------------------------------------------------------------
  //! @brief Propriété IconsColumn
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, IconsColumn);

  //---------------------------------------------------------------------------
  //! @brief Propriété MultiLineTabs
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, MultiLineTabs);

  //---------------------------------------------------------------------------
  //! @brief Propriété SaveBeforeBuild
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, SaveBeforeBuild);

  //---------------------------------------------------------------------------
  //! @brief Propriété Autocompletion
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, Autocompletion);

  //---------------------------------------------------------------------------
  //! @brief Propriété AlphaBlend
	//!
	//! Cette propriété active la transparence des fenêtres.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, AlphaBlend);

  //---------------------------------------------------------------------------
  //! @brief Propriété EndLineAcc
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, EndLineAcc);

  //---------------------------------------------------------------------------
  //! @brief Propriété SpacesAfterPar
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, SpacesAfterPar);

  //---------------------------------------------------------------------------
  //! @brief Propriété SearchString
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TConfig, TStrings *, SearchStrings);

  //---------------------------------------------------------------------------
  //! @brief Propriété ReplaceString
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TConfig, TStrings *, ReplaceStrings);

  //---------------------------------------------------------------------------
  //! @brief Propriété WholeWord
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, WholeWord);


  //---------------------------------------------------------------------------
  //! @brief Propriété CaseSensitive
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, CaseSensitive);


  //---------------------------------------------------------------------------
  //! @brief Propriété BegLine
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, BegLine);


  //---------------------------------------------------------------------------
  //! @brief Propriété EndLine
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, EndLine);


  //---------------------------------------------------------------------------
  //! @brief Propriété OnlyInComments
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, OnlyInComments);


  //---------------------------------------------------------------------------
  //! @brief Propriété NotInComments
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, NotInComments);


  //---------------------------------------------------------------------------
  //! @brief Propriété OnlyInQuotes
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, OnlyInQuotes);


  //---------------------------------------------------------------------------
  //! @brief Propriété NotInQuotes
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, NotInQuotes);


  //---------------------------------------------------------------------------
  //! @brief Propriété RegExp
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, RegExp);


  //---------------------------------------------------------------------------
  //! @brief Propriété KeepCase
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, KeepCase);


  //---------------------------------------------------------------------------
  //! @brief Propriété Loop
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, Loop);


  //---------------------------------------------------------------------------
  //! @brief Propriété SearchIn
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, SearchIn);


  //---------------------------------------------------------------------------
  //! @brief Propriété Recurse
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, Recurse);


  //---------------------------------------------------------------------------
  //! @brief Propriété SearchDirectory
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, AnsiString, SearchDirectory);


  //---------------------------------------------------------------------------
  //! @brief Propriété SearchExtensions
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, AnsiString, SearchExtensions);


  //---------------------------------------------------------------------------
  //! @brief Propriété ColorParams
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TConfig, COLORPARAM, ColorParams);


  //---------------------------------------------------------------------------
  //! @brief Propriété Font
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, TCustomFont *, Font);


  //---------------------------------------------------------------------------
  //! @brief Propriété AlignGrid
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, AlignGrid);


  //---------------------------------------------------------------------------
  //! @brief Propriété ShowGrid
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, bool, ShowGrid);


  //---------------------------------------------------------------------------
  //! @brief Propriété GridX
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, GridX);


  //---------------------------------------------------------------------------
  //! @brief Propriété GridY
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TConfig, int, GridY);


  //---------------------------------------------------------------------------
  //! @brief Propriété UserToolsCount
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TConfig, int, UserToolsCount);


	//---------------------------------------------------------------------------
  //! @brief Propriété UserTool
	//!
	//! Cette propriété permet la mémorisation dans la base de registre.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TConfig, const USERTOOL *, UserTools);


  //@}

};

//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------

extern TConfig * Config;

//---------------------------------------------------------------------------
#endif
