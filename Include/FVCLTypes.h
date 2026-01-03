//-----------------------------------------------------------------------------
//! @file FVCLTypes.h
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

#ifdef In_FVCLTypesH
#error "Inclusion circulaire de FVCLTypes"
#endif // In_FVCLTypesH

#define In_FVCLTypesH

#ifndef FVCLTypesH
#define FVCLTypesH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Compatibilité des compilateurs
//---------------------------------------------------------------------------

#ifndef FASTCALL
#ifdef __BORLANDC__
#define FASTCALL __fastcall
#else  // __BORLANDC__
#define FASTCALL
#endif  // __BORLANDC__
#endif  // FASTCALL


//---------------------------------------------------------------------------
// LINUX
//---------------------------------------------------------------------------

//! @name Types LINUX
//@{

#ifdef LINUX

#define WINAPI
#define BOOL int
#define TRUE ((BOOL) true)
#define FALSE ((BOOL) false)
#define BYTE unsigned char
#define LPBYTE unsigned char *
#define WORD unsigned short
#define DWORD unsigned long
#define UINT unsigned int
#define LONG long
#define ULONG unsigned long
#define LONGLONG long long
#define PSTR char *
#define LPSTR char *
#define PCSTR const char *
#define LPCSTR const char *
#define TCHAR char
#define PTSTR TCHAR *
#define LPTSTR TCHAR *
#define PCTSTR const TCHAR *
#define LPCTSTR const TCHAR *
#define WCHAR wchar_t
#define PWSTR WCHAR *
#define LPWSTR WCHAR *
#define PCWSTR const WCHAR *
#define LPCWSTR const WCHAR *
#define VOID void
#define PVOID void *
#define LPVOID void *
#define MB_OK 0
#define IDOK     1
#define IDCANCEL 2
#define IDABORT  3
#define IDRETRY  4
#define IDIGNORE 5
#define IDYES    6
#define IDNO     7
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define GetRValue(rgb) ((rgb) & 0x000000FF)
#define GetGValue(rgb) (((rgb) & 0x0000FF00)>>8)
#define GetBValue(rgb) (((rgb) & 0x00FF0000)>>16)
#define DIR_DELIMITER _T("/")

typedef VOID * HANDLE;
typedef HANDLE HMODULE;
typedef HANDLE HWND;
typedef HANDLE HGLOBAL;
typedef HANDLE HDC;
typedef HANDLE HINSTANCE;
typedef int HMENU;
typedef int HPEN;
typedef int HBRUSH;
typedef int HFONT;
typedef int HCURSOR;
typedef int HICON;
typedef DWORD COLORREF;


struct RECT {
  int left;
  int top;
  int right;
  int bottom;
};

typedef RECT * PRECT, * LPRECT;

struct POINT {
  int x;
  int y;
};

typedef POINT * PPOINT, LPPOINT;

struct SIZE {
  int cx;
  int cy;
};

typedef SIZE * PSIZE, LPSIZE;

struct TEXTMETRIC {
  LONG tmHeight;
  LONG tmAscent;
  LONG tmDescent;
  LONG tmInternalLeading;
  LONG tmExternalLeading;
  LONG tmAveCharWidth;
  LONG tmMaxCharWidth;
};

#define SB_HORZ             0
#define SB_VERT             1
#define SB_CTL              2
#define SB_BOTH             3

#else  // LINUX

#define DIR_DELIMITER _T("\\")

#endif  // LINUX

//@}


//---------------------------------------------------------------------------
// Enums
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @enum TYPEPROPERTY
//!
//! @brief Type des propriétés publiées
//---------------------------------------------------------------------------

enum TYPEPROPERTY {
	tpNull,												//!< Type inconnu
	tpText,												//!< Type texte (AnsiString)
	tpChar,												//!< Type Caractère
	tpNumber,											//!< Type Nombre entier
	tpBool,												//!< Type booléen
	tpColor,											//!< Type Couleur
	tpChoice,											//!< Type choix unique
	tpMultipleChoice,							//!< Type choix multiple
	tpNumbers,										//!< Type liste de nombres
	tpStrings,										//!< Type liste de chaînes
	tpObject,											//!< Type objet
	tpGraphic,										//!< Type graphique
	tpData,												//!< Type données hexadécimal
	tpEvent												//!< Type évènement
};

