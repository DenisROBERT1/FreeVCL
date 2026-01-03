//-----------------------------------------------------------------------------
//! @file TUndoRedo.h
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


#ifndef TUndoRedoH
#define TUndoRedoH


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------
class TUndoRedoUser;


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

#pragma pack(push, 1)

//---------------------------------------------------------------------------
//! @struct BUFFERUNDO
//!
//! @brief Buffer pour le UNDO
//---------------------------------------------------------------------------

#pragma warning(disable : 4200) // Désactive le warning pour structure de taille 0

typedef struct {
  TUndoRedoUser * Sender;
  bool bUndo;
  unsigned int LngData;
  BYTE Data[0];
} BUFFERUNDO, * LPBUFFERUNDO;

typedef struct {
	int Categorie;
  TCHAR Info[0];
} BEGINOP, * LPBEGINOP;

#pragma warning(default : 4200)

#pragma pack(pop)


#define UNDO_NOERR    0   //!< Erreur "Opération réussie"
#define UNDO_ALLUNDO  1   //!< Erreur "Toutes les opérations ont été défaites"
#define UNDO_ALLREDO  2   //!< Erreur "Toutes les opérations ont été refaites"
#define UNDO_NOOP     3   //!< Erreur "Aucune opération en cours"
#define UNDO_EXECOP   4   //!< Erreur "Une opération est en cours d'exécution"


//---------------------------------------------------------------------------
//! @class TUndoRedo
//!
//! @brief Gestion des undo/redo
//!
//! Cette classe (@c TUndoRedo) gère la mémorisation des opérations de
//! modification et l'exécution des undo et redo (défaire / refaire).
//!
//! @author Denis ROBERT
//!
/*! @verbatim
 Comment ça marche ?

 Cette classe écrit dans un fichier séquentiel (UNDOXXXX.TMP dans le
 répertoire temporaire) toutes les opérations faites sur les fichiers. Le
 fichier est géré sous forme de pile (Dernier entré = premier sorti). Chaque
 opération est un ensemble d'opérations élémentaires précédées d'un en-tête
 (UndoRedoUser = NULL) donnant le nom de l'opération. Les blocs
 d'opérations élémentaires doivent être encadrées par des appels aux
 méthodes BeginOperation et EndOperation (ou AbortOperation).

 Les objets utilisant cette classe doivent hériter de TUndoRedoUser, et
 implémenter les méthodes virtuelles ProcessUndo et ProcessRedo. Lorsqu'un
 objet de cette sorte fait une modif, il appelle la fonction MemoModif avec
 deux buffers, un pour le Undo et un autre pour le Redo. Le contenu de ces
 buffers est totalement dépendant de l'objet appelant, chacun pouvant
 adopter ses propres conventions. Ces infos sont empilées telles quelles
 dans le fichier UNDO.TMP.

 Lorsque l'utilisateur déclenche un Undo ou un Redo, cette classe renvoie
 successivement à chaque objet le buffer qu'il lui avait transmis lors de
 l'enregistrement. L'objet effectue alors l'action permettant de faire ou
 défaire cette opération élémentaire.

 Il est possible de définir des catégories d'opérations. Par exemple, pour un
 éditeur de texte, il peut être utile de différentier les déplacements de
 curseur des opérations de modification du texte. La plupart des méthodes
 ont donc un argument @b catégorie. Si vous n'avez qu'une catégorie
 d'opérations, mettez ce paramètre à zéro. La catégorie s'applique à l'ensemble
 de l'opération (c'est à dire qu'on ne peut pas mettre des actions
 élémentaires de catégories différentes).

 Le fichier UNDO.TMP est détruit dans le destructeur de l'objet UndoRedo.

 @endverbatim */
//---------------------------------------------------------------------------
class TUndoRedo {

protected:

  int DebutOp;                         //!< Nombre d'opération en cours (entre BeginOperation et EndOperation)
  bool bOperationName;                 //!< true s'il faut mémoriser le nom de l'opération

  AnsiString asFileUndo;               //!< Nom du fichier Undo
  HANDLE hFileUndo;                    //!< Handle du fichier
  unsigned int PosFileUndo;            //!< Position courante dans le fichier undo

  GLOBALHANDLE hgBufferUndo;           //!< Handle de la zone mémoire du buffer

