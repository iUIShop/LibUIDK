// HtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
#if(_MSC_VER <= 1200) // VC6.0
#include <afxdisp.h>
#include <../src/occimpl.h>
#include "CustSite.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlCtrl

LRESULT APIENTRY InternetExplorer_ServerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ERASEBKGND)
	{
		return TRUE;
	}

	// Call the old window proc
	WNDPROC oldWndProc = (WNDPROC)GetProp(hwnd, GETPROP_OLDWNDPROC);

	if (oldWndProc != NULL)
	{
		return CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);
	}

	ASSERT(FALSE);
	return 0;
}

LRESULT APIENTRY ShellDocObjectViewWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PARENTNOTIFY)
	{
		WORD wEvent = LOWORD(wParam);
		if (wEvent == WM_CREATE)
		{
			WORD id = HIWORD(wParam);
			HWND hWndInternetExplorer_Server = (HWND)lParam;
			WNDPROC oldhWndInternetExplorer_ServerWndProc = (WNDPROC)IUISetWindowLong(hWndInternetExplorer_Server, GWLP_WNDPROC, (LONG_PTR)InternetExplorer_ServerWndProc);
			SetProp(hWndInternetExplorer_Server, GETPROP_OLDWNDPROC, oldhWndInternetExplorer_ServerWndProc);
		}
	}

	// Call the old window proc
	WNDPROC oldWndProc = (WNDPROC)GetProp(hwnd, GETPROP_OLDWNDPROC);

	if (oldWndProc != NULL)
	{
		return CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);
	}

	ASSERT(FALSE);
	return 0;
}

LRESULT APIENTRY ShellEmbeddingWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PARENTNOTIFY)
	{
		WORD wEvent = LOWORD(wParam);
		if (wEvent == WM_CREATE)
		{
			WORD id = HIWORD(wParam);
			HWND hWndShellDocObjectView = (HWND)lParam;
			WNDPROC oldShellDocObjectViewWndProc = (WNDPROC)IUISetWindowLong(hWndShellDocObjectView, GWLP_WNDPROC, (LONG_PTR)ShellDocObjectViewWndProc);
			SetProp(hWndShellDocObjectView, GETPROP_OLDWNDPROC, oldShellDocObjectViewWndProc);
		}
	}

	// Call the old window proc
	WNDPROC oldWndProc = (WNDPROC)GetProp(hwnd, GETPROP_OLDWNDPROC);

	if (oldWndProc != NULL)
	{
		return CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);
	}

	ASSERT(FALSE);
	return 0;
}

struct HTMLCTRLMEMBER
{
public:
	HTMLCTRLMEMBER()
	{
#if(_MSC_VER >= 1310) // VC7.1
		m_dwXDocHostUI_Flag = 0;
		m_dwXDocHostUI_DblClk = 0;
#endif
	}

#if(_MSC_VER <= 1200) // VC6.0
	CCustomOccManager m_pOccManager;
#else
	DWORD m_dwXDocHostUI_Flag;
	DWORD m_dwXDocHostUI_DblClk;
#endif
};


CHtmlCtrl::CHtmlCtrl()
{
	m_pMember = new HTMLCTRLMEMBER;
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	EnableAutomation();		// For script call c++.
	m_pDoc = NULL;
}

CHtmlCtrl::~CHtmlCtrl()
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (HTMLCTRLMEMBER *)m_pMember;
		m_pMember = NULL;
	}

	if (m_pDoc != NULL)
	{
		m_pDoc->Release();
	}
}

int CHtmlCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_HTMLCTRL, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CHtmlCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_HTMLCTRL && ect != CT_HTMLCTRL)
	{
		return -3;
	}

	HTMLCTRLPROPERTIES *pHtmlCtrlProp = (HTMLCTRLPROPERTIES *)pCtrlProp;

	ShowBorder(!(BOOL)pHtmlCtrlProp->m_bHideBorder, FALSE);
	ShowIEScrollBar(!(BOOL)pHtmlCtrlProp->m_bHideScrollBar, FALSE);
	ShowContextMenu(!(BOOL)pHtmlCtrlProp->m_bHideContextMenu);

	// Open the url
	CString strUrl = (LPCTSTR)pHtmlCtrlProp->m_strStartPage;
	if (strUrl.GetLength() > 0)
	{
		Navigate2(strUrl, NULL, NULL);
	}

	return 0;
}

