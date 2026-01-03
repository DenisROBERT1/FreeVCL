//-----------------------------------------------------------------------------
//! @file TAnimate.h
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

#ifdef In_TAnimateH
#error "Inclusion circulaire de TAnimate"
#endif // In_TAnimateH

#define In_TAnimateH

#ifndef TAnimateH
#define TAnimateH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TCommonAVI {
	aviNone,                      //!< Aucune
	aviFindFolder,                //!< Recherche d'un emplacement
	aviFindFile,                  //!< Recherche d'un fichier
	aviFindComputer,              //!< Recherche d'un ordinateur sur le réseau
  aviCopyFiles,                 //!< Copie de plusieurs fichiers
	aviCopyFile,                  //!< Copie d'un fichier
	aviRecycleFile,               //!< Restauration de fichiers depuis la corbeille
	aviEmptyRecycle,              //!< Vidage de la corbeille
  aviDeleteFile                 //!< Destruction de fichier
};


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnOpen.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnOpen)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnStart.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnStart)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnStop.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnStop)(TObject *Sender);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnClose2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnClose2)(TObject *Sender);


//-----------------------------------------------------------------------------
//! @class TAnimate
//!
//! @brief Image animée
//! @image html TAnimate.gif "TAnimate"
//!
//! Cette classe permet d'afficher une image animée (.avi), soit parmi celles
//! pré-définies dans le système, soit une image définie par l'utilisateur dans
//! les resssources ou un fichier AVI externe.@n
//! L'image affichée est définie par les propriétés @b CommonAVI, @b FileName,
//! @b ResID ou @b ResName. Ensuite, utilisez les méthodes @b Play et @b Stop
//! pour lancer et arrêter l'animation.
//! @warning Sur les systèmes VISTA et SEVEN, les images animées système
//! nécessitent une DLL supplémentaire dans le répertoire système pour que
//! la fonctionnalité @b CommonAVI continue à fonctionner. Il est donc
//! recommandé de n'utiliser que des animations utilisateur (fichier ou
//! ressources). Voir propriété TAnimate::CommonAVI pour plus de détails.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TAnimate: public TWinControl {
private:
  HINSTANCE hDllShell32;
