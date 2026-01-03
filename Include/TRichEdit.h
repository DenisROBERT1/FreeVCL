//-----------------------------------------------------------------------------
//! @file TRichEdit.h
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

#ifdef In_TRichEditH
#error "Inclusion circulaire de TRichEdit"
#endif // In_TRichEditH

#define In_TRichEditH

#ifndef TRichEditH
#define TRichEditH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#ifndef TWinControlH
#include "TWinControl.h"
#endif // TWinControlH

#include "TTextAttributes.h"
#include "TParaAttributes.h"

#include <RichEdit.h>
#include <RichOle.h>
#include "ImageDataObject.h"


//-----------------------------------------------------------------------------
//! @class TRichEdit
//!
//! @brief Définition de la classe TRichEdit
//!
//! Cette classe est une classe VCL: voir doc correspondante en cliquant sur le
//! @htmlonly <a href="bcb6vcl.hlp">ce lien</a> @endhtmlonly
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TRichEdit: public TWinControl {
private:
  HINSTANCE hDllRichEdit;
	static LONG IndexBuf;
	static LONG SizeBuf;

protected:
  TTextAttributes* FDefAttributes;
  TTextAttributes* FSelAttributes;
  TParaAttributes* FParaAttributes;
  TStringList *FLines;
  bool bNoChange;
  TBorderStyle FBorderStyle;
	bool FHideScrollBars;
	int NbBeginUpdate;

  //---------------------------------------------------------------------------
  //! @brief Traitement des messages Windows
  //---------------------------------------------------------------------------

  virtual bool ProcessCommand(WORD Notify);

  void ReCreateWindow(void);

  //---------------------------------------------------------------------------
  // Interface COM pour affichage des images
  //---------------------------------------------------------------------------

	class IExRichEditOleCallback;
  // forward declaration (see below in this header file)

  IExRichEditOleCallback* m_pIRichEditOleCallback;
  BOOL m_bCallbackSet;


  class IExRichEditOleCallback : public IRichEditOleCallback
  {
  public:
		IExRichEditOleCallback();
		virtual ~IExRichEditOleCallback();
    int m_iNumStorages;
    IStorage* pStorage;
    DWORD m_dwRef;

    virtual HRESULT STDMETHODCALLTYPE GetNewStorage(LPSTORAGE* lplpstg);
    virtual HRESULT STDMETHODCALLTYPE
            QueryInterface(REFIID iid, void ** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    virtual HRESULT STDMETHODCALLTYPE
            GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
            LPOLEINPLACEUIWINDOW FAR *lplpDoc,
            LPOLEINPLACEFRAMEINFO lpFrameInfo);
    virtual HRESULT STDMETHODCALLTYPE ShowContainerUI(BOOL fShow);
    virtual HRESULT STDMETHODCALLTYPE
            QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp);
    virtual HRESULT STDMETHODCALLTYPE DeleteObject(LPOLEOBJECT lpoleobj);
    virtual HRESULT STDMETHODCALLTYPE
            QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
            DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
    virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
    virtual HRESULT STDMETHODCALLTYPE
            GetClipboardData(CHARRANGE FAR *lpchrg,
            DWORD reco, LPDATAOBJECT FAR *lplpdataobj);
    virtual HRESULT STDMETHODCALLTYPE
            GetDragDropEffect(BOOL fDrag,
            DWORD grfKeyState, LPDWORD pdwEffect);
    virtual HRESULT STDMETHODCALLTYPE
            GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj,
            CHARRANGE FAR *lpchrg, HMENU FAR *lphmenu);
  };

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TRichEdit(TComponent* AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TRichEdit();

  //@}


  //! @name Méthodes
  //@{

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
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

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
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);



  FVCL_BEGIN_EVENTS
  void StringListChange(TObject* Sender);

  virtual void SetEvent(TEvent *Event, const TCHAR *EventCall) {
		SET_EVENT_STR_1(TRichEdit, TNotifyEvent, OnChange, StringListChange, TObject*);
  }

  FVCL_END_EVENTS

  static DWORD CALLBACK ReadStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
  static DWORD CALLBACK WriteStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


  //---------------------------------------------------------------------------
  //! @brief Début de mise à jour
  //!
  //! Cette méthode permet de modifier la liste sans provoquer une mise à jour
	//! de l'affichage (lorsqu'il y en a un). Ceci permet une meilleure rapidité
	//! lorsqu'il y a un grand nombre de modifications à effectuer. Appelez la
	//! méthode @b EndUpdate en fin de modifications. L'objet comptabilise le
	//! nombre d'appels à @b BeginUpdate, ce qui signifie que vous pouvez appeler
	//! plusieurs fois de suite cette méthode, l'important étant d'appeler la
	//! méthode @b EndUpdate autant de fois. Utilisez la propriété @b UpdateCount
	//! pour connaître le nombre de fois où @b BeginUpdate a été appelé sans
	//! @b EndUpdate.
  //!
	//! @sa EndUpdate UpdateCount
  //---------------------------------------------------------------------------
  void FASTCALL BeginUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Fin de mise à jour
  //!
  //! Appelez cette méthode après un @b BeginUpdate pour indiquer que les mises
	//! à jour sont terminées. Lorsque la méthode @b EndUpdate est appelée, le
	//! système raffraichit l'affichage (lorsqu'il y en a un). S'il y a plusieurs
	//! @b BeginUpdate imbriqués, l'affichage n'est mis à jour que sur le dernier
	//! appel à @b EndUpdate (c'est à dire lorsque la propriété @b UpdateCount
	//! redevient égale à zéro). C'est pour cela qu'il faut appeler @b EndUpdate
	//! exactement autant de fois que @b BeginUpdate.
  //!
	//! @sa EndUpdate UpdateCount
  //---------------------------------------------------------------------------
  void FASTCALL EndUpdate(void);

  //---------------------------------------------------------------------------
  //! @brief Effacement complet du contrôle
  //!
  //! Cette méthode met à zéro la liste.
  //---------------------------------------------------------------------------
  void FASTCALL Clear(void);

  //---------------------------------------------------------------------------
  //! @brief Lecture depuis un fichier
  //!
  //! Cette méthode charge en mémoire le contenu d'un fichier.
  //!
  //! @param[in]        FileName Nom du fichier à lire
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
  bool FASTCALL LoadFromFile(const AnsiString FileName);

  //---------------------------------------------------------------------------
  //! @brief Lecture depuis un buffer
  //!
  //! Cette méthode charge en mémoire le contenu d'un buffer au format RTF
	//! (c'est à dire avec les balises).
  //!
  //! @param[in]        szBuf Buffer à lire
  //! @param[in]        Size Taille du buffer
  //!
  //! @return @c bool true si Ok.
  //---------------------------------------------------------------------------
	bool FASTCALL LoadRTF(const char *szBuf, DWORD Size);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'une image dans le RichEdit
  //!
  //! Cette méthode insère une image à la position du curseur.
  //!
  //! @param[in]        NewBitmap Nouvelle bitmap à ajouter
  //!
  //! @return @c bool true si l'opération a réussi.
  //!
  //! @warning L'objet TRichEdit fait une copie de la bitmap, mais n'en devient pas
	//! propriétaire. La fonction appelante est donc responsable de la
	//! destruction de l'objet TBitmap après utilisation.
  //---------------------------------------------------------------------------

  bool FASTCALL AddBitmap(TBitmap *NewBitmap);

  //---------------------------------------------------------------------------
  //! @brief Impression
  //!
  //! Cette méthode imprime le contenu du RichEdit. Cette méthode gère le choix
	//! de l'imprimante, le format (portrait/paysage, A4/A3/...) et les
	//! paramètres (pages à imprimer, ...).
  //!
  //! @param[in]        Caption Titre de la page
  //!
  //! @return @c int 0 si Ok. Le numéro d'erreur dans le cas contraire.
  //---------------------------------------------------------------------------
  int FASTCALL Print(const AnsiString Caption);

  //@}


  //! @name Propriétés
  //@{

	//---------------------------------------------------------------------------
  //! @brief Propriété BorderStyle
	//!
	//! Cette propriété permet de définir le style de bordure de la list-box.
	//! Seuls les styles @b bsNone et @b bsSingle peuvent être utilisés pour ce
	//! composant.
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, TBorderStyle, BorderStyle);


  //---------------------------------------------------------------------------
  //! @brief Propriété WordWrap
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, bool, WordWrap);


  //---------------------------------------------------------------------------
  //! @brief Propriété HideScrollBars
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, bool, HideScrollBars);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelStart
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, int, SelStart);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelLength
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, int, SelLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété DefAttributes
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, TTextAttributes *, DefAttributes);


  //---------------------------------------------------------------------------
  //! @brief Propriété SelAttributes
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, TTextAttributes *, SelAttributes);


  //---------------------------------------------------------------------------
  //! @brief Propriété ParaAttributes
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, TParaAttributes *, ParaAttributes);


  //---------------------------------------------------------------------------
  //! @brief Propriété Lines
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, TStringList *, Lines);

  //---------------------------------------------------------------------------
  //! @brief Propriété ScrollBars
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TRichEdit, TScrollStyle, ScrollBars);


  //@}


  //! @name Evènements
  //@{

  //---------------------------------------------------------------------------
  //! @brief Evènement OnChange
	//!
	//! Cet évènement est déclenché lorsque le texte du composant a changé, soit
	//! parce que l'utilisateur a tapé du texte, soit suite à une commande.
  //!
  //! @param[in]        Sender Objet à l'origine de l'évènement
  //---------------------------------------------------------------------------

  DECLARE_EVENT_1(TNotifyEvent, OnChange, TObject *);

  //@}


};


//---------------------------------------------------------------------------
#endif

#undef In_TRichEditH
