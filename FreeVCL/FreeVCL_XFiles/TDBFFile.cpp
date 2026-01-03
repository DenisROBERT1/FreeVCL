//-----------------------------------------------------------------------------
//! @file TDBFFile.cpp
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
#include <FreeVcl_XFiles.h>


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TDBFFile::TDBFFile(TComponent *AOwner): TComponent(AOwner) {
  // Initialisations
  hFile = INVALID_HANDLE_VALUE;
  HeaderDbf.nbRecords = 0;
  Record = NULL;
}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TDBFFile::~TDBFFile() {
  CloseFile();
}

//---------------------------------------------------------------------------
bool TDBFFile::OpenFile(AnsiString asFileName) {
  DWORD dw;
  FIELDDBF FieldDbf;
  FIELDDBF4 FieldDbf4;
  int NbFields;
  int FieldPos;
  int i;
  char c;
  bool Ok = true;
  AnsiString asErrMsg;


  // Remise à zéro
  CloseFile();
  FFields.clear();
  memset(&HeaderDbf, 0, sizeof(HeaderDbf));

  hFile = ::CreateFile(asFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    // Erreur ouverture asFileName
		FLastError = GetLastError();
    asErrMsg = _TT("Erreur ouverture ") + asFileName;
    MessageBox(NULL, asErrMsg, _TT("Erreur"), MB_OK | MB_ICONSTOP);
    Ok = false;
  }

  if (Ok) {
    ReadFile(hFile, &HeaderDbf, sizeof(HEADERDBF), &dw, NULL);
    if (HeaderDbf.ValidDBASE != 0x03 &&
        HeaderDbf.ValidDBASE != 0x04 &&
        HeaderDbf.ValidDBASE != 0x30 &&
        HeaderDbf.ValidDBASE != 0x83 &&
        HeaderDbf.ValidDBASE != 0x84 &&
        HeaderDbf.ValidDBASE != 0xF5) {
      // Erreur "Fichier de type incorrect"
      asErrMsg = _TT("Fichier de type incorrect ") + asFileName;
      MessageBox(NULL, asErrMsg, _TT("Erreur"), MB_OK | MB_ICONSTOP);
      Ok = false;
    }
  }

  if (Ok) {
    Version = 3;
    if (HeaderDbf.ValidDBASE == 0x04 ||
        HeaderDbf.ValidDBASE == 0x84) Version = 4;

    // Lecture des champs du DBF
    FieldPos = 0;
    if (Version == 4) {
      SetFilePointer(hFile, sizeof(HEADERDBF4) - sizeof(HEADERDBF), NULL, FILE_CURRENT);
      NbFields = (HeaderDbf.LengthHeader - (short) sizeof(HEADERDBF4) - 1) / (short) sizeof(FIELDDBF4);
    }
    else {
      NbFields = (HeaderDbf.LengthHeader - (short) sizeof(HEADERDBF) - 1) / (short) sizeof(FIELDDBF);
    }
    for (i = 0; i < NbFields; i++) {
      if (Version == 4) {
        ReadFile(hFile, &FieldDbf4, sizeof(FIELDDBF4), &dw, NULL);
      }
      else {
        ReadFile(hFile, &FieldDbf, sizeof(FIELDDBF), &dw, NULL);
        lstrcpyA(FieldDbf4.FieldName, FieldDbf.FieldName);
        FieldDbf4.FieldType = FieldDbf.FieldType;
        FieldDbf4.FieldLength = FieldDbf.FieldLength;
        FieldDbf4.FieldDecimal = FieldDbf.FieldDecimal;
      }
      FieldDbf4.FieldPos = FieldPos + 1;
      FieldPos += FieldDbf.FieldLength;
      FFields.push_back(FieldDbf4);
    }
    ReadFile(hFile, &c, 1, &dw, NULL);
    DebData = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

    // Buffer enregistrement
    if (Record) delete[] Record;
    if (Data) delete[] Data;
    Record = new char[HeaderDbf.LengthRecord];
    Data = new char[HeaderDbf.LengthRecord + 1];
    NumRecord = -1;
    bModifRecord = false;
    bModifFile = false;

  }

  return Ok;
}