  bool FASTCALL WriteBuffer(LPBUFFERUNDO lpBufferUndo);
  bool FASTCALL PrevBuffer(LPBUFFERUNDO lpBufferUndo);
  bool FASTCALL NextBuffer(LPBUFFERUNDO lpBufferUndo);

  bool FASTCALL MemoUndoRedo(TUndoRedoUser * Sender,
      void * UndoRedo, unsigned int LenUndoRedo, bool bUndo);

public:

  //---------------------------------------------------------------------------
  //! Création et ouverture d'un fichier UNDO
  //---------------------------------------------------------------------------
  TUndoRedo(void);


  //---------------------------------------------------------------------------
  //! Fermeture et destruction d'un fichier UNDO
  //---------------------------------------------------------------------------
  virtual ~TUndoRedo(void);


  //---------------------------------------------------------------------------
  //! @brief Effacement de toutes les données Undo
  //!
  //! Cette fonction remet à zéro les opérations mémorisées. De ce fait, aucun
  //! Undo ni Redo ne peut être effectué ensuite (sauf si d'autres opérations
  //! sont enregistrées).
  //!
  //! @warning Aucune opération ne doit être en cours d'exécution. C'est à dire
  //! que la fonction EndOperation doit avoir été appelée
  //---------------------------------------------------------------------------
  bool FASTCALL Clear(void);


  //---------------------------------------------------------------------------
  //! @brief Début d'une opération pouvant être défaite ou refaite
  //!
  //! Début d'une opération pouvant être défaite ou refaite. Les appels peuvent
  //! être imbriqués, mais à chaque BeginOperation doit correspondre un appel
  //! à EndOperation.
  //!
  //! @param[in] Categorie Catégorie de l'opération
  //! @param[in] asOperationName Nom de l'opération
  //!
  //! @return @c bool true si Ok
  //!
  //! @warning L'appel successif de BeginOperation, MemoModif, EndOperation
  //! doit être fait en une seule fois et sans laisser la main à l'utilisateur
  //! (pour éviter qu'il ne puisse faire un undo/redo en cours d'opération).
  //---------------------------------------------------------------------------
  bool FASTCALL BeginOperation(int Categorie, AnsiString asOperationName);


  //---------------------------------------------------------------------------
  //! @brief Fin d'une opération pouvant être défaite ou refaite
  //!
  //! Fin d'une opération pouvant être défaite ou refaite. Chaque EndOperation
  //! doit correspondre à un BeginOperation.
  //!
  //! @return @c bool true si Ok. La fonction renvoie false si BeginOperation
  //!         n'a pas été lancé, mais aussi si aucune modif n'est intervenue
  //!         entre le BeginOperation et le EndOperation.
  //---------------------------------------------------------------------------
  bool FASTCALL EndOperation(void);


  //---------------------------------------------------------------------------
  //! @brief Annulation d'une opération en cours d'exécution
  //!
  //! Effectue une opération undo sur l'opération en cours (en général suite à
  //! une erreur, cela permet de garder des fichiers cohérents). En sortie de
  //! fonction, aucune opération n'est en cours (c'est à dire qu'il faut appeler
  //! BeginOperation avant de refaire des modifs).
  //!
  //! @param[out] asOperationName nom de l'opération qui a été annulée
  //!
  //! @return @c bool true si Ok. La fonction renvoie false si BeginOperation
  //!         n'a pas été lancé, mais aussi si aucune modif n'est intervenue
  //!         entre le BeginOperation et le AbortOperation.
  //!
  //! @warning Une opération doit être en cours d'exécution. C'est à dire que la
  //! fonction BeginOperation doit avoir été appelée
  //---------------------------------------------------------------------------
  bool FASTCALL AbortOperation(AnsiString * asOperationName);


  //---------------------------------------------------------------------------
  //! @brief Mémorisation d'une modification
  //!
  //! Cette fonction mémorise une modification (une opération élémentaire)
  //!
  //! @param[in] Sender Objet appelant
  //! @param[in] Undo Buffer à passer à la fonction FnctUndo
  //! @param[in] LenUndo Longueur du buffer Undo
  //!
  //! @return @c bool true si Ok
  //!
  //! @warning L'appel de BeginOperation est obligatoire avant l'appel ou une
  //! série d'appel à cette fonction.
  //---------------------------------------------------------------------------
  bool FASTCALL MemoUndo(TUndoRedoUser * Sender, void * Undo, unsigned int LenUndo);