void CHtmlCtrl::DoDataExchange(CDataExchange *pDX)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlCtrl)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_DISPATCH_MAP(CHtmlCtrl, CHtmlView)
END_DISPATCH_MAP()

BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	//{{AFX_MSG_MAP(CHtmlCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlCtrl diagnostics

#ifdef _DEBUG
void CHtmlCtrl::AssertValid() const
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CHtmlView::AssertValid();
}

void CHtmlCtrl::Dump(CDumpContext &dc) const
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmlCtrl message handlers

#if(_MSC_VER <= 1200) // VC6.0
BOOL CHtmlCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	//	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	m_pCreateContext = pContext;

	if (!CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd,  nID, pContext))
	{
		return FALSE;
	}

	AfxEnableControlContainer(&pMember->m_pOccManager);

	///////////////////////////////////
	// create the control window:

	CRect rcBrowser = rect;
	rcBrowser.DeflateRect(-rect.left, -rect.top, rect.left, rect.top);
	if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, lpszWindowName, WS_VISIBLE | WS_CHILD, rcBrowser, this, AFX_IDW_PANE_FIRST))
	{
		DestroyWindow();
		return FALSE;
	}

	LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void **) &m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		m_pBrowserApp = NULL;
		m_wndBrowser.DestroyWindow();
		DestroyWindow();
		return FALSE;
	}

	SetClassLong(this->m_hWnd, GCL_STYLE, CS_DBLCLKS);

	///////////////////////////////////

	return TRUE;
}

#else
HRESULT CHtmlCtrl::OnGetHostInfo(DOCHOSTUIINFO *pInfo)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
	pInfo->dwFlags = pMember->m_dwXDocHostUI_Flag;
	pInfo->dwDoubleClick = pMember->m_dwXDocHostUI_DblClk;

	return CHtmlView::OnGetHostInfo(pInfo);
}

HRESULT CHtmlCtrl::OnShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	if (pMember->m_dwXDocHostUI_Flag & DOCHOSTUIFLAG_DISABLE_HELP_MENU)
	{
		return S_OK;
	}

	return S_FALSE;
}

HRESULT CHtmlCtrl::OnGetExternal(LPDISPATCH *lppDispatch)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	*lppDispatch = GetIDispatch(TRUE);
	return S_OK;
}

#endif

// Handle OnSize to resolve the debug warning when resize the html view (In output window):
// Warning: constructing COleException, scode = DISP_E_MEMBERNOTFOUND ($80020003).
void CHtmlCtrl::OnSize(UINT nType, int cx, int cy)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CFormView::OnSize(nType, cx, cy);	// NOT CHtmlView::OnSize

	// TODO: Add your message handler code here
	// The following code copy from CHtmlView::Onsize(), but modify the parameter 2 of ::AdjustWindowRectEx.
	if (::IsWindow(m_wndBrowser.m_hWnd))
	{
		// need to push non-client borders out of the client area
		CRect rect;
		GetClientRect(rect);
		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

LRESULT CHtmlCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	// Draw DirectUI mode
	if (message == WM_DRAWCONTROL)
	{
		CDC *pDC = (CDC *)wParam;
	}

	else if (message == WM_PARENTNOTIFY)
	{
		WORD wEvent = LOWORD(wParam);
		if (wEvent == WM_CREATE)
		{
			WORD id = HIWORD(wParam);
			HWND hWndShellEmbedding = (HWND)lParam;
			WNDPROC oldShellEmbeddingProc = (WNDPROC)IUISetWindowLong(hWndShellEmbedding, GWLP_WNDPROC, (LONG_PTR)ShellEmbeddingWndProc);
			SetProp(hWndShellEmbedding, GETPROP_OLDWNDPROC, oldShellEmbeddingProc);
		}
		else if (wEvent == WM_LBUTTONDOWN)
		{
			int n = 0;
		}
	}

	return CHtmlView::WindowProc(message, wParam, lParam);
}

int CHtmlCtrl::ShowBorder(BOOL bShow, BOOL bRedraw/*=TRUE*/)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

#if(_MSC_VER <= 1200) // VC6.0
	DWORD dwFlags = pMember->m_pOccManager.m_pCustSite->GetXDocHostUI_Flag();
	bShow ? dwFlags &= ~DOCHOSTUIFLAG_NO3DBORDER : dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
	pMember->m_pOccManager.m_pCustSite->SetXDocHostUI_Flag(dwFlags);
