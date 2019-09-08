// IUIMDIChildWnd.cpp.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#if(_MSC_VER <= 1200) // VC6.0
#include <../src/afximpl.h>
#else
# if (_MSC_VER <= 1310) // VC7.1
#include <../src/mfc/afximpl.h>
#endif
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIUIMDIChildWnd

IMPLEMENT_DYNCREATE(CIUIMDIChildWnd, CIUIFrameWnd)

CIUIMDIChildWnd::CIUIMDIChildWnd()
{
	m_pDocument = NULL;
	m_hMenuShared = NULL;
	m_bPseudoInactive = FALSE;
	m_bVisible = TRUE;
}

CIUIMDIChildWnd::~CIUIMDIChildWnd()
{
	m_hMenuShared = NULL;	// Don't call DestroyMenu to destroy this menu, sine it's be destroyed by CMultiDocTemplate::~CMultiDocTemplate
}

BOOL CIUIMDIChildWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, CCreateContext *pContext)
{
	// pParentWnd is NULL

	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	//	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);
	ASSERT(m_hMenuShared == NULL);      // only do once

	//	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

	// parent must be MDI Frame (or NULL for default)
	ASSERT(pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

	// if available - get MDI child menus from doc template
	ASSERT(m_hMenuShared == NULL);      // only do once
	CMultiDocTemplate *pTemplate;
	if (pContext != NULL &&
		(pTemplate = (CMultiDocTemplate *)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT_KINDOF(CMultiDocTemplate, pTemplate);
		// get shared menu from doc template
		m_hMenuShared = pTemplate->m_hMenuShared;
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		TRACE0("[LibUIDK]===Warning: no shared menu/acceltable for MDI Child window.\n");
		// if this happens, programmer must load these manually
	}

	CString strFullString, strTitle;
	if (strFullString.LoadString(nIDResource))
	{
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string
	}

	ASSERT(m_hWnd == NULL);
	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource),
			strTitle, dwDefaultStyle, rectDefault,
			(CIUIMDIFrameWnd *)pParentWnd, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}

BOOL CIUIMDIChildWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CIUIMDIFrameWnd *pParentWnd, CCreateContext *pContext)
{
	if (pParentWnd == NULL)
	{
		CWnd *pMainWnd = AfxGetThread()->m_pMainWnd;
		ASSERT(pMainWnd != NULL);
		pParentWnd = (CIUIMDIFrameWnd *)pMainWnd;
	}
	ASSERT(::IsWindow(pParentWnd->m_wndMDIClient.GetSafeHwnd()));

	// insure correct window positioning
	pParentWnd->RecalcLayout();

	// first copy into a CREATESTRUCT for PreCreate
	CREATESTRUCT cs;
	cs.dwExStyle = 0L;
	cs.lpszClass = lpszClassName;
	cs.lpszName = lpszWindowName;
	cs.style = dwStyle;
	cs.x = rect.left;
	cs.y = rect.top;
	cs.cx = rect.right - rect.left;
	cs.cy = rect.bottom - rect.top;
	cs.hwndParent = pParentWnd->m_hWnd;
	cs.hMenu = NULL;
	cs.hInstance = AfxGetInstanceHandle();
	cs.lpCreateParams = (LPVOID)pContext;

	if (!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return FALSE;
	}
	// extended style must be zero for MDI Children (except under Win4)
#if(_MSC_VER <= 1200) // VC6.0
	ASSERT(afxData.bWin4 || cs.dwExStyle == 0);
#else
	ASSERT(cs.dwExStyle == 0);
#endif

	ASSERT(cs.hwndParent == pParentWnd->m_hWnd);    // must not change

	// now copy into a MDICREATESTRUCT for real create
	MDICREATESTRUCT mcs;
	mcs.szClass = cs.lpszClass;
	mcs.szTitle = cs.lpszName;
	mcs.hOwner = cs.hInstance;
	mcs.x = cs.x;
	mcs.y = cs.y;
	mcs.cx = cs.cx;
	mcs.cy = cs.cy;
	mcs.style = cs.style & ~(WS_MAXIMIZE | WS_VISIBLE);
	mcs.lParam = (LPARAM)cs.lpCreateParams;

	// create the window through the MDICLIENT window
	AfxHookWindowCreate(this);
	HWND hWnd = (HWND)::SendMessage(pParentWnd->m_wndMDIClient.GetSafeHwnd(),
			WM_MDICREATE, 0, (LPARAM)&mcs);
	if (!AfxUnhookWindowCreate())
	{
		PostNcDestroy();    // cleanup if MDICREATE fails too soon
	}

	if (hWnd == NULL)
	{
		return FALSE;
	}

	// special handling of visibility (always created invisible)
	if (m_bVisible)
	{
		// place the window on top in z-order before showing it
		::BringWindowToTop(hWnd);

		// Show window by CTaskWndMgr

		// make sure it is active (visibility == activation)
		pParentWnd->MDIActivate(this);

		// refresh MDI Window menu
		::SendMessage(pParentWnd->m_wndMDIClient.GetSafeHwnd(), WM_MDIREFRESHMENU, 0, 0);
	}

	ASSERT(hWnd == m_hWnd);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CIUIMDIChildWnd, CIUIFrameWnd)
	//{{AFX_MSG_MAP(CIUIMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// virtual