enum TFormBorderStyle {
	bsNone,												//!< Fenêtre sans bordure
	bsSingle,											//!< Fenêtre avec bordure simple
	bsSizeable,										//!< Fenêtre redimensionnable
	bsDialog,											//!< Fenêtre de type dialogue (non redimensionnable)
	bsToolWindow,									//!< Fenêtre avec barre de titre réduite
	bsSizeToolWin									//!< Fenêtre redimensionnable avec barre de titre réduite
};

enum TCursor {
	crMin = -32768,								//!< Numéro minimum de curseur
	crMax = 32767									//!< Numéro maximum de curseur
};

enum TScrollStyle {
	ssNone,												//!< Aucune barre de défilement
	ssHorizontal,									//!< Barre de défilement horizontale
	ssVertical,										//!< Barre de défilement verticale
	ssBoth												//!< Barres de défilement horizontale et verticale
};

enum TOwnerDrawState {
	odSelected,										//!< Elément sélectionné
	odGrayed,											//!< Elément grisé
	odDisabled,										//!< Elément sélectionné
	odChecked,										//!< Elément coché
	odFocused,										//!< Elément possédant le focus
  odDefault,										//!< Elément par défaut
	odHotLight,										//!< Elément en surbrillance
	odInactive,										//!< Elément inactif
	odNoAccel,										//!< Le soulignement indiquant le raccourci ne doit pas être dessiné
	odNoFocusRect,								//!< Le rectangle de focus ne doit pas être dessiné
  odReserved1,									//!< Reservé 1
	odReserved2,									//!< Reservé 2
	odComboBoxEdit								//!< Elément est un élément de la liste déroulante d'une boîte de choix
};

typedef Set<TOwnerDrawState, odSelected, odComboBoxEdit> TOwnerDrawStates;  //!< Combinaison de plusieurs TOwnerDrawState

enum TDragMode {
	dmManual,											//!< Drag and drop désactivé ou géré par programme
	dmAutomatic										//!< Drag and drop activé et automatique
};

enum TDragState {
	dsDragEnter,									//!< La souris entre dans la zone de drop
	dsDragLeave,									//!< La souris quitte la zone de drop
	dsDragMove									  //!< La souris survole la zone de drop
};

enum TDragKind {
	dkDrag,									      //!< Style drag and drop (glissé-déposé)
	dkDock									      //!< Style dock
};

enum TScrollCode {
	scLineUp,									    //!< Ligne (ou colonne) précédente
	scLineDown,									  //!< Ligne (ou colonne) suivante
	scPageUp,									    //!< Page précédente
	scPageDown,									  //!< Page suivante
	scPosition,									  //!< Position définie
	scTrack,									    //!< Changement de position (non terminé)
	scTop,									      //!< Haut (ou partie gauche) de fichier
	scBottom,									    //!< Bas (ou partie droite) de fichier
	scEndScroll									  //!< Fin de l'opération de changement de position
};

enum TScrollBarKind {
	sbHorizontal = SB_HORZ,				//!< Ascenceur horizontal
	sbVertical = SB_VERT					//!< Ascenceur vertical
};


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------

#define crDefault	  ((TCursor)   0)
#define crNone	    ((TCursor)  -1)
#define crArrow	    ((TCursor)  -2)
#define crCross	    ((TCursor)  -3)
#define crIBeam	    ((TCursor)  -4)
#define crSizeNESW	((TCursor)  -6)
#define crSizeNS	  ((TCursor)  -7)
#define crSizeNWSE  ((TCursor)  -8)
#define crSizeWE	  ((TCursor)  -9)
#define crUpArrow	  ((TCursor) -10)
#define crHourGlass	((TCursor) -11)
#define crDrag	    ((TCursor) -12)
#define crNoDrop	  ((TCursor) -13)
#define crHSplit	  ((TCursor) -14)
#define crVSplit	  ((TCursor) -15)
#define crMultiDrag	((TCursor) -16)
#define crSQLWait	  ((TCursor) -17)
#define crNo	      ((TCursor) -18)
#define crAppStart	((TCursor) -19)
#define crHelp	    ((TCursor) -20)
#define crHandPoint	((TCursor) -21)
#define crSize	    ((TCursor) -22)
#define crSizeAll	  ((TCursor) -22)

