//-----------------------------------------------------------------------------
//! @file TFrame_SaisieDialog.h
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

#ifndef TFrame_SaisieDialogH
#define TFrame_SaisieDialogH

//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------

#include <vector>
#include <TUndoRedo.h>
#include "TFrame_FVCLIDEBase.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TElemBase;
class TElemVCL;
class TForm_Proprietes;


//---------------------------------------------------------------------------
// Macros, enums
//---------------------------------------------------------------------------

#define X_PIXEL(x) ((int) (((double) ((x) * baseunitX) / 4) + .5))        // Taille des contrôles dans les boîtes de dialogues
#define Y_PIXEL(y) ((int) (((double) ((y) * baseunitY) / 8) + .5))

#define X_DLG(x) ((int) (((double) ((x) * 4) / baseunitX) + .5))
#define Y_DLG(y) ((int) (((double) ((y) * 8) / baseunitY) + .5))

enum MODECONSTR {mcNull, mcSelect, mcCreat};


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

enum ACTIONUNDODIAL {
	auSelection,
	auCreateDialog,
	auDeleteDialog,
	auCreateObj,
	auDeleteObj,
	auResize,
	auChangeLeft,
	auChangeTop,
	auChangeWidth,
	auChangeHeight,
	auChangeAnchors,
	auChangeProperty,
	auSwap,
	auMove
};

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int Index;
} UNDODIALSELECTION, *LPUNDODIALSELECTION;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int IndexDialog;
	int NumeroElement;
	int Left;
  int Top;
	int Width;
  int Height;
} UNDODIALCREATEDIALOG, *LPUNDODIALCREATEDIALOG;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int IndexDialog;
} UNDODIALDELETEDIALOG, *LPUNDODIALDELETEDIALOG;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int IndexDialog;
	int IndexParent;
	int NumeroElement;
	int Left;
  int Top;
	int Width;
  int Height;
  TAnchors Anchors;
	char szTypeObj[30];
} UNDODIALCREATEOBJ, *LPUNDODIALCREATEOBJ;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int Index;
} UNDODIALDELETEOBJ, *LPUNDODIALDELETEOBJ;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int Index;
  int Value;
} UNDODIALCHANGEVALUE, *LPUNDODIALCHANGEVALUE;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int Index;
	char szPropertyName[50];
	TCHAR szPropertyValue[1];
} UNDODIALCHANGEPROPERTY, *LPUNDODIALCHANGEPROPERTY;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int IndexDialog;
	int Left;
  int Top;
	int Width;
  int Height;
} UNDODIALRESIZE, *LPUNDODIALRESIZE;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int Index1;
	int Index2;
} UNDODIALSWAP, *LPUNDODIALSWAP;

typedef struct {
  ACTIONUNDODIAL ActionUndoDial;
	int Index;
	int IndexParent;
	int IndexInsertAfter;
} UNDODIALMOVE, *LPUNDODIALMOVE;

// DIALOGUE
typedef struct {
  TElemVCL *Form;
	TTreeNode *TreeNode;
} DIALOGUE;

//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnNewFormInit.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnNewFormInit)(TObject *Sender, TElemBase *Form);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnNewComponentInit.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnNewComponentInit)(TObject *Sender, TElemBase *Form, TElemBase *Parent, TElemBase *Component);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnChangeSelection.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnChangeSelection)(TObject *Sender);


//---------------------------------------------------------------------------
class TFrame_SaisieDialog : public TFrame_FVCLIDEBase, public TUndoRedoUser {
private:
protected:
  TUndoRedo *FUndoRedo;

  TObject *CliquedControl;
  std::vector<DIALOGUE> Dialogues;
  int SelectedForm;
  int NbComponents;
	bool bNotTreeView_FormsChange;

  // Création d'objets
  MODECONSTR FModeConstr;
	TElemBase *FObjSelected;
  int NumeroElement;

  AnsiString FTypeACreer;

  void Clear(void);
  bool ChangePropriete(TElemBase *Element, AnsiString asProperty, AnsiString asValue);