BOOL CIUIMDIChildWnd::PreCreateWindow(CREATESTRUCT &cs)
{
	ASSERT(cs.style & WS_CHILD);
	// MFC V2 requires that MDI Children are created with proper styles,
	//  usually: WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW.
	// See Technical note TN019 for more details on MFC V1->V2 migration.
	BOOL bRet = CIUIFrameWnd::PreCreateWindow(cs);

	// Create a Invisible ui window, show it in WM_CREATE if m_uiwndProperties.m_bVisible is true
	m_bVisible = cs.style & WS_VISIBLE;
	if (m_bVisible)
	{
		cs.style &= ~WS_VISIBLE;
		UIWNDPROPERTIES *pUIProp = (UIWNDPROPERTIES *)GetUIWndProperties();
		pUIProp->m_bVisible = false;
	}

	return bRet;
}

BOOL CIUIMDIChildWnd::DestroyWindow()
{
	if (m_hWnd == NULL)
	{
		return FALSE;
	}

	// avoid changing the caption during the destroy message(s)
	CIUIMDIFrameWnd *pFrameWnd = GetMDIFrame();
	HWND hWndFrame = pFrameWnd->m_hWnd;
	ASSERT(::IsWindow(hWndFrame));
	DWORD dwStyle = SetWindowLong(hWndFrame, GWL_STYLE,
			GetWindowLong(hWndFrame, GWL_STYLE) & ~FWS_ADDTOTITLE);

	MDIDestroy();

	if (::IsWindow(hWndFrame))
	{
		ASSERT(hWndFrame == pFrameWnd->m_hWnd);
		SetWindowLong(hWndFrame, GWL_STYLE, dwStyle);
		pFrameWnd->OnUpdateFrameTitle(TRUE);
	}

	return TRUE;
}

void CIUIMDIChildWnd::PostNcDestroy()
{
	delete this;
}

void CIUIMDIChildWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
	{
		return;    // leave child window alone!
	}

	CDocument *pDocument = GetActiveDocument();
	if (bAddToTitle)
	{
		TCHAR szText[256 + _MAX_PATH];
		if (pDocument == NULL)
		{
			lstrcpy(szText, m_strTitle);
		}
		else
		{
			lstrcpy(szText, pDocument->GetTitle());
		}
		if (m_nWindow > 0)
		{
			wsprintf(szText + lstrlen(szText), _T(":%d"), m_nWindow);
		}

		// set title if changed, but don't remove completely
		AfxSetWindowText2(szText);
	}
}

LRESULT CIUIMDIChildWnd::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefMDIChildProc(m_hWnd, nMsg, wParam, lParam);
}