protected:
  bool FActive;
  TCommonAVI FCommonAVI;
  AnsiString FFileName;
  int FFrameCount;
  int FFrameHeight;
  int FFrameWidth;
  bool FOpen;
  int FRepetitions;
  HINSTANCE FResHandle;
  int FResId;
  AnsiString FResName;
  short FStartFrame;
  short FStopFrame;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TAnimate(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TAnimate(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Joue un intervalle de l'animation AVI.
  //!
  //! Cette méthode permet de jouer @b Count fois la séquence d'images allant
	//! de l'image numéro @b FromFrame à l'image numéro @b ToFrame (incluses).
	//! @b ToFrame doit être supérieur ou égal à @b FromFrame, ou bien égal à
	//! zéro (signifie dans ce cas la dernière image). Si @b Count est égal à
	//! zéro, la séquence est jouée jusqu'à l'appel de la méthode @b Stop.
	//! L'appel de cette méthode a le même effet que l'affectation des
	//! propriétés @b StartFrame, @b StopFrame, @b Repetitions, puis de
	//! la valeur true à la propriété @b Active.
  //!
  //! @param[in]        FromFrame Première image à afficher (première du fichier = 1)
  //! @param[in]        ToFrame Dernière image à afficher (0 = Dernière du fichier)
  //! @param[in]        Count Nombre de répétitions (0 = boucle infinie)
  //---------------------------------------------------------------------------
  void FASTCALL Play(Word FromFrame, Word ToFrame, int Count);

  //---------------------------------------------------------------------------
  //! @brief Réinitialise l'animation
	//!
	//! Cette méthode permet de réinitialiser l'animation. Les évènements
	//! @b OnClose et @b OnOpen sont déclenchés. Appelez la méthode @b Play pour
	//! relancer l'animation.
  //---------------------------------------------------------------------------
  void FASTCALL Reset(void);

  //---------------------------------------------------------------------------
  //! @brief Affiche une image isolée de l'animation
	//!
	//! Cette méthode affiche une image isolée de l'animation (arrêt sur image).
	//! Frame doit être inférieure ou égale au nombre d'images de l'animation.
	//! @note L'appel de cette méthode provoque l'ouverture de l'image (c'est à
	//! dire met la propriété @b Open à true).
  //!
  //! @param[in]        Frame Numéro d'image à afficher (première du fichier = 1)
  //---------------------------------------------------------------------------
  void FASTCALL Seek(short Frame);

  //---------------------------------------------------------------------------
  //! @brief Stoppe l'animation
	//!
	//! Cette méthode permet d'interrompre l'animation. Si l'animation était déjà
	//! stoppée, cette méthode ne fait rien. L'appel de @b Stop a le même effet
	//! que l'affectation de la valeur false à la propriété @b Active.
  //---------------------------------------------------------------------------
  void FASTCALL Stop(void);

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
  //! @brief Propriété Active: true si l'animation est en train d'être jouée.
	//!
	//! Cette propriété permet de jouer l'animation depuis l'image numéro
	//! @b StartFrame jusqu'à l'image numéro @b StopFrame, et ceci un nombre de
	//! fois égal à @b Repetitions.
	//! @note Mettez la propriété @b Open à true avant d'activer l'animation.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, bool, Active);


  //---------------------------------------------------------------------------
  //! @brief Propriété Center: Positionnement de l'animation dans le contrôle.
	//!
	//! Si @b Center est égal à true, l'animation est centrée dans le contrôle.
	//! Sinon, elle est positionnée sur le coin supérieur gauche. Cette propriété
	//! n'a évidemment de signification que si la largeur ou la hauteur du
	//! contrôle sont plus grandes que les dimensions de l'animation (propriétés
	//! @b FrameWidth et @b FrameHeight).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, bool, Center);


  //---------------------------------------------------------------------------
  //! @brief Propriété CommonAVI: Indique l'animation système à afficher
	//!
	//! Cette méthode permet d'afficher une animation standard du système
	//! indiquant les opérations utilisateur courantes, copie ou la destruction
	//! de fichiers, par exemple. Les animations sont lues dans le fichier
	//! Shell32.dll ou ShellXP32.dll du répertoire système (Windows@\System32 par
	//! exemple) selon votre version de système (voir ci-dessous).
	//! @warning Sur les systèmes VISTA et suivants, le format des animations
	//! dans le fichier Shell32.dll a été modifié. Par conséquent, il n'est plus
	//! possible de lire les animations standard dans ce fichier. Par souci de
	//! compatibilité, on lit donc un fichier nommé ShellXP32.dll. Ce fichier
	//! n'est pas standard, il vous faudra le créer et l'installer avec votre
	//! application.@n
	//! Une solution facile serait de le récupérer et de le renommer depuis un
	//! système XP, mais ça ne serait pas légal.
	//! @note L'affectation d'une valeur à la propriété @b CommonAVI remet
	//! automatiquement à zéro ou à chaîne vide les autres propriétés
	//! similaires (@b FileName, @b ResID et @b ResName).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, TCommonAVI, CommonAVI);


  //---------------------------------------------------------------------------
  //! @brief Propriété FileName: Indique un fichier AVI à afficher
	//!
	//! Cette méthode permet d'afficher une animation à partir d'un fichier AVI.
	//! La propriété contient le chemin complet ou relatif au répertoire courant
	//! du fichier à afficher.@n
	//! Le fichier AVI ne doit pas contenir de son.
	//! @note L'affectation d'une valeur à la propriété @b FileName remet
	//! automatiquement à zéro ou à chaîne vide les autres propriétés
	//! similaires (@b CommonAVI, @b ResID et @b ResName).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, AnsiString, FileName);

  //---------------------------------------------------------------------------
  //! @brief Propriété FrameCount: Nombre total d'images dans l'animation.
  //!
	//! Lisez la propriété @b FrameCount pour connaître le nombre total d'image
	//! dans l'animation. La propriété @b StopFrame doit être inférieure ou égale
	//! à @b FrameCount.
	//! @bug Ne marche pas dans la version actuelle. Renvoie toujours zéro.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TAnimate, int, FrameCount);


  //---------------------------------------------------------------------------
  //! @brief Propriété FrameHeight: Hauteur de l'animation en pixels.
	//!
	//! Affectez la propriété @b FrameHeight pour dimensionner verticalement
	//! l'animation dans le contrôle. La position de cette animation dans le
	//! contrôle est déterminé par la propriété TAnimate::Center.
	//! @note Si @b AutoSize a la valeur true, le contrôle animation se
	//! redimensionne automatiquement lorsque @b FrameHeight est modifié.
	//! @todo Aujourd'hui cette propriété est en lecture seule uniquement
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TAnimate, int, FrameHeight);


  //---------------------------------------------------------------------------
  //! @brief Propriété FrameWidth: Largeur de l'animation en pixels.
	//!
	//! Affectez la propriété @b FrameHeight pour dimensionner horizontalement
	//! l'animation dans le contrôle. La position de cette animation dans le
	//! contrôle est déterminé par la propriété TAnimate::Center.
	//! @note Si @b AutoSize a la valeur true, le contrôle animation se
	//! redimensionne automatiquement lorsque @b FrameHeight est modifié.
	//! @todo Aujourd'hui cette propriété est en lecture seule uniquement
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TAnimate, int, FrameWidth);


  //---------------------------------------------------------------------------
  //! @brief Propriété Open: Indique si l'animation est prête à s'afficher.
	//!
	//! La propriété @b Open indique si l'animation est chargée en mémoire. Quand
	//! les propriétés @b CommonAVI, @b FileName, @b ResName ou @b ResID sont
	//! affectées, l'animation est automatiquement ouverte. Si Open a la valeur
	//! true, la première image (spécifiée par @b StartFrame) est affichée
	//! jusqu'à ce que l'animation devienne active par l'affectation de la valeur
	//! true à la propriété @b Active ou par l'appel de la méthode @b Play.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, bool, Open);


  //---------------------------------------------------------------------------
  //! @brief Propriété Repetitions: Nombre de répétitions de l'animation
	//!
	//! Indique le nombre de fois que sera jouée l'animation à partir du moment
	//! où elle sera active. Une valeur de @b Repetition égale à zéro signifie
	//! que l'animation est jouée sans arrêt jusqu'à l'affectation de la valeur
	//! false à la propriété @b Active ou par l'appel de la méthode @b Stop.
	//! @note Cette propriété est équivalente au paramètre @b Count de la
	//! méthode @b Play
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, int, Repetitions);


  //---------------------------------------------------------------------------
  //! @brief Propriété ResHandle: Handle du module contenant la ressource AVI.
	//!
	//! Affectez à la propriété @b ResHandle la valeur du handle d'instance ou de
	//! module contenant la ressource AVI. Affectez ensuite une valeur à la
	//! propriété @b ResID ou à la propriété @b ResName pour indiquer la
	//! ressource correspondante à l'animation dans ce module.@n
	//! La valeur par défaut de la propriété @b ResHandle est le handle
	//! d'instance de l'application (HInstance).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, HINSTANCE, ResHandle);


  //---------------------------------------------------------------------------
  //! @brief Propriété ResId: Identifiant de la ressource AVI.
	//!
	//! Indique l'identifiant de la ressource à jouer dans le module spécifié
	//! par la propriété @b ResHandle (par défaut le module courant).
	//! Si la ressource est identifiée par un nom, utilisez la propriété
	//! @b ResName.@n
	//! La ressource AVI ne doit pas contenir de son.
	//! @note L'affectation d'une valeur à la propriété @b ResId remet
	//! automatiquement à zéro ou à chaîne vide les autres propriétés
	//! similaires (@b CommonAVI, @b FileName et @b ResName).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, int, ResId);


  //---------------------------------------------------------------------------
  //! @brief Propriété ResName: Nom de la ressource AVI
	//!
	//! Indique le nom de la ressource à jouer dans le module spécifié
	//! par la propriété @b ResHandle (par défaut le module courant).
	//! Si la ressource est identifiée par un numéro, utilisez la propriété
	//! @b ResId.@n
	//! La ressource AVI ne doit pas contenir de son.
	//! @note L'affectation d'une valeur à la propriété @b ResName remet
	//! automatiquement à zéro ou à chaîne vide les autres propriétés
	//! similaires (@b CommonAVI, @b FileName et @b ResID).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, AnsiString, ResName);


  //---------------------------------------------------------------------------
  //! @brief Propriété StartFrame: Première image à jouer.
	//!
	//! Indique la première image de l'animation à jouer lorsqu'elle sera active
	//! (la première du fichier a le numéro 1), et le point de départ des
	//! occurrences suivantes si la propriété @b Repetitions est supérieur à 1.
	//! @note Cette propriété est équivalente au paramètre @b FromFrame de la
	//! méthode @b Play
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, short, StartFrame);


  //---------------------------------------------------------------------------
  //! @brief Propriété StopFrame: Dernière image à jouer.
	//!
	//! Indique la dernière image de l'animation à joue lorsqu'elle sera active
	//! (la première du fichier a le numéro 1). Ce nombre doit être compris entre
	//! @b StartFrame et @b FrameCount, ou bien être égal à zéro (dans ce cas
	//! l'animation est jouée jusqu'à la dernière image).
	//! @note Cette propriété est équivalente au paramètre @b ToFrame de la
	//! méthode @b Play
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, short, StopFrame);


  //---------------------------------------------------------------------------
  //! @brief Propriété Timers: Utilisation de timers.
	//!
	//! Si @b Timers est à false (valeur par défaut), l'animation est jouée dans
	//! un thread séparé. Sinon, un timer est créé en interne, ce qui permet de
	//! synchroniser l'animation avec d'autres actions, par exemple un son.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, bool, Timers);


  //---------------------------------------------------------------------------
  //! @brief Propriété Transparent: couleur de fond transparente.
	//!
	//! Si cette propriété est à true, l'animation est dessinée avec une couleur
	//! de fond transparente. La couleur de fond (qui devient transparente dans
	//! ce cas) est définie en interne dans l'animation. Sinon, le fond est
	//! dessiné de façon opaque.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TAnimate, bool, Transparent);


  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnClose: Fermeture de l'animation
	//!
	//! Evènement déclenché lorsque l'animation est fermée (par l'affectation de
	//! la valeur false à la propriété @b Open).@n
	//! Pour effectuer une action à la fin de l'exécution de l'animation utilisez
	//! l'évènement @b OnStop.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnClose2, OnClose, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnOpen: Ouverture de l'animation 
	//!
	//! Evènement déclenché lorsque l'animation est ouverte (par l'affectation de
	//! la valeur true à la propriété @b Open, par exemple).@n
	//! Pour effectuer une action au démarrage de l'animation utilisez
	//! l'évènement @b OnStart.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnOpen, OnOpen, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnStart: Démarrage de l'animation
	//!
	//! Evènement déclenché lorsque l'animation démarre (par l'affectation de
	//! la valeur true à la propriété @b Active, ou bien par l'appel de la
	//! méthode @b Play ou @b Seek).@n
	//! Pour effectuer une action au chargement de l'animation utilisez
	//! l'évènement @b OnOpen.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnStart, OnStart, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnStop: Arrêt de l'animation
	//!
	//! Evènement déclenché lorsque l'animation s'arrête (par l'affectation de
	//! la valeur false à la propriété @b Active, ou bien par l'appel de la
	//! méthode @b Stop).@n
	//! Pour effectuer une action lors de la fermeture de l'animation utilisez 
	//! l'évènement @b OnClose.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnStop, OnStop, TObject *);

  //@}


};


#else  // TAnimateH

class TAnimate;

#endif  // TAnimateH


#undef In_TAnimateH