  virtual bool ProcessUndo(void * Undo, int LenUndo);
  virtual bool ProcessRedo(void * Redo, int LenRedo);

public:
  FVCL_BEGIN_COMPONENTS
  TImageList *ImageList_Popup;
  TMenuItem *MenuItem_AlignGrille;
  TMenuItem *MenuItem_UnifWidth;
  TMenuItem *MenuItem_UnifHeight;
  TMenuItem *MenuItem_UnifSize;
  TMenuItem *MenuItem_Unif;
  TMenuItem *MenuItem_AlignDepLeft;
  TMenuItem *MenuItem_AlignDepCenterH;
  TMenuItem *MenuItem_AlignDepRight;
  TMenuItem *MenuItem_AlignDepTop;
  TMenuItem *MenuItem_AlignDepCenterV;
  TMenuItem *MenuItem_AlignDepBottom;
  TMenuItem *MenuItem_AlignDep;
  TMenuItem *MenuItem_AlignDimLeft;
  TMenuItem *MenuItem_AlignDimRight;
  TMenuItem *MenuItem_AlignDimTop;
  TMenuItem *MenuItem_AlignDimBottom;
  TMenuItem *MenuItem_AlignDim;
  TMenuItem *MenuItem_AlignButtonsBottom;
  TMenuItem *MenuItem_AlignButtonsRight;
  TMenuItem *MenuItem_AlignButtons;
	TMenuItem *MenuItem_Aspect;
  TMenuItem *MenuItem_Sep1;
  TMenuItem *MenuItem_Couper;
  TMenuItem *MenuItem_Copier;
  TMenuItem *MenuItem_Coller;
  TMenuItem *MenuItem_Sep2;
  TMenuItem *MenuItem_Supprimer;
  TMenuItem *MenuItem_Sep3;
  TMenuItem *MenuItem_MettreAvant;
  TMenuItem *MenuItem_MettreApres;
  TMenuItem *MenuItem_MettreEnfant;
  TMenuItem *MenuItem_MettreFrere;
  TMenuItem *MenuItem_Sep4;
  TMenuItem *MenuItem_Proprietes;
  TPopupMenu *PopupMenu_ModifObjet;
  TLabel *Label_Dialogues1;
  TTreeView *TreeView_Forms;
  TLabel *Label_Dialogues2;
  TLabel *Label_NbDialogues;
  TLabel *Label_Controles;
  TLabel *Label_NbControles;
  TPanel *Panel1;
  TSplitter *Splitter;
  TLabel *Label_ObjToCreate;
  TLabel *Label_ObjName;
  TPageControl *PageControl_ObjToCreate;
  TBitBtn *BitBtn_Select;
  TCheckBox *CheckBox_Aligner;
  TCheckBox *CheckBox_Afficher;
  TLabel *Label_GridSizeX;
  TEdit *Edit_GridSizeX;
  TUpDown *UpDown_GridSizeX;
  TLabel *Label_GridSizeY;
  TEdit *Edit_GridSizeY;
  TUpDown *UpDown_GridSizeY;
	TGroupBox *GroupBox_Grid;
  TCheckBox *CheckBox_OrdreTab;
	TButton *Button_Renumeroter;
  TButton *Button_MettreAvant;
  TButton *Button_MettreApres;
	TButton *Button_MettreEnfant;
	TButton *Button_MettreFrere;
  TButton *Button_Aide;
  TPanel *Panel2;
  FVCL_END_COMPONENTS

