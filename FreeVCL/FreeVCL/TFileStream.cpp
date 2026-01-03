//-----------------------------------------------------------------------------
//! @file TFileStream.cpp
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
// TFileStream
//---------------------------------------------------------------------------

TFileStream::TFileStream(const AnsiString asFileName, Word Mode): TStream() {
  DWORD dwDesiredAccess = 0;
  DWORD dwShareMode = 0;
	DWORD dwCreationDisposition = 0;

	FMode = Mode;
	FSize = 0;
	FPosition = 0;

	if (FMode & fmOpenRead) dwDesiredAccess |= GENERIC_READ;
	if (FMode & fmOpenWrite) dwDesiredAccess |= GENERIC_WRITE;

	if (FMode & fmCreate) dwCreationDisposition |= CREATE_ALWAYS;
	else dwCreationDisposition |= OPEN_EXISTING;

	if ((FMode & fmShareDenyRead) == 0) dwCreationDisposition |= FILE_SHARE_READ;
	if ((FMode & fmShareDenyWrite) == 0) dwCreationDisposition |= FILE_SHARE_WRITE;

  hFile = CreateFile(asFileName, dwDesiredAccess, dwCreationDisposition, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD SizeLow, SizeHigh;
		SizeLow = GetFileSize(hFile, &SizeHigh);
		FSize = ((LongLong) SizeHigh << 32) + SizeLow;
	}
	else {
		FLastError = GetLastError();
	}
}

TFileStream::~TFileStream(void) {
  if (hFile != INVALID_HANDLE_VALUE) {
		if (FMode & fmOpenWrite) {
			LARGE_INTEGER liDistanceToMove;
			liDistanceToMove.QuadPart = FSize;
			SetFilePointerEx(hFile, liDistanceToMove, NULL, FILE_BEGIN);
			SetEndOfFile(hFile);
		}
		CloseHandle(hFile);
	}
}


//---------------------------------------------------------------------------
// Charge la totalité du flux dans le tampon mémoire.
//---------------------------------------------------------------------------

bool FASTCALL TFileStream::LoadFromStream(TStream *Stream) {

	if (Stream == this) return true;

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

  return true;
}

//---------------------------------------------------------------------------
// Implémentation de la méthode virtuelle de lecture du flux.
//---------------------------------------------------------------------------

LongLong FASTCALL TFileStream::Read(void *Buffer, LongLong Count) {
  LongLong ByteRead;
	DWORD dwByteRead;
	DWORD dwCount;

	ByteRead = 0;
	while (Count) {
		if (Count <= UINT_MAX) dwCount = (DWORD) Count;
		else dwCount = UINT_MAX;
		ReadFile(hFile, Buffer, dwCount, &dwByteRead, NULL);
		ByteRead += dwByteRead;
		Count -= dwByteRead;
		if (dwByteRead < dwCount) break;
	}
	FPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

	return ByteRead;
}

//---------------------------------------------------------------------------
// Se déplace vers une position spécifiée de la ressource de flux.
//---------------------------------------------------------------------------

int FASTCALL TFileStream::Seek(int Offset, TSeekOrigin Origin) {
	return (int) Seek((LongLong) Offset, Origin);
}

LongLong FASTCALL TFileStream::Seek(const LongLong Offset, TSeekOrigin Origin) {
	DWORD dwMoveMethod = FILE_BEGIN; // Sinon warning "dwMoveMethod may be used uninitialized"
	LARGE_INTEGER liDistanceToMove;


	if (Origin == soBeginning) dwMoveMethod = FILE_BEGIN;
	else if (Origin == soCurrent) dwMoveMethod = FILE_CURRENT;
	else if (Origin == soEnd) dwMoveMethod = FILE_END;
	else dwMoveMethod = FILE_BEGIN;  // Probablement

	liDistanceToMove.QuadPart = Offset;
	return (LongLong) SetFilePointerEx(hFile, liDistanceToMove, 0, dwMoveMethod);
}

//---------------------------------------------------------------------------
// Implémentation de la méthode virtuelle pour l'écriture dans le flux.
//---------------------------------------------------------------------------

LongLong FASTCALL TFileStream::Write(const void *Buffer, LongLong Count) {
  LongLong ByteWrite;
	DWORD dwByteWrite;
	DWORD dwCount;

	ByteWrite = 0;
	while (Count) {
		if (Count <= UINT_MAX) dwCount = (DWORD) Count;
		else dwCount = UINT_MAX;
		WriteFile(hFile, Buffer, dwCount, &dwByteWrite, NULL);
		ByteWrite += dwByteWrite;
		Count -= dwByteWrite;
		if (dwByteWrite < dwCount) break;
	}
	FPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	if (FSize < FPosition) FSize = FPosition;

	return ByteWrite;
}
