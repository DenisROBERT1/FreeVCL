//-----------------------------------------------------------------------------
//! @file TForm_ConvertNumbers.cpp
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

#include <FreeVCL.h>

#include "TTextEditor.h"
#include "TConfig.h"
#include "TFVCLIDEAutomation.h"
#include "TForm_DisplayError.h"
#include "TForm_ConvertNumbers.h"
#include "HLP_FVCLIDE_Fr.h"


//---------------------------------------------------------------------------
TForm_ConvertNumbers::TForm_ConvertNumbers(TComponent *Owner, LPCTSTR szName)
    : TForm(Owner, szName, FVCL_ARG_SUPP) {
	bNoEvent = false;
	Shape_2->Pen->Color = clBtnFace;
	Shape_2->Brush->Color = clBtnFace;
}

//---------------------------------------------------------------------------
TForm_ConvertNumbers::~TForm_ConvertNumbers(void) {
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_ChaineHexaChange(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
    LngString = HexaToAscii(((AnsiString) Edit_ChaineHexa->Text), szString, MAX_LEN_STRING);
		UpdateDialog(Edit_ChaineHexa);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_ChaineAsciiChange(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
		lstrcpynA(szString, ((AnsiString) Edit_ChaineAscii->Text).AsCharString(), MAX_LEN_STRING);
		LngString = lstrlenA(szString);
		UpdateDialog(Edit_ChaineAscii);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_ChaineUnicodeChange(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
		lstrcpynW((wchar_t *) szString, ((AnsiString) Edit_ChaineUnicode->Text).AsWideString(), MAX_LEN_STRING);
		LngString = lstrlenW((wchar_t *) szString) * sizeof(wchar_t);
		UpdateDialog(Edit_ChaineUnicode);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_ShortIntChange(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
    if (CheckBox_Signe->Checked) {
	    *((unsigned short *) szString) = (unsigned short) ((AnsiString) Edit_ShortInt->Text).ToIntDef(0);
		}
		else {
	    *((short *) szString) = (short) ((AnsiString) Edit_ShortInt->Text).ToIntDef(0);
		}
    LngString = 2;
		UpdateDialog(Edit_ShortInt);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_LongIntChange(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
    if (CheckBox_Signe->Checked) {
	    *((unsigned long *) szString) = (unsigned long) ((AnsiString) Edit_LongInt->Text).ToIntDef(0);
		}
		else {
	    *((long *) szString) = (long) ((AnsiString) Edit_LongInt->Text).ToIntDef(0);
		}
    LngString = 4;
		UpdateDialog(Edit_LongInt);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_Int64Change(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
    if (CheckBox_Signe->Checked) {
	    *((unsigned __int64 *) szString) = (unsigned __int64) ((AnsiString) Edit_Int64->Text).ToIntDef(0);
		}
		else {
	    *((__int64 *) szString) = (__int64) ((AnsiString) Edit_Int64->Text).ToIntDef(0);
		}
    LngString = 8;
		UpdateDialog(Edit_Int64);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_FloatChange(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
    *((float *) szString) = (float) ((AnsiString) Edit_Float->Text).ToDouble();
    LngString = 4;
		UpdateDialog(Edit_Float);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Edit_DoubleChange(TObject *Sender) {
  if (!bNoEvent) {
		memset(szString, 0, MAX_LEN_STRING);
    *((double *) szString) = ((AnsiString) Edit_Double->Text).ToDouble();
    LngString = 8;
		UpdateDialog(Edit_Double);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Button_RotLeftClick(TObject *Sender) {
	int i;

  if (!bNoEvent) {
    for (i = 0; i < LngString; i++) {
      szString[i] = ((BYTE) szString[i] << 1) + ((BYTE) szString[i + 1] >> 7);
    }
		UpdateDialog(NULL);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Button_RotRightClick(TObject *Sender) {
	int i;
	BYTE b;

  if (!bNoEvent) {
    for (i = LngString - 1; i >= 0; i--) {
      if (i) b = ((BYTE) szString[i - 1] & 0x01);
      else b = 0;
      szString[i] = ((BYTE) szString[i] >> 1) + (b << 7);
    }
		UpdateDialog(NULL);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Button_InvertClick(TObject *Sender) {
  int i;
  BYTE b;

  if (!bNoEvent) {
    for (i = 0; i < LngString / 2; i++) {
      b = szString[i];
      szString[i] = szString[LngString - i - 1];
      szString[LngString - i - 1] = b;
    }
		UpdateDialog(NULL);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::CheckBox_SigneClick(TObject *Sender) {
  if (!bNoEvent) {
		UpdateDialog(NULL);
	}
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::Button_AideClick(TObject *Sender) {
	Application->HelpFile = (AnsiString) Config->InstallPath +  _T("\\Help\\FVCLIDE_Fr.chm");
	Application->HelpCommand(HELP_CONTEXT, HLP_CONVERTNUMBERS);
}

//---------------------------------------------------------------------------
void FASTCALL TForm_ConvertNumbers::UpdateDialog(TControl *Control) {
  int i;
  TCHAR szBuf[MAX_LEN_STRING * 3];
  wchar_t szBufUnicode[MAX_LEN_STRING];


  bNoEvent = true;

  if (Control != Edit_ChaineHexa) {
    AsciiToHexa(szString, LngString, szBuf);
		Edit_ChaineHexa->Text = szBuf;
  }

  if (Control != Edit_ChaineAscii) {
    for (i = 0; i < LngString; i++) {
      if ((BYTE) szString[i] < ' ') szBuf[i] = '.';
			else szBuf[i] = (BYTE) szString[i];
    }
    szBuf[i] = '\0';
		Edit_ChaineAscii->Text = szBuf;
  }

  if (Control != Edit_ChaineUnicode) {
    for (i = 0; i < (int) (LngString / sizeof(wchar_t)); i++) {
      if (((wchar_t *) szString)[i] < L' ') szBufUnicode[i] = L'.';
			else szBufUnicode[i] = ((wchar_t *) szString)[i];
    }
    szBufUnicode[i] = '\0';
		Edit_ChaineUnicode->Text = szBufUnicode;
  }

  if (Control != Edit_ShortInt) {
    if (CheckBox_Signe->Checked) {
			wsprintf(szBuf, _T("%i"), * ((short *) szString));
    }
    else {
			wsprintf(szBuf, _T("%u"), * ((unsigned short *) szString));
    }
		Edit_ShortInt->Text = szBuf;
  }

  if (Control != Edit_LongInt) {
    if (CheckBox_Signe->Checked) {
			wsprintf(szBuf, _T("%i"), * ((long *) szString));
    }
    else {
			wsprintf(szBuf, _T("%u"), * ((unsigned long *) szString));
    }
		Edit_LongInt->Text = szBuf;
  }

  if (Control != Edit_Int64) {
    if (CheckBox_Signe->Checked) {
			wsprintf(szBuf, _T("%I64i"), * ((__int64 *) szString));
    }
    else {
			wsprintf(szBuf, _T("%I64u"), * ((unsigned __int64 *) szString));
    }
		Edit_Int64->Text = szBuf;
  }

  if (Control != Edit_Float) {
		char szCharBuf[1024];
		sprintf_s(szCharBuf, sizeof(szCharBuf), "%f", * ((float *) szString));
		Edit_Float->Text = szCharBuf;
  }

  if (Control != Edit_Double) {
		char szCharBuf[1024];
		sprintf_s(szCharBuf, sizeof(szCharBuf), "%f", * ((double *) szString));
		Edit_Double->Text = szCharBuf;
  }

  bNoEvent = false;
}


//---------------------------------------------------------------------------

void TForm_ConvertNumbers::AsciiToHexa(LPCSTR szString1, int Lng, LPTSTR szString2) {
  int i;
  BYTE c;


  for (i = 0; i < Lng; i++) {
    c = szString1[i];
    wsprintf(&szString2[3 * i], _T("%02X"), c);
    szString2[3 * i + 2] = ' ';
  }
  szString2[3 * i] = '\0';
}


//---------------------------------------------------------------------------

int TForm_ConvertNumbers::HexaToAscii(LPCTSTR szString1, LPSTR szString2, int SizeString2) {
  int i, j;
  int H, L;
  int Lng, nbO;
  char c;


  Lng = lstrlen(szString1);
  j = 0;
  nbO = 0;
  H = 0;
  L = 0;
  for (i = 0; i <= Lng; i++) {
    c = (char) szString1[i];
    if (c == ' ' || c == '\0' || nbO == 2) {
      if (nbO > 0) {
        c = (char) (H * 16 + L);
        szString2[j++] = c;
        nbO = 0;
        H = 0;
        L = 0;
        i--;
      }
    }
    else {
      H = L;
      if ('0' <= c && c <= '9') L = c - '0';
      else if ('A' <= c && c <= 'F') L = c - 'A' + 10;
      else if ('a' <= c && c <= 'f') L = c - 'a' + 10;
      else L = 0;
      nbO++;
    }

		if (j == SizeString2 - 1) break;

  }
  szString2[j] = '\0';
  
  return j;
}

//---------------------------------------------------------------------------
