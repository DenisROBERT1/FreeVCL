//-----------------------------------------------------------------------------
//! @file TBaseTree.h
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

#ifndef TBaseTreeH
#define TBaseTreeH

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TBaseTree;


//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnChangeText.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnChangeText)(TObject *Sender, AnsiString NewText);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnInsertNode.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnInsertNode)(TObject *Sender, int Index, TBaseTree *NewNode);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnSupprNode.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnSupprNode)(TObject *Sender, int Index);

//---------------------------------------------------------------------------
//! @class TBaseTree
//!
//! @brief Classe de base des trees pouvant être synchronisés
//!
//! Cette classe (@c TBaseTree) est la classe virtuelle de base de toutes les
//! structures arborescentes (trees) pouvant être synchronisés.
//!
//! @author D. Robert
//!
//! @warning Etant donné que les descendants peuvent gérer de façon
//! différente la mémoire, cette classe ne fait aucun new et aucun delete. Il
//! revient aux classes dérivées le soin de détruire les objets, notamment
//! dans les méthodes Clear, SupprNodeVirt et dans le destructeur.
//---------------------------------------------------------------------------

class TBaseTree: public TPersistent {
private:

protected:
  AnsiString FText;
  TBaseTree *FParent;
  int FIndex;

  //---------------------------------------------------------------------------
  //! @brief Met à zéro une structure arborescente
  //!
  //! Cette fonction vide entièrement une structure arborescente.
  //!
  //! @return @c bool true si Ok.
  //!
  //! @note Cette fonction n'est pas virtuelle pure, parce que par défaut elle
  //! fait un SupprNode de tous les noeuds. Ca marche, mais ce n'est pas très
  //! efficace. Il est donc vivement conseillé d'implémenter cette fonction si
  //! possible. La classe dérivée est responsable de la destruction physique de
	//! l'objet par un delete, à moins que ce soit géré par un autre mécanisme.
  //---------------------------------------------------------------------------
  virtual bool ClearVirt();

