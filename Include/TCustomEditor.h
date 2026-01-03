//-----------------------------------------------------------------------------
//! @file TCustomEditor.h
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

#ifndef TCustomEditorH
#define TCustomEditorH

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <vector>
#include <TUndoRedo.h>


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum TYPESYNTAX {
	tsUndefined = -1,					//!< Indéfini
	tsNormal,									//!< Texte normal
	tsSelect,									//!< Texte sélectionné
	tsKeyWord,								//!< Mots clé
	tsPreProc,								//!< Instructions pré-processeur
	tsNumber,									//!< Nombres
	tsString,									//!< Chaînes
	tsComment,								//!< Commentaires
	tsDisassembly,						//!< Code désassemblé
	tsLinesNumbers,						//!< Numéros de ligne
	tsIconsColumn,						//!< Colonne d'icônes
};

enum TEditorLinesFlag {
	lfBookmark,						    //!< Marque-pages
	lfBreakpoint,						  //!< Point d'arrêt
	lfCurStatement						//!< Instruction courante
};

typedef Set<TEditorLinesFlag, lfBookmark, lfCurStatement> TEditorLinesFlags;

#define MIN_TYPESYNTAX -1								//!< Première valeur de l'enum TYPESYNTAX
#define MAX_TYPESYNTAX tsIconsColumn		//!< Dernière valeur de l'enum TYPESYNTAX

typedef struct {
  TColor Background;
  TColor Foreground;
	bool bBold;
	bool bItalic;
} COLORPARAM, *LPCOLORPARAM;

typedef struct {
  bool WholeWord;					//!< Mot entier
  bool CaseSensitive;			//!< Différentie les minuscules et les majuscules
  bool BegLine;						//!< Recherche en début de ligne
  bool EndLine;						//!< Recherche en fin de ligne
  bool OnlyInComments;		//!< Recherche uniquement dans les commentaires
  bool NotInComments;			//!< Recherche uniquement hors des commentaires
  bool OnlyInQuotes;			//!< Recherche uniquement dans les guillemets
  bool NotInQuotes;				//!< Recherche uniquement hors des guillemets
  bool RegExp;						//!< Expression régulière
  bool KeepCase;					//!< Conserver la casse
	bool Down;							//!< Recherche vers le bas
	bool SinceCursor;				//!< Recherche depuis le curseur
	bool InSelection;				//!< Recherche uniquement dans la sélection
	bool Loop;							//!< Continue la recherche en début lorsque la fin de fichier est atteinte
} SEARCHPARAM, *LPSEARCHPARAM;

#define UR_MOVECUR 0
#define UR_MODIF   1


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnMessage.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnMessage)(TObject *Sender, const AnsiString &Message);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnFlagChanged.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnFlagChanged)(TObject *Sender, bool Changed);


