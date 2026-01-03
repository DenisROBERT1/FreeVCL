//-----------------------------------------------------------------------------
//! @file Plugin.h
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
#ifndef Plugin_H
#define Plugin_H

#ifdef DLLIMPORT
#undef DLLIMPORT
#endif /* DLLIMPORT */

#ifdef _USRDLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not _USRDLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not _USRDLL */

//---------------------------------------------------------------------------
//! @brief Enums
//---------------------------------------------------------------------------

enum TYPESTEPBYSTEP {
	tsbs_Continue,
	tsbs_Next,
	tsbs_In,
	tsbs_Out
};

enum TYPEDISPLAYVALUE {
	tdv_Decimal,
	tdv_Hexa
};

//---------------------------------------------------------------------------
//! @brief Types de fonctions
//---------------------------------------------------------------------------

typedef BOOL (WINAPI * CALLBACK_ERROR) (const wchar_t *wszMessage, const wchar_t *wszTitle, int Flags);
typedef BOOL (WINAPI * CALLBACK_DEFVAR) (const wchar_t *wszVarName, int TypeDefinition, const wchar_t *wszDescription, const wchar_t *wszTypeContenant, const wchar_t *wszTypeVariable, const wchar_t *wszFileName, int NumLine, int NumCol);

typedef BOOL (WINAPI * PLUGIN_INSTALL) (void);
typedef BOOL (WINAPI * PLUGIN_INIT) (CALLBACK_ERROR CallbackError);
typedef HBITMAP (WINAPI * PLUGIN_GETBITMAP20) (void);
typedef HBITMAP (WINAPI * PLUGIN_GETBITMAP32) (void);
typedef HBITMAP (WINAPI * PLUGIN_GETBITMAP48) (void);
typedef BOOL (WINAPI * PLUGIN_GETDESCRIPTION) (wchar_t *wszDescription, int SizeDescription);
typedef BOOL (WINAPI * PLUGIN_CREATE) (wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd);
typedef BOOL (WINAPI * PLUGIN_OPEN) (wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd);
typedef BOOL (WINAPI * PLUGIN_GETINPUTS) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szInputFileNames);
typedef BOOL (WINAPI * PLUGIN_FREE_GETINPUTS) (const wchar_t *szInputFileNames);
typedef BOOL (WINAPI * PLUGIN_GETOUTPUT) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szOutputFileName);
typedef BOOL (WINAPI * PLUGIN_FREE_GETOUTPUT) (const wchar_t *szOutputFileName);
typedef BOOL (WINAPI * PLUGIN_ADDFILE) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szFileName);
typedef BOOL (WINAPI * PLUGIN_REMOVEFILE) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szFileName);
typedef BOOL (WINAPI * PLUGIN_RENAMEFILE) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szOldFileName, const wchar_t *szNewFileName);
typedef BOOL (WINAPI * PLUGIN_PRECOMPIL) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent, CALLBACK_DEFVAR CallbackDefVar);
typedef BOOL (WINAPI * PLUGIN_BUILD) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent);
typedef BOOL (WINAPI * PLUGIN_RUN) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent);
typedef BOOL (WINAPI * PLUGIN_STARTDEBUG) (const wchar_t *szProjectFileName, const wchar_t *szProjectContent);
typedef BOOL (WINAPI * PLUGIN_STOPDEBUG) (void);
typedef BOOL (WINAPI * PLUGIN_STEPBYSTEP) (TYPESTEPBYSTEP TypeStepByStep, const wchar_t **szFileName, int *NumLine1, int *NumCol1, int *NumLine2, int *NumCol2);
typedef BOOL (WINAPI * PLUGIN_ADDBREAKPOINT) (const wchar_t *szFileName, int NumLine, int NumCol);
typedef BOOL (WINAPI * PLUGIN_DELBREAKPOINT) (const wchar_t *szFileName, int NumLine, int NumCol);
typedef BOOL (WINAPI * PLUGIN_CLEARBREAKPOINTS) (void);
typedef BOOL (WINAPI * PLUGIN_GETCALLSTACK) (const wchar_t **szCallStack);
typedef BOOL (WINAPI * PLUGIN_GETVARIABLEVALUE) (const wchar_t *szVarName, wchar_t *szVarValue, int CountMax);

//---------------------------------------------------------------------------
//! @brief Bitmap 20 x 20
//!
//! Cette fonction renvoie le handle d'une bitmap 20 x 20 pixels. Cette image
//! est utilisée pour identifier le type de projet dans les boîtes de
//! dialogues.
//!
//! @return @c HBITMAP Handle de bitmap Windows
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap20(void);

//---------------------------------------------------------------------------
//! @brief Bitmap 32 x 32
//!
//! Cette fonction renvoie le handle d'une bitmap 32 x 32 pixels. Cette image
//! est utilisée pour identifier le type de projet dans les boîtes de
//! dialogues.
//!
//! @return @c HBITMAP Handle de bitmap Windows
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap32(void);

