//-----------------------------------------------------------------------------
//! @file TMemoryStream.cpp
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
// TMemoryStream
//---------------------------------------------------------------------------

TMemoryStream::TMemoryStream(void): TStream() {
	Data = NULL;
	FSize = 0;
	FPosition = 0;
	bAllocated = false;
	bReadOnly = false;
}

TMemoryStream::TMemoryStream(const void *Buffer, LongLong Count) {
	Data = (Byte *) Buffer;
	FSize = Count;
	FPosition = 0;
	bAllocated = false;
	bReadOnly = true;
}

TMemoryStream::TMemoryStream(void *Buffer, LongLong Count) {
	Data = (Byte *) Buffer;
	FSize = Count;
	FPosition = 0;
	bAllocated = false;
	bReadOnly = false;
}

TMemoryStream::~TMemoryStream(void) {
	if (bAllocated) delete[] Data;
}


//---------------------------------------------------------------------------
// Met à zéro le tampon mémoire.
//---------------------------------------------------------------------------

void FASTCALL TMemoryStream::Clear(void) {
	if (bAllocated) delete[] Data;
	Data = NULL;
	FSize = 0;
	FPosition = 0;
	bAllocated = false;
	bReadOnly = false;
}

//---------------------------------------------------------------------------
// Définit une nouvelle taille du tampon mémoire.
//---------------------------------------------------------------------------

void FASTCALL TMemoryStream::SetSize(LongLong NewSize) {
	FSize = NewSize;
	if (bAllocated) delete[] Data;
	Data = new Byte[(int) FSize];
	FPosition = 0;
	bAllocated = true;
	bReadOnly = false;
}

//---------------------------------------------------------------------------
// Charge la totalité du flux dans le tampon mémoire.
//---------------------------------------------------------------------------

bool FASTCALL TMemoryStream::LoadFromStream(TStream* Stream) {

	if (Stream == this) return true;
	if (bReadOnly) return false;

	SetSize(Stream->Size);
	Stream->Position = 0;
	Stream->ReadBuffer(Data, FSize);
	FPosition = 0;

  return true;
}

//---------------------------------------------------------------------------
// Implémentation de la méthode virtuelle de lecture du flux.
//---------------------------------------------------------------------------

LongLong FASTCALL TMemoryStream::Read(void *Buffer, LongLong Count) {
	
	if (Count > FSize - FPosition) Count = FSize - FPosition;
	memcpy(Buffer, &Data[FPosition], (int) Count);
	FPosition += Count;

  return Count;
}

//---------------------------------------------------------------------------
// Implémentation de la méthode virtuelle pour l'écriture dans le flux.
//---------------------------------------------------------------------------

LongLong FASTCALL TMemoryStream::Write(const void *Buffer, LongLong Count) {

	if (bReadOnly) return 0;

	if (Count > FSize - FPosition) {
		if (bAllocated || Data == NULL) {
			Byte *NewData;
			LongLong NewSize;
			NewSize = Position + Count;
			NewData = new Byte[(int) NewSize];
			memcpy(NewData, Data, (size_t) FSize);
			delete[] Data;
			Data = NewData;
			FSize = NewSize;
			bAllocated = true;
		}
		else {
			Count = FSize - FPosition;
		}
	}
	memcpy(&Data[FPosition], Buffer, (int) Count);
	FPosition += Count;

  return Count;
}