// MESSAGE MAP
#define FVCL_BEGIN_MESSAGE_MAP                          \
	virtual void FASTCALL Dispatch(TMessage *Message) {   \
		switch  (Message->Msg) {

// La documentation BORLAND et le .h indiquent
// meth(*((type *) Message));
// ... mais ça ne compile pas ??? (cannot convert parameter from 'TMessage' to 'TMessage *')

#define FVCL_MESSAGE_HANDLER(msg, type, meth)           \
			case msg:                                         \
				meth((type *) Message);                         \
				break;

#define FVCL_END_MESSAGE_MAP(base)                      \
			default:                                          \
				base::Dispatch(Message);                        \
				break;                                          \
			}                                                 \
		}

// Macros MESSAGE MAP compatibles Borland (Mais conflit avec ATL)
#ifndef BEGIN_MESSAGE_MAP
#define BEGIN_MESSAGE_MAP FVCL_BEGIN_MESSAGE_MAP
#endif // BEGIN_MESSAGE_MAP

#ifndef MESSAGE_HANDLER
#define VCL_MESSAGE_HANDLER FVCL_MESSAGE_HANDLER
#define MESSAGE_HANDLER FVCL_MESSAGE_HANDLER
#endif // MESSAGE_HANDLER

#ifndef END_MESSAGE_MAP
#define END_MESSAGE_MAP FVCL_END_MESSAGE_MAP
#endif // END_MESSAGE_MAP

#define ERROR_VCL_BEGIN 0x20000000

//---------------------------------------------------------------------------
// Typedef
//---------------------------------------------------------------------------

typedef signed char ShortInt, *PShortInt;
typedef short SmallInt, *PSmallInt;
typedef int LongInt, *PLongInt;
typedef LONGLONG LongLong, *PLongLong;
typedef BYTE Byte, *PByte;
typedef WORD Word, *PWord;
typedef int Integer, *PInteger;
typedef unsigned int Cardinal, *PCardinal;
typedef bool Boolean, *PBoolean;
typedef unsigned char	ByteBool, *PByteBool;
typedef unsigned short WordBool, *PWordBool;
typedef BOOL LongBool, *PLongBool;
typedef char Char;
typedef unsigned char * PChar;  // Oui, unsigned. C'est comme ça dans la doc
typedef char AnsiChar;
typedef unsigned char * PAnsiChar;  // Oui, unsigned. C'est comme ça dans la doc
typedef wchar_t WideChar, *PWideChar;
typedef float Single, *PSingle;
typedef double Double, *PDouble;
typedef long double Extended, *PExtended;
typedef double Real, *PReal;
typedef void * Pointer, *PPointer;

typedef int TModalResult;      //!< Code de retour d'une fenêtre modale (mrOk, mrCancel, etc...)
typedef Word TShortCut;
typedef int TImageIndex;
typedef TFormBorderStyle TBorderStyle;
typedef Byte TFontCharset;
typedef __int64 THelpContext;



//---------------------------------------------------------------------------
//
//                         déclarations anticipées
//
//---------------------------------------------------------------------------

class TObject;


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//! @brief Structure d'un message système
//!
//! Cette structure encapsule un message système (commande et paramètres).
//---------------------------------------------------------------------------

struct TMessage {
	Cardinal Msg;
	union	{
		struct {
			Word WParamLo;
			Word WParamHi;
			Word LParamLo;
			Word LParamHi;
			Word ResultLo;
			Word ResultHi;
		};
		struct {
			int WParam;
			int LParam;
			int Result;
		};
	};
};

#ifndef LINUX

struct TCreateParams {
  char *Caption;
  int Style;
  int ExStyle;
  int X;
  int Y;
  int Width;
  int Height;
  HWND WndParent;
  void *Param;
  WNDCLASS WindowClass;
  char WinClassName[64];
};

struct TWMKey {
	unsigned Msg;
	Word CharCode;
	Word Unused;
	int KeyData;
	int Result;
};

#endif  // LINUX


class TVarRec {
public:
  union {
    Integer      VInteger;
    Boolean      VBoolean;
    Char         VChar;
    PExtended    VExtended;
    Pointer      VPointer;
    PChar        VPChar;
    TObject*     VObject;
    WideChar     VWideChar;
    PWideChar    VPWideChar;
    Pointer      VAnsiString;
    // PCurrency    VCurrency;

    // PVariant     VVariant;
  };
  union {
    Byte VType;
  };
};

#endif  // FVCLTypesH


#undef In_FVCLTypesH
