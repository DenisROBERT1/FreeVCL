//-----------------------------------------------------------------------------
//! @file TCppWebBrowser.cpp
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
#include <ole2.h>
#include <exdisp.h>
#include <TCppWebBrowser.h>


//---------------------------------------------------------------------------
// VARIABLES GLOBALES
//---------------------------------------------------------------------------

TCppWebBrowserEvents *CppWebBrowserEvents;

/*

// MIDL_INTERFACE("D30C1661-CDAF-11D0-8A3E-00C04FC9E26E") IID_IWebBrowser2
const IID IID_IWebBrowser2 = {0xD30C1661, 0xCDAF, 0x11D0, {0x8A, 0x3E, 0x00, 0xC0, 0x4F, 0xC9, 0xE2, 0x6E}};

// MIDL_INTERFACE("8856F961-340A-11D0-A96B-00C04FD705A2") CLSID_WebBrowser
const IID CLSID_WebBrowser = {0x8856F961, 0x340A, 0x11D0, {0xA9, 0x6B, 0x00, 0xC0, 0x4F, 0xD7, 0x05, 0xA2}};

// MIDL_INTERFACE{"332C4425-26CB-11D0-B483-00C04FD90119") IID_IHTMLDocument2
const IID IID_IHTMLDocument = {0x626fc520, 0xa41e, 0x11cf, {0xa7, 0x31, 0x0, 0xa0, 0xc9, 0x8, 0x26, 0x37}};
const IID IID_IHTMLDocument2 = {0x332C4425, 0x26CB, 0x11D0, {0xB4, 0x83, 0x00, 0xC0, 0x4F, 0xD9, 0x01, 0x19}};
const IID IID_IHTMLDocument3 = {0x3050f485, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};

// MIDL_INTERFACE{"332C4425-26CB-11D0-B483-00C04FD90119") IID_IHTMLBody
const IID IID_IHTMLBody = {0x332C4425, 0x26CB, 0x11D0, {0xB4, 0x83, 0x00, 0xC0, 0x4F, 0xD9, 0x01, 0x19}};

const IID IID_IHTMLDocument = {0x626fc520, 0xa41e, 0x11cf, {0xa7, 0x31, 0x0, 0xa0, 0xc9, 0x8, 0x26, 0x37}};
const IID IID_IHTMLDocument2 = {0x332c4425, 0x26cb, 0x11d0, {0xb4, 0x83, 0x0, 0xc0, 0x4f, 0xd9, 0x1, 0x19}};
const IID IID_IHTMLElement = {0x3050f1ff, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0xb}};
const IID IID_IHTMLSelectionObject = {0x3050f25a, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0xb}};
const IID IID_IHTMLTxtRange = {0x3050f220, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLImgElement = {0x3050f240, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBodyElement = {0x3050f1d8, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFontElement = {0x3050f1d9, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLAnchorElement = {0x3050f1da, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLUListElement = {0x3050f1dd, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLOListElement = {0x3050f1de, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLLIElement = {0x3050f1e0, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBRElement = {0x3050f1f0, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDListElement = {0x3050f1f1, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDDElement = {0x3050f1f2, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDTElement = {0x3050f1f3, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLHRElement = {0x3050f1f4, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLParaElement = {0x3050f1f5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLHeaderElement = {0x3050f1f6, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFormElement = {0x3050f1f7, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDivElement = {0x3050f200, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBaseFontElement = {0x3050f202, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLMetaElement = {0x3050f203, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBaseElement = {0x3050f204, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLLinkElement = {0x3050f205, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLIsIndexElement = {0x3050f206, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLNextIdElement = {0x3050f207, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBlockElement = {0x3050f208, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLUnknownElement = {0x3050f209, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLPhraseElement = {0x3050f20a, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLCommentElement = {0x3050f20c, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLListElement = {0x3050f20e, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLOptionElement = {0x3050f211, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDivPosition = {0x3050f212, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDialog = {0x3050f216, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTextElement = {0x3050f218, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTable = {0x3050f21e, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLElementCollection = {0x3050f21f, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTextContainer = {0x3050f230, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableCol = {0x3050f23a, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableSection = {0x3050f23b, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableRow = {0x3050f23c, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableCell = {0x3050f23d, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLSelectElement = {0x3050f244, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLObjectElement = {0x3050f24f, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyle = {0x3050f25e, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLEmbedElement = {0x3050f25f, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLAreaElement = {0x3050f265, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLMapElement = {0x3050f266, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLScriptElement = {0x3050f28b, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLControlRange = {0x3050f29c, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLInputHiddenElement = {0x3050f2a4, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLInputTextElement = {0x3050f2a6, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTextAreaElement = {0x3050f2aa, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLInputFileElement = {0x3050f2ad, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLInputButtonElement = {0x3050f2b2, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLMarqueeElement = {0x3050f2b5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLButtonElement = {0x3050f2bb, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLOptionButtonElement = {0x3050f2bc, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLInputImage = {0x3050f2c2, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyleSheet = {0x3050f2e3, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyleSheetRulesCollection = {0x3050f2e5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableCaption = {0x3050f2eb, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFrameBase = {0x3050f311, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFrameElement = {0x3050f313, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLIFrameElement = {0x3050f315, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFrameSetElement = {0x3050f319, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTitleElement = {0x3050f322, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLLabelElement = {0x3050f32a, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLEventObj = {0x3050f32d, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyleSheetRule = {0x3050f357, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLScreen = {0x3050f35c, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBGsound = {0x3050f369, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyleElement = {0x3050f375, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFontNamesCollection = {0x3050f376, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFontSizesCollection = {0x3050f377, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLOptionsHolder = {0x3050f378, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyleSheetsCollection = {0x3050f37e, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLAreasCollection = {0x3050f383, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLNoShowElement = {0x3050f38a, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLOptionElementFactory = {0x3050f38c, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLImageElementFactory = {0x3050f38e, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLRuleStyle = {0x3050f3cf, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyleFontFace = {0x3050f3d5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLCurrentStyle = {0x3050f3db, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLSpanFlow = {0x3050f3e5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFieldSetElement = {0x3050f3e7, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLLegendElement = {0x3050f3ea, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFiltersCollection = {0x3050f3ee, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDatabinding = {0x3050f3f2, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLSpanElement = {0x3050f3f3, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLMimeTypesCollection = {0x3050f3fc, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLPluginsCollection = {0x3050f3fd, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLOpsProfile = {0x3050f401, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTextRangeMetrics = {0x3050f40b, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableRowMetrics = {0x3050f413, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLElement2 = {0x3050f434, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDocument3 = {0x3050f485, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLEventObj2 = {0x3050f48b, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLUserDataOM = {0x3050f48f, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableRow2 = {0x3050f4a1, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLStyle2 = {0x3050f4a2, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLRect = {0x3050f4a3, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLRectCollection = {0x3050f4a4, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTextRangeMetrics2 = {0x3050f4a6, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLRuleStyle2 = {0x3050f4ac, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTable2 = {0x3050f4ad, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLWindow3 = {0x3050f4ae, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDOMAttribute = {0x3050f4b0, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDOMTextNode = {0x3050f4b1, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDataTransfer = {0x3050f4b3, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLGenericElement = {0x3050f4b7, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLPersistDataOM = {0x3050f4c0, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLAttributeCollection = {0x3050f4c3, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLPersistData = {0x3050f4c5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLObjectElement2 = {0x3050f4cd, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBookmarkCollection = {0x3050f4ce, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLUniqueName = {0x3050f4d0, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLLinkElement2 = {0x3050f4e5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLIFrameElement2 = {0x3050f4e6, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLControlElement = {0x3050f4e9, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFormElement2 = {0x3050f4f6, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDOMChildrenCollection = {0x3050f5ab, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLBodyElement2 = {0x3050f5c5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFrameSetElement2 = {0x3050f5c6, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLTableSection2 = {0x3050f5c7, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLAppBehavior2 = {0x3050f5c9, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLAppBehavior = {0x3050f5ca, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLInputElement = {0x3050f5d2, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDOMNode = {0x3050f5da, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDialog2 = {0x3050f5e0, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLUrnCollection = {0x3050f5e2, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLModelessInit = {0x3050f5e4, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLDocumentFragment = {0x3050f5e5, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLAreasCollection2 = {0x3050f5ec, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLSelectElement2 = {0x3050f5ed, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLElementCollection2 = {0x3050f5ee, 0x98b5, 0x11cf, {0xbb, 0x82, 0x0, 0xaa, 0x0, 0xbd, 0xce, 0x0b}};
const IID IID_IHTMLFramesCollection2 = {0x332c4426, 0x26cb, 0x11d0, {0xb4, 0x83, 0x0, 0xc0, 0x4f, 0xd9, 0x01, 0x19}};
const IID IID_IHTMLWindow2 = {0x332c4427, 0x26cb, 0x11d0, {0xb4, 0x83, 0x0, 0xc0, 0x4f, 0xd9, 0x01, 0x19}};
const IID IID_IHTMLLocation = {0x163bb1e0, 0x6e00, 0x11cf, {0x83, 0x7a, 0x48, 0xdc, 0x04, 0xc1, 0x0, 0x0}};
*/