  FVCL_BEGIN_EVENTS
  void FASTCALL Frame_SaisieDialogShow(TObject *Sender);
  void FASTCALL Frame_SaisieDialogHide(TObject *Sender);
  void FASTCALL BitBtn_SelectClick(TObject *Sender);
  void FASTCALL CheckBox_AlignerClick(TObject *Sender);
  void FASTCALL CheckBox_AfficherClick(TObject *Sender);
  void FASTCALL Edit_GridSizeXChange(TObject *Sender);
  void FASTCALL Edit_GridSizeYChange(TObject *Sender);
  void FASTCALL CheckBox_OrdreTabClick(TObject *Sender);
  void FASTCALL Button_RenumeroterClick(TObject *Sender);
  void FASTCALL Button_AideClick(TObject *Sender);
  void FASTCALL TreeView_FormsContextPopup(TObject *Sender, const TPoint &, bool &);
  void FASTCALL TreeView_FormsChange(TObject *Sender, TTreeNode *TreeNodeChanged);
  void FASTCALL TreeView_FormsDblClick(TObject *Sender);
  void FASTCALL TreeView_FormsDragOver(TObject *Sender,
          TObject *Source, int X, int Y, TDragState State, bool &Accept);
  void FASTCALL TreeView_FormsEndDrag(TObject *Sender,
          TObject *Target, int X, int Y);
  void FASTCALL TreeView_FormsEdited(TObject *Sender, TTreeNode *TreeNode, AnsiString asNewName);
  void FASTCALL BitBtn_ObjToCreateClick(TObject *Sender);
  void FASTCALL MenuItem_AlignGrilleClick(TObject *Sender);
  void FASTCALL MenuItem_UnifWidthClick(TObject *Sender);
  void FASTCALL MenuItem_UnifHeightClick(TObject *Sender);
  void FASTCALL MenuItem_UnifSizeClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDepLeftClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDepCenterHClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDepRightClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDepTopClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDepCenterVClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDepBottomClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDimLeftClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDimRightClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDimTopClick(TObject *Sender);
  void FASTCALL MenuItem_AlignDimBottomClick(TObject *Sender);
  void FASTCALL MenuItem_AlignButtonsBottomClick(TObject *Sender);
  void FASTCALL MenuItem_AlignButtonsRightClick(TObject *Sender);
  void FASTCALL MenuItem_SupprimerClick(TObject *Sender);
  void FASTCALL MenuItem_MettreAvantClick(TObject *Sender);
  void FASTCALL MenuItem_MettreApresClick(TObject *Sender);
  void FASTCALL MenuItem_MettreEnfantClick(TObject *Sender);
  void FASTCALL MenuItem_MettreFrereClick(TObject *Sender);
  void FASTCALL MenuItem_ProprietesClick(TObject *Sender);
  void FASTCALL NewFormInit(TObject *Sender, TElemBase * NewForm);
  void FASTCALL NewComponentInit(TObject *Sender, TElemBase * Form,
                        TElemBase * ElemVCLParent, TElemBase * NewElemBase);
	void FASTCALL Form_SaisieDialogChangeSelection(TObject *Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TFrame_SaisieDialog, TOnShow, OnShow, Frame_SaisieDialogShow, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TOnHide, OnHide, Frame_SaisieDialogHide, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, BitBtn_SelectClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, CheckBox_AlignerClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, CheckBox_AfficherClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnChange, Edit_GridSizeXChange, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnChange, Edit_GridSizeYChange, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, CheckBox_OrdreTabClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, Button_RenumeroterClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, Button_AideClick, TObject *);
		SET_EVENT_STR_3(TFrame_SaisieDialog, TOnContextPopup, OnContextPopup, TreeView_FormsContextPopup, TObject *, const TPoint &, bool &);
		SET_EVENT_STR_2(TFrame_SaisieDialog, TOnChange2, OnChange, TreeView_FormsChange, TObject *, TTreeNode *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnDblClick, TreeView_FormsDblClick, TObject *);
		SET_EVENT_STR_6(TFrame_SaisieDialog, TOnDragOver, OnDragOver, TreeView_FormsDragOver, TObject *, TObject *, int, int, TDragState, bool &);
		SET_EVENT_STR_4(TFrame_SaisieDialog, TOnEndDrag, OnEndDrag, TreeView_FormsEndDrag, TObject *, TObject *, int, int);
		SET_EVENT_STR_3(TFrame_SaisieDialog, TOnEdited, OnDblClick, TreeView_FormsEdited, TObject *, TTreeNode *, AnsiString);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, BitBtn_ObjToCreateClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignGrilleClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_UnifWidthClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_UnifHeightClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_UnifSizeClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDepLeftClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDepCenterHClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDepRightClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDepTopClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDepCenterVClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDepBottomClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDimLeftClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDimRightClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDimTopClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignDimBottomClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignButtonsBottomClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_AlignButtonsRightClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_SupprimerClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_MettreAvantClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_MettreApresClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_MettreEnfantClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_MettreFrereClick, TObject *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TNotifyEvent, OnClick, MenuItem_ProprietesClick, TObject *);
		SET_EVENT_STR_2(TFrame_SaisieDialog, TOnNewFormInit, OnNewFormInit, NewFormInit, TObject *, TElemBase *);
		SET_EVENT_STR_4(TFrame_SaisieDialog, TOnNewComponentInit, OnNewComponentInit, NewComponentInit, TObject *, TElemBase *, TElemBase *, TElemBase *);
		SET_EVENT_STR_1(TFrame_SaisieDialog, TOnChangeSelection, OnChangeSelection, Form_SaisieDialogChangeSelection, TObject *);
  }

  FVCL_END_EVENTS

  TFrame_SaisieDialog(TWinControl *AOwner);
  virtual ~TFrame_SaisieDialog(void);
  void SetFormSelected(int Index);

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Recherche de l'objet parent d'un objet
  //!
  //! Cette méthode renvoie l'objet parent d'un objet (zéro si aucun)
  //!
  //! @param[in]        Element Objet enfant
  //!
  //! @return @c TElemBase * Adresse de l'objet parent (0 si objet non trouvé).
  //---------------------------------------------------------------------------
  TElemBase *GetObjParent(TElemBase *Element);

  //---------------------------------------------------------------------------
  //! @brief Recherche de l'élément de TreeView_Forms auquel est associé un objet
  //!
  //! Cette méthode renvoie l'indice du tableau TreeView_Forms auquel est
	//! associé un objet
  //!
  //! @param[in]        Element Objet à rechercher
  //!
  //! @return @c int Indice de l'objet (-1 si non trouvé).
  //---------------------------------------------------------------------------
  int GetIdTreeview(TElemBase *Element);

  //---------------------------------------------------------------------------
  //! @brief Recherche de l'objet associé au noeud d'indice Index
  //!
  //! Cette méthode renvoie l'objet associé au noeud du Treeview qui a l'indice
	//! Index.
  //!
  //! @param[in]        Index Indice du noeud
  //!
  //! @return @c TElemBase * objet trouvé (NULL si non trouvé).
  //---------------------------------------------------------------------------
	TElemBase *GetElemTreeview(int Index);

  //---------------------------------------------------------------------------
  //! @brief Recherche de l'index du dialogue auquel apartient l'objet
  //!
  //! Cette méthode renvoie l'indice du tableau Dialogues dans lequel est inclus un objet
  //!
  //! @param[in]        Element Objet à rechercher
  //!
  //! @return @c int Indice de l'objet (-1 si non trouvé).
  //---------------------------------------------------------------------------
  int GetIndexDialog(TElemBase *Element);

  //---------------------------------------------------------------------------
  //! @brief Méthode appelée en fin de construction du cadre
  //---------------------------------------------------------------------------

  virtual void FASTCALL AfterConstruction(void);

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété DisplayedFileName
  //!
  //! Cette méthode permet de lire la propriété DisplayedFileName
  //!
  //! @return @c AnsiString Propriété DisplayedFileName
  //---------------------------------------------------------------------------

  virtual AnsiString Get_DisplayedFileName(void);

  //---------------------------------------------------------------------------
  //! @brief Régénération de la fenêtre
  //!
  //! Cette méthode provoque une régénération de la fenêtre de visualisation.
  //!
  //! @return @c bool La valeur de retour n'est pas exploitée
  //---------------------------------------------------------------------------
  bool Reconst(void);

  //---------------------------------------------------------------------------
  //! @brief Saisie des propriétés d'un objet
  //!
  //! Cette méthode ouvre une boîte de dialogue permettant de saisir les
  //! caractéristiques d'un objet.
  //!
  //! @param[in]        Element Objet cliqué
  //! @param[in]        Elements Liste des objets à modifier
  //!
  //! @return @c bool true si l'utilisateur a validé les modifs.
  //---------------------------------------------------------------------------
  bool SaisieProprietes(TElemBase *Element, std::vector<TElemBase *> &Elements);

  //---------------------------------------------------------------------------
  //! @brief Création d'un nouveau dialogue
  //!
  //! Cette méthode crée un nouveau dialogue vide (avec juste les boutons Ok, Annuler et Aide).
  //!
  //! @param[in]        tsDescription Description du nouveau dialogue (comme un DFM mais en version simplifiée)
  //! @param[in]        hWnd Fenêtre existante à partir de laquelle créer le dialogue, NULL sinon
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	bool CreateNewDialog(TStrings *tsDescription, HWND hWnd);

  //---------------------------------------------------------------------------
  //! @brief Ouverture d'un fichier
  //!
  //! Cette méthode ouvre un fichier.
  //!
  //! @param[in]        asFileName Nom du fichier à ouvrir.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool OpenFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un fichier
  //!
  //! Cette méthode enregistre les données dans le fichier asFileName. Si le
  //! fichier existe déjà, il est écrasé sans confirmation de l'utilisateur.
  //!
  //! @param[in]        asFileName Nom du fichier à enregistrer.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	virtual bool SaveToFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération undo
  //!
  //! Cette fonction effectue une opération undo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Undo(void);

  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette fonction effectue une opération redo
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool Redo(void);

  //---------------------------------------------------------------------------
  //! @brief Liste des trois premières modifications
  //!
  //! Cette méthode renvoie le libellé des premières modifications
  //!
  //! @return @c AnsiString Liste des modifications (séparées par un retour à
	//! la ligne)
  //---------------------------------------------------------------------------
  virtual AnsiString FASTCALL GetModificationsList(void);

  //---------------------------------------------------------------------------
  //! @brief Couper et copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CutToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Copier dans le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL CopyToClipboard(void);

  //---------------------------------------------------------------------------
  //! @brief Coller depuis le presse-papiers
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  virtual bool FASTCALL PasteFromClipboard(void);

	//---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //!
  //! Cette méthode crée un objet fils de l'objet ElementParent.
  //!
  //! @param[in]        ElementParent Objet parent de l'objet à créer (-1 si pas de parent)
  //! @param[in]        Left Position X de l'objet
  //! @param[in]        Top Position Y de l'objet
  //! @param[in]        Width Largeur de l'objet
  //! @param[in]        Height Hauteur de l'objet
  //! @param[in]        Anchors Ancrages de l'objet
  //!
  //! @return @c int Numéro de l'objet créé (-1 si erreur de création)
  //!
  //! @note Ne pas appeler la méthode EnrObjet. Le nouvel objet est
  //! automatiquement enregistré après avoir été créé.
  //---------------------------------------------------------------------------
  TElemBase * CreationObjet(TElemBase *ElementParent,
              int Left, int Top, int Width, int Height,
              TAnchors Anchors);

  //---------------------------------------------------------------------------
  //! @brief Création d'un dialogue
  //!
  //! @param[in]        IndexDialog Index du dialogue dans le tableau "Dialogues"
  //! @param[in]        NumeroElement Numéro séquentiel. Le dialogue aura pour nom "TForm" + NumeroElement
  //! @param[in]        Left Position X du dialogue
  //! @param[in]        Top Position Y du dialogue
  //! @param[in]        Width Largeur du dialogue
  //! @param[in]        Height Hauteur du dialogue
  //!
  //! @return @c TElemBase * Element créé (NULL si erreur)
  //---------------------------------------------------------------------------
  TElemBase *CreateDialog2(int IndexDialog, int NumeroElement, int Left, int Top, int Width, int Height);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un dialogue
  //!
  //! @param[in]        IndexDialog Index du dialogue dans le tableau "Dialogues"
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool DeleteDialog(int IndexDialog);

  //---------------------------------------------------------------------------
  //! @brief Création d'un objet
  //!
  //! @param[in]        ElementParent Numéro de l'objet parent de l'objet à créer.
  //! @param[in]        IndexDialog Index du dialogue dans le tableau "Dialogues"
  //! @param[in]        NumeroElement Numéro séquentiel. L'objet aura pour nom asTypeObj + NumeroElement
  //! @param[in]        Left Position X de l'objet
  //! @param[in]        Top Position Y de l'objet
  //! @param[in]        Width Largeur de l'objet
  //! @param[in]        Height Hauteur de l'objet
  //! @param[in]        Anchors Ancrages de l'objet
  //! @param[in]        asTypeObj Type de l'objet à créer
  //!
  //! @return @c TElemBase * Element créé (NULL si erreur)
  //---------------------------------------------------------------------------
  TElemBase * CreateObj(TElemBase *ElementParent, int IndexDialog, int NumeroElement, int Left, int Top, int Width, int Height, TAnchors Anchors, AnsiString asTypeObj);

  //---------------------------------------------------------------------------
  //! @brief Suppression d'un objet
  //!
  //! @param[in]  ElementParent Numéro de l'objet parent de l'objet à supprimer.
  //! @param[in]  Element Objet à supprimer.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool DeleteObj(TElemBase *ElementParent, TElemBase *Element);

  //---------------------------------------------------------------------------
  //! @brief Echange de deux objets
  //!
  //! @param[in]  Index1 Index du premier objet à échanger.
  //! @param[in]  Index2 Index du second objet à échanger.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool SwapObjects(int Index1, int Index2);

  //---------------------------------------------------------------------------
  //! @brief Déplacement d'un objet
  //!
  //! @param[in]  Index Index de l'objet à déplacer.
  //! @param[in]  IndexParent Index de l'objet parent.
  //! @param[in]  IndexInsertAfter Index de l'objet parent.
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool MoveObject(int Index, int IndexParent, int IndexInsertAfter);

  //---------------------------------------------------------------------------
  //! @brief Mémorisation d'un objet
  //!
  //! Cette méthode mémorise la position d'un objet.
  //!
  //! @param[in]        ElementParent Objet parent de l'objet à enregistrer
  //! @param[in]        Element Numéro de l'objet à enregistrer (n° unique)
  //! @param[in]        Left Position X de l'objet
  //! @param[in]        Top Position Y de l'objet
  //! @param[in]        Width Largeur de l'objet
  //! @param[in]        Height Hauteur de l'objet
  //! @param[in]        Anchors Ancrages de l'objet
  //! @param[in]        bParentPotentiel Indique si cet objet peut être un parent
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool EnrObjet(TElemBase *ElementParent, TElemBase *Element,
                         int Left, int Top, int Width, int Height,
                         TAnchors Anchors, bool bParentPotentiel);

  //---------------------------------------------------------------------------
  //! @brief Modification d'un ControlSizer
  //!
  //! Cette méthode est appelée par un objet lors d'une modification.
  //---------------------------------------------------------------------------
  void ElemChange(TObject *Sender);

  //---------------------------------------------------------------------------
  //! @brief Affichage du menu popup
  //!
  //! Cette méthode affiche le menu popup à la position courante du curseur
  //!
  //! @return @c bool true si l'utilisateur a choisi une option
  //---------------------------------------------------------------------------
  bool AffPopupMenu(void);

  //@}


  //! @name Propriétés
  //@{

	//---------------------------------------------------------------------------
  //! @brief Propriété AlignGrid
	//!
	//! Cette propriété permet d'afficher l'ordre des tabulations.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, bool, AlignGrid);

	//---------------------------------------------------------------------------
  //! @brief Propriété ShowGrid
	//!
	//! Cette propriété permet d'afficher l'ordre des tabulations.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, bool, ShowGrid);

  //---------------------------------------------------------------------------
  //! @brief Propriété GridSizeX
	//!
	//! Cette propriété permet de définir l'espacement de la grille en X.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, int, GridSizeX);


  //---------------------------------------------------------------------------
  //! @brief Propriété GridSizeY
	//!
	//! Cette propriété permet de définir l'espacement de la grille en Y.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, int, GridSizeY);


	//---------------------------------------------------------------------------
  //! @brief Propriété OrdreTab
	//!
	//! Cette propriété permet d'afficher l'ordre des tabulations.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, bool, OrdreTab);

  //---------------------------------------------------------------------------
  //! @brief Propriété ObjSelected
	//!
	//! Cette propriété permet de lire l'objet sélectionné (cliqué)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, TElemBase *, ObjSelected);


  //---------------------------------------------------------------------------
  //! @brief Propriété ModeConstr
	//!
	//! Cette propriété permet de lire et de définir le mode contruction, c'est
	//! à dire le mode qui permet de poser des composants dans la boîte de
	//! dialogues.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, MODECONSTR, ModeConstr);


  //---------------------------------------------------------------------------
  //! @brief Propriété TypeACreer
	//!
	//! Cette propriété permet de définir le type de composant (la classe pour
	//! les composants VCL) à créer.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TFrame_SaisieDialog, AnsiString, TypeACreer);

  //---------------------------------------------------------------------------
  //! @brief Propriété UndoRedo
	//!
	//! Cette propriété permet d'obtenir l'objet permettant d'enregistrer les
	//! actions pouvant être défaites.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TFrame_SaisieDialog, TUndoRedo *, UndoRedo);

  //@}

  //! @name Evènements
  //@{

  //@}

};
//---------------------------------------------------------------------------
#endif