#else
	bShow ? pMember->m_dwXDocHostUI_Flag &= ~DOCHOSTUIFLAG_NO3DBORDER : pMember->m_dwXDocHostUI_Flag |= DOCHOSTUIFLAG_NO3DBORDER;
#endif

	if (bRedraw)
	{
		Navigate2(GetLocationURL(), 0, NULL, NULL);
	}

	return 0;
}

int CHtmlCtrl::ShowIEScrollBar(BOOL bShow, BOOL bRedraw/*=TRUE*/)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

#if(_MSC_VER <= 1200) // VC6.0
	DWORD dwFlags = pMember->m_pOccManager.m_pCustSite->GetXDocHostUI_Flag();
	bShow ? dwFlags &= ~DOCHOSTUIFLAG_SCROLL_NO : dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
	pMember->m_pOccManager.m_pCustSite->SetXDocHostUI_Flag(dwFlags);
#else
	bShow ? pMember->m_dwXDocHostUI_Flag &= ~DOCHOSTUIFLAG_SCROLL_NO : pMember->m_dwXDocHostUI_Flag |= DOCHOSTUIFLAG_SCROLL_NO;
#endif

	if (bRedraw)
	{
		Navigate2(GetLocationURL(), 0, NULL, NULL);
	}

	return 0;
}

int CHtmlCtrl::ShowContextMenu(BOOL bShow)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

#if(_MSC_VER <= 1200) // VC6.0
	DWORD dwFlags = pMember->m_pOccManager.m_pCustSite->GetXDocHostUI_Flag();
	bShow ? dwFlags &= ~DOCHOSTUIFLAG_DISABLE_HELP_MENU : dwFlags |= DOCHOSTUIFLAG_DISABLE_HELP_MENU;
	pMember->m_pOccManager.m_pCustSite->SetXDocHostUI_Flag(dwFlags);
#else
	bShow ? pMember->m_dwXDocHostUI_Flag &= ~DOCHOSTUIFLAG_DISABLE_HELP_MENU : pMember->m_dwXDocHostUI_Flag |= DOCHOSTUIFLAG_DISABLE_HELP_MENU;
#endif

	return 0;
}

//<<Begin>>////////////////////////////////////////////////////////////////////////

void CHtmlCtrl::OnDocumentComplete(LPCTSTR lpszURL)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CHtmlView::OnDocumentComplete(lpszURL);
	SetScriptDocument();
}

const CString CHtmlCtrl::GetSystemErrorMessage(DWORD dwError)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CString strError;
	LPTSTR lpBuffer;

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,  dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL))

	{
		strError = "FormatMessage Netive Error" ;
	}
	else
	{
		strError = lpBuffer;
		LocalFree(lpBuffer);
	}
	return strError;
}

CString CHtmlCtrl::GetNextToken(CString &strSrc, const CString strDelim, BOOL bTrim, BOOL bFindOneOf)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CString strToken;
	int idx = bFindOneOf ? strSrc.FindOneOf(strDelim) : strSrc.Find(strDelim);
	if (idx != -1)
	{
		strToken  = strSrc.Left(idx);
		strSrc = strSrc.Right(strSrc.GetLength() - (idx + 1));
	}
	else
	{
		strToken = strSrc;
		strSrc.Empty();
	}
	if (bTrim)
	{
		strToken.TrimLeft();
		strToken.TrimRight();
	}
	return strToken;
}

BOOL CHtmlCtrl::SetScriptDocument()
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	LPDISPATCH pDisp = GetHtmlDocument();
	if (pDisp == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_pDoc = NULL;

	CComPtr<IDispatch> spDisp = pDisp;

	HRESULT hr = spDisp->QueryInterface(IID_IHTMLDocument2, (void **)&m_pDoc);
	if (FAILED(hr))
	{
		MessageBox(_T("Failed to get HTML document COM object"));
		return FALSE;
	}
	return TRUE;
}

BOOL CHtmlCtrl::GetJScript(IDispatch **ppDisp)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	if (m_pDoc == NULL)
	{
		return FALSE;
	}

	HRESULT hr = m_pDoc->get_Script(ppDisp);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

BOOL CHtmlCtrl::GetJScripts(IHTMLElementCollection *pColl)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	if (m_pDoc == NULL)
	{
		return FALSE;
	}

	HRESULT hr = m_pDoc->get_scripts(&pColl);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

