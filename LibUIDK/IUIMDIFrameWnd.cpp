// IUIMDIFrameWnd.cpp : implementation of the CIUIMDIFrameWnd class
//

#include "stdafx.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIUIMDIFrameWnd

BEGIN_MESSAGE_MAP(CIUIMDIFrameWnd, CIUIFrameWnd)
	//{{AFX_MSG_MAP(CIUIMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_MESSAGE(WM_GETDOCVIEWCLIENTRECT, OnGetDocumentViewClientRect)
	ON_MESSAGE(WM_MDIREFRESHMENU, OnMDIRefreshMenu)
	ON_MESSAGE(WM_GETMENUBAR, OnGetMenuBar)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnFileMruFile)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateFileMruFile1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIUIMDIFrameWnd construction/destruction

IMPLEMENT_DYNCREATE(CIUIMDIFrameWnd, CIUIFrameWnd)

CIUIMDIFrameWnd::CIUIMDIFrameWnd()
{
	m_pMenuBarDefault = NULL;
	m_hMenuDefault = NULL;
}

CIUIMDIFrameWnd::~CIUIMDIFrameWnd()
{
}

//////////////////////////////////////////////////////////////////////////
// virtual

void CIUIMDIFrameWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
	{
		return;    // leave it alone!
	}

#ifndef _AFX_NO_OLE_SUPPORT
	// allow hook to set the title (used for OLE support)
	//	if (m_pNotifyHook != NULL && m_pNotifyHook->OnUpdateFrameTitle())
	//		return;
#endif

	CIUIMDIChildWnd *pActiveChild = NULL;
	CIUIDocument *pDocument = GetActiveDocument();
	if (bAddToTitle &&
		(pActiveChild = MDIGetActive()) != NULL &&
		(pActiveChild->GetStyle() & WS_MAXIMIZE) == 0 &&
		(pDocument != NULL ||
			(pDocument = pActiveChild->GetActiveDocument()) != NULL))
	{
		UpdateFrameTitleForDocument(pDocument->GetTitle());
	}
	else
	{
		LPCTSTR lpstrTitle = NULL;
		CString strTitle;

		if (pActiveChild != NULL)
		{
			strTitle = pActiveChild->GetTitle();
			if (!strTitle.IsEmpty())
			{
				lpstrTitle = strTitle;
			}
		}
		UpdateFrameTitleForDocument(lpstrTitle);
	}
}

void CIUIMDIFrameWnd::UpdateFrameTitleForDocument(LPCTSTR lpszDocName)
{
	// copy first part of title loaded at time of frame creation
	TCHAR szText[256 + _MAX_PATH];

	if (GetStyle() & FWS_PREFIXTITLE)
	{
		szText[0] = '\0';   // start with nothing

		// get name of currently active view
		if (lpszDocName != NULL)
		{
			lstrcpy(szText, lpszDocName);
			// add current window # if needed
			if (m_nWindow > 0)
			{
				wsprintf(szText + lstrlen(szText), _T(":%d"), m_nWindow);
			}
			lstrcat(szText, _T(" - "));
		}
		lstrcat(szText, m_strTitle);
	}
	else
	{
		// get name of currently active view
		lstrcpy(szText, m_strTitle);
		if (lpszDocName != NULL)
		{
			lstrcat(szText, _T(" - "));
			lstrcat(szText, lpszDocName);
			// add current window # if needed
			if (m_nWindow > 0)
			{
				wsprintf(szText + lstrlen(szText), _T(":%d"), m_nWindow);
			}
		}
	}

	// set title if changed, but don't remove completely
	// Note: will be excessive for MDI Frame with maximized child
	AfxSetWindowText(m_hWnd, szText);
}

void CIUIMDIFrameWnd::MDINext()
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_wndMDIClient.GetSafeHwnd(), WM_MDINEXT, 0, 0);
}

int CIUIMDIFrameWnd::SetDefaultMenuBar(CMenuBar *pMenuBar)
{
	m_pMenuBarDefault = pMenuBar;

	if (m_pMenuBarDefault->GetSafeHwnd() != NULL)
	{
		m_pMenuBarDefault->SetHMENU(m_hMenuDefault);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

void CIUIMDIFrameWnd::MDIActivate(CWnd *pWndActivate)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_wndMDIClient.GetSafeHwnd(), WM_MDIACTIVATE, 0, (WPARAM)pWndActivate->m_hWnd);
}

CIUIMDIChildWnd *CIUIMDIFrameWnd::MDIGetActive(BOOL *pbMaximized/* = NULL*/) const
{
	// check first for MDI client window not created
	if (m_wndMDIClient.GetSafeHwnd() == NULL)
	{
		if (pbMaximized != NULL)
		{
			*pbMaximized = FALSE;
		}
		return NULL;
	}

	// MDI client has been created, get active MDI child
	HWND hWnd = (HWND)::SendMessage(m_wndMDIClient.GetSafeHwnd(), WM_MDIGETACTIVE, 0,
			(LPARAM)pbMaximized);
	CIUIMDIChildWnd *pWnd = (CIUIMDIChildWnd *)CWnd::FromHandle(hWnd);
	ASSERT(pWnd == NULL || pWnd->IsKindOf(RUNTIME_CLASS(CIUIMDIChildWnd)));

	// check for special pseudo-inactive state
	if (pWnd != NULL && pWnd->m_bPseudoInactive &&
		(pWnd->GetStyle() & WS_VISIBLE) == 0)
	{
		// Window is hidden, active, but m_bPseudoInactive -- return NULL
		pWnd = NULL;
		// Ignore maximized flag if pseudo-inactive and maximized
		if (pbMaximized != NULL)
		{
			*pbMaximized = FALSE;
		}
	}
	return pWnd;
}


