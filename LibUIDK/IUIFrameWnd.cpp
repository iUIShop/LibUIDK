// IUIFrameWnd.cpp.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#if(_MSC_VER <= 1200) // VC6.0
#include <../src/afximpl.h>
#else
#include <afxtempl.h>
#include <afxdlgs.h>
#include <../src/mfc/afximpl.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIUIFrameWnd

IMPLEMENT_DYNCREATE(CIUIFrameWnd, CUIWnd)


const AFX_DATADEF CRect CIUIFrameWnd::rectDefault(CW_USEDEFAULT, CW_USEDEFAULT, 0 /* 2*CW_USEDEFAULT */, 0 /* 2*CW_USEDEFAULT */);


CIUIFrameWnd::CIUIFrameWnd()
{
	m_pViewActive = NULL;
	m_hAccelTable = NULL;
	AutoMenuEnable(TRUE);       // auto enable on by default
}

CIUIFrameWnd::~CIUIFrameWnd()
{
	if (m_hMenuDefault != NULL)
	{
		::DestroyMenu(m_hMenuDefault);
		m_hMenuDefault = NULL;
	}
}

BEGIN_MESSAGE_MAP(CIUIFrameWnd, CUIWnd)
	//{{AFX_MSG_MAP(CIUIFrameWnd)
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// virtual

BOOL CIUIFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, BOOL bLoadBkImageFromFile/*=TRUE*/, BOOL bUseLayeredWindow/*=TRUE*/)
{
	ASSERT(m_hWnd == NULL);

	// Load default menu before the frame be created. the WM_CREATE message will be use the default menu.
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_MENU);
	if ((m_hMenuDefault = ::LoadMenu(hInst, MAKEINTRESOURCE(nIDResource))) == NULL)
	{
		TRACE0("[LibUIDK]===Warning: failed to load menu for CIUIFrameWnd.\n");
	}

	BOOL bRet = CUIWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, bLoadBkImageFromFile, bUseLayeredWindow);
	if (!bRet)
	{
		return FALSE;
	}

	return TRUE;
}