CString CHtmlCtrl::ScanJScript(CString &strAText, CStringArray &args)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	args.RemoveAll();
	CString strDelim(" \n\r\t"), strSrc(strAText);
	BOOL bFound = FALSE;
	while (!strSrc.IsEmpty())
	{
		CString strStart = GetNextToken(strSrc, strDelim);
		if (strStart == "function")
		{
			bFound = TRUE;
			break;
		}
		if (strStart == "/*")
		{
			// Skip comments
			while (!strSrc.IsEmpty())
			{
				CString strStop = GetNextToken(strSrc, strDelim);
				if (strStop == "*/")
				{
					break;
				}
			}
		}
	}

	if (!bFound)
	{
		return _T("");
	}

	CString strFunc = GetNextToken(strSrc, _T("("), TRUE);
	CString strArgs = GetNextToken(strSrc, _T(")"), TRUE);

	// Parse arguments
	CString strArg;
	while (!(strArg = GetNextToken(strArgs, _T(","))).IsEmpty())
	{
		args.Add(strArg);
	}

	strAText = strSrc;
	return strFunc;
}

BOOL CHtmlCtrl::JScriptOK()
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	return (m_pDoc != NULL);
}

BOOL CHtmlCtrl::CallJScript(const CString strFunc, _variant_t *pVarResult)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CStringArray paramArray;
	return CallJScript(strFunc, paramArray, pVarResult);
}

BOOL CHtmlCtrl::CallJScript(const CString strFunc, const CString strArg1, _variant_t *pVarResult)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CStringArray paramArray;
	paramArray.Add(strArg1);
	return CallJScript(strFunc, paramArray, pVarResult);
}

BOOL CHtmlCtrl::CallJScript(const CString strFunc, const CString strArg1, const CString strArg2, _variant_t *pVarResult)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CStringArray paramArray;
	paramArray.Add(strArg1);
	paramArray.Add(strArg2);
	return CallJScript(strFunc, paramArray, pVarResult);
}

BOOL CHtmlCtrl::CallJScript(const CString strFunc, const CString strArg1, const CString strArg2, const CString strArg3, _variant_t *pVarResult)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CStringArray paramArray;
	paramArray.Add(strArg1);
	paramArray.Add(strArg2);
	paramArray.Add(strArg3);
	return CallJScript(strFunc, paramArray, pVarResult);
}

BOOL CHtmlCtrl::CallJScript(const CString strFunc, const CStringArray &paramArray, _variant_t *pVarResult)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CComPtr<IDispatch> spScript;
	if (!GetJScript(&spScript))
	{
		MessageBox(_T("函数GetJScrip调用失败！"));
		return FALSE;
	}
	CComBSTR bstrFunc(strFunc);
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL, &bstrFunc, 1,
			LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(hr))
	{
		MessageBox(GetSystemErrorMessage(hr));
		return FALSE;
	}

	INT_PTR arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = (UINT)arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];

	for (int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = (LPCTSTR)paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
	_variant_t vaResult;
	UINT nArgErr = (UINT) - 1; // initialize to invalid arg

	hr = spScript->Invoke(dispid, IID_NULL, 0,
			DISPATCH_METHOD, &dispparams, &vaResult, &excepInfo, &nArgErr);

	delete [] dispparams.rgvarg;
	if (FAILED(hr))
	{
		MessageBox(GetSystemErrorMessage(hr));
		return FALSE;
	}

	if (pVarResult)
	{
		*pVarResult = vaResult;
	}
	return TRUE;
}

// 修改网页元素的内容
BOOL CHtmlCtrl::PutElementHtml(CString ElemID, CString Html)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	IHTMLDocument2 *pDoc = NULL;
	IHTMLElementCollection *pElemColl = NULL;
	IDispatch *pElemDisp = NULL;
	IHTMLElement *pElem = NULL;

	BOOL Result = FALSE;

	pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	if (pDoc != NULL)
	{
		pDoc->get_all(&pElemColl);
		if (pElemColl != NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(LONG(0));
			HRESULT hr = pElemColl->item(varID, varIdx, &pElemDisp);
			if (SUCCEEDED(hr))
			{
				if (pElemDisp != NULL)
				{
					hr = pElemDisp->QueryInterface(IID_IHTMLElement, (void **)&pElem);
					if (SUCCEEDED(hr))
					{
						BSTR bstrHtml = Html.AllocSysString();
						pElem->put_innerHTML(bstrHtml);
						pElem->Release();
						::SysFreeString(bstrHtml);
						Result = TRUE;
					}
					pElemDisp->Release();
				}
			}
		}//if(pAllElem!=NULL)
		pDoc->Release();
	}//if(pDoc!=NULL)
	return Result;
}

