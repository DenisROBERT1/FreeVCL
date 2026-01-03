//-----------------------------------------------------------------------------
//! @file THexaEditor.h
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

#ifndef THexaEditorH
#define THexaEditorH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <vector>
#include <TCustomEditor.h>


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum TYPEUNDOHEXA {
	tuhNull,						          //!< Aucun
	tuhMoveCur,										//!< Déplacement du curseur texte
	tuhText,						          //!< Ajout de texte
	tuhDelete,						        //!< Suppression de texte
	tuhReplace,						        //!< Remplacement
	tuhExterne						        //!< Opération externe
};

//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnCaretMove2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnCaretMove2)(TObject *Sender, __int64 Pos);


//---------------------------------------------------------------------------
//! @class THexaEditor
//!
//! @brief Editeur de fichier texte
//!
//! Cette classe (@c THexaEditor) permet de visualiser et d'éditer un fichier
//! binaire quelconque (texte ou pas). Elle comprend des méthodes de
//! recherche, de sélection, de modification de lignes, dans le but de faire
//! aisément un programme d'édition. Les couleurs, la police de caractères,
//! peuvent être paramétrés. Les actions sont mémorisées pour pouvoir
//! implémenter aisément un mécanisme de undo/redo.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class THexaEditor: public TCustomEditor {
private:
protected:
  bool bCaret;               //!< true si le caret est visible
  bool bMouseCapture;        //!< true si sélection en cours
  bool bReadOnly;            //!< true si readonly
  bool bInsert;              //!< true si mode insertion
  bool bDblClick;            //!< true si double-clic ( => ignorer le click qui suit)
  int FFirstLine;            //!< Première ligne affichée
  int NbLinesWin;            //!< Nombre de lignes visibles (y compris les lignes coupées)
  int NbColsWin;             //!< Nombre de colonnes visibles
	int NbBytesPerLine;        //!< Nombre d'octets affichés par ligne
  int LinesCount;            //!< Nombre de lignes visibles (y compris les lignes coupées)
  __int64 FilePosBegSel;     //!< Position de début de la sélection
  __int64 FilePosEndSel;     //!< Position de fin de la sélection
	bool bRightColumn;         //!< true si le caret est dans la partie droite
	bool bHighValue;           //!< true si le caret est sur la valeur haute de l'octet
  bool FIconsColumn;				 //!< true si les icônes d'état sont affichées

	__int64 FFilePos;					 //!< Position de l'octets affiché
	__int64 FileSize;					 //!< Taille totale du fichier
	__int64 AllocatedSize;		 //!< Taille allouée en mémoire
	char *FileContent;				 //!< Contenu du fichier
  bool FUndoRedoMoveCur;
	std::vector<__int64> Bookmarks;  //! @brief Marque-pages

  void FASTCALL UpdateVScroll();

  //---------------------------------------------------------------------------
  //! @brief Suppression d'une partie de texte dans une ligne.
  //!
  //! Cette méthode supprime une partie du fichier. les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        AFilePos position dans le fichier à modifier
  //! @param[in]        Count Nombre de caractères à supprimer
  //!
  //! @return @c int valeur du paramètre Index
  //---------------------------------------------------------------------------

	void DeleteString(__int64 AFilePos, __int64 Count);

  //---------------------------------------------------------------------------
  //! @brief Insertion d'un texte dans le fichier.
  //!
  //! Cette méthode insère un texte dans le fichier. les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        AFilePos position dans le fichier à modifier
  //! @param[in]        Count Nombre de caractères à insérer
  //! @param[in]        str Texte à insérer
  //---------------------------------------------------------------------------

	void InsertString(__int64 AFilePos, __int64 Count, char *str);

  //---------------------------------------------------------------------------
  //! @brief Modification d'un texte dans le fichier.
  //!
  //! Cette méthode modifie un texte dans le fichier. les modifications sont
	//! mémorisées dans le buffer undo/redo pour une annulation éventuelle.
  //!
  //! @param[in]        AFilePos position dans le fichier à modifier
  //! @param[in]        Count Nombre de caractères à modifier
  //! @param[in]        str Texte à enregistrer
  //---------------------------------------------------------------------------

	void ChangeString(__int64 AFilePos, __int64 Count, char *str);

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
  // Dessin d'une ligne
  //---------------------------------------------------------------------------

  void FASTCALL DrawLine(TCanvas *ACanvas, int NumLine);

	//---------------------------------------------------------------------------
	// Sélection ordonnée
	//---------------------------------------------------------------------------
	void GetSelOrdered(__int64 *FilePosBegSel2, __int64 *FilePosEndSel2);

	//---------------------------------------------------------------------------
	//! @brief Teste si le caractère (AFilePos) est dans la sélection.
  //!
  //! Cette méthode teste si un caractère est dans la sélection.
  //!
  //! @param[in]        AFilePos Position dans le fichier
  //! @param[in]        bARightColumn True si le curseur est dans la colonne de droite
  //!
  //! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	bool IsInSelection(__int64 AFilePos, bool bARightColumn);

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
  bool SetTypeUndoRedo(TYPEUNDOHEXA TypeUndo, AnsiString asOperationName = "");
  TYPEUNDOHEXA CurrentTypeUndo;

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
  void FASTCALL HexaEditorResize(TObject *Sender);
  void FASTCALL HexaEditorEnter(TObject *Sender);
  void FASTCALL HexaEditorExit(TObject *Sender);
  void FASTCALL HexaEditorKeyDown(TObject *Sender,
        WORD Key, TShiftState Shift);
  void FASTCALL HexaEditorKeyPress(TObject *Sender, TCHAR Key);
  void FASTCALL HexaEditorMouseDown(
        TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
        int Y);
  void FASTCALL HexaEditorMouseUp(TObject *Sender,
        TMouseButton Button, TShiftState Shift, int X, int Y);
  void FASTCALL HexaEditorMouseMove(
        TObject *Sender, TShiftState Shift, int X, int Y);
  void FASTCALL HexaEditorMouseWheel(TObject *Sender, TShiftState Shift,
        int WheelDelta, const TPoint &MousePos, bool &Handled);
  void FASTCALL HexaEditorDblClick(TObject *Sender);
  void FASTCALL VertScrollBarScroll(TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(THexaEditor, TNotifyEvent, OnResize, HexaEditorResize, TObject *);
		SET_EVENT_STR_1(THexaEditor, TOnEnter, OnEnter, HexaEditorEnter, TObject *);
		SET_EVENT_STR_1(THexaEditor, TOnExit, OnExit, HexaEditorExit, TObject *);
		SET_EVENT_STR_3(THexaEditor, TOnKeyDown, OnKeyDown, HexaEditorKeyDown, TObject *, WORD, TShiftState);
		SET_EVENT_STR_2(THexaEditor, TOnKeyPress, OnKeyPress, HexaEditorKeyPress, TObject *, TCHAR);
		SET_EVENT_STR_5(THexaEditor, TOnMouseDown, OnMouseDown, HexaEditorMouseDown, TObject *, TMouseButton, TShiftState, int, int);
		SET_EVENT_STR_5(THexaEditor, TOnMouseUp, OnMouseUp, HexaEditorMouseUp, TObject *, TMouseButton, TShiftState, int, int);
		SET_EVENT_STR_4(THexaEditor, TOnMouseMove, OnMouseMove, HexaEditorMouseMove, TObject *, TShiftState, int, int);
		SET_EVENT_STR_5(THexaEditor, TOnMouseWheel, OnMouseWheel, HexaEditorMouseWheel, TObject *, TShiftState, int, const TPoint &, bool &);
		SET_EVENT_STR_1(THexaEditor, TNotifyEvent, OnDblClick, HexaEditorDblClick, TObject *);
		SET_EVENT_STR_3(THexaEditor, TOnScroll, OnScroll, VertScrollBarScroll, TObject*, TScrollCode, int &);
  }

  FVCL_END_EVENTS


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  THexaEditor(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~THexaEditor(void);

  //@}


  //! @name Méthodes
  //@{

	//---------------------------------------------------------------------------
  //! @brief Déplacement du curseur
  //!
  //! Cette fonction déplace le curseur texte (le caret) à la position
	//! (NewFilePos). Si les flags bSelection est activé, le texte entre
	//! l'ancienne position et la nouvelle est sélectionné.
  //!
  //! @param[in]        NewFilePos Nouvelle position dans le fichier
  //! @param[in]        bNewHighValue Nouvelle position dans l'octet
  //! @param[in]        bSelection Active la sélection du texte
  //---------------------------------------------------------------------------

	void FASTCALL MoveCur(__int64 NewFilePos, bool bNewHighValue, bool bSelection);

	//---------------------------------------------------------------------------
  //! @brief Position dans le fichier correspondante aux coordonnées X, Y
  //!
  //! Cette fonction renvoie la position dans le fichier correspondante au
	//! point de coordonnées (X, Y). Le calcul tient compte des scrolls, de
	//! la taille de la police, etc... Les coordonnées X, Y sont des coordonnées
	//! client (par rapport au point supérieur gauche de la fenêtre).
  //!
  //! @param[in]        Y Coordonnée Y
  //! @param[in]        X Coordonnée X
  //! @param[out]       AFilePos Position dans le fichier
  //! @param[out]       bRightColumn true si l'utilisateur a cliqué dans la
	//!                   colonne de droite.
  //! @param[out]       bHighValue true si l'utilisateur a cliqué sur la
	//!                   partie haute.
  //---------------------------------------------------------------------------

  void FASTCALL GetPosAt(int Y, int X, __int64 *AFilePos, bool *bRightColumn, bool *bHighValue);

  //---------------------------------------------------------------------------
  //! @brief Lecture depuis un fichier
  //!
  //! Cette fonction charge en mémoire le contenu d'un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à lire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromFile(const AnsiString FileName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un fichier
  //!
  //! Cette fonction enregistre les données dans un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à écrire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL SaveToFile(const AnsiString FileName);

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
  //! @brief Déplacement de la fenêtre de visualisation
  //!
  //! Cette méthode décale la fenêtre de visualisation de manière à rendre
	//! visible la position AFilePos.
  //!
  //! @param[in]        AFilePos Position du fichier à visualiser
  //---------------------------------------------------------------------------
  void FASTCALL ScrollInView(__int64 AFilePos);

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
  //! Cette fonction copie le contenu de la sélection dans un buffer de type
	//! texte.
  //!
  //! @return @c AnsiString Contenu de la sélection. Les lignes sont séparées
	//! par le caractère LF.
  //---------------------------------------------------------------------------

	virtual AnsiString FASTCALL GetSelText(void);

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
	//! THexaEditor::BeginUpdate). Si les modifications qui ont été faites
	//! ont un effet sur l'affichage, la fenêtre est raffraichie à ce moment.
  //---------------------------------------------------------------------------

	virtual void FASTCALL EndUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Renvoie la valeur d'un flag
  //!
  //! Cette méthode renvoie true si le flag Flag a été mis à la position
	//! AFilePos.
  //!
  //! @param[in]        AFilePos Position à tester
  //! @param[in]        Flag Flag à tester
  //!
  //! @return @c bool true si positionné
  //---------------------------------------------------------------------------

	bool GetFlag(__int64 AFilePos, TEditorLinesFlag Flag);

  //---------------------------------------------------------------------------
  //! @brief Positionne un flag
  //!
  //! Cette méthode positionne un flag à la position AFilePos.
  //!
  //! @param[in]        AFilePos Position du flag
  //! @param[in]        Flag Flag à positionner
  //! @param[in]        bSet true si positionné
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	bool SetFlag(__int64 AFilePos, TEditorLinesFlag Flag, bool bSet);

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
  bool FASTCALL BeginOperation(AnsiString asOperationName);

  //---------------------------------------------------------------------------
  //! @brief Fin d'une opération pouvant être défaite ou refaite
  //!
  //! Fin d'une opération pouvant être défaite ou refaite. Chaque EndOperation
  //! doit correspondre à un BeginOperation.
  //---------------------------------------------------------------------------
  bool FASTCALL EndOperation(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération undo
  //!
  //! Cette fonction effectue une opération undo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Undo(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette fonction effectue une opération redo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Redo(void);

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
  //! @brief Propriété FilePos: position dans le fichier
	//!
	//! Cette propriété définit la position du curseur dans le fichier (en nombre
	//! de caractères.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THexaEditor, __int64, FilePos);

  //---------------------------------------------------------------------------
  //! @brief Propriété FirstLine: première ligne affichée
	//!
	//! Cette propriété définit la première ligne affichée.
	//!
	//! @sa SetFirstLine
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THexaEditor, int, FirstLine);

  //---------------------------------------------------------------------------
  //! @brief Propriété IconsColumn: colonne des icônes visible
	//!
	//! Cette propriété détermine si la colonne contenant les icônes
	//! (marque-pages par exemple) est visible ou pas. Si elle n'est pas visible,
	//! cela n'empêche pas de positionner des marque-pages.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THexaEditor, bool, IconsColumn);

	//---------------------------------------------------------------------------
  //! @brief Propriété SelText: Texte sélectionné
	//!
	//! Cette propriété permet de lire le texte sélectionné. Selon que le curseur
	//! est sur la partie gauche ou droite de l'éditeur, la propriété renvoie le
	//! texte brut ou bien le texte en format hexadécimal.
	//!
	//! @sa GetSelText
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THexaEditor, AnsiString, SelText);

  //---------------------------------------------------------------------------
  //! @brief Propriété UndoRedoMoveCur: mémorisation des déplacements curseur
	//!
	//! Cette propriété permet de mémoriser les mouvement de curseur dans le
	//! undo/redo. Sinon, seules les modifications de texte sont mémorisées et
	//! peuvent être défaites et refaites.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(THexaEditor, bool, UndoRedoMoveCur);

  //@}

  //! @name Evènements
  //@{

	  //---------------------------------------------------------------------------
  //! @brief Evènement OnCaretMove
	//!
	//! Cet évènement est déclenché lorsque le curseur texte (caret) change de
	//! position. Ceci permet d'afficher la position dans le fichier, par
	//! exemple. Les deux paramètres indiquent le numéro de colonne et de ligne
	//! (int commençant à 1).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nouvelle position dans le fichier
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnCaretMove2, OnCaretMove, TObject *, __int64);


  //@}

};


//---------------------------------------------------------------------------
#endif
