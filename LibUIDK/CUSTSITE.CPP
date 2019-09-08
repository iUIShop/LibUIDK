// CustomOleControlSite.cpp : implementation of the CCustomOleControlSite class
//

#include "stdafx.h"
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

#include <AFXDISP.H>
#include <../src/occimpl.h>

#undef AFX_DATA
#define AFX_DATA AFX_DATA_EXPORT

#include "custsite.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// m_dwXDocHostUI_DblClk interface

//#if(_MSC_VER > 1200) // VC6.0

DWORD CCustomOleControlSite::GetXDocHostUI_DblClk()
{
	return m_dwXDocHostUI_DblClk;
}

void CCustomOleControlSite::SetXDocHostUI_DblClk(DWORD dwSet)
{
	m_dwXDocHostUI_DblClk = dwSet;
}


/////////////////////////////////////////////////////////////////////////////
// m_dwXDocHostUI_Flag interface

DWORD CCustomOleControlSite::GetXDocHostUI_Flag()
{
	return m_dwXDocHostUI_Flag;
}

void CCustomOleControlSite::SetXDocHostUI_Flag(DWORD dwSet)
{
	m_dwXDocHostUI_Flag = dwSet;
}



BEGIN_INTERFACE_MAP(CCustomOleControlSite, COleControlSite)
	INTERFACE_PART(CCustomOleControlSite, IID_IDocHostUIHandler, DocHostUIHandler)
	INTERFACE_PART(CCustomOleControlSite, IID_IDocHostShowUI, DocHostShowUI)
END_INTERFACE_MAP()

	

ULONG CCustomOleControlSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return pThis->ExternalAddRef();
}

ULONG CCustomOleControlSite::XDocHostUIHandler::Release()
{                            
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return pThis->ExternalRelease();
}

HRESULT CCustomOleControlSite::XDocHostUIHandler::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}

// * CImpIDocHostUIHandler::GetHostInfo 
// * 
// * Purpose: Called at initialization
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::GetHostInfo(/* [i/o] */	DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
	pInfo->dwFlags = pThis->GetXDocHostUI_Flag();
	pInfo->dwDoubleClick = pThis->GetXDocHostUI_DblClk();

	return S_OK;
}

// * CImpIDocHostUIHandler::ShowUI 
// * 
// * Purpose: Called when MSHTML.DLL shows its UI 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::ShowUI(/* [in ] */	DWORD dwID,
		/* [in ] */	IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		/* [in ] */	IOleCommandTarget __RPC_FAR *pCommandTarget,
		/* [in ] */	IOleInPlaceFrame __RPC_FAR *pFrame,
		/* [in ] */	IOleInPlaceUIWindow __RPC_FAR *pDoc)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	// We've already got our own UI in place so just return S_OK 
	return S_OK;
}

// * CImpIDocHostUIHandler::HideUI 
// * 
// * Purpose: Called when MSHTML.DLL hides its UI 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::HideUI(void)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return S_OK;
}

// * CImpIDocHostUIHandler::UpdateUI 
// * 
// * Purpose: Called when MSHTML.DLL updates its UI 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::UpdateUI(void)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	// MFC is pretty good about updating it's UI in it's Idle loop so I don't do anything here 
	return S_OK;
}

// * CImpIDocHostUIHandler::EnableModeless 
// * 
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::EnableModeless 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::EnableModeless(/* [in ] */	BOOL fEnable)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnDocWindowActivate 
// * 
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnDocWindowActivate 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::OnDocWindowActivate(/* [in ] */	BOOL fActive)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnFrameWindowActivate 
// * 
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnFrameWindowActivate 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::OnFrameWindowActivate(/* [in ] */	BOOL fActive)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ResizeBorder 
// * 
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::ResizeBorder 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::ResizeBorder(/* [in ] */	LPCRECT prcBorder, /* [in ] */	IOleInPlaceUIWindow __RPC_FAR *pUIWindow, /* [in ] */	BOOL fFrameWindow)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ShowContextMenu 
// * 
// * Purpose: Called when MSHTML.DLL would normally display its context menu 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::ShowContextMenu(/* [in ] */	DWORD dwID, /* [in ] */	POINT __RPC_FAR *ppt, /* [in ] */	IUnknown __RPC_FAR *pcmdtReserved, /* [in ] */	IDispatch __RPC_FAR *pdispReserved)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	// Don't show context menu
	if (pThis->GetXDocHostUI_Flag() & DOCHOSTUIFLAG_DISABLE_HELP_MENU)
		return S_OK;
	// Otherwise, show default
 	return S_FALSE;
}

// * CImpIDocHostUIHandler::TranslateAccelerator 
// * 
// * Purpose: Called from MSHTML.DLL's TranslateAccelerator routines 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::TranslateAccelerator(/* [in ] */	LPMSG lpMsg, /* [in ] */	const GUID __RPC_FAR *pguidCmdGroup, /* [in ] */	DWORD nCmdID)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::GetOptionKeyPath 
// * 
// * Purpose: Called by MSHTML.DLL to find where the host wishes to store  
// *its options in the registry 
// * 
STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::GetOptionKeyPath(/* [out] */	LPOLESTR __RPC_FAR *pchKey, /* [in ] */	DWORD dwReserved)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::GetDropTarget( 
		/* [in ] */	IDropTarget __RPC_FAR *pDropTarget,
		/* [out] */	IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::GetExternal( 
		/* [out] */	IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	// return the IDispatch we have for extending the object Model 
	return E_NOTIMPL;
}

STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::TranslateUrl( 
		/* [in ] */	DWORD dwTranslate,
		/* [in ] */	OLECHAR __RPC_FAR *pchURLIn,
		/* [out] */	OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}

STDMETHODIMP CCustomOleControlSite::XDocHostUIHandler::FilterDataObject( 
		/* [in ] */	IDataObject __RPC_FAR *pDO,
		/* [out] */	IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostUIHandler)
	return E_NOTIMPL;
}



/////////////////////////////////////////////////////////////////////////////
// CCustomOleControlSite: IDocHostShowUI::IUnknown methods

ULONG CCustomOleControlSite::XDocHostShowUI::AddRef()
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostShowUI)
	return pThis->ExternalAddRef();
}

ULONG CCustomOleControlSite::XDocHostShowUI::Release()
{                            
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostShowUI)
	return pThis->ExternalRelease();
}

HRESULT CCustomOleControlSite::XDocHostShowUI::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CCustomOleControlSite, DocHostShowUI)
	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}


/////////////////////////////////////////////////////////////////////////////
// CCustomOleControlSite: IDocHostShowUI methods

// ShowHelp
STDMETHODIMP CCustomOleControlSite::XDocHostShowUI::ShowHelp(
		/* [in ] */	HWND hwnd,
		/* [in ] */	LPOLESTR pszHelpFile,
		/* [in ] */	UINT uCommand,
		/* [in ] */	DWORD dwData,
		/* [in ] */	POINT ptMouse,
		/* [out] */	IDispatch __RPC_FAR *pDispatchObjectHit)
{
	return S_OK;
}

// ShowMessage
STDMETHODIMP CCustomOleControlSite::XDocHostShowUI::ShowMessage(
		/* [in ] */	HWND hwnd,
		/* [in ] */	LPOLESTR lpstrText,
		/* [in ] */	LPOLESTR lpstrCaption,
		/* [in ] */	DWORD dwType,
		/* [in ] */	LPOLESTR lpstrHelpFile,
		/* [in ] */	DWORD dwHelpContext,
		/* [out] */	LRESULT __RPC_FAR *plResult)
{
	return S_OK;
}

//#endif // VC6
