// CustomOleControlSite.h : interface of the CCustomOleControlSite class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _R_OLE_CONTROL_SITE_H_
#define _R_OLE_CONTROL_SITE_H_

#include <mshtmhst.h> // for IDocHostUIHandler

//#if(_MSC_VER > 1200) // VC6.0

class CCustomOleControlSite : public COleControlSite
{
	DWORD m_dwXDocHostUI_DblClk;
	DWORD m_dwXDocHostUI_Flag;

public:
	CCustomOleControlSite(COleControlContainer *pCnt = NULL):COleControlSite(pCnt)
	{
		m_dwXDocHostUI_DblClk = DOCHOSTUIDBLCLK_DEFAULT;
		m_dwXDocHostUI_Flag = 0;
	}

// m_dwXDocHostUI_DblClk interface
	DWORD GetXDocHostUI_DblClk();
	void SetXDocHostUI_DblClk(DWORD dwSet);

// m_dwXDocHostUI_Flag interface
	DWORD GetXDocHostUI_Flag();
	void SetXDocHostUI_Flag(DWORD dwSet);


protected:

DECLARE_INTERFACE_MAP();
BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)

	STDMETHODIMP ShowContextMenu(
		/* [in ] */	DWORD dwID,
		/* [in ] */	POINT __RPC_FAR *ppt,
		/* [in ] */	IUnknown __RPC_FAR *pcmdtReserved,
		/* [in ] */	IDispatch __RPC_FAR *pdispReserved);

	STDMETHODIMP GetHostInfo( 
		/* [i/o] */	DOCHOSTUIINFO __RPC_FAR *pInfo);

	STDMETHODIMP ShowUI( 
		/* [in ] */	DWORD dwID,
		/* [in ] */	IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		/* [in ] */	IOleCommandTarget __RPC_FAR *pCommandTarget,
		/* [in ] */	IOleInPlaceFrame __RPC_FAR *pFrame,
		/* [in ] */	IOleInPlaceUIWindow __RPC_FAR *pDoc);

	STDMETHODIMP HideUI(void);

	STDMETHODIMP UpdateUI(void);

	STDMETHODIMP EnableModeless(/* [in ] */	BOOL fEnable);

	STDMETHODIMP OnDocWindowActivate(/* [in ] */	BOOL fActive);

	STDMETHODIMP OnFrameWindowActivate(/* [in ] */	BOOL fActive);

	STDMETHODIMP ResizeBorder(
		/* [in ] */	LPCRECT prcBorder,
		/* [in ] */	IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
		/* [in ] */	BOOL fFrameWindow);

	STDMETHODIMP TranslateAccelerator(
		/* [in ] */	LPMSG lpMsg,
		/* [in ] */	const GUID __RPC_FAR *pguidCmdGroup,
		/* [in ] */	DWORD nCmdID);

	STDMETHODIMP GetOptionKeyPath(
		/* [out] */	LPOLESTR __RPC_FAR *pchKey,
		/* [in ] */	DWORD dwReserved);

	STDMETHODIMP GetDropTarget(
		/* [in ] */	IDropTarget __RPC_FAR *pDropTarget,
		/* [out] */	IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);

	STDMETHODIMP GetExternal(
		/* [out] */	IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);

	STDMETHODIMP TranslateUrl(
		/* [in ] */	DWORD dwTranslate,
		/* [in ] */	OLECHAR __RPC_FAR *pchURLIn,
		/* [out] */	OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);

	STDMETHODIMP FilterDataObject(
		/* [in ] */	IDataObject __RPC_FAR *pDO,
		/* [out] */	IDataObject __RPC_FAR *__RPC_FAR *ppDORet);

END_INTERFACE_PART(DocHostUIHandler)

BEGIN_INTERFACE_PART(DocHostShowUI, IDocHostShowUI)

// ShowHelp
	STDMETHODIMP ShowHelp(
		/* [in ] */	HWND hwnd,
		/* [in ] */	LPOLESTR pszHelpFile,
		/* [in ] */	UINT uCommand,
		/* [in ] */	DWORD dwData,
		/* [in ] */	POINT ptMouse,
		/* [out] */	IDispatch __RPC_FAR *pDispatchObjectHit);

// ShowMessage
	STDMETHODIMP ShowMessage(
		/* [in ] */	HWND hwnd,
		/* [in ] */	LPOLESTR lpstrText,
		/* [in ] */	LPOLESTR lpstrCaption,
		/* [in ] */	DWORD dwType,
		/* [in ] */	LPOLESTR lpstrHelpFile,
		/* [in ] */	DWORD dwHelpContext,
		/* [out] */	LRESULT __RPC_FAR *plResult);


END_INTERFACE_PART(DocHostShowUI)
};


class CCustomOccManager :public COccManager
{
public:
	CCustomOleControlSite *m_pCustSite;

	CCustomOccManager()
	{
		m_pCustSite = NULL;
	}

	COleControlSite* CreateSite(COleControlContainer* pCtrlCont)
	{
		m_pCustSite = new CCustomOleControlSite(pCtrlCont);
		return m_pCustSite;
	}
};

//#endif // VC6

#endif // _R_OLE_CONTROL_SITE_H_
