// UIWnd.cpp : implementation file
//

#include "stdafx.h"

#include <map>
#include <AFXPRIV.H>

#if(_MSC_VER <= 1200) // VC6.0
#include <../src/afximpl.h>
#else
#include <../src/mfc/afximpl.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDT_DELAYSTARTSTORY						712
#define IDT_STORYBOARD							713

//////////////////////////////////////////////////////////////////////////

int AddToUIWndHandlerList(CUIWndHandler *pHandler)
{
	if (pHandler == NULL)
	{
		return -1;
	}

	UINT uWinID = pHandler->GetBindWindowID();
	if (uWinID <= 0)
	{
		ASSERT(FALSE);
		return -2;
	}

	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	if (pThreadState == NULL)
	{
		ASSERT(FALSE);
		return -3;
	}

	std::map<UINT, std::vector<CUIWndHandler *> >::iterator it =
		pThreadState->m_mapUIWndHandler.find(uWinID);

	// Not found
	if (it == pThreadState->m_mapUIWndHandler.end())
	{
		std::vector<CUIWndHandler *> vUIWndHandlers;
		vUIWndHandlers.push_back(pHandler);

		pThreadState->m_mapUIWndHandler[uWinID] = vUIWndHandlers;
	}
	// Found
	else
	{
		ASSERT(it->second.size() > 0);
		it->second.push_back(pHandler);
	}

	return 0;
}

int DeleteFromUIWndHandlerList(CUIWndHandler *pHandler)
{
	if (pHandler == NULL)
	{
		return -1;
	}

	UINT uWinID = pHandler->GetBindWindowID();
	if (uWinID <= 0)
	{
		ASSERT(FALSE);
		return -2;
	}

	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	if (pThreadState == NULL)
	{
		ASSERT(FALSE);
		return -3;
	}

	std::map<UINT, std::vector<CUIWndHandler *> >::iterator it = pThreadState->m_mapUIWndHandler.find(uWinID);
	if (it == pThreadState->m_mapUIWndHandler.end())
	{
		ASSERT(FALSE);
		return -4;
	}

	int nWinIDBindUIWndHandlerCount = (int)it->second.size();
	if (nWinIDBindUIWndHandlerCount <= 0)
	{
		ASSERT(FALSE);
		return -5;
	}

	BOOL bFound = FALSE;
	for (int i = 0; i < nWinIDBindUIWndHandlerCount; ++i)
	{
		CUIWndHandler *pUIWndHandler = it->second[i];

		if (pUIWndHandler == pHandler)
		{
			bFound = TRUE;

			it->second.erase(it->second.begin() + i);

			// If the second empty, remove the window id node from map.
			if (it->second.empty())
			{
				pThreadState->m_mapUIWndHandler.erase(it);
			}

			break;
		}
	}

	return bFound ? 0 : -6;
}

CUIWndHandler::CUIWndHandler(UINT uWinID)
	: m_uBindWinID(uWinID)
	, m_pBindUIWnd(NULL)
	, m_nMsgHandlerCount(0)
	, m_lParam(0)
{
	AddToUIWndHandlerList(this);
}

CUIWndHandler::~CUIWndHandler()
{
	DeleteFromUIWndHandlerList(this);
}

UINT CUIWndHandler::GetBindWindowID() const
{
	return m_uBindWinID;
}

CUIWnd *CUIWndHandler::GetBindUIWnd()
{
	return m_pBindUIWnd;
}

const CUIWnd *CUIWndHandler::GetBindUIWnd() const
{
	return m_pBindUIWnd;
}

LPARAM CUIWndHandler::SetUserData(LPARAM lParam)
{
	LPARAM lOldParam = m_lParam;
	m_lParam = lParam;
	return lOldParam;
}

LPARAM CUIWndHandler::GetUserData() const
{
	return m_lParam;
}

BOOL CUIWndHandler::PreTranslateMessage(CUIWnd *pUIWnd, MSG *pMsg)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

struct UIWNDMEMBER
{
	UIWNDMEMBER()
	{
		m_dwLastError = 0;

		m_bNoCodeUIMode = FALSE;
		m_bDragResize = FALSE;
		m_bUseLayeredWindow = TRUE;
		m_bIsUpdateLayeredWindow = FALSE;
		m_bAutoResetRgn = TRUE;
		m_bKeepAspectRatio = FALSE;
		m_lMaxHorRatio = 100;
		m_lMaxVerRatio = 100;
		m_nHitTestRet = HTCLIENT;
		m_ptLButtonDown.x = m_ptLButtonDown.y = 0;
		m_rcLButtonDown.SetRectEmpty();
		m_nTitleBarHeight = 24;

		m_pRectCtrlBind = NULL;

		m_sizeBkImg.cx = 0;
		m_sizeBkImg.cy = 0;

		m_nCXBorder = 4;
		m_nCYBorder = 4;
		m_sizeScroll = CSize(16, 16);

		bTransparentBk = FALSE;
		bDrawBKColor = FALSE;
		m_crBK = GetSysColor(COLOR_3DFACE);
		m_crMask = RGB(255, 0, 255);

		m_bLoadBkImageFromFile = TRUE;
		m_bResizePtTransparent = FALSE;

		m_himgOrigBk = NULL;	// the base bitmap background( there is no control on it ), store the image with alpha channel.
		m_himgBK = NULL;   // the base bitmap background( there is no control on it ), store the image without alpha channel or image with alpha blend bk color and m_himgOrigBk
		m_eBkImageResizeMode = IRM_9GRID;
		m_ptBkImageResize.x = m_ptBkImageResize.y = 2;
		m_lBkImageRepeatX = 1;
		m_lBkImageRepeatY = 1;

		m_bSetUIWndPropertiesOutside = FALSE;

		m_bCreateAsDialog = FALSE;
		m_hWndTop = NULL;
		m_nDefaultPushButtonID = IDOK;

		m_uDefaultMenuID = -1;

		// We use Handler mode(No UI Code)，there was no ON_COMMAND message macro.
		// So, define m_bAutoMenuEnable to FALSE
		m_bAutoMenuEnable = FALSE;

		m_hAccelTable = NULL;

		m_nCurStoryboardFrame = 0;
		m_bStoryboardEnabled = TRUE;
		m_nStoryboardFPS = 40;
	}

	virtual ~UIWNDMEMBER()
	{
		m_dwLastError = 0;

		m_bNoCodeUIMode = FALSE;
		m_bDragResize = FALSE;
		m_bUseLayeredWindow = TRUE;
		m_bIsUpdateLayeredWindow = FALSE;
		m_bAutoResetRgn = TRUE;
		m_bKeepAspectRatio = FALSE;
		m_lMaxHorRatio = 100;
		m_lMaxVerRatio = 100;
		m_nHitTestRet = HTCLIENT;
		m_ptLButtonDown.x = m_ptLButtonDown.y = 0;
		m_rcLButtonDown.SetRectEmpty();
		m_nTitleBarHeight = 24;

		m_pRectCtrlBind = NULL;

		m_sizeBkImg.cx = 0;
		m_sizeBkImg.cy = 0;

		m_nCXBorder = 4;
		m_nCYBorder = 4;
		m_sizeScroll = CSize(16, 16);

		bTransparentBk = FALSE;
		bDrawBKColor = FALSE;
		m_crBK = GetSysColor(COLOR_3DFACE);
		m_crMask = RGB(255, 0, 255);

		m_bLoadBkImageFromFile = TRUE;
		m_bResizePtTransparent = FALSE;

		ReleaseIUIImage(m_himgOrigBk);
		m_himgOrigBk = NULL;
		ReleaseIUIImage(m_himgBK);
		m_himgBK = NULL;

		m_bSetUIWndPropertiesOutside = FALSE;

		m_bCreateAsDialog = FALSE;
		m_hWndTop = NULL;
		m_nDefaultPushButtonID = IDOK;

		m_uDefaultMenuID = -1;
		m_bAutoMenuEnable = TRUE;

		m_hAccelTable = NULL;

		m_nCurStoryboardFrame = 0;
		m_bStoryboardEnabled = TRUE;
		m_nStoryboardFPS = 40;
	}

	BOOL m_bLoadBkImageFromFile;

	// for resizing the window
	BOOL m_bNoCodeUIMode;
	int m_nHitTestRet; // the result of Hit test the resizing window witch can be resizing by dragging its board. default to HTCLIENT;
	BOOL m_bDragResize; // If the window can be resized by dragging the board, default to FALSE
	BOOL m_bUseLayeredWindow;
	BOOL m_bIsUpdateLayeredWindow;
	BOOL m_bAutoResetRgn;	// Auto reset rgn while resizing the window.
	BOOL m_bKeepAspectRatio;	// keep ratio for default resize mode's control.
	LONG m_lMaxHorRatio;	// Default to 100, if more then 100, means the window layout the children while window width is from origin width to origin width * m_lMaxHorRatio / 100.
	LONG m_lMaxVerRatio;
	CSize m_sizeBkImg;
	CSize m_sizeScroll;
	int m_nCXBorder; // see SM_CXBORDER of GetSystemMetrics
	int m_nCYBorder; // see SM_CYBORDER of GetSystemMetrics
	CPoint m_ptLButtonDown;
	CRect m_rcLButtonDown;

	int m_nTitleBarHeight;

	// Client area
	CRect m_rcClientArea;	// volatile
	CRectCtrl *m_pRectCtrlBind;

	// Rgn when not use layered window
	CRgn m_rgnLT;
	CRgn m_rgnRT;
	CRgn m_rgnLD;
	CRgn m_rgnRD;
	std::vector<int> m_vLeft;
	std::vector<int> m_vTop;
	std::vector<int> m_vRight;
	std::vector<int> m_vDown;
	BOOL m_bResizePtTransparent;

	HIUIIMAGE m_himgOrigBk;	// the base bitmap background( there is no control on it ), store the image with alpha channel.
	HIUIIMAGE m_himgBK;   // the base bitmap background( there is no control on it ), store the image without alpha channel or image with alpha blend bk color and m_himgOrigBk
	CBitmap m_bmpCompatible;	// the compatible bitmap to CPaintDC in WM_PAINT message.
	IMAGE_RESIZE_MODE m_eBkImageResizeMode;
	POINT m_ptBkImageResize;
	LONG m_lBkImageRepeatX;
	LONG m_lBkImageRepeatY;

	BOOL bTransparentBk;
	BOOL bDrawBKColor;
	COLORREF m_crBK;	// if m_himgBK is NULL, use m_crBK to draw the window background.
	COLORREF m_crMask;

	std::vector<CChildInfo> m_vChildWndList;	// Only include child control created by LibUIDK.
	DWORD m_dwLastError;
	UIWNDPROPERTIES m_uiwndProperties;
	BOOL m_bSetUIWndPropertiesOutside;

	CString m_strTag;

	// record rect control binds child window,
	// button binds popup window,
	// TaskWndMgr binds sheet window. etc.
	std::map<CUIWnd *, int> m_mapBindWindows;

	// for Dialog
	BOOL m_bCreateAsDialog;
	HWND m_hWndTop;                 // top level parent window (may be disabled)
	int m_nDefaultPushButtonID;		// The default push button id to handle Enter key. default to IDOK

	// for menu
	UINT m_uDefaultMenuID;
	BOOL m_bAutoMenuEnable;
	// TRUE => menu items without handlers will be disabled

	HACCEL m_hAccelTable;

	// for storyboard
	int m_nCurStoryboardFrame;
	BOOL m_bStoryboardEnabled;
	int m_nStoryboardFPS;
};

/////////////////////////////////////////////////////////////////////////////
// CUIWnd

IMPLEMENT_DYNCREATE(CUIWnd, CWnd)

CUIWnd::CUIWnd(CWnd *pParentWnd/* = NULL*/)
{
	m_pParentWnd = pParentWnd;

	m_pMember = new UIWNDMEMBER;
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	IDD = -1;
	m_pUIWndHandler = NULL;
}

CUIWnd::~CUIWnd()
{
	IDD = -1;
	m_pUIWndHandler = NULL;

	if (m_pMember != NULL)
	{
		delete (UIWNDMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CUIWnd, CWnd)
	//{{AFX_MSG_MAP(CUIWnd)
	ON_WM_DESTROY()
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_PAINT()
	ON_WM_INITMENUPOPUP()
	ON_MESSAGE(WM_ISCHILDMOVE, OnChildMove) // can't move to WindowProc
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUIWnd message handlers

DWORD CUIWnd::GetLastCreateError()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_dwLastError;
}

class CShadowWnd : public CUIWnd
{
public:
	CShadowWnd()
	{
		m_bCreated = FALSE;
		m_pChildModalDlgMsgHandler = NULL;
	}
	~CShadowWnd() {}

protected:
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			CUIWnd::WindowProc(message, wParam, lParam);

			m_bCreated = TRUE;

			LONG lExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
			SetWindowLong(m_hWnd, GWL_EXSTYLE, lExStyle | WS_EX_TRANSPARENT);
			SetWindowText(_T("Shadow"));

			// 弹出高级设置对话框
			CUIWnd::PopupModalWindow(m_uModalDlgID, this, m_pChildModalDlgMsgHandler, m_uModalDlgID);

			// 当高级设置对话框返回（关闭）后，阴影窗口也应该销毁。
			return -1;
		}

		case WM_ISSHADOWCREATED:
			return m_bCreated;
		}

		return CUIWnd::WindowProc(message, wParam, lParam);
	}

public:
	int SetChildModalDlgHandler(CUIWndHandler *pMsgHandler)
	{
		m_pChildModalDlgMsgHandler = pMsgHandler;
		return 0;
	}

public:
	UINT m_uModalDlgID;
	BOOL m_bCreated;
	CUIWndHandler *m_pChildModalDlgMsgHandler;
};

