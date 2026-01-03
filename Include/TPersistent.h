//-----------------------------------------------------------------------------
//! @file TPersistent.h
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


#ifdef In_TPersistentH
#error "Inclusion circulaire de TPersistent"
#endif // In_TPersistentH

#define In_TPersistentH

#ifndef TPersistentH
#define TPersistentH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include "TObject.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

class TStringList;


//---------------------------------------------------------------------------
// MACROS
//---------------------------------------------------------------------------

#define FVCL_BEGIN_EVENTS
#define FVCL_END_EVENTS


//-----------------------------------------------------------------------------
//! @class TPersistent
//!
//! @brief Définition de la classe TPersistent
//!
//! Cette classe est la classe de base des objets VCL pouvant être enregistrés
//! dans une ressource de type DFM. Elle contient le nom de l'objet,
//! différentes méthodes facilitant la lecture dans le DFM, la lecture et
//! l'affectation des propriétés et les propriétés permettant la gestion des
//! erreurs. Elle comprend également la gestion des évènements, c'est à dire
//! que tout objet interceptant un évènement doit être un descendant de cette
//! classe. Ne pas implémenter cette classe, mais uniquement ses descendants.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TPersistent: public TObject {
private:
protected:
  AnsiString FName;
	int FLastError;

  //---------------------------------------------------------------------------
  //! @brief Découpage d'une chaîne en mots
  //!
  //! Cette méthode permet de découper une chaîne en mots, selon un délimiteur
  //! paramétrable.
  //!
  //! @param[in]        asLigne Chaîne à découper
  //! @param[in]        asDelim1 Caractère(s) neutres (espaces par exemple)
  //! @param[in]        asDelim2 Délimiteur(s) (virgule et parenthèse, par exemple)
  //! @param[in, out]   Cur In: position de départ du découpage
  //!                       Out: position de départ du découpage suivant
  //!
  //! @return @c AnsiString Mot (chaîne vide si fin atteinte)
  //---------------------------------------------------------------------------

  AnsiString UnMot(AnsiString asLigne, AnsiString asDelim1, AnsiString asDelim2, int *Cur);

  //---------------------------------------------------------------------------
  //! @brief Suppression des guillemets VCL
  //!
  //! Cette méthode permet de supprimer les guillemets VCL et de convertir les
  //! caractères spéciaux \#xxx en caractère normal.
  //!
  //! @param[in]        asTexte Chaîne avec guillemets
  //!
  //! @return @c AnsiString chaîne transformée
  //---------------------------------------------------------------------------

  AnsiString SuppGuillVCL(AnsiString asTexte);

  //---------------------------------------------------------------------------
  //! @brief Formatage d'une propriété
  //!
  //! Cette méthode permet de formater les propriétés. Par exemple, elle permet
  //! de transformer la chaîne "akLeft\nakTop\n" en "[akLeft, akTop]"
  //!
  //! @param[in]        asLigne Mots séparés par le caractère \n (le dernier est optionnel)
  //! @param[in]        asDeb Délimiteur de début ("[" dans l'exemple ci-dessus)
  //! @param[in]        asFin Délimiteur de fin ("]" dans l'exemple)
  //! @param[in]        asDelim Séparateur (", " dans l'exemple)
  //!
  //! @return @c AnsiString chaîne formatée
  //---------------------------------------------------------------------------
  AnsiString FormatProperty(AnsiString asLigne,
        AnsiString asDeb, AnsiString asFin, AnsiString asDelim);

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TPersistent(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TPersistent(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Copie d'un composant sur un autre
  //!
  //! Cette méthode permet d'initialiser le composant à partir d'un autre.
  //! C'est une méthode virtuelle pure, il faut donc l'implémenter dans toutes
	//! les classes qui dérivent de celle-ci.
  //!
  //! @param[in]        Source Composant d'origine.
  //---------------------------------------------------------------------------

	virtual void FASTCALL Assign(TPersistent *Source);

  //---------------------------------------------------------------------------
  //! @brief Message d'erreur correspondant à un numéro
  //!
  //! Cette méthode permet de lire le texte d'une erreur à partir de son
	//! numéro (numéro correspondant à la propriété LastError). Chaque classe
	//! dérivée peut redéfinir cette méthode pour ajouter ses propres messages
	//! d'erreur. Il faut donc veiller à appeler la méthode du même objet que
	//! celui qui a provoqué l'erreur.
  //!
  //! @param[in]        ErrorNumber Numéro d'erreur.
  //!
  //! @return @c AnsiString Texte du message d'erreur.
  //---------------------------------------------------------------------------

	virtual AnsiString FASTCALL GetErrorMessage(int ErrorNumber);

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  /*! @verbatim
      En général, cette méthode se contente d'ajouter les propriétés spécifiques
      de l'objet, puis passe la main à l'objet dont il dérive, par exemple:

      TWinControl::GetListProperties(ListProperties);

      ListProperties->Add(_T("Default"));
      ListProperties->Add(_T("Cancel"));
      ListProperties->Add(_T("ModalResult")); @endverbatim */
  //!
  //! @warning Ne pas mettre à zéro la liste ListProperties avant de la
  //! compléter, cette méthode pouvant avoir été appelée par un objet dérivé.
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
  /*! @verbatim
      En général, cette méthode renseigne le type des propriétés que l'objet
      implémente, et sinon passe la main à l'objet dont il dérive, par exemple:

      if (asProperty == "Default") {
        return tpBool;
      }
      if (asProperty == "Cancel") {
        return tpBool;
      }
      if (asProperty == "ModalResult") {
        *asInfos = "mrNone\nmrOk\nmrCancel\nmrAbort\nmrRetry\nmrIgnore\nmrYes\nmrNo\nmrAll\nmrNoToAll\nmrYesToAll";
        return tpChoice;
      }
      return TWinControl::GetTypeProperty(asProperty, asInfos); @endverbatim */
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  /*! @verbatim
      En général, cette méthode renseigne la valeur par défaut des propriétés
      que l'objet implémente, et sinon passe la main à l'objet dont il dérive,
      par exemple:

      if (asProperty == _T("Default")) {
        return _T("False");
      }
      if (asProperty == _T("Cancel")) {
        return _T("False");
      }
      if (asProperty == _T("ModalResult")) {
        return _T("mrNone");
      }
      return TWinControl::GetDefaultProperty(asProperty);

      Seules les valeurs différentes de la valeur par défaut sont mémorisées
      dans les fichiers DFM. @endverbatim */
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
  //! @brief Propriété Name: Nom de l'objet
	//!
	//! Cette propriété renvoie le nom de l'objet, défini dans le DFM juste après
	//! le mot-clé "object" (mais qui peut aussi être défini ou modifié par
	//! programme).
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TPersistent, AnsiString, Name);

  //---------------------------------------------------------------------------
  //! @brief Propriété LastError: Dernière erreur
	//!
	//! Cette propriété indique la dernière erreur survenue dans un objet. Les
	//! valeurs retournées sont les mêmes que la fonction GetLastError de Windows
	//! pour les erreurs système, mais certaines classes VCL peuvent ajouter des
	//! types d'erreurs spécifiques. Vous pouvez lire le message d'erreur associé
	//! à un numéro en utilisant la méthode @b GetErrorMessage.@n
	//! Les erreurs spécifiques VCL commencent au numéro défini par
	//! @b ERROR_VCL_BEGIN.
	//!
	//! @note Cette propriété n'est pas "thead safe", c'est à dire qu'il ne faut
	//! pas l'utiliser dans un objet lorsque plusieurs processus peuvent y
	//! accéder simultanément. Mais ce cas est relativement rare pour une
	//! utilisation normale de la VCL.
	//!
	//! @sa GetErrorMessage
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TPersistent, int, LastError);

  //@}

};


#else  // TPersistentH

class TPersistent;

#endif  // TPersistentH

#undef In_TPersistentH