  //---------------------------------------------------------------------------
  //! @brief Création d'un nouvel objet TBaseTree.
  //!
  //! Cette fonction crée un nouvel objet de type TBaseTree, mais en fait un
  //! descendant, puisque cette classe est abstraite.
  //!
  //! @return @c TBaseTree * Pointeur sur le nouvel onjet créé.
  //!
  //! @note Cette méthode est virtuelle pure, elle doit être implémentée par
  //! les classes dérivées.
  //---------------------------------------------------------------------------
  virtual TBaseTree *CreateNodeVirt(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Insère un noeud dans une structure arborescente
  //!
  //! Cette fonction insère un noeud dans le tree à l'index Index.
  //!
  //! @param[in]        Index Index d'insertion (Premier = zéro, -1 = à la fin)
  //! @param[in]        NewNode Noeud à insérer
  //!
  //! @return @c TBaseTree * adresse du nouveau noeud si Ok, NULL sinon.
  //!
  //! @note Cette méthode est virtuelle pure, elle doit être implémentée par
  //! les classes dérivées.
  //---------------------------------------------------------------------------
  virtual TBaseTree * InsertNodeVirt(int Index, TBaseTree &NewNode) = 0;

  //---------------------------------------------------------------------------
  //! @brief Supprime un noeud dans une structure arborescente
  //!
  //! Cette fonction supprime le noeud d'index Index du tree.
  //!
  //! @param[in]        Index Index du noeud à supprimer (Premier = zéro)
  //!
  //! @return @c bool true si Ok.
  //!
  //! @note Cette méthode est virtuelle pure, elle doit être implémentée par
  //! les classes dérivées. Ces classes doivent également détruire physiquement
	//! l'objet par un delete, à moins que la destruction ne soit gérée par un
	//! autre moyen (cas des TVCLTreeView qui ont un Owner responsable de leur
	//! destruction).
  //---------------------------------------------------------------------------
  virtual bool SupprNodeVirt(int Index) = 0;

  //---------------------------------------------------------------------------
  //! @brief Modification du texte d'un noeud
  //!
  //! Cette fonction change le texte d'un noeud.
  //!
  //! @param[in]        asNewText Nouveau texte du noeud
  //!
  //! @return @c bool true si Ok.
  //!
  //! @note Cette méthode est virtuelle. Par défaut elle ne fait rien. Elle peut
  //! être surchargée par les classes dérivées si besoin.
  //---------------------------------------------------------------------------
  virtual bool SetTextVirt(AnsiString asNewText) {return true;}

  //---------------------------------------------------------------------------
  //! @brief Renvoie le nombre de noeud enfants de la structure arborescente.
  //!
  //! Cette fonction retourne le nombre de noeuds enfants directs du tree.
  //!
  //! @return @c int Nombre de noeuds. -1 si erreur (je ne vois pas bien
  //! quelles erreurs il pourrait y avoir).
  //!
  //! @note Cette méthode est virtuelle pure, elle doit être implémentée par
  //! les classes dérivées.
  //---------------------------------------------------------------------------
  virtual int GetCountVirt(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Lecture du noeud d'index Index
  //!
  //! Cette fonction retourne l'adresse du noeud d'index Index
  //!
  //! @param[in]        Index Index du noeud à lire (Premier = zéro)
  //!
  //! @return @c TBaseTree * Pointeur sur le noeud
  //!
  //! @note Cette méthode est virtuelle pure, elle doit être implémentée par
  //! les classes dérivées.
  //---------------------------------------------------------------------------
  virtual TBaseTree * GetNodeVirt(int Index) = 0;

public:


  //! @name Constructeurs / destructeurs
  //@{


  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TBaseTree(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TBaseTree(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Copie d'une structure arborescente dans une autre.
  //!
  //! Cette fonction copie une structure arborescente à partir d'une autre.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool Assign(TBaseTree &Source);

  //---------------------------------------------------------------------------
  //! @brief Met à zéro une structure arborescente
  //!
  //! Cette fonction vide entièrement une structure arborescente.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool Clear();

  //---------------------------------------------------------------------------
  //! @brief Création d'un nouvel objet TBaseTree.
  //!
  //! Cette fonction crée un nouvel objet de type TBaseTree, mais en fait un
  //! descendant, puisque cette classe appelle la méthode virtuelle pure
	//! CreateNodeVirt.
  //!
  //! @return @c TBaseTree * Pointeur sur le nouvel onjet créé.
  //---------------------------------------------------------------------------
  TBaseTree *CreateNode(void);

  //---------------------------------------------------------------------------
  //! @brief Insère un noeud dans une structure arborescente
  //!
  //! Cette fonction insère un noeud dans le tree à l'index Index.
  //!
  //! @param[in]        Index Index d'insertion (Premier = zéro, -1 = à la fin)
  //! @param[in]        NewNode Noeud à insérer
  //!
  //! @return @c TBaseTree * adresse du nouveau noeud si Ok, NULL sinon. Dans ce
  //! dernier cas, la fonction appelante est responsable de la destruction du
  //! noeud NewNode.
  //---------------------------------------------------------------------------
  TBaseTree * InsertNode(int Index, TBaseTree &NewNode);

  //---------------------------------------------------------------------------
  //! @brief Insère un noeud dans une structure arborescente
  //!
  //! Cette fonction insère un noeud dans le tree à l'index Index.
  //!
  //! @param[in]        Index Index d'insertion (Premier = zéro, -1 = à la fin)
  //! @param[in]        asText Texte du noeud
  //!
  //! @return @c TBaseTree * adresse du nouveau noeud si Ok, NULL sinon.
  //---------------------------------------------------------------------------
  TBaseTree * InsertNode(int Index, AnsiString asText);

  //---------------------------------------------------------------------------
  //! @brief Ajoute un noeud à la fin des autres noeuds
  //!
  //! Cette fonction ajoute un noeud en fin de liste
  //!
  //! @param[in]        NewNode Noeud à insérer
  //!
  //! @return @c TBaseTree * adresse du nouveau noeud si Ok, NULL sinon. Dans ce
  //! dernier cas, la fonction appelante est responsable de la destruction du
  //! noeud NewNode.
  //!
  //! @note Equivalent à faire InsertNode(-1, NewNode);
  //---------------------------------------------------------------------------
  TBaseTree * AddNode(TBaseTree &NewNode);

  //---------------------------------------------------------------------------
  //! @brief Ajoute un noeud à la fin des autres noeuds
  //!
  //! Cette fonction ajoute un noeud en fin de liste
  //!
  //! @param[in]        asText Texte du noeud
  //!
  //! @return @c TBaseTree * adresse du nouveau noeud si Ok, NULL sinon.
  //---------------------------------------------------------------------------
  TBaseTree * AddNode(AnsiString asText);

  //---------------------------------------------------------------------------
  //! @brief Supprime un noeud dans une structure arborescente
  //!
  //! Cette fonction supprime le noeud d'index Index du tree.
  //!
  //! @param[in]        Index Index du noeud à supprimer (Premier = zéro)
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool SupprNode(int Index);

  //---------------------------------------------------------------------------
  //! @brief Recherche du tree-view racine d'un noeud
  //!
  //! Cette fonction retourne le noeud racine dont descent this.
  //!
  //! @return @c TBaseTree * Pointeur sur le noeud racine
  //---------------------------------------------------------------------------
  TBaseTree * RootNode(void);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Count
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TBaseTree, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété Node
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TBaseTree, TBaseTree *, Node);

  //---------------------------------------------------------------------------
  //! @brief Propriété Text
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TBaseTree, AnsiString, Text);

  //---------------------------------------------------------------------------
  //! @brief Propriété Parent
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TBaseTree, TBaseTree *, Parent);

  //---------------------------------------------------------------------------
  //! @brief Propriété Index (index du noeud, -1 si pas de parent)
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TBaseTree, int, Index);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété OnClear
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnClear, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Propriété OnInsertNode
  //---------------------------------------------------------------------------

  DECLARE_EVENT_3(TOnInsertNode, OnInsertNode, TObject *, int, TBaseTree *);

  //---------------------------------------------------------------------------
  //! @brief Propriété OnSupprNode
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnSupprNode, OnSupprNode, TObject *, int);

  //---------------------------------------------------------------------------
  //! @brief Propriété OnChangeText
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnChangeText, OnChangeText, TObject *, AnsiString);

  //@}
};


//---------------------------------------------------------------------------
#endif