int CUIWnd::PopupModalWindow(
	UINT nPopupWndID,
	CWnd *pWndParent,
	CUIWndHandler *pMsgHandler,
	LPARAM lParam)
{
	if (pWndParent == NULL)
	{
		return 0;
	}

	// create shadow window
	UIWNDPROPERTIES *pUIWndProp = CUIMgr::GetUIWNDPROPERTIES(nPopupWndID);
	if (pUIWndProp == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	CString strShadowImage = (LPCTSTR)pUIWndProp->m_strShadowImage;
	if (!strShadowImage.IsEmpty() && !(BOOL)pWndParent->SendMessage(WM_ISSHADOWCREATED))
	{
		CShadowWnd *pShadow = new CShadowWnd;
		UIWNDPROPERTIES *pShadowProp = new UIWNDPROPERTIES;
		pShadowProp->m_rcControl = CRect(0, 0, 1, 1);
		// 在窗口属性中，应该可以设置阴影窗口的一些属性，例如m_bAppWindow
		pShadowProp->m_bAppWindow = false;
		pShadowProp->m_bUpdateLayeredWindow = true;
		pShadowProp->m_strBKNo = (LPCTSTR)strShadowImage;
		pShadowProp->m_ptImageResize = pUIWndProp->m_ptShadowResize;
		pShadow->SetUIWndProperties(pShadowProp);

		pShadow->m_uModalDlgID = nPopupWndID;
		delete pShadowProp;

		int nRet = pShadow->DoModal();
		SafeDelete(pShadow);
		return nRet;
	}

	// create modal dialog
	CUIWnd *pBindWnd = new CUIWnd(pWndParent);
	pBindWnd->IDD = nPopupWndID;

	if (pMsgHandler != NULL)
	{
		pMsgHandler->m_pBindUIWnd = pBindWnd;
		pMsgHandler->SetUserData(lParam);
	}
	pBindWnd->m_pUIWndHandler = pMsgHandler;

	// Give a chance to reset the message handler.
	CHILDMSGHANDLER cmh;
	cmh.pUIWnd = pBindWnd;
	cmh.pUIWndHandler = pBindWnd->m_pUIWndHandler;

	pWndParent->SendMessage(WM_GETCHILDMSGHANDLER, pBindWnd->IDD, LPARAM(&cmh));

	pBindWnd->m_pUIWndHandler = cmh.pUIWndHandler;
	if (pBindWnd->m_pUIWndHandler != NULL)
	{
		pBindWnd->m_pUIWndHandler->m_pBindUIWnd = pBindWnd;
	}

	int nRet = pBindWnd->DoModal();

	// User can handle WM_BIND_DIALOG_END message to get some values in the binds handler.
	pWndParent->SendMessage(WM_BIND_DIALOG_END, (WPARAM)pBindWnd, nRet);

	// Give user a chance to release memory.
	BOOL bRet = (BOOL)pWndParent->SendMessage(WM_RELEASECHILDMSGHANDLER, pBindWnd->IDD, LPARAM(&cmh));
	if (bRet)
	{
		pBindWnd->m_pUIWndHandler = NULL;
	}

	//
	delete pBindWnd;
	pBindWnd = NULL;

	return nRet;
}

CUIWnd *CUIWnd::PopupModelessWindow(
	UINT nPopupWndID,
	CWnd *pWndNotify,
	CUIWndHandler *pMsgHandler,
	LPARAM lParam,
	CWnd *pWndParent)
{
	// create shadow window
	UIWNDPROPERTIES *pUIWndProp = CUIMgr::GetUIWNDPROPERTIES(nPopupWndID);
	if (pUIWndProp == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	CString strShadowImage = (LPCTSTR)pUIWndProp->m_strShadowImage;
	if (!strShadowImage.IsEmpty() && !(BOOL)pWndNotify->SendMessage(WM_ISSHADOWCREATED))
	{
		CShadowWnd *pShadow = new CShadowWnd;
		UIWNDPROPERTIES *pShadowProp = new UIWNDPROPERTIES;
		pShadowProp->m_rcControl = CRect(0, 0, 1, 1);
		// 在窗口属性中，应该可以设置阴影窗口的一些属性，例如m_bAppWindow
		pShadowProp->m_bAppWindow = false;
		pShadowProp->m_bUpdateLayeredWindow = true;
		pShadowProp->m_strBKNo = (LPCTSTR)strShadowImage;
		pShadowProp->m_ptImageResize = pUIWndProp->m_ptShadowResize;
		pShadow->SetUIWndProperties(pShadowProp);

		pShadow->m_uModalDlgID = nPopupWndID;
		delete pShadowProp;

		pShadow->SetChildModalDlgHandler(pMsgHandler);
		pShadow->LoadFrame(0, WS_VISIBLE, pWndNotify);
		delete pShadow;
		return 0;
	}

	CUIWnd *pBindWnd = new CUIWnd;
	pBindWnd->IDD = nPopupWndID;
	if (pMsgHandler != NULL)
	{
		pMsgHandler->m_pBindUIWnd = pBindWnd;
		pMsgHandler->SetUserData(lParam);
	}
	pBindWnd->m_pUIWndHandler = pMsgHandler;

	// Give a chance to reset the message handler.
	CHILDMSGHANDLER cmh;
	cmh.pUIWnd = pBindWnd;
	cmh.pUIWndHandler = pBindWnd->m_pUIWndHandler;

	pWndNotify->SendMessage(WM_GETCHILDMSGHANDLER, pBindWnd->IDD, LPARAM(&cmh));

	pBindWnd->m_pUIWndHandler = cmh.pUIWndHandler;
	if (pBindWnd->m_pUIWndHandler != NULL)
	{
		pBindWnd->m_pUIWndHandler->m_pBindUIWnd = pBindWnd;
	}

	pBindWnd->LoadFrame(0,
		WS_POPUP | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX,
		pWndParent);

	pBindWnd->ShowWindow(SW_SHOW);
	pBindWnd->Update();

	// Add the bind window pointer to map for release while the parent be destroy.
	pWndNotify->SendMessage(WM_ADD_BIND_UIWND, 0, (LPARAM)pBindWnd);

	return pBindWnd;
}

CUIWnd *CUIWnd::CreateChildWindow(
	LPCTSTR lpszWindowName,
	UINT nChildWndID,
	DWORD dwStyle,
	CWnd *pWndParent,
	CUIWndHandler *pMsgHandler,
	LPARAM lParam)
{
	if (pWndParent == NULL)
	{
		return NULL;
	}

	UIWNDPROPERTIES *pChildUIProp = CUIMgr::GetUIWNDPROPERTIES(nChildWndID);
	if (pChildUIProp == NULL)
	{
		return NULL;
	}

	CUIWnd *pBindWnd = new CUIWnd;
	pBindWnd->IDD = nChildWndID;
	if (pMsgHandler != NULL)
	{
		pMsgHandler->m_pBindUIWnd = pBindWnd;
		pMsgHandler->SetUserData(lParam);
	}
	pBindWnd->m_pUIWndHandler = pMsgHandler;

	// Give a chance to reset the message handler.
	CHILDMSGHANDLER cmh;
	cmh.pUIWnd = pBindWnd;
	cmh.pUIWndHandler = pBindWnd->m_pUIWndHandler;

	pWndParent->SendMessage(WM_GETCHILDMSGHANDLER, pBindWnd->IDD, LPARAM(&cmh));

	pBindWnd->m_pUIWndHandler = cmh.pUIWndHandler;
	if (pBindWnd->m_pUIWndHandler != NULL)
	{
		pBindWnd->m_pUIWndHandler->m_pBindUIWnd = pBindWnd;
	}

	RECT rcControl = (RECT)pChildUIProp->m_rcControl;
	BOOL bRet = pBindWnd->Create(NULL, lpszWindowName, dwStyle, rcControl, pWndParent, pBindWnd->IDD);

	// Add the bind window pointer to map
	pWndParent->SendMessage(WM_ADD_BIND_UIWND, 0, (LPARAM)pBindWnd);

	return pBindWnd;
}

CUIWnd *CUIWnd::CreateChildWindow(
	LPCTSTR lpszWindowName,
	UINT nChildWndID,
	DWORD dwStyle,
	CRectCtrl *pCtrlRect,
	CWnd *pWndParent,
	CUIWndHandler *pMsgHandler,
	LPARAM lParam)
{
	if (pWndParent == NULL)
	{
		return NULL;
	}

	if (pCtrlRect == NULL)
	{
		return NULL;
	}

	CUIWnd *pBindWnd = new CUIWnd;
	pBindWnd->IDD = nChildWndID;
	if (pMsgHandler != NULL)
	{
		pMsgHandler->m_pBindUIWnd = pBindWnd;
		pMsgHandler->SetUserData(lParam);
	}
	pBindWnd->m_pUIWndHandler = pMsgHandler;

	// Give a chance to reset the message handler.
	CHILDMSGHANDLER cmh;
	cmh.pUIWnd = pBindWnd;
	cmh.pUIWndHandler = pBindWnd->m_pUIWndHandler;

	pWndParent->SendMessage(WM_GETCHILDMSGHANDLER, pBindWnd->IDD, LPARAM(&cmh));

	pBindWnd->m_pUIWndHandler = cmh.pUIWndHandler;
	if (pBindWnd->m_pUIWndHandler != NULL)
	{
		pBindWnd->m_pUIWndHandler->m_pBindUIWnd = pBindWnd;
	}

	BOOL bRet = pBindWnd->Create(lpszWindowName, dwStyle, pCtrlRect, pWndParent, pBindWnd->IDD);

	// Add the bind window pointer to map for release while the parent be destroy.
	pWndParent->SendMessage(WM_ADD_BIND_UIWND, 0, (LPARAM)pBindWnd);

	return pBindWnd;
}

int CUIWnd::DeleteChildWindow(CUIWnd *pUIChildWnd)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;
	if (pMember->m_mapBindWindows.empty())
	{
		return -1;
	}

	std::map<CUIWnd *, int>::iterator it = pMember->m_mapBindWindows.find(pUIChildWnd);

	if (it != pMember->m_mapBindWindows.end())
	{
		CUIWnd *pBindWnd = it->first;

		// The bind window may be destroy. such as popup window.
		if (::IsWindow(pBindWnd->GetSafeHwnd()))
		{
			pBindWnd->DestroyWindow();
		}

		CHILDMSGHANDLER cmh;
		cmh.pUIWnd = pBindWnd;
		cmh.pUIWndHandler = pBindWnd->m_pUIWndHandler;

		BOOL bRet = (BOOL)SendMessage(WM_RELEASECHILDMSGHANDLER, pBindWnd->GetIDD(), LPARAM(&cmh));

		SafeDelete(pBindWnd);

		pMember->m_mapBindWindows.erase(it);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

UINT CUIWnd::GetIDD() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return IDD;
}

int CUIWnd::SetIDD(UINT nID)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	IDD = nID;

	return 0;
}

void CUIWnd::SetNoCodeUIMode(BOOL bNoCodeUIMode)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_bNoCodeUIMode = bNoCodeUIMode;
}

BOOL CUIWnd::IsNoCodeUIMode() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_bNoCodeUIMode;
}

BOOL CUIWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext /* = NULL */)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	BOOL bRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	return bRet;
}

BOOL CUIWnd::Create(LPCTSTR lpszWindowName, DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID, CCreateContext *pContext /* = NULL */)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	CRect rect;
	pRectCtrl->GetWindowRect(rect);

	BOOL bRet = CWnd::Create(NULL, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if (!bRet)
	{
		return FALSE;
	}

	pRectCtrl->AddBindWindow(this);
	pMember->m_pRectCtrlBind = pRectCtrl;

	return TRUE;
}

BOOL CUIWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, BOOL bLoadBkImageFromFile/*=TRUE*/, BOOL bUseLayeredWindow/*=TRUE*/)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_bLoadBkImageFromFile = bLoadBkImageFromFile;
	pMember->m_bUseLayeredWindow = bUseLayeredWindow;
	pMember->m_uDefaultMenuID = nIDResource;

	// attempt to create the window
	CString strMyClass;
	strMyClass = _T("UIWND");
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDResource));
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = (LPCTSTR) strMyClass;

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	BOOL bVisible = (dwDefaultStyle & WS_VISIBLE);
	if (bVisible)
	{
		dwDefaultStyle &= ~WS_VISIBLE;
	}

	CString strTitle;
	strTitle.LoadString(nIDResource);
	if (!CreateEx(0, strMyClass, strTitle, dwDefaultStyle, CRect(0, 0, 100, 100), pParentWnd, 0))
	{
		return FALSE;
	}

	// send initial update to all views (and other controls) in the frame
	SendMessageToDescendants(WM_IUIINITIALUPDATE, 0, 0, TRUE, TRUE);

	pMember->m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource));

	if (bVisible)
	{
		ShowWindow(SW_SHOW);
	}

	return TRUE;
}

void CUIWnd::OnDestroy()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	DestroyChildControls();
	ReleaseGdiObject();

	// Reset the UpdateLayeredWindow state
	pMember->m_bIsUpdateLayeredWindow = FALSE;
	pMember->m_nCurStoryboardFrame = 0;
	pMember->m_bStoryboardEnabled = TRUE;

	//
	::RemoveProp(m_hWnd, GETPROP_UIWND_STRINGID);
}

