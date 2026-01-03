//-----------------------------------------------------------------------------
//! @file Property.cpp
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
	{{{Type}}} F{{{Property}}};

	//---------------------------------------------------------------------------
	//! @brief Propriété {{{Property}}}
	//---------------------------------------------------------------------------

	DECLARE_PROPERTY({{{Class}}}, {{{Type}}}, {{{Property}}});

	F{{{Property}}} = 0;

//---------------------------------------------------------------------------
// Accesseurs de la propriété {{{Property}}}
//---------------------------------------------------------------------------

{{{Type}}} {{{Class}}}::Get_{{{Property}}}(void) {
	return F{{{Property}}};
}

bool {{{Class}}}::Set_{{{Property}}}({{{Type}}} New{{{Property}}}) {

	if (F{{{Property}}} != New{{{Property}}}) {
		F{{{Property}}} = New{{{Property}}};
	}

	return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------

	ListProperties->Add(_T("{{{Property}}}"));

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

	if (asProperty == _T("{{{Property}}}")) {
		return tpText;
	}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

	if (asProperty == _T("{{{Property}}}")) {
		return _T("");
	}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

	if (asProperty == _T("{{{Property}}}")) {
		return Get_{{{Property}}}();
	}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

	if (asProperty == _T("{{{Property}}}")) {
		Set_{{{Property}}}(asValeur);
		return true;
	}
  
