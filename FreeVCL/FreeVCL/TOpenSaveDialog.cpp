//-----------------------------------------------------------------------------
//! @file TOpenSaveDialog.cpp
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
// TOpenSaveDialog
//---------------------------------------------------------------------------

TOpenSaveDialog::TOpenSaveDialog(TComponent* AOwner): TComponent(AOwner) {

  // Initialisations
  if (AOwner) Handle = ((TControl *) AOwner)->Handle;
  else Handle = NULL;
  Flags = OFN_EXPLORER;
  FFilterIndex = 1;
}

TOpenSaveDialog::~TOpenSaveDialog(void) {
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Title
//---------------------------------------------------------------------------

AnsiString TOpenSaveDialog::Get_Title(void) {
  return FTitle;
}

bool TOpenSaveDialog::Set_Title(AnsiString NewTitle) {
  if (FTitle != NewTitle) {
    FTitle = NewTitle;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété InitialDir
//---------------------------------------------------------------------------

AnsiString TOpenSaveDialog::Get_InitialDir(void) {
  return FInitialDir;
}

bool TOpenSaveDialog::Set_InitialDir(AnsiString NewInitialDir) {
  if (FInitialDir != NewInitialDir) {
    FInitialDir = NewInitialDir;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FileName
//---------------------------------------------------------------------------

AnsiString TOpenSaveDialog::Get_FileName(void) {
  return FFileName;
}

bool TOpenSaveDialog::Set_FileName(AnsiString NewFileName) {
  if (FFileName != NewFileName) {
    FFileName = NewFileName;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Filter
//---------------------------------------------------------------------------

AnsiString TOpenSaveDialog::Get_Filter(void) {
  return FFilter;
}

bool TOpenSaveDialog::Set_Filter(AnsiString NewFilter) {
  if (FFilter != NewFilter) {
    FFilter = NewFilter;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété FilterIndex
//---------------------------------------------------------------------------

int TOpenSaveDialog::Get_FilterIndex(void) {
  return FFilterIndex;
}

bool TOpenSaveDialog::Set_FilterIndex(int NewFilterIndex) {
  if (FFilterIndex != NewFilterIndex) {
    FFilterIndex = NewFilterIndex;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété DefaultExt
//---------------------------------------------------------------------------

AnsiString TOpenSaveDialog::Get_DefaultExt(void) {
  return FDefaultExt;
}

bool TOpenSaveDialog::Set_DefaultExt(AnsiString NewDefaultExt) {
  if (FDefaultExt != NewDefaultExt) {
    FDefaultExt = NewDefaultExt;
  }
  return true;
}


//---------------------------------------------------------------------------
// Accesseurs de la propriété Options
//---------------------------------------------------------------------------

TOpenOptions TOpenSaveDialog::Get_Options(void) {
  TOpenOptions Options;
  Options.Clear();
  if (Flags & OFN_READONLY) Options << ofReadOnly;
  if (Flags & OFN_OVERWRITEPROMPT) Options << ofOverwritePrompt;
  if (Flags & OFN_HIDEREADONLY) Options << ofHideReadOnly;
  if (Flags & OFN_NOCHANGEDIR) Options << ofNoChangeDir;
  if (Flags & OFN_SHOWHELP) Options << ofShowHelp;
  if (Flags & OFN_ALLOWMULTISELECT) Options << ofAllowMultiSelect;
  if (Flags & OFN_EXTENSIONDIFFERENT) Options << ofExtensionDifferent;
  if (Flags & OFN_PATHMUSTEXIST) Options << ofPathMustExist;
  if (Flags & OFN_FILEMUSTEXIST) Options << ofFileMustExist;
  if (Flags & OFN_CREATEPROMPT) Options << ofCreatePrompt;
  if (Flags & OFN_SHAREAWARE) Options << ofShareAware;
  if (Flags & OFN_NOREADONLYRETURN) Options << ofNoReadOnlyReturn;
  if (Flags & OFN_NOTESTFILECREATE) Options << ofNoTestFileCreate;
  if (Flags & OFN_NONETWORKBUTTON) Options << ofNoNetworkButton;
  if (Flags & OFN_NOLONGNAMES) Options << ofNoLongNames;
  if (!(Flags & OFN_EXPLORER)) Options << ofOldStyleDialog;
  if (Flags & OFN_NODEREFERENCELINKS) Options << ofNoDereferenceLinks;
  if (Flags & OFN_ENABLEINCLUDENOTIFY) Options << ofEnableIncludeNotify;
  if (Flags & OFN_ENABLESIZING) Options << ofEnableSizing;

  return Options;
}

bool TOpenSaveDialog::Set_Options(TOpenOptions NewOptions) {
  Flags = OFN_EXPLORER;
  if (NewOptions.Contains(ofReadOnly)) Flags |= OFN_READONLY;
  if (NewOptions.Contains(ofOverwritePrompt)) Flags |= OFN_OVERWRITEPROMPT;
  if (NewOptions.Contains(ofHideReadOnly)) Flags |= OFN_HIDEREADONLY;
  if (NewOptions.Contains(ofNoChangeDir)) Flags |= OFN_NOCHANGEDIR;
  if (NewOptions.Contains(ofShowHelp)) Flags |= OFN_SHOWHELP;
  if (NewOptions.Contains(ofAllowMultiSelect)) Flags |= OFN_ALLOWMULTISELECT;
  if (NewOptions.Contains(ofExtensionDifferent)) Flags |= OFN_EXTENSIONDIFFERENT;
  if (NewOptions.Contains(ofPathMustExist)) Flags |= OFN_PATHMUSTEXIST;
  if (NewOptions.Contains(ofFileMustExist)) Flags |= OFN_FILEMUSTEXIST;
  if (NewOptions.Contains(ofCreatePrompt)) Flags |= OFN_CREATEPROMPT;
  if (NewOptions.Contains(ofShareAware)) Flags |= OFN_SHAREAWARE;
  if (NewOptions.Contains(ofNoReadOnlyReturn)) Flags |= OFN_NOREADONLYRETURN;
  if (NewOptions.Contains(ofNoTestFileCreate)) Flags |= OFN_NOTESTFILECREATE;
  if (NewOptions.Contains(ofNoNetworkButton)) Flags |= OFN_NONETWORKBUTTON;
  if (NewOptions.Contains(ofNoLongNames)) Flags |= OFN_NOLONGNAMES;
  if (NewOptions.Contains(ofOldStyleDialog)) Flags &= ~OFN_EXPLORER;
  if (NewOptions.Contains(ofNoDereferenceLinks)) Flags |= OFN_NODEREFERENCELINKS;
  if (NewOptions.Contains(ofEnableIncludeNotify)) Flags |= OFN_ENABLEINCLUDENOTIFY;
  if (NewOptions.Contains(ofEnableSizing)) Flags |= OFN_ENABLESIZING;

  return true;
}


//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TOpenSaveDialog::GetListProperties(TStrings *ListProperties) {

  TComponent::GetListProperties(ListProperties);

  ListProperties->Add(_T("Title"));
  ListProperties->Add(_T("InitialDir"));
  ListProperties->Add(_T("FileName"));
  ListProperties->Add(_T("Filter"));
  ListProperties->Add(_T("FilterIndex"));
  ListProperties->Add(_T("DefaultExt"));
  ListProperties->Add(_T("Options"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TOpenSaveDialog::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Title")) {
    return tpText;
  }
  if (asProperty == _T("InitialDir")) {
    return tpText;
  }
  if (asProperty == _T("FileName")) {
    return tpText;
  }
  if (asProperty == _T("Filter")) {
    return tpText;
  }
  if (asProperty == _T("FilterIndex")) {
    return tpNumber;
  }
  if (asProperty == _T("DefaultExt")) {
    return tpText;
  }
  if (asProperty == _T("Options")) {
    *asInfos = _T("ofReadOnly\nofOverwritePrompt\nofHideReadOnly\nofNoChangeDir\nofShowHelp\nofNoValidate\nofAllowMultiSelect\nofExtensionDifferent\nofPathMustExist\nofFileMustExist\nofCreatePrompt\nofShareAware\nofNoReadOnlyReturn\nofNoTestFileCreate\nofNoNetworkButton\nofNoLongNames\nofOldStyleDialog\nofNoDereferenceLinks\nofEnableIncludeNotify\nofEnableSizing");
    return tpMultipleChoice;
  }
  return TComponent::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TOpenSaveDialog::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Title")) {
    return _T("");
  }
  if (asProperty == _T("InitialDir")) {
    return _T("");
  }
  if (asProperty == _T("FileName")) {
    return _T("");
  }
  if (asProperty == _T("Filter")) {
    return _T("");
  }
  if (asProperty == _T("FilterIndex")) {
    return _T("1");
  }
  if (asProperty == _T("DefaultExt")) {
    return _T("");
  }
  if (asProperty == _T("Options")) {
    return _T("");
  }
  return TComponent::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TOpenSaveDialog::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Title")) {
    return Get_Title();
  }
  if (asProperty == _T("InitialDir")) {
    return Get_InitialDir();
  }
  if (asProperty == _T("FileName")) {
    return Get_FileName();
  }
  if (asProperty == _T("Filter")) {
    return Get_Filter();
  }
  if (asProperty == _T("FilterIndex")) {
    return IntToStr(Get_FilterIndex());
  }
  if (asProperty == _T("DefaultExt")) {
    return Get_DefaultExt();
  }
  if (asProperty == _T("Options")) {
    AnsiString asOptions;
    TOpenOptions Options = Get_Options();
    if (Flags & OFN_READONLY) asOptions += _T("ofReadOnly\n");
    if (Flags & OFN_OVERWRITEPROMPT) asOptions += _T("ofOverwritePrompt\n");
    if (Flags & OFN_HIDEREADONLY) asOptions += _T("ofHideReadOnly\n");
    if (Flags & OFN_NOCHANGEDIR) asOptions += _T("ofNoChangeDir\n");
    if (Flags & OFN_SHOWHELP) asOptions += _T("ofShowHelp\n");
    if (Flags & OFN_ALLOWMULTISELECT) asOptions += _T("ofAllowMultiSelect\n");
    if (Flags & OFN_EXTENSIONDIFFERENT) asOptions += _T("ofExtensionDifferent\n");
    if (Flags & OFN_PATHMUSTEXIST) asOptions += _T("ofPathMustExist\n");
    if (Flags & OFN_FILEMUSTEXIST) asOptions += _T("ofFileMustExist\n");
    if (Flags & OFN_CREATEPROMPT) asOptions += _T("ofCreatePrompt\n");
    if (Flags & OFN_SHAREAWARE) asOptions += _T("ofShareAware\n");
    if (Flags & OFN_NOREADONLYRETURN) asOptions += _T("ofNoReadOnlyReturn\n");
    if (Flags & OFN_NOTESTFILECREATE) asOptions += _T("ofNoTestFileCreate\n");
    if (Flags & OFN_NONETWORKBUTTON) asOptions += _T("ofNoNetworkButton\n");
    if (Flags & OFN_NOLONGNAMES) asOptions += _T("ofNoLongNames\n");
    if (!(Flags & OFN_EXPLORER)) asOptions += _T("ofOldStyleDialog\n");
    if (Flags & OFN_NODEREFERENCELINKS) asOptions += _T("ofNoDereferenceLinks\n");
    if (Flags & OFN_ENABLEINCLUDENOTIFY) asOptions += _T("ofEnableIncludeNotify\n");
    if (Flags & OFN_ENABLESIZING) asOptions += _T("ofEnableSizing\n");
    return FormatProperty(asOptions, _T("["), _T("]"), _T(", "));
  }
  return TComponent::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TOpenSaveDialog::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  int Cur;
  AnsiString asMot;


  if (asProperty == _T("Title")) {
    Set_Title(asValue);
    return true;
  }
  if (asProperty == _T("InitialDir")) {
    Set_InitialDir(asValue);
    return true;
  }
  if (asProperty == _T("FileName")) {
    Set_FileName(asValue);
    return true;
  }
  if (asProperty == _T("Filter")) {
    Set_Filter(asValue);
    return true;
  }
  if (asProperty == _T("FilterIndex")) {
    Set_FilterIndex(asValue.ToIntDef(0));
    return true;
  }
  if (asProperty == _T("DefaultExt")) {
    Set_DefaultExt(asValue);
    return true;
  }
  if (asProperty == _T("Options")) {
    Flags = OFN_EXPLORER;
    Cur = 0;
    do {
      asMot = UnMot(asValue, _T("[ "), _T(", ]"), &Cur);
      if (asMot == _T("ofReadOnly")) Flags |= OFN_READONLY;
      else if (asMot == _T("ofOverwritePrompt")) Flags |= OFN_OVERWRITEPROMPT;
      else if (asMot == _T("ofHideReadOnly")) Flags |= OFN_HIDEREADONLY;
      else if (asMot == _T("ofNoChangeDir")) Flags |= OFN_NOCHANGEDIR;
      else if (asMot == _T("ofShowHelp")) Flags |= OFN_SHOWHELP;
      else if (asMot == _T("ofAllowMultiSelect")) Flags |= OFN_ALLOWMULTISELECT;
      else if (asMot == _T("ofExtensionDifferent")) Flags |= OFN_EXTENSIONDIFFERENT;
      else if (asMot == _T("ofPathMustExist")) Flags |= OFN_PATHMUSTEXIST;
      else if (asMot == _T("ofFileMustExist")) Flags |= OFN_FILEMUSTEXIST;
      else if (asMot == _T("ofCreatePrompt")) Flags |= OFN_CREATEPROMPT;
      else if (asMot == _T("ofShareAware")) Flags |= OFN_SHAREAWARE;
      else if (asMot == _T("ofNoReadOnlyReturn")) Flags |= OFN_NOREADONLYRETURN;
      else if (asMot == _T("ofNoTestFileCreate")) Flags |= OFN_NOTESTFILECREATE;
      else if (asMot == _T("ofNoNetworkButton")) Flags |= OFN_NONETWORKBUTTON;
      else if (asMot == _T("ofNoLongNames")) Flags |= OFN_NOLONGNAMES;
      else if (asMot == _T("ofOldStyleDialog")) Flags &= ~OFN_EXPLORER;
      else if (asMot == _T("ofNoDereferenceLinks")) Flags |= OFN_NODEREFERENCELINKS;
      else if (asMot == _T("ofEnableIncludeNotify")) Flags |= OFN_ENABLEINCLUDENOTIFY;
      else if (asMot == _T("ofEnableSizing")) Flags |= OFN_ENABLESIZING;
    } while (!asMot.IsEmpty());

    return true;
  }
  return TComponent::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