BOOL CHtmlCtrl::GetElementValue(CString ElemID, CString &Value)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	Value.Empty();

	IHTMLDocument2 *pDoc = NULL;
	IHTMLElementCollection *pAllElem = NULL;
	IDispatch *pElemDisp = NULL;
	IHTMLElement *pElem = NULL;

	IHTMLInputElement *pInput = NULL;
	IHTMLSelectElement *pSelect = NULL;
	IHTMLTextAreaElement *pTextArea = NULL;


	BOOL Result = FALSE;
	HRESULT hr = S_FALSE;

	pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	if (pDoc != NULL)
	{
		hr = pDoc->get_all(&pAllElem);
		if (SUCCEEDED(hr) && pAllElem != NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(LONG(0));
			hr = pAllElem->item(varID, varIdx, &pElemDisp);
			if (SUCCEEDED(hr) && pElemDisp != NULL)
			{
				hr = pElemDisp->QueryInterface(IID_IHTMLElement, (void **)&pElem);
				if (SUCCEEDED(hr) && pElem != NULL)
				{
					BSTR bstrTagName;
					hr = pElem->get_tagName(&bstrTagName);
					if (SUCCEEDED(hr))
					{
						CString strTagName(bstrTagName);
						SysFreeString(bstrTagName);
						strTagName.MakeUpper();
						if (strTagName == "INPUT")
						{
							hr = pElem->QueryInterface(&pInput);
							if (SUCCEEDED(hr) && pInput != NULL)
							{
								BSTR bstrType;
								hr = pInput->get_type(&bstrType);
								if (SUCCEEDED(hr))
								{
									CString strType(bstrType);
									strType.MakeUpper();
									SysFreeString(bstrType);
									if (strType == "TEXT" || strType == "HIDDEN")
									{
										BSTR bstrValue;
										hr = pInput->get_value(&bstrValue);
										if (SUCCEEDED(hr))
										{
											CString strValue(bstrValue);
											SysFreeString(bstrValue);
											Value = strValue;
											Result = TRUE;
										}
									} // if(strType == "TEXT" || strType == "HIDDEN")
									else if (strType == "CHECKBOX" || strType == "RADIO")
									{
										VARIANT_BOOL Check;
										hr = pInput->get_checked(&Check);
										if (SUCCEEDED(hr))
										{
											if (Check == VARIANT_TRUE)
											{
												Value = "1";
											}
											else
											{
												Value = "0";
											}
											Result = TRUE;
										}
									} // else if(strType=="CHECKBOX" || strType == "RADIO")
								} // if(SUCCEEDED(hr))
								pInput->Release();
							} // if(SUCCEEDED(hr) && pInput!=NULL)
						} // if(strTagName == "INPUT")
						else if (strTagName == "SELECT")
						{
							hr = pElem->QueryInterface(&pSelect);
							if (SUCCEEDED(hr) && pSelect != NULL)
							{
								long index;
								hr = pSelect->get_selectedIndex(&index);
								if (SUCCEEDED(hr))
								{
									Value.Format(_T("%d"), index);
									Result = TRUE;
								}
								pSelect->Release();
							} // if(SUCCEEDED(hr) && pSelect!=NULL)
						} // else if(strTagName == "SELECT")
						else if (strTagName == "TEXTAREA")
						{
							hr = pElem->QueryInterface(&pTextArea);
							if (SUCCEEDED(hr) && pTextArea != NULL)
							{
								BSTR bstrValue;
								hr = pTextArea->get_value(&bstrValue);
								if (SUCCEEDED(hr))
								{
									CString strValue(bstrValue);
									SysFreeString(bstrValue);
									Value = strValue;
									Result = TRUE;
								}
								pTextArea->Release();
							}
						} // else if(strTagName == "TEXTAREA")
					}
					pElem->Release();
				}
			} // if(SUCCEEDED(hr) && pElem != NULL)
			pAllElem->Release();
		} // if(SUCCEEDED(hr) && pAllElem != NULL)
		pDoc->Release();
	} // if(pDoc != NULL)

	return Result;
}


