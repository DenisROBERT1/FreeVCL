//-----------------------------------------------------------------------------
//! @file TTextEditor.h
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

#ifndef TTextEditorH
#define TTextEditorH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <vector>
#include <TCustomEditor.h>
#include <TTooltip.h>


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum TYPEUNDO {
	tuNull,						            //!< Aucun
	tuMoveCur,						        //!< Déplacement du curseur texte
	tuIndent,						          //!< Indentation
	tuComment,						        //!< Ajout de commentaires
	tuText,						            //!< Ajout de texte
	tuDelete,						          //!< Suppression de texte
	tuInsert,						          //!< Insertion de texte
	tuReturn,						          //!< Retour à la ligne
	tuPaste,						          //!< Coller le contenu du presse-papier
	tuReplace,					          //!< Remplacement
	tuExterne						          //!< Opération externe
};

enum CHARENCODE {
	ceASCII,						          //!< ASCII (8 bits)
	ceUtf8,												//!< UTF8 (7 bits multi-caractères)
	ceUNICODE							        //!< UNICODE (16 caractères)
};

typedef struct {
  int IndexChar;
  TYPESYNTAX Type;
} COLORSYNTAX, *LPCOLORSYNTAX;

typedef struct {
	const TCHAR *szLine;
	int Index;
  int LenLine;
  bool bBeginWord;
  bool bBeginWordPreProc;
  bool bBeginNombreDec;     // Nombre décimal
  bool bBeginNombreHex;     // Nombre hexadécimal
  bool bBeginQuote1;				// Guillemets normaux (doubles)
  bool bBeginQuote2;				// Guillemets simples
  bool bBeginQuote3;				// Guillemets '<' (de l'instruction include)
  bool bPossibleGuill3;     // True lorsque l'ouverture des guillemets '<' est possible
  bool bBeginComm1;         // Commentaire mono-ligne
  bool bBeginComm2;         // Commentaire multi-lignes
  bool bBeginComm3;         // Commentaire multi-lignes de type 2
	bool bIsWord;
	int IndexBeginWord;
	int IndexBeginQuote;
} PARAMLINE, *LPPARAMLINE;



//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TTextEditorLines;


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnCaretMove.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnCaretMove)(TObject *Sender, int NumCol, int NumLine);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnAutocompletionOn.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnAutocompletionOn)(TObject *Sender, AnsiString asParent, int X, int Y, int Type);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnAutocompletionOff.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnAutocompletionOff)(TObject *Sender);