/////////////////////////////////////////////////////////////////////////////
// CIUIMDIFrameWnd message handlers

int CIUIMDIFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CIUIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// MDI client
	UINT nID = -1;
	GetMDIClientWindowID(&nID);
	UIWNDPROPERTIES *pWndProperties = CUIMgr::GetUIWNDPROPERTIES(CUIMgr::StringIDFromInt(nID));
	if (!bool(pWndProperties->m_bLoaded))
	{
		CUIMgr::DelayLoadWindow(pWndProperties);
	}
	m_wndMDIClient.SetUIWndProperties(pWndProperties);
	m_wndMDIClient.Create(NULL, _T("MDIClient"), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 2);
	CRect rcMDIClient;
	GetClientAreaRect(rcMDIClient);
	OnGetMDIClientRect(rcMDIClient);
	m_wndMDIClient.MoveWindow(rcMDIClient);
	m_wndMDIClient.ShowWindow(SW_SHOW);

	return 0;
}

void CIUIMDIFrameWnd::OnDestroy() ///
{
	CIUIFrameWnd::OnDestroy();
}

void CIUIMDIFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	CIUIFrameWnd::OnSize(nType, cx, cy);

	if (m_wndMDIClient.GetSafeHwnd() != NULL)
	{
		CRect rcMDIClient;
		GetClientAreaRect(rcMDIClient);
		OnGetMDIClientRect(rcMDIClient);
		m_wndMDIClient.MoveWindow(rcMDIClient);
	}
}

LRESULT CIUIMDIFrameWnd::OnGetDocumentViewClientRect(WPARAM wParam, LPARAM lParam)
{
	LPRECT lpRect = (LPRECT)lParam;

	return (LRESULT)OnGetMDIClientRect(lpRect);
}

LRESULT CIUIMDIFrameWnd::OnMDIRefreshMenu(WPARAM wParam, LPARAM lParam)
{
	CIUIDocument *pDoc = GetActiveDocument();
	CMenuBar *pMenuBar = GetDefaultMenuBar();

	if (pMenuBar == NULL)
	{
		return NULL;
	}

	if (pDoc == NULL && GetDefaultMenuID() != -1)
	{
		pMenuBar->SetMenuID(GetDefaultMenuID());
	}
	else
	{
		pMenuBar->SetHMENU(pDoc->GetDefaultMenu());
	}

	return NULL;
}

LRESULT CIUIMDIFrameWnd::OnGetMenuBar(WPARAM wParam, LPARAM lParam)
{
	CMenuBar **ppMenuBar = (CMenuBar **)lParam;
	if (ppMenuBar == NULL)
	{
		return -1;
	}

	*ppMenuBar = GetDefaultMenuBar();

	return 0;
}

void CIUIMDIFrameWnd::OnFileNew()
{
	CIUIWinApp *pApp = (CIUIWinApp *)AfxGetApp();
	pApp->OnFileNew();
}

void CIUIMDIFrameWnd::OnFileOpen()
{
	CIUIWinApp *pApp = (CIUIWinApp *)AfxGetApp();
	pApp->OnFileOpen();
}

void CIUIMDIFrameWnd::OnFileClose()
{
	CIUIMDIChildWnd *pActiveChild = MDIGetActive();
	ASSERT(pActiveChild != NULL);
	CIUIView *pActiveView = pActiveChild->GetActiveView();
	CIUIDocument *pDoc = pActiveView->GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->OnFileClose();
}

void CIUIMDIFrameWnd::OnFileSave()
{
	CIUIMDIChildWnd *pActiveChild = MDIGetActive();
	ASSERT(pActiveChild != NULL);
	CIUIView *pActiveView = pActiveChild->GetActiveView();
	CIUIDocument *pDoc = pActiveView->GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->OnFileSave();
}

void CIUIMDIFrameWnd::OnFileSaveAs()
{
	CIUIMDIChildWnd *pActiveChild = MDIGetActive();
	ASSERT(pActiveChild != NULL);
	CIUIView *pActiveView = pActiveChild->GetActiveView();
	CIUIDocument *pDoc = pActiveView->GetDocument();
	ASSERT(pDoc != NULL);

	pDoc->OnFileSaveAs();
}

void CIUIMDIFrameWnd::OnAppExit()
{
	SendMessage(WM_CLOSE);
}

#if(_MSC_VER <= 1200) // VC6.0
void CIUIMDIFrameWnd::OnFileMruFile(UINT uID)
#else
BOOL CIUIMDIFrameWnd::OnFileMruFile(UINT uID)
#endif
{
	CIUIWinApp *pApp = (CIUIWinApp *)AfxGetApp();
	pApp->OnOpenRecentFile(uID);
#if(_MSC_VER > 1200) // VC6.0
	return TRUE;
#endif
}

void CIUIMDIFrameWnd::OnUpdateFileMruFile1(CCmdUI *pCmdUI)
{
	CIUIWinApp *pApp = (CIUIWinApp *)AfxGetApp();
	pApp->OnUpdateRecentFileMenu(pCmdUI);
}