//---------------------------------------------------------------------------
//! @brief Bitmap 48 x 48
//!
//! Cette fonction renvoie le handle d'une bitmap 48 x 48 pixels. Cette image
//! est utilisée pour identifier le type de projet dans les boîtes de
//! dialogues.
//!
//! @return @c HBITMAP Handle de bitmap Windows
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT HBITMAP WINAPI Plugin_GetBitmap48(void);

//---------------------------------------------------------------------------
//! @brief Description du type de projet.
//!
//! Cette fonction renvoie une description courte (une ligne) permettant
//! d'identifier le type de projet dans les boîtes de dialogue.
//!
//! @param[in, out]    wszDescription Texte de la description.
//! @param[in]         SizeDescription Taille du buffer wszDescription.
//!
//! @return @c AnsiString Description
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetDescription(wchar_t *wszDescription, int SizeDescription);

//---------------------------------------------------------------------------
//! @brief Création d'un projet
//!
//! Cette fonction permet de créer un nouveau projet. La mémorisation du
//! projet fonctionne comme lors de l'ouverture (voir fonction
//! @b Plugin_Open).@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//! Ne pas retourner FALSE sans information à l'utilisateur.
//!
//! @param[out]        szProjectFileName Nom du fichier projet.
//! @param[out]        szProjectContent Contenu du projet (longueur maxi: 2048 octets).
//! @param[in]         hWnd Handle de la fenêtre parente des boîtes de 
//!                    dialogues et des messages d'erreur.
//!
//! @return @c BOOL TRUE si ouverture réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Create(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd);

//---------------------------------------------------------------------------
//! @brief Ouverture du projet
//!
//! Cette fonction permet d'ouvrir un projet pour le modifier ou pour le
//! créer. Il y a deux modes de mémorisation possibles: dans un fichier ou
//! directement dans le XML du WorkSpace. Dans le premier cas le premier
//! paramètre est utilisé (en entrée comme en sortie). Dans le deuxième cas,
//! c'est le deuxième paramètre qui est utilisé. Le paramètre non utilisé
//! est vide.@n
//! S'il s'agit d'un nouveau projet, les deux premiers paramètres sont
//! vides. Le plugin peut soit se charger de l'enregistrement d'un fichier
//! projet et retourner un nom de fichier, soit retourner un contenu dans
//! le deuxième paramètre, contenu qui sera mémorisé par l'IDE.@n
//! Le contenu du projet est dépendant de chaque projet, mais il doit
//! être de longueur modérée car il est mémorisé dans le fichier XML.@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//! Ne pas retourner FALSE sans information à l'utilisateur.
//!
//! @param[in, out]    szProjectFileName Nom du fichier projet. Vide si nouveau
//!                    projet. Dans ce cas szProjectFileName contient le nouveau
//!                    nom en retour de la fonction.
//! @param[in, out]    szProjectContent Contenu du projet. Vide si nouveau projet.
//!                    Dans ce cas szProjectContent contient le nouveau contenu
//!                    en retour de la fonction (longueur maxi: 2048 octets).
//! @param[in]         hWnd Handle de la fenêtre parente des boîtes de 
//!                    dialogues et des messages d'erreur.
//!
//! @return @c BOOL TRUE si ouverture réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Open(wchar_t *szProjectFileName, wchar_t *szProjectContent, HWND hWnd);

//---------------------------------------------------------------------------
//! @brief Liste des fichiers en entrée.
//!
//! Cette fonction renvoie la liste des fichiers en entrée, séparés par un
//! caractère zéro (double séro à la fin de la liste)
//!
//! @param[in]         szProjectFileName Nom du fichier projet (vide si utilisation szProjectContent)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//! @param[in, out]    szInputFileNames Nom des fichiers en entrée.
//!
//! @return @c BOOL TRUE si opération réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetInputs(const wchar_t *szProjectFileNames, const wchar_t *szProjectContent, const wchar_t **szInputFileNames);

//---------------------------------------------------------------------------
//! @brief Libération du buffer de la liste des fichiers en entrée.
//!
//! Cette fonction libère le buffer alloué par la fonction Plugin_GetInputs.
//! Ne plus utiliser le pointeur après le retour de cette fonction.
//!
//! @param[in, out]    szInputFileNames Pointeur du buffer à libérer.
//!
//! @return @c BOOL TRUE si opération réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetInputs(const wchar_t *szInputFileNames);

//---------------------------------------------------------------------------
//! @brief Nom du fichier de sortie.
//!
//! Cette fonction renvoie la liste des fichiers en sortie, séparés par un
//! caractère zéro (double séro à la fin de la liste)
//!
//! @param[in]         szProjectFileName Nom du fichier projet (vide si utilisation szProjectContent)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//! @param[in, out]    szOutputFileName Nom du fichier en sortie.
//!
//! @return @c BOOL TRUE si opération réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetOutput(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t **szOutputFileName);

//---------------------------------------------------------------------------
//! @brief Libération du buffer du nom du fichier de sortie.
//!
//! Cette fonction libère le buffer alloué par la fonction Plugin_GetOutput.
//! Ne plus utiliser le pointeur après le retour de cette fonction.
//!
//!
//! @param[in, out]    szOutputFileName Pointeur du buffer à libérer.
//!
//! @return @c BOOL TRUE si opération réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Free_GetOutput(const wchar_t *szOutputFileName);

