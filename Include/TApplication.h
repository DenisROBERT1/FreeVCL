//-----------------------------------------------------------------------------
//! @file TApplication.h
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

#ifdef In_TApplicationH
#error "Inclusion circulaire de TApplication"
#endif // In_TApplicationH

#define In_TApplicationH

#ifndef TApplicationH
#define TApplicationH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TPersistentH
#include "TPersistent.h"
#endif  // TPersistentH

#ifndef TranslateH
#include "Translate.h"
#endif  // TranslateH

//---------------------------------------------------------------------------
// enum
//---------------------------------------------------------------------------

enum TSelectDirOpt {
	sdAllowCreate,					//!< Affiche un bouton "Nouveau répertoire"
	sdPerformCreate,				//!< Crée le répertoire choisi s'il n'existe pas
	sdPrompt,								//!< Demande la confirmation de création du répertoire (utilisé avec @b sdAllowCreate)
	sdShowEdit,							//!< Affiche une boîte d'édition où l'utilisateur peut entrer directement un nom de répertoire
	sdShowShares,						//!< Affiche les répertoires partagés
	sdNewUI,								//!< Nouvelle interface (automatiquement activée)
	sdShowFiles,						//!< Affichage des fichiers dans les répertoires
	sdValidateDir,					//!< Valide le nom du répertoire
	sdBrowseForComputer,		//!< Choix d'un ordinateur en réseau uniquement (bouton Ok grisé sinon)
	sdBrowseForPrinters,		//!< Choix d'une imprimante uniquement (bouton Ok grisé sinon)
	sdDontGoBelowDomain,		//!< Pas d'affichage des répertoires réseau pour les ordinateur du domaine
	sdReturnFSAncestors,		//!< Choix d'un parent d'un répertoire système uniquement (bouton Ok grisé sinon)
	sdReturnOnlyFSDirs,			//!< Choix d'un répertoire système uniquement (bouton Ok grisé sinon)
	sdBrowseFileJunctions		//!< Affiche les répertoires compressés et le contenu des fichiers ZIP
};
typedef Set<TSelectDirOpt, sdAllowCreate, sdReturnOnlyFSDirs> TSelectDirOpts;  //!< Combinaison de plusieurs TSelectDirOpt

// Nouveaux noms
#define sdNewFolder sdAllowCreate

#ifndef BIF_BROWSEFILEJUNCTIONS
#define BIF_BROWSEFILEJUNCTIONS 0x00010000
#endif // BIF_BROWSEFILEJUNCTIONS


enum TReplaceFlag {
	rfReplaceAll,						//!< Remplacement systématique (si absent, uniquement la première occurrence)
	rfIgnoreCase						//!< Ignore la casse (majuscules et minuscules sont équivalentes)
};
typedef Set<TReplaceFlag, rfReplaceAll, rfIgnoreCase> TReplaceFlags;  //!< Combinaison de plusieurs TReplaceFlag

//! @brief Macros pour simplifier l'écriture multi-langages
#define _TT(x) Application->Translate(_T(x))		//!< Traduction d'une chaîne littérale
#define _TA(x) Application->Translate(x)				//!< Traduction d'une AnsiString

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TForm;
class TIcon;
class THintWindow;

//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnActivate.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnActivate)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnDeactivate.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnDeactivate)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnMinimize.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnMinimize)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnRestore.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnRestore)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnEndSession.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnEndSession)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnQueryEndSession.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnQueryEndSession)(TObject *Sender, bool *Accept);


//---------------------------------------------------------------------------
// Fonctions globales
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Nombre de paramètres de la ligne de commandes
//!
//! Cette fonction renvoie le nombre de paramètres de la ligne de commandes,
//! c'est à dire le nombre d'éléments accessible par la fonction
//! @b ParamStr. Les paramètres doivent être séparés par des espaces ou des
//! tabulations. Si un paramètre contient des espaces ou des tabulations
//! (par exemple "Program Files"), mettez le entre guillemets pour qu'il soit
//! considéré comme un seul argument.
//!
//! @return @c int Nombre de paramètres
//---------------------------------------------------------------------------

int FASTCALL ParamCount(void);

//---------------------------------------------------------------------------
//! @brief Argument d'indice i
//!
//! Cette fonction renvoie l'argument d'indice numéro i (le premier ayant le
//! numéro zéro), le premier argument est le nom complet du programme (chemin +
//! nom). Si i est supérieur ou égal au nombre d'arguments, cette fonction
//! retourne une chaîne vide. Utilisez la fonction @b ParamCount pour connaitre
//! le nombre d'arguments.  Les arguments entre guillemets sont considérés
//! comme un seul argument (y compris s'ils contiennent des espaces ou des
//! tabulations). Les guillemets eux-mêmes sont automatiquement éliminés. Si un
//! argument doit contenir des guillemets (sans que ce soit un délimiteur),
//! faites-le précéder d'un anti-slash @b @\.
//!
//! @param[in]        i Indice de l'argument (premier = 0)
//!
//! @return @c AnsiString Argument d'indice i.
//---------------------------------------------------------------------------

AnsiString FASTCALL ParamStr(int i);

//---------------------------------------------------------------------------
//! @brief Décomposition de la ligne de commande
//!
//! Cette fonction décompose la ligne de commande. Ne pas l'utiliser, elle est
//! appelée en interne par les fonctions @b ParamCount et @b ParamStr.
//---------------------------------------------------------------------------

void FASTCALL SplitCommandLine(void);

//---------------------------------------------------------------------------
//! @brief Transformation d'un raccourci en chaîne.
//!
//! Cette fonction transforme un raccourci en chaîne de caractères pour
//! l'afficher dans un menu, par exemple. La chaîne est de la forme
//! "Maj + A", "Ctrl + Z", "Ctrl + Alt + Supp", par exemple.
//!
//! @param[in]        ShortCut Raccourci
//!
//! @return @c AnsiString Chaîne représentant le raccourci.
//---------------------------------------------------------------------------

