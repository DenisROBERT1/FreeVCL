//-----------------------------------------------------------------------------
//! @file TNumVersion.h
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

#ifndef TNumVersionH
#define TNumVersionH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------


class TNumVersion {
private:
protected:
	int Num[4];
public:


  //! @name Constructeurs / destructeurs
  //@{


  //---------------------------------------------------------------------------
  //! @brief Constructeurs
  //---------------------------------------------------------------------------

  TNumVersion(void);

	TNumVersion(const TCHAR *szNumVersion);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  ~TNumVersion(void);

  //@}


  //! @name Méthodes
  //@{

	TNumVersion & operator = (const TCHAR *szNumVersion);
	bool operator == (TNumVersion &src) const;
	bool operator != (TNumVersion &src) const;
	bool operator < (TNumVersion &src) const;
	bool operator > (TNumVersion &src) const;
	bool operator <= (TNumVersion &src) const;
	bool operator >= (TNumVersion &src) const;

  //@}


  //! @name Propriétés
  //@{

  //@}
};

//---------------------------------------------------------------------------
#endif
