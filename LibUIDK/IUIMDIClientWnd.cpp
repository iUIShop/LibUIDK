// IUIMDIClientWnd.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIUIMDIClientWnd

CIUIMDIClientWnd::CIUIMDIClientWnd()
{
	m_nMDITabID = 10000;
	m_nNextChildFrameID = 0x0000FF00;
	m_nSwitchDocumentButtonID = -1;
	m_nCloseDocumentButtonID = -1;
	m_bCloseBtnInTab = FALSE;
}

CIUIMDIClientWnd::~CIUIMDIClientWnd()
{
}


BEGIN_MESSAGE_MAP(CIUIMDIClientWnd, CUIWnd)
	//{{AFX_MSG_MAP(CIUIMDIClientWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
	ON_MESSAGE(WM_MDICREATE, OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY, OnMDIDestroy)
	ON_MESSAGE(WM_MDIGETACTIVE, OnMDIGetActive)
	ON_MESSAGE(WM_MDISETMENU, OnMDISetMenu)
	ON_MESSAGE(WM_PARENTEVENT, OnParentEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// virtual

BOOL CIUIMDIClientWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	// If the control is a tab in CTaskWndMgr, activate the child frame
	UINT nControlID = LOWORD(wParam);
	if (0 == m_TaskChildFrameMgr.SwitchToTaskWindow(nControlID))
	{
		CTaskWndInfo *pTaskInfo = m_TaskChildFrameMgr.FindTaskWindow(nControlID);
		if (pTaskInfo != NULL)
		{
			CIUIMDIChildWnd *pChildFrame = (CIUIMDIChildWnd *)pTaskInfo->m_pTaskWnd;
			if (pChildFrame->IsKindOf(RUNTIME_CLASS(CIUIMDIChildWnd)))
			{
				CIUIMDIFrameWnd *pFrm = (CIUIMDIFrameWnd *)AfxGetMainWnd();
				pFrm->MDIActivate(pChildFrame);
			}
		}
	}

	// If the control is "switch document button", pup-up a child frame list menu.
	if (nControlID == m_nSwitchDocumentButtonID)
	{
		CSkinButton *pWnd = (CSkinButton *)GetDlgItem(nControlID);
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		CSkinMenu menu;
		CWnd *pFrm = AfxGetMainWnd();
		if (pFrm != NULL)
		{
			CMenuBar *pMenuBar = NULL;
			pFrm->SendMessage(WM_GETMENUBAR, 0, LPARAM(&pMenuBar));
			if (pMenuBar != NULL)
			{
				CTRLPROPERTIES *pMenuStyle = pMenuBar->GetBindMenuStyle();
				menu.BindStyle(pMenuStyle);
			}
		}
		menu.CreatePopupMenu();

		// Init the menu item by document list
		CWnd *pTopWnd = GetTopWindow();
		int nMenuIndex = 0;
		while (pTopWnd != NULL)
		{
			if ((BOOL)::SendMessage(pTopWnd->m_hWnd, WM_ISIUIFRAMEWND, 0, 0))
			{
				CIUIMDIChildWnd *pChildFrame = (CIUIMDIChildWnd *)pTopWnd;
				if (pChildFrame->IsKindOf(RUNTIME_CLASS(CIUIMDIChildWnd)))
				{
					CString strItem;
					pChildFrame->GetWindowText(strItem);
					CSkinMenuItem *pMenuItem = new CSkinMenuItem;
					pMenuItem->m_dwID = pChildFrame->GetDlgCtrlID();
					pMenuItem->m_strText = strItem;
					menu.InsertSkinMenu(0, MF_BYPOSITION, pMenuItem);
				}
			}
			pTopWnd = pTopWnd->GetNextWindow();
		}

		menu.TrackPopupMenu(TPM_LEFTALIGN, rcWnd.left, rcWnd.bottom, this);
		pWnd->SetCheck(BST_UNCHECKED);
	}

	// If the control is "close document button", close the active document.
	if (nControlID == m_nCloseDocumentButtonID)
	{
		HWND hWnd = (HWND)SendMessage(WM_MDIGETACTIVE);
		if (hWnd != NULL)
		{
			CIUIMDIChildWnd *pActiveChild = (CIUIMDIChildWnd *)CWnd::FromHandle(hWnd);
			ASSERT(pActiveChild != NULL);
			CIUIView *pActiveView = pActiveChild->GetActiveView();
			CIUIDocument *pDoc = pActiveView->GetDocument();
			ASSERT(pDoc != NULL);

			pDoc->OnFileClose();
			return CUIWnd::OnCommand(wParam, lParam);
		}
	}

	// If the control is menu item of menu "switch document". active the document
	int nFlag = HIWORD(wParam);
	if (nFlag == 0 && lParam == 0)	// menu item
	{
		CIUIMDIChildWnd *pChildFrame = (CIUIMDIChildWnd *)GetDlgItem(nControlID);
		if (pChildFrame->IsKindOf(RUNTIME_CLASS(CIUIMDIChildWnd)))
		{
			CIUIMDIFrameWnd *pFrm = (CIUIMDIFrameWnd *)AfxGetMainWnd();
			pFrm->MDIActivate(pChildFrame);
		}
	}

	return CUIWnd::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// public

CIUIMDIChildWnd *CIUIMDIClientWnd::GetActiveChildFrame()
{
	HWND hWnd = (HWND)SendMessage(WM_MDIGETACTIVE);
	if (hWnd == NULL)
	{
		return NULL;
	}

	CIUIMDIChildWnd *pChildFrame = (CIUIMDIChildWnd *)CWnd::FromHandle(hWnd);
	ASSERT(pChildFrame->IsKindOf(RUNTIME_CLASS(CIUIMDIChildWnd)));
	return pChildFrame;
}

int CIUIMDIClientWnd::SetCloseButtonInTab(BOOL bCloseBtnInTab)
{
	m_bCloseBtnInTab = bCloseBtnInTab;

	return 0;
}

BOOL CIUIMDIClientWnd::IsCloseButtonInTab()
{
	return m_bCloseBtnInTab;
}

int CIUIMDIClientWnd::UpdateMDIClientTitleBar()
{
	CIUIMDIChildWnd *pChildFrame = GetActiveChildFrame();

	// Show or hide the close and switch document button
	CWnd *pBtnSwitch = GetDlgItem(m_nSwitchDocumentButtonID);
	if (pBtnSwitch != NULL)
	{
		if (pChildFrame == NULL)
		{
			pBtnSwitch->ShowWindow(SW_HIDE);
		}
		else
		{
			pBtnSwitch->ShowWindow(SW_SHOW);
		}
	}
	CWnd *pBtnClose = GetDlgItem(m_nCloseDocumentButtonID);
	if (pBtnClose != NULL)
	{
		if (pChildFrame == NULL)
		{
			pBtnClose->ShowWindow(SW_HIDE);
		}
		else
		{
			pBtnClose->ShowWindow(SW_SHOW);
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CIUIMDIClientWnd message handlers

int CIUIMDIClientWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// Task page
	CRect rcChildFrame;
	GetClientAreaRect(rcChildFrame);
	((CIUIMDIFrameWnd *)AfxGetMainWnd())->GetMDIViewRect(rcChildFrame);
	m_TaskChildFrameMgr.Create(NULL, WS_VISIBLE | WS_CHILD, rcChildFrame, this, 1, NULL);

	// Tab area
	CRect rcTabs;
	((CIUIMDIFrameWnd *)AfxGetMainWnd())->GetMDITabsRect(rcTabs);
	m_stackPanel.Create(NULL, 0, rcTabs, this, 1, NULL);

	// Document list button and close document button
	((CIUIMDIFrameWnd *)AfxGetMainWnd())->GetSwitchDocumentButtonID(&m_nSwitchDocumentButtonID);
	((CIUIMDIFrameWnd *)AfxGetMainWnd())->GetCloseDocumentButtonID(&m_nCloseDocumentButtonID);

	// Show or hide the close and switch document button
	UpdateMDIClientTitleBar();

	return 0;
}

void CIUIMDIClientWnd::OnSize(UINT nType, int cx, int cy)
{
	CUIWnd::OnSize(nType, cx, cy);

	if (m_TaskChildFrameMgr.IsCreated())
	{
		CRect rcChild;
		m_TaskChildFrameMgr.GetOrigWindowRect(rcChild);

		CRect rcNew;
		RelayoutElement(GetUIWndProperties(), rcChild, CPoint(2, 2), rcNew,
			cx, cy, cx, cy, CLH_FILL, CLV_FILL, CRect(0, 0, 0, 0),
			IsKeepAspectRatio(), GetMaxHorRatio(), GetMaxVerRatio());

		m_TaskChildFrameMgr.MoveWindow(rcNew);
	}

	if (m_stackPanel.IsCreated())
	{
		CRect rcChild;
		m_stackPanel.GetOrigWindowRect(rcChild);

		CRect rcNew;
		RelayoutElement(GetUIWndProperties(), rcChild, CPoint(2, 2), rcNew,
			cx, cy, cx, cy, CLH_FILL, CLV_TOP, CRect(0, 0, 0, 0),
			IsKeepAspectRatio(), GetMaxHorRatio(), GetMaxVerRatio());

		m_stackPanel.MoveWindow(rcNew);
	}
}

void CIUIMDIClientWnd::OnMDIActivate(BOOL bActivate, CWnd *pActivateWnd, CWnd *pDeactivateWnd)
{
	CUIWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	m_TaskChildFrameMgr.SwitchToTaskWindow(pActivateWnd);

	// Show the "close document button"
	UpdateMDIClientTitleBar();

	// Ensure the active tab is visible
	UINT uID = m_TaskChildFrameMgr.GetCurrentTaskWindow();
	CWnd *pParent = m_stackPanel.GetParent();
	ASSERT(pParent != NULL);
	CWnd *pRadTab = pParent->GetDlgItem(uID);
	m_stackPanel.EnsureVisible(pRadTab, FALSE);
}

LRESULT CIUIMDIClientWnd::OnMDICreate(WPARAM wParam, LPARAM lParam)
{
	MDICREATESTRUCT *pmcs = (MDICREATESTRUCT *)lParam;

	_AFX_THREAD_STATE *pThreadState = AfxGetThreadState();
	CIUIMDIChildWnd *pCreateWnd = (CIUIMDIChildWnd *)pThreadState->m_pWndInit;
	ASSERT(pCreateWnd != NULL);
	ASSERT(pCreateWnd->GetSafeHwnd() == NULL);
	BOOL bRet = pCreateWnd->CUIWnd::Create(pmcs->szClass, pmcs->szTitle, pmcs->style, CRect(pmcs->x, pmcs->y, pmcs->x + pmcs->cx, pmcs->y + pmcs->cy), this, m_nNextChildFrameID++, (CCreateContext *)pmcs->lParam);

	// Create a tab
	CSkinButton *pBtn = new CSkinButton;
	bRet = pBtn->Create(NULL, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 101, 22), this, m_nMDITabID++);
	CString strImageName[8];
	((CIUIMDIFrameWnd *)AfxGetMainWnd())->GetMDITabImages(&strImageName[0], &strImageName[1],
		&strImageName[2], &strImageName[3],
		&strImageName[4], &strImageName[5],
		&strImageName[6], &strImageName[7]);

	pBtn->CButtonBase::SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
		strImageName[0], strImageName[1], strImageName[2], strImageName[3]);
	pBtn->CButtonBase::SetCheckedBitmap(CONTROL_STATE_CHECKED_ALL,
		strImageName[4], strImageName[5], strImageName[6], strImageName[7]);

	if (m_bCloseBtnInTab)
	{
		pBtn->SetActiveArea(CRect(80, 0, 100, 22));
	}
	if (!strImageName[0].IsEmpty())
	{
		HIUIIMAGE h = CreateIUIImage(strImageName[0]);
		if (h != NULL)
		{
			pBtn->MoveWindow(CRect(0, 0, h->GetWidth(), h->GetHeight()));
			ReleaseIUIImage(h);
		}
	}
	m_stackPanel.AddElement(pBtn);

	int nCount = m_stackPanel.GetElementCount();
	if (nCount == 1)
	{
		LONG lStyle = GetWindowLong(pBtn->GetSafeHwnd(), GWL_STYLE);
		SetWindowLong(pBtn->GetSafeHwnd(), GWL_STYLE, lStyle | WS_GROUP);
	}
	m_TaskChildFrameMgr.AddTaskWindow(m_nMDITabID - 1, TRUE, pCreateWnd, 0);

	// Active the new document
	CIUIMDIFrameWnd *pFrm = (CIUIMDIFrameWnd *)AfxGetMainWnd();
	pFrm->MDIActivate(pCreateWnd);

	return (LRESULT)pCreateWnd->GetSafeHwnd();
}

LRESULT CIUIMDIClientWnd::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
{
	CIUIMDIChildWnd *pChildFrameDestroy = (CIUIMDIChildWnd *)CWnd::FromHandle((HWND)wParam);

	// Deactivate the destroy window
	::SendMessage(pChildFrameDestroy->GetSafeHwnd(), WM_MDIACTIVATE, (WPARAM)pChildFrameDestroy->GetSafeHwnd()/*Deactivate*/, 0/*Activate*/);

	// Destroy child frame
	pChildFrameDestroy->CIUIFrameWnd::DestroyWindow();

	// 1. Active new child frame
	CIUIFrameWnd *pTopWnd = GetTopIUIFrame();
	if (pTopWnd != NULL)
	{
		if (pTopWnd->IsKindOf(RUNTIME_CLASS(CIUIMDIChildWnd)))
		{
			::SendMessage(pTopWnd->GetSafeHwnd(), WM_MDIACTIVATE, 0/*Deactivate*/, (LPARAM)pTopWnd->GetSafeHwnd()/*Activate*/);
		}
	}

	// 2. Remove the tab radio button bind to old document from CTaskWndMgr
	UINT nID = m_TaskChildFrameMgr.GetTaskRadioButtonID(pChildFrameDestroy);
	m_TaskChildFrameMgr.RemoveTaskWindow(nID, FALSE);


	// 3. switch to the activate child frame
	if (pTopWnd != NULL)
	{
		if (pTopWnd->IsKindOf(RUNTIME_CLASS(CIUIMDIChildWnd)))
		{
			CIUIMDIFrameWnd *pFrm = (CIUIMDIFrameWnd *)AfxGetMainWnd();
			pFrm->MDIActivate(pTopWnd);
		}
	}

	// 4. Free radio button
	CWnd *pRad = GetDlgItem(nID);
	ASSERT(IsWindow(pRad->GetSafeHwnd()));
	pRad->DestroyWindow();
	delete pRad;

	// 5. Update mdi client title bar
	UpdateMDIClientTitleBar();

	// The order of above can't be modified.

	return 0;
}

LRESULT CIUIMDIClientWnd::OnMDIGetActive(WPARAM wParam, LPARAM lParam)
{
	UINT uRadID = m_TaskChildFrameMgr.GetCurrentTaskWindow();
	CTaskWndInfo *pChildFrameInfo = m_TaskChildFrameMgr.FindTaskWindow(uRadID);
	if (pChildFrameInfo == NULL)
	{
		return NULL;
	}

	return (LRESULT)pChildFrameInfo->m_pTaskWnd->GetSafeHwnd();
}

LRESULT CIUIMDIClientWnd::OnMDISetMenu(WPARAM wParam, LPARAM lParam)
{
	HMENU hNewFrameMenu = (HMENU)wParam;
	HMENU hNewWindowMenu = (HMENU)lParam;

	CIUIMDIFrameWnd *pFrame = (CIUIMDIFrameWnd *)AfxGetMainWnd();
	CMenuBar *pMenuBar = pFrame->GetDefaultMenuBar();
	if (pMenuBar != NULL)
	{
		pMenuBar->SetHMENU(hNewFrameMenu);
	}

	return (LRESULT)NULL;
}

LRESULT CIUIMDIClientWnd::OnParentEvent(WPARAM wParam, LPARAM lParam)
{
	UINT message = LOWORD(wParam);
	UINT uID = HIWORD(wParam);

	if (message == WM_LBUTTONUP && m_bCloseBtnInTab)
	{
		CTaskWndInfo *pRet = m_TaskChildFrameMgr.FindTaskWindow(uID);
		if (pRet != NULL)
		{
			CSkinButton *pBtn = (CSkinButton *)GetDlgItem(uID);
			BOOL bActiveArea = pBtn->IsHasActiveArea();
			if (bActiveArea && (pBtn->GetCheck() == BST_CHECKED))
			{
				CRect rcActive;
				pBtn->GetActiveArea(rcActive);
				CPoint pt(lParam);
				if (rcActive.PtInRect(pt))
				{
					CIUIMDIChildWnd *pActiveChild = (CIUIMDIChildWnd *)pRet->m_pTaskWnd;
					ASSERT(pActiveChild != NULL);
					CIUIView *pActiveView = pActiveChild->GetActiveView();
					CIUIDocument *pDoc = pActiveView->GetDocument();
					ASSERT(pDoc != NULL);

					pDoc->OnFileClose();

					return TRUE;
				}
			}
		}
	}

	return 0;
}