AnsiString FASTCALL ShortCutToText(TShortCut ShortCut);

//---------------------------------------------------------------------------
//! @brief Transformation d'une chaîne en raccourci
//!
//! Cette fonction fait l'inverse de la fonction @b ShortCutToText, c'est à
//! dire qu'elle transforme une chaîne de la forme "Ctrl + Z" (par exemple)
//! en raccourci. L'ordre des éléments dans la chaîne n'a pas d'importance:
//! les chaînes "Maj + Ctrl + A" et "Ctrl + Maj + A", par exemple, donnent le
//! même raccourci. Elle est insensible à la casse ("Ctrl" et "CTRL" sont
//! équivalents).
//!
//! @param[in]        asText Chaîne à transformer
//!
//! @return @c TShortCut Raccourci
//!
//! @note Pour des questions de rapidité, évitez d'utiliser @b TextToShortCut
//! s'il est possible d'utiliser directement un @b TShortCut.
//---------------------------------------------------------------------------

TShortCut FASTCALL TextToShortCut(AnsiString asText);

//---------------------------------------------------------------------------
//! @brief Décomposition d'un @b TShortCut en @b WORD et @b TShiftState
//!
//! Cette fonction décompose un @b TShortCut en @b WORD (format Windows
//! utilisé dans WM_KEYDOWN entre autres) et @b TShiftState (état des touches
//! Alt, Maj et Ctrl).
//!
//! @param[in]        ShortCut Raccourci en entrée
//! @param[out]       Key Touche principale
//! @param[out]       Shift état des touches Alt, Maj et Ctrl
//---------------------------------------------------------------------------

void FASTCALL ShortCutToKey(TShortCut ShortCut, WORD &Key, TShiftState &Shift);

//---------------------------------------------------------------------------
//! @brief Boîte de dialogue de sélection d'un répertoire.
//!
//! Cette fonction ouvre la boîte de dialogue du système permettant de
//! sélectionner un répertoire.
//!
//! @param[in]        Caption Titre de la boîte de dialogue
//! @param[in]        Root Répertoire de départ
//! @param[out]       Directory Répertoire choisi par l'utilisateur
//! @param[in]        SelectDirOpts Options d'affichage de la boîte de dialogue (voir ::TSelectDirOpt)
//!
//! @return @c bool true si l'utilisateur a choisi un répertoire. false sinon
//! (si l'utilisateur a cliqué sur annuler, ou a fermé la boîte).
//---------------------------------------------------------------------------

bool FASTCALL SelectDirectory(const AnsiString Caption, const AnsiString Root,
    AnsiString &Directory, TSelectDirOpts SelectDirOpts = 0);

//---------------------------------------------------------------------------
//! @brief Test de l'existence d'un fichier
//!
//! Cette fonction renvoie true si le fichier testé existe, false sinon. Le
//! fait qu'un fichier existe ne veut pas forcément dire qu'il est accessible
//! (il peut être en lecture seule, déjà utilisé par un autre processus,
//! etc...).
//!
//! @param[in]        asFileName Nom complet du fichier à tester
//!
//! @return @c bool true si fichier existe
//---------------------------------------------------------------------------

bool FASTCALL FileExists(const AnsiString asFileName);

//---------------------------------------------------------------------------
//! @brief Forme standard d'un nom de fichier
//!
//! Cette fonction transforme un nom de fichier sous une forme standard unique,
//! pour permettre ensuite de faire des comparaisons sur le nom. Par exemple
//! sous windows, le nom est converti en minuscules.
//!
//! @note Le nom uniquement est transformé, le fichier lui-même n'est pas
//! renommé ou modifié, il peut même être existant.
//!
//! @param[in]        asFileName Nom du fichier à standardiser
//!
//! @return @c AnsiString Nom du fichier sous forme standard 
//---------------------------------------------------------------------------

AnsiString FASTCALL FileNameStandardFormat(const AnsiString asFileName);

//---------------------------------------------------------------------------
//! @brief Destruction d'un fichier
//!
//! Cette fonction détruit définitivement ou met dans la corbeille un fichier.
//!
//! @param[in]        asFileName Nom du fichier à détruire.
//! @param[in]        bToRecycleBin (argument facultatif, peut être omis) Si true: le fichier est transféré dans la corbeille. Sinon il est détruit définitivement.
//!
//! @return @c bool false si le fichier ne peut être détruit (soit il n'existe
//! pas, soit il est protégé ou utilisé).
//!
//! @note L'option bToRecycleBin ne marche pas sous Windows CE
//---------------------------------------------------------------------------

bool FASTCALL DeleteFile(const AnsiString asFileName, bool bToRecycleBin = false);

//---------------------------------------------------------------------------
//! @brief Changement de répertoire
//!
//! Cette fonction change le répertoire courant. Ce répertoire est celui où
//! le système recherche en priorité les fichiers, les DLL et les
//! dépendances.
//!
//! @param[in]        asDir Nom complet du répertoire à sélectionner
//!
//! @return @c bool true si l'opération s'est déroulée correctement
//---------------------------------------------------------------------------

bool FASTCALL ChDir(const AnsiString asDir);

//---------------------------------------------------------------------------
//! @brief Création d'un répertoire
//!
//! Cette fonction crée un nouveau répertoire. Le répertoire créé a les mêmes
//! paramètres de sécurité (droits d'accès) que son répertoire parent.
//!
//! @param[in]        asDir Nom complet du répertoire à créer
//!
//! @return @c bool true si la création s'est déroulée correctement
//---------------------------------------------------------------------------