//---------------------------------------------------------------------------
//! @class TTextEditor
//!
//! @brief Editeur de fichier texte
//!
//! Cette classe (@c TTextEditor) permet d'éditer un fichier texte avec
//! coloration syntaxique (typiquement, du code). Elle comprend des méthodes
//! de recherche, de sélection, de modification de lignes, dans le but de
//! faire aisément un programme d'édition. Les couleurs, la police de
//! caractères, les mots-clés à colorer peuvent être paramétrés. Les actions
//! sont mémorisées pour pouvoir implémenter aisément un mécanisme de
//! undo/redo.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TTextEditor: public TCustomEditor {
private:
protected:
  bool bCaret;               //!< true si le caret est visible
  bool bMouseCapture;        //!< true si sélection en cours
  bool bReadOnly;            //!< true si readonly
  bool bInsert;              //!< true si mode insertion
  bool bDblClick;            //!< true si double-clic ( => ignorer le click qui suit)
  bool bNoEvent;             //!< true pour ignorer les évènement provoqués en interne
  int NbLinesWin;            //!< Nombre de lignes visibles (y compris les lignes coupées)
  int NbColsWin;             //!< Nombre de colonnes visibles
  int NbCol;                 //!< Nombre de colonnes de la plus longue ligne
	bool FBigEndian;           //!< Ordre des octets
	bool FByteOrderMark;       //!< Présence de l'en-tête BOM
	bool FLinuxEndLine;        //!< Fin de lignes Linux
	CHARENCODE FEncodage;      //!< ASCII, Utf8 ou Unicode
  int FFirstLine;            //!< Première ligne affichée
  int FFirstCol;             //!< Première colonne affichée
  int FNumLine;              //!< Ligne du curseur
  int FNumCol;               //!< Colonne du curseur
  int NumLineBegSel;         //!< Ligne de début de la sélection
  int NumColBegSel;          //!< Colonne de début de la sélection
  int NumLineEndSel;         //!< Ligne de fin de la sélection
  int NumColEndSel;          //!< Colonne de fin de la sélection
  bool bRectSel;						 //!< true si sélection carrée (avec CTRL ALT)
  bool bAutocompletionOn;    //!< true si autocompletion active
	bool bScrolling;           //!< true si scroll en cours
	TBitmap *BitmapScroll;     //!< Image d'aide au scroll
	bool bNoEndOperation;      //!< si true: permet d'encapsuler des opérations sans EndOperation

  int FNbSpacesTab;
  bool FSpacesToTabs;
  bool FTabsToSpaces;
  TTextEditorLines *FLines;
  bool FLinesNumbers;				 //!< true si numéro de lignes sont affichées
  bool FIconsColumn;				 //!< true si les icônes d'état sont affichées
  TStrings* FKeywords;
  bool FTypeAuto;
  bool FIndentAuto;
  bool FCurAfterEndLine;
  bool FUndoRedoMoveCur;
	AnsiString FAlphaChars;
	bool FAutocompletion;
	AnsiString FAutocompletion1;
	AnsiString FAutocompletion2;
	AnsiString FAutocompletion3;
	bool FCaseSensitive;
	AnsiString FCommentSingle;
	AnsiString FCommentSingleBeg;
	AnsiString FCommentBegin;
	AnsiString FCommentEnd;
	AnsiString FCommentBegin2;
	AnsiString FCommentEnd2;
	AnsiString FBlockBegin;
	AnsiString FBlockEnd;
	AnsiString FPreProc;
	TCHAR FQuoteBegin;
	TCHAR FQuoteEnd;
	TCHAR FQuoteBegin2;
	TCHAR FQuoteEnd2;
	TCHAR FQuoteBegin3;
	TCHAR FQuoteEnd3;
	AnsiString FEscapeString;
	AnsiString FMultiLine;
	AnsiString FAssociated1;
	AnsiString FAssociated2;
	static TTooltip *FTooltip;

  void FASTCALL UpdateHScroll(void);
  void FASTCALL UpdateVScroll(void);
  void FASTCALL TestNoHScroll(void);
  void FASTCALL TestNoVScroll(void);
	int FASTCALL NbIndentAfterLine(AnsiString asLine);
  bool IsAlpha(TCHAR c);
  bool IsNum(TCHAR c);
  bool IsNumHexa(TCHAR c);
  bool IsAlphaNum(TCHAR c);
  bool IsBlanc(TCHAR c);

  //---------------------------------------------------------------------------
  //! @brief Sélection d'un bloc de quelque chose
	//!
	//! Cette méthode sélectionne un bloc délimité par les caractères Beg et End
	//! à partir de la position (NumLine, NumCol). Les délimiteurs peuvent être
	//! imbriqués (comme les parenthèses par exemple)
  //!
  //! @param[in]        NumLine Numéro de ligne
  //! @param[in]        NumCol Numéro de colonne
  //! @param[in]        Beg Délimiteur de début
  //! @param[in]        End Délimiteur de fin
  //!
  //! @return @c bool true si bloc sélectionné.
  //---------------------------------------------------------------------------

	bool FASTCALL SelectBlockDelimiters(int NumLine, int NumCol, AnsiString Beg, AnsiString End);

  //---------------------------------------------------------------------------
  //! @brief Numéro de colonne d'un index dans une ligne
  //!
  //! Cette méthode calcule le numéro de colonne d'un index en tenant compte
	//! des tabulations (c'est à dire que les tabulations comptent pour autant de
	//! caractères que défini dans la configuration). Le caractère après la fin
	//! de ligne est compté pour une colonne (le curseur est souvent en fin de
	//! ligne).
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        Index Position à calculer (index 0 = premier caractère)
  //!
  //! @return @c int Numéro de colonne (-1 si fin de chaîne plus un caractère dépassés).
  //---------------------------------------------------------------------------

	int Lines_NumCol(int NumLine, int Index);

  //---------------------------------------------------------------------------
  //! @brief Index d'un numéro de colonne dans une ligne
  //!
  //! Cette méthode calcule l'index dans la chaîne correspondant à un numéro de
	//! colonne en tenant compte des tabulations (c'est à dire que les
	//! tabulations comptent pour autant de caractères que défini dans la
	//! configuration).
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        NumCol Numéro de colonne (première = 0)
  //! @param[out]       NumColDebChar Numéro de colonne du début du caractère (cas d'une tabulation)
  //!
  //! @return @c int Index dans la chaîne (-1 si fin de chaîne dépassée).
  //---------------------------------------------------------------------------

	int Lines_Index(int NumLine, int NumCol, int *NumColDebChar = NULL);

  //---------------------------------------------------------------------------
  //! @brief Index d'un numéro de colonne dans une ligne
  //!
  //! Cette méthode fait comme Lines_Index, sauf que le caractère n + 1 est
	//! comptabilisé comme un caractère (le curseur étant souvent en fin de
	//! ligne).
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        NumCol Numéro de colonne (première = 0)
  //!
  //! @return @c int Index dans la chaîne (-1 si fin de chaîne + 1 caractère dépassés).
  //!
  //! @sa TTextEditor::Lines_Index.
  //!
  //! @warning L'index retourné peut être supérieur à la taille de la ligne. Il
	//! ne faut donc pas faire Buffer[Lines_Index2(NumCol)], par exemple.
  //---------------------------------------------------------------------------

	int Lines_Index2(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Index d'un numéro de colonne dans une ligne
  //!
  //! Cette méthode fait comme Lines_Index, sauf que si NumCol est après la fin
	//! de la ligne, elle renvoie la position du caractère n + 1.
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        NumCol Numéro de colonne (première = 0)
  //!
  //! @return @c int Index dans la chaîne
  //!
  //! @sa TTextEditor::Lines_Index.
  //!
  //! @warning L'index retourné peut être supérieur à la taille de la ligne. Il
	//! ne faut donc pas faire Buffer[Lines_Index3(NumCol)], par exemple.
  //---------------------------------------------------------------------------

	int Lines_Index3(int NumLine, int NumCol);

	//---------------------------------------------------------------------------
  //! @brief Remplit une ligne d'espaces ou de tabulations.
  //!
  //! Cette méthode remplit la ligne NumLine d'espace ou de tabulations
	//! jusqu'à ce qu'elle fasse la longueur LengthMin. La longueur est calculée
	//! en tenant compte des tabulations (c'est à dire que LengthMin est un numéro
	//! de colonne).
  //!
  //! @param[in]        NumLine Numéro de ligne à compléter
  //! @param[in]        LengthMin Longueur à atteindre
  //!
  //! @return @c int Longueur finale de la ligne (peut être supérieur à
	//! LengthMin si la ligne avait déjà une longueur initiale supérieure)
  //---------------------------------------------------------------------------

	int Lines_SetLengthMin(int NumLine, int LengthMin);

	//---------------------------------------------------------------------------
  //! @brief Calcul du nombre maxi de colonnes du fichier.
  //!
  //! Cette méthode parcourt tout le fichier et calcule le nombre de colonnes
	//! de la ligne la plus longue. Les caractères tabulations sont remplacés par
	//! le nombre correspondant d'espaces pour le calcul. Cette méthode est
	//! utilisée en interne pour ajuster le scroll horizontal.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	bool CalculNbCol(void);

  //---------------------------------------------------------------------------
  //! @brief Affichage du caret
  //---------------------------------------------------------------------------

  void FASTCALL SetCaretVisible(bool bVisible);

  //---------------------------------------------------------------------------
  //! @brief Positionnement du caret
  //---------------------------------------------------------------------------

	void FASTCALL SetCurrentCaretPos(void);

  //---------------------------------------------------------------------------
  // Dessin de la fenêtre
  //---------------------------------------------------------------------------

  void FASTCALL Paint(void);

  //---------------------------------------------------------------------------
  //! @brief Type Undo / redo
  //!
  //! Cette méthode définit le type de modification. Si le type est différent
	//! du type courant, elle appelle la méthode EndOperation, puis la méthode
	//! BeginOperation avec le libellé correspondant au nouveau type (sauf si le
	//! nouveau type est égal à tuNull).
  //!
  //! @param[in]        TypeUndo Type de l'opération undo/redo
  //! @param[in]        asOperationName Nom de l'opération undo/redo si TypeUndo = tuExterne (ignoré sinon)
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool SetTypeUndoRedo(TYPEUNDO TypeUndo, AnsiString asOperationName = "");
  TYPEUNDO CurrentTypeUndo;

  //---------------------------------------------------------------------------
  //! @brief Dessin d'une ligne
  //---------------------------------------------------------------------------

  void FASTCALL DrawLine(TCanvas *ACanvas, int NumLine);

  //---------------------------------------------------------------------------
  //! @brief Coloration syntaxique
  //---------------------------------------------------------------------------

  bool FASTCALL ColorationSyntaxique(int NumLine, AnsiString asLine,
			bool bSelectColor,
      std::vector<COLORSYNTAX> *ColorSyntax,
      bool *bComment, bool *bComment2, bool *bQuote1, bool *bQuote2);

	bool FASTCALL ParseLine(PARAMLINE *ParamLine, bool bRight = true);
	bool FASTCALL FindBeginString(PARAMLINE *ParamLine, AnsiString asSearch);
	bool FASTCALL FindEndString(PARAMLINE *ParamLine, AnsiString asSearch);

	//---------------------------------------------------------------------------
	//! @brief Sélection ordonnée
	//---------------------------------------------------------------------------

	void GetSelOrdered(int *NumLineBegSel2, int *NumColBegSel2, int *NumLineEndSel2, int *NumColEndSel2);

	//---------------------------------------------------------------------------
	//! @brief Teste si le caractère (NumLine, NumCol) est dans la sélection.
	//---------------------------------------------------------------------------

	bool IsInSelection(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération élémentaire Undo
  //!
  //! Cette méthode est l'implémentation de la méthode abstraite de l'objet
	//! TUndoRedoUser.
  //!
  //! @param[out]       Undo Buffer Undo
  //! @param[in]        LenUndo Longueur du buffer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	virtual bool ProcessUndo(void * Undo, int LenUndo);

	//---------------------------------------------------------------------------
  //! @brief Effectue une opération élémentaire Redo
  //!
  //! Cette méthode est l'implémentation de la méthode abstraite de l'objet
	//! TUndoRedoUser.
  //!
  //! @param[out]       Redo Buffer Redo
  //! @param[in]        LenRedo Longueur du buffer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool ProcessRedo(void * Redo, int LenRedo);

public:


  FVCL_BEGIN_EVENTS
  void FASTCALL LinesChange(TObject *Sender);
  void FASTCALL TextEditorResize(TObject *Sender);
  void FASTCALL TextEditorEnter(TObject *Sender);
  void FASTCALL TextEditorExit(TObject *Sender);
  void FASTCALL TextEditorKeyDown(TObject *Sender,
        WORD Key, TShiftState Shift);
  void FASTCALL TextEditorKeyPress(TObject *Sender, TCHAR Key);
  void FASTCALL TextEditorMouseDown(
        TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
        int Y);
  void FASTCALL TextEditorMouseUp(TObject *Sender,
        TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL TextEditorMouseMove(
        TObject *Sender, TShiftState Shift, int X, int Y);
  void FASTCALL TextEditorMouseWheel(TObject *Sender, TShiftState Shift,
        int WheelDelta, const TPoint &MousePos, bool &Handled);
  void FASTCALL TextEditorDblClick(TObject *Sender);
  void FASTCALL HorzScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);
  void FASTCALL VertScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TTextEditor, TNotifyEvent, OnChange, LinesChange, TObject *);
		SET_EVENT_STR_1(TTextEditor, TNotifyEvent, OnResize, TextEditorResize, TObject *);
		SET_EVENT_STR_1(TTextEditor, TOnEnter, OnEnter, TextEditorEnter, TObject *);
		SET_EVENT_STR_1(TTextEditor, TOnExit, OnExit, TextEditorExit, TObject *);
		SET_EVENT_STR_3(TTextEditor, TOnKeyDown, OnKeyDown, TextEditorKeyDown, TObject *, WORD, TShiftState);
		SET_EVENT_STR_2(TTextEditor, TOnKeyPress, OnKeyPress, TextEditorKeyPress, TObject *, TCHAR);
		SET_EVENT_STR_5(TTextEditor, TOnMouseDown, OnMouseDown, TextEditorMouseDown, TObject *, TMouseButton, TShiftState, int, int);
		SET_EVENT_STR_5(TTextEditor, TOnMouseUp, OnMouseUp, TextEditorMouseUp, TObject *, TMouseButton, TShiftState, int, int);
		SET_EVENT_STR_4(TTextEditor, TOnMouseMove, OnMouseMove, TextEditorMouseMove, TObject *, TShiftState, int, int);
		SET_EVENT_STR_5(TTextEditor, TOnMouseWheel, OnMouseWheel, TextEditorMouseWheel, TObject *, TShiftState, int, const TPoint &, bool &);
		SET_EVENT_STR_1(TTextEditor, TNotifyEvent, OnClick, TextEditorDblClick, TObject *);
		SET_EVENT_STR_3(TTextEditor, TOnScroll, OnScroll, HorzScrollBarScroll, TObject *, TScrollCode, int &);
		SET_EVENT_STR_3(TTextEditor, TOnScroll, OnScroll, VertScrollBarScroll, TObject *, TScrollCode, int &);
  }

  FVCL_END_EVENTS


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TTextEditor(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTextEditor(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Déplacement du curseur
  //!
  //! Cette méthode déplace le curseur texte (le caret) à la position
	//! (NewNumLine, NewNumCol). Si les flags bSelection et/ou bNewRectSel sont
	//! activés, le texte entre l'ancienne position et la nouvelle est
	//! sélectionné.
  //!
  //! @param[in]        NewNumLine Nouvelle position en Y
  //! @param[in]        NewNumCol Nouvelle position en X
  //! @param[in]        bSelection Active la sélection du texte
  //! @param[in]        bNewRectSel Active la sélection "en rectangle".
	//!
	//! @note Etant donné que le caractère tabulation est représenté par
	//! plusieurs espaces, ne pas confondre la position en numéro de colonne
	//! et l'index dans la ligne.
  //---------------------------------------------------------------------------

	void  FASTCALL MoveCur(int NewNumLine, int NewNumCol, bool bSelection, bool bNewRectSel);

	//---------------------------------------------------------------------------
  //! @brief Position dans le fichier correspondante aux coordonnées X, Y
  //!
  //! Cette fonction renvoie la position dans le fichier correspondante au
	//! point de coordonnées (X, Y). Le calcul tient compte des scrolls, de
	//! la taille de la police, etc... Les coordonnées X, Y sont des coordonnées
	//! client (par rapport au point supérieur gauche de la fenêtre).@n
	//! ATTENTION : dans certains cas, les numéros de ligne ou de colonne peuvent
	//! ne pas être des numéros valides (soit négatifs, soit supérieurs au
	//! maximum). Tester la valeur retournée avant de l'utiliser.
  //!
  //! @param[in]        Y Coordonnée Y
  //! @param[in]        X Coordonnée X
  //! @param[out]       NumLine Numéro de ligne
  //! @param[out]       NumCol Numéro de colonne
  //---------------------------------------------------------------------------

  void FASTCALL GetPosAt(int Y, int X, int *NumLine, int *NumCol);

	//---------------------------------------------------------------------------
  //! @brief Coordonnées client d'une position ligne/colonne
  //!
  //! Cette fonction renvoie les coordonnées du coin supérieur gauche du
	//! caractère à la position (NumLine, NumCol). Le calcul tient compte des
	//! scrolls, de la taille de la police, etc... Les coordonnées X, Y sont des
	//! coordonnées client (par rapport au point supérieur gauche de la fenêtre).
  //!
  //! @param[in]        NumLine Numéro de ligne
  //! @param[in]        NumCol Numéro de colonne
  //! @param[out]       X Coordonnée X
  //! @param[out]       Y Coordonnée Y
  //---------------------------------------------------------------------------

  void FASTCALL GetXYOf(int NumLine, int NumCol, int *Y, int *X);

	//---------------------------------------------------------------------------
  //! @brief Lecture depuis un fichier
  //!
  //! Cette méthode charge en mémoire le contenu d'un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à lire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromFile(const AnsiString FileName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un fichier
  //!
  //! Cette méthode enregistre les données dans un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à écrire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL SaveToFile(const AnsiString FileName);

  //---------------------------------------------------------------------------
  //! @brief Scroll horizontal de la fenêtre
	//!
	//! Cette méthode translate horizontalement la fenêtre de manière à ce que la
	//! colonne NewFirstCol devienne la première visible.
  //!
  //! @param[in]        NewFirstCol Nouveau numéro de la première colonne
  //---------------------------------------------------------------------------
  void FASTCALL SetFirstCol(int NewFirstCol);

  //---------------------------------------------------------------------------
  //! @brief Scroll vertical de la fenêtre
	//!
	//! Cette méthode translate verticalement la fenêtre de manière à ce que la
	//! ligne NewFirstLine devienne la première visible (sauf si le nombre de
	//! lignes jusqu'à la fin de fichier est inférieur au nombre de lignes
	//! visibles).
  //!
  //! @param[in]        NewFirstLine Nouveau numéro de la première ligne
  //---------------------------------------------------------------------------

  void FASTCALL SetFirstLine(int NewFirstLine);

  //---------------------------------------------------------------------------
  //! @brief Sélection d'un mot complet
	//!
	//! Cette méthode sélectionne un mot complet à partir de la position
	//! (NumLine, NumCol).
  //!
  //! @param[in]        NumLine Numéro de ligne
  //! @param[in]        NumCol Numéro de colonne
  //!
  //! @return @c bool true si mot sélectionné.
  //---------------------------------------------------------------------------

  bool FASTCALL SelectWord(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Sélection d'un mot complet
	//!
	//! Cette méthode retourne un mot complet à partir de la position
	//! (NumLine, NumCol).
  //!
  //! @param[in]        NumLine Numéro de ligne
  //! @param[in]        NumCol Numéro de colonne
  //! @param[out]       asWord Mot trouvé
  //!
  //! @return @c bool true si mot trouvé.
  //---------------------------------------------------------------------------

  bool FASTCALL WordFromPos(int NumLine, int NumCol, AnsiString *asWord);

  //---------------------------------------------------------------------------
  //! @brief Sélection d'un bloc d'instructions
	//!
	//! Cette méthode sélectionne un bloc d'instructions (instructions délimitées
	//! par des accolades en c ou c++) à partir de la position (NumLine, NumCol).
  //!
  //! @param[in]        NumLine Numéro de ligne
  //! @param[in]        NumCol Numéro de colonne
  //!
  //! @return @c bool true si bloc sélectionné.
  //---------------------------------------------------------------------------

  bool FASTCALL SelectBlock(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Sélection d'une parenthèse
	//!
	//! Cette méthode sélectionne le contenu d'une parenthèse avec les
	//! parenthères elles-mêmes, à partir de la position (NumLine, NumCol).
  //!
  //! @param[in]        NumLine Numéro de ligne
  //! @param[in]        NumCol Numéro de colonne
  //!
  //! @return @c bool true si parenthèse sélectionnée.
  //---------------------------------------------------------------------------

  bool FASTCALL SelectParenthesis(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Déplacement du curseur au début du mot précédent
	//!
	//! Cette méthode déplace le curseur jusqu'au début du mot précédent. Si le
	//! flag bShift est activé, les caractères entre l'ancienne position et la
	//! nouvelle sont sélectionnés.
  //!
  //! @param[in]        bShift Activation de la sélection.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  bool FASTCALL MovePrevWord(bool bShift);

  //---------------------------------------------------------------------------
  //! @brief Déplacement du curseur au début du mot suivant
  //!
	//! Cette méthode déplace le curseur jusqu'au début du mot suivant. Si le
	//! flag bShift est activé, les caractères entre l'ancienne position et la
	//! nouvelle sont sélectionnés.
  //!
  //! @param[in]        bShift Activation de la sélection.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  bool FASTCALL MoveNextWord(bool bShift);

  //---------------------------------------------------------------------------
  //! @brief Déplacement du curseur au premier caractère non blanc de la ligne
  //!
	//! Cette méthode déplace le curseur jusqu'au début de la ligne (espaces et
	//! tabulations non comprises). Si le flag bShift est activé, les caractères
	//! entre l'ancienne position et la nouvelle sont sélectionnés.
  //!
  //! @param[in]        bShift Activation de la sélection.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  bool FASTCALL MoveFirstChar(bool bShift);

  //---------------------------------------------------------------------------
  //! @brief Déplacement de la fenêtre de visualisation
  //!
  //! Cette méthode décale la fenêtre de visualisation de manière à rendre
	//! visible le caractère situé à la position (NumLine, NumCol).
  //!
  //! @param[in]        NumLine Numéro de ligne à visualiser
  //! @param[in]        NumCol Numéro de colonne à visualiser
  //---------------------------------------------------------------------------
  void FASTCALL ScrollInView(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Indentation de la sélection
  //!
  //! Cette méthode indente les lignes sélectionnées. Les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  bool FASTCALL IndentSelection(void);

  //---------------------------------------------------------------------------
  //! @brief Desindentation de la sélection
  //!
  //! Cette méthode désindente les lignes sélectionnées. Les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  bool FASTCALL UnindentSelection(void);

  //---------------------------------------------------------------------------
  //! @brief Mise en commentaires de la sélection
  //!
  //! Cette méthode met en commentaire les lignes sélectionnées. Les
	//! modifications sont mémorisées dans le buffer undo/redo pour une
	//! annulation éventuelle.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  bool FASTCALL CommentSelection(void);

  //---------------------------------------------------------------------------
  //! @brief Decommente la sélection
  //!
  //! Cette méthode décommente les lignes sélectionnées. Les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  bool FASTCALL UncommentSelection(void);

  //---------------------------------------------------------------------------
  //! @brief Effacement de la sélection
  //!
  //! Cette méthode efface le contenu de la sélection. Les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  void FASTCALL ClearSelection(void);

  //---------------------------------------------------------------------------
  //! @brief Couper et copier dans le presse-papier
  //!
  //! Cette méthode copie le contenu de la sélection dans le presse-papiers,
	//! puis l'efface. Les modifications sont mémorisées dans le buffer undo/redo
	//! pour une annulation éventuelle.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL CutToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Copier dans le presse-papier
  //!
  //! Cette méthode copie le contenu de la sélection dans le presse-papiers.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL CopyToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Coller depuis le presse-papier
  //!
  //! Cette méthode colle le contenu du presse-papiers à la position du
	//! curseur. Les modifications sont mémorisées dans le buffer undo/redo pour
	//! une annulation éventuelle.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL PasteFromClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Chercher un texte
  //!
  //! Cette méthode cherche le texte asSearch avec les paramètres memorisés
	//! dans la structure lpSearchParam.
  //!
  //! @param[in]        asSearch Chaîne à rechercher
  //! @param[in]        lpSearchParam Paramètres de recherche
  //!
  //! @return @c bool true si chaîne trouvée.
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Search(AnsiString asSearch, LPSEARCHPARAM lpSearchParam);

  //---------------------------------------------------------------------------
  //! @brief Remplacer un texte suite à une recherche
  //!
  //! Cette méthode remplace la sélection par la chaîne asReplace. Elle est en
	//! général appelée après l'appel de la méthode Search.
	//! 
  //! @param[in]        asReplace Chaîne de remplacement.
  //! @param[in]        bKeepCase Conservation de la casse.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Replace(AnsiString asReplace, bool bKeepCase);

  //---------------------------------------------------------------------------
  //! @brief Efface tout le texte du contrôle de saisie.
  //!
  //! Cette méthode met à zéro l'ensemble du texte. Cette action n'est pas
	//! mémorisée dans le buffer Undo/Redo.
  //---------------------------------------------------------------------------

  virtual void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Copie le texte sélectionné dans une chaîne.
  //!
  //! Cette méthode copie le contenu de la sélection dans un buffer de type
	//! texte.
  //!
  //! @return @c AnsiString Contenu de la sélection. Les lignes sont séparées
	//! par le caractère LF.
  //---------------------------------------------------------------------------

	virtual AnsiString FASTCALL GetSelText(void);

  //---------------------------------------------------------------------------
  //! @brief Copie le texte sélectionné au format RTF.
  //!
  //! Cette méthode copie le contenu de la sélection dans un buffer de type
	//! Rich Text Format (texte avec mise en forme, police, couleurs, etc...).
  //!
  //! @return @c CharString Contenu de la sélection au format RTF.
  //---------------------------------------------------------------------------

	virtual CharString FASTCALL GetSelRtf(void);

  //---------------------------------------------------------------------------
  //! @brief Copie le texte sélectionné dans un tampon.
  //!
  //! Cette méthode copie le texte sélectionné dans une zone mémoire.
  //!
  //! @param[in, out]   Buffer Tampon destination
  //! @param[in]        BufSize Taille du tampon (zéro terminal compris)
  //!
  //! @return @c int Longueur de la chaîne copiée.
  //---------------------------------------------------------------------------

	virtual int FASTCALL GetSelTextBuf(TCHAR * Buffer, int BufSize);

  //---------------------------------------------------------------------------
  //! @brief Remplace le texte sélectionné par une chaîne à zéro terminal.
  //!
  //! Cette méthode efface la sélection, puis insère le texte passé en
	//! paramètre. La sélection comme le texte peuvent être multilignes.
  //!
  //! @param[in]        Buffer Texte à insérer.
  //---------------------------------------------------------------------------

  virtual void FASTCALL SetSelTextBuf(const TCHAR * Buffer);

  //---------------------------------------------------------------------------
  //! @brief Sélectionne tout le texte.
  //!
  //! Cette méthode sélectionne l'ensemble du texte.
  //---------------------------------------------------------------------------

  virtual void FASTCALL SelectAll(void);

  //---------------------------------------------------------------------------
  //! @brief Début d'une série de modifications (pas de raffraichissement).
  //!
  //! Cette méthode permet de faire une série de modifications sans que la
	//! fenêtre soit raffraichie, ce qui évite un clignotement et accélère le
	//! traitement.@n
	//! Appelez BeginUpdate, puis faites vos modifications, et appelez EndUpdate
	//! lorsque la série de modifications est finie.
  //---------------------------------------------------------------------------
	virtual void FASTCALL BeginUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Fin d'une série de modifications (raffraichissement éventuel).
  //!
  //! Cette méthode permet de terminer une série de modifications (voir
	//! TTextEditor::BeginUpdate). Si les modifications qui ont été faites
	//! ont un effet sur l'affichage, la fenêtre est raffraichie à ce moment.
  //---------------------------------------------------------------------------

	virtual void FASTCALL EndUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Lecture de la ligne NumLine
  //!
  //! Cette méthode lit la ligne numéro NumLine pour affichage (c'est à dire
	//! qu'elle transforme les tabulations par le nombre d'espaces correspondant).
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //!
  //! @return @c AnsiString Ligne lue
  //---------------------------------------------------------------------------

	AnsiString Lines_Strings(int NumLine);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un caractère dans une ligne
  //!
  //! Cette méthode lit un caractère d'une ligne, en tenant compte des
	//! tabulations. Par exemple, si une ligne contient "\tABCD" (avec deux
	//! caractères par tabulation), LineChar(0, 0) et LineChar(0, 1) renverront
	//! tous deux le caractère tabulation, et LineChar(0, 2) renverra le
	//! caractère 'A'.
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        NumCol Numéro de colonne du caractère à lire (premier = 0)
  //!
  //! @return @c TCHAR Caractère lu (Caractère null si Index est supérieur à la
	//! longueur de la chaîne).
  //---------------------------------------------------------------------------

	TCHAR Lines_Char(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Partie gauche d'une ligne
  //!
  //! Cette méthode renvoie la partie gauche de la ligne NumLine jusqu'à la
	//! position Index (non compris), en tenant compte des tabulations (c'est à
	//! dire que les tabulations comptent pour autant de caractères que défini
	//! dans la configuration).
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        NumCol Position du découpage (index 0 = chaîne vide)
  //!
  //! @return @c AnsiString Partie gauche de la ligne
  //---------------------------------------------------------------------------

	AnsiString Lines_Left(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Partie droite d'une ligne
  //!
  //! Cette méthode renvoie la partie droite de la ligne NumLine depuis la
	//! position Index (compris), en tenant compte des tabulations (c'est à dire
	//! que les tabulations comptent pour autant de caractères que défini dans la
	//! configuration).
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        NumCol Position du découpage (index 0 = toute la ligne)
  //!
  //! @return @c AnsiString Partie droite de la ligne
  //---------------------------------------------------------------------------

	AnsiString Lines_Right(int NumLine, int NumCol);

  //---------------------------------------------------------------------------
  //! @brief Partie d'une ligne
  //!
  //! Cette méthode renvoie la partie de la ligne NumLine depuis la position
	//! Index1 (compris), jusqu'à la position Index2 (non compris), en tenant
	//! compte des tabulations (c'est à dire que les tabulations comptent pour
	//! autant de caractères que défini dans la configuration).
  //!
  //! @param[in]        NumLine Numéro de ligne à lire (première = 0)
  //! @param[in]        NumCol Début du découpage (index 0 = début de la ligne)
  //! @param[in]        Count Longueur du découpage
  //!
  //! @return @c AnsiString Partie de la ligne
  //---------------------------------------------------------------------------

	AnsiString Lines_SubString(int NumLine, int NumCol, int Count);

	//---------------------------------------------------------------------------
  //! @brief Calcul de la longueur d'une ligne
  //!
  //! Cette méthode calcule la longueur d'une ligne après remplacement des
	//! tabulations par le nombre d'espaces correspondant.
  //!
  //! @param[in]        NumLine Numéro de ligne
  //!
  //! @return @c int Longueur en nombre de caractères.
  //---------------------------------------------------------------------------

	int Lines_Length(int NumLine);

  //---------------------------------------------------------------------------
  //! @brief Remplace une ligne de l'éditeur.
  //!
  //! Cette méthode remplace la ligne NumLine par le contenu de la chaîne
	//! str. Les modifications sont mémorisées dans le buffer undo/redo pour une
	//! annulation éventuelle.
  //!
  //! @param[in]        NumLine Numéro de ligne à modifier
  //! @param[in]        str Chaîne de remplacement
  //!
  //! @return @c int Longueur de la ligne après remplacement
  //---------------------------------------------------------------------------

	int Lines_Change(int NumLine, AnsiString str);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une partie de texte dans une ligne.
  //!
  //! Cette méthode supprime une partie de la ligne NumLine. les modifications
	//! sont mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        NumLine Numéro de ligne à modifier
  //! @param[in]        Index Index de la partie à supprimer
  //! @param[in]        Count Nombre de caractères à supprimer
  //!
  //! @return @c int valeur du paramètre Index
	//!
	//! @note Index est bien un index de caractère dans la chaîne, et non un
	//! numéro de colonne, car cette méthode est utilisée par le undo/redo, et
	//! elle doit fonctionner même si l'utilisateur change le nombre de
	//! caractères des tabulations en cours d'utilisation.
  //---------------------------------------------------------------------------

  int Lines_DeleteSubString(int NumLine, int Index, int Count);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un texte dans une ligne.
  //!
  //! Cette méthode insère un texte dans la ligne NumLine. les modifications
	//! sont mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        NumLine Numéro de ligne à modifier
  //! @param[in]        str Texte à insérer
  //! @param[in]        Index Index où insérer le texte
  //!
  //! @return @c int Longueur de la ligne après modification
  //---------------------------------------------------------------------------

  int Lines_InsertSubString(int NumLine, AnsiString str, int Index);

  //---------------------------------------------------------------------------
  //! @brief Coupure d'une ligne en deux parties.
  //!
  //! Cette méthode coupe la ligne NumLine en deux parties. les modifications
	//! sont mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        NumLine Numéro de la ligne à couper
  //! @param[in]        Index Index de la coupure
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool Lines_Split(int NumLine, int Index);

  //---------------------------------------------------------------------------
  //! @brief Fusion de deux lignes consécutives.
  //!
  //! Cette méthode fusionne la ligne NumLine et la suivante. les modifications
	//! sont mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        NumLine Numéro de la première ligne à fusionner
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool Lines_Merge(int NumLine);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une ligne.
  //!
  //! Cette méthode supprime la ligne NumLine. les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        NumLine Numéro de ligne à modifier
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  bool Lines_DeleteLine(int NumLine);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'une ligne.
  //!
  //! Cette méthode insère la ligne NumLine. L'ancienne ligne NumLine est
	//! décalée vers le bas. Les modifications sont mémorisées dans le buffer
	//! undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        NumLine Numéro de ligne à modifier
  //! @param[in]        str Texte de la ligne à insérer
  //!
  //! @return @c int Longueur de la ligne après modification
  //---------------------------------------------------------------------------

  int Lines_InsertLine(int NumLine, AnsiString str);

  //---------------------------------------------------------------------------
  //! @brief Modification de l'ensemble du fichier.
  //!
  //! Cette méthode affecte l'ensemble du fichier avec le contenu du paramètre
	//! @b str. Les modifications sont mémorisées dans le buffer undo/redo pour
	//! une annulation éventuelle.
  //!
  //! @param[in]        NewLines Nouveau contenu du fichier
  //!
  //! @return @c bool true su Ok
  //---------------------------------------------------------------------------

  bool Lines_SetLines(TStrings *NewLines);

  //---------------------------------------------------------------------------
  //! @brief Renvoie la valeur d'un flag
  //!
  //! Cette méthode renvoie true si le flag Flag a été mis à la ligne NumLine.
  //!
  //! @param[in]        NumLine Numéro de ligne (première = 0)
  //! @param[in]        Flag Flag à tester
  //!
  //! @return @c bool true si positionné
  //---------------------------------------------------------------------------

	bool GetFlag(int NumLine, TEditorLinesFlag Flag);

  //---------------------------------------------------------------------------
  //! @brief Positionne un flag
  //!
  //! Cette méthode positionne un flag à la ligne NumLine.
  //!
  //! @param[in]        NumLine Numéro de ligne (première = 0)
  //! @param[in]        Flag Flag à positionner
  //! @param[in]        bSet true si positionné
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	bool SetFlag(int NumLine, TEditorLinesFlag Flag, bool bSet);

  //---------------------------------------------------------------------------
  //! @brief Supprime tous les flag d'un type
  //!
  //! Cette méthode supprime tous les flag du type Flag.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	bool ClearFlag(TEditorLinesFlag Flag);

  //---------------------------------------------------------------------------
  //! @brief Début d'une opération pouvant être défaite ou refaite
  //!
  //! Début d'une opération pouvant être défaite ou refaite. A chaque
	//! BeginOperation doit correspondre un appel à EndOperation.
  //!
  //! @param[in] asOperationName Nom de l'opération
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL BeginOperation(AnsiString asOperationName);

  //---------------------------------------------------------------------------
  //! @brief Fin d'une opération pouvant être défaite ou refaite
  //!
  //! Fin d'une opération pouvant être défaite ou refaite. Chaque EndOperation
  //! doit correspondre à un BeginOperation.
  //---------------------------------------------------------------------------
  virtual bool FASTCALL EndOperation(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération undo
  //!
  //! Cette méthode effectue une opération undo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Undo(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette méthode effectue une opération redo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Redo(void);

  //---------------------------------------------------------------------------
  //! @brief Autocomplétion
  //!
  //! @param[in]        NumCompletion Type d'autocomplétion reconnue (. ou -> par exemple) ou -1 si commande lancée manuellement
  //!
  //! Cette méthode ouvre la fenêtre d'auto-complétion.
  //---------------------------------------------------------------------------
  void FASTCALL AutocompletionOn(int NumCompletion);

  //---------------------------------------------------------------------------
  //! @brief Arrêt autocomplétion
  //!
  //! Cette méthode ferme la fenêtre d'auto-complétion
  //---------------------------------------------------------------------------
  void FASTCALL AutocompletionOff(void);

	//---------------------------------------------------------------------------
  //! @brief Traitement d'un redimensionnement de la fenêtre.
  //!
  //! Cette méthode est appelée lorsque les dimensions de la fenêtre ou de la
	//! font ont changé.
  //---------------------------------------------------------------------------

  virtual void FASTCALL ProcessResize(void);

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
  //! @brief Propriété BigEndian
	//!
	//! Cette propriété détermine si les caractères sont codés avec la valeur
	//! haute en premier.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, BigEndian);

  //---------------------------------------------------------------------------
  //! @brief Propriété ByteOrderMark
	//!
	//! Cette propriété détermine si le fichier en lecture et en enregistrement
	//! contient un en-tête Byte Order Mark (BOM). 
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, ByteOrderMark);

  //---------------------------------------------------------------------------
  //! @brief Propriété LinuxEndLine
	//!
	//! Cette propriété détermine si le fichier texte contient des fins de lignes
	//! de type Linux/Unix (\\n soit 0A) ou bien des fins de lignes de type
	//! Windows (\\r\\n soit 0D 0A). Si un même fichier contient les deux types
	//! de fin de lignes, la valeur de LinuxEndLine sera @b false (c'est à dire
	//! que le type Windows sera choisi). 
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, LinuxEndLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété Encodage
	//!
	//! Cette propriété détermine le format des chaînes de caractères. Cette
	//! propriété n'influe que sur la méthode SaveToFile, car en interne les
	//! chaînes ont le format déterminé par l'option de compilation UNICODE.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, CHARENCODE, Encodage);

  //---------------------------------------------------------------------------
  //! @brief Propriété FirstLine: première ligne affichée
	//!
	//! Cette propriété définit la première ligne affichée (en commençant à
	//! zéro).
	//!
	//! @sa SetFirstLine
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, int, FirstLine);


  //---------------------------------------------------------------------------
  //! @brief Propriété FirstCol
	//!
	//! Cette propriété définit la première colonne affichée (en commençant à
	//! zéro).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, int, FirstCol);

  //---------------------------------------------------------------------------
  //! @brief Propriété NumLine: ligne du curseur
	//!
	//! Cette propriété définit la ligne où est le curseur texte (caret).@n
	//! @b ATTENTION : dans le cas d'un fichier vide, cette propriété renvoie
	//! la valeur 0, qui est après la fin de fichier. Vous devez donc tester la
	//! validité de la valeur renvoyée avant de l'utiliser.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, int, NumLine);


  //---------------------------------------------------------------------------
  //! @brief Propriété NumCol: colonne du curseur
	//!
	//! Cette propriété définit la colonne où est le curseur texte (caret).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, int, NumCol);

  //---------------------------------------------------------------------------
  //! @brief Propriété Keywords
	//!
	//! Cette propriété définit la liste des mot-clés. Ceux-ci s'affichent avec
	//! une couleur différente dans l'éditeur.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TStrings*, Keywords);

  //---------------------------------------------------------------------------
  //! @brief Propriété Lines
	//!
	//! Cette propriété permet d'accéder directement aux lignes de l'éditeur,
	//! sans passer par le système de mémorisation des modifications (undo/redo).
	//! Pour cette raison, vous devez plutôt utiliser les méthodes de
	//! modification des lignes qui gèrent la mémorisation.
	//!
	//! @sa Lines_Change Lines_DeleteSubString Lines_InsertSubString Lines_Split
	//! Lines_Merge Lines_DeleteLine Lines_InsertLine Lines_SetLines
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TStrings*, Lines);

  //---------------------------------------------------------------------------
  //! @brief Propriété LinesNumbers: colonne des numéros de lignes visible
	//!
	//! Cette propriété détermine si la colonne contenant les numéros de lignes
	//! est visible ou pas.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, LinesNumbers);

  //---------------------------------------------------------------------------
  //! @brief Propriété IconsColumn: colonne des icônes visible
	//!
	//! Cette propriété détermine si la colonne contenant les icônes
	//! (marque-pages par exemple) est visible ou pas. Si elle n'est pas visible,
	//! cela n'empêche pas de positionner des marque-pages.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, IconsColumn);

  //---------------------------------------------------------------------------
  //! @brief Propriété NbSpacesTab: nombre d'espaces par tabulation
	//!
	//! Cette propriété détermine le nombre d'espaces que représente un caractère
	//! tabulation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, int, NbSpacesTab);

  //---------------------------------------------------------------------------
  //! @brief Propriété SpacesToTabs: transformation des espaces en tabulations
	//!
	//! Si cette propriété est @b true, le programme transforme automatiquement
	//! les espaces en tabulations dès la saisie. Le nombre d'espaces est
	//! déterminé par la propriété @b NbSpacesTab. Cette propriété est
	//! incompatible avec la propriété @b TabsToSpaces (le fait d'en activer une
	//! désactive automatiquement l'autre).
	//!
	//! @sa NbSpacesTab TabsToSpaces
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, SpacesToTabs);

  //---------------------------------------------------------------------------
  //! @brief Propriété TabsToSpaces: transformation des tabulations en espaces
	//!
	//! Si cette propriété est @b true, le programme transforme automatiquement
	//! les tabulations en espaces dès la saisie. Le nombre d'espaces est
	//! déterminé par la propriété @b NbSpacesTab. Cette propriété est
	//! incompatible avec la propriété @b SpacesToTabs (le fait d'en activer une
	//! désactive automatiquement l'autre).
	//!
	//! @sa NbSpacesTab SpacesToTabs
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, TabsToSpaces);

  //---------------------------------------------------------------------------
  //! @brief Propriété SelText: texte sélectionné
	//!
	//! Cette propriété permet de lire ou de remplacer le texte sélectionné. La
	//! modification est mémorisée pour un éventuel undo.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, SelText);

  //---------------------------------------------------------------------------
  //! @brief Propriété TypeAuto
	//!
	//! Cette propriété permet de taper automatiquement les parenthèses et
	//! accolades fermantes lorsque l'utilisateur en ouvre une.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, TypeAuto);

  //---------------------------------------------------------------------------
  //! @brief Propriété IndentAuto: indentation automatique
	//!
	//! Cette propriété permet d'indenter automatiquement le texte lorsque
	//! l'utilisateur entre un retour chariot.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, IndentAuto);

  //---------------------------------------------------------------------------
  //! @brief Propriété CurAfterEndLine: autorisation curseur après la fin de ligne
	//!
	//! Cette propriété permet à l'utilisateur de positionner le curseur après
	//! la fin d'une ligne. Sinon, le curseur se positionne automatiquement en
	//! fin de ligne si l'utilisateur clique après la fin de la ligne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, CurAfterEndLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété UndoRedoMoveCur: mémorisation des déplacements curseur
	//!
	//! Cette propriété permet de mémoriser les mouvement de curseur dans le
	//! undo/redo. Sinon, seules les modifications de texte sont mémorisées et
	//! peuvent être défaites et refaites.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, UndoRedoMoveCur);

  //---------------------------------------------------------------------------
  //! @brief Propriété AlphaChars: caractères alpha-numériques
	//!
	//! Cette propriété définit les caractères alphabétiques autorisés pour
	//! les mots du texte. Cette liste de caractère permet d'isoler les mots des
	//! autres signes et caractères séparateurs (lorsqu'on fait un double-clic
	//! sur un mot par exemple). Par défaut, cette liste contient les caractères
	//! strictement alphabétiques sans accents.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, AlphaChars);

  //---------------------------------------------------------------------------
  //! @brief Propriété Autocompletion: activation de l'autocomplétion
	//!
	//! Cette propriété active l'autocomplétion. Si cette propriété est activée,
	//! les évènements @b OnAutocompletionOn et @b OnAutocompletionOff seront
	//! déclenchés lorsque l'utilisateur tapera une chaîne correspondante à
	//! l'autocomplétion.
	//!
	//! @sa OnAutocompletionOn Autocompletion1 Autocompletion2 Autocompletion3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, Autocompletion);

  //---------------------------------------------------------------------------
  //! @brief Propriété Autocompletion1: chaîne d'autocomplétion 1
	//!
	//! Cette propriété définit une chaîne qui, lorsqu'elle est tapée par
	//! l'utilisateur, provoque l'autocomplétion. Il peut y avoir trois chaînes
	//! de caractères différentes qui ont cet effet.
	//!
	//! @sa OnAutocompletionOn Autocompletion Autocompletion2 Autocompletion3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, Autocompletion1);

  //---------------------------------------------------------------------------
  //! @brief Propriété Autocompletion2: chaîne d'autocomplétion 2
	//!
	//! Cette propriété définit une chaîne qui, lorsqu'elle est tapée par
	//! l'utilisateur, provoque l'autocomplétion. Il peut y avoir trois chaînes
	//! de caractères différentes qui ont cet effet.
	//!
	//! @sa OnAutocompletionOn Autocompletion Autocompletion1 Autocompletion3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, Autocompletion2);

  //---------------------------------------------------------------------------
  //! @brief Propriété Autocompletion3: chaîne d'autocomplétion 3
	//!
	//! Cette propriété définit une chaîne qui, lorsqu'elle est tapée par
	//! l'utilisateur, provoque l'autocomplétion. Il peut y avoir trois chaînes
	//! de caractères différentes qui ont cet effet.
	//!
	//! @sa OnAutocompletionOn Autocompletion Autocompletion1 Autocompletion2
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, Autocompletion3);

  //---------------------------------------------------------------------------
  //! @brief Propriété CaseSensitive: sensibilité à la casse
	//!
	//! Cette propriété détermine si la différence entre les minuscules et les
	//! majuscules doit être prise en compte pour la reconnaissance des mots-clé,
	//! ou des noms de variables.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, bool, CaseSensitive);

  //---------------------------------------------------------------------------
  //! @brief Propriété CommentSingle: commentaires mono-ligne
	//!
	//! Cette propriété définit la chaîne de caractères indiquant les
	//! commentaires sur une ligne. Le commentaire se termine au premier
	//! caractère de retour à la ligne.
	//!
	//! @sa CommentSingleBeg
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, CommentSingle);

  //---------------------------------------------------------------------------
  //! @brief Propriété CommentSingleBeg: commentaires mono-ligne en début de ligne
	//!
	//! Cette propriété définit la chaîne de caractères indiquant les
	//! commentaires sur une ligne, mais, à la différence de la propriété
	//! @b CommentSingle, uniquement si cette chaîne de caractères débute une
	//! ligne, caractères de séparations mis à part (espaces et tabulations). Le
	//! commentaire se termine au premier caractère de retour à la ligne.
	//!
	//! @sa CommentSingle
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, CommentSingleBeg);

  //---------------------------------------------------------------------------
  //! @brief Propriété CommentBegin: début de commentaire multi-lignes
	//!
	//! Cette propriété définit la chaîne de caractères indiquant les
	//! commentaires sur plusieurs lignes. Le commentaire se termine lorsque la
	//! chaîne @b CommentEnd est rencontrée.
	//!
	//! @sa CommentEnd
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, CommentBegin);

  //---------------------------------------------------------------------------
  //! @brief Propriété CommentEnd: fin de commentaire multi-lignes
	//!
	//! Cette propriété définit la chaîne de caractères indiquant la fin des
	//! commentaires sur plusieurs lignes.
	//!
	//! @sa CommentBegin
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, CommentEnd);

  //---------------------------------------------------------------------------
  //! @brief Propriété CommentBegin2: début de commentaire multi-lignes
	//!
	//! Cette propriété définit la chaîne de caractères indiquant les
	//! commentaires sur plusieurs lignes. Le commentaire se termine lorsque la
	//! chaîne @b CommentEnd2 est rencontrée.
	//!
	//! @sa CommentEnd2
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, CommentBegin2);

  //---------------------------------------------------------------------------
  //! @brief Propriété CommentEnd2: fin de commentaire multi-lignes
	//!
	//! Cette propriété définit la chaîne de caractères indiquant la fin des
	//! commentaires sur plusieurs lignes.
	//!
	//! @sa CommentBegin2
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, CommentEnd2);

  //---------------------------------------------------------------------------
  //! @brief Propriété BlockBegin: début de bloc
	//!
	//! Cette propriété définit la chaîne de caractères indiquant le début d'un
	//! bloc (et provoquant généralement l'incrémentation de l'indentation dans
	//! la plupart des langages).
	//!
	//! @sa BlockEnd
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, BlockBegin);

  //---------------------------------------------------------------------------
  //! @brief Propriété BlockEnd: fin de bloc
	//!
	//! Cette propriété définit la chaîne de caractères indiquant la fin d'un
	//! bloc (et provoquant généralement la décrémentation de l'indentation dans
	//! la plupart des langages).
	//!
	//! @sa BlockBegin
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, BlockEnd);

  //---------------------------------------------------------------------------
  //! @brief Propriété PreProc: instruction pré-processeur
	//!
	//! Cette propriété définit la chaîne de caractères indiquant une instruction
	//! pré-processeur. L'instruction pré-processeur se termine au premier
	//! caractère de retour à la ligne.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, PreProc);

  //---------------------------------------------------------------------------
  //! @brief Propriété QuoteBegin: début de chaîne de caractères
	//!
	//! Cette propriété définit la chaîne de caractères indiquant le début d'une
	//! chaîne de caractères littérale.
	//!
	//! @sa QuoteEnd QuoteBegin2 QuoteEnd2 QuoteBegin3 QuoteEnd3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TCHAR, QuoteBegin);

  //---------------------------------------------------------------------------
  //! @brief Propriété QuoteEnd: fin de chaîne de caractères
	//!
	//! Cette propriété définit la chaîne de caractères indiquant la fin d'une
	//! chaîne de caractères littérale.
	//!
	//! @sa QuoteBegin QuoteBegin2 QuoteEnd2 QuoteBegin3 QuoteEnd3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TCHAR, QuoteEnd);

  //---------------------------------------------------------------------------
  //! @brief Propriété QuoteBegin2: début de chaîne de caractères
	//!
	//! Cette propriété définit la chaîne de caractères indiquant le début d'une
	//! chaîne de caractères littérale.
	//!
	//! @sa QuoteBegin QuoteEnd QuoteEnd2 QuoteBegin3 QuoteEnd3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TCHAR, QuoteBegin2);

  //---------------------------------------------------------------------------
  //! @brief Propriété QuoteEnd2: fin de chaîne de caractères
	//!
	//! Cette propriété définit la chaîne de caractères indiquant la fin d'une
	//! chaîne de caractères littérale.
	//!
	//! @sa QuoteBegin QuoteEnd QuoteBegin2 QuoteBegin3 QuoteEnd3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TCHAR, QuoteEnd2);

  //---------------------------------------------------------------------------
  //! @brief Propriété QuoteBegin3: début de chaîne de caractères
	//!
	//! Cette propriété définit la chaîne de caractères indiquant le début d'une
	//! chaîne de caractères littérale.
	//!
	//! @sa QuoteBegin QuoteEnd QuoteBegin2 QuoteEnd2 QuoteEnd3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TCHAR, QuoteBegin3);

  //---------------------------------------------------------------------------
  //! @brief Propriété QuoteEnd3: fin de chaîne de caractères
	//!
	//! Cette propriété définit la chaîne de caractères indiquant la fin d'une
	//! chaîne de caractères littérale.
	//!
	//! @sa QuoteBegin QuoteEnd QuoteBegin2 QuoteEnd2 QuoteBegin3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, TCHAR, QuoteEnd3);

  //---------------------------------------------------------------------------
  //! @brief Propriété EscapeString: caractère d'échappement
	//!
	//! Cette propriété définit la chaîne de caractères utilisée pour indiquer
	//! que le ou les caractères suivants ne doivent pas être interprétés comme
	//! une fin de chaîne littérale.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, EscapeString);

  //---------------------------------------------------------------------------
  //! @brief Propriété MultiLine: instruction sur plusieurs lignes
	//!
	//! Cette propriété définit la chaîne de caractères utilisée pour indiquer
	//! que l'instruction courante se continue sur la ligne suivante.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, MultiLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété Associated1: fichier associé
	//!
	//! Cette propriété définit l'extension associée au type de fichier. Par
	//! exemple, les fichiers de type ".cpp" ont comme extension associée ".h".
	//! Lorsqu'on fait un clic droit sur l'onglet d'un fichier cpp, le programme
	//! proposera d'ouvrir le fichier h correspondant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, Associated1);

  //---------------------------------------------------------------------------
  //! @brief Propriété Associated2: deuxième fichier associé
	//!
	//! Cette propriété définit une deuxième extension associée au type de
	//! fichier.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTextEditor, AnsiString, Associated2);

  //---------------------------------------------------------------------------
  //! @brief Propriété Tooltip
	//!
	//! Cette propriété permet de lire l'objet Tooltip. Cet objet représente
	//! l'info-bulle (mais est un peu différent de Hint dans le fonctionnement).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TTextEditor, TTooltip *, Tooltip);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnCaretMove: déplacement du curseur texte
	//!
	//! Cet évènement est déclenché lorsque le curseur texte (caret) change de
	//! position. Ceci permet d'afficher les numéros de ligne et de colonne, par
	//! exemple. Les deux paramètres indiquent le numéro de colonne et de ligne
	//! (int commençant à 1).
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnCaretMove, OnCaretMove, TObject *, int, int);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnAutocompletionOn: début de l'autocomplétion
	//!
	//! Cet évènement est déclenché lorsque les caractères correspondants à
	//! l'autocomplétion viennent d'être tapés par l'utilisateur. Les deux
	//! premiers paramètres indiquent les coordonnées où afficher la fenêtre
	//! d'autocomplétion, le troisième paramètre indique quelle chaîne
	//! d'autocomplétion a été reconnue.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//! @param[in]        Arg2 Objet parent des attributs, propriétés ou méthodes à proposer
  //! @param[in]        Arg3 Position en X de la fenêtre
  //! @param[in]        Arg4 Position en Y de la fenêtre
  //! @param[in]        Arg5 Type d'autocomplétion reconnu
	//!
	//! @sa OnAutocompletionOff Autocompletion Autocompletion1 Autocompletion2
	//! Autocompletion3
  //---------------------------------------------------------------------------

  DECLARE_EVENT_5(TOnAutocompletionOn, OnAutocompletionOn, TObject *, AnsiString, int, int, int);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnAutocompletionOff: fin de l'autocomplétion
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur sort de
	//! l'autocompletion.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//!
	//! @sa OnAutocompletionOn Autocompletion Autocompletion1 Autocompletion2
	//! Autocompletion3
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnAutocompletionOff, OnAutocompletionOff, TObject *);

  //@}

};


//---------------------------------------------------------------------------
#endif