//---------------------------------------------------------------------------
bool TDBFFile::CreateFile(AnsiString asFileName) {
  bool Ok = true;
  AnsiString asErrMsg;
  TDateTime ToDay;


  // Remise à zéro
  CloseFile();
  FFields.clear();
  memset(&HeaderDbf, 0, sizeof(HeaderDbf));

  hFile = ::CreateFile(asFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    // Erreur ouverture asFileName
		FLastError = GetLastError();
    asErrMsg = _TT("Erreur ouverture ") + asFileName;
    MessageBox(NULL, asErrMsg, _TT("Erreur"), MB_OK | MB_ICONSTOP);
    Ok = false;
  }

  if (Ok) {
    memset(&HeaderDbf, 0, sizeof(HeaderDbf));
    HeaderDbf.ValidDBASE = 0x03;
    ToDay.CurrentDate();
    HeaderDbf.YY = ToDay.Year - 1900;
    HeaderDbf.MM = (BYTE) ToDay.Month;
    HeaderDbf.DD = (BYTE) ToDay.Day;
    HeaderDbf.nbRecords = 0;
    HeaderDbf.LengthHeader = sizeof(HEADERDBF);
    HeaderDbf.LengthRecord = 1;

    Version = 3;
    NumRecord = -1;
    FWorkAreaId = 0;
    bModifRecord = false;
    bModifFile = true;

    DebData = sizeof(HEADERDBF);

    // Buffer enregistrement
    if (Record) delete[] Record;
    if (Data) delete[] Data;

  }

  return Ok;
}