bool FASTCALL CreateDir(const AnsiString asDir);

//---------------------------------------------------------------------------
//! @brief Renomme un fichier
//!
//! Cette fonction renomme le fichier désigné par @b OldName en lui donnant le
//! nouveau nom @b NewName.
//!
//! @param[in]        OldName Nom complet du fichier à renommer
//! @param[in]        NewName Nouveau nom de fichier (sans le chemin)
//!
//! @return @c bool true si Ok. false si un autre fichier de même nom existe
//! déjà, ou bien si fichier en lecture seule, par exemple.
//---------------------------------------------------------------------------
bool FASTCALL RenameFile(const AnsiString OldName, const AnsiString NewName);

//---------------------------------------------------------------------------
//! @brief Extraction du nom de répertoire d'un nom de fichier.
//!
//! Cette fonction extrait le nom de répertoire du nom complet d'un fichier.
//! L'antislash de fin est supprimé. Donc Le retour de cette fonction peut être
//! utilisé directement pour appeler la fonction @b CreateDir, @b GetCurrentDir,
//! @b RemoveDir et @b SetCurrentDir, par exemple. Si vous désirez extraire le
//! nom de répertoire avec l'antislash de fin (pour lui concaténer un nom de
//! fichier, par exemple), utilisez la fonction @b ExtractFilePath.@n
//! Le retour de cette fonction est vide si le nom de fichier contient un nom
//! incomplet ou non valide.
//!
//! @param[in]        FileName Nom complet du fichier
//!
//! @return @c AnsiString Nom du répertoire
//---------------------------------------------------------------------------

AnsiString FASTCALL ExtractFileDir(const AnsiString FileName);

//---------------------------------------------------------------------------
//! @brief Extraction du nom de disque d'un nom de fichier.
//!
//! Cette fonction extrait le nom de disque du nom complet d'un fichier. Le nom
//! comprend la lettre du lecteur (sans le ":") pour les disques locaux, ou bien
//! une chaîne de la forme "\\<server>\<name>" pour les lecteurs réseau.@n
//! Le retour de cette fonction est vide si le nom de fichier contient un nom
//! incomplet ou non valide.
//!
//! @param[in]        FileName Nom complet du fichier
//!
//! @return @c AnsiString Nom de disque
//---------------------------------------------------------------------------

AnsiString FASTCALL ExtractFileDrive(const AnsiString FileName);

//---------------------------------------------------------------------------
//! @brief Extraction de l'extension d'un fichier
//!
//! Cette fonction retourne l'extension d'un nom de fichier complet, avec le
//! point la précédent. Si le nom de fichier contient plusieurs points
//! ("c:\FileName.txt.old", par exemple), seule la chaîne après le dernier
//! point est extrait (".old", dans l'exemple).@n
//! Le retour de cette fonction est vide si le nom de fichier ne contient pas
//! d'extension.
//!
//! @param[in]        FileName Nom complet du fichier
//!
//! @return @c AnsiString Extension.
//---------------------------------------------------------------------------

AnsiString FASTCALL ExtractFileExt(const AnsiString FileName);

//---------------------------------------------------------------------------
//! @brief Extraction du nom + extension de fichier d'un nom complet de fichier.
//!
//! Cette fonction retourne le nom de fichier (nom et extension) du nom complet
//! de fichier (c'est à dire la partie à droite du dernier slash ou ":"). Si
//! le nom de fichier ne contient ni nom de disque ni chemin, cette fonction
//! retourne la même chaîne qu'en entrée.
//!
//! @param[in]        FileName Nom complet (ou pas) du fichier
//!
//! @return @c AnsiString Nom du fichier sans le chemin.
//!
//! @note Pour obtenir le nom sans l'extension, vous pouvez ensuite utiliser
//! la fonction @b ChangeFileExt avec une nouvelle extension vide.
//---------------------------------------------------------------------------

AnsiString FASTCALL ExtractFileName(const AnsiString FileName);

//---------------------------------------------------------------------------
//! @brief Extraction du nom de répertoire d'un nom de fichier.
//!
//! Cette fonction extrait le nom de répertoire du nom complet d'un fichier,
//! avec le slash ou l'antislash de fin.@n
//! Le retour de cette fonction est vide si le nom de fichier contient un nom
//! incomplet ou non valide.
//!
//! @param[in]        FileName Nom complet du fichier
//!
//! @return @c AnsiString Nom du répertoire + "\"
//---------------------------------------------------------------------------

AnsiString FASTCALL ExtractFilePath(const AnsiString FileName);

//---------------------------------------------------------------------------
//! @brief Remplacement de l'extension d'un fichier.
//!
//! Cette fonction remplace l'extension d'un fichier par une autre.
//!
//! @param[in]        FileName Nom (complet ou pas) du fichier
//! @param[in]        Extension Nouvelle extension.
//!
//! @return @c AnsiString Nouveau nom de fichier.
//!
//! @note Cette fonction ne renomme pas le fichier, elle ne fait qu'un
//! remplacement dans le nom. Si vous désirez renommer le fichier désigné par
//! @b FileName, utilisez ensuite @b RenameFile.
//! @note Si la nouvelle extension ne commence pas par un ".", le point est
//! automatiquement ajouté (sauf si l'extension est vide).
//---------------------------------------------------------------------------

AnsiString FASTCALL ChangeFileExt(const AnsiString FileName, const AnsiString Extension);

