//-----------------------------------------------------------------------------
//! @file TStream.cpp
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

#include <FreeVcl.h>



//---------------------------------------------------------------------------
// TStream
//---------------------------------------------------------------------------

TStream::TStream(void): TObject() {
	// Initialisations
	FPosition = 0;
	FSize = 0;
	FLastError = 0;
}

TStream::~TStream(void) {
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Position
//---------------------------------------------------------------------------

LongLong TStream::Get_Position(void) {
	return FPosition;
}

bool TStream::Set_Position(LongLong NewPosition) {
	if (FPosition != NewPosition) {
		FPosition = NewPosition;
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Size
//---------------------------------------------------------------------------

LongLong TStream::Get_Size(void) {
	return FSize;
}

bool TStream::Set_Size(LongLong NewSize) {
	if (FSize != NewSize) {
		FSize = NewSize;
	}
	return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété LastError
//---------------------------------------------------------------------------

int TStream::Get_LastError(void) {
	return FLastError;
}

//---------------------------------------------------------------------------
// Copie directement le nombre spécifié d'octets d'un flux vers un autre flux.
//---------------------------------------------------------------------------

LongLong FASTCALL TStream::CopyFrom(TStream* Source, LongLong Count) {
	return 0;
}

//---------------------------------------------------------------------------
// Lit les octets du flux et les place dans Buffer.
//---------------------------------------------------------------------------

void FASTCALL TStream::ReadBuffer(void *Buffer, LongLong Count) {
}

//---------------------------------------------------------------------------
// Se déplace vers une position spécifiée de la ressource de flux.
//---------------------------------------------------------------------------

int FASTCALL TStream::Seek(int Offset, TSeekOrigin Origin) {
	return 0;
}

LongLong FASTCALL TStream::Seek(const LongLong Offset, TSeekOrigin Origin) {
	return 0;
}

//---------------------------------------------------------------------------
// Ecrit Count octets de Buffer dans le flux et avance la position en cours dans le flux de Count octets.
//---------------------------------------------------------------------------

void FASTCALL TStream::WriteBuffer(const void *Buffer, LongLong Count) {
}