//---------------------------------------------------------------------------
bool TDBFFile::CloseFile() {
  if (hFile != INVALID_HANDLE_VALUE) {
    if (bModifRecord) WriteRecord();
    if (bModifFile) WriteHeader();
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    if (Record) {
      delete[] Record;
      delete[] Data;
      Record = NULL;
    }

    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété NbFields
//---------------------------------------------------------------------------

int TDBFFile::Get_NbFields() {
  return (int) FFields.size();
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété NbRecords
//---------------------------------------------------------------------------

int TDBFFile::Get_NbRecords() {
  return HeaderDbf.nbRecords;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FieldLength
//---------------------------------------------------------------------------

int TDBFFile::Get_FieldLength(int i) {
  return FFields[i].FieldLength;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FieldType
//---------------------------------------------------------------------------

char TDBFFile::Get_FieldType(int i) {
  return FFields[i].FieldType;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété FieldName
//---------------------------------------------------------------------------

AnsiString TDBFFile::Get_FieldName(int i) {
  return AnsiString(FFields[i].FieldName);
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DBFVersion
//---------------------------------------------------------------------------

BYTE TDBFFile::Get_DBFVersion(void) {
  return HeaderDbf.ValidDBASE;
}

bool TDBFFile::Set_DBFVersion(BYTE NewDBFVersion) {

  if (HeaderDbf.ValidDBASE != NewDBFVersion) {
    HeaderDbf.ValidDBASE = NewDBFVersion;
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété SecretCode
//---------------------------------------------------------------------------

unsigned long TDBFFile::Get_SecretCode(void) {
  return HeaderDbf.SecretCode;
}

bool TDBFFile::Set_SecretCode(unsigned long NewSecretCode) {

  if (HeaderDbf.SecretCode != NewSecretCode) {
    HeaderDbf.SecretCode = NewSecretCode;
  }
  return true;
}

//---------------------------------------------------------------------------
// Ajout d'un champ dans le fichier
//---------------------------------------------------------------------------

bool TDBFFile::AddField(AnsiString asName, BYTE Type, int Length, int Decimal, int WorkAreaId) {
  FIELDDBF4 FieldDbf4;
  int i;


  if (HeaderDbf.nbRecords != 0) return false;

  memset(&FieldDbf4, 0, sizeof(FIELDDBF4));
	asName.CharChar(FieldDbf4.FieldName, sizeof(FieldDbf4.FieldName));
  FieldDbf4.FieldType = Type;
  FieldDbf4.FieldLength = Length;
  i = (int) FFields.size();
  if (i == 0) FieldDbf4.FieldPos = 1;
  else FieldDbf4.FieldPos = FFields[i - 1].FieldPos + FFields[i - 1].FieldLength;
  FieldDbf4.FieldDecimal = Decimal;
  FieldDbf4.ProductionMDX = WorkAreaId;
  FFields.push_back(FieldDbf4);

  if (Version == 3) {
    DebData = sizeof(HEADERDBF) + (int) FFields.size() * sizeof(FIELDDBF) + 1;
    HeaderDbf.LengthHeader = (short) (sizeof(HEADERDBF) + (int) FFields.size() * sizeof(FIELDDBF) + 1);
  }
  else {
    DebData = sizeof(HEADERDBF4) + (int) FFields.size() * sizeof(FIELDDBF4) + 1;
    HeaderDbf.LengthHeader = (short) (sizeof(HEADERDBF4) + (int) FFields.size() * sizeof(FIELDDBF4) + 1);
  }

  HeaderDbf.LengthRecord += Length;
  if (Record) delete[] Record;
  if (Data) delete[] Data;
  Record = new char[HeaderDbf.LengthRecord];
  Data = new char[HeaderDbf.LengthRecord + 1];

  return true;
}

//---------------------------------------------------------------------------
// Lecture d'un champ
//---------------------------------------------------------------------------

AnsiString TDBFFile::GetData(int NumEnr, int NumField) {
  AnsiString asRet;
  int Len;


  if (NumField < 0 || NumField >= (int) FFields.size()) return asRet;
  if (NumEnr < 0 || NumEnr >= HeaderDbf.nbRecords) return asRet;
  if (!ReadRecord(NumEnr)) return asRet;

  Len = FFields[NumField].FieldLength;
  memcpy(Data, &Record[FFields[NumField].FieldPos], Len);
  Data[Len] = _T('\0');
  switch (FFields[NumField].FieldType) {
  case _T('C'):
    OemToAnsi(Data, Data);
    asRet = Data;
    asRet = asRet.TrimRight();
    break;
  case _T('N'):
    asRet = Data;
    asRet = asRet.TrimRight();
    break;
  case _T('D'):
    asRet = Data;
    asRet = asRet.TrimRight();
    break;
  case _T('L'):
    asRet = Data;
    asRet = asRet.TrimRight();
    break;
  }

  return asRet;
}

//---------------------------------------------------------------------------
// Ecriture d'un champ
//---------------------------------------------------------------------------

bool TDBFFile::SetData(int NumEnr, int NumField, int Valeur) {
  return SetData(NumEnr, NumField, AnsiString(Valeur));
}

bool TDBFFile::SetData(int NumEnr, int NumField, TDateTime &ValDate) {
  return SetData(NumEnr, NumField, ValDate.FormatString(_T("yyyymmaa")));
}

bool TDBFFile::SetData(int NumEnr, int NumField, AnsiString asValue) {
  int Len, LenStr;


  if (NumField < 0 || NumField >= (int) FFields.size()) return false;
  if (NumEnr < 0) return false;
  if (!ReadRecord(NumEnr)) return false;

  Len = FFields[NumField].FieldLength;
  LenStr = Len;
  if (LenStr > asValue.Length()) LenStr = asValue.Length();
  switch (FFields[NumField].FieldType) {
  case _T('C'):
    memset(Data, _T(' '), Len);
    Data[Len] = _T('\0');
    memcpy(Data, (LPCTSTR) asValue, LenStr);
    AnsiToOem(Data, Data);
    memcpy(&Record[FFields[NumField].FieldPos], Data, Len);
    break;
  case _T('N'):
    memset(Data, _T(' '), Len);
    Data[Len] = _T('\0');
    memcpy(Data, (LPCTSTR) asValue, LenStr);
    memcpy(&Record[FFields[NumField].FieldPos], Data, Len);
    break;
  case _T('D'):
    memset(Data, _T(' '), Len);
    Data[Len] = _T('\0');
    memcpy(Data, (LPCTSTR) asValue, LenStr);
    memcpy(&Record[FFields[NumField].FieldPos], Data, Len);
    break;
  case _T('L'):
    memset(Data, _T(' '), Len);
    Data[Len] = _T('\0');
    memcpy(Data, (LPCTSTR) asValue, LenStr);
    memcpy(&Record[FFields[NumField].FieldPos], Data, Len);
    break;
  }
  bModifRecord = true;

  return true;
}

//---------------------------------------------------------------------------
// Lecture d'un enregistrement
//---------------------------------------------------------------------------

bool TDBFFile::ReadRecord(int NumEnr) {
  DWORD dw;
  int i;


  if (NumEnr != NumRecord) {
    if (bModifRecord) WriteRecord();
    if (NumEnr < HeaderDbf.nbRecords) {
      SetFilePointer(hFile, DebData + NumEnr * HeaderDbf.LengthRecord, NULL, FILE_BEGIN);
      ReadFile(hFile, Record, HeaderDbf.LengthRecord, &dw, NULL);
    }
    else {
      memset(Record, _T(' '), HeaderDbf.LengthRecord);
      SetFilePointer(hFile, DebData + (NumRecord + 1) * HeaderDbf.LengthRecord, NULL, FILE_BEGIN);
      for (i = HeaderDbf.nbRecords; i < NumEnr; i++) {
        WriteFile(hFile, Record, HeaderDbf.LengthRecord, &dw, NULL);
      }
      bModifRecord = true;
    }
    NumRecord = NumEnr;
  }

  return true;
}

//---------------------------------------------------------------------------
// Ecriture d'un enregistrement
//---------------------------------------------------------------------------

bool TDBFFile::WriteRecord() {
  DWORD dw;


  SetFilePointer(hFile, DebData + NumRecord * HeaderDbf.LengthRecord, NULL, FILE_BEGIN);
  WriteFile(hFile, Record, HeaderDbf.LengthRecord, &dw, NULL);
  bModifRecord = false;
  bModifFile = true;
  if (HeaderDbf.nbRecords < NumRecord + 1) HeaderDbf.nbRecords = NumRecord + 1;

  return true;
}

//---------------------------------------------------------------------------
// Ecriture du header
//---------------------------------------------------------------------------

bool TDBFFile::WriteHeader() {
  DWORD dw;
  LPHEADERDBF lpHeaderDbf3;
  FIELDDBF FieldDbf;
  int i;


  SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
  if (Version == 4) {
    WriteFile(hFile, &HeaderDbf, sizeof(HEADERDBF4), &dw, NULL);
  }
  else {
    lpHeaderDbf3 = (LPHEADERDBF) &HeaderDbf;
    lpHeaderDbf3->SecretCode = HeaderDbf.SecretCode;
    WriteFile(hFile, &HeaderDbf, sizeof(HEADERDBF), &dw, NULL);
  }

  for (i = 0; i < (int) FFields.size(); i++) {
    if (Version == 4) {
      WriteFile(hFile, &FFields[i], sizeof(FIELDDBF4), &dw, NULL);
    }
    else {
      memset(&FieldDbf, 0, sizeof(FIELDDBF));
      lstrcpyA(FieldDbf.FieldName, FFields[i].FieldName);
      FieldDbf.FieldType = FFields[i].FieldType;
      FieldDbf.FieldLength = FFields[i].FieldLength;
      FieldDbf.FieldDecimal = FFields[i].FieldDecimal;
      FieldDbf.WorkAreaId = FFields[i].ProductionMDX;
      WriteFile(hFile, &FieldDbf, sizeof(FIELDDBF), &dw, NULL);
    }
  }
  WriteFile(hFile, "\r", 1, &dw, NULL);

  if (HeaderDbf.nbRecords == 0) WriteFile(hFile, "\x1A", 1, &dw, NULL);

  return true;
}



//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TDBFFile::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("NbFields"));
  ListProperties->Add(_T("NbRecords"));
  ListProperties->Add(_T("DBFVersion"));
  ListProperties->Add(_T("SecretCode"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TDBFFile::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("NbFields")) {
    return tpNumber;
  }
  if (asProperty == _T("NbRecords")) {
    return tpNumber;
  }
  if (asProperty == _T("DBFVersion")) {
    return tpNumber;
  }
  if (asProperty == _T("SecretCode")) {
    return tpText;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TDBFFile::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("NbFields")) {
    return _T("0");
  }
  if (asProperty == _T("NbRecords")) {
    return _T("0");
  }
  if (asProperty == _T("DBFVersion")) {
    return _T("3");
  }
  if (asProperty == _T("SecretCode")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TDBFFile::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("NbFields")) {
    return Get_NbFields();
  }
  if (asProperty == _T("NbRecords")) {
    return Get_NbRecords();
  }
  if (asProperty == _T("DBFVersion")) {
    return Get_DBFVersion();
  }
  if (asProperty == _T("SecretCode")) {
    return Get_SecretCode();
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TDBFFile::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  AnsiString asMot;


  if (asProperty == _T("NbFields")) {
    return false;
  }
  if (asProperty == _T("NbRecords")) {
    return false;
  }
  if (asProperty == _T("DBFVersion")) {
    Set_DBFVersion(asValue.ToInt());
    return true;
  }
  if (asProperty == _T("SecretCode")) {
    Set_SecretCode(asValue.ToInt());
    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

