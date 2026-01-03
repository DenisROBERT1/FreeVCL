//-----------------------------------------------------------------------------
//! @file TMemo.h
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

#ifdef In_TMemoH
#error "Inclusion circulaire de TMemo"
#endif // In_TMemoH

#define In_TMemoH

#ifndef TMemoH
#define TMemoH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#include "TMemoLines.h"


//-----------------------------------------------------------------------------
//! @class TMemo
//!
//! @brief Définition de la classe TMemo
//! @image html TMemo.jpg "TMemo"
//!
//! Cette classe définit le contrôle permettant à l'utilisateur de saisir du
//! texte sur plusieurs lignes.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TMemo: public TWinControl {
private:
	TMemoLines *FLines;
protected:

	//---------------------------------------------------------------------------
	//! @brief Traitement des messages Windows
	//---------------------------------------------------------------------------

	virtual bool ProcessCommand(WORD Notify);

public:

	//! @name Constructeurs et destructeur
  //@{

	//---------------------------------------------------------------------------
	//! @brief Constructeur
	//---------------------------------------------------------------------------

	TMemo(TComponent* AOwner);

	//---------------------------------------------------------------------------
	//! @brief Destructeur
	//---------------------------------------------------------------------------

	virtual ~TMemo(void);

  //@}


	//! @name Méthodes
	//@{

	//---------------------------------------------------------------------------
	//! @brief Destruction du contrôle Windows associé au composant
	//!
	//! Cette méthode détruit la fenêtre Windows associée au composant, ceci
	//! pouvant être nécessaire au changement de certaines propriétés. Appeler
	//! ensuite CreateWnd pour recréer une fenêtre avec les mêmes propriétés.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
	//---------------------------------------------------------------------------
	void FASTCALL DestroyWnd(void);

	//---------------------------------------------------------------------------
	//! @brief Création du contrôle Windows associé au composant
	//!
	//! Cette méthode crée la fenêtre Windows associée au composant, ceci pouvant
	//! être nécessaire suite au changement de certaines propriétés. Appeler
	//! DestroyWnd, puis CreateWnd pour recréer la fenêtre. Les propriétés sont
	//! conservées entre la destruction et la création.
	//!
	//! @note Sous Linux, cette méthode ne fait rien, les composants n'ayant pas
	//! besoin d'être recréés pour répercuter un changement de style.
	//---------------------------------------------------------------------------
	void FASTCALL CreateWnd(void);

	//---------------------------------------------------------------------------
	//! @brief Mise à zéro
	//!
	//! Cette méthode permet d'effacer complètement le TMemo
	//---------------------------------------------------------------------------

	virtual void Clear(void);

	//---------------------------------------------------------------------------
	//! @brief Setter de la propriété WantReturns
	//!
	//! Cette méthode permet d'écrire la propriété WantReturns
	//!
	//! @param[in]				NewWantReturns Propriété à enregistrer
	//!
	//! @return @c bool true si Ok
	//---------------------------------------------------------------------------

	virtual bool Set_WantReturns(bool NewWantReturns);

	//---------------------------------------------------------------------------
	//! @brief Coupe vers le presse-papiers
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	bool FASTCALL CutToClipboard(void);

	//---------------------------------------------------------------------------
	//! @brief Copie dans le presse-papiers
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	bool FASTCALL CopyToClipboard(void);

	//---------------------------------------------------------------------------
	//! @brief Copie depuis le presse-papiers
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	bool FASTCALL PasteFromClipboard(void);

	//---------------------------------------------------------------------------
	//! @brief Liste des propriétés publiées
	//!
	//! @param[in, out] 	ListProperties Liste des propriétés de l'objet
	//!
	//! @sa TPersistent::GetListProperties
	//---------------------------------------------------------------------------
	virtual void GetListProperties(TStrings *ListProperties);

	//---------------------------------------------------------------------------
	//! @brief Type des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété
	//! @param[out] 			asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
	//!
	//! @return @c TYPEPROPERTY Type de la propriété concernée.
	//!
	//! @sa TPersistent::GetTypeProperty
	//---------------------------------------------------------------------------

	virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

	//---------------------------------------------------------------------------
	//! @brief Valeur par défaut d'une propriété
	//!
	//! @param[in]				asProperty Nom de la propriété
	//!
	//! @return @c AnsiString Valeur par défaut de la propriété.
	//!
	//! @sa TPersistent::GetDefaultProperty
	//---------------------------------------------------------------------------

	virtual AnsiString GetDefaultProperty(AnsiString asProperty);

	//---------------------------------------------------------------------------
	//! @brief Lecture des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété à lire
	//!
	//! @return @c AnsiString Valeur de la propriété.
	//---------------------------------------------------------------------------

	virtual AnsiString GetProperty(AnsiString asProperty);

	//---------------------------------------------------------------------------
	//! @brief Affectation des propriétés publiées
	//!
	//! @param[in]				asProperty Nom de la propriété à enregistrer
	//! @param[in]				asValue Valeur de la propriété à enregistrer
	//! @param[in]				Sender Boîte de dialogue (utilisé par les évènements)
	//!
	//! @return @c bool True si Ok.
	//---------------------------------------------------------------------------

	virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
													 TPersistent *Sender);

	//---------------------------------------------------------------------------
	//! @brief Sélection du texte
	//---------------------------------------------------------------------------

	void FASTCALL SelectAll(void);

	//@}

	//! @name Propriétés
	//@{

	//---------------------------------------------------------------------------
	//! @brief Propriété Text: Texte du TMemo
	//! TMemo::Text
	//!
	//! Cette Propriété permet de lire ou de modifier le texte contenu dans
	//! l'objet TMemo.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemo, AnsiString, Text);

	//---------------------------------------------------------------------------
	//! @brief Propriété SelStart: Début du texte sélectionné
	//!
	//! Cette propriété permet de lire ou de positionner le début de la zone
	//! sélectionnée. Le premier caractère commence à l'indice 0. L'affectation
	//! de cette propriété met automatiquement à zéro la longueur de la zone
	//! sélectionnée @b SelLength. Affectez donc cette propriété après
	//! @b SelStart si vous désirez sélectionner une partie de texte. Sinon,
	//! l'affectation de @b SelStart aura uniquement pour effet de déplacer le
	//! curseur.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemo, int, SelStart);

	//---------------------------------------------------------------------------
	//! @brief Propriété SelLength: Longueur du texte sélectionné
	//!
	//! Cette propriété permet de lire ou de positionner la longueur de la zone
	//! sélectionnée à partir de SelStart. Cette longueur peut être positive (en
	//! avant) ou négative (en arrière). Le curseur est situé à la position
	//! SelStart + SelLength. Si la longueur est supérieure au nombre de
	//! caractères disponibles après SelStart (ou avant dans le cas d'une longueur
	//! négative), la longueur sera automatiquement réduite à ce nombre, et une
	//! lecture ultérieure de la propriété renverra ce nombre.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemo, int, SelLength);

	//---------------------------------------------------------------------------
	//! @brief Propriété ReadOnly: Lecture seule
	//!
	//! Cette propriété permet d'interdire la modification d'un TMemo. En
	//! général, le système met automatiquement un fond grisé aux zones d'édition
	//! non modifiables.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemo, bool, ReadOnly);

	//---------------------------------------------------------------------------
	//! @brief Propriété WordWrap: Coupure des lignes
	//!
	//! Cette propriété permet de passer automatiquement à la ligne suivante
	//! lorsque la longueur de la ligne dépasse la largeur du contrôle. Sinon, la
	//! partie qui dépasse est cachée et l'utilisateur devra utiliser la barre de
	//! défilement (si présente, voir TMemo::ScrollBars) pour la visualiser. Le
	//! passage à la ligne est uniquement visuel, un GetText ne fera pas
	//! apparaître le caractère "retour charriot" au milieu de la ligne coupée,
	//! le nombre et le contenu des lignes de la propriété @b Lines n'est pas
	//! modifié.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemo, bool, WordWrap);

	//---------------------------------------------------------------------------
	//! @brief Propriété Lines: Accès aux lignes du TMemo.
	//!
	//! Cette propriété permet d'ajouter, de modifier et de supprimer des lignes.
	//! Si vous souhaitez modifier un grand nombre de lignes, il est généralement
	//! plus rapide d'utiliser la propriété @b Text. Vous pouvez également
	//! utiliser les méthodes TStrings::BeginUpdate et TStrings::EndUpdate de
	//! l'objet TStrings pour éviter le réaffichage entre plusieurs modifications
	//! successives.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemo, TStrings *, Lines);

	//---------------------------------------------------------------------------
	//! @brief Propriété ScrollBars: barres de défilement
	//!
	//! Cette propriété permet d'afficher ou de cacher les barres de défilement.
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY(TMemo, TScrollStyle, ScrollBars);


	//@}


	//! @name Evènements
	//@{

	//---------------------------------------------------------------------------
	//! @brief Evènement OnChange: changement du contenu.
	//!
	//! Cet évènement est déclenché lorsque le text du TMemo est modifié, soit
	//! par une action de l'utilisateur, soit par programme.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
	//---------------------------------------------------------------------------

	DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

	//@}


};


#else  // TMemoH

class TMemo;

#endif	// TMemoH


#undef In_TMemoH