//---------------------------------------------------------------------------
//! @brief Remplacement dans une chaîne.
//!
//! Cette fonction remplace la chaîne @b OldPattern par la chaîne @b NewPattern
//! dans la chaîne @b S, avec diverses options (remplacement systématique ou
//! seulement la première occurrence, sensible à la casse ou pas).@n
//! Après un remplacement, la recherche continue à la fin de la chaîne
//! remplacée: par exemple, si vous remplacez "to" par "toto" dans la chaîne
//! "tititototata", vous obtiendrez "tititototototata" (chaque "to" n'est
//! remplacé qu'une seule fois).
//!
//! @param[in]        S Chaîne dans laquelle le remplacement doit être effectué.
//! @param[in]        OldPattern Chaîne à rechercher.
//! @param[in]        NewPattern Chaîne à remplacer.
//! @param[in]        Flags Options de remplacement (voir ::TReplaceFlag)
//!
//! @return @c AnsiString Chaîne après remplacement.
//---------------------------------------------------------------------------

AnsiString FASTCALL StringReplace(const AnsiString S, const AnsiString OldPattern, const AnsiString NewPattern, TReplaceFlags Flags);

//---------------------------------------------------------------------------
//! @brief Etat courant des touches Alt, Maj et Ctrl.
//!
//! Cette fonction retourne l'état courant des touches Alt, Maj (ou shift selon
//! votre clavier), et Ctrl (Control) au moment de l'appel.
//!
//! @return @c ::TShiftState Etat des touches.
//---------------------------------------------------------------------------

TShiftState FASTCALL GetShiftState(void);

//---------------------------------------------------------------------------
//! @brief Entrée d'une valeur au clavier
//!
//! Cette fonction ouvre une simple boîte demandant à l'utilisateur une
//! valeur.
//!
//! @param[in]        ACaption Titre de la boîte
//! @param[in]        APrompt Label inscrit à gauche du contrôle d'édition
//! @param[in]        ADefault Valeur par défaut du contrôle d'édition
//! @param[in]        WidthEditControl Largeur du contrôle d'édition (pixels)
//!
//! @return @c AnsiString Valeur entrée. Si l'utilisateur appuie sur
//! "annuler", la fonction renvoie la valeur par défaut.
//---------------------------------------------------------------------------

AnsiString FASTCALL InputBox(const AnsiString ACaption, const AnsiString APrompt, const AnsiString ADefault, int WidthEditControl = 200);

//-----------------------------------------------------------------------------
//! @class TApplication
//!
//! @brief Classe représentant l'application.
//!
//! Cette classe permet de gérer une application entière (ensemble de fenêtres
//! actives ou pas). Elle offre des méthodes et des propriétés permettant la
//! création et la destruction des fenêtres, des informations sur ces fenêtres
//! (handle de la fenêtre principale), la gestion des exceptions, etc...@n
//! Une instance unique de @b TApplication nommé Application est
//! systématiquement créé lorsqu'on crée un nouveau projet. Ses propriétés ne
//! sont pas accessibles par l'éditeur de propriétés puisqu'aucun DFM n'y est
//! associé.@n
//! Cette classe ne gère pas les paramètres d'affichage. L'objet qui le fait
//! est l'instance @b Screen de la classe ::TScreen.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TApplication: public TPersistent {
private:
protected:
  TForm *FMainForm;
  HWND FHandle;
  TIcon *FIcon;
  AnsiString FHelpFile;
  int FHintPause;
  int FHintHidePause;
	HINSTANCE hTranslate;
  TRANSLATE_CHOOSELANGUAGE Translate_ChooseLanguage;
  TRANSLATE_GETLANGUAGE Translate_GetLanguage;
  TRANSLATE_SETLANGUAGE Translate_SetLanguage;
  TRANSLATE_TRANSLATE Translate_Translate;
  TRANSLATE_ADD Translate_Add;
  void FASTCALL OpenDllTranslate(void);
