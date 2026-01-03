//-----------------------------------------------------------------------------
//! @file TCppWebBrowser.h
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

#ifdef In_TCppWebBrowserH
#error "Inclusion circulaire de TCppWebBrowser"
#endif // In_TCppWebBrowserH

#define In_TCppWebBrowserH

#ifndef TCppWebBrowserH
#define TCppWebBrowserH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <Mshtml.h>
#include <mshtmhst.h>
#include <Exdisp.h>
#include <Exdispid.h>
#include "TWinControl.h"


//---------------------------------------------------------------------------
// Déclarations anticipées
//---------------------------------------------------------------------------

struct IWebBrowser2;
class TCppWebBrowser;


//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnNavigateComplete2.
//!
//! Ce type d'évènement est utilisé pour l'évènement OnNavigateComplete2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//! @param[in]        Url Url de la page atteinte.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnNavigateComplete2)(TObject *Sender, AnsiString Url);


//---------------------------------------------------------------------------
//! @brief Type d'évènement TOnBeforeNavigate2.
//!
//! Ce type d'évènement est utilisé pour l'évènement OnBeforeNavigate2.
//!
//! @param[in]        Sender Objet à l'origine de l'évènement.
//! @param[in]        Url Url de la page atteinte.
//---------------------------------------------------------------------------
typedef void (TObject::* TOnBeforeNavigate2)(TObject *Sender, IDispatch *pDisp, AnsiString Url, LONG Flags, AnsiString TargetFrameName, TStringList *PostData, AnsiString Headers);



//-----------------------------------------------------------------------------
//! @class TCppWebBrowser
//!
//! @brief Définition de la classe TCppWebBrowser
//!
//! Cette classe encapsule un navigateur WEB dans une fenêtre. Il est ensuite
//! possible de d'accéder à un site de votre choix en modifiant la propriété
//! @b LocationURL.
//!
//! @note Ce composant utilise le composant ActiveX IWebBrowser2. Par
//! conséquent, pour modifier la compatibilité par défaut (de ie7 à ie11),
//! il faut ajouter le nom de votre exécutable une des clés de registre
//! suivantes :@n
//! HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION (applications 64 bits)@n
//! HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION (32 bits)@n
//! Pour plus de précisions voir le site microsoft.com.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TCppWebBrowser: public TWinControl {
private:
  IWebBrowser2 *pWebBrowser2;
	IHTMLDocument2 *pHTMLDocument2;
	IHTMLDocument3 *pHTMLDocument3;
	IHTMLDocument6 *pHTMLDocument6;
	IHTMLElement *pHTMLBody;
protected:
  AnsiString FLocationURL;
	IConnectionPoint *pConnectionPoint;
	DWORD dwCookie; // Utilisé par Advise() et Unadvise()

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool TraitmntCommand(WORD Notify);

	void FreeDocument(void);

public:
  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TCppWebBrowser(TComponent* AOwner);

  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TCppWebBrowser(void);

  //! @name Méthodes
  //@{

	HRESULT ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt, VARIANT *pvaIn, VARIANT *pvaOut);
	HRESULT GoBack(void);
	HRESULT GoForward(void);
	HRESULT GoHome(void);

	HRESULT Refresh2(void);

	/*
	REFRESH_NORMAL
	Perform a lightweight refresh that does not include the pragma:nocache header. The pragma:nocache header tells the server not to return a cached copy. This can cause problems with some servers.

	REFRESH_IFEXPIRED
	Only refresh if the page has expired. Do not include the pragma:nocache header.

	REFRESH_COMPLETELY
	Perform a full refresh, including the pragma:nocache header. Using this option is the same as calling the Refresh method.
	*/
	HRESULT Refresh2(int Level);

	AnsiString GetValueById(AnsiString Id);
	bool SetValueById(AnsiString Id, AnsiString Value);

	bool SetFocusById(AnsiString Id);

	bool ClickById(AnsiString Id);

	AnsiString GetInnerHTML(void);
	AnsiString GetInnerText(void);

	bool ProcessEvent_OnBeforeNavigate2(IDispatch *pDisp, AnsiString Url, LONG Flags, AnsiString TargetFrameName, TStringList *PostData, AnsiString Headers);
	bool ProcessEvent_OnNavigateComplete2(AnsiString asUrl);

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfo Liste des choix (si type = tpChoice ou tpMultipleChoice)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfo);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les événement)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}

  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété LocationURL
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TCppWebBrowser, AnsiString, LocationURL);


  //---------------------------------------------------------------------------
  //! @brief Propriété Document2
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCppWebBrowser, IHTMLDocument2 *, Document2);


  //---------------------------------------------------------------------------
  //! @brief Propriété Document3
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCppWebBrowser, IHTMLDocument3 *, Document3);


  //---------------------------------------------------------------------------
  //! @brief Propriété Document6
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCppWebBrowser, IHTMLDocument6 *, Document6);


  //---------------------------------------------------------------------------
  //! @brief Propriété Boby
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TCppWebBrowser, IHTMLElement *, Body);


  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnBeforeNavigate2
	//!
	//! Cet évènement est déclenché lorsque le navigateur est sur le point de
	//! changer de page, soit suite à l'affectation de la propriété LocationURL,
	//! soit suite à une action de l'utilisateur (clic sur un lien) ou à une
	//! redirection.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 IDispatch *pDisp
  //! @param[in]        Arg3 URL de la page atteinte
  //! @param[in]        Arg4 LONG Flags
  //! @param[in]        Arg5 AnsiString TargetFrameName
  //! @param[in]        Arg6 TStringList *PostData
  //! @param[in]        Arg7 AnsiString Headers
  //---------------------------------------------------------------------------

  DECLARE_EVENT_7(TOnBeforeNavigate2, OnBeforeNavigate2, TObject *, IDispatch *, AnsiString, LONG, AnsiString, TStringList *, AnsiString);

  //---------------------------------------------------------------------------
  //! @brief Evènement OnNavigateComplete2
	//!
	//! Cet évènement est déclenché lorsque le navigateur change de page, soit
	//! suite à l'affectation de la propriété LocationURL, soit suite à une
	//! action de l'utilisateur (clic sur un lien) ou à une redirection.@n
	//! La page réellement atteinte peut être différente de la page demandée via
	//! LocationURL, à cause de la réécriture des caractères spéciaux dans les
	//! URL.
	//!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //! @param[in]        Arg2 URL de la page atteinte
  //---------------------------------------------------------------------------

  DECLARE_EVENT_2(TOnNavigateComplete2, OnNavigateComplete2, TObject *, AnsiString);

  //@}


};

// Gestion des évènements active-X de l'objet IWebBrowser2
class TCppWebBrowserEvents : public DWebBrowserEvents2 {
public:
	// Méthodes IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	// Méthodes IDispatch
	STDMETHODIMP GetTypeInfoCount(UINT *pctinfo);
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

	TCppWebBrowserEvents(void);
protected:
	LONG NbRef;
};

// Instance globale unique
extern TCppWebBrowserEvents *CppWebBrowserEvents;

#else  // TCppWebBrowserH

class TCppWebBrowser;
class TCppWebBrowserEvents;

#endif  // TCppWebBrowserH


#undef In_TCppWebBrowserH
