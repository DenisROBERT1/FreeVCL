//-----------------------------------------------------------------------------
//! @file Set.h
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

#ifdef In_SetH
#error "Inclusion circulaire de Set"
#endif // In_SetH

#define In_SetH

#ifndef SetH
#define SetH

//---------------------------------------------------------------------------

template < class T, unsigned char minEl, unsigned char maxEl > class Set {

protected:
  unsigned char Data[((int) maxEl - minEl) / 8 + 1];

public:
  Set() {memset(Data, 0, sizeof(Data));}
  Set(const Set& src) {
		int i;
    for (i = 0; i < (int) sizeof(Data); i++)
        Data[i] = src.Data[i];
  }
  Set(unsigned int src) {
		int i;
    for (i = 0; i < (int) sizeof(Data); i++) {
      Data[i] = src & 0xFF;
      src >>= 8;
		}
  }

  Set& operator = (const Set& rhs) {
    if (this != &rhs) {
			int i;
      for (i = 0; i < (int) sizeof(Data); i++)
          Data[i] = rhs.Data[i];
    }
    return * this;
  }

  Set& operator += (const Set& rhs) {
		int i;
    for (i = 0; i < (int) sizeof(Data); i++)
        Data[i] |= rhs.Data[i];
    return * this;
  }

  Set& operator -= (const Set& rhs) {
		int i;
    for (i = 0; i < (int) sizeof(Data); i++)
        Data[i] ^= (Data[i] & rhs.Data[i]);
    // $$$ Mettre à zéro les bits non significatifs
    return * this;
  }

  Set& operator *= (const Set& rhs) {
		int i;
    for (i = 0; i < (int) sizeof(Data); i++)
        Data[i] &= rhs.Data[i];
    return * this;
  }

  Set operator + (const Set& rhs) const {
    Set < T, minEl, maxEl > s;
		int i;
    for (i = 0; i < (int) sizeof(Data); i++)
        s.Data[i] = Data[i] | rhs.Data[i];
    return s;
  }

  Set operator - (const Set& rhs) const {
    Set < T, minEl, maxEl > s;
		int i;
    for (i = 0; i < (int) sizeof(Data); i++)
        s.Data[i] = Data[i] ^ (Data[i] & rhs.Data[i]);
    // $$$
    return s;
  }

  Set operator * (const Set& rhs) const {
    Set < T, minEl, maxEl > s;
		int i;
    for (i = 0; i < (int) sizeof(Data); i++)
        s.Data[i] = Data[i] & rhs.Data[i];
    return s;
  }

  Set& operator << (const T el) {
    if (el >= minEl && el <= maxEl) {
      Data[(int) (el / 8) - (int) (minEl / 8)] |=
        (unsigned char) ((unsigned short) 1 << (el % 8));
    }
    return * this;
  }

  Set& operator >> (const T el) {
    if (el >= minEl && el <= maxEl) {
      Data[(int) (el / 8) - (int) (minEl / 8)] &=
        (unsigned char)~((unsigned short) 1 << (el % 8));
    }
    return * this;
  }

  bool Contains(const T el) const {
    return (el >= minEl && el <= maxEl)?
        ((Data[(int) (el / 8) - (int) (minEl / 8)] &
        (unsigned char) ((unsigned short) 1 << (el % 8))) != 0): false;
  }

  Set& Clear() {
    memset(Data, 0, sizeof(Data));
    return * this;
  }

  bool Empty() const {
		int i;
    for (i = 0; i < (int) sizeof(Data); i++) {
      if (Data[i] != 0) return false;
    }
    return true;
  }

  bool operator == (const Set& rhs) const {
		int i;
    for (i = 0; i < (int) sizeof(Data); i++) {
      if (Data[i] != rhs.Data[i]) return false;
    }
    return true;
  }

  bool operator != (const Set& rhs) const {return !operator == (rhs);}
};

//---------------------------------------------------------------------------
#endif

#undef In_SetH