void CUIWnd::OnPaint()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rcClient;
	GetClientRect(rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	if (pMember->m_bmpCompatible.GetSafeHandle() != NULL)
	{
		pMember->m_bmpCompatible.DeleteObject();
	}
	pMember->m_bmpCompatible.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	ASSERT(pMember->m_bmpCompatible.GetSafeHandle() != NULL);
	CBitmap *pOldMemBmp = memDC.SelectObject(&pMember->m_bmpCompatible);

	OnDraw(&memDC);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldMemBmp);

	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CUIWnd::DestroyChildControls()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	int nChildCount = (int)pMember->m_vChildWndList.size();
	for (int i = 0; i < nChildCount; ++i)
	{
		CWnd *pChild = pMember->m_vChildWndList[i].m_pChildCtrl;
		if (pChild == NULL)
		{
			continue;
		}

		CONTROL_TYPE eControlType = pMember->m_vChildWndList[i].m_eControlType;

		if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK || eControlType == CT_RADIO)
		{
			((CSkinButton *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_COMBOBOX)
		{
			((CSkinComboBox *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_EDIT)
		{
			((CSkinEdit *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_STATIC)
		{
			((CSkinStatic *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_SLIDER)
		{
		}
		else if (eControlType == CT_IPADDRESS)
		{
		}
		else if (eControlType == CT_HOTKEY)
		{
		}
		else if (eControlType == CT_PROGRESS)
		{
			((CSkinProgressCtrl *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_SLIDEREX)
		{
			((CSliderCtrlEx *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_SPIN)
		{
		}
		else if (eControlType == CT_TREECTRL)
		{
		}
		else if (eControlType == CT_LISTCTRL)
		{
		}
		else if (eControlType == CT_SCROLLBAREX)
		{
		}
		else if (eControlType == CT_THUMBNAIL)
		{
		}
		else if (eControlType == CT_TREELIST)
		{
		}
		else if (eControlType == CT_RICHEDIT)
		{
		}
		else if (eControlType == CT_RICHEDIT_IM)
		{
		}
		else if (eControlType == CT_HTMLCTRL)
		{
		}
		else if (eControlType == CT_DATETIMECTRL)
		{
			((CSkinDateTimeCtrl *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_WAVECTRL)
		{
		}
		else if (eControlType == CT_PICTURE)
		{
		}
		else if (eControlType == CT_MENUBAR)
		{

		}
		else if (eControlType == CT_SPLITTER)
		{
			((CSplitterBar *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_WL_RECTCTRL)
		{
		}
		else if (eControlType == CT_WL_SPLITTER)
		{
		}
		else if (eControlType == CT_WL_LINE)
		{
		}
		else if (eControlType == CT_WL_TEXT)
		{
		}
		else if (eControlType == CT_WL_BUTTON
			|| eControlType == CT_WL_CHECK
			|| eControlType == CT_WL_RADIO)
		{
			((CWLButton *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_WL_SLIDER)
		{
		}
		else if (eControlType == CT_WL_PICTURE)
		{
		}
		else if (eControlType == CT_WL_RICHEDIT)
		{
		}
		else if (eControlType == CT_WL_RICHEDIT_IM)
		{
		}
		else if (eControlType == CT_TASK_WND_MGR)
		{
			((CTaskWndMgr *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_PNL_DOCK)
		{
			((CDockPanel *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_UNIFORM_GRID)
		{
			((CUniformGridPanel *)pChild)->ReleaseObject();
		}
		else if (eControlType == CT_PNL_STACK)
		{
			((CStackPanel *)pChild)->ReleaseObject();
		}
		else
		{
			ASSERT(FALSE);
		}

		if (eControlType == CT_WL_RECTCTRL
			|| eControlType == CT_WL_SPLITTER
			|| eControlType == CT_WL_LINE
			|| eControlType == CT_WL_TEXT
			|| eControlType == CT_WL_BUTTON
			|| eControlType == CT_WL_CHECK
			|| eControlType == CT_WL_RADIO
			|| eControlType == CT_WL_SLIDER
			|| eControlType == CT_WL_PICTURE
			|| eControlType == CT_WL_RICHEDIT
			|| eControlType == CT_WL_RICHEDIT_IM
			|| eControlType == CT_TASK_WND_MGR
			|| eControlType == CT_PNL_DOCK
			|| eControlType == CT_UNIFORM_GRID
			|| eControlType == CT_PNL_STACK)
		{
			CRectCtrl *pRectCtrl = (CRectCtrl *)pMember->m_vChildWndList[i].m_pChildCtrl;

			if (pRectCtrl->IsCreated())
			{
				pRectCtrl->DestroyWindow();
			}

			delete pRectCtrl;
		}
		else
		{
			CWnd *pChildWnd = pMember->m_vChildWndList[i].m_pChildCtrl;
			ASSERT(pChildWnd != NULL);

			// This child control may be destroyed.
			if (::IsWindow(pChildWnd->GetSafeHwnd()))
			{
				pChildWnd->DestroyWindow();
			}

			delete pChildWnd;
		}

		pMember->m_vChildWndList[i].m_pChildCtrl = NULL;
	}

	pMember->m_vChildWndList.clear();

	// destroy bind window
	std::map<CUIWnd *, int>::iterator it = pMember->m_mapBindWindows.begin();
	for (; it != pMember->m_mapBindWindows.end(); ++it)
	{
		CUIWnd *pBindWnd = it->first;

		// The bind window may be destroy. such as popup window.
		if (::IsWindow(pBindWnd->GetSafeHwnd()))
		{
			pBindWnd->DestroyWindow();
		}

		CHILDMSGHANDLER cmh;
		cmh.pUIWnd = pBindWnd;
		cmh.pUIWndHandler = pBindWnd->m_pUIWndHandler;

		BOOL bRet = (BOOL)SendMessage(WM_RELEASECHILDMSGHANDLER, pBindWnd->GetIDD(), LPARAM(&cmh));

		SafeDelete(pBindWnd);
	}
	pMember->m_mapBindWindows.clear();

	return TRUE;
}

int CUIWnd::CallLibUIDKHandlerMap(UINT message, WPARAM wParam, LPARAM lParam)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// Call the handler in handler map.

	if (m_pUIWndHandler == NULL)
	{
		// Must return 0 to continue message
		return 0;
	}

	const IUI_MSGHANDLER_MAP *pMsgMap = m_pUIWndHandler->GetMessageMap();
	if (pMsgMap == NULL)
	{
		// Must return 0 to continue message
		return 0;
	}

	DWORD dw = GetTickCount();

	// Use the last setting handler in handler map,
	for (int i = m_pUIWndHandler->m_nMsgHandlerCount - 1;
		i >= 0 && i < m_pUIWndHandler->m_nMsgHandlerCount;
		--i)
	{
		const IUI_MSGHANDLER_MAP *pMsg = pMsgMap + i;
		if (pMsg->nMessage != message)
		{
			continue;
		}

		IUI_PMSGHANDLER handler = pMsg->pfn;
		if (handler == NULL)
		{
			continue;
		}

		if (pMsg->nMessage == 0)
		{
			continue;
		}

		if (message == WM_COMMAND)
		{
			WORD nNotify = HIWORD(wParam);
			WORD nID = LOWORD(wParam);
			HWND hWndCtrl = (HWND)lParam;

			// Copy from CWnd::OnCommand
			if (hWndCtrl == NULL)
			{
				// zero IDs for normal commands are not allowed
				if (nID == 0)
				{
					continue;
				}

				// menu or accelerator
				nNotify = CN_COMMAND;
			}

			// pMsg->nLastID default is 0, mean handler ON_COMMAND,
			// if pMsg->nLastID isn't 0, mean handler ON_COMMAND_RANGE
			UINT nLastID = pMsg->nID;
			if (pMsg->nLastID != 0)
			{
				nLastID = pMsg->nLastID;
			}

			if (nNotify == pMsg->nCode && nID >= pMsg->nID && nID <= nLastID)
			{
				if (handler != NULL)
				{
					return (int)(m_pUIWndHandler->*handler)(this, wParam, lParam);
				}
				else
				{
					return 0;
				}
			}
		}
		else if (message == WM_NOTIFY)
		{
			UINT uCtrlID = (UINT)wParam;
			NMHDR *pnmhdr = (NMHDR *)lParam;

			UINT nLastID = pMsg->nID;
			if (pMsg->nLastID != 0)
			{
				nLastID = pMsg->nLastID;
			}

			if (pnmhdr->code == pMsg->nCode && uCtrlID >= pMsg->nID && uCtrlID <= nLastID)
			{
				if (handler != NULL)
				{
					return (int)(m_pUIWndHandler->*handler)(this, wParam, lParam);
				}
				else
				{
					return 0;
				}
			}
		}
		// General message
		else
		{
			if (handler != NULL)
			{
				return (int)(m_pUIWndHandler->*handler)(this, wParam, lParam);
			}
			else
			{
				return 0;
			}
		}
	}

	DWORD d2 = GetTickCount() - dw;

	return 0;
}

int CUIWnd::ApplyStoryboardItem(const CTRLPROPERTIES *pCtrlProp)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	int nID = CUIMgr::StringIDToInt(pCtrlProp->m_strID);
	CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;

	if (eControlType >= CT_PUSHBUTTON && eControlType <= CT_SPLITTER)	// control has HWND handle
	{
		CWnd *pCtrl = GetDlgItem(nID);
		if (pCtrl == NULL)
		{
			// Control's creation shouldn't be as a story frame, use show/hide control instead.
			ASSERT(FALSE);
		}
		else
		{
			LibUIDK::ApplyStoryboardItem(pCtrl, TRUE, pCtrlProp);
		}
	}
	else if (eControlType >= CT_WL_RECTCTRL && eControlType <= CT_PNL_STACK)
	{
		CWLWnd *pWLCtrl = GetDlgWLItem(nID);
		if (pWLCtrl == NULL)
		{
			ASSERT(FALSE);
		}
		else
		{
			LibUIDK::ApplyStoryboardItem(pWLCtrl, FALSE, pCtrlProp);
		}
	}

	return 0;
}

int GetMaskRgn(HIUIIMAGE hbmp, CPoint pt, CRgn &rgnLT, CRgn &rgnRT, CRgn &rgnLD, CRgn &rgnRD,
	std::vector<int> &vLeft, std::vector<int> &vTop, std::vector<int> &vRight, std::vector<int> &vDown,
	BOOL &bResizePtTransparent, COLORREF crMask)
{
	if (hbmp == NULL)
	{
		return -1;
	}

	CDC *pDC = AfxGetMainWnd()->GetWindowDC();
	CDC	memDC;
	memDC.CreateCompatibleDC(pDC);

	HBITMAP hOldMemBmp = (HBITMAP)memDC.SelectObject(hbmp);

	int w = hbmp->GetWidth();
	int h = hbmp->GetWidth();

	int x = 0;
	int y = 0;
	// left-top
	if (pt.x > 0 && pt.y > 0)
	{
		rgnLT.CreateRectRgn(0, 0, pt.x, pt.y);
		for (x = 0; x < pt.x; x++)
		{
			for (y = 0; y < pt.y; y++)
			{
				COLORREF crCur = memDC.GetPixel(x, y);
				if (crCur == crMask)
				{
					int y2 = y + 1;
					while (y2 < pt.y && memDC.GetPixel(x, y2) == crMask)
					{
						y2++;
					}

					CRgn rgnTemp;
					rgnTemp.CreateRectRgn(x, y, x + 1, y2);
					rgnLT.CombineRgn(&rgnLT, &rgnTemp, RGN_XOR);
					rgnTemp.DeleteObject();
					y = y2;
				}
			}
		}
	}

	// right-top
	if (pt.x < w - 1 && pt.y > 0)
	{
		rgnRT.CreateRectRgn(pt.x + 1, 0, w, pt.y);
		for (x = pt.x + 1; x < w; x++)
		{
			for (y = 0; y < pt.y; y++)
			{
				COLORREF crCur = memDC.GetPixel(x, y);
				if (crCur == crMask)
				{
					int y2 = y + 1;
					while (y2 < pt.y && memDC.GetPixel(x, y2) == crMask)
					{
						y2++;
					}

					CRgn rgnTemp;
					rgnTemp.CreateRectRgn(x, y, x + 1, y2);
					rgnRT.CombineRgn(&rgnRT, &rgnTemp, RGN_XOR);
					rgnTemp.DeleteObject();
					y = y2;
				}
			}
		}
	}

	// left-down
	if (pt.x > 0 && pt.y < h - 1)
	{
		rgnLD.CreateRectRgn(0, pt.y + 1, pt.x, h);
		for (x = 0; x < pt.x; x++)
		{
			for (y = pt.y + 1; y < h; y++)
			{
				COLORREF crCur = memDC.GetPixel(x, y);
				if (crCur == crMask)
				{
					int y2 = y + 1;
					while (y2 < h && memDC.GetPixel(x, y2) == crMask)
					{
						y2++;
					}

					CRgn rgnTemp;
					rgnTemp.CreateRectRgn(x, y, x + 1, y2);
					rgnLD.CombineRgn(&rgnLD, &rgnTemp, RGN_XOR);
					rgnTemp.DeleteObject();
					y = y2;
				}
			}
		}
	}

	// right-down
	if (pt.x < w - 1 && pt.y < h - 1)
	{
		rgnRD.CreateRectRgn(pt.x + 1, pt.y + 1, w, h);
		for (x = pt.x + 1; x < w; x++)
		{
			for (y = pt.y + 1; y < h; y++)
			{
				COLORREF crCur = memDC.GetPixel(x, y);
				if (crCur == crMask)
				{
					int y2 = y + 1;
					while (y2 < h && memDC.GetPixel(x, y2) == crMask)
					{
						y2++;
					}

					CRgn rgnTemp;
					rgnTemp.CreateRectRgn(x, y, x + 1, y2);
					rgnRD.CombineRgn(&rgnRD, &rgnTemp, RGN_XOR);
					rgnTemp.DeleteObject();
					y = y2;
				}
			}
		}
	}

	// left
	if (pt.x > 0)
	{
		for (x = 0; x < pt.x; ++x)
		{
			COLORREF crCur = memDC.GetPixel(x, pt.y);
			if (crCur != crMask)
			{
				vLeft.push_back(x);
			}
		}
	}
	// top
	if (pt.y > 0)
	{
		for (y = 0; y < pt.y; ++y)
		{
			COLORREF crCur = memDC.GetPixel(pt.x, y);
			if (crCur != crMask)
			{
				vTop.push_back(y);
			}
		}
	}
	// right
	if (pt.x < w - 1)
	{
		for (x = pt.x + 1; x < w; ++x)
		{
			COLORREF crCur = memDC.GetPixel(x, pt.y);
			if (crCur != crMask)
			{
				vRight.push_back(x);
			}
		}
	}
	// down
	if (pt.y < h - 1)
	{
		for (y = pt.y + 1; y < h; ++y)
		{
			COLORREF crCur = memDC.GetPixel(pt.x, y);
			if (crCur != crMask)
			{
				vDown.push_back(y);
			}
		}
	}

	// resize point
	bResizePtTransparent = (memDC.GetPixel(pt.x, pt.y) == crMask);

	if (hOldMemBmp != NULL)
	{
		memDC.SelectObject(hOldMemBmp);
	}
	if (pDC != NULL)
	{
		AfxGetMainWnd()->ReleaseDC(pDC);
	}

	return 0;
}

int CUIWnd::Init()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_nTitleBarHeight = (LONG)pMember->m_uiwndProperties.m_lTitleBarHeight;
	if (CUIMgr::IsEnableDPI())
	{
		long lTitleBarHeight = pMember->m_nTitleBarHeight;
		GetDPIPointY(lTitleBarHeight);
		pMember->m_nTitleBarHeight = lTitleBarHeight;
	}

	// Background transparent only for child window
	pMember->bTransparentBk = pMember->m_uiwndProperties.m_bTransparentBk;

	// Init background color
	pMember->bDrawBKColor = pMember->m_uiwndProperties.m_bDrawBKColor;
	CUIMgr::GetColorResource(pMember->m_uiwndProperties.m_strBackgoundColorResID, &pMember->m_crBK);
	CUIMgr::GetColorResource(pMember->m_uiwndProperties.m_strMaskColorResID, &pMember->m_crMask);

	// Init background image
	HIUIIMAGE himgN = NULL;
	HIUIIMAGE himgH = NULL;
	HIUIIMAGE himgS = NULL;
	HIUIIMAGE himgD = NULL;

	pMember->m_dwLastError = InitGdiObject(&pMember->m_himgBK, &himgN, &himgH, &himgS, &himgD);
	if (pMember->m_dwLastError > 0)
	{
		CString strDebug;
		strDebug.Format(_T("[LibUIDK]===InitGdiObject return %d\n"), pMember->m_dwLastError);
		OutputDebugString(strDebug);

		return -1;
	}

	// If the background has alpha channel, use pMember->m_himgOrigBk to store.
	// And generate pMember->m_himgBK by alpha blend background color and image
	// using AlphaBlendBkImageAndColor.
	if (pMember->m_himgBK->IsHasAlphaChannel() && pMember->bDrawBKColor)
	{
		pMember->m_himgOrigBk = pMember->m_himgBK;
		pMember->m_himgOrigBk->AddRef();
		AlphaBlendBkImageAndColor();
	}

	// Set the size of this window
	CRect rcUIWnd = pMember->m_uiwndProperties.m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rcUIWnd);
	}
	::SetWindowPos(m_hWnd, NULL, 0, 0, rcUIWnd.Width(), rcUIWnd.Height(), SWP_NOMOVE | SWP_NOZORDER);

	// Init the bitmap size
	if (pMember->m_himgBK->GetSafeHBITMAP() != NULL)
	{
		pMember->m_sizeBkImg.cx = pMember->m_himgBK->GetWidth();
		pMember->m_sizeBkImg.cy = pMember->m_himgBK->GetHeight();
	}

	// Keep children aspect ratio
	pMember->m_bKeepAspectRatio = pMember->m_uiwndProperties.m_bKeepChildrenAspectRatio;
	pMember->m_lMaxHorRatio = (LONG)pMember->m_uiwndProperties.m_lMaxHorRatio;
	pMember->m_lMaxVerRatio = (LONG)pMember->m_uiwndProperties.m_lMaxVerRatio;

	// Tag
	SetTag(pMember->m_uiwndProperties.m_strTag);

	// First stretch the background to the size as UI Editor.
	HIUIIMAGE himgStretchN = NULL;
	HIUIIMAGE himgStretchH = NULL;
	HIUIIMAGE himgStretchS = NULL;
	HIUIIMAGE himgStretchD = NULL;

	CIUIImage::ZoomImage(himgN,
		pMember->m_eBkImageResizeMode, pMember->m_ptBkImageResize,
		pMember->m_lBkImageRepeatX, pMember->m_lBkImageRepeatY,
		&himgStretchN, rcUIWnd.Width(), rcUIWnd.Height());
	CIUIImage::ZoomImage(himgH,
		pMember->m_eBkImageResizeMode, pMember->m_ptBkImageResize,
		pMember->m_lBkImageRepeatX, pMember->m_lBkImageRepeatY,
		&himgStretchH, rcUIWnd.Width(), rcUIWnd.Height());
	CIUIImage::ZoomImage(himgS,
		pMember->m_eBkImageResizeMode, pMember->m_ptBkImageResize,
		pMember->m_lBkImageRepeatX, pMember->m_lBkImageRepeatY,
		&himgStretchS, rcUIWnd.Width(), rcUIWnd.Height());
	CIUIImage::ZoomImage(himgD,
		pMember->m_eBkImageResizeMode, pMember->m_ptBkImageResize,
		pMember->m_lBkImageRepeatX, pMember->m_lBkImageRepeatY,
		&himgStretchD, rcUIWnd.Width(), rcUIWnd.Height());

	if (!CreateChildControls(&pMember->m_uiwndProperties, this,
			himgStretchN, himgStretchH, himgStretchS, himgStretchD, &pMember->m_vChildWndList))
	{
		pMember->m_dwLastError = E_CREATECHILD;

		CString strDebug;
		strDebug.Format(_T("[LibUIDK]===CreateChildControls return %d\n"), pMember->m_dwLastError);
		OutputDebugString(strDebug);

		return -1;
	}

	ReleaseIUIImage(himgN);
	ReleaseIUIImage(himgH);
	ReleaseIUIImage(himgS);
	ReleaseIUIImage(himgD);

	ReleaseIUIImage(himgStretchN);
	ReleaseIUIImage(himgStretchH);
	ReleaseIUIImage(himgStretchS);
	ReleaseIUIImage(himgStretchD);

	// If use SetLayeredWindowAttributes to make a no-rectangle window, there are 5 problems:
	// 1. can't get the screen snapshot by MSN shell or QQ snapshot tool.
	// 2. when use layered window, can't receive WM_PAINT message when another window cover over it and remove away.
	// 3. NOT redraw when its children show or hide(if the child not in screen).
	// 4. the Video plays by WMP control is showing error.
	// 5. when resizing the parent window and move its children, the children's side appear black block.
	// When use GDIPlus to draw the window, only RGN can applied to support mask color
	UseLayeredWindow(pMember->m_uiwndProperties.m_bLayeredWindow);

	return 0;
}

int CUIWnd::InitPath()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (IDD == -1 && !pMember->m_bSetUIWndPropertiesOutside)
	{
		WindowID();
	}

	if (!pMember->m_bSetUIWndPropertiesOutside)
	{
		CString strID = CUIMgr::StringIDFromInt(IDD);
		m_strID = strID;
		UIWNDPROPERTIES *pUIProp = CUIMgr::GetUIWNDPROPERTIES(strID);
		if (pUIProp != NULL)
		{
			// May be delay loaded.
			if (!BOOL(pUIProp->m_bLoaded))
			{
				CUIMgr::DelayLoadWindow(pUIProp);
			}

			pMember->m_uiwndProperties = *pUIProp;
		}
		pMember->m_rcClientArea = (RECT)pMember->m_uiwndProperties.m_rcClientArea;
	}

	return 0;
}

#define TILEX         1
#define TILEY         1

int CUIWnd::InitGdiObject(HIUIIMAGE *pbmpBase,
	HIUIIMAGE *pbmpN, HIUIIMAGE *pbmpH, HIUIIMAGE *pbmpS, HIUIIMAGE *pbmpD)
{
	if (pbmpBase == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (pbmpBase == NULL || pbmpN == NULL || pbmpH == NULL || pbmpD == NULL)
	{
		return -1;
	}

	ASSERT(pMember->m_himgBK == NULL);

	CString strBKN = (LPCTSTR)pMember->m_uiwndProperties.m_strBKN;

	// 1. load base background image (no controls on it)
	*pbmpBase = CreateEmptyIUIImage((LPCTSTR)pMember->m_uiwndProperties.m_strBKNo);
	if (*pbmpBase == NULL)
	{
		TRACE(_T("[LibUIDK]===Failed to Load background '%s' of window '%s', Try to load normal background '%s' instead.\n"),
			(LPCTSTR)pMember->m_uiwndProperties.m_strBKNo, (LPCTSTR)pMember->m_uiwndProperties.m_strID, (LPCTSTR)pMember->m_uiwndProperties.m_strBKN);
		// if the base background image if not exist, then load normal image for it.
		*pbmpBase = CreateEmptyIUIImage(strBKN);
	}
	if (*pbmpBase == NULL)
	{
		TRACE(_T("[LibUIDK]===Failed to Load normal background '%s' of window '%s' too.\n"),
			(LPCTSTR)pMember->m_uiwndProperties.m_strBKN, (LPCTSTR)pMember->m_uiwndProperties.m_strID);
	}

	pMember->m_eBkImageResizeMode = pMember->m_uiwndProperties.m_eBkImageResizeMode;
	pMember->m_ptBkImageResize = pMember->m_uiwndProperties.m_ptImageResize;
	pMember->m_lBkImageRepeatX = pMember->m_uiwndProperties.m_uXRepeatPixel;
	pMember->m_lBkImageRepeatY = pMember->m_uiwndProperties.m_uYRepeatPixel;

	// 2. Load the normal status background image, If load failed, the application will can't be run.
	*pbmpN = CreateEmptyIUIImage(strBKN);
	if (*pbmpN == NULL)
	{
		pMember->m_dwLastError = E_LOADBMPN;
	}

	// 3. Load the highlight status background image
	*pbmpH = CreateEmptyIUIImage((LPCTSTR)pMember->m_uiwndProperties.m_strBKH);
	if (*pbmpH == NULL)
	{
		pMember->m_dwLastError = E_LOADBMPH;
	}

	// 4. Load the selected (press down) status background image
	*pbmpS = CreateEmptyIUIImage((LPCTSTR)pMember->m_uiwndProperties.m_strBKS);
	if (*pbmpS == NULL)
	{
		pMember->m_dwLastError = E_LOADBMPS;
	}

	// 5. Load the disable status background image
	*pbmpD = CreateEmptyIUIImage((LPCTSTR)pMember->m_uiwndProperties.m_strBKD);
	if (*pbmpD == NULL)
	{
		pMember->m_dwLastError = E_LOADBMPD;
	}

	return 0;
}

int CUIWnd::ReleaseGdiObject()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	ReleaseIUIImage(pMember->m_himgBK);
	pMember->m_himgBK = NULL;

	if (pMember->m_rgnLT.GetSafeHandle() != NULL)
	{
		pMember->m_rgnLT.DeleteObject();
	}
	if (pMember->m_rgnRT.GetSafeHandle() != NULL)
	{
		pMember->m_rgnRT.DeleteObject();
	}
	if (pMember->m_rgnLD.GetSafeHandle() != NULL)
	{
		pMember->m_rgnLD.DeleteObject();
	}
	if (pMember->m_rgnRD.GetSafeHandle() != NULL)
	{
		pMember->m_rgnRD.DeleteObject();
	}

	return 0;
}

int CUIWnd::OnDraw(CDC *pMemDC)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	// If the child window background is transparent.
	LONG_PTR lStyle = IUIGetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bChild = ((lStyle & WS_CHILD) == WS_CHILD);
	if (bChild)
	{
		if (pMember->bTransparentBk)
		{
			// Is the top level window called UpdateLayeredWindow?
			HWND hWndToplevel = CWnd::GetSafeOwner_(m_hWnd, NULL);
			ASSERT(hWndToplevel != NULL);
			BOOL bUpdateLayeredWindow = (BOOL)::SendMessage(hWndToplevel, WM_ISUPDATELAYEREDWINDOW, 0, 0);

			if (!bUpdateLayeredWindow)
			{
				CWnd *pParent = GetParent();
				ASSERT(::IsWindow(pParent->GetSafeHwnd()));

				BOOL bCompatibleBitmap = TRUE;
				HBITMAP hParent = (HBITMAP)pParent->SendMessage(WM_GETBACKGROUND, 0, (LPARAM)&bCompatibleBitmap);
				if (hParent == NULL)
				{
					return -1;
				}

				CRect rcWin;
				GetWindowRect(rcWin);
				pParent->ScreenToClient(rcWin);

				if (bCompatibleBitmap)
				{
					BitBltG(pMemDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
						hParent, rcWin.left, rcWin.top, SRCCOPY);
				}
				else
				{
					// Resize the background
					POINT ptBkImageResize;
					ptBkImageResize.x = ptBkImageResize.y = 0;
					BOOL bRet = (BOOL)pParent->SendMessage(WM_GETBKIMGRESIZEPOINT,
							(WPARAM)m_hWnd, (LPARAM)&ptBkImageResize);

					POINT ptRepeatPixel;
					ptRepeatPixel.x = ptRepeatPixel.y = 0;
					bRet = (BOOL)pParent->SendMessage(WM_GETREPEATPIXEL,
							(WPARAM)m_hWnd, (LPARAM)&ptRepeatPixel);

					CRect rcClient;
					pParent->GetClientRect(rcClient);

					HBITMAP hBk = NULL;
					CIUIImage::NineGridBitmapG(hParent,
						pMember->m_ptBkImageResize, pMember->m_lBkImageRepeatX, pMember->m_lBkImageRepeatY,
						&hBk, rcClient.Width(), rcClient.Height());

					BitBltG(pMemDC->GetSafeHdc(),
						0, 0, rcWin.Width(), rcWin.Height(), hBk, rcWin.left, rcWin.top, SRCCOPY);

					SafeDeleteHBITMAP(hBk);
				}
			}
		}
	}

	// Draw background color
	if (pMember->m_himgBK->IsHasAlphaChannel())
	{
		if (pMember->bDrawBKColor)
		{
		}
	}
	else
	{
		// If not set background image, fill solid color.
		if (!bChild || !pMember->bTransparentBk)
		{
			if (pMember->m_himgBK == NULL)
			{
				pMemDC->FillSolidRect(rcClient, pMember->m_crBK);
			}
		}
	}

	// Draw background image
	if (pMember->m_himgBK->GetSafeHBITMAP() != NULL)
	{
		IUIDrawImage(pMemDC->GetSafeHdc(), rcClient, pMember->m_himgBK,
			pMember->m_eBkImageResizeMode, pMember->m_ptBkImageResize,
			pMember->m_lBkImageRepeatX, pMember->m_lBkImageRepeatY);
	}

	// Draw windowless children control.
	DrawWLChildren(pMemDC);

	// Draw children with direct-ui mode.
	DrawULWChildren(pMemDC);

	// Draw transparent rich edit for refuse flicker.
	HWND hRichEdit = ::FindWindowEx(m_hWnd, NULL, GetRichEditClassName(), NULL);
	while (hRichEdit != NULL)
	{
		::SendMessage(hRichEdit, WM_WL_CALLONDRAW, (WPARAM)pMemDC, 0);

		hRichEdit = ::FindWindowEx(m_hWnd, hRichEdit, GetRichEditClassName(), NULL);
	}

	return 0;
}

// while called UpdateLayeredWindow, draw the child win32 control.
int CUIWnd::DrawULWChildren(CDC *pMemDC)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;
	if (!pMember->m_bIsUpdateLayeredWindow)
	{
		return 1;
	}

	size_t nChildCount = (int)pMember->m_vChildWndList.size();
	for (size_t i = 0; i < nChildCount; ++i)
	{
		CWnd *pChild = pMember->m_vChildWndList[i].m_pChildCtrl;
		if (pChild->GetSafeHwnd() == NULL)
		{
			continue;
		}

		LONG_PTR lStyle = IUIGetWindowLong(pChild->GetSafeHwnd(), GWL_STYLE);
		BOOL bVisible = IsIncludeFlag(lStyle, WS_VISIBLE);
		if (bVisible)
		{
			pChild->SendMessage(WM_DRAWCONTROL, (WPARAM)pMemDC);

			// The following code can show the control's old view.
			// 			CRect rcCtrl;
			// 			pChild->GetWindowRect(rcCtrl);
			// 			ScreenToClient(rcCtrl);
			//
			// 			CDC memDC;
			// 			memDC.CreateCompatibleDC(pMemDC);
			// 			CIUIImage img;
			// 			img.Create(rcCtrl.Width(), rcCtrl.Height(), 32, CIUIImage::createAlphaChannel);
			// 			memDC.SelectObject(img);
			// 			pChild->SendMessage(WM_PRINTCLIENT, (WPARAM)memDC.GetSafeHdc());
			//
			// 			StretchAlphaBlend(pMemDC->GetSafeHdc(),
			// 				rcCtrl.left, rcCtrl.top,
			// 				rcCtrl.Width(), rcCtrl.Height(),
			// 				memDC.GetSafeHdc(),
			// 				0, 0,
			// 				rcCtrl.Width(), rcCtrl.Height());
		}
	}

	return 0;
}

int CUIWnd::DrawWLChildren(CDC *pMemDC)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CWLWnd *pChild = GetFirstWLWindow(m_hWnd);
	while (pChild != NULL)
	{
		if (!pChild->IsCreated())
		{
			continue;
		}

		if (pChild->IsWindowVisible())
		{
			// Apply child's region.
			HRGN hOldRgn = ApplyWLRgn(this, pMemDC, pChild);

			pChild->OnDraw(pMemDC);

			::SelectClipRgn(pMemDC->GetSafeHdc(), hOldRgn);
			DeleteObject(hOldRgn);
		}

		pChild = GetNextWLWindow(pChild, GW_HWNDNEXT);
	}

	return 0;
}

int CUIWnd::SetLayeredWindow(BOOL bSet)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// use layered window
	LibUIDK::SetLayeredWindow(m_hWnd, bSet);

	UpdateUIWindow();

	return 0;
}

// Alpha blend background color and background image with alpha channel, to make a new image without alpha channel for performance.
// After alpha blend, the pMember->m_himgBK will lost alpha channel
int CUIWnd::AlphaBlendBkImageAndColor()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (pMember->m_himgOrigBk->GetSafeHBITMAP() == NULL)
	{
		return 1;
	}

	CClientDC dc(this);

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(dc.GetSafeHdc(), OBJ_FONT);

	CDC dcMemDest;
	dcMemDest.CreateCompatibleDC(&dc);
	CBitmap bmpDest;
	bmpDest.CreateCompatibleBitmap(&dc,
		pMember->m_himgOrigBk->GetWidth(), pMember->m_himgOrigBk->GetHeight());
	CBitmap *pbmpOldDest = dcMemDest.SelectObject(&bmpDest);

	// Fill background color
	dcMemDest.FillSolidRect(0, 0,
		pMember->m_himgOrigBk->GetWidth(), pMember->m_himgOrigBk->GetHeight(),
		pMember->m_crBK);

	// Alpha blend background image
	IUIBitBlt(dcMemDest.GetSafeHdc(),
		0, 0, pMember->m_himgOrigBk->GetWidth(), pMember->m_himgOrigBk->GetHeight(),
		pMember->m_himgOrigBk, 0, 0, SRCCOPY);

	dcMemDest.SelectObject(pbmpOldDest);

	ReleaseIUIImage(pMember->m_himgBK);
	AddCustomImage(_T("IDC_IMG_BK"), (HBITMAP)bmpDest.Detach());
	pMember->m_himgBK = CreateIUIImage(_T("IDC_IMG_BK"));
	ReleaseIUIImage(_T("IDC_IMG_BK"));

	if (hFontOld != NULL)
	{
		::SelectObject(dc.GetSafeHdc(), hFontOld);
	}

	pMember->m_himgOrigBk->Destroy();

	return 0;
}

int CUIWnd::HandleTabKey(MSG *pMsg)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CWnd *pWndFocus = GetFocus();
	if (pWndFocus == NULL)
	{
		return 1;    //IsDialogMessage(pMsg);
	}

	BOOL bCtrl = GetKeyState(VK_CONTROL) < 0;
	BOOL bShift = GetKeyState(VK_SHIFT) < 0;

	int nFocusID = pWndFocus->GetDlgCtrlID();

	//
	// 1. If current focus control is a tab control in a CTaskWndMgr.
	// press Ctrl+Tab switch next tab, press Ctrl+Shift+tab switch to prev tab, Press Tab switch to the first child of task window.
	//

	// Find in all CTaskWndMgr
	CTaskWndInfo TaskInfo;
	CTaskWndMgr *pTaskWndMgr = FindTaskWindow(nFocusID, &TaskInfo);

	if (pTaskWndMgr != NULL)
	{
		// Loop between tabs
		if (bCtrl)
		{
			if (bShift)
			{
				pTaskWndMgr->SwitchToPrevTaskWindow();
			}
			else
			{
				pTaskWndMgr->SwitchToNextTaskWindow();
			}

			return 2;//CWnd::PreTranslateMessage(pMsg);
		}
		// Loop between controls
		else
		{
			if (!bShift)
			{
				if (TaskInfo.m_pTaskWnd->GetSafeHwnd() == NULL)
				{
					return 1;    //IsDialogMessage(pMsg);
				}

				CWnd *pFirstChild = TaskInfo.m_pTaskWnd->GetWindow(GW_CHILD);
				if (pFirstChild->GetSafeHwnd() == NULL)
				{
					return 1;    //IsDialogMessage(pMsg);
				}

				pFirstChild->SetFocus();
				return 3;//TRUE;
			}

			return 1;//IsDialogMessage(pMsg);
		}
	}

	//
	// 2. If the focus control is last control of task window. Press tab key switch to next tab-stop control in parent of task window.
	//
	CWnd *pWndLastTabstop = GetLastTabstopCtrl(NULL);
	if (pWndFocus->GetSafeHwnd() == pWndLastTabstop->GetSafeHwnd())
	{
		CTaskWndMgr *pTaskWndMgr = NULL;
		BOOL bHasTaskWndParent = IsControlInRadioTaskWnd(pWndFocus, &pTaskWndMgr);
		if (bHasTaskWndParent)
		{
			//pTaskWndMgr->GetParent()->GetDlgItem(pTaskWndMgr->GetCurrentTaskWindow())->SetFocus();
			CWnd *pRadio = pTaskWndMgr->GetParent()->GetDlgItem(pTaskWndMgr->GetCurrentTaskWindow());
			CWnd *pNextTabstop = pTaskWndMgr->GetParent()->GetNextDlgTabItem(pRadio, FALSE);
			if (pNextTabstop != NULL)
			{
				pNextTabstop->SetFocus();
				return 3;//TRUE;
			}
		}
	}

	return 1;
}

int CUIWnd::Update()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (pMember->m_bIsUpdateLayeredWindow)
	{
		UpdateUIWindow();
	}
	else
	{
		Invalidate();
		UpdateWindow();
	}

	return 0;
}

int CUIWnd::UpdateUIWindow(int nTransparent/*=255*/)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

#ifdef _DEBUG
	TRACE(_T("[LibUIDK]===Call UpdateUIWindow.\n"));
#endif

	// ::UpdateLayeredWindow only used with layered window.
	if (!IsLayeredWindow(m_hWnd))
	{
		return -1;
	}

	if (IsIconic())
	{
		return -2;
	}

	DWORD dw = GetTickCount();
	if (nTransparent < 0)
	{
		nTransparent = 0;
	}
	if (nTransparent > 255)
	{
		nTransparent = 255;
	}

	CRect rcWin;
	GetWindowRect(rcWin);

	// draw image to memory DC
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rcWin.Width(), rcWin.Height());
	CBitmap *pOldBmp = memDC.SelectObject(&bitmap);

	IUINineGridBlt(memDC.GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(), pMember->m_himgBK,
		pMember->m_ptBkImageResize, pMember->m_lBkImageRepeatX, pMember->m_lBkImageRepeatY);

	// Draw windowless controls
	DrawWLChildren(&memDC);

	// Draw children with direct-ui mode.
	DrawULWChildren(&memDC);

	// Update Layered Window
	POINT ptWinPos = {rcWin.left, rcWin.top};
	SIZE sizeWindow = {rcWin.Width(), rcWin.Height()};
	POINT ptSrc = {0, 0};

	BLENDFUNCTION blend;
	blend.BlendOp = 0x00;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = nTransparent;//1~255
	blend.AlphaFormat = 0;
	if (AF_AC_SRC_ALPHA == (ALPHA_FORMAT)pMember->m_uiwndProperties.m_eAlphaFormat)
	{
		blend.AlphaFormat = 0x01;    // AC_SRC_ALPHA;
	}

#if(_MSC_VER <= 1200) // VC6.0
	BOOL bRet = UpdateLayeredWindow(m_hWnd, pDC->GetSafeHdc(), &ptWinPos, &sizeWindow, memDC.GetSafeHdc(), &ptSrc, 0, &blend, 2);
#else
	BOOL bRet = ::UpdateLayeredWindow(m_hWnd, pDC->GetSafeHdc(), &ptWinPos, &sizeWindow, memDC.GetSafeHdc(), &ptSrc, 0, &blend, 2);
#endif
	pMember->m_bIsUpdateLayeredWindow = TRUE;

	memDC.SelectObject(pOldBmp);
	ReleaseDC(pDC);

	DWORD dw2 = GetTickCount() - dw;
	return bRet ? 0 : -1;
}

BOOL CUIWnd::PreCreateWindow(CREATESTRUCT &cs)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	InitPath();

	if (pMember->m_uiwndProperties.m_bClipChildren)
	{
		cs.style |= WS_CLIPCHILDREN;
	}
	if (pMember->m_uiwndProperties.m_bClipSiblings)
	{
		cs.style |= WS_CLIPSIBLINGS;
	}
	if (pMember->m_uiwndProperties.m_bTabStop)
	{
		cs.style |= WS_TABSTOP;
	}

#if(_WIN32_WINNT >= 0x0500)
	if (pMember->m_uiwndProperties.m_bNoActivate)
	{
		cs.dwExStyle |= WS_EX_NOACTIVATE;
	}
#endif /* _WIN32_WINNT >= 0x0500 */
	if (pMember->m_uiwndProperties.m_bAppWindow)
	{
		cs.dwExStyle |= WS_EX_APPWINDOW;
	}
	if (pMember->m_uiwndProperties.m_bTopmost)
	{
		cs.dwExStyle |= WS_EX_TOPMOST;
	}
	if (pMember->m_uiwndProperties.m_bAcceptFiles)
	{
		cs.dwExStyle |= WS_EX_ACCEPTFILES;
	}
	if (pMember->m_uiwndProperties.m_bToolWindow)
	{
		cs.dwExStyle |= WS_EX_TOOLWINDOW;
	}

	return CWnd::PreCreateWindow(cs);
}

// Support section508
BOOL CUIWnd::PreTranslateMessage(MSG *pMsg)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	// Call bind hanlder's PreTranslateMessage
	if (m_pUIWndHandler != NULL && m_pUIWndHandler->PreTranslateMessage(this, pMsg))
	{
		return TRUE;
	}

	// Following code copy from "BOOL CWnd::PreTranslateInput(LPMSG lpMsg)"
	// don't translate non-input events
	if ((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		(pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
	{
		return CWnd::PreTranslateMessage(pMsg);
	}

	if (pMember->m_bCreateAsDialog)
	{
		// The following code copy from CDialog::PreTranslateMessage in Dlgcore.cpp
		// fix around for VK_ESCAPE in a multiline Edit that is on a Dialog
		// that doesn't have a cancel or the cancel is disabled.
		if (pMsg->message == WM_KEYDOWN)
		{
			if ((pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL) &&
				(::GetWindowLong(pMsg->hwnd, GWL_STYLE) & ES_MULTILINE) &&
				IUICompareClassName(pMsg->hwnd, _T("Edit")))
			{
				HWND hItem = ::GetDlgItem(m_hWnd, IDCANCEL);
				if (hItem == NULL || ::IsWindowEnabled(hItem))
				{
					SendMessage(WM_COMMAND, IDCANCEL, 0);
					return TRUE;
				}
			}
			else if (pMsg->wParam == VK_TAB)
			{
				int nRet = HandleTabKey(pMsg);
				if (nRet == 1)
				{
					return IsDialogMessage(pMsg);
				}
				else if (nRet == 2)
				{
					return CWnd::PreTranslateMessage(pMsg);
				}
				else if (nRet == 3)
				{
					return TRUE;
				}
			}
		}

		// End copy
	}
	else
	{
		// The general window don't handle esc key.
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL)	// The general window NOT handle the escape key.
			{
				return CWnd::PreTranslateMessage(pMsg);
			}
			else if (pMsg->wParam == VK_TAB)
			{
				int nRet = HandleTabKey(pMsg);
				if (nRet == 1)
				{
					return IsDialogMessage(pMsg);
				}
				else if (nRet == 2)
				{
					return CWnd::PreTranslateMessage(pMsg);
				}
				else if (nRet == 3)
				{
					return TRUE;
				}
			}
		}
	}

	// Copy from CMDIFrameWnd::PreTranslateMessage
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators for frame and any children
		if (pMember->m_hAccelTable != NULL && ::TranslateAccelerator(m_hWnd, pMember->m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}

	// Press VK_SPACE, the IsDialogMessage will return TRUE, so CWnd::WalkPreTranslateTree will
	// NOT call the parent's PreTranslateMessage, so the parent can't reveive the VK_SPACE
	// in PreTranslateMessage.
	return IsDialogMessage(pMsg);
}

BOOL CUIWnd::AdjustChildControls()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// 	int nIndex = 0;
	// 	while (true)
	// 	{
	// 		CString strSection(m_strUIWndName);
	// 		CString strIndex;
	// 		strIndex.Format(_T("_%d"), nIndex++);
	// 		strSection += strIndex;
	//
	// 		TCHAR szType[32] = {0};
	// 		GetPrivateProfileString(strSection, _T("Type"), _T(""), szType, 31, CUIMgr::GetUIPath());
	// 		CString strType = szType;
	// 		if (!strType.IsEmpty())
	// 		{
	// 			// Adjust Button position
	// 			if (strType.CompareNoCase(PUSHBUTTON) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	// 				BOOL bTabStop = GetPrivateProfileInt(strSection, _T("IsTabStop"), 1, CUIMgr::GetUIPath());
	// 				BOOL bDefPushButton = GetPrivateProfileInt(strSection, _T("IsDefPushButton"), 0, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 					pWnd->MoveWindow(rect);
	// 			}
	// 			// Adjust Check box
	// 			if (strType.CompareNoCase(CHECK) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("Caption"), _T(""), szName, MAX_PATH - 1, CUIMgr::GetUIPath());
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	// 				BOOL bTabStop = GetPrivateProfileInt(strSection, _T("IsTabStop"), 1, CUIMgr::GetUIPath());
	// 				BOOL bMultiline = GetPrivateProfileInt(strSection, _T("IsMultiline"), 0, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 					pWnd->SetWindowText(szName);
	// 				}
	// 			}
	// 			// Adjust Combo box
	// 			if (strType.CompareNoCase(COMBOBOX) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	// 				int nComboStyle = GetPrivateProfileInt(strSection, _T("ComboStyle"), 1, CUIMgr::GetUIPath());
	// 				int nDropListHeight = GetPrivateProfileInt(strSection, _T("DropListHeight"), 16, CUIMgr::GetUIPath());
	// 				rect.bottom += nDropListHeight;
	// 				BOOL bAutoHScroll = GetPrivateProfileInt(strSection, _T("IsAutoHScroll"), 0, CUIMgr::GetUIPath());
	// 				BOOL bSort = GetPrivateProfileInt(strSection, _T("IsSort"), 1, CUIMgr::GetUIPath());
	// 				BOOL bUppercase = GetPrivateProfileInt(strSection, _T("IsUppercase"), 0, CUIMgr::GetUIPath());
	// 				BOOL bLowercase = GetPrivateProfileInt(strSection, _T("IsLowercase"), 0, CUIMgr::GetUIPath());
	// 				BOOL bTabStop = GetPrivateProfileInt(strSection, _T("IsTabStop"), 1, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust Edit
	// 			if (strType.CompareNoCase(EDIT) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				BOOL bR2LReadOrder = GetPrivateProfileInt(strSection, _T("IsR2LReadOrder"), 0, CUIMgr::GetUIPath());
	// 				BOOL bAutoHScroll = GetPrivateProfileInt(strSection, _T("IsAutoHScroll"), 1, CUIMgr::GetUIPath());
	// 				BOOL bAutoVScroll = GetPrivateProfileInt(strSection, _T("IsAutoVScroll"), 0, CUIMgr::GetUIPath());
	// 				BOOL bLowerCase = GetPrivateProfileInt(strSection, _T("IsLowerCase"), 0, CUIMgr::GetUIPath());
	// 				BOOL bUpperCase = GetPrivateProfileInt(strSection, _T("IsUpperCase"), 0, CUIMgr::GetUIPath());
	// 				BOOL bNumber = GetPrivateProfileInt(strSection, _T("IsNumber"), 0, CUIMgr::GetUIPath());
	// 				BOOL bPassword = GetPrivateProfileInt(strSection, _T("IsPassword"), 0, CUIMgr::GetUIPath());
	// 				BOOL bReadOnly = GetPrivateProfileInt(strSection, _T("IsReadOnly"), 0, CUIMgr::GetUIPath());
	// 				BOOL bMultiline = GetPrivateProfileInt(strSection, _T("IsMultiline"), 1, CUIMgr::GetUIPath());
	// 				BOOL bTabStop = GetPrivateProfileInt(strSection, _T("IsTabStop"), 1, CUIMgr::GetUIPath());
	// 				int nHorAligMode = GetPrivateProfileInt(strSection, _T("HorAligMode"), ES_LEFT, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust Radio button
	// 			if (strType.CompareNoCase(RADIO) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("Caption"), _T(""), szName, MAX_PATH - 1, CUIMgr::GetUIPath());
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	// 				int nGroup = GetPrivateProfileInt(strSection, _T("IsGroup"), 0, CUIMgr::GetUIPath());
	// 				BOOL bTabStop = GetPrivateProfileInt(strSection, _T("IsTabStop"), 0, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 					pWnd->SetWindowText(szName);
	// 				}
	// 			}
	// 			// Adjust Slider
	// 			if (strType.CompareNoCase(SLIDER) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	// 				int nVertical = GetPrivateProfileInt(strSection, _T("IsVertical"), 0, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust Static
	// 			if (strType.CompareNoCase(STATIC) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("Caption"), _T(""), szName, MAX_PATH - 1, CUIMgr::GetUIPath());
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust Progress
	// 			if (strType.CompareNoCase(PROGRESS) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	// 				int nVertical = GetPrivateProfileInt(strSection, _T("IsVertical"), 0, CUIMgr::GetUIPath());
	// 				int nShowText = GetPrivateProfileInt(strSection, _T("IsShowText"), 0, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust Spin button
	// 			if (strType.CompareNoCase(SPIN) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	// 				int nVertical = GetPrivateProfileInt(strSection, _T("IsVertical"), 1, CUIMgr::GetUIPath());
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust Hotkey
	// 			if (strType.CompareNoCase(HOTKEY) == 0)
	// 			{
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust IPAddress
	// 			if (strType.CompareNoCase(IPADDRESS) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust SliderEx
	// 			if (strType.CompareNoCase(SLIDEREX) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				// Is Vertical
	// 				int nVertical = GetPrivateProfileInt(strSection, _T("IsVertical"), 0, CUIMgr::GetUIPath());
	//
	// 				// Style
	// 				DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	// 				if (nVertical == 1)
	// 					dwStyle |= TBS_VERT;
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust ListCtrl
	// 			if (strType.CompareNoCase(LISTCTRL) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				// Style
	// 				int nListStyle = GetPrivateProfileInt(strSection, _T("ListStyle"), 0, CUIMgr::GetUIPath());
	// 				DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	// 				switch (nListStyle)
	// 				{
	// 				case 0:
	// 					dwStyle |= LVS_ICON;
	// 					break;
	// 				case 1:
	// 					dwStyle |= LVS_SMALLICON;
	// 					break;
	// 				case 2:
	// 					dwStyle |= LVS_LIST;
	// 					break;
	// 				case 3:
	// 					dwStyle |= LVS_REPORT;
	// 					break;
	// 				default:
	// 					break;
	// 				}
	// 				int nListAlign = GetPrivateProfileInt(strSection, _T("ListAlign"), 0, CUIMgr::GetUIPath());
	// 				switch (nListAlign)
	// 				{
	// 				case 0:
	// 					dwStyle |= LVS_ALIGNTOP;
	// 					break;
	// 				case 1:
	// 					dwStyle |= LVS_ALIGNLEFT;
	// 					break;
	// 				default:
	// 					break;
	// 				}
	// 				int nListSort = GetPrivateProfileInt(strSection, _T("ListSort"), 0, CUIMgr::GetUIPath());
	// 				switch (nListSort)
	// 				{
	// 				case 0:
	// 					break;
	// 				case 1:
	// 					dwStyle |= LVS_SORTASCENDING;
	// 					break;
	// 				case 2:
	// 					dwStyle |= LVS_SORTDESCENDING;
	// 					break;
	// 				default:
	// 					break;
	// 				}
	// 				// Is Auto Arrange
	// 				BOOL bAutoArrange = GetPrivateProfileInt(strSection, _T("IsAutoArrange"), 0, CUIMgr::GetUIPath());
	// 				if (bAutoArrange)
	// 					dwStyle |= LVS_AUTOARRANGE;
	// 				// Is Edit Labels
	// 				BOOL bEditLabels = GetPrivateProfileInt(strSection, _T("IsEditLabels"), 0, CUIMgr::GetUIPath());
	// 				if (bEditLabels)
	// 					dwStyle |= LVS_EDITLABELS;
	// 				// Is No Header
	// 				BOOL bNoHeader = GetPrivateProfileInt(strSection, _T("IsNoHeader"), 0, CUIMgr::GetUIPath());
	// 				if (bNoHeader)
	// 					dwStyle |= LVS_NOCOLUMNHEADER;
	// 				// Is No Label Wrap
	// 				BOOL bNoLabelWrap = GetPrivateProfileInt(strSection, _T("IsNoLabelWrap"), 0, CUIMgr::GetUIPath());
	// 				if (bNoLabelWrap)
	// 					dwStyle |= LVS_NOLABELWRAP;
	// 				// Is No Scroll
	// 				BOOL bNoScroll = GetPrivateProfileInt(strSection, _T("IsNoScroll"), 0, CUIMgr::GetUIPath());
	// 				if (bNoScroll)
	// 					dwStyle |= LVS_NOSCROLL;
	// 				// Is No Sort Header
	// 				BOOL bNoSortHeader = GetPrivateProfileInt(strSection, _T("IsNoSortHeader"), 0, CUIMgr::GetUIPath());
	// 				if (bNoSortHeader)
	// 					dwStyle |= LVS_NOSORTHEADER;
	// 				// Is Show Selection Always
	// 				BOOL bShowSelAlways = GetPrivateProfileInt(strSection, _T("IsShowSelAlways"), 0, CUIMgr::GetUIPath());
	// 				if (bShowSelAlways)
	// 					dwStyle |= LVS_SHOWSELALWAYS;
	// 				// Is Single Selection
	// 				BOOL bSingleSel = GetPrivateProfileInt(strSection, _T("IsSingleSel"), 0, CUIMgr::GetUIPath());
	// 				if (bSingleSel)
	// 					dwStyle |= LVS_SINGLESEL;
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			// Adjust TreeCtrl
	// 			if (strType.CompareNoCase(TREECTRL) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			if (strType.CompareNoCase(SCROLLBAREX) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			if (strType.CompareNoCase(THUMBNAIL) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			if (strType.CompareNoCase(TREELIST) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 			if (strType.CompareNoCase(RICHEDIT) == 0)
	// 			{
	// 				// Rect
	// 				TCHAR szRect[24] = {0};
	// 				GetPrivateProfileString(strSection, _T("Rect"), _T("0,0,0,0"), szRect, 23, CUIMgr::GetUIPath());
	// 				CRect rect = CUIMgr::MAKERECT(szRect);
	// 				TCHAR szName[MAX_PATH] = {0};
	//
	// 				// IDs
	// 				TCHAR szIDs[MAX_PATH] = {0};
	// 				GetPrivateProfileString(strSection, _T("IDs"), _T(""), szIDs, MAX_PATH-1, CUIMgr::GetUIPath());
	// 				int nID = CUIMgr::StringIDToInt(szIDs);
	//
	// 				CWnd *pWnd = GetDlgItem(nID);
	// 				if (pWnd->GetSafeHwnd() != NULL)
	// 				{
	// 					pWnd->MoveWindow(rect);
	// 				}
	// 			}
	// 		}
	// 		else
	// 		{
	// 			break;
	// 		}
	// 	}

	return TRUE;
}


int SetUIWndRgn(CWnd *pWnd, CRgn *pRgnLT, CRgn *pRgnRT, CRgn *pRgnLD, CRgn *pRgnRD,
	std::vector<int> &vLeft, std::vector<int> &vTop, std::vector<int> &vRight, std::vector<int> &vDown,
	BOOL bResizePtTransparent, int cx, int cy, int nOrgCX, int nOrgCY, int nResizingX, int nResizingY)
{
	CRgn rgn;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgn.CopyRgn(pRgnLT);

	CRgn rgnRT;
	rgnRT.CreateRectRgn(0, 0, 0, 0);
	rgnRT.CopyRgn(pRgnRT);

	CRgn rgnLD;
	rgnLD.CreateRectRgn(0, 0, 0, 0);
	rgnLD.CopyRgn(pRgnLD);

	CRgn rgnRD;
	rgnRD.CreateRectRgn(0, 0, 0, 0);
	rgnRD.CopyRgn(pRgnRD);

	int nAddWidth = cx - nOrgCX;
	int nAddHeigth = cy - nOrgCY;

	rgnRT.OffsetRgn(max(nAddWidth, 0), 0);
	rgnLD.OffsetRgn(0, max(nAddHeigth, 0));
	rgnRD.OffsetRgn(max(nAddWidth, 0), max(nAddHeigth, 0));

	size_t i = 0;
	size_t nSize = 0;
	// create RGN 1
	CRgn rgn1;
	nSize = vTop.size();
	if (nSize > 0)
	{
		rgn1.CreateRectRgn(0, 0, 0, 0);
		for (i = 0; i < nSize; ++i)
		{
			CRgn rgnTemp;
			rgnTemp.CreateRectRgn(nResizingX, vTop[i], nResizingX + nAddWidth + 1, vTop[i] + 1);
			rgn1.CombineRgn(&rgn1, &rgnTemp, RGN_OR);
		}
	}
	// create RGN 3
	CRgn rgn3;
	nSize = vLeft.size();
	if (nSize > 0)
	{
		rgn3.CreateRectRgn(0, 0, 0, 0);
		for (i = 0; i < nSize; ++i)
		{
			CRgn rgnTemp;
			rgnTemp.CreateRectRgn(vLeft[i], nResizingY, vLeft[i] + 1, nResizingY + nAddHeigth + 1);
			rgn3.CombineRgn(&rgn3, &rgnTemp, RGN_OR);
		}
	}
	// create RGN 4
	CRgn rgn4;
	if (!bResizePtTransparent)
	{
		rgn4.CreateRectRgn(nResizingX, nResizingY, nResizingX + nAddWidth + 1, nResizingY + nAddHeigth + 1);
	}
	// create RGN 5
	CRgn rgn5;
	nSize = vRight.size();
	if (nSize > 0)
	{
		rgn5.CreateRectRgn(0, 0, 0, 0);
		for (i = 0; i < nSize; ++i)
		{
			CRgn rgnTemp;
			rgnTemp.CreateRectRgn(vRight[i] + nAddWidth, nResizingY, vRight[i] + nAddWidth + 1, nResizingY + nAddHeigth + 1);
			rgn5.CombineRgn(&rgn5, &rgnTemp, RGN_OR);
		}
	}
	// create RGN 7
	CRgn rgn7;
	nSize = vDown.size();
	if (nSize > 0)
	{
		rgn7.CreateRectRgn(0, 0, 0, 0);
		for (i = 0; i < nSize; ++i)
		{
			CRgn rgnTemp;
			rgnTemp.CreateRectRgn(nResizingX, vDown[i] + nAddHeigth, nResizingX + nAddWidth + 1, vDown[i] + nAddHeigth + 1);
			rgn7.CombineRgn(&rgn7, &rgnTemp, RGN_OR);
		}
	}

	rgn.CombineRgn(&rgn, &rgnRT, RGN_OR);
	rgn.CombineRgn(&rgn, &rgnLD, RGN_OR);
	rgn.CombineRgn(&rgn, &rgnRD, RGN_OR);
	rgn.CombineRgn(&rgn, &rgn1, RGN_OR);
	rgn.CombineRgn(&rgn, &rgn3, RGN_OR);
	rgn.CombineRgn(&rgn, &rgn4, RGN_OR);
	rgn.CombineRgn(&rgn, &rgn5, RGN_OR);
	rgn.CombineRgn(&rgn, &rgn7, RGN_OR);
	pWnd->SetWindowRgn(rgn, TRUE);

	return 0;
}

LRESULT CUIWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

#ifdef _DEBUG
	LPCTSTR lpszMessage = CDebug::GetMessageText(message);
#endif // _DEBUG

	if (message == WM_CREATE)
	{
		if (Init() == -1)
		{
			return -1;
		}

		// when CWnd::WindowProc(message, wParam, lParam) calls CYourWnd::OnCreate, the CUIWnd has completed all tasks.
#ifdef TRIAL_MSG
		if (AfxGetMainWnd()->GetSafeHwnd() == m_hWnd)
		{
			AfxMessageBox(_T("This is a trial version, please register it from http://www.iuishop.com,\n and Email:1584793892@qq.com"));
		}
#endif // TRIAL_MSG

		::SetProp(m_hWnd, GETPROP_UIWND_STRINGID, (HANDLE)(LPCTSTR)pMember->m_uiwndProperties.m_strID);

		// Resizing border
		SetDragResizeFlag(pMember->m_uiwndProperties.m_bResizingBorder);

		// Start storyboard
		if (pMember->m_bStoryboardEnabled)
		{
			SetTimer(IDT_DELAYSTARTSTORY, 1000, NULL);    // After 1000ms start the animation
		}
	}

	else if (message == WM_WINDOWPOSCHANGED)
	{
		CWnd *pParent = GetParent();
		if (pParent != NULL)
		{
			BOOL bShadow = (BOOL)pParent->SendMessage(WM_ISSHADOWCREATED);
			if (bShadow)
			{
				WINDOWPOS *pPos = (WINDOWPOS *)lParam;

				CRect rcMargin = pMember->m_uiwndProperties.m_rcShadow;
				SyncHostWindow(this, pPos->flags, rcMargin, NULL);
			}
		}
	}

	else if (message == WM_TIMER)
	{
		UINT_PTR uID = UINT_PTR(wParam);
		if (IDT_DELAYSTARTSTORY == uID)
		{
			KillTimer(IDT_DELAYSTARTSTORY);

			PlayStoryboard();
		}
		else if (IDT_STORYBOARD == uID)
		{
			// If the story end, kill the timer.
			BOOL bStopStory = FALSE;
			int nFrameCount = (int)pMember->m_uiwndProperties.m_vStoryFrame.size();
			if (nFrameCount <= 0)
			{
				bStopStory = TRUE;
			}
			else
			{
				int nMaxFrame = pMember->m_uiwndProperties.m_vStoryFrame[nFrameCount - 1]->nFrameIndex;
				if (pMember->m_nCurStoryboardFrame > nMaxFrame)
				{
					//				// if loop story frame
					////				if (m_bLoopPlay)
					//				{
					//					pMember->m_nCurStoryboardFrame = 0;
					//				}
					//				//else
					//				//{
					bStopStory = TRUE;
					//}
				}
			}

			if (bStopStory)
			{
				KillTimer(IDT_STORYBOARD);
				SendMessage(WM_ENDSTORYBOARD, IDT_STORYBOARD, 0);
				return CWnd::WindowProc(message, wParam, lParam);
			}

			// Play the frame.
			if (pMember->m_nCurStoryboardFrame >= 0)	// Restore init status
			{
				STORY_FRAME *pStoryItem = pMember->m_uiwndProperties.FindStoryFrame(pMember->m_nCurStoryboardFrame);
				if (pStoryItem != NULL)
				{
					SendMessage(WM_BEFORESTORYFRAME, pMember->m_nCurStoryboardFrame);

					int nCtrlCount = (int)pStoryItem->m_vStoryFrameCtrlItem.size();
					for (int i = 0; i < nCtrlCount; ++i)
					{
						const CTRLPROPERTIES *pCtrlProp = pStoryItem->m_vStoryFrameCtrlItem[i];

						ApplyStoryboardItem(pCtrlProp);
					}
				}
			}

			if (BOOL(pMember->m_uiwndProperties.m_bUpdateLayeredWindow))
			{
				UpdateUIWindow();
			}

			pMember->m_nCurStoryboardFrame++;
		}
	}

	else if (message == WM_INITDIALOG)
	{
		if (pMember->m_bCreateAsDialog)
		{
			BOOL bRet = OnInitDialog();

			// Give message handler a chance.
			if (bRet)
			{
				BOOL nRet = (BOOL)CallLibUIDKHandlerMap(message, wParam, lParam);
				if (!nRet)
				{
					return nRet;
				}
			}

			return bRet;
		}
		else
		{
			ASSERT(FALSE);
			return 0;
		}
	}

	else if (message == WM_SYSCOMMAND)
	{
		if (SC_CLOSE == wParam)
		{
			if (pMember->m_bCreateAsDialog)
			{
				EndDialog(IDCANCEL);
			}
		}

		// Update the check status of Maximize and Restore box.
		if (SC_MAXIMIZE == wParam || SC_RESTORE == wParam)
		{
			size_t nChildrenCount = pMember->m_vChildWndList.size();
			for (size_t i = 0; i < nChildrenCount ; ++i)
			{
				CChildInfo *pci = &pMember->m_vChildWndList[i];
				if ((CONTROL_TYPE)pci->m_pCtrlProperties->m_eControlType != CT_CHECK)
				{
					continue;
				}

				CHKPROPERTIES *pp = (CHKPROPERTIES *)pci->m_pCtrlProperties;
				if (CT_MAXIMIZEBOX == (CHK_TYPE)pp->m_eChkType)
				{
					if (SC_MAXIMIZE == wParam)
					{
						((CSkinButton *)(pci->m_pChildCtrl))->SetCheck(BST_CHECKED);
					}
					else if (SC_RESTORE == wParam)
					{
						((CSkinButton *)(pci->m_pChildCtrl))->SetCheck(BST_UNCHECKED);
					}
					else
					{
						ASSERT(FALSE);
					}
				}
			}
		}
	}

	// Handle minimize box and close box
	// Handle bind popup window, modal dialog and task window
	else if (message == WM_COMMAND)
	{
		WORD nNotify = HIWORD(wParam);
		WORD nID = LOWORD(wParam);
		HWND hControl = (HWND)lParam;
		if (nNotify == BN_CLICKED)
		{
			if (IsWindow(hControl))		// If the command send by a control, the hControl isn't NULL
			{
				CONTROL_TYPE eControlType;
				::SendMessage(hControl, WM_GETCONTROLTYPE, 0, (LPARAM)(&eControlType));

				// Handle bind popup window, modal dialog
				if (eControlType == CT_PUSHBUTTON
					|| eControlType == CT_CHECK
					|| eControlType == CT_RADIO)
				{
					CSkinButton *pButton = (CSkinButton *)CWnd::FromHandle(hControl);
					UINT uBindDlgID = pButton->GetBindModalDlgID();
					if (uBindDlgID > 0)
					{
						CUIWnd::PopupModalWindow(uBindDlgID, this, NULL, 0);
					}

					UINT uBindModelessID = pButton->GetBindModelessWndID();
					if (uBindModelessID > 0)
					{
						CUIWnd::PopupModelessWindow(uBindModelessID, this, NULL, 0, NULL);
					}
				}

				// Minimize box or Close box
				if (eControlType == CT_PUSHBUTTON)
				{
					CSkinButton *pButton = (CSkinButton *)CWnd::FromHandle(hControl);
					BTN_TYPE eButtonType = pButton->GetPushButtonType();
					if (BT_MINIMIZEBOX == eButtonType)
					{
						SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
					}
					if (BT_CLOSEBOX == eButtonType)
					{
						if (pMember->m_bCreateAsDialog)
						{
							EndDialog(nID);
						}
						else
						{
							return SendMessage(WM_SYSCOMMAND, SC_CLOSE);
						}
					}
				}

				// Maximize box
				else if (eControlType == CT_CHECK)
				{
					CSkinButton *pButton = (CSkinButton *)CWnd::FromHandle(hControl);
					CHK_TYPE eCheckBoxType = pButton->GetCheckBoxType();
					if (CT_MAXIMIZEBOX == eCheckBoxType)
					{
						if (IsZoomed())
						{
							SendMessage(WM_SYSCOMMAND, SC_RESTORE);
						}
						else
						{
							if (!IsIconic())
							{
								SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
							}
						}
					}
				}

				// Tab in CTaskWndMgr
				else if (eControlType == CT_RADIO)
				{
					UINT nRadID = LOWORD(wParam);
					CSkinButton *pRadTab = (CSkinButton *)CWnd::FromHandle(hControl);

					// call SwitchToTaskWindow for each CTaskWndMgr
					size_t nChildCount = pMember->m_vChildWndList.size();
					for (size_t i = 0; i < nChildCount; ++i)
					{
						CChildInfo *pci = &pMember->m_vChildWndList[i];
						CONTROL_TYPE eControlType = pci->m_eControlType;
						if (eControlType != CT_TASK_WND_MGR)
						{
							continue;
						}

						CTaskWndMgr *pChild = (CTaskWndMgr *)pci->m_pChildCtrl;
						if (!pChild->IsCreated())
						{
							continue;
						}

						// If the bind window not add to task window list, add it.
						UINT uBindTaskWnd = pRadTab->GetBindTaskWindow();
						UINT uBindTaskMgr = pRadTab->GetBindTaskWndMgr();
						if (uBindTaskMgr > 0 && uBindTaskWnd > 0)
						{
							CTaskWndMgr *pBindTaskWndMgr = (CTaskWndMgr *)GetDlgWLItem(uBindTaskMgr);

							const CTaskWndInfo *pInfo = pBindTaskWndMgr->FindTaskWindow(nRadID);
							if (pInfo == NULL)
							{
								UIWNDPROPERTIES *pBindUIWndProp = CUIMgr::GetUIWNDPROPERTIES(uBindTaskWnd);
								pBindTaskWndMgr->AddTaskWindow(nRadID, TRUE, uBindTaskWnd,
									(pBindUIWndProp == NULL) ? NULL : (LPCTSTR)pBindUIWndProp->m_strID);
							}
						}

						// Switch to task window
						if (pChild->IsWindowVisible())
						{
							if (pChild->FindTaskWindow(nRadID) != NULL)
							{
								pChild->SwitchToTaskWindow(nRadID);
							}
						}
					}
				}
			}
			else	// Handle Esc key while as dialog, the hControl is NULL
			{
				if (pMember->m_bCreateAsDialog && nID == IDCANCEL)
				{
					EndDialog(IDCANCEL);
				}
			}
		}
	}

	else if (message == WM_KICKIDLE)
	{
		LONG lIdleCount = (LONG)lParam;

		if (this != NULL && m_hWnd != NULL && IsWindowVisible())
		{
			SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0, TRUE, TRUE);
		}
	}
	else if (message == WM_SIZE)
	{
		if (SIZE_MINIMIZED == wParam)
		{
			return CWnd::WindowProc(message, wParam, lParam);
		}
		else if (wParam == SIZE_RESTORED)	// Performance
		{
			// Call UpdateWindow to make WM_PAINT called,
			// and generate pMember->m_bmpCompatible. Because while minimized, the size of the ui window is 0.
			// the pMember->m_bmpCompatible not valid.
			// The following two call effect performance
			//			Invalidate();
			//			UpdateWindow();

			if (pMember->m_bIsUpdateLayeredWindow)
			{
				UpdateUIWindow();
			}

			size_t nChildCount = pMember->m_vChildWndList.size();
			for (size_t i = 0; i < nChildCount; ++i)
			{
				CWnd *pChild = pMember->m_vChildWndList[i].m_pChildCtrl; // pChild may be a windowless control pointer.

				CONTROL_TYPE eControlType = pMember->m_vChildWndList[i].m_eControlType;
				if (eControlType == CT_DATETIMECTRL)
				{
					pChild->SendMessage(WM_NCPAINT);
				}
			}
		}

		if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED || wParam == SIZE_MAXSHOW)
		{
			int nWindowCurWidth = LOWORD(lParam);
			int nWindowCurHeight = HIWORD(lParam);
			int cx = nWindowCurWidth;
			int cy = nWindowCurHeight;

			// GDI plus can only use background color
			if (cx < pMember->m_sizeBkImg.cx)
			{
				cx = pMember->m_sizeBkImg.cx;
			}
			if (cy < pMember->m_sizeBkImg.cy)
			{
				cy = pMember->m_sizeBkImg.cy;
			}

			// resize client area
			CRect rcOrigClientArea = pMember->m_uiwndProperties.m_rcClientArea;
			if (CUIMgr::IsEnableDPI())
			{
				GetDPIRect(rcOrigClientArea);
			}

			ResizeRect(rcOrigClientArea, pMember->m_rcClientArea);

			RelayoutChildren(this, &pMember->m_uiwndProperties,
				cx, cy, nWindowCurWidth, nWindowCurHeight, &pMember->m_vChildWndList,
				pMember->m_bKeepAspectRatio, pMember->m_lMaxHorRatio, pMember->m_lMaxVerRatio,
				CPoint(0, 0),
				FALSE);

			// reset RGN
			POINT ptWindowResize = pMember->m_uiwndProperties.m_bWindowResizePointSameAsImageResizePoint
				? pMember->m_uiwndProperties.m_ptImageResize
				: pMember->m_uiwndProperties.m_ptWindowResize;

			if (CUIMgr::IsEnableDPI())
			{
				GetDPIPoint(&ptWindowResize);
			}

			if (pMember->m_bAutoResetRgn
				&& !pMember->m_bUseLayeredWindow
				&& BOOL(pMember->m_uiwndProperties.m_bUseMaskColor))
			{
				SetUIWndRgn(this,
					&pMember->m_rgnLT, &pMember->m_rgnRT, &pMember->m_rgnLD, &pMember->m_rgnRD,
					pMember->m_vLeft, pMember->m_vTop, pMember->m_vRight, pMember->m_vDown,
					pMember->m_bResizePtTransparent,
					cx, cy, pMember->m_sizeBkImg.cx, pMember->m_sizeBkImg.cy,
					ptWindowResize.x, ptWindowResize.y);
			}
		}
	}

	// Change the default push button.
	else if (message == DM_SETDEFID)
	{
		pMember->m_nDefaultPushButtonID = (int)wParam;

		// The following code don't work.
		// ::DefDlgProc(m_hWnd, message, wParam, lParam);

		return TRUE;
	}
	else if (message == DM_GETDEFID)
	{
		LONG lRet = MAKELONG(pMember->m_nDefaultPushButtonID, DC_HASDEFID);
		return lRet;
	}

	// if the window can be resized by dragging its board, set its position while mouse moving.
	else if (message == WM_MOUSEMOVE)
	{
		CPoint point((DWORD)lParam);
		if (pMember->m_bDragResize)
		{
			CRect rect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
			MINMAXINFO MMI;
			CRect rcUIWnd = pMember->m_uiwndProperties.m_rcControl;

			// Maybe has a true border.
			CRect rcWnd;
			GetWindowRect(rcWnd);
			CRect rcClient;
			GetClientRect(rcClient);

			MMI.ptMinTrackSize.x = rcUIWnd.Width() + (rcWnd.Width() - rcClient.Width());
			MMI.ptMinTrackSize.y = rcUIWnd.Height() + (rcWnd.Height() - rcClient.Height());
			MMI.ptMaxSize.x = rect.Width();
			MMI.ptMaxSize.y = rect.Height();
			SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)(&MMI));

			CPoint pt = point;
			ClientToScreen(&pt);
			int nDx, nDy;
			nDx = pt.x - pMember->m_ptLButtonDown.x;
			nDy = pt.y - pMember->m_ptLButtonDown.y;

			int nNewW, nNewH;

			switch (pMember->m_nHitTestRet)
			{
			case HTTOPLEFT:
				nNewW = pMember->m_rcLButtonDown.Width() - nDx;
				if (nNewW < MMI.ptMinTrackSize.x)
				{
					nNewW = MMI.ptMinTrackSize.x;
				}
				nNewH = pMember->m_rcLButtonDown.Height() - nDy;
				if (nNewH < MMI.ptMinTrackSize.y)
				{
					nNewH = MMI.ptMinTrackSize.y;
				}
				SetWindowPos(NULL, pMember->m_rcLButtonDown.right - nNewW, pMember->m_rcLButtonDown.bottom - nNewH, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;
			case HTTOP:
				nNewW = pMember->m_rcLButtonDown.Width();
				nNewH = pMember->m_rcLButtonDown.Height() - nDy;
				if (nNewH < MMI.ptMinTrackSize.y)
				{
					nNewH = MMI.ptMinTrackSize.y;
				}
				SetWindowPos(NULL, pMember->m_rcLButtonDown.left, pMember->m_rcLButtonDown.bottom - nNewH, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;
			case HTTOPRIGHT:
				nNewW = pMember->m_rcLButtonDown.Width() + nDx;
				nNewH = pMember->m_rcLButtonDown.Height() - nDy;
				if (nNewH < MMI.ptMinTrackSize.y)
				{
					nNewH = MMI.ptMinTrackSize.y;
				}
				SetWindowPos(NULL, pMember->m_rcLButtonDown.left, pMember->m_rcLButtonDown.bottom - nNewH, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;
			case HTRIGHT:
				nNewW = pMember->m_rcLButtonDown.Width() + nDx;
				nNewH = pMember->m_rcLButtonDown.Height();
				SetWindowPos(NULL, pMember->m_rcLButtonDown.left, pMember->m_rcLButtonDown.top, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;
			case HTBOTTOMRIGHT:
				nNewW = pMember->m_rcLButtonDown.Width() + nDx;
				nNewH = pMember->m_rcLButtonDown.Height() + nDy;
				SetWindowPos(NULL, pMember->m_rcLButtonDown.left, pMember->m_rcLButtonDown.top, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;
			case HTBOTTOM:
				nNewW = pMember->m_rcLButtonDown.Width();
				nNewH = pMember->m_rcLButtonDown.Height() + nDy;
				SetWindowPos(NULL, pMember->m_rcLButtonDown.left, pMember->m_rcLButtonDown.top, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;
			case HTBOTTOMLEFT:
				nNewW = pMember->m_rcLButtonDown.Width() - nDx;
				if (nNewW < MMI.ptMinTrackSize.x)
				{
					nNewW = MMI.ptMinTrackSize.x;
				}
				nNewH = pMember->m_rcLButtonDown.Height() + nDy;
				SetWindowPos(NULL, pMember->m_rcLButtonDown.right - nNewW, pMember->m_rcLButtonDown.top, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;
			case HTLEFT:
				nNewW = pMember->m_rcLButtonDown.Width() - nDx;
				if (nNewW < MMI.ptMinTrackSize.x)
				{
					nNewW = MMI.ptMinTrackSize.x;
				}
				nNewH = pMember->m_rcLButtonDown.Height();
				SetWindowPos(NULL, pMember->m_rcLButtonDown.right - nNewW, pMember->m_rcLButtonDown.top, nNewW, nNewH, 0);
				if (pMember->m_bIsUpdateLayeredWindow)
				{
					UpdateUIWindow();
				}
				else
				{
					UpdateWindow();
				}
				break;

			case HTCLIENT:
				HitTest(point, TRUE);
				break;
			}
		}
	}

	else if (message == WM_CHANGESKIN)
	{
		ReleaseGdiObject();
		if (Init() != 0)
		{
			return -1;
		}

		if (!AdjustChildControls())
		{
			pMember->m_dwLastError = E_ADJUSTCHILD;
			return -1;
		}

		return 0;
	}

	else if (message == WM_GETBKIMGRESIZEPOINT)
	{
		POINT *ppt = (POINT *)lParam;

		if (ppt != NULL)
		{
			POINT pt = pMember->m_uiwndProperties.m_ptImageResize;

			*ppt = pt;

			return TRUE;
		}
	}
	else if (message == WM_GETREPEATPIXEL)
	{
		POINT *ppt = (POINT *)lParam;

		if (ppt != NULL)
		{
			ppt->x = pMember->m_uiwndProperties.m_uXRepeatPixel;
			ppt->y = pMember->m_uiwndProperties.m_uYRepeatPixel;

			return TRUE;
		}
	}

	else if (message == WM_ISNEEDRESIZE)
	{
		CRect *pRect = (CRect *)wParam;
		return IsChildNeedResize(pRect);
	}

	else if (message == WM_ISUIWND)
	{
		return TRUE;
	}

	// resize the window by dragging its board
	if (pMember->m_bDragResize)
	{
		if (message == WM_LBUTTONDOWN)
		{
			CPoint point((DWORD)lParam);

			pMember->m_nHitTestRet = HitTest(point, TRUE);
			if (pMember->m_nHitTestRet != HTCLIENT)
			{
				SetCapture();
				pMember->m_ptLButtonDown = point;
				ClientToScreen(&pMember->m_ptLButtonDown);
				GetWindowRect(&pMember->m_rcLButtonDown);
			}
		}

		if (message == WM_LBUTTONUP)
		{
			pMember->m_nHitTestRet = HTCLIENT;
			pMember->m_rcLButtonDown.SetRectEmpty();
			pMember->m_ptLButtonDown.x = pMember->m_ptLButtonDown.y = 0;
			ReleaseCapture();
		}

		if (message == WM_CAPTURECHANGED)
		{
			HWND hWndNew = HWND(lParam);
			if (hWndNew != m_hWnd)
			{
				pMember->m_nHitTestRet = HTCLIENT;
				pMember->m_rcLButtonDown.SetRectEmpty();
				pMember->m_ptLButtonDown.x = pMember->m_ptLButtonDown.y = 0;
			}
		}
	}

	if (message == WM_LBUTTONDOWN && 0 != pMember->m_nTitleBarHeight)
	{
		CPoint point((DWORD)lParam);

		pMember->m_nHitTestRet = HitTest(point, TRUE);

		if (pMember->m_nHitTestRet == HTCLIENT && !IsZoomed() && !IsIconic())
		{
			if ((pMember->m_nTitleBarHeight > 0 && point.y < pMember->m_nTitleBarHeight) || (pMember->m_nTitleBarHeight < 0))
			{
				SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			}
		}
	}

	if (message == WM_LBUTTONDBLCLK && pMember->m_nTitleBarHeight != 0)
	{
		CPoint point((DWORD)lParam);

		pMember->m_nHitTestRet = HitTest(point, TRUE);

		ReleaseCapture();
		pMember->m_ptLButtonDown = point;
		ClientToScreen(&pMember->m_ptLButtonDown);
		GetWindowRect(&pMember->m_rcLButtonDown);

		// Maximize or restore the window.
		if (pMember->m_nHitTestRet == HTCLIENT || pMember->m_nHitTestRet == HTTOP)
		{
			// Find maximize box
			BOOL bHasMaxBox = FALSE;
			size_t nChildrenCount = pMember->m_vChildWndList.size();
			for (size_t i = 0; i < nChildrenCount ; ++i)
			{
				CChildInfo *pci = &pMember->m_vChildWndList[i];
				if ((CONTROL_TYPE)pci->m_pCtrlProperties->m_eControlType != CT_CHECK)
				{
					continue;
				}

				CHKPROPERTIES *pp = (CHKPROPERTIES *)pci->m_pCtrlProperties;
				if (CT_MAXIMIZEBOX == (CHK_TYPE)pp->m_eChkType)
				{
					bHasMaxBox = TRUE;
					break;
				}
			}

			if (bHasMaxBox)
			{
				if ((pMember->m_nTitleBarHeight > 0 && point.y < pMember->m_nTitleBarHeight) || (pMember->m_nTitleBarHeight < 0))
				{
					if (IsZoomed())
					{
						SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
					}
					else
					{
						SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
					}
				}
			}
		}

		// hit test again.
		pMember->m_nHitTestRet = HTCLIENT;
	}

	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (message == WM_WINDOWPOSCHANGING)
	{
		WINDOWPOS *pPos = (WINDOWPOS *)lParam;

		BOOL bNoSize = ((pPos->flags & SWP_NOSIZE) == SWP_NOSIZE);
		if (!IsIconic() && !bNoSize)
		{
			CRect rcClient;
			GetClientRect(rcClient);

			// Show window from iconic mode, the rcClient is empty, But rcWnd has error value.
			if (!rcClient.IsRectEmpty())
			{
				CRect rect;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
				CRect rcUIWnd = pMember->m_uiwndProperties.m_rcControl;

				// Maybe has a true border.
				CRect rcWnd;
				GetWindowRect(rcWnd);

				MINMAXINFO MMI;
				MMI.ptMinTrackSize.x = rcUIWnd.Width() + (rcWnd.Width() - rcClient.Width());
				MMI.ptMinTrackSize.y = rcUIWnd.Height() + (rcWnd.Height() - rcClient.Height());
				MMI.ptMaxSize.x = rect.Width();
				MMI.ptMaxSize.y = rect.Height();
				SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)(&MMI));

				if (pPos->cx < MMI.ptMinTrackSize.x)
				{
					pPos->cx = MMI.ptMinTrackSize.x;
				}
				if (pPos->cy < MMI.ptMinTrackSize.y)
				{
					pPos->cy = MMI.ptMinTrackSize.y;
				}
				if (pPos->cx > MMI.ptMaxSize.x)
				{
					pPos->cx = MMI.ptMaxSize.x;
				}
				if (pPos->cy > MMI.ptMaxSize.y)
				{
					pPos->cy = MMI.ptMaxSize.y;
				}
			}
		}
	}
	// See <Document\Windows Programming\无边框程序最大化>
	else if (message == WM_GETMINMAXINFO)
	{
		MINMAXINFO *pMMI = (MINMAXINFO *)lParam;

		HMONITOR hCurMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFO mi = {0};
		mi.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hCurMonitor, &mi);

		pMMI->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
		pMMI->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
	}

	if (message == WM_UPDATELAYEREDUIWND)
	{
		UpdateUIWindow();
	}

	if (message == WM_ISUPDATELAYEREDWINDOW)
	{
		CWnd::WindowProc(message, wParam, lParam);
		return pMember->m_bIsUpdateLayeredWindow;
	}

	if (message == WM_GETBACKGROUND)
	{
		LRESULT lr = CWnd::WindowProc(message, wParam, lParam);	// Give derived class a chance.
		if (lr != 0)	// The derived class has handle.
		{
			return lr;
		}

		BOOL *pbCompatibleBitmap = (BOOL *)lParam;

		if (pMember->m_bmpCompatible.GetSafeHandle() == NULL)
		{
			if (pbCompatibleBitmap != NULL)
			{
				*pbCompatibleBitmap = FALSE;
			}

			return (LRESULT)(HBITMAP)pMember->m_himgBK->GetSafeHBITMAP();
		}
		else
		{
			if (pbCompatibleBitmap != NULL)
			{
				*pbCompatibleBitmap = TRUE;
			}

			return (LRESULT)pMember->m_bmpCompatible.GetSafeHandle();
		}
	}

	// If lParam is zero, the message was sent because of a call to the ShowWindow function
	if (message == WM_SHOWWINDOW && lParam == 0)
	{
		CWnd *pParent = GetParent();
		if (pParent != NULL)
		{
			pParent->SendMessage(WM_SHOWUIWINDOW, wParam, (LPARAM)this);
		}
	}

	// Handle splitter bar dragging.
	if (message == WM_DRAGGING_SPLITTERBAR)
	{
		SplitterStruct *pss = (SplitterStruct *)lParam;

		// Get the minimize size of the panel.
		CSize size;
		GetBackgroundImageSize(&size);
		CRect rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

		// Maybe has a true border.
		CRect rcWnd;
		GetWindowRect(rcWnd);
		CRect rcClient;
		GetClientRect(rcClient);

		MINMAXINFO MMI;
		MMI.ptMinTrackSize.x = size.cx + (rcWnd.Width() - rcClient.Width());
		MMI.ptMinTrackSize.y = size.cy + (rcWnd.Height() - rcClient.Height());
		MMI.ptMaxSize.x = rect.Width();
		MMI.ptMaxSize.y = rect.Height();
		SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)(&MMI));

		// The old position of the panel.
		GetParent()->ScreenToClient(rcWnd);

		// The new position of the splitter bar.
		CRect rcSplitterNew = pss->m_rcNew;
		GetParent()->ScreenToClient(rcSplitterNew);

		// Adjust the position of the panel
		if (pss->m_eDragOrientation == SDO_LEFT)
		{
			rcWnd.right = rcSplitterNew.left;
		}
		else if (pss->m_eDragOrientation == SDO_RIGHT)
		{
			rcWnd.left = rcSplitterNew.left;
		}
		else if (pss->m_eDragOrientation == SDO_UP)
		{
			rcWnd.bottom = rcSplitterNew.top;
		}
		else if (pss->m_eDragOrientation == SDO_DOWN)
		{
			rcWnd.top = rcSplitterNew.top;
		}

		// Refuse the size small than the minimize size of the panel.
		if (pss->m_eDragOrientation == SDO_LEFT || pss->m_eDragOrientation == SDO_RIGHT)
		{
			if (rcWnd.Width() < MMI.ptMinTrackSize.x)
			{
				int nSplitterWidth = rcSplitterNew.Width();
				if (pss->m_eDragOrientation == SDO_LEFT)
				{
					rcSplitterNew.left = rcWnd.left + MMI.ptMinTrackSize.x;
					rcSplitterNew.right = rcSplitterNew.left + nSplitterWidth;
				}
				else if (pss->m_eDragOrientation == SDO_RIGHT)
				{
					rcSplitterNew.left = rcWnd.right - MMI.ptMinTrackSize.x;
					rcSplitterNew.right = rcSplitterNew.left + nSplitterWidth;
				}

				GetParent()->ClientToScreen(rcSplitterNew);
				pss->m_rcNew = rcSplitterNew;

				return -1;	// refuse dragging
			}
		}
		else if (pss->m_eDragOrientation == SDO_UP || pss->m_eDragOrientation == SDO_DOWN)
		{
			if (rcWnd.Height() < MMI.ptMinTrackSize.y)
			{
				int nSplitterHeight = rcSplitterNew.Height();
				if (pss->m_eDragOrientation == SDO_UP)
				{
					rcSplitterNew.top = rcWnd.top + MMI.ptMinTrackSize.y;
					rcSplitterNew.bottom = rcSplitterNew.top + nSplitterHeight;
				}
				else if (pss->m_eDragOrientation == SDO_DOWN)
				{
					rcSplitterNew.top = rcWnd.bottom - MMI.ptMinTrackSize.y;
					rcSplitterNew.bottom = rcSplitterNew.top + nSplitterHeight;
				}

				GetParent()->ClientToScreen(rcSplitterNew);
				pss->m_rcNew = rcSplitterNew;

				return -1;	// refuse dragging
			}
		}

		return 0;
	}
	// Handle end drag the splitter bar.
	if (message == WM_ENDDRAG_SPLITTERBAR)
	{
		SplitterStruct *pss = (SplitterStruct *)lParam;

		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);

		CRect rect = pss->m_rcOld;		// The m_rcNew is beyond the valid rang.
		GetParent()->ScreenToClient(rect);

		if (pss->m_eDragOrientation == SDO_LEFT)
		{
			rcWnd.right = rect.left;
		}
		else if (pss->m_eDragOrientation == SDO_RIGHT)
		{
			rcWnd.left = rect.left;
		}
		else if (pss->m_eDragOrientation == SDO_UP)
		{
			rcWnd.bottom = rect.top;
		}
		else if (pss->m_eDragOrientation == SDO_DOWN)
		{
			rcWnd.top = rect.top;
		}

		MoveWindow(rcWnd);

		return 0;
	}

	if (message == WM_SETWLITEMTEXT)
	{
		CWLWnd *pWLChild = GetDlgWLItem((int)wParam);
		if (pWLChild == NULL)
		{
			return -1;
		}

		pWLChild->SetWindowText((LPCTSTR)lParam);

		return 0;
	}

	if (message == WM_ADD_BIND_UIWND)
	{
		CUIWnd *pUIWnd = (CUIWnd *)lParam;

		if (pUIWnd != NULL)
		{
			pMember->m_mapBindWindows[pUIWnd] = 1;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	int nRet = CallLibUIDKHandlerMap(message, wParam, lParam);
	if (nRet != 0)
	{
		return nRet;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

// Set the size of the scroll that resizing the window
int CUIWnd::SetScrollSize(CSize size)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);
	if (size.cx >= rcClient.Width() || size.cy >= rcClient.Height())
	{
		return -1;
	}

	pMember->m_sizeScroll = size;
	return 0;
}

// Get the size of the scroll that resizing the window
int CUIWnd::GetScrollSize(CSize *pSize)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	*pSize = pMember->m_sizeScroll;
	return 0;
}

BOOL CUIWnd::IsChildNeedResize(LPCRECT rect)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	POINT pt = pMember->m_uiwndProperties.m_bWindowResizePointSameAsImageResizePoint ? pMember->m_uiwndProperties.m_ptImageResize : pMember->m_uiwndProperties.m_ptWindowResize;
	BOOL bX = pt.x >= rect->left && pt.x <= rect->right;
	BOOL bY = pt.y >= rect->top && pt.y <= rect->bottom;

	return (bX || bY);
}