//---------------------------------------------------------------------------
// TCppWebBrowser
//---------------------------------------------------------------------------

TCppWebBrowser::TCppWebBrowser(TComponent* AOwner): TWinControl(AOwner) {
	HFONT hFont;
	HWND hWndParent = NULL;
	HRESULT hr;
	IConnectionPointContainer* pConnectionPointContainer;


	// Initialisations
	FClassName = _T("TCppWebBrowser");
	if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;
	FExWindowStyle = WS_EX_CLIENTEDGE;
	pWebBrowser2 = NULL;
	pHTMLDocument2 = NULL;
	pHTMLDocument3 = NULL;
	pHTMLDocument6 = NULL;
	pHTMLBody = NULL;
	pConnectionPoint = NULL;

	HINSTANCE hDLL = LoadLibrary(_T("atl.dll"));
	// Définir le type de pointeur pour la fonction "AtlAxAttachControl":
	typedef HRESULT (WINAPI *PAttachControl)(IUnknown*, HWND, IUnknown**);
	// Récupérer l'adresse de la fonction "AtlAxAttachControl":
	PAttachControl AtlAxAttachControl = (PAttachControl) (void *) GetProcAddress(hDLL, "AtlAxAttachControl");

	// Création de la boîte
	FHandle = CreateWindowEx(FExWindowStyle,
												 WC_STATIC, FCaption,
												 FWindowStyle,
												 FLeft, FTop,
												 FWidth, FHeight,
												 hWndParent,
												 (HMENU) (UINT_PTR) FCommand, ::HInstance, this);

	if (FHandle) {
		hFont = FFont->Handle;
		SendMessage(FHandle, WM_SETFONT, (WPARAM) hFont, 0);

		InitStdControl();

		// Initialiser la librairie COM pour notre programme:
		::CoInitialize(NULL);
		// Créer une instance de l'objet WebBrowser et de l'interface IWebBrowser2:
		CoCreateInstance(CLSID_WebBrowser, 0, CLSCTX_ALL, IID_IWebBrowser2, (void**) &pWebBrowser2);
		// Attacher l'objet WebBrowser à notre conteneur:
		AtlAxAttachControl(pWebBrowser2, FHandle, 0);
		// Objet de traitement des évènements
		if (CppWebBrowserEvents == NULL) CppWebBrowserEvents = new TCppWebBrowserEvents;
		CppWebBrowserEvents->AddRef();
		BSTR bstrPropertyName = SysAllocString(L"CppWebBrowser_Ptr");
		VARIANT vPropertyValue;
		VariantInit(&vPropertyValue);
		vPropertyValue.vt = VT_I8;
		vPropertyValue.intVal = (INT) (__int64) this;
		pWebBrowser2->PutProperty(bstrPropertyName, vPropertyValue);
		SysFreeString(bstrPropertyName);
		VariantClear(&vPropertyValue);
		// Initialisation des évènements
		// Get an IConnectionPointContainer interface pointer from the site
		pConnectionPointContainer = NULL;
		hr = pWebBrowser2->QueryInterface(IID_IConnectionPointContainer, (void**) &pConnectionPointContainer);
		if (hr == S_OK) {
			// Now we use the IConnectionPointContainer interface to get an IConnectionPoint interface pointer that will handle DWebBrowserEvents2 "dispatch interface" events.
			// That means we have to plug our implementation of DWebBrowserEvents2 into the returned IConnectionPoint interface using its Advise() method, as below
			pConnectionPoint = NULL;
			hr = pConnectionPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &pConnectionPoint);
			if (hr == S_OK) {
				// Finally we can plug our event handler object CppWebBrowserEvents into the connection point and start receiving IE events
				// The advise cookie is just a return value we use when we want to "unplug" our event handler object from the connection point
				dwCookie = 0;
				pConnectionPoint->Advise((IUnknown*) CppWebBrowserEvents, &dwCookie);
			}
		}
		pConnectionPointContainer->Release();
		// Page blanche
		Set_LocationURL(_T("about:blank"));
	}

}