void CIUIFrameWnd::ActivateFrame(int nCmdShow)
// nCmdShow is the normal show mode this frame should be in
{
	// translate default nCmdShow (-1)
	if (nCmdShow == -1)
	{
		if (!IsWindowVisible())
		{
			nCmdShow = SW_SHOWNORMAL;
		}
		else if (IsIconic())
		{
			nCmdShow = SW_RESTORE;
		}
	}

	// bring to top before showing
	BringToTop(nCmdShow);

	if (nCmdShow != -1)
	{
		// Show window by CTaskWndMgr

		// and finally, bring to top after showing
		BringToTop(nCmdShow);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Setting title of frame window - UISG standard

void CIUIFrameWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
	{
		return;    // leave it alone!
	}

#ifndef _AFX_NO_OLE_SUPPORT
	// allow hook to set the title (used for OLE support)
	//	if (m_pNotifyHook != NULL && m_pNotifyHook->OnUpdateFrameTitle())
	return;
#endif

	CDocument *pDocument = GetActiveDocument();
	// 	if (bAddToTitle && pDocument != NULL)
	// 		UpdateFrameTitleForDocument(pDocument->GetTitle());
	// 	else
	// 		UpdateFrameTitleForDocument(NULL);
}

BOOL CIUIFrameWnd::OnCreateClient(LPCREATESTRUCT, CCreateContext *pContext)
{
	// default create client will create a view if asked for it
	if (pContext != NULL && pContext->m_pNewViewClass != NULL)
	{
		if (CreateView(pContext, AFX_IDW_PANE_FIRST) == NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CIUIFrameWnd::RecalcLayout(BOOL bNotify)
{
	// 	if (m_bInRecalcLayout)
	// 		return;
	//
	// 	m_bInRecalcLayout = TRUE;
	// 	// clear idle flags for recalc layout if called elsewhere
	// 	if (m_nIdleFlags & idleNotify)
	// 		bNotify = TRUE;
	// 	m_nIdleFlags &= ~(idleLayout|idleNotify);
	//
	// #ifndef _AFX_NO_OLE_SUPPORT
	// 	// call the layout hook -- OLE support uses this hook
	// 	if (bNotify && m_pNotifyHook != NULL)
	// 		m_pNotifyHook->OnRecalcLayout();
	// #endif
	//
	// 	// reposition all the child windows (regardless of ID)
	// 	if (GetStyle() & FWS_SNAPTOBARS)
	// 	{
	// 		CRect rect(0, 0, 32767, 32767);
	// 		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery,
	// 			&rect, &rect, FALSE);
	// 		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposExtra,
	// 			&m_rectBorder, &rect, TRUE);
	// 		CalcWindowRect(&rect);
	// 		SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
	// 			SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
	// 	}
	// 	else
	// 		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposExtra, &m_rectBorder);
	// 	m_bInRecalcLayout = FALSE;
}

BOOL CIUIFrameWnd::PreCreateWindow(CREATESTRUCT &cs)
{
	if (cs.lpszClass == NULL)
	{
		VERIFY(AfxDeferRegisterClass(AFX_WNDFRAMEORVIEW_REG));
		cs.lpszClass = AFX_WNDFRAMEORVIEW;  // COLOR_WINDOW background
	}

	if ((cs.style & FWS_ADDTOTITLE)
#if(_MSC_VER <= 1200) // VC6.0
		&& afxData.bWin4
#endif
	)
	{
		cs.style |= FWS_PREFIXTITLE;
	}

	// 	if (afxData.bWin4)
	// 		cs.dwExStyle |= WS_EX_CLIENTEDGE;

	return CUIWnd::PreCreateWindow(cs);
}

LRESULT CIUIFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CHILDACTIVATE)
	{
		CIUIView *pIUIView = GetActiveView();
		if (pIUIView != NULL && pIUIView->GetParent() == this)
		{
			::SendMessage(m_hWnd, WM_MDIACTIVATE, 0, (LPARAM)m_hWnd);
		}
	}
	else if (message == WM_ISIUIFRAMEWND)
	{
		return TRUE;
	}

	return CUIWnd::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// Smarts for the "Window" menu

HMENU CIUIFrameWnd::GetWindowMenuPopup(HMENU hMenuBar)
// find which popup is the "Window" menu
{
	if (hMenuBar == NULL)
	{
		return NULL;
	}

	ASSERT(::IsMenu(hMenuBar));

	int iItem = ::GetMenuItemCount(hMenuBar);
	while (iItem--)
	{
		HMENU hMenuPop = ::GetSubMenu(hMenuBar, iItem);
		if (hMenuPop != NULL)
		{
			int iItemMax = ::GetMenuItemCount(hMenuPop);
			for (int iItemPop = 0; iItemPop < iItemMax; iItemPop++)
			{
				UINT nID = GetMenuItemID(hMenuPop, iItemPop);
				if (nID >= AFX_IDM_WINDOW_FIRST && nID <= AFX_IDM_WINDOW_LAST)
				{
					return hMenuPop;
				}
			}
		}
	}

	// no default menu found
	TRACE0("[LibUIDK]===Warning: GetWindowMenuPopup failed!\n");
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// protected

LPCTSTR CIUIFrameWnd::GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource)
{
	ASSERT_VALID_IDR(nIDResource);
	HINSTANCE hInst = AfxFindResourceHandle(
			MAKEINTRESOURCE(nIDResource), RT_GROUP_ICON);
	HICON hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nIDResource));
	if (hIcon != NULL)
	{
		CREATESTRUCT cs;
		memset(&cs, 0, sizeof(CREATESTRUCT));
		cs.style = dwDefaultStyle;
		PreCreateWindow(cs);
		// will fill lpszClassName with default WNDCLASS name
		// ignore instance handle from PreCreateWindow.

		WNDCLASS wndcls;
		if (cs.lpszClass != NULL &&
			GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls) &&
			wndcls.hIcon != hIcon)
		{
			// register a very similar WNDCLASS
			return AfxRegisterWndClass(wndcls.style,
					wndcls.hCursor, wndcls.hbrBackground, hIcon);
		}
	}
	return NULL;        // just use the default
}