int CUIWnd::SetWindowResizePoint(const LPPOINT lpPt)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (lpPt == NULL)
	{
		return -1;
	}

	if (pMember->m_uiwndProperties.m_bWindowResizePointSameAsImageResizePoint)
	{
		return -2;
	}

	pMember->m_uiwndProperties.m_ptWindowResize = *lpPt;

	return 0;
}

int CUIWnd::GetWindowResizePoint(LPPOINT lpPt) const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (lpPt == NULL)
	{
		return -1;
	}

	if (pMember->m_uiwndProperties.m_bWindowResizePointSameAsImageResizePoint)
	{
		return -2;
	}

	*lpPt = pMember->m_uiwndProperties.m_ptWindowResize;

	return 0;
}

BOOL CUIWnd::IsKeepAspectRatio() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_bKeepAspectRatio;
}

LONG CUIWnd::GetMaxHorRatio() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_lMaxHorRatio;
}

LONG CUIWnd::GetMaxVerRatio() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_lMaxVerRatio;
}

int CUIWnd::SetBkColor(COLORREF crBk)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_crBK = crBk;

	AlphaBlendBkImageAndColor();

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return 0;
}

COLORREF CUIWnd::GetBkColor() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_crBK;
}

int CUIWnd::SetBkImage(LPCTSTR lpszImageName,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD, BOOL bResetSize/*=TRUE*/)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	IUISetControlImage(&pMember->m_himgBK, lpszImageName);

	if (bResetSize && pMember->m_himgBK != NULL)
	{
		pMember->m_sizeBkImg.cx = pMember->m_himgBK->GetWidth();
		pMember->m_sizeBkImg.cy = pMember->m_himgBK->GetHeight();
		::SetWindowPos(m_hWnd, NULL, 0, 0, pMember->m_sizeBkImg.cx, pMember->m_sizeBkImg.cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	//SetChildImage(lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	Invalidate();
	return 0;
}

int CUIWnd::SetBkImage(LPCTSTR lpszImageName)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	IUISetControlImage(&pMember->m_himgBK, lpszImageName);

	Invalidate();
	return 0;
}