//---------------------------------------------------------------------------
TCppWebBrowser::~TCppWebBrowser(void) {
	FreeDocument();
	if (pWebBrowser2) {
		// Libérer l'interface IWebBrowser2:
		pWebBrowser2->Quit();
		pWebBrowser2->Release();
	}
	if (pConnectionPoint) {
		pConnectionPoint->Unadvise(dwCookie);
		dwCookie=0;
		pConnectionPoint->Release();
		pConnectionPoint=NULL;
	}
	if (CppWebBrowserEvents) {
		if (CppWebBrowserEvents->Release() == 0) CppWebBrowserEvents = NULL;
	}

}

//---------------------------------------------------------------------------
bool TCppWebBrowser::TraitmntCommand(WORD Notify) {
	if (Notify == EN_CHANGE) {
		if (!FComponentState.Contains(csLoading)) OnChange(this);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
void TCppWebBrowser::FreeDocument(void) {

	if (pHTMLDocument2) {
		pHTMLDocument2->Release();
		pHTMLDocument2 = NULL;
	}
	if (pHTMLDocument3) {
		pHTMLDocument3->Release();
		pHTMLDocument3 = NULL;
	}
	if (pHTMLDocument6) {
		pHTMLDocument6->Release();
		pHTMLDocument6 = NULL;
	}
	if (pHTMLBody) {
		pHTMLBody->Release();
		pHTMLBody = NULL;
	}

}

//---------------------------------------------------------------------------
// Accesseurs de la propriété LocationURL
//---------------------------------------------------------------------------

AnsiString TCppWebBrowser::Get_LocationURL(void) {
	return FLocationURL;
}

bool TCppWebBrowser::Set_LocationURL(AnsiString NewLocationURL) {
	if (FLocationURL != NewLocationURL) {
		HRESULT hr;
		READYSTATE state;
		FLocationURL = NewLocationURL;
		VARIANT vEmpty;
		VariantInit(&vEmpty);
		int BufSize = NewLocationURL.WideCharBufSize();
		wchar_t *wstrURL = new wchar_t[BufSize];
		NewLocationURL.WideChar(wstrURL, BufSize);
		BSTR bstrURL = SysAllocString(wstrURL);

		hr = pWebBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
		//  Wait for the load.
		if (hr == S_OK) {
			do {
				hr = pWebBrowser2->get_ReadyState(&state);
			} while (SUCCEEDED(hr) && hr != S_OK); // Attention : SUCCEEDED(S_FALSE) == true
		}
		FLastError = hr;
		delete [] wstrURL;
		SysFreeString(bstrURL);
	}
	if (!FComponentState.Contains(csLoading)) OnChange(this);

	return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Document2
//---------------------------------------------------------------------------

IHTMLDocument2 *TCppWebBrowser::Get_Document2(void) {
	IDispatch *pDispatch;
	HRESULT hr;

	if (pWebBrowser2 != NULL && pHTMLDocument2 == NULL) {
		hr = pWebBrowser2->get_Document(&pDispatch);
		if (hr == S_OK) {
			pDispatch->QueryInterface(IID_IHTMLDocument2, (void**) &pHTMLDocument2);
		}
	}

	return pHTMLDocument2;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Document3
//---------------------------------------------------------------------------

IHTMLDocument3 *TCppWebBrowser::Get_Document3(void) {
	IDispatch *pDispatch;
	HRESULT hr;

	if (pWebBrowser2 != NULL && pHTMLDocument3 == NULL) {
		hr = pWebBrowser2->get_Document(&pDispatch);
		if (hr == S_OK) {
			pDispatch->QueryInterface(IID_IHTMLDocument3, (void**) &pHTMLDocument3);
		}
	}

	return pHTMLDocument3;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Document6
//---------------------------------------------------------------------------

IHTMLDocument6 *TCppWebBrowser::Get_Document6(void) {
	IDispatch *pDispatch;
	HRESULT hr;

	if (pWebBrowser2 != NULL && pHTMLDocument6 == NULL) {
		hr = pWebBrowser2->get_Document(&pDispatch);
		if (hr == S_OK) {
			pDispatch->QueryInterface(IID_IHTMLDocument6, (void**) &pHTMLDocument6);
		}
	}

	return pHTMLDocument6;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Body
//---------------------------------------------------------------------------

IHTMLElement *TCppWebBrowser::Get_Body(void) {

	Get_Document2();
	if (pHTMLDocument2 != NULL && pHTMLBody == NULL) {
		pHTMLDocument2->get_body(&pHTMLBody);
	}

	return pHTMLBody;
}

//---------------------------------------------------------------------------

HRESULT TCppWebBrowser::ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt, VARIANT *pvaIn, VARIANT *pvaOut) {
	return pWebBrowser2->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
}

//---------------------------------------------------------------------------

HRESULT TCppWebBrowser::GoBack(void) {
	return pWebBrowser2->GoBack();
}

//---------------------------------------------------------------------------

HRESULT TCppWebBrowser::GoForward(void) {
	return pWebBrowser2->GoForward();
}

//---------------------------------------------------------------------------

HRESULT TCppWebBrowser::GoHome(void) {
	return pWebBrowser2->GoHome();
}

//---------------------------------------------------------------------------

HRESULT TCppWebBrowser::Refresh2(void) {
	return pWebBrowser2->Refresh();
}

//---------------------------------------------------------------------------

HRESULT TCppWebBrowser::Refresh2(int Level) {
 VARIANT vLevel;
 HRESULT hr;
 VariantInit(&vLevel);
 vLevel.vt = VT_I4;
 vLevel.intVal = Level;
 hr = pWebBrowser2->Refresh2(&vLevel);
 VariantClear(&vLevel);

 return hr;
}

//---------------------------------------------------------------------------
AnsiString TCppWebBrowser::GetValueById(AnsiString Id) {
	AnsiString Value;
	IHTMLElement *Element = NULL;
	BSTR bstrId = SysAllocString(Id.AsWideString().c_str());
	BSTR bstrValue = NULL;
	Document3->getElementById(bstrId, &Element);  // Pas pHTMLDocument3 car il y a un traitement dans le Get
	if (Element) {
		Element->get_innerText(&bstrValue);
		if (bstrValue) Value = bstrValue;
	}
	SysFreeString(bstrId);
	SysFreeString(bstrValue);

	return Value;
}

//---------------------------------------------------------------------------
bool TCppWebBrowser::SetValueById(AnsiString Id, AnsiString Value) {
	IHTMLElement *Element = NULL;
	bool Ok = false;
	BSTR bstrId = SysAllocString(Id.AsWideString().c_str());
	BSTR bstrValue = SysAllocString(Value.AsWideString().c_str());
	Document3->getElementById(bstrId, &Element);
	if (Element) {
		Element->put_innerText(bstrValue);
		Ok = true;
	}
	SysFreeString(bstrId);
	SysFreeString(bstrValue);

	return Ok;
}

//---------------------------------------------------------------------------
bool TCppWebBrowser::SetFocusById(AnsiString Id) {
	AnsiString Value;
	IHTMLElement2 *Element = NULL;

	BSTR bstrId = SysAllocString(Id.AsWideString().c_str());
	Document6->getElementById(bstrId, &Element);  // Pas pHTMLDocument3 car il y a un traitement dans le Get
	if (Element) Element->focus();
	SysFreeString(bstrId);

	return true;
}

//---------------------------------------------------------------------------
bool TCppWebBrowser::ClickById(AnsiString Id) {
	AnsiString Value;
	IHTMLElement *Element = NULL;

	BSTR bstrId = SysAllocString(Id.AsWideString().c_str());
	Document3->getElementById(bstrId, &Element);  // Pas pHTMLDocument3 car il y a un traitement dans le Get
	if (Element) Element->click();
	SysFreeString(bstrId);

	return true;
}

//---------------------------------------------------------------------------
AnsiString TCppWebBrowser::GetInnerHTML(void) {
	HRESULT hr;
	BSTR bstrInnerHTML = NULL;
	AnsiString asInnerHTML;

	Get_Body();
	if (pHTMLBody != NULL) {
		hr = pHTMLBody->get_innerHTML(&bstrInnerHTML);
		if (hr == S_OK) {
			if (bstrInnerHTML) asInnerHTML = bstrInnerHTML;
		}
		FLastError = hr;
		SysFreeString(bstrInnerHTML);
	}

	return asInnerHTML;
}

//---------------------------------------------------------------------------
AnsiString TCppWebBrowser::GetInnerText(void) {
	HRESULT hr;
	BSTR bstrInnerText = NULL;
	AnsiString asInnerText;

	hr = Body->get_innerText(&bstrInnerText);
	if (hr == S_OK) {
		if (bstrInnerText) asInnerText = bstrInnerText;
	}
	FLastError = hr;
	SysFreeString(bstrInnerText);

	return asInnerText;
}

//---------------------------------------------------------------------------
bool TCppWebBrowser::ProcessEvent_OnBeforeNavigate2(IDispatch *pDisp, AnsiString Url, LONG Flags, AnsiString TargetFrameName, TStringList *PostData, AnsiString Headers) {
	OnBeforeNavigate2(this, pDisp, Url, Flags, TargetFrameName, PostData, Headers);

	return true;
}

//---------------------------------------------------------------------------
bool TCppWebBrowser::ProcessEvent_OnNavigateComplete2(AnsiString asUrl) {

	FreeDocument();
	FLocationURL = asUrl;
	OnNavigateComplete2(this, asUrl);

	return true;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TCppWebBrowser::GetListProperties(TStrings *ListProperties) {

	TWinControl::GetListProperties(ListProperties);

	ListProperties->Add("LocationURL");
	ListProperties->Add("OnChange");
	ListProperties->Add("OnBeforeNavigate2");
	ListProperties->Add("OnNavigateComplete2");
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TCppWebBrowser::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
	if (asProperty == _T("LocationURL")) {
		return tpText;
	}
	if (asProperty == _T("OnChange")) {
		*asInfos = _T("(TObject *Sender)");
		return tpEvent;
	}
	if (asProperty == _T("OnBeforeNavigate2")) {
		*asInfos = _T("(TObject *Sender, IDispatch *pDisp, AnsiString Url, LONG Flags, AnsiString TargetFrameName, TStringList *PostData, AnsiString Headers)");
		return tpEvent;
	}
	if (asProperty == _T("OnNavigateComplete2")) {
		*asInfos = _T("(TObject *Sender, AnsiString Url)");
		return tpEvent;
	}
	return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TCppWebBrowser::GetDefaultProperty(AnsiString asProperty) {
	if (asProperty == _T("LocationURL")) {
		return _T("");
	}
	if (asProperty == _T("OnChange")) {
		return _T("");
	}
	if (asProperty == _T("OnBeforeNavigate2")) {
		return _T("");
	}
	if (asProperty == _T("OnNavigateComplete2")) {
		return _T("");
	}
	return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TCppWebBrowser::GetProperty(AnsiString asProperty) {
	if (asProperty == _T("LocationURL")) {
		return Get_LocationURL();
	}
	if (asProperty == _T("OnChange")) {
		return OnChange.AsString();
	}
	if (asProperty == _T("OnBeforeNavigate2")) {
		return OnBeforeNavigate2.AsString();
	}
	if (asProperty == _T("OnNavigateComplete2")) {
		return OnNavigateComplete2.AsString();
	}
	return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TCppWebBrowser::SetProperty(AnsiString asProperty, AnsiString asValue,
															TPersistent *Sender) {
	if (asProperty == _T("LocationURL")) {
		Set_LocationURL(asValue);
		return true;
	}
	if (asProperty == _T("OnChange")) {
		OnChange.ObjCall = Sender;
		OnChange = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnBeforeNavigate2")) {
		OnBeforeNavigate2.ObjCall = Sender;
		OnBeforeNavigate2 = asValue.c_str();
		return true;
	}
	if (asProperty == _T("OnNavigateComplete2")) {
		OnNavigateComplete2.ObjCall = Sender;
		OnNavigateComplete2 = asValue.c_str();
		return true;
	}
	return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------

TCppWebBrowserEvents::TCppWebBrowserEvents(void): DWebBrowserEvents2() {
	NbRef = 0;
}

STDMETHODIMP TCppWebBrowserEvents::QueryInterface(REFIID riid, void **ppvObject) {
	// Check if ppvObject is a valid pointer
	if (IsBadWritePtr(ppvObject, sizeof(void*))) return E_POINTER;
	// Set *ppvObject to NULL
	(*ppvObject) = NULL;
	// See if the requested IID matches one that we support
	// If it doesn't return E_NOINTERFACE
	if(!IsEqualIID(riid, IID_IUnknown) && !IsEqualIID(riid, IID_IDispatch) && !IsEqualIID(riid, DIID_DWebBrowserEvents2)) return E_NOINTERFACE;
	// If it's a matching IID, set *ppvObject to point to the global CppWebBrowserEvents object
	(*ppvObject) = (void*) this;
	AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) TCppWebBrowserEvents::AddRef() {
	InterlockedIncrement(&NbRef);
	return NbRef;
}

STDMETHODIMP_(ULONG) TCppWebBrowserEvents::Release() {
	// Decrement the object's internal counter.
	LONG ulRefCount = InterlockedDecrement(&NbRef);
	if (0 == NbRef) delete this;

	return ulRefCount;
}

// We don't need to implement the next three methods because we are just a pure event sink
// We only care about Invoke() which is what IE calls to notify us of events

STDMETHODIMP TCppWebBrowserEvents::GetTypeInfoCount(UINT *pctinfo)
{
	UNREFERENCED_PARAMETER(pctinfo);

	return E_NOTIMPL;
}

STDMETHODIMP TCppWebBrowserEvents::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	UNREFERENCED_PARAMETER(iTInfo);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(ppTInfo);

	return E_NOTIMPL;
}

STDMETHODIMP TCppWebBrowserEvents::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
#ifndef __GNUG__
	UNREFERENCED_PARAMETER(riid);
#endif
	UNREFERENCED_PARAMETER(rgszNames);
	UNREFERENCED_PARAMETER(cNames);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(rgDispId);

	return E_NOTIMPL;
}

// This is called by IE to notify us of events
// Full documentation about all the events supported by DWebBrowserEvents2 can be found at
//  http://msdn.microsoft.com/en-us/library/aa768283(VS.85).aspx
STDMETHODIMP TCppWebBrowserEvents::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(wFlags);
	UNREFERENCED_PARAMETER(pVarResult);
	UNREFERENCED_PARAMETER(pExcepInfo);
	UNREFERENCED_PARAMETER(puArgErr);
	int n;
	bool Ok = false;
	PVOID pv;
	LONG lbound, ubound, sz;

	if (!IsEqualIID(riid, IID_NULL)) return DISP_E_UNKNOWNINTERFACE; // riid should always be IID_NULL

	// Traitement de l'évènement BeforeNavigate2
	if (dispIdMember == DISPID_BEFORENAVIGATE2) {
		VARIANT v[5];
		// Initialize the variants
		for (n = 0; n < 5; n++) VariantInit(&v[n]);

		VariantChangeType(&v[0], &pDispParams->rgvarg[5], 0, VT_BSTR); // url
		VariantChangeType(&v[1], &pDispParams->rgvarg[4], 0, VT_I4); // Flags
		VariantChangeType(&v[2], &pDispParams->rgvarg[3], 0, VT_BSTR); // TargetFrameName
		VariantChangeType(&v[3], &pDispParams->rgvarg[2], 0, VT_UI1 | VT_ARRAY); // PostData
		VariantChangeType(&v[4], &pDispParams->rgvarg[1], 0, VT_BSTR); // Headers
		if (v[3].vt != VT_EMPTY) {
			SafeArrayGetLBound(v[3].parray, 0, &lbound);
			SafeArrayGetUBound(v[3].parray, 0, &ubound);
			sz = ubound - lbound + 1;
			SafeArrayAccessData(v[3].parray, &pv);
		}
		else {
			sz = 0;
			pv = NULL;
		}
		Ok = true;
		if (v[3].vt != VT_EMPTY) SafeArrayUnaccessData(v[3].parray);
		if (Ok) *(pDispParams->rgvarg[0].pboolVal) = VARIANT_FALSE;  // i.e. Cancel = FALSE;
		else *(pDispParams->rgvarg[0].pboolVal) = VARIANT_TRUE;

		// Free the variants
		for (n = 0; n < 5; n++) VariantClear(&v[n]);
	}
	// Traitement de l'évènement NavigateComplete2
	else if (dispIdMember == DISPID_NAVIGATECOMPLETE2) {
		VARIANT vpDisp;
		VARIANT vUrl;
		IWebBrowser2 *pWebBrowser2;
		TCppWebBrowser *CppWebBrowser;
		// Initialisation des variants
		VariantInit(&vpDisp);
		VariantChangeType(&vpDisp, &pDispParams->rgvarg[1], 0, VT_DISPATCH);
		VariantInit(&vUrl);
		VariantChangeType(&vUrl, &pDispParams->rgvarg[0], 0, VT_BSTR);

		pWebBrowser2 = (IWebBrowser2 *) vpDisp.pdispVal;
		BSTR bstrPropertyName = SysAllocString(L"CppWebBrowser_Ptr");
		VARIANT vPropertyValue;
		VariantInit(&vPropertyValue);
		HRESULT hr = pWebBrowser2->GetProperty(bstrPropertyName, &vPropertyValue);
		if (hr == S_OK) {
			CppWebBrowser = dynamic_cast<TCppWebBrowser *>((TObject *) (size_t) vPropertyValue.intVal);
			if (CppWebBrowser) {
				CppWebBrowser->ProcessEvent_OnNavigateComplete2((LPOLESTR) vUrl.bstrVal);
			}
		}
		VariantClear(&vPropertyValue);
		SysFreeString(bstrPropertyName);

		// Free des variants
		VariantClear(&vpDisp);
		VariantClear(&vUrl);
	}

	return S_OK;
}
