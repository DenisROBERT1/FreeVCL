//-----------------------------------------------------------------------------
//! @file TTooltip.h
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

#ifdef In_TTooltipH
#error "Inclusion circulaire de TTooltip"
#endif // In_TTooltipH

#define In_TTooltipH

#ifndef TTooltipH
#define TTooltipH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//! @class TTooltip
//!
//! @brief Définition de la classe TTooltip
//!
//! Cette classe permet d'afficher les infos-bulles.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TTooltip: public TComponent {
private:
	UINT_PTR idTimer;
	int TimeNoEvents;
	int NbUse;
	bool Visible;
	HWND Handle;
	HWND HandleParent;
	POINT PtTooltip;
	static TTooltip *This;

	static LRESULT FAR PASCAL WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
	void FASTCALL OnPaint(HDC hdc, RECT &Rect);
	void FASTCALL TimerOnTimer(void);
	TRect FASTCALL GetTooltipRect(void);
protected:
	AnsiString FText;
public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

	TTooltip(void);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

	~TTooltip(void);

  //@}


  //! @name Méthodes
  //@{

	void FASTCALL Hide(void);
	void FASTCALL Event(HWND AHandleParent);

	int FASTCALL IncUse(void);
	int FASTCALL DecUse(void);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété Text: texte affiché
	//!
	//! Cette propriété contient le texte de l'info-bulle.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TTooltip, AnsiString, Text);

  //@}

  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnHint
	//!
	//! Cet évènement est déclenché lorsque l'info-bulle est sur le point
	//! d'apparaitre.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TOnHint, OnHint, TObject *);

  //@}

};


#else  // TTooltipH

class TTooltip;

#endif  // TTooltipH


#undef In_TTooltipH