  //---------------------------------------------------------------------------
  //! @brief Mémorisation d'une modification
  //!
  //! Cette fonction mémorise une modification (une opération élémentaire)
  //!
  //! @param[in] Sender Objet appelant
  //! @param[in] Redo Buffer à passer à la fonction FnctRedo
  //! @param[in] LenRedo Longueur du buffer Redo
  //!
  //! @return @c bool true si Ok
  //!
  //! @warning L'appel de BeginOperation est obligatoire avant l'appel ou une
  //! série d'appel à cette fonction.
  //---------------------------------------------------------------------------
  bool FASTCALL MemoRedo(TUndoRedoUser * Sender, void * Redo, unsigned int LenRedo);


  //---------------------------------------------------------------------------
  //! @brief Effectue une opération undo
  //!
  //! Cette fonction effectue une opération undo
  //!
  //! @param[out] asOperationName nom de l'opération qui a été défaite
  //!
  //! @return @c bool true si Ok
  //!
  //! @warning Aucune opération ne doit être en cours d'exécution. C'est à dire
  //! que la fonction EndOperation doit avoir été appelée
  //---------------------------------------------------------------------------
  bool FASTCALL Undo(AnsiString * asOperationName);


  //---------------------------------------------------------------------------
  //! @brief Effectue une opération redo
  //!
  //! Cette fonction effectue une opération redo
  //!
  //! @param[out] asOperationName nom de l'opération qui a été refaite
  //!
  //! @return @c bool true si Ok
  //!
  //! @warning Aucune opération ne doit être en cours d'exécution. C'est à dire
  //! que la fonction EndOperation doit avoir été appelée
  //---------------------------------------------------------------------------
  bool FASTCALL Redo(AnsiString * asOperationName);


  //---------------------------------------------------------------------------
  //! @brief Liste des modifs
  //!
  //! Cette fonction renvoie la liste des premières modifications.
  //!
  //! @param[in] Categorie Catégorie de l'opération (-1 pour toutes)
  //!
  //! @return @c AnsiString liste des trois premières modifications
  //---------------------------------------------------------------------------
  AnsiString GetModificationsList(int Categorie);


	//---------------------------------------------------------------------------
  //! @brief Teste si toutes les opérations ont été défaites
  //!
  //! Cette fonction renvoie true si toutes les opérations ont été défaites
  //!
  //! @param[in] Categorie Catégorie de l'opération (-1 pour toutes)
  //!
  //! @return @c bool true si la liste des opérations est vide.
  //---------------------------------------------------------------------------
  bool FASTCALL IsEmpty(int Categorie);

};

//---------------------------------------------------------------------------
//! @class TUndoRedoUser
//!
//! @brief Classe de base d'un utilisateur de undo/redo
//!
//! Cette classe (@c TUndoRedoUser) est la classe de base des utilisateurs
//! des undo et redo (défaire / refaire). Les classes dérivées doivent
//! implémenter les deux méthodes Undo et Redo (voir classe TUndoRedo). Il
//! est conseillé d'hériter de cette classe de façon protégée, de façon à
//! ce que les méthodes @b ProcessUndo et @b ProcessRedo ne soient pas
//! accessibles de l'extérieur.
//!
//! @author Denis ROBERT
//!
//! @sa TUndoRedo
//---------------------------------------------------------------------------
class TUndoRedoUser {
public:

	//---------------------------------------------------------------------------
  //! @brief Effectue une opération élémentaire Undo
  //!
  //! Cette méthode est appelée par l'objet Undo/Redo lorsque une action
	//! "défaire" est en cours. Ne pas appeler cette méthode, seule la classe
	//! UndoRedo est habilitée à l'utiliser.
  //!
  //! @param[out]       Undo Buffer Undo
  //! @param[in]        LenUndo Longueur du buffer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool ProcessUndo(void * Undo, int LenUndo) = 0;

	//---------------------------------------------------------------------------
  //! @brief Effectue une opération élémentaire Redo
  //!
  //! Cette méthode est appelée par l'objet Undo/Redo lorsque une action
	//! "refaire" est en cours. Ne pas appeler cette méthode, seule la classe
	//! UndoRedo est habilitée à l'utiliser.
  //!
  //! @param[out]       Redo Buffer Redo
  //! @param[in]        LenRedo Longueur du buffer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

	virtual bool ProcessRedo(void * Redo, int LenRedo) = 0;

};

#endif
