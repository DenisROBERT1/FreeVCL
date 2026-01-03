//-----------------------------------------------------------------------------
//! @file TObjetPdf.h
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

#ifdef In_TObjetPdfH
#error "Inclusion circulaire de TObjetPdf"
#endif // In_TObjetPdfH

#define In_TObjetPdfH

#ifndef TObjetPdfH
#define TObjetPdfH


//---------------------------------------------------------------------------
//! @class TObjetPdf
//!
//! @brief Objet de base d'un élément PDF
//!
//! Cette classe (@c TObjetPdf) est la classe de base des objets PDF. C'est
//! une classe abstraite, ne pas instancier cette classe mais uniquement ses
//! descendants.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TObjetPdf {
private:
	bool IsDelimiter(const char c, const char *szDelimiters) const;

protected:
  int FNumObj;
	void * FOwner;

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TObjetPdf(void *Owner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TObjetPdf(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Lecture de la description de l'objet.
  //!
  //! Cette méthode permet de lire la description de l'objet, telle qu'elle
	//! doit être enregistrée dans le fichier.
  //!
  //! @return @c CharString Description de l'objet.
  //---------------------------------------------------------------------------

  virtual CharString GetDescription(void) = 0;

  //---------------------------------------------------------------------------
  //! @brief Enregistrement de la description de l'objet
  //!
  //! Cette fonction permet de mémoriser la description de l'objet depuis un
	//! fichier.
  //!
  //! @param[in]        csDescription Description de l'objet.
  //!
  //! @return @c bool True si Ok
  //---------------------------------------------------------------------------

  virtual bool SetDescription(CharString csDescription) = 0;

  //---------------------------------------------------------------------------
  //! @brief Extraction du premier mot d'une chaîne.
  //!
  //! Cette méthode permet d'extraire le premier mot de la chaîne szString à
	//! partir de la position Pos. En sortie, Pos est positionné sur le début du
	//! mot suivant.
  //!
  //! @param[in]        szString Chaîne à découper.
  //! @param[in]        szDelimiters Liste des délimiteurs.
  //! @param[in, out]   Pos Début du découpage.
  //!
  //! @return @c CharString Mot ou chaîne vide si fin de chaîne atteinte.
  //---------------------------------------------------------------------------

	CharString ExtractFirstWord(const char *szString, const char *szDelimiters, int *Pos) const;

  //---------------------------------------------------------------------------
  //! @brief Extraction du dernier mot d'une chaîne.
  //!
  //! Cette méthode permet d'extraire le dernier mot de la chaîne szString à
	//! partir de la position Pos. En sortie, Pos est positionné sur le début du
	//! dernier mot.
  //!
  //! @param[in]        szString Chaîne à découper.
  //! @param[in]        szDelimiters Liste des délimiteurs.
  //! @param[in, out]   Pos Fin du découpage.
  //!
  //! @return @c CharString Mot ou chaîne vide si début de chaîne atteinte.
  //---------------------------------------------------------------------------

	CharString ExtractLastWord(const char *szString, const char *szDelimiters, int *Pos) const;

  //---------------------------------------------------------------------------
  //! @brief Extraction d'une valeur.
  //!
  //! Cette méthode permet d'extraire la valeur de nom csName (par exemple
	//! "/Type").
  //!
  //! @param[in]        szDescription description de l'objet.
  //! @param[in]        csName Nom de la valeur à extraire.
  //!
  //! @return @c CharString Valeur, ou chaîne vide si non trouvée.
  //---------------------------------------------------------------------------

	CharString ExtractValue(const char *szDescription, const CharString csName) const;

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété NumObj
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TObjetPdf, int, NumObj);

  //---------------------------------------------------------------------------
  //! @brief Propriété Owner: propriétaire de l'objet
	//!
	//! Cette propriété renvoie le propriétaire de l'objet @b this (objet chargé
	//! de sa destruction).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TObjetPdf, void *, Owner);


  //@}

};


#endif

#undef In_TObjetPdfH
