//-----------------------------------------------------------------------------
//! @file TControlSizer_Collection.h
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


#ifndef TControlSizer_CollectionH
#define TControlSizer_CollectionH


//---------------------------------------------------------------------------
// Fichiers inclus
//---------------------------------------------------------------------------

#include "TControlSizer.h"


//---------------------------------------------------------------------------
// Structures, macros, enums
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//! @class TControlSizer_Collection
//!
//! @brief Liste de composants de dimensionnement de composants
//!
//! Cette classe (@c TControlSizer_Collection) permet de mémoriser une liste
//! d'objets TControlSizer. Elle encapsule un vector de TControlSizer.
//---------------------------------------------------------------------------

class TControlSizer_Collection: public TObject {
private:
protected:
  ACTION FAction;
  std::vector<TControlSizer *> ControlSizers;
	TComponent *Owner;

public:

  //! @name Constructeur / destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------
  TControlSizer_Collection(TComponent *AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------
  ~TControlSizer_Collection(void);

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Ajout d'un objet TControlSizer
  //!
  //! Cette fonction permet d'ajouter un objet TControlSizer de la collection.
  //! Le contenant ne devient pas propriétaire de l'objet TControlSizer, c'est
  //! à l'utilisateur de le détruire après l'avoir enlevé de la collection.
  //---------------------------------------------------------------------------
  bool Add(TControlSizer *ControlSizer);

  //---------------------------------------------------------------------------
  //! @brief Enlèvement d'un objet TControlSizer
  //!
  //! Cette fonction permet de retirer un objet TControlSizer de la collection.
  //! L'objet TControlSizer n'est pas détruit.
  //!
  //! @param[in]        ControlSizer Objet à enlever
  //---------------------------------------------------------------------------
  bool Remove(TControlSizer *ControlSizer);

  //---------------------------------------------------------------------------
  //! @brief Elément à la position (X, Y)
  //!
  //! Cette méthode renvoie l'élément à la position (X, Y). Si plusieurs
	//! éléments sont superposés, l'élément le plus petit (le plus proche du
	//! point en principe) est renvoyé.
  //!
  //! @param[in]        X Coordonnée X du point à tester
  //! @param[in]        Y Coordonnée Y du point à tester
  //!
  //! @return @c int Index de l'élément à la position (X, Y)
  //---------------------------------------------------------------------------
	int GetItemAt(int X, int Y);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées client en coordonnées écran
  //!
  //! Cette méthode transforme les coordonnées client (par rapport au coin
  //! supérieur gauche de la fenêtre) en coordonnées écran.
  //!
  //! @param[in, out]    lpPoint Coordonnées
  //---------------------------------------------------------------------------
  void ClientToScreen(TObject *Sender, LPPOINT lpPoint);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées écran en coordonnées client
  //!
  //! Cette méthode transforme les coordonnées plan (coordonnées réelles) en
  //! coordonnées client en fonction des paramètres de zoom.
  //!
  //! @param[in, out]    lpPoint Coordonnées
  //---------------------------------------------------------------------------
  void PlanToClient(TObject *Sender, LPPOINT lpPoint);

  //---------------------------------------------------------------------------
  //! @brief Transformation des coordonnées écran en coordonnées client
  //!
  //! Cette méthode transforme les coordonnées client en coordonnées plan
  //! (coordonnées réelles) en fonction des paramètres de zoom.
  //!
  //! @param[in, out]    lpPoint Coordonnées
  //---------------------------------------------------------------------------
  void ClientToPlan(TObject *Sender, LPPOINT lpPoint);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Action
 	//!
	//! Cette propriété mémorise l'action à réaliser
	//---------------------------------------------------------------------------

  DECLARE_PROPERTY(TControlSizer_Collection, ACTION, Action);

  //---------------------------------------------------------------------------
  //! @brief Propriété Size
	//!
	//! Cette propriété retourne le nombre d'objets TControleSizer contenus dans
	//! la collection.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TControlSizer_Collection, int, Count);

  //---------------------------------------------------------------------------
  //! @brief Propriété Item
	//!
	//! Cette propriété retourne l'objet @b TControlSizer d'indice @b Index.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TControlSizer_Collection, TControlSizer*, Item);

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Changement de coordonnées client -> écran
	//!
	//! Cette évènement est appelé lorsqu'il est nécessaire de transformer les
	//! coordonnées client (relatives au coin supérieur gauche de la fenêtre
	//! parent) en coordonnées écran (relatives au coin supérieur gauche de
	//! l'écran).@n
	//! L'utilisateur de cette classe doit impérativement traiter cet évènement
	//! pour assurer son bon fonctionnement.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Coordonnées du point à transformer
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TClientToScreen, ClientToScreen_Collection, TObject *, POINT *);

  //---------------------------------------------------------------------------
  //! @brief Changement de coordonnées plan -> client
	//!
	//! Cette évènement est appelé lorsqu'il est nécessaire de transformer les
	//! coordonnées plan (coordonnées réelles) en coordonnées client en fonction
	//! des paramètres de zoom.@n
	//! Si l'utilisateur de cette classe ne traite pas cet évènement, cela
	//! signifie que les coordonnées plan et client sont les mêmes (c'est à dire
	//! qu'il n'y a pas de zoom).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Coordonnées du point à transformer
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TPlanToClient, PlanToClient_Collection, TObject *, POINT *);

  //---------------------------------------------------------------------------
  //! @brief Changement de coordonnées client -> plan
	//!
	//! Cette évènement est appelé lorsqu'il est nécessaire de transformer les
	//! coordonnées client en coordonnées plan (coordonnées réelles) en fonction
	//! des paramètres de zoom.@n
	//! Si l'utilisateur de cette classe ne traite pas cet évènement, cela
	//! signifie que les coordonnées plan et client sont les mêmes (c'est à dire
	//! qu'il n'y a pas de zoom).
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in, out]   Arg2 Coordonnées du point à transformer
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TClientToPlan, ClientToPlan_Collection, TObject *, POINT *);

  //@}

};

//---------------------------------------------------------------------------
#endif
