//-----------------------------------------------------------------------------
//! @file TFileForm.h
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

#ifndef TFileFormH
#define TFileFormH

#include <vector>

//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Types d'évènements
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnNewFormInit.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnNewFormInit)(TObject *Sender, TElemBase *Form);

//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnNewComponentInit.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnNewComponentInit)(TObject *Sender, TElemBase *Form, TElemBase *Parent, TElemBase *Component);


//---------------------------------------------------------------------------
//! @class TFileForm
//!
//! @brief Lecture d'un fichier de dialogue
//!
//! Cette classe (@c TFileForm) permet de lire différents formats de fichiers
//! de dialogue, et de les mémoriser sous une forme standard.
//!
//! @author Denis ROBERT
//---------------------------------------------------------------------------

class TFileForm: public TObject {
private:
  TElemVCL * NewFormElemVCL;
	HWND hWndMain;

protected:
  int NbStatics;

  AnsiString UnMot(AnsiString asLigne, AnsiString asDelim1, AnsiString asDelim2, int * Cur);
  AnsiString UnArg(AnsiString asLigne, int * Cur);
  AnsiString UneOption(AnsiString asLigne, int * Cur);
  AnsiString UnTexte(AnsiString asLigne, int * Cur);
  AnsiString UnTruc(AnsiString asLigne, int * Cur, char Delim);
  bool GetBaseUnit(HFONT hFont, int *baseunitX, int *baseunitY);
  void OrdreDesComposants(TForm * Form);
  bool SetProperty(TElemVCL *Component,
                              AnsiString asProperty,
                              AnsiString asValue);
  AnsiString AddGuillVCL(AnsiString asTexte);
  AnsiString SuppGuillVCL(AnsiString asTexte);
  AnsiString SuppGuillFRM(AnsiString asTexte);
  AnsiString FVCLDeclarations(TElemVCL *Control);
  AnsiString FVCLEvents(TElemVCL * Control);
  AnsiString FVCLCallbacks(TElemVCL * Control, AnsiString asClassName);
  bool SaveComponentsDFM(HANDLE hFile, TElemVCL * Control, int Indent);
  bool SaveComponentsRC(HANDLE hFile, TElemVCL * ElemVCL, int XInit, int YInit, int baseunitX, int baseunitY);
	bool CreateFileH(AnsiString asFileNameH, AnsiString asClassName);
	bool CreateFileCpp(AnsiString asFileNameCpp, AnsiString asClassName);
	int ArgsWithoutNames(AnsiString asListeArgs, AnsiString *asArgsWithoutNames);
	bool IsAlphaNum(TCHAR c);

public:


  //! @name Constructeurs / destructeurs
  //@{


  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TFileForm(void);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  ~TFileForm(void);

  //@}


  //! @name Méthodes
  //@{

  bool OpenRC(AnsiString asFileName);
  bool OpenDFM(AnsiString asFileName);
  bool OpenFRM(AnsiString asFileName);
  bool SaveRC(AnsiString asFileName, std::vector<DIALOGUE> &Dialogues);
  bool SaveDFM(AnsiString asFileName, std::vector<DIALOGUE> &Dialogues);
	bool LoadFromWindow(HWND hWnd);

	static BOOL CALLBACK EnumChildWnd(HWND hWnd, LPARAM lParam);
	BOOL EnumChildWnd(HWND hWnd);

  //@}


  //! @name Propriétés
  //@{

  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement initialisation d'une form
	//!
	//! Cet évènement est déclenché lorsqu'une nouvelle fenêtre est créée
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nouvel objet
  //---------------------------------------------------------------------------
  DECLARE_EVENT_2(TOnNewFormInit, OnNewFormInit, TObject *, TElemBase *);

  //---------------------------------------------------------------------------
  //! @brief Evènement initialisation d'un contrôle
	//!
	//! Cet évènement est déclenché lorsqu'un nouveau contrôle est créé
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 Nouveau contrôle
  //! @param[in]        Arg3 Fenêtre principale
  //! @param[in]        Arg4 Contrôle parent
  //! @param[in]        Arg5 Nouveau contrôle
  //---------------------------------------------------------------------------
  DECLARE_EVENT_4(TOnNewComponentInit, OnNewComponentInit, TObject *, TElemBase *, TElemBase *, TElemBase *);

  //@}

};


#endif