void CIUIMDIChildWnd::OnUpdateFrameMenu(BOOL bActivate, CWnd *pActivateWnd, HMENU hMenuAlt)
{
	CIUIMDIFrameWnd *pFrame = GetMDIFrame();

	if (hMenuAlt == NULL && bActivate)
	{
		// attempt to get default menu from document
		CDocument *pDoc = GetActiveDocument();
		if (pDoc != NULL)
		{
			hMenuAlt = pDoc->GetDefaultMenu();
		}
	}

	// use default menu stored in frame if none from document
	if (hMenuAlt == NULL)
	{
		hMenuAlt = m_hMenuShared;
	}

	if (hMenuAlt != NULL && bActivate)
	{
		ASSERT(pActivateWnd == this);

		// activating child, set parent menu
		::SendMessage(pFrame->m_wndMDIClient.GetSafeHwnd(), WM_MDISETMENU,
			(WPARAM)hMenuAlt, (LPARAM)pFrame->GetWindowMenuPopup(hMenuAlt));
	}
	else if (hMenuAlt != NULL && !bActivate && pActivateWnd == NULL)
	{
		// destroying last child
		HMENU hMenuLast = NULL;
		::SendMessage(pFrame->m_wndMDIClient.GetSafeHwnd(), WM_MDISETMENU,
			(WPARAM)pFrame->m_hMenuDefault, (LPARAM)hMenuLast);
	}
	else
	{
		// refresh MDI Window menu (even if non-shared menu)
		::SendMessage(pFrame->m_wndMDIClient.GetSafeHwnd(), WM_MDIREFRESHMENU, 0, 0);
	}
}

//////////////////////////////////////////////////////////////////////////
// public

CIUIMDIFrameWnd *CIUIMDIChildWnd::GetMDIFrame()
{
	ASSERT_KINDOF(CIUIMDIChildWnd, this);
	ASSERT(m_hWnd != NULL);
	HWND hWndMDIClient = ::GetParent(m_hWnd);
	ASSERT(hWndMDIClient != NULL);

	CIUIMDIFrameWnd *pMDIFrame;
	pMDIFrame = (CIUIMDIFrameWnd *)CWnd::FromHandle(::GetParent(hWndMDIClient));
	ASSERT(pMDIFrame != NULL);
	//	ASSERT_KINDOF(CIUIMDIFrameWnd, pMDIFrame);
	ASSERT(pMDIFrame->m_wndMDIClient.GetSafeHwnd() == hWndMDIClient);
	ASSERT_VALID(pMDIFrame);
	return pMDIFrame;
}

void CIUIMDIChildWnd::MDIDestroy()
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(GetParent()->m_hWnd, WM_MDIDESTROY, (WPARAM)m_hWnd, 0L);
}

void CIUIMDIChildWnd::ActivateFrame(int nCmdShow)
{
	BOOL bVisibleThen = (GetStyle() & WS_VISIBLE) != 0;
	CIUIMDIFrameWnd *pFrameWnd = GetMDIFrame();
	ASSERT_VALID(pFrameWnd);

	// determine default show command
	if (nCmdShow == -1)
	{
		// get maximized state of frame window (previously active child)
		BOOL bMaximized;
		pFrameWnd->MDIGetActive(&bMaximized);

		// convert show command based on current style
		DWORD dwStyle = GetStyle();
		if (bMaximized || (dwStyle & WS_MAXIMIZE))
		{
			nCmdShow = SW_SHOWMAXIMIZED;
		}
		else if (dwStyle & WS_MINIMIZE)
		{
			nCmdShow = SW_SHOWMINIMIZED;
		}
	}

	// finally, show the window
	CIUIFrameWnd::ActivateFrame(nCmdShow);

	// update the Window menu to reflect new child window
	CIUIMDIFrameWnd *pFrame = GetMDIFrame();
	::SendMessage(pFrame->m_wndMDIClient.GetSafeHwnd(), WM_MDIREFRESHMENU, 0, 0);

	// Note: Update the m_bPseudoInactive flag.  This is used to handle the
	//  last MDI child getting hidden.  Windows provides no way to deactivate
	//  an MDI child window.

	BOOL bVisibleNow = (GetStyle() & WS_VISIBLE) != 0;
	if (bVisibleNow == bVisibleThen)
	{
		return;
	}

	if (!bVisibleNow)
	{
		// get current active window according to Windows MDI
		HWND hWnd = (HWND)::SendMessage(pFrameWnd->m_wndMDIClient.GetSafeHwnd(),
				WM_MDIGETACTIVE, 0, 0);
		if (hWnd != m_hWnd)
		{
			// not active any more -- window must have been deactivated
			ASSERT(!m_bPseudoInactive);
			return;
		}

		// check next window
		ASSERT(hWnd != NULL);
		pFrameWnd->MDINext();

		// see if it has been deactivated now...
		hWnd = (HWND)::SendMessage(pFrameWnd->m_wndMDIClient.GetSafeHwnd(),
				WM_MDIGETACTIVE, 0, 0);
		if (hWnd == m_hWnd)
		{
			// still active -- fake deactivate it
			ASSERT(hWnd != NULL);
			OnMDIActivate(FALSE, NULL, this);
			m_bPseudoInactive = TRUE;   // so MDIGetActive returns NULL
		}
	}
	else if (m_bPseudoInactive)
	{
		// if state transitioned from not visible to visible, but
		//  was pseudo deactivated -- send activate notify now
		OnMDIActivate(TRUE, this, NULL);
		ASSERT(!m_bPseudoInactive); // should get set in OnMDIActivate!
	}
}