//---------------------------------------------------------------------------
//! @class TCustomEditor
//!
//! @brief Classe de base des editeurs
//!
//! Cette classe (@c TCustomEditor) est la classe de base permettant de créer
//! des éditeurs (de texte, ou hexadécimal, par exemple). Elle met en commun
//! les propriétés, évènements et méthodes communes (coloration, police de
//! caractères, etc...) et définit des méthodes virtuelles pures que les
//! classes dérivées devront implémenter (presse-papier, recherche,
//! undo/redo).
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TCustomEditor: public TCustomControl, protected TUndoRedoUser {
private:
protected:
  TUndoRedo *UndoRedo;

  int tmAveCharWidth;        //!< Largeur des caractères
  int tmHeight;              //!< Hauteur des caractères
  int tmInternalLeading;     //!< Intervalle entre lignes
	int NbBeginUpdate;				 //!< Incrémenté de 1 entre un BeginUpdate et un EndUpdate
	bool bMustRefresh;				 //!< true s'il faut faire un refresh après le dernier EndUpdate

	COLORPARAM FColorParams[MAX_TYPESYNTAX + 1];  // tsNormal = 0 à tsIconsColumn
	bool FChanged;

  //---------------------------------------------------------------------------
  //! @brief Demande de dessin du composant, ou bien mise à jour du flag bMustRefresh
  //---------------------------------------------------------------------------

  bool FASTCALL InvalidateOrSetFlag(void);

  //---------------------------------------------------------------------------
  //! @brief Définition de la couleur du texte en fonction du type
  //---------------------------------------------------------------------------

	bool FASTCALL SetColorSyntax(TCanvas *ACanvas, TYPESYNTAX Type);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCustomEditor(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCustomEditor(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture depuis un fichier
  //!
  //! Cette fonction charge en mémoire le contenu d'un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à lire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL LoadFromFile(const AnsiString FileName) = 0;

  //---------------------------------------------------------------------------
  //! @brief Enregistrement dans un fichier
  //!
  //! Cette fonction enregistre les données dans un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à écrire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------

	virtual bool FASTCALL SaveToFile(const AnsiString FileName) = 0;

  //---------------------------------------------------------------------------
  //! @brief Couper et copier dans le presse-papier
  //!
  //! Cette méthode copie le contenu de la sélection dans le presse-papiers,
	//! puis l'efface. Les modifications sont mémorisées dans le buffer undo/redo
	//! pour une annulation éventuelle.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL CutToClipboard(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Copier dans le presse-papier
  //!
  //! Cette méthode copie le contenu de la sélection dans le presse-papiers.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL CopyToClipboard(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Coller depuis le presse-papier
  //!
  //! Cette méthode colle le contenu du presse-papiers à la position du
	//! curseur. Les modifications sont mémorisées dans le buffer undo/redo pour
	//! une annulation éventuelle.
  //---------------------------------------------------------------------------

  virtual bool FASTCALL PasteFromClipboard(void) = 0;

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
  virtual bool FASTCALL Search(AnsiString asSearch, LPSEARCHPARAM lpSearchParam) = 0;

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
  virtual bool FASTCALL Replace(AnsiString asReplace, bool bKeepCase) = 0;

  //---------------------------------------------------------------------------
  //! @brief Efface tout le texte du contrôle de saisie.
  //!
  //! Cette méthode met à zéro l'ensemble du texte. Cette action n'est pas
	//! mémorisée dans le buffer Undo/Redo.
  //---------------------------------------------------------------------------

  virtual void FASTCALL Clear(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Copie le texte sélectionné dans une chaîne.
  //!
  //! Cette fonction copie le contenu de la sélection dans un buffer de type
	//! texte.
  //!
  //! @return @c AnsiString Contenu de la sélection. Les lignes sont séparées
	//! par le caractère LF.
  //---------------------------------------------------------------------------

	virtual AnsiString FASTCALL GetSelText(void) = 0;

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

	virtual int FASTCALL GetSelTextBuf(TCHAR * Buffer, int BufSize) = 0;

  //---------------------------------------------------------------------------
  //! @brief Remplace le texte sélectionné par une chaîne à zéro terminal.
  //!
  //! Cette méthode efface la sélection, puis insère le texte passé en
	//! paramètre. La sélection comme le texte peuvent être multilignes.
  //!
  //! @param[in]        Buffer Texte à insérer.
  //---------------------------------------------------------------------------

  virtual void FASTCALL SetSelTextBuf(const TCHAR * Buffer) = 0;

  //---------------------------------------------------------------------------
  //! @brief Sélectionne tout le texte.
  //!
  //! Cette méthode sélectionne l'ensemble du texte.
  //---------------------------------------------------------------------------

  virtual void FASTCALL SelectAll(void) = 0;

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
	//! TCustomEditor::BeginUpdate). Si les modifications qui ont été faites
	//! ont un effet sur l'affichage, la fenêtre est raffraichie à ce moment.
  //---------------------------------------------------------------------------

	virtual void FASTCALL EndUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief ClearUndo
  //!
  //! Cette méthode efface les opérations mémorisées dans le fichier undo/redo.
  //---------------------------------------------------------------------------

  void FASTCALL ClearUndo(void);

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
  //! Cette fonction effectue une opération undo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Undo(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette fonction effectue une opération redo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL Redo(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Liste des trois premières modifications
  //!
  //! Cette méthode renvoie le libellé des premières modifications
  //!
  //! @return @c AnsiString Liste des modifications (séparées par un retour à
	//! la ligne)
  //---------------------------------------------------------------------------
  AnsiString GetModificationsList(void);

  //---------------------------------------------------------------------------
  //! @brief Traitement d'un redimensionnement de la fenêtre.
  //!
  //! Cette méthode est appelée lorsque les dimensions de la fenêtre ou de la
	//! font ont changé.
  //---------------------------------------------------------------------------

  virtual void FASTCALL ProcessResize(void) = 0;

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
  //! @brief Propriété Changed
	//!
	//! Détermine si le texte a été changé. Cette propriété passe à true lorsque
	//! l'utilisateur effectue une modification, et repasse à false s'il fait un
	//! Undo des modifications. Le changement de cette propriété est toujours
	//! fait avant les évènements OnChange et OnFlagChanged.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomEditor, bool, Changed);

  //---------------------------------------------------------------------------
  //! @brief Propriété ColorParams: paramètres de couleur
	//!
	//! Cette propriété définit les couleurs de l'éditeur par catégories
	//! (mots-clés, commentaires, etc...).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TCustomEditor, COLORPARAM, ColorParams);

  //---------------------------------------------------------------------------
  //! @brief Propriété Font
	//!
	//! Cette propriété définit la police de caractères de l'éditeur. Veillez à
	//! ce que la police définie aie une largeur de caractères fixe.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCustomEditor, TCustomFont *, Font);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnMessage
	//!
	//! Cet évènement est déclenché lorsque l'éditeur a besoin d'afficher un
	//! message. Les principaux messages sont liés au undo/redo. Le message est
	//! passé en paramètre (AnsiString).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Message à afficher
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnMessage, OnMessage, TObject *, const AnsiString &);

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
	//! Ceci permet de mettre en évidence les fichiers modifiés. Le second
	//! paramètre est la nouvelle valeur du flag @b Changed;
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Flag Changed
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnFlagChanged, OnFlagChanged, TObject *, bool);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnPassBegin
	//!
	//! Cet évènement est déclenché lorsque le curseur passe le début de fichier,
	//! soit lors d'une recherche vers le haut, soit à l'exécution d'une macro.
	//! Ceci permet d'arrêter la macro ou d'afficher un message.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnPassBegin, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnPassEnd
	//!
	//! Cet évènement est déclenché lorsque le curseur passe la fin de fichier,
	//! soit lors d'une recherche vers le bas, soit à l'exécution d'une macro.
	//! Ceci permet d'arrêter la macro ou d'afficher un message.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnPassEnd, TObject *);

  //@}

};


//---------------------------------------------------------------------------
#endif