HIUIIMAGE CUIWnd::GetBkImage()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_himgBK;
}

// when switch another skin, set the images for children
int CUIWnd::SetChildImage(const CIUIImage &bmpN, const CIUIImage &bmpH, const CIUIImage &bmpS, const CIUIImage &bmpD)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return -1;
}

int CUIWnd::HitTest(CPoint pt, BOOL bSetCursor)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CRect rc, rcClient;

	if (IsZoomed())
	{
		return HTCLIENT;
	}

	GetClientRect(&rcClient);
	rc.left = rc.top = 0;
	rc.right = rc.bottom = pMember->m_nCXBorder;

	int nRet = HTCLIENT;

	// left top
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		nRet = HTTOPLEFT;
		return nRet;
	}

	// top
	rc.left = pMember->m_nCXBorder + 1;
	rc.right = rcClient.right - pMember->m_nCXBorder - 1;
	rc.bottom = pMember->m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		nRet = HTTOP;
		return nRet;
	}

	// right top
	rc.left = rcClient.right - pMember->m_nCXBorder;
	rc.right = rcClient.right;
	rc.bottom = pMember->m_nCXBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		nRet = HTTOPRIGHT;
		return nRet;
	}

	// right
	rc.top = pMember->m_nCXBorder + 1;
	rc.bottom = rcClient.bottom - pMember->m_sizeScroll.cy - 1;
	rc.left = rcClient.right - pMember->m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		nRet = HTRIGHT;
		return nRet;
	}

	// right bottom
	rc.top = rcClient.bottom - pMember->m_sizeScroll.cy;
	rc.bottom = rcClient.bottom;
	rc.left = rcClient.right - pMember->m_sizeScroll.cx;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		nRet = HTBOTTOMRIGHT;
		return nRet;
	}

	// bottom
	rc.left = pMember->m_nCXBorder + 1;
	rc.right = rcClient.right - pMember->m_sizeScroll.cx - 1;
	rc.top = rcClient.bottom - pMember->m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		nRet = HTBOTTOM;
		return nRet;
	}

	// left bottom
	rc.left = 0;
	rc.right = pMember->m_nCXBorder;
	rc.top = rcClient.bottom - pMember->m_nCXBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		nRet = HTBOTTOMLEFT;
		return nRet;
	}

	// left
	rc.top = pMember->m_nCXBorder + 1;
	rc.bottom = rcClient.bottom - pMember->m_nCXBorder - 1;
	rc.right = pMember->m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		nRet = HTLEFT;
		return nRet;
	}

	if (bSetCursor)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	return nRet;
}