//---------------------------------------------------------------------------
//! @brief Ajout d'un fichier au projet
//!
//! Cette fonction ajoute un fichier au projet. Elle est facultative, certains
//! plugins peuvent ne pas l'implémenter. Dans ce cas, les options du menu
//! correspondantes sont grisées.@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//! @param[in]         szFileName Nom du fichier à ajouter (ne peut pas être vide)
//!
//! @return @c BOOL TRUE si opération réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_AddFile(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szFileName);

//---------------------------------------------------------------------------
//! @brief Suppression d'un fichier du projet
//!
//! Cette fonction enlève un fichier du projet. Elle est facultative, certains
//! plugins peuvent ne pas l'implémenter.@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//! @param[in]         szFileName Nom du fichier à enlever (ne peut pas être vide)
//!
//! @return @c BOOL TRUE si opération réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_RemoveFile(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szFileName);

//---------------------------------------------------------------------------
//! @brief Renommage d'un fichier du projet
//!
//! Cette fonction renomme un fichier du projet. Elle est facultative, certains
//! plugins peuvent ne pas l'implémenter. Cette fonction ne renomme pas
//! physiquement le fichier, elle ne fait que changer le nom dans le projet.
//! Le renommage réel du fichier est fait par l'IDE FreeVCL.@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//! @param[in]         szOldFileName Nom du fichier à renommer (ne peut pas être vide)
//! @param[in]         szNewFileName Nouveau nom du fichier (ne peut pas être vide)
//!
//! @return @c BOOL TRUE si opération réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_RenameFile(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, const wchar_t *szOldFileName, const wchar_t *szNewFileName);

//---------------------------------------------------------------------------
//! @brief Pré-compilation d'un projet
//!
//! Cette fonction effectue une pré-compilation du projet (analyse des
//! commandes pré-processeur, et éventuellement analyse succincte des
//! définitions de variables et prototypes de fonctions).@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//! @param[in]         CallbackDefVar Fonction de retour pour enregistrement des variables
//!
//! @return @c BOOL TRUE si pré-comilation réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_PreCompil(const wchar_t *szProjectFileName, const wchar_t *szProjectContent, CALLBACK_DEFVAR CallbackDefVar);

//---------------------------------------------------------------------------
//! @brief Construction d'un projet
//!
//! Cette fonction effectue un build du projet (qui est dépendant du type de
//! projet lui-même).@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//!
//! @return @c BOOL TRUE si construction réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Build(const wchar_t *szProjectFileName, const wchar_t *szProjectContent);

//---------------------------------------------------------------------------
//! @brief Exécution d'un projet
//!
//! Cette fonction lance l'exécution du projet (sans débuggage). Elle est
//! facultative, certains plugins peuvent ne pas l'implémenter. Dans ce cas,
//! les options du menu correspondantes sont grisées.@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//!
//! @return @c BOOL TRUE si construction réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_Run(const wchar_t *szProjectFileName, const wchar_t *szProjectContent);

//---------------------------------------------------------------------------
//! @brief Démarrage du débuggage d'un projet
//!
//! Cette fonction lance le débuggage du projet. Elle est facultative,
//! certains plugins peuvent ne pas l'implémenter. Dans ce cas, les options
//! du menu correspondantes sont grisées.@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//!
//! @return @c BOOL TRUE si construction réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_StartDebug(const wchar_t *szProjectFileName, const wchar_t *szProjectContent);

//---------------------------------------------------------------------------
//! @brief Arrêt du débuggage d'un projet
//!
//! Cette fonction stoppe le débuggage du projet. Elle est facultative,
//! certains plugins peuvent ne pas l'implémenter. Dans ce cas, les options
//! du menu correspondantes sont grisées.@n
//! Le plugin est en charge de l'affichage des messages d'erreur éventuels.
//!
//! @param[in]         szProjectFileName Nom du fichier projet (peut être vide)
//! @param[in]         szProjectContent Contenu du fichier projet (vide si utilisation szProjectFileName)
//!
//! @return @c BOOL TRUE si construction réussie.
//---------------------------------------------------------------------------
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_StopDebug(void);

EXTERN_C DLLIMPORT BOOL WINAPI Plugin_StepByStep(TYPESTEPBYSTEP TypeStepByStep, const wchar_t **szFileName, int *NumLine1, int *NumCol1, int *NumLine2, int *NumCol2);
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_AddBreakpoint(const wchar_t *szFileName, int NumLine, int NumCol);
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_DelBreakpoint(const wchar_t *szFileName, int NumLine, int NumCol);
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_ClearBreakpoints(void);
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetCallStack(const wchar_t **szCallStack);
EXTERN_C DLLIMPORT BOOL WINAPI Plugin_GetVariableValue(const wchar_t *szVarName, wchar_t *szVarValue, int CountMax);

#endif /* Plugin_H */