int CIUIFrameWnd::OnCreateHelper(LPCREATESTRUCT lpcs, CCreateContext *pContext)
{
	if (CUIWnd::OnCreate(lpcs) == -1)
	{
		return -1;
	}

	// create special children first
	if (!OnCreateClient(lpcs, pContext))
	{
		TRACE0("[LibUIDK]===Failed to create client pane/view for frame.\n");
		return -1;
	}

	// post message for initial message string
	PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

	// make sure the child windows have been properly sized
	RecalcLayout();

	return 0;   // create ok
}

//////////////////////////////////////////////////////////////////////////
// public

CIUIMDIFrameWnd *CIUIFrameWnd::GetMDIFrame()
{
	ASSERT_KINDOF(CIUIFrameWnd, this);
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

void CIUIFrameWnd::MDIDestroy()
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(GetParent()->m_hWnd, WM_MDIDESTROY, (WPARAM)m_hWnd, 0L);
}

CIUIDocument *CIUIFrameWnd::GetActiveDocument()
{
	ASSERT_VALID(this);
	CIUIView *pView = GetActiveView();
	if (pView != NULL)
	{
		return pView->GetDocument();
	}
	return NULL;
}

void CIUIFrameWnd::SetActiveView(CIUIView *pViewNew, BOOL bNotify)
{
#ifdef _DEBUG
	if (pViewNew != NULL)
	{
		ASSERT(IsChild(pViewNew));
		ASSERT_KINDOF(CIUIView, pViewNew);
	}
#endif //_DEBUG

	CIUIView *pViewOld = m_pViewActive;
	if (pViewNew == pViewOld)
	{
		return;    // do not re-activate if SetActiveView called more than once
	}

	m_pViewActive = NULL;   // no active for the following processing

	// deactivate the old one
	if (pViewOld != NULL)
	{
		pViewOld->OnActivateView(FALSE, pViewNew, pViewOld);
	}

	// if the OnActivateView moves the active window,
	//    that will veto this change
	if (m_pViewActive != NULL)
	{
		return;    // already set
	}
	m_pViewActive = pViewNew;

	// activate
	if (pViewNew != NULL && bNotify)
	{
		pViewNew->OnActivateView(TRUE, pViewNew, pViewOld);
	}
}

CIUIView *CIUIFrameWnd::GetActiveView() const
{
	ASSERT(m_pViewActive == NULL ||
		m_pViewActive->IsKindOf(RUNTIME_CLASS(CIUIView)));
	return m_pViewActive;
}

void CIUIFrameWnd::InitialUpdateFrame(CDocument *pDoc, BOOL bMakeVisible)
{
	// if the frame does not have an active view, set to first pane
	CIUIView *pView = NULL;
	if (GetActiveView() == NULL)
	{
		CWnd *pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CIUIView)))
		{
			pView = (CIUIView *)pWnd;
			SetActiveView(pView, FALSE);

			// move the view
			CRect rcClientArea;
			GetClientAreaRect(rcClientArea);
			pView->MoveWindow(rcClientArea);
			pView->ShowWindow(SW_SHOW);
		}
	}

	if (bMakeVisible)
	{
		// send initial update to all views (and other controls) in the frame
		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

		// give view a chance to save the focus (CFormView needs this)
		if (pView != NULL)
		{
			pView->OnActivateFrame(WA_INACTIVE, this);
		}

		// finally, activate the frame
		// (send the default show command unless the main desktop window)
		int nCmdShow = -1;      // default
		CWinApp *pApp = AfxGetApp();
		if (pApp != NULL && pApp->m_pMainWnd == this)
		{
			nCmdShow = pApp->m_nCmdShow; // use the parameter from WinMain
			pApp->m_nCmdShow = -1; // set to default after first time
		}
		ActivateFrame(nCmdShow);
		if (pView != NULL)
		{
			pView->OnActivateView(TRUE, pView, pView);
		}
	}

	// update frame counts and frame title (may already have been visible)
	if (pDoc != NULL)
	{
		pDoc->UpdateFrameCounts();
	}
	OnUpdateFrameTitle(TRUE);
}