int CUIWnd::SetDragResizeFlag(BOOL bDragResize)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_bDragResize = bDragResize;
	return 0;
}

BOOL CUIWnd::GetDragResizeFlag() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_bDragResize;
}

int CUIWnd::SetXRepeatPixel(UINT nPixel)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_uiwndProperties.m_uXRepeatPixel = (LONG)nPixel;
	return 0;
}

UINT CUIWnd::GetXRepeatPixel() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return (UINT)(LONG)pMember->m_uiwndProperties.m_uXRepeatPixel;
}

int CUIWnd::SetYRepeatPixel(UINT nPixel)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_uiwndProperties.m_uYRepeatPixel = (LONG)nPixel;
	return 0;
}

UINT CUIWnd::GetYRepeatPixel() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return (UINT)(LONG)pMember->m_uiwndProperties.m_uYRepeatPixel;
}

int CUIWnd::GetBackgroundImageSize(CSize *pSize)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (pSize == NULL)
	{
		return -1;
	}

	*pSize = pMember->m_sizeBkImg;

	return 0;
}

int CUIWnd::GetOriginalClientAreaRect(LPRECT lpRect)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	CRect rcOrigClientArea = pMember->m_uiwndProperties.m_rcClientArea;
	*lpRect = rcOrigClientArea;

	return 0;
}