BOOL CHtmlCtrl::PutElementValue(CString ElemID, CString Value)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	IHTMLDocument2 *pDoc = NULL;
	IHTMLElementCollection *pAllElem = NULL;
	IDispatch *pElemDisp = NULL;
	IHTMLElement *pElem = NULL;

	IHTMLInputElement *pInput = NULL;
	IHTMLSelectElement *pSelect = NULL;
	IHTMLTextAreaElement *pTextArea = NULL;


	BOOL Result = FALSE;
	HRESULT hr = S_FALSE;

	pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	if (pDoc != NULL)
	{
		hr = pDoc->get_all(&pAllElem);
		if (SUCCEEDED(hr) && pAllElem != NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(LONG(0));
			hr = pAllElem->item(varID, varIdx, &pElemDisp);
			if (SUCCEEDED(hr) && pElemDisp != NULL)
			{
				hr = pElemDisp->QueryInterface(IID_IHTMLElement, (void **)&pElem);
				if (SUCCEEDED(hr) && pElem != NULL)
				{
					BSTR bstrTagName;
					hr = pElem->get_tagName(&bstrTagName);
					if (SUCCEEDED(hr))
					{
						CString strTagName(bstrTagName);
						SysFreeString(bstrTagName);
						strTagName.MakeUpper();
						if (strTagName == "INPUT")
						{
							hr = pElem->QueryInterface(&pInput);
							if (SUCCEEDED(hr) && pInput != NULL)
							{
								BSTR bstrType;
								hr = pInput->get_type(&bstrType);
								if (SUCCEEDED(hr))
								{
									CString strType(bstrType);
									strType.MakeUpper();
									SysFreeString(bstrType);
									if (strType == "TEXT" || strType == "HIDDEN")
									{
										BSTR bstrValue;
										bstrValue = Value.AllocSysString();
										hr = pInput->put_value(bstrValue);
										SysFreeString(bstrValue);
										if (SUCCEEDED(hr))
										{
											Result = TRUE;
										}
									} // if(strType == "TEXT" || strType == "HIDDEN")
									else if (strType == "CHECKBOX" || strType == "RADIO")
									{
										VARIANT_BOOL Check;
										if (Value == "1")
										{
											Check = VARIANT_TRUE;
										}
										else
										{
											Check = VARIANT_FALSE;
										}

										hr = pInput->put_checked(Check);
										if (SUCCEEDED(hr))
										{
											Result = TRUE;
										}
									} // else if(strType=="CHECKBOX" || strType == "RADIO")
								} // if(SUCCEEDED(hr))
								pInput->Release();
							} // if(SUCCEEDED(hr) && pInput!=NULL)
						} // if(strTagName == "INPUT")
						else if (strTagName == "SELECT")
						{
							hr = pElem->QueryInterface(&pSelect);
							if (SUCCEEDED(hr) && pSelect != NULL)
							{
								long index;
								index = LongFromString(Value);
								hr = pSelect->put_selectedIndex(index);
								if (SUCCEEDED(hr))
								{
									Result = TRUE;
								}
								pSelect->Release();
							} // if(SUCCEEDED(hr) && pSelect!=NULL)
						} // else if(strTagName == "SELECT")
						else if (strTagName == "TEXTAREA")
						{
							hr = pElem->QueryInterface(&pTextArea);
							if (SUCCEEDED(hr) && pTextArea != NULL)
							{
								BSTR bstrValue;
								bstrValue = Value.AllocSysString();
								hr = pTextArea->put_value(bstrValue);
								SysFreeString(bstrValue);
								if (SUCCEEDED(hr))
								{
									Result = TRUE;
								}
								pTextArea->Release();
							}
						} // else if(strTagName == "TEXTAREA")
					}
					pElem->Release();
				}
			} // if(SUCCEEDED(hr) && pElem != NULL)
			pAllElem->Release();
		} // if(SUCCEEDED(hr) && pAllElem != NULL)
		pDoc->Release();
	} // if(pDoc != NULL)

	return Result;
}

void CHtmlCtrl::ElementSetFocus(CString ElemID)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	IHTMLDocument2 *pDoc = NULL;
	IHTMLElementCollection *pAllElem = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLControlElement *pControl = NULL;
	HRESULT hr = S_FALSE;

	pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	if (pDoc != NULL)
	{
		hr = pDoc->get_all(&pAllElem);
		if (SUCCEEDED(hr) && pAllElem != NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(LONG(0));
			hr = pAllElem->item(varID, varIdx, (LPDISPATCH *)&pElem);
			if (SUCCEEDED(hr) && pElem != NULL)
			{
				pElem->QueryInterface(&pControl);
				if (pControl != NULL)
				{
					pControl->focus();
					pControl->Release();
				}
				pElem->Release();
			} // if(SUCCEEDED(hr) && pElem != NULL)
			pAllElem->Release();
		}//if(pAllElem!=NULL)
		pDoc->Release();
	}//if(pDoc!=NULL)
}