void CIUIMDIChildWnd::AfxSetWindowText2(LPCTSTR lpszNew)
{
	int nNewLen = lstrlen(lpszNew);
	TCHAR szOld[256];
	// fast check to see if text really changes (reduces flash in controls)
	if (nNewLen > _countof(szOld) ||
		::GetWindowText(m_hWnd, szOld, _countof(szOld)) != nNewLen ||
		lstrcmp(szOld, lpszNew) != 0)
	{
		// change it
		::SetWindowText(m_hWnd, lpszNew);

		// change the title of tab radio button
		CIUIMDIClientWnd *pMDIClient = (CIUIMDIClientWnd *)GetParent();
		ASSERT(pMDIClient != NULL);
		UINT uRadioBtnID = pMDIClient->m_TaskChildFrameMgr.GetTaskRadioButtonID(this);
		CWnd *pRadTab = pMDIClient->GetDlgItem(uRadioBtnID);
		ASSERT(pRadTab != NULL);
		pRadTab->SetWindowText(lpszNew);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIUIMDIChildWnd message handlers

int CIUIMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CIUIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// the following is MFC code
	// 	MDICREATESTRUCT* lpmcs;
	// 	lpmcs = (MDICREATESTRUCT*)lpCreateStruct->lpCreateParams;
	// 	CCreateContext* pContext = (CCreateContext*)lpmcs->lParam;

	CCreateContext *pContext = (CCreateContext *)lpCreateStruct->lpCreateParams;

	return OnCreateHelper(lpCreateStruct, pContext);
}

void CIUIMDIChildWnd::OnSize(UINT nType, int cx, int cy)
{
	CIUIFrameWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CIUIView *pIUIView = GetActiveView();
	if (pIUIView != NULL)
	{
		ASSERT(pIUIView->IsKindOf(RUNTIME_CLASS(CIUIView)));
		CRect rcClientArea;
		GetClientAreaRect(rcClientArea);
		pIUIView->MoveWindow(rcClientArea);
	}
}

void CIUIMDIChildWnd::OnMDIActivate(BOOL bActivate, CWnd *pActivateWnd, CWnd *)
{
	m_bPseudoInactive = FALSE;  // must be happening for real

	// make sure MDI client window has correct client edge
	//	UpdateClientEdge();

	// send deactivate notification to active view
	CIUIView *pActiveView = GetActiveView();
	if (!bActivate && pActiveView != NULL)
	{
		pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);
	}

	// allow hook to short circuit normal activation
	BOOL bHooked = FALSE;
	// #ifndef _AFX_NO_OLE_SUPPORT
	// 	if (m_pNotifyHook != NULL && m_pNotifyHook->OnDocActivate(bActivate))
	// 		bHooked = TRUE;
	// #endif

	// update titles (don't AddToTitle if deactivate last)
	if (!bHooked)
	{
		OnUpdateFrameTitle(bActivate || (pActivateWnd != NULL));
	}

	// re-activate the appropriate view
	if (bActivate)
	{
		if (pActiveView != NULL && GetMDIFrame() == GetActiveWindow())
		{
			pActiveView->OnActivateView(TRUE, pActiveView, pActiveView);
		}
	}

	// update menus
	if (!bHooked)
	{
		OnUpdateFrameMenu(bActivate, pActivateWnd, NULL);
		GetMDIFrame()->DrawMenuBar();
	}
}