int CUIWnd::GetClientAreaRect(LPRECT lpRect)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	*lpRect = pMember->m_rcClientArea;

	return 0;
}

int CUIWnd::ResizeRect(LPCRECT lpRectSrc, LPRECT lpRectResized)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (lpRectSrc == NULL || lpRectResized == NULL)
	{
		return -1;
	}

	POINT pt = pMember->m_uiwndProperties.m_bWindowResizePointSameAsImageResizePoint ? pMember->m_uiwndProperties.m_ptImageResize : pMember->m_uiwndProperties.m_ptWindowResize;
	CRect rcSrc = *lpRectSrc;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcUIWndOrig = pMember->m_uiwndProperties.m_rcControl;

	*lpRectResized = *lpRectSrc;
	if (lpRectSrc->right > pt.x)
	{
		lpRectResized->right = rcClient.Width() - (rcUIWndOrig.Width() - lpRectSrc->right);
		if (lpRectSrc->left > pt.x)
		{
			lpRectResized->left = lpRectResized->right - rcSrc.Width();
		}
	}
	if (lpRectSrc->bottom > pt.y)
	{
		lpRectResized->bottom = rcClient.Height() - (rcUIWndOrig.Height() - lpRectSrc->bottom);
		if (lpRectSrc->top > pt.y)
		{
			lpRectResized->top = lpRectResized->bottom - rcSrc.Height();
		}
	}

	return 0;
}

CRectCtrl *CUIWnd::GetRectChild(int nRectID)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CRectCtrl *pRectCtrl = NULL;

	int nChildCount = (int)pMember->m_vChildWndList.size();

	for (int i = 0; i < nChildCount; ++i)
	{
		CONTROL_TYPE eControlType = pMember->m_vChildWndList[i].m_eControlType;
		if (eControlType != CT_WL_RECTCTRL)
		{
			continue;
		}

		CTRLPROPERTIES *pCtrlProp = pMember->m_vChildWndList[i].m_pCtrlProperties;
		int nID = LONG(pCtrlProp->m_nID);
		ASSERT(nID != -1);
		if (nRectID == nID)
		{
			pRectCtrl = (CRectCtrl *)pMember->m_vChildWndList[i].m_pChildCtrl;
			break;
		}
	}

	return pRectCtrl;
}

CWLWnd *CUIWnd::GetDlgWLItem(int nItemID)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CWLWnd *pWLItem = NULL;

	int nChildCount = (int)pMember->m_vChildWndList.size();

	for (int i = 0; i < nChildCount; ++i)
	{
		CONTROL_TYPE eControlType = pMember->m_vChildWndList[i].m_eControlType;
		if (eControlType != CT_WL_RECTCTRL
			&& eControlType != CT_WL_SPLITTER
			&& eControlType != CT_WL_LINE
			&& eControlType != CT_WL_TEXT
			&& eControlType != CT_WL_BUTTON
			&& eControlType != CT_WL_CHECK
			&& eControlType != CT_WL_RADIO
			&& eControlType != CT_WL_SLIDER
			&& eControlType != CT_WL_PICTURE
			&& eControlType != CT_WL_RICHEDIT
			&& eControlType != CT_WL_RICHEDIT_IM
			&& eControlType != CT_TASK_WND_MGR
			&& eControlType != CT_PNL_DOCK
			&& eControlType != CT_UNIFORM_GRID
			&& eControlType != CT_PNL_STACK)
		{
			continue;
		}

		CTRLPROPERTIES *pCtrlProp = pMember->m_vChildWndList[i].m_pCtrlProperties;
		int nID = LONG(pCtrlProp->m_nID);
		ASSERT(nID != -1);
		if (nItemID == nID)
		{
			pWLItem = (CWLWnd *)pMember->m_vChildWndList[i].m_pChildCtrl;
			break;
		}
	}

	return pWLItem;
}

int CUIWnd::SetLoadBkImageFromFile(BOOL bFromFile)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_bLoadBkImageFromFile = bFromFile;
	return 0;
}

BOOL CUIWnd::IsLoadBkImageFromFile() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_bLoadBkImageFromFile;
}

