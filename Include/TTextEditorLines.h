//-----------------------------------------------------------------------------
//! @file TTextEditorLines.h
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

#ifdef In_TTextEditorLinesH
#error "Inclusion circulaire de TTextEditorLines"
#endif // In_TTextEditorLinesH

#define In_TTextEditorLinesH

#ifndef TTextEditorLinesH
#define TTextEditorLinesH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TStringsH
#include "TStrings.h"
#endif // TStringsH


//---------------------------------------------------------------------------
// Lignes de l'éditeur
//---------------------------------------------------------------------------

typedef struct {
  AnsiString asLine;
  bool FFlagCommentBegin;			//!< true si le début de la ligne est en commentaire (commentaire sur plusieurs lignes)
  bool FFlagCommentEnd;				//!< true si la fin de la ligne est en commentaire
  bool FFlagCommentBegin2;		//!< true si le début de la ligne est en commentaire (commentaire sur plusieurs lignes)
  bool FFlagCommentEnd2;			//!< true si la fin de la ligne est en commentaire
  bool FFlagQuote1Begin;			//!< true si le début de la ligne est entre guillemets (chaine sur plusieurs lignes avec anti-slash en fin de ligne)
  bool FFlagQuote1End;				//!< true si la fin de la ligne est entre guillemets
  bool FFlagQuote2Begin;			//!< true si le début de la ligne est entre guillemets (chaine sur plusieurs lignes avec anti-slash en fin de ligne)
  bool FFlagQuote2End;				//!< true si la fin de la ligne est entre guillemets
  TEditorLinesFlags FFlags;		//!< Flags de la bande d'icônes
} TEXTEDITORLINE, *LPTEXTEDITORLINE;

//-----------------------------------------------------------------------------
//! @class TTextEditorLines
//!
//! @brief Définition de la classe TTextEditorLines
//!
//! Cette classe permet de manipuler directement les lignes d'un objet
//! TTextEditor.
//!
//! @warning Les modifications effectuées sur cet objet sont rapides, mais
//! elles ne sont pas enregistrées dans le fichier undo/redo. Elles doivent
//! donc se limiter aux actions qui n'ont pas à être défaites (chargement d'un
//! fichier, et c'est à peu près tout).
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTextEditorLines: public TStrings {
protected:
  std::vector<TEXTEDITORLINE> FItems;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TTextEditorLines(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TTextEditorLines(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Getter de la propriété Strings
  //!
  //! Cette méthode permet de lire la propriété Strings
  //!
  //! @param[in]        i Index de la propriété à lire
  //!
  //! @return @c AnsiString Propriété Strings
  //---------------------------------------------------------------------------

  virtual AnsiString Get_Strings(int i);

  //---------------------------------------------------------------------------
  //! @brief Setter de la propriété Strings
  //!
  //! Cette méthode permet d'écrire la propriété Strings
  //!
  //! @param[in]        i Index de la propriété à enregistrer
  //! @param[in]        NewStrings Propriété à enregistrer
  //!
  //! @return @c bool true si Ok
  //---------------------------------------------------------------------------

  virtual bool Set_Strings(int i, AnsiString NewStrings);


  //---------------------------------------------------------------------------
  //! @brief Lecture du nombre de chaînes
  //!
  //! Cette méthode permet de lire le nombre d'éléments chaîne dans la liste.
  //!
  //! @return @c int Nombre de chaînes
  //---------------------------------------------------------------------------

  virtual int FASTCALL GetCount(void) const;


  //---------------------------------------------------------------------------
  virtual void FASTCALL SetUpdateState(bool Updating);

  virtual void FASTCALL Clear(void);
  virtual int FASTCALL Add(const AnsiString S);
  virtual void FASTCALL Insert(int Index, const AnsiString S);
  virtual void FASTCALL Delete(int Index);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Flags
	//!
	//! Cette propriété définit les options s'appliquant à une ligne seule.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, TEditorLinesFlags, Flags);

  //---------------------------------------------------------------------------
  //! @brief Propriété FlagCommentBegin: début de ligne en commentaire
	//!
	//! Cette propriété indique si le début de ligne est dans un commentaire
	//! multi-lignes.
	//!
	//! @sa FlagCommentEnd
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagCommentBegin);


  //---------------------------------------------------------------------------
  //! @brief Propriété FlagCommentEnd: fin de ligne en commentaire
	//!
	//! Cette propriété indique si le début de ligne est dans un commentaire
	//! multi-lignes.
	//!
	//! @sa FlagCommentBegin
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagCommentEnd);

  //---------------------------------------------------------------------------
  //! @brief Propriété FlagCommentBegin2: début de ligne en commentaire de type 2
	//!
	//! Cette propriété indique si le début de ligne est dans un commentaire
	//! multi-lignes de type 2.
	//!
	//! @sa FlagCommentEnd2
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagCommentBegin2);


  //---------------------------------------------------------------------------
  //! @brief Propriété FlagCommentEnd2: fin de ligne en commentaire de type 2
	//!
	//! Cette propriété indique si le début de ligne est dans un commentaire
	//! multi-lignes de type 2.
	//!
	//! @sa FlagCommentBegin2
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagCommentEnd2);

  //---------------------------------------------------------------------------
  //! @brief Propriété FlagQuote1Begin: début de ligne dans une chaîne 
	//!
	//! Cette propriété indique si le début de ligne est dans une chaîne
	//! littérale de type 1.
	//!
	//! @sa FlagQuote1End
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagQuote1Begin);


  //---------------------------------------------------------------------------
  //! @brief Propriété FlagQuote1End: fin de ligne dans une chaîne 
	//!
	//! Cette propriété indique si la fin de ligne est dans une chaîne
	//! littérale de type 1.
	//!
	//! @sa FlagQuote1Begin
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagQuote1End);

  //---------------------------------------------------------------------------
  //! @brief Propriété FlagQuote2Begin: début de ligne dans une chaîne de type 2
	//!
	//! Cette propriété indique si le début de ligne est dans une chaîne
	//! littérale de type 2.
	//!
	//! @sa FlagQuote2End
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagQuote2Begin);


  //---------------------------------------------------------------------------
  //! @brief Propriété FlagQuote2End: fin de ligne dans une chaîne de type 2
	//!
	//! Cette propriété indique si la fin de ligne est dans une chaîne
	//! littérale de type 2.
	//!
	//! @sa FlagQuote2Begin
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB(TTextEditorLines, bool, FlagQuote2End);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsqu'une ligne de l'éditeur est modifiée,
	//! soit par une action de l'utilisateur, soit par programme.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //@}


};


#else  // TTextEditorLinesH

class TTextEditorLines;

#endif  // TTextEditorLinesH


#undef In_TTextEditorLinesH