public:

  //! @name Constructeurs et destructeurs
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TApplication(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TApplication(void);

  //@}

  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Initialisation de l'application
	//!
	//! Par défaut, cette méthode ne fait qu'appeler la fonction @b initProc (si
	//! vous l'avez définie). Vous pouvez supprimer l'appel à cette méthode si
	//! vous n'utilisez pas @b initProc.
  //---------------------------------------------------------------------------
  void FASTCALL Initialize(void);

  //---------------------------------------------------------------------------
  //! @brief Création de la fenêtre principale
  //!
  //! Cette méthode ne fait en fait rien. La création de la @b TForm est faite
  //! par @b __classid, cette fonction se contente de passer le pointeur à la
  //! variable destination.
	//!	Une des application classique de CreateForm est la création de la
	//!	fiche principale lors de l'initialisation du projet:
	//! @code
	//!	Application->CreateForm(__classid(TForm_Principale), &Form_Principale);  // Syntaxe (1)
	//! @endcode
	//!	qui se traduit par:
	//! @code
	//!	Application->CreateForm((new TForm_Principale(NULL, _T("TForm_Principale"))), &Form_Principale);  // (2)
	//! @endcode
	//!	qui est équivalent à:
	//! @code
	//!	Form_Principale = new TForm_Principale(NULL, _T("TForm_Principale"));  // (3)
	//! @endcode
	//!	La syntaxe (1) doit être gardée par souci de compatibilité avec le logiciel
	//!	Builder. Si cet aspect n'est pas important pour vous, vous pouvez sans
	//!	problème adopter la syntaxe (3) qui est plus claire et plus lisible.
	//!
  //! @param[in]        Instance Pointeur sur la TForm créée par @b __classid
  //! @param[out]       Reference "Retour" de la fonction
  //---------------------------------------------------------------------------
  void FASTCALL CreateForm(TForm *Instance, void *Reference);

  //---------------------------------------------------------------------------
  //! @brief Lancement de l'application
	//!
	//! Cette méthode lance réellement l'application. Elle comprend une boucle de
	//! traitement des messages Windows, dont elle ne sortira qu'à la fin de
	//! l'application principale (Appel de la méthode @b Terminate ou bien
	//! @b Close de la fenêtre principale).
	//! La méthode @b Run ne doit être appelée qu'une seule fois dans
	//! l'application, en général dans la fonction @b WinMain. Cet appel étant
	//! créé automatiquement, vous n'avez en principe pas à utiliser cette
	//! méthode.
  //!
  //! @return @c int Valeur de retour de la fenêtre principale (transmis comme
	//! retour de la fonction @b WinMain).
  //---------------------------------------------------------------------------
  int FASTCALL Run(void);

	//---------------------------------------------------------------------------
  //! @brief Ouvre une info-bulle.
  //!
  //! Cette méthode ouvre l'info-bulle du contrôle où est la souris. Cette
	//! méthode est utilisée uniquement en interne, le développeur d'un
	//! programme n'a en général pas à se soucier ni de l'ouverture ni de la
	//! fermeture de l'info-bulle, celle-ci étant gérée par le système.
	//---------------------------------------------------------------------------
	void FASTCALL ActivateHint(TPoint CursorPos);

	//---------------------------------------------------------------------------
  //! @brief Ferme l'info-bulle.
  //!
  //! Cette méthode ferme l'info-bulle s'il y en a une d'ouverte. Elle est
	//! utilisée uniquement en interne, le développeur d'un programme n'a en
	//! général pas à se soucier ni de l'ouverture ni de la fermeture de l'info-
	//! bulle, celle-ci étant gérée par le système.
	//---------------------------------------------------------------------------
	void FASTCALL HideHint(void);

	//---------------------------------------------------------------------------
  //! @brief Empêche l'ouverture d'une info-bulle.
  //!
  //! Cette méthode empêche l'ouverture d'une info-bulle, au moment de la
	//! fermeture du programme, par exemple. N'appelez pas cette fonction, elle
	//! est utilisée uniquement en interne par le système.
	//---------------------------------------------------------------------------
	void FASTCALL CancelHint(void);

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
	//!
	//! Cette méthode permet de traiter les messages Windows dans l'attente d'un
	//! évènement, par exemple. Tous les messages Windows en attente sont traités
	//! et la méthode se termine lorsque la file de messages est vide. Dans le
	//! cas d'un traitement long, il est nécessaire d'appeler cette méthode de
	//! temps en temps pour permettre le réaffichage, les évènement liés à la
	//! souris, etc...
	//!
	//! @warning Si vous utilisez cette méthode, faites attention aux cas de
	//! réentrance: Car dans cette méthode, les commandes sont également
	//! traitées, ce qui permet à l'utilisateur, si aucun système de protection
	//! n'est mis en place, de lancer une nouvelle commande pendant l'exécution
	//! d'une autre commande (dans un traitement de texte, imaginez les
	//! conséquences d'une commande d'enregistrement pendant une commande de
	//! mise en forme).@n
	//! Evitez également d'appeler cette méthode dans une boucle sans aucun autre
	//! traitement. Ceci aurait pour effet de monopoliser inutilement le
	//! processeur au détriment des autres applications. Si vous ne pouvez faire
	//! autrement, ajoutez dans la boucle un appel à @b Sleep avec une valeur de
	//! quelques millisecondes (entre 10 et 100, par exemple, suivant la
	//! réactivité que vous souhaitez).
  //---------------------------------------------------------------------------
  void FASTCALL ProcessMessages(void);

  //---------------------------------------------------------------------------
  //! @brief Mise en icône de l'application.
	//!
	//! Cette méthode met l'application en icône dans la barre des tâches. Ne
	//! confondez pas cette méthode avec la propriété @b WindowState qui ne met
	//! qu'une seule fenêtre en icône.
  //---------------------------------------------------------------------------
  void FASTCALL Minimize(void);

  //---------------------------------------------------------------------------
  //! @brief Restauration de l'application à sa taille normale.
	//!
	//! Cette méthode redonne ses dimensions initiales à l'application (c'est à
	//! dire la remet dans l'état où elle était lors de l'exécution de la
	//! commande @b Minimize). Ne confondez pas cette méthode avec la propriété
	//! @b WindowState qui ne rétablit qu'une seule fenêtre à sa taille normale.
  //---------------------------------------------------------------------------
  void FASTCALL Restore(void);

  //---------------------------------------------------------------------------
  //! @brief Affiche un message d'exception et son adresse physique
	//!
	//! Cette méthode affiche une boîte de dialogue contenant le message de
	//! l'exception E et l'adresse où celle-ci est survenue.
  //!
  //! @param[out]       E Exception (en général, une classe fille de la classe @b Exception)
  //---------------------------------------------------------------------------
  void FASTCALL ShowException(Exception* E);

  //---------------------------------------------------------------------------
  //! @brief Provoque la fin de l'application
	//!
	//! Cette méthode ferme la fenêtre principale de l'application et sort de la
	//! boucle de messages principale (et également des boucles secondaires si
	//! cette méthode est appelée pendant un @b ProcessMessages, par exemple).
	//! @warning Ne terminez pas l'application par un appel à @b PostQuitMessage
	//! qui aurait pour effet de terminer l'application sans détruire les objets,
	//! avec les conséquences que ça peut avoir si des traitements ou sauvegardes
	//! sont effectués dans le destructeur de ces objets.
  //---------------------------------------------------------------------------
  void FASTCALL Terminate(void);

  //---------------------------------------------------------------------------
  //! @brief Demande de sortie de Windows.
  //!
	//! N'appelez pas cette méthode. Elle est automatiquement appelée par la
	//! fenêtre principale lorsqu'elle reçoit le message @b WM_QUERYENDSESSION
	//! (message Windows reçu par toutes les applications lorsqu'un ordre de
	//! fermeture de session est lancé). Cette méthode provoque à son tour un
	//! évènement @b OnQueryEndSession que vous pouvez intercepter pour effectuer
	//! un traitement (cas classique: affichage d'un message à l'utilisateur pour
	//! lui demander s'il veut sauvegarder ses données).
  //!
  //! @return @c bool false si la sortie de session est refusée (données non sauvegardées, par exemple).
	//!
	//! @sa TApplication::OnQueryEndSession
  //---------------------------------------------------------------------------
  bool FASTCALL ProcessQueryEndSession(void);

  //---------------------------------------------------------------------------
  //! @brief Sortie de windows.
	//!
	//! N'appelez pas cette méthode. Elle est automatiquement appelée par la
	//! fenêtre principale lorsqu'elle reçoit le message @b WM_ENDSESSION
	//! (message Windows reçu par toutes les applications lorsque toutes les
	//! applications on accepté la sortie de session et que celle-ci va donc
	//! se terminer). Cette méthode provoque à son tour un évènement
	//! @b OnEndSession que vous pouvez intercepter pour effectuer un
	//! traitement (sauvegarde systématique des données, par exemple).
  //!
  //! @return @c bool toujours true.
	//!
	//! @sa TApplication::OnEndSession
  //---------------------------------------------------------------------------
  bool FASTCALL ProcessEndSession(void);

  //---------------------------------------------------------------------------
  //! @brief Lancement de l'aide en ligne.
	//!
	//! Cette méthode appelle le fichier d'aide défini par la propriété
	//! @b HelpFile, (par défaut: dans le même répertoire que le programme), et
	//! ouvre la page définie par @b Command et @b Data.@n
	//! Consultez l'aide Windows @b WinHelp pour plus de détails sur les
	//! paramètres @b Command et @b Data.
  //!
  //! @param[in]        Command Commande à effectuer dans le fichier d'aide.
  //! @param[in]        Data Données additionnelles (dépend de @b Command).
  //!
  //! @return @c bool false si fichier d'aide non trouvé ou impossible à ouvrir.
	//!
	//! @note Il est possible d'ouvrir un fichier autre qu'un fichier HLP. Si le
	//! fichier est un fichier CHM, @b Data est le nom de la page casté en int
	//! 64 bits:
	//! @code
	//! Application->HelpFile = _T("MyHelpFile.chm");
	//! Application->HelpCommand(HELP_CONTEXT, (__int64) _T("MyPage.html"));
	//! @endcode
	//! Dans les autres cas (fichier TXT, par exemple), @b FreeVCL essaye de
	//! l'ouvrir avec le programme le plus adapté. Mais dans ce cas, aucune page
	//! n'est sélectionnée.
	//!
	//! @warning Ne fonctionne pas sous Windows Mobile.
	//!
	//! @sa TApplication::HelpFile
  //---------------------------------------------------------------------------
  bool FASTCALL HelpCommand(int Command, THelpContext Data);

  //---------------------------------------------------------------------------
  //! @brief Affiche un message.
  //!
  //! Cette méthode affiche le message @b Text dans une boîte de dialogue
	//! standard. Les messages longs sont éventuellements affichés sur plusieurs
	//! lignes (mais pas le titre). Les flags sont la combinaison d'un (et un
	//! seul) flag permettant de définir les boutons présents avec
	//! (éventuellement) un flag permettant de définir l'icône affichée:@n
	//! @n
	//! <TABLE>
	//! <TR><TD><B>FLAG</B></TD><TD><B>BOUTONS</B></TD></TR>
	//! <TR><TD>MB_OK</TD><TD>Ok</TD></TR>
	//! <TR><TD>MB_OKCANCEL</TD><TD>Ok Annuler</TD></TR>
	//! <TR><TD>MB_ABORTRETRYIGNORE</TD><TD>Abandonner Réessayer Ignorer</TD></TR>
	//! <TR><TD>MB_YESNOCANCEL</TD><TD>Oui Non Annuler</TD></TR>
	//! <TR><TD>MB_YESNO</TD><TD>Oui Non</TD></TR>
	//! <TR><TD>MB_RETRYCANCEL</TD><TD>Réessayer Annuler</TD></TR>
	//! </TABLE>
	//! @n
	//! <TABLE>
	//! <TR><TD><B>FLAG</B></TD><TD><B>ICONE</B></TD></TR>
	//! <TR><TD>MB_ICONERROR</TD><TD>Erreur fatale</TD></TR>
	//! <TR><TD>MB_ICONQUESTION</TD><TD>Point d'interrogation</TD></TR>
	//! <TR><TD>MB_ICONWARNING</TD><TD>Erreur non fatale</TD></TR>
	//! <TR><TD>MB_ICONINFORMATION</TD><TD>Information</TD></TR>
	//! </TABLE>
	//! @n
	//! <TABLE>
	//! <TR><TD><B>RETOUR</B></TD><TD><B>Bouton cliqué</B></TD></TR>
	//! <TR><TD>IDOK</TD><TD>Ok</TD></TR>
	//! <TR><TD>IDCANCEL</TD><TD>Annuler ou fermeture [X]</TD></TR>
	//! <TR><TD>IDIGNORE</TD><TD>Ignorer</TD></TR>
	//! <TR><TD>IDRETRY</TD><TD>Réessayer</TD></TR>
	//! <TR><TD>IDABORT</TD><TD>Abandonner</TD></TR>
	//! <TR><TD>IDYES</TD><TD>Oui</TD></TR>
	//! <TR><TD>IDNO</TD><TD>Non</TD></TR>
	//! </TABLE>
  //!
  //! @param[in]        Text Texte à afficher
  //! @param[in]        Caption Titre de la fenêtre
  //! @param[in]        Flags Boutons et icônes (voir la fonction MessageBox de
	//!                   Windows)
  //!
  //! @return @c int Bouton cliqué par l'utilisateur, ou zéro si erreur système
	//! (manque de mémoire).
  //!
  //! @note Comme toutes les boîtes de ce type, il est possible de faire Ctrl +
	//! C pour copier le texte de la boîte dans le presse-papiers.
  //---------------------------------------------------------------------------
  int FASTCALL MessageBox(const TCHAR * Text, const TCHAR * Caption, int Flags = MB_OK);

  //---------------------------------------------------------------------------
  //! @brief Choix de la langue.
	//!
	//! Cette méthode ouvre un menu popup dans lequel l'utilisateur peut choisir
	//! une langue parmi celles qui sont disponibles. Les langues disponibles
	//! sont celles pour lesquelles il existe un fichier .LNG (dans le même
	//! répertoire que le programme). Le texte affiché est le nom du fichier
	//! (sans l'extension).
	//!
  //! @return @c bool true si l'utilisateur a cliqué une des options.
	//!
  //! @warning Cette méthode nécessite la présence de la DLL Translate.
	//! Celle-ci peut être librement redistribuée, elle doit être installée avec
	//! votre programme si elle est nécessaire. Si cette DLL est absente, la
	//! méthode @b ChooseLanguage ne fait rien.
  //---------------------------------------------------------------------------
  bool FASTCALL ChooseLanguage(void);

  //---------------------------------------------------------------------------
  //! @brief Sélection de la langue.
  //!
  //! Cette méthode permet de définir une langue, c'est à dire ouvre le
	//! fichier LNG correspondant au paramètre @b asNewLanguage. En général, vous
	//! n'avez pas à utiliser cette méthode: elle est appelée directement lorsque
	//! l'utilisateur choisit une langue, et la langue choisie est mémorisée et
	//! resélectionnée en début de programme.
  //!
  //! @param[in]        asNewLanguage Langue = nom du fichier LNG.
  //!
  //! @return @c bool false si fichier LNG absent ou inaccessible. true sinon.
  //---------------------------------------------------------------------------
  bool FASTCALL SetLanguage(AnsiString asNewLanguage);

  //---------------------------------------------------------------------------
  //! @brief Traduction d'une phrase en fonction de la langue.
  //!
  //! Cette méthode recherche dans le fichier langue ouvert le texte
	//! correspondant à @b asInput, et retourne le texte traduit.@n
	//! Si aucun fichier n'est ouvert ou que le fichier Translate.dll est absent,
	//! cette méthode retourne le texte passé en paramètre sans transformation.
	//! Si le texte n'a pas été trouvé dans le fichier de langue et que celui-ci
	//! est accessible en écriture, le texte y est automatiquement ajouté (mais
	//! pas la traduction, bien sûr).
  //!
  //! @param[in]        asInput Texte à traduire
  //!
  //! @return @c AnsiString Texte traduit
  //---------------------------------------------------------------------------
  AnsiString FASTCALL Translate(AnsiString asInput);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété MainForm: Fenêtre principale de l'application.
	//!
	//! Cette propriété permet de lire ou de définir la fenêtre principale de
	//! l'application. En général vous n'avez pas à la définir, la première
	//! fenêtre créée dans le @b CreateForm devient automatiquement la fenêtre
	//! principale. Si vous devez réellement opérer un changement, le seul moyen
	//! est de mettre la propriété à NULL puis de lui affecter la nouvelle
	//! valeur.
	//! @note La fenêtre principale détermine la position des fenêtres qui ont la
	//! propriété @b Position egale à @b poMainFormCenter, c'est aussi celle qui
	//! provoque la fin de l'application lorsqu'elle est détruite. Son handle est
	//! égal à celui de @b Application et sert lors de l'appel à certaines
	//! fonctions Windows.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TApplication, TForm *, MainForm);

  //---------------------------------------------------------------------------
  //! @brief Propriété Handle: Handle de la fenêtre principale.
	//!
	//! Cette propriété permet d'obtenir un handle valide lors de l'appel de
	//! fonctions Windows (ShellExecute, MessageBox, ...). Affectez cette
	//! propriété uniquement dans un fichier DLL pour que les fenêtres créées
	//! dans la DLL soient dépendantes de l'application principale (c'est à dire
	//! réduite et restaurées avec la fenêtre principale). Dans tous les autres
	//! cas, programmes EXE notamment, ne changez pas la valeur de la propriété
	//! @b Handle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TApplication, HWND, Handle);

  //---------------------------------------------------------------------------
  //! @brief Propriété Icon: Icône de l'application.
	//!
	//! Affectez cette propriété, généralement en tout début de programme, pour
	//! définir l'icône de l'application. C'est cette icône qui apparaîtra dans
	//! la barre de titre, dans la barre de tâches et lorsque l'utilisateur fera
	//! ALT + TAB.@n
	//! L'objet TApplication fait une copie de l'icône, mais ne devient pas
	//! propriétaire de l'objet passé en paramètres. C'est donc à l'utilisateur
	//! de détruire cet objet lorsqu'il n'en a plus l'utilité.
	//! @note Pour que l'icône soit aussi celle qui apparaîsse dans
	//! l'explorateur, il faut qu'elle soit mise en premier dans le fichier des
	//! ressources.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TApplication, TIcon *, Icon);

  //---------------------------------------------------------------------------
  //! @brief Propriété ExeName: Nom complet de l'application.
	//!
	//! Cette propriété retourne le chemin et le nom de l'application.
	//!
	//! @sa ParamStr
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TApplication, AnsiString, ExeName);

  //---------------------------------------------------------------------------
  //! @brief Propriété ExePath: Chemin de l'application.
	//!
	//! Cette propriété retourne le chemin de l'application (sans l'antislash de
	//! fin).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TApplication, AnsiString, ExePath);

  //---------------------------------------------------------------------------
  //! @brief Propriété HelpFile: Nom du fichier d'aide.
	//!
	//! Affectez cette propriété pour définir le fichier d'aide à ouvrir lorsque
	//! la méthode TApplication::HelpCommand sera appelée. Ce nom de fichier peut
	//! être un nom complet (précédé du nom de répertoire), ou bien juste le nom
	//! de fichier (dans ce cas il sera recherché dans le même répertoire que
	//! l'application). Le fichier d'aide peut être soit un fichier HLP, soit
	//! un fichier CHM, soit tout autre fichier associé à une application
	//! (fichier TXT, DOC, HTML, ...)
	//!
	//! @note Le nom du fichier passe par la méthode TApplication::Translate, ce
	//! qui vous permet de définir un fichier d'aide spécifique pour chaque
	//! langue.
	//!
	//! @sa HelpCommand, Translate
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TApplication, AnsiString, HelpFile);

  //---------------------------------------------------------------------------
  //! @brief Propriété HintPause: délai d'apparition de l'info-bulle
	//!
	//! Affectez cette propriété pour définir le temps que mettent les infos-
	//! bulles à apparaitre après l'immobilisation de la souris. Le temps est en
	//! millisecondes, la valeur par défaut est de 500 (une demi-seconde).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TApplication, int, HintPause);

  //---------------------------------------------------------------------------
  //! @brief Propriété HintHidePause: délai d'effacement de l'info-bulle
	//!
	//! Affectez cette propriété pour définir le temps que mettent les infos-
	//! bulles à disparaitre après leur affichage (sauf si un mouvement de la
	//! souris est détecté, auquel cas l'info-bulle disparait immédiatement). Le
	//! temps est en millisecondes, la valeur par défaut est de 2500 (deux
	//! secondes et demie).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TApplication, int, HintHidePause);


  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnActivate: Activation de l'application.
	//!
	//! Cet évènement est déclenché lorsque l'application est activée, c'est à
	//! dire lorsqu'une de ses fenêtre reçoit le focus alors qu'une autre
	//! application était active.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//!
	//! @note Ne pas confondre l'évènement OnActivate de l'application et
	//! l'évènement OnActivate de l'objet TForm: le premier est déclenché lorsque
	//! l'utilisateur passe d'une application à l'autre (en cliquant sur l'icône
	//! dans la barre des tâches ou en faisant ALT + TAB, par exemple), le second
	//! est déclenché lorsqu'il passe d'une fenêtre à une autre (soit parce que
	//! ce sont des fenêtres non modales, soit parce que l'action sur une fenêtre
	//! en ouvre une autre).
	//! @sa TForm::OnActivate TApplication::OnDeactivate
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnActivate, OnActivate, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnDeactivate: Désactivation de l'application
	//!
	//! Cet évènement est déclenché lorsque l'application est désactivée, c'est à
	//! dire lorsqu'elle perd le focus et qu'une autre application le prend.@n
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//!
	//! @note Il s'applique les même remarques pour cet évènement que pour
	//! l'évènement OnActivate.
	//!
	//! @sa TForm::OnDeactivate, TApplication::OnActivate
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnDeactivate, OnDeactivate, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnMinimize: Mise en icône de l'application.
	//!
	//! Cet évènement est déclenché lorsque l'application est mise en icône dans
	//! la barre des tâches, soit parce que l'utilisateur a cliqué sur le bouton
	//! de mise en icône, soit parce que l'application a reçu la commande de mise
	//! en icône par l'intermédiaire de la méthode @b Minimize.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//!
	//! @sa Minimize, Restore, OnRestore
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnMinimize, OnMinimize, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnRestore: Restauration de l'application.
	//!
	//! Cet évènement est déclenché lorsque l'application reprend sa taille
	//! normale, soit parce que l'utilisateur a cliqué sur l'icône dans la barre
	//! de tâches, soit parce que l'application a reçu la commande de
	//! restauration par l'intermédiaire de la méthode @b Restore.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//!
	//! @sa Minimize, Restore, OnMinimize
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnRestore, OnRestore, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnQueryEndSession: Fermeture de la session Windows.
	//!
	//! Cet évènement est déclenché lorsque l'utilisateur ferme la session
	//! Windows ou lorsqu'une application appelle la fonction @b ExitWindows. Vous
	//! pouvez intercepter cet évènement pour faire un traitement (poser une
	//! question à l'utilisateur du genre "Voulez-vous quitter le programme", par
	//! exemple). Mettez la variable @b Accept à false si vous voulez annuler la
	//! fermeture de session. Laissez-la à true (sa valeur par défaut) pour
	//! continuer normalement le processus de fermeture.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Variable Accept
	//!
	//! @note Evitez d'interdire la fermeture de session sans conditions et
	//! confirmation ou message à l'utilisateur, ça pourrait rendre problématique
	//! la fermeture propre du système.
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnQueryEndSession, OnQueryEndSession, TObject *, bool *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnEndSession: fermeture de session
	//!
	//! Cet évènement est déclenché lorsque la session Windows va se terminer.
	//! Vous pouvez traiter l'évènement pour faire une sauvegarde ou fermer
	//! proprement l'application.
	//! @note Dans le cas d'une fermeture de session, les applications encore
	//! actives sont détruites sans appel de l'évènement OnClose ou OnDestroy.
	//! Une fermeture correcte de l'application dans cet évènement est donc
	//! vivement conseillée, surtout si celle-ci doit sauvegarder des données
	//! ou des paramètres en sortie.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnEndSession, OnEndSession, TObject *);

  //@}


};


#else  // TApplicationH

class TApplication;

#endif  // TApplicationH

#undef In_TApplicationH