IHTMLStyle *CHtmlCtrl::GetElementStyle(CString ElemID)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	IHTMLDocument2 *pDoc = NULL;
	IHTMLElementCollection *pElemColl = NULL;
	IDispatch *pElemDisp = NULL;
	IHTMLElement *pElem = NULL;
	IHTMLStyle *pStyle = NULL;

	BOOL Result = FALSE;

	pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	if (pDoc != NULL)
	{
		pDoc->get_all(&pElemColl);
		if (pElemColl != NULL)
		{
			_variant_t varID(ElemID);
			_variant_t varIdx(LONG(0));
			HRESULT hr = pElemColl->item(varID, varIdx, &pElemDisp);
			if (SUCCEEDED(hr))
			{
				if (pElemDisp != NULL)
				{
					hr = pElemDisp->QueryInterface(IID_IHTMLElement, (void **)&pElem);
					if (SUCCEEDED(hr))
					{
						hr = pElem->get_style(&pStyle);
						if (SUCCEEDED(hr))
						{
							Result = TRUE;
						}
						pElem->Release();
					}
					pElemDisp->Release();
				}
			}
		}//if(pAllElem!=NULL)
		pDoc->Release();
	}//if(pDoc!=NULL)

	return pStyle;
}

HRESULT CHtmlCtrl::GetElement(LPCTSTR szElementId, IDispatch **ppdisp,
	BOOL *pbCollection /*= NULL*/)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	CComPtr<IHTMLElementCollection> sphtmlAll;
	CComPtr<IHTMLElementCollection> sphtmlColl;
	CComPtr<IDispatch> spdispElem;
	CComVariant varName;
	CComVariant varIndex;
	HRESULT hr = S_OK;
	CComPtr<IHTMLDocument2> sphtmlDoc;
	USES_CONVERSION;

	*ppdisp = NULL;

	if (pbCollection)
	{
		*pbCollection = FALSE;
	}

	sphtmlDoc = (IHTMLDocument2 *)GetHtmlDocument();
	if (sphtmlDoc == NULL)
	{
		return hr;
	}

	varName.vt = VT_BSTR;
	varName.bstrVal = T2BSTR(szElementId);
	if (!varName.bstrVal)
	{
		hr = E_OUTOFMEMORY;
		goto __Error;
	}

	hr = sphtmlDoc->get_all(&sphtmlAll);
	if (sphtmlAll == NULL)
	{
		goto __Error;
	}
	hr = sphtmlAll->item(varName, varIndex, &spdispElem);
	if (spdispElem == NULL)
	{
		hr = E_NOINTERFACE;
		goto __Error;
	}

	spdispElem->QueryInterface(__uuidof(IHTMLElementCollection), (void **) &sphtmlColl);
	if (sphtmlColl)
	{
		if (pbCollection)
		{
			*pbCollection = TRUE;
		}
#ifdef _DEBUG
		else
		{
			TRACE(_T("[LibUIDK]===Warning: duplicate IDs or NAMEs.\n"));
			ATLASSERT(FALSE);
		}
#endif

	}
__Error:
	if (SUCCEEDED(hr))
	{
		*ppdisp = spdispElem;
		if (spdispElem)
		{
			(*ppdisp)->AddRef();
		}
	}
	return hr;
}

HRESULT CHtmlCtrl::GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	return GetElementInterface(szElementId, __uuidof(IHTMLElement), (void **) pphtmlElement);
}

HRESULT CHtmlCtrl::GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj)
{
	HTMLCTRLMEMBER *pMember = (HTMLCTRLMEMBER *)m_pMember;

	HRESULT hr = E_NOINTERFACE;
	*ppvObj = NULL;
	CComPtr<IDispatch> spdispElem;

	hr = GetElement(szElementId, &spdispElem);

	if (spdispElem)
	{
		hr = spdispElem->QueryInterface(riid, ppvObj);
	}
	return hr;
}

//<<End>>////////////////////////////////////////////////////////////////////////