void CIUIFrameWnd::BringToTop(int nCmdShow)
{
	// place the window on top except for certain nCmdShow
	if (nCmdShow != SW_HIDE &&
		nCmdShow != SW_MINIMIZE && nCmdShow != SW_SHOWMINNOACTIVE &&
		nCmdShow != SW_SHOWNA && nCmdShow != SW_SHOWNOACTIVATE)
	{
		// if no last active popup, it will return m_hWnd
		HWND hWndLastPop = ::GetLastActivePopup(m_hWnd);
		::BringWindowToTop(hWndLastPop);
	}
}

CWnd *CIUIFrameWnd::CreateView(CCreateContext *pContext, UINT nID)
{
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pContext != NULL);
	ASSERT(pContext->m_pNewViewClass != NULL);

	// Note: can be a CWnd with PostNcDestroy self cleanup
	CWnd *pView = (CWnd *)pContext->m_pNewViewClass->CreateObject();
	if (pView == NULL)
	{
		TRACE1("[LibUIDK]===Warning: Dynamic create of view type %hs failed.\n",
			pContext->m_pNewViewClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CWnd, pView);

	// views are always created with a border!
	if (!pView->Create(NULL, _T("IUIView"), WS_CHILD,
			CRect(0, 0, 0, 0), this, nID, pContext))
	{
		TRACE0("[LibUIDK]===Warning: could not create view for frame.\n");
		return NULL;        // can't continue without a view
	}

	if (
#if(_MSC_VER <= 1200) // VC6.0
		afxData.bWin4 &&
#endif
		(pView->GetExStyle() & WS_EX_CLIENTEDGE))
	{
		// remove the 3d style from the frame, since the view is
		//  providing it.
		// make sure to recalc the non-client area
		ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	}
	return pView;
}

/////////////////////////////////////////////////////////////////////////////
// CIUIFrameWnd message handlers

void CIUIFrameWnd::OnClose()
{
	// Note: only queries the active document
	CIUIDocument *pDocument = GetActiveDocument();
	if (pDocument != NULL && !pDocument->CanCloseIUIFrame(this))
	{
		// document can't close right now -- don't close it
		return;
	}
	CWinApp *pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if (pDocument == NULL && !pApp->SaveAllModified())
		{
			return;    // don't close it
		}

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		// close all documents first
		pApp->CloseAllDocuments(FALSE);

		// don't exit if there are outstanding component objects
		if (!AfxOleCanExitApp())
		{
			// take user out of control of the app
			AfxOleSetUserCtrl(FALSE);

			// don't destroy the main window and close down just yet
			//  (there are outstanding component (OLE) objects)
			return;
		}

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}

	// detect the case that this is the last frame on the document and
	// shut down with OnCloseDocument instead.
	if (pDocument != NULL && pDocument->m_bAutoDelete)
	{
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL)
		{
			CIUIView *pView = pDocument->GetNextIUIView(pos);
			ASSERT_VALID(pView);
			if (pView->GetParentFrame() != this)
			{
				bOtherFrame = TRUE;
				break;
			}
		}
		if (!bOtherFrame)
		{
			pDocument->OnCloseDocument();
			return;
		}

		// allow the document to cleanup before the window is destroyed
		pDocument->PreCloseIUIFrame(this);
	}

	// then destroy the window
	DestroyWindow();
}

void CIUIFrameWnd::OnMDIActivate(BOOL bActivate, CWnd *pActivateWnd, CWnd *)
{
}

void CIUIFrameWnd::OnInitMenuPopup(CMenu *pMenu, UINT nIndex, BOOL bSysMenu)
{
	//	CUIWnd::OnInitMenuPopup(pMenu, nIndex, bSysMenu);

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
	else if ((hParentMenu = m_hMenuDefault) != NULL)
	{
		CWnd *pParent = GetTopLevelParent();
		// child windows don't have menus -- need to go to the top!
		if (pParent != NULL)
		{
			BOOL bIUIFrameWnd = (BOOL)::SendMessage(pParent->m_hWnd, WM_ISIUIFRAMEWND, 0, 0);
			if (bIUIFrameWnd)
			{
				CIUIFrameWnd *pIUIParent = (CIUIFrameWnd *)pParent;
				hParentMenu = pIUIParent->m_hMenuDefault;
				if (hParentMenu != NULL)
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
		if (state.m_nID == (UINT) - 1)
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
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, IsAutoMenuEnable() && state.m_nID < 0xF000);
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
