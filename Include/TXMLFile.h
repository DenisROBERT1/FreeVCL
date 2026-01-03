//-----------------------------------------------------------------------------
//! @file TXMLFile.h
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

#ifndef TXMLFileH
#define TXMLFileH

//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

enum TYPEBALISE {
	tb_OpenTag,		  //!< 0 = Pas de slash (balise ouvrante) <balise>
	tb_CloseTag,		//!< 1 = Slash en début (balise fermante) </balise>
	tb_SingleTag,		//!< 2 = Slash en fin (balise isolée) <balise/>
	tb_Comment,			//!< 3 = commentaire <!-- balise -->
	tb_Header				//!< 4 = en-tête <?xml version='1.0' encoding='utf-8' ?>
};

enum TYPENODE {
	tn_Tag,					//!< 0 = Balise normale
	tn_Comment,			//!< 1 = commentaire
	tn_Header				//!< 2 = en-tête
};

//---------------------------------------------------------------------------
//! @class TXMLFile
//!
//! @brief Fichier XML
//!
//! Cette classe (@c TXMLFile) Permet de lire et d'enregistrer un fichier XML
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TXMLFile: public TComponent {
private:

protected:
  std::vector<TXMLFile *> FNodes;
  TStringList FAttributesNames;
  TStringList FAttributesValues;
  AnsiString FName;
  AnsiString FText;
  TYPENODE FType;
  bool FNoComment;
  TXMLFile *FParent;
  int FIndex;

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un noeud du fichier XML.
  //!
  //! Cette fonction enregistre un noeud du fichier XML et ses sous-noeuds.
  //!
  //! @param[in]        hFile Handle du fichier destination
  //! @param[in]        Indent Indentation du XML
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool EnrNode(HANDLE hFile, int Indent);

	bool FileLineInput(HANDLE hFile, AnsiString *asBuffer);

public:


	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TXMLFile(TComponent *AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  ~TXMLFile(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Met à zéro la structure arborescente
  //!
  //! Cette fonction vide entièrement la structure arborescente.
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool Clear();

  //---------------------------------------------------------------------------
  //! @brief Insère un noeud dans une structure arborescente
  //!
  //! Cette fonction insère un noeud dans le tree à l'index Index.
  //!
  //! @param[in]        Index Index d'insertion (Premier = zéro, -1 = à la fin)
  //! @param[in]        NewXMLNode Noeud à insérer
  //!
  //! @return @c TXMLFile * adresse du nouveau noeud si Ok, NULL sinon. Dans ce
  //! dernier cas, la fonction appelante est responsable de la destruction du
  //! noeud NewXMLNode.
  //---------------------------------------------------------------------------
  TXMLFile * InsertNode(int Index, TXMLFile &NewXMLNode);

  //---------------------------------------------------------------------------
  //! @brief Insère un noeud dans une structure arborescente
  //!
  //! Cette fonction insère un noeud dans le tree à l'index Index.
  //!
  //! @param[in]        Index Index d'insertion (Premier = zéro, -1 = à la fin)
  //! @param[in]        asName Nom du noeud (balise)
  //!
  //! @return @c TXMLFile * adresse du nouveau noeud si Ok, NULL sinon.
  //---------------------------------------------------------------------------
  TXMLFile * InsertNode(int Index, AnsiString asName);

  //---------------------------------------------------------------------------
  //! @brief Ajoute un noeud à la fin des autres noeuds
  //!
  //! Cette fonction ajoute un noeud en fin de liste
  //!
  //! @param[in]        NewXMLNode Noeud à insérer
  //!
  //! @return @c TXMLFile * adresse du nouveau noeud si Ok, NULL sinon. Dans ce
  //! dernier cas, la fonction appelante est responsable de la destruction du
  //! noeud NewXMLNode.
  //!
  //! @note Equivalent à faire InsertNode(-1, NewXMLNode);
  //---------------------------------------------------------------------------
  TXMLFile * AddNode(TXMLFile &NewXMLNode);

  //---------------------------------------------------------------------------
  //! @brief Ajoute un noeud à la fin des autres noeuds
  //!
  //! Cette fonction ajoute un noeud en fin de liste
  //!
  //! @param[in]        asName Nom du noeud (balise)
  //!
  //! @return @c TXMLFile * adresse du nouveau noeud si Ok, NULL sinon.
  //---------------------------------------------------------------------------
  TXMLFile * AddNode(AnsiString asName);

  //---------------------------------------------------------------------------
  //! @brief Supprime un noeud dans une structure arborescente
  //!
  //! Cette fonction supprime le noeud d'index Index du tree.
  //!
  //! @param[in]        Index Index du noeud à supprimer (Premier = zéro)
  //!
  //! @return @c bool true si Ok. Sinon, la fonction appelante est responsable
  //! de la destruction du noeud NewXMLNode.
  //---------------------------------------------------------------------------
  bool DeleteNode(int Index);

  //---------------------------------------------------------------------------
  //! @brief Lit un attribut d'un noeud
  //!
  //! Cette fonction lit un attribut d'un noeud
  //!
  //! @param[in]        asAttributeName Nom de l'attribut
  //!
  //! @return @c AnsiString Valeur de l'attribut, vide si attribut vide ou absent.
  //---------------------------------------------------------------------------
  AnsiString GetAttribute(AnsiString asAttributeName);

  //---------------------------------------------------------------------------
  //! @brief Ecrit un attribut dans un noeud
  //!
  //! Cette fonction écrit un attribut dans un noeud. Si un attribut de même
	//! nom existait déjà, il est écrasé.
  //!
  //! @param[in]        asAttributeName Nom de l'attribut
  //! @param[in]        asAttributeValue Valeur de l'attribut à enregistrer
  //!
  //! @return @c bool true si l'attribut a été créé. false s'il existait déjà
	//! et qu'il a été remplacé.
  //---------------------------------------------------------------------------
  bool SetAttribute(AnsiString asAttributeName, AnsiString asAttributeValue);

  //---------------------------------------------------------------------------
  //! @brief Détruit un attribut dans un noeud
  //!
  //! Cette fonction détruit un attribut dans un noeud.
  //!
  //! @param[in]        asAttributeName Nom de l'attribut
  //!
  //! @return @c bool true si l'attribut a été détruit. false s'il n'existait
	//! pas.
  //---------------------------------------------------------------------------
  bool DeleteAttribute(AnsiString asAttributeName);

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un fichier XML
  //!
  //! Cette fonction lit un fichier XML sur disque
  //!
  //! @param[in]        asXMLFile Nom du fichier à lire
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool LoadFromFile(AnsiString asXMLFile);

  //---------------------------------------------------------------------------
  //! @brief Enregistrement d'un fichier XML
  //!
  //! Cette fonction écrit un fichier XML sur disque
  //!
  //! @param[in]        asXMLFile Nom du fichier à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------
  bool SaveToFile(AnsiString asXMLFile);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Name
	//!
	//! Cette propriété permet de lire et de définir le nom du noeud (le texte
	//! entre crochets).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TXMLFile, AnsiString, Name);

  //---------------------------------------------------------------------------
  //! @brief Propriété Text
	//!
	//! Cette propriété permet de lire et de définir le texte du noeud (texte
	//! entre la balise de début et la balise de fin.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TXMLFile, AnsiString, Text);

  //---------------------------------------------------------------------------
  //! @brief Propriété Type
	//!
	//! Cette propriété permet de lire et de définir le type du noeud (si c'est
	//! un commentaire, un noeud normal ou l'entête).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TXMLFile, TYPENODE, Type);

  //---------------------------------------------------------------------------
  //! @brief Propriété NoComment
	//!
	//! Cette propriété permet de lire un fichier XML en ignorant les
	//! commentaires. La positionner à @b true avant d'appeler @b LoadFromFile.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TXMLFile, bool, NoComment);

  //---------------------------------------------------------------------------
  //! @brief Propriété Count
  //!
  //! Cette fonction retourne le nombre de noeuds enfants directs du tree.
  //!
  //! @return @c int Nombre de noeuds.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TXMLFile, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété AttributesCount
  //!
  //! Cette fonction retourne le nombre d'attributs du noeud.
  //!
  //! @return @c int Nombre d'attributs.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TXMLFile, int, AttributesCount);

  //---------------------------------------------------------------------------
  //! @brief Propriété AttributeName
  //!
  //! Cette fonction retourne le nom d'un attribut
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TXMLFile, AnsiString, AttributeName);

  //---------------------------------------------------------------------------
  //! @brief Propriété AttributeValue
  //!
  //! Cette fonction retourne la valeur d'un attribut
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TXMLFile, AnsiString, AttributeValue);

  //---------------------------------------------------------------------------
  //! @brief Propriété Node
  //!
  //! Cette fonction retourne l'adresse du noeud d'index Index
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TXMLFile, TXMLFile *, Node);

  //---------------------------------------------------------------------------
  //! @brief Propriété Parent
	//!
	//! Cette propriété renvoie le noeud parent, ou NULL si aucun parent (noeud
	//! racine).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TXMLFile, TXMLFile *, Parent);

  //---------------------------------------------------------------------------
  //! @brief Propriété Index (index du noeud, -1 si pas de parent)
	//!
	//! Index du noeud. Cette classe ne change pas l'ordre des noeuds dans
	//! l'arborescence. Cependant, la norme XML précise que l'ordre des noeuds
	//! n'est pas nécessairement conservée.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TXMLFile, int, Index);

  //@}

};

//---------------------------------------------------------------------------
#endif