int CUIWnd::UseLayeredWindow(BOOL bUsed)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	if (bUsed)
	{
		// Add the window WS_EX_LAYERED style, and Set the window transparent
		LibUIDK::SetLayeredWindow(m_hWnd, bUsed);

		// Must call UpdateLayeredWindow or SetLayeredWindowAttributes to show the layered window.

		// Call UpdateLayeredWindow show the layered window.
		if (BOOL(pMember->m_uiwndProperties.m_bUpdateLayeredWindow))
		{
			UpdateUIWindow(pMember->m_uiwndProperties.m_btAlphaValue);
		}
		// Call SetLayeredWindowAttributes to set the opacity and transparency color key of a layered window.
		else
		{
			HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
			if (hInst)
			{
				typedef BOOL (WINAPI * MYFUNC)(HWND, COLORREF, BYTE, DWORD);
				MYFUNC SetLayeredWindowAttributes = NULL;
				SetLayeredWindowAttributes = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
				if (SetLayeredWindowAttributes)
				{
					DWORD dwFlags = 0;
					if (pMember->m_uiwndProperties.m_bUseMaskColor)
					{
						dwFlags |= LWA_COLORKEY;
					}
					if (pMember->m_uiwndProperties.m_bAlpha)
					{
						dwFlags |= LWA_ALPHA;
					}

					BOOL bRet = SetLayeredWindowAttributes(this->GetSafeHwnd(), pMember->m_crMask, pMember->m_uiwndProperties.m_btAlphaValue, dwFlags);
				}
				FreeLibrary(hInst);
			}
		}
	}
	else
	{
		// Remove the window WS_EX_LAYERED style, and Set the window transparent
		LibUIDK::SetLayeredWindow(m_hWnd, bUsed);

		if (pMember->m_uiwndProperties.m_bUseMaskColor)
		{
			POINT pt = pMember->m_uiwndProperties.m_bWindowResizePointSameAsImageResizePoint ? pMember->m_uiwndProperties.m_ptImageResize : pMember->m_uiwndProperties.m_ptWindowResize;
			// erase the mask color
			if (pMember->m_rgnLT.GetSafeHandle() == NULL && pMember->m_rgnRT.GetSafeHandle() == NULL && pMember->m_rgnLD.GetSafeHandle() == NULL && pMember->m_rgnRD.GetSafeHandle() == NULL)
			{
				GetMaskRgn(pMember->m_himgBK, pt, pMember->m_rgnLT, pMember->m_rgnRT, pMember->m_rgnLD, pMember->m_rgnRD,
					pMember->m_vLeft, pMember->m_vTop, pMember->m_vRight, pMember->m_vDown, pMember->m_bResizePtTransparent, pMember->m_crMask);
			}

			CRect rc;
			GetWindowRect(rc);
			SetUIWndRgn(this, &pMember->m_rgnLT, &pMember->m_rgnRT, &pMember->m_rgnLD, &pMember->m_rgnRD,
				pMember->m_vLeft, pMember->m_vTop, pMember->m_vRight, pMember->m_vDown, pMember->m_bResizePtTransparent,
				rc.Width(), rc.Height(), pMember->m_sizeBkImg.cx, pMember->m_sizeBkImg.cy, pt.x, pt.y);
		}
	}

	pMember->m_bUseLayeredWindow = bUsed;
	return 0;
}

BOOL CUIWnd::IsUseLayeredWindow() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_bUseLayeredWindow;
}

int CUIWnd::AutoResetRgn(BOOL bAuto)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_bAutoResetRgn = bAuto;

	return 0;
}

BOOL CUIWnd::IsAutoResetRgn() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_bAutoResetRgn;
}

UINT CUIWnd::GetDefaultMenuID()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_uDefaultMenuID;
}

CIUIFrameWnd *CUIWnd::GetParentFrame() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (GetSafeHwnd() == NULL) // no Window attached
	{
		return NULL;
	}

	ASSERT_VALID(this);

	CWnd *pParentWnd = GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		if (pParentWnd->SendMessage(WM_ISIUIFRAMEWND, 0, 0))
		{
			return (CIUIFrameWnd *)pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

BOOL CUIWnd::AutoMenuEnable(BOOL bAuto)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	BOOL bOld = pMember->m_bAutoMenuEnable;
	pMember->m_bAutoMenuEnable = bAuto;

	return bOld;
}

BOOL CUIWnd::IsAutoMenuEnable() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_bAutoMenuEnable;
}

CIUIFrameWnd *CUIWnd::GetTopLevelFrame() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (GetSafeHwnd() == NULL) // no Window attached
	{
		return NULL;
	}

	ASSERT_VALID(this);

	CIUIFrameWnd *pFrameWnd = (CIUIFrameWnd *)this;
	if (!::SendMessage(m_hWnd, WM_ISIUIFRAMEWND, 0, 0))
	{
		pFrameWnd = GetParentFrame();
	}

	if (pFrameWnd != NULL)
	{
		CIUIFrameWnd *pTemp;
		while ((pTemp = pFrameWnd->GetParentFrame()) != NULL)
		{
			pFrameWnd = pTemp;
		}
	}
	return pFrameWnd;
}

CIUIFrameWnd *CUIWnd::GetTopIUIFrame() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (GetSafeHwnd() == NULL) // no Window attached
	{
		return NULL;
	}

	ASSERT_VALID(this);

	CWnd *pTopWnd = GetTopWindow();
	while (pTopWnd != NULL)
	{
		if ((BOOL)::SendMessage(pTopWnd->m_hWnd, WM_ISIUIFRAMEWND, 0, 0))
		{
			break;
		}

		pTopWnd = pTopWnd->GetNextWindow();
	}

	return (CIUIFrameWnd *)pTopWnd;
}

CTaskWndMgr *CUIWnd::FindTaskWindow(CWnd *pTaskWnd, CTaskWndInfo *pTaskInfo)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// Find in all CTaskWndMgr
	CTaskWndMgr *pTaskWndMgrRet = NULL;

	size_t nChildCount = pMember->m_vChildWndList.size();
	for (size_t i = 0; i < nChildCount; ++i)
	{
		CChildInfo *pci = &pMember->m_vChildWndList[i];
		CONTROL_TYPE eControlType = pci->m_eControlType;
		if (eControlType != CT_TASK_WND_MGR)
		{
			continue;
		}

		CTaskWndMgr *pTaskWndMgr = (CTaskWndMgr *)pci->m_pChildCtrl;
		if (!pTaskWndMgr->IsCreated())
		{
			continue;
		}

		const CTaskWndInfo *pInfo = pTaskWndMgr->FindTaskWindow(pTaskWnd);
		if (pInfo != NULL)
		{
			pTaskWndMgrRet = pTaskWndMgr;

			if (pTaskInfo != NULL)
			{
				*pTaskInfo = *pInfo;
			}

			break;
		}
	}

	return pTaskWndMgrRet;
}

int CUIWnd::SwitchToTaskRadio(UINT uRadioID)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CWnd *pRadioButton = GetDlgItem(uRadioID);
	if (pRadioButton->GetSafeHwnd() == NULL)
	{
		return -1;
	}

	CONTROL_TYPE eControlType;
	pRadioButton->SendMessage(WM_GETCONTROLTYPE, 0, (LPARAM)(&eControlType));

	if (eControlType != CT_RADIO)
	{
		return -2;
	}

	// Simulate click the Radio button
	SendMessage(WM_COMMAND, MAKEWPARAM(uRadioID, BN_CLICKED), (LPARAM)pRadioButton->GetSafeHwnd());

	return 0;
}

CTaskWndMgr *CUIWnd::FindTaskWindow(int nTabRadioID, CTaskWndInfo *pTaskInfo)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// Find in all CTaskWndMgr
	CTaskWndMgr *pTaskWndMgrRet = NULL;

	size_t nChildCount = pMember->m_vChildWndList.size();
	for (size_t i = 0; i < nChildCount; ++i)
	{
		CChildInfo *pci = &pMember->m_vChildWndList[i];
		CONTROL_TYPE eControlType = pci->m_eControlType;
		if (eControlType != CT_TASK_WND_MGR)
		{
			continue;
		}

		CTaskWndMgr *pTaskWndMgr = (CTaskWndMgr *)pci->m_pChildCtrl;
		if (!pTaskWndMgr->IsCreated())
		{
			continue;
		}

		const CTaskWndInfo *pInfo = pTaskWndMgr->FindTaskWindow(nTabRadioID);
		if (pInfo != NULL)
		{
			pTaskWndMgrRet = pTaskWndMgr;

			if (pTaskInfo != NULL)
			{
				*pTaskInfo = *pInfo;
			}

			break;
		}
	}

	return pTaskWndMgrRet;
}

CWnd *CUIWnd::GetLastTabstopCtrl(BOOL *pbHWND)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	CWnd *pWndRet = NULL;

	//
	// 1. Search for all HWND children.
	//
	CWnd *pLast = GetWindow(GW_CHILD);
	if (pLast == NULL)
	{
		return pWndRet;
	}

	pLast = pLast->GetWindow(GW_HWNDLAST);
	while (pLast->GetSafeHwnd() != NULL)
	{
		LONG lStyle = GetWindowLong(pLast->GetSafeHwnd(), GWL_STYLE);
		if ((lStyle & WS_TABSTOP))
		{
			pWndRet = pLast;
			if (pbHWND != NULL)
			{
				*pbHWND = TRUE;
			}
			break;
		}

		pLast = pLast->GetWindow(GW_HWNDPREV);
	}

	if (pWndRet != NULL)
	{
		return pWndRet;
	}

	return pWndRet;
}

// Check if a control is a child of the task window manager witch tab is a radio button.
BOOL CUIWnd::IsControlInRadioTaskWnd(CWnd *pCtrl, CTaskWndMgr **ppTaskWndMgr)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	BOOL bFount = FALSE;
	CUIWnd *pParent = (CUIWnd *)pCtrl->GetParent();
	CUIWnd *pParentParent = (CUIWnd *)pParent->GetParent();
	while (pParentParent != NULL)
	{
		// Find in all CTaskWndMgr
		CTaskWndInfo ti;
		CTaskWndMgr *pTaskWndMgr = NULL;
		pTaskWndMgr = pParentParent->FindTaskWindow(pParent, &ti);

		if (pTaskWndMgr != NULL && ti.m_bRadioButton)
		{
			bFount = TRUE;

			if (ppTaskWndMgr != NULL)
			{
				*ppTaskWndMgr = pTaskWndMgr;
			}

			break;
		}

		pParent = pParentParent;
		pParentParent = (CUIWnd *)pParentParent->GetParent();
	}

	return bFount;
}

int CUIWnd::DisableStoryboard()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_bStoryboardEnabled = FALSE;

	return 0;
}

int CUIWnd::SetStoryboardFPS(int nFPS)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (nFPS <= 0)
	{
		return -1;
	}

	pMember->m_nStoryboardFPS = nFPS;

	return 0;
}

int CUIWnd::GetStoryboardFPS() const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_nStoryboardFPS;
}

int CUIWnd::PlayStoryboard()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_nCurStoryboardFrame = 0;
	SetTimer(IDT_STORYBOARD, 1000 / pMember->m_nStoryboardFPS, NULL);

	return 0;
}

int CUIWnd::SeekToFrame(int nFrame)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	int nCurStoryboardFrame = 0;
	int nFrameCount = (int)pMember->m_uiwndProperties.m_vStoryFrame.size();
	if (nFrameCount <= 0)
	{
		return 1;
	}

	int nMaxFrameIndex = pMember->m_uiwndProperties.m_vStoryFrame[nFrameCount - 1]->nFrameIndex;

	while (true)
	{
		if (nCurStoryboardFrame > nMaxFrameIndex)
		{
			break;
		}

		if (nCurStoryboardFrame > nFrame)
		{
			break;
		}

		STORY_FRAME *pStoryItem = pMember->m_uiwndProperties.FindStoryFrame(nCurStoryboardFrame);
		if (pStoryItem == NULL)
		{
			continue;
		}

		int nCtrlCount = (int)pStoryItem->m_vStoryFrameCtrlItem.size();
		for (int i = 0; i < nCtrlCount; ++i)
		{
			const CTRLPROPERTIES *pCtrlProp = pStoryItem->m_vStoryFrameCtrlItem[i];

			ApplyStoryboardItem(pCtrlProp);
		}

		nCurStoryboardFrame++;
	}

	if (BOOL(pMember->m_uiwndProperties.m_bUpdateLayeredWindow))
	{
		UpdateUIWindow();
	}

	return 0;
}

LRESULT CUIWnd::OnChildMove(WPARAM wParam, LPARAM lParam)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return TRUE;
}

void CUIWnd::OnInitMenuPopup(CMenu *pMenu, UINT nIndex, BOOL bSysMenu)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	AfxCancelModes(m_hWnd);

	if (bSysMenu)
	{
		return;    // don't support system menu
	}

	// #ifndef _AFX_NO_OLE_SUPPORT
	// 	// allow hook to consume message
	// 	if (m_pNotifyHook != NULL)
	// 	{
	// #ifdef _AFXDLL
	// 		ASSERT(m_pModuleState != NULL);
	// 		if (m_pModuleState->m_dwVersion >= 0x423)
	// #endif
	// 			if (m_pNotifyHook->OnInitMenuPopup(pMenu, nIndex, bSysMenu))
	// 				return;
	// 	}
	// #endif

	ASSERT(pMenu != NULL);
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pMenu->m_hMenu)
	{
		state.m_pParentMenu = pMenu;    // parent == child for tracking popup
	}
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd *pParent = GetTopLevelParent();
		// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
		{
			continue;    // menu separator or invalid cmd - ignore it
		}

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT) - 1)	// Pop-up menu item
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT) - 1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'pMember->m_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, pMember->m_bAutoMenuEnable && state.m_nID < 0xF000);
		}

		// adjust for menu deletions and additions
		UINT nCount = pMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

int CUIWnd::SetUIWndProperties(const UIWNDPROPERTIES *pProperties)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_uiwndProperties = *pProperties;
	pMember->m_bSetUIWndPropertiesOutside = TRUE;

	return 0;
}

const UIWNDPROPERTIES *CUIWnd::GetUIWndProperties()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return &pMember->m_uiwndProperties;
}

int CUIWnd::SetTag(LPCTSTR lpszTag)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (lpszTag == NULL)
	{
		return -1;
	}

	pMember->m_strTag = lpszTag;

	return 0;
}

int CUIWnd::GetTag(CString &strTagContent) const
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	strTagContent = pMember->m_strTag;

	return 0;
}

// Get width of a window border, in pixels.
int CUIWnd::GetCXBorder()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_nCXBorder;
}

// Set width of a window border, in pixels.
int CUIWnd::SetCXBorder(int cx)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_nCXBorder = cx;
	return 0;
}

// Get height of a window border, in pixels.
int CUIWnd::GetCYBorder()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	return pMember->m_nCYBorder;
}

// Set height of a window border, in pixels.
int CUIWnd::SetCYBorder(int cy)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_nCYBorder = cy;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// For dialog

int CUIWnd::DoModal(DWORD dwStyle/* = 0*/)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	pMember->m_bCreateAsDialog = TRUE;
	// can be constructed with a resource template or InitModalIndirect

	// load resource as necessary

	// return -1 in case of failure to load the dialog template resource

	// disable parent (before creating dialog)
	HWND hWndParent = PreModal();
	AfxUnhookWindowCreate();
	BOOL bEnableParent = FALSE;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}

	TRY
	{
		// create modeless dialog
		AfxHookWindowCreate(this);

		TCHAR szDlgClass[] = _T("UIDIALOG");
		WNDCLASS wndcls;
		memset(&wndcls, 0, sizeof(WNDCLASS));
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.hInstance = AfxGetInstanceHandle();
		wndcls.hIcon = NULL;
		wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = szDlgClass;

		// Register the new class and exit if it fails
		if (!AfxRegisterClass(&wndcls))
		{
			TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
			// The call DoModal in DLL release mode. Register class fail.
#ifdef _AFXDLL	// Debug mode
			return FALSE;
#endif
		}

		BOOL bRet = FALSE;
		bRet = CreateEx(0, szDlgClass, _T("UIDIALOG"), WS_POPUP | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | dwStyle, CRect(0, 0, 390, 390), CWnd::FromHandle(hWndParent), NULL);
		if (bRet)
		{
			// Simulate WM_INITDIALOG. The WM_INITDIALOG message be send after WM_CREATE, WM_SIZE and WM_MOVE.
			// 通过在WM_WINDOWPOSCHANGED消息处理中加断点，查看调用栈发现：
			// SendMessage内部在发送WM_INITDIALOG消息时，会调用	user32.dll!_InternalCallWinProc@20()  + 0x23 bytes
			// 进而会调用到CWnd::CenterWindow把窗口居中。
			SendMessage(WM_INITDIALOG, NULL, NULL);
			ShowWindow(SW_SHOW);
			UpdateWindow();
			//			if (m_nFlags & WF_CONTINUEMODAL)
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				if (GetStyle() & DS_NOIDLEMSG)
				{
					dwFlags |= MLF_NOIDLEMSG;
				}

				//				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
				// when close the dialog by task bar pop-up menu or ALT+F4, the return value is -1;

				// In win7 with classical theme, the Layered modal dialog can't display.
				LONG lStyleEx = GetWindowLong(m_hWnd, GWL_EXSTYLE);
				if (lStyleEx & WS_EX_LAYERED)
				{
					Invalidate();
					UpdateWindow();
				}
				int nRet = RunModalLoop(dwFlags);
			}

			// hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
			{
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
					SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
			}
		}
	}
	CATCH_ALL(e)
	{
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL

	if (bEnableParent)
	{
		::EnableWindow(hWndParent, TRUE);
	}
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	{
		::SetActiveWindow(hWndParent);
	}

	// destroy modal window
	DestroyWindow();
	PostModal();

	// unlock/free resources as necessary

	return m_nModalResult;
}

HWND CUIWnd::PreModal()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	// cannot call DoModal on a dialog already constructed as modeless
	ASSERT(m_hWnd == NULL);

	// allow OLE servers to disable themselves
	CWinApp *pApp = AfxGetApp();
	if (pApp != NULL)
	{
		pApp->EnableModeless(FALSE);
	}

	// find parent HWND
	HWND hWnd = CWnd::GetSafeOwner_(m_pParentWnd->GetSafeHwnd(), &pMember->m_hWndTop);

	// hook for creation of dialog
	AfxHookWindowCreate(this);

	// return window to use as parent for dialog
	return hWnd;
}

void CUIWnd::PostModal()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	AfxUnhookWindowCreate();   // just in case
	Detach();               // just in case

	// re-enable windows
	if (::IsWindow(pMember->m_hWndTop))
	{
		::EnableWindow(pMember->m_hWndTop, TRUE);
	}
	pMember->m_hWndTop = NULL;
	CWinApp *pApp = AfxGetApp();
	if (pApp != NULL)
	{
		pApp->EnableModeless(TRUE);
	}
}

void CUIWnd::EndDialog(int nResult)
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));

	if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL))
	{
		EndModalLoop(nResult);
	}

	::EndDialog(m_hWnd, nResult);
}

BOOL CUIWnd::OnInitDialog()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (!pMember->m_bCreateAsDialog)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	// The following code copy from BOOL CDialog::OnInitDialog()
	// transfer data into the dialog from member variables
	if (!UpdateData(FALSE))
	{
		TRACE0("[LibUIDK]===Warning: UpdateData failed during dialog init.\n");
		EndDialog(-1);
		return FALSE;
	}

	// enable/disable help button automatically
	CWnd *pHelpButton = GetDlgItem(ID_HELP);
	if (pHelpButton != NULL)
	{
		pHelpButton->ShowWindow(AfxHelpEnabled() ? SW_SHOW : SW_HIDE);
	}

	return TRUE;
}

void CUIWnd::OnOK()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (!UpdateData(TRUE))
	{
		TRACE0("[LibUIDK]===UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	EndDialog(IDOK);
}

void CUIWnd::OnCancel()
{
	UIWNDMEMBER *pMember = (UIWNDMEMBER *)m_pMember;

	if (!pMember->m_bCreateAsDialog)
	{
		ASSERT(FALSE);
		return;
	}

	EndDialog(IDCANCEL);
}
